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
* File Name    : r_cg_timer_user.c
* Version      : Applilet3 for RL78/D1A V2.04.03.01 [15 Nov 2019]
* Device(s)    : R5F10CGB
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for TAU module.
* Creation Date: 01/02/2022
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt INTTM10 r_tau1_channel0_interrupt
#pragma interrupt INTTM12 r_tau1_channel2_interrupt
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_timer.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
static bool bUpdateHourMeterValue = FALSE;
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_tau1_channel0_interrupt
* Description  : This function is INTTM10 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
	/* Interrupt occure in every 50 mili second */
__interrupt static void r_tau1_channel0_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
	bUpdateHourMeterValue = TRUE;
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_tau1_channel2_interrupt
* Description  : This function is INTTM12 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
/* Interrupt occure in every 1 Milisec */
__interrupt static void r_tau1_channel2_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
	/* 1 ms timer for system clock */
	++gu32Clock1msTicker;
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
bool bGetHourMeterUpdateStatus(void)
{
  	return bUpdateHourMeterValue;
}

void vSetHourMeterUpdateStatus(bool StatusFlag)
{
	bUpdateHourMeterValue = StatusFlag;
}
/* End user code. Do not edit comment generated here */
