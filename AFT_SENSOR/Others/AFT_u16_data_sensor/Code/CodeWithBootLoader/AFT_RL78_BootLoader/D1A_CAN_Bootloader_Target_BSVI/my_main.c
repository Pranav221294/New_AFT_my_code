
#include "my_macrodriver.h"
#include "my_cgc.h"
#include "my_port.h"
#include "my_timer.h"
#include "my_can.h"
#include "my_userdefine.h"
#include "my_switch_INTP0.h"
#include "fsl.h"
#include "MY_ram_vector.h"
//----------------------------------------------------------------------------------------------------------------


//#define	HIGH_BEAM_MODE			PM14.0

//#define HIGH_BEAM_ON            P14 = P14 | 0x01
//#define HIGH_BEAM_OFF           P14 = P14 & 0xFE

#define IS_PROG_DONE  0

__near fsl_write_t my_fsl_write_str;

unsigned char *fsl_data_buffer_u08;



fsl_u08 my_fsl_status;
fsl_descriptor_t fsl_descr;
//fsl_u08 my_fsl_status;
fsl_write_t my_fsl_write_str;
unsigned char MY_RxByteBuffer[200];
unsigned char rxstatus,iverify_blk;
unsigned long Address;
unsigned char verify_block_cnt;
unsigned int n;
//----------------------------------------------------------------------------------------------------------------
unsigned char timer_flag;
__boolean rx_flag;
__boolean tx_flag;
__boolean DONE_flag;
__boolean Error_flag;
__boolean boot_flag;
__boolean bCANError;
extern unsigned char	rx_data_cnt;
unsigned char rec_val;
unsigned int i,poweronflag=0;
unsigned char my_intp0_cmd[]="Generate INTP0 Interrupt";
unsigned char my_sw_pressed[]="Switch pressed";
unsigned char my_send_next_half[]="Send next Half Packet";
unsigned char my_send_new_req[]="Send new Packet";
volatile unsigned long gu32Clock1msTicker ;
unsigned long u32BlinkTimeStamp =0;
unsigned char Blinkflag = 0;

unsigned char Data_Clear[9] = {27, 91, 50, 74, 0, 27, 91, 72, 0}; 
unsigned char my_tx_arr1[]={0x12,0x34,0x56,0x78,0x9A,0x21,0x43,0x65,0x87,0xA9};
unsigned char my_tx_arr2[]={0x92,0x94,0x99,0x78,0x53,0x78,0x29,0x02,0xac,0xde}; 
unsigned char my_xmodem_strt[]="Start XMODEM downlaod...Send file";
unsigned char my_xmodem_end[]="XMODEM DONE...";
//unsigned char my_STOP_arr[]="STOP MAN";
static const unsigned char ScipherKey[17] = "KALMAR2019PHASE3";
unsigned long TransmissionTimeStamp = 0;
extern __boolean sw_flag;
void MY_FLash_Write(void);

void MY_FLash_Erase(void);
void MyErrorHandler(void);
void MY_Application_Run(void);
void dummyfunction(void);
void lcdInit(void);
//----------------------------------------------------------------------------------------------------------------


void main(void) 
{
	unsigned char resetReason = RESF ;

	int x =0;
    //MY_TAU0_Channel0_Start();		//TAU0 start
 //  MY_Application_Run();
  unsigned long temp = 0xFFFFFFFF;
   WDTE=0xAC;
    //for(i=0;i<60000;i++);// for(i=0;i<60000;i++); for(i=0;i<60000;i++);
  //  for(i=0;i<60000;i++); for(i=0;i<60000;i++); for(i=0;i<60000;i++);
  // lcdInit();
   
   // WDTE=0xAC;
    //	 MY_Switch_Init();		//External INTP0 initiallization
    
    //HIGH_BEAM_MODE = 0; // to make output
    //CAN_S_PM = 0;
    //CAN_S = 0;
    MY_CAN_Init();
    MY_CAN_MsgBuf_Init();  
  //  RAM_INTP0_ISR = & MY_ISR_INTP0;   
   //RAM_INTP0_ISR = & dummyfunction;
    RAM_INTC0REC_ISR = &MY_Rx_MsgBuf_Processing;
    RAM_INTC0WUP_ISR = &MY_ISR_CAN_Wakeup;
    RAM_INTC0ERR_ISR = &MY_ISR_CAN_Error;
    RAM_INTC0TRX_ISR = &MY_Tx_MsgBuf;
    RAM_INTTM00_ISR = &system1msClock;

    
RAM_INTCLM_ISR = &dummyfunction;
RAM_INTWDTI_ISR = &dummyfunction;
RAM_INTAD_ISR = &dummyfunction;
RAM_INTIIC11_ISR = &dummyfunction;
RAM_INTTM01_ISR = &dummyfunction;
RAM_INTTM17_ISR = &dummyfunction;
RAM_INTTM20_ISR = &dummyfunction;
RAM_INTTM21_ISR = &dummyfunction;
RAM_INTTM22_ISR = &dummyfunction;
RAM_INTTM24_ISR = &dummyfunction;

system1msClockEnable();


    	   // NOP();
	    //jump to application
 
	    
	fsl_descr.fsl_flash_voltage_u08 = 0x00;
	fsl_descr.fsl_frequency_u08 = 0x10;				// it is 0x10 = 16 MHz
	fsl_descr.fsl_auto_status_check_u08 = 0x01;

	my_fsl_status = FSL_Init((__far fsl_descriptor_t*)&fsl_descr);
	if(my_fsl_status != FSL_OK) 
	MyErrorHandler();

   
    EI();

    MY_Rx_MsgBuf_Init(0,FILTER_ID_1);
    MY_Rx_MsgBuf_Init(1,FILTER_ID_1);
    MY_Rx_MsgBuf_Init(2,FILTER_ID_1);
    MY_Rx_MsgBuf_Init(3,FILTER_ID_1);
    MY_Rx_MsgBuf_Init(4,FILTER_ID_1);
    MY_Rx_MsgBuf_Init(5,FILTER_ID_1);
    MY_Rx_MsgBuf_Init(6,FILTER_ID_1);
    MY_Rx_MsgBuf_Init(7,FILTER_ID_1);

    MY_Rx_MsgBuf_Init(8,FILTER_ID_1);	//update & stop
    MY_Rx_MsgBuf_Init(9,FILTER_ID_1);	//random data lower
	MY_Rx_MsgBuf_Init(10,FILTER_ID_1);	//random data higher

	/*MY_Tx_MsgBuf_Init(13,TX_ID13, MSG_LEN);	//update acknowledgement
	MY_Tx_MsgBuf_Init(12,TX_ID12, MSG_LEN);	//packet acknowledgement
	MY_Tx_MsgBuf_Init(14,TX_ID14, MSG_LEN);	//received encypt data lower
    MY_Tx_MsgBuf_Init(15,TX_ID15, MSG_LEN);	//received encypt data higher
    
  for(i=0;i<60000;i++);*/
  for(i=0;i<60000;i++);

  /************************debug************************/
//  	if ( (resetReason) == 0x10 )
//    	MY_Tx_MsgBuf_Processing(0x8,15,MSG_LEN,"hel    "); //DEBUG ONLY
//	else
// 	 	MY_Tx_MsgBuf_Processing(0x9,14,MSG_LEN,"fgt    "); //DEBUG ONLY

  
  if ( (resetReason) == 0x80)
  {
	  MY_Tx_MsgBuf_Processing(0x10,8,MSG_LEN,"TRA    "); //DEBUG ONLY
  }
  else if ((resetReason) == 0x10)
  {
	  MY_Tx_MsgBuf_Processing(0x15,13,MSG_LEN,"WDT    "); //DEBUG ONLY
  }
  else if ( (resetReason) == 0x04)
  {
	  MY_Tx_MsgBuf_Processing(0x12,10,MSG_LEN,"RPERF  "); //DEBUG ONLY
  }
  else if ( (resetReason) == 0x02 )
  {
	  MY_Tx_MsgBuf_Processing(0x13,11,MSG_LEN,"IAWRF  "); //DEBUG ONLY
	  //while(1);
  }
  else if ( (resetReason) == 0x01 )
  {
	  MY_Tx_MsgBuf_Processing(0x14,12,MSG_LEN,"LVIRF  "); //DEBUG ONLY
  }
  else if (resetReason == 0x00)
  {
	  MY_Tx_MsgBuf_Processing(0x11,9,MSG_LEN,"POWer  "); //DEBUG ONLY
  }
  else
  {
	  MY_Tx_MsgBuf_Processing(0x16,14,MSG_LEN,"ELSE   "); //DEBUG ONLY
  }
  



	/*********************************************/
  
  if(boot_flag == 1)  
  {
	 MY_Tx_MsgBuf_Processing(0x12,12,MSG_LEN,my_sw_pressed);
	 sendRandomNumber();
	 while(!baes1 || !baes2 )
	 {
	 	WDTE=0xAC;
	 }
	
	 KeyMatch();
	if(bKeyMatch)
	{   
 		MY_Tx_MsgBuf_Processing(0x12,12,MSG_LEN,my_sw_pressed);	
 //--------------------------------------------------------------------
	    rx_data_cnt = 0;
	    Address = 0x4000;
	    WDTE=0xAC;
	    iverify_blk = START_BLOCK;
	    DI();
	    MY_FLash_Erase();
	    EI();
	    WDTE=0xAC;
	    //if(boot_flag == 1)
	 	
	    while(!DONE_flag)
	    {
		/*	if(bCANError)
			{
				PCKSEL = 0x00;
				  MY_CAN_Init();
  				  MY_CAN_MsgBuf_Init();  
				bCANError = 0;
			}*/
		    MY_check_half_packet_rx();					//check if half packet received?
		    if(!DONE_flag)
		    {
			    WDTE=0xAC;
				
			    MY_Tx_MsgBuf_Processing(0x13,13,MSG_LEN,my_send_next_half);	//request next half packet
			    WDTE=0xAC;
			    MY_check_half_packet_rx();					// check if remaining half packet received?
			    WDTE=0xAC;
			    DI();
			    if(poweronflag==0)
			    {
				    for(i=0;i<60000;i++);for(i=0;i<60000;i++);
				    poweronflag=1;
			    }
			   //  MY_Tx_MsgBuf_Processing(13,MSG_LEN,my_send_next_half);
			   for(x =0; x < 200; x++)
			   {
				   	MY_RxByteBuffer[x] = MY_RxByteBuffer[x] + 0x01;
			   }
			   
			    MY_FLash_Write();						//Write 128 byte packet on to flash
			    EI();
			  //  WDTE=0xAC;
			    MY_Tx_MsgBuf_Processing(0x13,13,MSG_LEN,my_send_next_half);	//request next half packet
			    rx_data_cnt = 0;
			    WDTE=0xAC;
				
		    }
    		}
	}
   }
    while(1)
    {
	    WDTE=0xAC;
	    NOP();
		
		vRecoveryFromBusOffState();
		
		//PCKSEL =0x00;
		if(Error_flag == 0)
		{
		    //jump to application
			
		    MY_Application_Run();
			
		}
		else
		{

			if(gu32Clock1msTicker - u32BlinkTimeStamp > 1000)
			{
				u32BlinkTimeStamp = gu32Clock1msTicker;
				if(Blinkflag)
				{ 
					//HIGH_BEAM_ON;
					Blinkflag = 0;
				}
				else
				{
					//HIGH_BEAM_OFF;
					Blinkflag = 1;
				}
			}
			
		 	//Bilink LED
		}
    }
 
    
}

//----------------------------------------------------------------------------------------------------------------
void MY_FLash_Erase(void)
{
	
	FSL_Open();    	
	FSL_PrepareFunctions();	
	
	for( n=START_BLOCK; n<=END_BLOCK; n++ )
	{
		WDTE=0xAC;
		my_fsl_status = FSL_Erase(n);
		while(my_fsl_status == FSL_BUSY)
		{
			my_fsl_status = FSL_StatusCheck();
			WDTE=0xAC;
		}
		if(my_fsl_status == FSL_OK)
		{
			//PASS
		}
		else
		{
			//FAIL
			MyErrorHandler();
		}
		
	}
	FSL_Close();
	
}
//----------------------------------------------------------------------------------------------------------------
void MY_FLash_Write(void)
{
	
	FSL_Open();    	
	FSL_PrepareFunctions();	
	
	my_fsl_write_str.fsl_data_buffer_p_u08 = (fsl_u08 *)&MY_RxByteBuffer[0];
	my_fsl_write_str.fsl_word_count_u08 = 0x20;
	my_fsl_write_str.fsl_destination_address_u32 = Address;
	my_fsl_status = FSL_Write((__near fsl_write_t*)&my_fsl_write_str);
	if(my_fsl_status != FSL_OK) 
	MyErrorHandler();
	
	verify_block_cnt++;
        if(verify_block_cnt>=8)		//check if one bloak filled. If yes, verify.
        {
        	my_fsl_status = FSL_IVerify(iverify_blk);
        	iverify_blk++;
        	verify_block_cnt = 0;
        }
	
	if(my_fsl_status == FSL_OK) 
	{
		Address += 128;
		//Status = PROG_PASS;
	}
	else
	{
		MyErrorHandler();
		//Status = PROG_FAIL;
	}
	FSL_Close();

}
//----------------------------------------------------------------------------------------------------------------
void MY_Application_Run(void)
{
	//__far unsigned int *ptr;
	__far unsigned int *ptr;
	pt2Function fp;
	

	ptr = (__far unsigned int *)( FIRST_USER_FLASH_ADDR );

	if(boot_flag == 0)
	{
		if ( *ptr != 0xffff )
		{
			// call user program and never return
			MY_Tx_MsgBuf_Processing(0x40,13,MSG_LEN,"InBoot "); //DEBUG ONLY
			
			//DI();
			fp = (pt2Function) ptr;
			fp( 0, 0);
			//MY_Tx_MsgBuf_Processing(0x44,13,MSG_LEN,"BELOW  "); //DEBUG ONLY
		}
		MY_Tx_MsgBuf_Processing(0x45,13,MSG_LEN,"NoApp  "); //DEBUG ONLY
	}
	else
	{
		// invalid user reset address
		// some problem return
		return;
	}

	
}
//----------------------------------------------------------------------------------------------------------------
void MyErrorHandler(void)
{
	WDTE = 0xAC;
	NOP();
}


void lcdInit(void)
{
    LCDMD = 0x10;
    /* Set segment output */
    LCDPF0 = 0xFFU;
    LCDPF1 = 0x78U;
    LCDPF3 = 0xFFU;
    LCDPF5 = 0xF0U;
    LCDPF7 = 0x3CU;
    LCDM = 0x00;
    LCDC0 = 0x40;
    SCOC = 1U; //lcdVoltageOn
    LCDON = 1U;
    //SEG0=0xff;SEG1=0xff;SEG2=0xff;SEG3=0xff;SEG4=0xff;SEG5=0xff;SEG6=0xff;SEG7=0xff;SEG8=0xff;
}

void dummyfunction(void)
{
NOP();
}

void CanDisable(void)
{
	C0CTRL &= 0xf8ff;
	C0CTRL |= 0x0007;
	while(C0CTRL & 0x0007 !=0);
	C0GMCTRL = 0x0001;
	NOP();NOP(); 
}
