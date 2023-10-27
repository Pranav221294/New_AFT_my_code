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
* File Name    : r_cg_timer.c
* Version      : Applilet3 for RL78/D1A V2.04.03.01 [15 Nov 2019]
* Device(s)    : R5F10CGB
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for TAU module.
* Creation Date: 4/5/2022
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
#include "r_cg_timer.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_TAU1_Create
* Description  : This function initializes the TAU1 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU1_Create(void)
{
    TAU1EN = 1U;        /* supply input clock */
    TPS1 = _0000_TAU_CKM0_FCLK_0 | _0000_TAU_CKM1_FCLK_0 | _0000_TAU_CKM2_FCLK_0 | _4000_TAU_CKM3_FCLK_4;
    /* Stop all channels */
    TT1 = _0001_TAU_CH0_STOP_TRG_ON | _0002_TAU_CH1_STOP_TRG_ON | _0004_TAU_CH2_STOP_TRG_ON |
          _0008_TAU_CH3_STOP_TRG_ON | _0010_TAU_CH4_STOP_TRG_ON | _0020_TAU_CH5_STOP_TRG_ON |
          _0040_TAU_CH6_STOP_TRG_ON | _0080_TAU_CH7_STOP_TRG_ON;
    /* Mask channel 0 interrupt */
    TMMK10 = 1U;        /* disable INTTM10 interrupt */
    TMIF10 = 0U;        /* clear INTTM10 interrupt flag */
    /* Mask channel 2 interrupt */
    TMMK12 = 1U;        /* disable INTTM12 interrupt */
    TMIF12 = 0U;        /* clear INTTM12 interrupt flag */
    /* Set INTTM10 low priority */
    TMPR110 = 1U;
    TMPR010 = 1U;
    /* Set INTTM12 low priority */
    TMPR112 = 1U;
    TMPR012 = 1U;
    /* Channel 0 used as interval timer */
    TMR10 = _C000_TAU_CLOCK_SELECT_CKM3 | _0000_TAU_CLOCK_MODE_CKS | _0000_TAU_MODE_INTERVAL_TIMER |
            _0001_TAU_START_INT_USED;
    TDR10 = _C34F_TAU_TDR10_VALUE;
    TOM1 &= ~_0001_TAU_CH0_OUTPUT_COMBIN;
    TOL1 &= ~_0001_TAU_CH0_OUTPUT_LEVEL_L;
    TOE1 &= ~_0001_TAU_CH0_OUTPUT_ENABLE;
    /* Channel 2 used as interval timer */
    TMR12 = _0000_TAU_CLOCK_SELECT_CKM0 | _0000_TAU_CLOCK_MODE_CKS | _0000_TAU_MODE_INTERVAL_TIMER |
            _0001_TAU_START_INT_USED;
    TDR12 = _3E7F_TAU_TDR12_VALUE;
    TOM1 &= ~_0004_TAU_CH2_OUTPUT_COMBIN;
    TOL1 &= ~_0004_TAU_CH2_OUTPUT_LEVEL_L;
    TOE1 &= ~_0004_TAU_CH2_OUTPUT_ENABLE;
    /* Set noise filter sampling clock divisor and channels selected*/
    TNFSMP1 = _00_TAU1_NOISE_DIVISOR;
    TNFCS1 = _00_TAU1_NOISE_CHANNEL_SELECT;
}

/***********************************************************************************************************************
* Function Name: R_TAU1_Channel0_Start
* Description  : This function starts TAU1 channel 0 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU1_Channel0_Start(void)
{
    TMIF10 = 0U;        /* clear INTTM10 interrupt flag */
    TMMK10 = 0U;        /* enable INTTM10 interrupt */
    TS1 |= _0001_TAU_CH0_START_TRG_ON;
}

/***********************************************************************************************************************
* Function Name: R_TAU1_Channel0_Stop
* Description  : This function stops TAU1 channel 0 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU1_Channel0_Stop(void)
{
    TT1 |= _0001_TAU_CH0_STOP_TRG_ON;
    TMMK10 = 1U;        /* disable INTTM10 interrupt */
    TMIF10 = 0U;        /* clear INTTM10 interrupt flag */
}

/***********************************************************************************************************************
* Function Name: R_TAU1_Channel2_Start
* Description  : This function starts TAU1 channel 2 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU1_Channel2_Start(void)
{
    TMIF12 = 0U;        /* clear INTTM12 interrupt flag */
    TMMK12 = 0U;        /* enable INTTM12 interrupt */
    TS1 |= _0004_TAU_CH2_START_TRG_ON;
}

/***********************************************************************************************************************
* Function Name: R_TAU1_Channel2_Stop
* Description  : This function stops TAU1 channel 2 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU1_Channel2_Stop(void)
{
    TT1 |= _0004_TAU_CH2_STOP_TRG_ON;
    TMMK12 = 1U;        /* disable INTTM12 interrupt */
    TMIF12 = 0U;        /* clear INTTM12 interrupt flag */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
