/************************************************************
 * up_assert.c
 *
 *   Copyright (C) 2007 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <spudmonkey@racsa.co.cr>
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
 * 3. Neither the name Gregory Nutt nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
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
 ************************************************************/

/************************************************************
 * Included Files
 ************************************************************/

#include <nuttx/config.h>
#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <sched.h>
#include <debug.h>
#include <8052.h>
#include "os_internal.h"
#include "up_internal.h"
#include "up_mem.h"

/************************************************************
 * Definitions
 ************************************************************/

/************************************************************
 * Private Data
 ************************************************************/

/************************************************************
 * Private Functions
 ************************************************************/

/************************************************************
 * Name: _up_assert
 ************************************************************/

static void _up_assert(int errorcode) /* __attribute__ ((noreturn)) */
{
  /* Are we in an interrupt handler or the idle task? */

  if (g_irqtos || ((FAR _TCB*)g_readytorun.head)->pid == 0)
    {
       (void)irqsave();
        for(;;)
          {
#ifdef CONFIG_ARCH_LEDS
            up_ledon(LED_PANIC);
            up_delay(250);
            up_ledoff(LED_PANIC);
            up_delay(250);
#endif
          }
    }
  else
    {
      exit(errorcode);
    }
}

/************************************************************
 * Public Functions
 ************************************************************/

/************************************************************
 * Name: up_assert
 ************************************************************/

void up_assert(void)
{
#if CONFIG_TASK_NAME_SIZE > 0
  _TCB *rtcb = (_TCB*)g_readytorun.head;
#endif

  up_ledon(LED_ASSERTION);

#if CONFIG_TASK_NAME_SIZE > 0
  lldbg("%s: Assertion failed\n", rtcb->name);
#else
  lldbg("Assertion failed\n");
#endif

  up_dumpstack();
  _up_assert(EXIT_FAILURE);
}

/************************************************************
 * Name: up_assert_code
 ************************************************************/

void up_assert_code(int errorcode)
{
#if CONFIG_TASK_NAME_SIZE > 0
  _TCB *rtcb = (_TCB*)g_readytorun.head;
#endif

  up_ledon(LED_ASSERTION);

#if CONFIG_TASK_NAME_SIZE > 0
  lldbg("%s: Assertion failed, error=%d\n", rtcb->name, errorcode);
#else
  lldbg("Assertion failed , error=%d\n", errorcode);
#endif

  up_dumpstack();
 _up_assert(errorcode);
}
