
#ifndef __FaultDiagnostic_H
#define __FaultDiagnostic_H



// Global prototypes --------------------------------------------------------

/// @addtogroup FAULT_DIAGNOSTIC_LIBRARY
/// @{
/// @defgroup FaultDiagnosticLib  functions used 
//// 
///   
/// @addtogroup FaultDiagnosticHeaderLib
/// @{

	void vTx_Critical_Internally_Data();

/// @} // endgroup FaultDiagnosticHeaderLib

/// @} // endgroup FAULT_DIAGNOSTIC_LIBRARY


extern u32 u32GetClockTicks(void);
extern u16 u16GetClockTicks(void);
extern u32 u32GetFreqCaptureValue(void);

#endif

