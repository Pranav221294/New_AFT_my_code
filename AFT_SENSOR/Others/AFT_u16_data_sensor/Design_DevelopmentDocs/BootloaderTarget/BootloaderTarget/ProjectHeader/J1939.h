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
/*****************************************************************************
**  Module:J1939.h
**
** ---------------------------------------------------------------------------
**  Description:
**      Header file for J1939.h
**
*****************************************************************************/
#ifndef _J1939_H_
#define _J1939_H_

// constant declarations -----------------------------------------------------

#include "r_cg_macrodriver.h"
#include "r_cg_wdt.h"
#include "customTypedef.h"

// constant declarations -----------------------------------------------------
#define NO_OF_DTC_CODE        						70 //maximum is 64 DTCs

#define BADDATA						0xFE 				
#define ERRORDATA					0XFF

#define BADDATA_2BYTE				0xFE00 
#define BADDATA_4BYTE				0xFE000000

#define TEMP_UND_RANGE            	0x64    //J1939 value at temp 60 deg 
#define TEMP_OVR_RANGE            	0xA1    //J1939 value at temp >120 deg 

#define OAT_UND_RANGE            	0x2220    //J1939 value at 0 deg temp
#define OAT_OVR_RANGE            	99    	  // 99 deg Temp

#define RX_QUE_SIZE      20		//Buffer for receive CAN msg
#define ERR_OK           0U		//Not used            

// public interface ---------------------------------------------------------
// (Not used ) Required ISO PGN List 
#define PGN60928                60928           // TxRx 0xEE00 ISO Adress claim
#define PGN59392                59392           // TxRx 0xE800 ISO Acknowledgement


// (Not used ) Proprietary PGN List
#define PGN_USER_INTERFACE      65401           // Rx Proprietary PGN used for calibration
#define PGN_USER_INTERFACE_ext  0x18FF7900      // 29 bit Tx Proprietary used for calibration

#define J1939_NULL_ADDRESS    	254

#define J1939_GLOBAL_ADDRESS    255
#define J1939_MAX_PF_PDU1FORMAT 239             // From J1939-21 section 5.2.5 "PDU Specific (PS)
                                                // PDU1 range is 0-239 and is for PDU destination format
                                                // PDU2 range is 240-255 and is for PDU group extension values
//PGN transmmit for request
#define PGN65257				65257			// AFE
#define PGN65257_ext            0x18FEE900		// 29 bit ID priority 7
#define PGN65219				65219			// TRANSMISSION RANGE LOW
#define PGN65219_ext            0x1CFEC300		// 29 bit ID priority 7
#define PGN65088                65088			//Trailer
#define PGN65088_ext            0x18FE4000      // 29 bit ID priority 6


//Request PGN
#define PGN59904                59904           // Rx 0xEA00 ISO Request PGN (data length = 3)
#define PGN59904_0x00_ext		0x18EA0017
#define PGN59904_0x21           59904           // Rx 0xEA00 ISO Request PGN (data length = 3)
#define PGN59904_0x21_ext		0x18EA2117



typedef enum
{
	EmptyTxSPN,
	eNumofTxDataTypes
} eTxDataType;

typedef enum
{
	EmptyTxPGN, 
	eNoOfPgnTx	
} tPGN_TxIndexerType;


typedef enum
{
   	eJIILFaultDiag,
    eNumofRxDataTypes  
} eRxDataType;

typedef enum
{

	eRx65303_0xE1 =0,
	eNoOfPgnRx
	
} tPGN_RxIndexerType;                                                

// Identifier format
// bits | 28 27 26 | 25 | 24 | 23 22 21 20 19 18 17 16 | 15 14 13 12 11 10 09 08 | 07 06 05 04 03 02 01 00 |
//      | priority | R  | DP | PDU format (PF)         | PDU specific (PS)       | Source address          |
//                 |<-------------------------- PGN Id ------------------------->|

//NEC MCU have a architecture little endian so change stucture according to it.
typedef union
{
    u32  Identifier;
    struct
    {
	u8 SourceAddress;            	// 0 - 7
	u8 PduSpecific;              	// 8 - 15
	u8 PduFormat;                	// 16 - 23
        struct
        {
            u8 DataPage       :1;    // 24
            u8 Reserved       :1;    // 25
            u8 Priority       :3;    // 26 - 28
            u8 UnusedMSB      :3;    // 29 - 31
        } sBitField;
        
    } sIdField;
} sJ1939_Id_Struct_Type;

// J1939 message buffer structure
typedef struct
{
    sJ1939_Id_Struct_Type   sMsgID;           // Message ID
    u32                     u32TimeStamp;     // Time of message reception
    u32                     u32ValidDataTS;   // Time stamp of last valid data
                                              // The message processor will copy the timestamp into this variable
                                              // if at least one of the datums was valid
    bool                    bDataRdy;         // True = fresh data for processing
    u8                      ucNumofRxBytes;   // 8 or less received
    u8                      u8data[8];
	bool				 	bDataOutDate;
} sJ1939_RX_MSG_TYPE;



extern sJ1939_RX_MSG_TYPE   sJ1939RxQueue[RX_QUE_SIZE];
extern sJ1939_RX_MSG_TYPE   J1939RxData[eNoOfPgnRx];


// Global prototypes --------------------------------------------------------

/// @addtogroup J1939_LIBRARY
/// @{
 
/// @defgroup J1939HeaderLib Global functions used in j1939 file
//// 
///   
/// @addtogroup J1939HeaderLib
/// @{

void vResetDTCParameters(void);
u8 u8GetDTCStatus(void);
u8 u8GetDTCCount(void);
u8 u8GetDTCSA(void);

void vJ1939Init(void);
void vJ1939_ServiceManager(void);
bool bProcessISORequest(u32);
u8 u8GetBitData(eRxDataType);
u32  u32GetPGNdata(eRxDataType);
void vJ1939OnFullRx(void);
bool bGetPGNTimeOut(eRxDataType);
bool bGetPGNDataReady(eRxDataType);
void ClearPGNDataReady(eRxDataType );
void vJ1939_TxMsgEnableDisable(tPGN_TxIndexerType  , bool );
void vJ1939TxInit(void);





/// @} // endgroup J1939HeaderLib

/// @} // endgroup J1939_LIBRARY


#endif
