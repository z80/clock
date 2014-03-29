
#ifndef __DISPLAY_H_
#define __DISPLAY_H_

#include "ch.h"

typedef enum
{
    Left,
    Cetnter,
    Right
} Align;

typedef enum
{
    ModeTime,
    ModeMenu
} DisplayMode;

void initDisplay( void );
void setDisplayMode( DisplayMode m );



#endif

