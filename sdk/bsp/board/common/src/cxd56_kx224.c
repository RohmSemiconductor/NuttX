/****************************************************************************
 * bsp/board/common/src/cxd56_kx224.c
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

#include <stdio.h>
#include <debug.h>
#include <errno.h>

#include <nuttx/board.h>

#include <nuttx/sensors/kx224.h>
#ifdef CONFIG_KX224_SCU
#include <arch/chip/cxd56_scu.h>
#endif

#ifdef CONFIG_KX224_SCU
#  ifdef CONFIG_CXD56_DECI_KX224
#    define KX224_FIFO_CNT 3
#  else
#    define KX224_FIFO_CNT 1
#  endif
#endif

#include "cxd56_i2c.h"

#if defined(CONFIG_CXD56_I2C) && defined(CONFIG_KX224)

#ifdef CONFIG_KX224_SCU
int board_kx224_initialize(FAR const char *devpath, int bus)
{
  int fifoid = 0;
  int ret;
  FAR struct i2c_master_s *i2c;

  sninfo("Initializing KX224...\n");

  /* Initialize i2c deivce */

  i2c = cxd56_i2cbus_initialize(bus);
  if (!i2c)
    {
      snerr("ERROR: Failed to initialize i2c%d.\n", bus);
      return -ENODEV;
    }

  ret = kx224_init(i2c, bus);
  if (ret < 0)
    {
      snerr("Error initialize KX224.\n");
      return ret;
    }

  /* Register devices for each FIFOs at I2C bus */

  for (fifoid = 0; fifoid < KX224_FIFO_CNT; fifoid++)
    {
      ret = kx224_register(devpath, fifoid, i2c, bus);
      if (ret < 0)
        {
          snerr("Error registering KX224.\n");
          return ret;
        }
    }

  return ret;
}
#endif /* CONFIG_KX224_SCU */

#endif /* CONFIG_CXD56_I2C && CONFIG_KX224 */
