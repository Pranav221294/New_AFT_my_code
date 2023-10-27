#ifndef _CAN_H_
#define _CAN_H_

#include "customTypedef.h"


// constant declarations -----------------------------------------------------

#define CRxD_Port_PM	PM0.1
#define CRxD_Port		P0.1
#define CTxD_Port_PM	PM0.0
#define CTxD_Port		P0.0

//Recieving or Transmmiting mesage buffer starting address
#define	MSGBUF_BASE_ADD	((u32)(0x0F0600)) 

//15 buffers used for msg receiving and 1 buffer is used for msg transmmiting 
#define BUFFER_NUMBER_0		0U
#define BUFFER_NUMBER_1		1U
#define BUFFER_NUMBER_2		2U
#define BUFFER_NUMBER_3		3U
#define BUFFER_NUMBER_4		4U
#define BUFFER_NUMBER_5		5U
#define BUFFER_NUMBER_6		6U
#define BUFFER_NUMBER_7		7U
#define BUFFER_NUMBER_8		8U
#define BUFFER_NUMBER_9		9U
#define BUFFER_NUMBER_10	10U
#define BUFFER_NUMBER_11	11U
#define BUFFER_NUMBER_12	12U
#define BUFFER_NUMBER_13	13U
#define BUFFER_NUMBER_14	14U
#define BUFFER_NUMBER_15	15U
#define MSG_LEN 			8U
#define MSG_LEN_3 			3U

						

//Below 10 filters assige to individual buffers and according to filters buffer received only single ID.
#define FILTER_ID_1 	0x1CFF17E1 

#define FILTER_ID_2 	0x1BDA17F1	  
#define FILTER_ID_3 	0x1BDA17F1
#define FILTER_ID_4 	0x1BDA17F1

#define FILTER_ID_5 	0x1BDA17F9
#define FILTER_ID_6 	0x1BDA17F9
#define FILTER_ID_7 	0x1BDA17F9
#define FILTER_ID_8 	0x1BDA17F9

#define FILTER_ID_9 	0x18DB33F1
#define FILTER_ID_10 	0x18DB33F1
#define FILTER_ID_11 	0x18DB33F1

#define FILTER_ID_12 	0x18DB33F9
#define FILTER_ID_13 	0x18DB33F9  
#define FILTER_ID_14 	0x18DB33F9

/*
#define MASK_1			0x1400073BU
#define MASK_2			0x0001BF33U
#define MASK_3			0x0001FF33U
#define MASK_4			0x00070003U
*/


/*
// for testing if want to remove filters and masks

#define MASK_1			0xFFFFFFFF	 
#define MASK_2			0xFFFFFFFF	 
#define MASK_3			0xFFFFFFFF	 
#define MASK_4			0xFFFFFFFF


#define FILTER_ID_1 	0xFFFFFFFF	 
#define FILTER_ID_2 	0xFFFFFFFF	  
#define FILTER_ID_3 	0xFFFFFFFF	
#define FILTER_ID_4 	0xFFFFFFFF	 
#define FILTER_ID_5 	0xFFFFFFFF	  
#define FILTER_ID_6 	0xFFFFFFFF	
#define FILTER_ID_7 	0xFFFFFFFF	 
#define FILTER_ID_8 	0xFFFFFFFF	  
#define FILTER_ID_9 	0xFFFFFFFF	
#define FILTER_ID_10 	0xFFFFFFFF	


*/




// Global declarations --------------------------------------------------------



// Global prototypes ---------------------------------------------------------

/// @addtogroup CAN_LIBRARY
/// @{
 
/// @defgroup CanHeaderLib Global  functions used in CAN file
//// 
///   
/// @addtogroup CanHeaderLib
/// @{


void vTxMsgBufProcessing(u32 ID_msg_rx ,u8 tx_msg_DLC,u8* tx_msg_data);
void vCanInit(void);
void vReCANInit(void);
void vRecoveryFromBusOffState(void);
void vClearCanBusOffFlag(void);
bool bGetCanBusOffFlagStatus(void);


/// @} // endgroup CanHeaderLib

/// @} // endgroup CAN_LIBRARY

#endif


