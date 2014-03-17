
#ifndef __3310_H_
#define __3310_H_

#include "3310-backend.h"

#define NULL                       0
#define BFALSE                     0
#define BTRUE                      1

#define LCD_X_RES                  84
#define LCD_Y_RES                  48
#define LCD_CACHE_SIZE             ((LCD_X_RES * LCD_Y_RES) / 8)

typedef enum
{
    LCD_CMD  = 0,
    LCD_DATA = 1

} LcdCmdData;

typedef enum
{
    PIXEL_OFF =  0,
    PIXEL_ON  =  1,
    PIXEL_XOR =  2

} LcdPixelMode;

typedef enum
{
    FONT_1X = 1,
    FONT_2X = 2

} LcdFontSize;

void lcdInit       ( void );
void lcdPower 	   ( uint8_t en );
void lcdContrast   ( uint8_t contrast );
void lcdClear      ( void );
void lcdUpdate     ( void );
void lcdGotoXy     ( uint8_t x, uint8_t y );
void lcdChr        ( LcdFontSize size, uint8_t ch );
void lcdStr        ( LcdFontSize size, uint8_t * dataPtr );
void lcdStrConst   ( LcdFontSize size, const char * dataPtr );
void lcdPixel      ( uint8_t x, uint8_t y, LcdPixelMode mode );
void lcdLine       ( uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, LcdPixelMode mode );

typedef enum
{
    THorizontal = 0,
    TVertical
} LcdAddressing;

typedef enum
{
	TBasic = 0,
	TExtended
} TInstructionSet;

typedef enum
{
	TBlank = 0,
	TNormalMode,
	TAllOn,
	TInverseMode
} TDisplayCtrl;

typedef enum
{
   TTemp0 = 0,
   TTemp1,
   TTemp2,
   TTemp3
}  TTempCoef;

// Display tuning routines.
void lcdFuncSet( uint8_t powerDown,
		         LcdAddressing addr,
		         TInstructionSet instSet );
void lcdDispCtrl( TDisplayCtrl ctrl );
void lcdYAddr( uint8_t y );
void lcdXAddr( uint8_t x );
void lcdTempCtrl( TTempCoef t );
void lcdBias( uint8_t bias );
void lcdVop( uint8_t vop ); // Contrast

#endif



