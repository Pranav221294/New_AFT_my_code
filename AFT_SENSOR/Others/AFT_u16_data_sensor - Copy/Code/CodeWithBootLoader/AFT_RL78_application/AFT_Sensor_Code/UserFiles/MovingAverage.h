
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
    e_numberOfData,             /*dont delete this */
}dataName_eType;


void getMovingAveargeOf(dataName_eType e_DataName, const void *dataIn, void *getMovAvg);


#endif


