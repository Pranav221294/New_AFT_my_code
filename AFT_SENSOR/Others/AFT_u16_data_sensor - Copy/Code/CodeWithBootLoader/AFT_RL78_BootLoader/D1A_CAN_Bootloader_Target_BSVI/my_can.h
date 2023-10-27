
//----------------------------------------------------------------------------------------------------------------

#define CRxD_Port_PM 	 PM0.1  // PM7.0   //PM0.1
#define CRxD_Port	     P0.1   // P7.0    //P0.1
#define CTxD_Port_PM 	 PM0.0  // PM7.1   //PM0.0
#define CTxD_Port	     P0.0   // P7.1    //P0.0

#define CAN_S		P3.7
//#define	CAN_S_PM	PM3.7

#define	MSGBUF_BASE_ADD	((unsigned long)(0x0F0600))

#define RX_ID0	0x0000
#define RX_ID1	0x0001
#define RX_ID2 	0x0002
#define RX_ID3	0x0003
#define RX_ID4	0x0004
#define RX_ID5 	0x0005
#define RX_ID6	0x0006
#define RX_ID7	0x0007
#define RX_ID8 	0x0008
#define RX_ID9	0x0009
#define RX_ID10	0x000a
#define RX_ID11 0x0011
#define RX_ID12	0x0012
#define RX_ID13	0x0013
#define RX_ID14 0x0014
#define RX_ID15	0x0015

#define TX_ID15	0x000f
#define TX_ID14	0x000e
#define TX_ID13	0x000d
#define TX_ID12	0x000c

//#define MASK_1 			0xFFFFFFFF
//#define MASK_2 			0xFFFFFFFF
//#define MASK_3 			0xFFFFFFFF	
#define MASK_1 			0x000000FF
#define FILTER_ID_1		0x00000000

//#define MASK_1 			0xFFFFFFFF
//#define FILTER_ID_1		0xFFFFFFFF

#define MASK_2 			0x000000FF
#define MASK_3 			0x000000FF	

#define MSG_LEN 8
//----------------------------------------------------------------------------------------------------------------

void MY_CAN_MsgBuf_Init();
void MY_CAN_Init();
void MY_Tx_MsgBuf_Processing(unsigned long,unsigned char ,unsigned char  ,unsigned char* );
void MY_Rx_MsgBuf_Init(unsigned char ,unsigned int );
void MY_Tx_MsgBuf_Init(unsigned char ,unsigned int ,unsigned char );
void MY_check_half_packet_rx(void);

void MY_Rx_MsgBuf_Processing(void);
void MY_Tx_MsgBuf(void);
void MY_ISR_CAN_Wakeup(void);
void MY_ISR_CAN_Error(void);
void vRecoveryFromBusOffState(void);
void CanDisable(void);
//----------------------------------------------------------------------------------------------------------------



