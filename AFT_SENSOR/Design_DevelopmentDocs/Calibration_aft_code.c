#include <stdio.h>

#include <stdint.h>




#define CONST_14_4          ((uint16_t)58982)

#define CONST_7_07          ((uint16_t)28959)

#define CONST_6_3           ((uint16_t)25808)

// #define CONST_14_4          ((uint16_t)58982)

// #define CONST_14_4          ((uint16_t)58982)





typedef struct Calibration

{

    uint16_t p0Empty;

    uint16_t p1Empty;

    uint16_t p2Empty;

    uint16_t p1Full;

    uint16_t p2Full;

}pxData_sType;





typedef struct CalibParam

{

    uint16_t rawP1Span;

    uint16_t rawP2Span;

    uint16_t constant_m;

    uint16_t divAtCal;

    uint16_t p1Span;

}calcParam_sType;




const static pxData_sType sCalibData = {




                    10004,      // p0 empty

                    14417,      // p1 empty

                    14794,      // p2 empty

                    24271,      // p1 full

                    19917       // p2 full

};




static calcParam_sType sCalcParam = {0};




int main()

{

    sCalcParam.rawP1Span    = sCalibData.p1Full - sCalibData.p1Empty ;

    sCalcParam.rawP2Span    = sCalibData.p2Full - sCalibData.p2Empty ;

    sCalcParam.constant_m   = (uint16_t)(CONST_14_4 - ((CONST_7_07 * sCalcParam.rawP1Span) / sCalcParam.rawP2Span)) ;

    sCalcParam.divAtCal     = (uint16_t)(((sCalcParam.constant_m * sCalcParam.rawP1Span) / sCalcParam.rawP2Span) - CONST_6_3) ;

    sCalcParam.p1Span       = (uint16_t)((sCalcParam.rawP1Span * (uint16_t)4096) / sCalcParam.divAtCal) ;

    printf("\n------------------- Calculated data at calibration time--------------------------\n");

    printf("p0 empty            = %u\n", sCalibData.p0Empty);

    printf("p1 empty            = %u\n", sCalibData.p1Empty);

    printf("p2 empty            = %u\n", sCalibData.p2Empty);

    printf("p1 Full             = %u\n", sCalibData.p1Full);

    printf("p2 Full             = %u\n", sCalibData.p2Full);

    printf("rawP1Span           = %u\n", sCalcParam.rawP1Span);

    printf("rawP2Span           = %u\n", sCalcParam.rawP2Span);

    printf("constant_m          = %u\n", sCalcParam.constant_m);

    printf("divAtCal in Memory  = %u\n", sCalcParam.divAtCal);

    printf("divAtCal at GUI     = %.4f\n", (float)sCalcParam.divAtCal / 4096);

    printf("p1Span              = %u\n", sCalcParam.p1Span);

    printf("\n-----------------------------------------------------");





    return 0;

}