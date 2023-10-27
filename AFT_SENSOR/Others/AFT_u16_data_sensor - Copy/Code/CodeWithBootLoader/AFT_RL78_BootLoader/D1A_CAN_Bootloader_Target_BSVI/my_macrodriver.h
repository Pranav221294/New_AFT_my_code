
#ifndef STATUS_H
#define STATUS_H
//----------------------------------------------------------------------------------------------------------------
#pragma sfr
#pragma DI
#pragma EI
#pragma NOP
#pragma HALT
#pragma STOP
//----------------------------------------------------------------------------------------------------------------
#ifndef __TYPEDEF__
typedef signed char         int8_t;
typedef unsigned char       uint8_t;
typedef signed short        int16_t;
typedef unsigned short      uint16_t;
typedef signed long         int32_t;
typedef unsigned long       uint32_t;
typedef unsigned short      MD_STATUS;
#define __TYPEDEF__
#endif

//----------------------------------------------------------------------------------------------------------------
#endif
