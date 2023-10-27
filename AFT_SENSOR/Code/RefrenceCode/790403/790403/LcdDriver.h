/************************************************************

Filename : lcddrv.h

Description: LCD driver header file defining external
             function and structure prototypes.
Purpose: This file is to be used in conjuction with 
         the LCD low level driver.  File should be 
         included in and compiled with the LCD application
         program
*************************************************************/

#include "customTypedef.h"
#include "r_cg_userdefine.h"




#define NO		0u
#define YES		1u


#define HOUR_METER_SYMBOL_ON			SEG0  |= 0b00000001;
#define HOUR_METER_SYMBOL_OFF			SEG0 &=0xFE;

#define DECIMAL_DOT_SYMBOL_ON			SEG_1_DOT_SYSMOL;
#define DECIMAL_DOT_SYMBOL_OFF			SEG15 &=0xFE;


#define SEG_1A		SEG24  |= 0b00001000;
#define SEG_1B		SEG24  |= 0b00000100;
#define SEG_1C		SEG24  |= 0b00000010;
#define SEG_1D		SEG24  |= 0b00000001;
#define SEG_1F		SEG15  |= 0b00001000;			/* Hour Symbol */
#define SEG_1G		SEG15  |= 0b00000100;
#define SEG_1E		SEG15  |= 0b00000010;
#define SEG_1_DOT_SYSMOL		SEG15  |= 0b00000001;
#define CLEAR_1		SEG24 &=0XF0; SEG15 &=0XF1; 


#define SEG_2A		SEG14  |= 0b00001000;
#define SEG_2B		SEG14  |= 0b00000100;
#define SEG_2C		SEG14  |= 0b00000010;
#define SEG_2D		SEG14  |= 0b00000001;
#define SEG_2F		SEG9  |= 0b00001000; 			/* BATT. Symbol */
#define SEG_2G		SEG9  |= 0b00000100;
#define SEG_2E		SEG9  |= 0b00000010;
#define SEG_2_BLANK		SEG9  |= 0b00000001;
#define CLEAR_2		SEG9 &=0XF1; SEG14 &=0XF0; 


#define SEG_3A		SEG7  |= 0b00001000;
#define SEG_3B		SEG7  |= 0b00000100;
#define SEG_3C 		SEG7  |= 0b00000010;
#define SEG_3D		SEG7  |= 0b00000001;
#define SEG_3F		SEG6  |= 0b00001000; 			/* Petrol pump */
#define SEG_3G		SEG6  |= 0b00000100; 
#define SEG_3E		SEG6  |= 0b00000010; 
#define SEG_3_BLANK		SEG6  |= 0b00000001; 
#define CLEAR_3		SEG6 &=0XF1; SEG7 &=0XF0;

#define SEG_4A		SEG5  |= 0b00001000;
#define SEG_4B		SEG5  |= 0b00000100;
#define SEG_4C		SEG5  |= 0b00000010;
#define SEG_4D		SEG5  |= 0b00000001;			/* Service Symbol */
#define SEG_4F		SEG4  |= 0b00001000;
#define SEG_4G		SEG4  |= 0b00000100;
#define SEG_4E		SEG4  |= 0b00000010;
#define SEG_4_BLANK		SEG4  |= 0b00000001;
#define CLEAR_4		SEG4 &=0XF1; SEG5 &=0XF0;

#define SEG_5A		SEG3   |= 0b00001000;
#define SEG_5B		SEG3   |= 0b00000100;
#define SEG_5C		SEG3   |= 0b00000010;
#define SEG_5D		SEG3   |= 0b00000001;
#define SEG_5F 		SEG2  |= 0b00001000;			/* Blank */
#define SEG_5G		SEG2  |= 0b00000100;
#define SEG_5E		SEG2  |= 0b00000010; 
#define SEG_5_BLANK		SEG2  |= 0b00000001;
#define CLEAR_5		SEG2 &=0XF1; SEG3 &=0XF0; 


#define SEG_6A		SEG1  |= 0b00001000;
#define SEG_6B 		SEG1  |= 0b00000100;
#define SEG_6C		SEG1  |= 0b00000010;
#define SEG_6D		SEG1  |= 0b00000001;
#define SEG_6F		SEG0  |= 0b00001000;			/* Blank */
#define SEG_6G		SEG0  |= 0b00000100;
#define SEG_6E		SEG0  |= 0b00000010;
#define SEG_6_HOUR_SYMBOL		SEG0  |= 0b00000001;
#define CLEAR_6		SEG0 &=0XF1; SEG1 &=0XF0;


/* Hour Display Digits - Position 1 */
#define     DISPLAY_POS_1_VALUE_off   CLEAR_1;  
#define     DISPLAY_POS_1_VALUE_dash  SEG_1G; 
#define     DISPLAY_POS_1_VALUE_0     SEG_1A; SEG_1B; SEG_1C; SEG_1D; SEG_1E; SEG_1F;  
#define     DISPLAY_POS_1_VALUE_1     SEG_1B; SEG_1C;
#define     DISPLAY_POS_1_VALUE_2     SEG_1A; SEG_1B; SEG_1D; SEG_1E; SEG_1G; 
#define     DISPLAY_POS_1_VALUE_3     SEG_1A; SEG_1B; SEG_1C; SEG_1D; SEG_1G; 
#define     DISPLAY_POS_1_VALUE_4     SEG_1B; SEG_1C; SEG_1F; SEG_1G; 
#define     DISPLAY_POS_1_VALUE_5     SEG_1A; SEG_1C; SEG_1D; SEG_1F; SEG_1G; 
#define     DISPLAY_POS_1_VALUE_6     SEG_1A; SEG_1C; SEG_1D; SEG_1E; SEG_1F; SEG_1G; 
#define     DISPLAY_POS_1_VALUE_7     SEG_1A; SEG_1B; SEG_1C;
#define     DISPLAY_POS_1_VALUE_8     SEG_1A; SEG_1B; SEG_1C; SEG_1D; SEG_1E; SEG_1F; SEG_1G; 
#define     DISPLAY_POS_1_VALUE_9     SEG_1A; SEG_1B; SEG_1C; SEG_1D; SEG_1F; SEG_1G; 


/* Hour Display Digits - Position 2 */
#define     DISPLAY_POS_2_VALUE_off   CLEAR_2;
#define     DISPLAY_POS_2_VALUE_dash  SEG_2G; 
#define     DISPLAY_POS_2_VALUE_0     SEG_2A; SEG_2B; SEG_2C; SEG_2D; SEG_2E; SEG_2F; 
#define     DISPLAY_POS_2_VALUE_1     SEG_2B; SEG_2C; 
#define     DISPLAY_POS_2_VALUE_2     SEG_2A; SEG_2B; SEG_2D; SEG_2E; SEG_2G; 
#define     DISPLAY_POS_2_VALUE_3     SEG_2A; SEG_2B; SEG_2C; SEG_2D; SEG_2G; 
#define     DISPLAY_POS_2_VALUE_4     SEG_2B; SEG_2C; SEG_2F; SEG_2G; 
#define     DISPLAY_POS_2_VALUE_5     SEG_2A; SEG_2C; SEG_2D; SEG_2F; SEG_2G; 
#define     DISPLAY_POS_2_VALUE_6     SEG_2A; SEG_2C; SEG_2D; SEG_2E; SEG_2F; SEG_2G; 
#define     DISPLAY_POS_2_VALUE_7     SEG_2A; SEG_2B; SEG_2C; 
#define     DISPLAY_POS_2_VALUE_8     SEG_2A; SEG_2B; SEG_2C; SEG_2D; SEG_2E; SEG_2F; SEG_2G; 
#define     DISPLAY_POS_2_VALUE_9     SEG_2A; SEG_2B; SEG_2C; SEG_2D; SEG_2F; SEG_2G; 


/* Hour Display Digits - Position 3 */
#define     DISPLAY_POS_3_VALUE_off   CLEAR_3;
#define     DISPLAY_POS_3_VALUE_dash  SEG_3G; 
#define     DISPLAY_POS_3_VALUE_0     SEG_3A; SEG_3B; SEG_3C; SEG_3D; SEG_3E; SEG_3F;
#define     DISPLAY_POS_3_VALUE_1     SEG_3B; SEG_3C; 
#define     DISPLAY_POS_3_VALUE_2     SEG_3A; SEG_3B; SEG_3D; SEG_3E; SEG_3G; 
#define     DISPLAY_POS_3_VALUE_3     SEG_3A; SEG_3B; SEG_3C; SEG_3D; SEG_3G; 
#define     DISPLAY_POS_3_VALUE_4     SEG_3B; SEG_3C; SEG_3F; SEG_3G; 
#define     DISPLAY_POS_3_VALUE_5     SEG_3A; SEG_3C; SEG_3D; SEG_3F; SEG_3G; 
#define     DISPLAY_POS_3_VALUE_6     SEG_3A; SEG_3C; SEG_3D; SEG_3E; SEG_3F; SEG_3G; 
#define     DISPLAY_POS_3_VALUE_7     SEG_3A; SEG_3B; SEG_3C; 
#define     DISPLAY_POS_3_VALUE_8     SEG_3A; SEG_3B; SEG_3C; SEG_3D; SEG_3E; SEG_3F; SEG_3G; 
#define     DISPLAY_POS_3_VALUE_9     SEG_3A; SEG_3B; SEG_3C; SEG_3D; SEG_3F; SEG_3G; 


/* Hour Display Digits - Position 4 */
#define     DISPLAY_POS_4_VALUE_off   CLEAR_4;
#define     DISPLAY_POS_4_VALUE_dash  SEG_4G; 
#define     DISPLAY_POS_4_VALUE_0     SEG_4A; SEG_4B; SEG_4C; SEG_4D; SEG_4E; SEG_4F; 
#define     DISPLAY_POS_4_VALUE_1     SEG_4B; SEG_4C;
#define     DISPLAY_POS_4_VALUE_2     SEG_4A; SEG_4B; SEG_4D; SEG_4E; SEG_4G; 
#define     DISPLAY_POS_4_VALUE_3     SEG_4A; SEG_4B; SEG_4C; SEG_4D; SEG_4G; 
#define     DISPLAY_POS_4_VALUE_4     SEG_4B; SEG_4C; SEG_4F; SEG_4G; 
#define     DISPLAY_POS_4_VALUE_5     SEG_4A; SEG_4C; SEG_4D; SEG_4F; SEG_4G; 
#define     DISPLAY_POS_4_VALUE_6     SEG_4A; SEG_4C; SEG_4D; SEG_4E; SEG_4F; SEG_4G; 
#define     DISPLAY_POS_4_VALUE_7     SEG_4A; SEG_4B; SEG_4C;  
#define     DISPLAY_POS_4_VALUE_8     SEG_4A; SEG_4B; SEG_4C; SEG_4D; SEG_4E; SEG_4F; SEG_4G; 
#define     DISPLAY_POS_4_VALUE_9     SEG_4A; SEG_4B; SEG_4C; SEG_4D; SEG_4F; SEG_4G; 


/* Hour Display Digits - Position 5 */
#define     DISPLAY_POS_5_VALUE_off   CLEAR_5;
#define     DISPLAY_POS_5_VALUE_dash  SEG_5G; 
#define     DISPLAY_POS_5_VALUE_0     SEG_5A; SEG_5B; SEG_5C; SEG_5D; SEG_5E; SEG_5F;  
#define     DISPLAY_POS_5_VALUE_1     SEG_5B; SEG_5C; 
#define     DISPLAY_POS_5_VALUE_2     SEG_5A; SEG_5B; SEG_5D; SEG_5E; SEG_5G; 
#define     DISPLAY_POS_5_VALUE_3     SEG_5A; SEG_5B; SEG_5C; SEG_5D; SEG_5G; 
#define     DISPLAY_POS_5_VALUE_4     SEG_5B; SEG_5C; SEG_5F; SEG_5G; 
#define     DISPLAY_POS_5_VALUE_5     SEG_5A; SEG_5C; SEG_5D; SEG_5F; SEG_5G; 
#define     DISPLAY_POS_5_VALUE_6     SEG_5A; SEG_5C; SEG_5D; SEG_5E; SEG_5F; SEG_5G; 
#define     DISPLAY_POS_5_VALUE_7     SEG_5A; SEG_5B; SEG_5C; 
#define     DISPLAY_POS_5_VALUE_8     SEG_5A; SEG_5B; SEG_5C; SEG_5D; SEG_5E; SEG_5F; SEG_5G; 
#define     DISPLAY_POS_5_VALUE_9     SEG_5A; SEG_5B; SEG_5C; SEG_5D; SEG_5F; SEG_5G; 


/* Hour Display Digits - Position 6 */
#define     DISPLAY_POS_6_VALUE_off   CLEAR_6;
#define     DISPLAY_POS_6_VALUE_dash  SEG_6G; 
#define     DISPLAY_POS_6_VALUE_0     SEG_6A; SEG_6B; SEG_6C; SEG_6D; SEG_6E; SEG_6F; 
#define     DISPLAY_POS_6_VALUE_1     SEG_6B; SEG_6C; 
#define     DISPLAY_POS_6_VALUE_2     SEG_6A; SEG_6B; SEG_6D; SEG_6E; SEG_6G; 
#define     DISPLAY_POS_6_VALUE_3     SEG_6A; SEG_6B; SEG_6C; SEG_6D; SEG_6G; 
#define     DISPLAY_POS_6_VALUE_4     SEG_6B; SEG_6C; SEG_6F; SEG_6G; 
#define     DISPLAY_POS_6_VALUE_5     SEG_6A; SEG_6C; SEG_6D; SEG_6F; SEG_6G; 
#define     DISPLAY_POS_6_VALUE_6     SEG_6A; SEG_6C; SEG_6D; SEG_6E; SEG_6F; SEG_6G; 
#define     DISPLAY_POS_6_VALUE_7     SEG_6A; SEG_6B; SEG_6C; 
#define     DISPLAY_POS_6_VALUE_8     SEG_6A; SEG_6B; SEG_6C; SEG_6D; SEG_6E; SEG_6F; SEG_6G; 
#define     DISPLAY_POS_6_VALUE_9     SEG_6A; SEG_6B; SEG_6C; SEG_6D; SEG_6F; SEG_6G; 

// Global prototypes --------------------------------------------------------

/// @addtogroup LCD_LIBRARY
/// @{
 
/// @defgroup LCDHeaderLib Global functions used in lcd file
//// 
///   
/// @addtogroup LCDHeaderLib
/// @{

void TurnONAllLcdSeg						(void);
void TurnOffAllLcdSeg						(void);
void vHourMeter_Display						(void);
void vDisplayHourDigits						(u32 HoursValue);
/// @} // endgroup LCDHeaderLib

/// @} // endgroup addtogroupLCD_LIBRARY



