
#include "trigger.h"
#include "hal.h"

static Mailbox  mailbox;
static msg_t    message;


static void extCb( EXTDriver *extp, expchannel_t channel );


static const EXTConfig extcfg =
{
  {
    { EXT_CH_MODE_DISABLED, NULL },
    { EXT_CH_MODE_DISABLED, NULL },
    { EXT_CH_MODE_DISABLED, NULL },
    { EXT_CH_MODE_DISABLED, NULL },
    { EXT_CH_MODE_DISABLED, NULL },
    { EXT_CH_MODE_RISING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOA, extCb },
    { EXT_CH_MODE_DISABLED, NULL },
    { EXT_CH_MODE_DISABLED, NULL },
    { EXT_CH_MODE_DISABLED, NULL },
    { EXT_CH_MODE_DISABLED, NULL },
    { EXT_CH_MODE_DISABLED, NULL },
    { EXT_CH_MODE_DISABLED, NULL },
    { EXT_CH_MODE_DISABLED, NULL },
    { EXT_CH_MODE_DISABLED, NULL },
    { EXT_CH_MODE_DISABLED, NULL }
  }
};

static void extCb( EXTDriver *extp, expchannel_t channel )
{
    (void)extp;
    (void)channel;
    chSysLockFromIsr();
        chMBPostI( &mailbox, 1 );
    chSysUnlockFromIsr();
}


void initTrigger( void )
{
    chMBInit( &mailbox, &message, 1 );
    palSetPadMode( GPIOB, 5, PAL_MODE_INPUT );
    // Initialize external interrupt on falling edge.
    extStart( &EXTD1, &extcfg );
}

void waitForTrigger( void )
{
    // Goto sleep mode.
    PWR->CR |= (PWR_CR_PDDS | PWR_CR_LPDS | PWR_CR_CSBF | PWR_CR_CWUF);
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    __WFI();

    // When recovered from sleep wait for a mail.
    msg_t msg;
    chMBFetch( &mailbox, &msg, TIME_INFINITE );
}










