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
* Copyright (C) 2012, 2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_timer_user.c
* Version      : Applilet3 for RL78/D1A V2.04.03.01 [15 Nov 2019]
* Device(s)    : R5F10DPG
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for TAU module.
* Creation Date: 22/6/2022
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt INTTM01 r_tau0_channel1_interrupt
#pragma interrupt INTTM04 r_tau0_channel4_interrupt
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_timer.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"
#include "StepperDriver.h"
/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_tau0_channel1_interrupt
* Description  : This function is INTTM01 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_tau0_channel1_interrupt(void)
//void r_tau0_channel1_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
	//100us timer interrupt 
	#define SKIP_STEPS  4

	static signed char step[eNumOfMeters]={-1};
	short goingClockWise;
	static int countDown[eNumOfMeters];
	signed char var;
	unsigned char num;
	num = 0;
	
	// Do nothing if in position
	while(num < eNumOfMeters)
	{
		// num = activeMotorNo;
		if (stepperCB[num].s16CurrentTarget != stepperCB[num].s16CurrentPosition && bMoveStepperMotor[num] == TRUE)
		{
			// Using 100usec constant period clock
			if (--countDown[num] <= 0)
			{
				// Reload the count down value
				countDown[num] = stepperCB[num].s16CountdownReload;

				// Set direction flag and check
				if (!(goingClockWise = ((stepperCB[num].s16CurrentTarget - stepperCB[num].s16CurrentPosition) >= 0)))
				{
					// Going CCW so predecrement the step
					if(bStartUp == FALSE) 
					{
						// Update the absolute position
						stepperCB[num].s16CurrentPosition--;
						step[num] = step[num] - 1;
						if (step[num] < 0)
						step[num] = STEP_ARRAY_SIZE - 1;
					}
					else 
					{
						step[num] = step[num] - SKIP_STEPS; 
						stepperCB[num].s16CurrentPosition -= SKIP_STEPS;
						if (step[num] < 0)
						step[num] = STEP_ARRAY_SIZE - SKIP_STEPS + 1; //i have no idea why it works better with +1  
					}
					
					 if(stepperCB[num].s16CurrentPosition < stepperCB[num].S16ZeroOffSet)
					 	stepperCB[num].s16CurrentPosition = stepperCB[num].S16ZeroOffSet;
					
				}
				else
				{
					if(bStartUp == FALSE) 
					{
						// Update the absolute position
						stepperCB[num].s16CurrentPosition++;
						step[num] = step[num] + 1;    
					}
					else 
					{
						step[num] = step[num] + SKIP_STEPS;
						stepperCB[num].s16CurrentPosition += SKIP_STEPS;
					}
					// Post increment the step
					if (step[num] >= STEP_ARRAY_SIZE)
						step[num] = 0;
					
                    if(stepperCB[num].s16CurrentPosition > stepperCB[num].s16MaxNeedlePos + EXTRA_STEPS_COMPENSATE)
						stepperCB[num].s16CurrentPosition = stepperCB[num].s16MaxNeedlePos + EXTRA_STEPS_COMPENSATE;
				}
				//SPD_TablePtr = step;
				var = step[num];

				switch(num) 
				{
					case eDefGauge : 
						MCMP10 	=  MicroStepPWM[var].u16Mtr1Pwm;	//  PWM value comparition resister(sin) 
						MCMP11 	=  MicroStepPWM[var].u16Mtr2Pwm;	//  PWM value comparition resister(cos)
						MCMPC1 	=  MicroStepPWM[var].u8Polarity; 		
					break;
					default :
					/* Do Nothing */
					break;

				};//end switch

				stepperCB[num].s16CountsFromStart++;
			}//end if (--countDown[num]
		}//end if (stepperCB[num].s16CurrentTarget 
		num++;
	}//end while

    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_tau0_channel4_interrupt
* Description  : This function is INTTM04 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_tau0_channel4_interrupt(void)
//void r_tau0_channel4_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
	gu32Clock1msTicker++;
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
