
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
/*****************************************************************************
 **  Module: J1939.c
 **
 ** ---------------------------------------------------------------------------
 **  Description:
 **      Provides all J1939 functions except address claim
 **
 *****************************************************************************/

#include "J1939.h" 
#include "Can.h"
#include "r_cg_userdefine.h"
#include "User.h"
#include "CalibrationByCAN.h"
#include "CalibrationByCAN.h"
#include "DiagnosticData.h"

// constant declarations -----------------------------------------------------
extern gui_connect_struct_type sGUIconnect;


PGN_DATA_TYPE sPGNFF69_oneTimeCalib = {0x00, 0x00, 0x00};
PGN_DATA_TYPE sPGNFF70_timeStamp	= {0x00, 0x00, 0x00};

static u8 rx_data_cnt_user = 0 ; 
static u8 rx_msg_DLC_user = 0  ;
static u32 MsgBuf_address_user = 0  ;
u8 debugData = 0  ;
/*********************************************************************//**
 *
 * This method is called from the CAN receive interrupt.
 * 
 *
 * @param      None
 *
 * @return     None
 *************************************************************************/
void vJ1939OnFullRx(void)
{
	u32 MsgBuf_address;
	u8 rx_msg_DLC;	
	u32 rx_msg_ID;
	u8  rx_buffer_number;
	u8 rx_data_cnt;
	u16 PGN =0;
	

	C0INTS = 0x0002;	
	rx_buffer_number = C0LIPT;						// Get receive message buffer number
	MsgBuf_address = (MSGBUF_BASE_ADD + (0x10 * rx_buffer_number));		// Set CAN message buffer[n] register address

			
	// Get the ID
	*((u32 *)(MsgBuf_address + 0x0e)) = 0x0004;		// Clear DN bit
	rx_msg_DLC = *((u8 *)(MsgBuf_address + 0x08));	// Get receive message data length
	rx_msg_ID = (*((uint32_t *)(MsgBuf_address + 0x0c))<< 16 )  | (*((uint32_t *)(MsgBuf_address + 0x0A)));
	rx_msg_ID &= ~(0x80000000U);                                    // clear the IDE bit

	PGN = (rx_msg_ID>> 8) & 0x0003FFFFU;

	rx_data_cnt_user = rx_data_cnt ;
	rx_msg_DLC_user  = rx_msg_DLC ;
	MsgBuf_address_user = MsgBuf_address ;

	switch(PGN)
	{
		case PGN_FF61_startLiveData:
			setGuiCommStatus(e_liveDataStarted);
			//sPGNFF61.DataReady = TRUE;

			// Deposit the data
			// for(rx_data_cnt = 0 ;((rx_data_cnt < rx_msg_DLC) && (rx_data_cnt < 8)) ; rx_data_cnt++)
			// {
			// 	sPGNFF61.data[rx_data_cnt] = *((u8 *)(MsgBuf_address + (0x01 * rx_data_cnt)));	
			// }
			// sPGNFF61.CanTimeStamp =  u32GetMilTick();
			break;

		case PGN_FF62_stopLiveData:
			setGuiCommStatus(e_liveDataStopped);
			break;
		
		case PGN_FF63_getStoredData:
			setGuiCommStatus(e_getStoredData);
			break;

		case PGN_FF64_ConnectWithGUI:

			#if ( CALIBRATION_METHOD == BY_CAN_BUS)

			setGuiCommStatus(e_connectWithGUIforCali);
			sGUIconnect.dataReady = true ;
			sGUIconnect.CANTimeStamp = u32GetMilTick();

			#endif
			
			break;

		case PGN_FF68_DisconnectWithGUI:
			setButtonState(e_buttonClickDisconnect);
			break;

		case PGN_FF65_StartCaliEmpty:
			setButtonState(e_buttonClickEmptyCalStart);
			break;

		case PGN_FF66_StartCaliFull:
			setButtonState(e_buttonClickFullCalStart);
			break;

		case PGN_FF67_CalibrationReset:
			setButtonState(e_buttonClickResetCalibration);
			break;

		case PGN_FF69_OneTimeCalibrationOff:

			#if ( CALIBRATION_METHOD == BY_CALIBRATION_PIN_IN_HARDWARE)

			extractDataOfPGN(&sPGNFF69_oneTimeCalib);
			setGuiCommStatus(e_oneTimeCalibOff);
			debugData = extractByteFromPGN(&sPGNFF69_oneTimeCalib, eData_D4);

			#endif

			break;

		case PGN_FF70_getCalibTimeStamp:
			extractDataOfPGN(&sPGNFF70_timeStamp);
			break;

		case PGN_FF71_getDiagnosticData:
			setDiagnosticDataRequestFlag(true);
			break;

		default:
			break;
	}
					

}

void extractDataOfPGN(PGN_DATA_TYPE *ptrPGN)
{
	for(rx_data_cnt_user = 0 ;((rx_data_cnt_user < rx_msg_DLC_user) && (rx_data_cnt_user < 8)) ; rx_data_cnt_user++)
	{
		ptrPGN->data[rx_data_cnt_user] = *((u8 *)(MsgBuf_address_user + (0x01 * rx_data_cnt_user)));	
	}
}

u8 extractByteFromPGN(PGN_DATA_TYPE *ptrPGN, PGN_DataByte_eType eData_Dx)
{
	return ptrPGN->data[eData_Dx];
}
