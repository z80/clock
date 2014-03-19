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
#include "3310.h"

#include "ff.h"

/*
 * Application entry point.
 */
int main(void) {

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


  MMCDriver MMCD1;

  /* Maximum speed SPI configuration (18MHz, CPHA=0, CPOL=0, MSb first).*/
  //static SPIConfig hs_spicfg = { NULL, GPIOB, 12, 0 };
  static SPIConfig hs_spicfg = { NULL, GPIOB, 12, SPI_CR1_BR_2 | SPI_CR1_BR_1 };

  /* Low speed SPI configuration (281.250kHz, CPHA=0, CPOL=0, MSb first).*/
  static SPIConfig ls_spicfg = { NULL, GPIOB, 12,
                                 SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0 };

  /* MMC/SD over SPI driver configuration.*/
  static MMCConfig mmccfg = {&SPID1, &ls_spicfg, &hs_spicfg};

  palSetPad( GPIOB, 12 );
  palSetPad( GPIOB, 13 );
  palSetPad( GPIOB, 14 );
  palSetPad( GPIOB, 15 );
  palSetPadMode( GPIOB, 13, PAL_MODE_STM32_ALTERNATE_PUSHPULL );     // SCK
  palSetPadMode( GPIOB, 14, PAL_MODE_STM32_ALTERNATE_PUSHPULL );     // MISO
  palSetPadMode( GPIOB, 15, PAL_MODE_STM32_ALTERNATE_PUSHPULL );     // MOSI
  palSetPadMode( GPIOB, 12, PAL_MODE_OUTPUT_PUSHPULL );              // CS
  palSetPad( GPIOB, 12 ); // Set CS high

  mmcObjectInit(&MMCD1);
  mmcStart(&MMCD1, &mmccfg);
  int res = -0;
  if (mmcConnect(&MMCD1))
  {
    // Fail.
    res = 1;
  } else {
    //chprintf(chp, "SD CARD SUCCEEDED");
    res = 0;
  }


  //playbackInit();

  //lcdInit();
  //lcdClear();
  //lcdGotoXy( 0, 0 );
  //lcdStrConst( FONT_1X, "Hi!" );
  //lcdUpdate();
  while ( TRUE )
  {
    //play( "0:anthem01.raw" );
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
