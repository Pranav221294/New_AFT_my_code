/***********************************************************************************************************************
* DISCLAIMER
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
* File Name    : r_cg_userdefine.h
* Version      : Applilet3 for RL78/D1A V2.03.00.03 [07 Aug 2014]
* Device(s)    : R5F10CGB
* Tool-Chain   : CA78K0R
* Description  : This file includes user definition.
* Creation Date: 26-May-16
***********************************************************************************************************************/


#ifndef _USER_DEF_H
#define _USER_DEF_H

#include "CustomTypedef.h"
/***********************************************************************************************************************
User definitions
***********************************************************************************************************************/



#define OFF					0U
#define ON					1U

/*---------------------------------------------------------------------------*/


/* Start user code for function. Do not edit comment generated here */

/* Given below all MCU Unused Pins keep as output low from MCU */ 
#define MCU_PIN_01			P2.3
#define MCU_PIN_02			P2.2
#define MCU_PIN_03			P2.1
#define MCU_PIN_04			P2.0

#define MCU_PIN_37			P9.4
#define MCU_PIN_38			P9.3
#define MCU_PIN_39			P9.2
#define MCU_PIN_40			P9.1
#define MCU_PIN_44			P8.3
#define MCU_PIN_45			P8.2
#define MCU_PIN_46			P8.1
#define MCU_PIN_47			P8.0
#define MCU_PIN_48			P2.7

#define MCU_PIN_13			P6.0
#define MCU_PIN_14			P6.1
#define MCU_PIN_15			P1.0
#define MCU_PIN_16			P1.1
#define MCU_PIN_17			P1.2
#define MCU_PIN_18			P7.5
#define MCU_PIN_19			P7.4
#define MCU_PIN_20			P1.3
/*--------------------------------------------------------------------------*/
extern u32 gu32Clock1msTicker;
extern void delay							(u16);
extern u32  u32GetClockTicks				(void);
extern u16 u16GetClockTicks					(void);



/* End user code. Do not edit comment generated here */
#endif
