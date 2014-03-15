
#include "3310-backend.h"
#include "hal.h"

#define SPI_3310  SPID1

#define PORT_CS   GPIOC
#define PIN_CS    4

#define PORT_DC   GPIOB
#define PIN_DC    0

#define PORT_RST  GPIOC
#define PIN_RST   5

#define PORT_SCK  GPIOA
#define PIN_SCK   5

#define PORT_MOSI GPIOA
#define PIN_MOSI  7

#define DC_LOW()    palResetPad( PORT_DC, PIN_DC )
#define DC_HIGH()   palSetPad(   PORT_DC, PIN_DC )
#define CS_LOW()    palResetPad( PORT_CS, PIN_CS )
#define CS_HIGH()   palSetPad(   PORT_CS, PIN_CS )
#define RST_LOW()   palResetPad( PORT_RST, PIN_RST )
#define RST_HIGH()  palSetPad(   PORT_RST, PIN_RST )

BYTE g_dmaStarted = 0;

void delay3310( int cnt )
{
    volatile int i;
    for ( i=0; i<cnt; i++ )
        ;
}

static void initDma( BYTE * mem, int cnt );

//static SPIConfig spicfg = {NULL, GPIOB, 12, 0};
static SPIConfig spicfg = {NULL, GPIOB, 12,
                              SPI_CR1_BR_2 | SPI_CR1_BR_1};

void init3310( BYTE * data, int cnt )
{
    CS_HIGH();
    DC_HIGH();
    RST_HIGH();

    GPIO_InitTypeDef GPIO_InitStructure;
    
    // Enable SPI1 and GPIO clocks */
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI1,  ENABLE );
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE );
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );
    
    GPIO_InitStructure.GPIO_Pin = PIN_CS | PIN_DC | PIN_RST;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init( PORT_CS, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = PIN_PWR;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init( PORT_PWR, &GPIO_InitStructure);
 
    /* Configure SPI1 pins: SCK and MOSI only to their Alternative (SPI) function */
    GPIO_InitStructure.GPIO_Pin = PIN_SCK | PIN_MOSI;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( PORT_CS, &GPIO_InitStructure );
 
    // To prevent powering from data lines.
    RST_HIGH();
    CS_HIGH();
    DC_HIGH();
    // Turn power on.
    PWR_HIGH();
    delay3310( 100000 );


    /* Enable SPI1 */
    SPI_Cmd( SPI_3310, ENABLE );
    //SPI_SSOutputCmd( SPI_3310, ENABLE );

    //initDma( data, cnt );
}

void finit3310( void )
{
    CS_HIGH();
    RST_HIGH();
    DC_HIGH();
    SPI_Cmd( SPI_3310, DISABLE );
    delay3310( 100000 );
}

/** * @brief Sends a byte through the SPI interface and return the byte
    * received from the SPI bus. 
    * @param byte : byte to send. 
    * @retval : The value of the received byte. 
    */ 
void sendByte3310( uint8_t byte )
{
    //SPI_I2S_ReceiveData( SPI_3310 );
    /* Loop while DR register in not emplty */
    while ( SPI_I2S_GetFlagStatus( SPI_3310, SPI_I2S_FLAG_BSY ) == SET )
        ;
    /* Send byte through the SPI_3310 peripheral */
    SPI_I2S_SendData( SPI_3310, byte );
    /* Wait to receive a byte */
    //while ( SPI_I2S_GetFlagStatus( SPI_3310, SPI_I2S_FLAG_RXNE ) == RESET )
    //    ;
    /* Return the byte read from the SPI bus */
    //return SPI_I2S_ReceiveData( SPI_3310 );
}

void sendArray3310( uint8_t * data, int cnt )
{
    spiAcquireBus( &SPID1 );              /* Acquire ownership of the bus.    */
    spiStart( &SPID1, &spicfg );       /* Setup transfer parameters.       */
    spiSelect( &SPID1 );                  /* Slave Select assertion.          */
    spiExchange( &SPID1, 512,
                txbuf, rxbuf );          /* Atomic transfer operations.      */
    spiUnselect( &SPID1 );                /* Slave Select de-assertion.       */
    spiReleaseBus( &SPID1 );              /* Ownership release.               */

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









