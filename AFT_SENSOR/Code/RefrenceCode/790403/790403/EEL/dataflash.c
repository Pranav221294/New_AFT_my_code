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
#include "dataflash.h"
#include "r_cg_userdefine.h"
#include "r_cg_macrodriver.h"
#include "HourMeter.h"

//constant declarations -------------------------------------------------------------------------------
//Total Engine Hour 3 Byte    = 3
#define TOTAL_ENGINE_HOUR_MEM_START_INDEX				0
#define TOTAL_ENGINE_HOUR_MEM_END_INDEX 				2

//Total Engine  Hour sec mili sec 2 Byte
#define ENG_HR_SEC_MILI_SEC_MEM_START_INDEX				0
#define ENG_HR_SEC_MILI_SEC_MEM_END_INDEX 				1

//local declarations ----------------------------------------------------------------------------------
static eel_status_t 		_snewEelStatus;				// Static Structure for EEl status
static unsigned char __far*  			_pnewVersionStringu8;		// Static pointer
static eel_request_t 		_snewEelRequestStr;			// Static Structure for EEl request
static eel_request_t 		_snewRefrEelRequestStr;		// Static Structure for EEl reference request
static eel_driver_status_t 	_snewEelDriverStatus;		// Static Structure for EEl Driver status

static fdl_status_t 		_snewFdlStatus;				// Static Structure for FDL status
static fdl_request_t 		_snewFdlRequestStr;			// Static Structure for FDL request
static fdl_request_t 		_snewRefrFdlRequestStr;		// Static Structure for FDL reference request



extern    __far const fdl_descriptor_t  _sfdlDescriptorStr;


static u8 u8HourSystemParmeter[HOUR_SYS_PARAMETER];
static u8 u8SecMiliSecSystemParmeter[SEC_MILI_SEC_SYS_PARAMETER];

/*********************************************************************//**
 *
 * Handles Errors while EEL write or read for 3 clock cycles.
 *
 * @param      None 
 *
 * @return     None
 *************************************************************************/
static void _vMyErrorHandler(void)
{
	NOP();NOP();NOP();
}

/*********************************************************************//**
 *
 * Clears all blocks in EEL pool. This is a global function. Can be called from main.
 *
 * @param      None 
 *
 * @return     None
 *************************************************************************/
void vEelFormat()
{
	_snewEelRequestStr.command_enu = EEL_CMD_FORMAT;
    EEL_Execute(&_snewEelRequestStr);
    while (_snewEelRequestStr.status_enu == EEL_BUSY)
    {
      	EEL_Handler();
    }
    if (_snewEelRequestStr.status_enu != EEL_OK)
    {
      	_vMyErrorHandler();
    }	
}

/*********************************************************************//**
 *
 * Initializing EEL pool.
 *
 * @param      None 
 *
 * @return     None
 *************************************************************************/
static void _vEelInit()
{
	_snewEelStatus = EEL_Init();
	if(_snewEelStatus != EEL_OK)
    {
      	_vMyErrorHandler();
	}
	//__nop();
	NOP();
	EEL_Open();
	//__nop();
	NOP();
	EEL_GetDriverStatus((__near eel_driver_status_t*)&_snewEelDriverStatus);		
}


/*********************************************************************//**
 *
 * Initializing FDL pool.
 *
 * @param      None 
 *
 * @return     None
 *************************************************************************/
static void _vFdlInit()
{
	_snewFdlStatus = FDL_Init(&fdl_descriptor_str);
	if(_snewFdlStatus != FDL_OK)
	{
		_vMyErrorHandler();
	}
	else
	{
		NOP();
	}
	NOP();
	FDL_Open();
	NOP();
}

/*********************************************************************//**
 *
 * Initiates EEL bus. Error handling if BUSY.
 *
 * @param      None 
 *
 * @return     None
 *************************************************************************/
static void _vEelStartUp()
{
	_snewEelRequestStr.command_enu = EEL_CMD_UNDEFINED;
	_snewEelRequestStr.identifier_u8 = 0;
	_snewEelRequestStr.address_pu8 = 0;
	_snewEelRequestStr.status_enu = EEL_ERR_PARAMETER;
	_snewEelRequestStr.command_enu = EEL_CMD_STARTUP;
	EEL_Execute(&_snewEelRequestStr);

	while (_snewEelRequestStr.status_enu == EEL_BUSY)
	{
	        EEL_Handler();
	}
	if (_snewEelRequestStr.status_enu != EEL_OK)
	{
	        _vMyErrorHandler();
	}
	if (_snewEelRequestStr.status_enu == EEL_ERR_POOL_INCONSISTENT)
	{
	        vEelFormat();
	}
	if (_snewEelRequestStr.status_enu == EEL_ERR_POOL_EXHAUSTED)
	{
	        vEelFormat();
	}
	EEL_GetDriverStatus((__near eel_driver_status_t*)&_snewEelDriverStatus);	
}


/*********************************************************************//**
 *
 * Initiates FLASH memeory.
 *
 * @param      None 
 *
 * @return     None
 *************************************************************************/
void vDataFlashinit()
{
	_vFdlInit();
	_vEelInit();
	_vEelStartUp();
}

/*********************************************************************//**
 *
 * Write Eol array Data with respect to identifier.
 *
 * @param     Data Array to be written 
 * @param      Identifier 
 *
 * @return     None
 *************************************************************************/
static void _vEolWrite(unsigned char* write_data, unsigned char u8id)
{
	_snewEelRequestStr.command_enu = EEL_CMD_WRITE;
	_snewEelRequestStr.address_pu8 = (__near unsigned char*)&write_data[0];
	_snewEelRequestStr.identifier_u8 = u8id;

	do
	{
	 	EEL_Handler();
	  	EEL_Execute(&_snewEelRequestStr);
	} while (_snewEelRequestStr.status_enu == EEL_ERR_REJECTED);

	while (_snewEelRequestStr.status_enu == EEL_BUSY)
	{
	  	EEL_Handler();
	}

	if (_snewEelRequestStr.status_enu == EEL_ERR_POOL_FULL)
	{
	  _snewRefrEelRequestStr.command_enu = EEL_CMD_REFRESH;
	  _snewRefrEelRequestStr.identifier_u8 = 0;
	  _snewRefrEelRequestStr.address_pu8 = 0;
	  _snewRefrEelRequestStr.status_enu = EEL_ERR_PARAMETER;
	  do
	  {
	          EEL_Handler();
	          EEL_Execute(&_snewRefrEelRequestStr);
	  } while (_snewRefrEelRequestStr.status_enu == EEL_ERR_REJECTED);

	  while (_snewRefrEelRequestStr.status_enu == EEL_BUSY)
	  {
	          EEL_Handler();
	  }

	  if (_snewRefrEelRequestStr.status_enu != EEL_OK)
	  {
	          _vMyErrorHandler();
	  }
	}
	else if (_snewEelRequestStr.status_enu != EEL_OK)
	{
	  	_vMyErrorHandler();
	}		
}

/*********************************************************************//**
 *
 * Read the last written identifier in EEL pool.
 *
 * @param      Data Array to be written 
 * @param      System Parameter Index 
 *
 * @return     None
 *************************************************************************/
static void _vEolRead(unsigned char* data, unsigned char u8id)
{
	_snewEelRequestStr.command_enu = EEL_CMD_READ;
    _snewEelRequestStr.address_pu8 = (__near eel_u8*)&data[0];
    _snewEelRequestStr.identifier_u8 = u8id;
    do
    {
          	EEL_Handler();
          	EEL_Execute(&_snewEelRequestStr);
    } while (_snewEelRequestStr.status_enu == EEL_ERR_REJECTED);

    while (_snewEelRequestStr.status_enu == EEL_BUSY)
    {
            EEL_Handler();
    }

    if (_snewEelRequestStr.status_enu != EEL_OK)
    {
            _vMyErrorHandler();
    }
}

/*********************************************************************//**
 *
 * Write data to EEL Pool.
 *
 * @param      Data Array to be written 
 * @param      System Parameter Identifier 
 *
 * @return     None
 *************************************************************************/
static void _vSysParmsWrite(unsigned long int write_data, unsigned char u8id)
{
	_snewEelRequestStr.command_enu = EEL_CMD_WRITE;
	_snewEelRequestStr.address_pu8 = (__near unsigned char*)&write_data;
	_snewEelRequestStr.identifier_u8 = u8id;

	do
	{
	 	EEL_Handler();
	  	EEL_Execute(&_snewEelRequestStr);
	} while (_snewEelRequestStr.status_enu == EEL_ERR_REJECTED);

	while (_snewEelRequestStr.status_enu == EEL_BUSY)
	{
	  	EEL_Handler();
	}

	if (_snewEelRequestStr.status_enu == EEL_ERR_POOL_FULL)
	{
	  _snewRefrEelRequestStr.command_enu = EEL_CMD_REFRESH;
	  _snewRefrEelRequestStr.identifier_u8 = 0;
	  _snewRefrEelRequestStr.address_pu8 = 0;
	  _snewRefrEelRequestStr.status_enu = EEL_ERR_PARAMETER;
	  do
	  {
	          EEL_Handler();
	          EEL_Execute(&_snewRefrEelRequestStr);
	  } while (_snewRefrEelRequestStr.status_enu == EEL_ERR_REJECTED);

	  while (_snewRefrEelRequestStr.status_enu == EEL_BUSY)
	  {
	          EEL_Handler();
	  }

	  if (_snewRefrEelRequestStr.status_enu != EEL_OK)
	  {
	          _vMyErrorHandler();
	  }
	}
	else if (_snewEelRequestStr.status_enu != EEL_OK)
	{
	  	_vMyErrorHandler();
	}		
}

/*********************************************************************//**
 *
 * Read the last written identifier in EEL pool.
 *
 * @param      System Parameter Identifier 
 *
 * @return     Data read from eeprom
 *************************************************************************/
static unsigned long int _ulSysParmsRead(unsigned char u8id)
{
	static unsigned long int _ulEolReadData;
	_snewEelRequestStr.command_enu = EEL_CMD_READ;
    _snewEelRequestStr.address_pu8 = (__near eel_u8*)&_ulEolReadData;
    _snewEelRequestStr.identifier_u8 = u8id;
    do
    {
      	EEL_Handler();
      	EEL_Execute(&_snewEelRequestStr);
    } while (_snewEelRequestStr.status_enu == EEL_ERR_REJECTED);

    while (_snewEelRequestStr.status_enu == EEL_BUSY)
    {
        EEL_Handler();
    }

    if (_snewEelRequestStr.status_enu != EEL_OK)
    {
        _vMyErrorHandler();
    }
	return _ulEolReadData;
	
}

/*********************************************************************//**
 *
 * Read Hour Parameter Values from memory.
 *
 * @param      None
 *
 * @return     None
 *************************************************************************/
void _vReadHourSysParms(void)
{	
	u32 HourData = 0u;
	u32 HourDataDuplicate = 0u;
	u8 MinuteData = 0u;
	u8 MinuteDataDuplicate = 0u;
	u8 HrSecondsData = 0u;
	u8 HrSecondsDataDuplicate = 0u;
	u8 HrMiliSecData = 0u;
	u8 HrMiliSecDataDuplicate = 0u;
	
	
	
	NOP();NOP();
	/* Hr Mili seconds --------------------------------*/
	HrMiliSecData = _ulSysParmsRead(eSecMiliSecIdentifier);
	HrMiliSecDataDuplicate = _ulSysParmsRead(eSecMiliSecIdentifier_Duplicate); 	/* Dupli Copy */
	if(HrMiliSecData == HrMiliSecDataDuplicate)
	{
		vSetHoursMiliSeconds(HrMiliSecData);							/* Set Engine hour mili Seconds */
	}
	else if(HrMiliSecData < HrMiliSecDataDuplicate)
	{
		vSetHoursMiliSeconds(HrMiliSecDataDuplicate);
	}
	else if(HrMiliSecData > HrMiliSecDataDuplicate)
	{
		vSetHoursMiliSeconds(HrMiliSecData);
	}
	else
	{
		/*Do Nothing*/
	}
	NOP();NOP();
	/*Hr Second  ----------------------------------------*/
	HrSecondsData = _ulSysParmsRead(eSecondIdentifier);
	HrSecondsDataDuplicate = _ulSysParmsRead(eSecondIdentifier_Duplicate); 		/* Dupli Copy */
	if(HrSecondsData == HrSecondsDataDuplicate)
	{
		vSetHoursSecondValue(HrSecondsData);							/* Set Engine hour mili Seconds */
	}
	else if(HrSecondsData < HrSecondsDataDuplicate)
	{
		vSetHoursSecondValue(HrSecondsDataDuplicate);
	}
	else if(HrSecondsData > HrSecondsDataDuplicate)
	{
		vSetHoursSecondValue(HrSecondsData);					/* Set Engine hour  Seconds */
	}
	else
	{
		/*Do Nothing*/
	}
	NOP();NOP();
	/*Hr Minutes  ----------------------------------------*/						
	MinuteData = _ulSysParmsRead(eMinuteIdentifier);
	MinuteDataDuplicate = _ulSysParmsRead(eMinuteIdentifier_Duplicate); 			/* Dupli Copy */
	if(MinuteData == MinuteDataDuplicate)
	{
		vSetEngineHoursMinutes(MinuteData);							/* Set Engine hour mili Seconds */
	}
	else if(MinuteData < MinuteDataDuplicate)
	{
		vSetEngineHoursMinutes(MinuteDataDuplicate);
	}
	else if(MinuteData > MinuteDataDuplicate)
	{
		vSetEngineHoursMinutes(MinuteData);					/* Set Engine hour  Seconds */
	}
	else
	{
		/*Do Nothing*/
	}
	NOP();NOP();
	/*Total Hr   ----------------------------------------*/			
	HourData = _ulSysParmsRead(eHourIdentifier);	
	HourDataDuplicate = _ulSysParmsRead(eHourIdentifier_Duplicate); 				/* Dupli Copy */
	if(HourData == HourDataDuplicate)
	{
		vSetTotalEngineHoursValue(HourData);							/* Set Engine hour mili Seconds */
	}
	else if(HourData < HourDataDuplicate)
	{
		vSetTotalEngineHoursValue(HourDataDuplicate);
	}
	else if(HourData > HourDataDuplicate)
	{
		vSetTotalEngineHoursValue(HourData);					/* Set Engine hour  Seconds */
	}
	else
	{
		/*Do Nothing*/
	}
	
}



/*********************************************************************//**
 *
 * Packing hour Parameter and save in  memory.
 * ex. Hour = 0x12F934 , in zero location goes Msb byte ,means byte 0x12 
 *	  Fraction and second is merge in 2 byte so 
 * @param      None
 *
 * @return     None
 *************************************************************************/
void _vSaveHourSysParms(void)
{	
	u32 Temp = 0;	
	 
	Temp = u32GetTotalEngineHours();			  /* Read total engine hours */
	_vSysParmsWrite(Temp,eHourIdentifier);
	NOP();NOP();
	_vSysParmsWrite(Temp,eHourIdentifier_Duplicate);/* Duplicate identifire ------------------------*/
}

/*********************************************************************//**
 *
 * write Hr minutes Parameter Values in memory.
 *
 * @param      None
 *
 * @return     None
 *************************************************************************/
void _vWriteHourMinutesValue(void)
{
    u8 Temp1 = 0;
	
    Temp1 = u8GetEngineHoursMinutes();						/* Read total engine hours minutes */
   _vSysParmsWrite(Temp1,eMinuteIdentifier);
    NOP();NOP();
   _vSysParmsWrite(Temp1,eMinuteIdentifier_Duplicate);	   	/* Duplicate identifire ------------------------*/
   
}

/*********************************************************************//**
 *
 *  Second Parameter and save in  memory.
 * 
 * @param      None
 *
 * @return     None
 *************************************************************************/
void _vSaveHrSecondsMiliSecSysParms(void)
{
    u8 Temp1 = 0;
	u8 Temp2 = 0;
		
	Temp1 = u8GetHoursMiliSeconds();						/* Read total mili sec	*/
	Temp2 = u8GetHoursSecondValue();
	_vSysParmsWrite(Temp1,eSecMiliSecIdentifier);
	_vSysParmsWrite(Temp1,eSecMiliSecIdentifier_Duplicate);	/* Duplicate identifire ------------------------*/
	NOP();NOP();NOP();
	_vSysParmsWrite(Temp2,eSecondIdentifier);			/* Read total  sec	 */
	_vSysParmsWrite(Temp2,eSecondIdentifier_Duplicate); /* Duplicate identifire ------------------------*/
	
}


/*********************************************************************//**
 *
 * This Function sets all the required variables to DATA FLASH.
 *
 * @param      None
 *
 * @return     None
 *************************************************************************/
void vSetDefaultVariablesToFlash(void)
{
	unsigned char u8SetDefaultFlag1 = 0u;
	unsigned char u8SetDefaultFlag2 = 0u;
	u8  i = 0;

	u8SetDefaultFlag1 = _ulSysParmsRead(eSetDefaultFlag1);
	u8SetDefaultFlag2 = _ulSysParmsRead(eSetDefaultFlag2);

	if(u8SetDefaultFlag1 != 0xCC && u8SetDefaultFlag2 != 0xCD)
	{
		// If condition is false the code will again read for valid datas, if not 
		// found all the necessary variables will be initialized. 
		u8SetDefaultFlag1 = _ulSysParmsRead(eSetDefaultFlag1);
		u8SetDefaultFlag2 = _ulSysParmsRead(eSetDefaultFlag2);
		if(u8SetDefaultFlag1 != 0xCC && u8SetDefaultFlag2 != 0xCD)
		{
			u8SetDefaultFlag1 = 0xCC;
			u8SetDefaultFlag2 = 0xCD;

			_vSysParmsWrite(0u,eBootLoaderIdentifier);
			_vSysParmsWrite(0u,eHourIdentifier);
			_vSysParmsWrite(0u,eMinuteIdentifier);
			_vSysParmsWrite(0u,eSecondIdentifier);
			_vSysParmsWrite(0u,eSecMiliSecIdentifier);
			_vSysParmsWrite(0u,eHourIdentifier_Duplicate);
			_vSysParmsWrite(0u,eMinuteIdentifier_Duplicate);
			_vSysParmsWrite(0u,eSecondIdentifier_Duplicate);
			_vSysParmsWrite(0u,eSecMiliSecIdentifier_Duplicate);
			
			_vSysParmsWrite(u8SetDefaultFlag1, eSetDefaultFlag1);				// DO NOT MODIFY
			_vSysParmsWrite(u8SetDefaultFlag2, eSetDefaultFlag2);	

		}
	}
	
}
