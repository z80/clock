
#ifndef __DAC_H_
#define __DAC_H_

#include "ch.h"

typedef struct 
{
	uint16_t dac1;
	uint16_t dac2;
} DacCfg;

void dacInit( void );
void dacSet( DacCfg * dac );


#endif



