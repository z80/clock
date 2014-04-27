
#include "servo.h"
#include "hal.h"

#define HEIL_CLOCK_FREQ                   1000000
#define HEIL_PERIOD_US                    20000

//CFREQ/1000000 = CLK/PERIOD_US
//CLK = CFREQ/1000000*PERIOD_US = 8000000/1000000*20000 = 160000
static PWMConfig pwmcfg =
{
    1000000,         // 8MHz PWM clock frequency.
    20000, // 20ms period
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

#define HEIL_POWERUP_DELAY_MS 20
#define HEIL_HEIL_PORT        GPIOB
#define HEIL_HEIL_PAD         8
#define HEIL_PWR_60_PORT      GPIOC
#define HEIL_PWR_60_PAD       11
#define HEIL_PWR_400_PORT     GPIOC
#define HEIL_PWR_400_PAD      10
// RPMs, accelerations, angles.
#define HEIL_V              3000  // Percent_x_10 per second
#define HEIL_A              6000  // Percent_x_10 per second^2
// Timings.
#define HEIL_HIGH_US                      1700
#define HEIL_LOW_US                       1250
// Convertions.
#define HEIL_US_2_PERCENT_X_10( us )      ( 1000 * ((us) - HEIL_LOW_US) / (HEIL_HIGH_US-HEIL_LOW_US) )
#define HEIL_PERCENT_X_10_2_US( percent ) ( HEIL_LOW_US + (HEIL_HIGH_US-HEIL_LOW_US)*(percent)/1000 )
#define HEIL_PERCENT_2_US( percent )      ( HEIL_LOW_US + (HEIL_HIGH_US-HEIL_LOW_US)*(percent)/1000 )


static void moveTo( int fromUs, int toUs );

void initServo( void )
{
    palClearPad(   HEIL_HEIL_PORT,     HEIL_HEIL_PAD );
    palClearPad(   HEIL_PWR_60_PORT,   HEIL_PWR_60_PAD );
    palClearPad(   HEIL_PWR_400_PORT,  HEIL_PWR_400_PAD );
    palSetPadMode( HEIL_HEIL_PORT,     HEIL_HEIL_PAD,    PAL_MODE_STM32_ALTERNATE_PUSHPULL );
    palSetPadMode( HEIL_PWR_60_PORT,   HEIL_PWR_60_PAD,  PAL_MODE_OUTPUT_PUSHPULL );
    palSetPadMode( HEIL_PWR_400_PORT,  HEIL_PWR_400_PAD, PAL_MODE_OUTPUT_PUSHPULL );
}

void heilUp( void )
{
    palSetPad( HEIL_PWR_60_PORT,   HEIL_PWR_60_PAD );
    chThdSleepMilliseconds( HEIL_POWERUP_DELAY_MS );
    palSetPad( HEIL_PWR_400_PORT,  HEIL_PWR_400_PAD );
    // Turn PWM on.
    pwmStart( &PWMD4, &pwmcfg );

    moveTo( HEIL_LOW_US, HEIL_HIGH_US );

    pwmDisableChannel( &PWMD4, 2 );
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

    pwmDisableChannel( &PWMD4, 2 );
    pwmStop( &PWMD4 );
    palClearPad( HEIL_PWR_60_PORT,   HEIL_PWR_60_PAD );
    palClearPad( HEIL_PWR_400_PORT,  HEIL_PWR_400_PAD );
}

#define DABS( x ) ( ((x) >= 0) ? (x) : (-(x)) )

static void moveTo( int fromUs, int toUs )
{
    fromUs = HEIL_US_2_PERCENT_X_10( fromUs );
    toUs   = HEIL_US_2_PERCENT_X_10( toUs );
    uint32_t t0, t;
    RTCTime  rtc;
    rtcGetTime( &RTCD1, &rtc );
    t0 = rtc.tv_sec * 1000 + rtc.tv_msec;
    int x = fromUs;
    int dStart, dStop;
    int d = (HEIL_V * HEIL_V) / ( 2 * HEIL_A );
    if ( 2*d < DABS(fromUs - toUs) )
    {
        if ( fromUs < toUs )
        {
            dStart = fromUs + d;
            dStop  = toUs - d;
        }
        else
        {
            dStart = fromUs - d;
            dStop  = toUs + d;

        }
    }
    else
    {
        dStart = (toUs - fromUs) / 2;
        dStop  = dStart;
    }
    int pwm;
    while ( DABS( x - fromUs ) < DABS( dStart - fromUs ) )
    {
        rtcGetTime( &RTCD1, &rtc );
        t = rtc.tv_sec * 1000 + rtc.tv_msec - t0;
        if ( toUs > fromUs )
            x = fromUs + HEIL_A * t * t / (2000000);
        else
            x = fromUs - HEIL_A * t * t / (2000000);
        pwm = HEIL_PERCENT_2_US( x );
        pwmEnableChannel( &PWMD4, 2, PWM_FRACTION_TO_WIDTH( &PWMD4, HEIL_PERIOD_US, pwm ) );
        chThdSleepMilliseconds( 20 );
    }
    int x0 = x;
    rtcGetTime( &RTCD1, &rtc );
    t0 = rtc.tv_sec * 1000 + rtc.tv_msec;
                    dStop = toUs;
    while ( DABS( x - fromUs ) < DABS( dStop - fromUs ) )
    {
        rtcGetTime( &RTCD1, &rtc );
        t = rtc.tv_sec * 1000 + rtc.tv_msec - t0;
        if ( toUs > fromUs )
            x = x0 + HEIL_V * t / 1000;
        else
            x = x0 - HEIL_V * t / 1000;
        pwm = HEIL_PERCENT_2_US( x );
        pwmEnableChannel( &PWMD4, 2, PWM_FRACTION_TO_WIDTH( &PWMD4, HEIL_PERIOD_US, pwm ) );
        chThdSleepMilliseconds( 20 );
    }
    /*x0 = x;
    rtcGetTime( &RTCD1, &rtc );
    t0 = rtc.tv_sec * 1000 + rtc.tv_msec;
    while ( DABS( x - fromUs ) < DABS( toUs - fromUs ) )
    {
        // x = -A*t^2 + b*t + c
        // dx/dt(V/A) = 0 = -2*A*V/A + b
        // b = 2*V
        // to = -A*(V/A)^2 + 2*V*V/A + c
        // to = V^2/A + c
        // c = to - V^2/A
        rtcGetTime( &RTCD1, &rtc );
        t = rtc.tv_sec * 1000 + rtc.tv_msec - t0;
        if ( toUs > fromUs )
            x = x0 - HEIL_A*t*t/1000000 + 2*HEIL_V*t/1000 + toUs - HEIL_V*HEIL_V/HEIL_A;
        else
            x = x0 + HEIL_A*t*t/1000000 - 2*HEIL_V*t/1000 - toUs + HEIL_V*HEIL_V/HEIL_A;
        pwm = HEIL_PERCENT_2_US( x );
        pwmEnableChannel( &PWMD4, 2, PWM_FRACTION_TO_WIDTH( &PWMD4, HEIL_PERIOD_US, pwm ) );
        chThdSleepMilliseconds( 20 );
    }*/
}












