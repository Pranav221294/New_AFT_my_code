
#ifndef __Hr_DataSave_H
#define __Hr_DataSave_H


#include "r_cg_macrodriver.h" 
#include "r_cg_userdefine.h"
#include "r_cg_wdt.h"



/*Global Prototye ------------------------------------------*/
void vSaveHourData						(void);
void MemoryInit							(void);
void vResetHourData						(void);


bool GetUpdate6MinutesFlag(void);
void vSetUpdate6MinutesFlag(bool status);

#endif