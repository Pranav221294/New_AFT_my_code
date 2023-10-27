
/*
    Author : Rahul Dhiman
*/
#ifndef __MOVING_AVERAGE_H__
#define __MOVING_AVERAGE_H__

#include "r_cg_macrodriver.h"
#include "CustomDataTypes.h"

/* In the below enum write the descriptive name of data which is similar to array in which data want to store */
typedef enum
{

    /*e_demoDataName, */          
    e_LiveDivisor,
    e_p0AtCalibration,
    e_LiveP1,
    e_LiveP2,
    e_LiveP0,
    e_TD10_overflow_P0,
    e_TD10_overflow_P1,
    e_TD10_overflow_P2,
    e_TD10_value_reg_P0,
    e_TD10_value_reg_P1,
    e_TD10_value_reg_P2,
    e_LiveDivisor_u16,            /*dont delete this */
    e_p1AtemptyCalibration,
    e_p2AtemptyCalibration,
    e_p1AtfullCalibration,
    e_p2AtfullCalibration,  
    e_raw_p0,
    e_raw_p1,
    e_raw_p2,
    e_numberOfData,

}dataName_eType;


void getMovingAveargeOf(dataName_eType e_DataName, const void *dataIn, void *getMovAvg);


#endif


