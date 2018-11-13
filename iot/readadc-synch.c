/**
 * \addtogroup launchpad-examples
 *
 * @{
 */

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
 *    readadc-synch.c
 * \author
 *         Marcus Linderoth <linderoth.marcus@gmail.com>
 * \brief
 *    ADC reading example. One process periodically reads the ADC on one channel
 *    and updates a blink speed variable. Another sets a LED blink according to
 *    that variable. Program execution is blocked while ADC is converting.
 *
 */

#include <stdio.h>
#include "contiki.h"
#include "dev/leds.h"
#include "dev/adc.h"

static volatile uint16_t adc = 0;///告訴compiler, adc變數具有揮發性的特性, 所以與它有關的程式碼請不要作最佳化動作.
/*--------------------------------------------------------------------------*/
PROCESS(adc_reading_process, "ADC reading process");
PROCESS(blink_process, "My Process");
AUTOSTART_PROCESSES(&adc_reading_process, &blink_process);
/*--------------------------------------------------------------------------*/
static uint16_t time = 0;///unsigned short
static struct etimer etb;
PROCESS_THREAD(blink_process, ev, data) {
  PROCESS_POLLHANDLER();
  PROCESS_EXITHANDLER();
  PROCESS_BEGIN();
  while(1) {
    leds_toggle(LEDS_RED);///紅燈一明一滅
    if(adc > 0) {///adc 值大於 0
      time = (CLOCK_SECOND*adc)/1024;///time 設定成 1*adc再除以1024 -> adc數值越大，time越大
    } else {///adc 值小於 0
      time = CLOCK_SECOND/64;///time 設定成 1除以64 -> 1/64 -> 0.015s -> 15ms
    }
    if(time == 0) {///不能讓time < 0
      time = 1;
    }
    etimer_set(&etb, time);///設置event timer過期時間
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&etb));///卡住直到etb過期
  }
  PROCESS_END();
}
/* -------------------------------------------------------------------------- */
static struct etimer et;
PROCESS_THREAD(adc_reading_process, ev, data)
{
  PROCESS_BEGIN();
  while(1) {

    adc = adc_get(A7);///read analog in A7, block until done and store result in the variable adc
    etimer_set(&et, CLOCK_SECOND/8);///設置event timer過期時間為1/8s -> 125ms
    printf("ADC: %u\n", adc);///印出讀取到的變數值
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));///卡住直到et過期
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
/** @} */
