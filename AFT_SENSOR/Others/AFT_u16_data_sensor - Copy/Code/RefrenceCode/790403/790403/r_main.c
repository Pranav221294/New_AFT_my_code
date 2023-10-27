/***********************************************************************************************************************
* DISCLAIMER.......................................................................................................
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
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
* Copyright (C) 2012, 2014 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_main.c
* Version      : Applilet3 for RL78/D1A V2.03.00.03 [07 Aug 2014]
* Device(s)    : R5F10CGB
* Tool-Chain   : CA78K0R
* Description  : This file implements main function.
* Creation Date: 26-May-16
***********************************************************************************************************************/

/*****************************************************************************
**  Project : LCD hour Meter(790304) 
**
** ---------------------------------------------------------------------------
** Software developed for overseas Customer
   
    ? Displays Total engine running hours.
	? Reset total hour by Input pin.
	
**          Devices used:
**              NEC R5F10CGB (RL78 series ) 48 pin device
**                  24Kb flash  
**                  2Kb RAM  
**                  8Kb Data Flash
**                  LCD(Seg x com)  16 x 4
**                  Stepper motor driver inbuilt 1 ch
**----------------------------------------------------------------------------

/**  Revision History *************************************************************************************************
**
**  Date         Name             Version     	Description
** ==========   ===============   ========    	=========================================================================
** 04/05/2022   Dharmendra singh  V1.0    		Initial Version, for this part #790403 reference taken from 
**												http://20.198.78.198/Bongogitserver/Repository/a7f6afde-84c4-4a7b-b142-8096b104f5f1/master/Tree/Code/250812_KingCAT
**
**********************************************************************************************************************/
/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */



/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_wdt.h"
#include "r_cg_lcd.h"
#include "r_cg_timer.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "CustomTypedef.h"
#include "r_cg_userdefine.h"
#include "HrDataSave.h"
#include "LcdDriver.h"
#include "HourMeter.h"
#include "Event.h"
#include "InputSense.h"
#include "dataflash.h"





/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

#define Software_Version    	V1.0     /*  Version number */
#define INIT_WAITING_DELAY   	1000u	 /*  1ms * 1000 = 1sec*/
#define POWER_STABLE_DELAY   	475u 	 /*  1ms * 497 = 497ms*/

// Global declarations -----------------------------------------------------------------------------------------------------------------
u32 gu32Clock1msTicker = 0;
/*******************************************************
 CHAR & bool DATA TYPE
********************************************************/

/* Function prototyping -------------------------------------------------------*/
void R_MAIN_UserInit							(void);
static void _vUnUsedPinMakeAsOutPut				(void);



/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
	static u16 temp_var = 0u;  							/* General purpose variable */
	
	R_MAIN_UserInit();
	/* Start user code. Do not edit comment generated here */
	_vUnUsedPinMakeAsOutPut(); 
	R_TAU1_Channel2_Start();		             		/* timer 1 mili sec interval timer */  
	/* For Hourmeter data init. */
	R_LCD_VoltageOn();                              	/* On Inbuilt LCD Driver */
	R_LCD_Start();
	/* Lcd Self check for all char. ok or not----------------- */
	TurnONAllLcdSeg();									/* All char ON when system turn ON */
	/* wait for power stable so that no spike occur during reading hour meter data from EEPROM */
	temp_var = (u16)u32GetClockTicks();
	while((u16)u32GetClockTicks() - temp_var < POWER_STABLE_DELAY )	/* Delay < 2 seconds for self check*/
	{
	  	R_WDT_Restart();
	}	
	vDataFlashinit();									/* Data Flash init compulsory before read flash memory */
	vSetDefaultVariablesToFlash();						/* SET default hour meter value*/
	MemoryInit();                  						/* Read hr data from EEprom  */
	//_vAddStartUpTime();
	R_TAU1_Channel0_Start();		             		/* Timer 50 milisec interval timer */
	temp_var = (u16)u32GetClockTicks();
	while((u16)u32GetClockTicks() - temp_var < INIT_WAITING_DELAY )	/* Delay < 2 seconds for self check*/
	{
	  	R_WDT_Restart();
	}	
	TurnOffAllLcdSeg();									/* All segments clear */
	DECIMAL_DOT_SYMBOL_ON;
	

    while(1U)
    {     
		R_WDT_Restart();                        /* Clr wathdog timer -------------*/
		vStart_Hour_Meter();					/* Hour Meter Enable check */
		UpdateHourMeter();						/* Engine Hour meter increment  */
		vSaveHourData();						/* Save Engine Hour data meter increment  */
		vHourMeter_Display();				 	/* hour meter data & Fuel bars display and Hours Reset service riminder logic check */
		vResetHourData();						/* Reset Hour Meter data */	
	}

	/* End user code. Do not edit comment generated here */
}



/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MAIN_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */
    EI();
    /* End user code. Do not edit comment generated here */
}



/*********************************************************************//**
 *
 * _vUnUsedPinMakeAsOutPut 		This function is used for unused 12 nos mcu pins make as 
 * 								output to zero from MCU side.
 *  					
 *
 * @param      none
 *
 * @return     none
 *************************************************************************/
static void _vUnUsedPinMakeAsOutPut(void)
{
	MCU_PIN_01	= 0;
	MCU_PIN_02	= 0;
	MCU_PIN_03	= 0;
	MCU_PIN_04	= 0;

	MCU_PIN_37	= 0;
	MCU_PIN_38	= 0;
	MCU_PIN_39	= 0;
	MCU_PIN_40	= 0;
	MCU_PIN_44	= 0;
	MCU_PIN_45	= 0;
	MCU_PIN_46	= 0;
	MCU_PIN_47	= 0;
	MCU_PIN_48	= 0;

	MCU_PIN_13	= 0;
	MCU_PIN_14	= 0;
	MCU_PIN_15	= 0;
	MCU_PIN_16	= 0;
	MCU_PIN_17	= 0;
	MCU_PIN_18	= 0;
	MCU_PIN_19	= 0;
	MCU_PIN_20	= 0;	
}

/*********************************************************************//**
 *
 * 1ms System clock
 *
 * @param      None
 *
 * @return     system clock value in u32
 *************************************************************************/
u32 u32GetClockTicks(void)
{
	return gu32Clock1msTicker;
}

/*********************************************************************//**
 *
 * 1ms System clock
 *
 * @param      None
 *
 * @return     system clock value in u16
 *************************************************************************/
u16 u16GetClockTicks(void)
{
	return (u16)gu32Clock1msTicker;
}





/* End user code. Do not edit comment generated here */
