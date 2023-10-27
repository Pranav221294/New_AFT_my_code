
#include "r_cg_macrodriver.h"
#include "customTypedef.h" 
#include "stepperDriver.h"
#include "r_cg_userdefine.h"
#include "j1939.h"
#include "defGauge.h"
#include "TellTales.h"

// Constant declarations -----------------------------------------------------


#define MAX_DEF_GUAGE_LVL					250u
#define DEF_AVG_SIZE						2
#define LOW_DEF_WARNING_DATA_ENTRY			0x3E
#define LOW_DEF_WARNING_DATA_EXIT			0x40

// local declarations --------------------------------------------------------
typedef struct
{	
	u16     u16DefSignalData[DEF_AVG_SIZE];      	 
	u8      u8DefLevel;      			 
	u8      u8PreviousDefLevel;      			 	
	u8      u8DefBarNo;
	bool    bSymboleStatus;
} DEF_DATA_TYPE;

DEF_DATA_TYPE sDEF = {0x00,0x00,0x00,0x00,0x00};

static u8 WarningLedStatus = eLedOn;

/*********************************************************************//**
 *
 * Check def CAN msg and CAN lost msg ,According to def level calculate bars.
 *
 *
 * @param      None 
 *
 * @return     None 
 *************************************************************************/
void vDefGauge(void)
{	
	u32 RowData = 0u;
	u8 i = 0;
	static u8 u8AvgSampleCount = 0;	
	u32 microsteps = 0;
	u32 temp = 0;
	static bool FirstEntry = TRUE;
	static u8 InDefGaugeTimeOutEntry = TRUE;
	static bool bCAN_TimeOut = FALSE;
	static u8 Hysterisis = 0;
	
	
	
	// Take CAN data -------------------------*/
	bCAN_TimeOut = bGetPGNTimeOut(eJLowDEFGauge);
	RowData = u32GetPGNdata(eJLowDEFGauge);			/* Row data ---*/
	
	if(bCAN_TimeOut)
	{
		WarningLedStatus = eLampSingleBlink;
		Hysterisis =0;
		if(InDefGaugeTimeOutEntry == FALSE)
		{
			WarningLedStatus = eLedOn;
			vLongHoming();
			vShortHoming();
			InDefGaugeTimeOutEntry = TRUE;
		}
		RowData = 0u;
	}	
	else if(RowData >= 0xFE)
	{
		RowData = 0u;
		WarningLedStatus = eLampDoubleBlink;
		Hysterisis =0;
	}
	else if(RowData <= LOW_DEF_WARNING_DATA_ENTRY && bCAN_TimeOut == FALSE)
	{
		WarningLedStatus = eLedOn;
		Hysterisis = 2;
	}
	/* Hysterisis added */
	else if(RowData >= (LOW_DEF_WARNING_DATA_ENTRY + Hysterisis))
	{
		WarningLedStatus = eLedOff;
		Hysterisis =0;
	}
	
	if(bGetPGNDataReady(eJLowDEFGauge))
	{
		InDefGaugeTimeOutEntry = FALSE;
		ClearPGNDataReady(eJLowDEFGauge);
	}
	
	
	//Averging of CAN data-----------------
	if(FirstEntry == TRUE)
	{
		for(i = 0; i < DEF_AVG_SIZE; i++)
			sDEF.u16DefSignalData[i]  = (u16)RowData;	
		u8AvgSampleCount = DEF_AVG_SIZE - 1;
		FirstEntry = FALSE;
	}

	/* Calculate microsteps for drive stepper of Def gauge ---------------*/
	microsteps = u16GaugeMaxMicroSteps(eDefGauge);		
	temp = (( microsteps * RowData )/ MAX_DEF_GUAGE_LVL) + EXTRA_STEPS_COMPENSATE;  // Calculate Target position 
	vSetTargetPosition((u16)temp,eDefGauge);
   	
}


/*********************************************************************//**
 *
 * Check def warning status
 *
 * @param      def warning status 
 *
 * @return     None
 *************************************************************************/
u8 u8GetLowDefGuageWarningLampsStatus(void)
{
	static bool firstentry = TRUE;
	static u16 timestamp = 0;

	if(firstentry)
	{
		timestamp = u16GetClockTicks();
		firstentry = FALSE;
	}
	if(u16GetClockTicks() - timestamp < 600)
	{
		WarningLedStatus = eLedOff;
	}

	return WarningLedStatus;
}
