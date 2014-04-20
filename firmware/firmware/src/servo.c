
#include "servo.h"
#include "hal.h"

static PWMConfig pwmcfg = {
  24000000,                 // 24MHz PWM clock frequency.
  48000,                    // Initial PWM freq 24MHz/48kHz = 2ms.
  NULL/*pwmpcb*/,
  {
   { PWM_OUTPUT_DISABLED,    NULL },
   { PWM_OUTPUT_DISABLED,    NULL },
   { PWM_OUTPUT_ACTIVE_HIGH, NULL },
   { PWM_OUTPUT_DISABLED,    NULL }
  },
  0,
  0
};

#define HEIL_POWERUP_DELAY_MS 500
#define HEIL_HEIL_PORT       GPIOB
#define HEIL_HEIL_PAD        8
#define HEIL_PWR_60_PORT     GPIOC
#define HEIL_PWR_400_PORT    GPIOC
#define HEIL_PWR_60_PAD      11
#define HEIL_PWR_400_PAD     10
#define HEIL_HIGH_US         1500
#define HEIL_LOW_US          1000
#define HEIL_HIGH  (1000 * HEIL_HIGH_US / 1000)
#define HEIL_LOW   (1000 * HEIL_LOW_US / 1000)

void initServo( void )
{
    palSetPadMode( HEIL_HEIL_PORT,     HEIL_HEIL_PAD,    PAL_MODE_STM32_ALTERNATE_PUSHPULL );
    palSetPadMode( HEIL_PWR_60_PORT,   HEIL_PWR_60_PAD,  PAL_MODE_OUTPUT_PUSHPULL );
    palSetPadMode( HEIL_PWR_400_PORT,  HEIL_PWR_400_PAD, PAL_MODE_OUTPUT_PUSHPULL );
    palClearPad(   HEIL_PWR_60_PORT,   HEIL_PWR_60_PAD );
    palClearPad(   HEIL_PWR_400_PORT,  HEIL_PWR_400_PAD );
}

void heilUp( void )
{
    palSetPad( HEIL_PWR_60_PORT,   HEIL_PWR_60_PAD );
    chThdSleepMilliseconds( HEIL_POWERUP_DELAY_MS );
    palSetPad( HEIL_PWR_400_PORT,  HEIL_PWR_400_PAD );
    // Turn PWM on.
    pwmStart( &PWMD4, &pwmcfg );
    // Changes the PWM channel 0 to 0% duty cycle.
    pwmEnableChannel( &PWMD4, 2, PWM_PERCENTAGE_TO_WIDTH( &PWMD4, HEIL_LOW_US ) );

    // Stupid delay.
    chThdSleepMilliseconds( HEIL_POWERUP_DELAY_MS );

    pwmEnableChannel( &PWMD4, 2, PWM_PERCENTAGE_TO_WIDTH( &PWMD4, HEIL_HIGH_US ) );

    // Stupid delay.
    chThdSleepMilliseconds( HEIL_POWERUP_DELAY_MS );

    pwmEnableChannel( &PWMD4, 2, PWM_PERCENTAGE_TO_WIDTH( &PWMD4, 0 ) );
    pwmStop( &PWMD4 );
    palClearPad( HEIL_PWR_60_PORT,   HEIL_PWR_60_PAD );
    palClearPad( HEIL_PWR_400_PORT,  HEIL_PWR_400_PAD );
}

void heilDown( void )
{
    palSetPad( HEIL_PWR_60_PORT,   HEIL_PWR_60_PAD );
    chThdSleepMilliseconds( HEIL_POWERUP_DELAY_MS );
    palSetPad( HEIL_PWR_400_PORT,  HEIL_PWR_400_PAD );
    // Turn PWM on.
    pwmStart( &PWMD4, &pwmcfg );
    // Changes the PWM channel 0 to 0% duty cycle.
    pwmEnableChannel( &PWMD4, 2, PWM_PERCENTAGE_TO_WIDTH( &PWMD4, HEIL_HIGH_US ) );

    // Stupid delay.
    chThdSleepMilliseconds( HEIL_POWERUP_DELAY_MS );

    pwmEnableChannel( &PWMD4, 2, PWM_PERCENTAGE_TO_WIDTH( &PWMD4, HEIL_LOW_US ) );

    // Stupid delay.
    chThdSleepMilliseconds( HEIL_POWERUP_DELAY_MS );

    pwmEnableChannel( &PWMD4, 2, PWM_PERCENTAGE_TO_WIDTH( &PWMD4, 0 ) );
    pwmStop( &PWMD4 );
    palClearPad( HEIL_PWR_60_PORT,   HEIL_PWR_60_PAD );
    palClearPad( HEIL_PWR_400_PORT,  HEIL_PWR_400_PAD );
}





