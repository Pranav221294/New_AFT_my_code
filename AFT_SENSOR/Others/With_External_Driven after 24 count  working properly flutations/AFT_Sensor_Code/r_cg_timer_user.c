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
* File Name    : r_cg_timer_user.c
* Version      : Applilet3 for RL78/D1A V2.04.05.02 [08 Nov 2021]
* Device(s)    : R5F10DGD
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for TAU module.
* Creation Date: 28/07/2023
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt INTTM00 r_tau0_channel0_interrupt
#pragma interrupt INTTM01 r_tau0_channel1_interrupt
#pragma interrupt INTTM10 r_tau1_channel0_interrupt
#pragma interrupt INTTM21 r_tau2_channel1_interrupt
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
/* Start user code for global. Do not edit comment generated here */
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
volatile u8 overflow_65535 = 0 ; 

//extern volatile u8 freqCount;
extern volatile u16 freqCount;

extern volatile bool counter_555_255_flag ; 
extern volatile u16 TDR10_value  ;
extern volatile u8 TDR10_overflow ;


//typedef struct 
//{
//    u8     TD10_overflow_v;
//    u16    TD10_value_reg_v;
//}raw_probe_value;

raw_probe_value p0_raw;
raw_probe_value p1_raw;
raw_probe_value p2_raw;
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_tau0_channel0_interrupt
* Description  : This function is INTTM00 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_tau0_channel0_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    vIncreaseMillCount();
	/* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_tau0_channel1_interrupt
* Description  : This function is INTTM01 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_tau0_channel1_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
        freqIncr();
	//P2.3 =~P2.3;
    
   /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_tau1_channel0_interrupt
* Description  : This function is INTTM10 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_tau1_channel0_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    P2.3 =~P2.3;
  overflow_65535++;
// if (complete_flag == 0)
//  {
//    if(counter_555_255_flag== true )
//      { //P2.3 =~P2.3;
//      switch(index_px)
//      {
//          case 0:
//              ENREF_P2_SEL 	= 0 ; // disselect channel
//              ENPROBE_P1_SEL 	= 0 ; // disselect channel
//              freqCount = 0 ;
//              TDR10 =  65535 ; 
//              //TDR10 =  51999 ;
//              index_px = 1 ;
//              TMIF01 = 0U;        /* clear INTTM01 interrupt flag */
//              TMMK01 = 0U;        /* enable INTTM01 interrupt */
//              TS0 |= _0002_TAU_CH1_START_TRG_ON;
//              counter_555_255_flag = false;
// 	       TDR10_overflow = 0;
// 	       TDR10_value = 0;
// 	       overflow_65535 = 0;
// 	     //  RLF  = 0;
// 	      // P2.3 =~P2.3;
//              break;
         
//          case 1:
//   //           p0_in = freqCount ;
//              p0_raw.TD10_overflow_v = TDR10_overflow;
//              p0_raw.TD10_value_reg_v = TDR10_value;
//              // p1 select
//              ENREF_P2_SEL 	= 0 ; // disselect channel
//              ENPROBE_P1_SEL 	= 1 ; // select channel
//              freqCount = 0 ;
// 	            TDR10 =  65535 ;		// 100msec interrupt    
//              //TDR10 = 51999 ;
//              index_px = 2 ;      //2
//              TMIF01 = 0U;        /* clear INTTM01 interrupt flag */
//              TMMK01 = 0U;        /* enable INTTM01 interrupt */
//              TS0 |= _0002_TAU_CH1_START_TRG_ON;               
//              counter_555_255_flag = false;
// 	       TDR10_overflow = 0;
// 	       TDR10_value = 0;
// 	       overflow_65535 = 0;
// 	     //  P2.3 = ~ P2.3;
//              break;
         
//          case 2:
// //             p1_in = freqCount ;
//              p1_raw.TD10_overflow_v = TDR10_overflow;
//              p1_raw.TD10_value_reg_v = TDR10_value;
             
//              // p2 select
//              ENPROBE_P1_SEL 	= 0 ; // disselect channel
//              ENREF_P2_SEL 	= 1 ; // select channel
//              freqCount = 0 ;
// 			        TDR10 = 65535 ;
//             // TDR10 = 51999 ;		
//              counter_555_255_flag = false;
//              index_px = 3 ;
//              TMIF01 = 0U;        /* clear INTTM01 interrupt flag */
//              TMMK01 = 0U;        /* enable INTTM01 interrupt */
//              TS0 |= _0002_TAU_CH1_START_TRG_ON;
//              TDR10_overflow = 0;
// 	            TDR10_value = 0; 
//              overflow_65535 = 0;
// 	     //  P3.3 = ~P3.3;
// 	       break;
         
//          case 3:
// //               p2_in = freqCount ;
//              p2_raw.TD10_overflow_v = TDR10_overflow;
//              p2_raw.TD10_value_reg_v = TDR10_value;
//              freqCount = 0 ;
// 	          TDR10 = 65535 ;	
//            //TDR10 = 51999 ;	
// //               counter_555_255_flag = false;
// //               TMIF01 = 0U;        /* clear INTTM01 interrupt flag */
// //               TMMK01 = 0U;        /* enable INTTM01 interrupt */
// //               TS0 |= _0002_TAU_CH1_START_TRG_ON;


//              if(++stableCount > CYCLES_FOR_STABLE_COUNT)
//              {
//                  complete_flag = 1 ;
//                  stableCount = CYCLES_FOR_STABLE_COUNT;
//              }
//              // ENREF_P2_SEL 	= 0 ; // disselect channel
//              // ENPROBE_P1_SEL 	= 0 ; // disselect channel
//              // freqCount = 0 ;
//              index_px = 0 ;
//              TDR10_overflow = 0;
// 	       TDR10_value = 0; 
//              overflow_65535 = 0;
// 	       break;
         
         
//      }

//  }

//  } 
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_tau2_channel1_interrupt
* Description  : This function is INTTM21 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_tau2_channel1_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
  //  P2.3 =~P2.3;
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
void probe_selection()
{
  if (complete_flag == 0)
 {
   if(counter_555_255_flag== true )
     { //P2.3 =~P2.3;
     switch(index_px)
     {
         case 0:
             ENREF_P2_SEL 	= 0 ; // disselect channel
             ENPROBE_P1_SEL 	= 0 ; // disselect channel
             freqCount = 0 ;
             TDR10 =  65535 ; 
             //TDR10 =  51999 ;
             index_px = 1 ;
             TMIF01 = 0U;        /* clear INTTM01 interrupt flag */
             TMMK01 = 0U;        /* enable INTTM01 interrupt */
             TS0 |= _0002_TAU_CH1_START_TRG_ON;
             counter_555_255_flag = false;
	       TDR10_overflow = 0;
	       TDR10_value = 0;
	       overflow_65535 = 0;
         R_TAU1_Channel0_Start();
	     //  RLF  = 0;
	      // P2.3 =~P2.3;
             break;
         
         case 1:
  //           p0_in = freqCount ;
             p0_raw.TD10_overflow_v = TDR10_overflow;
             p0_raw.TD10_value_reg_v = TDR10_value;
             // p1 select
             ENREF_P2_SEL 	= 0 ; // disselect channel
             ENPROBE_P1_SEL 	= 1 ; // select channel
             freqCount = 0 ;
	            TDR10 =  65535 ;		// 100msec interrupt    
             //TDR10 = 51999 ;
             index_px = 2 ;      //2
             TMIF01 = 0U;        /* clear INTTM01 interrupt flag */
             TMMK01 = 0U;        /* enable INTTM01 interrupt */
             TS0 |= _0002_TAU_CH1_START_TRG_ON;               
             counter_555_255_flag = false;
	       TDR10_overflow = 0;
	       TDR10_value = 0;
	       overflow_65535 = 0;
         R_TAU1_Channel0_Start();
	     //  P2.3 = ~ P2.3;
             break;
         
         case 2:
//             p1_in = freqCount ;
             p1_raw.TD10_overflow_v = TDR10_overflow;
             p1_raw.TD10_value_reg_v = TDR10_value;
             
             // p2 select
             ENPROBE_P1_SEL 	= 0 ; // disselect channel
             ENREF_P2_SEL 	= 1 ; // select channel
             freqCount = 0 ;
			        TDR10 = 65535 ;
            // TDR10 = 51999 ;		
             counter_555_255_flag = false;
             index_px = 3 ;
             TMIF01 = 0U;        /* clear INTTM01 interrupt flag */
             TMMK01 = 0U;        /* enable INTTM01 interrupt */
             TS0 |= _0002_TAU_CH1_START_TRG_ON;
             TDR10_overflow = 0;
	            TDR10_value = 0; 
             overflow_65535 = 0;
             R_TAU1_Channel0_Start();
	     //  P3.3 = ~P3.3;
	       break;
         
         case 3:
//               p2_in = freqCount ;
             p2_raw.TD10_overflow_v = TDR10_overflow;
             p2_raw.TD10_value_reg_v = TDR10_value;
             freqCount = 0 ;
	          TDR10 = 65535 ;	
           //TDR10 = 51999 ;	
//               counter_555_255_flag = false;
//               TMIF01 = 0U;        /* clear INTTM01 interrupt flag */
//               TMMK01 = 0U;        /* enable INTTM01 interrupt */
//               TS0 |= _0002_TAU_CH1_START_TRG_ON;


             if(++stableCount > CYCLES_FOR_STABLE_COUNT)
             {
                 complete_flag = 1 ;
                 stableCount = CYCLES_FOR_STABLE_COUNT;
             }
             // ENREF_P2_SEL 	= 0 ; // disselect channel
             // ENPROBE_P1_SEL 	= 0 ; // disselect channel
             // freqCount = 0 ;
             index_px = 0 ;
             TDR10_overflow = 0;
	       TDR10_value = 0; 
             overflow_65535 = 0;
	       break;
         
         
     }

 }

 } 

}

/* End user code. Do not edit comment generated here */
