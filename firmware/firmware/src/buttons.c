
#include "buttons.h"
#include "display.h"
#include "hal.h"



typedef enum
{
    BtnNone = 8,
    BtnMenuPush = 0,
    BtnMenuPop  = 1,
    BtnMenuUp   = 2,
    BtnMenuDown = 3,
    BtnTrigger  = 4
} Button;

// Delay in button press handling.
#define BUTTON_DELAY 30
static int lastButtonTime = 0;
static Button lastButton     = BtnNone;
static VirtualTimer vtimer;

static void extCb( EXTDriver *extp, expchannel_t channel );
static void timeout( void * arg );


static const EXTConfig extcfg =
{
  {
    { EXT_CH_MODE_DISABLED, NULL },
    { EXT_CH_MODE_DISABLED, NULL },
    { EXT_CH_MODE_DISABLED, NULL },
    { EXT_CH_MODE_DISABLED, NULL },
    { EXT_CH_MODE_DISABLED, NULL },
    { EXT_CH_MODE_FALLING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOA, extCb },
    { EXT_CH_MODE_FALLING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOA, extCb },
    { EXT_CH_MODE_FALLING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOA, extCb },
    { EXT_CH_MODE_DISABLED, NULL },
    { EXT_CH_MODE_DISABLED, NULL },
    { EXT_CH_MODE_FALLING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOC, extCb },
    { EXT_CH_MODE_FALLING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOC, extCb },
    { EXT_CH_MODE_DISABLED, NULL },
    { EXT_CH_MODE_DISABLED, NULL },
    { EXT_CH_MODE_DISABLED, NULL }
  }
};


void initButtons( void )
{
    palSetPadMode( GPIOB, 5, PAL_MODE_INPUT );
    palSetPadMode( GPIOB, 6, PAL_MODE_INPUT );
    palSetPadMode( GPIOB, 7, PAL_MODE_INPUT );
    // Initialize external interrupt on falling edge.
    extStart( &EXTD1, &extcfg );
}

static void extCb( EXTDriver *extp, expchannel_t channel )
{
    (void)extp;
    (void)channel;
    chSysLockFromIsr();
    if ( !chVTIsArmedI(&vtimer) )
      chVTSetI( &vtimer, MS2ST( BUTTON_DELAY ), timeout, NULL );
    chSysUnlockFromIsr();
}

static void timeout( void * arg )
{
    switch ( lastButton )
    {
    case BtnMenuPush:
        break;
    case BtnMenuPop:
        break;
    case BtnMenuUp:
        break;
    case BtnMenuDown:
        break;
    case BtnTrigger:
        break;
    }
    // ResetButtonState.
    chSysLockFromIsr();
        lastButton = 0;
    chSysUnlockFromIsr();
}









