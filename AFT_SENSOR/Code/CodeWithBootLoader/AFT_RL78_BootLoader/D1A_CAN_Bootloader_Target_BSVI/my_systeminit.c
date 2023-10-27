
#include "my_macrodriver.h"
#include "my_cgc.h"
#include "my_port.h"
#include "my_timer.h"
#include "my_userdefine.h"

//----------------------------------------------------------------------------------------------------------------
void MY_Systeminit(void)
{
    //PIOR = 0x00U;
    WDTE=0xAC;
    MY_CGC_Get_ResetSource();
   // MY_PORT_Create();
    MY_CGC_Create();
 timer0Init();
    //MY_TAU0_Create();
    CRC0CTL = 0x00U;
    IAWCTL = 0x00U;
    WDTE=0xAC;
}

//----------------------------------------------------------------------------------------------------------------
void hdwinit(void)
{
    DI();
    MY_Systeminit();
  //  EI();
}

//----------------------------------------------------------------------------------------------------------------
