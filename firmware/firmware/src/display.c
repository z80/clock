
#include "display.h"
#include "hal.h"
#include "lcd3310.h"

#define OPT_HOURLY    1
#define OPT_QUARTELY  2
#define OPT_ALARM     4
#define OPT_SOLUTING  8

static uint8_t     displayState    = OPT_QUARTELY | OPT_SOLUTING;
static DisplayMode displayMode     = ModeTime;
static int         displayMenuItem = 0;
static Mailbox     mailbox;
static msg_t       message;

static RTCTime  timespec;
static RTCAlarm alarmspec;


static const SPIConfig spicfg = {
  NULL,
  GPIOC,
  4,
  SPI_CR1_BR_2 | SPI_CR1_BR_1
};



static WORKING_AREA( displayWA, 128 );
static msg_t display_thd(void *arg);
static void rtcCallback( RTCDriver *rtcp, rtcevent_t event );
//rtcSetCallback(&RTCD1, my_cb);


static void  refresh( DisplayMode m );
static void  refreshI( DisplayMode m );
static void  time( int * h, int * m, int * s );
static void  numToStr( int v, char * str );
static void  timeStr( char * str );

// Display functions.
static void displayTime( void );
static void displayRootMenu( void );

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

    rtcSetCallback( &RTCD1, rtcCallback );
}

void setDisplayMode( DisplayMode m )
{
    displayMode = m;
}

static void  refresh( DisplayMode m )
{
    chMBPost( &mailbox, m, TIME_INFINITE );
}

static void  refreshI( DisplayMode m )
{
    chMBPostI( &mailbox, m );
}

static msg_t display_thd(void *arg)
{
  (void)arg;
  for ( ;; )
  {
    static msg_t msg;
    if ( chMBFetch( &mailbox, &msg, TIME_INFINITE ) == RDY_OK )
    {
        switch ( msg )
        {
        case ModeTime:
            if ( displayMode == ModeTime )
                displayTime();
            break;
        case ModeRootMenu:
            displayRootMenu();
            break;
        }
    }
  }
  return 0;
}

static void rtcCallback( RTCDriver *rtcp, rtcevent_t event )
{
    (void)rtcp;

    switch (event) {
    case RTC_EVENT_OVERFLOW:
      //palTogglePad(GPIOC, GPIOC_LED);
      break;
    case RTC_EVENT_SECOND:
      chMBPostI( &mailbox, ModeTime );
      break;
    case RTC_EVENT_ALARM:
      //palTogglePad(GPIOC, GPIOC_LED);
      //chSysLockFromIsr();
      //chBSemSignalI(&alarm_sem);
      //chSysUnlockFromIsr();
      break;
    }
}

static void  time( int * h, int * m, int * s )
{
    static RTCTime rtct;
    rtcGetTime( &RTCD1, &rtct );
    static int t;
    t = rtct.tv_sec;
    *h = t / 3600;
    *m = (t - *h * 3600 ) / 60;
    *s = (t - *h * 3600 - *m*60);
}

static void  numToStr( int v, char * stri )
{
    static int s;
    s = v / 10;
    stri[0] = s+'0';
    s = v - s * 10;
    stri[1] = s + '0';
    stri[2] = '\0';
}

static void  timeStr( char * stri )
{
    static int h, m, s;
    time( &h, &m, &s );
    numToStr( h, stri );
    stri[2] = ':';
    numToStr( m, &stri[3] );
    stri[5] = ':';
    numToStr( s, &stri[6] );
    stri[8] = '\0';
}


static uint8_t lines[4][12];

static void displayTime( void )
{
    char * stri = (char *)lines[1];
    timeStr( stri );

    lcd3310Clear( &SPID1 );
    lcd3310SetPosXY( &SPID1, 20, 3 );
    lcd3310WriteText( &SPID1, (const uint8_t *)lines[1] );
}

static void displayRootMenu( void )
{
}

void pushMenuI( void )
{
    switch ( displayMode )
}

void popMenuI( void )
{

}

void upMenuI( void )
{

}

void downMenuI( void )
{

}




















