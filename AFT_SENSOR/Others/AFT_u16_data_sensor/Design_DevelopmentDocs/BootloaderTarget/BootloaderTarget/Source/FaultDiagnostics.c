#include "r_cg_macrodriver.h"
#include "customTypedef.h"

#include "r_cg_userdefine.h"
#include "TML_14229_UDSH.h"

#include "J1939.h"
#include "Can.h"

#define TX_ID 0x1CFF1717

u8 u8ReadFlashData[256];
static sJ1939_RX_MSG_TYPE ProprietaryResponse;
__far u8 *pFlashDataPtr;		// Far pinting to an 8bit Data.



typedef union
{	
	u8 TxDataConfig[2];
	struct
	{
		struct
		{
			u8 FlashDataTxFlag : 1; //bit 0
			u8 EepromDataTxFlag : 1; 
			u8 SpeedometerDataTxFlag : 1;
			u8 TachometerDataTxFlag : 1;
			u8 ApgDataTxFlag : 1;
			u8 FuelGaugeDataTxFlag : 1;
			u8 TempGaugeDataTxFlag : 1;
			u8 OilPressDataTxFlag:1;	//MSB
					
		}Byte1;

		struct
		{
			u8 DefDataTxFlag :1;	//LSB
			u8 AfeDataTxFlag:1;
			u8 OdoDataTxFlag:1;
			u8 HourDataTxFlag:1;
			u8 res5:1;
			u8 res6:1;
			u8 res7:1;
			u8 res8:1;	//MSB
			
		}Byte2;	
		
	}TxDataConfigBytes;
	
}TX_DATA_CONFIG;

typedef enum
{
	eResumeNormalMode =0,
	eflash_Cmd,
	eEeprom_Cmd,
	eSpeedo_Cmd,
	eTacho_Cmd,
	eAirPressure_Cmd,
	eFuel_Cmd,
	eTemperature_Cmd,
	eOilPressure_Cmd,
	eDef_Cmd,
	eAfe_Cmd,
	eOdo_Cmd,
	eHour_Cmd,
	NoOfCommand
}eCommand;

#define KEY1	0x37U
#define KEY2	0x96U

TX_DATA_CONFIG TxData;
static eCommand CountCmd = 0;


static void _vReadAndSendFlashData(void);


void vTx_Critical_Internally_Data()
{
	u8 temp1;
	u16 temp;
	u8 FramNum =0;
	u32 tempdata =0;

	temp1 = u8GetIILSecurityAccessStatus();
	if(temp1 == eIILServerUnlocked)
	{
	
		tempdata = u32GetPGNdata(eJIILFaultDiag);

		temp1 = (u8)(tempdata & 0x000000FF);
		if(temp1 != KEY1)
	 	{
	 		return;
	 	}

		temp1 = (u8) ( (tempdata >> 8) & 0x000000FF );
		if(temp1 != KEY2)
	 	{
	 		return;
	 	}

		temp1 = (u8) ( (tempdata >> 24) & 0x000000FF );
		
		if(temp1 != 1)
	 	{
	 		return;
	 	}

		temp1 = (u8) ( (tempdata >> 16) & 0x000000FF );

		switch(temp1)
		{
			case eResumeNormalMode: 
				TxData.TxDataConfig[0] = 0;
				TxData.TxDataConfig[1] = 0;
			//Clear security flag and communication stop flag
			break;
			
			case eflash_Cmd: 
				TxData.TxDataConfigBytes.Byte1.FlashDataTxFlag = TRUE;				
			break;
			
		}
		
		if( TxData.TxDataConfigBytes.Byte1.FlashDataTxFlag == TRUE )
		{				
			_vReadAndSendFlashData();
			//set flag FALSE so that data tx only ones time
			TxData.TxDataConfigBytes.Byte1.FlashDataTxFlag  = FALSE;  
		}
	}
		
}

static void _vReadAndSendFlashData()
{
	static bool bReadOk = FALSE;
	u32 temp =  0;
	u16 i = 0;
	u16 j = 0, x=0;
	u32 u32TimeStamp = 0;
	pFlashDataPtr = 0xf400;
	
	while(temp < 0x04 )//&& (pFlashDataPtr & 0xffff) == 0)
	{
		R_WDT_Restart();
		temp = pFlashDataPtr;
		temp = temp >> 16;
		if(pFlashDataPtr == 0xffff)
		{
			R_WDT_Restart();
		}
		for(i = 0; i<=255; i++)
		{
			R_WDT_Restart();
			u8ReadFlashData[i] = *pFlashDataPtr;	// get all 259 data into an Array.
			if(pFlashDataPtr == 0xffff)
				pFlashDataPtr = 0x10000;
			else if(pFlashDataPtr == 0x1ffff)
				pFlashDataPtr = 0x20000;
			else if(pFlashDataPtr == 0x2ffff)
				pFlashDataPtr = 0x30000;
			else if(pFlashDataPtr == 0x3ffff){
				pFlashDataPtr = 0x40000;
				temp = 4;}
			else
				pFlashDataPtr++;
		}
		
		ProprietaryResponse.bDataRdy = TRUE;
		for(x = 0; x<= 41; x++)
		{
			if( ProprietaryResponse.bDataRdy == TRUE )
			{
				ProprietaryResponse.sMsgID.Identifier = 0x1cff1717;		
				
				for(j = 0; j<=7; j++)
				{
					if(j == 0)
					{
						ProprietaryResponse.u8data[0] = x+1;
					}
					else if(j == 1)
					{
						ProprietaryResponse.u8data[1] = 1;
					}
					else
					{
						ProprietaryResponse.u8data[j] = u8ReadFlashData[j + (6*x) - 2];
					}
				}
				
				vTxMsgBufProcessing(ProprietaryResponse.sMsgID.Identifier,8,ProprietaryResponse.u8data);
				u32TimeStamp = u32GetClockTicks();
				while(u32GetClockTicks() - u32TimeStamp < 1)
				{
					R_WDT_Restart();
				}
			}			
		}
		if(x == 42)
		{
			if( ProprietaryResponse.bDataRdy == TRUE )
			{
				ProprietaryResponse.sMsgID.Identifier = 0x1cff1717;		
				
				for(j = 0; j<=7; j++)
				{
					if(j == 0)
					{
						ProprietaryResponse.u8data[0] = x+1;
					}
					else if(j == 1)
					{
						ProprietaryResponse.u8data[1] = 1;
					}
					else if(j == 2 || j == 3 || j == 4 || j == 5 )
					{
						ProprietaryResponse.u8data[j] = u8ReadFlashData[j + (6*x) - 2];
					}				
				}
				vTxMsgBufProcessing(ProprietaryResponse.sMsgID.Identifier,6,ProprietaryResponse.u8data);
				u32TimeStamp = u32GetClockTicks();
				while(u32GetClockTicks() - u32TimeStamp < 1)
				{
					R_WDT_Restart();
				}
			}
		}
		ProprietaryResponse.bDataRdy = FALSE;
	}
}


