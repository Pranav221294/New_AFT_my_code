
#include "r_cg_macrodriver.h"
#include "customTypedef.h" 
#include "r_cg_userdefine.h"
#include "j1939.h"
#include "SoftVersion.h"


// Constant declarations -----------------------------------------------------
// Program Version
const char *progVersion = "V1.2";


// local declarations --------------------------------------------------------


/*********************************************************************//**
 *
 * Check def CAN msg and CAN lost msg ,According to def level calculate bars.
 *
 *
 * @param      None 
 *
 * @return     None 
 *************************************************************************/
void vSoftwareVersionRead(void)
{	
	u8 Count = 0;	
	u32 Data = 0;
	static bool FirstEntry = TRUE;
  	u32 requestedPGN;
    u8 data[8];
    int i, j;

	
	//Take CAN data
	requestedPGN = u32GetPGNdata(eJRequestPGNSoftVersion);
	//Check Timer stamps for CAN msg is coming or Not
	if(bGetPGNTimeOut(eJRequestPGNSoftVersion))	
	{
		vJ1939_TxMsgEnableDisable(eJSoftVersionDataTx,FALSE);
		vJ1939_TxSendOnDemandDisable(eTx65303);
		requestedPGN = 0;
	}
	
    // Check validity
    if (requestedPGN == PGN65303_ext)
    {
        // Assemble the version message
        for (i=j=0; i<8; i++)
            if (progVersion[j] != 0)
                data[i] = progVersion[j++];
            else
                data[i] = 0xFF;
			
		for(Count = 4; Count > 0u; Count--)
		{
			Data = Data<<8 | data[Count - 1];
		}
        // Use the proprietary PGN for the response
		vJ1939_TxSendOnDemandEnable(eTx65303);
		vJ1939_TxMsgEnableDisable(eJSoftVersionDataTx,TRUE);
		vPutPGNdata(eJSoftVersionDataTx,Data,0U);
		
    }
	else
	{
		vJ1939_TxSendOnDemandDisable(eTx65303);
		vJ1939_TxMsgEnableDisable(eJSoftVersionDataTx,FALSE);	
	}
    // Set message buffer available
 
}


