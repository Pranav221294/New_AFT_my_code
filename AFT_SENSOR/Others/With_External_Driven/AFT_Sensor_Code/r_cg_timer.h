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
* File Name    : r_cg_timer.h
* Version      : Applilet3 for RL78/D1A V2.04.05.02 [08 Nov 2021]
* Device(s)    : R5F10DGD
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for TAU module.
* Creation Date: 25/10/2023
***********************************************************************************************************************/

#ifndef TAU_H
#define TAU_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/*
    Peripheral Enable Register 0 (PER0)
*/
/* Control of timer array unit input clock (TAU2EN) */
#define _00_TAU2_CLOCK_STOP                 (0x00U)    /* stop supply of input clock */
#define _04_TAU2_CLOCK_SUPPLY               (0x04U)    /* supply input clock */
/* Control of timer array unit input clock (TAU1EN) */
#define _00_TAU1_CLOCK_STOP                 (0x00U)    /* stop supply of input clock */
#define _02_TAU1_CLOCK_SUPPLY               (0x02U)    /* supply input clock */
/* Control of timer array unit input clock (TAU0EN) */
#define _00_TAU0_CLOCK_STOP                 (0x00U)    /* stop supply of input clock */
#define _01_TAU0_CLOCK_SUPPLY               (0x01U)    /* supply input clock */

/*
    Timer Clock Select Register m (TPSm)
*/
/* Operating mode and clear mode selection (PRSm33 - PRSm30) */
#define _0000_TAU_CKM3_FCLK_0               (0x0000U)    /* CKm3-fCLK */
#define _1000_TAU_CKM3_FCLK_1               (0x1000U)    /* CKm3-fCLK/2^1 */
#define _2000_TAU_CKM3_FCLK_2               (0x2000U)    /* CKm3-fCLK/2^2 */
#define _3000_TAU_CKM3_FCLK_3               (0x3000U)    /* CKm3-fCLK/2^3 */
#define _4000_TAU_CKM3_FCLK_4               (0x4000U)    /* CKm3-fCLK/2^4 */
#define _5000_TAU_CKM3_FCLK_5               (0x5000U)    /* CKm3-fCLK/2^5 */
#define _6000_TAU_CKM3_FCLK_6               (0x6000U)    /* CKm3-fCLK/2^6 */
#define _7000_TAU_CKM3_FCLK_7               (0x7000U)    /* CKm3-fCLK/2^7 */
#define _8000_TAU_CKM3_FCLK_8               (0x8000U)    /* CKm3-fCLK/2^8 */
#define _9000_TAU_CKM3_FCLK_9               (0x9000U)    /* CKm3-fCLK/2^9 */
#define _A000_TAU_CKM3_FCLK_10              (0xA000U)    /* CKm3-fCLK/2^10 */
#define _B000_TAU_CKM3_FCLK_11              (0xB000U)    /* CKm3-fCLK/2^11 */
#define _C000_TAU_CKM3_FCLK_12              (0xC000U)    /* CKm3-fCLK/2^12 */
#define _D000_TAU_CKM3_FCLK_13              (0xD000U)    /* CKm3-fCLK/2^13 */
#define _E000_TAU_CKM3_FCLK_14              (0xE000U)    /* CKm3-fCLK/2^14 */
#define _F000_TAU_CKM3_FCLK_15              (0xF000U)    /* CKm3-fCLK/2^15 */
/* Operating mode and clear mode selection (PRSm23 - PRSm20) */
#define _0000_TAU_CKM2_FCLK_0               (0x0000U)    /* CKm2-fCLK */
#define _0100_TAU_CKM2_FCLK_1               (0x0100U)    /* CKm2-fCLK/2^1 */
#define _0200_TAU_CKM2_FCLK_2               (0x0200U)    /* CKm2-fCLK/2^2 */
#define _0300_TAU_CKM2_FCLK_3               (0x0300U)    /* CKm2-fCLK/2^3 */
#define _0400_TAU_CKM2_FCLK_4               (0x0400U)    /* CKm2-fCLK/2^4 */
#define _0500_TAU_CKM2_FCLK_5               (0x0500U)    /* CKm2-fCLK/2^5 */
#define _0600_TAU_CKM2_FCLK_6               (0x0600U)    /* CKm2-fCLK/2^6 */
#define _0700_TAU_CKM2_FCLK_7               (0x0700U)    /* CKm2-fCLK/2^7 */
#define _0800_TAU_CKM2_FCLK_8               (0x0800U)    /* CKm2-fCLK/2^8 */
#define _0900_TAU_CKM2_FCLK_9               (0x0900U)    /* CKm2-fCLK/2^9 */
#define _0A00_TAU_CKM2_FCLK_10              (0x0A00U)    /* CKm2-fCLK/2^10 */
#define _0B00_TAU_CKM2_FCLK_11              (0x0B00U)    /* CKm2-fCLK/2^11 */
#define _0C00_TAU_CKM2_FCLK_12              (0x0C00U)    /* CKm2-fCLK/2^12 */
#define _0D00_TAU_CKM2_FCLK_13              (0x0D00U)    /* CKm2-fCLK/2^13 */
#define _0E00_TAU_CKM2_FCLK_14              (0x0E00U)    /* CKm2-fCLK/2^14 */
#define _0F00_TAU_CKM2_FCLK_15              (0x0F00U)    /* CKm2-fCLK/2^15 */
/* Operating mode and clear mode selection (PRSm13 - PRSm10) */
#define _0000_TAU_CKM1_FCLK_0               (0x0000U)    /* CKm1-fCLK */
#define _0010_TAU_CKM1_FCLK_1               (0x0010U)    /* CKm1-fCLK/2^1 */
#define _0020_TAU_CKM1_FCLK_2               (0x0020U)    /* CKm1-fCLK/2^2 */
#define _0030_TAU_CKM1_FCLK_3               (0x0030U)    /* CKm1-fCLK/2^3 */
#define _0040_TAU_CKM1_FCLK_4               (0x0040U)    /* CKm1-fCLK/2^4 */
#define _0050_TAU_CKM1_FCLK_5               (0x0050U)    /* CKm1-fCLK/2^5 */
#define _0060_TAU_CKM1_FCLK_6               (0x0060U)    /* CKm1-fCLK/2^6 */
#define _0070_TAU_CKM1_FCLK_7               (0x0070U)    /* CKm1-fCLK/2^7 */
#define _0080_TAU_CKM1_FCLK_8               (0x0080U)    /* CKm1-fCLK/2^8 */
#define _0090_TAU_CKM1_FCLK_9               (0x0090U)    /* CKm1-fCLK/2^9 */
#define _00A0_TAU_CKM1_FCLK_10              (0x00A0U)    /* CKm1-fCLK/2^10 */
#define _00B0_TAU_CKM1_FCLK_11              (0x00B0U)    /* CKm1-fCLK/2^11 */
#define _00C0_TAU_CKM1_FCLK_12              (0x00C0U)    /* CKm1-fCLK/2^12 */
#define _00D0_TAU_CKM1_FCLK_13              (0x00D0U)    /* CKm1-fCLK/2^13 */
#define _00E0_TAU_CKM1_FCLK_14              (0x00E0U)    /* CKm1-fCLK/2^14 */
#define _00F0_TAU_CKM1_FCLK_15              (0x00F0U)    /* CKm1-fCLK/2^15 */
/* Operating mode and clear mode selection (PRSm03 - PRSm00) */
#define _0000_TAU_CKM0_FCLK_0               (0x0000U)    /* CKm0-fCLK */
#define _0001_TAU_CKM0_FCLK_1               (0x0001U)    /* CKm0-fCLK/2^1 */
#define _0002_TAU_CKM0_FCLK_2               (0x0002U)    /* CKm0-fCLK/2^2 */
#define _0003_TAU_CKM0_FCLK_3               (0x0003U)    /* CKm0-fCLK/2^3 */
#define _0004_TAU_CKM0_FCLK_4               (0x0004U)    /* CKm0-fCLK/2^4 */
#define _0005_TAU_CKM0_FCLK_5               (0x0005U)    /* CKm0-fCLK/2^5 */
#define _0006_TAU_CKM0_FCLK_6               (0x0006U)    /* CKm0-fCLK/2^6 */
#define _0007_TAU_CKM0_FCLK_7               (0x0007U)    /* CKm0-fCLK/2^7 */
#define _0008_TAU_CKM0_FCLK_8               (0x0008U)    /* CKm0-fCLK/2^8 */
#define _0009_TAU_CKM0_FCLK_9               (0x0009U)    /* CKm0-fCLK/2^9 */
#define _000A_TAU_CKM0_FCLK_10              (0x000AU)    /* CKm0-fCLK/2^10 */
#define _000B_TAU_CKM0_FCLK_11              (0x000BU)    /* CKm0-fCLK/2^11 */
#define _000C_TAU_CKM0_FCLK_12              (0x000CU)    /* CKm0-fCLK/2^12 */
#define _000D_TAU_CKM0_FCLK_13              (0x000DU)    /* CKm0-fCLK/2^13 */
#define _000E_TAU_CKM0_FCLK_14              (0x000EU)    /* CKm0-fCLK/2^14 */
#define _000F_TAU_CKM0_FCLK_15              (0x000FU)    /* CKm0-fCLK/2^15 */

/*
    Timer Mode Register mn (TMRmn)
*/
/* Selection of macro clock (MCK) of channel n (CKSmn1,CKSmn0) */
#define _C000_TAU_CLOCK_SELECT_CLR          (0xC000U)    /* clear the bits */
#define _0000_TAU_CLOCK_SELECT_CKM0         (0x0000U)    /* operation clock CKm0 set by TPSm register */
#define _4000_TAU_CLOCK_SELECT_CKM1         (0x4000U)    /* operation clock CKm1 set by TPSm register */
#define _8000_TAU_CLOCK_SELECT_CKM2         (0x8000U)    /* operation clock CKm2 set by TPSm register */
#define _C000_TAU_CLOCK_SELECT_CKM3         (0xC000U)    /* operation clock CKm3 set by TPSm register */
/* Selection of count clock (TCLK) of channel n (CCSmn) */
#define _1000_TAU_CLOCK_MODE_CLR            (0x1000U)    /* clear the bits */
#define _0000_TAU_CLOCK_MODE_CKS            (0x0000U)    /* macro clock MCK specified by CKSmn bit */
#define _1000_TAU_CLOCK_MODE_TIMN           (0x1000U)    /* valid edge of input signal input from TImn pin */
/* Selection of slave/master of channel n (MASTERmn) */
#define _0800_TAU_COMBINATION_CLR           (0x0800U)    /* clear the bits */
#define _0000_TAU_COMBINATION_SLAVE         (0x0000U)    /* operate as slave channel with combination operation function */
#define _0800_TAU_COMBINATION_MASTER        (0x0800U)    /* operate as master channel with combination operation function */
/* Setting of start trigger or capture trigger of channel n (STSmn2 - STSmn0) */
#define _0700_TAU_TRIGGER_CLR               (0x0700U)    /* clear the bits */
#define _0000_TAU_TRIGGER_SOFTWARE          (0x0000U)    /* only software trigger start is valid */
#define _0100_TAU_TRIGGER_TIMN_VALID        (0x0100U)    /* valid edge of TImn pin input is used as both the start trigger and capture trigger */
#define _0200_TAU_TRIGGER_TIMN_BOTH         (0x0200U)    /* both the edges of TImn pin input are used as a start trigger and a capture trigger */
#define _0400_TAU_TRIGGER_MASTER_INT        (0x0400U)    /* interrupt signal of the master channel is used */
/* Selection of TImn pin input valid edge (CISmn1, CISmn0) */
#define _00C0_TAU_TIMN_EDGE_CLR             (0x00C0U)    /* clear the bits */
#define _0000_TAU_TIMN_EDGE_FALLING         (0x0000U)    /* falling edge */
#define _0040_TAU_TIMN_EDGE_RISING          (0x0040U)    /* rising edge */
#define _0080_TAU_TIMN_EDGE_BOTH_LOW        (0x0080U)    /* both edges (when low-level width is measured) */
#define _00C0_TAU_TIMN_EDGE_BOTH_HIGH       (0x00C0U)    /* both edges (when high-level width is measured) */
/* Operation mode of channel n (MDmn3, MDmn0) */
#define _000F_TAU_MODE_CLR                  (0x000FU)    /* clear the bits */
#define _0000_TAU_MODE_INTERVAL_TIMER       (0x0000U)    /* interval timer mode */
#define _0006_TAU_MODE_EVENT_COUNT          (0x0006U)    /* event counter mode */
#define _0004_TAU_MODE_CAPTURE              (0x0004U)    /* capture mode */
#define _000C_TAU_MODE_HIGHLOW_MEASURE      (0x000CU)    /* high-/low-level width measurement mode */
#define _0001_TAU_MODE_PWM_MASTER           (0x0001U)    /* PWM Function (Master Channel) mode */
#define _0009_TAU_MODE_PWM_SLAVE            (0x0009U)    /* PWM Function (Slave Channel) mode */
#define _0008_TAU_MODE_ONESHOT              (0x0008U)    /* one-shot pulse output mode */
/* Setting of starting counting and interrupt (MDmn0) */
#define _0001_TAU_START_INT_CLR             (0x0001U)    /* clear the bits */
#define _0000_TAU_START_INT_UNUSED          (0x0000U)    /* timer interrupt is not generated when counting is started */
#define _0001_TAU_START_INT_USED            (0x0001U)    /* timer interrupt is generated when counting is started */

/*
    Timer Status Register mn (TSRmn)
*/
/* Counter overflow status of channel n (OVF) */
#define _0001_TAU_OVERFLOW_STATUS           (0x0001U)
#define _0000_TAU_OVERFLOW_NOT_OCCURS       (0x0000U)    /* overflow does not occur */
#define _0001_TAU_OVERFLOW_OCCURS           (0x0001U)    /* overflow occurs */

/*
    Timer Channel Enable Status Register m (TEm)
*/
/* Indication of operation enable/stop status of channel 7 (TEm7) */
#define _0000_TAU_CH7_OPERATION_STOP        (0x0000U)    /* stop operation */
#define _0080_TAU_CH7_OPERATION_ENABLE      (0x0080U)    /* enable operation */
/* Indication of operation enable/stop status of channel 6 (TEm6) */
#define _0000_TAU_CH6_OPERATION_STOP        (0x0000U)    /* stop operation */
#define _0040_TAU_CH6_OPERATION_ENABLE      (0x0040U)    /* enable operation */
/* Indication of operation enable/stop status of channel 5 (TEm5) */
#define _0000_TAU_CH5_OPERATION_STOP        (0x0000U)    /* stop operation */
#define _0020_TAU_CH5_OPERATION_ENABLE      (0x0020U)    /* enable operation */
/* Indication of operation enable/stop status of channel 4 (TEm4) */
#define _0000_TAU_CH4_OPERATION_STOP        (0x0000U)    /* stop operation */
#define _0010_TAU_CH4_OPERATION_ENABLE      (0x0010U)    /* enable operation */
/* Indication of operation enable/stop status of channel 3 (TEm3) */
#define _0000_TAU_CH3_OPERATION_STOP        (0x0000U)    /* stop operation */
#define _0008_TAU_CH3_OPERATION_ENABLE      (0x0008U)    /* enable operation */
/* Indication of operation enable/stop status of channel 2 (TEm2) */
#define _0000_TAU_CH2_OPERATION_STOP        (0x0000U)    /* stop operation */
#define _0004_TAU_CH2_OPERATION_ENABLE      (0x0004U)    /* enable operation */
/* Indication of operation enable/stop status of channel 1 (TEm1) */
#define _0000_TAU_CH1_OPERATION_STOP        (0x0000U)    /* stop operation */
#define _0002_TAU_CH1_OPERATION_ENABLE      (0x0002U)    /* enable operation */
/* Indication of operation enable/stop status of channel 0 (TEm0) */
#define _0000_TAU_CH0_OPERATION_STOP        (0x0000U)    /* stop operation */
#define _0001_TAU_CH0_OPERATION_ENABLE      (0x0001U)    /* enable operation */

/*
    Timer Channel Start Register m (TSm)
*/
/* Operation enable (start) trigger of channel 7 (TSm7) */
#define _0000_TAU_CH7_START_TRG_OFF         (0x0000U)    /* no trigger operation */
#define _0080_TAU_CH7_START_TRG_ON          (0x0080U)    /* enable operation (generate start software trigger) */
/* Operation enable (start) trigger of channel 6 (TSm6) */
#define _0000_TAU_CH6_START_TRG_OFF         (0x0000U)    /* no trigger operation */
#define _0040_TAU_CH6_START_TRG_ON          (0x0040U)    /* enable operation (generate start software trigger) */
/* Operation enable (start) trigger of channel 5 (TSm5) */
#define _0000_TAU_CH5_START_TRG_OFF         (0x0000U)    /* no trigger operation */
#define _0020_TAU_CH5_START_TRG_ON          (0x0020U)    /* enable operation (generate start software trigger) */
/* Operation enable (start) trigger of channel 4 (TSm4) */
#define _0000_TAU_CH4_START_TRG_OFF         (0x0000U)    /* no trigger operation */
#define _0010_TAU_CH4_START_TRG_ON          (0x0010U)    /* enable operation (generate start software trigger) */
/* Operation enable (start) trigger of channel 3 (TSm3) */
#define _0000_TAU_CH3_START_TRG_OFF         (0x0000U)    /* no trigger operation */
#define _0008_TAU_CH3_START_TRG_ON          (0x0008U)    /* enable operation (generate start software trigger) */
/* Operation enable (start) trigger of channel 2 (TSm2) */
#define _0000_TAU_CH2_START_TRG_OFF         (0x0000U)    /* no trigger operation */
#define _0004_TAU_CH2_START_TRG_ON          (0x0004U)    /* enable operation (generate start software trigger) */
/* Operation enable (start) trigger of channel 1 (TSm1) */
#define _0000_TAU_CH1_START_TRG_OFF         (0x0000U)    /* no trigger operation */
#define _0002_TAU_CH1_START_TRG_ON          (0x0002U)    /* enable operation (generate start software trigger) */
/* Operation enable (start) trigger of channel 0 (TSm0) */
#define _0000_TAU_CH0_START_TRG_OFF         (0x0000U)    /* no trigger operation */
#define _0001_TAU_CH0_START_TRG_ON          (0x0001U)    /* enable operation (generate start software trigger) */

/*
    Timer Channel Stop Register m (TTm)
*/
/* Operation stop trigger of channel 7 (TTm7) */
#define _0000_TAU_CH7_STOP_TRG_OFF          (0x0000U)    /* no trigger operation */
#define _0080_TAU_CH7_STOP_TRG_ON           (0x0080U)    /* stop operation (generate stop trigger) */
/* Operation stop trigger of channel 6 (TTm6) */
#define _0000_TAU_CH6_STOP_TRG_OFF          (0x0000U)    /* no trigger operation */
#define _0040_TAU_CH6_STOP_TRG_ON           (0x0040U)    /* stop operation (generate stop trigger) */
/* Operation stop trigger of channel 5 (TTm5) */
#define _0000_TAU_CH5_STOP_TRG_OFF          (0x0000U)    /* no trigger operation */
#define _0020_TAU_CH5_STOP_TRG_ON           (0x0020U)    /* stop operation (generate stop trigger) */
/* Operation stop trigger of channel 4 (TTm4) */
#define _0000_TAU_CH4_STOP_TRG_OFF          (0x0000U)    /* no trigger operation */
#define _0010_TAU_CH4_STOP_TRG_ON           (0x0010U)    /* stop operation (generate stop trigger) */
/* Operation stop trigger of channel 3 (TTm3) */
#define _0000_TAU_CH3_STOP_TRG_OFF          (0x0000U)    /* no trigger operation */
#define _0008_TAU_CH3_STOP_TRG_ON           (0x0008U)    /* stop operation (generate stop trigger) */
/* Operation stop trigger of channel 2 (TTm2) */
#define _0000_TAU_CH2_STOP_TRG_OFF          (0x0000U)    /* no trigger operation */
#define _0004_TAU_CH2_STOP_TRG_ON           (0x0004U)    /* stop operation (generate stop trigger) */
/* Operation stop trigger of channel 1 (TTm1) */
#define _0000_TAU_CH1_STOP_TRG_OFF          (0x0000U)    /* no trigger operation */
#define _0002_TAU_CH1_STOP_TRG_ON           (0x0002U)    /* stop operation (generate stop trigger) */
/* Operation stop trigger of channel 0 (TTm0) */
#define _0000_TAU_CH0_STOP_TRG_OFF          (0x0000U)    /* no trigger operation */
#define _0001_TAU_CH0_STOP_TRG_ON           (0x0001U)    /* stop operation (generate stop trigger) */

/*
    Timer Output Enable Register m (TOEm)
*/
/* Timer output enable/disable of channel 7 (TOEm7) */
#define _0080_TAU_CH7_OUTPUT_ENABLE         (0x0080U)    /* enable TOm7 operation by count operation */
#define _0000_TAU_CH7_OUTPUT_DISABLE        (0x0000U)    /* stop TOm7 operation by count operation */
/* Timer output enable/disable of channel 6 (TOEm6) */
#define _0040_TAU_CH6_OUTPUT_ENABLE         (0x0040U)    /* enable TOm7 operation by count operation */
#define _0000_TAU_CH6_OUTPUT_DISABLE        (0x0000U)    /* stop TOm7 operation by count operation */
/* Timer output enable/disable of channel 5 (TOEm5) */
#define _0020_TAU_CH5_OUTPUT_ENABLE         (0x0020U)    /* enable TOm7 operation by count operation */
#define _0000_TAU_CH5_OUTPUT_DISABLE        (0x0000U)    /* stop TOm7 operation by count operation */
/* Timer output enable/disable of channel 4 (TOEm4) */
#define _0010_TAU_CH4_OUTPUT_ENABLE         (0x0010U)    /* enable TOm7 operation by count operation */
#define _0000_TAU_CH4_OUTPUT_DISABLE        (0x0000U)    /* stop TOm7 operation by count operation */
/* Timer output enable/disable of channel 3 (TOEm3) */
#define _0008_TAU_CH3_OUTPUT_ENABLE         (0x0008U)    /* enable TOm7 operation by count operation */
#define _0000_TAU_CH3_OUTPUT_DISABLE        (0x0000U)    /* stop TOm7 operation by count operation */
/* Timer output enable/disable of channel 2 (TOEm2) */
#define _0004_TAU_CH2_OUTPUT_ENABLE         (0x0004U)    /* enable TOm7 operation by count operation */
#define _0000_TAU_CH2_OUTPUT_DISABLE        (0x0000U)    /* stop TOm7 operation by count operation */
/* Timer output enable/disable of channel 1 (TOEm1) */
#define _0002_TAU_CH1_OUTPUT_ENABLE         (0x0002U)    /* enable TOm7 operation by count operation */
#define _0000_TAU_CH1_OUTPUT_DISABLE        (0x0000U)    /* stop TOm7 operation by count operation */
/* Timer output enable/disable of channel 0 (TOEm0) */
#define _0001_TAU_CH0_OUTPUT_ENABLE         (0x0001U)    /* enable TOm7 operation by count operation */
#define _0000_TAU_CH0_OUTPUT_DISABLE        (0x0000U)    /* stop TOm7 operation by count operation */

/*
    Timer Output Register m (TOm)
*/
/* Timer output of channel 7 (TOm7) */
#define _0000_TAU_CH7_OUTPUT_VALUE_0        (0x0000U)    /* timer output value is "0" */
#define _0080_TAU_CH7_OUTPUT_VALUE_1        (0x0080U)    /* timer output value is "1" */
/* Timer output of channel 6 (TOm6) */
#define _0000_TAU_CH6_OUTPUT_VALUE_0        (0x0000U)    /* timer output value is "0" */
#define _0040_TAU_CH6_OUTPUT_VALUE_1        (0x0040U)    /* timer output value is "1" */
/* Timer output of channel 5 (TOm5) */
#define _0000_TAU_CH5_OUTPUT_VALUE_0        (0x0000U)    /* timer output value is "0" */
#define _0020_TAU_CH5_OUTPUT_VALUE_1        (0x0020U)    /* timer output value is "1" */
/* Timer output of channel 4 (TOm4) */
#define _0000_TAU_CH4_OUTPUT_VALUE_0        (0x0000U)    /* timer output value is "0" */
#define _0010_TAU_CH4_OUTPUT_VALUE_1        (0x0010U)    /* timer output value is "1" */
/* Timer output of channel 3 (TOm3) */
#define _0000_TAU_CH3_OUTPUT_VALUE_0        (0x0000U)    /* timer output value is "0" */
#define _0008_TAU_CH3_OUTPUT_VALUE_1        (0x0008U)    /* timer output value is "1" */
/* Timer output of channel 2 (TOm2) */
#define _0000_TAU_CH2_OUTPUT_VALUE_0        (0x0000U)    /* timer output value is "0" */
#define _0004_TAU_CH2_OUTPUT_VALUE_1        (0x0004U)    /* timer output value is "1" */
/* Timer output of channel 1 (TOm1) */
#define _0000_TAU_CH1_OUTPUT_VALUE_0        (0x0000U)    /* timer output value is "0" */
#define _0002_TAU_CH1_OUTPUT_VALUE_1        (0x0002U)    /* timer output value is "1" */
/* Timer output of channel 0 (TOm0) */
#define _0000_TAU_CH0_OUTPUT_VALUE_0        (0x0000U)    /* timer output value is "0" */
#define _0001_TAU_CH0_OUTPUT_VALUE_1        (0x0001U)    /* timer output value is "1" */

/*
    Timer Output Level Register 0 (TOLm)
*/
/* Control of timer output level of channel 7 (TOLm7) */
#define _0000_TAU_CH7_OUTPUT_LEVEL_H        (0x0000U)    /* positive logic output (active-high) */
#define _0080_TAU_CH7_OUTPUT_LEVEL_L        (0x0080U)    /* inverted output (active-low) */
/* Control of timer output level of channel 6 (TOLm6) */
#define _0000_TAU_CH6_OUTPUT_LEVEL_H        (0x0000U)    /* positive logic output (active-high) */
#define _0040_TAU_CH6_OUTPUT_LEVEL_L        (0x0040U)    /* inverted output (active-low) */
/* Control of timer output level of channel 5 (TOLm5) */
#define _0000_TAU_CH5_OUTPUT_LEVEL_H        (0x0000U)    /* positive logic output (active-high) */
#define _0020_TAU_CH5_OUTPUT_LEVEL_L        (0x0020U)    /* inverted output (active-low) */
/* Control of timer output level of channel 4 (TOLm4) */
#define _0000_TAU_CH4_OUTPUT_LEVEL_H        (0x0000U)    /* positive logic output (active-high) */
#define _0010_TAU_CH4_OUTPUT_LEVEL_L        (0x0010U)    /* inverted output (active-low) */
/* Control of timer output level of channel 3 (TOLm3) */
#define _0000_TAU_CH3_OUTPUT_LEVEL_H        (0x0000U)    /* positive logic output (active-high) */
#define _0008_TAU_CH3_OUTPUT_LEVEL_L        (0x0008U)    /* inverted output (active-low) */
/* Control of timer output level of channel 2 (TOLm2) */
#define _0000_TAU_CH2_OUTPUT_LEVEL_H        (0x0000U)    /* positive logic output (active-high) */
#define _0004_TAU_CH2_OUTPUT_LEVEL_L        (0x0004U)    /* inverted output (active-low) */
/* Control of timer output level of channel 1 (TOLm1) */
#define _0000_TAU_CH1_OUTPUT_LEVEL_H        (0x0000U)    /* positive logic output (active-high) */
#define _0002_TAU_CH1_OUTPUT_LEVEL_L        (0x0002U)    /* inverted output (active-low) */
/* Control of timer output level of channel 0 (TOLm0) */
#define _0000_TAU_CH0_OUTPUT_LEVEL_H        (0x0000U)    /* positive logic output (active-high) */
#define _0001_TAU_CH0_OUTPUT_LEVEL_L        (0x0001U)    /* inverted output (active-low) */

/*
    Timer Output Mode Register m (TOMm)
*/
/* Control of timer output mode of channel 7 (TOMm7) */
#define _0000_TAU_CH7_OUTPUT_TOGGLE         (0x0000U)    /* toggle operation mode */
#define _0080_TAU_CH7_OUTPUT_COMBIN         (0x0080U)    /* combination operation mode */
/* Control of timer output mode of channel 6 (TOMm6) */
#define _0000_TAU_CH6_OUTPUT_TOGGLE         (0x0000U)    /* toggle operation mode */
#define _0040_TAU_CH6_OUTPUT_COMBIN         (0x0040U)    /* combination operation mode */
/* Control of timer output mode of channel 5 (TOMm5) */
#define _0000_TAU_CH5_OUTPUT_TOGGLE         (0x0000U)    /* toggle operation mode */
#define _0020_TAU_CH5_OUTPUT_COMBIN         (0x0020U)    /* combination operation mode */
/* Control of timer output mode of channel 4 (TOMm4) */
#define _0000_TAU_CH4_OUTPUT_TOGGLE         (0x0000U)    /* toggle operation mode */
#define _0010_TAU_CH4_OUTPUT_COMBIN         (0x0010U)    /* combination operation mode */
/* Control of timer output mode of channel 3 (TOMm3) */
#define _0000_TAU_CH3_OUTPUT_TOGGLE         (0x0000U)    /* toggle operation mode */
#define _0008_TAU_CH3_OUTPUT_COMBIN         (0x0008U)    /* combination operation mode */
/* Control of timer output mode of channel 2 (TOMm2) */
#define _0000_TAU_CH2_OUTPUT_TOGGLE         (0x0000U)    /* toggle operation mode */
#define _0004_TAU_CH2_OUTPUT_COMBIN         (0x0004U)    /* combination operation mode */
/* Control of timer output mode of channel 1 (TOMm1) */
#define _0000_TAU_CH1_OUTPUT_TOGGLE         (0x0000U)    /* toggle operation mode */
#define _0002_TAU_CH1_OUTPUT_COMBIN         (0x0002U)    /* combination operation mode */
/* Control of timer output mode of channel 0 (TOMm0) */
#define _0000_TAU_CH0_OUTPUT_TOGGLE         (0x0000U)    /* toggle operation mode */
#define _0001_TAU_CH0_OUTPUT_COMBIN         (0x0001U)    /* combination operation mode */

/*
    Noise Filter Enable Register 0~2 (TNFEN0~2)
*/
/* Enable/disable using noise filter of TIm7 pin input signal (TNFENm7) */
#define _80_TAU_CH7_NOISE_CLR               (0x80U)    /* clear the bits */
#define _00_TAU_CH7_NOISE_OFF               (0x00U)    /* noise filter OFF */
#define _80_TAU_CH7_NOISE_ON                (0x80U)    /* noise filter ON */
/* Enable/disable using noise filter of TIm6 pin input signal (TNFENm6) */
#define _40_TAU_CH6_NOISE_CLR               (0x40U)    /* clear the bits */
#define _00_TAU_CH6_NOISE_OFF               (0x00U)    /* noise filter OFF */
#define _40_TAU_CH6_NOISE_ON                (0x40U)    /* noise filter ON */
/* Enable/disable using noise filter of TIm5 pin input signal (TNFENm5) */
#define _20_TAU_CH5_NOISE_CLR               (0x20U)    /* clear the bits */
#define _00_TAU_CH5_NOISE_OFF               (0x00U)    /* noise filter OFF */
#define _20_TAU_CH5_NOISE_ON                (0x20U)    /* noise filter ON */
/* Enable/disable using noise filter of TIm4 pin input signal (TNFENm4) */
#define _10_TAU_CH4_NOISE_CLR               (0x10U)    /* clear the bits */
#define _00_TAU_CH4_NOISE_OFF               (0x00U)    /* noise filter OFF */
#define _10_TAU_CH4_NOISE_ON                (0x10U)    /* noise filter ON */
/* Enable/disable using noise filter of TIm3 pin input signal (TNFENm3) */
#define _08_TAU_CH3_NOISE_CLR               (0x08U)    /* clear the bits */
#define _00_TAU_CH3_NOISE_OFF               (0x00U)    /* noise filter OFF */
#define _08_TAU_CH3_NOISE_ON                (0x08U)    /* noise filter ON */
/* Enable/disable using noise filter of TIm2 pin input signal (TNFENm2) */
#define _04_TAU_CH2_NOISE_CLR               (0x04U)    /* clear the bits */
#define _00_TAU_CH2_NOISE_OFF               (0x00U)    /* noise filter OFF */
#define _04_TAU_CH2_NOISE_ON                (0x04U)    /* noise filter ON */
/* Enable/disable using noise filter of TIm1 pin input signal (TNFENm1) */
#define _02_TAU_CH1_NOISE_CLR               (0x02U)    /* clear the bits */
#define _00_TAU_CH1_NOISE_OFF               (0x00U)    /* noise filter OFF */
#define _02_TAU_CH1_NOISE_ON                (0x02U)    /* noise filter ON */
/* Enable/disable using noise filter of TIm0 pin input signal (TNFENm0) */
#define _01_TAU_CH0_NOISE_CLR               (0x01U)    /* clear the bits */
#define _00_TAU_CH0_NOISE_OFF               (0x00U)    /* noise filter OFF */
#define _01_TAU_CH0_NOISE_ON                (0x01U)    /* noise filter ON */

/*
    Timer input select else register (TISELSE)
*/
/* TAU unit0 CH5 input alternate selection (TI05SEL1 - TI05SEL0) */
#define _00_TAU0_CH5_TI05                   (0x00U)    /* Use TI05 as selectetd by TIS051 - TIS050 */
#define _01_TAU0_CH5_FIL                    (0x01U)    /* Low-speed on-chip oscillator clock (fIL) */
#define _02_TAU0_CH5_FSUB                   (0x02U)    /* Sub system clock (fSUB) */
#define _03_TAU0_CH5_FEX                    (0x03U)    /* Main external clock (fEX) */
/* Timer conjunction function control (TOTICON0) */
#define _00_TAU0_CH1_TO_CH0_CUTOFF          (0x00U)    /* Cut off the connection of TAU unit0 CH1 output to CH0 input */
#define _40_TAU0_CH1_TO_CH0_CONNECT         (0x40U)    /* Connect TAU unit0 CH1 output to TAU unit0 CH0 input */
/* Timer conjunction function control (TOTICON1) */
#define _00_TAU2_CH1_TO_CH0_CUTOFF          (0x00U)    /* Cut off the connection of TAU unit2 CH1 output to CH0 input */
#define _80_TAU2_CH1_TO_CH0_CONNECT         (0x80U)    /* Connect TAU unit2 CH1 output to TAU unit2 CH0 input */

/*
    RTC1Hz pin select register (RTCSEL)
*/
/* RTC1Hz output pin selection (RTCTIS11 - RTCTIS00) */
#define _00_TAU_RTC_TI06_DISCONNECTED       (0x00U)    /* Disconnected to TI06 */
#define _01_TAU_RTC_TI06_CONNECTED          (0x01U)    /* Connected to TI06 */
#define _00_TAU_RTC_TI07_DISCONNECTED       (0x00U)    /* Disconnected to TI07 */
#define _02_TAU_RTC_TI07_CONNECTED          (0x02U)    /* Connected to TI07 */
#define _00_TAU_RTC_TI16_DISCONNECTED       (0x00U)    /* Disconnected to TI16 */
#define _04_TAU_RTC_TI16_CONNECTED          (0x04U)    /* Connected to TI16 */
#define _00_TAU_RTC_TI17_DISCONNECTED       (0x00U)    /* Disconnected to TI17 */
#define _08_TAU_RTC_TI17_CONNECTED          (0x08U)    /* Connected to TI17 */

/*
    Serial communication pin select register (STSEL1)
*/
/* Input source switch of TAU unit1 CH4 (TMCAN0) */
#define _00_TAU1_CH4_TI14                   (0x00U)    /* Input from TI14 (after selected by TIS141 to 0 bits) */
#define _01_TAU1_CH4_TSOUT                  (0x01U)    /* TSOUT of aFCAN0 (CAN0 time stamp function) */
/* Input source switch of TAU unit1 CH5 (TMCAN1) */
#define _00_TAU1_CH5_TI15                   (0x00U)    /* Input from TI15 (after selected by TIS151 to 0 bits) */
#define _02_TAU1_CH5_TSOUT                  (0x02U)    /* TSOUT of aFCAN1 (CAN1 time stamp function) */


/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* 16-bit timer data register 00 (TDR00) */
#define _3E7F_TAU_TDR00_VALUE               (0x3E7FU)
/* Clock divisor for TAU0 channel0 */
#define _1_TAU0_CHANNEL0_DIVISOR            (1U)
/* 16-bit timer data register 01 (TDR01) */
#define _000F_TAU_TDR01_VALUE               (0x000FU)
/* Clock divisor for TAU0 channel1 */
#define _1_TAU0_CHANNEL1_DIVISOR            (1U)
/* Clock divisor for noise filter */
#define _00_TAU0_NOISE_DIVISOR              (0x00U)
/* Set channels that use noise filter sampling clock */
#define _00_TAU0_NOISE_CHANNEL_SELECT       (0x00U)
/* 16-bit timer data register 10 (TDR10) */
#define _C34F_TAU_TDR10_VALUE               (0xC34FU)
/* Clock divisor for TAU1 channel0 */
#define _8_TAU1_CHANNEL0_DIVISOR            (8U)
/* Clock divisor for noise filter */
#define _00_TAU1_NOISE_DIVISOR              (0x00U)
/* Set channels that use noise filter sampling clock */
#define _00_TAU1_NOISE_CHANNEL_SELECT       (0x00U)
/* 16-bit timer data register 20 (TDR20) */
#define _047F_TAU_TDR20_VALUE               (0x047FU)
/* 16-bit timer data register 25 (TDR25) */
#define _0000_TAU_TDR25_VALUE               (0x0000U)
/* Clock divisor for TAU2 channel0 */
#define _1_TAU2_CHANNEL0_DIVISOR            (1U)
/* Clock divisor for TAU2 channel0 */
#define _1_TAU2_CHANNEL5_DIVISOR            (1U)
/* Clock divisor for noise filter */
#define _00_TAU2_NOISE_DIVISOR              (0x00U)
/* Set channels that use noise filter sampling clock */
#define _00_TAU2_NOISE_CHANNEL_SELECT       (0x00U)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_TAU0_Create(void);
void R_TAU0_Channel0_Start(void);
void R_TAU0_Channel0_Stop(void);
void R_TAU0_Channel1_Start(void);
void R_TAU0_Channel1_Stop(void);
void R_TAU1_Create(void);
void R_TAU1_Channel0_Start(void);
void R_TAU1_Channel0_Stop(void);
void R_TAU2_Create(void);
void R_TAU2_Channel0_Start(void);
void R_TAU2_Channel0_Stop(void);

/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif
