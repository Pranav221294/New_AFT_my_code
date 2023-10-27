
/*
    Author : Rahul Dhiman
*/

#include "MovingAverage.h"

/************************************ Typedef *******************************/
typedef enum
{
    e_int8_type,
    e_int16_type,
    e_int32_type,
    e_uint8_type,
    e_uint16_type,
    e_uint32_type,
    e_float_type,
}e_dataType_eType;

typedef struct
{
    dataName_eType e_Dataname;
    void *addOfArray;
    e_dataType_eType dataType;
    uint16_t sizeOfDataArray;
}dataDetails_sType;

/*-----------------------------------------------------------------------------*/

/******************* Local variables *******************************************/
static bool firstTimeRunflags[e_numberOfData] = {false};
static uint32_t sumU32 = 0 ;
static int32_t  sumI32 = 0 ; 
static float    sumFloat = 0 ;

/*------------------------------------------------------------------------------*/

/******************************** Local functions ***************************************/
static void copyDataFirstTime(const uint8_t indexOfRow, const void *dataIn);
static void shiftDataInArray(const uint8_t indexOfRow, const void *dataIn);
static void calculateAveargeOfArray(const uint8_t indexOfRow, void *getFinalMovAvg);
static void calculateMovingAverage(const uint8_t indexOfRow, const void *dataIn, void *getMovAvg);

/*------------------------------------------------------------------------------*/


/******************************** add array of samples data for moving average and sample Size *************************/

/* Here write the Samples Size */
//#define DEMO_SENSOR_SAMPLES                5u
#define DIVISOR_SAMPLE_SIZE                     16u
#define P0_CALIBRATION_SAMPLE_SIZE              16u
#define P1_LIVE_SAMPLE_SIZE                     16U
#define P2_LIVE_SAMPLE_SIZE                     16U
#define raw_p0_array                     40U
/* Here add the array in which data will be stored */
//static float demoSensorData[DEMO_SENSOR_SAMPLES];
float liveDivisorArray[DIVISOR_SAMPLE_SIZE];
uint16_t P0_atCalibration[P0_CALIBRATION_SAMPLE_SIZE];
uint16_t P1_atemptyCalibration[P0_CALIBRATION_SAMPLE_SIZE];
uint16_t P2_atemptyCalibration[P0_CALIBRATION_SAMPLE_SIZE];
uint16_t P1_atfullCalibration[P0_CALIBRATION_SAMPLE_SIZE];
uint16_t P2_atfullCalibration[P0_CALIBRATION_SAMPLE_SIZE];

uint16_t P0_liveArray[P1_LIVE_SAMPLE_SIZE];
uint16_t P1_liveArray[P1_LIVE_SAMPLE_SIZE];
uint16_t P2_liveArray[P2_LIVE_SAMPLE_SIZE];
uint16_t liveDivisorArray_u16[DIVISOR_SAMPLE_SIZE];

uint16_t TD10_value_reg_P0_Array[P1_LIVE_SAMPLE_SIZE];
uint16_t TD10_value_reg_P1_Array[P1_LIVE_SAMPLE_SIZE];
uint16_t TD10_value_reg_P2_Array[P1_LIVE_SAMPLE_SIZE];

uint8_t TD10_overflow_P0_Array[P1_LIVE_SAMPLE_SIZE];
uint8_t TD10_overflow_P1_Array[P1_LIVE_SAMPLE_SIZE];
uint8_t TD10_overflow_P2_Array[P1_LIVE_SAMPLE_SIZE];

uint16_t raw_p0[raw_p0_array];
uint16_t raw_p1[raw_p0_array];
uint16_t raw_p2[raw_p0_array];

/* Configure this array as per demo array */
static const dataDetails_sType details[19] = 
{
    /* dataName_eType(in .h file)           address of array    e_dataType_eType                how many samples*/

    //{ e_demoDataName,                       demoSensorData,        e_float_type,               DEMO_SENSOR_SAMPLES},  // demo

    { e_LiveDivisor,                       liveDivisorArray,        e_float_type,                DIVISOR_SAMPLE_SIZE},
    { e_p0AtCalibration,                   P0_atCalibration,        e_uint16_type,               P0_CALIBRATION_SAMPLE_SIZE},
    { e_LiveP1,                             P1_liveArray,           e_uint16_type,                 P1_LIVE_SAMPLE_SIZE},
    { e_LiveP2,                             P2_liveArray,           e_uint16_type,                 P2_LIVE_SAMPLE_SIZE},
    { e_LiveDivisor_u16,                   liveDivisorArray_u16,   e_uint16_type,                DIVISOR_SAMPLE_SIZE},
    { e_p1AtemptyCalibration,              P1_atemptyCalibration,  e_uint16_type,                P0_CALIBRATION_SAMPLE_SIZE},
    { e_p2AtemptyCalibration,              P2_atemptyCalibration,  e_uint16_type,                P0_CALIBRATION_SAMPLE_SIZE},
    { e_p1AtfullCalibration,              P1_atfullCalibration,   e_uint16_type,                P0_CALIBRATION_SAMPLE_SIZE},
    { e_p2AtfullCalibration,              P2_atfullCalibration,   e_uint16_type,                P0_CALIBRATION_SAMPLE_SIZE},
    { e_LiveP0,                             P0_liveArray,           e_uint16_type,                 P1_LIVE_SAMPLE_SIZE},
    { e_TD10_value_reg_P0,                  TD10_value_reg_P0_Array, e_uint16_type,                 P1_LIVE_SAMPLE_SIZE},
    { e_TD10_value_reg_P1,                  TD10_value_reg_P1_Array, e_uint16_type,                 P1_LIVE_SAMPLE_SIZE},
    { e_TD10_value_reg_P2,                  TD10_value_reg_P2_Array, e_uint16_type,                 P1_LIVE_SAMPLE_SIZE},
    { e_TD10_overflow_P0,                    TD10_overflow_P0_Array,  e_uint8_type,                 P1_LIVE_SAMPLE_SIZE},
    { e_TD10_overflow_P1,                    TD10_overflow_P1_Array,   e_uint8_type,                 P1_LIVE_SAMPLE_SIZE},
    { e_TD10_overflow_P2,                    TD10_overflow_P1_Array,   e_uint8_type,                 P1_LIVE_SAMPLE_SIZE},
    { e_raw_p0,                               raw_p0,                  e_uint16_type,                 P1_LIVE_SAMPLE_SIZE},
    { e_raw_p1,                               raw_p1,                  e_uint16_type,                 P1_LIVE_SAMPLE_SIZE},
    { e_raw_p2,                               raw_p2,                  e_uint16_type,                 P1_LIVE_SAMPLE_SIZE},

   



};


/************************************************************************************************************************/


void getMovingAveargeOf( dataName_eType e_DataName, const void *dataIn, void *getMovAvg)
{
    uint16_t i = 0 ;
    int8_t index = -1 ;
    for (i = 0 ; i < e_numberOfData; i++)
    {
        if (e_DataName == details[i].e_Dataname)
        {
            index = i ;
            break;
        }
        
    }

    if (index != -1)
    {
        calculateMovingAverage( index , dataIn, getMovAvg);
    }
    else
    {
        /* Data not found */
    }
    
}



static void calculateMovingAverage(uint8_t indexOfRow, const void *dataIn, void *getMovAvg)
{
   
    

    /* here we are running this code for one time only when function call */
    if (firstTimeRunflags[details[indexOfRow].e_Dataname] == false)
    {
        firstTimeRunflags[details[indexOfRow].e_Dataname] = true;
        copyDataFirstTime(indexOfRow, dataIn);
    }

    
    else if (firstTimeRunflags[details[indexOfRow].e_Dataname] == true)
    {
        /* here data is shifting left in the array */
        shiftDataInArray(indexOfRow, dataIn);
    }
    
    else
    { }

    /* Here we are calculating the average */
    calculateAveargeOfArray(indexOfRow, getMovAvg);

}



static void copyDataFirstTime(const uint8_t indexOfRow, const void *dataIn)
{
    uint16_t i = 0 ; 
    switch(details[indexOfRow].dataType)
    {
        case e_int8_type:
            {
                for (i = 0 ; i < details[indexOfRow].sizeOfDataArray; i++)
                {
                    *((int8_t *)details[indexOfRow].addOfArray + i ) = *(int8_t *)dataIn ;
                }
            }
            break;
        
        case e_int16_type:
            {
                for (i = 0 ; i < details[indexOfRow].sizeOfDataArray; i++)
                {
                    *((int16_t *)details[indexOfRow].addOfArray + i ) = *(int16_t *)dataIn ;
                }
            }
            break;
        
        case e_int32_type:
            {
                for (i = 0 ; i < details[indexOfRow].sizeOfDataArray; i++)
                {
                    *((int32_t *)details[indexOfRow].addOfArray + i ) = *(int32_t *)dataIn ;
                }
            }
            break;
            
        case e_uint8_type:
            {
                for (i = 0 ; i < details[indexOfRow].sizeOfDataArray; i++)
                {
                    *((uint8_t *)details[indexOfRow].addOfArray + i ) = *(uint8_t *)dataIn ;
                }
            }
            break;
        
        case e_uint16_type:
            {
                for (i = 0 ; i < details[indexOfRow].sizeOfDataArray; i++)
                {
                    *((uint16_t *)details[indexOfRow].addOfArray + i ) = *(uint16_t *)dataIn ;
                }
            }
            break;
        
        case e_uint32_type:
            {
                for (i = 0 ; i < details[indexOfRow].sizeOfDataArray; i++)
                {
                    *((uint32_t *)details[indexOfRow].addOfArray + i ) = *(uint32_t *)dataIn ;
                }
            }
            break;
        
        case e_float_type:
            {
                for (i = 0 ; i < details[indexOfRow].sizeOfDataArray; i++)
                {
                    *((float *)details[indexOfRow].addOfArray + i ) = *(float *)dataIn ;
                }
            }
            break;
            
        default:
            break;
        
    }
}

static void shiftDataInArray(const uint8_t indexOfRow, const void *dataIn)
{
    uint16_t i = 0 ; 
    switch(details[indexOfRow].dataType)
    {
        case e_int8_type:
            {
                for (i = 0 ; i < details[indexOfRow].sizeOfDataArray - 1; i++)
                {
                    *((int8_t *)details[indexOfRow].addOfArray + i ) =  *((int8_t *)details[indexOfRow].addOfArray + i + 1);
                }
                
                *((int8_t *)details[indexOfRow].addOfArray + i ) = *(int8_t *)dataIn ;
            }
            break;
        
        case e_int16_type:
            {
                for (i = 0 ; i < details[indexOfRow].sizeOfDataArray - 1; i++)
                {
                    *((int16_t *)details[indexOfRow].addOfArray + i ) = *((int16_t *)details[indexOfRow].addOfArray + i + 1) ;
                }
                
                *((int16_t *)details[indexOfRow].addOfArray + i ) = *(int16_t *)dataIn ;
            }
            break;
        
        case e_int32_type:
            {
                for (i = 0 ; i < details[indexOfRow].sizeOfDataArray - 1; i++)
                {
                    *((int32_t *)details[indexOfRow].addOfArray + i ) = *((int32_t *)details[indexOfRow].addOfArray + i + 1) ;
                }
                
                *((int32_t *)details[indexOfRow].addOfArray + i ) = *(int32_t *)dataIn ;
            }
            break;
            
        case e_uint8_type:
            {
                for (i = 0 ; i < details[indexOfRow].sizeOfDataArray - 1; i++)
                {
                    *((uint8_t *)details[indexOfRow].addOfArray + i ) = *((uint8_t *)details[indexOfRow].addOfArray + i + 1) ;
                }
                
                *((uint8_t *)details[indexOfRow].addOfArray + i ) = *(uint8_t *)dataIn ;
            }
            break;
        
        case e_uint16_type:
            {
                for (i = 0 ; i < details[indexOfRow].sizeOfDataArray - 1; i++)
                {
                    *((uint16_t *)details[indexOfRow].addOfArray + i ) = *((uint16_t *)details[indexOfRow].addOfArray + i + 1) ;
                }
                
                *((uint16_t *)details[indexOfRow].addOfArray + i ) = *(uint16_t *)dataIn ;
            }
            break;
        
        case e_uint32_type:
            {
                for (i = 0 ; i < details[indexOfRow].sizeOfDataArray - 1; i++)
                {
                    *((uint32_t *)details[indexOfRow].addOfArray + i ) = *((uint32_t *)details[indexOfRow].addOfArray + i + 1) ;
                }
                
                *((uint32_t *)details[indexOfRow].addOfArray + i ) = *(uint32_t *)dataIn ;
            }
            break;
        
        case e_float_type:
            {
                for (i = 0 ; i < details[indexOfRow].sizeOfDataArray - 1; i++)
                {
                    *((float *)details[indexOfRow].addOfArray + i ) = *((float *)details[indexOfRow].addOfArray + i + 1) ;
                }
                
                *((float *)details[indexOfRow].addOfArray + i ) = *(float *)dataIn ;
            }
            break;
            
        default:
            break;
        
    }
}


static void calculateAveargeOfArray(const uint8_t indexOfRow, void *getFinalMovAvg)
{
    uint16_t i = 0 ; 
    switch(details[indexOfRow].dataType)
    {
        case e_int8_type:
            {
                sumI32 = 0 ; 
                for (i = 0 ; i < details[indexOfRow].sizeOfDataArray; i++)
                {
                    sumI32 += *((int8_t *)details[indexOfRow].addOfArray + i );
                }
                
                *(int8_t *)getFinalMovAvg = sumI32 / details[indexOfRow].sizeOfDataArray ;
            }
            break;
        
        case e_int16_type:
            {
                sumI32 = 0 ; 
                for (i = 0 ; i < details[indexOfRow].sizeOfDataArray; i++)
                {
                    sumI32 += *((int16_t *)details[indexOfRow].addOfArray + i );
                }
                *(int16_t *)getFinalMovAvg = sumI32 / details[indexOfRow].sizeOfDataArray ;
            }
            break;
        
        case e_int32_type:
            {
                sumI32 = 0 ; 
                for (i = 0 ; i < details[indexOfRow].sizeOfDataArray; i++)
                {
                    sumI32 += *((int32_t *)details[indexOfRow].addOfArray + i );
                }
                
                *(int32_t *)getFinalMovAvg = sumI32 / details[indexOfRow].sizeOfDataArray ;
            }
            break;
            
        case e_uint8_type:
            {
                sumU32 = 0 ; 
                for (i = 0 ; i < details[indexOfRow].sizeOfDataArray; i++)
                {
                    sumU32 += *((uint8_t *)details[indexOfRow].addOfArray + i );
                }
                
                *(uint8_t *)getFinalMovAvg = sumU32 / details[indexOfRow].sizeOfDataArray ;
            }
            break;
        
        case e_uint16_type:
            {
                sumU32 = 0 ; 
                for (i = 0 ; i < details[indexOfRow].sizeOfDataArray; i++)
                {
                    sumU32 += *((uint16_t *)details[indexOfRow].addOfArray + i );
                }
                
                *(uint16_t *)getFinalMovAvg = sumU32 / details[indexOfRow].sizeOfDataArray ;
            }
            break;
        
        case e_uint32_type:
            {
                sumU32 = 0 ; 
                for (i = 0 ; i < details[indexOfRow].sizeOfDataArray; i++)
                {
                    sumU32 += *((uint32_t *)details[indexOfRow].addOfArray + i );
                }
                
                *(uint32_t *)getFinalMovAvg = sumU32 / details[indexOfRow].sizeOfDataArray ;
            }
            break;
        
        case e_float_type:
            {
                sumFloat = 0 ; 
                for (i = 0 ; i < details[indexOfRow].sizeOfDataArray; i++)
                {
                    sumFloat += *((float *)details[indexOfRow].addOfArray + i );
                }
                
                *(float *)getFinalMovAvg = sumFloat / details[indexOfRow].sizeOfDataArray ;
            }
            break;
            
        default:
            break;
        
    }
}
