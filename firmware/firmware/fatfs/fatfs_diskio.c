/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
/*-----------------------------------------------------------------------*/
/* This is a stub disk I/O module that acts as front end of the existing */
/* disk I/O modules and attach it to FatFs module with common interface. */
/*-----------------------------------------------------------------------*/

#include "ch.h"
#include "hal.h"
#include "ffconf.h"
#include "diskio.h"

/* Definitions for MMC/SDC command */
#define CMD0    (0x40+0)    /* GO_IDLE_STATE */
#define CMD1    (0x40+1)    /* SEND_OP_COND (MMC) */
#define ACMD41  (0xC0+41)   /* SEND_OP_COND (SDC) */
#define CMD8    (0x40+8)    /* SEND_IF_COND */
#define CMD9    (0x40+9)    /* SEND_CSD */
#define CMD10   (0x40+10)   /* SEND_CID */
#define CMD12   (0x40+12)   /* STOP_TRANSMISSION */
#define ACMD13  (0xC0+13)   /* SD_STATUS (SDC) */
#define CMD16   (0x40+16)   /* SET_BLOCKLEN */
#define CMD17   (0x40+17)   /* READ_SINGLE_BLOCK */
#define CMD18   (0x40+18)   /* READ_MULTIPLE_BLOCK */
#define CMD23   (0x40+23)   /* SET_BLOCK_COUNT (MMC) */
#define ACMD23  (0xC0+23)   /* SET_WR_BLK_ERASE_COUNT (SDC) */
#define CMD24   (0x40+24)   /* WRITE_BLOCK */
#define CMD25   (0x40+25)   /* WRITE_MULTIPLE_BLOCK */
#define CMD55   (0x40+55)   /* APP_CMD */
#define CMD58   (0x40+58)   /* READ_OCR */


/* Manley EK-STM32F board does not offer socket contacts -> dummy values: */
#define SOCKPORT    1           /* Socket contact port */
#define SOCKWP      0           /* Write protect switch (PB5) */
#define SOCKINS     0           /* Card detect switch (PB4) */

enum speed_setting { INTERFACE_SLOW, INTERFACE_FAST };



#if HAL_USE_MMC_SPI && HAL_USE_SDC
#error "cannot specify both MMC_SPI and SDC drivers"
#endif

#if HAL_USE_MMC_SPI
extern MMCDriver MMCD1;
#elif HAL_USE_SDC
extern SDCDriver SDCD1;
#else
#error "MMC_SPI or SDC driver must be specified"
#endif

#if HAL_USE_RTC
#include "chrtclib.h"
extern RTCDriver RTCD1;
#endif

/*-----------------------------------------------------------------------*/
/* Correspondence between physical drive number and physical drive.      */

#define MMC     0
#define SDC     0

/* Card-Select Controls  (Platform dependent) */
#define SELECT()        spiSelect( MMCD1.config.spip )    /* MMC CS = L */
#define DESELECT()      spiUnselect( MMCD1.config.spip )  /* MMC CS = H */

#define power_on()      {}
#define power_off       {}


static void interface_speed( enum speed_setting speed )
{
    //MMCD1.config.spip
    //MMCD1.config.lscfg
    //MMCD1.config.hscfg

    tmp = SPI_SD->CR1;
    if ( speed == INTERFACE_SLOW ) {
        /* Set slow clock (100k-400k) */
        spiStart( MMCD1.config.spip, MMCD1.config.lscfg );
    } else {
        /* Set fast clock (depends on the CSD) */
        spiStart( MMCD1.config.spip, MMCD1.config.hscfg );
    }
}

/*-----------------------------------------------------------------------*/
/* Wait for card ready                                                   */
/*-----------------------------------------------------------------------*/

static
BYTE wait_ready (void)
{
    BYTE res;


    Timer2 = 50;    /* Wait for ready in timeout of 500ms */
    rcvr_spi();
    do
        res = rcvr_spi();
    while ((res != 0xFF) && Timer2);

    return res;
}


/*-----------------------------------------------------------------------*/
/* Send a command packet to MMC                                          */
/*-----------------------------------------------------------------------*/

static
BYTE send_cmd (
    BYTE cmd,       /* Command byte */
    DWORD arg       /* Argument */
)
{
    BYTE n, res;


    if (cmd & 0x80) {   /* ACMD<n> is the command sequence of CMD55-CMD<n> */
        cmd &= 0x7F;
        res = send_cmd(CMD55, 0);
        if (res > 1) return res;
    }

    /* Select the card and wait for ready */
    DESELECT();
    SELECT();
    if (wait_ready() != 0xFF) {
        return 0xFF;
    }

    /* Send command packet */
    xmit_spi(cmd);                      /* Start + Command index */
    xmit_spi((BYTE)(arg >> 24));        /* Argument[31..24] */
    xmit_spi((BYTE)(arg >> 16));        /* Argument[23..16] */
    xmit_spi((BYTE)(arg >> 8));         /* Argument[15..8] */
    xmit_spi((BYTE)arg);                /* Argument[7..0] */
    n = 0x01;                           /* Dummy CRC + Stop */
    if (cmd == CMD0) n = 0x95;          /* Valid CRC for CMD0(0) */
    if (cmd == CMD8) n = 0x87;          /* Valid CRC for CMD8(0x1AA) */
    xmit_spi(n);

    /* Receive command response */
    if (cmd == CMD12) rcvr_spi();       /* Skip a stuff byte when stop reading */

    n = 10;                             /* Wait for a valid response in timeout of 10 attempts */
    do
        res = rcvr_spi();
    while ((res & 0x80) && --n);

    return res;         /* Return with the response value */
}


/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */

DSTATUS disk_initialize (
    BYTE drv                /* Physical drive nmuber (0..) */
)
{
    BYTE n, cmd, ty, ocr[4];

    if (drv) return STA_NOINIT;         /* Supports only single drive */
    if (Stat & STA_NODISK) return Stat; /* No card in the socket */

    //power_on();                         /* Force socket power on and initialize interface */
    interface_speed( INTERFACE_SLOW );
    for (n = 10; n; n--) rcvr_spi();    /* 80 dummy clocks */

    ty = 0;
    if (send_cmd(CMD0, 0) == 1) {           /* Enter Idle state */
        Timer1 = 100;                       /* Initialization timeout of 1000 milliseconds */
        if (send_cmd(CMD8, 0x1AA) == 1) {   /* SDHC */
            for (n = 0; n < 4; n++) ocr[n] = rcvr_spi();        /* Get trailing return value of R7 response */
            if (ocr[2] == 0x01 && ocr[3] == 0xAA) {             /* The card can work at VDD range of 2.7-3.6V */
                while (Timer1 && send_cmd(ACMD41, 1UL << 30));  /* Wait for leaving idle state (ACMD41 with HCS bit) */
                if (Timer1 && send_cmd(CMD58, 0) == 0) {        /* Check CCS bit in the OCR */
                    for (n = 0; n < 4; n++) ocr[n] = rcvr_spi();
                    ty = (ocr[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2;
                }
            }
        } else {                            /* SDSC or MMC */
            if (send_cmd(ACMD41, 0) <= 1)   {
                ty = CT_SD1; cmd = ACMD41;  /* SDSC */
            } else {
                ty = CT_MMC; cmd = CMD1;    /* MMC */
            }
            while (Timer1 && send_cmd(cmd, 0));         /* Wait for leaving idle state */
            if (!Timer1 || send_cmd(CMD16, 512) != 0)   /* Set R/W block length to 512 */
                ty = 0;
        }
    }
    CardType = ty;
    release_spi();

    if (ty) {           /* Initialization succeeded */
        Stat &= ~STA_NOINIT;        /* Clear STA_NOINIT */
        interface_speed(INTERFACE_FAST);
    } else {            /* Initialization failed */
        power_off();
    }

    return Stat;
}



/*-----------------------------------------------------------------------*/
/* Return Disk Status                                                    */

DSTATUS disk_status (
    BYTE drv        /* Physical drive nmuber (0..) */
)
{
  DSTATUS stat;

  switch (drv) {
#if HAL_USE_MMC_SPI
  case MMC:
    stat = 0;
    /* It is initialized externally, just reads the status.*/
    if (blkGetDriverState(&MMCD1) != BLK_READY)
      stat |= STA_NOINIT;
    if (mmcIsWriteProtected(&MMCD1))
      stat |= STA_PROTECT;
    return stat;
#else
  case SDC:
    stat = 0;
    /* It is initialized externally, just reads the status.*/
    if (blkGetDriverState(&SDCD1) != BLK_READY)
      stat |= STA_NOINIT;
    if (sdcIsWriteProtected(&SDCD1))
      stat |= STA_PROTECT;
    return stat;
#endif
  }
  return STA_NODISK;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */

DRESULT disk_read (
    BYTE drv,        /* Physical drive nmuber (0..) */
    BYTE *buff,        /* Data buffer to store read data */
    DWORD sector,    /* Sector address (LBA) */
    BYTE count        /* Number of sectors to read (1..255) */
)
{
  switch (drv) {
#if HAL_USE_MMC_SPI
  case MMC:
    if (blkGetDriverState(&MMCD1) != BLK_READY)
      return RES_NOTRDY;
    if (mmcStartSequentialRead(&MMCD1, sector))
      return RES_ERROR;
    while (count > 0) {
      if (mmcSequentialRead(&MMCD1, buff))
        return RES_ERROR;
      buff += MMCSD_BLOCK_SIZE;
      count--;
    }
    if (mmcStopSequentialRead(&MMCD1))
        return RES_ERROR;
    return RES_OK;
#else
  case SDC:
    if (blkGetDriverState(&SDCD1) != BLK_READY)
      return RES_NOTRDY;
    if (sdcRead(&SDCD1, sector, buff, count))
      return RES_ERROR;
    return RES_OK;
#endif
  }
  return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */

#if _READONLY == 0
DRESULT disk_write (
    BYTE drv,            /* Physical drive nmuber (0..) */
    const BYTE *buff,    /* Data to be written */
    DWORD sector,        /* Sector address (LBA) */
    BYTE count            /* Number of sectors to write (1..255) */
)
{
  switch (drv) {
#if HAL_USE_MMC_SPI
  case MMC:
    if (blkGetDriverState(&MMCD1) != BLK_READY)
        return RES_NOTRDY;
    if (mmcIsWriteProtected(&MMCD1))
        return RES_WRPRT;
    if (mmcStartSequentialWrite(&MMCD1, sector))
        return RES_ERROR;
    while (count > 0) {
        if (mmcSequentialWrite(&MMCD1, buff))
            return RES_ERROR;
        buff += MMCSD_BLOCK_SIZE;
        count--;
    }
    if (mmcStopSequentialWrite(&MMCD1))
        return RES_ERROR;
    return RES_OK;
#else
  case SDC:
    if (blkGetDriverState(&SDCD1) != BLK_READY)
      return RES_NOTRDY;
    if (sdcWrite(&SDCD1, sector, buff, count))
      return RES_ERROR;
    return RES_OK;
#endif
  }
  return RES_PARERR;
}
#endif /* _READONLY */



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */

DRESULT disk_ioctl (
    BYTE drv,        /* Physical drive nmuber (0..) */
    BYTE ctrl,        /* Control code */
    void *buff        /* Buffer to send/receive control data */
)
{
  switch (drv) {
#if HAL_USE_MMC_SPI
  case MMC:
    switch (ctrl) {
    case CTRL_SYNC:
        return RES_OK;
    case GET_SECTOR_SIZE:
        *((WORD *)buff) = MMCSD_BLOCK_SIZE;
        return RES_OK;
#if _USE_ERASE
    case CTRL_ERASE_SECTOR:
        mmcErase(&MMCD1, *((DWORD *)buff), *((DWORD *)buff + 1));
        return RES_OK;
#endif
    default:
        return RES_PARERR;
    }
#else
  case SDC:
    switch (ctrl) {
    case CTRL_SYNC:
        return RES_OK;
    case GET_SECTOR_COUNT:
        *((DWORD *)buff) = mmcsdGetCardCapacity(&SDCD1);
        return RES_OK;
    case GET_SECTOR_SIZE:
        *((WORD *)buff) = MMCSD_BLOCK_SIZE;
        return RES_OK;
    case GET_BLOCK_SIZE:
        *((DWORD *)buff) = 256; /* 512b blocks in one erase block */
        return RES_OK;
#if _USE_ERASE
    case CTRL_ERASE_SECTOR:
        sdcErase(&SDCD1, *((DWORD *)buff), *((DWORD *)buff + 1));
        return RES_OK;
#endif
    default:
        return RES_PARERR;
    }
#endif
  }
  return RES_PARERR;
}

DWORD get_fattime(void) {
#if HAL_USE_RTC
    return rtcGetTimeFat(&RTCD1);
#else
    return ((uint32_t)0 | (1 << 16)) | (1 << 21); /* wrong but valid time */
#endif
}



