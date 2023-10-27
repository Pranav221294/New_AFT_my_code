
//#pragma interrupt INTTM00 my_tau0_channel0_interrupt
//----------------------------------------------------------------------------------------------------------------
#include "my_macrodriver.h"
#include "my_timer.h"
#include "my_userdefine.h"
//----------------------------------------------------------------------------------------------------------------

// Create a clock tick variable
extern volatile unsigned long gu32Clock1msTicker ;
extern unsigned char timer_flag;
__boolean DelayTimerUnderFlowFlag;
//----------------------------------------------------------------------------------------------------------------
/*
//__interrupt static void my_tau0_channel0_interrupt(void)
void my_tau0_channel0_interrupt(void)
{
    DelayTimerUnderFlowFlag = 1;
    MY_TAU0_Channel0_Stop();
    timer_flag=1;
    LED1++;
}*/
//----------------------------------------------------------------------------------------------------------------
/***********************************************************************************************************************
* Function Name: system1msClock
* Description  : This function is INTTM00 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
//rr  __interrupt static void system1msClock(void)
  void  system1msClock(void)
{
    /* Start user code. Do not edit comment generated here */
    
    ++gu32Clock1msTicker;

    /* End user code. Do not edit comment generated here */
}


