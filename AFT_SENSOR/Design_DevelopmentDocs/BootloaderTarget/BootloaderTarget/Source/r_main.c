/* CA78K0R C Source Converter V1.00.03.01 [10 May 2018] */
/*****************************************************************************
 DISCLAIMER
 This software is supplied by Renesas Electronics Corporation and is only
 intended for use with Renesas products. No other uses are authorized. This
 software is owned by Renesas Electronics Corporation and is protected under
 all applicable laws, including copyright laws.
 THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
 LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
 TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
 ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
 FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
 ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
 BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 Renesas reserves the right, without notice, to make changes to this software
 and to discontinue the availability of this software. By using this software,
 you agree to the additional terms and conditions found by accessing the
 following link:
 http://www.renesas.com/disclaimer
 Copyright (C) 2016-2018 Renesas Electronics Corporation. All rights reserved.
******************************************************************************/

                                                                                                                           

 /// \file main.c
 /// \brief BSVI Documentation
 /// \author Copyright (c) 2012 Indication Instruments Limited. All rights reserved.
 /// \version 1.0
 /// \date 19/12/2018
 /// Revision History : Initial version
 
/*!
	\mainpage 

	\section welcome Welcome
	
	**  Bootloader with UDS services Support
	
	\section sect1 "BootLoaderProject" Documentation
*/


/**************************************************************************
** Project :  523265/66  BSVI
** Description: 
**  
**  Bootloader with UDS services
**  
**  
**  
** Device(s) used :
**                   NEC RL78\D1A group R5F10TPJ -  100 pins 
** memory map:
**                   16  KB RAM                 0xFDF00 - 0xFFEDF
**                   8  KB Data Flash          0xF1000 - 0xF2FFF
**                   256 KB flash              0x00000 - 0x1FFFF
**                        
**                   16 MHz External crystal 
**                   1 CAN Module
**                   10 bit ADC
** --------------------------------------------------------------------------
**  COPYRIGHT:  Indication Instruments Limited 
**              Plot No. 19,Sector6
**              Faridabad - 121006         
** --------------------------------------------------------------------------
**  IDE				: CubeSuite+(CS+)
**  Version			: V3.00.00
**  Compiler		: CA78K0R
**  Debuging Tool	: Renesas E1 programmer
**  

**---------------------------------------------------------------------------
**  Co-ordinator                Mukul Goyal
**  Software Manager		  Khusboo Lalani
**---------------------------------------------------------------------------
**  Revision History
**
**  Date        	Name    			Version              Description
** ==========  =======  =======  =====================================
** 01/01/2019   Vishnu Singh/Pratyusg       Initial Version.       


/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_adc.h"
#include "r_cg_timer.h"
#include "r_cg_wdt.h"
#include "r_cg_serial.h"

#include "r_cg_sg.h"
#include "customTypedef.h"
#include "Can.h"
#include "J1939.h" 
#include "r_cg_userdefine.h"
#include "P15765H.h"
#include "TML_14229_UDSH.h"
#include "addressClaim.h"
#include "interrupts.h"
#include "ramVector.h"
#include "dataflash.h"
#include "Flash.h"
#include "FaultDiagnostics.h"

#define IGN_SENSE_TIME            	100     //Ignition ON/OFF time sense in ms

bool bFlashWritingStatus = eClearWritingStatus;
bool bEraseFlashStatus = 0;
bool bCheckSum = FALSE;


u32 gu32Clock1msTicker = 0;

typedef struct
{	
	u16     u16IgnTimeStamp;   //Time stamp for sense Ignition 
	bool 	bIgnitionStatusFlag;
	
} IGN_TYPE;

//Stucture Object declaration-------------------------------------------------
IGN_TYPE sIgnition;
eCLUSTER_STATE_MACHINE clusterStatus = eClusterBoot;



// Pins declarations -----------------------------------------------------

void MY_Application_Run(void);
void dummyfunction(void);
void lcdInit(void);
void UdsServiceSupportedFromMainLoop();
static void _vMapInterrupts();
void dummyfunction(void)
{
	NOP();
}

/* End user code. Do not edit comment generated here */
void R_MAIN_UserInit(void);
void MY_Application_Run(void);
/// @} // endgroup MainLib

/// @} // endgroup MAIN_LIBRARY

/*********************************************************************//**
 *
 * Main system processing 
 *
 * @param      None    
 *
 * @return     None
 *************************************************************************/
void main(void)
{
	static u16 TimeStamp = 0;
	static bool LedFlag = 1;
	static bool isIgnition = FALSE;
	static u16 TimeChecker = 0;
	static bool TempFlag = 0;
	u8 bootloaderIdentifiler =0;
	u8 temp =0;
	u32 SecurityAccessTimestamp = 0;
	
	R_MAIN_UserInit();			
	R_WDT_Restart();				
	vDataFlashinit();
	vDataFlashinit();			
	bootloaderIdentifiler = u8GetBootloaderState();
	EEL_Close();
	FDL_Close();
	
	//check bootloader identfier			
	if(bootloaderIdentifiler == eBootloaderMode)
	{							   
		;										 
	}
	else
	{
		vJ1939_ServiceManager();
		
		if(bGetPGNDataReady(eJIILFaultDiag))//data ready check
		{
			SecurityAccessTimestamp = u32GetClockTicks();
			while ( (temp == FALSE ) && ( (u32GetClockTicks()- SecurityAccessTimestamp) < 40000 ))
			{
				R_WDT_Restart();  
				vJ1939_ServiceManager();
				processDiagnosticsMessages();
				sendDiagnosticsResponse();
				sessionTimeout();
				temp = u8GetIILSecurityAccessStatus();
				if(temp == eIILServerUnlocked)
				{
				 	temp = TRUE;
				}
			}
			vTx_Critical_Internally_Data();
			MY_Application_Run();							
		}
		else
		{
			MY_Application_Run();
		}	
		
	}
	

	while (1U)
	{	
		switch (clusterStatus)
		{
			case eClusterBoot:
				TimeChecker = u16GetClockTicks();
				while(u16GetClockTicks() - TimeChecker < IGN_SENSE_TIME)
				{
					isIgnition = bCheckIgnitionOn();	
				}
				vFlashInit();
			break;
			case eClusterNormalOperation:

				R_WDT_Restart();
				
				
				vJ1939_ServiceManager();				
				processDiagnosticsMessages();
				UdsServiceSupportedFromMainLoop();	
				sendDiagnosticsResponse();
				sessionTimeout();
				vRecoveryFromBusOffState();
				vReCANInit(); 

				if(u16GetClockTicks() - TimeStamp > 500)
				{
					 TimeStamp = u16GetClockTicks();
					 if(LedFlag)
					 {
						 LedFlag = FALSE;
						 HIGH_BEAM_ON;
					 }
					 else
					 {
						LedFlag = TRUE; 
						HIGH_BEAM_OFF;
					 }
					 
				}

				isIgnition = bCheckIgnitionOff(); 
				
			break;
			case eClusterEnterSleep:
				HIGH_BEAM_OFF;
				 R_WDT_Restart(); 
				isIgnition = bCheckIgnitionOn();
			break;
			case eClusterRestart:
					vResetCpu();
			break;
		}
		if( clusterStatus == eClusterBoot )
		{
			if(isIgnition == TRUE)	
			{
				clusterStatus = eClusterNormalOperation;
			}
			else	
			{
				clusterStatus = eClusterEnterSleep; 
			}
			
		}
		else if( clusterStatus == eClusterNormalOperation )					
		{
			if(isIgnition == FALSE ) 			
			{	
				clusterStatus = eClusterEnterSleep; 
			}

			TempFlag = bIsECUResetByService();
			if(TempFlag == TRUE)
			{
			  if(u16GetClockTicks() - TimeChecker > 500)
			  {
				clusterStatus = eClusterRestart;
			  }
			}
			else
			{
				TimeChecker = u16GetClockTicks();				
			}
		}
		else if( clusterStatus == eClusterEnterSleep )	
		{
			if(isIgnition == TRUE ) 
			{
				clusterStatus = eClusterRestart;
			}
		}	
							
							
	}//End while
	
    /* End user code. Do not edit comment generated here */
}

/*********************************************************************//**
 *
 * Enable interrupts and initialize varibale and 
 * check power on reset status before doing stepper self test
 *
 * @param      None    
 *
 * @return     None
 *************************************************************************/

void R_MAIN_UserInit(void)
{
	/* Start user code. Do not edit comment generated here */
	_vMapInterrupts();
	R_TAU0_Channel4_Start();  		//TAU04 1 ms timer for system clock
	vCanInit();
    diagnosticDataInit();
	//Must do enable interrupt  
	EI();
	/* End user code. Do not edit comment generated here */
}

/*********************************************************************//**
 *
 * Check Ignition status with debounce time if ignition singal 
 * have glitch then reset debounce time,call this function when ignition ON 
 *
 *
 * @param      None    
 *
 * @return     None
 *************************************************************************/

bool bCheckIgnitionOff()
{
	bool status = TRUE;

	//Start igniton off time counter and if any glitch occur in signal then reset off time sense counter
	if(IGNITION == LOW)
	{
		if( u16GetClockTicks() - sIgnition.u16IgnTimeStamp  >= IGN_SENSE_TIME)       
		status = FALSE;
	}
	else
	{
		sIgnition.u16IgnTimeStamp = u16GetClockTicks();
	}

	if(status == TRUE)
		sIgnition.bIgnitionStatusFlag = TRUE;
	else
		sIgnition.bIgnitionStatusFlag = FALSE;
	
	return(status);
}

/*********************************************************************//**
 *
 * Check Ignition status with debounce time if ignition singal 
 * have glitch then reset debounce time ,call this function when ignition OFF
 *
 *
 * @param      None    
 *
 * @return     None
 *************************************************************************/

bool bCheckIgnitionOn()
{
	bool status = FALSE;
	
	//Start igniton ON time counter and if any glitch occur in signal then reset On time sense counter

	if(IGNITION == HIGH)
	{
		if( u16GetClockTicks() - sIgnition.u16IgnTimeStamp  >= IGN_SENSE_TIME)
		status = TRUE;
	}
	else
	{
		sIgnition.u16IgnTimeStamp = u16GetClockTicks();
	}

	if(status == TRUE)
		sIgnition.bIgnitionStatusFlag = TRUE;
	else
		sIgnition.bIgnitionStatusFlag = FALSE;

	return(status);
}



/*********************************************************************//**
 *
 * UDS services that support from main loop
 *
 * @param      None    
 *
 * @return     None
 *************************************************************************/
void UdsServiceSupportedFromMainLoop()
{
	u16 temp = 0;
	static u16 EcuResetTimeStamp = 0;

	//***** Routine control service status Handler *********
	
	temp = u16GetRoutineControlServiceStatus();
	
	if(temp != 0)
	{
		vEraseFlash();
		bEraseFlashStatus = TRUE;
	 	RoutineService();	
		// Must call this function before start writting flash  
		vInitFlashImpVariable();
	}
	//******************************************

	//******Transfer Data Service Handler *************
	
	temp = bTransferDataServiceStatus();
	
	if(temp == TRUE)
	{
		temp = bWriteFlash();
		if(temp == TRUE)
		{
		 	bFlashWritingStatus = eSuccess;
			bCheckSum = TRUE;
		}
		else
		{
		  	bFlashWritingStatus = eFail;	//  Fail in case of check sum not okay 
		  	bCheckSum = FALSE;
		}
			
		
	 	TransferData();		
	}
	
	//*****************************************
	//******CheckSum Verification Reprogrammming Counter ***************
	
	temp = bIsCheckSumVarificationDone();
	
	if(temp == TRUE)
	{

		temp = u16GetNoOfBlockReturn();
		//If no of block is equal to Max block means checksum is okay and flash written succesfully
		if(temp == MAX_NO_OF_BLOCK_IN_FLASH)
		{
			vDataFlashinit();
			vDataFlashinit();
			
			vGetEolData(eReProgrammingCounterBytes);
			
			temp = u8ReprogrammingCounter[0];
			temp = temp << 8;
			temp = temp | u8ReprogrammingCounter[1];

			temp++;

			u8ReprogrammingCounter[0] = (u8)(temp >> 8);
 			u8ReprogrammingCounter[1] = (u8)(temp);
			
			vSaveEolData(eReProgrammingCounterBytes);			
			vSaveBootloaderState(eApplicationMode);			
			ClearCheckSumVarificationDoneFlag();	
			
			EEL_Close();
			FDL_Close();
		}
	}
	
	//*****************************************

	//******Ecu reset Service  Handler ***************
	temp = bIsECUResetByService();
	
	if(temp == TRUE)
	{
		EI();
		//500 ms delay must required to reset mcu so that Ecu reset service respons send succesfully 
		if(u16GetClockTicks() - EcuResetTimeStamp > 500)
		{
			vResetCpu();
		}
	}
	else
	{
		EcuResetTimeStamp = u16GetClockTicks();
	}
	  
	//****************************************
	
}


/*********************************************************************//**
 *
 * 1ms System clock
 *
 * @param      None
 *
 * @return     system clock value in u32
 *************************************************************************/
u32 u32GetClockTicks(void)
{
	return gu32Clock1msTicker;
}

/*********************************************************************//**
 *
 * 1ms System clock
 *
 * @param      None
 *
 * @return     system clock value in u16
 *************************************************************************/
u16 u16GetClockTicks(void)
{
	return (u16)gu32Clock1msTicker;
}

/*********************************************************************//**
 * This function hold CPU processing in it as per passing time in ms.
 * 
 *
 * @param      time in ms
 *
 * @return     None 
 *************************************************************************/
void vWatingTimeMs(u16 WaitTime)
{
	u16 WatingClk = 0;
	WatingClk = u16GetClockTicks();
	while((u16GetClockTicks() - WatingClk) < WaitTime)
	{
		R_WDT_Restart(); 
	}
}



/*****************************************************************************

**  Function name:  _vMapInterrupts
**
**  Description:    Mapped Interrupts to vector table
**
**  Parameters:     None
**
**  Return value:   None
**
*****************************************************************************/
static void _vMapInterrupts()
{
	RAM_INTTM01_ISR = &dummyfunction;
	RAM_INTTM03_ISR = &dummyfunction;
	RAM_INTTM04_ISR = &r_tau0_channel4_interrupt;
	RAM_INTTM05_ISR = &dummyfunction;
	RAM_INTTM16_ISR = &dummyfunction;
		
	RAM_INTAD_ISR = &dummyfunction;
	RAM_INTCSI10_ISR = &dummyfunction;
	
	RAM_INTIIC11_ISR = &dummyfunction;
	
	RAM_INTWDTI_ISR = &r_wdt_interrupt;
	
	RAM_INTC0REC_ISR = &MY_Rx_MsgBuf_Processing;
	RAM_INTC0TRX_ISR = &MY_Tx_MsgBuf;
	RAM_INTC0ERR_ISR = &MY_ISR_CAN_Error;
	RAM_INTC0WUP_ISR = &MY_ISR_CAN_Wakeup;	


}

//************Common function in application and bootloader code ********************

/*********************************************************************//**
 *
 * Controller mode status
 *
 * @param      None    
 *
 * @return     None
 *************************************************************************/
u8 GetControllerMode()
{
	// Get value mcu mode value from EEPROM then return
 return(eBootloaderMode);
}

/*********************************************************************//**
 *
 * Vehicle speed in bootloader code it will always zero
 *
 * @param      None    
 *
 * @return     None
 *************************************************************************/
u16 u16GetVehicleSpeed()
{
 return(0);
}

/*********************************************************************//**
 *
 * Engine speed in bootloader code it will always zero
 *
 * @param      None    
 *
 * @return     None
 *************************************************************************/

u16 u16GetEngineSpeed()
{
 return(0);
}

/*********************************************************************//**
 *
 * Service reminder status ,in bootloader code it will always "eServRemdNoDisp"
 *
 * @param      None    
 *
 * @return     None
 *************************************************************************/
eSERVICE_REMINDER_TYPE eGetServiceStatus()
{

  return eServRemdNoDisp;
}

/*********************************************************************//**
 *
 * Clear service reminder status ,in bootloader code keep it blank
 *
 * @param      None    
 *
 * @return     None
 *************************************************************************/

void vClearServiceReminder()
{
  ;
}

/*********************************************************************//**
 *
 * Ckeck depedencies , In bootloader code it will always return TRUE
 *
 * @param      None    
 *
 * @return     None
 *************************************************************************/
bool bCheckDepenDencies()
{
 	return(TRUE);
}

/*********************************************************************//**
 *
 * Return Flash Erasing status
 *
 * @param      None    
 *
 * @return     None
 *************************************************************************/

bool bFlashEarseStatus()
{
	return(bEraseFlashStatus);

}
/*********************************************************************//**
 *
 * Clear Flash Erasing status flag
 *
 * @param      None    
 *
 * @return     None
 *************************************************************************/

void ClearFlashEraseFlag()
{
	bEraseFlashStatus = FALSE;
}

/*********************************************************************//**
 *
 * Return flash writing status
 *
 * @param      None    
 *
 * @return     None
 *************************************************************************/

bool bFlashWriteStatus()
{
 return(bFlashWritingStatus);
}

/*********************************************************************//**
 *
 * Clear flash writing status
 *
 * @param      None    
 *
 * @return     None
 *************************************************************************/

void vClearFlashWriteStatus()
{
	 bFlashWritingStatus = eClearWritingStatus;
}

/*********************************************************************//**
 *
 * Return flash writing checksum status
 *
 * @param      None    
 *
 * @return     None
 *************************************************************************/

bool bCheckSumStatus()
{

u16 temp = 0;
temp = u16GetNoOfBlockReturn();
//If no of block is equal to Max block means checksum is okay and flash written succesfully
if(temp != MAX_NO_OF_BLOCK_IN_FLASH)
	bCheckSum = 0;

 return(bCheckSum);
}

/*********************************************************************//**
 *
 * 
 *
 * @param      None    
 *
 * @return     None
 *************************************************************************/

void MyErrorHandler(void)
{
	WDTE = 0xAC;
	NOP();
}

/*********************************************************************//**
 *
 * Jump in application code 
 *
 * @param      None    
 *
 * @return     None
 *************************************************************************/

void MY_Application_Run(void)
{
	//__far unsigned int *ptr;
	__far unsigned int *ptr;
	pt2Function fp;

	ptr = (__far unsigned int *)( FIRST_USER_FLASH_ADDR );

	if ( *ptr != 0xffff )
	{
		// call user program and never return
		fp = (pt2Function) ptr;
		fp( 0, 0);
	}
	else
	{
		// invalid user reset address
		// some problem return
		return;
	}

	
}


/* End user code. Do not edit comment generated here */
