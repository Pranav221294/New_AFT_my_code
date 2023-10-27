
#include "HourMeter.h"
#include "dataflash.h"
#include "CustomTypedef.h"
#include "LcdDriver.h"
#include "Event.h"
#include "InputSense.h"



HOUR_DATA_TYPE sHourMeter = {0,0,0,0,FALSE,FALSE,FALSE,FALSE};



/*********************************************************************//**
 *
 * UpdateHourMeter This functio used for Hour meter increment 
 *
 * @param      None    
 *
 * @return     None
 *************************************************************************/
void UpdateHourMeter(void)
{
	if(bGetHourMeterEnableStatus() == TRUE)
	{
		if(bGetHourMeterUpdateStatus() == TRUE)							/* Get update in every 50 ms */
		{
			vSetHourMeterUpdateStatus(FALSE);
			sHourMeter.u8HrMtrMiliSeconds++;
			sHourMeter.bUpdateHourMtrSecondCounter = TRUE;
			if(sHourMeter.u8HrMtrMiliSeconds >= ONE_SECOND_COUNTER)		/* Get update in every 20 * 50ms = 1 sec */
			{
				sHourMeter.u8HrMtrMiliSeconds = 0;				
				sHourMeter.u8HrMtrSeconds++;
			}
			
			if(sHourMeter.u8HrMtrSeconds >= ONE_MINUTE_SECONDS)			/* Get update in every 1 minute */
			{
				sHourMeter.u8HrMtrSeconds = 0;
				sHourMeter.u8EngineHoursMinutes++;
				sHourMeter.bUpdateOneMinuteHourMtr = TRUE;
			}
			
			if(sHourMeter.u8EngineHoursMinutes >= SIX_MINUTE)			/* Get update in every 6 minutes */
			{
				sHourMeter.u8EngineHoursMinutes = 0u;
				sHourMeter.bUpdateTotalEngineHourMtr =  TRUE;			/* Set when fraction value change */
				sHourMeter.u32EngineHours++;
				if(sHourMeter.u32EngineHours > MAX_ENGINE_HOUR_VALUE)
				{
					sHourMeter.u32EngineHours = 0u;
				}
			}		
		}
	}
	
}

/*****************************************************************************
**  Function name: 
**		_vAddStartUpTime()
**
**  Description:
**		Initialize Hour variable and if ignition ON then add 500ms time in hour 
**		becouse module start up time is 500ms.(500ms time checked by CRO by pin toggle)
**    
**  Parameters:     
**		None  
**
**  Return value:   
**		None
**
*****************************************************************************/
void _vAddStartUpTime(void)
{
	u8 TempVer = 0;
	
	if (bGetHourEnableInputPinsStatus() == TRUE)		/* When Ign pin connect  */
	{
		TempVer = u8GetHoursMiliSeconds();//u8MilliSecond;
		TempVer += 10;               	//Timer interrupt is 50ms ,So 10x50 = 500ms time added
		if (TempVer >= 20)		     	//Check if it is more 1000ms i.e.50*20 =1000
		{   
			TempVer -=20;		        //Minus 1 sec time
			if (++sHourMeter.u8HrMtrSeconds >= ONE_MINUTE_SECONDS)
			{
				sHourMeter.u8HrMtrSeconds = 0;
				sHourMeter.bUpdateOneMinuteHourMtr = TRUE;
				if (++sHourMeter.u8EngineHoursMinutes >= SIX_MINUTE)
				{
					sHourMeter.u8EngineHoursMinutes = 0;
					++sHourMeter.u32EngineHours;
				}
			} 
		}
		vSetHoursMiliSeconds(TempVer); //update remaining time milli second 
	}
}	
/*********************************************************************//**
 *
 * vStart_Hour_Meter 	This function is use for Hour meter start and Stop w.r.t Hour 
 *						enbale pin is connect/disconnect. 
 *
 * @param      None    
 *
 * @return     None
 *************************************************************************/

void vStart_Hour_Meter(void)
{
    if(bGetHourEnableInputPinsStatus() == TRUE)
    {
        sHourMeter.bHourMeterEnable = TRUE;
    }
    else
    {
        sHourMeter.bHourMeterEnable = FALSE;
    }
}

/*********************************************************************//**
 *
 * vSet Engine Hours Minutes
 *
 * @param     vSet Engine Hours Minutes
 *
 * @return    none
 *************************************************************************/
void vSetEngineHoursMinutes(u8 value)	
{
	sHourMeter.u8EngineHoursMinutes = value;
}

/*********************************************************************//**
 *
 * Get Engine Hours Minutes
 *
 * @param      None    
 *
 * @return     Get Engine Hours Minutes
 *************************************************************************/
u8 u8GetEngineHoursMinutes(void)	
{
	return sHourMeter.u8EngineHoursMinutes;
}


/*********************************************************************//**
 *
 * vSet vSetHours Mili Seconds
 *
 * @param     vSet Hours Mili Seconds
 *
 * @return    none
 *************************************************************************/
void vSetHoursMiliSeconds(u8 value)	
{
	sHourMeter.u8HrMtrMiliSeconds = value;
}

/*********************************************************************//**
 *
 * Get Hours Mili Seconds
 *
 * @param      None    
 *
 * @return     Get Hours Mili Seconds
 *************************************************************************/
u8 u8GetHoursMiliSeconds(void)	
{
	return sHourMeter.u8HrMtrMiliSeconds;
}

/*********************************************************************//**
 *
 * Set Hours Counter Value
 *
 * @param      Hours Counter Value    
 *
 * @return     none
 *************************************************************************/
void vSetHoursSecondValue(u8 value)
{
	sHourMeter.u8HrMtrSeconds = value;
}

/*********************************************************************//**
 *
 * Get Hours Counter Value
 *
 * @param      None    
 *
 * @return     Hours Counter Value
 *************************************************************************/
u8 u8GetHoursSecondValue(void)
{
	return sHourMeter.u8HrMtrSeconds;
}

/*********************************************************************//**
 *
 * Set Total Engine Hours Value
 *
 * @param      Total Engine Hours Value   
 *
 * @return     none
 *************************************************************************/
void vSetTotalEngineHoursValue(u32 value)
{
	sHourMeter.u32EngineHours = value;
}

/*********************************************************************//**
 *
 * Get Total Engine Hours Value
 *
 * @param      None    
 *
 * @return     Total Engine Hours Value
 *************************************************************************/
u32 u32GetTotalEngineHours(void)
{
	return sHourMeter.u32EngineHours;
}


/*********************************************************************//**
 *
 * Set Total Engine Hours Update Status
 *
 * @param     Total Engine Hours Update Status    
 *
 * @return    none
 *************************************************************************/
void vSetTotalEngineHoursUpdateStatus(bool status)
{				
	sHourMeter.bUpdateTotalEngineHourMtr =  status;				/* Set when fraction value change */
}

/*********************************************************************//**
 *
 * Get Total Engine Hours  Update Status
 *
 * @param      None    
 *
 * @return    Total Engibe Hours  Update Status
 *************************************************************************/
bool bGetTotalEngineHoursUpdateStatus(void)
{
	return sHourMeter.bUpdateTotalEngineHourMtr;				/* Set when fraction value change */
}


/*********************************************************************//**
 *
 * Set Hour Meter Start Status
 *
 * @param      Hour Meter Enable Status    
 *
 * @return     none
 *************************************************************************/

void vSetHourMeterEnableStatus(bool Status)
{
	sHourMeter.bHourMeterEnable = Status;
}

/*********************************************************************//**
 *
 * Get Hour Meter Start Status
 *
 * @param      None    
 *
 * @return     Start Status of Hour Meter
 *************************************************************************/

bool bGetHourMeterEnableStatus(void)
{
	return (sHourMeter.bHourMeterEnable);
}

/*********************************************************************//**
 *
 * vSet Hours Seconds Update Status
 *
 * @param     vSet Hours Seconds Update Status 
 *
 * @return    none
 *************************************************************************/
void vSetHoursSecondsUpdateStatus(bool status)
{
	sHourMeter.bUpdateHourMtrSecondCounter = status;
}

/*********************************************************************//**
 *
 * Get Hours Seconds Update Status
 *
 * @param      None    
 *
 * @return     Get Hours Seconds Update Status
 *************************************************************************/
bool bGetHoursSecondsUpdateStatus(void)
{
	return sHourMeter.bUpdateHourMtrSecondCounter;
}

/*********************************************************************//**
 *
 * vSet OneMinute Update Status
 *
 * @param     vSet OneMinute Update Status 
 *
 * @return    none
 *************************************************************************/
void vSetOneMinuteUpdateStatus(bool status)
{
	sHourMeter.bUpdateOneMinuteHourMtr = status;
}

/*********************************************************************//**
 *
 * Get bGet OneMinute Update Status
 *
 * @param      None    
 *
 * @return     Get bGet OneMinute Update Status
 *************************************************************************/
bool bGetOneMinuteUpdateStatus(void)
{
	return sHourMeter.bUpdateOneMinuteHourMtr;
}

