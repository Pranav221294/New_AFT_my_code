
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
#include "addressClaim.h"
#include "Can.h"
#include "r_cg_userdefine.h"
#include "SoftVersion.h"

// constant declarations -----------------------------------------------------


// J1939 PGN List (3 msb = priority
//Transmitted PGN(Not implemented transmite data)
//#define PGN64773            		64773			// J1939 PGN Direct lamp control data 
//#define PGN64773_ext            	0x18FD0500    	// 29 bit ID priority 6
//Received PGN 
// J1939 WATER IN FUEL  
#define PGN65279	              65279U         	
//#define PGN65279_0x00_ext            0x18FEFF00    	// 29 bit ID priority 6
// Gear shiftup/down
//#define PGN65279_0x21                65279         	
//#define PGN65279_0x21_ext            0x18FEFF21    	// 29 bit ID priority 6

// J1939 Aftertreatment 1 Diesel Exhaust Fluid Tank 1 Information 
#define PGN65110                65110U         	
//#define PGN65110_ext            0x18FE5600    	// 29 bit ID priority 6
#define PGN65252                65252U         	// Engine pre Heat 1
//#define PGN65252_ext            0x18FEE400    	// 29 bit ID priority 6 
#define PGN65262				65262U		  	// J1939 coolant Temperature
//#define PGN65262_ext			0x18FEEE00	  	// 29 bit ID priority 6
//Check Engine , ABS Mulfunction,Check Mulfunction and stop engine DM1 
#define PGN65226                65226U        	
//#define PGN65226_ext            0x18FECA00	  	// 29 bit ID priority 6
#define PGN65265	           65265U           //Cruise control
//#define PGN65265_0x00_ext       0x18FEF100	  	// 29 bit ID priority 6
#define PGN65265_0x21           65265U           //Parking break
//#define PGN65265_0x21_ext       0x18FEF100	  	// 29 bit ID priority 6

#define PGN61445                61445U           //Nuteral
//#define PGN61445_ext            0x18F00500      // 29 bit ID priority 6
#define PGN61444                61444U           //RPM
//#define PGN61444_ext            0x0CF00400      // 29 bit ID priority 3 
#define PGN64917                64917U           //Trans. Flud. temp.
//#define PGN64917_ext            0x18FD9500	  	// 29 bit ID priority 6
#define PGN64892                64892U           //DPF and High exhaust system temp.
//#define PGN64892_ext            0x18FD7E00      // 29 bit ID priority 6
#define PGN64775                64775U           //Malfunction indicator
//#define PGN64775_ext            0x18FD0700      // 29 bit ID priority 6
#define PGN65098                65098U           //Check Transmission 
//#define PGN65098_ext            0x18FE4A00      // 29 bit ID priority 6
#define PGN61441                61441U           //Check Transmission 
//#define PGN61441_ext            0x18F00100      // 29 bit ID priority 6
#define PGN64814                64814U           //Check Transmission 
//#define PGN64814_ext            0x0CFD2E00       // 29 bit ID priority 6
#define PGN64972				64972U			// Left/RightTurn
//#define PGN64972_ext            0x0CFDCC00		// 29 bit ID priority 3
#define PGN65089				65089U			// Front and rear lamp
//#define PGN65089_ext            0x0CFE4100		// 29 bit ID priority 3
#define PGN65103				65103U			// Left/RightTurn
//#define PGN65103_ext            0x18FE4F00		// 29 bit ID priority 6
#define PGN65276				65276U			// Fuel Gauge
//#define PGN65276_ext            0x18FEFC00		// 29 bit ID priority 6
#define PGN65269				65269U			// OAT
//#define PGN65269_ext            0x18FEF500		// 29 bit ID priority 6
#define PGN64712				64712U			// Engine Mode Selection
//#define PGN64712_ext            0x18FEF500		// 29 bit ID priority 6
#define PGN61442				61442U			// Engine Mode Selection
//#define PGN61442_ext            0x1CF00200		// 29 bit ID priority 3
#define PGN65263				65263U			// Engine oil Pressure 	

#define PGN61440				61440U			// Retarder Requesting Brake Light 

#define PGN65102				65102U			// DC1 - Door Open

#define PGN65378				65378U			// VECU - Proprietery 18FF6221

#define PGN64964				64964U			// EBC5

#define PGN64998 				64998U 			//Low Brake fluid
#define PGN65376				65376U			//RPAS Buzzer

#define PGN60416				60416U //TP_CM 
#define PGN60160				60160U //TP_DT
#define PGN60160_0x03			60160U //TP_DT
#define PGN60160_0x10			60160U //TP_DT
#define PGN60160_0x0B			60160U //TP_DT
#define PGN60160_0x33			60160U //TP_DT


#define PGN65226_0				0xCAU				
#define PGN65226_1				0xFEU
#define PGN65226_2				0U




//Transmmiting PGN
#define PGN65361				65361U			// ODO data,Engine Hour data
#define PGN65361_ext            0x18FF5117U		// 29 bit ID priority 6
// OdoTripA,TRIP A Fuel Eff. Km/L or km/Kg, TripA Fuel Eff. L/hr,HourTripA
#define PGN65362				65362U			
#define PGN65362_ext            0x18FF5217U		// 29 bit ID priority 6
// OdoTripB,TRIP B Fuel Eff. Km/L
#define PGN65363				65363U			
#define PGN65363_ext            0x18FF5317U		// 29 bit ID priority 6
// Fuel level ,DTE,BrakeWear,ClutchWear
#define PGN65364				65364U			
#define PGN65364_ext            0x18FF5417U	// 29 bit ID priority 6
//
#define PGN65365				65365U			
#define PGN65365_ext            0x18FF5517U		// 29 bit ID priority 6
//trip reset DA and SA TBD
#define PGN56832				56832U
#define PGN56832_ext            0x1CDE0017U		// 29 bit ID priority 7
// Air Pressure Front & rear
#define PGN65198				65198U			
#define PGN65198_ext            0x18FEAE17U		// 29 bit ID priority 6

#define PGN65276				65276U
#define PGN65276_ext			0x18FEFC17U

#define PGN65426				65426U
#define PGN65426_ext			0x0CFF9247U

//Source Address macro

#define EMS_SA1_EXT						0x00U
#define BCM_SA_EXT						0x21U
#define ABS_SA_EXT						0x0BU
#define TCU_SA_EXT						0x03U
#define RETARDER_ECU_SA_EXT				0x10U
#define TPMS_SA_EXT						0x33U



#define RTS_TIMEOUT						1500U
#define BAM_TIMEOUT						750U

//ABS malfunction trailer
//Exhaust break PGN TBD	
 
#define BITDATA                     	0xFFU    // indicates bit data
#define SERVICE_INTERVAL            	10U      // service time interval in msec
#define SEND_ON_DEMAND              	0U

typedef enum
{
	ePGN_J1939 = 0U,
	ePGN_NMEA,
	ePGN_FAST_PACKET
} PGN_TYPE;



// local declarations --------------------------------------------------------


typedef enum
{
	eEmsSA = 0U,
	eRetarderEcuSA,
	eAbsSA,
	eTcuSA,
	eTpmsSA,
	NumberOfSA
} eSourceAddType;


typedef enum
{
	eNoNewMultiPacket,
	eTPCMRTS,
	eTPCMBAM,
	eTPDT
}MULTI_PACKET_STATE;

//variables used for multipacket data
// multipacket timestamp holds time of last frame reception
static u32 u32MultiPacketTimer[NumberOfSA];
//indicates if multipacket frames are being received
static u8 u8MultiPacketDM1[NumberOfSA] = {eNoNewMultiPacket,eNoNewMultiPacket,eNoNewMultiPacket,eNoNewMultiPacket,eNoNewMultiPacket}; 
//holds the address of multipacket being processed
static u8 u8MultiFrameSA[NumberOfSA] = {J1939_NULL_ADDRESS,J1939_NULL_ADDRESS,J1939_NULL_ADDRESS,J1939_NULL_ADDRESS,J1939_NULL_ADDRESS};	
// number of frames in the multipacket being processed
static u8 u8DM1NumFrames = 0U;
//TRUE if fresh DM1 DTCs have been received
static bool u8DTCready = FALSE;
//source address of the ECU that sent DTCs
static u8 u8DTCSA = J1939_NULL_ADDRESS;
//number of DTCs in DM1
static u8 u8DTCCount = 0U;
static u16 u16lTotalBytes = 0U;



typedef struct
{
	bool	bEnabled;          // not related to address claim
	bool    bSendOnDemand;     // used used for on demand and retry output
	u8      u8data[8];         // Data buffer
	u32     u32TimeStamp;      // Last output tim	
	
} J1939_TX_DATA_TYPE;

typedef struct
{
	u8    u8data[8];
	u32   u32TimeStamp;     	// Time of message reception
} J1939_RX_DATA_TYPE;



typedef struct
{
	u32	PGN_ext;        // J1939 29 bit ID
	u32	timeInterval;   // If != 0 then periodic output interval in msec.
	                    // If == 0, then output is on demand rather than periodic.
} J1939_TX_TYPE;

typedef struct
{
	u8	PGNindex;           // index into sPGNconstantsTx
	u8	byteIndex;          // index into the PGN message data
	u8	byteSize;           // number of bytes of data
	u8	bitMask;            // useful only for bit data
	s16	dataOffset;         // j1939 data offset
	u32	bitRes;             // j1939 bit resolution
	u32	overrangeData;      // data to output if calculation results in a value beyond acceptable range
	u32	underrangeData;      // data to output if calculation results in a value beyond acceptable range
	u32	u32TimeStamp;
} PGNdataType;

#define MAX_OVERRANGE 	20U    // maximum percent overrange

//Can transmit not implement 
typedef struct
{
	u16         PGN;
	u32         ID;         // 29 bit ID
	PGN_TYPE    PGNtype;
	u16         rate;       // in msec
} tPGNconstTxType;                                      


const tPGNconstTxType sPGNconstantsTx[eNoOfPgnTx] = 
{                                                     
	//  PGN         	ID              	PGNtype         rate     	  
	{   PGN65303,   	PGN65303_ext,   	ePGN_J1939,      SEND_ON_DEMAND    },  
};

// The bit resolution in this case is the reciprocal of the
const PGNdataType sPGNdataTx[eNumofTxDataTypes] =                                     // J1939 bit resolution multiplied by 100 for integer math
{                                                                   
	//  PGNIndex    	byteIndex   byteSize    bitMask     dataOffset  bitRes  overrangeData  		
	{   eTx65303,   	0U,          4U,          0x00U,       0U,         	1U, 		0xFFU	},		// ODO data			1.1   
};


typedef struct
{
	u16		PGN;
	u8      u8SourceAdd;
	u8		u8Priority;
	void	(*RxFunct)(sJ1939_RX_MSG_TYPE *);    // pointer to PGN processing method
	
} tPGNsonstRxType;

const PGNdataType sPGNdataRx[eNumofRxDataTypes] =
{
	//  PGNindex    byteIndex  byteSize   		bitMask     dataOffset  	bitRes      overrangeData underrange   		In ms            
	{   eRx65110,   	0U,          1U,          0x00U,       0,          1U,   	   BADDATA,			0U,				3000U	},   //  0.4%/bit   DEF Gauge 	    SPN 1761   1 Def have .4 resolution but we are using direct j1939 data	
	{   eRx65110,   	4U,          1U,          0x1FU,       0,          BITDATA,   	BADDATA,		0U,				3000U	},   //  0.4%/bit   DEF Gauge Lamp 	SPN 5245   1 Def lamp have, we are using direct j1939 data	
	{	eRx59904,		0U,			 4U,		  0x00U,	   0,		   1U,			0xFFFFFFFF,		0U,				3000U	},	 // Request PGN for software version read.
};

sJ1939_RX_MSG_TYPE   sJ1939RxQueue[RX_QUE_SIZE];
sJ1939_RX_MSG_TYPE   J1939RxData[eNoOfPgnRx]; 
J1939_TX_TYPE        J1939_Tx[eNoOfPgnTx];
DTCdata DTC_J1939[NO_OF_DTC_CODE];
u8 u8DtcFramNum = 0U;
// Create an array corresponding to sPGNconstantsTx above for the PGN data
J1939_TX_DATA_TYPE  J1939TxData[eNoOfPgnTx]; 
static bool bSpnDataReadyStatus[eNumofRxDataTypes];
static u32 u32DtcFrameData =0;

// Private prototypes --------------------------------------------------------

/// @addtogroup J1939_LIBRARY
/// @{
 
/// @defgroup J1939Lib Private functions used in J1939 File
//// 
///   
/// @addtogroup J1939Lib
/// @{


static void _vPGN65110_DEF(sJ1939_RX_MSG_TYPE *msg);
static void _vPGN59904_SoftVersionRead(sJ1939_RX_MSG_TYPE *msg);
static void _vResetDM1Reception(u8 SourceAdd);
static void _J1939_RxManager(void);
static void _J1939_TxManager(void);

/// @} // endgroup J1939Lib

/// @} // endgroup J1939_LIBRARY


// prototype dependent definitions ------------------------------------------ 
// This needs to be defined after prototype definitions because they are used in this structure
const tPGNsonstRxType sPGNconstantsRx[eNoOfPgnRx] =
{
	//  PGN        SorceAdd   Priority      		RxFunct             				  			
	{   PGN65110,  0x00U,   	0x06,		_vPGN65110_DEF },
	{   PGN59904,  0x00U,   	0x06,		_vPGN59904_SoftVersionRead },
};
	

/*********************************************************************//**
 *
 * J1939 services main loop
 *
 * @param      None    
 *
 * @return     None
 *************************************************************************/
void vJ1939_ServiceManager(void)
{
	static u32 serviceTime = 0U;

	if ((u32GetClockTicks() - serviceTime) >= SERVICE_INTERVAL)
	{
		// Service the J1939 received messages
		_J1939_RxManager();

		// Service the J1939 transmit messages		
		_J1939_TxManager();

		// Reset the timer
		serviceTime = u32GetClockTicks();
	}
}


/*********************************************************************//**
 *
 * Initialize receive and transmit message structures. Set all PGN
 * message data to invalid.
 *
 * @param      None    
 *
 * @return     None
 *************************************************************************/
void vJ1939Init(void)
{
	s16 i, j;

	// Initialize the receive queue
	for (i = 0U; i < RX_QUE_SIZE; ++i)
	{
		sJ1939RxQueue[i].bDataRdy = FALSE;
	}
	// Initialize the Tx data structure
	for (i = 0; i < eNoOfPgnTx; ++i)
	{
		J1939TxData[i].bEnabled = FALSE;
		J1939TxData[i].bSendOnDemand = FALSE;
		J1939TxData[i].u32TimeStamp = u32GetClockTicks();
		// Set all data invalid
		for (j = 0U; j < 8U; ++j)
		J1939TxData[i].u8data[j] = 0xFFU;
		
	}
		
	// Initialize the Rx data structure
	for (i = 0U; i < eNoOfPgnRx; ++i)
	{
		J1939RxData[i].u32TimeStamp = 0;
		J1939RxData[i].bDataOutDate = TRUE;
		J1939RxData[i].bDataRdy = FALSE;
		// Set all data invalid
		for (j = 0U; j < 8U; ++j)
		{
			J1939RxData[i].u8data[j] = 0xFFU;
		}
		
	}
}

/*********************************************************************//**
 *
 * Tx message buffer initialization
 *
 * @param      None    
 *
 * @return     None
 *************************************************************************/
void vJ1939TxInit(void)
{
	s16 i, j;
	// Initialize the Tx data structure
	for (i = 0U; i < eNoOfPgnTx; ++i)
	{
		J1939TxData[i].bEnabled = TRUE;
		J1939TxData[i].bSendOnDemand = FALSE;
		J1939TxData[i].u32TimeStamp = u32GetClockTicks();
		// Set all data invalid
		for (j = 0U; j < 8U; ++j)
		{
			J1939TxData[i].u8data[j] = 0xFFU;
		}
		
	}
	
}

/*********************************************************************//**
 *
 * Processs all interested received J1939 messages
 *
 * @param      None    
 *
 * @return     None
 *************************************************************************/
static void _J1939_RxManager(void)
{
	u32 PGN,ID;	
	sJ1939_RX_MSG_TYPE *msg,*UDSbufPtr,UDSMsg;	   
   	u16  RxQuePush;
	u16 i, timeNow;
	u8 RxQuePop;
	u8 SOURCEADD;
	u8 Priority;
	u8  framNum, frameIndex = 0U;	
	u8 SourceAddType = 0U;

	u8DtcFramNum = 0U;
	
	// Check the entire input queue for fresh messaages
	for (RxQuePop = 0U; RxQuePop < RX_QUE_SIZE; ++RxQuePop)
	{
		if (sJ1939RxQueue[RxQuePop].bDataRdy)
		{
			// Process fresh message
			msg = &sJ1939RxQueue[RxQuePop];
			UDSMsg = sJ1939RxQueue[RxQuePop];

			// Extract the PGN
			ID = msg->sMsgID.Identifier;
			Priority = (msg->sMsgID.Identifier >> 24) & 0x1F000000U;
			PGN = (msg->sMsgID.Identifier >> 8) & 0x0003FFFFU;
			SOURCEADD = (msg->sMsgID.Identifier) & 0x000000FFU;
			
			// Check all other PGN inputs
			for(i=0U; i < eNoOfPgnRx; ++i)
			{
				if(sPGNconstantsRx[i].u8Priority == 6 && PGN == sPGNconstantsRx[i].PGN  && SOURCEADD == sPGNconstantsRx[i].u8SourceAdd )
				{	
					sPGNconstantsRx[i].RxFunct(msg);
				}						
			}
					
			// if message is valid
			if ((msg->sMsgID.sIdField.PduFormat <= J1939_MAX_PF_PDU1FORMAT)
			// and it's for me
			&& ((msg->sMsgID.sIdField.PduSpecific == ucGetMyClaimedAddress())
			// or a broadcast message
			|| (msg->sMsgID.sIdField.PduSpecific == J1939_GLOBAL_ADDRESS)))
			{
				// Remove destination address from specific PGN number
				PGN ^= msg->sMsgID.sIdField.PduSpecific;

				if (bIsAddressClaimed())
				{
				// Special processing for these PGNs if already claimed an address
					switch (PGN)
					{
						case PGN59904 :
							// This request has come in after successful address claim
							// We may be getting bumped
							vRxISOrequest(&sJ1939RxQueue[RxQuePop]);
							if (bHasMyClaimedAddressedBeenBumped())
							{
							// Pause communications
							timeNow = u16GetClockTicks();
							while ((u16GetClockTicks() - timeNow) < 250U)
							R_WDT_Restart();//WDog1_Clear();
							vClearAddressClaimBumped();
							}
						break;

						case PGN60928 :
							vRxISOaddressClaim(&sJ1939RxQueue[RxQuePop]);
						break;
					}
				}
				else if (PGN == PGN59904)
				{
					// This request may be for an address claim
					vRxISOrequest(&sJ1939RxQueue[RxQuePop]);
				}
			}
			//}//end if(commInfo.networkCommRxEnabled == TRUE)
			// Clear the queue slot
			sJ1939RxQueue[RxQuePop].bDataRdy = FALSE;
		}// end if (sJ1939RxQueue[RxQuePop].bDataRdy)
	}//end for (RxQuePop = 0; RxQuePop < RX_QUE_SIZE; ++RxQuePop)
}

/*********************************************************************//**
 *
 * Processs all transmit J1939 messages
 *
 * @param      None    
 *
 * @return     None
 *************************************************************************/
static void _J1939_TxManager(void)
{
	static u8 n = 0U;
	u8 TxReturnCode;
	bool StatusFlag = FALSE;

//If Network communication disable then dont tx CAN messaage
//	if(commInfo.networkCommTxEnabled == FALSE)
//	{
//		return;
//	}
	
	//Check CAN Transmmision Buffer availability 

//	StatusFlag = bCheckCanTxBufferAvailable();
//	if(StatusFlag == FALSE)
//	{
//		return;	//return if nuffer is not empty 
//	}
	
	++n;			
	if(n >= eNoOfPgnTx)
	{
		n =0U;
	}	
	
	if (J1939TxData[n].bEnabled)
	{
		// Determine whether sending on demand or periodic
		if (((sPGNconstantsTx[n].rate != SEND_ON_DEMAND)
		&& ((u32GetClockTicks() - J1939TxData[n].u32TimeStamp) >= sPGNconstantsTx[n].rate))
		|| J1939TxData[n].bSendOnDemand)
		{
			switch (sPGNconstantsTx[n].PGNtype)
			{
				case ePGN_J1939 :
					// Engine instance is part of the PGN
					vTxMsgBufProcessing(sPGNconstantsTx[n].ID,MSG_LEN,J1939TxData[n].u8data,J1939_TX);	
				break;				
			}			
			// Reset the timer and on demand flag
			J1939TxData[n].u32TimeStamp = u32GetClockTicks();
			J1939TxData[n].bSendOnDemand = FALSE;			
		}
	}
	
}

/*********************************************************************//**
 *
 * Enable Disable Transmit PGN 
 *
 * @param      None    
 *
 * @return     None
 *************************************************************************/
 void vJ1939_TxMsgEnableDisable(tPGN_TxIndexerType eTxPgn , bool EnableFlag)
{
	J1939TxData[eTxPgn].bEnabled = EnableFlag;
}


/*********************************************************************//**
 *
 * Enable Disable Transmit PGN 
 *
 * @param      None    
 *
 * @return     None
 *************************************************************************/
void vJ1939_TxSendOnDemandEnable(tPGN_TxIndexerType eTxPgn )
{
	J1939TxData[eTxPgn].bSendOnDemand = TRUE;
}

/*********************************************************************//**
 *
 * Enable Disable Transmit PGN 
 *
 * @param      None    
 *
 * @return     None
 *************************************************************************/
void vJ1939_TxSendOnDemandDisable(tPGN_TxIndexerType eTxPgn )
{
	J1939TxData[eTxPgn].bSendOnDemand = FALSE;
}
/*********************************************************************//**
 *
 * (Not use in code) Determines whether or not the PGN is supported for ISO requests.
 * If it is supported, it is then transmitted.
 *
 * @param      PGN    
 *
 * @return     TRUE if PGN is supported, FALSE otherwise
 *************************************************************************/
bool bProcessISORequest(u32 PGN)
{
	s16 n;
	bool foundPGN;

	// Find a matching PGN
	foundPGN = FALSE;
	n = -1;
	do 
	{
		++n;
		foundPGN = (bool)(sPGNconstantsTx[n].PGN == PGN);
	} while ((n < eNoOfPgnTx) && (!foundPGN));

	if (foundPGN)
	{
		// Set flag to force immediate output
		if(n < eNoOfPgnTx)
		{
			J1939TxData[n].bSendOnDemand = TRUE;
		}
	}

	return foundPGN;
}


/*********************************************************************//**
 *
 * (Not using transmite )Convert input data into PGN specification and deposit into the
 * PGN data for the output message.
 *
 * @param      data type,
 *             data,
 *             +- percent overrange   
 *
 * @return     None
 *************************************************************************/
void vPutPGNdata(eTxDataType dtype, u32 rawData, s16 overrange)
{
	u32 data;
	u8 *dataPtr;
	
	struct 
		{ 
		u8 byteSize;
		u8 bitMask; 
	} b;

	// Set a pointer to the PGN message data
	dataPtr = (u8 *)&J1939TxData[sPGNdataTx[dtype].PGNindex].u8data[sPGNdataTx[dtype].byteIndex];

	if (dtype < eNumofTxDataTypes)
	{
		// Valid data type - determine whether it is byte or bit data
		if (sPGNdataTx[dtype].bitRes == BITDATA)
		{
			// Bit data --------------
			// Get the bit mask and clear the bits
			b.bitMask = sPGNdataTx[dtype].bitMask;
			*dataPtr &= b.bitMask;

			// Shift the raw data bits up to the proper position
			while ((b.bitMask & 1) == 1)
			{
				rawData <<= 1;
				b.bitMask >>= 1;
			}

			// Set the bits
			*dataPtr |= rawData;
		}
		else
		{	
			// unused and needs to be modified for future use
			// Byte data ------------- 
			if (overrange >= MAX_OVERRANGE)
			{
				data = sPGNdataTx[dtype].overrangeData;
			}
			else
			{
				// Convert rawData to PGN format for integer math
				 if(dtype == eJSoftVersionDataTx)	
				 {
					data = rawData;
				 }
				//data = ( ( (rawData*(u32)sPGNdataTx[dtype].bitRes)) - sPGNdataTx[dtype].dataOffset);
				b.byteSize = sPGNdataTx[dtype].byteSize;							
				do 
				{
				 	*dataPtr++ = (u8)data;
					data >>= 8;
				} while (--b.byteSize != 0U);
						 				 
			}
			
		}
		
	}
	
}

/*********************************************************************//**
 *
 * Convert input data into PGN specification and deposit into the
 * PGN data for the output message
 *
 * @param      data type,
 *             data,
 *             +- percent overrange   
 *
 * @return     None
 *************************************************************************/
u32 u32GetPGNdata(eRxDataType dtype)
{
	u32 data = 0U;	
	u8 i;
	union 
	{
		u8 byteSize;
		u8 bitMask; 
	} b;

	// Get the data bit mask
	b.bitMask = sPGNdataRx[dtype].bitMask;

	if (dtype < eNumofRxDataTypes)
	{
		// Valid data type - determine whether it is byte or bit data
		if (sPGNdataRx[dtype].bitRes == BITDATA)
		{
			// Set a pointer to the PGN message data
			data = J1939RxData[sPGNdataRx[dtype].PGNindex].u8data[sPGNdataRx[dtype].byteIndex];
			// Keep only the relevant bits of the data
			data &= ~(u8)b.bitMask;
			// Shift them down to the least significant bit location
			while ((b.bitMask & 1U) == 1U)
			{
				b.bitMask >>= 1;
				data >>= 1;
			}
		}
		else
		{
			// Byte data -------------
			// Get the data byte size
			b.byteSize = sPGNdataRx[dtype].byteSize;
			if(b.byteSize==1U)
			{
				data = (u8)J1939RxData[sPGNdataRx[dtype].PGNindex].u8data[sPGNdataRx[dtype].byteIndex];

				if(data < sPGNdataRx[dtype].overrangeData ) 
				{				
				   if(data <= sPGNdataRx[dtype].underrangeData)
						data = sPGNdataRx[dtype].underrangeData;
					data = (data * (u32)sPGNdataRx[dtype].bitRes) +  sPGNdataRx[dtype].dataOffset;
					
				}
			}
			else
			{
				for( i = 0U; i< b.byteSize; i++)
				{
					data |= (u32)J1939RxData[sPGNdataRx[dtype].PGNindex].u8data[sPGNdataRx[dtype].byteIndex + i] << (i*8);
				}
			}
			
			if(data < sPGNdataRx[dtype].overrangeData ) 
			{
			   if(data <= sPGNdataRx[dtype].underrangeData)
			   	{
			   		data = sPGNdataRx[dtype].underrangeData;
			   	}
				data =  (data * sPGNdataRx[dtype].bitRes)  +  sPGNdataRx[dtype].dataOffset;
				
			}
					  
		}
		
	}//end if (dtype < eNumofRxDataTypes)
	return data;
}

/*********************************************************************//**
 *
 * (Not using this function) Gets the latest bit data from PGN received message buffer. Bits are
 * shifted down to the least significant bits before returning.
 *
 * @param      data type to fetch  
 *
 * @return     bit data
 *************************************************************************/
u8 u8GetBitData(eRxDataType dType)
{
	// Get the data bit mask
	u8 bitMask = sPGNdataRx[dType].bitMask;

	// Get the data
	u8 data = J1939RxData[sPGNdataRx[dType].PGNindex].u8data[sPGNdataRx[dType].byteIndex];

	// Keep only the relevant bits of the data
	data &= ~(u8)bitMask;

	// Shift them down to the least significant bit location
	while ((bitMask & 1U) == 1U)
	{
		bitMask >>= 1;
		data >>= 1;
	}

	return data;
}


/*********************************************************************//**
 *
 * Copy PGN 65110 data from rx receiving buffer to its actual buffer
 *
 * @param      Rx Pgn type
 *
 * @return     None
 *************************************************************************/
static void _vPGN65110_DEF(sJ1939_RX_MSG_TYPE *msg)
{
	s16 i;	  

	for (i=0U; i < 8U; ++i)
	{
		J1939RxData[eRx65110].u8data[i] = msg->u8data[i];
	}

	J1939RxData[eRx65110].u32TimeStamp = msg->u32TimeStamp;
	J1939RxData[eRx65110].sMsgID.Identifier = msg->sMsgID.Identifier;	  
	J1939RxData[eRx65110].bDataRdy = msg->bDataRdy;
	//Set SPN Data ready flag 
   	bSpnDataReadyStatus[eJLowDEFLamp] = TRUE;
	bSpnDataReadyStatus[eJLowDEFGauge] = TRUE;
	  
}
/*********************************************************************//**
 *
 * Copy PGN 59904 data from rx receiving buffer to its actual buffer
 *
 * @param      Rx Pgn type
 *
 * @return     None
 *************************************************************************/
static void _vPGN59904_SoftVersionRead(sJ1939_RX_MSG_TYPE *msg)
{
	s16 i;	  
	
	for (i=0U; i < 8U; ++i)
	{
		J1939RxData[eRx59904].u8data[i] = msg->u8data[i];
	}
	
	J1939RxData[eRx59904].u32TimeStamp = msg->u32TimeStamp;
	J1939RxData[eRx59904].sMsgID.Identifier = msg->sMsgID.Identifier;	  
	J1939RxData[eRx59904].bDataRdy = msg->bDataRdy;
	//Set SPN Data ready flag 
   	bSpnDataReadyStatus[eJRequestPGNSoftVersion] = TRUE;
	
	vSoftwareVersionRead();
	  
}

void vResetDTCParameters(void)
{
	u8DTCSA = J1939_NULL_ADDRESS;
	u8DTCready = FALSE;
	u8DTCCount = 0U;
}

u8 u8GetDTCStatus(void)
{
	return u8DTCready;
}

u8 u8GetDTCCount(void)
{
	return u8DTCCount;
}

u8 u8GetDTCSA(void)
{
	return u8DTCSA;
}

u8 u8GetDtcFramNum()
{
	return u8DtcFramNum;
}

u16 u16GetDtcTotalBytes()
{
	return u16lTotalBytes;
}

static void _vResetDM1Reception(u8 SourceAdd)
{
	//reset the DTC parameters at the end of multipacket reception
	u8MultiPacketDM1[SourceAdd] = eNoNewMultiPacket; 
	u8MultiFrameSA[SourceAdd] = J1939_NULL_ADDRESS;	
	
}



/*****************************************************************************
 **  Description:
 **      Get data ready Of each PGN.
 **
 ** ---------------------------------------------------------------------------
 **  Parameters:     data type,
 **                  
 **                  
 **  Return value:   True if msg Received ,False if not
 **
 *****************************************************************************/
bool bGetPGNDataReady(eRxDataType dtype)
{
	
	return J1939RxData[sPGNdataRx[dtype].PGNindex].bDataRdy;
}

/*****************************************************************************
 **  Description:
 **     Clear data ready Of each PGN.
 **
 ** ---------------------------------------------------------------------------
 **  Parameters:     data type,
 **                  
 **                  
 **  Return value:   Non
 **
 *****************************************************************************/
void ClearPGNDataReady(eRxDataType dtype)
{
	J1939RxData[sPGNdataRx[dtype].PGNindex].bDataRdy = FALSE;
	
}


/*****************************************************************************
 **  Description:
 **      Get SPN data ready status
 **
 ** ---------------------------------------------------------------------------
 **  Parameters:     data type,
 **                  
 **                  
 **  Return value:   True if msg Received ,False if not
 **
 *****************************************************************************/
bool bGetSPNDataReady(eRxDataType dtype)
{
	
	return bSpnDataReadyStatus[dtype];
}

/*****************************************************************************
 **  Description:
 **     Clear data ready status of SPN
 **
 ** ---------------------------------------------------------------------------
 **  Parameters:     data type,
 **                  
 **                  
 **  Return value:   Non
 **
 *****************************************************************************/
void vClearSPNDataReady(eRxDataType dtype)
{
	bSpnDataReadyStatus[dtype] = FALSE;
	
}





/*****************************************************************************
 **  Description:
 **      Check the timeout Of each PGN.
 **
 ** ---------------------------------------------------------------------------
 **  Parameters:     data type,
 **                  
 **                  
 **  Return value:   True if timeout ,False if not
 **
 *****************************************************************************/
bool bGetPGNTimeOut(eRxDataType dtype)
{
	if (dtype < eNumofRxDataTypes)
	{
		if((u32GetClockTicks() - J1939RxData[sPGNdataRx[dtype].PGNindex].u32TimeStamp) >= sPGNdataRx[dtype].u32TimeStamp)
		{
			J1939RxData[sPGNdataRx[dtype].PGNindex].bDataOutDate =TRUE;
		}
		else
		{
			J1939RxData[sPGNdataRx[dtype].PGNindex].bDataOutDate =FALSE;
		}
	}	
	return J1939RxData[sPGNdataRx[dtype].PGNindex].bDataOutDate;
}


/*********************************************************************//**
 *
 * This method is called from the CAN receive interrupt.
 * It will find an available queue slot and save the message for later
 * processing. If there is no room in the queue, the message will be
 * dropped.
 *
 * @param      None
 *
 * @return     None
 *************************************************************************/
void vJ1939OnFullRx(void)
{
	u32	MsgBuf_address;
	u8	rx_msg_DLC;	
	u32 rx_msg_ID;
	u8  rx_buffer_number;
	u8	rx_data_cnt;
	
//	sJ1939_Id_Struct_Type ID;
	s16 RxQuePush;

	
	//  Find an available queue slot
	for (RxQuePush = 0U;
	(RxQuePush < RX_QUE_SIZE) && (sJ1939RxQueue[RxQuePush].bDataRdy == TRUE);
	++RxQuePush)
	{
		// empty statement for this for loop
	}

	if( RxQuePush < RX_QUE_SIZE )
	{
		//  Ignore message if no room in the queue
		if (sJ1939RxQueue[RxQuePush].bDataRdy == FALSE)
		{
		
			C0INTS = 0x0002;	
			rx_buffer_number = C0LIPT;						// Get receive message buffer number
			MsgBuf_address = (MSGBUF_BASE_ADD + (0x10 * rx_buffer_number));		// Set CAN message buffer[n] register address
			rx_data_cnt = 0U;
			

			// Time stamp this message regardless of buffer availability
			sJ1939RxQueue[RxQuePush].u32TimeStamp = u32GetClockTicks();
			
			// Get the ID
			*((u32 *)(MsgBuf_address + 0x0e)) = 0x0004;		// Clear DN bit
			rx_msg_DLC = *((u8 *)(MsgBuf_address + 0x08));	// Get receive message data length
			rx_msg_ID = (*((uint32_t *)(MsgBuf_address + 0x0c))<< 16 )  | (*((uint32_t *)(MsgBuf_address + 0x0A)));
			rx_msg_ID &= ~(0x80000000U);                                    // clear the IDE bit
			

			// Save the ID
			sJ1939RxQueue[RxQuePush].sMsgID.Identifier = rx_msg_ID;
			
				// Get the byte count
				sJ1939RxQueue[RxQuePush].ucNumofRxBytes = rx_msg_DLC;

				// Deposit the data
				for(rx_data_cnt = 0 ;((rx_data_cnt < rx_msg_DLC) && (rx_data_cnt < 8)) ; rx_data_cnt++)
				{
					sJ1939RxQueue[RxQuePush].u8data[rx_data_cnt] = *((u8 *)(MsgBuf_address + (0x01 * rx_data_cnt)));	
				}
				// Set data valid
				sJ1939RxQueue[RxQuePush].bDataRdy = TRUE;
			
		}
	}
	else
	{
	   //if message receiving buffer not available empty room then clear CAN interrupt 
		C0INTS = 0x0002;
		rx_buffer_number = C0LIPT;
		// Set CAN message buffer[n] register address
		MsgBuf_address = (MSGBUF_BASE_ADD + (0x10 * rx_buffer_number));		
		*((u32 *)(MsgBuf_address + 0x0e)) = 0x0004;		// Clear DN bit
	}

}


void vClearDTC(void)
{
	u8 i = 0;
	vResetDTCParameters();

}

