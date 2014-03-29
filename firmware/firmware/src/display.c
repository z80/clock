
#include "display.h"
#include "hal.h"

static DisplayMode mode = ModeTime;
static Mailbox  mailbox;
static msg_t    message;



static WORKING_AREA( displayWA, 128 );
static msg_t display_thd(void *arg);
static void  refresh( void );
static void  refreshI( void );
static void  time( int * h, int * m, int * s );
static void  numToStr( int v, char * str );
static void  timeStr( char * str );

void initDisplay( void )
{
    palSetPadMode( GPIOA, 5, PAL_MODE_STM32_ALTERNATE_PUSHPULL );
    palSetPadMode( GPIOA, 7, PAL_MODE_STM32_ALTERNATE_PUSHPULL );
    palSetPadMode( GPIOC, 4, PAL_MODE_OUTPUT_PUSHPULL );
    palSetPadMode( GPIOC, 5, PAL_MODE_OUTPUT_PUSHPULL );
    palSetPadMode( GPIOB, 0, PAL_MODE_OUTPUT_PUSHPULL );
    palSetPad( GPIOC, 4 ); // CS
    palSetPad( GPIOC, 5 ); // RST

    chMBInit( &mailbox, &message, 1 );

    spiStart(&SPID1, &spicfg );       // Setup transfer parameters.
    lcd3310Init( &SPID1 );

    chThdCreateStatic( displayWA, sizeof(displayWA), NORMALPRIO, display_thd, NULL );
}

void setDisplayMode( DisplayMode m )
{
    mode = m;
}

static void  refresh( void )
{
    chMBPost( &mailbox, mode );
}

static void  refreshI( void )
{
    chMBPostI( &mailbox, mode );
}

static msg_t display_thd(void *arg)
{
  (void)arg;
  while (TRUE){
    chThdSleepMilliseconds(100);

    if ( chMBFetch( &mailbox, &msg, TIME_INFINITE ) == RDY_OK )
        mode = msg;

    lcd3310Clear( &SPID1 );
    lcd3310SetPosXY( &SPID1, 5, 3 );
    lcd3310WriteText( &SPID1, (const uint8_t *)"Hello!" );



  }
  return 0;
}

static void  time( int * h, int * m, int * s );
{
    static RTCTime rtct;
    rtcGetTime( &RTCD1, &t );
    static int t;
    t = rtct.tv_sec;
    *h = t / 3600;
    *m = (t - *h * 3600 ) / 60;
    *s = (t - *h * 3600 - *m*60);
}

static void  numToStr( int v, char * str )
{
    static s;
    s = v / 10;
    stri[0] = s+'0';
    s = v - s * 10;
    stri[1] = s + '0';
    stri[2] = '\0';
}

static void  timeStr( char * str )
{
    static int h, m, s;
    time( &h, &m, &s );

}





