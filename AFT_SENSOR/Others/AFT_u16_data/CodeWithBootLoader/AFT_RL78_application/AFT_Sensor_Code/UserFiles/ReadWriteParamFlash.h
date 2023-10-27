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

#ifndef __READWRITEPARAMFLASH_H__
#define __READWRITEPARAMFLASH_H__

/****************************************************************************************************************/
/* File Includes                                                                                                */
/****************************************************************************************************************/

#include "r_cg_macrodriver.h"
#include "CustomDataTypes.h"
#include "User.h"
#include "Calibration.h"
#include "levelCalc.h"
#include "dataflash.h"




/****************************************************************************************************************/
/* Constant Defines                                                                                             */
/****************************************************************************************************************/


/****************************************************************************************************************/
/* Enumeration types                                                                                            */
/****************************************************************************************************************/


/****************************************************************************************************************/
/* Type definitions                                                                                             */
/****************************************************************************************************************/


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

void vSave_P0_cal_inFlash(u16 value_p0_cal);
void vSave_P1E_cal_inFlash(u16 value_P1E_cal);
void vSave_P2E_cal_inFlash(u16 value_P2E_cal);
void vSave_P1F_cal_inFlash(u16 value_P1F_cal);
void vSave_P2F_cal_inFlash(u16 value_P2F_cal);
void vSave_m_inFlash(float value_m);
void vSave_Div_cal_inFlash(float value_Div_cal);
void vSave_Span_cal_inFlash(float value_span_cal);
void vSave_Cali_Status_inFlash(bool value_Cali_status);
void vSave_Div_Avg_Store_inFlash(float value_DivAvgStore);
void vSave_OneTimeCalibFlag_inFlash(bool calibStatus);
void vSave_NumberOfTimeCalibration_inFlash(u32 NumberOfTimeCalibration);
void vSave_CalibrationLastState_inFlash(e_calibration_status );
void vSave_CalibTimeStampHour_inFlash(u8 hours);
void vSave_CalibTimeStampMinutes_inFlash(u8 minutes);
void vSave_CalibTimeStampSeconds_inFlash(u8 seconds);
void vSave_CalibTimeStampAMorPM_inFlash(AMorPM_type);
void vSave_CalibTimeStampDay_inFlash(u8 day);
void vSave_CalibTimeStampMonths_inFlash(u8 Months);
void vSave_CalibTimeStampYear_inFlash(u8 year);
void vSave_CalibrationMethod_inFlash(u8 calibrationMethod);

void vSave_m_u16_inFlash(uint16_t value_m);
void vSave_Div_cal_u16_inFlash(uint16_t value_Div_cal);
void vSave_Span_cal_u16_inFlash(uint16_t value_span_cal);
void vSave_Div_Avg_u16_Store_inFlash(uint16_t value_DivAvgStore);


u16 vRead_P0_cal_fromFlash(void);
u16 vRead_P1E_cal_fromFlash(void);
u16 vRead_P2E_cal_fromFlash(void);
u16 vRead_P1F_cal_fromFlash(void);
u16 vRead_P2F_cal_fromFlash(void);
float vRead_m_fromFlash(void);
float vRead_Div_cal_fromFlash(void);
float vRead_Span_cal_fromFlash(void);
bool vRead_Cali_Status_fromFlash(void);
float vRead_Div_Avg_Store_fromFlash(void);
bool vRead_OneTimeCalibFlag_fromFlash(void);
u32 vRead_NumberOfTimeCalibration_fromFlash(void);
u8 vRead_CalibrationLastState_fromFlash(void);
u8 vRead_CalibTimeStampHour_fromFlash(void);
u8 vRead_CalibTimeStampMinutes_fromFlash(void);
u8 vRead_CalibTimeStampSeconds_fromFlash(void);
u8 vRead_CalibTimeStampAMorPM_fromFlash(void);
u8 vRead_CalibTimeStampDay_fromFlash(void);
u8 vRead_CalibTimeStampMonths_fromFlash(void);
u8 vRead_CalibTimeStampYear_fromFlash(void);
u8 vRead_CalibrationMethod_fromFlash(void);

uint16_t vRead_m_u16_fromFlash(void);
uint16_t vRead_Div_cal_u16_fromFlash(void);
uint16_t vRead_Span_cal_u16_fromFlash(void);
uint16_t vRead_Div_Avg_u16_Store_fromFlash(void);

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