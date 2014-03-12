
#include "playback.h"
#include "hal.h"
#include "ff.h"

#define FILE_BUF_SZ 512
uint8_t sndBuf[ 2*FILE_BUF_SZ ];
int     sndIndex = 0;
int     sndStop  = 0;

static void pwmc1cb( PWMDriver * pwmp );
static int  playbackStart( void );
static void playbackStop( void );

static PWMConfig pwmcfg = {
  24000000,                               // 24MHz PWM clock frequency.
  1000,                                   // Initial PWM freq 24MHz/1000 = 24kHz.
  pwmpcb,
  {
   {PWM_OUTPUT_ACTIVE_HIGH, pwmc1cb},
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL}
  },
  0,
  0
};

static int is_playing = 0;



// **************************************************************************************
//                              FAT FS configuration.
// **************************************************************************************
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
static SPIConfig hs_spicfg = {NULL, GPIOB, 12, 0};

/* Low speed SPI configuration (281.250kHz, CPHA=0, CPOL=0, MSb first).*/
static SPIConfig ls_spicfg = {NULL, GPIOB, 12,
                              SPI_CR1_BR_2 | SPI_CR1_BR_1};

/* MMC/SD over SPI driver configuration.*/
static MMCConfig mmccfg = {&SPID2, &ls_spicfg, &hs_spicfg};



// **************************************************************************************
//                              Synchronization.
// **************************************************************************************
Mailbox  mailbox;
msg_t    message;


void playbackInit( void )
{
    chMBInit( &mailbox, &message, 1 );
}

static int playbackStart( void )
{
    // Access to SD flash.
    mmcObjectInit( &MMCD1 );
    mmcStart( &MMCD1, &mmccfg );
    if ( !mmcConnect( &MMCD1 ) )
    {
        FRESULT err = f_mount(0, &MMC_FS);
        if (err != FR_OK)
        {
            return 1;
        }
    }

    // Turn PWM on.
    pwmStart( &PWMD1, &pwmcfg );
    palSetPadMode( GPIOA, 8, PAL_MODE_ALTERNATE(2) );

    // Changes the PWM channel 0 to 50% duty cycle.
    pwmEnableChannel( &PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH( &PWMD1, 5000 ) );

    return 0;
}

static void playbackStop( void )
{
    f_mount(0, NULL);
    if ( mmcDisconnect( &MMCD1 ) )
        sysHalt();
    // Stop PWM driver.
    pwmStop( &PWMD1 );
}

int  play( char * file )
{
    is_playing = 1;
    if ( playbackStart() )
        return 1;

    FIL file;
    //FRESULT err = f_open( &file, "0:tmstmp.tst", FA_WRITE | FA_OPEN_ALWAYS);
    FRESULT err = f_open( &file, file, FA_READ );
    if ( err == F_OK )
    {
        sndIndex = 0;
        // Here sequential buffer read with sending flags from timer interrupt.
        size_t bytesRead;
        for ( bytesRead=0; bytesRead<FILE_BUF_SZ*2; bytesRead++ )
            sndBuf[bytesRead] = 0;
        err =  f_read( &file, sndBuf, FILE_BUF_SZ, &bytesRead );
        if ( bytesRead > 0 )
            err =  f_read( &file, &sndBuf[FILE_BUF_SZ], FILE_BUF_SZ, &bytesRead );
        while ( bytesRead > 0 )
        {
            msg_t msg;
            if ( chMBFetch( &mailbox, &msg, 0 ) == RDY_OK )
            {
                if ( second == 0 )
                    err =  f_read( &file, sndBuf, FILE_BUF_SZ, &bytesRead );
                else
                    f_read( &file, &sndBuf[FILE_BUF_SZ], FILE_BUF_SZ, &bytesRead );
            }
        }
        is_playing = 0;
    }
    else
        return 2;

    playbackStop();
    return 0;
}

int isPlaying( void )
{
    is_playing = 0;
}

static void pwmc1cb( PWMDriver * pwmp )
{
    int value = (int)sndBuf[ sndIndex++ ];
    value = value * 10000 / 255;
    pwmEnableChannel( &PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH( &PWMD1, value ) );
    if ( sndIndex == FILE_BUF_SZ )
    {
        if ( is_playing == 0 )
            pwmDisableChannelI( &PWMD1, 0 )
        else
            chMBPostI( &mailbox, 0 );
    }
    else if ( sndIndex == 2*FILE_BUF_SZ )
    {
            if ( is_playing == 0 )
                pwmDisableChannelI( &PWMD1, 0 )
            else
                chMBPostI( &mailbox, 1 );
    }
}











