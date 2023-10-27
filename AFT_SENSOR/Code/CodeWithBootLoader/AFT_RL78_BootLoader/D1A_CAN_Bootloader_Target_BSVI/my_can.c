
//----------------------------------------------------------------------------------------------------------------
//#pragma	interrupt INTC0REC MY_Rx_MsgBuf_Processing
//#pragma	interrupt INTC0ERR MY_ISR_CAN_Error
//#pragma interrupt INTC0WUP MY_ISR_CAN_Wakeup
//----------------------------------------------------------------------------------------------------------------
#include "my_macrodriver.h"
#include "my_can.h"

//----------------------------------------------------------------------------------------------------------------
unsigned char tx_msg_data[8];
unsigned char Rx_Databuf[8];
unsigned char Aes_Databuf[16];
unsigned char update[8] = " UPDATE "; 
unsigned char my_STOP_arr[8]="STOP MAN";
__boolean OK_flag;
__boolean buf0_flag;
__boolean buf1_flag;
__boolean buf2_flag;
__boolean buf3_flag;
__boolean buf4_flag;
__boolean buf5_flag;
__boolean buf6_flag;
__boolean buf7_flag;
extern __boolean DONE_flag;
extern __boolean boot_flag;
extern __boolean bCANError;
extern __boolean Error_flag;
static unsigned char  bBusOffFlag = 0;
__boolean baes1;
__boolean baes2;
extern unsigned char MY_RxByteBuffer[];
unsigned char f,k;
unsigned char	rx_data_cnt;
unsigned char	aescount;
unsigned char my_boot[] = "Start boot";
extern void MY_FLash_Erase(void);
extern unsigned long TransmissionTimeStamp;
extern volatile unsigned long gu32Clock1msTicker ;
extern unsigned long Address;
//----------------------------------------------------------------------------------------------------------------
void MY_CAN_Init()
{
	SCAN0 = 1;			// 1-CAN pins on P00(TX) & P01(RX)
						// 0-CAN pins on P71(TX) & P70(RX)
	CRxD_Port_PM =	1;		//Make Rx pin input  	
	CRxD_Port =	1;					
	CTxD_Port_PM =  0;              //Make Tx pin Output 
	PCKSEL |= 0x10;   
	
	/*CAN0 global module clock select register by default valure is 0f 
	set prescaler to 0
	*/
	C0GMCS  = 0x00;         	 

	/*
	CAN Global Module Control Register
	GOM  =1;
	*/
	C0GMCTRL= 0x0100; 

	
	//C0BRP	= 0x03;	
	//C0BTR	= 0x0104;  
	C0GMCS	= 0x01;	   
	C0BRP = 0x07;		
	C0BTR	= 0x0104;	
			
	C0IE = 0x3f00;	
	C0RECIF = 0;			// clear receive interrupt flag
	C0RECMK = 0;			// Enable Receive interrupt
	C0WUPIF = 0;			// clear wakeup interrupt flag
	C0WUPMK = 0;			// enable wakeup interrupt
	C0ERRIF = 0;			// clear error interrupt flag
	C0ERRMK = 0;			// enable error interrupt

	C0CTRL   = 0x817E;   		// clear AL,VALID(no receive valid message),PSMODE1,PSMODE0(no power save mode)
				 	// clear OPMODE2,OPMODE1 and set OPMODE0(normal mode) 
				 	// set CCERC(clear C0ERC and C0INFO registers in initialization mode) 

C0MASK1L = (unsigned int)MASK_1;
	C0MASK1H = (unsigned int)(MASK_1 >> 16);

	C0MASK2L = (unsigned int)MASK_2;
	C0MASK2H = (unsigned int)(MASK_2 >> 16);

	C0MASK3L = (unsigned int)MASK_3;
	C0MASK3H = (unsigned int)(MASK_3 >> 16);
}

//----------------------------------------------------------------------------------------------------------------
void MY_CAN_MsgBuf_Init()
{
	unsigned char	buffer_number;
	unsigned long	MsgBuf_address;

	for (buffer_number = 0 ; buffer_number < 16 ; buffer_number++)		// Init all message buffer
	 {
		
		MsgBuf_address = (MSGBUF_BASE_ADD + (0x10 * buffer_number));	// Set CAN message buffer[n] register address
		
		*((unsigned short *)(MsgBuf_address + 0x0e)) = 0x0006;		// Clear TRQ, DN bit
				
		*((unsigned short *)(MsgBuf_address + 0x0e)) = 0x0001;		// Clear RDY bit
		
		*((unsigned char *)(MsgBuf_address + 0x09)) &= 0xf8;		// Clear MA0 bit
	 }
}
//----------------------------------------------------------------------------------------------------------------
void MY_Tx_MsgBuf_Init(unsigned char buffer_number,unsigned int tx_msg_ID,unsigned char tx_msg_DLC)
{
	unsigned long 	MsgBuf_address;
	unsigned char data_cnt;
	
	MsgBuf_address = (MSGBUF_BASE_ADD + (0x10 * buffer_number));		// Set CAN message buffer[n] register address

	*((unsigned char *)(MsgBuf_address + 0x09)) = 0x01;      		// Set C0MCONFm register, Transmit message buffer, MA0=1,msg buffer used

    *(( unsigned int *)(MsgBuf_address + 0x0a)) = (unsigned int )tx_msg_ID;   			//Extended frame,C0MIDLm = ID0 to ID15;
    *(( unsigned int  *)(MsgBuf_address + 0x0c)) = (unsigned int )(tx_msg_ID >> 16) ; 	//C0MIDHm = ID16 to ID28;
    *(( unsigned int *)(MsgBuf_address + 0x0c)) |= (unsigned int )(1<<15); 	//C0MIDHm --> set IDE -->
                                                                                //IDE = 1 --> Extended format mode (ID28 to ID0: 29 bits)



	*(( unsigned char *)(MsgBuf_address + 0x08)) = tx_msg_DLC;     		// set C0MDLCm -data length

	for(data_cnt = 0 ; data_cnt < tx_msg_DLC ; data_cnt++)			
	{
		*(( unsigned char *)(MsgBuf_address + (0x01 * data_cnt))) = 0x00;    // clear each u8 data=0x00
	}
	

	#if 1
	*(( unsigned int  *)(MsgBuf_address + 0x0e)) = 0x001e;  // C0MCTRLm
								// clear MOW,IE,DN,TRQ bit
								// MOV=0,The message buffer is not overwritten by a newly received data frame.
								// IE=0,Normal message transmission completion interrupt disabled
								// DN=0,A data frame or remote frame is not stored in the message buffer.
								// TRQ=0,No message frame transmitting request that is pending or being transmitted
	#endif

	#if 0							
	*(( unsigned int  *)(MsgBuf_address + 0x0e)) = 0x0816;  // clear MOW,DN,TRQ bit
								// set IE=1,Normal message transmission completion interrupt enabled
	#endif
	
	*(( unsigned int  *)(MsgBuf_address + 0x0e)) = 0x0100; 	// // Set RDY bit,The CAN module can write to the message buffer

}
//----------------------------------------------------------------------------------------------------------------
void MY_Rx_MsgBuf_Init(unsigned char buffer_number,unsigned int rx_mask_ID)
{
		 unsigned long MsgBuf_address;
	
    MsgBuf_address = (MSGBUF_BASE_ADD + (0x10 * buffer_number));		// Set CAN message buffer[n] register address

	// Set C0MCONFm register 
    *(( unsigned char *)(MsgBuf_address + 0x09)) = 0x09;   // Receive message buffer(no mask), MA0=1,msg buffer used
	
    if((buffer_number==0)||(buffer_number==1)||(buffer_number==2)||(buffer_number==3)||(buffer_number==4)||(buffer_number==5))
    {
        *(( unsigned char *)(MsgBuf_address + 0x09)) = 0x11;    	// Receive message buf
    }
    else if ((buffer_number==6)||(buffer_number==7)||(buffer_number==8)||(buffer_number==9)||(buffer_number==10))
    {
        *(( unsigned char *)(MsgBuf_address + 0x09)) = 0x19;   // Receive message buffer(mask 2), MA0=1,msg buffer used
    }
    else if ((buffer_number==11)||(buffer_number==12)||(buffer_number==13)||(buffer_number==14))
    {
        *(( unsigned char *)(MsgBuf_address + 0x09)) = 0x21;   // Receive message buffer(mask 3), MA0=1,msg buffer used
    }
    else
    {
          // do nothing
    }

    *(( unsigned int *)(MsgBuf_address + 0x0a)) = (unsigned int)rx_mask_ID;   			//Extended frame,C0MIDLm = ID0 to ID15;
    
    *(( unsigned int *)(MsgBuf_address + 0x0c)) = (unsigned int)(rx_mask_ID >> 16) ; 	//C0MIDHm = ID16 to ID28;
    
    *(( unsigned int *)(MsgBuf_address + 0x0c)) |= (unsigned int)(1<<15); 	//C0MIDHm --> set IDE -->
                                                                                //IDE = 1 --> Extended format mode (ID28 to ID0: 29 bits)

	
	*(( unsigned int *)(MsgBuf_address + 0x0e)) = 0x0916;  // Set C0MCTRLm register 
								// clear MOW,DN,TRQ bit
								// MOV=0,The message buffer is not overwritten by a newly received data frame.
								// set IE=1,Valid message reception completion interrupt enabled.
								// DN=0,A data frame or remote frame is not stored in the message buffer.
								// TRQ=0,No message frame transmitting request that is pending or being transmitted
	
	*(( unsigned int *)(MsgBuf_address + 0x0e)) |= 0x0100;  // Set RDY bit,The CAN module can write to the message buffer
	
}


//----------------------------------------------------------------------------------------------------------------
void MY_Tx_MsgBuf_Processing(unsigned long id,unsigned char buffer_number,unsigned char tx_msg_DLC,unsigned char* tx_msg_data)
{
	unsigned long	MsgBuf_address;
	unsigned short  C0MCTRLm;
	
	TransmissionTimeStamp = gu32Clock1msTicker;
	
	MsgBuf_address = (MSGBUF_BASE_ADD + (0x10 * buffer_number));	// Set CAN message buffer[n] register address
	MY_Tx_MsgBuf_Init(buffer_number,id,tx_msg_DLC);
	C0MCTRLm = *((unsigned char *)(MsgBuf_address + 0x0e));		// Check TRQ bit
	if((C0MCTRLm & 0x0002) != 0)
	{
		return;
	}

	
	*((unsigned short *)(MsgBuf_address + 0x0e)) = 0x0001;    	// Clear RDY bit ,The message buffer can be written by software.
	
	C0MCTRLm = *((unsigned char *)(MsgBuf_address + 0x0e));

	
	if((C0MCTRLm & 0x0001) == 0)					// Set C0MDATAxm register
	{
		unsigned char data_cnt;
		
		for(data_cnt = 0 ; data_cnt < tx_msg_DLC ; data_cnt++)
		{
			*((unsigned char *)(MsgBuf_address + (0x01 * data_cnt))) = tx_msg_data[data_cnt];    //clear each byte data=0x00
		}
		
		*((unsigned short *)(MsgBuf_address + 0x0e)) = 0x0100;	// Set RDY bit
		
		*((unsigned short *)(MsgBuf_address + 0x0e)) = 0x0200;	// Set TRQ bit
	}
	
	//C0MCTRLm = *((unsigned char *)(MsgBuf_address + 0x0e));
	while((C0MCTRLm & 0x0002) == 0x0002)
	{
		NOP();
		NOP();
	}
	return;
}

//----------------------------------------------------------------------------------------------------------------
//__interrupt void MY_Rx_MsgBuf_Processing()
void MY_Rx_MsgBuf_Processing(void)
{
	unsigned long	MsgBuf_address;
	unsigned char	rx_msg_DLC;
	
	unsigned short 	rx_msg_ID;
	unsigned char rx_buffer_number;
	
	C0INTS = 0x0002;
	
	rx_buffer_number = C0LIPT;						// Get receive message buffer number
	
	MsgBuf_address = (MSGBUF_BASE_ADD + (0x10 * rx_buffer_number));		// Set CAN message buffer[n] register address
	
	while(((*((unsigned short *)(MsgBuf_address + 0x0e))) & 0x2004) != 0) 	//check DN and MUC bit
	{
		/*
		*((unsigned short *)(MsgBuf_address + 0x0e)) = 0x0004;		// Clear DN bit
		
		rx_msg_DLC = *((unsigned char *)(MsgBuf_address + 0x08));	// Get receive message data length

		rx_msg_ID = *((unsigned short *)(MsgBuf_address + 0x0C));	// Get receive message ID
		
		//rx_msg_ID = ((*((unsigned short *)(MsgBuf_address + 0x0C)))>>2) & (0x07ff);     //standard frame,just need C0MIDHm,C0MIDLm=0x0000
		*/
		// Clear DN bit
		*((unsigned long *)(MsgBuf_address + 0x0e)) = 0x0004;		
		// Get receive message data length
		rx_msg_DLC = *((unsigned char *)(MsgBuf_address + 0x08));	
		// Get the ID
		rx_msg_ID = (*((unsigned long *)(MsgBuf_address + 0x0c))<< 16 )  | (*((unsigned long *)(MsgBuf_address + 0x0A)));
		// clear the IDE bit
		rx_msg_ID &= ~(0x80000000);                                  
		//ID.Identifier = rx_msg_ID;
		//rx_msg_ID = ((*((unsigned short *)(MsgBuf_address + 0x0C)))>>2) & (0x07ff);     //standard frame,just need C0MIDHm,C0MIDLm=0x0000
		
		switch(rx_msg_ID)
		{
			case (0x0000):    	//ID=0x0000
				
				for(f=0; f<0x08 ; f++)
				{
					MY_RxByteBuffer[rx_data_cnt++] = *((unsigned char *)(MsgBuf_address + (0x01 * f)));	// Get receive data
				}
				
				buf0_flag = 1;
				break;
			case (0x0001):	 	//ID=0x0001
				for(f=0; f<0x08 ; f++)
				{
					MY_RxByteBuffer[rx_data_cnt++] = *((unsigned char *)(MsgBuf_address + (0x01 * f)));	// Get receive data
				}
				buf1_flag = 1;
				break;
			case (0x0002):		//ID=0x0002
				for(f=0; f<0x08 ; f++)
				{
					MY_RxByteBuffer[rx_data_cnt++] = *((unsigned char *)(MsgBuf_address + (0x01 * f)));	// Get receive data
				}
				buf2_flag = 1;
				break;
			case (0x0003):    	//ID=0x0003
				for(f=0; f<0x08 ; f++)
				{
					MY_RxByteBuffer[rx_data_cnt++] = *((unsigned char *)(MsgBuf_address + (0x01 * f)));	// Get receive data
				}
				buf3_flag = 1;
				break;
			case (0x004):	 	//ID=0x0004
				for(f=0; f<0x08 ; f++)
				{
					MY_RxByteBuffer[rx_data_cnt++] = *((unsigned char *)(MsgBuf_address + (0x01 * f)));	// Get receive data
				}
				buf4_flag = 1;
				break;
			case (0x005):		//ID=0x0005
				for(f=0; f<0x08 ; f++)
				{
					MY_RxByteBuffer[rx_data_cnt++] = *((unsigned char *)(MsgBuf_address + (0x01 * f)));	// Get receive data
				}
				buf5_flag = 1;
				break;
			case (0x006):    	//ID=0x0006
				for(f=0; f<0x08 ; f++)
				{
					MY_RxByteBuffer[rx_data_cnt++] = *((unsigned char *)(MsgBuf_address + (0x01 * f)));	// Get receive data
				}
				buf6_flag = 1;
				break;
			case (0x007):	 	//ID=0x0007
				for(f=0; f<0x08 ; f++)
				{
					MY_RxByteBuffer[rx_data_cnt++] = *((unsigned char *)(MsgBuf_address + (0x01 * f)));	// Get receive data
				}
				buf7_flag = 1;
				break;
			case (0x0008):		//ID=0x0008
				for(k=0; k<0x08 ; k++)
				{
					Rx_Databuf[k] = *((unsigned char *)(MsgBuf_address + (0x01 * k)));	// Get receive data
				}
		
					if((Rx_Databuf[0] == ' ') &&(Rx_Databuf[1] == 'U')&&(Rx_Databuf[2] == 'P') &&(Rx_Databuf[3] == 'D')
			&& (Rx_Databuf[4] == 'A')&&(Rx_Databuf[5] == 'T')&&(Rx_Databuf[6] == 'E')&&(Rx_Databuf[7] == ' '))
				
			{
				DONE_flag = 0;
				boot_flag = 1;
			}
		 if((Rx_Databuf[0] == 'S') &&(Rx_Databuf[1] == 'T')&&(Rx_Databuf[2] == 'O') &&(Rx_Databuf[3] == 'P')
		 &&(Rx_Databuf[4] == ' ')&&(Rx_Databuf[5] == 'M')&&(Rx_Databuf[6] == 'A')&&(Rx_Databuf[7] == 'N'))
				
			{
				DONE_flag = 1;
				boot_flag = 0;
			}
			break;
		case (0x009):		//ID=0x0009
				//boot_flag = 1;	
			for(k=0; k<0x08 ; k++)
				{
					Aes_Databuf[aescount++] = *((unsigned char *)(MsgBuf_address + (0x01 * k)));	// Get receive data
				}
				baes1 = 1;
				break;
		case (0x0010):		//ID=0x000a
				//boot_flag = 1;	
			for(k=0; k<0x08 ; k++)
				{
					Aes_Databuf[aescount++] = *((unsigned char *)(MsgBuf_address + (0x01 * k)));	// Get receive data
				}
		
			baes2 = 1;
			
		
				break;	
			default:
				break;
		}

		
		/*#if 0
		
		for(rx_data_cnt = 0 ;((rx_data_cnt < rx_msg_DLC) && (rx_data_cnt < 8)) ; rx_data_cnt++)
		{
			Rx_Databuf[rx_data_cnt] = *((unsigned char *)(MsgBuf_address + (0x01 * rx_data_cnt)));	// Get receive data
		}
		#endif*/
	//OK_flag = 1;
			
	}
}
//----------------------------------------------------------------------------------------------------------------
void MY_CAN_SleepMode_Setting()
{
	C0CTRL = 0x0810;   			// set PSMODE0=1,PSMODE1=0, setting CAN sleep mode
	while((C0CTRL&0x0008) == 0); 		// check PSMODE0=1
}
//----------------------------------------------------------------------------------------------------------------
void MY_CAN_StopMode_Setting()
{
	MY_CAN_SleepMode_Setting();
	C0CTRL = 0x1800;   			// set PSMODE0=1,PSMODE1=1, setting CAN stop mode
	while((C0CTRL&0x0010) == 0); 		// check PSMODE1=1
}
//----------------------------------------------------------------------------------------------------------------
void MY_CAN_StopMode_Release()
{
	C0CTRL = 0x0810;   			// clear PSMODE1=0, release stop mode to sleep mode
}
//----------------------------------------------------------------------------------------------------------------
void MY_CAN_SleepMode_Release()
{
	C0CTRL = 0x0008;    			// clear PSMODE0=0,release sleep mode by software
	
	#if 0
	//Sleep mode can also be released by a falling edge at the CAN reception pin---wakeup signal
	C0INTS = 0x0020;    			// clear CINTS5(Wakeup interrupt)
	#endif
}
//----------------------------------------------------------------------------------------------------------------
//__interrupt void MY_ISR_CAN_Wakeup()
void MY_ISR_CAN_Wakeup(void)
{
	C0INTS = 0x0020;    			// clear CINTS5(Wakeup interrupt)
}
//----------------------------------------------------------------------------------------------------------------
//__interrupt void MY_ISR_CAN_Error(void)
void MY_ISR_CAN_Error(void)
{
	//C0INTS = 0x001C;
	//bCANError = 1;
	unsigned int read_regiter = 0;

	if((C0INTS & 0x0004) == 0x0004)   // // CINTS2
	{
						
		if((C0INFO & (unsigned int)(1<<4)) == 0x10)		
		{
			bBusOffFlag = 1;
		}
		
		C0INTS|=0x0004;     //(1<<2);  //0x0004
		
	}
	 if((C0INTS & 0x0008) == 0x0008);   // // CINTS3
	{
		read_regiter = C0LEC;
		C0INTS|=0x0008;             //(1<<3);  //0x0008
		
	}
	 if((C0INTS & 0x0010) == 0x0010)   // // CINTS4
	{
		C0INTS|=0x0010;            //(1<<4);        //0x0010
		
}
}

/*********************************************************************//**
*
* When the CAN module is in the bus-off state then The CAN module recovers 
*	from the bus-off state
*
* @param	None
*
*
* @return	  None
*************************************************************************/

void vRecoveryFromBusOffState(void)
{
   unsigned char temp;
   unsigned int *Ptr_C0MCTRL;

   if(!bBusOffFlag)
   		return;
   bBusOffFlag = 0;
   
   Ptr_C0MCTRL=&C0MCTRL00;
		
	for(temp=0;temp<16;temp++)
	{
	   *Ptr_C0MCTRL=  0x0002;		// Clear all TRQ, 	   
	    Ptr_C0MCTRL++;
	NOP();
	}
	C0CTRL &= 0xf8ff;    // C0CTRL = 0x0007;
	C0CTRL |= 0x0007;    // Clear OPMODE
	NOP();NOP();NOP();

	if(C0GMCTRL == 0x0000)       //SET GOM BIT
	{
		C0GMCTRL = 0x0200;   //SET EFSD BIT check it
		NOP();
		C0CTRL|=0x8000;  //SET CCERC BIT
		C0CTRL = 0x0700; //SET OPMODE
	}
	else
	{
		C0CTRL = 0x0700;  //SET OPMODE
	}

	
}


//rr __interrupt void MY_Tx_MsgBuf()
void MY_Tx_MsgBuf(void)
{
    // CINTS0 Interrupt on normal completion of transmission of message frame from message buffer m
    C0INTS = 0x0001;
   NOP();
}


//----------------------------------------------------------------------------------------------------------------
void MY_check_half_packet_rx(void)
{
	while((!buf0_flag) || (!buf1_flag) || (!buf2_flag) || (!buf3_flag) || (!buf4_flag) || (!buf5_flag) || (!buf6_flag) || (!buf7_flag))
	{
		NOP();
		if(gu32Clock1msTicker - TransmissionTimeStamp > 30000)
		{
			Address = 0x4000;
			TransmissionTimeStamp = gu32Clock1msTicker;
			DONE_flag = 1;
			DI();
			MY_FLash_Erase();
			EI();
			Error_flag = 1;
		}
		if(DONE_flag)
		{
			NOP();
			return;
			//break;
		}
		WDTE=0xAC;
	}
	buf0_flag = 0;
	buf1_flag = 0;
	buf2_flag = 0;
	buf3_flag = 0;
	buf4_flag = 0;
	buf5_flag = 0;
	buf6_flag = 0;
	buf7_flag = 0;
}