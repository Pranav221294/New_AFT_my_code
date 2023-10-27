
//#pragma	interrupt INTP0 MY_ISR_INTP0
//----------------------------------------------------------------------------------------------------------------
#include "my_macrodriver.h"
#include "my_switch_INTP0.h"
#include "my_userdefine.h"
//----------------------------------------------------------------------------------------------------------------

__boolean sw_flag;

//----------------------------------------------------------------------------------------------------------------
void MY_Switch_Init(void)
{
	SW_INPUT = OUTPUT_LOW;
    	MODE_SW_INPUT = INPUT;
	
	EGN0 = 1;
	EGP0 = 0;		// Falling edge selected for interrupt
	
	PIF0 = 0;		// Interrupt flag-->clear flag initially
	PMK0 = 0;		// Interrupt Mask flag-->enabled
    	PPR00 = 0;
	PPR01 = 0;		// Interrupt priority selection-->highest priority
	
	
}
//----------------------------------------------------------------------------------------------------------------
//__interrupt void MY_ISR_INTP0()
void MY_ISR_INTP0(void)
{
	sw_flag=1;
}
//----------------------------------------------------------------------------------------------------------------
