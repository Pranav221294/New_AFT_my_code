/** ###################################################################
**     Filename  : Curve.c
**     Project   : Y1ClusterDiagnostics
**     Processor : MC9S12XHZ256VAG
**     Compiler  : CodeWarrior HCS12X C Compiler
**     Date/Time : 25/01/2018, 14:28
**     Contents  :
**         User source code
**
** ###################################################################*/

/* MODULE Curve */


#include "CurveHeader.h"

// constant declarations -----------------------------------------------------

// Each of these tables may be constructed using any number of entries and any
// width needed thereby providing a process to create translations at any
// level of accuracy needed.


/////////////////////////////////
#define LEVEL_TO_OUTPUT_VOLTAGE_ROW		6U
#define LEVEL_TO_OUTPUT_VOLTAGE_COL		2U


const u32 FuellevelToVoltage[LEVEL_TO_OUTPUT_VOLTAGE_ROW][LEVEL_TO_OUTPUT_VOLTAGE_COL] =
{
		/*Level*/									/*voltages in mV*/
		// {MAX_VALUE_AT_FULL_LEVEL					,	    5000u	},		
		// {7508u										,	    3850u	},
		// {5071u										,	    2750u	},
		// {2561u										,	    1640u  },
		// {1433u										,	    1070u  },
		// {0u											,	    500u  },

	{MAX_VALUE_AT_FULL_LEVEL							,	    5000u	},		
	{7600u											,	    3850u	},
	{5200u											,	    2750u	},
	{2600u											,	    1640u  },
	{1300u											,	    1070u  },
	{0u												,	    500u  },

};





// local declarations --------------------------------------------------------
// These are variables used for the linear interprolation calculation.
// They are defined here to save stack space during use.
s32   R1;
s32   R2;
s32   R3;
s32   V1;
s32   V2;
s32   V3;

s32 overrange;

// private prototypes --------------------------------------------------------
static s16 _iGetTableValue(u32 const *, u32, u32 *, u8, u8, u8);


/*****************************************************************************
 **  Description:
 **      Returns the value from specified table.
 **
 ** ---------------------------------------------------------------------------
 **  Parameters:     table ID,
 **                  resistance value
 **                  pointer to word to store the value
 **
 **  Return value:   amount of overrange as:
 **                      0   returned value is within the table range,
 **                      < 0 returned value is below the lowest value in the table,
 **                      > 0 returned value is above the highest value in the table
 **
 *****************************************************************************/


void vGetCurveDataValue(eCurveType curve, u32 inVal, u32 *value)
{
	//int overrange; // overrange can use in future if required

	switch (curve)
	{
	case eFuelLevelToVoltage :
		overrange = _iGetTableValue((u32 *)FuellevelToVoltage, inVal, value, LEVEL_TO_OUTPUT_VOLTAGE_ROW, LEVEL_TO_OUTPUT_VOLTAGE_COL, 1);
		break;

	default :
		*value = (u32)-1;
//		overrange = 0;
		break;
	}

}

/*****************************************************************************
 **  Description:
 **      This functi0n finds the closest match in the table for the given
 **      value. Linear interprolation is performed between values. It will
 **      set the amount of overrange.
 **
 **      What this function will handle:
 **          Tables of any size up to 256 x 256
 **          Tables with increasing or decreasing values in any of the columns.
 **
 **      Linear Interprolation Algorithm
 **           table where,
 **                  R = the resistance or other input value
 **                  V = the converted value (pressure, temperature, etc)
 **                 R  |  V
 **                 -------
 **                 R1 | V1 (table values)
 **   (input value) R2 | V2 (the calculated output value)
 **                 R3 | V3 (table values)
 **
 **                  (R2 - R1) * (V3 - V1)
 **          V2 =    --------------------- + V1
 **                        (R3 - R1)
 **
 **      What this function will not handle:
 **          Tables with negative numbers.
 **
 ** ---------------------------------------------------------------------------
 **  Parameters:     table pointer,
 **                  input value,
 **                  pointer to word for value storage
 **                  number of rows in the table,
 **                  number of columns in the table,
 **                  column index
 **
 **  Return value:   an integer indicating the percent of +- overrange
 **
 *****************************************************************************/
static s16 _iGetTableValue(u32 const *tabPtr, u32 inVal, u32 *valPtr, u8 numOfRows, u8 numOfCols, u8 colIndex)
{
	s16 i;
	bool withinTableLimits = FALSE;

	if (tabPtr[numOfCols] > tabPtr[0])
	{
		// Table first column is increasing
		if (inVal <= tabPtr[0])
		{
			// Get the value and set the amount of overrange
			*valPtr = tabPtr[0 + colIndex];
			overrange = (((s32)inVal - (s32)tabPtr[0]) * 100) / (s32)tabPtr[0];
		}
		else if (inVal >= tabPtr[(numOfRows-1) * numOfCols])
		{
			// Get the value and set the amount of overrange
			*valPtr = tabPtr[(numOfRows-1) * numOfCols + colIndex];
			overrange = (((s32)inVal - (s32)tabPtr[(numOfRows-1) * numOfCols]) * 100) / (s32)tabPtr[(numOfRows-1) * numOfCols];
		}
		else {
			// Set the index
			withinTableLimits = TRUE;
			i = 0;
			while (tabPtr[i * numOfCols] < inVal)
				++i;
		}
	}
	else {
		// Table first column is decreasing
		if (inVal >= tabPtr[0])
		{
			// Get the value and set the amount of overrange
			*valPtr = tabPtr[0 + colIndex];
			overrange = (((s32)inVal - (s32)tabPtr[0]) * 100) / (s32)tabPtr[0];
		}
		else if (inVal <= tabPtr[(numOfRows-1) * numOfCols])
		{
			// Get the value and set the amount of overrange
			*valPtr = tabPtr[(numOfRows-1) * numOfCols + colIndex];
			overrange = (((s32)inVal - (s32)tabPtr[(numOfRows-1) * numOfCols]) * 100) / (s32)tabPtr[(numOfRows-1) * numOfCols];
		}
		else {
			// Set the index
			withinTableLimits = TRUE;
			i = 0;
			while (tabPtr[i * numOfCols] > inVal)
				++i;
		}
	}

	if (withinTableLimits)
	{
		// It is not beyond the table limits
		overrange = 0;

		i *= numOfCols; // index of R1

		// Use these variables to save stack space during calculation
		R1 = (s32)tabPtr[i];
		R3 = (s32)tabPtr[i-numOfCols];
		V1 = (s32)tabPtr[i+colIndex];
		V3 = (s32)tabPtr[i-numOfCols+colIndex];
		R2 = (s32)inVal;

		V2 = ((100 * ((R2 - R1) * (V3 - V1))) / (R3 - R1)) + (100 * V1);
		*valPtr = (u32)(V2 / 100);
	}

	return (s16)overrange;
}



/* END Curve */
