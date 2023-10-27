
#include "Flash.h"

#include "fsl.h"

#include "fsl_types.h"
#include "r_cg_userdefine.h"
#define START_BLOCK	0x3D
#define END_BLOCK	0xFE

__near fsl_write_t my_fsl_write_str;
unsigned char *fsl_data_buffer_u08;
fsl_u08 my_fsl_status;
fsl_descriptor_t fsl_descr;
fsl_write_t my_fsl_write_str;

static u16 TesterBlockCount=0;
u32 u32Address = FIRST_USER_FLASH_ADDR;
u8 u8verify_block_cnt = 0;
u16 iverify_blk = 0;

u8 u8FlashData[256];
unsigned char MY_RxByteBuffer[256] = "StartXMODEMdownloadSendfileStartXMODEMdownloadSendfileStartXMODEMdownloadSendfileStartXMODEMdownloadSendfileStartXMODEMdownloadSendfileStartXMODEMdownloadSendfileStartXMODEMdownloadSendfileStartXMODEMdownloadSendfileStartXMODEMdownloadStartXMODEMdownloaddd";
unsigned char rxstatus;

static bool _bVerifyFlashData();

/*********************************************************************//**
 *
 * Verify the latest 256 bytes written in the flash data and increment 
 * TesterBlockCount.
 *
 * @param      None 
 *
 * @return     None
 *************************************************************************/
static bool _bVerifyFlashData()
{	
	u16 i = 0;
	u8 u8DataOk = 0;
	__far u8 *FlashPointer;		// Far pinting to an 8bit Data.
	
	if(u32Address > FIRST_USER_FLASH_ADDR)
	{
		FlashPointer = u32Address-256;	// Address assigned after decrementing 256 bytes 
	}									// to identify the startaddress of latest 256 bytes.
	else
	{
		return FALSE;
	}
	for(i = 0; i<= 0xff; i++)
	{
		u8FlashData[i] = *FlashPointer;// get all 256 data into an Array.
		FlashPointer++;
	}
	for(i = 0; i< 0xff; i++)
	{
		if(	u8FlashData[i] == MY_RxByteBuffer[i])	// Compare the read written data with recieved 
		{											// buffer.
			u8DataOk++;
		}	
		else
		{
			u8DataOk = 0;
		}
	}
	if(u8DataOk == 255)
	{
		TesterBlockCount++;							// if the compare function gives dataok count as 255 (0-255)
		return TRUE;								// increment testerBlockCount and return true.
	}
	else
	{
		TesterBlockCount=0;
		return FALSE;
	}
}

/*********************************************************************//**
 *
 * 
 * Return No of block written in flash
 *
 * @param      None 
 *
 * @return     None
 *************************************************************************/
u16 u16GetNoOfBlockReturn()
{
	return (TesterBlockCount);
}

/*********************************************************************//**
 *
 * Initilize important variable before start flash writting 
 *
 * @param      None 
 *
 * @return     None
 *************************************************************************/
void vInitFlashImpVariable()
{
	u32Address = FIRST_USER_FLASH_ADDR;
	TesterBlockCount = 0;	
	u8verify_block_cnt = 0;
	iverify_blk = 0;
}

/*********************************************************************//**
 *
 * Initialize flash Library. 
 *
 * @param      None 
 *
 * @return     None
 *************************************************************************/
void vFlashInit()
{
	DI();
	fsl_descr.fsl_flash_voltage_u08 = 0x00;
	fsl_descr.fsl_frequency_u08 = 0x18;
	fsl_descr.fsl_auto_status_check_u08 = 0x01;

	my_fsl_status = FSL_Init((__far fsl_descriptor_t*)&fsl_descr);
	if(my_fsl_status != FSL_OK) 
	MyErrorHandler();
	
	EI(); 
}

/*********************************************************************//**
 *
 * Erase one block (1kb or 1024 bytes )data in the flash memory. Number 
 * of blocks starts at macro START_BLOCK and ends at macro END_BLOCK. 
 *
 * @param      None 
 *
 * @return     None
 *************************************************************************/
void vEraseFlash()
{
	u8 u8BlockNumber = 0;
	DI();
	FSL_Open();    	
	FSL_PrepareFunctions();	
	
	for( u8BlockNumber=START_BLOCK; u8BlockNumber<=END_BLOCK; u8BlockNumber++ )
	{
		WDTE=0xAC;
		my_fsl_status = FSL_Erase(u8BlockNumber);
		while(my_fsl_status == FSL_BUSY)
		{
			my_fsl_status = FSL_StatusCheck();
			WDTE=0xAC;
		}
		if(my_fsl_status == FSL_OK)
		{
			//R_WDT_Restart();//PASS
		}
		else
		{
			//FAIL
			MyErrorHandler();
		}
		
	}
	FSL_Close();
	EI();
}

/*********************************************************************//**
 *
 * This Block writes 256 bytes of data into the flash memory and increments
 * u32Address by 256 bytes for next write cycle.
 *
 * @param      None 
 *
 * @return     Written data is OK or NOT OK.
 *************************************************************************/
bool bWriteFlash(void)
{	
	bool bVerifyWrittenBlock = FALSE;
	DI();
	if(u32Address > 0x3FF00)
	{
		EI();
		return TRUE;	
	}
	FSL_Open();    	
	FSL_PrepareFunctions();	
	
	my_fsl_write_str.fsl_data_buffer_p_u08 = (fsl_u08 *)&MY_RxByteBuffer[0];	// Recieve buffer address
	my_fsl_write_str.fsl_word_count_u08 = 0x40;									// 64 word counts
	my_fsl_write_str.fsl_destination_address_u32 = u32Address;					
	my_fsl_status = FSL_Write((__near fsl_write_t*)&my_fsl_write_str);
	if(my_fsl_status != FSL_OK) 
	MyErrorHandler();
	/*
	u8verify_block_cnt++;
    if(u8verify_block_cnt>=4)		//check if one block filled. If yes, verify.
    {
    	my_fsl_status = FSL_IVerify(iverify_blk);
    	iverify_blk++;
    	u8verify_block_cnt = 0;
    }
	*/
	
	if(my_fsl_status == FSL_OK) 
	{
		u32Address += 256;
	}
	else
	{
		MyErrorHandler();
	}
	FSL_Close();
	bVerifyWrittenBlock = _bVerifyFlashData();	// Verify written data with recieve data and return True 
	EI();												// or False.
	return bVerifyWrittenBlock;
}





