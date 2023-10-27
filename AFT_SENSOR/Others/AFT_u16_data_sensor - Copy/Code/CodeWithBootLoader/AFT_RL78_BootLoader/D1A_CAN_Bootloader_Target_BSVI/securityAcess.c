#include <stdlib.h>
#include "my_can.h"
#include "string.h"
//#include "my_userdefine.h"
//#include "my_macrodriver.h"
extern void data_decrypt(unsigned char *state, const unsigned char *key);
extern unsigned char Aes_Databuf[16];
__boolean bKeyMatch;
unsigned char data[16];

//local macros
#define getRandomNumber (unsigned char)(rand() % 255)
extern volatile unsigned long gu32Clock1msTicker ;

static const unsigned char ScipherKey[16] = "KALMAR2019PHASE3";
void sendRandomNumber()
{
	int i =0;
	if(gu32Clock1msTicker == 0)
		srand(178538);
	else
		srand((unsigned int) gu32Clock1msTicker);
	for(i = 0; i < 16  ; i++)
		data[i] = getRandomNumber;
		
	MY_Tx_MsgBuf_Processing(0x14,14,8,&data[0]);		//Send "Switch pressed" command
	//for(i =0 ;i <=5000;i++);
	MY_Tx_MsgBuf_Processing(0x15,15,8,&data[8]);		//Send "Switch pressed" command
	
}

void KeyMatch()
{
	data_decrypt(Aes_Databuf,ScipherKey);
			
	if((memcmp(Aes_Databuf,data,16)== 0))
	{
		bKeyMatch = 1;
	}
}