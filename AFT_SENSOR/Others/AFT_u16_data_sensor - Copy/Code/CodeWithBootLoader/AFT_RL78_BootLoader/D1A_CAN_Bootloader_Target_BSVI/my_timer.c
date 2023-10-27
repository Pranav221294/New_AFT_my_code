#include "my_macrodriver.h"
#include "my_timer.h"
#include "my_userdefine.h"
extern __boolean DelayTimerUnderFlowFlag;
/***********************************************************************************************************************
* Function Name: timer0Init
* Description  : This function initializes the TAU0 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void timer0Init(void)
{
	
	
	TAU0EN = 1U;        /* supply input clock */
    TPS0 = _0000_TAU_CKM0_FCLK_0 | _0000_TAU_CKM1_FCLK_0 | _0000_TAU_CKM2_FCLK_0 | _0000_TAU_CKM3_FCLK_0;
    /* Stop all channels */
    TT0 = _0001_TAU_CH0_STOP_TRG_ON | _0002_TAU_CH1_STOP_TRG_ON | _0004_TAU_CH2_STOP_TRG_ON |
          _0008_TAU_CH3_STOP_TRG_ON | _0010_TAU_CH4_STOP_TRG_ON | _0020_TAU_CH5_STOP_TRG_ON |
          _0040_TAU_CH6_STOP_TRG_ON | _0080_TAU_CH7_STOP_TRG_ON;
    /* Mask channel 0 interrupt */
    TMMK00 = 1U;        /* disable INTTM00 interrupt */
    TMIF00 = 0U;        /* clear INTTM00 interrupt flag */
    /* Set INTTM00 low priority */
    TMPR100 = 1U;
    TMPR000 = 1U;
    /* Channel 0 used as interval timer */
    TMR00 = _0000_TAU_CLOCK_SELECT_CKM0 | _0000_TAU_CLOCK_MODE_CKS | _0000_TAU_MODE_INTERVAL_TIMER |
            _0001_TAU_START_INT_USED;
    TDR00 = _3E7F_TAU_TDR00_VALUE;
    TOM0 &= ~_0001_TAU_CH0_OUTPUT_COMBIN;
    TOL0 &= ~_0001_TAU_CH0_OUTPUT_LEVEL_L;
    TOE0 &= ~_0001_TAU_CH0_OUTPUT_ENABLE;
    /* Set noise filter sampling clock divisor and channels selected*/
    TNFSMP0 = _00_TAU0_NOISE_DIVISOR;
    TNFCS0 = _00_TAU0_NOISE_CHANNEL_SELECT;
}

/***********************************************************************************************************************
* Function Name: system1msClockEnable
* Description  : This function starts TAU0 channel 0 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void system1msClockEnable(void)
{
    TMIF00 = 0U;        /* clear INTTM00 interrupt flag */
    TMMK00 = 0U;        /* enable INTTM00 interrupt */
    TS0 |= _0001_TAU_CH0_START_TRG_ON;
}

/***********************************************************************************************************************
* Function Name: system1msClockDisable
* Description  : This function stops TAU0 channel 0 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void system1msClockDisable(void)
{
    TT0 |= _0001_TAU_CH0_STOP_TRG_ON;
    TMMK00 = 1U;        /* disable INTTM00 interrupt */
    TMIF00 = 0U;        /* clear INTTM00 interrupt flag */
}

