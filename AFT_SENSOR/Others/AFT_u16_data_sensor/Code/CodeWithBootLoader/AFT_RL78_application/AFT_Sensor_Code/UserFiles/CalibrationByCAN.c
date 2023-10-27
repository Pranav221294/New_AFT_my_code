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

#include "CalibrationByCAN.h"

/****************************************************************************************************************/
/****************************************************************************************************************/
/* Defines                                                                                                      */
/****************************************************************************************************************/

#define CAN_CALIB_DISCONNECT_TIMEOUT                                1100u               // 1 Sec
#define CAN_ID_18FF5625                                             0x18FF5625ul        // Send response of request recieve from GUI to maintain connectivity with GUI
#define CAN_ID_18FF5925_RESPONSE_OF_DISCONNECT                      0x18FF5925ul
#define CAN_ID_18FF5825_RESPONSE_OF_RESET                           0x18FF5825ul
#define CAN_ID_18FF5725_RESPONSE_DURING_CALIB                       0x18FF5725ul
//#define CAN_ID_18FF6025_RESPONSE_DURING_FULL_CALIB                  0x18FF6025ul

#define MINIMUM_LEVEL_FOR_CALIBRATION                               1100
/****************************************************************************************************************/
/* Enumeration                                                                                                  */
/***************************************************************************************************************/

typedef enum
{
    eInitial,
    eCalibrationDoingCorrectly,
    eCalibrationIsNotDoingCorrectly,
    eMinimumLevelOfFuelRequired,
    eMinimumLevelOfFuelAchieved,
}levelStatus_eType;


/****************************************************************************************************************/
/* Extern variable                                                                                                  */
/***************************************************************************************************************/

extern e_states_type e_runStatus ;
extern s_Parameter_type caliPara_struct;

extern volatile u8 complete_flag ;
extern volatile u32 p0_in ;
extern volatile u32 p1_in ;
extern volatile u32 p2_in  ;
extern Accum_t accu_v;
/****************************************************************************************************************/
/* Type definitions                                                                                             */
/****************************************************************************************************************/

typedef struct 
{
    levelStatus_eType eMinimumLevelStatus;
    levelStatus_eType eWrongCalibrationStatus;
}levelStatus_sType;


/****************************************************************************************************************/
/* Local variables                                                                                              */
/****************************************************************************************************************/

gui_connect_struct_type sGUIconnect;

buttonClick_type eButtonClick = e_buttonNotCickedInitial;

Can_Calibration_State_type eCanCalibState = e_canCalibrationNotStart;

gui_connectivity_type debugStatus = e_SystemNotConnectedWithGUI ; // for debug

bool CAN_CalibrationCompleteFlag = false ;

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
static Can_Calibration_State_type _eCanCalibAtEmptyLevel(void);
static Can_Calibration_State_type _eCanCalibAtFullLevel(void);
static void _vButtonDisconnectResponse(void);
static void _vButtonResetResponse(void);
static void _vResponseDuringCalibration(u8 timeInSeconds, Can_Calibration_State_type calibState);
static void _checkLevelForCalibration(levelStatus_sType *const ptrStatus);
static void _vSendRequiredLevelStatus(levelStatus_sType * const ptrStatus);

//static void _vResponseDuringFullCalibration(u8 timeInSeconds, Can_Calibration_State_type calibState);

/// @} // endgroup ModuleLib  

/// @} // endgroup MODULE_LIBRARY

/****************************************************************************************************************/
/* Local function definitions                                                                                   */
/****************************************************************************************************************/

/*************************************************************************************************************//**
 *
 * Purpose  	:   This function get the value of p0 p1 and p2 at empty level
 *
 * @param[in]	:  	None    
 * @param[out]	:	None
 * @return   	:	Can_Calibration_State_type
 ****************************************************************************************************************/

static Can_Calibration_State_type _eCanCalibAtEmptyLevel(void)
{
    Can_Calibration_State_type Calibstate = e_CanCalibrationEmptyStart ; 
    u32 timeStamp = 0 ; 
    u16 tempPxAcc = 0 ;
    u8 TimeInSeconds = 0 ; 
    timeStamp = u32GetMilTick();

    while(Calibstate != e_CanCalibrationEmptySuccess)
    {
        
        /* Restart watchdog timer*/
		R_WDT_Restart();

        sendGuiConnectivityResponse();
        
        if (eIsSystemConnected() == e_SystemNotConnectedWithGUI )
        {
            Calibstate = e_CanBusDisconnectedDuringEmptyCalib ;
            break;
        }


        if ( getButtonClickedState() == e_buttonClickDisconnect)
        {
            Calibstate = e_ClickedOnDisconnectDuringEmptyCalib ;
            break;
        }

        if ( (u32GetMilTick() - timeStamp) < CALIBRATION_TIME)
		{

			if (complete_flag == 1)
			{
                vEmptyCalibration();
				complete_flag = 0 ;
			}			
			
		}
        else
        {
            Calibstate = e_CanCalibrationEmptySuccess ;
        }

        TimeInSeconds = (u32GetMilTick() - timeStamp ) / 1000u ;
        _vResponseDuringCalibration(TimeInSeconds, e_CanCalibrationEmptyInProcess);
    }

    // if (Calibstate == e_CanCalibrationEmptyStart )
    // {
    //     Calibstate = e_CanCalibrationEmptyFailed ;
    // }

    _vResponseDuringCalibration(TimeInSeconds, Calibstate);

    return Calibstate ;
}



/*************************************************************************************************************//**
 *
 * Purpose  	:   This function get the value of p1 and p2 at full level
 *
 * @param[in]	:  	None    
 * @param[out]	:	None
 * @return   	:	Can_Calibration_State_type
 ****************************************************************************************************************/

static Can_Calibration_State_type _eCanCalibAtFullLevel(void)
{
    Can_Calibration_State_type Calibstate = e_CanCalibrationFullStart ; 
    u32 timeStamp = 0 ; 
    u16 tempPxAcc = 0 ;
    u8 TimeInSeconds = 0 ; 
    timeStamp = u32GetMilTick();

    while(Calibstate != e_CanCalibrationFullSuccess)
    {
        
        /* Restart watchdog timer*/
		R_WDT_Restart();

        sendGuiConnectivityResponse();
        
        if (eIsSystemConnected() == e_SystemNotConnectedWithGUI )
        {
            Calibstate = e_CanBusDisconnectedDuringFullCalib ;
            break;
        }


        if ( getButtonClickedState() == e_buttonClickDisconnect)
        {
            Calibstate = e_ClickedOnDisconnectDuringFullCalib ;
            break;
        }

        if ( (u32GetMilTick() - timeStamp) < CALIBRATION_TIME)
		{

			if (complete_flag == 1)
			{
                vFullCalibration();
				complete_flag = 0 ;
			}			
			
		}
        else
        {
            Calibstate = e_CanCalibrationFullSuccess ;
        }

        TimeInSeconds = (u32GetMilTick() - timeStamp ) / 1000u ;
        _vResponseDuringCalibration(TimeInSeconds, e_CanCalibrationFullInProcess);
    }

    // if (Calibstate == e_CanCalibrationFullStart )
    // {
    //     Calibstate = e_CanCalibrationFullFailed ;
    // }

    _vResponseDuringCalibration(TimeInSeconds, Calibstate);

    return Calibstate ;
}


/*************************************************************************************************************//**
 *
 * Purpose  	:   This function send the response when disconnect button clicked on GUI
 *
 * @param[in]	:  	None    
 * @param[out]	:	None
 * @return   	:	None
 ****************************************************************************************************************/
static void _vButtonDisconnectResponse(void)
{
    u8 DLC_data[8] = {0};
    if(bCheckCanTxBufferAvailable() == TRUE)
    {
        vTxMsgBufProcessing(CAN_ID_18FF5925_RESPONSE_OF_DISCONNECT,8u,DLC_data,J1939_TX);
    }
}


/*************************************************************************************************************//**
 *
 * Purpose  	:   This function send the response when reset button clicked on GUI. This functionality not implemented yet in GUI
 *
 * @param[in]	:  	None    
 * @param[out]	:	None
 * @return   	:	None
 ****************************************************************************************************************/
static void _vButtonResetResponse(void)
{
    u8 DLC_data[8] = {0};
    if(bCheckCanTxBufferAvailable() == TRUE)
    {
        vTxMsgBufProcessing(CAN_ID_18FF5825_RESPONSE_OF_RESET,8u,DLC_data,J1939_TX);
    }
}


/*************************************************************************************************************//**
 *
 * Purpose  	:   This function send the information of time of calibration and the state of calibration 
 *
 * @param[in]	:  	timeInSeconds,    Can_Calibration_State_type 
 * @param[out]	:	None
 * @return   	:	None
 ****************************************************************************************************************/
static void _vResponseDuringCalibration(u8 timeInSeconds, Can_Calibration_State_type calibState)
{
    u8 DLC[8] = {0};
    static s8 lastTime = -1 ; 
    static Can_Calibration_State_type lastCaliState = e_CanCalibrationEmptyInProcess ;

    if ((lastTime != timeInSeconds) || ( lastCaliState != calibState ))
    {
        DLC[0] = timeInSeconds ;
        DLC[1] = calibState ;
        if(bCheckCanTxBufferAvailable() == TRUE)
        {
            vTxMsgBufProcessing(CAN_ID_18FF5725_RESPONSE_DURING_CALIB,8u,DLC,J1939_TX);
        }

        lastCaliState = calibState;
        lastTime = timeInSeconds ;
    }
    
}



/*************************************************************************************************************//**
 *
 * Purpose  	:   This function send the status of required fuel and direction of fuel level for calibration
 *
 * @param[in]	:  	levelStatus_sType
 * @param[out]	:	None
 * @return   	:	None
 ****************************************************************************************************************/
static void _vSendRequiredLevelStatus(levelStatus_sType * const ptrStatus)
{
    u8 DLC[8] = {0};
    static levelStatus_sType lastLevelState = {eInitial} ;

    if ((lastLevelState.eMinimumLevelStatus != ptrStatus->eMinimumLevelStatus) || ( lastLevelState.eWrongCalibrationStatus != ptrStatus->eWrongCalibrationStatus ))
    {
        DLC[2] = ptrStatus->eMinimumLevelStatus ;
        DLC[3] = ptrStatus->eWrongCalibrationStatus ;
        if(bCheckCanTxBufferAvailable() == TRUE)
        {
            vTxMsgBufProcessing(CAN_ID_18FF5725_RESPONSE_DURING_CALIB,8u,DLC,J1939_TX);
        }

        lastLevelState.eMinimumLevelStatus = ptrStatus->eMinimumLevelStatus;
        lastLevelState.eWrongCalibrationStatus = ptrStatus->eWrongCalibrationStatus ;
    }
    
}


/*************************************************************************************************************//**
 *
 * Purpose  	:   This function check the required level for calibration
 *
 * @param[in]	:  	None    
 * @param[out]	:	None
 * @return   	:	Can_Calibration_State_type
 ****************************************************************************************************************/

static void _checkLevelForCalibration(levelStatus_sType *const ptrStatus)
{
    int16_t levelDiif = 0 ;

    /* get data of p1 and p2 */
    if (complete_flag == 1)
    {
            vFullCalibration();
            complete_flag = 0 ;
        

        /* check level diffrence */
        levelDiif = caliPara_struct.p1_full - caliPara_struct.p1_empty ;

        /*  checking that calibration is doing in correct direction means low calibration 
            should be done at low level and full calibration at high level , not opposite 
        */
        if ( levelDiif > -4 )
        {
            /* if difference is positive then calibration is being done correctly */
            ptrStatus->eWrongCalibrationStatus = eCalibrationDoingCorrectly ;

            /* Here checked minimum level of fuel achived or not */
            if (levelDiif > MINIMUM_LEVEL_FOR_CALIBRATION)
            {
                ptrStatus->eMinimumLevelStatus = eMinimumLevelOfFuelAchieved ;
            }
            else
            {
                ptrStatus->eMinimumLevelStatus = eMinimumLevelOfFuelRequired ;
            }

            
        }

        /* calibration is being done in opposite direction i.e. wrongly */ 
        else if ( levelDiif < (-10))
        {
            ptrStatus->eWrongCalibrationStatus = eCalibrationIsNotDoingCorrectly;
        }

        else
        {}

        /* send status of level */
        _vSendRequiredLevelStatus(ptrStatus);

    }

}

/****************************************************************************************************************/
/* Global function definitions                                                                                   */
/****************************************************************************************************************/

//*************************************************************************************************************//**
 /*
 * Purpose  	:   this function handles the calibration by CAN
 *
 * @param[in]   :	None
 * @return    	: 	Can_Calibration_State_type
 ****************************************************************************************************************/

Can_Calibration_State_type calibrationStartWithCAN(void)
{
    Can_Calibration_State_type calibState = e_canCalibrationNotStart ;
    buttonClick_type tempButtonState = e_buttonNotCickedInitial;
    levelStatus_sType levelStatus = {eInitial} ;
    bool readyToCalibAtFullLevelFlag = false ;
    /* initialize variables */

    static u32 timeOutTimeStamp = 0 ; 
    levelStatus.eMinimumLevelStatus = eInitial ;
    levelStatus.eWrongCalibrationStatus = eInitial ;
    if (timeOutTimeStamp == 0 )
    {
        timeOutTimeStamp = u32GetMilTick();
    }

    /* Set output voltage 0v when entered in calibration mode */
    vSetOutputVoltage(0);

    saveCalibrationTimeStampInFlash();

    while (1)
    {
        tempButtonState = getButtonClickedState();

        /* send response to GUI continuesly */
        sendGuiConnectivityResponse();

        /* Restart watchdog timer*/
		R_WDT_Restart();

        /* if CAN bus disconnected jump to main App  */
        if (eIsSystemConnected() == e_SystemNotConnectedWithGUI)
        {
            calibState = e_CanBusDisconnected ;
            //vResetSystemByWatchDog();

            break;
        }
        
        /* if clicked on disconnected jump to main app */
        if (tempButtonState == e_buttonClickDisconnect)
        {
            calibState = e_ClickedOnDisconnect ;
            _vButtonDisconnectResponse();
            //vResetSystemByWatchDog();
            break;
        }

        /* If clicked on calibration start at empty level */
        if (tempButtonState == e_buttonClickEmptyCalStart)
        {
            calibState = _eCanCalibAtEmptyLevel();
            setButtonState(e_buttonNotCickedInitial);
            if (calibState != e_CanCalibrationEmptySuccess)
            {
                break;
            }

            else
            {
                /* set output voltage 500mV when calibration at empty success */
                vSetOutputVoltage(500);
            }
        }

        /* Monitor level after doing calibration at empty level */
        if ( calibState == e_CanCalibrationEmptySuccess)
        {
            _checkLevelForCalibration(&levelStatus);
            if ( levelStatus.eMinimumLevelStatus == eMinimumLevelOfFuelAchieved && levelStatus.eWrongCalibrationStatus == eCalibrationDoingCorrectly )
            {
                readyToCalibAtFullLevelFlag = true ;
            }
            else
            {
                readyToCalibAtFullLevelFlag = false ;
            }
        }

        /* If clicked on calibration start at full level */
        if (tempButtonState == e_buttonClickFullCalStart && readyToCalibAtFullLevelFlag == true)
        {
            calibState = _eCanCalibAtFullLevel();
            setButtonState(e_buttonNotCickedInitial);
            if (calibState != e_CanCalibrationFullSuccess)
            {
                break;
            }

            /* if calibrated successfully save the data and run the main app */
            else
            {
                calibState = e_CancalibrationSuccess ;
                calculateParamAndSaveInFlash();
                CAN_CalibrationCompleteFlag = true ;
                break;
            }
        }

        // if (tempButtonState == e_buttonClickResetCalibration)
        // {
        //     _vButtonResetResponse();
        //     break;
        // }

    }

    // if (calibState != e_CancalibrationSuccess)
    // {
    //     e_runStatus = e_mainAppStart;
    // }
    
    return calibState ;
}


/*************************************************************************************************************//**
 *
 * Purpose  	:   This function set the button state
 *
 * @param[in]	:  	buttonClick_type    
 * @param[out]	:	None
 * @return   	:	None
 ****************************************************************************************************************/
void setButtonState(buttonClick_type state)
{
    eButtonClick = state ;
}


/*************************************************************************************************************//**
 *
 * Purpose  	:   this function return the button state that is clicked on GUI
 *
 * @param[in]	:  	None    
 * @param[out]	:	None
 * @return   	:	buttonClick_type
 ****************************************************************************************************************/
buttonClick_type getButtonClickedState(void)
{
    return eButtonClick ;
}

//*************************************************************************************************************//**
 /*
 * Purpose  	:  
 *
 * @param[in]   :	None
 * @return    	: 	None
 ****************************************************************************************************************/
u32 lastTime = 0 ; 
u32 debugTime = 0 ; 
 void sendGuiConnectivityResponse(void)
{
    u8 DLC_FF56[8] = {0};
    //static u32 lastGuiConnectStamp = 0 ; 
    #if 0
    static u8 flag = 0 ; 

    if (sGUIconnect.dataReady == true)
    {
        sGUIconnect.dataReady = false;

        if(bCheckCanTxBufferAvailable() == TRUE)
        {
            vTxMsgBufProcessing(CAN_ID_18FF5625,8u,DLC_FF56,J1939_TX);
        }

        flag++;
    }

    if ((u32GetMilTick() - sGUIconnect.CANTimeStamp) < (CAN_CALIB_DISCONNECT_TIMEOUT + 10u ))
    {
        if (flag == 2)
        {
            sGUIconnect.eGUIConnectivity = e_SystemConnectedWithGUI;
            flag = 1 ;
        }
        
    }
    else
    {
        flag = 0 ; 
        sGUIconnect.eGUIConnectivity = e_SystemNotConnectedWithGUI;
    }

    #endif

    static u8 index = 0 ;
    static u32 prevTime = 0 ; 
    static u32 currentTime = 0 ;
    //static u32 lastTime = 0 ; 
    u32 timeDiff = 0 ; 
    // if (prevTime == 0 )
    // {
    //     prevTime = u32GetMilTick();
    // }

    if (sGUIconnect.dataReady == true)
    {
        sGUIconnect.dataReady = false ;
        if(bCheckCanTxBufferAvailable() == TRUE)
        {
            vTxMsgBufProcessing(CAN_ID_18FF5625,8u,DLC_FF56,J1939_TX);
        }

        if (index == 0 )
        {
            prevTime = sGUIconnect.CANTimeStamp ;
            index = 1 ;
        }

        else if (index == 1)
        {
            currentTime = sGUIconnect.CANTimeStamp ;
            index = 2 ;
        }

        lastTime = u32GetMilTick();
    }

    if (index == 2)
    {
        timeDiff = currentTime - prevTime ;
        if (timeDiff < (CAN_CALIB_DISCONNECT_TIMEOUT + 5u ))
        {
            sGUIconnect.eGUIConnectivity = e_SystemConnectedWithGUI ;
            prevTime = currentTime ;
            index = 1 ;
        }
        else
        {
            sGUIconnect.eGUIConnectivity = e_SystemNotConnectedWithGUI ;
            index = 0 ;
        }
    }

    debugTime = u32GetMilTick() - lastTime;
    if ((u32GetMilTick() - lastTime) > (CAN_CALIB_DISCONNECT_TIMEOUT + 5u))
    {
        sGUIconnect.eGUIConnectivity = e_SystemNotConnectedWithGUI ;
        //index = 0 ;
    }

}


//*************************************************************************************************************//**
 /*
 * Purpose  	:  
 *
 * @param[in]   :	None
 * @return    	: 	None
 ****************************************************************************************************************/
gui_connectivity_type eIsSystemConnected(void)
{
    return sGUIconnect.eGUIConnectivity ;
}


//*************************************************************************************************************//**
 /*
 * Purpose  	:  
 *
 * @param[in]   :	None
 * @return    	: 	None
 ****************************************************************************************************************/




/****************************************************************************************************************/
/* End of file {file_name}                                                                                      */ 
/****************************************************************************************************************/
