/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.h"
#include "hal.h"
//#include "test.h"

#include "dac.h"
#include "playback.h"
#include "lcd3310.h"


/*
 * Application entry point.
 */

void main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();


  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state.
   */

  //palSetPad( GPIO,  )
  lcd3310Init( &SPID1 );
  //lcdInit();
  //playbackInit();

  for (;;)
  {
    lcd3310Clear( &SPID1 );
    lcd3310SetPosXY( &SPID1, 10, 10 );
    lcd3310WriteText( &SPID1, (const uint8_t *)"Hello!" );

    chThdSleepSeconds( 1 );
    //play( "anthem02.raw" );

    lcd3310Clear( &SPID1 );
    lcd3310SetPosXY( &SPID1, 10, 10 );
    lcd3310WriteText( &SPID1, (const uint8_t *)"World!" );
    chThdSleepSeconds( 1 );
    //play( "anthem01.raw" );
  }



  /*
  int val = 0;
  dacInit();
  while (TRUE) {
    while (val < (1<<12))
    {
    	DacCfg d;
    	d.dac1 = val;
    	d.dac2 = val;
    	dacSet( &d );
        chThdSleepMilliseconds(1);
    }
    val = 0;
  }
  */
}
