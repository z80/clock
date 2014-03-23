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

/* Martin Thomas begin */

/* Card type flags (CardType) */
#define CT_MMC              0x01
#define CT_SD1              0x02
#define CT_SD2              0x04
#define CT_SDC              (CT_SD1|CT_SD2)
#define CT_BLOCK            0x08

/* Manley EK-STM32F board does not offer socket contacts -> dummy values: */
#define SOCKPORT    1           /* Socket contact port */
#define SOCKWP      0           /* Write protect switch (PB5) */
#define SOCKINS     0           /* Card detect switch (PB4) */


static const DWORD socket_state_mask_cp = (1 << 0);
static const DWORD socket_state_mask_wp = (1 << 1);

static volatile
DSTATUS Stat = STA_NOINIT;  /* Disk status */

static volatile
DWORD Timer1, Timer2;   /* 100Hz decrement timers */

static
BYTE CardType;          /* Card type flags */


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
#define SELECT()        spiSelect( MMCD1.config->spip )    /* MMC CS = L */
#define DESELECT()      spiUnselect( MMCD1.config->spip )  /* MMC CS = H */

#define power_on()      {}
#define power_off()     {}
static int chk_power(void)
{
    return 1; /* fake powered */
}

static void interface_speed( enum speed_setting speed )
{
    //MMCD1.config.spip
    //MMCD1.config.lscfg
    //MMCD1.config.hscfg

    if ( speed == INTERFACE_SLOW )
    {
        /* Set slow clock (100k-400k) */
        spiStart( MMCD1.config->spip, MMCD1.config->lscfg );
    }
    else
    {
        /* Set fast clock (depends on the CSD) */
        spiStart( MMCD1.config->spip, MMCD1.config->hscfg );
    }
}

/*-----------------------------------------------------------------------*/
/* Transmit/Receive a byte to MMC via SPI  (Platform dependent)          */
/*-----------------------------------------------------------------------*/
static BYTE stm32_spi_rw( BYTE out )
{
    /* Loop while DR register in not empty */
    /// not needed: while (SPI_I2S_GetFlagStatus(SPI_SD, SPI_I2S_FLAG_TXE) == RESET) { ; }

    /* Send byte through the SPI peripheral */
    //SPI_I2S_SendData(SPI_SD, out);
    spiSend( MMCD1.config->spip, 1, &out );

    /* Wait to receive a byte */
    //while (SPI_I2S_GetFlagStatus(SPI_SD, SPI_I2S_FLAG_RXNE) == RESET) { ; }

    /* Return the byte read from the SPI bus */
    BYTE res;
    spiReceive( MMCD1.config->spip, 1, &res );
    //return SPI_I2S_ReceiveData(SPI_SD);
    return res;
}



/*-----------------------------------------------------------------------*/
/* Transmit a byte to MMC via SPI  (Platform dependent)                  */
/*-----------------------------------------------------------------------*/

#define xmit_spi(dat)  stm32_spi_rw(dat)

/*-----------------------------------------------------------------------*/
/* Receive a byte from MMC via SPI  (Platform dependent)                 */
/*-----------------------------------------------------------------------*/

static
BYTE rcvr_spi (void)
{
    return stm32_spi_rw(0xff);
}

/* Alternative macro to receive data fast */
#define rcvr_spi_m(dst)  *(dst)=stm32_spi_rw(0xff)

/*-----------------------------------------------------------------------*/
/* Wait for card ready                                                   */
/*-----------------------------------------------------------------------*/
static uint32_t currentTime( void )
{
    RTCTime t;
    rtcGetTime( &RTCD1, &t );
    uint32_t result = t.tv_sec*1000 + t.tv_msec;
    return result;
}

static
BYTE wait_ready (void)
{
    BYTE res;

    uint32_t t0, t1;
    t0 = currentTime();
    //Timer2 = 50;    /* Wait for ready in timeout of 500ms */
    rcvr_spi();
    do
    {
        res = rcvr_spi();
        t1 = currentTime();
    } while ( (res != 0xFF) && ((t1 - t0) < 500) );

    return res;
}

/*-----------------------------------------------------------------------*/
/* Deselect the card and release SPI bus                                 */
/*-----------------------------------------------------------------------*/

static
void release_spi (void)
{
    DESELECT();
    rcvr_spi();
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
/* Receive a data packet from MMC                                        */
/*-----------------------------------------------------------------------*/

static
//BOOL rcvr_datablock (
bool_t rcvr_datablock (
    BYTE *buff,         /* Data buffer to store received data */
    UINT btr            /* Byte count (must be multiple of 4) */
)
{
    BYTE token;


    uint32_t t0, t1;
    t0 = currentTime();
    //Timer1 = 10;
    do {                            /* Wait for data packet in timeout of 100ms */
        token = rcvr_spi();
        t1 = currentTime();
    } while ( (token == 0xFF) &&
              ((t1 - t0) < 100) );
    if(token != 0xFE) return FALSE; /* If not valid data token, return with error */

    do {                            /* Receive the data block into buffer */
        rcvr_spi_m(buff++);
        rcvr_spi_m(buff++);
        rcvr_spi_m(buff++);
        rcvr_spi_m(buff++);
    } while (btr -= 4);

    rcvr_spi();                     /* Discard CRC */
    rcvr_spi();

    return TRUE;                    /* Return with success */
}

/*-----------------------------------------------------------------------*/
/* Send a data packet to MMC                                             */
/*-----------------------------------------------------------------------*/

#if _FS_READONLY == 0
static
//BOOL xmit_datablock (
bool_t xmit_datablock (
    const BYTE *buff,   /* 512 byte data block to be transmitted */
    BYTE token          /* Data/Stop token */
)
{
    BYTE resp;
    BYTE wc;

    if (wait_ready() != 0xFF) return FALSE;

    xmit_spi(token);                    /* transmit data token */
    if (token != 0xFD) {    /* Is data token */

        wc = 0;
        do {                            /* transmit the 512 byte data block to MMC */
            xmit_spi(*buff++);
            xmit_spi(*buff++);
        } while (--wc);

        xmit_spi(0xFF);                 /* CRC (Dummy) */
        xmit_spi(0xFF);
        resp = rcvr_spi();              /* Receive data response */
        if ((resp & 0x1F) != 0x05)      /* If not accepted, return with error */
            return FALSE;
    }

    return TRUE;
}
#endif /* _READONLY */


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
        //Timer1 = 100;                       /* Initialization timeout of 1000 milliseconds */
        uint32_t t0, t1;
        t0 = currentTime();
        if (send_cmd(CMD8, 0x1AA) == 1) {   /* SDHC */
            for (n = 0; n < 4; n++) ocr[n] = rcvr_spi();        /* Get trailing return value of R7 response */
            if (ocr[2] == 0x01 && ocr[3] == 0xAA) {             /* The card can work at VDD range of 2.7-3.6V */
                t1 = t0;
                while ( ((t1 - t0) < 1000) && send_cmd(ACMD41, 1UL << 30));  /* Wait for leaving idle state (ACMD41 with HCS bit) */
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
            while ((currentTime() - t0 < 1000) && send_cmd(cmd, 0));         /* Wait for leaving idle state */
            if ( /*!Timer1*/ ((currentTime() - t0) >= 1000) || send_cmd(CMD16, 512) != 0)   /* Set R/W block length to 512 */
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
    if (drv) return STA_NOINIT;     /* Supports only single drive */
    return Stat;
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
    if (drv || !count) return RES_PARERR;
    if (Stat & STA_NOINIT) return RES_NOTRDY;

    if (!(CardType & CT_BLOCK)) sector *= 512;  /* Convert to byte address if needed */

    if (count == 1) {   /* Single block read */
        if (send_cmd(CMD17, sector) == 0)   { /* READ_SINGLE_BLOCK */
            if (rcvr_datablock(buff, 512)) {
                count = 0;
            }
        }
    }
    else {              /* Multiple block read */
        if (send_cmd(CMD18, sector) == 0) { /* READ_MULTIPLE_BLOCK */
            do {
                if (!rcvr_datablock(buff, 512)) {
                    break;
                }
                buff += 512;
            } while (--count);
            send_cmd(CMD12, 0);             /* STOP_TRANSMISSION */
        }
    }
    release_spi();

    return count ? RES_ERROR : RES_OK;
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
    if (drv || !count) return RES_PARERR;
    if (Stat & STA_NOINIT) return RES_NOTRDY;
    if (Stat & STA_PROTECT) return RES_WRPRT;

    if (!(CardType & CT_BLOCK)) sector *= 512;  /* Convert to byte address if needed */

    if (count == 1) {   /* Single block write */
        if ((send_cmd(CMD24, sector) == 0)  /* WRITE_BLOCK */
            && xmit_datablock(buff, 0xFE))
            count = 0;
    }
    else {              /* Multiple block write */
        if (CardType & CT_SDC) send_cmd(ACMD23, count);
        if (send_cmd(CMD25, sector) == 0) { /* WRITE_MULTIPLE_BLOCK */
            do {
                if (!xmit_datablock(buff, 0xFC)) break;
                buff += 512;
            } while (--count);
            if (!xmit_datablock(0, 0xFD))   /* STOP_TRAN token */
                count = 1;
        }
    }
    release_spi();

    return count ? RES_ERROR : RES_OK;
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
    DRESULT res;
    BYTE n, csd[16], *ptr = buff;
    WORD csize;

    if (drv) return RES_PARERR;

    res = RES_ERROR;

    if (ctrl == CTRL_POWER) {
        switch (*ptr) {
        case 0:     /* Sub control code == 0 (POWER_OFF) */
            if (chk_power())
                power_off();        /* Power off */
            res = RES_OK;
            break;
        case 1:     /* Sub control code == 1 (POWER_ON) */
            power_on();             /* Power on */
            res = RES_OK;
            break;
        case 2:     /* Sub control code == 2 (POWER_GET) */
            *(ptr+1) = (BYTE)chk_power();
            res = RES_OK;
            break;
        default :
            res = RES_PARERR;
        }
    }
    else {
        if (Stat & STA_NOINIT) return RES_NOTRDY;

        switch (ctrl) {
        case CTRL_SYNC :        /* Make sure that no pending write process */
            SELECT();
            if (wait_ready() == 0xFF)
                res = RES_OK;
            break;

        case GET_SECTOR_COUNT : /* Get number of sectors on the disk (DWORD) */
            if ((send_cmd(CMD9, 0) == 0) && rcvr_datablock(csd, 16)) {
                if ((csd[0] >> 6) == 1) {   /* SDC version 2.00 */
                    csize = csd[9] + ((WORD)csd[8] << 8) + 1;
                    *(DWORD*)buff = (DWORD)csize << 10;
                } else {                    /* SDC version 1.XX or MMC*/
                    n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
                    csize = (csd[8] >> 6) + ((WORD)csd[7] << 2) + ((WORD)(csd[6] & 3) << 10) + 1;
                    *(DWORD*)buff = (DWORD)csize << (n - 9);
                }
                res = RES_OK;
            }
            break;

        case GET_SECTOR_SIZE :  /* Get R/W sector size (WORD) */
            *(WORD*)buff = 512;
            res = RES_OK;
            break;

        case GET_BLOCK_SIZE :   /* Get erase block size in unit of sector (DWORD) */
            if (CardType & CT_SD2) {    /* SDC version 2.00 */
                if (send_cmd(ACMD13, 0) == 0) { /* Read SD status */
                    rcvr_spi();
                    if (rcvr_datablock(csd, 16)) {              /* Read partial block */
                        for (n = 64 - 16; n; n--) rcvr_spi();   /* Purge trailing data */
                        *(DWORD*)buff = 16UL << (csd[10] >> 4);
                        res = RES_OK;
                    }
                }
            } else {                    /* SDC version 1.XX or MMC */
                if ((send_cmd(CMD9, 0) == 0) && rcvr_datablock(csd, 16)) {  /* Read CSD */
                    if (CardType & CT_SD1) {    /* SDC version 1.XX */
                        *(DWORD*)buff = (((csd[10] & 63) << 1) + ((WORD)(csd[11] & 128) >> 7) + 1) << ((csd[13] >> 6) - 1);
                    } else {                    /* MMC */
                        *(DWORD*)buff = ((WORD)((csd[10] & 124) >> 2) + 1) * (((csd[11] & 3) << 3) + ((csd[11] & 224) >> 5) + 1);
                    }
                    res = RES_OK;
                }
            }
            break;

        case MMC_GET_TYPE :     /* Get card type flags (1 byte) */
            *ptr = CardType;
            res = RES_OK;
            break;

        case MMC_GET_CSD :      /* Receive CSD as a data block (16 bytes) */
            if (send_cmd(CMD9, 0) == 0      /* READ_CSD */
                && rcvr_datablock(ptr, 16))
                res = RES_OK;
            break;

        case MMC_GET_CID :      /* Receive CID as a data block (16 bytes) */
            if (send_cmd(CMD10, 0) == 0     /* READ_CID */
                && rcvr_datablock(ptr, 16))
                res = RES_OK;
            break;

        case MMC_GET_OCR :      /* Receive OCR as an R3 resp (4 bytes) */
            if (send_cmd(CMD58, 0) == 0) {  /* READ_OCR */
                for (n = 4; n; n--) *ptr++ = rcvr_spi();
                res = RES_OK;
            }
            break;

        case MMC_GET_SDSTAT :   /* Receive SD status as a data block (64 bytes) */
            if (send_cmd(ACMD13, 0) == 0) { /* SD_STATUS */
                rcvr_spi();
                if (rcvr_datablock(ptr, 64))
                    res = RES_OK;
            }
            break;

        default:
            res = RES_PARERR;
        }

        release_spi();
    }

    return res;
}

DWORD get_fattime(void)
{
    return rtcGetTimeFat( &RTCD1 );
}



