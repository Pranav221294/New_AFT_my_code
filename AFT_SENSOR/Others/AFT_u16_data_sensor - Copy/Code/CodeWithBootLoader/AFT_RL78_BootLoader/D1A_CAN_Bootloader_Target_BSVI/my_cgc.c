	
#include "my_macrodriver.h"
#include "my_cgc.h"
#include "my_userdefine.h"
//----------------------------------------------------------------------------------------------------------------
void MY_CGC_Create(void)
{
//   CMC = 0x10;			// Set fMX
//    
//    MSTOP = 1U;
//    
//    MCM0 = 0U;			// Set fMAIN
//
//    XTSTOP = 0U;		// Set fSUB
//    
//    OSMC = 0x10;
//    
//    CSS = 0U;			// Set fCLK
//    
//    HIOSTOP = 0U;		// Set fIH
//--------------------------------


#if 1
volatile uint8_t  temp_stab_set;
    volatile uint8_t  temp_stab_wait;

    OSMC = _10_CGC_RTC_CLK_FIL;
    /* Set fMX */
    CMC = _40_CGC_HISYS_OSC | _01_CGC_SYSOSC_OVER10M;
    OSTS = _07_CGC_OSCSTAB_SEL18;
    MSTOP = 0U;
    temp_stab_set = _FF_CGC_OSCSTAB_STA18;

    do
    {
        temp_stab_wait = OSTC;
        temp_stab_wait &= temp_stab_set;
    }
    while (temp_stab_wait != temp_stab_set);

    /* Set fMAIN */
    MCM0 = 1U;

    while (MCS == 0U)
    {
        ;
    }

    MDIV = _00_CGC_FMP_DIV_DEFAULT;
    /* Set fMP to clock through mode */
    SELPLL = 0U;
    /* Set fIH */
    HIOSTOP = 0U;		// changed from 1u to 0u
    /* Set RTC/Interval Timer clock source */
    RTCCL = _01_CGC_RTC_FIL;

#else

//16 MHz Internal Osc

//	OSMC = _10_CGC_RTC_CLK_FIL;
//    /* Set fMX */
//    CMC = _00_CGC_HISYS_PORT | _00_CGC_SYSOSC_DEFAULT;
//    MSTOP = 1U;
//    /* Set fMAIN */
//    MCM0 = 0U;
//    MDIV = _00_CGC_FMP_DIV_DEFAULT;
//    /* Set fMP to clock through mode */
//    SELPLL = 0U;
//    /* Set fIH */
//    HIOSTOP = 0U;
//    /* Set RTC/Interval Timer clock source */
//    RTCCL = _01_CGC_RTC_FIL;

	
#endif	
	

}

//----------------------------------------------------------------------------------------------------------------
