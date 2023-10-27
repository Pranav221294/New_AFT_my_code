/*****************************************************************************
 **  Module: TellTales
 **
 ** ---------------------------------------------------------------------------
 **  Description:
 **      This Module is designed for TellTales Operation. Check input status of
 **      each indicator and oprates accordinly.
 **
 *****************************************************************************/


/* MODULE TellTales */
#include "j1939.h"
#include "TellTales.h"
#include "customTypedef.h"
#include "dataFlash.h"
#include "r_cg_userdefine.h"
#include "defGauge.h"





#define LED_BLINK_RATE						667				// Macro to used for blinking LED time 0.7 hz



/* MODULE TellTales */




// private prototypes --------------------------------------------------------
//Lamp Connected With IIC1

/// @addtogroup TELL_TALE_LIBRARY
/// @{ 
/// @defgroup TellTaleLib Private functions used in Tell tale file
//// 
///   
/// @addtogroup TellTaleLib
/// @{

static void _vLampDEF(void);

/// @} // endgroup TellTaleLib

/// @} // endgroup TELL_TALE_LIBRARY




void vControlDefTellTales(void)
{
	static bool blinkflag = 0;
	static u32 blinktimestamp = 0;
	static bool FirstEntry = FALSE;
	static u8 LedStatus = 0U;
	static u8 blinkCount = 0U;
	static u16 badBlinkRate = 50U;
	bool btimeout;
	
	LedStatus = u8GetLowDefGuageWarningLampsStatus();
	
	switch(LedStatus)
	{
		case eLedOn:
			LOW_DEF_WARNING_LED_ON;
		break;
		case eLampSingleBlink:	/* Time out, 100 ,def data > 0xFA*/
			if(FirstEntry == FALSE)
			{
				blinktimestamp = u32GetClockTicks();
				FirstEntry = TRUE;
				blinkflag = FALSE;
			}
			if(blinkCount >= 2U)
			{
				blinkCount = 0U;
				badBlinkRate = 1950;
				blinkflag = FALSE;
				blinktimestamp = u32GetClockTicks();
			}	
			if(u32GetClockTicks() - blinktimestamp >= badBlinkRate)
			{
				badBlinkRate = 50U;
				blinktimestamp = u32GetClockTicks();
				blinkflag = blinkflag ^ 0xFF;
				blinkCount++;
			}
			if(blinkflag == FALSE)
				LOW_DEF_WARNING_LED_OFF;
			else
				LOW_DEF_WARNING_LED_ON;
		break;
		case eLampDoubleBlink:	/* Time out, 100 ,def data > 0xFA*/
			if(FirstEntry == FALSE)
			{
				blinktimestamp = u32GetClockTicks();
				FirstEntry = TRUE;
				blinkflag = FALSE;
			}
			if(blinkCount >= 4U)
			{
				blinkCount = 0U;
				badBlinkRate = 1950;
				blinkflag = FALSE;
				blinktimestamp = u32GetClockTicks();
			}	
			if(u32GetClockTicks() - blinktimestamp >= badBlinkRate)
			{
				badBlinkRate = 50U;
				blinktimestamp = u32GetClockTicks();
				blinkflag = blinkflag ^ 0xFF;
				blinkCount++;
			}
			if(blinkflag == FALSE)
				LOW_DEF_WARNING_LED_OFF;
			else
				LOW_DEF_WARNING_LED_ON;
		break;
		case eLedOff:
			LOW_DEF_WARNING_LED_OFF;
		break;
		default:
			LOW_DEF_WARNING_LED_OFF;
		break;
	}
	
}




/* END TellTales */
