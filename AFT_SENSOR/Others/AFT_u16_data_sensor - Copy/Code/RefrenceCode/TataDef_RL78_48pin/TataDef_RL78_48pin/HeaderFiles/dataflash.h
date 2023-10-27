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
/************************************************************************************
**  Project : BS6Cluster
**	File	: dataflash.h
** ----------------------------------------------------------------------------------
**  Description:
**      All include files
**		EOL variables size
**		EOL variable address
**      System parameters declaration
**		System parameters Index
**		EOL Variables index
**	EOL data in FDL pool.
** ----------------------------------------------------------------------------------
**  COPYRIGHT: 
**              INDICATION INSTRUMENTS LTD.
**              PLOT-19, SECTOR 6, FARIDABAD.
**
** ----------------------------------------------------------------------------------
**
**  Revision History
**
**  Date       Name             Description
** ==========  ===============  =====================================================
** 09-10-2018  Pratyush Abhay   Initial Version.
*************************************************************************************/




#ifndef _DATA_FLASH
#define _DATA_FLASH


#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "eel_types.h"
#include "eel_user_types.h"
#include "eel_descriptor.h"
#include "eel.h"
#include "fdl.h"
#include "fdl_types.h"
#include "fdl_descriptor.h"


/*******************************************************************************************************
******************************************SYS DATA Enumeration******************************************
						ADD TO THIS ENUMERATION FOR MORE SYSTEM PARAMETERS

/* PARAMETERS CAN BE DELETED ACCORDING TO NEEDS.
/* NEVER DELETE "SetDefaultFlag" VARIABLES.
********************************************************************************************************/
typedef enum
{
	eBootLoaderIdentifier	= 1U	,			//Bootloader Identifier
	eHourSecondId					,	
	eHourIdentifier					,
	eSetDefaultFlag1				,
	eSetDefaultFlag2				,
	eNoOfSysParaMeter				
	/* if required to add new parameters, follow steps below :
	/*	* Add required parameters to eSysParmsIndex enum squencially as defined in dataflsh.h.
	/*	* Navigate to eel_descriptor.c and add required Parameters in eel_descriptor array in same sequence
	/*	  as mentioned in eSysParmsIndex enum.
	/*	  Example : (eel_u8)1,  /* id=15             */  
	/*	  			(eel_u8) is typedef, 1 is the Data Size of the variable.
	/*	NOTE : the sequence of defining enum should be same as sequence of size defined in eel_descriptor.c 
	/*	
	/*	* Navigate to eel_descriptor.h and change the value of EEL_VAR_NO to :  
	/*	  |14+(no. of added parameters)|.
	/*	  Example : If adding 4 more paramenters, then the value of EEL_VAR_NO changes to 18 (14+4).*/
	
}eSysParmsIndex;


//-------------Global declarations --------------------------------------------------------




/********************************************************************************************************
**********************************Public Function Prototype Declaration**********************************
*********************************************************************************************************/

/// @addtogroup DATAFLASH_LIBRARY
/// @{
/// @defgroup DataFlashLib  functions used 
//// 
///   
/// @addtogroup DataFlashLib
/// @{

void vEelFormat();
void vDataFlashinit();
void vSetDefaultVariablesToFlash(void);
void vSaveFullSystemParameter();
void vReadSystemParameter();
/// @} // endgroup DataFlashLib

/// @} // endgroup DATAFLASH_LIBRARY


#endif
