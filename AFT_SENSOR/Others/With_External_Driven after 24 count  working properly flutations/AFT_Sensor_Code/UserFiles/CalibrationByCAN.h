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

#ifndef __CALIBRATIONBYCAN_H__
#define __CALIBRATIONBYCAN_H__

/****************************************************************************************************************/
/* File Includes                                                                                                */
/****************************************************************************************************************/

#include "r_cg_macrodriver.h"
#include "CustomDataTypes.h"
#include "User.h"
#include "GuiCommProtocol.h"
#include "Calibration.h"
#include "Can.h"
#include "r_cg_wdt.h"
#include "levelCalc.h"
#include "DiagnosticData.h"

/****************************************************************************************************************/
/* Constant Defines                                                                                             */
/****************************************************************************************************************/


/****************************************************************************************************************/
/* Enumeration types                                                                                            */
/****************************************************************************************************************/

typedef enum
{
    e_SystemNotConnectedWithGUI,
    e_SystemConnectedWithGUI,
}gui_connectivity_type;

typedef struct
{
    bool dataReady;
    u32 CANTimeStamp;
    gui_connectivity_type eGUIConnectivity;
}gui_connect_struct_type;


typedef enum
{
    e_buttonNotCickedInitial,
    e_buttonClickDisconnect,
    e_buttonClickEmptyCalStart,
    e_buttonClickFullCalStart,
    e_buttonClickResetCalibration,
    e_buttonClickConnect,
}buttonClick_type;


typedef enum
{
    e_canCalibrationNotStart,
    e_CanCalibrationEmptyStart,
    e_CanCalibrationEmptySuccess,
    e_CanCalibrationEmptyFailed,
    e_CanCalibrationEmptyInProcess,
    e_CanCalibrationFullStart,
    e_CanCalibrationFullSuccess,
    e_CanCalibrationFullFailed,
    e_CanCalibrationFullInProcess,
    e_CancalibrationSuccess,
    e_CancalibrationFailed,
    e_CanCalibrationTimeOut,
    e_CanBusDisconnected,
    e_ClickedOnDisconnect,
    e_ClickedOnDisconnectDuringEmptyCalib,
    e_ClickedOnDisconnectDuringFullCalib,
    e_CanBusDisconnectedDuringEmptyCalib,
    e_CanBusDisconnectedDuringFullCalib,
}Can_Calibration_State_type;

/****************************************************************************************************************/
/* Type definitions                                                                                             */
/****************************************************************************************************************/


/****************************************************************************************************************/
/* Global function declarations                                                                                 */
/****************************************************************************************************************/

/// @addtogroup FILE_LIBRARY
/// @{
/// @defgroup FileLib  functions used in main file 
//// 
///   
/// @addtogroup FileLib
/// @{

// Global function declarations   ------------------------------------------------------

Can_Calibration_State_type calibrationStartWithCAN(void);
void sendGuiConnectivityResponse(void);
gui_connectivity_type eIsSystemConnected(void);
void setButtonState(buttonClick_type state);
buttonClick_type getButtonClickedState(void);

/// @} // endgroup FileLib

/// @} // endgroup FILE_LIBRARY


/****************************************************************************************************************/
/* Inline functions                                                                                             */
/****************************************************************************************************************/

/// @addtogroup FILE_INLINE_LIBRARY
/// @{
/// @defgroup FileInlineLib  functions used in main file 
//// 
///   
/// @addtogroup FileInlineLib  
/// @{

// Inline functions declarations   ------------------------------------------------------




/// @} // endgroup FileInlineLib  

/// @} // endgroup FILE_INLINE_LIBRARY


#endif 