
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
    ModeTime = 0,
    ModeRootMenu,
    ModeTimeMenu,
    ModeAlarmMenu,
    ModeNotifyMenu
} DisplayMode;

void initDisplay( void );
void setDisplayMode( DisplayMode m );

// Sublevel or level up.
void pushMenu( void );
void popMenu( void );

// Up and down on the same level.
void upMenu( void );
void downMenu( void );


#endif








