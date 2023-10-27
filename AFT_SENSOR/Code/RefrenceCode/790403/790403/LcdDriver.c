/********************************************************************** 

Filename : lcdDriver.c

Description:  Performs user friendly initialization 
              of LCD Control Register 0 and 1. Source file
              initializes LCD duty cycle and bias.  By
              providing the program with your desired frame
              frequency and given oscillator frequency, this
              program calculates the correct clock divider.
              User is also capable of enabling special modes
              like Pseudostop or Wait modes.  Front plane start
              and stop points are also entered. Finally, the 
              program enables the LCD to the on state. 

              Additional routines provide the user with the 
              ability to turn on and off all the LCD segments.

 ************************************************************************/
#include "HourMeter.h"
#include "LcdDriver.h"
#include "CustomTypedef.h"
#include "r_cg_userdefine.h"
#include "HrDataSave.h"
#include "InputSense.h"



/*Constant declaration -----------------------------------------------------------*/
#define HOUR_SYMBOL_BLINK_TIME				999u	/* Time in miliSec */

/* Local prototye ----------------------------------------------------------------*/
static void vHourMeterSymbolBlink			(void);



/* Local Var. declaration --------------------------------------------------------*/



/**************************************************//**
*vDisplayHourDigits this function used for display Engine 
*
*@param u32 Hours Value
*
*@return none
*
***************************************************/
void vDisplayHourDigits(u32 HoursValue)
{
	u8 digitPos1,digitPos2,digitPos3,digitPos4,digitPos5,digitPos6;    /* Temporary HoursValue for each of the 6 positions */
	u32 temp;
	
	DECIMAL_DOT_SYMBOL_ON;
	
	digitPos1 = (u8) (HoursValue%10);   
	temp = HoursValue/10;
	DISPLAY_POS_1_VALUE_off
	
	switch(digitPos1)
	{ 	
		case 0: DISPLAY_POS_1_VALUE_0   break;
		case 1: DISPLAY_POS_1_VALUE_1   break;
		case 2: DISPLAY_POS_1_VALUE_2   break;
		case 3: DISPLAY_POS_1_VALUE_3   break;
		case 4: DISPLAY_POS_1_VALUE_4   break;
		case 5: DISPLAY_POS_1_VALUE_5   break;
		case 6: DISPLAY_POS_1_VALUE_6   break;
		case 7: DISPLAY_POS_1_VALUE_7   break;
		case 8: DISPLAY_POS_1_VALUE_8   break;
		case 9: DISPLAY_POS_1_VALUE_9   break;
		default: DISPLAY_POS_1_VALUE_off   break;
	}

	digitPos2 = (u8) (temp%10);  
	temp = temp/10;

	DISPLAY_POS_2_VALUE_off
	switch(digitPos2)
	{    
		case 0: DISPLAY_POS_2_VALUE_0   break;
		case 1: DISPLAY_POS_2_VALUE_1   break;
		case 2: DISPLAY_POS_2_VALUE_2   break;
		case 3: DISPLAY_POS_2_VALUE_3   break;
		case 4: DISPLAY_POS_2_VALUE_4   break;
		case 5: DISPLAY_POS_2_VALUE_5   break;
		case 6: DISPLAY_POS_2_VALUE_6   break;
		case 7: DISPLAY_POS_2_VALUE_7   break;
		case 8: DISPLAY_POS_2_VALUE_8   break;
		case 9: DISPLAY_POS_2_VALUE_9   break;
		default: DISPLAY_POS_2_VALUE_off   break;
	}

	digitPos3 = (u8) (temp%10);   
	temp = temp/10;

	if(!temp && !digitPos3)
		digitPos3 = 10;
		
	DISPLAY_POS_3_VALUE_off;
			
	switch(digitPos3)
	{	
		case 0: DISPLAY_POS_3_VALUE_0   break;
		case 1: DISPLAY_POS_3_VALUE_1   break;
		case 2: DISPLAY_POS_3_VALUE_2   break;
		case 3: DISPLAY_POS_3_VALUE_3   break;
		case 4: DISPLAY_POS_3_VALUE_4   break;
		case 5: DISPLAY_POS_3_VALUE_5   break;
		case 6: DISPLAY_POS_3_VALUE_6   break;
		case 7: DISPLAY_POS_3_VALUE_7   break;
		case 8: DISPLAY_POS_3_VALUE_8   break;
		case 9: DISPLAY_POS_3_VALUE_9   break;
		default: DISPLAY_POS_3_VALUE_off; break;
	}

	digitPos4 = (u8) (temp%10);   
	temp = temp/10;

	if(!temp && !digitPos4)
		digitPos4 = 10;
		
	DISPLAY_POS_4_VALUE_off;
	
	switch(digitPos4)
	{ 
		case 0: DISPLAY_POS_4_VALUE_0   break;
		case 1: DISPLAY_POS_4_VALUE_1   break;
		case 2: DISPLAY_POS_4_VALUE_2   break;
		case 3: DISPLAY_POS_4_VALUE_3   break;
		case 4: DISPLAY_POS_4_VALUE_4   break;
		case 5: DISPLAY_POS_4_VALUE_5   break;
		case 6: DISPLAY_POS_4_VALUE_6   break;
		case 7: DISPLAY_POS_4_VALUE_7   break;
		case 8: DISPLAY_POS_4_VALUE_8   break;
		case 9: DISPLAY_POS_4_VALUE_9   break;
		default: DISPLAY_POS_4_VALUE_off; break;
	}

	digitPos5 =(u8) (temp%10);   
	temp = temp/10;

	if(!temp && !digitPos5)
		digitPos5 = 10;
	DISPLAY_POS_5_VALUE_off
	switch(digitPos5)
	{ 
		case 0: DISPLAY_POS_5_VALUE_0   break;
		case 1: DISPLAY_POS_5_VALUE_1   break;
		case 2: DISPLAY_POS_5_VALUE_2   break;
		case 3: DISPLAY_POS_5_VALUE_3   break;
		case 4: DISPLAY_POS_5_VALUE_4   break;
		case 5: DISPLAY_POS_5_VALUE_5   break;
		case 6: DISPLAY_POS_5_VALUE_6   break;
		case 7: DISPLAY_POS_5_VALUE_7   break;
		case 8: DISPLAY_POS_5_VALUE_8   break;
		case 9: DISPLAY_POS_5_VALUE_9   break;
		default: DISPLAY_POS_5_VALUE_off   break;
	}
	
	digitPos6 =(u8)temp;
	if(!temp)
		digitPos6 = 10;
	DISPLAY_POS_6_VALUE_off
	switch(digitPos6)
	{ 
		case 0: DISPLAY_POS_6_VALUE_0   break;
		case 1: DISPLAY_POS_6_VALUE_1   break;
		case 2: DISPLAY_POS_6_VALUE_2   break;
		case 3: DISPLAY_POS_6_VALUE_3   break;
		case 4: DISPLAY_POS_6_VALUE_4   break;
		case 5: DISPLAY_POS_6_VALUE_5   break;
		case 6: DISPLAY_POS_6_VALUE_6   break;
		case 7: DISPLAY_POS_6_VALUE_7   break;
		case 8: DISPLAY_POS_6_VALUE_8   break;
		case 9: DISPLAY_POS_6_VALUE_9   break;
		default: DISPLAY_POS_6_VALUE_off   break;
	}

}

/**************************************************//**
*TurnONAllLcdSeg for use all seg. char ON 
*
*@param none
*
*@return none
*
***************************************************/
void TurnONAllLcdSeg(void)
{
	SEG0 = 0xFF;	SEG1 = 0xFF;	SEG2 = 0xFF;
	SEG3 = 0xFF;	SEG4 = 0xFF;	SEG5 = 0xFF;
	SEG6 = 0xFF;	SEG7 = 0xFF;	SEG9 = 0xFF;
	SEG14 = 0xFF;	SEG15 = 0xFF;	SEG24 = 0xFF;
}

/**************************************************//**
* TurnOffAllLcdSeg for use Lcd clear
*
* @param none
*
* @return none
*
***************************************************/
void TurnOffAllLcdSeg(void)
{
	SEG0 = 0x00;	SEG1 = 0x00;	SEG2 = 0x00;
	SEG3 = 0x00;	SEG4 = 0x00;	SEG5 = 0x00;
	SEG6 = 0x00;	SEG7 = 0x00;	SEG9 = 0x00;
	SEG14 = 0x00;	SEG15 = 0x00;	SEG24 = 0x00;
}



/*********************************************************************//**
 *
 * vHourMeter_FuelBars_Display This function used for Hour meter Display for Engine , oil meter  & Fuel bars
 *
 * @param      None    
 *
 * @return     None
 *************************************************************************/
void vHourMeter_Display(void)
{
	static u32 u32TotalEngineHour;
	static bool FirstEntry = TRUE;
	
	if(GetUpdate6MinutesFlag() == TRUE || FirstEntry == TRUE)
	{
    	vSetUpdate6MinutesFlag(FALSE);
		FirstEntry = FALSE;
		u32TotalEngineHour = u32GetTotalEngineHours();		/* Read total engine hours */	
		vDisplayHourDigits(u32TotalEngineHour);      		/* Display Engine hours */
	}
	vHourMeterSymbolBlink();							/* Hour symbol blinking when hour enable pin connect to BATT */
	
}

/*********************************************************************//**
 *
 * vHourMeterSymbolBlink This function used for Hour meter Symbol Blink
 *
 * @param      None    
 *
 * @return     None
 *************************************************************************/
static void vHourMeterSymbolBlink(void)
{
	static u16 TimeStamp = 0u;
	static bool status = 0u;
	static bool FirstEntry = FALSE;
	
	if(bGetHourMeterEnableStatus() == TRUE)
	{
		if(FirstEntry == FALSE)
		{
			TimeStamp = u16GetClockTicks();
			HOUR_METER_SYMBOL_OFF;
			FirstEntry = TRUE;
			status = 0;
		}
		else if((u16GetClockTicks() - TimeStamp) > HOUR_SYMBOL_BLINK_TIME )  /* 1ms* >999ms = 1 sec ON/OFF */
		{
			TimeStamp = u16GetClockTicks();
			status = ~status;
			if(status)
			{
				HOUR_METER_SYMBOL_ON;
			}
			else
			{
				HOUR_METER_SYMBOL_OFF;
			}
		}
		else
		{
			/*NA*/
		}
	}
	else
	{
		HOUR_METER_SYMBOL_ON;
		TimeStamp = u16GetClockTicks();
		FirstEntry = FALSE;
	}
	
}





