/* CA78K0R C Source Converter V1.00.03.01 [10 May 2018] */
/*****************************************************************************
 DISCLAIMER
 This software is supplied by Renesas Electronics Corporation and is only
 intended for use with Renesas products. No other uses are authorized. This
 software is owned by Renesas Electronics Corporation and is protected under
 all applicable laws, including copyright laws.
 THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
 LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
 TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
 ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
 FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
 ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
 BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 Renesas reserves the right, without notice, to make changes to this software
 and to discontinue the availability of this software. By using this software,
 you agree to the additional terms and conditions found by accessing the
 following link:
 http://www.renesas.com/disclaimer
 Copyright (C) 2016-2018 Renesas Electronics Corporation. All rights reserved.
******************************************************************************/
/*****************************************************************************
**  Module:J1939.h
**
** ---------------------------------------------------------------------------
**  Description:
**      Header file for J1939.h
**
*****************************************************************************/
#ifndef _J1939_H_
#define _J1939_H_

// constant declarations -----------------------------------------------------

#include "r_cg_macrodriver.h"
#include "CustomDataTypes.h"
#include "GuiCommProtocol.h"


// Global prototypes --------------------------------------------------------

/// @addtogroup J1939_LIBRARY
/// @{
 
/// @defgroup J1939HeaderLib Global functions used in j1939 file
//// 
///   
/// @addtogroup J1939HeaderLib
/// @{

void vJ1939OnFullRx(void);

/// @} // endgroup J1939HeaderLib

/// @} // endgroup J1939_LIBRARY


#endif

