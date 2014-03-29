
#include "playback.h"
#include "hal.h"
#include "ff.h"

#define FILE_BUF_SZ 512
uint8_t sndBuf[ 2*FILE_BUF_SZ ];
int     sndIndex = 0;
int     sndStop  = 0;

static void pwmpcb( PWMDriver * pwmp );
static int  playbackStart( void );
static void playbackStop( void );

static PWMConfig pwmcfg = {
  24000000,                               // 24MHz PWM clock frequency.
  1000,                                   // Initial PWM freq 24MHz/1000 = 24kHz.
  pwmpcb,
  {
   {PWM_OUTPUT_ACTIVE_HIGH, NULL},
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL}
  },
  0,
  0
};

static int is_playing = 0;
static int do_exit_pwm = 0;


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
//static bool_t fs_ready = FALSE;

/* Maximum speed SPI configuration (18MHz, CPHA=0, CPOL=0, MSb first).*/
static SPIConfig hs_spicfg = { NULL, GPIOB, 12, 0 };

/* Low speed SPI configuration (281.250kHz, CPHA=0, CPOL=0, MSb first).*/
static SPIConfig ls_spicfg = { NULL, GPIOB, 12,
                               SPI_CR1_BR_2 | SPI_CR1_BR_1 };

/* MMC/SD over SPI driver configuration.*/
static MMCConfig mmccfg = { &SPID2, &ls_spicfg, &hs_spicfg };



// **************************************************************************************
//                              Synchronization.
// **************************************************************************************
static Mailbox  mailbox;
static msg_t    message;


void playbackInit( void )
{
    chMBInit( &mailbox, &message, 1 );
    // SPI setup.
    palSetPadMode( GPIOB, 13, PAL_MODE_STM32_ALTERNATE_PUSHPULL );     // SCK
    palSetPadMode( GPIOB, 14, PAL_MODE_STM32_ALTERNATE_PUSHPULL );     // MISO
    palSetPadMode( GPIOB, 15, PAL_MODE_STM32_ALTERNATE_PUSHPULL );     // MOSI
    palSetPadMode( GPIOB, 12, PAL_MODE_OUTPUT_PUSHPULL );              // CS
    palSetPadMode( GPIOC,  9, PAL_MODE_OUTPUT_PUSHPULL );              // Amp enable.
    palSetPad( GPIOB, 12 ); // Set CS high.
    palClearPad( GPIOC,  9 ); // Clear Amp. enable.

    // PWM pad.
    palSetPadMode( GPIOA, 8, PAL_MODE_STM32_ALTERNATE_PUSHPULL );

    mmcObjectInit( &MMCD1 );
}

static int playbackStart( void )
{
    // Access to SD flash.
    mmcStart( &MMCD1, &mmccfg );
    FRESULT err = f_mount( 0, &MMC_FS );
    if ( err != FR_OK )
        return 1;
    palSetPad( GPIOC,  9 ); // Set Amp. enable.
    return 0;
}

static void playbackStop( void )
{
    f_mount(0, NULL);
    if ( mmcDisconnect( &MMCD1 ) )
    {
        //sysHalt();
        ;
    }
    // Stop PWM driver.
    pwmStop( &PWMD1 );
    palClearPad( GPIOC,  9 ); // Clear Amp. enable.
}

int  play( char * file )
{
    is_playing = 1;
    do_exit_pwm = 0;
    if ( playbackStart() )
        return 1;

    FIL f;
    //FRESULT err = f_open( &file, "0:tmstmp.tst", FA_WRITE | FA_OPEN_ALWAYS);
    FRESULT err = f_open( &f, file, FA_READ );
    if ( err == FR_OK )
    {
        sndIndex = 0;
        // Here sequential buffer read with sending flags from timer interrupt.
        size_t bytesRead;
        // Initialize buffer with zeros.
        for ( bytesRead=0; bytesRead<FILE_BUF_SZ*2; bytesRead++ )
            sndBuf[bytesRead] = 0;
        // Read initial buffer values.
        err =  f_read( &f, sndBuf, FILE_BUF_SZ, &bytesRead );
        if ( bytesRead > 0 )
            err =  f_read( &f, &sndBuf[FILE_BUF_SZ], FILE_BUF_SZ, &bytesRead );

        // Turn PWM on.
        pwmStart( &PWMD1, &pwmcfg );
        // Changes the PWM channel 0 to 0% duty cycle.
        pwmEnableChannel( &PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH( &PWMD1, 0 ) );

        // Loop over buffer while receiving messages from PWM.
        while ( bytesRead > 0 )
        {
            msg_t msg;
            if ( chMBFetch( &mailbox, &msg, TIME_INFINITE ) == RDY_OK )
            {
                if ( msg == 0 )
                    err = f_read( &f, sndBuf, FILE_BUF_SZ, &bytesRead );
                else
                    err = f_read( &f, &sndBuf[FILE_BUF_SZ], FILE_BUF_SZ, &bytesRead );
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
    return is_playing;
}

static void pwmpcb( PWMDriver * pwmp )
{
    (void)pwmp;
    int value = (int)sndBuf[ sndIndex++ ];
    //value = value * 10000 / 255; // 8 bit sound.
    chSysLockFromIsr();

    pwmEnableChannel( &PWMD1, 0, PWM_FRACTION_TO_WIDTH( &PWMD1, 255, value ) );
    if ( sndIndex == FILE_BUF_SZ )
    {
        if ( do_exit_pwm )
        {
            pwmDisableChannelI( &PWMD1, 0 );
        }
        if ( is_playing == 0 )
            do_exit_pwm = 1;
        else
        {
            chMBPostI( &mailbox, 0 );
        }
    }
    else if ( sndIndex == 2*FILE_BUF_SZ )
    {
        if ( do_exit_pwm )
        {
            pwmDisableChannelI( &PWMD1, 0 );
        }
        if ( is_playing == 0 )
            do_exit_pwm = 1;
        else
        {
            chMBPostI( &mailbox, 1 );
        }
        sndIndex = 0;
    }
    chSysUnlockFromIsr();
}



#if HAL_USE_MMC_SPI
    /* Board-related functions related to the MMC_SPI driver.*/
    bool_t mmc_lld_is_card_inserted(MMCDriver *mmcp) {

        (void)mmcp;
        return TRUE;
    }

    bool_t mmc_lld_is_write_protected(MMCDriver *mmcp) {

        (void)mmcp;
        return TRUE;
    }
#endif











