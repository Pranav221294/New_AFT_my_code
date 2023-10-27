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
 * File Name    : r_main.c
 * Version      : Applilet3 for RL78/D1A V2.04.02.01 [21 Mar 2018]
 * Device(s)    : R5F10CGB
 * Tool-Chain   : CA78K0R
 * Description  : This file implements main function.
 * Creation Date: 18/07/2022
 ***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */

/* Please for Algorithm part go to level.c 
   There you find more condition related for AFT sensor.
   How the formula is used.
*/ 

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_timer.h"
#include "r_cg_wdt.h"
/* Start user code for include. Do not edit comment generated here */
#include "User.h"
#include "Calibration.h"
#include "levelCalc.h"
#include "ReadWriteParamFlash.h"
#include "J1939.h"
#include "Can.h"
#include "GuiCommProtocol.h"
#include "CalibrationByCAN.h"
#include "MovingAverage.h"
#include "DiagnosticData.h"

/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */

/* Below flag set in Timer ISR after getting counts of p0, p1 and p2 */
extern volatile u8 complete_flag ; // used in timer_user.c file in ISR
extern bool isCountStable ;
/* End user code. Do not edit comment generated here */
void R_MAIN_UserInit(void);

/***********************************************************************************************************************
 * Function Name: main
 * Description  : This function implements main function.
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/

// u32 numberOfTimeCali = 0 ;					// for debug
// e_calibration_status e_caliState = e_cali_not_calibrated  ;
// u8 statusCali = 0 ; 

#if ( CALIBRATION_METHOD == BY_CALIBRATION_PIN_IN_HARDWARE )
	e_states_type e_runStatus = e_checkForCalibration;
#elif ( CALIBRATION_METHOD == BY_CAN_BUS)
	e_states_type e_runStatus = e_mainAppStart;
#endif


void main(void)
{

	
	R_MAIN_UserInit();

	/* Set unused pins as zero to reduce the power consumption */
	//vSetUnusedPinsAsZero();
         R_TAU1_Create();
	/* 100 msec timer for counts*/
	//R_TAU0_Channel2_Start();
        R_TAU1_Channel0_Start();
	/* Start user code. Do not edit comment generated here */

	/* can module initialize  */
	vCanInit();

	/* Timer 0 start to capture the frequency */
	R_TAU0_Channel1_Start();

	/* Timer 0 channel 0 start for 1 msec delay */
	R_TAU0_Channel0_Start();

	/* Timer 2 start to generate the PWM */
	R_TAU2_Channel0_Start();

	/* Both Probe deselected */
	vSelectProbe(e_P0_select);

	/* Data flash function init */
	vDataFlashinit();

	/* save default parameter in flash  */
	vSetDefaultVariablesToFlash();
	//calculateParamAndSaveInFlash();	
	
	//testData();

	while (1U)
	{
		

		/* Restart watchdog timer*/
		R_WDT_Restart();
		

		//vJ1939_ServiceManager();
		vRecoveryFromBusOffState();

		/* Send response to GUI */
		sendGuiConnectivityResponse();

		switch (e_runStatus)
		{
		case e_checkForCalibration:
		{
			#if ( CALIBRATION_METHOD == BY_CALIBRATION_PIN_IN_HARDWARE )
				u16 i = 0;
				u8 temp = 0;
				for (i = 0; i < (10u + DEBOUNCE_TIME); i++)	// check 10msec more than debounce time
				{
					/* 1 msec block delay */
					vSetBlockDelayMilli(1);	

					/* Restart watchdog timer*/
					R_WDT_Restart();				

					/* read the status of calibration pin */			
					temp = bReadCaliPinStatus();
				}
				
				/* Calculating,  How many times system tried to calibrate */
				if (temp == LOW)
				{
					u32 NumberOfTimescalibration = vRead_NumberOfTimeCalibration_fromFlash();
					NumberOfTimescalibration++;
					vSave_NumberOfTimeCalibration_inFlash(NumberOfTimescalibration);
				}

				/* if calibration pin is connected to ground, then calibration start */
				#if (ONE_TIME_CALIBRATION == ON )			
					if ( (temp == LOW ) && ( vRead_OneTimeCalibFlag_fromFlash() == CALIBRATION_NOT_DONE ) )
					{
						e_runStatus = e_calibrationStart;
					}
					else
					{
						/* if calibration pin is not connected with ground, then jump to main application */
						e_runStatus = e_mainAppStart;
					}

				#else
					if (temp == LOW )
					{
						e_runStatus = e_calibrationStart;
					}
					else
					{
						/* if calibration pin is not connected with ground, then jump to main application */
						e_runStatus = e_mainAppStart;
					}
				#endif


			#endif

			break;
		}

		case e_calibrationStart:
		{
		
			#if ( CALIBRATION_METHOD == BY_CALIBRATION_PIN_IN_HARDWARE )

			e_calibration_status calibStatus ;
			/* get the calibration status whether its failed or success */
			calibStatus = calibrationStart();

			/* if calibration is successed , then jump to main */
			if (calibStatus == e_cali_success)
			{
				//e_runStatus = e_mainAppStart;
				for (;;)
				{
					/* in existing AFT, Voltage increase and decrease when system calibrated successfully*/
					vSetOutputVoltage(5000);
					vSetBlockDelayMilli(1200);
					vSetOutputVoltage(0);
					vSetBlockDelayMilli(1200);

					/* Restart watchdog timer*/
					R_WDT_Restart();

					/* Out from this loop if disconnect the calibration pin */
					if (bReadCaliPinStatus() == HIGH)
					{
						e_runStatus = e_mainAppStart;
						break;
					}

				}
				

			}

			else
			{
				
				while(1) 
				{
					/* if calibration failed set output voltage 3000 mVolt for failure indications */
					vSetOutputVoltage(3000);

					/* Restart watchdog timer*/
					R_WDT_Restart();
				}
				
				/////////
			}

			#elif ( CALIBRATION_METHOD == BY_CAN_BUS)

				Can_Calibration_State_type calibStatus = e_cali_not_calibrated ;
				
				calibStatus = calibrationStartWithCAN();

				/* save calibration status in flash memory for diagnostic purpose */
				vSave_CalibrationLastState_inFlash(calibStatus);

				/* Reset the system by watchdog */
				vResetSystemByWatchDog();
				
			#endif


			break;


		}

		case e_mainAppStart:
		{
			s16 level = 0;

			/* calculate fuel level here and produce output volatge accordingly */
			if (complete_flag == 1u)
			{
				level = calculateFuelLevel();
				vSetPWM(level);
				complete_flag = 0 ;			// Reset flag

				/* This function handles the communication with GUI */
				GuiCommManager();
				sendDiagnosticDataToGUI();
			}
			
			break;
		}
		}
	}
	/* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
 * Function Name: R_MAIN_UserInit
 * Description  : This function adds user code before implementing main function.
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
void R_MAIN_UserInit(void)
{
	/* Start user code. Do not edit comment generated here */
	EI();
	/* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
