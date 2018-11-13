/*
 * Copyright (c) 2013, Marcus Linderoth, http://forfunandprof.it
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/**
 * \file
 *         Blink example application,
 *          Demonstrates starting another process from one, setting timers, using LEDs.
 *          expected result from running it:
 *          red LED flashing fast (toggling every 125 ms), green every second.
 * \author
 *         Marcus Linderoth <linderoth.marcus@gmail.com>
 */

#include <stdio.h>
#include "contiki.h"
#include "dev/leds.h"

/* -------------------------------------------------------------------------- */
PROCESS(red_process, "Red light process");
PROCESS(blink_process, "Blink");
AUTOSTART_PROCESSES(&blink_process);
/*---------------------------------------------------------------------------*/
/* This process is started from the other process and blinks an LED fast.
*/
static struct etimer etr;
PROCESS_THREAD(red_process, ev, data)
{
  PROCESS_POLLHANDLER();
  PROCESS_EXITHANDLER();
  PROCESS_BEGIN();
  while(1) {
    leds_toggle(LEDS_RED);///紅燈一明一滅
    etimer_set(&etr, CLOCK_SECOND/8);///設置event timer 過期時間為1/8秒 -> 125ms \ (放在迴圈外也可以吧?)
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&etr));///卡在這裡直到event timer 過期
  }
  PROCESS_END();
}

/*--------------------------------------------------------------------------*/
/* this process starts another process and then periodically blinks an LED */
static struct etimer et;

PROCESS_THREAD(blink_process, ev, data)
{
  PROCESS_BEGIN();

  etimer_set(&et, CLOCK_SECOND);///設置event timer過期時間為1秒
  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));///卡在此行直到event timer到了自行設定的過期時間
  process_start(&red_process, NULL);///啟動"red process"

  while(1) {
    /* unnecessary, messy but wanted sth else than what the red process
     * is doing so instead of just toggling, turn on when even seconds and
     * turn off on uneven seconds.
     */
    if(clock_seconds() & 1) {///如果系統時間除以2 == 1 -> 奇數時間
      leds_on(LEDS_GREEN);///開綠燈
    } else {///系統時間除以2 == 0 -> 偶數時間
      leds_off(LEDS_GREEN);///關綠燈
    }
    etimer_set(&et, CLOCK_SECOND);///設置event timer過期時間為1秒
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));///卡在此行直到event timer到了自行設定的過期時間
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/

