/****************************************************************************
 * bluetooth/bluetooth_main.c
 *
 *   Copyright 2018 Sony Semiconductor Solutions Corporation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Sony Semiconductor Solutions Corporation nor
 *    the names of its contributors may be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <sdk/config.h>

#include <sys/ioctl.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <assert.h>
#include <termios.h>

extern int board_gpio_config(uint32_t pin, int mode, bool input, bool drive,
        int pull);
extern void board_gpio_write(uint32_t pin, int value);

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifndef CONFIG_EXAMPLES_BLESENSOR_DEVNAME
#  define CONFIG_EXAMPLES_BLESENSOR_DEVNAME "/dev/ttyS2"
#endif


/****************************************************************************
 * Private Data
 ****************************************************************************/

static char *g_data;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void wait_connection(int fd)
{

  int i, ret;
  char c;
  char resp[128];
  int cmp = -1;

  printf("Waiting for connection\n");

  while(cmp !=0){

	  c = 0;

	  for (i = 0; i < 128 && c != '\n';)
		{
		  ret = read(fd, &c, 1);
		  if(ret<0){
		  	  int errcode = errno;
		  	  fprintf(stderr, "%s: read() failed: %s (%d)\n", CONFIG_EXAMPLES_BLESENSOR_DEVNAME, strerror(errcode), errcode);
		  }
		  if(c != 0 && c !=255){
			  resp[i] = c;
			  i++;
		  }
		}
	  resp[i-2] = '\0';

	  cmp = strcmp(resp, "CONNECT");
  }
  printf("Connected\n");


}

static int sensing_main(int fd)
{

	int ret;
	char atd[] = "ATD\r";
	//char adv_data[] = "ATS150=02010605030f180a180508524f484d\r"; //ROHM
	char adv_data[] = "ATS150=02010605030f180a1809084c61706973446576\r\n";


	ret = write(fd, adv_data, sizeof(adv_data));
	if(ret<0){
		int errcode = errno;
		fprintf(stderr, "%s: write() failed: %s (%d)\n", CONFIG_EXAMPLES_BLESENSOR_DEVNAME, strerror(errcode), errcode);
	}

	for (;;){

		ret = write(fd, atd, sizeof(atd));
		if(ret<0){
			int errcode = errno;
			fprintf(stderr, "%s: write() failed: %s (%d)\n", CONFIG_EXAMPLES_BLESENSOR_DEVNAME, strerror(errcode), errcode);
		}

		wait_connection(fd);

		for (;;)
		{
			memset(g_data, 0, 128);

			int i;
			char c;

			c=0;

			for (i = 0; i < 128 && c != '\n';)
			{
				ret = read(fd, &c, 1);
				if(ret<0){
					int errcode = errno;
					fprintf(stderr, "%s: read() failed: %s (%d)\n", CONFIG_EXAMPLES_BLESENSOR_DEVNAME, strerror(errcode), errcode);
				}
				if(c != 0 && c !=255){
					g_data[i] = c;
					printf("Read %c\n", c);
					i++;
				}

				if(c == 'Z'){
					printf("Write test\n");
					ret = write(fd, "WRITE TEST\r\n", 12);
				}
			}

			g_data[i-2] = '\0';
			if(!strcmp(g_data, "NO CARRIER")){
				printf("Connection lost\n");
				break;
			}

		}
	}

	ASSERT(ret == 0);

	return 0;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * sensor_main
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int bluetooth_main(int argc, char *argv[])
#endif
{
  int fd, ret;
  struct termios tio;

  printf("Bluetooth start...\n");

  ret = board_gpio_config(80,0,false,false,1);
  if(ret<0){
	  int errcode = errno;
	  fprintf(stderr, "ERROR: board_gpio_config() failed: %d\n", errcode);
  }

  board_gpio_write(80,0);

  ret = board_gpio_config(79,0,false,false,1);
  if(ret<0){
	  int errcode = errno;
	  fprintf(stderr, "ERROR: board_gpio_config() failed: %d\n", errcode);
  }

  board_gpio_write(79,0);
  usleep(1000);
  board_gpio_write(79,1);

  g_data = (char *)malloc(128);
  if (!g_data)
    {
      printf("Memory allocation failure.\n");
      return -1;
    }
  memset(g_data, 0, 128);

  fd = open(CONFIG_EXAMPLES_BLESENSOR_DEVNAME, O_RDWR);
  if (fd < 0)
    {
      printf("Device %s open failure. %d\n",
             CONFIG_EXAMPLES_BLESENSOR_DEVNAME, fd);
      free(g_data);
      return -1;
    }

  ret = tcgetattr(fd, &tio);
  if (ret < 0)
  {
  	  int errcode = errno;
  	  fprintf(stderr, "%s: tcgetattr() failed: %s (%d)\n", CONFIG_EXAMPLES_BLESENSOR_DEVNAME, strerror(errcode), errcode);
  }

  ret = cfsetspeed(&tio, B57600);
  if (ret < 0)
  {
  	  int errcode = errno;
  	  fprintf(stderr, "%s: cfsetspeed() failed: %s (%d)\n", CONFIG_EXAMPLES_BLESENSOR_DEVNAME, strerror(errcode), errcode);
  }

  tio.c_cflag = 0xBEDF;

  ret = tcsetattr(fd, TCSANOW, &tio);
  if (ret < 0)
  {
  	  int errcode = errno;
  	  fprintf(stderr, "%s: tcsetattr() failed: %s (%d)\n", CONFIG_EXAMPLES_BLESENSOR_DEVNAME, strerror(errcode), errcode);
  }

  usleep(10000);

  sensing_main(fd);

  close(fd);

  free(g_data);

  return 0;
}
