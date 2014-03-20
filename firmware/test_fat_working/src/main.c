/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Version            : V3.3.0
* Date               : 21-March-2011
* Description        : Mass Storage demo main file
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#ifdef STM32L1XX_MD
 #include "stm32l1xx.h"
#else
 #include "stm32f10x.h"
#endif /* STM32L1XX_MD */
 
#include "ff.h"

#define FLASH_SECTOR_SIZE 512
#define FIRMWARE_START_ADDRESS 0x8000000
#define DFU_FILE_NAME          "anthem01.raw"
#define DFU_REPORT_NAME        "dfu_report.txt"
#define NULL                   0

static void    dfu( void );
static void    reportFailure( const char * stri );

typedef void (*pFunction)(void);
pFunction Jump_To_Application;
uint32_t JumpAddress;

uint8_t do_report_failure = 0;

int main(void)
{
    dfu();
    while (1)
    {
    }
}

#ifdef USE_FULL_ASSERT
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/


static void    dfu( void )
{
    FRESULT rc;
    FATFS fatfs;
    FIL   fil;
    // FILINFO info;
    UINT br;
    // UINT bw, i;
    rc = f_mount( 0, &fatfs );
    if ( rc != FR_OK )
        return;

    rc = f_open( &fil, DFU_FILE_NAME, FA_READ );
    if ( rc != FR_OK )
    {
        reportFailure( _T( "No firmware.bin file to reflash the MCU\n" ) );
        return;
    }

    uint8_t buffer[ FLASH_SECTOR_SIZE ];
    // First compare flash content with file content.
    // To check if reflash is really necessary.
    do {
        rc = f_read( &fil, buffer, sizeof(buffer), &br );
        if ( rc != FR_OK )
            return;
    } while ( br > 0 );
    f_close( &fil );
    f_close( &fil );
 
    f_mount( 0, 0 );
}

static void    reportFailure( const char * stri )
{
    FRESULT rc;
    FATFS fatfs;
    FIL   fil;
    // FILINFO info;
    // UINT br;
    // UINT bw;
 
    rc = f_mount( 0, &fatfs );
    if ( rc != FR_OK )
        goto report_end;


report_end:
    f_mount( 0, NULL );
}


DWORD get_fattime (void)
{
    return ((DWORD)(2010 - 1980) << 25)	/* Fixed to Jan. 1, 2010 */
           | ((DWORD)1 << 21)
           | ((DWORD)1 << 16)
           | ((DWORD)0 << 11)
           | ((DWORD)0 << 5)
           | ((DWORD)0 >> 1);
}


