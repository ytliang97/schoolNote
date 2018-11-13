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
 *        Testing serial in-/output
 *        usage
 *          compile, upload to LP, login over serial
 *          remember to adjust the rxtx headers on the board
 *        expected result when running it:
 *          will print out the time every second, will repeat back anything sent to it
 *          and if sent "red" or "green", it will toggle the corresponding LED
 * \author
 *         Marcus Linderoth <linderoth.marcus@gmail.com>
 */

#include <stdio.h>
#include <string.h>
#include "contiki.h"
#include "dev/leds.h"
#include "dev/serial-line.h"

/*---------------------------------------------------------------------------*/
PROCESS(serial_read_process, "Serial Reader");
AUTOSTART_PROCESSES(&serial_read_process);
/*--------------------------------------------------------------------------*/
/* will repeatedly wait for serial data and then repeat back what it received.
 * If the data is "red" or "green", it will toggle the corresponding LED */
PROCESS_THREAD(serial_read_process, ev, data) {
  PROCESS_BEGIN();
  printf("starting serial echo\n");///啟動時印出
  while (1) {
    char* buf;
    PROCESS_WAIT_EVENT_UNTIL(ev == serial_line_event_message);
    buf = data;

    // if(!strncmp(buf, "red", 3)) {///收到"red"字串
    //   leds_toggle(LEDS_RED);///紅燈一明一滅
    // } else if (!strncmp(buf, "green", 5)) {///收到"green"字串
    //   leds_toggle(LEDS_GREEN);///綠燈一明一滅
    // } else if (!strncmp(buf, "off", 3)) {///收到off字串
    //   leds_off(LEDS_GREEN | LEDS_RED);///
    // }

    printf("Got:%s\n", buf);///印出收到的字串
  }
  PROCESS_END();
}

/*--------------------------------------------------------------------------*/
/* repeatedly printing out the time over the serial port (9600 baud, 8,n,1) */
static struct etimer et;

PROCESS_THREAD(blink_process, ev, data)
{
  PROCESS_BEGIN();
  while(1) {
    printf("Time since bootup: %lu\n", clock_seconds());///印出現在的系統時間(距離開機已多少個tick
    etimer_set(&et, CLOCK_SECOND);///設置event timer 過期時間1秒
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));///卡住直到event timer et 過期
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
