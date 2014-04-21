
#include "servo.h"
#include "hal.h"

static PWMConfig pwmcfg =
{
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
#define HEIL_HEIL_PORT        GPIOB
#define HEIL_HEIL_PAD         8
#define HEIL_PWR_60_PORT      GPIOC
#define HEIL_PWR_60_PAD       11
#define HEIL_PWR_400_PORT     GPIOC
#define HEIL_PWR_400_PAD      10
// RPMs, accelerations, angles.
#define HEIL_V              30000  // This is not the whole evolution but
#define HEIL_A              120000
// Timings.
#define HEIL_HIGH_US                      1500
#define HEIL_LOW_US                       1000
// Convertions.
#define HEIL_US_2_PERCENT_X_10( us )      ( 1000 * (us - HEIL_LOW_US) / 2000 )
#define HEIL_PERCENT_X_10_2_US( percent ) ( 2000 * percent / 1000 + HEIL_LOW_US )
#define HEIL_PWM( percent )               ( HEIL_US_2_PERCENT_X_10( (HEIL_HIGH_US - HEIL_LOW_US)*percent/1000 + HEIL_LOW_US ) )


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
    int x;
    int dStart = (HEIL_V * HEIL_V) / ( 2 * HEIL_A ) + fromUs;
    int dStop  = HEIL_US_2_PERCENT_X_10( toUs ) - dStart;
    if ( ( ( fromUs < toUs ) && ( dStop < dStart ) ) ||
         ( ( fromUs > toUs ) && ( dStop > dStart ) ) )
    {
        dStart = (toUs - fromUs) / 2;
        dStop  = dStart;
    }
    while ( DABS( x - fromUs ) < DABS( dStart - fromUs ) )
    {
        t = rtcGetTimeFat( &RTCD1 ) - t0;
        x = fromUs + HEIL_A * t * t / 2;
        pwmEnableChannel( &PWMD4, 2, PWM_PERCENTAGE_TO_WIDTH( &PWMD4, x ) );
        chThdSleepMilliseconds( 2 );
    }
    int x0 = x;
    t0 = t;
    while ( DABS( x - fromUs ) < DABS( dStop - fromUs ) )
    {
        t = rtcGetTimeFat( &RTCD1 ) - t0;
        x = x0 + HEIL_V * t;
        pwmEnableChannel( &PWMD4, 2, PWM_PERCENTAGE_TO_WIDTH( &PWMD4, x ) );
        chThdSleepMilliseconds( 2 );
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
        x = x0 - HEIL_A*t*t + 2*HEIL_V*t + toUs - HEIL_V*HEIL_V/HEIL_A;
        pwmEnableChannel( &PWMD4, 2, PWM_PERCENTAGE_TO_WIDTH( &PWMD4, x ) );
        chThdSleepMilliseconds( 2 );
    }
}












