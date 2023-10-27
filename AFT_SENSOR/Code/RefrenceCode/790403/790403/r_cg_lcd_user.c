/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2012, 2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_lcd_user.c
* Version      : Applilet3 for RL78/D1A V2.04.03.01 [15 Nov 2019]
* Device(s)    : R5F10CGB
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for LCD module.
* Creation Date: 4/5/2022
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_lcd.h"
/* Start user code for include. Do not edit comment generated here */
#include "r_cg_wdt.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
#define SEG_1A	SEG0  |= 0b00000001;
#define SEG_1B	SEG1  |= 0b00000001;
#define SEG_1C	SEG1  |= 0b00000100;
#define SEG_1D  SEG0  |= 0b00001000;
#define SEG_1E	SEG0  |= 0b00000100;
#define SEG_1F	SEG0  |= 0b00000010;
#define SEG_1G	SEG1  |= 0b00000010;

#define Clear_Pos_1	SEG0 &=0XF0; SEG1 &=0XF8; 

#define SEG_2A	SEG2  |= 0b00000001;
#define SEG_2B	SEG3  |= 0b00000001;
#define SEG_2C	SEG3  |= 0b00000100;
#define SEG_2D  SEG2  |= 0b00001000;
#define SEG_2E	SEG2  |= 0b00000100;
#define SEG_2F	SEG2  |= 0b00000010;
#define SEG_2G	SEG3  |= 0b00000010;

#define Clear_Pos_2	SEG2 &=0XF0; SEG3 &=0XF8; 

#define SEG_3A	SEG4  |= 0b00000001;
#define SEG_3B	SEG5  |= 0b00000001;
#define SEG_3C	SEG5  |= 0b00000100;
#define SEG_3D  SEG4  |= 0b00001000;
#define SEG_3E	SEG4  |= 0b00000100;
#define SEG_3F	SEG4  |= 0b00000010;
#define SEG_3G	SEG5  |= 0b00000010;

#define Clear_Pos_3	SEG4 &=0XF0; SEG5 &=0XF8; 

#define SEG_4A	SEG9   |= 0b00000001;
#define SEG_4B	SEG14  |= 0b00000001;
#define SEG_4C	SEG14  |= 0b00000100;
#define SEG_4D  SEG9   |= 0b00001000;
#define SEG_4E	SEG9   |= 0b00000100;
#define SEG_4F	SEG9   |= 0b00000010;
#define SEG_4G	SEG14  |= 0b00000010;

#define Clear_Pos_4	SEG9 &=0XF0; SEG14 &=0XF8; 

#define SEG_5A	SEG15   |= 0b00000001;
#define SEG_5B	SEG24   |= 0b00000001;
#define SEG_5C	SEG24   |= 0b00000100;
#define SEG_5D  SEG15   |= 0b00001000;
#define SEG_5E	SEG15   |= 0b00000100;
#define SEG_5F	SEG15   |= 0b00000010;
#define SEG_5G	SEG24   |= 0b00000010;

#define Clear_Pos_5	SEG15 &=0XF0; SEG24 &=0XF8; 

#define SEG_6A	SEG25   |= 0b00000001;
#define SEG_6B	SEG26   |= 0b00000001;
#define SEG_6C	SEG26   |= 0b00000100;
#define SEG_6D  SEG25   |= 0b00001000;
#define SEG_6E	SEG25   |= 0b00000100;
#define SEG_6F	SEG25   |= 0b00000010;
#define SEG_6G	SEG26   |= 0b00000010;

#define Clear_Pos_6	SEG25 &=0XF0; SEG26 &=0XF8;

#define SEG_7A	SEG27   |= 0b00000001;
#define SEG_7B	SEG29   |= 0b00000001;
#define SEG_7C	SEG29   |= 0b00000100;
#define SEG_7D  SEG27   |= 0b00001000;
#define SEG_7E	SEG27   |= 0b00000100;
#define SEG_7F	SEG27   |= 0b00000010;
#define SEG_7G	SEG29   |= 0b00000010;

#define Clear_Pos_7	SEG27 &=0XF0; SEG29 &=0XF8;


#define Display_Pos_1_Digit_0   SEG_1A;SEG_1B;SEG_1C;SEG_1D;SEG_1E;SEG_1F;
#define Display_Pos_1_Digit_1   SEG_1B;SEG_1C;
#define Display_Pos_1_Digit_2   SEG_1A;SEG_1B;SEG_1D;SEG_1E;SEG_1G;
#define Display_Pos_1_Digit_3   SEG_1A;SEG_1B;SEG_1C;SEG_1D;SEG_1G;
#define Display_Pos_1_Digit_4   SEG_1B;SEG_1C;SEG_1F;SEG_1G;
#define Display_Pos_1_Digit_5   SEG_1A;SEG_1C;SEG_1D;SEG_1F;SEG_1G;
#define Display_Pos_1_Digit_6   SEG_1A;SEG_1C;SEG_1D;SEG_1E;SEG_1F;SEG_1G;
#define Display_Pos_1_Digit_7   SEG_1A;SEG_1B;SEG_1C; 
#define Display_Pos_1_Digit_8   SEG_1A;SEG_1B;SEG_1C;SEG_1D;SEG_1E;SEG_1F;SEG_1G;
#define Display_Pos_1_Digit_9   SEG_1A;SEG_1B;SEG_1C;SEG_1D;SEG_1F;SEG_1G;


#define Display_Pos_2_Digit_0   SEG_2A;SEG_2B;SEG_2C;SEG_2D;SEG_2E;SEG_2F;
#define Display_Pos_2_Digit_1   SEG_2B;SEG_2C;
#define Display_Pos_2_Digit_2   SEG_2A;SEG_2B;SEG_2D;SEG_2E;SEG_2G;
#define Display_Pos_2_Digit_3   SEG_2A;SEG_2B;SEG_2C;SEG_2D;SEG_2G;
#define Display_Pos_2_Digit_4   SEG_2B;SEG_2C;SEG_2F;SEG_2G;
#define Display_Pos_2_Digit_5   SEG_2A;SEG_2C;SEG_2D;SEG_2F;SEG_2G;
#define Display_Pos_2_Digit_6   SEG_2A;SEG_2C;SEG_2D;SEG_2E;SEG_2F;SEG_2G;
#define Display_Pos_2_Digit_7   SEG_2A;SEG_2B;SEG_2C; 
#define Display_Pos_2_Digit_8   SEG_2A;SEG_2B;SEG_2C;SEG_2D;SEG_2E;SEG_2F;SEG_2G;
#define Display_Pos_2_Digit_9   SEG_2A;SEG_2B;SEG_2C;SEG_2D;SEG_2F;SEG_2G;

 
#define Display_Pos_3_Digit_0   SEG_3A;SEG_3B;SEG_3C;SEG_3D;SEG_3E;SEG_3F;
#define Display_Pos_3_Digit_1   SEG_3B;SEG_3C;
#define Display_Pos_3_Digit_2   SEG_3A;SEG_3B;SEG_3D;SEG_3E;SEG_3G;
#define Display_Pos_3_Digit_3   SEG_3A;SEG_3B;SEG_3C;SEG_3D;SEG_3G;
#define Display_Pos_3_Digit_4   SEG_3B;SEG_3C;SEG_3F;SEG_3G;
#define Display_Pos_3_Digit_5   SEG_3A;SEG_3C;SEG_3D;SEG_3F;SEG_3G;
#define Display_Pos_3_Digit_6   SEG_3A;SEG_3C;SEG_3D;SEG_3E;SEG_3F;SEG_3G;
#define Display_Pos_3_Digit_7   SEG_3A;SEG_3B;SEG_3C; 
#define Display_Pos_3_Digit_8   SEG_3A;SEG_3B;SEG_3C;SEG_3D;SEG_3E;SEG_3F;SEG_3G;
#define Display_Pos_3_Digit_9   SEG_3A;SEG_3B;SEG_3C;SEG_3D;SEG_3F;SEG_3G; 


#define Display_Pos_4_Digit_0   SEG_4A;SEG_4B;SEG_4C;SEG_4D;SEG_4E;SEG_4F;
#define Display_Pos_4_Digit_1   SEG_4B;SEG_4C;
#define Display_Pos_4_Digit_2   SEG_4A;SEG_4B;SEG_4D;SEG_4E;SEG_4G;
#define Display_Pos_4_Digit_3   SEG_4A;SEG_4B;SEG_4C;SEG_4D;SEG_4G;
#define Display_Pos_4_Digit_4   SEG_4B;SEG_4C;SEG_4F;SEG_4G;
#define Display_Pos_4_Digit_5   SEG_4A;SEG_4C;SEG_4D;SEG_4F;SEG_4G;
#define Display_Pos_4_Digit_6   SEG_4A;SEG_4C;SEG_4D;SEG_4E;SEG_4F;SEG_4G;
#define Display_Pos_4_Digit_7   SEG_4A;SEG_4B;SEG_4C; 
#define Display_Pos_4_Digit_8   SEG_4A;SEG_4B;SEG_4C;SEG_4D;SEG_4E;SEG_4F;SEG_4G;
#define Display_Pos_4_Digit_9   SEG_4A;SEG_4B;SEG_4C;SEG_4D;SEG_4F;SEG_4G; 


#define Display_Pos_5_Digit_0   SEG_5A;SEG_5B;SEG_5C;SEG_5D;SEG_5E;SEG_5F;
#define Display_Pos_5_Digit_1   SEG_5B;SEG_5C;
#define Display_Pos_5_Digit_2   SEG_5A;SEG_5B;SEG_5D;SEG_5E;SEG_5G;
#define Display_Pos_5_Digit_3   SEG_5A;SEG_5B;SEG_5C;SEG_5D;SEG_5G;
#define Display_Pos_5_Digit_4   SEG_5B;SEG_5C;SEG_5F;SEG_5G;
#define Display_Pos_5_Digit_5   SEG_5A;SEG_5C;SEG_5D;SEG_5F;SEG_5G;
#define Display_Pos_5_Digit_6   SEG_5A;SEG_5C;SEG_5D;SEG_5E;SEG_5F;SEG_5G;
#define Display_Pos_5_Digit_7   SEG_5A;SEG_5B;SEG_5C; 
#define Display_Pos_5_Digit_8   SEG_5A;SEG_5B;SEG_5C;SEG_5D;SEG_5E;SEG_5F;SEG_5G;
#define Display_Pos_5_Digit_9   SEG_5A;SEG_5B;SEG_5C;SEG_5D;SEG_5F;SEG_5G; 


#define Display_Pos_6_Digit_0   SEG_6A;SEG_6B;SEG_6C;SEG_6D;SEG_6E;SEG_6F;
#define Display_Pos_6_Digit_1   SEG_6B;SEG_6C;
#define Display_Pos_6_Digit_2   SEG_6A;SEG_6B;SEG_6D;SEG_6E;SEG_6G;
#define Display_Pos_6_Digit_3   SEG_6A;SEG_6B;SEG_6C;SEG_6D;SEG_6G;
#define Display_Pos_6_Digit_4   SEG_6B;SEG_6C;SEG_6F;SEG_6G;
#define Display_Pos_6_Digit_5   SEG_6A;SEG_6C;SEG_6D;SEG_6F;SEG_6G;
#define Display_Pos_6_Digit_6   SEG_6A;SEG_6C;SEG_6D;SEG_6E;SEG_6F;SEG_6G;
#define Display_Pos_6_Digit_7   SEG_6A;SEG_6B;SEG_6C; 
#define Display_Pos_6_Digit_8   SEG_6A;SEG_6B;SEG_6C;SEG_6D;SEG_6E;SEG_6F;SEG_6G;
#define Display_Pos_6_Digit_9   SEG_6A;SEG_6B;SEG_6C;SEG_6D;SEG_6F;SEG_6G; 

#define Display_Pos_7_Digit_0   SEG_7A;SEG_7B;SEG_7C;SEG_7D;SEG_7E;SEG_7F;
#define Display_Pos_7_Digit_1   SEG_7B;SEG_7C;
#define Display_Pos_7_Digit_2   SEG_7A;SEG_7B;SEG_7D;SEG_7E;SEG_7G;
#define Display_Pos_7_Digit_3   SEG_7A;SEG_7B;SEG_7C;SEG_7D;SEG_7G;
#define Display_Pos_7_Digit_4   SEG_7B;SEG_7C;SEG_7F;SEG_7G;
#define Display_Pos_7_Digit_5   SEG_7A;SEG_7C;SEG_7D;SEG_7F;SEG_7G;
#define Display_Pos_7_Digit_6   SEG_7A;SEG_7C;SEG_7D;SEG_7E;SEG_7F;SEG_7G;
#define Display_Pos_7_Digit_7   SEG_7A;SEG_7B;SEG_7C; 
#define Display_Pos_7_Digit_8   SEG_7A;SEG_7B;SEG_7C;SEG_7D;SEG_7E;SEG_7F;SEG_7G;
#define Display_Pos_7_Digit_9   SEG_7A;SEG_7B;SEG_7C;SEG_7D;SEG_7F;SEG_7G; 

#define DISPLAY_DOT      SEG1 |= 0x08;
#define DISPLAY_CLEAR    SEG1 &= 0x07; 

#define DISPLAY_HR_ICON      SEG3 |= 0x08; 
#define DISPLAY_HR_CLEAR     SEG3 &= 0x07;


/* End user code. Do not edit comment generated here */

/* Start user code for adding. Do not edit comment generated here */

void vDisplayHrIcon(ENUM_TYPE status)
{
	
switch(status)
	{
		case eSolidON:
		     DISPLAY_HR_ICON; 
		 break;
		case eOff:
		      DISPLAY_HR_CLEAR; 
		 break;
		case eBlink:
            		  
			  	
		 break;
  	
			 
		default:
		 break;
	}		
	
}

void vDisplayDigit(unsigned long Value )
{
 unsigned int loop=7;  
 unsigned char temp,ar[8];
 
	while(loop>=1)
	 {
	   temp=Value%10;
	   ar[loop]=temp;
	   Value=Value/10;
	   loop--;
	 }


   for(temp=1;temp<6;temp++)
    {
         if(ar[temp]!=0)
            break;
    } 
     
        for(loop=1;loop<temp;loop++)
          ar[loop]='0';
      
      
  	DISPLAY_DOT;     
  
     
	for(loop = 1; loop < 8; loop++)
	{   
              
		 
		 switch(loop)
		 {
 
	     case 1:
                 Clear_Pos_1
                 switch(ar[7])
                  {
		        	case 0: Display_Pos_1_Digit_0   break;
		        	case 1: Display_Pos_1_Digit_1   break;
		        	case 2: Display_Pos_1_Digit_2   break;
		        	case 3: Display_Pos_1_Digit_3   break;
		        	case 4: Display_Pos_1_Digit_4   break;
		        	case 5: Display_Pos_1_Digit_5   break;
		        	case 6: Display_Pos_1_Digit_6   break;
		        	case 7: Display_Pos_1_Digit_7   break;
		        	case 8: Display_Pos_1_Digit_8   break;
		        	case 9: Display_Pos_1_Digit_9   break;
		        	default: Clear_Pos_1			break;                   
                  } 
                 break;
          case 2: 
                 Clear_Pos_2
                 switch(ar[6])
                  {
		        	case 0: Display_Pos_2_Digit_0   break;
		        	case 1: Display_Pos_2_Digit_1   break;
		        	case 2: Display_Pos_2_Digit_2   break;
		        	case 3: Display_Pos_2_Digit_3   break;
		        	case 4: Display_Pos_2_Digit_4   break;
		        	case 5: Display_Pos_2_Digit_5   break;
		        	case 6: Display_Pos_2_Digit_6   break;
		        	case 7: Display_Pos_2_Digit_7   break;
		        	case 8: Display_Pos_2_Digit_8   break;
		        	case 9: Display_Pos_2_Digit_9   break;
		        	default: Clear_Pos_2			break;                   
                  }  
                 break;
          case 3: 
                Clear_Pos_3  
                 switch(ar[5])
                  {
		        	case 0: Display_Pos_3_Digit_0   break;
		        	case 1: Display_Pos_3_Digit_1   break;
		        	case 2: Display_Pos_3_Digit_2   break;
		        	case 3: Display_Pos_3_Digit_3   break;
		        	case 4: Display_Pos_3_Digit_4   break;
		        	case 5: Display_Pos_3_Digit_5   break;
		        	case 6: Display_Pos_3_Digit_6   break;
		        	case 7: Display_Pos_3_Digit_7   break;
		        	case 8: Display_Pos_3_Digit_8   break;
		        	case 9: Display_Pos_3_Digit_9   break;
		        	default: Clear_Pos_3			break;                   
	                  
		          }   
                     
                 break;
          case 4: 
                 Clear_Pos_4 	
                  switch(ar[4])
                  {
		        	case 0: Display_Pos_4_Digit_0   break;
		        	case 1: Display_Pos_4_Digit_1   break;
		        	case 2: Display_Pos_4_Digit_2   break;
		        	case 3: Display_Pos_4_Digit_3   break;
		        	case 4: Display_Pos_4_Digit_4   break;
		        	case 5: Display_Pos_4_Digit_5   break;
		        	case 6: Display_Pos_4_Digit_6   break;
		        	case 7: Display_Pos_4_Digit_7   break;
		        	case 8: Display_Pos_4_Digit_8   break;
		        	case 9: Display_Pos_4_Digit_9   break;
		        	default: Clear_Pos_4			break;                   	                  

                  }
                 break;
                  
          case 5: 
                   Clear_Pos_5 
                  switch(ar[3])
                   {
		        	case 0: Display_Pos_5_Digit_0   break;
		        	case 1: Display_Pos_5_Digit_1   break;
		        	case 2: Display_Pos_5_Digit_2   break;
		        	case 3: Display_Pos_5_Digit_3   break;
		        	case 4: Display_Pos_5_Digit_4   break;
		        	case 5: Display_Pos_5_Digit_5   break;
		        	case 6: Display_Pos_5_Digit_6   break;
		        	case 7: Display_Pos_5_Digit_7   break;
		        	case 8: Display_Pos_5_Digit_8   break;
		        	case 9: Display_Pos_5_Digit_9   break;
		        	default: Clear_Pos_5			break;                         
                   }
                 break;
          case 6:
                  Clear_Pos_6 
                  switch(ar[2])
                  {
		        	case 0: Display_Pos_6_Digit_0   break;
		        	case 1: Display_Pos_6_Digit_1   break;
		        	case 2: Display_Pos_6_Digit_2   break;
		        	case 3: Display_Pos_6_Digit_3   break;
		        	case 4: Display_Pos_6_Digit_4   break;
		        	case 5: Display_Pos_6_Digit_5   break;
		        	case 6: Display_Pos_6_Digit_6   break;
		        	case 7: Display_Pos_6_Digit_7   break;
		        	case 8: Display_Pos_6_Digit_8   break;
		        	case 9: Display_Pos_6_Digit_9   break;
		        	default: Clear_Pos_6		break;
                  }
                  break;
         case 7:
                  Clear_Pos_7 
                  switch(ar[1])
                  {
		        	case 0: Display_Pos_7_Digit_0   break;
		        	case 1: Display_Pos_7_Digit_1   break;
		        	case 2: Display_Pos_7_Digit_2   break;
		        	case 3: Display_Pos_7_Digit_3   break;
		        	case 4: Display_Pos_7_Digit_4   break;
		        	case 5: Display_Pos_7_Digit_5   break;
		        	case 6: Display_Pos_7_Digit_6   break;
		        	case 7: Display_Pos_7_Digit_7   break;
		        	case 8: Display_Pos_7_Digit_8   break;
		        	case 9: Display_Pos_7_Digit_9   break;
		        	default: Clear_Pos_7		break;
                  }
                  break;		  
           default:
                  break;
         
		
		 }
		 
	}


}

	




/* End user code. Do not edit comment generated here */
