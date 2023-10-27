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

/****************************************************************************************************************/
/* Includes                                                                                                     */
/****************************************************************************************************************/

#include "Calibration.h"
#include "r_cg_timer.h"
#include "levelCalc.h"
#include "r_cg_wdt.h"
#include "MovingAverage.h"
#include "Configuration.h"
//#include <stdbool.h>


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

extern volatile u8 complete_flag ; 
extern volatile u32 p0_in ;
extern volatile u32 p1_in ;
extern volatile u32 p2_in  ;

/****************************************************************************************************************/
/* Type definitions                                                                                             */
/****************************************************************************************************************/


/****************************************************************************************************************/
/* Local variables                                                                                              */
/****************************************************************************************************************/

s_Parameter_type caliPara_struct;

volatile u32 freqCount = 0 ;
Accum_t accu_v;


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
 * Purpose  	:  	This function return the High, Low Status of Calibration after checking debouncing
 *
 * @param[in]   :	None
 * @return    	: 	High or Low
 ****************************************************************************************************************/
bool bReadCaliPinStatus(void)
 {
	 static u32 timeStamp1 = 0 ;
	 static u32 timeStamp2 = 0 ;	 
	 static u8 status = HIGH ;
	 
	 if (CALIBRATION_IN == HIGH)
	 {
		 if ( (u32GetMilTick() - timeStamp2) > DEBOUNCE_TIME )
		 {
			 status = HIGH;
		 }
		 timeStamp1 = u32GetMilTick();
	 }
	 
	 else
	 {
		if ( (u32GetMilTick() - timeStamp1) > DEBOUNCE_TIME )
		 {
			 status = LOW;
		 } 
		 timeStamp2 = u32GetMilTick();
	 }
	 
	 return status;
 }
 
 //*************************************************************************************************************//**
 /*
 * Purpose  	:  	This function increament the freqCount variable
 *
 * @param[in]   :	None
 * @return    	: 	None
 ****************************************************************************************************************/
 void freqIncr(void)
{
	freqCount++;
}
 

 //*************************************************************************************************************//**
 /*
 * Purpose  	:  	This function calculate the empty value p0,p1,p2 and full value p1,p2 and return the status whether it is 
					success or failure.  This function called when we do calibration by Pin.
 *
 * @param[in]   :	None
 * @return    	: 	e_calibration_status
 ****************************************************************************************************************/

e_calibration_status calibrationStart(void)
{
	u16 tempRead = 0 ; 
	u32 timeStamp = 0 ;
	

	e_calibration_status cali_enum = e_cali_failed ;
	e_calibration_status calibLastState = e_cali_not_calibrated ;	
	
	caliPara_struct.cal_status = false;					// initially calibration status is false

	timeStamp = u32GetMilTick();

	/* calibration start for empty level */
	while ( (bReadCaliPinStatus() == LOW) && (cali_enum != e_cali_empty_success))
	{

		/* Restart watchdog timer*/
		R_WDT_Restart();

		cali_enum  = e_cali_empty_start ;

		if ( (u32GetMilTick() - timeStamp) < CALIBRATION_TIME)
		{

			if (complete_flag == 1)
			{
				vEmptyCalibration();
				complete_flag = 0 ;
			}			
			
		}

		else
		{
			cali_enum = e_cali_empty_success ;

			/* Setting 500mV at output just to indicate that calibration at empty level is completed, we can set any voltage */
			vSetOutputVoltage(500);		// 500 mean 500mV
		}


		// incase power off while calibrating at low level, then also state should be stored 
		if (calibLastState != cali_enum)
		{
			vSave_CalibrationLastState_inFlash(cali_enum);
			calibLastState = cali_enum ;
		}
		
	}

	/* if calibration pin disconnected before time, change the status of calibration */
	if (cali_enum == e_cali_empty_start )
	{
		cali_enum = e_cali_empty_failed ;
	}

	/* calibration state saved in flash */
	vSave_CalibrationLastState_inFlash(cali_enum);

	if (cali_enum == e_cali_empty_success)
	{
		/* waiting for dissonection of calibration wire with ground */
		while(bReadCaliPinStatus() != HIGH)
		{
			/* Restart watchdog timer*/
			R_WDT_Restart();			
		}		

		/* waiting for connection of calibration wire with ground to start calibration for full level*/
		while(bReadCaliPinStatus() != LOW)
		{
			/* Restart watchdog timer*/
			R_WDT_Restart();		
		}	

		timeStamp = u32GetMilTick();

		/* calibration start for full level */
		while( (bReadCaliPinStatus() == LOW)  && ( cali_enum != e_cali_full_success ))
		{

			/* Restart watchdog timer*/
			R_WDT_Restart();
		
			cali_enum  = e_cali_full_start ;
			if ( (u32GetMilTick() - timeStamp) < CALIBRATION_TIME)
			{

				if (complete_flag == 1)
				{
					vFullCalibration();
					complete_flag = 0 ;
				}
					
			}

			else
			{
				cali_enum = e_cali_full_success ;
			}

			// incase power off while calibrating at high level, then also state should be stored 
			if (calibLastState != cali_enum)
			{
				vSave_CalibrationLastState_inFlash(cali_enum);
				calibLastState = cali_enum ;
			}
			
		}

		/* if calibration pin disconnected before time, change the status of calibration */
		if (cali_enum == e_cali_full_start )
		{
			cali_enum = e_cali_full_failed ;
		}

		/* calibration state saved in flash */
		vSave_CalibrationLastState_inFlash(cali_enum);

	}


	/* if calibration is successfull then calculate the parameter and store in flash */
	if (cali_enum == e_cali_full_success)
	{
		calculateParamAndSaveInFlash();
		
		#if (ONE_TIME_CALIBRATION == ON)						// this macro define in Calibration.h file
			vSave_OneTimeCalibFlag_inFlash(true);				// calibration done
		#endif	

		/******************************************************************************************/
		/* Now probe is fully calibrated */
		cali_enum = e_cali_success ;

	}

	/* calibration state saved in flash */
	vSave_CalibrationLastState_inFlash(cali_enum);

	/* return the status of calibration */
	return cali_enum ;
	
}

 //*************************************************************************************************************//**
 /*
 * Purpose  	:  	This function get the value of p0, p1, p2 at empty level
 *
 * @param[in]   :	None
 * @return    	: 	none
 ****************************************************************************************************************/
void vEmptyCalibration(void)
{
	uint16_t tempPxAcc = 0  ;

	tempPxAcc = VALUE_FOR_SUBSTRACTION - p0_in ;
	//caliPara_struct.p0_empty = getAccumalatedCount(accu_v.Accum_p0, tempPxAcc, ACCUMULATED_COUNT_CALI);
	//caliPara_struct.p0_empty = tempPxAcc ;
	getMovingAveargeOf(e_p0AtCalibration, &tempPxAcc, &caliPara_struct.p0_empty);

	tempPxAcc = VALUE_FOR_SUBSTRACTION - p1_in ;
	//tempPxAcc = VALUE_FOR_SUBSTRACTION - p2_in ;
	caliPara_struct.p1_empty = getAccumalatedCount(accu_v.Accum_p1, tempPxAcc, ACCUMULATED_COUNT_CALI);
	

	tempPxAcc = VALUE_FOR_SUBSTRACTION - p2_in ;
	//tempPxAcc = VALUE_FOR_SUBSTRACTION - p1_in ;
	caliPara_struct.p2_empty = getAccumalatedCount(accu_v.Accum_p2, tempPxAcc, ACCUMULATED_COUNT_CALI);
				

}

 //*************************************************************************************************************//**
 /*
 * Purpose  	:  	This function get the value of p1 and p2 at full level
 *
 * @param[in]   :	None
 * @return    	: 	None
 ****************************************************************************************************************/
void vFullCalibration(void)
{
	u16 tempPxAcc = 0  ;

	tempPxAcc = VALUE_FOR_SUBSTRACTION - p1_in ;
	//tempPxAcc = VALUE_FOR_SUBSTRACTION - p2_in ;
	caliPara_struct.p1_full = getAccumalatedCount(accu_v.Accum_p1_f, tempPxAcc, ACCUMULATED_COUNT_CALI);
	

	tempPxAcc = VALUE_FOR_SUBSTRACTION - p2_in ;
	//tempPxAcc = VALUE_FOR_SUBSTRACTION - p1_in ;
	caliPara_struct.p2_full = getAccumalatedCount(accu_v.Accum_p2_f, tempPxAcc, ACCUMULATED_COUNT_CALI);
					
}



 //*************************************************************************************************************//**
 /*
 * Purpose  	:  	This function calculate the parameters and save in flash data memory
 *
 * @param[in]   :	None
 * @return    	: 	None
 ****************************************************************************************************************/
void calculateParamAndSaveInFlash(void)
{
	

   //   caliPara_struct.p0_empty    = 534;
   //  caliPara_struct.p1_empty    = 10758;
   //  caliPara_struct.p2_empty = 7802;
   //  caliPara_struct.p1_full = 17536;
   //  caliPara_struct.p2_full = 12266;
     /* get value of raw_p1_span */
	
	
	caliPara_struct.raw_p1_span = caliPara_struct.p1_full - caliPara_struct.p1_empty ;	

	/* get value of raw_p2_span */
	caliPara_struct.raw_p2_span = caliPara_struct.p2_full - caliPara_struct.p2_empty ;	
	
	/* get value of m, associativity in original expression */
	caliPara_struct.constant_m_f  = (CONST_14_4 - ( (CONST_7_07 * caliPara_struct.raw_p1_span ) / (caliPara_struct.raw_p2_span)))  ; 
	caliPara_struct.constant_m_f = getFloatUpto3place(caliPara_struct.constant_m_f);

	/* calculate div at cali value, devide by 100 as in above expressio multiply by 100 */   
	caliPara_struct.div_cal_f		= (((caliPara_struct.constant_m_f * caliPara_struct.raw_p1_span) / ( caliPara_struct.raw_p2_span)) - CONST_6_3)  ;
	caliPara_struct.div_cal_f = getFloatUpto3place(caliPara_struct.div_cal_f);

	/* calculate the p1_span */
	caliPara_struct.p1_span_f		= ( (caliPara_struct.raw_p1_span ) / caliPara_struct.div_cal_f)  ;  
	
	/* Update the calibration status */
	caliPara_struct.cal_status = true;

	/************************** Here we are storing calibration parameter in flash  ********************/ 

	vSave_P0_cal_inFlash(caliPara_struct.p0_empty);
	vSave_P1E_cal_inFlash(caliPara_struct.p1_empty);
	vSave_P2E_cal_inFlash(caliPara_struct.p2_empty);
	vSave_P1F_cal_inFlash(caliPara_struct.p1_full);
	vSave_P2F_cal_inFlash(caliPara_struct.p2_full);
	vSave_m_inFlash(caliPara_struct.constant_m_f);
	vSave_Div_cal_inFlash(caliPara_struct.div_cal_f);
	vSave_Div_Avg_Store_inFlash(caliPara_struct.div_cal_f);
	vSave_Span_cal_inFlash(caliPara_struct.p1_span_f);
	vSave_Cali_Status_inFlash(caliPara_struct.cal_status);

}

/****************************************************************************************************************/
/* End of file {file_name}                                                                                      */ 
/****************************************************************************************************************/
