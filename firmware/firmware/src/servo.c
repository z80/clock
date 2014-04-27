
#include "servo.h"
#include "hal.h"

static PWMConfig pwmcfg =
{
    24000000,                 // 24MHz PWM clock frequency.
    480000,                   // Initial PWM freq 24MHz/4.8kHz = 20ms.
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
#define HEIL_HEIL_PORT        GPIOB
#define HEIL_HEIL_PAD         8
#define HEIL_PWR_60_PORT      GPIOC
#define HEIL_PWR_60_PAD       11
#define HEIL_PWR_400_PORT     GPIOC
#define HEIL_PWR_400_PAD      10
// RPMs, accelerations, angles.
#define HEIL_V              300  // Percent_over_2 per second
#define HEIL_A              1200 // Percent_over_2 per second^2
// Timings.
#define HEIL_HIGH_US                      1300
#define HEIL_LOW_US                       1100
// Convertions.
#define HEIL_US_2_PERCENT_X_10( us )      ( 1000 * ((us) - HEIL_LOW_US) / (HEIL_HIGH_US-HEIL_LOW_US) )
#define HEIL_PERCENT_X_10_2_US( percent ) ( HEIL_LOW_US + (HEIL_HIGH_US-HEIL_LOW_US)*(percent)/1000 )
#define HEIL_PERCENT_2_PWM( percent )     ( 1000 * (HEIL_LOW_US + (HEIL_HIGH_US-HEIL_LOW_US)*(percent)/1000 ) / 20000 )


static void moveTo( int fromUs, int toUs );

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

    moveTo( HEIL_LOW_US, HEIL_HIGH_US );

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

    moveTo( HEIL_HIGH_US, HEIL_LOW_US );

    pwmEnableChannel( &PWMD4, 2, PWM_PERCENTAGE_TO_WIDTH( &PWMD4, 0 ) );
    pwmStop( &PWMD4 );
    palClearPad( HEIL_PWR_60_PORT,   HEIL_PWR_60_PAD );
    palClearPad( HEIL_PWR_400_PORT,  HEIL_PWR_400_PAD );
}

#define DABS( x ) ( (x >= 0) ? x : (-x) )

static void moveTo( int fromUs, int toUs )
{
    fromUs = HEIL_US_2_PERCENT_X_10( fromUs );
    toUs   = HEIL_US_2_PERCENT_X_10( toUs );
    uint32_t t0, t;
    t0 = rtcGetTimeFat( &RTCD1 );
    int x = fromUs;
    int dStart = (HEIL_V * HEIL_V) / ( 2 * HEIL_A ) + fromUs;
    int dStop  = toUs - dStart;
    if ( ( ( fromUs < toUs ) && ( dStop < dStart ) ) ||
         ( ( fromUs > toUs ) && ( dStop > dStart ) ) )
    {
        dStart = (toUs - fromUs) / 2;
        dStop  = dStart;
    }
    int pwm;
    while ( DABS( x - fromUs ) < DABS( dStart - fromUs ) )
    {
        t = rtcGetTimeFat( &RTCD1 ) - t0;
        x = fromUs + HEIL_A * t * t / (2000000);
        pwm = HEIL_PERCENT_2_PWM( x );
        pwmEnableChannel( &PWMD4, 2, PWM_PERCENTAGE_TO_WIDTH( &PWMD4, pwm ) );
        chThdSleepMilliseconds( 20 );
    }
    int x0 = x;
    t0 = t;
    while ( DABS( x - fromUs ) < DABS( dStop - fromUs ) )
    {
        t = rtcGetTimeFat( &RTCD1 ) - t0;
        x = x0 + HEIL_V * t / 1000;
        pwm = HEIL_PERCENT_2_PWM( x );
        pwmEnableChannel( &PWMD4, 2, PWM_PERCENTAGE_TO_WIDTH( &PWMD4, pwm ) );
        chThdSleepMilliseconds( 20 );
    }
    x0 = x;
    t0 = t;
    while ( DABS( x - fromUs ) < DABS( toUs - fromUs ) )
    {
        // x = -A*t^2 + b*t + c
        // dx/dt(V/A) = 0 = -2*A*V/A + b
        // b = 2*V
        // to = -A*(V/A)^2 + 2*V*V/A + c
        // to = V^2/A + c
        // c = to - V^2/A
        t = rtcGetTimeFat( &RTCD1 ) - t0;
        x = x0 - HEIL_A*t*t/1000000 + 2*HEIL_V*t/1000 + toUs - HEIL_V*HEIL_V/HEIL_A;
        pwm = HEIL_PERCENT_2_PWM( x );
        pwmEnableChannel( &PWMD4, 2, PWM_PERCENTAGE_TO_WIDTH( &PWMD4, pwm ) );
        chThdSleepMilliseconds( 20 );
    }
}












