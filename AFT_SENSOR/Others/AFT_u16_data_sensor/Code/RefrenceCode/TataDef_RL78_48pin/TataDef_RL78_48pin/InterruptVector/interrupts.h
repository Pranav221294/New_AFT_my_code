#ifndef __InterruptHeader_H
#define __Interrupteader_H


// Interrupt prototype -------------------------------
void r_tau0_channel4_interrupt(void);
void r_tau0_channel1_interrupt(void);
void r_wdt_interrupt(void);
void MY_Rx_MsgBuf_Processing(void);
void MY_Tx_MsgBuf(void);
void MY_ISR_CAN_Error(void);
void MY_ISR_CAN_Wakeup(void);
 

#endif