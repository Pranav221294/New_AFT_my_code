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

#include "DiagnosticData.h"
#include "J1939.h"
#include "ReadWriteParamFlash.h"

/****************************************************************************************************************/
/****************************************************************************************************************/
/* Defines                                                                                                      */
/****************************************************************************************************************/
#define CAN_ID_18FF7025             0x18FF7025u         // calibration time stamp
#define CAN_ID_18FF7225             0x18FF7225u         // send miscellaneous data 1
#define CAN_ID_18FF7325             0x18FF7325u         // send miscellaneous data 2

#define TEST_CAN_ID                 0x18FF9035u
u8 testDataDLC[8] = {0} ;
/****************************************************************************************************************/
/* Enumeration                                                                                                  */
/***************************************************************************************************************/


/****************************************************************************************************************/
/* Extern variable                                                                                                  */
/***************************************************************************************************************/

extern PGN_DATA_TYPE sPGNFF70_timeStamp ;
/****************************************************************************************************************/
/* Type definitions                                                                                             */
/****************************************************************************************************************/


/****************************************************************************************************************/
/* Local variables                                                                                              */
/****************************************************************************************************************/
static bool diagnosticDataRequestFlag = false ;
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
static void _sendTimeStamp(void);
static void _sendMiscellaneousData1(void);
static void _sendMiscellaneousData2(void);

/// @} // endgroup ModuleLib  

/// @} // endgroup MODULE_LIBRARY

/****************************************************************************************************************/
/* Local function definitions                                                                                   */
/****************************************************************************************************************/

/*************************************************************************************************************//**
 *
 * Purpose  	:   Send time stamp
 *
 * @param[in]	:  	None    
 * @param[out]	:	None
 * @return   	:	None
 ****************************************************************************************************************/

static void _sendTimeStamp(void)
{
    u8 dataTimeStamp[8] = {0};

    dataTimeStamp[eData_D1] = 0 ;
    dataTimeStamp[eData_D2] = vRead_CalibTimeStampHour_fromFlash() ;
    dataTimeStamp[eData_D3] = vRead_CalibTimeStampMinutes_fromFlash() ;
    dataTimeStamp[eData_D4] = vRead_CalibTimeStampSeconds_fromFlash() ;
    dataTimeStamp[eData_D5] = vRead_CalibTimeStampAMorPM_fromFlash() ;
    dataTimeStamp[eData_D6] = vRead_CalibTimeStampDay_fromFlash() ;
    dataTimeStamp[eData_D7] = vRead_CalibTimeStampMonths_fromFlash() ;
    dataTimeStamp[eData_D8] = vRead_CalibTimeStampYear_fromFlash() ;

    if(bCheckCanTxBufferAvailable() == TRUE)
    {
        vTxMsgBufProcessing(CAN_ID_18FF7025,8u,dataTimeStamp,J1939_TX);
    }
}



/*************************************************************************************************************//**
 *
 * Purpose  	:   Send time stamp
 *
 * @param[in]	:  	None    
 * @param[out]	:	None
 * @return   	:	None
 ****************************************************************************************************************/

static void _sendMiscellaneousData1(void)
{
    u8 dataDLC[8] = {0};
    u32 temp = 0 ;
    temp = vRead_NumberOfTimeCalibration_fromFlash();

    /* Software version */
    dataDLC[0] = MAJOR_VERSION ;
    dataDLC[1] = MINOR_VERSION ;
    dataDLC[2] = PATCH_VERSION ;
    dataDLC[3] = 0 ;

    /* No of time calibration attempted by calibration pin */ 
    dataDLC[4] = (u8)( temp >> 24u ) ;
    dataDLC[5] = (u8)( temp >> 16u ) ;
    dataDLC[6] = (u8)( temp >> 8u  ) ;
    dataDLC[7] = (u8)( temp >> 0u  ) ;


    if(bCheckCanTxBufferAvailable() == TRUE)
    {
        vTxMsgBufProcessing(CAN_ID_18FF7225,8u,dataDLC,J1939_TX);
    }

}

/*************************************************************************************************************//**
 *
 * Purpose  	:   Send time stamp
 *
 * @param[in]	:  	None    
 * @param[out]	:	None
 * @return   	:	None
 ****************************************************************************************************************/

static void _sendMiscellaneousData2(void)
{
    u8 dataDLC[8] = {0};

    /* Read Calibration method used */
    dataDLC[0] = vRead_CalibrationMethod_fromFlash(); 

    /* Read last calibration state */
    dataDLC[1] = vRead_CalibrationLastState_fromFlash();


    if(bCheckCanTxBufferAvailable() == TRUE)
    {
        vTxMsgBufProcessing(CAN_ID_18FF7325,8u,dataDLC,J1939_TX);
    }

}


/****************************************************************************************************************/
/* Global function definitions                                                                                   */
/****************************************************************************************************************/

//*************************************************************************************************************//**
 /*
 * Purpose  	:   get time stamp of calibration by GUI and save it in the flash 
 *
 * @param[in]   :	None
 * @return    	: 	None
 ****************************************************************************************************************/

void saveCalibrationTimeStampInFlash(void)
{
    u8 temp = 0 ;
    u8 DLC_data[8] = {0};
    static bool flag = false ;
    temp = extractByteFromPGN(&sPGNFF70_timeStamp, eData_D1);
    if (temp == 0xAA && flag == false)
    {   
        flag = true ;
        DLC_data[eData_D1] = 0x55;

        temp = extractByteFromPGN(&sPGNFF70_timeStamp, eData_D2);
        DLC_data[eData_D2] = temp ;
        vSave_CalibTimeStampHour_inFlash(temp);

        temp = extractByteFromPGN(&sPGNFF70_timeStamp, eData_D3);
        DLC_data[eData_D3] = temp ;
        vSave_CalibTimeStampMinutes_inFlash(temp);

        temp = extractByteFromPGN(&sPGNFF70_timeStamp, eData_D4);
        DLC_data[eData_D4] = temp ;
        vSave_CalibTimeStampSeconds_inFlash(temp);

        temp = extractByteFromPGN(&sPGNFF70_timeStamp, eData_D5);
        DLC_data[eData_D5] = temp ;
        vSave_CalibTimeStampAMorPM_inFlash(temp);

        temp = extractByteFromPGN(&sPGNFF70_timeStamp, eData_D6);
        DLC_data[eData_D6] = temp ;
        vSave_CalibTimeStampDay_inFlash(temp);

        temp = extractByteFromPGN(&sPGNFF70_timeStamp, eData_D7);
        DLC_data[eData_D7] = temp ;
        vSave_CalibTimeStampMonths_inFlash(temp);

        temp = extractByteFromPGN(&sPGNFF70_timeStamp, eData_D8);
        DLC_data[eData_D8] = temp ;
        vSave_CalibTimeStampYear_inFlash(temp);

        if(bCheckCanTxBufferAvailable() == TRUE)
        {
            vTxMsgBufProcessing(CAN_ID_18FF7025,8u,DLC_data,J1939_TX);
        }
    }

}


//*************************************************************************************************************//**
 /*
 * Purpose  	:   Send Diagnostic data Requested by GUI
 *
 * @param[in]   :	None
 * @return    	: 	None
 ****************************************************************************************************************/

void sendDiagnosticDataToGUI(void)
{
    if (getDiagnosticDataRequestFlagStatus() == true)
    {
        _sendTimeStamp();
        _sendMiscellaneousData1();
        _sendMiscellaneousData2();
        
        setDiagnosticDataRequestFlag(false);
    }

}


//*************************************************************************************************************//**
 /*
 * Purpose  	:   Set diagnostic data flag
 *
 * @param[in]   :	None
 * @return    	: 	None
 ****************************************************************************************************************/

void setDiagnosticDataRequestFlag(bool setFlag)
{
    diagnosticDataRequestFlag = setFlag ;
}

//*************************************************************************************************************//**
 /*
 * Purpose  	:   Get status of diagnostic data flag
 *
 * @param[in]   :	None
 * @return    	: 	None
 ****************************************************************************************************************/

bool getDiagnosticDataRequestFlagStatus(void)
{
    return diagnosticDataRequestFlag ;
}

void testData(void)
{
    testDataDLC[0] = vRead_CalibrationMethod_fromFlash() ;
    testDataDLC[1] = vRead_CalibTimeStampHour_fromFlash() ;
    testDataDLC[2] = vRead_CalibTimeStampMinutes_fromFlash() ;
    testDataDLC[3] = vRead_CalibTimeStampSeconds_fromFlash() ;
    testDataDLC[4] = vRead_CalibTimeStampDay_fromFlash() ;
    testDataDLC[5] = vRead_CalibTimeStampMonths_fromFlash() ;
    testDataDLC[6] = vRead_CalibTimeStampYear_fromFlash() ;
    testDataDLC[7] = vRead_CalibTimeStampAMorPM_fromFlash() ;
    
    //calibrationMode = 20u ;
    if(bCheckCanTxBufferAvailable() == TRUE)
    {
        vTxMsgBufProcessing(TEST_CAN_ID,8u,testDataDLC,J1939_TX);
    }
}

/****************************************************************************************************************/
/* End of file {file_name}                                                                                      */ 
/****************************************************************************************************************/
