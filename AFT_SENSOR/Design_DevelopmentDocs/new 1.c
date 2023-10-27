#pragma config FOSC = INTOSCIO, WDTE = OFF, PWRTE = OFF, MCLRE = ON, CP = OFF, BOREN = ON, CLKOUTEN = OFF

#define _XTAL_FREQ 4000000

// Globals
unsigned char TMR1U = 0;
unsigned char RXTXCOUNT = 0;
unsigned char OPTCOUNT = 0;
unsigned char COMMSBEENON = 0;
unsigned char FLAGS232 = 0;
unsigned char RXREG = 0;
unsigned char SENDPARAMS = 0;
unsigned char WRITE232 = 0;
unsigned char READ232 = 0;
unsigned char EEWRADDRESS = 0;

// Function prototypes
void ADDTOTIMEp();
void TIMESHIFT();
void TRANSFERP1();
void TRANSFERP2();
void TRANSFERP0();
void ADDTOTIMEt();
void EEREADADRW();
void SENDTXW();
void SENDEESTORE();

void interrupt isr(void) 
{
    if (T0COUNTING)
	{
        if (TMR0IF) 
		{
            TMR0IF = 0;
            T0COUNTING = 0;
            ADDTOTIMEt();
        }
    } 
	else 
	 {
        if (TMR0IF) 
		{
            TMR1U++;
            T0COUNTING = 1;
            TMR0IF = 0;
            OPTCOUNT++;
            if (OPTCOUNT == OPTsetT0wait)
			{
                T0WAITSTART = 1;
                OPTCOUNT = 0;
            }
        }
    }
    if (TMR1IF) 
	{
        T1CONbits.TMR1ON = 0;
        TMR1IF = 0;
        if (PIR1bits.TMR1IF) 
		{
            TMR1U++;
        }
        ADDTOTIMEp();
        TIMESHIFT();
        if (GETTINGP1)
		{
            TRANSFERP1();
        }
        if (GETTINGP2)
		{
            TRANSFERP2();
        }
        if (GETTINGP0) 
		{
            TRANSFERP0();
        }
        PIR1bits.TMR1IF = 0;
        return;
    }
    if (RXTXCOUNT)
	{
        RXTXCOUNT--;
    }
    if (RCIF) 
	{
        RXREG = RCREG;
        COMMSBEENON = 1;
        if (RXTXCOUNT == 0)
			{
            FLAGS232 = 0;
            if (RXREG == 170) {
                COMF(RXREG, W);
                SENDTXW(~RXREG);
                WRITE232 = 1;
                RXTXCOUNT = 11;
            } else if (RXREG == 85) {
                COMF(RXREG, W);
                SENDTXW(~RXREG);
                READ232 = 1;
                RXTXCOUNT = 11;
            } else if (RXREG == 165) {
                SENDPARAMS = 1;
            } else if (RXREG == 90) {
                SENDEESTORE();
            }
        }
    }
}

void main(void) {
    // Oscillator configuration
    OSCCONbits.IRCF = 0b110; // 4 MHz
    OSCCONbits.SCS = 0b00; // Use FOSC as system clock

    // Timer0 configuration
    OPTION_REGbits.PSA = 0; // Prescaler assigned to Timer0
    OPTION_REGbits.PS = 0b111; // 1:256 prescaler
    TMR0 = 0;


void addToTimep(uint16_t* time) 
{
uint8_t saveFlag = 0;
FSR0H = 0;
FSR0L = (uint8_t) time;
WREG = 0x01;
WREG += *INDF0;
INCF(FSR0L, 1);
WREG += *INDF0;
*INDF0 = WREG;
if (*INDF0 == 0) {
saveFlag = 1;
}
if (saveFlag) {
SET_BIT(SAVEFLAG, 0);
}
return;
}


void timeshift() {
    unsigned char tmr1shift = <value>; // initialize with the desired value
    unsigned char loopcount = tmr1shift;
    if (loopcount == 0) {
        return;
    }
    // code that comes after this snippet
}
Note that the data types used in the C code are unsigned characters since the original code appears to be working with bytes.



void TRANSFERP1(void)
{
TMR1LP1 = TMR1L;
TMR1HP1 = TMR1H;
TMR1UP1 = TMR1U;
GETTINGP1 = 0;
return;
}


void getP1()
 {
    PORTC &= 0xF3;    // both probes disabled
    PORTC |= enP1;    // set probe connection
    GETTINGP1 = 1;
    T0WAITSTART = 1;
    while (GETTINGP1) 
	{
        continue;
    }
    return;
}


void addToTimet(uint16_t *time) 
{
    *time += 1; // Add 1 to the low word
    if (*time == 0) 
	{
        *time += 1; // Add 1 to the high word if the low word wrapped around
        SAVEFLAG |= 1; // Set the SAVEFLAG bit if there was an overflow
    }
}

void sendTxW(unsigned char txData)

{
    while(!TXIF);  // wait until transmit buffer is empty
    TXREG = txData; // send data
}