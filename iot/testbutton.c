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
 *         Testing button example
 *          When pressing the button, the red LED will toggle until enough presses,
 *          then the green blinks stops and the button becomes unresponsive.
 * \author
 *         Marcus Linderoth <linderoth.marcus@gmail.com>
 */

#include <stdio.h>
#include "contiki.h"
#include "dev/leds.h"
#include "dev/button.h"

/*
 * how to read a Contiki program: start by finding AUTOSTART_PROCESSES(...).
 * The first process in that list is started first. Then, if it starts a new
 * process by process_start() execution flow continues in that process until
 * any PROCESS_YIELD_* or PROCESS_WAIT_*.
 */
/* -------------------------------------------------------------------------- */
PROCESS(button_process, "Button process");
PROCESS(blink_process, "Blink process");
AUTOSTART_PROCESSES(&blink_process, &button_process);///兩個process同時啟動
/*---------------------------------------------------------------------------*/
/* This process handles button presses; up until 5 presses it toggles red LED
after that it kills the blink process if the button is pressed again. */
static struct etimer etr;

PROCESS_THREAD(button_process, ev, data)
{
  PROCESS_POLLHANDLER();
  PROCESS_EXITHANDLER();
  PROCESS_BEGIN();
  static uint8_t btnpresses = 0;///宣告一個unsigned char 作為計數的變數
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == button_event);///卡住直到ev與button_event相同
    leds_toggle(LEDS_RED);///紅燈一明一滅
    btnpresses++;///unsigned char + 1
    if(btnpresses == 5) {///如果累計到五次
      process_exit(&blink_process);///結束掉"blink_process"
    }
  }
  PROCESS_END();
}
/*--------------------------------------------------------------------------*/
/* this process periodically blinks an LED */
static struct etimer et;

PROCESS_THREAD(blink_process, ev, data)
{
  PROCESS_BEGIN();
  while(1) {
    leds_toggle(LEDS_GREEN);///綠燈一明一滅
    etimer_set(&et, CLOCK_SECOND/8);///設置event timer et 過期時間為1/8s -> 125ms
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));///卡住直到event timer過期
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
