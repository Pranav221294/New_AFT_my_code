/****************************************************************************************************************/
/* COPYRIGHT                                                                                                    */
/* Inidication Instruments Ltd.(c) 2022                                                                         */
/****************************************************************************************************************/
/* File name   : {file_name}                                                                                    */
/* Module      : {file_path}                                                                                    */
/*--------------------------------------------------------------------------------------------------------------*/
/* Description :                                                                                                */
/****************************************************************************************************************/
/* History                                                                                                      */
/*--------------------------------------------------------------------------------------------------------------*/
/* Date       | Revised By  | Rev. No.  | Description                                                           */
/*--------------------------------------------------------------------------------------------------------------*/
/* 
/*Observation: Last value of Average Divisor Value (liveParam_struct.div_average_f) is used when P1 level is used that is less than 20%.
(By Rahul Dhiman in testing obsevation) 
Observation: Remove 5 to 10 sec condition is not applied.Durning power up condition.(But in document it is mention in it.)          
*/
/****************************************************************************************************************/
/*Calculation of with respect of live P1 level Calculation
    
   P1 level  is greater than 20%
   
    Condition 
      Normal Average divisor live < 0.97 of Calibration_divisor
          than it will take this (0.97 of Calibration_divisor)
     
     Normal Average divisor live > 0.97 of Calibration_divisor
          than it will take this (Normal Average Divisor)

  Also save data at every 15 min in Flash (i.e.Normal Average Divisor)
        
    
                                                   P1 level  is less than 20%
                                                          ||
      ---------------------------------------------------------------------------------------------------------------
       ||                                                                                                          ||
       ||                                                                                                          ||
       ||                                                                                                          ||
//////////////////////////////////////////////////////////                                         //  ///////////////////////////////////////////
// Power on found                                       //                                         // High to Low(Diesel go High to Low) live //
// level P1 is less 20%                                 //                                         //   that last value divisor average is used //  
//                                                      //                                          // ///////////////////////////////////////////
// It is taken at last 15 minutes Normal Average Divisor //                                       
// If Average divisor value < 0.97 of Calibration_divisor //
//  than it will take this (0.97 of Calibration_divisor)//
//                                                       //
// If Average divisor value < 0.97 of Calibration_divisor//
//   than it will take this (Normal Average Divisor)     //
//   that is stored in last 15 minute                   //
///////////////////////////////////////////////////////



Document PWM range:0 to 1024
Rahul Software Range: 0 to 10000


liveParam_struct.level_fluid_Q1_f = ( 1.0f * liveParam_struct.current_level_p1 * MAX_VALUE_AT_FULL_LEVEL / liveParam_struct.currUsedDiv /storedParam_struct.span_stored) ;

MAX_VALUE_AT_FULL_LEVEL(10000)

Because level(e.g. 0.1230) comes out in decimal so we actually multiply with 10000(MAX_VALUE_AT_FULL_LEVEL).
Here use 10000 because to minimise the step or accuracy to be increased.  

Another observation also occured when it will check with water than it will give negative divisor.
It only check with 177.9 mm it working fine within range.
*/

/****************************************************************************************************************/

/****************************************************************************************************************/
/* Includes                                                                                                     */
/****************************************************************************************************************/

#include "levelCalc.h"
#include <stdio.h>
#include "r_cg_timer.h"
#include "MovingAverage.h"
#include "User.h"
/****************************************************************************************************************/
/****************************************************************************************************************/
/* Defines                                                                                                      */
/****************************************************************************************************************/

#define DIV_SAMPLES_COUNT                   16u   // 16              // divisor samples count , 16 is based on document
// #define HYSTRSIS                        2u
// #define SAMPLES_COUNTS_FOR_LEVEL        6u
#define CONST_6_3_0							6.30f

#define CONST_6_3_4096_1						((uint16_t)25804)
#define left_v_1                                   5u 
/*

Line graph between level and voltage is not completly linear
graph is something like i.e. draw below
1. from Full(F) level to 1_4 level output voltage changes almost linearly
2. and from 1_4 level to Empty low (EL) level voltage changes linearly
3. but from Full level to Empty Low level output voltage not changed linearly


            *
               *
                  *
                     *
                        *
                           *
                              *
                                 *
                                    *
                                        *
                                             *
                                                  *
                                                       *
                                                            * 
            F    3_4    1_2       1_4         EH            EL                                       
*/

/* Level values that return by levelCalc function */
#define LEVEL_FULL                      MAX_VALUE_AT_FULL_LEVEL
#define LEVEL_3_4                       7508u                                            // 7870u      // Derived after testing
#define LEVEL_1_2                       5071u                                            // 5547u      // Derived after testing
#define LEVEL_1_4                       2561u                                            // 2955u      // Derived after testing  
#define LEVEL_EMPTY_HIGH                1433u                                            // 1468u      // Derived after testing  
#define LEVEL_EMPTY_LOW                 0u

/* Corresponding output voltages(mV) at above levels. Dont change below values */
#define VOUT_AT_FULL                   5000u
#define VOUT_AT_3_4                    3850u
#define VOUT_AT_1_2                    2750u
#define VOUT_AT_1_4                    1640u
#define VOUT_AT_EMPTY_HIGH             1070u
#define VOUT_AT_EMPTY_LOW              500u



/****************************************************************************************************************/
/* Enumeration                                                                                                  */
/***************************************************************************************************************/


/****************************************************************************************************************/
/* Extern variable                                                                                                  */
/***************************************************************************************************************/

extern s_Parameter_type caliPara_struct;
extern volatile u32 freqCount ;
extern volatile u32 p0_in ;
extern volatile u32 p1_in ;
extern volatile u32 p2_in  ;
u32 p1_merge = 0, p2_merge =0,p0_merge =0 ;
u32 p1_1_2bytes = 0, p2_1_2bytes =0,p0_1_2bytes =0 ;
 u16 p0_dataTempPx = 0,p1_dataTempPx = 0,p2_dataTempPx = 0;
extern raw_probe_value p0_raw;
extern raw_probe_value p1_raw;
extern raw_probe_value p2_raw;
//s16 levelValue = 0 ;

/****************************************************************************************************************/
/* Type definitions                                                                                             */
/****************************************************************************************************************/

typedef enum
{
    e_spanGreaterThan20Percnt,
    e_spanLessThan20Percnt,
    e_spanInitial,
} spanFlag_type ;


typedef struct
{

    float div_samples[DIV_SAMPLES_COUNT];        // hold div samples for average 
    uint8_t div_sample_oneTimeRunFlag;              // flag for one time run, releted to above array

    uint16_t accCount_p0[ACCUMULATE];          // for debug only
    uint8_t accCountOneTimeFlag_p0;

    uint16_t accCount_p1[ACCUMULATE];          // for debug only
    uint8_t accCountOneTimeFlag_p1;
    
    uint16_t accCount_p2[ACCUMULATE];          // for debug only
    uint8_t accCountOneTimeFlag_p2;
    

} t_samples_type;

typedef struct 
{
   u16     p0_stored;
    u16     p1E_stored;
    u16     p2E_stored;
    u16     p1F_stored;
    u16     p2F_stored;
    u16     m_stored_u16;
    u16     div_stored_u16;
    u16     div_Avg_Stored_u16;
    float   m_stored;
    float   div_stored;
    float   div_Avg_Stored;
    float   span_stored;
    u16     p1_span_stored;
    bool    cali_status;
    u16     cali_div_97;
}s_StoredInFlash_type;

s_StoredInFlash_type storedParam_struct = {0};

/****************************************************************************************************************/
/* Local variables                                                                                              */
/****************************************************************************************************************/

s_liveParam_type liveParam_struct;
t_samples_type samples_struct;

/****************************************************************************************************************/
/* Local function declarations                                                                                  */
/****************************************************************************************************************/
/// @addtogroup MODULE_LIBRARY
/// @{
/// @defgroup ModuleLib  Private functions used 
//// 
///   
/// @addtogroup ModuleLib  
/// @{
      		
	//Local function declarations.....................................
static u16 _voltageFromLinearFormula(u16 x1_level, u16 x2_level, u16 y1_vOut, u16 y2_vOut, u16 x_level );
static u16 _GetVoltage(s16 level);
static void handlePxHysteresis(uint32_t *addP1, uint32_t *addP2);

/// @} // endgroup ModuleLib  

/// @} // endgroup MODULE_LIBRARY

/****************************************************************************************************************/
/* Local function definitions                                                                                   */
/****************************************************************************************************************/

 //*************************************************************************************************************//**
 /*
 * Purpose  	:  	This function calculate the voltage by linearity formula based on the level
 *
 * @param[in]   :	address of array
 * @return    	: 	Accumulated count value
 ****************************************************************************************************************/

static u16 _GetVoltage(s16 level)
{
    u16 voltage = 0 ; 
    if ((level >= LEVEL_EMPTY_LOW) && (level <= LEVEL_EMPTY_HIGH))
    {
        voltage = _voltageFromLinearFormula(LEVEL_EMPTY_LOW, LEVEL_EMPTY_HIGH, VOUT_AT_EMPTY_LOW, VOUT_AT_EMPTY_HIGH, level);
    }

    else if ((level >= LEVEL_EMPTY_HIGH) && (level <= LEVEL_1_4))
    {
        voltage = _voltageFromLinearFormula(LEVEL_EMPTY_HIGH, LEVEL_1_4, VOUT_AT_EMPTY_HIGH, VOUT_AT_1_4, level);
    }

    else if ((level >= LEVEL_1_4) && (level <= LEVEL_1_2))
    {
        voltage = _voltageFromLinearFormula(LEVEL_1_4, LEVEL_1_2, VOUT_AT_1_4, VOUT_AT_1_2, level);
    }

    else if ((level >= LEVEL_1_2) && (level <= LEVEL_3_4))
    {
        voltage = _voltageFromLinearFormula(LEVEL_1_2, LEVEL_3_4, VOUT_AT_1_2, VOUT_AT_3_4, level);
    }

    else if ((level >= LEVEL_3_4) && (level <= LEVEL_FULL))
    {
        voltage = _voltageFromLinearFormula(LEVEL_3_4, LEVEL_FULL, VOUT_AT_3_4, VOUT_AT_FULL, level);
    }

    else
    {}

    return voltage ;
    
}


 //*************************************************************************************************************//**
 /*
 * Purpose  	:  	This function calculate the voltage by linearity formula 
 *
 * @param[in]   :	level 1
 * @param[in]   :	level 2
 * @param[in]   :	Vout 1
 * @param[in]   :	Vout 2
 * @param[in]   :	Fuel level
 * @return    	: 	Calculated voltage
 ****************************************************************************************************************/
static u16 _voltageFromLinearFormula(u16 x1_level, u16 x2_level, u16 y1_vOut, u16 y2_vOut, u16 x_level )
{
    u16 voltage = 0 ; 

    /*  
        formula to find the equation between 2 points i.e. (x1, y1) & (x2, y2)
        y = (   ((y2 - y1) * ( x - x1 )) / (x2 - x1) +  y1 )

    */
    voltage = (   ((uint32_t)(y2_vOut - y1_vOut) * (uint32_t)( x_level - x1_level ) ) / (x2_level - x1_level) +  y1_vOut    ) ;
    return voltage ;
}



/****************************************************************************************************************/
/* Global function definitions                                                                                   */
/****************************************************************************************************************/


/*************************************************************************************************************
 *
 * Purpose  	:   This function calculate the moving average of data
 * param		:  	address of array which holds the samples of data
 * param		:	the actual live data
 * param		:	number of samples that need to be averaged
 * return   	:	moving average value
 ****************************************************************************************************************/
uint16_t getMovingAverage(uint16_t *ptrArraySamples, uint16_t data, uint8_t samplesCount)
{
    uint8_t i = 0 ;
    uint32_t sumOfSamples = 0 ;
    uint16_t average = 0 ;      // change data type accordingly


    /* calculation to reach the onetimeflag of relative array of sample */
    uint8_t *oneTimeRunFlag =  (ptrArraySamples + samplesCount );	


    /* here we are running this code for one time when function call */
    if (*oneTimeRunFlag == false)
    {
        *oneTimeRunFlag = true;
        for ( i = 0 ; i < samplesCount; i++)
        {
            *(ptrArraySamples + i) = data ;
        }
    }

    else if (*oneTimeRunFlag == true)
    {

	/* here data is shifting left in the array */
        for (i = 0 ; i < samplesCount - 1; i++)
        {
            *(ptrArraySamples + i) = *(ptrArraySamples + i + 1) ;
        }
        *(ptrArraySamples + i) = data ;
    }


    /* Here we are calculating the average */
    for ( i = 0 ; i < samplesCount; i++)
    {
        sumOfSamples += *(ptrArraySamples + i);
    }

    average = sumOfSamples / samplesCount ;

    return average ;

}


/*************************************************************************************************************
 *
 * Purpose  	:   This function calculate the moving average of float data
 * param		:  	address of array which holds the samples of float data
 * param		:	the actual live float data
 * param		:	number of samples that need to be averaged
 * return   	:	float type moving average value
 ****************************************************************************************************************/
float getMovingAverageFloat(float *ptrArraySamples, float data, uint8_t samplesCount)
{
    uint8_t i = 0 ;
    float sumOfSamples = 0 ;
    float average = 0 ;      // change data type accordingly

    
    /* calculation to reach the onetimeflag of relative array of sample */
    uint8_t *oneTimeRunFlag =  (ptrArraySamples + samplesCount  );

    data = getFloatUpto3place(data);

    /* here we are running this code for one time when function call */
    if (*oneTimeRunFlag == false)
    {
        *oneTimeRunFlag = true;
        for ( i = 0 ; i < samplesCount; i++)
        {
            *(ptrArraySamples + i) = data ;
        }
    }

    else if (*oneTimeRunFlag == true)
    {

	/* here data is shifting left in the array */
        for (i = 0 ; i < samplesCount - 1; i++)
        {
            *(ptrArraySamples + i) = *(ptrArraySamples + i + 1) ;
        }
        *(ptrArraySamples + i) = data ;
    }

    //printData(ptrArraySamples, samplesCount, sizeof(uint16_t));
    /* Here we are calculating the average */
    for ( i = 0 ; i < samplesCount; i++)
    {
        sumOfSamples += *(ptrArraySamples + i);
    }

    average = sumOfSamples / samplesCount ;

    average = getFloatUpto3place(average) ;
    return average ;

}


 //*************************************************************************************************************//**
 /*
 * Purpose  	:  	This function calculate the current level of fuel in the range of 0 - 10000
 *
 * @param[in]   :	None
 * @return    	: 	u16, fuel level
 ****************************************************************************************************************/

float debugCurrUsedDiv = 0 ;        // for debug only
s16 calculateFuelLevel(void)
{
    uint8_t resultAvg_8u = 0 ; 
    u16 dataTempPx = 0 ;        // testing
    u8 dataTempoverFlow = 0 ;  

    uint16_t resultAvg = 0 ;    // testing
  //  u16 dataTempPx = 0 ;        // testing
    uint16_t resultAvg_u16 = 0 ; 
    s16 levelValue = 0 ;
    static u32 timeStamp = 0;
    static u32 timeStamp_2 = 0 ;
    static u8 firstTimeEntry = 0 ;
    static u16 tempCountPx[3] ;
    //static u8 index = 0 ;
    static spanFlag_type e_spanLow20Flag = e_spanLessThan20Percnt ;
    static spanFlag_type e_spanLow  = e_spanInitial;
    static spanFlag_type e_spanHigh = e_spanInitial;
    
    //static float divAvgLess20perc = 0 ;
    static s16 lastP1 = -1 ;
    static s16 lastP2 = -1 ;
    float tempDivAvg = 0 ; 

    
    bool is_live_div_neg = false;
    u32 tempP1 = p1_in ; 
    u32 tempP2 = p2_in ; 

//     u16 m = 2433;
//     u16 p1 = 10304;
//     u16 p2 = 5293;
//     u16 lz;
    /* Here reading data from flash */
    if (firstTimeEntry == 0 )
    {
        storedParam_struct.p0_stored        =   vRead_P0_cal_fromFlash();
        storedParam_struct.p1E_stored       =   vRead_P1E_cal_fromFlash();
        storedParam_struct.p2E_stored       =   vRead_P2E_cal_fromFlash();
        storedParam_struct.p1F_stored       =   vRead_P1F_cal_fromFlash();
        storedParam_struct.p2F_stored       =   vRead_P2F_cal_fromFlash();
       // storedParam_struct.m_stored       =   vRead_m_fromFlash();
        storedParam_struct.m_stored_u16     =    vRead_m_u16_fromFlash();
        storedParam_struct.div_stored       =   vRead_Div_cal_fromFlash();
        storedParam_struct.span_stored      =   vRead_Span_cal_fromFlash();
        storedParam_struct.cali_status      =   vRead_Cali_Status_fromFlash();
        storedParam_struct.div_stored_u16   =   vRead_Div_cal_u16_fromFlash();
        storedParam_struct.p1_span_stored   =   vRead_Span_cal_u16_fromFlash();
        firstTimeEntry = 1 ;
    }

    storedParam_struct.div_Avg_Stored   =   vRead_Div_Avg_Store_fromFlash();

    // Calculating raw_p1_span
    caliPara_struct.raw_p1_span = storedParam_struct.p1F_stored - storedParam_struct.p1E_stored ;
    storedParam_struct.cali_div_97 =  ((storedParam_struct.div_stored_u16)/32);
    storedParam_struct.cali_div_97 = storedParam_struct.div_stored_u16 - storedParam_struct.cali_div_97;

    ///////////////////////////////////////////////////////////////////// for debug only ////////////////////
    // caliPara_struct.p0_empty            =   9880;               //9629;
    // storedParam_struct.p0_stored        =   caliPara_struct.p0_empty;

    // caliPara_struct.p1_empty            =   23462;               //10986;
    // storedParam_struct.p1E_stored       =   caliPara_struct.p1_empty ;

    // caliPara_struct.p2_empty            =   25249;               //11111;
    // storedParam_struct.p2E_stored       =  caliPara_struct.p2_empty ;

    // caliPara_struct.p1_full             =    25404;               //11198;
    // storedParam_struct.p1F_stored       =  caliPara_struct.p1_full ;

    // caliPara_struct.p2_full             =    27313;               //11335;
    // storedParam_struct.p2F_stored       =   caliPara_struct.p2_full ;

	// caliPara_struct.raw_p1_span = caliPara_struct.p1_full - caliPara_struct.p1_empty ;	// store in EEPROM

    // /* get value of raw_p2_span */
    // caliPara_struct.raw_p2_span = caliPara_struct.p2_full - caliPara_struct.p2_empty ;	// store in EEPROM
    
    // /* associativity in original expression */
    // caliPara_struct.constant_m_f        = (14.4 - ( (7.07 * caliPara_struct.raw_p1_span ) / (caliPara_struct.raw_p2_span)))  ; // store in EEPROM
    // storedParam_struct.m_stored         =   caliPara_struct.constant_m_f ;

    // /* calculate div at cali value, devide by 100 as in above expressio multiply by 100 */   // store in EEPROM
    // caliPara_struct.div_cal_f		    = (((caliPara_struct.constant_m_f * caliPara_struct.raw_p1_span) / ( caliPara_struct.raw_p2_span)) - 6.3)  ;
    // storedParam_struct.div_stored       =    caliPara_struct.div_cal_f	;

    // /* calculate the p1_span */
    // caliPara_struct.p1_span_f		    = ( (caliPara_struct.raw_p1_span ) / caliPara_struct.div_cal_f)  ;  // store in EEPROM
    // storedParam_struct.span_stored      =  caliPara_struct.p1_span_f ;



///////////////////////////////////////////////////////////////////////


    /* For first entry */
    if (timeStamp == 0 )
    {
        timeStamp = u32GetMilTick();
    }

    /* For first entry */
    if (timeStamp_2 == 0)
    {
        timeStamp_2 = u32GetMilTick();
    }
    
    //handlePxHysteresis(&tempP1, &tempP2);

    /* get p0 live counts */
  // liveParam_struct.p0_live = storedParam_struct.p0_stored;
 //  dataTempoverFlow = p0_raw.TD10_overflow_v ;

   p0_merge = merge_of_overflow_n_reg_value(p0_raw.TD10_overflow_v,p0_raw.TD10_value_reg_v);
   dataTempPx = ((uint16_t)(shifting_of_merge_overflow_n_reg_v( p0_raw.TD10_overflow_v,p0_raw.TD10_value_reg_v,4))); 
   p0_dataTempPx = ((uint16_t)(shifting_of_merge_overflow_n_reg_v( p0_raw.TD10_overflow_v,p0_raw.TD10_value_reg_v,0)));;
   getMovingAveargeOf(e_LiveP0, &dataTempPx, &resultAvg);
    
    liveParam_struct.p0_live = resultAvg  ;
  // liveParam_struct.p0_live = (u16)p0_1_2bytes;//dataTempPx  ;
   
  // liveParam_struct.p0_live =  10004;
  
  // liveParam_struct.p0_live = ((uint16_t)(shifting_of_merge_overflow_n_reg_v( p0_raw.TD10_overflow_v,p0_raw.TD10_value_reg_v,4))) ;
       //liveParam_struct.p0_live = VALUE_FOR_SUBSTRACTION - p0_in ;

    
    /* get p1 live counts */
    // dataTempPx = VALUE_FOR_SUBSTRACTION - tempP1 ;
    p1_merge = merge_of_overflow_n_reg_value(p1_raw.TD10_overflow_v,p1_raw.TD10_value_reg_v);
    p1_merge = p1_merge >> 8; 
    dataTempPx = (u16)p1_merge ;
    getMovingAveargeOf(e_LiveP1, &dataTempPx, &resultAvg);
    //liveParam_struct.p1_live = getAccumalatedCount(samples_struct.accCount_p1, VALUE_FOR_SUBSTRACTION - tempP1, ACCUMULATE);
    liveParam_struct.p1_live = getAccumalatedCount(samples_struct.accCount_p1, resultAvg, 16);

   // dataTempPx = ((uint16_t)(shifting_of_merge_overflow_n_reg_v( p1_raw.TD10_overflow_v,p1_raw.TD10_value_reg_v,4))); 
   // p1_dataTempPx = ((uint16_t)(shifting_of_merge_overflow_n_reg_v( p1_raw.TD10_overflow_v,p1_raw.TD10_value_reg_v,0)));
    //getMovingAveargeOf(e_LiveP1, &dataTempPx, &resultAvg);
   // liveParam_struct.p1_live = getAccumalatedCount(samples_struct.accCount_p1, VALUE_FOR_SUBSTRACTION - tempP1, ACCUMULATE);
     //liveParam_struct.p1_live = getAccumalatedCount(samples_struct.accCount_p1, resultAvg, ACCUMULATE);
     
    //   liveParam_struct.p1_live =  resultAvg ;
    //  //liveParam_struct.p1_live = (u16)p1_1_2bytes; //dataTempPx  ;
    //  if(liveParam_struct.p1_live >=  storedParam_struct.p1F_stored)
    //    {
    //     liveParam_struct.p1_live = storedParam_struct.p1F_stored;
    //    }
      //  liveParam_struct.p1_live = 24271;
    
  
    
  /* get p2 live counts */
     // dataTempPx = VALUE_FOR_SUBSTRACTION - tempP2 ;
     p2_merge = merge_of_overflow_n_reg_value(p2_raw.TD10_overflow_v,p2_raw.TD10_value_reg_v);
     p2_merge = p2_merge >> 8;
     dataTempPx = (u16)p2_merge;
     getMovingAveargeOf(e_LiveP2, &dataTempPx, &resultAvg);
    //liveParam_struct.p2_live = getAccumalatedCount(samples_struct.accCount_p2, VALUE_FOR_SUBSTRACTION - tempP2, ACCUMULATE);
    liveParam_struct.p2_live = getAccumalatedCount(samples_struct.accCount_p2, resultAvg, 16);
    
    //   dataTempPx = ((uint16_t)(shifting_of_merge_overflow_n_reg_v( p2_raw.TD10_overflow_v,p2_raw.TD10_value_reg_v,4))); 
    //   p2_dataTempPx = ((uint16_t)(shifting_of_merge_overflow_n_reg_v( p2_raw.TD10_overflow_v,p2_raw.TD10_value_reg_v,0)));
    //   getMovingAveargeOf(e_LiveP2, &dataTempPx, &resultAvg);
    //   //liveParam_struct.p2_live = getAccumalatedCount(samples_struct.accCount_p2, VALUE_FOR_SUBSTRACTION - tempP2, ACCUMULATE);
    //   liveParam_struct.p2_live = getAccumalatedCount(samples_struct.accCount_p2, resultAvg, ACCUMULATE);
  //    liveParam_struct.p2_live = 15017;
       
//       liveParam_struct.p2_live = resultAvg;
//    // liveParam_struct.p2_live= (u16)p2_1_2bytes;
//     if(liveParam_struct.p2_live >=  storedParam_struct.p2F_stored)
//        {
//         liveParam_struct.p2_live = storedParam_struct.p2F_stored;
//        }              

/* get p1 live counts */
    // dataTempPx = VALUE_FOR_SUBSTRACTION - tempP1 ;
    // getMovingAveargeOf(e_LiveP1, &dataTempPx, &resultAvg);
    // //liveParam_struct.p1_live = getAccumalatedCount(samples_struct.accCount_p1, VALUE_FOR_SUBSTRACTION - tempP1, ACCUMULATE);
    // liveParam_struct.p1_live = getAccumalatedCount(samples_struct.accCount_p1, resultAvg, ACCUMULATE);

    /* get p2 live counts */
    // dataTempPx = VALUE_FOR_SUBSTRACTION - tempP2 ;
    // getMovingAveargeOf(e_LiveP2, &dataTempPx, &resultAvg);
    // //liveParam_struct.p2_live = getAccumalatedCount(samples_struct.accCount_p2, VALUE_FOR_SUBSTRACTION - tempP2, ACCUMULATE);
    // liveParam_struct.p2_live = getAccumalatedCount(samples_struct.accCount_p2, resultAvg, ACCUMULATE);
    

//   // liveParam_struct.p1_live = 17507;
       // liveParam_struct.p2_live = 19917;
//     // if(lastP1 == -1)
//     // {
//     //     lastP1 = liveParam_struct.p1_live ;
//     // }

//     // if (lastP2 == -1)
//     // {
//     //     lastP2 = liveParam_struct.p2_live ;
//     // }

//     // /* handle hysterisis for p1 live */
//     // if ((liveParam_struct.p1_live > (lastP1 - 1)) && (liveParam_struct.p1_live < (lastP1 + 1)) )
//     // {
//     //     liveParam_struct.p1_live = lastP1;
//     // }
//     // else
//     // {
//     //     lastP1 = liveParam_struct.p1_live ;
//     // }

//     // /* handle hysterisis for p2 live */
//     // if ((liveParam_struct.p2_live > (lastP2 - 1)) && (liveParam_struct.p2_live < (lastP2 + 1)) )
//     // {
//     //     liveParam_struct.p2_live = lastP2;
//     // }
//     // else
//     // {
//     //     lastP2 = liveParam_struct.p2_live ;
//     // }
    
//     /**********************************************************************************************
//      * ******************************** Temperature Effect ****************************************
//      * *******************************************************************************************/

    /* Temperature effects on Electronics. */
    /* calculate p0_x */                                                                            
    liveParam_struct.p0_x = liveParam_struct.p0_live - storedParam_struct.p0_stored;                   

    
    /* if above value is negative then set tha negative flag */
    if (liveParam_struct.p0_x < 0)
    {
        liveParam_struct.negative_flag = true ;

        /* create compliment */
        //liveParam_struct.p0_x = (liveParam_struct.p0_x ^ 0xffff) + 1 ;
    }
    else
    {
        liveParam_struct.negative_flag = false ;
    }

//     /**********************************************************************************************/

    
//     /* caliPara_struct.p0_empty will be fetched from EEPROM */
//     /* Temperature effects on Probe 1. Value of constant, 1.4, is based on empirical data. Value will be negative if negative flag set */
         liveParam_struct.p1_drift_factor = (1.4 * (int)(liveParam_struct.p0_live - storedParam_struct.p0_stored));
 
//     /* Temperature effects on Probe 2. Value of constant, 1.05, is based on empirical data. Value will be negative if negative flag set */
       liveParam_struct.p2_drift_factor = (1.05 * (int)(liveParam_struct.p0_live - storedParam_struct.p0_stored)) ;

//     /* Current level in Probe 1 */
       liveParam_struct.current_level_p1 = liveParam_struct.p1_live - storedParam_struct.p1E_stored;
       
       if(liveParam_struct.negative_flag == false)
       {
        liveParam_struct.current_level_p1 = liveParam_struct.current_level_p1 - liveParam_struct.p1_drift_factor;
       }
       else if (liveParam_struct.negative_flag == true)
       {
        liveParam_struct.current_level_p1 = liveParam_struct.current_level_p1 + liveParam_struct.p1_drift_factor;
       }
       

//     /* Current level in Probe 2 */
      liveParam_struct.current_level_p2 = liveParam_struct.p2_live - storedParam_struct.p2E_stored ;
      if(liveParam_struct.negative_flag == false)
       {
        liveParam_struct.current_level_p2 = liveParam_struct.current_level_p2 - liveParam_struct.p2_drift_factor;
       }
       else if (liveParam_struct.negative_flag == true)
       {
        liveParam_struct.current_level_p2 = liveParam_struct.current_level_p2 + liveParam_struct.p2_drift_factor;
       }
       


//     ///////////// for debug //////////////
    if (liveParam_struct.current_level_p1 < 0 )
    {
        liveParam_struct.current_level_p1 = 0 ;
    }

    if (liveParam_struct.current_level_p2 < 0 )
    {
        liveParam_struct.current_level_p2 = 0 ;
    }

    if ( (liveParam_struct.current_level_p1)  < ( 0.2f * caliPara_struct.raw_p1_span))
    {
    //liveParam_struct.div_average_f = liveParam_struct.div_avg_store_f ;  /* copy from EEPROM */

        //liveParam_struct.div_average_f = getFloatUpto3place(storedParam_struct.div_Avg_Stored); // comment because last avg divisor used below 20%
    
        e_spanLow20Flag = e_spanLessThan20Percnt ;     // span is less than 20%
        e_spanLow       = e_spanLessThan20Percnt ;
        /* Reset timeStamp used to store divisor after every 15 minutes if span below 20 %, means dont store div if level less than 20 % */
        timeStamp = u32GetMilTick() ;

        
    }
    else
    {
        e_spanLow20Flag = e_spanGreaterThan20Percnt ;     // span is greater than 20%
        e_spanHigh      = e_spanGreaterThan20Percnt ;
    }

    ///////////////////////////////////////



//  liveParam_struct.live_div_u16 =  ((uint32_t)storedParam_struct.m_stored_u16*(uint32_t)liveParam_struct.current_level_p1/(uint32_t)liveParam_struct.current_level_p2) - CONST_6_3_4096_1;
  
 
  //liveParam_struct.div_average_97 = ((uint32_t)storedParam_struct.div_stored_u16/32);
//  liveParam_struct.div_average_97 = (44479/32);
  // liveParam_struct.div_average_97 = storedParam_struct.div_stored_u16 - liveParam_struct.div_average_97; 
  // liveParam_struct.level_fluid_Q1 = (((uint32_t)((((uint32_t)liveParam_struct.current_level_p1_new)/(uint32_t)liveParam_struct.div_average_97)*460))/storedParam_struct.p1_span_stored);
  // liveParam_struct.level_fluid_Q1 =  liveParam_struct.level_fluid_Q1 * 460;
  liveParam_struct.live_div_u16 =  ((uint32_t)storedParam_struct.m_stored_u16*(uint32_t)liveParam_struct.current_level_p1/(uint32_t)liveParam_struct.current_level_p2) - CONST_6_3_4096_1;
//   if((int32_t)liveParam_struct.live_div_u16  <  0 )// This is used to check whether live divisor is coming out to be negtive than it will used calibrated stored divisor only
//        {                                   //Only observation in case of negative divisor is water only only test for 177 mm variant 
                                         
// /        is_live_div_neg = true;
     
//       }   
//      else
       
//         is_live_div_neg = false;

    //liveParam_struct.live_div_f = getFloatUpto3place(liveParam_struct.live_div_f);
  // liveParam_struct.live_div_u16 =  ((uint32_t)storedParam_struct.m_stored_u16*(uint32_t)liveParam_struct.current_level_p1/(uint32_t)liveParam_struct.current_level_p2) - CONST_6_3_4096_1;
    /* Calculating Average divisor , stop calculation if span less than 20% */
    // if (e_spanLow20Flag != e_spanLessThan20Percnt) 
    // {
    //     liveParam_struct.div_average_f = getMovingAverageFloat(samples_struct.div_samples, liveParam_struct.live_div_f, DIV_SAMPLES_COUNT) ;
    // }

    
    if (e_spanLow20Flag != e_spanLessThan20Percnt)
    {
        //tempDivAvg = getMovingAverageFloat(samples_struct.div_samples, liveParam_struct.live_div_f, DIV_SAMPLES_COUNT) ;
     //   getMovingAveargeOf(e_LiveDivisor, &liveParam_struct.live_div_f, &tempDivAvg);
        getMovingAveargeOf(e_LiveDivisor_u16, &liveParam_struct.live_div_u16, &resultAvg_u16);
        liveParam_struct.div_average_u16 = resultAvg_u16 ;
    } 

    liveParam_struct.currUsedDiv_u16 = liveParam_struct.div_average_u16; 
    
    /* Lower limitation on value of divisor is 0.97 x div@cal */
    if (liveParam_struct.div_average_u16 < (storedParam_struct.cali_div_97) && ( e_spanLow20Flag == e_spanGreaterThan20Percnt ))
    {
        liveParam_struct.div_average_u16 = storedParam_struct.cali_div_97 ;    // tempcomm
        liveParam_struct.currUsedDiv_u16 = storedParam_struct.cali_div_97 ;
       // liveParam_struct.currUsedDiv = getFloatUpto3place(liveParam_struct.currUsedDiv);
    }

    /* Every 15 minutes (900 Seconds ) the average value of divisor is stored to EEPROM. The EEPROM has 1 x 10^6 write cycles 
    so assuming 24-hour operation the probe has a life of 28.5 years. */
    if ( (u32GetMilTick() - timeStamp ) > 900000ul )           //   15 Minutes = 900000 mSec 
    {
        timeStamp = u32GetMilTick() ;

        /* Need to Store this value in EEPROM */
        //liveParam_struct.div_avg_store_f = liveParam_struct.div_average_f ;
       
        vSave_Div_Avg_u16_Store_inFlash(liveParam_struct.div_average_u16);
    }


    #if 0
    /* At low immersion levels the divisor calculation can be affected by turbulence and mechanical variations (probe eccentricity). 
    To stop the calculation at low levels, a 20% diesel fill limitation is used 
    then ignore div_average and use div_average_store                          */
    if ( (liveParam_struct.current_level_p1)  < ( 0.2f * caliPara_struct.raw_p1_span))
    {
       //liveParam_struct.div_average_f = liveParam_struct.div_avg_store_f ;  /* copy from EEPROM */

        //liveParam_struct.div_average_f = getFloatUpto3place(storedParam_struct.div_Avg_Stored); // comment because last avg divisor used below 20%
    
        e_spanLow20Flag = e_spanLessThan20Percnt ;     // span is less than 20%

        /* Reset timeStamp used to store divisor after every 15 minutes if span below 20 %, means dont store div if level less than 20 % */
        timeStamp = u32GetMilTick() ;

        
    }
    else
    {
        e_spanLow20Flag = e_spanGreaterThan20Percnt ;     // span is greater than 20%
    }

    // not used in existing
    

    /* At power up condition, before a new divisor can be calculated, stored divisor is used. In the case of switching off and refilling
     the tank with a different fluid, an error might be apparent for first 5-10 seconds after power up while recalculation occurs. */
    if ( (u32GetMilTick() - timeStamp_2) < 5000u )
    {
        
        /* use div value stored in EEPROM */
        //liveParam_struct.div_average_f = liveParam_struct.div_avg_store_f ;
        //liveParam_struct.div_average_f = storedParam_struct.div_Avg_Stored;   // temp comment
        liveParam_struct.currUsedDiv_u16  = storedParam_struct.div_Avg_Stored_u16 ;
    }
    #endif

    /* Below code handle the condition if span is below 20% while power up the system then use the 
    *  stored avg div for calculations. if store avg div is less than 97% of stored div then use 97% of stored div
    *  This condition is observed in existing Fozmula software.
    */
    if (e_spanLow == e_spanLessThan20Percnt && e_spanHigh == e_spanInitial)
    {
        if (storedParam_struct.div_Avg_Stored_u16 < (storedParam_struct.cali_div_97))
        {
            liveParam_struct.currUsedDiv_u16  = storedParam_struct.cali_div_97 ;
        }
        else
        {
             liveParam_struct.currUsedDiv_u16  = storedParam_struct.div_Avg_Stored_u16 ;
        }
    }

    /* currently used divisor */
    //liveParam_struct.currUsedDiv = liveParam_struct.div_average_f ; 
     

    //debugCurrUsedDiv = liveParam_struct.div_average_f ;
    /* Calculate level of fluid */
    // if(is_live_div_neg == true)// This is used to check whether live divisor is coming out to be negtive than it will used calibrated stored divisor only
    // {                          //Only observation in case of negative divisor is water only test for 177 mm variant.
    // liveParam_struct.currUsedDiv = storedParam_struct.div_stored;
    // }
   
   // liveParam_struct.level_fluid_Q1_f = ( 1.0f * liveParam_struct.current_level_p1 * MAX_VALUE_AT_FULL_LEVEL / liveParam_struct.currUsedDiv /storedParam_struct.span_stored) ;

     liveParam_struct.current_level_p1_new =((uint32_t) liveParam_struct.current_level_p1 * 4096);

    // liveParam_struct.current_level_p1_new = 4169728;
    // liveParam_struct.currUsedDiv_u16 = 3895;
    // storedParam_struct.p1_span_stored = 1044;

//    liveParam_struct.level_fluid_Q1 = (((uint32_t)((((uint32_t)liveParam_struct.current_level_p1_new)/(uint32_t)liveParam_struct.currUsedDiv)*460))/storedParam_struct.p1_span_stored);
   liveParam_struct.level_fluid_Q1 = (((uint32_t)((((uint32_t)liveParam_struct.current_level_p1_new)/(uint32_t)liveParam_struct.currUsedDiv_u16)*460))/storedParam_struct.p1_span_stored);



   liveParam_struct.level_fluid_Q1_f = ((float)liveParam_struct.current_level_p1_new /(float)liveParam_struct.currUsedDiv_u16) * 460.0 /(float)storedParam_struct.p1_span_stored;
   //liveParam_struct.level_percentage =  (liveParam_struct.level_fluid_Q1_f / 460.0) * 100.0;
   liveParam_struct.level_percentage =  (liveParam_struct.level_fluid_Q1 / 460.0) * 100.0;
   liveParam_struct.level_percentage_into_100 = liveParam_struct.level_percentage * 100;
    /* Store int part only */
   // levelValue = liveParam_struct.level_fluid_Q1_f ;
      //levelValue = liveParam_struct.level_fluid_Q1 ;
       levelValue = liveParam_struct.level_percentage_into_100 ;
    // if value is negative make it 0
    if (levelValue < 0)
    {
        levelValue = 0 ;
    }

    /* set max value */    
    if (levelValue > MAX_VALUE_AT_FULL_LEVEL)
    {
        levelValue = MAX_VALUE_AT_FULL_LEVEL ;
    }

    return levelValue ;

}

 //*************************************************************************************************************//**
 /*
 * Purpose  	:  	This function sets the output voltage based on fuel level
 *
 * @param[in]   :	Fuel level
 * @return    	: 	void
 ****************************************************************************************************************/
 void vSetPWM(s16 levelValue)
 {
     u16 voltageCal = 0 ;
     
     if(levelValue > 10000)levelValue = 10000;
     

     
     
     vGetCurveDataValue(eFuelLevelToVoltage, levelValue, &voltageCal);
     if (voltageCal > VOUT_AT_FULL)
     {
         voltageCal = VOUT_AT_FULL;
     }

     vSetOutputVoltage(voltageCal);   

 }

//void vSetPWM(s16 levelValue)
//{
//    u16 voltageCal = 0 ;
//    //voltageCal = _GetVoltage(levelValue);
//  //  vGetCurveDataValue(eFuelLevelToVoltage, levelValue, &voltageCal);
//    // if (voltageCal > VOUT_AT_FULL)
//    // {
//    //     voltageCal = VOUT_AT_FULL;
//    // }
//    if (levelValue < 50) levelValue  = 50;
//      voltageCal = levelValue * 5 + 50; 
// 
//   
//    vSetOutputVoltage(voltageCal);   
//
//}


 //*************************************************************************************************************//**
 /*
 * Purpose  	:  	This function accumulate the counts, and return the current accumulated value
 *
 * @param[in]   :	address of array
 * @param[in]   :	live data
 * @param[in]   :	Size of Array
 * @return    	: 	Accumulated count value
 ****************************************************************************************************************/

uint32_t getAccumalatedCount(uint16_t *ptrArraySamples, uint16_t data, uint8_t samplesCount)
{
    uint8_t i = 0 ;
    uint32_t sumOfSamples = 0 ;
    uint16_t average = 0 ;      // change data type accordingly


    /* calculation to reach the onetimeflag of relative array of sample */
    uint8_t *oneTimeRunFlag =  (ptrArraySamples + samplesCount );	


    /* here we are running this code for one time when function call */
    if (*oneTimeRunFlag == false)
    {
        *oneTimeRunFlag = true;
        for ( i = 0 ; i < samplesCount; i++)
        {
            *(ptrArraySamples + i) = data ;
        }
    }

    else if (*oneTimeRunFlag == true)
    {

	/* here data is shifting left in the array */
        for (i = 0 ; i < samplesCount - 1; i++)
        {
            *(ptrArraySamples + i) = *(ptrArraySamples + i + 1) ;
        }
        *(ptrArraySamples + i) = data ;
    }


    /* Here we are calculating the average */
    for ( i = 0 ; i < samplesCount; i++)
    {
        sumOfSamples += *(ptrArraySamples + i);
    }

    //average = sumOfSamples / samplesCount ;

    return sumOfSamples ;

}


static void handlePxHysteresis(uint32_t *addP1, uint32_t *addP2)
{
    static u32 lastP1 = 0 ;
    static u32 lastP2 = 0 ;
    
    if (lastP1 == 0 )
    {
        lastP1 = *addP1 ;
    }

    if (lastP2 == 0)
    {
        lastP2 = *addP2 ;
    }

    /* Hysteresis handle for p1 */
    if ( ((*addP1 > (lastP1 + 1)) || (*addP1 < (lastP1 - 1)) )  )
    {
        lastP1 = *addP1 ;
        //lastP2 = *addP2 ;
    }

    else
    {
        *addP1 = lastP1 ;
        //*addP2 = lastP2 ;
    }

    /* Hysteresis handle for p2 */
    if ( (*addP2 > (lastP2 + 1)) || (*addP2 < (lastP2 - 1)) )
    {
        lastP2 = *addP2 ;
    }
    else
    {
        *addP2 = lastP2 ;
    }


}

/****************************************************************************************************************/
/* End of file {file_name}                                                                                      */ 
/****************************************************************************************************************/

