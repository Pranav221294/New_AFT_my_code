#include "r_cg_macrodriver.h" 
#include "r_cg_userdefine.h"
#include "r_cg_wdt.h"
#include "HrDataSave.h"
#include "HourMeter.h"
#include "CustomTypedef.h"
#include "Event.h"
#include "dataflash.h"
#include "InputSense.h"


/* Constatnt declaration ----------------------------------------------------------- */

/* Local Proto ----------------------------------------------------------------------- */

/* Local var ----------------------------------------------------------------------- */
static bool Minutes6UpdateFlag  = FALSE;

/***********************************************************************************************************************
* Function Name: vSaveHourData
* Description  : This function save hour , minute,seconds hour data in flash EEPROM
* Arguments    : None
* Return Value : None
*  
***********************************************************************************************************************/

void vSaveHourData(void)
{
	static bool bOneTimeStatus = FALSE;

	if(bGetHourEnableInputPinsStatus() == FALSE)		/* When Ign pin disconnect ,save seconds & mili sec. data */
	{
		if(bGetHoursSecondsUpdateStatus() == TRUE)
	 	{
	 		vSetHoursSecondsUpdateStatus(FALSE);
			vSetTotalEngineHoursUpdateStatus(FALSE);		/* Total Engine hours updated status false */
			vSetOneMinuteUpdateStatus(FALSE);				/* one complete updated status false */
		  	_vSaveHrSecondsMiliSecSysParms();
			_vSaveHourSysParms();							/* Write hour data pack in internal data flash */
			_vWriteHourMinutesValue();						/* Minutes save*/
	 	}
	}
	
	if(bGetOneMinuteUpdateStatus() == TRUE)
	{
		vSetOneMinuteUpdateStatus(FALSE);				/* one complete updated status false */
		_vWriteHourMinutesValue();						/* Minutes save*/
	}				

	if(bGetTotalEngineHoursUpdateStatus() == TRUE) 		/* When 6 minutes complete */
	{
		vSetTotalEngineHoursUpdateStatus(FALSE);		/* Total Engine hours updated status false */
		_vSaveHourSysParms();							/* Write hour data pack in internal data flash */
		Minutes6UpdateFlag = TRUE;						/* Update in every 6 minutes */
	}
	
}


/**************************************************************************************************
* Function Name: memory_init
* Description  : This function read Hour data ,  data, Trip data from external EEPROM
* Arguments    : None
* Return Value : None
**************************************************************************************************/

void MemoryInit(void)
{
	/* Read total engine hours minutes,seconds,Mili seconds */
	_vReadHourSysParms();				/* Read hour data from data flash pack */

}

/**************************************************************************************************
* Function Name: vResetHourData
* Description  : This function is use for Reset the hour zero
* Arguments    : None
* Return Value : None
**************************************************************************************************/
void vResetHourData(void)
{
	static bool bOneTimeStatus = FALSE;
	
	if(bGetHourResetInputPinsStatus() == TRUE)
	{
		if(bOneTimeStatus == FALSE)
		{
			 vSetHoursMiliSeconds(0);		/* Set Engine mili Seconds */
			 vSetHoursSecondValue(0);		/* Set Engine hour Seconds */
			 vSetEngineHoursMinutes(0);
			 vSetTotalEngineHoursValue(0);
			 _vSaveHrSecondsMiliSecSysParms();
			 _vSaveHourSysParms();
			 _vWriteHourMinutesValue();
			 vSetUpdate6MinutesFlag(TRUE);	/* For Lcd update 0.0 */
			 bOneTimeStatus = TRUE;
		}
	}
	else
	{
		bOneTimeStatus = FALSE;
	}
	
}

void vSetUpdate6MinutesFlag(bool status)
{
	Minutes6UpdateFlag = status;
}

bool GetUpdate6MinutesFlag(void)
{
	return Minutes6UpdateFlag;
}





