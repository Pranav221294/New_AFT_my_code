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

/****************************************************************************************************************/
/* Includes                                                                                                     */
/****************************************************************************************************************/

#include "ReadWriteParamFlash.h"
#include "Calibration.h"

/****************************************************************************************************************/
/****************************************************************************************************************/
/* Defines                                                                                                      */
/****************************************************************************************************************/

/* 
    As we cant store float numbers in flash directly so to store those float values in flash first we need to convert 
    float values into u32 data type, below 'VALUE_FOR_MULT_DIVI_FLOAT_UL_CONVERSION' macro will work as multiplier 
    and divident for conversion.
*/
#define VALUE_FOR_MULT_DIVI_FLOAT_UL_CONVERSION                 100000ul

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

/****************************************************************************************************************/
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
 * Purpose  	:   This function save the value of P0_cal parameter
 *
 * @param[in]   :	value
 * @return    	: 	None
 ****************************************************************************************************************/
void vSave_P0_cal_inFlash(u16 value_p0_cal)
{
    vSysParamWriteUser(value_p0_cal, eStore_P0_Cali_Identifier);
}

//*************************************************************************************************************//**
 /*
 * Purpose  	:   This function save the value of P1E_cal parameter
 *
 * @param[in]   :	value
 * @return    	: 	None
 ****************************************************************************************************************/
void vSave_P1E_cal_inFlash(u16 value_P1E_cal)
{
    vSysParamWriteUser(value_P1E_cal, eStore_P1E_Cali_Identifier);
}

//*************************************************************************************************************//**
 /*
 * Purpose  	:   This function save the value of P2E_cal parameter
 *
 * @param[in]   :	value
 * @return    	: 	None
 ****************************************************************************************************************/
void vSave_P2E_cal_inFlash(u16 value_P2E_cal)
{
    vSysParamWriteUser(value_P2E_cal, eStore_P2E_Cali_Identifier);
}

//*************************************************************************************************************//**
 /*
 * Purpose  	:   This function save the value of P1F_cal parameter
 *
 * @param[in]   :	value
 * @return    	: 	None
 ****************************************************************************************************************/
void vSave_P1F_cal_inFlash(u16 value_P1F_cal)
{
    vSysParamWriteUser(value_P1F_cal, eStore_P1F_Cali_Identifier);
}

//*************************************************************************************************************//**
 /*
 * Purpose  	:   This function save the value of P2F_cal parameter
 *
 * @param[in]   :	value
 * @return    	: 	None
 ****************************************************************************************************************/
void vSave_P2F_cal_inFlash(u16 value_P2F_cal)
{
    vSysParamWriteUser(value_P2F_cal, eStore_P2F_Cali_Identifier);
}


//*************************************************************************************************************//**
 /*
 * Purpose  	:   This function save the value of 'm' parameter
 * 
 * @param[in]   :	value
 * @return    	: 	None
 ****************************************************************************************************************/
void vSave_m_inFlash(float value_m)
{   
    u32 ValuefloatToUL = (u32)(value_m * VALUE_FOR_MULT_DIVI_FLOAT_UL_CONVERSION) ;
    vSysParamWriteUser(ValuefloatToUL, eStore_m_cali_Identifier);
}

//*************************************************************************************************************//**
 /*
 * Purpose  	:   This function save the value of 'm' parameter in u16 format
 * 
 * @param[in]   :	value
 * @return    	: 	None
 ****************************************************************************************************************/
void vSave_m_u16_inFlash(uint16_t value_m)
{   
    //u32 ValuefloatToUL = (u32)(value_m * VALUE_FOR_MULT_DIVI_FLOAT_UL_CONVERSION) ;
    vSysParamWriteUser(value_m, eStore_m_cali_Identifier);
}

//*************************************************************************************************************//**
 /*
 * Purpose  	:   This function save the value of Div_cal parameter
 *
 * @param[in]   :	value
 * @return    	: 	None
 ****************************************************************************************************************/
void vSave_Div_cal_inFlash(float value_Div_cal)
{
    u32 ValuefloatToUL = (u32)(value_Div_cal * VALUE_FOR_MULT_DIVI_FLOAT_UL_CONVERSION) ;
    vSysParamWriteUser(ValuefloatToUL, eStore_Div_cali_Identifier);
}

//*************************************************************************************************************//**
 /*
 * Purpose  	:   This function save the value of Div_cal parameter in u16 format
 *
 * @param[in]   :	value
 * @return    	: 	None
 ****************************************************************************************************************/
void vSave_Div_cal_u16_inFlash(uint16_t value_Div_cal)
{
    //u32 ValuefloatToUL = (u32)(value_Div_cal * VALUE_FOR_MULT_DIVI_FLOAT_UL_CONVERSION) ;
    vSysParamWriteUser(value_Div_cal, eStore_Div_cali_Identifier);
}

//*************************************************************************************************************//**
 /*
 * Purpose  	:   This function save the value of Span_cal parameter
 *
 * @param[in]   :	value
 * @return    	: 	None
 ****************************************************************************************************************/
void vSave_Span_cal_inFlash(float value_span_cal)
{
    u32 ValuefloatToUL = (u32)(value_span_cal * VALUE_FOR_MULT_DIVI_FLOAT_UL_CONVERSION) ;
    vSysParamWriteUser(ValuefloatToUL, eStore_Span_Cali_Identifier);
}

//*************************************************************************************************************//**
 /*
 * Purpose  	:   This function save the value of Span_cal parameter in u16 format
 *
 * @param[in]   :	value
 * @return    	: 	None
 ****************************************************************************************************************/
void vSave_Span_cal_u16_inFlash(uint16_t value_span_cal)
{
    //u32 ValuefloatToUL = (u32)(value_span_cal * VALUE_FOR_MULT_DIVI_FLOAT_UL_CONVERSION) ;
    vSysParamWriteUser(value_span_cal, eStore_Span_Cali_Identifier);
}

//*************************************************************************************************************//**
 /*
 * Purpose  	:   This function save the value of Cali_Status parameter
 *
 * @param[in]   :	value
 * @return    	: 	None
 ****************************************************************************************************************/
void vSave_Cali_Status_inFlash(bool value_Cali_status)
{
     vSysParamWriteUser((u8)value_Cali_status, eStore_Status_Cali_Identifier);
}

//*************************************************************************************************************//**
 /*
 * Purpose  	:   This function save the value of Div_avg_Store parameter
 *
 * @param[in]   :	value
 * @return    	: 	None
 ****************************************************************************************************************/
void vSave_Div_Avg_Store_inFlash(float value_DivAvgStore)
{
    u32 ValuefloatToUL = (u32)(value_DivAvgStore * VALUE_FOR_MULT_DIVI_FLOAT_UL_CONVERSION) ;
    vSysParamWriteUser(ValuefloatToUL, eStore_Div_Avg_Store_Identifier);
}

//*************************************************************************************************************//**
 /*
 * Purpose  	:   This function save the value of Div_avg_Store parameter
 *
 * @param[in]   :	value
 * @return    	: 	None
 ****************************************************************************************************************/
void vSave_Div_Avg_u16_Store_inFlash(uint16_t value_DivAvgStore)
{
    //u32 ValuefloatToUL = (u32)(value_DivAvgStore * VALUE_FOR_MULT_DIVI_FLOAT_UL_CONVERSION) ;
    vSysParamWriteUser(value_DivAvgStore, eStore_Div_Avg_Store_Identifier);
}


//*************************************************************************************************************//**
 /*
 * Purpose  	:   This function save the one time calibration flag in flash
 *
 * @param[in]   :	status true or false
 * @return    	: 	None
 ****************************************************************************************************************/
void vSave_OneTimeCalibFlag_inFlash(bool calibStatus)
{
    vSysParamWriteUser((u8)calibStatus, eStore_OneTimeCalibration_Identifier);
}

//*************************************************************************************************************//**
 /*
 * Purpose  	:   Number of time system tried to calibrate
 *
 * @param[in]   :	NumberOfTimeCalibration
 * @return    	: 	None
 ****************************************************************************************************************/
void vSave_NumberOfTimeCalibration_inFlash(u32 NumberOfTimeCalibration)
{
    vSysParamWriteUser(NumberOfTimeCalibration, eStore_NumberOfTimeCalibration_Identifier);
}


//*************************************************************************************************************//**
 /*
 * Purpose  	:   Save last state of calibration 
 *
 * @param[in]   :	e_calibration_status value
 * @return    	: 	None
 ****************************************************************************************************************/
void vSave_CalibrationLastState_inFlash(e_calibration_status e_StateOfCalibration)
{
    vSysParamWriteUser(e_StateOfCalibration, eStore_CalibrationLastState_Identifier);
}

//*************************************************************************************************************//**
 /*
 * Purpose  	:   Save hours when calibration done by CAN
 *
 * @param[in]   :	u8
 * @return    	: 	None
 ****************************************************************************************************************/
void vSave_CalibTimeStampHour_inFlash(u8 hours)
{
    vSysParamWriteUser(hours, eStore_TimeStampHour_identifier);
}

//*************************************************************************************************************//**
 /*
 * Purpose  	:   Save minutes when calibration done by CAN
 *
 * @param[in]   :	u8
 * @return    	: 	None
 ****************************************************************************************************************/
void vSave_CalibTimeStampMinutes_inFlash(u8 minutes)
{
    vSysParamWriteUser(minutes, eStore_TimeStampMinutes_identifier);
}

//*************************************************************************************************************//**
 /*
 * Purpose  	:   Save seconds when calibration done by CAN
 *
 * @param[in]   :	u8
 * @return    	: 	None
 ****************************************************************************************************************/
void vSave_CalibTimeStampSeconds_inFlash(u8 seconds)
{
    vSysParamWriteUser(seconds, eStore_TimeStampSeconds_identifier);
}


//*************************************************************************************************************//**
 /*
 * Purpose  	:   Save Am or PM when calibration done by CAN
 *
 * @param[in]   :	AMorPM_type
 * @return    	: 	None
 ****************************************************************************************************************/
void vSave_CalibTimeStampAMorPM_inFlash(AMorPM_type eCalibTime)
{
    vSysParamWriteUser(eCalibTime, eStore_TimeStampAMorPM_identifier);
}


//*************************************************************************************************************//**
 /*
 * Purpose  	:   Save Day when calibration done by CAN
 *
 * @param[in]   :	u8
 * @return    	: 	None
 ****************************************************************************************************************/
void vSave_CalibTimeStampDay_inFlash(u8 day)
{
    vSysParamWriteUser(day, eStore_TimeStampDay_identifier);
}


//*************************************************************************************************************//**
 /*
 * Purpose  	:   Save Months when calibration done by CAN
 *
 * @param[in]   :	u8
 * @return    	: 	None
 ****************************************************************************************************************/
void vSave_CalibTimeStampMonths_inFlash(u8 Months)
{
    vSysParamWriteUser(Months, eStore_TimeStampMonths_identifier);
}


//*************************************************************************************************************//**
 /*
 * Purpose  	:   Save Year when calibration done by CAN
 *
 * @param[in]   :	u8
 * @return    	: 	None
 ****************************************************************************************************************/
void vSave_CalibTimeStampYear_inFlash(u8 year)
{
    vSysParamWriteUser(year, eStore_TimeStampYear_identifier);
}



//*************************************************************************************************************//**
 /*
 * Purpose  	:   set calibration method
 *
 * @param[in]   :	u8
 * @return    	: 	None
 ****************************************************************************************************************/
void vSave_CalibrationMethod_inFlash(u8 calibrationMethod)
{
    vSysParamWriteUser(calibrationMethod, eStore_CalibrationMethod_identifier);
}


/***************************************************************************************************************
****************************************** FUNCTION TO READ FROM FLASH******************************************
****************************************************************************************************************
*/

//*************************************************************************************************************//**
 /*
 * Purpose  	:   This function return the value of P0_Cal Parameter
 *
 * @param[in]   :	None
 * @return    	: 	value
 ****************************************************************************************************************/
u16 vRead_P0_cal_fromFlash(void)
{
    u16 temp = 0 ;
    temp = (u16)ulSysParmsReadUser(eStore_P0_Cali_Identifier);
    return temp;
}

//*************************************************************************************************************//**
 /*
 * Purpose  	:   This function return the value of P1E_cal Parameter
 *
 * @param[in]   :	None
 * @return    	: 	value
 ****************************************************************************************************************/
u16 vRead_P1E_cal_fromFlash(void)
{
    u16 temp = 0 ;
    temp = (u16)ulSysParmsReadUser(eStore_P1E_Cali_Identifier);
    return temp;
}

//*************************************************************************************************************//**
 /*
 * Purpose  	:   This function return the value of P2E_cal Parameter
 *
 * @param[in]   :	None
 * @return    	: 	value
 ****************************************************************************************************************/
u16 vRead_P2E_cal_fromFlash(void)
{
    u16 temp = 0 ;
    temp = (u16)ulSysParmsReadUser(eStore_P2E_Cali_Identifier);
    return temp;
}

//*************************************************************************************************************//**
 /*
 * Purpose  	:   This function return the value of P1F_cal Parameter
 *
 * @param[in]   :	None
 * @return    	: 	value
 ****************************************************************************************************************/
u16 vRead_P1F_cal_fromFlash(void)
{
    u16 temp = 0 ;
    temp = (u16)ulSysParmsReadUser(eStore_P1F_Cali_Identifier);
    return temp;
}

//*************************************************************************************************************//**
 /*
 * Purpose  	:   This function return the value of P2F_cal Parameter
 *
 * @param[in]   :	None
 * @return    	: 	value
 ****************************************************************************************************************/
u16 vRead_P2F_cal_fromFlash(void)
{
    u16 temp = 0 ;
    temp = (u16)ulSysParmsReadUser(eStore_P2F_Cali_Identifier);
    return temp;
}

//*************************************************************************************************************//**
 /*
 * Purpose  	:   This function return the value of 'm' Parameter
 *
 * @param[in]   :	None
 * @return    	: 	value
 ****************************************************************************************************************/
float vRead_m_fromFlash(void)
{
    float temp = 0 ;
    temp = (float)( 1.0 * (u32)ulSysParmsReadUser(eStore_m_cali_Identifier) / VALUE_FOR_MULT_DIVI_FLOAT_UL_CONVERSION);

    return temp;
}

//*************************************************************************************************************//**
 /*
 * Purpose  	:   This function return the value of 'm' Parameter in u16 format
 *
 * @param[in]   :	None
 * @return    	: 	value
 ****************************************************************************************************************/
uint16_t vRead_m_u16_fromFlash(void)
{
    u16 temp = 0 ;
    temp = (u16)ulSysParmsReadUser(eStore_m_cali_Identifier);

    return temp;
}

//*************************************************************************************************************//**
 /*
 * Purpose  	:   This function return the value of Div_cal Parameter
 *
 * @param[in]   :	None
 * @return    	: 	value
 ****************************************************************************************************************/
float vRead_Div_cal_fromFlash(void)
{
    float temp = 0 ;
    temp = (float)( 1.0 * (u32)ulSysParmsReadUser(eStore_Div_cali_Identifier) / VALUE_FOR_MULT_DIVI_FLOAT_UL_CONVERSION);

    return temp;
}


//*************************************************************************************************************//**
 /*
 * Purpose  	:   This function return the value of Div_cal Parameter
 *
 * @param[in]   :	None
 * @return    	: 	value
 ****************************************************************************************************************/
uint16_t vRead_Div_cal_u16_fromFlash(void)
{
    u16 temp = 0 ;
    temp = (u16)ulSysParmsReadUser(eStore_Div_cali_Identifier);

    return temp;
}

//*************************************************************************************************************//**
 /*
 * Purpose  	:   This function return the value of Span_cal Parameter
 *
 * @param[in]   :	None
 * @return    	: 	value
 ****************************************************************************************************************/
float vRead_Span_cal_fromFlash(void)
{
    float temp = 0 ;
    temp = (float)( 1.0 * (u32)ulSysParmsReadUser(eStore_Span_Cali_Identifier) / VALUE_FOR_MULT_DIVI_FLOAT_UL_CONVERSION);    
    return temp;
}

//*************************************************************************************************************//**
 /*
 * Purpose  	:   This function return the value of Span_cal Parameter in u16 format
 *
 * @param[in]   :	None
 * @return    	: 	value
 ****************************************************************************************************************/
uint16_t vRead_Span_cal_u16_fromFlash(void)
{
    u16 temp = 0 ;
    temp = (u16)ulSysParmsReadUser(eStore_Span_Cali_Identifier);
   
    return temp;
}

//*************************************************************************************************************//**
 /*
 * Purpose  	:   This function return the value of Cali_Status Parameter
 *
 * @param[in]   :	None
 * @return    	: 	value
 ****************************************************************************************************************/
bool vRead_Cali_Status_fromFlash(void)
{
    unsigned char temp = 0 ;
    temp = (u8)ulSysParmsReadUser(eStore_Status_Cali_Identifier);
    return temp;
}

//*************************************************************************************************************//**
 /*
 * Purpose  	:   This function return the value of Div_cal Parameter
 *
 * @param[in]   :	None
 * @return    	: 	value
 ****************************************************************************************************************/
float vRead_Div_Avg_Store_fromFlash(void)
{
    float temp = 0 ;
    temp = (float)( 1.0 * (u32)ulSysParmsReadUser(eStore_Div_Avg_Store_Identifier) / VALUE_FOR_MULT_DIVI_FLOAT_UL_CONVERSION);

    return temp;
}

//*************************************************************************************************************//**
 /*
 * Purpose  	:   This function return the value of Div_cal Parameter
 *
 * @param[in]   :	None
 * @return    	: 	value
 ****************************************************************************************************************/
uint16_t vRead_Div_Avg_u16_Store_fromFlash(void)
{
    u16 temp = 0 ;
    temp = (u16)ulSysParmsReadUser(eStore_Div_Avg_Store_Identifier);
   
    return temp;
}


//*************************************************************************************************************//**
 /*
 * Purpose  	:   This function return the value of one time calibration flag
 *
 * @param[in]   :	None
 * @return    	: 	flag value true or false
 ****************************************************************************************************************/
bool vRead_OneTimeCalibFlag_fromFlash(void)
{
    unsigned char temp = 0 ;
    temp = (u8)ulSysParmsReadUser(eStore_OneTimeCalibration_Identifier);
    return temp;
}


//*************************************************************************************************************//**
 /*
 * Purpose  	:   Read number of time calibration
 *
 * @param[in]   :	None
 * @return    	: 	Count of number of time calibration
 ****************************************************************************************************************/
u32 vRead_NumberOfTimeCalibration_fromFlash(void)
{
    u32 temp = 0 ;
    temp = ulSysParmsReadUser(eStore_NumberOfTimeCalibration_Identifier);
    return temp;
}


//*************************************************************************************************************//**
 /*
 * Purpose  	:   Read status of last calibration state
 *
 * @param[in]   :	None
 * @return    	: 	e_calibration_status value
 ****************************************************************************************************************/
u8 vRead_CalibrationLastState_fromFlash(void)
{
    u8 e_state = 0 ;
    e_state = ulSysParmsReadUser(eStore_CalibrationLastState_Identifier);
    return e_state;
}




//*************************************************************************************************************//**
 /*
 * Purpose  	:   Read hours when calibration done by CAN
 *
 * @param[in]   :	None
 * @return    	: 	u8
 ****************************************************************************************************************/
u8 vRead_CalibTimeStampHour_fromFlash(void)
{
    u8 temp = 0 ;
    temp = ulSysParmsReadUser(eStore_TimeStampHour_identifier);
    return temp;
}

//*************************************************************************************************************//**
 /*
 * Purpose  	:   Read minutes when calibration done by CAN
 *
 * @param[in]   :	None
 * @return    	: 	u8
 ****************************************************************************************************************/
u8 vRead_CalibTimeStampMinutes_fromFlash(void)
{
    u8 temp = 0 ;
    temp = ulSysParmsReadUser(eStore_TimeStampMinutes_identifier);
    return temp;
}

//*************************************************************************************************************//**
 /*
 * Purpose  	:   Read seconds when calibration done by CAN
 *
 * @param[in]   :	None
 * @return    	: 	u8
 ****************************************************************************************************************/
u8 vRead_CalibTimeStampSeconds_fromFlash(void)
{
    u8 temp = 0 ;
    temp = ulSysParmsReadUser(eStore_TimeStampSeconds_identifier);
    return temp;
}


//*************************************************************************************************************//**
 /*
 * Purpose  	:   Read Am or PM when calibration done by CAN
 *
 * @param[in]   :	None
 * @return    	: 	u8
 ****************************************************************************************************************/
u8 vRead_CalibTimeStampAMorPM_fromFlash(void)
{
    u8 temp = 0 ;
    temp = ulSysParmsReadUser(eStore_TimeStampAMorPM_identifier);
    return temp;
}


//*************************************************************************************************************//**
 /*
 * Purpose  	:   Read Day when calibration done by CAN
 *
 * @param[in]   :	None
 * @return    	: 	u8
 ****************************************************************************************************************/
u8 vRead_CalibTimeStampDay_fromFlash(void)
{
    u8 temp = 0 ;
    temp = ulSysParmsReadUser(eStore_TimeStampDay_identifier);
    return temp;
}


//*************************************************************************************************************//**
 /*
 * Purpose  	:   Read Months when calibration done by CAN
 *
 * @param[in]   :	None
 * @return    	: 	u8
 ****************************************************************************************************************/
u8 vRead_CalibTimeStampMonths_fromFlash(void)
{
    u8 temp = 0 ;
    temp = ulSysParmsReadUser(eStore_TimeStampMonths_identifier);
    return temp;
}


//*************************************************************************************************************//**
 /*
 * Purpose  	:   Read Year when calibration done by CAN
 *
 * @param[in]   :	None
 * @return    	: 	u8
 ****************************************************************************************************************/
u8 vRead_CalibTimeStampYear_fromFlash(void)
{
    u8 temp = 0 ;
    temp = ulSysParmsReadUser(eStore_TimeStampYear_identifier);
    return temp;
}


//*************************************************************************************************************//**
 /*
 * Purpose  	:   Read calibration method
 *
 * @param[in]   :	None
 * @return    	: 	u8
 ****************************************************************************************************************/
u8 vRead_CalibrationMethod_fromFlash(void)
{
    u8 temp = 0 ;
    temp = ulSysParmsReadUser(eStore_CalibrationMethod_identifier);
    return temp;
}

/****************************************************************************************************************/
/* End of file {file_name}                                                                                      */ 
/****************************************************************************************************************/
