/****************************************************************************
 * halleffect/halleffect_main.c
 *
 *   Copyright (C) 2018 ROHM Co.,Ltd.
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
 * 3. Neither the name NuttX nor Sony nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAHALLEFFECTES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAHALLEFFECTE.
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

#ifdef CONFIG_bd7411g
#include <nuttx/sensors/bd7411g.h>
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifndef CONFIG_EXAMPLES_HALLEFFECT_DEVNAME
#  define CONFIG_EXAMPLES_HALLEFFECT_DEVNAME "/dev/halle0"
#endif

#ifndef CONFIG_EXAMPLES_HALLEFFECT_SIGNO
#  define CONFIG_EXAMPLES_HALLEFFECT_SIGNO 13
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

static char *g_data;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int sensing_main(int fd)
{
  while (1)
  {
    read(fd, g_data, 128);
    if (*g_data == 0) {
      printf("Magnet field detected\n");
      
    }
    else {
      printf("\n");
    }
    usleep(300000);
  }
  return OK;
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
int halleffect_main(int argc, char *argv[])
#endif
{
  int fd;

  printf("Sensing start...\n");

  g_data = (char *)malloc(1024);
  if (!g_data)
    {
      printf("Memory allocation failure.\n");
      return -1;
    }
  memset(g_data, 0, 1024);

  fd = open(CONFIG_EXAMPLES_HALLEFFECT_DEVNAME, O_RDONLY);
  if (fd < 0)
    {
      printf("Device %s open failure. %d\n",
             CONFIG_EXAMPLES_HALLEFFECT_DEVNAME, fd);
      return -1;
    }

  sensing_main(fd);

  close(fd);

  free(g_data);

  return 0;
}
