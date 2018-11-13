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
 *         Blink with rtimer example application,
 *          Demonstrates starting another process from one, setting timers, using LEDs.
 *          expected result from running it:
 *          red LED lighting up for 1/4 second every second, the green toggling every s.
 * \author
 *         Marcus Linderoth <linderoth.marcus@gmail.com>
 */

#include <stdio.h>
#include "contiki.h"
#include "dev/leds.h"

/*---------------------------------------------------------------------------*/
PROCESS(blink_process, "Blink");
AUTOSTART_PROCESSES(&blink_process);
/* -------------------------------------------------------------------------- */
static void
rtcb(struct rtimer *r, void *d)
{
  leds_off(LEDS_RED);///關紅燈
}
/*--------------------------------------------------------------------------*/
static struct rtimer rt;
static struct etimer et;

PROCESS_THREAD(blink_process, ev, data)
{
  PROCESS_BEGIN();
  while(1) {
    leds_on(LEDS_RED);///紅燈亮
    /* start an rtimer; the 1 signifies how long time in rtimer ticks the task
     * will take (ie the callback) but is currently not implemented on a deeper
     * level. So, use just any value for now. */
    rtimer_set(&rt, RTIMER_NOW() + RTIMER_SECOND/4, 1, rtcb, NULL);
    ///第二個參數是：第三個參數指定的function要在系統時間到"參數"時間以前完成
    ///假設現已100ticks，到100+RTIMER_SECOND/4 ticks前，rtcb要完成執行
    ///因為RTIMER_SECOND約一秒一tick的樣子，所以可以大約想成：開紅燈後，要在0.25s以內完成關燈

    leds_toggle(LEDS_GREEN);///一明一滅綠燈
    etimer_set(&et, CLOCK_SECOND/2);///設置event timer過期時間 1/2s -> 500ms
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));///卡住直到etimer_expired檢測到et過期
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/

