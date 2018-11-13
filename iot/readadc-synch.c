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

static volatile uint16_t adc = 0;///�i�Dcompiler, adc�ܼƨ㦳���o�ʪ��S��, �ҥH�P���������{���X�Ф��n�@�̨Τưʧ@.
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
    leds_toggle(LEDS_RED);///���O�@���@��
    if(adc > 0) {///adc �Ȥj�� 0
      time = (CLOCK_SECOND*adc)/1024;///time �]�w�� 1*adc�A���H1024 -> adc�ƭȶV�j�Atime�V�j
    } else {///adc �Ȥp�� 0
      time = CLOCK_SECOND/64;///time �]�w�� 1���H64 -> 1/64 -> 0.015s -> 15ms
    }
    if(time == 0) {///������time < 0
      time = 1;
    }
    etimer_set(&etb, time);///�]�mevent timer�L���ɶ�
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&etb));///�d����etb�L��
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
    etimer_set(&et, CLOCK_SECOND/8);///�]�mevent timer�L���ɶ���1/8s -> 125ms
    printf("ADC: %u\n", adc);///�L�XŪ���쪺�ܼƭ�
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));///�d����et�L��
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
/** @} */
