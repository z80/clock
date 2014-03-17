
#include "dac.h"
#include "hal.h"
#include "stm32f10x.h"

#define RCC_APB1ENR_DACEN ((uint32_t)0x20000000)

void dacInit( void )
{
	palSetPadMode( GPIOA, 4, PAL_MODE_INPUT_ANALOG );
	palSetPadMode( GPIOA, 5, PAL_MODE_INPUT_ANALOG );
	RCC->APB1ENR |= RCC_APB1ENR_DACEN;
	//RCC->APB1RSTR |= RCC_APB1ENR_DACRST;
	//RCC->APB1RSTR &= ~(RCC_APB1ENR_DACRST);
	// Without trigger data should go to output immediately after writing.
	DAC->CR = DAC_CR_EN1 | DAC_CR_EN2;
}

void dacSet( DacCfg * dac )
{
	DAC->DHR12R1 = dac->dac1;
	DAC->DHR12R2 = dac->dac2;
}

