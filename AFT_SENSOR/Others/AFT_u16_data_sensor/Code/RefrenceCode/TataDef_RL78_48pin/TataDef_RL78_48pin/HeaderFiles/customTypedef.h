/** ---------------------------------------------------------------------------
 **  COPYRIGHT: Indication Instruments Ltd
 **             Faridabad Haryana
 **             India
 **		   2018	
 ** ---------------------------------------------------------------------------
 **
 **  Revision History
 **
 **  Date       			Name             						Description
 ** ==========  ===============  ==============================================
 ** 
 ** 19/9/2017	Rajni Rajpoot       	Initial Version of CustomTyprDef
 ** 5/9/2017		Rajni Rajpoot	      	Comment typedef bool for freescale controller as its already
 **                                 			define in PE_Types.h
 ** 							       extern commom variable in this file only.
 ** 10/4/2018	Vishnu Jat		Removed external variable. Used typedef instead of #define
 **				Khusboo Lalani
 **10/04/2018  Vishnu  Jat              Correct format of typedef
 ** ***********************************************************************************/                    

#ifndef 	_CUSTOM_TYPE_DEF_H
#define     _CUSTOM_TYPE_DEF_H

#include "r_cg_macrodriver.h"

typedef 	unsigned char		u8;
typedef 	unsigned int		u16;
typedef 	unsigned long		u32;
typedef 	signed char			s8;
typedef 	signed int			s16;
typedef 	signed long			s32;
typedef		float				f32;
typedef		double				f64;
typedef		unsigned char 	    bool;


#define     TRUE            1
#define     FALSE           0
#define     HIGH            1
#define     LOW             0



#endif


