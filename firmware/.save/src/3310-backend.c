
#include "3310-backend.h"
#include "hal.h"

#define SPI_3310  SPID1

#define PORT_CS   GPIOC
#define PAD_CS    4

#define PORT_DC   GPIOB
#define PAD_DC    0

#define PORT_RST  GPIOC
#define PAD_RST   5

#define PORT_SCK  GPIOA
#define PAD_SCK   5

#define PORT_MOSI GPIOA
#define PAD_MOSI  7

#define DC_LOW()    palClearPad( PORT_DC, PAD_DC )
#define DC_HIGH()   palSetPad(   PORT_DC, PAD_DC )
#define CS_LOW()    palClearPad( PORT_CS, PAD_CS )
#define CS_HIGH()   palSetPad(   PORT_CS, PAD_CS )
#define RST_LOW()   palClearPad( PORT_RST, PAD_RST )
#define RST_HIGH()  palSetPad(   PORT_RST, PAD_RST )

void delay3310( int cnt )
{
    volatile int i;
    for ( i=0; i<cnt; i++ )
        ;
}

//static SPIConfig spicfg = {NULL, GPIOB, 12, 0};
static SPIConfig spicfg = {NULL, GPIOB, 12,
                              SPI_CR1_BR_2 | SPI_CR1_BR_1};

void init3310( void )
{
    palSetPad( PORT_CS, PAD_CS ); // Set CS high
    palSetPadMode( PORT_SCK,  PAD_SCK,  PAL_MODE_STM32_ALTERNATE_PUSHPULL );    // SCK
    palSetPadMode( PORT_MOSI, PAD_MOSI, PAL_MODE_STM32_ALTERNATE_PUSHPULL );    // MISO
    palSetPadMode( PORT_CS,   PAD_CS,   PAL_MODE_OUTPUT_PUSHPULL );             // CS

    CS_HIGH();
    DC_HIGH();
    RST_HIGH();
    palSetPadMode( PORT_DC,  PAD_DC,  PAL_MODE_OUTPUT_PUSHPULL );
    palSetPadMode( PORT_CS,  PAD_CS,  PAL_MODE_OUTPUT_PUSHPULL );
    palSetPadMode( PORT_RST, PAD_RST, PAL_MODE_OUTPUT_PUSHPULL );

    delay3310( 100000 );
}

void finit3310( void )
{
    CS_HIGH();
    RST_HIGH();
    DC_HIGH();
    delay3310( 100000 );
}

/** * @brief Sends a byte through the SPI interface and return the byte
    * received from the SPI bus. 
    * @param byte : byte to send. 
    * @retval : The value of the received byte. 
    */ 
void sendByte3310( uint8_t byte )
{
    spiAcquireBus( &SPI_3310 );     // Acquire ownership of the bus.
    spiStart( &SPI_3310, &spicfg ); // Setup transfer parameters.
    spiSelect( &SPI_3310 );         // Slave Select assertion.
    spiStartSend( &SPI_3310, 1, &byte );
    //spiExchange( &SPI_3310, 512,
    //             txbuf, rxbuf );  // Atomic transfer operations.
    spiUnselect( &SPI_3310 );       // Slave Select de-assertion.
    spiReleaseBus( &SPI_3310 );     // Ownership release.
}

void sendArray3310( uint8_t * data, int cnt )
{
    spiAcquireBus( &SPID1 );     // Acquire ownership of the bus.
    spiStart( &SPID1, &spicfg ); // Setup transfer parameters.
    spiSelect( &SPID1 );         // Slave Select assertion.
    //spiExchange( &SPID1, 512,
    //            txbuf, rxbuf );  // Atomic transfer operations.
    spiStartSend( &SPI_3310, cnt, data );
    spiUnselect( &SPID1 );       // Slave Select de-assertion.
    spiReleaseBus( &SPID1 );     // Ownership release.
}

void setModeCmd3310( void )
{
    DC_LOW();
}

void setModeData3310( void )
{
    DC_HIGH();
}


void dcLow(void)
{
    DC_LOW();
}

void dcHigh(void)
{
    DC_HIGH();
}

void csLow(void)
{
    CS_LOW();
}

void csHigh(void)
{
    CS_HIGH();
}

void rstLow(void)
{
    RST_LOW();
}

void rstHigh(void)
{
    RST_HIGH();
}

void pwrLow(void)
{
}

void pwrHigh(void)
{
}











