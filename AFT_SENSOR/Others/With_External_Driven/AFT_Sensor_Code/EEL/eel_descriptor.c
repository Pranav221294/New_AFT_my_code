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

/*******************************************************************************
* Library       : EEPROM Emulation Library (T02)
*
* File Name     : $Source: eelDescriptor.c $
* Lib. Version  : $RL78_EEL_LIB_VERSION_T02: V1.00 $
* Mod. Revision : $Revision: 1.8 $
* Mod. Date     : $Date: 2012/10/26 09:28:20MESZ $
* Device(s)     : RL78
* Description   : contains the user defined EEL-variable descriptor
*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/



/*==============================================================================================*/
/* compiler settings                                                                            */
/*==============================================================================================*/
#pragma name eel_descriptor
#pragma sfr

#pragma section @@CNSTL EEL_CNST

#include "dataflash.h"
#include "r_cg_userdefine.h"


/*==============================================================================================*/
/* include files list                                                                           */
/*==============================================================================================*/
#define  __EEL_DESCRIPTOR_C
  #include  "eel_types.h"
  #include  "eel_descriptor.h"
#undef   __EEL_DESCRIPTOR_C

#include  "fdl_types.h"


/* definition of variable types registered at EEL */
#include  "eel_user_types.h"


/*==============================================================================================*/
/* import list                                                                                  */
/*==============================================================================================*/
/* empty */


/*==============================================================================================*/
/* data definition                                                                              */
/*==============================================================================================*/
/* empty */



/*********************************************************************************************************/
/*******                                                                                           *******/
/*******      B E G I N    O F    C U S T O M I Z A B L E    D E C L A R A T I O N    A R E A      *******/
/*******                                                                                           *******/
/*********************************************************************************************************/

__far const eel_u8 eel_descriptor[EEL_VAR_NO + 2] =
{
	(eel_u8)(EEL_VAR_NO),  /* variable count   			*/  \
	(eel_u8)1 ,		/* 1 byte bootloader identifier		*/  \

	(eel_u8)2	,  	/* P0_cal */
	(eel_u8)2	,  	/* P1E_cal */
	(eel_u8)2	,  	/* P2E_cal */
	(eel_u8)2	,  	/* P1F_cal */
	(eel_u8)2	,  	/* P2F_cal */
	(eel_u8)4	,  	/* m This value is in float so store in 4 byte int */
	(eel_u8)4	,  	/* Div_cal,  This value is in float so store in 4 byte int */
	(eel_u8)4	,  	/* Span_cal, 4 byte */
	(eel_u8)1	,  	/* status Cali, 1 byte for true or false */
	(eel_u8)4	,  	/* div_avg_stored, This is float value so 4 bytes */
	(eel_u8)1	,  	/* One Time Calibration flag */
	(eel_u8)4	,  	/* Number of time calibration, How many times system tried to calibrate */
	(eel_u8)1	,  	/* Last state of calibration */
	(eel_u8)1	,  	/* time stamp hour */
	(eel_u8)1	,  	/* time stamp minutes */
	(eel_u8)1	,  	/* time stamp seconds */
	(eel_u8)1	,  	/* time stamp AM or PM */
	(eel_u8)1	,  	/* time stamp day */
	(eel_u8)1	,  	/* time stamp Months */
	(eel_u8)1	,  	/* time stamp Year */
	(eel_u8)1	,  	/* calibration method */


	(eel_u8)1	,  	/* eSetDefaultFlag1			*/  \
	(eel_u8)1	,  	/* eSetDefaultFlag2			*/  \
	
	

  
	
    /*	* Add required Parameters in eelDescriptor array in same sequence
	/*	  as mentioned in eSysParmsIndex enum.
	/*	  Example : (eel_u8)1,  /* id=15             */  \
	/*	  			(eel_u8) is typedef, 1 is the Data Size of the variable.
	/*	NOTE : the sequence of defining enum should be same as sequence of size defined in eelDescriptor.c
	/*
  	/*  * Navigate to eel_descriptor.h and change the value of EEL_VAR_NO to :  
	/*	  |14+(no. of added parameters)|.
	/*	  Example : If adding 4 more paramenters, then the value of EEL_VAR_NO changes to 18 (14+4).*/

  
  (eel_u8)(0x00),            /* zero terminator  */  \
};

/*********************************************************************************************************/
/*******                                                                                           *******/
/*******      E N D    O F    C U S T O M I Z A B L E    D E C L A R A T I O N    A R E A          *******/
/*******                                                                                           *******/
/*********************************************************************************************************/




/*********************************************************************************************************/
/*******                                                                                           *******/
/*******      B E G I N    O F    U N T O U C H A B L E     D E C L A R A T I O N    A R E A       *******/
/*******                                                                                           *******/
/*********************************************************************************************************/
__far const eel_u8   eel_internal_cfg_cau08[]     = {0x40};
/*********************************************************************************************************/
/*******                                                                                           *******/
/*******      E N D    O F    U N T O U C H A B L E     D E C L A R A T I O N    A R E A           *******/
/*******                                                                                           *******/
/*********************************************************************************************************/

