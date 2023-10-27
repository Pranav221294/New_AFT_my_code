/** ###################################################################
 **     Filename  : TellTales.h
 **     Project   : Y1ClusterDiagnostics
 **     Processor : MC9S12XHZ256VAG
 **     Compiler  : CodeWarrior HCS12X C Compiler
 **     Date/Time : 10/4/2016, 6:03 PM
 **     Contents  :
 **         User source code
 **
 ** ###################################################################*/

#ifndef __TellTales_H
#define __TellTales_H
#include "customTypedef.h"



// Global prototypes ---------------------------------------------------------
typedef enum
{                       // bit data
	eLedOff = 0,       	// 000
	eLedOn,            	// 001
	eLampSingleBlink,   	// 100
	eLampDoubleBlink,
};

#define LOW_DEF_WARNING_LED				P9.0
#define LOW_DEF_WARNING_LED_ON			(LOW_DEF_WARNING_LED = 0x01)
#define LOW_DEF_WARNING_LED_OFF			(LOW_DEF_WARNING_LED = 0x00)

/// @addtogroup TELL_TALE_LIBRARY
/// @{
 
/// @defgroup TellTaleHeaderLib Global functions used in Tell tale
//// 
///   
/// @addtogroup TellTaleHeaderLib
/// @{
void vWakeupStatePinProcess			(void);
void vLedPrevStateInit				(void);
void vControlDefTellTales			(void);
void vAllLampsOff					(void);
void vAllLampsOn					(void);
void vFeedbackwhileSelfTesting		();

/// @} // endgroup TellTaleHeaderLib

/// @} // endgroup TELL_TALE_LIBRARY

/* END TellTales */

#endif

