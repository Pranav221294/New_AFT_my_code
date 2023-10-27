/****************************************************************************************************************/
/* COPYRIGHT                                                                                                    */
/* Inidication Instruments Ltd.(c) 2022                                                                         */
/****************************************************************************************************************/
/* File name   : {file_name}                                                                                    */
/* Module      : {file_path}                                                                                    */
/*--------------------------------------------------------------------------------------------------------------*/
/* Description :                                                                                                */
/****************************************************************************************************************/
/* History                                                                                                      */
/*--------------------------------------------------------------------------------------------------------------*/
/* Date       | Revised By  | Rev. No.  | Description                                                           */
/*--------------------------------------------------------------------------------------------------------------*/
/*                                                                                                              */
/****************************************************************************************************************/

/****************************************************************************************************************/
/* Includes                                                                                                     */
/****************************************************************************************************************/

#include "GuiCommProtocol.h"
#include "r_cg_wdt.h"
#include "J1939.h"
/****************************************************************************************************************/
/****************************************************************************************************************/
/* Defines                                                                                                      */
/****************************************************************************************************************/

#define GUI_RESPONSE_TIMEOUT        310u                // 310msec, 10 msec more

#define DLC_LENGTH                  8u

#define CAN_ID_18FF5125             0x18FF5125u         // send live data to GUI Pkt 1
#define CAN_ID_18FF5225             0x18FF5225u         // send live data to GUI Pkt 2
#define CAN_ID_18FF5325             0x18FF5325u         // send stored data to GUI Pkt 1
#define CAN_ID_18FF5425             0x18FF5425u         // send stored data to GUI Pkt 2
#define CAN_ID_18FF5525             0x18FF5525u         // send live data to GUI Pkt 3

#define CAN_ID_18FF6925             0x18FF6925u         // get one time clibration command and send response on same

/****************************************************************************************************************/
/* Enumeration                                                                                                  */
/***************************************************************************************************************/


/****************************************************************************************************************/
/* Extern variable                                                                                                  */
/***************************************************************************************************************/

extern e_states_type e_runStatus ;
extern bool CAN_CalibrationCompleteFlag ;
extern s_liveParam_type liveParam_struct;

extern PGN_DATA_TYPE sPGNFF69_oneTimeCalib ;
/****************************************************************************************************************/
/* Type definitions                                                                                             */
/****************************************************************************************************************/

/****************************************************************************************************************/
/* Local variables                                                                                              */
/****************************************************************************************************************/

static u8 DLC_PGN_FF51[DLC_LENGTH];
static u8 DLC_PGN_FF52[DLC_LENGTH];
static u8 DLC_PGN_FF53[DLC_LENGTH];
static u8 DLC_PGN_FF54[DLC_LENGTH];
static u8 DLC_PGN_FF55[DLC_LENGTH];

static guiCommState_type GuiCommFlag = e_noTransOfDataInitial;

/****************************************************************************************************************/
/* Local function declarations                                                                                  */
/****************************************************************************************************************/
/// @addtogroup MODULE_LIBRARY
/// @{
/// @defgroup ModuleLib  Private functions used 
//// 
///   
/// @addtogroup ModuleLib  
/// @{
      		
	//Local function declarations.....................................

static void displayDataOnGUI(u32 CAN_Id, u8 *Can_Data);
static void fillLiveDataInDLC_PGN_FF51(void);
static void fillLiveDataInDLC_PGN_FF52(void);
static void fillLiveDataInDLC_PGN_FF55(void);

static void fillStoredDataInDLC_PGN_FF53(void);
static void fillStoredDataInDLC_PGN_FF54(void);

static void oneTimeCalibrationResponse(void);



/// @} // endgroup ModuleLib  

/// @} // endgroup MODULE_LIBRARY

/****************************************************************************************************************/
/* Local function definitions                                                                                   */
/****************************************************************************************************************/

/*************************************************************************************************************//**
 *
 * Purpose  	:   send msg through CAN
 *
 * @param[in]	:  	None    
 * @param[out]	:	None
 * @return   	:	None
 ****************************************************************************************************************/

static void displayDataOnGUI(u32 CAN_Id, u8 *Can_Data)
{
    if(bCheckCanTxBufferAvailable() == TRUE)
	{
		vTxMsgBufProcessing(CAN_Id,DLC_LENGTH,Can_Data,J1939_TX);
	}
}

/*************************************************************************************************************//**
 *
 * Purpose  	:   Fill Live data in DLC of PGN FF51
 *
 * @param[in]	:  	None    
 * @param[out]	:	None
 * @return   	:	None
 ****************************************************************************************************************/

static void fillLiveDataInDLC_PGN_FF51(void)
{
    u16 temp = 0 ;
    
    temp = liveParam_struct.p0_live;
    DLC_PGN_FF51[0] = (u8)(temp >> 8) ;     // upper byte extract
    DLC_PGN_FF51[1] = (u8)(temp & 0x00FF);  // lower byte extract

    temp = liveParam_struct.p1_live;
    DLC_PGN_FF51[2] = (u8)(temp >> 8) ;     // upper byte extract
    DLC_PGN_FF51[3] = (u8)(temp & 0x00FF);  // lower byte extract

    temp = liveParam_struct.p2_live;
    DLC_PGN_FF51[4] = (u8)(temp >> 8) ;     // upper byte extract
    DLC_PGN_FF51[5] = (u8)(temp & 0x00FF);  // lower byte extract

    temp = liveParam_struct.current_level_p1;
    DLC_PGN_FF51[6] = (u8)(temp >> 8) ;     // upper byte extract
    DLC_PGN_FF51[7] = (u8)(temp & 0x00FF);  // lower byte extract
}


/*************************************************************************************************************//**
 *
 * Purpose  	:   Fill Live data in DLC of PGN FF52
 *
 * @param[in]	:  	None    
 * @param[out]	:	None
 * @return   	:	None
 ****************************************************************************************************************/
u16 debugCurrentMiliVolt  = 0 ;         // for debug only
static void fillLiveDataInDLC_PGN_FF52(void)
{
    u16 temp = 0 ;
    float tempFloat = 0 ;
    temp = liveParam_struct.current_level_p2;
    DLC_PGN_FF52[0] = (u8)(temp >> 8) ;     // upper byte extract
    DLC_PGN_FF52[1] = (u8)(temp & 0x00FF);  // lower byte extract

  //  tempFloat = liveParam_struct.live_div_f;
    temp = tempFloat * 1000 ;
    DLC_PGN_FF52[2] = (u8)(temp >> 8) ;     // upper byte extract
    DLC_PGN_FF52[3] = (u8)(temp & 0x00FF);  // lower byte extract

 //   tempFloat = liveParam_struct.div_average_f;
    temp = tempFloat * 1000 ;
    DLC_PGN_FF52[4] = (u8)(temp >> 8) ;     // upper byte extract
    DLC_PGN_FF52[5] = (u8)(temp & 0x00FF);  // lower byte extract

	// send voltage in milivolt, devide by 10 to send data upto 2 decimal digit
    temp = ( ( MAX_OUT_VOL_AT_50_DUTY * (TDR25)) ) / MAX_VAL_TDR25_FOR_50_DUTY  / 10 ;
    debugCurrentMiliVolt = temp ;   // for debug only
    DLC_PGN_FF52[6] = (u8)(temp >> 8) ;     // upper byte extract
    DLC_PGN_FF52[7] = (u8)(temp & 0x00FF);  // lower byte extract

}


/*************************************************************************************************************//**
 *
 * Purpose  	:   Fill Live data in DLC of PGN FF55
 *
 * @param[in]	:  	None    
 * @param[out]	:	None
 * @return   	:	None
 ****************************************************************************************************************/
static void fillLiveDataInDLC_PGN_FF55(void)
{
    u16 temp = 0 ; 
    float temp_float = 0 ;

    /* data packed for PGN ff54 DLC */
 //   temp_float = liveParam_struct.currUsedDiv ;
    temp = temp_float * 1000 ;
    DLC_PGN_FF55[0] = (u8)(temp >> 8) ;     // upper byte extract
    DLC_PGN_FF55[1] = (u8)(temp & 0x00FF);  // lower byte extract

    temp_float = vRead_Div_Avg_Store_fromFlash();
    temp       = temp_float * 1000 ;
    DLC_PGN_FF55[2] = (u8)(temp >> 8) ;         // upper byte extract
    DLC_PGN_FF55[3] = (u8)(temp & 0x00FF);      // lower byte extract


    DLC_PGN_FF55[4] = (u8)(0xFF) ;              // upper byte extract
    DLC_PGN_FF55[5] = (u8)(0xFF);               // lower byte extract
    DLC_PGN_FF55[6] = (u8)(0xFF) ;              // upper byte extract
    DLC_PGN_FF55[7] = (u8)(0xFF);               // lower byte extract 
}


/*************************************************************************************************************//**
 *
 * Purpose  	:   Fill Stored data in DLC of PGN FF53
 *
 * @param[in]	:  	None    
 * @param[out]	:	None
 * @return   	:	None
 ****************************************************************************************************************/
static void fillStoredDataInDLC_PGN_FF53(void)
{
    u16 temp = 0 ;     
    /* data packed for PGN ff53 DLC  */
    temp = vRead_P0_cal_fromFlash();
    DLC_PGN_FF53[0] = (u8)(temp >> 8) ;     // upper byte extract
    DLC_PGN_FF53[1] = (u8)(temp & 0x00FF);  // lower byte extract

    temp = vRead_P1E_cal_fromFlash();
    DLC_PGN_FF53[2] = (u8)(temp >> 8) ;     // upper byte extract
    DLC_PGN_FF53[3] = (u8)(temp & 0x00FF);  // lower byte extract

    temp = vRead_P2E_cal_fromFlash();
    DLC_PGN_FF53[4] = (u8)(temp >> 8) ;     // upper byte extract
    DLC_PGN_FF53[5] = (u8)(temp & 0x00FF);  // lower byte extract

    temp = vRead_P1F_cal_fromFlash();
    DLC_PGN_FF53[6] = (u8)(temp >> 8) ;     // upper byte extract
    DLC_PGN_FF53[7] = (u8)(temp & 0x00FF);  // lower byte extract

}


/*************************************************************************************************************//**
 *
 * Purpose  	:   Fill Stored data in DLC of PGN FF54
 *
 * @param[in]	:  	None    
 * @param[out]	:	None
 * @return   	:	None
 ****************************************************************************************************************/
static void fillStoredDataInDLC_PGN_FF54(void)
{
    u16 temp = 0 ; 
    float temp_float = 0 ;

    /* data packed for PGN ff54 DLC */
    temp = vRead_P2F_cal_fromFlash();
    DLC_PGN_FF54[0] = (u8)(temp >> 8) ;     // upper byte extract
    DLC_PGN_FF54[1] = (u8)(temp & 0x00FF);  // lower byte extract

    temp_float = vRead_Div_cal_fromFlash();
    temp = temp_float * 1000 ;
    DLC_PGN_FF54[2] = (u8)(temp >> 8) ;     // upper byte extract
    DLC_PGN_FF54[3] = (u8)(temp & 0x00FF);  // lower byte extract

    temp = vRead_Span_cal_fromFlash();
    DLC_PGN_FF54[4] = (u8)(temp >> 8) ;     // upper byte extract
    DLC_PGN_FF54[5] = (u8)(temp & 0x00FF);  // lower byte extract

    temp = vRead_Cali_Status_fromFlash();
    DLC_PGN_FF54[6] = (u8)(temp >> 8) ;     // upper byte extract
    DLC_PGN_FF54[7] = (u8)(temp & 0x00FF);  // lower byte extract 
}


/*************************************************************************************************************//**
 *
 * Purpose  	:   one time calibration request check and send response
 *
 * @param[in]	:  	None    
 * @param[out]	:	None
 * @return   	:	None
 ****************************************************************************************************************/

static void oneTimeCalibrationResponse(void)
{
    u8 dataByte = 0 ; 
    u8 DLC_data[8] = {0};
    dataByte = extractByteFromPGN(&sPGNFF69_oneTimeCalib, eData_D1);

    /* if D1 byte is 0xAA then only send response with add 0x55 at D1 byte */
    if (dataByte == 0xAA)
    {
        DLC_data[0] = 0x55 ;
        /* clear one time calibration flag */
        vSave_OneTimeCalibFlag_inFlash(false);

        /* send response of that on etime calibration flag has been cleared */
        displayDataOnGUI(CAN_ID_18FF6925, DLC_data);
    }
}


/****************************************************************************************************************/
/* Global function definitions                                                                                   */
/****************************************************************************************************************/

//*************************************************************************************************************//**
 /*
 * Purpose  	:   set the status of GuiCommFlag
 *
 * @param[in]   :	status
 * @return    	: 	None
 ****************************************************************************************************************/

void setGuiCommStatus(guiCommState_type state)
{
    GuiCommFlag = state ;
}


//*************************************************************************************************************//**
 /*
 * Purpose  	:  get the status of GuiCommFlag
 *
 * @param[in]   :	None
 * @return    	: 	status
 ****************************************************************************************************************/

guiCommState_type getGuiCommStatus(void)
{
    return GuiCommFlag ;
}


//*************************************************************************************************************//**
 /*
 * Purpose  	:   this function handles the GUI communication
 *
 * @param[in]   :	None
 * @return    	: 	status
 ****************************************************************************************************************/
void GuiCommManager(void)
{
    static guiCommState_type liveDataFlag = 0 ;
    static bool CAN_CaliCommStatusFlag = false ;
    
    guiCommState_type commStatusTemp = getGuiCommStatus();

    if (commStatusTemp == e_getStoredData)  
    {
        fillStoredDataInDLC_PGN_FF53();
        fillStoredDataInDLC_PGN_FF54();
        fillLiveDataInDLC_PGN_FF55();
        displayDataOnGUI(CAN_ID_18FF5425, DLC_PGN_FF54);
        vSetBlockDelayMilli(1);
        displayDataOnGUI(CAN_ID_18FF5325, DLC_PGN_FF53);
        vSetBlockDelayMilli(1);
        displayDataOnGUI(CAN_ID_18FF5525, DLC_PGN_FF55);    // it sends div stored data on get data request

        /* change status flag */
        setGuiCommStatus(e_noTransOfDataInitial);
    }

    else if (commStatusTemp == e_liveDataStarted)
    {
        liveDataFlag =  e_liveDataStarted;
    }

    else if (commStatusTemp == e_liveDataStopped)
    {
        liveDataFlag = e_liveDataStopped ;
    }

    else if ( commStatusTemp == e_connectWithGUIforCali )
    {
        #if ( CALIBRATION_METHOD == BY_CAN_BUS)

        sendGuiConnectivityResponse();
        if (eIsSystemConnected() == e_SystemConnectedWithGUI && CAN_CalibrationCompleteFlag != true)
        {
            if(e_runStatus != e_calibrationStart)
            {
                e_runStatus = e_calibrationStart ;
                setButtonState(e_buttonNotCickedInitial);
            }
        }

        #endif

        setGuiCommStatus(e_noTransOfDataInitial);
    }

    else if (commStatusTemp == e_oneTimeCalibOff)
    {
       setGuiCommStatus(e_noTransOfDataInitial);
       oneTimeCalibrationResponse();
    }

    else{}
    // *****************************************************************************//

    if (liveDataFlag == e_liveDataStarted)
    {
        fillLiveDataInDLC_PGN_FF51();
        fillLiveDataInDLC_PGN_FF52();
        fillLiveDataInDLC_PGN_FF55();
        displayDataOnGUI(CAN_ID_18FF5125, DLC_PGN_FF51);
        vSetBlockDelayMilli(1);
        displayDataOnGUI(CAN_ID_18FF5225, DLC_PGN_FF52);
        /* Added blocking delay of 1 msec between two can msg as PGN id 18ff5525 is not able to send if not put delay */
        vSetBlockDelayMilli(1);                         
        displayDataOnGUI(CAN_ID_18FF5525, DLC_PGN_FF55);
        //vSetBlockDelayMilli(1);
    }
    else{}
    
}



/****************************************************************************************************************/
/* End of file {file_name}                                                                                      */ 
/****************************************************************************************************************/
