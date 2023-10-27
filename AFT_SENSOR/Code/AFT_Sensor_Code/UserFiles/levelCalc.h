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
/* Date       | Revised By  | Rev. No.  | Description                                                           */
/*--------------------------------------------------------------------------------------------------------------*/
/*                                                                                                              */
/****************************************************************************************************************/

#ifndef __LEVELCALC_H__
#define __LEVELCALC_H__

/****************************************************************************************************************/
/* File Includes                                                                                                */
/****************************************************************************************************************/

//#include "stdio.h"
#include "CustomDataTypes.h"
#include "r_cg_macrodriver.h"
#include "Calibration.h"
#include "User.h"
#include "CurveHeader.h"

/****************************************************************************************************************/
/* Constant Defines                                                                                             */
/****************************************************************************************************************/


							

/****************************************************************************************************************/
/* Enumeration types                                                                                            */
/****************************************************************************************************************/


/****************************************************************************************************************/
/* Type definitions                                                                                             */
/****************************************************************************************************************/

typedef struct
{
    u16 p0_live;
    u16 p1_live;
    u16 p2_live;
	s16 p0_x;       // data type is s16 as result could be negative
	s8  negative_flag;
	s16 p1_drift_factor;
	float p1_drift_factor_f;
	s16 p2_drift_factor;
	float p2_drift_factor_f;
	s16 current_level_p1;
	s16 current_level_p2;
	s16 live_div;
	float live_div_f;
	s16 div_average;
	float div_average_f;
	s16 div_avg_store;
	float div_avg_store_f;
	float currUsedDiv;
	s16 level_fluid_Q1;
	float level_fluid_Q1_f;
	//s16 PWM_Value;	// not used
	
}s_liveParam_type;

/****************************************************************************************************************/
/* Global function declarations                                                                                 */
/****************************************************************************************************************/

/// @addtogroup FILE_LIBRARY
/// @{
/// @defgroup FileLib  functions used in main file 
//// 
///   
/// @addtogroup FileLib
/// @{

// Global function declarations   ------------------------------------------------------
uint16_t getMovingAverage(uint16_t *ptrArraySamples, uint16_t data, uint8_t samplesCount);
s16 calculateFuelLevel(void);
float getMovingAverageFloat(float *ptrArraySamples, float data, uint8_t samplesCount);

void vSetPWM(s16 levelValue);

uint32_t getAccumalatedCount(uint16_t *ptrArraySamples, uint16_t data, uint8_t samplesCount);	// for debug

/// @} // endgroup FileLib

/// @} // endgroup FILE_LIBRARY


/****************************************************************************************************************/
/* Inline functions                                                                                             */
/****************************************************************************************************************/

/// @addtogroup FILE_INLINE_LIBRARY
/// @{
/// @defgroup FileInlineLib  functions used in main file 
//// 
///   
/// @addtogroup FileInlineLib  
/// @{

// Inline functions declarations   ------------------------------------------------------




/// @} // endgroup FileInlineLib  

/// @} // endgroup FILE_INLINE_LIBRARY


#endif 