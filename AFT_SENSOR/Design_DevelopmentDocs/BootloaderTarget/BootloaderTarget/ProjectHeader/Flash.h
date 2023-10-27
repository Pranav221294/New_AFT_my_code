#ifndef _FLASH
#define _FLASH


#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_wdt.h"
#include "r_cg_userdefine.h"
  
 #define MAX_NO_OF_BLOCK_IN_FLASH	780 //(End Add - start add )/block length i.e.  ( ( 0x3FFE0 - 0xF400)/256 ) + 32

extern unsigned char MY_RxByteBuffer[256];

void vFlashInit();

void vEraseFlash();
bool bWriteFlash();
void vInitFlashImpVariable();
u16 u16GetNoOfBlockReturn();


#endif

