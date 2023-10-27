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

#ifndef __DIAGNOSTICDATA_H__
#define __DIAGNOSTICDATA_H__

/****************************************************************************************************************/
/* File Includes                                                                                                */
/****************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "CustomDataTypes.h"


/****************************************************************************************************************/
/* Constant Defines                                                                                             */
/****************************************************************************************************************/

#define PGN_FF71_getDiagnosticData                  0xFF71u     /* get diagnostic data*/

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
void saveCalibrationTimeStampInFlash(void);
void testData(void);
void setDiagnosticDataRequestFlag(bool setFlag);
bool getDiagnosticDataRequestFlagStatus(void);
void sendDiagnosticDataToGUI(void);
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