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
* Copyright (C) 2012, 2021 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_timer.c
* Version      : Applilet3 for RL78/D1A V2.04.05.02 [08 Nov 2021]
* Device(s)    : R5F10DGD
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for TAU module.
* Creation Date: 11/07/2023
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
* Function Name: R_TAU0_Create
* Description  : This function initializes the TAU0 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU0_Create(void)
{
    TAU0EN = 1U;        /* supply input clock */
    TPS0 = _0000_TAU_CKM0_FCLK_0 | _0000_TAU_CKM1_FCLK_0 | _0000_TAU_CKM2_FCLK_0 | _0000_TAU_CKM3_FCLK_0;
    /* Stop all channels */
    TT0 = _0001_TAU_CH0_STOP_TRG_ON | _0002_TAU_CH1_STOP_TRG_ON | _0004_TAU_CH2_STOP_TRG_ON |
          _0008_TAU_CH3_STOP_TRG_ON | _0010_TAU_CH4_STOP_TRG_ON | _0020_TAU_CH5_STOP_TRG_ON |
          _0040_TAU_CH6_STOP_TRG_ON | _0080_TAU_CH7_STOP_TRG_ON;
    /* Mask channel 0 interrupt */
    TMMK00 = 1U;        /* disable INTTM00 interrupt */
    TMIF00 = 0U;        /* clear INTTM00 interrupt flag */
    /* Mask channel 1 interrupt */
    TMMK01 = 1U;        /* disable INTTM01 interrupt */
    TMIF01 = 0U;        /* clear INTTM01 interrupt flag */
    /* Set INTTM00 level 1 priority */
    TMPR100 = 0U;
    TMPR000 = 1U;
    /* Set INTTM01 low priority */
    TMPR101 = 1U;
    TMPR001 = 1U;
    /* Channel 0 used as interval timer */
    TMR00 = _0000_TAU_CLOCK_SELECT_CKM0 | _0000_TAU_CLOCK_MODE_CKS | _0000_TAU_MODE_INTERVAL_TIMER |
            _0001_TAU_START_INT_USED;
    TDR00 = _3E7F_TAU_TDR00_VALUE;
    TOM0 &= ~_0001_TAU_CH0_OUTPUT_COMBIN;
    TOL0 &= ~_0001_TAU_CH0_OUTPUT_LEVEL_L;
    TOE0 &= ~_0001_TAU_CH0_OUTPUT_ENABLE;
    /* Channel 1 used as external event counter */
    TMR01 = _0000_TAU_CLOCK_SELECT_CKM0 | _1000_TAU_CLOCK_MODE_TIMN | _0040_TAU_TIMN_EDGE_RISING |
            _0006_TAU_MODE_EVENT_COUNT;
    TDR01 = _0000_TAU_TDR01_VALUE;
    TOM0 &= ~_0002_TAU_CH1_OUTPUT_COMBIN;
    TOL0 &= ~_0002_TAU_CH1_OUTPUT_LEVEL_L;
    TOE0 &= ~_0002_TAU_CH1_OUTPUT_ENABLE;
    TNFEN0 |= _02_TAU_CH1_NOISE_ON;
    /* Set noise filter sampling clock divisor and channels selected*/
    TNFSMP0 = _02_TAU0_NOISE_DIVISOR;
    TNFCS0 = _00_TAU0_NOISE_CHANNEL_SELECT;
    /* Set TI01 pin */
    TIS00 |= 0x08U;
    PM9 |= 0x10U;
}

/***********************************************************************************************************************
* Function Name: R_TAU0_Channel0_Start
* Description  : This function starts TAU0 channel 0 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU0_Channel0_Start(void)
{
    TMIF00 = 0U;        /* clear INTTM00 interrupt flag */
    TMMK00 = 0U;        /* enable INTTM00 interrupt */
    TS0 |= _0001_TAU_CH0_START_TRG_ON;
}

/***********************************************************************************************************************
* Function Name: R_TAU0_Channel0_Stop
* Description  : This function stops TAU0 channel 0 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU0_Channel0_Stop(void)
{
    TT0 |= _0001_TAU_CH0_STOP_TRG_ON;
    TMMK00 = 1U;        /* disable INTTM00 interrupt */
    TMIF00 = 0U;        /* clear INTTM00 interrupt flag */
}

/***********************************************************************************************************************
* Function Name: R_TAU0_Channel1_Start
* Description  : This function starts TAU0 channel 1 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU0_Channel1_Start(void)
{
    TMIF01 = 0U;        /* clear INTTM01 interrupt flag */
    TMMK01 = 0U;        /* enable INTTM01 interrupt */
    TS0 |= _0002_TAU_CH1_START_TRG_ON;
}

/***********************************************************************************************************************
* Function Name: R_TAU0_Channel1_Stop
* Description  : This function stops TAU0 channel 1 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU0_Channel1_Stop(void)
{
    TT0 |= _0002_TAU_CH1_STOP_TRG_ON;
    TMMK01 = 1U;        /* disable INTTM01 interrupt */
    TMIF01 = 0U;        /* clear INTTM01 interrupt flag */
}

/***********************************************************************************************************************
* Function Name: R_TAU1_Create
* Description  : This function initializes the TAU1 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU1_Create(void)
{
    TAU1EN = 1U;        /* supply input clock */
    TPS1 = _0000_TAU_CKM0_FCLK_0 | _0050_TAU_CKM1_FCLK_5 | _0000_TAU_CKM2_FCLK_0 | _0000_TAU_CKM3_FCLK_0;
    /* Stop all channels */
    TT1 = _0001_TAU_CH0_STOP_TRG_ON | _0002_TAU_CH1_STOP_TRG_ON | _0004_TAU_CH2_STOP_TRG_ON |
          _0008_TAU_CH3_STOP_TRG_ON | _0010_TAU_CH4_STOP_TRG_ON | _0020_TAU_CH5_STOP_TRG_ON |
          _0040_TAU_CH6_STOP_TRG_ON | _0080_TAU_CH7_STOP_TRG_ON;
    /* Mask channel 0 interrupt */
    TMMK10 = 1U;        /* disable INTTM10 interrupt */
    TMIF10 = 0U;        /* clear INTTM10 interrupt flag */
    /* Set INTTM10 low priority */
    TMPR110 = 1U;
    TMPR010 = 1U;
    /* Channel 0 used as interval timer */
    TMR10 = _4000_TAU_CLOCK_SELECT_CKM1 | _0000_TAU_CLOCK_MODE_CKS | _0000_TAU_MODE_INTERVAL_TIMER |
            _0001_TAU_START_INT_USED;
    TDR10 = _C34F_TAU_TDR10_VALUE;
    TOM1 &= ~_0001_TAU_CH0_OUTPUT_COMBIN;
    TOL1 &= ~_0001_TAU_CH0_OUTPUT_LEVEL_L;
    TOE1 &= ~_0001_TAU_CH0_OUTPUT_ENABLE;
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
* Function Name: R_TAU2_Create
* Description  : This function initializes the TAU2 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU2_Create(void)
{
    TAU2EN = 1U;        /* supply input clock */
    TPS2 = _0000_TAU_CKM0_FCLK_0 | _0000_TAU_CKM1_FCLK_0 | _0000_TAU_CKM2_FCLK_0 | _0000_TAU_CKM3_FCLK_0;
    /* Stop all channels */
    TT2 = _0001_TAU_CH0_STOP_TRG_ON | _0002_TAU_CH1_STOP_TRG_ON | _0004_TAU_CH2_STOP_TRG_ON |
          _0008_TAU_CH3_STOP_TRG_ON | _0010_TAU_CH4_STOP_TRG_ON | _0020_TAU_CH5_STOP_TRG_ON |
          _0040_TAU_CH6_STOP_TRG_ON | _0080_TAU_CH7_STOP_TRG_ON;
    /* Mask channel 0 interrupt */
    TMMK20 = 1U;        /* disable INTTM20 interrupt */
    TMIF20 = 0U;        /* clear INTTM20 interrupt flag */
    /* Channel 0 is used as master channel for PWM output function */
    TMR20 = _0000_TAU_CLOCK_SELECT_CKM0 | _0000_TAU_CLOCK_MODE_CKS | _0800_TAU_COMBINATION_MASTER |
            _0000_TAU_TRIGGER_SOFTWARE | _0001_TAU_MODE_PWM_MASTER;
    TDR20 = _0199_TAU_TDR20_VALUE;
    TOM2 &= ~_0001_TAU_CH0_OUTPUT_COMBIN;
    TOL2 &= ~_0001_TAU_CH0_OUTPUT_LEVEL_L;
    TOE2 &= ~_0001_TAU_CH0_OUTPUT_ENABLE;
    /* Channel 5 is used as slave channel for PWM output function */
    TMR25 = _0000_TAU_CLOCK_SELECT_CKM0 | _0000_TAU_CLOCK_MODE_CKS | _0000_TAU_COMBINATION_SLAVE |
            _0400_TAU_TRIGGER_MASTER_INT | _0009_TAU_MODE_PWM_SLAVE;
    TDR25 = _0000_TAU_TDR25_VALUE;
    TOM2 |= _0020_TAU_CH5_OUTPUT_COMBIN;
    TOL2 &= ~_0020_TAU_CH5_OUTPUT_LEVEL_L;
    TO2 &= ~_0020_TAU_CH5_OUTPUT_VALUE_1;
    TOE2 |= _0020_TAU_CH5_OUTPUT_ENABLE;
    /* Set noise filter sampling clock divisor and channels selected*/
    TNFSMP2 = _00_TAU2_NOISE_DIVISOR;
    TNFCS2 = _00_TAU2_NOISE_CHANNEL_SELECT;
    /* Set TO25 pin */
    TOS21 |= 0x04U;
    P9 &= 0xFBU;
    PM9 &= 0xFBU;
}

/***********************************************************************************************************************
* Function Name: R_TAU2_Channel0_Start
* Description  : This function starts TAU2 channel 0 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU2_Channel0_Start(void)
{
    TOE2 |= _0020_TAU_CH5_OUTPUT_ENABLE;
    TS2 |= _0001_TAU_CH0_START_TRG_ON | _0020_TAU_CH5_START_TRG_ON;
}

/***********************************************************************************************************************
* Function Name: R_TAU2_Channel0_Stop
* Description  : This function stops TAU2 channel 0 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU2_Channel0_Stop(void)
{
    TT2 |= _0001_TAU_CH0_STOP_TRG_ON | _0020_TAU_CH5_STOP_TRG_ON;
    TOE2 &= ~_0020_TAU_CH5_OUTPUT_ENABLE;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
