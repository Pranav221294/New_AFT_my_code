/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2012, 2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_lcd.h
* Version      : Applilet3 for RL78/D1A V2.04.03.01 [15 Nov 2019]
* Device(s)    : R5F10CGB
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for LCD module.
* Creation Date: 24/5/2022
***********************************************************************************************************************/

#ifndef LCD_H
#define LCD_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/*
    LCD Mode Register (LCDMD)
*/
/* LCD drive voltage generator selection (MDSET1, MDSET0) */
#define _00_LCD_VOLTAGE_MODE_EXTERNAL   (0x00U)    /* no internal resistor connection */
#define _10_LCD_VOLTAGE_MODE_INTERNAL   (0x10U)    /* internal resistance division method (no step-down transforming) */
#define _30_LCD_VOLTAGE_MODE_CAPACITOR  (0x30U)    /* internal resistance division method (step-down transforming) */

/*
    LCD Display Mode Register (LCDM)
*/
/* LCD display enable/disable (LCDON, SCOC) */
#define _00_LCD_DISPLAY_GROUND          (0x00U)    /* output ground level to segment/common pin */
#define _40_LCD_DISPLAY_OFF             (0x40U)    /* display off (all segment outputs are deselected) */
#define _C0_LCD_DISPLAY_ON              (0xC0U)    /* display on */
/* LCD controller/driver display mode selection (LCDM2, LCDM0) */
#define _00_LCD_DISPLAY_MODE0           (0x00U)    /* 4 time slices, 1/3 bias mode */
#define _01_LCD_DISPLAY_MODE1           (0x01U)    /* 3 time slices, 1/3 bias mode */
#define _04_LCD_DISPLAY_STATIC          (0x04U)    /* static */

/*
    LCD Clock Control Register (LCDC0)
*/
/* LCD source clock (fLCD) selection (LCDC6 - LCDC4) */
#define _00_LCD_SOURCE_CLOCK_FSUB       (0x00U)    /* fSUB */
#define _10_LCD_SOURCE_CLOCK_FMAIN_32   (0x10U)    /* fMAIN/2^5 */
#define _20_LCD_SOURCE_CLOCK_FMAIN_64   (0x20U)    /* fMAIN/2^6 */
#define _30_LCD_SOURCE_CLOCK_FMAIN_128  (0x30U)    /* fMAIN/2^7 */
#define _40_LCD_SOURCE_CLOCK_FMAIN_256  (0x40U)    /* fMAIN/2^8 */
#define _50_LCD_SOURCE_CLOCK_FMAIN_512  (0x50U)    /* fMAIN/2^9 */
#define _60_LCD_SOURCE_CLOCK_FMAIN_1024 (0x60U)    /* fMAIN/2^10 */
#define _70_LCD_SOURCE_CLOCK_FIL        (0x70U)    /* fIL */
/* LCD clock (LCDCL) selection (LCDC2 - LCDC0) */
#define _00_LCD_CLOCK_FLCD_16           (0x00U)    /* fLCD/2^4 */
#define _01_LCD_CLOCK_FLCD_32           (0x01U)    /* fLCD/2^5 */
#define _02_LCD_CLOCK_FLCD_64           (0x02U)    /* fLCD/2^6 */
#define _03_LCD_CLOCK_FLCD_128          (0x03U)    /* fLCD/2^7 */
#define _04_LCD_CLOCK_FLCD_256          (0x04U)    /* fLCD/2^8 */
#define _05_LCD_CLOCK_FLCD_512          (0x05U)    /* fLCD/2^9 */


/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_LCD_Create(void);
void R_LCD_Start(void);
void R_LCD_Stop(void);
void R_LCD_VoltageOn(void);
void R_LCD_VoltageOff(void);

/* Start user code for function. Do not edit comment generated here */



typedef enum 
     {
	    eSolidON = 0,
	    eOff,
	    eBlink,
	    eOnOffSeparate   
     }ENUM_TYPE; 

void vLcdInit();
void vDisplayDigit(unsigned long);
void vDisplayHrIcon(ENUM_TYPE);   
     
/* End user code. Do not edit comment generated here */
#endif
