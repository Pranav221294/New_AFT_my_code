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
#include "CustomDataTypes.h"
#include "Configuration.h"

//constant declarations -------------------------------------------------------------------------------


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

			/////////////////////////////// To set default value//////////////////////////////////
			_vSysParmsWrite(0u,eStore_P0_Cali_Identifier);
			_vSysParmsWrite(0u,eStore_P1E_Cali_Identifier);
			_vSysParmsWrite(0u,eStore_P2E_Cali_Identifier);
			_vSysParmsWrite(0u,eStore_P1F_Cali_Identifier);
			_vSysParmsWrite(0u,eStore_P2F_Cali_Identifier);
			_vSysParmsWrite(0u,eStore_m_cali_Identifier);
			_vSysParmsWrite(0u,eStore_Div_cali_Identifier);
			_vSysParmsWrite(0u,eStore_Span_Cali_Identifier);
			_vSysParmsWrite(0u,eStore_Status_Cali_Identifier);
			_vSysParmsWrite(0u,eStore_Div_Avg_Store_Identifier);
			_vSysParmsWrite(0u,eStore_OneTimeCalibration_Identifier);
			_vSysParmsWrite(0u,eStore_NumberOfTimeCalibration_Identifier);
			_vSysParmsWrite(0u,eStore_CalibrationLastState_Identifier);			// default 0 means not calibrated yet
			_vSysParmsWrite(0u,eStore_TimeStampHour_identifier);
			_vSysParmsWrite(0u,eStore_TimeStampMinutes_identifier);
			_vSysParmsWrite(0u,eStore_TimeStampSeconds_identifier);
			_vSysParmsWrite(0u,eStore_TimeStampAMorPM_identifier);
			_vSysParmsWrite(0u,eStore_TimeStampDay_identifier);
			_vSysParmsWrite(0u,eStore_TimeStampMonths_identifier);
			_vSysParmsWrite(0u,eStore_TimeStampYear_identifier);

			#if (CALIBRATION_METHOD == BY_CALIBRATION_PIN_IN_HARDWARE)
				_vSysParmsWrite(11u,eStore_CalibrationMethod_identifier);
			#elif (CALIBRATION_METHOD == BY_CAN_BUS)
				_vSysParmsWrite(23u,eStore_CalibrationMethod_identifier);
			#endif
			
			////////////////////////////////////////////////////////////////////////////////////////
			_vSysParmsWrite(u8SetDefaultFlag1, eSetDefaultFlag1);				// DO NOT MODIFY
			_vSysParmsWrite(u8SetDefaultFlag2, eSetDefaultFlag2);	

		}
	}
	
}




 //*************************************************************************************************************//**
 /*
 * Purpose  	:  	As function '_vSysParmsWrite' is local function for this file thats why we cant use '_vSysParmsWrite' fucntion
					in another file. So to use this function in 'ReadWriteParamFlash.h and in .c ' file we created this global function
					Same Created for read function 
 *
 * @param[in]   :	Value that need to be write
 * @param[in]   :	identifier of that value
 * @return    	: 	None
 ****************************************************************************************************************/
u32 countWriteInFlash = 0 ; // added for debug only, to check multiple write in flash
void vSysParamWriteUser(unsigned long writeParameter, unsigned char u8id)
{
	countWriteInFlash++;
	_vSysParmsWrite(writeParameter, u8id);
}


 //*************************************************************************************************************//**
 /*
 * Purpose  	:  	As function '_ulSysParmsRead' is local function for this file thats why we cant use '_vSysParmsWrite' fucntion
					in another file. So to use this function in 'ReadWriteParamFlash.h and in .c ' file we created this global function
 *
 * @param[in]   :	identifier of that value
 * @return    	: 	value at address of passing identifier
 ****************************************************************************************************************/
unsigned long ulSysParmsReadUser(unsigned char u8id)
{
	unsigned long temp = 0 ;
	temp = _ulSysParmsRead(u8id);
	return temp ;
}
