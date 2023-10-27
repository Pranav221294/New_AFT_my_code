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
#include "r_cg_macrodriver.h"
#include "Calibration.h"
#include "r_cg_timer.h"
#include "levelCalc.h"
#include "r_cg_wdt.h"
#include "MovingAverage.h"
#include "Configuration.h"
//#include <stdbool.h>
#include "User.h"
#include "CustomDataTypes.h"
/****************************************************************************************************************/
/****************************************************************************************************************/
/* Defines                                                                                                      */
/****************************************************************************************************************/
#define left_v     5u 		

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

volatile u16 freqCount = 0 ;
//volatile u8 freqCount = 0 ;
volatile u16 TDR10_value = 0 ;
volatile u8 TDR10_overflow = 0;
volatile bool counter_555_255_flag = true; 
Accum_t accu_v;
extern volatile u8 overflow_65535  ;
//extern volatile u16 overflow_65535  ;

extern raw_probe_value p0_raw;
extern raw_probe_value p1_raw;
extern raw_probe_value p2_raw;

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
	// P2.3 = ~ P2.3;
	counter_555_255_flag = false;
	if(freqCount>=255)//255,510,765,1020,355
	{ 
	  // P2.3 = ~ P2.3;
	    TT1 |= _0001_TAU_CH0_STOP_TRG_ON;
        TMMK10 = 1U;        /* disable INTTM10 interrupt */
        TMIF10 = 0U;        /* clear INTTM10 interrupt flag */ 

	   TT0 |= _0002_TAU_CH1_STOP_TRG_ON;
       TMMK01 = 1U;        /* disable INTTM01 interrupt */
       TMIF01 = 0U; 

    
        // TT1 |= _0001_TAU_CH0_STOP_TRG_ON;
        // TMMK10 = 1U;        /* disable INTTM10 interrupt */
        // TMIF10 = 0U;        /* clear INTTM10 interrupt flag */
       
	   TDR10_value = 65535 - TCR10;
	   //TDR10_value = 51999 - TCR10;
	   TDR10_overflow = overflow_65535;
           overflow_65535 = 0;
	   TDR10 = 65535;
	   //TDR10 = 51999;
	   counter_555_255_flag = true; 
		freqCount = 0;
		// P2.3 = ~P2.3;
		 //P3.3 =~P3.3;
	}
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
             probe_selection();
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
                   probe_selection();
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
    ThreeBytes ThreeBytes_temp;
	//tempPxAcc = VALUE_FOR_SUBSTRACTION - p0_in ;
	 // tempPxAcc =((uint16_t)(shifting_of_merge_overflow_n_reg_v( p0_raw.TD10_overflow_v,p0_raw.TD10_value_reg_v,4))) ;
      ThreeBytes_temp = merge_and_get_bytes(p0_raw.TD10_overflow_v, p0_raw.TD10_value_reg_v);
      ThreeBytes_temp = leftShiftBytesN(ThreeBytes_temp,4); 
      tempPxAcc= merge_first_two_bytes(ThreeBytes_temp);

	//caliPara_struct.p0_empty = getAccumalatedCount(accu_v.Accum_p0, tempPxAcc, ACCUMULATED_COUNT_CALI);
	//caliPara_struct.p0_empty = tempPxAcc ;
	 getMovingAveargeOf(e_p0AtCalibration, &tempPxAcc, &caliPara_struct.p0_empty);

	// tempPxAcc =((uint16_t)(shifting_of_merge_overflow_n_reg_v( p1_raw.TD10_overflow_v,p1_raw.TD10_value_reg_v,4))) ;
	  
	  ThreeBytes_temp = merge_and_get_bytes(p1_raw.TD10_overflow_v, p1_raw.TD10_value_reg_v);
      ThreeBytes_temp = leftShiftBytesN(ThreeBytes_temp, 4); ; 
      tempPxAcc= merge_first_two_bytes(ThreeBytes_temp);
	
	getMovingAveargeOf(e_p1AtemptyCalibration,&tempPxAcc, &caliPara_struct.p1_empty);
	//caliPara_struct.p1_empty = getAccumalatedCount(accu_v.Accum_p1, tempPxAcc, ACCUMULATED_COUNT_CALI);
	

	//tempPxAcc = VALUE_FOR_SUBSTRACTION - p2_in ;
	//tempPxAcc = VALUE_FOR_SUBSTRACTION - p1_in ;
	 //tempPxAcc =((uint16_t)(shifting_of_merge_overflow_n_reg_v( p2_raw.TD10_overflow_v,p2_raw.TD10_value_reg_v,4))) ;
      ThreeBytes_temp = merge_and_get_bytes(p2_raw.TD10_overflow_v, p2_raw.TD10_value_reg_v);
      ThreeBytes_temp = leftShiftBytesN(ThreeBytes_temp,4); 
      tempPxAcc= merge_first_two_bytes(ThreeBytes_temp);

	 getMovingAveargeOf(e_p2AtemptyCalibration,&tempPxAcc, &caliPara_struct.p2_empty);
	//caliPara_struct.p2_empty = getAccumalatedCount(accu_v.Accum_p2, tempPxAcc, ACCUMULATED_COUNT_CALI);
				

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
    ThreeBytes ThreeBytes_temp;
	//tempPxAcc = VALUE_FOR_SUBSTRACTION - p1_in ;
	 //tempPxAcc =((uint16_t)(shifting_of_merge_overflow_n_reg_v( p1_raw.TD10_overflow_v,p1_raw.TD10_value_reg_v,4))) ;

	  ThreeBytes_temp = merge_and_get_bytes(p1_raw.TD10_overflow_v, p1_raw.TD10_value_reg_v);
      ThreeBytes_temp = leftShiftBytesN(ThreeBytes_temp, 4); ; 
      tempPxAcc= merge_first_two_bytes(ThreeBytes_temp);
	//tempPxAcc = VALUE_FOR_SUBSTRACTION - p2_in ;
	//caliPara_struct.p1_full = getAccumalatedCount(accu_v.Accum_p1_f, tempPxAcc, ACCUMULATED_COUNT_CALI);
	getMovingAveargeOf(e_p1AtfullCalibration, &tempPxAcc, &caliPara_struct.p1_full);


	//tempPxAcc = VALUE_FOR_SUBSTRACTION - p2_in ;
	//tempPxAcc =((uint16_t)(shifting_of_merge_overflow_n_reg_v( p2_raw.TD10_overflow_v,p2_raw.TD10_value_reg_v,4))) ;
	//tempPxAcc = VALUE_FOR_SUBSTRACTION - p1_in ;
	//caliPara_struct.p2_full = getAccumalatedCount(accu_v.Accum_p2_f, tempPxAcc, ACCUMULATED_COUNT_CALI);
	ThreeBytes_temp = merge_and_get_bytes(p2_raw.TD10_overflow_v, p2_raw.TD10_value_reg_v);
    ThreeBytes_temp = leftShiftBytesN(ThreeBytes_temp,4); 
    tempPxAcc= merge_first_two_bytes(ThreeBytes_temp);
	getMovingAveargeOf(e_p2AtfullCalibration, &tempPxAcc, &caliPara_struct.p2_full);				
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
	
//
//      caliPara_struct.p0_empty    = 10004;
//     caliPara_struct.p1_empty    = 14417;
//     caliPara_struct.p2_empty = 14794;
//     caliPara_struct.p1_full = 24271;
//     caliPara_struct.p2_full = 19917;
     /* get value of raw_p1_span */
	
	
	/* get value of raw_p1_span */
	caliPara_struct.raw_p1_span = caliPara_struct.p1_full - caliPara_struct.p1_empty ;	

	/* get value of raw_p2_span */
	caliPara_struct.raw_p2_span = caliPara_struct.p2_full - caliPara_struct.p2_empty ;	
	
	/* get value of m, associativity in original expression */
	caliPara_struct.constant_m  = (uint16_t)((uint32_t)CONST_14_4_4096 - ( ((uint32_t)CONST_7_07_4096 * (uint32_t)caliPara_struct.raw_p1_span ) / (caliPara_struct.raw_p2_span)))  ; 
	

	/* calculate div at cali value, devide by 100 as in above expressio multiply by 100 */   
	caliPara_struct.div_cal		= (uint16_t)((((uint32_t)caliPara_struct.constant_m * (uint32_t)caliPara_struct.raw_p1_span) / ( caliPara_struct.raw_p2_span)) - CONST_6_3_4096)  ;
	

	/* calculate the p1_span */
	caliPara_struct.p1_span		= (uint16_t)( ((uint32_t)caliPara_struct.raw_p1_span * (uint32_t)4096) / caliPara_struct.div_cal)  ;  
	
	/* Update the calibration status */
	caliPara_struct.cal_status = true;

	/************************** Here we are storing calibration parameter in flash  ********************/ 

	vSave_P0_cal_inFlash(caliPara_struct.p0_empty);
	vSave_P1E_cal_inFlash(caliPara_struct.p1_empty);
	vSave_P2E_cal_inFlash(caliPara_struct.p2_empty);
	vSave_P1F_cal_inFlash(caliPara_struct.p1_full);
	vSave_P2F_cal_inFlash(caliPara_struct.p2_full);
	//vSave_m_inFlash(caliPara_struct.constant_m_f);
	//vSave_Div_cal_inFlash(caliPara_struct.div_cal_f);
	vSave_Div_Avg_Store_inFlash(caliPara_struct.div_cal_f);
	//vSave_Span_cal_inFlash(caliPara_struct.p1_span_f);
	vSave_Cali_Status_inFlash(caliPara_struct.cal_status);

	vSave_m_u16_inFlash(caliPara_struct.constant_m);
	vSave_Div_cal_u16_inFlash(caliPara_struct.div_cal);
	vSave_Span_cal_u16_inFlash(caliPara_struct.p1_span);

}

/****************************************************************************************************************/
/* End of file {file_name}                                                                                      */ 
/****************************************************************************************************************/
