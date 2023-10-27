#include <xc.h>

#define OPTsetT0wait 0b01010101 // option register value for T0 wait
#define COMMSBEENON RB0 // flag for communication activity

unsigned char TMR1U = 0;
unsigned char RXTXCOUNT = 0;
unsigned char OPTCOUNT = 0;
unsigned char FLAGS232 = 0;
unsigned char WRITE232 = 0;
unsigned char READ232 = 0;
unsigned char SENDPARAMS = 0;
unsigned char T0WAITSTART = 0;
unsigned char T0COUNTING = 0;
unsigned char GETTINGP1 = 0;
unsigned char GETTINGP2 = 0;
unsigned char GETTINGP0 = 0;

void ADDTOTIMEp(void);
void TIMESHIFT(void);
void TRANSFERP1(void);
void TRANSFERP2(void);
void TRANSFERP0(void);
void SENDTXW(void);
void EEREADADRW(void);
void EEWRITEWNEXT(void);
void SENDEESTORE(void);

void main(void) {
    // ORG 04
    // Initialize registers and flags
    TMR1U = 0;
    RXTXCOUNT = 0;
    OPTCOUNT = 0;
    FLAGS232 = 0;
    WRITE232 = 0;
    READ232 = 0;
    SENDPARAMS = 0;
    T0WAITSTART = 0;
    T0COUNTING = 0;
    GETTINGP1 = 0;
    GETTINGP2 = 0;
    GETTINGP0 = 0;

    // BANKSEL 0
    // Check for T0 interrupt and overflow
    while (1) {
        if (T0COUNTING) {
            if (!TMR0IF) {
                goto NOTT0INT;
            }
            // stop timer
            T1CONbits.TMR1ON = 0;

            // Check for TMR1 overflow
            if (PIR1bits.TMR1IF) {
                TMR1U++;
            }
            T0COUNTING = 0;
            INTCONbits.TMR0IE = 0; // disable interrupt
            ADDTOTIMEp();

            TIMESHIFT();

            if (GETTINGP1) {
                TRANSFERP1();
            }
            if (GETTINGP2) {
                TRANSFERP2();
            }
            if (GETTINGP0) {
                TRANSFERP0();
            }
            goto COMMTEST;
        }

        // disable interrupt
        INTCONbits.TMR0IE = 0;

        ADDTOTIMEp();

        COMMTEST:
        if (PIR1bits.RCIF) {
            // Ready to receive command
            goto DOCOMMS;
        }

        if (T0WAITSTART) {
            // not ready to start cycle yet
            goto INTEND;
        }

        // Synchronize T0 and T1
        T1CONbits.TMR1ON = 0; // stop TMR1
        TMR1L = 0;
        TMR1H = 0;
        TMR1U = 0;
        OPTION_REG = OPTsetT0wait; // set option register for T0 wait
        TMR0 = 0xFF;
        INTCONbits.TMR0IF = 0;
        while (!INTCONbits.T0IF) {
            continue; // loop until rollover
        }

        OPTCOUNT = 0;
        TMR0 = OPTCOUNT;
        T0COUNTING = 1;
        INTCONbits.TMR0IF = 0;
