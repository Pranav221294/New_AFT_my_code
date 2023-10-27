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

#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

/****************************************************************************************************************/
/* File Includes                                                                                                */
/****************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "CustomDataTypes.h"

/****************************************************************************************************************/
/* Constant Defines                                                                                             */
/****************************************************************************************************************/

/************************************* Configurable parameters **************************************************/



/* Debounce time on calibration pin */
#define DEBOUNCE_TIME					            1000u				// 1000 miliseconds

/* Below Macro is used to define the timing for calibration */
#define CALIBRATION_TIME					        10000ul					// 10 Seconds




/******************************************* SET CALIBRATION METHOD ***************************/

/*Assign one of the below macros to CALIBRATION_METHOD*/
#define BY_CALIBRATION_PIN_IN_HARDWARE				0u
#define BY_CAN_BUS									1u

#define CALIBRATION_METHOD							BY_CALIBRATION_PIN_IN_HARDWARE

/**********************************************************************************************/



/* if below macro (ONE_TIME_CALIBRATION_ON) is -
OFF 	= System will go in calibration mode every time if we connect calibration pin to Ground
ON		= Sytem will go in calibration mode on first time calibration , after successfull calibration 
		  system will never go into calibration mode, to calibrate again need to erase flash data first
          And this is applicable only if calibration method is BY_CALIBRATION_PIN_IN_HARDWARE
*/
#if (CALIBRATION_METHOD == BY_CALIBRATION_PIN_IN_HARDWARE)

    #define ONE_TIME_CALIBRATION				        OFF				// Write ON or OFF

#endif

#if (CALIBRATION_METHOD == BY_CALIBRATION_PIN_IN_HARDWARE)

    #define STORE_CALIBRATION_METHOD_IN_FLASH               11u

#elif ( CALIBRATION_METHOD == BY_CAN_BUS)

    #define STORE_CALIBRATION_METHOD_IN_FLASH               22u

#endif


/*--------------------------------------------------------------------------------------------------------------*/


/************************************ Software Version ***********************************************************/

/* Major version numbers change whenever there is some significant change being introduced. */
#define MAJOR_VERSION                                           1u

/* Minor version numbers change when a new, minor feature is introduced or when a set of smaller features is rolled out. */
#define MINOR_VERSION                                           1u

/* This is normally for small bug-fixes or the like. */
#define PATCH_VERSION                                           1u

/*_________________________________<<<<<<<<<< End >>>>>>>>>>________________________________________________________*/

/****************************************************************************************************************/
/* Enumeration types                                                                                            */
/****************************************************************************************************************/


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

/*//////////////////////////////////////<<<<<<<>>>>>>>\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\


\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\<<<<<<<>>>>>>>///////////////////////////////////////////*/


#endif 