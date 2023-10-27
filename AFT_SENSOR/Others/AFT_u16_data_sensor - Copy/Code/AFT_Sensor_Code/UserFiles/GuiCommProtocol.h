

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

#ifndef __GUICOMMPROTOCOL_H__
#define __GUICOMMPROTOCOL_H__

/****************************************************************************************************************/
/* File Includes                                                                                                */
/****************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "CustomDataTypes.h"
#include "ReadWriteParamFlash.h"
#include "Can.h"
#include "User.h"
#include "CalibrationByCAN.h"

/****************************************************************************************************************/
/* Constant Defines                                                                                             */
/****************************************************************************************************************/


/* Received from GUI */
#define PGN_FF61_startLiveData                      0xFF61u     /* To get live data on GUI from the sensor */
#define PGN_FF62_stopLiveData                       0xFF62u     /* To stop getting live data from sensor */
#define PGN_FF63_getStoredData                      0xFF63u     /* To get stored data from sensor */

#define PGN_FF64_ConnectWithGUI                     0xFF64u     /* To Stablish connection with sensor while calibrating via CAN */
#define PGN_FF65_StartCaliEmpty                     0xFF65u     /* To start calibration at Full */
#define PGN_FF66_StartCaliFull                      0xFF66u     /* To start calibration at Full */
#define PGN_FF67_CalibrationReset                   0xFF67u     /* To Reset the calibration process */
#define PGN_FF68_DisconnectWithGUI                  0xFF68u     /* To disconnect system from GUI */

/* same pgn for Request and Response */
#define PGN_FF69_OneTimeCalibrationOff              0xFF69u     /* To off one time calibration */
#define PGN_FF70_getCalibTimeStamp                  0xFF70u     /* get time stamp of calibration time*/

/****************************************************************************************************************/
/* Enumeration types                                                                                            */
/****************************************************************************************************************/

/* State of communication with GUI */
typedef enum commState
{
    e_noTransOfDataInitial = 0 ,
    e_liveDataStarted,
    e_liveDataStopped,
    e_getStoredData,
    e_connectWithGUIforCali,
    e_oneTimeCalibOff,             
}guiCommState_type;



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

guiCommState_type getGuiCommStatus(void);
void setGuiCommStatus(guiCommState_type state);
void GuiCommManager(void);

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




