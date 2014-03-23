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

//#include <stdio.h>
//#include <string.h>
//#include <time.h>

#include "ch.h"
#include "hal.h"

#include "chrtclib.h"
#include "ff.h"


/**
 * @brief FS object.
 */
FATFS MMC_FS;


MMCDriver MMCD1;

// FS mounted and ready.
static bool_t fs_ready = FALSE;

// Maximum speed SPI configuration (18MHz, CPHA=0, CPOL=0, MSb first).
static SPIConfig hs_spicfg = {NULL, IOPORT2, GPIOB_SPI2NSS, 0};

// Low speed SPI configuration (281.250kHz, CPHA=0, CPOL=0, MSb first).
static SPIConfig ls_spicfg = {NULL, IOPORT2, GPIOB_SPI2NSS,
                              SPI_CR1_BR_2 | SPI_CR1_BR_1};

// MMC/SD over SPI driver configuration.
static MMCConfig mmccfg = {&SPID2, &ls_spicfg, &hs_spicfg};

bool_t mmc_lld_is_write_protected(MMCDriver *sdcp) {
  (void)sdcp;
  return FALSE;
}

bool_t mmc_lld_is_card_inserted(MMCDriver *sdcp) {
  (void)sdcp;
  return TRUE;
}


void cmd_sdiotest( void )
{
    FRESULT err;
    //FILINFO FileInfo;
    struct tm;

    // To not go into FatFS until debugger is running.
    int i = 1;
    while ( i )
        chThdSleepSeconds( 1 );


    err = f_mount(0, &MMC_FS);
    if (err != FR_OK)
    {
      chSysHalt();
    }
    else
    {
      fs_ready = TRUE;
    }

    FRESULT rc;
    FIL   fil;
    // FILINFO info;
    UINT br;
    // UINT bw, i;

    rc = f_open( &fil, "anthem01.raw", FA_READ );
    if ( rc != FR_OK )
        return;

    uint8_t buffer[ 512 ];
    // First compare flash content with file content.
    // To check if reflash is really necessary.
    do {
        rc = f_read( &fil, buffer, sizeof(buffer), &br );
        if ( rc != FR_OK )
            return;
    } while ( br > 0 );
    f_close( &fil );

    f_mount( 0, 0 );

}


/*===========================================================================*/
/* Generic code.                                                             */
/*===========================================================================*/

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


  // SPI setup.
  palSetPadMode( GPIOB, 13, PAL_MODE_STM32_ALTERNATE_PUSHPULL );     // SCK
  palSetPadMode( GPIOB, 14, PAL_MODE_STM32_ALTERNATE_PUSHPULL );     // MISO
  palSetPadMode( GPIOB, 15, PAL_MODE_STM32_ALTERNATE_PUSHPULL );     // MOSI
  palSetPadMode( GPIOB, 12, PAL_MODE_OUTPUT_PUSHPULL );              // CS
  palSetPad( GPIOB, 12 ); // Set CS high

  /*
   * Initializes the SDIO drivers.
   */
  mmcObjectInit(&MMCD1);
  mmcStart(&MMCD1, &mmccfg);

  //int i = 0;
  for (;;)
  {
      cmd_sdiotest();
      chThdSleepSeconds( 5 );
      //i += 1;
      //i += 2;
      //i += 3;
  }
}


















