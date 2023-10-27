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
/* Date - 20/07/2022       | Revised By  | Rev. No.  | Description                                                           */
/*--------------------------------------------------------------------------------------------------------------*/
/*                                                                                                              */
/****************************************************************************************************************/

#ifndef __CALIBRATION_H__
#define __CALIBRATION_H__

/****************************************************************************************************************/
/* File Includes                                                                                                */
/****************************************************************************************************************/

#include "r_cg_macrodriver.h"
#include "CustomDataTypes.h"
#include "User.h"
#include "ReadWriteParamFlash.h"
#include "levelCalc.h"
#include "Configuration.h"

/****************************************************************************************************************/
/* Constant Defines                                                                                             */
/****************************************************************************************************************/

#define CALIBRATION_NOT_DONE				0u	

#define ACCUMULATED_COUNT_CALI				ACCUMULATE

/*
As we have observed that when we increase the fuel level then count value decreases But we want to increase the value
of count with increase of fuel level , so to resolve this issue we are substracting the counts from a constant value
*/
#define VALUE_FOR_SUBSTRACTION				        7000ul


/* Below constant used in formulas */
#define CONST_14_4							14.40f				// 14.4
#define CONST_7_07							7.07f				// 7.07
#define CONST_6_3							6.30f				// 6.3

#define CONST_14_4_4096						((uint16_t)58982)							
#define CONST_7_07_4096						((uint16_t)28959)
#define CONST_6_3_4096						((uint16_t)25804)

/****************************************************************************************************************/
/* Enumeration types                                                                                            */
/****************************************************************************************************************/

typedef enum
{
	eCalibTimeStamp_AM = 1,
	eCalibTimeStamp_PM,
}AMorPM_type;

/****************************************************************************************************************/
/* Type definitions                                                                                             */
/****************************************************************************************************************/

/* values, that will be stored during calibration */
typedef struct
{
	u16 p0_empty;
	u16 p1_empty;
	u16 p2_empty;
	u16 p1_full;
	u16 p2_full;
	u16 raw_p1_span;			// p1_full - p1_empty
	u16 raw_p2_span;			// p2_full - p2_empty
	u16 constant_m;				// 14.4 � (7.07 x (raw_p1_span) / (raw_p2_span))
	float constant_m_f;
	u16 div_cal;				// (m x (raw_p1_span) / (raw_p2_span)) � 6.3
	float div_cal_f;
	u16 p1_span;				// raw_p1_span / div_cal
	float p1_span_f;
	bool cal_status;
}s_Parameter_type;



/* state during calibration */
typedef enum
{
	e_cali_not_calibrated,
	e_cali_failed,
	e_cali_success,
	e_cali_empty_failed,
	e_cali_empty_success,
	e_cali_full_failed,
	e_cali_full_success,
	e_cali_empty_start,
	e_cali_full_start
}e_calibration_status;


typedef struct CaliAccumSample
{
	/* data */
	u16 Accum_p0[ACCUMULATED_COUNT_CALI];					// Array to accumulate P0 counts
	u8 accum_p0_oneTimeFlag;								

	u16 Accum_p1[ACCUMULATED_COUNT_CALI];					// Array to accumulate P1E counts
	u8 accum_p1_oneTimeFlag;

	u16 Accum_p2[ACCUMULATED_COUNT_CALI];					// Array to accumulate P2E counts
	u8 accum_p2_oneTimeFlag;

	u16 Accum_p1_f[ACCUMULATED_COUNT_CALI];					// Array to accumulate P1F counts
	u8 accum_p1_f_oneTimeFlag;

	u16 Accum_p2_f[ACCUMULATED_COUNT_CALI];					// Array to accumulate P2F counts
	u8 accum_p2_f_oneTimeFlag;

	
}Accum_t;

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
bool bReadCaliPinStatus(void);

e_calibration_status calibrationStart(void);

void vEmptyCalibration(void);
void vFullCalibration(void);
void calculateParamAndSaveInFlash(void);
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