#ifndef __INPUT_SENSE_H
#define __INPUT_SENSE_H

#include "r_cg_macrodriver.h"
#include "customTypedef.h"



/* Constant declarations ---------------------------------*/
#define	IGN_INPUT_PIN							P9.0
#define HOUR_RESET_INPUT_PIN					P13.7



bool bGetHourEnableInputPinsStatus			(void);
bool bGetHourResetInputPinsStatus			(void);



#endif
