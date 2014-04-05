
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
void pushMenuI( void );
void popMenuI( void );

// Up and down on the same level.
void upMenuI( void );
void downMenuI( void );


#endif








