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
* Copyright (C) 2012, 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_timer_user.c
* Version      : Applilet3 for RL78/D1A V2.04.02.01 [21 Mar 2018]
* Device(s)    : R5F10CGB
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for TAU module.
* Creation Date: 18/07/2022
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
//#pragma interrupt INTTM00 r_tau0_channel0_interrupt
//#pragma interrupt INTTM01 r_tau0_channel1_interrupt
//#pragma interrupt INTTM02 r_tau0_channel2_interrupt
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_timer.h"
/* Start user code for include. Do not edit comment generated here */
#include "CustomDataTypes.h"
#include "User.h"
#include "Calibration.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* For TAU0_ch1 pulse measurement */
volatile uint32_t g_tau0_ch1_width = 0UL;

/* Start user code for global. Do not edit comment generated here */
#define CYCLES_FOR_STABLE_COUNT                 1u
#define VALUE_CHANGE                            3000u
#define TIMER_VALUE_1                           (49999u + VALUE_CHANGE + 500u )
#define TIMER_VALUE_2                           (44999u + VALUE_CHANGE + 0u)



volatile u16 countDiff = 0 ; 
volatile u8 index_px = 0 ;
volatile u32 p0_in = 0 ;
volatile u32 p1_in = 0 ;
volatile u32 p2_in = 0 ;
volatile u8 complete_flag = 0 ;
volatile u8 stableCount = 0 ;

extern volatile u32 freqCount;
//volatile u32 milliSec = 0 ;

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_tau0_channel1_interrupt
* Description  : This function is INTTM01 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
//__interrupt static void r_tau0_channel1_interrupt(void)
void r_tau0_channel1_interrupt(void)
{
    if ((TSR01 & _0001_TAU_OVERFLOW_OCCURS) == 1U)    /* overflow occurs */
    {            
        g_tau0_ch1_width = (uint32_t)(TDR01 + 1UL) + 0x10000UL;
    }
    else
    {
        g_tau0_ch1_width = (uint32_t)(TDR01 + 1UL);
    }

    /* Start user code. Do not edit comment generated here */
	//freqCount = g_tau0_ch1_width ;
	freqIncr();
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_tau0_channel0_interrupt
* Description  : This function is INTTM00 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
//__interrupt static void r_tau0_channel0_interrupt(void)
void r_tau0_channel0_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
	//P2.3 = ~P2.3 ;
	vIncreaseMillCount();
    
    /* End user code. Do not edit comment generated here */
}



/***********************************************************************************************************************
* Function Name: r_tau0_channel2_interrupt
* Description  : This function is INTTM02 interrupt service routine. 100msec timer
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
//__interrupt static void r_tau0_channel2_interrupt(void)
void r_tau0_channel2_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
	
	//P2.3 = ~P2.3 ;
    if (complete_flag == 0)
    {

        switch(index_px)
        {
            case 0:
                ENREF_P2_SEL 	= 0 ; // disselect channel
                ENPROBE_P1_SEL 	= 0 ; // disselect channel
                freqCount = 0 ;
                index_px = 1 ;
                break;
            
            case 1:
                p0_in = freqCount ;
                // p1 select
                ENREF_P2_SEL 	= 0 ; // disselect channel
                ENPROBE_P1_SEL 	= 1 ; // select channel
                freqCount = 0 ;
				//TDR02 = 47499u ;		// 95msec interrupt 1
                TDR02 = TIMER_VALUE_1 ;		// 100msec interrupt    
                index_px = 2 ;      //2
                break;
            
            case 2:
                p1_in = freqCount ;
                
                // p2 select
                ENPROBE_P1_SEL 	= 0 ; // disselect channel
                ENREF_P2_SEL 	= 1 ; // select channel
                freqCount = 0 ;
				//TDR02 = 49999u ;		// 100msec interrupt    1
                TDR02 = TIMER_VALUE_2 ;		
                index_px = 3 ;
                break;
            
            case 3:
                p2_in = freqCount ;
                if(++stableCount > CYCLES_FOR_STABLE_COUNT)
                {
                    complete_flag = 1 ;
                    stableCount = CYCLES_FOR_STABLE_COUNT;
                }
                // ENREF_P2_SEL 	= 0 ; // disselect channel
                // ENPROBE_P1_SEL 	= 0 ; // disselect channel
                // freqCount = 0 ;
                index_px = 0 ;
                break;
            
            
        }

    }
    

    /* End user code. Do not edit comment generated here */
}


/* Start user code for adding. Do not edit comment generated here */


/* End user code. Do not edit comment generated here */
