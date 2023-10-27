
#include "r_cg_macrodriver.h"
#include "customTypedef.h"
#include "r_cg_userdefine.h"
#include "InputSense.h"



#define IGN_PIN_SENSE_TIME          5u     /* Ignition ON/OFF time sense in ms */
#define HR_RESET_SENSE_TIME			500u	/* He reset ON/OFF time sense in ms */




/*********************************************************************//**
 *
 * get bGet Hour Reset Input Pins Status
 *
 *
 * @param      None    
 *
 * @return     Bool
 *************************************************************************/
bool bGetHourResetInputPinsStatus(void)
{
	static bool status = FALSE;
	static u16 InputOnTimeStamp = 0u;
	static u16 InputOffTimeStamp = 0u;			
	
	if(HOUR_RESET_INPUT_PIN == HIGH )
	{
		status = FALSE;
		InputOnTimeStamp = u16GetClockTicks();
	}
	else if(HOUR_RESET_INPUT_PIN == LOW )
	{
		if( u16GetClockTicks() - InputOnTimeStamp	>= HR_RESET_SENSE_TIME)
		{
			status = TRUE;
		}
		else
		{
			/*NA*/
		}
		InputOffTimeStamp = u16GetClockTicks() ;
	}
	else
	{
		/*NA*/
	}
		
	return status;	
  
}


/*********************************************************************//**
 *
 * get bGet HourEnable Input Pins Status
 *
 *
 * @param      None    
 *
 * @return     Bool
 *************************************************************************/
bool bGetHourEnableInputPinsStatus(void)
{
	static bool status = FALSE;
	static u16 InputOnTimeStamp = 0u;
	static u16 InputOffTimeStamp = 0u;			
	

	if(IGN_INPUT_PIN == HIGH )
	{
		status = FALSE;
		InputOnTimeStamp = u16GetClockTicks() ;					/* Do not need debaunce at this condition*/
	}
	else if(IGN_INPUT_PIN == LOW )
	{
		if( u16GetClockTicks() - InputOnTimeStamp	>= IGN_PIN_SENSE_TIME)
		{
			status = TRUE;	
		}
		else
		{
			/*NA*/
		}
		InputOffTimeStamp = u16GetClockTicks() ;
	}
	else
	{
		/*NA*/
	}

	return status;	
  
}



