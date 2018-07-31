/****************************************************************************
 * drivers/sensors/bd7411g.c
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

#include <stdlib.h>
#include <fixedmath.h>
#include <errno.h>
#include <debug.h>

#include <nuttx/kmalloc.h>
#include <nuttx/fs/fs.h>
#include <nuttx/spi/spi.h>
#include <nuttx/sensors/bd7411g.h>

#include <arch/chip/pin.h>
#include <arch/board/common/cxd56_gpioif.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
// Define pin as UART2 RXD
#define GPIO_PIN	68

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/* Character driver methods */

static int     bd7411g_open(FAR struct file *filep);
static int     bd7411g_close(FAR struct file *filep);
static ssize_t bd7411g_read(FAR struct file *filep, FAR char *buffer,
                            size_t len);


/****************************************************************************
 * Private Data
 ****************************************************************************/

/* This the vtable that supports the character driver interface */

static const struct file_operations g_bd7411gfops =
{
  bd7411g_open,    /* open */
  bd7411g_close,   /* close */
  bd7411g_read,    /* read */
  0,               /* write */
  0,               /* seek */
};

/****************************************************************************
 * Name: bd7411g_open
 *
 * Description:
 *   Standard character driver open method.
 *
 ****************************************************************************/

static int bd7411g_open(FAR struct file *filep)
{
  return OK;
}

/****************************************************************************
 * Name: bd7411g_close
 *
 * Description:
 *   Standard character driver close method.
 *
 ****************************************************************************/

static int bd7411g_close(FAR struct file *filep)
{
  return OK;
}

/****************************************************************************
 * Name: bd7411g_read
 *
 * Description:
 *   Standard character driver read method.
 *
 ****************************************************************************/

static ssize_t bd7411g_read(FAR struct file *filep, FAR char *buffer, size_t len)
{
  *buffer = board_gpio_read(GPIO_PIN);
  return OK;
}

/****************************************************************************
 * Name: bd7411g_register
 *
 * Description:
 *   Register the BD7411G character device as 'devpath'
 *
 * Input Parameters:
 *   devpath - The full path to the driver to register. E.g., "/dev/halle0"
 *   dev     - An instance of the SPI interface to use to communicate with
 *             BD7411G
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno value on failure.
 *
 ****************************************************************************/

#ifdef CONFIG_BD7411G
int bd7411g_register(FAR const char *devpath)
{
  int ret = 0;

  ret = register_driver(devpath, &g_bd7411gfops, 0666, NULL);
  if (ret < 0)
    {
      snerr("Failed to register driver: %d\n", ret);
    }

  sninfo("BD7411G driver loaded successfully!\n");

  cxd56_gpio_config(GPIO_PIN, true);

  return OK;
}

#endif /* CONFIG_BD7411G */
