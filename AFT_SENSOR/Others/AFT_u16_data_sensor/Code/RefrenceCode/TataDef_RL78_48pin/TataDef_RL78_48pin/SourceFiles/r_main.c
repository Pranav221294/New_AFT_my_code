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
/// \file main.c
 /// \brief Adblue Guage Documentation
 /// \author Copyright (c) 2012 Indication Instruments Limited. All rights reserved.
 /// \version 1.0
 /// \date 22/05/2019
 /// Revision History : Initial version
 
/*!

 	\mainpage 

 	\section welcome Welcome
	 
	 **  This Project has 1 Gauges and warning led
	 
	 **  1) DEF gauge based on CAN input.
	 **  2) Warning TelleTale on low def level
	 **  
		\section sect1 " TATA DEF Gauge" Documentation
*/
 /**************************************************************************

 ** Devices used :
 ** 		Renesas R5F10DGD  				48 pin memory map
 **                	   8  KB RAM			
 **     	           48 KB flash			
 **				   
 **				   		16 MHz External crystal
 **	     	       		1 CAN Module
 **                		10 bit ADC, 8 ADC channels
 ** ---------------------------------------------------------------------------
 **  COPYRIGHT:  Indication Instruments Ltd., 
 **  	         Plot No. 19,Sector6
 **  		   	 Faridabad - 121006	    
 ** ----------------------------------------------------------------------------
 ** IDE				: CubeSuite+(CS+)
 ** Version     	: Applilet3 for RL78/D1A V2.03.00.03 [07 Aug 2014]
 ** Debuging Tool	: Renesas E1 programmer
 ** Tool-Chain 		: CA78K0R

 **----------------------------------------------------------------------------
 **  Co-ordinator                Nikita Malpani
 **	 Software Manager			 Vishnu JAAT
 **----------------------------------------------------------------------------
 **  Revision History
 **
 **  Date      		 Name          	 	Version   				Description
 ** ==========  	=============== 	==========			====================================
 ** 12/07/2022  	Dharmendra Singh    Version V1.0 		Initial version of 387306 ,base reference taken from 
 **															http://20.198.78.198/Bongogitserver/Repository/d95c6be3-f435-43e8-9b91-d6fef09cdfe3/master/Tree/Code/387933_AdblueGaugeRL78 GIT
 **															Requirements implemented as per existing Sample on STM8 mcu based.
 **
 ***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_main.c
* Version      : Applilet3 for RL78/D1A V2.04.03.01 [15 Nov 2019]
* Device(s)    : R5F10DPG
* Tool-Chain   : CA78K0R
* Description  : This file implements main function.
* Creation Date: 22/6/2022
***********************************************************************************************************************/

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
#include "r_cg_timer.h"
#include "r_cg_wdt.h"

/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"
#include "customTypedef.h"
#include "interrupts.h"
#include "ramVector.h"
#include "StepperDriver.h"
#include "Can.h"
#include "J1939.h" 
#include "addressClaim.h"
#include "defGauge.h"
#include "TellTales.h"
#include "SoftVersion.h"



/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */

// Global declarations -----------------------------------------------------------------------------------------------------------------
u32 gu32Clock1msTicker = 0;


static void _vMapInterrupts							(void);
static void _vTasksAfterSysTurn_ON					(void);
static void _vTasksAfterLongHoming					(void);
static void vUnusedMCUPinsMakeAsOutPut				(void);

/* End user code. Do not edit comment generated here */
void R_MAIN_UserInit(void);

/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
	static u16 TimeStamp = 0;
	static bool bstatus = 0;
	
    R_MAIN_UserInit();
	
	_vTasksAfterSysTurn_ON();
	_vTasksAfterLongHoming();
    /* Start user code. Do not edit comment generated here */
	
	
    while (1U)
    {
		vAddressClaimStateMgr();  				/* Send CAN msg to ECU */ 
		vJ1939_ServiceManager();	
		vStepperPositionDriver(eDefGauge);		/* Calculated Def gauge stepper pos*/
		vDefGauge();
		vControlDefTellTales();					/* Low def lamp warning */
		vRecoveryFromBusOffState();				/* Auto recover CAN communication */
        R_WDT_Restart();
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

/* Start user code for adding. Do not edit comment generated here */

/*********************************************************************//**
 *
 * Enable interrupts and initialize varibale and 
 * check power on 
 *
 * @param      None    
 *
 * @return     None
 *************************************************************************/
static void _vTasksAfterSysTurn_ON(void)
{
	/* Start user code. Do not edit comment generated here */
	//_vMapInterrupts();
	vUnusedMCUPinsMakeAsOutPut();		/*Unused MCU PIN keep as output to zero from MCU side*/
	LOW_DEF_WARNING_LED_ON;				/* Warning Led OFF */
	//This flag must be true here so that clock can count second before read flash data
	vStepperInit();		
	R_TAU0_Channel1_Start();  			//TAU01 100us timer for stepper motors 
	R_TAU0_Channel4_Start();  			//TAU04 1 ms timer for system clock
	EI();                           	//Enable all interrupts
	
	R_WDT_Restart();
	vLongHoming();
	vShortHoming();
    /* End user code. Do not edit comment generated here */
}

/*********************************************************************//**
 *
 *  init and do self testing 
 *
 * @param      None    
 *
 * @return     None
 *************************************************************************/
static void _vTasksAfterLongHoming(void)
{
	vCanInit();	
	vJ1939Init();               	// J1939 buffer init before CAN init.	
}

/*****************************************************************************

**  Function name:  _vMapInterrupts
**
**  Description:    Mapped Interrupts to vector table
**
**  Parameters:     None
**
**  Return value:   None
**
*****************************************************************************/
static void _vMapInterrupts()
{
//	RAM_INTTM01_ISR = &r_tau0_channel1_interrupt;
//	RAM_INTTM04_ISR = &r_tau0_channel4_interrupt;
//	RAM_INTWDTI_ISR = &r_wdt_interrupt;
//	RAM_INTC0REC_ISR = &MY_Rx_MsgBuf_Processing;
//	RAM_INTC0TRX_ISR = &MY_Tx_MsgBuf;
//	RAM_INTC0ERR_ISR = &MY_ISR_CAN_Error;
//	RAM_INTC0WUP_ISR = &MY_ISR_CAN_Wakeup;	
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

/*****************************************************************************

**  Function name:  vUnusedMCUPinsMakeAsOutPut
**
**  Description:    This function is used for All unused MCU pins keep as out put Zero
**
**  Parameters:     None
**
**  Return value:   None
**
*****************************************************************************/
static void vUnusedMCUPinsMakeAsOutPut(void)
{
	MCU_PIN_NO_1 	= 0u;
	MCU_PIN_NO_2	= 0u;
	MCU_PIN_NO_3 	= 0u;
	MCU_PIN_NO_4 	= 0u;

	MCU_PIN_NO_13   = 0u;
	MCU_PIN_NO_14   = 0u;
	MCU_PIN_NO_15   = 0u;
	MCU_PIN_NO_16   = 0u;
	MCU_PIN_NO_17   = 0u;
	MCU_PIN_NO_18   = 0u;
	MCU_PIN_NO_19   = 0u;
	MCU_PIN_NO_20   = 0u;
	MCU_PIN_NO_21   = 0u;
	MCU_PIN_NO_24   = 0u;

	MCU_PIN_NO_25   = 0u;
	MCU_PIN_NO_26   = 0u;
	MCU_PIN_NO_27   = 0u;
	MCU_PIN_NO_28   = 0u;
	MCU_PIN_NO_29   = 0u;
	MCU_PIN_NO_30   = 0u;
	MCU_PIN_NO_31   = 0u;
	MCU_PIN_NO_32   = 0u;
	
	MCU_PIN_NO_37   = 0u;
	MCU_PIN_NO_38   = 0u;
	MCU_PIN_NO_39   = 0u;
	MCU_PIN_NO_40   = 0u;
	
}

/* End user code. Do not edit comment generated here */
