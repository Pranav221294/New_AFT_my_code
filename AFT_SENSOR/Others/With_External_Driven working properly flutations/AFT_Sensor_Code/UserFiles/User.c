

/****************************************************************************************************************/
/* COPYRIGHT                                                                                                    */
/* Inidication Instruments Ltd.(c) 2022                                                                         */
/****************************************************************************************************************/
/* File name   : {file_name}                                                                                    */
/* Module      : {file_path}                                                                                    */
/*--------------------------------------------------------------------------------------------------------------*/
/* Description :                                                                                                */
/****************************************************************************************************************/
/* History                                                                                                      */
/*--------------------------------------------------------------------------------------------------------------*/
/* Date - 19/07/2022       | Revised By  | Rev. No.  | Description                                                           */
/*--------------------------------------------------------------------------------------------------------------*/
/*                                                                                                              */
/****************************************************************************************************************/

/****************************************************************************************************************/
/* Includes                                                                                                     */
/****************************************************************************************************************/

#include "CustomDataTypes.h"
#include "User.h"
#include "Can.h"
#include "r_cg_wdt.h"
/****************************************************************************************************************/
/****************************************************************************************************************/
/* Defines                                                                                                      */
/****************************************************************************************************************/



/****************************************************************************************************************/
/* Enumeration                                                                                                  */
/***************************************************************************************************************/


/****************************************************************************************************************/
/* Extern variable                                                                                                  */
/***************************************************************************************************************/


/****************************************************************************************************************/
/* Type definitions                                                                                             */
/****************************************************************************************************************/


/****************************************************************************************************************/
/* Local variables                                                                                              */
/****************************************************************************************************************/

volatile u32 millCount = 0 ;
ThreeBytes ThreeBytes_result;
/**********************************************************************************/
/* Local function declarations                                                                                  */
/****************************************************************************************************************/
/// @addtogroup MODULE_LIBRARY
/// @{
/// @defgroup ModuleLib  Private functions used 
//// 
///   
/// @addtogroup ModuleLib  
/// @{
      		
	//Local function declarations.....................................


/// @} // endgroup ModuleLib  

/// @} // endgroup MODULE_LIBRARY

/****************************************************************************************************************/
/* Local function definitions                                                                                   */
/****************************************************************************************************************/

/*************************************************************************************************************//**
 *
 * Purpose  	:  
 *
 * @param[in]	:  	None    
 * @param[out]	:	None
 * @return   	:	None
 ****************************************************************************************************************/

/****************************************************************************************************************/
/* Global function definitions                                                                                   */
/****************************************************************************************************************/

//*************************************************************************************************************//**
 /*
 * Purpose  	:  This function is used to selcted the probe input through bidirectional switch IC
 *
 * @param[in]   :	(e_probeSelcect_type), probe selection 
 * @return    	: 	None
 ****************************************************************************************************************/
void vSelectProbe(e_probeSelcect_type probeSel)
{
	switch(probeSel)
	{
		case e_P0_select:
			ENREF_P2_SEL 	= 0 ; // disselect channel
			ENPROBE_P1_SEL 	= 0 ; // disselect channel
			break;
		
		case e_P1_select:
			ENREF_P2_SEL 	= 0 ; // disselect channel
			ENPROBE_P1_SEL 	= 1 ; // select channel
			break;
		
		case e_P2_select:
			ENPROBE_P1_SEL 	= 0 ; // disselect channel
			ENREF_P2_SEL 	= 1 ; // select channel
			break;
	}
}
 

//*************************************************************************************************************//**
 /*
 * Purpose  	:  This function is called in 1 msec interrupt ISR to increase the variable count after each millisec
 *
 * @param[in]   :	None
 * @return    	: 	None
 ****************************************************************************************************************/
void vIncreaseMillCount(void)
{
	millCount++;
}
 


//*************************************************************************************************************//**
 /*
 * Purpose  	:  
 *
 * @param[in]   :	None
 * @return    	: 	None
 ****************************************************************************************************************/
void vSetBlockDelayMilli(u32 millSec)
{
	u32 TimeStamp = u32GetMilTick();
	while( (u32GetMilTick() - TimeStamp) <= millSec )
	{
		/* Restart watchdog timer*/
		R_WDT_Restart();
	}
}


//*************************************************************************************************************//**
 /*
 * Purpose  	:   This function returns the milli second count
 *
 * @param[in]   :	None
 * @return    	: 	(u32) milli Second Counts
 ****************************************************************************************************************/
u32 u32GetMilTick(void)
{
	return millCount ;
}



//*************************************************************************************************************//**
 /*
 * Purpose  	:   This function set the output voltage between the value set by Macros  SET_MIN_OUT_VOLATEG_AT_0_DUTY_CYCLE &
 *					SET_MAX_OUT_VOLATEG_AT_100_DUTY_CYCLE
 * @param[in]   :	milivolts Ex. for 5.10V need to pass 5100 value
 * @return    	: 	none
 ****************************************************************************************************************/
void vSetOutputVoltage(u16 miliVolt)
{
	u16 tempTDR25 = 0 ;

	tempTDR25 = ( ( MAX_VAL_TDR25_FOR_50_DUTY * miliVolt ) / MAX_OUT_VOL_AT_50_DUTY) ;
	TDR25 = tempTDR25 ;
}


float getFloatUpto3place(float dataFloat)
{
	u16 tempu16 = 0 ; 
	float tempFloat = 0 ; 
	tempu16 = dataFloat * 1000 ;
	tempFloat = (float)tempu16 / 1000 ;
	return (tempFloat);

}

//*************************************************************************************************************//**
 /*
 * Purpose  	:   Set unused pins as zero to reduce the power
 * @param[in]   :	none
 * @return    	: 	none
 ****************************************************************************************************************/

void vSetUnusedPinsAsZero(void)
{
	UNUSED_PIN_P10	= 	0 ;				/* MCU pin number 15 */
	UNUSED_PIN_P11	= 	0 ;				/* MCU pin number 16 */
	UNUSED_PIN_P12	= 	0 ;				/* MCU pin number 17 */
	UNUSED_PIN_P13	= 	0 ;				/* MCU pin number 20 */
	UNUSED_PIN_P14	= 	0 ;				/* MCU pin number 21 */
	UNUSED_PIN_P20	= 	0 ;				/* MCU pin number 4 */
	UNUSED_PIN_P21	= 	0 ;				/* MCU pin number 3 */
	UNUSED_PIN_P22	= 	0 ;				/* MCU pin number 2 */
	//UNUSED_PIN_P23	= 	0 ;				/* MCU pin number 1 */
	UNUSED_PIN_P27	= 	0 ;				/* MCU pin number 48 */
	UNUSED_PIN_P30	= 	0 ;				/* MCU pin number 26 */
//	UNUSED_PIN_P31	= 	0 ;				/* MCU pin number 25 */
//	UNUSED_PIN_P33	= 	0 ;				/* MCU pin number 24 */
	UNUSED_PIN_P54	= 	0 ;				/* MCU pin number 30 */
	UNUSED_PIN_P55	= 	0 ;				/* MCU pin number 29 */
	UNUSED_PIN_P56	= 	0 ;				/* MCU pin number 28 */
	UNUSED_PIN_P57	= 	0 ;				/* MCU pin number 27 */
	UNUSED_PIN_P61	= 	0 ;				/* MCU pin number 14 */
	UNUSED_PIN_P72	= 	0 ;				/* MCU pin number 31 */
	UNUSED_PIN_P74	= 	0 ;				/* MCU pin number 19 */
	UNUSED_PIN_P75	= 	0 ;				/* MCU pin number 18 */
	UNUSED_PIN_P80	= 	0 ;				/* MCU pin number 47 */
	UNUSED_PIN_P81	= 	0 ;				/* MCU pin number 46 */
	UNUSED_PIN_P82	= 	0 ;				/* MCU pin number 45 */
	UNUSED_PIN_P83	= 	0 ;				/* MCU pin number 44 */
	UNUSED_PIN_P90	= 	0 ;				/* MCU pin number 41 */
	UNUSED_PIN_P91	= 	0 ;				/* MCU pin number 40 */
}

//*************************************************************************************************************//**
 /*
 * Purpose  	:   This function reset the system by watchdog
 * @param[in]   :	none
 * @return    	: 	none
 ****************************************************************************************************************/
void vResetSystemByWatchDog(void)
{
	while(1)
	{
		/* Stuck processor here, so that watchdog reset the system */
	}
}
/****************************************************************************************************************/
/* u32 merge_of_overflow_n_reg_value( u8 overflow_value, u16 reg_value)                                                                                       */ 
/****************************************************************************************************************/
u32 merge_of_overflow_n_reg_value( u8 overflow_value, u16 reg_value)
{
   u32 merge_value ; 
   merge_value = ((u32)overflow_value << 16) | reg_value;
  // merge_value = (merge_value >> 8) & 0xFFFF;
   return merge_value;

}
/****************************************************************************************************************/
/* u32 merge_of_overflow_n_reg_value( u8 overflow_value, u16 reg_value)                                                                                       */ 
/****************************************************************************************************************/
u32 shifting_of_merge_overflow_n_reg_v(u8 overflow_value, u16 reg_value,u8 shifting_value)
{
   u32 shifted_value = 0 ;
   u32 masked_value = 0;
   
   shifted_value = (merge_of_overflow_n_reg_value(overflow_value,reg_value) << shifting_value)|(merge_of_overflow_n_reg_value(overflow_value,reg_value) >>(16 - shifting_value));
   masked_value = (shifted_value >> 8) & 0xFFFF;

   return masked_value;  


}


ThreeBytes merge_and_get_bytes(u8 overflow_value, u16 reg_value)
 {
    
    
    u32 merge_value = ((u32)overflow_value << 16) | reg_value;

    ThreeBytes_result.upper_byte = (u8)((merge_value >> 16) & 0xFF); // Get the most significant byte
    ThreeBytes_result.higher_byte = (u8)((merge_value >> 8) & 0xFF);  // Get the middle byte
    ThreeBytes_result.lower_byte = (u8)(merge_value & 0xFF);         // Get the least significant byte


    return ThreeBytes_result;
}

// void leftShiftBytes(unsigned char* byte1, unsigned char* byte2, unsigned char* byte3) 
// {
//     int carry1 = (*byte1 & 0x80) ? 1 : 0; // Save the carry out bit from byte1
//     int carry2 = (*byte2 & 0x80) ? 1 : 0; // Save the carry out bit from byte2

//     // Perform the left shift on the bytes
//     *byte1 <<= 1;
//     *byte2 <<= 1;
//     *byte3 <<= 1;

//     // Accumulate the shifted bits into the next byte if there was a carry
//     if (carry1) {
//         *byte2 |= 0x01;
//     }
//     if (carry2) {
//         *byte3 |= 0x01;
//     }
// }

//  ThreeBytes leftShiftBytes(ThreeBytes bytes, int num_shifts) 
//  {
// 	int shift;  
//     for (shift = 0; shift < num_shifts; shift++) 
// 	{
//         int carry1 = (bytes.upper_byte & 0x80) ? 1 : 0; // Save the carry out bit from the upper_byte
//         int carry2 = (bytes.higher_byte & 0x80) ? 1 : 0; // Save the carry out bit from the higher_byte

//         // Perform the left shift on the bytes
//         bytes.upper_byte <<= 1;
//         bytes.higher_byte <<= 1;
//         bytes.lower_byte <<= 1;

//         // Accumulate the shifted bits into the next byte if there was a carry
//         if (carry1) {
//             bytes.higher_byte |= 0x01;
//         }
//         if (carry2) {
//             bytes.lower_byte |= 0x01;
//         }
//     }
//     return bytes;
// }

ThreeBytes leftShiftBytes(ThreeBytes timers) 
{
    int carry1 = (timers.lower_byte & 0x80) ? 1 : 0; // Save the carry out bit from TMR1L
    int carry2 = (timers.higher_byte & 0x80) ? 1 : 0; // Save the carry out bit from TMR1H

    // Perform the left shift on the timers
    timers.lower_byte <<= 1;
    timers.higher_byte <<= 1;
    timers.upper_byte <<= 1;

    // Accumulate the shifted bits into the next timer if there was a carry
    if (carry1) 
	{
        timers.higher_byte |= 0x01;
    }
    if (carry2) 
	{
        timers.upper_byte |= 0x01;
    }

    return timers;
}

ThreeBytes leftShiftBytesN(ThreeBytes timers, int n) 
{
	int i;
    for (i = 0; i < n; i++) {
        timers = leftShiftBytes(timers);
    }
    return timers;
}

u32 merge_shifted_bytes(ThreeBytes shifted_bytes)
{
    return ((u32)shifted_bytes.upper_byte << 16) |((u32)shifted_bytes.higher_byte << 8) |shifted_bytes.lower_byte;
}

u16 merge_first_two_bytes(ThreeBytes shifted_bytes) 
{
    return ((u16)shifted_bytes.upper_byte << 8) | shifted_bytes.higher_byte;
}