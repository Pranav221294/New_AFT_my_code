
#ifndef _USER_DEF_H
#define _USER_DEF_H
//----------------------------------------------------------------------------------------------------------------
//#define	LED1	P2.7
//#define	LED2	P13.0

#define	INPUT	1
#define	OUTPUT	0

#define	OUTPUT_LOW	0
#define OUTPUT_HIGH	1
//#define SW_INPUT	P1.7//P6.2    //P1.7
//#define	MODE_SW_INPUT	PM1.7//PM6.2   //PM1.7

//#define sw2 P6.2 
#define FIRST_USER_FLASH_ADDR	0x4000
#define MS_PER_TIMER_COMPARE_MATCH 	1
#define OK				0
#define ERROR				1
#define TIMEOUT				2
#define LINEFEED 			10
#define CARRIAGE_RETURN 		13

#define PROG_PASS					0x00

#define PROG_FAIL					0x02

#define START_BLOCK	0x10
//#define END_BLOCK	0xFF		// Block counts 255 for R5F10TPJ
#define END_BLOCK	0x2F		// block counts 47 FOR R5F10DGD

void sendRandomNumber();
typedef unsigned short read_datum;
extern unsigned char Rx_Databuf[8];

extern __boolean baes1;
extern __boolean baes2;
extern __boolean bKeyMatch;
typedef unsigned char (*pt2Function)(unsigned long, unsigned short * );

extern void KeyMatch();


//----------------------------------------------------------------------------------------------------------------
#endif
