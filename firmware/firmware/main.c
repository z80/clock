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

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "ch.h"
#include "hal.h"

//#include "shell.h"
//#include "chprintf.h"
#include "chrtclib.h"
#include "ff.h"


/**
 * @brief FS object.
 */
FATFS MMC_FS;

/**
 * MMC driver instance.
 */
MMCDriver MMCD1;

/* FS mounted and ready.*/
static bool_t fs_ready = FALSE;

/* Maximum speed SPI configuration (18MHz, CPHA=0, CPOL=0, MSb first).*/
static SPIConfig hs_spicfg = {NULL, IOPORT2, GPIOB_SPI2NSS, 0};

/* Low speed SPI configuration (281.250kHz, CPHA=0, CPOL=0, MSb first).*/
static SPIConfig ls_spicfg = {NULL, IOPORT2, GPIOB_SPI2NSS,
                              SPI_CR1_BR_2 | SPI_CR1_BR_1};

/* MMC/SD over SPI driver configuration.*/
static MMCConfig mmccfg = {&SPID2, &ls_spicfg, &hs_spicfg};

/**
 *
 */
bool_t mmc_lld_is_write_protected(MMCDriver *sdcp) {
  (void)sdcp;
  return FALSE;
}

/**
 *
 */
bool_t mmc_lld_is_card_inserted(MMCDriver *sdcp) {
  (void)sdcp;
  return TRUE;
}

/**
 *
 */
void cmd_sdiotest( void )
{
    FRESULT err;
    FIL FileObject;
    //FILINFO FileInfo;
    struct tm timp;

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

    chThdSleepMilliseconds(100);

    rtcGetTimeTm(&RTCD1, &timp);

    chThdSleepMilliseconds(100);
    err = f_open(&FileObject, "0:tmstmp.tst", FA_READ );
    if (err != FR_OK) {
      chSysHalt();
    }


    chThdSleepMilliseconds(100);
    err = f_close(&FileObject);
    if (err != FR_OK) {
      chSysHalt();
    }

//    chprintf(chp, "Obtaining file info ... ");
//    chThdSleepMilliseconds(100);
//    err = f_stat("0:tmstmp.tst", &FileInfo);
//    if (err != FR_OK) {
//      chSysHalt();
//    }
//    else{
//      chprintf(chp, "OK\r\n");
//      chprintf(chp, "    Timestamp: %u-%u-%u %u:%u:%u\r\n",
//                         ((FileInfo.fdate >> 9) & 127) + 1980,
//                         (FileInfo.fdate >> 5) & 15,
//                         FileInfo.fdate & 31,
//                         (FileInfo.ftime >> 11) & 31,
//                         (FileInfo.ftime >> 5) & 63,
//                         (FileInfo.ftime & 31) * 2);
//    }

    f_mount(0, NULL);

    chThdSleepMilliseconds(100);
    if (mmcDisconnect(&MMCD1))
      chSysHalt();
    chThdSleepMilliseconds(100);
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


  /*
   * Initializes the SDIO drivers.
   */
  mmcObjectInit(&MMCD1);
  mmcStart(&MMCD1, &mmccfg);

  while (TRUE)
  {
      cmd_sdiotest();
      chThdSleepSeconds( 5 );
  }
}


