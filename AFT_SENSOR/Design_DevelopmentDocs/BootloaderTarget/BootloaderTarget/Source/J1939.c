
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
#include "P15765H.h"



// constant declarations -----------------------------------------------------


#define PGN65303				65303			// TTWA - Proprietery 1CFF1721
#define PGN65378				65378			// VECU - Proprietery 18FF6221

//Source Address macro

#define EMS_SA1_EXT				0x00
#define BCM_SA_EXT				0x21
#define ABS_SA_EXT				0x0B
#define TCU_SA_EXT				0x03
#define RETARDER_ECU_SA_EXT		0x10


#define RTS_TIMEOUT				1500
#define BAM_TIMEOUT				750

//ABS malfunction trailer
//Exhaust break PGN TBD	
 
#define BITDATA                 0xFF    // indicates bit data
#define SERVICE_INTERVAL        10      // service time interval in msec
#define SEND_ON_DEMAND          0

typedef enum
{
	ePGN_J1939 = 0,
	ePGN_NMEA,
	ePGN_FAST_PACKET
} PGN_TYPE;



// local declarations --------------------------------------------------------


typedef enum
{
	eEmsSA = 0,
	eRetarderEcuSA,
	eAbsSA,
	eTcuSA,
	NumberOfSA
} eSourceAddType;

unsigned char tx_msg_data[8];
unsigned char Rx_Databuf[8];
unsigned char Aes_Databuf[16];

extern unsigned char MY_RxByteBuffer[];
unsigned char f,k;
unsigned char	rx_data_cnt;
unsigned char	aescount;



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

#define MAX_OVERRANGE 		20    // maximum percent overrange

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
	{0,				0,						0,				0},
};
// The bit resolution in this case is the reciprocal of the
const PGNdataType sPGNdataTx[eNumofTxDataTypes] =                                     // J1939 bit resolution multiplied by 100 for integer math
{  
//  PGNIndex    	byteIndex   byteSize    bitMask     dataOffset  bitRes  overrangeData  		
                                                             
	{0,				0,				0,			0,			0,			0,		0},
};


typedef struct
{
	u16		PGN;
	u8      u8SourceAdd;
	void	(*RxFunct)(sJ1939_RX_MSG_TYPE *);    // pointer to PGN processing method
	
} tPGNsonstRxType;


const PGNdataType sPGNdataRx[eNumofRxDataTypes] =
{
	//  PGNindex    byteIndex  byteSize   		bitMask     dataOffset  	bitRes      overrangeData underrange   In ms            
	{   eRx65303_0xE1, 	0,          4,          0x00,       0,          100000,     BADDATA_4BYTE,		0,			5000	},   //      IIL Proprietery for fault Diganostic					
};

sJ1939_RX_MSG_TYPE   sJ1939RxQueue[RX_QUE_SIZE];
sJ1939_RX_MSG_TYPE   J1939RxData[eNoOfPgnRx]; 
J1939_TX_TYPE        J1939_Tx[eNoOfPgnTx];
// Create an array corresponding to sPGNconstantsTx above for the PGN data
J1939_TX_DATA_TYPE  J1939TxData[eNoOfPgnTx];  

// Private prototypes --------------------------------------------------------

/// @addtogroup J1939_LIBRARY
/// @{
 
/// @defgroup J1939Lib Private functions used in J1939 File
//// 
///   
/// @addtogroup J1939Lib
/// @{

static void _vPGN65303_IILPropData(sJ1939_RX_MSG_TYPE *);
static void _vResetDM1Reception(u8);
static void _J1939_RxManager(void);
static void _J1939_TxManager(void);


/// @} // endgroup J1939Lib

/// @} // endgroup J1939_LIBRARY


// prototype dependent definitions ------------------------------------------ 
// This needs to be defined after prototype definitions because they are used in this structure
const tPGNsonstRxType sPGNconstantsRx[eNoOfPgnRx] =
{
	
	{   PGN65303,		0xE1,          _vPGN65303_IILPropData },
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
	static u32 serviceTime = 0;
// commented _J1939_TxManager and service time interval to execute fast receiption in bootloader 
	//if ((u32GetClockTicks() - serviceTime) >= SERVICE_INTERVAL)
	{
		// Service the J1939 received messages
		_J1939_RxManager();

		// Service the J1939 transmit messages		
		//_J1939_TxManager();

		// Reset the timer
		//serviceTime = u32GetClockTicks();
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
	for (i = 0; i < RX_QUE_SIZE; ++i)
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
		for (j = 0; j < 8; ++j)
		J1939TxData[i].u8data[j] = 0xFF;
		
	}
	
	// Initialize the Rx data structure
	for (i = 0; i < eNoOfPgnRx; ++i)
	{
		J1939RxData[i].u32TimeStamp = u32GetClockTicks();
		J1939RxData[i].bDataOutDate = TRUE;
		J1939RxData[i].bDataRdy = FALSE;
		// Set all data invalid
		for (j = 0; j < 8; ++j)
		J1939RxData[i].u8data[j] = 0xFF;
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
	for (i = 0; i < eNoOfPgnTx; ++i)
	{
		J1939TxData[i].bEnabled = TRUE;
		J1939TxData[i].bSendOnDemand = FALSE;
		J1939TxData[i].u32TimeStamp = u32GetClockTicks();
		// Set all data invalid
		for (j = 0; j < 8; ++j)
		J1939TxData[i].u8data[j] = 0xFF;
		
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
	u8  framNum, frameIndex = 0;	
	u8 SourceAddType = 0;
	
	// Check the entire input queue for fresh messaages
	for (RxQuePop = 0; RxQuePop < RX_QUE_SIZE; ++RxQuePop)
	{
		if (sJ1939RxQueue[RxQuePop].bDataRdy)
		{
			// Process fresh message
			msg = &sJ1939RxQueue[RxQuePop];
			UDSMsg = sJ1939RxQueue[RxQuePop];

			// Extract the PGN
			ID = msg->sMsgID.Identifier;
			
			PGN = (msg->sMsgID.Identifier >> 8) & 0x0003FFFF;
			SOURCEADD = (msg->sMsgID.Identifier) & 0x000000FF;
//****************Dignostic message *********************************

	//both diagnostics and pass through messages
	   if((UDSMsg.sMsgID.sIdField.SourceAddress == REQ_PHYSICAL_SA1 || 
		   UDSMsg.sMsgID.sIdField.SourceAddress == REQ_PHYSICAL_SA2)
		   && 
		   (UDSMsg.sMsgID.sIdField.PduSpecific == PREFERRED_SA || //0x1BDA17F1/F9
		   UDSMsg.sMsgID.sIdField.PduSpecific == REQ_FUNCTIONAL_SA) //0x1BDB33F1/F9
		)
	   {
		   // Find an available queue slot
		   for (RxQuePush = 0;
		   (RxQuePush < RX_QUE_SIZE) && (diagnosticsBuf[RxQuePush].bDataRdy == TRUE);
		   ++RxQuePush);
		   if( RxQuePush < RX_QUE_SIZE )
		   {
			   //UDSbufPtr = &diagnosticsBuf[RxQuePush];

			   diagnosticsBuf[RxQuePush].sMsgID.Identifier = ((sJ1939RxQueue[RxQuePop].sMsgID.Identifier)  & 0xffff0000);
				for(i= 0;i < 8; i++)
			    diagnosticsBuf[RxQuePush].u8data[i] = sJ1939RxQueue[RxQuePop].u8data[i];
				diagnosticsBuf[RxQuePush].u32TimeStamp = sJ1939RxQueue[RxQuePop].u32TimeStamp;
				diagnosticsBuf[RxQuePush].ucNumofRxBytes = sJ1939RxQueue[RxQuePop].ucNumofRxBytes;
			   diagnosticsBuf[RxQuePush].bDataRdy = TRUE;

			   
			   diagnosticsTimer = u32GetClockTicks();
			   testerAddress = UDSMsg.sMsgID.sIdField.SourceAddress;
		   }
	   }
	  

			//done only for diagnostics messages
			//	UDSbufPtr->sMsgID.Identifier = ID & 0xffff0000;
		        
    //*****************************************************************

			if(SOURCEADD == EMS_SA1_EXT)
				SourceAddType = eEmsSA;
			
//*****************************************************************


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
							while ((u16GetClockTicks() - timeNow) < 250)
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
			// Check all other PGN inputs
			for(i=0; i < eNoOfPgnRx; ++i)
			{
				if(PGN == sPGNconstantsRx[i].PGN && SOURCEADD == sPGNconstantsRx[i].u8SourceAdd )
				{	
					sPGNconstantsRx[i].RxFunct(msg);
				}
				
			}
			// Clear the queue slot
			sJ1939RxQueue[RxQuePop].bDataRdy = FALSE;
		}
	}
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
						vTxMsgBufProcessing(sPGNconstantsTx[n].ID,MSG_LEN_3,J1939TxData[n].u8data);						
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
		// Set flag to force immediate output
		J1939TxData[n].bSendOnDemand = TRUE;

	return foundPGN;
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
	u32 data = 0;	
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
			while ((b.bitMask & 1) == 1)
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
			if(b.byteSize==1)
				data = (u8)J1939RxData[sPGNdataRx[dtype].PGNindex].u8data[sPGNdataRx[dtype].byteIndex];
			else
			{
				for( i = 0; i< b.byteSize; i++)
				{
					data |= (u32)J1939RxData[sPGNdataRx[dtype].PGNindex].u8data[sPGNdataRx[dtype].byteIndex + i] << (i*8);
				}

			}
			if(data < sPGNdataRx[dtype].overrangeData ) 
			{
			   if(data <= sPGNdataRx[dtype].underrangeData)
			   		data = sPGNdataRx[dtype].underrangeData;
				//data = ( (data * sPGNdataRx[dtype].bitRes) / 100000 ) + (u32) sPGNdataRx[dtype].dataOffset;
				
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
	while ((bitMask & 1) == 1)
	{
		bitMask >>= 1;
		data >>= 1;
	}

	return data;
}


static void _vPGN65303_IILPropData(sJ1939_RX_MSG_TYPE *msg)
{
	s16 i;	 
	
	// Save the latest received data
	for (i=0; i < 8; ++i)
	 J1939RxData[eRx65303_0xE1].u8data[i] = msg->u8data[i];
	 
	J1939RxData[eRx65303_0xE1].u32TimeStamp = msg->u32TimeStamp;
	J1939RxData[eRx65303_0xE1].sMsgID.Identifier = msg->sMsgID.Identifier;   
	J1939RxData[eRx65303_0xE1].bDataRdy = msg->bDataRdy;

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


