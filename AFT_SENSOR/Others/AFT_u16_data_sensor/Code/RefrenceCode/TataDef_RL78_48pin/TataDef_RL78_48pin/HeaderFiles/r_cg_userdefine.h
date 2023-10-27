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
* File Name    : r_cg_userdefine.h
* Version      : Applilet3 for RL78/D1A V2.04.03.01 [15 Nov 2019]
* Device(s)    : R5F10DPG
* Tool-Chain   : CA78K0R
* Description  : This file includes user definition.
* Creation Date: 22/6/2022
***********************************************************************************************************************/

#ifndef _USER_DEF_H
#define _USER_DEF_H

#include "r_cg_macrodriver.h"
#include "customTypedef.h"
/***********************************************************************************************************************
User definitions
***********************************************************************************************************************/

/* Start user code for function. Do not edit comment generated here */
#define STD_ENABLE					1U
#define STD_DISABLE					0U


/*All unused PIN make as Output --------------------------*/

#define MCU_PIN_NO_1 				P2.3
#define MCU_PIN_NO_2				P2.2
#define MCU_PIN_NO_3 				P2.1
#define MCU_PIN_NO_4 				P2.0

#define MCU_PIN_NO_13   			P6.0
#define MCU_PIN_NO_14   			P6.1
#define MCU_PIN_NO_15   			P1.0
#define MCU_PIN_NO_16   			P1.1
#define MCU_PIN_NO_17   			P1.2
#define MCU_PIN_NO_18   			P7.5
#define MCU_PIN_NO_19   			P7.4
#define MCU_PIN_NO_20   			P1.3
#define MCU_PIN_NO_21   			P1.4
#define MCU_PIN_NO_24   			P3.3

#define MCU_PIN_NO_25   			P3.1
#define MCU_PIN_NO_26   			P3.0
#define MCU_PIN_NO_27   			P5.7
#define MCU_PIN_NO_28   			P5.6
#define MCU_PIN_NO_29   			P5.5
#define MCU_PIN_NO_30   			P5.4
#define MCU_PIN_NO_31   			P7.2
#define MCU_PIN_NO_32   			P7.3

#define MCU_PIN_NO_37   			P9.4
#define MCU_PIN_NO_38   			P9.3
#define MCU_PIN_NO_39   			P9.2
#define MCU_PIN_NO_40   			P9.1

// Global declarations ------------------------------------------------------
extern u32 gu32Clock1msTicker;


/// @addtogroup MAIN_LIBRARY
/// @{
/// @defgroup MainLib  functions used in main file 
//// 
///   
/// @addtogroup MainLib
/// @{

// Global prototypes --------------------------------------------------------

u16 u16GetClockTicks				(void);
u32 u32GetClockTicks				(void);

/// @} // endgroup MainLib

/// @} // endgroup MAIN_LIBRARY
/* End user code. Do not edit comment generated here */
#endif
