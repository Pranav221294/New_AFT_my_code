
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
/* Date - 19/07/2022      | Revised By  | Rev. No.  | Description                                                           */
/*--------------------------------------------------------------------------------------------------------------*/
/*                                                                                                              */
/****************************************************************************************************************/

#ifndef __USER_H__
#define __USER_H__

/****************************************************************************************************************/
/* File Includes                                                                                                */
/****************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "CustomDataTypes.h"
#include "Configuration.h"

/****************************************************************************************************************/
/* Constant Defines                                                                                             */
/****************************************************************************************************************/

/* Pins */


/* Commenting below macro will swap the probes */
#define SWAP_PINS

#ifndef SWAP_PINS
	#define ENREF_P2_SEL				            P9.3			// CN1	= P1 probe
	#define ENPROBE_P1_SEL				            P7.3			// CN2	= P2 probe
#else
	#define ENREF_P2_SEL				            P7.3			// CN2	= P2 probe
	#define ENPROBE_P1_SEL				            P9.3			// CN1	= P1 probe
#endif




#define CALIBRATION_IN				P6.0

/* Unused Pins which need to be set as zero to reduce the power consumption */
 #define UNUSED_PIN_P10				P1.0			/* MCU pin number 15 */
 #define UNUSED_PIN_P11				P1.1			/* MCU pin number 16 */
 #define UNUSED_PIN_P12				P1.2			/* MCU pin number 17 */
 #define UNUSED_PIN_P13				P1.3			/* MCU pin number 20 */
 #define UNUSED_PIN_P14				P1.4			/* MCU pin number 21 */
 #define UNUSED_PIN_P20				P2.0			/* MCU pin number 4 */
 #define UNUSED_PIN_P21				P2.1			/* MCU pin number 3 */
 #define UNUSED_PIN_P22				P2.2			/* MCU pin number 2 */
 #define UNUSED_PIN_P23				P2.3			/* MCU pin number 1 */
 #define UNUSED_PIN_P27				P2.7			/* MCU pin number 48 */
 #define UNUSED_PIN_P30				P3.0			/* MCU pin number 26 */
 #define UNUSED_PIN_P31				P3.1			/* MCU pin number 25 */
 #define UNUSED_PIN_P33				P3.3			/* MCU pin number 24 */
 #define UNUSED_PIN_P54				P5.4			/* MCU pin number 30 */
 #define UNUSED_PIN_P55				P5.5			/* MCU pin number 29 */
 #define UNUSED_PIN_P56				P5.6			/* MCU pin number 28 */
 #define UNUSED_PIN_P57				P5.7			/* MCU pin number 27 */
 #define UNUSED_PIN_P61				P6.1			/* MCU pin number 14 */
 #define UNUSED_PIN_P72				P7.2			/* MCU pin number 31 */
 #define UNUSED_PIN_P74				P7.4			/* MCU pin number 19 */
 #define UNUSED_PIN_P75				P7.5			/* MCU pin number 18 */
 #define UNUSED_PIN_P80				P8.0			/* MCU pin number 47 */
 #define UNUSED_PIN_P81				P8.1			/* MCU pin number 46 */
 #define UNUSED_PIN_P82				P8.2			/* MCU pin number 45 */
 #define UNUSED_PIN_P83				P8.3			/* MCU pin number 44 */
 #define UNUSED_PIN_P90				P9.0			/* MCU pin number 41 */
 #define UNUSED_PIN_P91				P9.1			/* MCU pin number 40 */								 


// This value set after increasing the value of TDR25 and find out at which value Vout is maximum
// Note -> we found one thing to generate the 100% duty cycle TDR25 value should be >=800
// but after increasing duty cycle more than 90.25%(TDR25 = 726) Vout doesnt change, So thats why we set the value of below macro 
// accordingly

/*
At 20KHz pwm freq TDR25 value should be 800 for 100% duty cycle
at 19.531KHZ freq TDR25 value should be 819(~820) for 100% duty cycle, Existing PCB is creating 19.531KHz 
*/
#define MAX_VAL_TDR25_FOR_100_DUTY								819ul//711UL		// for 19.531kHz PWM Freq
#define MAX_VAL_TDR25_FOR_50_DUTY								(MAX_VAL_TDR25_FOR_100_DUTY / 2u )


//#define MAX_OUT_VOL_AT_100_DUTY									9000UL		// at connector out PIN, 10000 means 10V
#define MAX_OUT_VOL_AT_50_DUTY									5060UL		// at connector out PIN, 10000 means 10V
//#define MAX_OUT_VOLTAGE											MAX_OUT_VOL_AT_100_DUTY


/* set the below 2 parameters to define the upper limit and lower limit of voltage */
#define SET_MAX_OUT_VOLATEG_AT_100_DUTY_CYCLE					5000UL		// 10Volt mean 10000, 5volt means 5000
#define SET_MIN_OUT_VOLATEG_AT_0_DUTY_CYCLE						500UL		// 0.5v means 500

#define OUT_VOLTAGE_RANGE										(SET_MAX_OUT_VOLATEG_AT_100_DUTY_CYCLE -  SET_MIN_OUT_VOLATEG_AT_0_DUTY_CYCLE)

#define MAX_COUNT_TO_SET_100_DUTY								1000UL

#define MAX_VALUE_AT_FULL_LEVEL									10000u		// value of Q1



/* To increase the number of counts we are accumulating those counts, So below macro defines that how many times counts will accumulate*/
#define ACCUMULATE                                  5u              // don't change it unless required 



/****************************************************************************************************************/
/* Enumeration types                                                                                            */
/****************************************************************************************************************/


typedef enum
{
	e_P0_select,
	e_P1_select,
	e_P2_select
}e_probeSelcect_type;

typedef enum
{
	e_checkForCalibration,
	e_calibrationStart,
	e_mainAppStart,
	//e_CAN_Calib_ConnectRequest,
	//e_calibrationByCanStart,
}e_states_type;


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

void vSelectProbe(e_probeSelcect_type probe);
u32 u32GetMilTick(void);
void vIncreaseMillCount(void);

void vSetBlockDelayMilli(u32 millSec);
void vSetOutputVoltage(u16 miliVolt);

void vSetUnusedPinsAsZero(void);

void freqIncr(void);

float getFloatUpto3place(float dataFloat);
void vResetSystemByWatchDog(void);

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