	TITLE     "PA240-16F1823 (c) 1/3/2011 P.R.GORTON"
	LIST      C=130,N=60

;*****************************************************************************;
; This program is protected as an unpublished work under the Copyright Act of ;
; 1975 & the Computer Program Act of 1980. It may not be copied or disclosed  ;
; to any third party without the express permission in writing of the author. ;
;*****************************************************************************;

SETFORTEST	EQU	0
SWAPPROBEPINS	EQU	1	;0 = no change, 1 = swap pins

;Saved as : PA240 - 16F1823 V8m.asm
;Device/Oscillator : 16F1823/Ext.Osc 20MHz
;Last update : 20/10/11

;Notes: #1 normal PWM slowed to 4.88KHz
;#2 extra bits to allow manual testing of D.Resistive output from MPLab with PICkit3. REM out for normal use.
;V4 - CCPR1L set to zero in setup
;V5 - Use pulse frequency modulation for digistal resistive output ###3
;V6 - Level alarm output added ###4. OP pin not designated yet
;CV6 - For revision C of PCB - pins altered ###5
;CV7 - Use advanced setting for count and interval between samples ###6
;###7 use alarm OP to indicate calibration state
;CV8 start of conversion for 2 probe ###8
;###A correction
;Version 8b ###B calculated divisor y = mx -c (pointer at cal send out as zero).
;Version 8c ###C for use with 0V08 diagnostic software.
;Divisor at calibration now stored and sent out in diagnostic extrended EEPROM query with m, c & divisor lower limit
;New EEPROM write routine for changing divisor calculation parameters 
;Version 8d - minor correction to send of divisor at calibration ###D
;Version 8e - calculation routines changed - variable names changed to suit 2 probe
;###9 no probe value calculation (P0)
;###10 calibration routines altered
;###11 changes to EE values sent
;temp boo
;Send params now 1 shot

;NOTE: DISABLE GETFALSEDATA FOR REAL TESTING & SET EE CALIBRATION DATA TO 0

;Disable done in version g onwards
;###h Alterations to averaging. If average<div@cal then use div@cal in P1 calcs.

;###m Controlled shift for TMR1U/H/L to speed up acquisition time
;###n no averaging if under 20% of calspan
;###p (no version o) default n changed to 2 from 4
;###q single probe version works like 2 probe version except that divisor is held at 1
;###r change to correct 'under empty' start up error
;when in D RESISTIVE mode the resistance multiplier is sent out instead of PWM
;###s negative check corrected
;###t modifications requested by Dave Burton

;NOTE: MAKE RESISTIVE PWM SLOW TO CHANGE,


;Use MOVLB for BSR setting & MOVLP fOR PCLATH setting or use BANKSEL 'regname' to select bank & PAGESEL 'routine name' to select page.

        	IFDEF __12F1822
	#INCLUDE P12F1822.INC		
	ENDIF

        	IFDEF __12L1822
	#INCLUDE P12LF1822.INC		
	ENDIF

        	IFDEF __16F1823
	#INCLUDE P16F1823.INC		
	ENDIF
	
        	IFDEF __16LF1823
	#INCLUDE P16L1823.INC		
	ENDIF
	
	
	
	ERRORLEVEL	1

;    __CONFIG _CONFIG1, _FOSC_ECH & _WDTE_ON & _PWRTE_ON & _MCLRE_ON & _CP_ON & _CPD_OFF & _BOREN_ON & _CLKOUTEN_OFF & _IESO_OFF & _FCMEN_OFF
;    __CONFIG _CONFIG2, _WRT_ALL & _PLLEN_OFF & _STVREN_ON & _BORV_25 & _LVP_OFF

    __CONFIG _CONFIG1, _FOSC_HS & _WDTE_OFF & _PWRTE_OFF & _MCLRE_OFF & _CP_OFF & _CPD_OFF & _BOREN_ON & _CLKOUTEN_OFF & _IESO_OFF & _FCMEN_OFF
    __CONFIG _CONFIG2, _WRT_OFF & _PLLEN_OFF & _STVREN_ON & _BORV_LO & _LVP_OFF


;register labels (20-7F + 0AF-0BF)

GENSTA0	EQU	20						;;; Register used to hold flags
FLASHER	EQU	21						;;; need to find the use
SWTIMER	EQU	22						;;; need to find the use

PWML	EQU	23						;;; To hold lower 8 bit of PWM value
PWMH	EQU	24						;;; To hold upper 8 bit of PWM value
PWMTEMP	EQU	25						;;; To hold the PWM value temprory 

DIVATCALL	EQU	26					;;; To hold the lower 8 bit of div@cal value
DIVATCALH	EQU	27					;;; To hold the upper 8 bit of div@cal value

AVDIVL_OVER20	EQU	28				;;; To hold the lower 8 bit of average divisor value when level is over 20% 
AVDIVH_OVER20	EQU	29				;;; To hold the upper 8 bit of average divisor value when level is over 20% 

P1SPANATCALL	EQU	2A				;;; To hold the lower 8 bit of P1span@cal
P1SPANATCALH	EQU	2B				;;; To hold the upper 8 bit of P1span@cal

DIVUSEDL	EQU	2C	;for diagnostics only		;;; To hold the lower 8 bit of current used Divisor for diagnostic purpose only
DIVUSEDH	EQU	2D	;for diagnostics only		;;; To hold the upper 8 bit of current used Divisor for diagnostic purpose only

TEST	EQU	2F	;for test only

TMR1U	EQU	30						;;;
TMR1UP1	EQU	31                      ;;;
TMR1HP1	EQU	32                      ;;;
TMR1LP1	EQU	33                      ;;;
TMR1UP2	EQU	34                      ;;;
TMR1HP2	EQU	35                      ;;;
TMR1LP2	EQU	36                      ;;;
TMR1UP0	EQU	37                      ;;;
TMR1HP0	EQU	38                      ;;;
TMR1LP0	EQU	39                      ;;;

P1EMPTYL	EQU	3A					;;; hold the lower 8 bit of p1 empty 
P1EMPTYH	EQU	3B                  ;;; hold the upper 8 bit of p1 empty

MULTCOUNT	EQU	3C					;;;;;;;;;;;;;;;;;;;;;; not used in program

TXCOUNT	EQU	3D						;;;;;;;;;;;;;;;;;;;;;;  not used in program
DELAY	EQU	3E						;;;;;;;;;;;;;;;;;;;;;;  not used in program
TXBYTE	EQU	3F						;;;;;;;;;;;;;;;;;;;;;;  not used in program

AARGB0	EQU	40						;;;  Used to store the result of fixed point multiplication	, AARGB0 most significant byte
AARGB1	EQU	41                      ;;;  Used to store the result of fixed point multiplication
AARGB2	EQU	42                      ;;;  Used to store the result of fixed point multiplication
AARGB3	EQU	43                      ;;;  Used to store the result of fixed point multiplication , AARGB3 least significant byte

DIVH	EQU	44						;;; hold the upper 8 bit of divisor
DIVL	EQU	45						;;; hold the lower 8 bit of divisor

M1H	EQU	44							;;; hold the upper 8 bit of multiplier m1
M1L	EQU	45							;;; hold the lower 8 bit of multiplier m1

REMB0	EQU	46
REMB1	EQU	47

M2H	EQU	46							;;; hold the upper 8 bit of multiplier m2
M2L	EQU	47                          ;;; hold the lower 8 bit of multiplier m2

MATHSTEMP	EQU	48
LOOPCOUNT	EQU	49					;;; Hold loop counts
OLOOPCOUNT	EQU	4A                  ;;; Hold OUTER loop counts

OPSPANHI	EQU	4B					;;; To hold the value of Output Span
OPSPANLO	EQU	4C                  ;;; To hold the value of Output Span
OPOFFSETHI	EQU	4D
OPOFFSETLO	EQU	4E
OPALARM	EQU	4F

P2EMPTYL	EQU	50					;;; hold the lower 8 bit of p2 empty 
P2EMPTYH	EQU	51                  ;;; hold the upper 8 bit of p2 empty

ALARMLEVHI	EQU	52
ALARMLEVLO	EQU	53
GENSTA1	EQU	54						;;; Register used to hold flags

CALCP1H	EQU	55						;;; To hold the value of CalcP1H value
CALCP1L	EQU	56                      ;;; To hold the value of CalcP1L value
CALCP2H	EQU	57                      ;;; To hold the value of CalcP2H value
CALCP2L	EQU	58                      ;;; To hold the value of CalcP2L value

CURRDIVH	EQU	59					;;; current division
CURRDIVL	EQU	5A                  ;;; current division
AVDIVU	EQU	5B
AVDIVH	EQU	5C
AVDIVL	EQU	5D
AVREGU	EQU	5E
AVREGH	EQU	5F
AVREGL	EQU	60

P0DFH	EQU	61
P0DFL	EQU	62
P0EMPTYL	EQU	63					;;; hold the lower 8 bit of p0 empty 
P0EMPTYH	EQU	64                  ;;; hold the upper 8 bit of p0 empty
P1DFL	EQU	65						;;; P1 drift factor i.e  = 1.4 x (p0 - p0@empty)
P1DFH	EQU	66                      ;;; P1 drift factor i.e  = 1.4 x (p0 - p0@empty)
P2DFL	EQU	67						;;; P2 drift factor i.e  = 1.05 x (p0 - p0@empty)
P2DFH	EQU	68                      ;;; P2 drift factor i.e  = 1.05 x (p0 - p0@empty)
NAVCOUNT	EQU	69					;;; To hold the value of 'n'
M12L	EQU	6A						;;; Constant 'm'
M12H	EQU	6B                      ;;; Constant 'm'
PROBE1H	EQU	6C
PROBE1L	EQU	6D

DIVATCAL97L	EQU	6E					;;; To store lower limitation of divisor i.e 0.97 x div@cal
DIVATCAL97H	EQU	6F                  ;;; To store lower limitation of divisor i.e 0.97 x div@cal


;accessible from all banks (70-7F)

TASKFLAGS	equ	70					;;; register to store flags
RXREG	EQU	71
RXTXCOUNT	EQU	72
FLAGS232	EQU	73
EEWRADDRESS	EQU	74
CALSTATUS	EQU	75
TYPE	EQU	76

ADVANCED	EQU	77
OPTCOUNT	EQU	78

TEMPHI	EQU	7E
TEMPLO	EQU	7F

;bank 1

TIMEL	EQU	0A0
TIMEH	EQU	0A1						
TIMEU	EQU	0A2
TIMEUU	EQU	0A3						;;; for subroutine RESETSAVETIME, TIMEUU = 1 in this subroutine

;definitions


#DEFINE	_C	STATUS,C

#DEFINE	ENABLEINT	INTCON,7					;;; GLOBAL INTERRUPT

#DEFINE	INTDONE	GENSTA0,0
#DEFINE	LCALWAIT	GENSTA0,1
#DEFINE	LCJUSTDONE	GENSTA0,2
#DEFINE	HCJUSTDONE	GENSTA0,3
#DEFINE	FAULT	GENSTA0,4						;;; NO further occurance
#DEFINE	DRESISTIVE	GENSTA0,5
#DEFINE	COMMSBEENON	GENSTA0,6
#DEFINE	OPREVERSE	GENSTA0,7	;OP falls as level rises

#DEFINE	ALARMOPFLAG	GENSTA1,0	;DO NOT MOVE!!!!
#DEFINE	ALARMONatHI	GENSTA1,1
#DEFINE	DRIFTNEG	GENSTA1,2
#DEFINE	SAVEFLAG	GENSTA1,3
#DEFINE	CDIVISLOW	GENSTA1,4	;current division below div@cal
#DEFINE	OVER20PCCSPAN	GENSTA1,5
#DEFINE	SAVEINDICATOR	GENSTA1,6

#DEFINE	EMPTYCAL	CALSTATUS,0
#DEFINE	SPANCAL	CALSTATUS,1

#DEFINE	ALARMOP	PORTC,4	;was PORTA,1 ###5

	IF	(SWAPPROBEPINS==0)
#DEFINE	ENPROBE	PORTC,2	;was 4 ###5
#DEFINE	ENREF_P2	PORTC,3
	ELSE
#DEFINE	ENPROBE	PORTC,3
#DEFINE	ENREF_P2	PORTC,2
	ENDIF

#DEFINE	WRITE232	FLAGS232,0
#DEFINE	READ232	FLAGS232,1	
#DEFINE	GOTADDR	FLAGS232,2

#DEFINE	GETTINGP0	TASKFLAGS,0
#DEFINE	GETTINGP1	TASKFLAGS,1
#DEFINE	GETTINGP2	TASKFLAGS,2
#DEFINE	T0COUNTING	TASKFLAGS,3
#DEFINE	T0WAITSTART	TASKFLAGS,4
#DEFINE	SENDPARAMS	TASKFLAGS,5

#DEFINE	TWOPROBE	ADVANCED,7

;constants


boo	equ	.1000000

;BANK0 REGISTER SETTING

INTCONset	EQU	B'11000000'
T1CONset	EQU	B'00000001'
T1GCONset	EQU	B'0'
PR2set	EQU	B'0'
T2CONsetPS1	EQU	B'00000100'		;prescale 1, postscale 1
T2CONsetPS4	EQU	B'00000101'		;prescale 4, postscale 1
T2CONsetPS16	EQU	B'00000110'		;prescale 16, postscale 1
T2CONsetPS64	EQU	B'00000111'		;prescale 64, postscale 1
CPSCON0set	EQU	B'0'
CPSCON1set	EQU	B'0'

;BANK1 REGISTER SETTING

TRISAset     	EQU       	B'11111110'

TRISCset	EQU	B'11000011'

PIE1set	EQU	B'00000001'
PIE2set	EQU	B'0'

OPTsetPS2	EQU	B'01100000'		;pull up on. TMR0 PS=2
OPTsetPS4	EQU	B'01100001'		;pull up on. TMR0 PS=4
OPTsetPS8	EQU	B'01100010'		;pull up on. TMR0 PS=8
OPTsetPS16	EQU	B'01100011'		;pull up on. TMR0 PS=16
OPTsetPS32	EQU	B'01100100'		;pull up on. TMR0 PS=32
OPTsetPS64	EQU	B'01100101'		;pull up on. TMR0 PS=64
OPTsetPS128	EQU	B'01100110'		;pull up on. TMR0 PS=128
OPTsetPS512	EQU	B'01100111'		;pull up on. TMR0 PS=256

OPTsetT0wait	EQU	B'01100000'		;pull up on. TMR0 PS=2
PCONset	EQU	B'00010000'		;BOR enabled
WDTCONset	EQU	B'00011100'		;1:2^19 (16S)

OSCCONset	EQU	B'01110000'		;8MHz

ADCON0set	EQU	B'0'
ADCON0setRV	EQU	B'00010001'
ADCON1set	EQU	B'10100000'

;BANK2 REGISTER SETTING

CM1CON0set	EQU	B'0'
CM1CON1set	EQU	B'0'
CM2CON0set	EQU	B'0'
CM2CON1set	EQU	B'0'

BORCONset	EQU	B'0'
FVRCONset	EQU	B'0'
DACCON0set	EQU	B'0'
DACCON1set	EQU	B'0'
SRCON0set	EQU	B'0'
SRCON1set	EQU	B'0'

APFCONset	EQU	B'10000100'


;BANK3 REGISTER SETTING

ANSELAset	EQU	B'0'
	
ANSELCset	EQU	B'00000001'

EECON1set	EQU	B'0'
EECON2set	EQU	B'0'

SPBRGLset	EQU	LOW .520
SPBRGHset	EQU	HIGH .520

RCSTAset	EQU	B'10010000'
TXSTAset	EQU	B'00100100'
BAUDCONset	EQU	B'00001000'

;BANK4 REGISTER SETTING

WPUAset	EQU	B'00000011'

WPUCset	EQU	B'0'

SSP1ADDset	EQU	B'0'
SSP1MSKset	EQU	B'0'

SSP1STATset	EQU	B'0'
SSP1CON1set	EQU	B'0'
SSP1CON2set	EQU	B'0'
SSP1CON3set	EQU	B'0'

;BANK5 REGISTER SETTING

CCP1CONset	EQU	B'00001100'
PWM1CONset	EQU	B'0'
CCP1AScon	EQU	B'0'
PSTR1CONset	EQU	B'00000001'

;BANK6 REGISTER SETTING

;BANK7 REGISTER SETTING

IOCAPset	EQU	B'0'
IOCANset	EQU	B'0'

CLRKRCON	EQU	B'0'

MDCONset	EQU	B'0'
MDSRCset	EQU	B'0'
MDCARLset	EQU	B'0'
MDCARHset	EQU	B'0'

;general constants

LSB	EQU	.0
MSB     	EQU     	.7

	IF	(SWAPPROBEPINS==0)
enP1	EQU	B'00000100'
enref_P2	EQU	B'00001000'
	ELSE
enP1	EQU	B'00001000'
enref_P2	EQU	B'00000100'
	ENDIF

emptydone	EQU	B'00000001'
spandone	EQU	B'00000011'

lowoffset	EQU	.0
minpwm	EQU	.0
uncalpwm	EQU	.0	;.512
tmr1shift	EQU	.4

ADwait	EQU	.6	;uSec @ 20MHz

;fudge factors

defp1df	EQU	.358	;1.40 x 2^8 ###t was 1.45
defp2df	EQU	.269	;1.05 x 2^8
defaultn	EQU	.2

const1  	EQU	.58982	;(14.4 x 2^12) ###t was 13
const2	EQU	.28959	;(7.07 x 2^12)rounded up ###t was 4.726
const3	EQU	.25808	;(6.3 x 2^12)rounded up ###t was 7.43

onepdivH	EQU	HIGH .4096	;2^12
onepdivL	EQU	LOW .4096


;EE Addresses

calstatus_EA			EQU	.1
P1emptyL_EA				EQU	.2
P1emptyH_EA				EQU	.3
P2emptyL_EA				EQU	.4
P2emptyH_EA				EQU	.5
P0emptyL_EA				EQU	.6
P0emptyH_EA				EQU	.7

P1fullL_EA				EQU	.8
P1fullH_EA				EQU	.9
P2fullL_EA				EQU	.10
P2fullH_EA				EQU	.11
	
caldiv12L_EA			EQU	.12
caldiv12H_EA			EQU	.13
			
P1calspanL_EA			EQU	.14
P1calspanH_EA			EQU	.15

m12L_EA					EQU	.16
m12H_EA					EQU	.17

currdiv12L_EA			EQU	.18
currdiv12H_EA			EQU	.19

type_EA					EQU	.35
OPspanH_EA				EQU	.36
OPspanL_EA				EQU	.37
OPminoffsetH_EA			EQU	.38
OPminoffsetL_EA			EQU	.39
OPalarmH_EA				EQU	.40
OPalarmL_EA				EQU	.41
advanced_EA				EQU	.42

P1dfL_EA				EQU	.50
P1dfH_EA				EQU	.51
P2dfL_EA				EQU	.52
P2dfH_EA				EQU	.53
n_av_EA					EQU	.54



;power up entry *************************************************************

	ORG	00

    	PAGESEL 	CSTART
   	PAGESEL	CSTART
	GOTO    	CSTART	; When using debug header, first inst.
                              	; may be passed over by ICD2.
	GOTO      	CSTART


;interrupt vector ***********************************************************

; Note the 16F1826/27 family automatically handles context restoration for 
; W, STATUS, BSR, FSR, and PCLATH

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;ISR START ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	ORG       04
	
	BANKSEL	0	
	BTFSC	T0COUNTING
	BTFSS	INTCON,TMR0IF
	GOTO	NOTT0INT	
	BCF	T1CON,TMR1ON	;stop timer	

	BTFSC	PIR1,TMR1IF	;check for overflow
	INCF	TMR1U	
	BCF	T0COUNTING

	BCF	INTCON,TMR0IE	;disable interrupt	;###m moved
	CALL	ADDTOTIMEp			;###m moved

	CALL	TIMESHIFT			;###m new

	BTFSC	GETTINGP1
	CALL	TRANSFERP1
	BTFSC	GETTINGP2
	CALL	TRANSFERP2
	BTFSC	GETTINGP0
	CALL	TRANSFERP0

;	BCF	INTCON,TMR0IE	;disable interrupt	;###m
;	CALL	ADDTOTIMEp			;###m
		
	BTFSS	PIR1,TMR1IF
	GOTO	INTENDNOFLAG
	BCF	PIR1,TMR1IF
	GOTO	INTEND

NOTT0INT	BCF	PIR1,TMR1IF							;;; PERIPHERAL INTERRUPT REQUEST REGISTER 1, 0 = Interrupt is not pending
	BTFSC	T0COUNTING
	INCF	TMR1U
	BTFSS	T0COUNTING
	CALL	ADDTOTIMEt
	
COMMSTIMEOUT	TSTF	RXTXCOUNT
	SKPZ
	DECF	RXTXCOUNT

COMMTEST	BTFSC	PIR1,RCIF
	GOTO	DOCOMMS

	BTFSS	T0WAITSTART	;ready to start cycle?
	GOTO	INTEND	;no
	
T0SYNCH	BCF	T1CON,TMR1ON		;stop TMR1
	CLRF	TMR1L
	CLRF	TMR1H
	CLRF	TMR1U

	MOVLW	LOW OPTION_REG	;for INDF0
	MOVWF	FSR0L
	MOVLW	HIGH OPTION_REG
	MOVWF	FSR0H

	MOVLW	OPTsetT0wait							;;; OPTsetT0wait	EQU	B'01100000'		;pull up on. TMR0 PS=2
	MOVWF	INDF0
	
	MOVLW	0FF
	MOVWF	TMR0
	BCF	INTCON,TMR0IF								;;; 0 = TMR0 register did not overflow
	BTFSS	INTCON,T0IF
	GOTO	($-1)	;loop until rollover
;	MOVLW	OPTset	;ready
	MOVFW	OPTCOUNT	;###6
	CLRF	TMR0	;clear timer
	MOVWF	INDF0	;change prescale
	BSF	T1CON,TMR1ON	;start TMR1
	BCF	INTCON,TMR0IF	;clear flag
	BSF	INTCON,TMR0IE	;enable interrupt
	BCF	T0WAITSTART	;signal wait over
	BSF	T0COUNTING	;signal T0 running
	GOTO	INTEND	;exit

DOCOMMS	BSF	COMMSBEENON	;signal comms activity - no PWM update

	BANKSEL	RCREG
	MOVFW	RCREG
	BANKSEL	0
	MOVWF	RXREG
	BCF	SENDPARAMS	;flag off

	TSTF	RXTXCOUNT
	BNZ	PROCESS232
	CLRF	FLAGS232

WRITECHECK	MOVFW	RXREG
	XORLW	.170	;is it 170 (write)
	BNZ	READCHECK
WRITESTART	COMF	RXREG,W
	CALL	SENDTXW	;send back inverted
	BSF	WRITE232
	MOVLW	.11
	MOVWF	RXTXCOUNT
	GOTO	INTEND

READCHECK	MOVFW	RXREG
	XORLW	.85	;is it 85 (read)
	BNZ	DIAGNCHECK
READSTART	COMF	RXREG,W
	CALL	SENDTXW	;send back inverted
	BSF	READ232
	MOVLW	.11	;WAS 11
	MOVWF	RXTXCOUNT
	GOTO	INTEND

DIAGNCHECK	MOVFW	RXREG
	XORLW	0A5	;is it 0A5 (get P1/P2/PWM)
	BNZ	GETEESTORE
	BSF	SENDPARAMS	;flag to send at every update	
	GOTO	INTEND

GETEESTORE	MOVFW	RXREG
	XORLW	05A	;is it 05A (get EEstore)
	SKPNZ
	CALL	SENDEESTORE
	GOTO	INTEND

PROCESS232	MOVLW	.5	;was 5
	MOVWF	RXTXCOUNT

	BTFSC	WRITE232
	GOTO	WRITE232DO
READ232DO	MOVFW	RXREG	;address
	CALL	EEREADADRW
	CALL	SENDTXW	;send back data
	GOTO	INTEND


WRITE232DO	BTFSC	GOTADDR
	GOTO	WRITE232DOa
	MOVFW	RXREG
	MOVWF	EEWRADDRESS
	COMF	RXREG,W
	CALL	SENDTXW	;send address back inverted	
	;DECF	RXREG,W	;allows use of EEWRITEWNEXT
	;BANKSEL	EEADRL
	;MOVLW	.31
	;MOVWF	EEADRL
	BANKSEL	0
	BSF	GOTADDR
	GOTO	INTEND

WRITE232DOa	MOVFW	RXREG
	CALL	EEWRITEW
	BANKSEL	EEDATL
	COMF	EEDATL	;invert for test to see if it's restored
	BANKSEL	0
	
	CALL	EEREADSAME
	CALL	SENDTXW	;send EE data back
	INCF	EEWRADDRESS	;next

MAININT
	
	
	
INTEND	INCF	FLASHER
	BANKSEL	0
	BSF	INTDONE
INTENDNOFLAG	BSF	T1CON,TMR1ON	;make sure timer running	
	RETFIE	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;ISR END ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;	

;tables/vectors *************************************************************

COUNTTABLE	MOVFW	ADVANCED	;all ###67   
	ANDLW	B'00000111'
	ADDWF	PCL
	RETLW	OPTsetPS2
	RETLW	OPTsetPS4
	RETLW	OPTsetPS8
	RETLW	OPTsetPS16
	RETLW	OPTsetPS32
	RETLW	OPTsetPS64
	RETLW	OPTsetPS128
	RETLW	OPTsetPS512

INTERVALTABLE	SWAPF	ADVANCED,W	;all ###6
	ANDLW	B'00000011'
	ADDWF	PCL
	RETLW	B'00011111'
	RETLW	B'00111111'
	RETLW	B'01111111'
	RETLW	B'11111111'

;subroutines **************************************************************************************************************

TESTSENDTXW	BTFSS	SENDPARAMS
	RETURN

SENDTXW	BTFSS	PIR1,TXIF	;high if empty
	GOTO	($-1)	;loop is not empty
	BANKSEL	TXREG
	MOVWF	TXREG
	BANKSEL	0
	RETURN

SENDEESTORE	MOVLW 	calstatus_EA	;###11 additions/notes
	CALL	EEREADADRW
	CALL	SENDTXW	;calstat
	CALL	EEREADNEXT
	CALL	SENDTXW	;p1ecalL
	CALL	EEREADNEXT
	CALL	SENDTXW	;p1ecalH
	CALL	EEREADNEXT
	CALL	SENDTXW	;p2ecalL
	CALL	EEREADNEXT
	CALL	SENDTXW	;p2ecalH
	CALL	EEREADNEXT
	CALL	SENDTXW	;p0calL
	CALL	EEREADNEXT
	CALL	SENDTXW	;p0calH
	CALL	EEREADNEXT
	CALL	SENDTXW	;p1fcalL
	CALL	EEREADNEXT
	CALL	SENDTXW	;p1fcalH
	CALL	EEREADNEXT
	CALL	SENDTXW	;p2fcalL
	CALL	EEREADNEXT
	CALL	SENDTXW	;p2fcalH	
	CALL	EEREADNEXT	
	CALL	SENDTXW	;divcalL
	CALL	EEREADNEXT	
	CALL	SENDTXW	;divcalH
	CALL	EEREADNEXT	
	CALL	SENDTXW	;spancalL
	CALL	EEREADNEXT	
	CALL	SENDTXW	;spancalH

	MOVLW	P1dfL_EA	;sends five bytes of calculation parameters
	CALL	EEREADADRW
	CALL	SENDTXW	;p1dfL
	CALL	EEREADNEXT
	CALL	SENDTXW	;p1dfH
	CALL	EEREADNEXT
	CALL	SENDTXW	;p2dfL
	CALL	EEREADNEXT
	CALL	SENDTXW	;p2dfH
	CALL	EEREADNEXT
	CALL	SENDTXW	;n
	
	MOVLW	currdiv12L_EA
	CALL	EEREADADRW
	CALL	SENDTXW	;currdiv12L_EA
	CALL	EEREADNEXT
	CALL	SENDTXW	;currdiv12H_EA
		
	RETURN
	
T2SETUP	MOVLW	T2CONsetPS1
	BTFSC	TYPE,4	;ResD1
	MOVLW	T2CONsetPS4
	BTFSC	TYPE,5	;ResD2
	MOVLW	T2CONsetPS16
	BTFSC	TYPE,6	;ResD3
	MOVLW	T2CONsetPS64
	MOVWF	T2CON	;setup T2
	RETURN

SETUP
SETUPB0	MOVLB	0	;set bank 0
	MOVLW	INTCONset					;;; INTCONset = B'11000000'
	MOVWF	INTCON						;;; INTERRUPT CONTROL REGISTER, global interrupt enable bit = Enables all active interrupts, peripheral entruupt enable bit = Enables all active peripheral interrupts
	MOVLW	T1CONset					;;; T1CONset = B'00000001'
	MOVWF	T1CON						;;; TIMER1 CONTROL REGISTER, TIMER 1 ON
	MOVLW	T1GCONset					;;; T1GCONset = B'0'
	MOVWF	T1GCON						;;; TIMER1 GATE CONTROL REGISTER , If TMR1ON = 1: d7 0 = Timer1 counts regardless of Timer1 gate function
	MOVLW	PR2set						;;; PR2set = B'0'
	MOVWF	PR2							;;; Timer2 Period Register
;	MOVLW	T2CONsetPS4	;#1	
	MOVLW	0
	MOVWF	T2CON						;;; TIMER2 CONTROL REGISTER,  TIMER 2 IS OFF
	MOVLW	CPSCON0set					;;; CPSCON0set = B'0'
	MOVWF	CPSCON0						;;; CAPACITIVE SENSING CONTROL REGISTER 0
	MOVLW	CPSCON1set					;;; CPSCON1set	EQU	B'0'
	MOVWF	CPSCON1						;;; 


SETUPB1	MOVLB	.1	;bank 1
	MOVLW	TRISAset 					;;; TRISAset = B'11111110'
	MOVWF	TRISA						;;; PA0 Make as output

	IFDEF __16F1823
	MOVLW	TRISCset 					;;; TRISCset = B'11000011'
	MOVWF	TRISC						;;; make PC2-PC5 as output others as input
	ENDIF
	
	IFDEF __16LF1823
	MOVLW	TRISCset 
	MOVWF	TRISC
	ENDIF
    		
	MOVLW	PIE1set						;;; PIE1set = B'00000001'
	MOVWF	PIE1						;;; PERIPHERAL INTERRUPT ENABLE REGISTER 1, Enables the Timer1 overflow interrupt
	MOVLW	PIE2set						;;; PIE2set = 0
	MOVWF	PIE2						;;; PERIPHERAL INTERRUPT ENABLE REGISTER 2

	MOVLW	OPTsetPS64					;;; OPTsetPS64 = B'01100101'
	MOVWF	OPTION_REG					;;; OPTION REGISTER, Interrupt on rising edge of RA2/INT pin,Transition on RA2/T0CKI pin
										;;; Increment on low-to-high transition on RA2/T0CKI pin, Prescaler 1 : 64
	MOVLW	PCONset						;;; PCONset = B'00010000'  ;BOR enabled, NEED TO CHECK IT 
	MOVWF	PCON
	MOVLW	WDTCONset					;;; WDTCONset	EQU	B'00011100'		;1:2^19 (16S)
	MOVWF	WDTCON						;;; 01110 = 1:524288 (2^19) (Interval 16s typ)  Bit Value = Prescale Rate
	MOVLW	OSCCONset					;;; OSCCONset = B'01110000'
	MOVWF	OSCCON						;;; OSCILLATOR CONTROL REGISTER 8 MHz selected
	MOVLW	ADCON0set					;;; ADCON0set	EQU	B'0'
	MOVWF	ADCON0
	MOVLW	ADCON1set					;;; ADCON1set	EQU	B'10100000'
	MOVWF	ADCON1						;;; A/D Result Format Select bit = Left justified. A/D Conversion Clock Select bits = FOSC/32 


SETUPB2	MOVLB	.2	;bank 2	
	MOVLW	CM1CON0set					;;; CM1CON0set = 0 
	MOVWF	CM1CON0						;;; COMPARATOR 1 CONTROL REGISTER 0
	MOVLW	CM1CON1set					;;; CM1CON1set = 0 
	MOVWF	CM1CON1						;;; COMPARATOR CX CONTROL REGISTER 1
	
	IFDEF __16F1823
	MOVLW	CM2CON0set					;;; 0
	MOVWF	CM2CON0
	MOVLW	CM2CON1set					;;; 0
	MOVWF	CM2CON1
	ENDIF

	IFDEF __16LF1823
	MOVLW	CM2CON0set	
	MOVWF	CM2CON0
	MOVLW	CM2CON1set
	MOVWF	CM2CON1
	ENDIF

	MOVLW	BORCONset					;;; 0
	MOVWF	BORCON						;;; ROWN-OUT RESET CONTROL REGISTER
	MOVLW	FVRCONset					;;; 0
	MOVWF	FVRCON						;;; FIXED VOLTAGE REFERENCE CONTROL REGISTER
	MOVLW	DACCON0set					;;; 0
	MOVWF	DACCON0						;;; DIGITAL TO ANALOG CONROL REGISTER
	MOVLW	DACCON1set					;;; 0
	MOVWF	DACCON1
	MOVLW	SRCON0set					;;; 0
	MOVWF	SRCON0						;;; SR LATCH CONTROL 0 REGISTER
	MOVLW	SRCON1set					;;; 0
	MOVWF	SRCON1
	
	MOVLW	APFCONset					;;; APFCONset = B'10000100'
	MOVWF	APFCON						;;; Alternate Pin Function Control, RX/DT function is on RA5,  TX/CK function is on RA0
	
SETUPB3	MOVLB	.3	;bank 3
	MOVLW	ANSELAset					;;; ANSELAset = 0 
	MOVWF	ANSELA						;;; PORTA ANALOG SELECT REGISTER, 

	IFDEF __16F1823
	MOVLW	ANSELCset					;;; ANSELCset = B'00000001'
	MOVWF	ANSELC						;;; PORTC ANALOG SELECT REGISTER, RC0 set as analog input, Digital input buffer disabled.
	ENDIF

	IFDEF __16LF1823
	MOVLW	ANSELCset
	MOVWF	ANSELC
	ENDIF

	MOVLW	EECON1set					;;; 0
	MOVWF	EECON1						;;; EEPROM CONTROL 1 REGISTER
	MOVLW	EECON2set					;;; 0
	MOVWF	EECON2
	MOVLW	SPBRGLset					;;; SPBRGLset	EQU	LOW .520
	MOVWF	SPBRGL						;;; baud rate generator
	MOVLW	SPBRGHset					;;; SPBRGHset	EQU	HIGH .520
	MOVWF	SPBRGH						;;; baud rate generator
	MOVLW	RCSTAset					;;; B'10010000'
	MOVWF	RCSTA						;;; RECEIVE STATUS AND CONTROL REGISTER, serial port enable, enable reciever
	MOVLW	TXSTAset					;;; B'00100100'
	MOVWF	TXSTA						;;; TRANSMIT STATUS AND CONTROL REGISTER, transmit enable
	MOVLW	BAUDCONset					;;; B'00001000'
	MOVWF	BAUDCON						;;; BAUD RATE CONTROL REGISTER, 16 bit baud rate generator used

SETUPB4	MOVLB	.4	;bank 4
	MOVLW	WPUAset						;;; B'00000011'
	MOVWF	WPUA						;;; WEAK PULL-UP PORTA REGISTER, PA0,PA1 pullup enable

	IFDEF __16F1823
	MOVLW	WPUCset						;;; 0
	MOVWF	WPUC						;;; WEAK PULL-UP PORTC REGISTER, DISABLED
	ENDIF
	
	IFDEF __16LF1823
	MOVLW	WPUCset
	MOVWF	WPUC
	ENDIF


	MOVLW	SSP1ADDset					;;; 0
	MOVWF	SSP1ADD						;;; MSSP1 ADDRESS AND BAUD RATE REGISTER (I2C MODE)
	MOVLW	SSP1MSKset					;;; 0
	MOVWF	SSP1MSK

	MOVLW	SSP1STATset
	MOVWF	SSP1STAT
	MOVLW	SSP1CON1set
	MOVWF	SSP1CON1
	MOVLW	SSP1CON2set
	MOVWF	SSP1CON2
	MOVLW	SSP1CON3set
	MOVWF	SSP1CON3


SETUPB5	MOVLB	.5	;bank 5
	CLRF	CCPR1L					;;; Capture/Compare/PWM Register x low
	MOVLW	CCP1CONset				;;; B'00001100'
	MOVWF	CCP1CON					;;; CCP1 CONTROL REGISTER, PWM mode: P1A, P1C active-high; P1B, P1D active-high
	MOVLW	PWM1CONset				;;; 0
	MOVWF	PWM1CON					;;; ENHANCED PWM CONTROL REGISTER
	MOVLW	CCP1AScon				;;; 0
	MOVWF	CCP1AS					;;; CCP1 AUTO-SHUTDOWN CONTROL REGISTER
	MOVLW	PSTR1CONset				;;; B'00000001'
	MOVWF	PSTR1CON				;;; PWM STEERING CONTROL REGISTER,P1A pin has the PWM waveform with polarity control from CCP1M<1:0> 
    CLRF	CCPR1L
;no SETUPB6	

SETUPB7	MOVLB	.7	;bank 7
	MOVLW	IOCAPset				;;; 0
	MOVWF	IOCAP					;;; INTERRUPT-ON-CHANGE PORTA POSITIVE EDGE REGISTER
	MOVLW	IOCANset				;;; 0
	MOVWF	IOCAN					;;; INTERRUPT-ON-CHANGE PORTA NEGATIVE EDGE REGISTER

	MOVLB	0	;bank 0
	RETURN

INITMR1	CLRF	TMR1L				;;; CLEAR TIMER 1 LOWER COUNT REG
	CLRF	TMR1H					;;; CLEAR TIMER 1 LOWER COUNT REG
	MOVLW	T1CONset				;;; B'00000001'
	MOVWF	T1CON					;;; TIMER1 CONTROL REGISTER. TIMER 1 ON
	BCF	PIR1,TMR1IF					;;; PERIPHERAL INTERRUPT REQUEST REGISTER 1, TMR1IF =0 ; Interrupt is not pending
	RETURN

EEREADNEXT	
	BANKSEL	EEADRL
	INCF	EEADRL,W
EEREADADRW			;get data from EEPROM, addr in W
	BANKSEL	EEADRL					;;; bank select
	MOVWF	EEADRL					;;; COPY address stored in w into EEADRL
	
EEREADSAME
	BANKSEL	EECON1					;;; bank select EEPROM CONTROL 1 REGISTER
	BTFSC	EECON1,WR				;;; check operation with eeprom, if wr = 1 means busy
	GOTO	($-1)
	CLRF	EEDATL	;				;;; cleared EEPROM DATA REGISTER
	BCF	EECON1,CFGS					;;; CFGS = 0; Accesses Flash Program or data EEPROM Memory
	BCF	EECON1,EEPGD				;;; EEPGD = 0 ; Accesses data EEPROM memory
	BSF	EECON1,RD					;;; RD= 1; Initiates an program Flash or data EEPROM read.
	MOVFW	EEDATL					;;; now data from eeprom location is in w
	BANKSEL	0
	RETURN


EEWRITEWNEXT	
	BANKSEL	EECON1
	MOVWF	EEDATL
	INCF	EEADRL
	GOTO	EEWRSETUP

EEWRITEW			;data in W, address in EEWRADDRESS
	BANKSEL	EECON1
	MOVWF	EEDATL	
	MOVFW	EEWRADDRESS
	MOVWF	EEADRL

EEWRSETUP	BTFSC	EECON1,WR
	GOTO	($-1)
	BCF	EECON1,CFGS
	BCF	EECON1,EEPGD
	BSF	EECON1,WREN
	
	BTFSS	INTCON,GIE	;int enabled?
	GOTO	EEWRITEMAIN	;int already off, exit through this

	BCF	INTCON,GIE	;interrupt off
	BTFSC	INTCON,GIE	;check
	GOTO	($-2)
	CALL	EEWRITEMAIN
	BSF	INTCON,GIE	;interrupt on
	RETURN

EEWRITEMAIN	MOVLW	55
	MOVWF	EECON2
	MOVLW	0AA
	MOVWF	EECON2
	BSF	EECON1,WR
	BCF	EECON1,WREN

	BTFSC	EECON1,WR
	GOTO	($-1)

	BANKSEL	0
	RETURN

PWMSET	SWAPF	PWML,W
	ANDLW	B'00110000'
	MOVWF	PWMTEMP	;store lower bits in correct position
	RRF	PWMH
	RRF	PWML
	RRF	PWMH
	RRF	PWML,W	;MS 8-bits in W
	BANKSEL	CCPR1L
	MOVWF	CCPR1L
	BANKSEL	0
	MOVFW	PWMTEMP
	BANKSEL	CCP1CON
	BCF	CCP1CON,4
	BCF	CCP1CON,5
	IORWF	CCP1CON
	BANKSEL	0
	RETURN

CALCPERCENT	CLRF	M1H	;all ###4
	MOVLW	.100
	MOVWF	M1L
	CALL	CALCMAIN
	BTFSC	AARGB3,7	;negative? ###s
	CLRF	AARGB3	;yes, value 0	###j here


CALCPERCENTa 	MOVLW	OPalarmH_EA	;get alarm parameters
	CALL	EEREADADRW
	MOVWF	ALARMLEVHI
	CALL	EEREADNEXT
	MOVWF	ALARMLEVLO

	BCF	ALARMONatHI	;extract alarm flag
	BTFSC	ALARMLEVHI,7
	BSF	ALARMONatHI

	MOVLW	B'01111111'
	ANDWF	ALARMLEVHI	;remove alarm flag

SETALOPFLAG	MOVFW	AARGB3
	SUBWF	ALARMLEVHI,W
	SKPC
	BSF	ALARMOPFLAG
	
	MOVFW	ALARMLEVLO
	SUBWF	AARGB3,W
	SKPC
	BCF	ALARMOPFLAG	

	BTFSS	ALARMONatHI
	GOTO	SETALARMOPLO

SETALARMOPHI	BTFSC	ALARMOPFLAG	;###5 here
	BSF	ALARMOP
	BTFSS	ALARMOPFLAG
	BCF	ALARMOP	;###5 to here
	RETURN
	
SETALARMOPLO	BTFSC	ALARMOPFLAG	;###5 here
	BCF	ALARMOP
	BTFSS	ALARMOPFLAG
	BSF	ALARMOP	;###5 to here
	RETURN

		
;**********************************************************************
;(CAP RESULT x  OPSPAN)/SPAN 
;Multiplication by output span in PWM terms


;CALCULATE 	###r	
GETOPPARAM	MOVLW	OPspanH_EA	;get op parameters
	CALL	EEREADADRW
	MOVWF	OPSPANHI

	BCF	OPREVERSE	;extract reverse flag
	BTFSC	OPSPANHI,7
	BSF	OPREVERSE

	MOVLW	B'00000111'
	ANDWF	OPSPANHI	;remove reverse flag
	MOVFW	OPSPANHI
	MOVWF   	M1H

	CALL	EEREADNEXT
	MOVWF	OPSPANLO
	MOVWF   	M1L
	CALL	EEREADNEXT
	MOVWF	OPOFFSETHI
	CALL	EEREADNEXT
	MOVWF	OPOFFSETLO
	RETURN
	
CALCMAIN	MOVFW	PROBE1H
	MOVWF   	M2H
  	MOVFW	PROBE1L
	MOVWF   	M2L

	CALL    	MULT16X16	;result in AARGB0-4

	MOVFW	P1SPANATCALL	;get SPAN into divisor
	MOVWF	DIVL
	MOVFW	P1SPANATCALH
	MOVWF	DIVH
		
	CALL	UDIV3216L	;result in AARGB3(L) & AARGB2(H)
	RETURN

SPANCHECK	MOVFW	OPSPANLO		;MAXSPAN - SPANCALCRESULT
	SUBWF	AARGB3,W	
	MOVFW	OPSPANHI		
	SUBWFB	AARGB2,W
	SKPC
	RETURN
	
SETMAXSPAN	MOVFW	OPSPANLO	;if calculated span too great, set to max
	MOVWF	AARGB3
	MOVFW	OPSPANHI
	MOVWF	AARGB2
	RETURN

;16-Bit subtract with valid carry-out (zero flag invalid after subtraction)

REVERSEPWM	MOVFW	AARGB3	;DEST = DEST-SOURCE
	SUBWF	OPSPANLO,W	;
	MOVWF	AARGB3	;return to AARG
	MOVFW	AARGB2	;
	SKPC		;
	INCF	AARGB2,W	;was INCFSZ
	SUBWF	OPSPANHI,W
	MOVWF	AARGB2	;return to AARG
	RETURN

;*******************************************************************

;Primes average with stored average

AVPRIME	MOVLW	n_av_EA							;;; copy n_av_EA eeprom address in w
	CALL	EEREADADRW							;;; read data from eeprom location which is stored in W
	MOVWF	NAVCOUNT							;;; value from eeprom location n_av_EA stored in NAVCOUNT(location 69)
	MOVWF	LOOPCOUNT							;;; same value copied into LOOPCOUNT (location 49)
	CLRF	AVREGU								;;; AVREGU	EQU	5E, clear

	MOVLW	currdiv12L_EA
	CALL	EEREADADRW
	MOVWF	AVREGL								;;; copy from currdiv12L_EA eeprom to AVREGL	EQU	60, 
	MOVWF	CURRDIVL							;;; copy from eeprom to CURRDIVL	EQU	5A                  ;;; current division
	MOVWF	AVDIVL_OVER20	;###j				;;; copy from eeprom to AVDIVL_OVER20

	CALL	EEREADNEXT							;;; read data from next location
	MOVWF	AVREGH								;;; copy from currdiv12L_EA + 1 eeprom to AVREGH
	MOVWF	CURRDIVH							;;; copy from eeprom to CURRDIVH	EQU	59
	MOVWF	AVDIVH_OVER20	;###j				;;; same copy in AVDIVH_OVER20	EQU	29

PRIMELOOP	CLRC								;;; cleard carry bit in status
	RLF	AVREGL									;;; Rotate Left f through Carry
	RLF	AVREGH
	RLF	AVREGU
	DECFSZ	LOOPCOUNT
	GOTO	PRIMELOOP
	RETURN

;*******************************************************************

;16 BIT WINDOW AVERAGE with window controlled by n_av_EA (2 to 8)

EXTRACTAVDIV	MOVFW	NAVCOUNT	;###h alterations made
	MOVWF	LOOPCOUNT

	MOVFW	AVREGU
	MOVWF	AVDIVU
	MOVFW	AVREGH
	MOVWF	AVDIVH
	MOVFW	AVREGL
	MOVWF	AVDIVL

EXTRACTLOOP	CLRC
	RRF	AVDIVU
	RRF	AVDIVH
	RRF	AVDIVL	;divide by 2
	DECFSZ	LOOPCOUNT
	GOTO	EXTRACTLOOP
	RETURN
	
DOAVERAGE	CALL	EXTRACTAVDIV

	
TRANSIFOVER20	BTFSS	OVER20PCCSPAN	;###j - over 20% of spancal?	
	GOTO	DOAVERAGEa	;no, move on
	MOVFW	AVDIVL
	MOVWF	AVDIVL_OVER20
	MOVFW	AVDIVH
	MOVWF	AVDIVH_OVER20
	
DOAVERAGEa	MOVFW	CURRDIVL	;add in new
	ADDWF	AVREGL
	MOVFW	CURRDIVH
	
	ADDWFC	AVREGH
	CLRW
	ADDWFC	AVREGU

	MOVFW	AVDIVL	;subtract old
	SUBWF	AVREGL
	MOVFW	AVDIVH
	SUBWFB	AVREGH
	MOVFW	AVDIVU
	SUBWFB	AVREGU
	
	RETURN

		
;*******************************************************************

;16x16 FIXED POINT MULTIPLICATION ROUTINE (PM: 26, DM: 11)
;Input:  fixed point arguments in M1H/L and M2H/L
;Output: AARG0-4 (AARGB0 is most significant byte)


MULT16X16	CLRF	AARGB0
	CLRF	AARGB1
	CLRF	AARGB2
	CLRF	AARGB3
	
	MOVLW	.16
	MOVWF	OLOOPCOUNT
	
MULT16X16LOOP	RLF	M1L
	RLF	M1H
	BNC	ROTATE32

ADDM2TOTOTAL	MOVFW	M2L
	ADDWF	AARGB3
	MOVFW	M2H
	ADDWFC	AARGB2
	CLRW	
	ADDWFC	AARGB1
	CLRW	
	ADDWFC	AARGB0

ROTATE32	DECFSZ	OLOOPCOUNT
	GOTO	ROTATE32a
	RETURN

ROTATE32a	CLRC
	RLF	AARGB3
	RLF	AARGB2
	RLF	AARGB1
	RLF	AARGB0
	GOTO	MULT16X16LOOP
	

;start of division *********************************************************************

;32/16 FIXED POINT DIVIDE ROUTINE (PM: 68, DM: 11)
;Input:  fixed point arguments in AARG and DIVH/L (AARGB0 is most significant byte)
;Output: quotient AARG/DIV followed by remainder in REM

;AARGB0-3 can be anywhere in data memory but must be in one bank and in the order shown

UDIV3216L	MOVFW	DIVH		;test & exit if zero divisor
	IORWF	DIVL,W
	SKPNZ
	RETURN

	CLRF	REMB0
	CLRF	REMB1

	MOVLW	HIGH AARGB0		;set up address pointer
	MOVWF	FSR0H
	MOVLW	LOW AARGB0
	MOVWF	FSR0L

	CLRF	MATHSTEMP
	RLF	INDF0,W		;#0
	RLF	REMB1
	MOVFW	DIVL

	CALL	SUB1

	RLF	INDF0		;#0

	MOVLW	.4
	MOVWF	OLOOPCOUNT		;set outer loop

INNERLOOP	MOVLW           7
	MOVWF           LOOPCOUNT

LOOPU3216A	CALL	ROTATE1		;###X
	BTFSS	INDF0,LSB		;#0
	GOTO	UADD3216A

	CALL	SUB1
	GOTO	UOK3216A

UADD3216A	CALL	ADD1
        
UOK3216A 	RLF	INDF0		;#0

              DECFSZ	LOOPCOUNT
	GOTO	LOOPU3216A

	DECFSZ	OLOOPCOUNT,W		;if last part of loop, miss next part
	GOTO	INCPOINTER
	GOTO	LASTPART

INCPOINTER	INCF	FSR0L		;point to next byte

	CALL	ROTATE1
	DECF	FSR0L		;back to previous
                
	BTFSS	INDF0,LSB		;#0
	GOTO	UADD3216B

	CALL	SUB1
	GOTO	UOK3216B

UADD3216B	CALL	ADD1
        
UOK3216B 	INCF	FSR0L
	RLF	INDF0		;##1

	DECFSZ	OLOOPCOUNT
	GOTO	INNERLOOP

LASTPART	BTFSC	INDF0,LSB		;####3
	RETURN
	MOVF	DIVL,W
	CALL	ADD1		;with extra un-needed bits
	RETURN

;division subroutines

ROTATE1      	RLF	INDF0,W		;#0
             	RLF	REMB1
	RLF	REMB0
	RLF	MATHSTEMP
	MOVFW	DIVL
	RETURN

SUB1   	SUBWF	REMB1
	MOVFW	DIVH
	SUBWFB	REMB0
	CLRW
	SUBWFB	MATHSTEMP
	RETURN


ADD1	ADDWF	REMB1
	MOVFW	DIVH
	ADDWFC	REMB0
	CLRW
	ADDWFC	MATHSTEMP
	RETURN

	
;end of division ***********************************************************************


MT_DF_M_N_DVC	MOVLW	P1emptyL_EA	;get stored empty values
	CALL	EEREADADRW
	MOVWF	P1EMPTYL
	CALL	EEREADNEXT
	MOVWF	P1EMPTYH	
;	BTFSS	TWOPROBE	;###8 HERE
;	RETURN		;miss next if single probe

	CALL	EEREADNEXT
	MOVWF	P2EMPTYL
	CALL	EEREADNEXT
	MOVWF	P2EMPTYH

	CALL	EEREADNEXT
	MOVWF	P0EMPTYL
	CALL	EEREADNEXT
	MOVWF	P0EMPTYH

	MOVLW	P1dfL_EA	;get stored drift values 
	CALL	EEREADADRW
	MOVWF	P1DFL
	CALL	EEREADNEXT
	MOVWF	P1DFH
	CALL	EEREADNEXT
	MOVWF	P2DFL
	CALL	EEREADNEXT
	MOVWF	P2DFH

	MOVLW	m12L_EA	;get M value
	CALL	EEREADADRW
	MOVWF	M12L
	CALL	EEREADNEXT
	MOVWF	M12H

	MOVLW	n_av_EA	
	CALL	EEREADADRW
	MOVWF	NAVCOUNT
	
	MOVLW	caldiv12L_EA	;get div @ cal value
	CALL	EEREADADRW
	MOVWF	DIVATCALL
	CALL	EEREADNEXT
	MOVWF	DIVATCALH	
	CALL	EEREADNEXT	;###j here
	MOVWF	P1SPANATCALL
	CALL	EEREADNEXT
	MOVWF	P1SPANATCALH	;###j to here	
	RETURN		;###8 to here

GETSPANCAL	CLRF	PWML
	CLRF	PWMH
	CALL	PWMSET

;	CLRF	EEWRADDRESS	;###10

GETHIGHLEVEL	BSF	ALARMOP	;###7
	;CALL	GETCOMPPROBE	;###10
	CALL	GETP2
	CALL	GETP1

	IF (SETFORTEST==1)
	CALL	GETFALSEDATA
	ENDIF

SAVEP12FULL	MOVLW	P1fullL_EA	;store full values
	MOVWF	EEWRADDRESS
	MOVFW	TMR1HP1
	CALL	EEWRITEW
	MOVFW	TMR1UP1
	CALL	EEWRITEWNEXT
	MOVFW	TMR1HP2
	CALL	EEWRITEWNEXT
	MOVFW	TMR1UP2
	CALL	EEWRITEWNEXT	;###10 to here


GETLOWLEVEL	CALL	MT_DF_M_N_DVC

;###q	BTFSS	TWOPROBE
;###q	GOTO	ONEPSPANCAL

RAWP1SPANCAL	MOVFW	P1EMPTYL	;P1 RAW SPAN = P1-P1EMPTY 
	SUBWF	TMR1HP1	;
	MOVFW	P1EMPTYH	;
	SUBWFB	TMR1UP1

RAWP2SPANCAL	MOVFW	P2EMPTYL	;P2 RAW SPAN = P2-P2EMPTY 
	SUBWF	TMR1HP2	;
	MOVFW	P2EMPTYH	;
	SUBWFB	TMR1UP2

;Calculate M12 = ((const1) - (((const2) x P1 RAW SPAN)/P2 RAW SPAN))
;where const1 = (13 x 2^12) and const2 = (4.726 x 2^12)

M12CAL	MOVFW	TMR1HP1
	MOVWF	M1L
	MOVFW	TMR1UP1
	MOVWF	M1H	;P1 RAW SPAN to M1
	
	MOVLW	LOW const2
	MOVWF	M2L
	MOVLW	HIGH const2
	MOVWF	M2H	;const2 to M2
	
	CALL	MULT16X16	;multiply

	MOVFW	TMR1HP2
	MOVWF	DIVL
	MOVFW	TMR1UP2
	MOVWF	DIVH	;P2 RAW SPAN to DIV
	
	CALL	UDIV3216L	;divide - result in lower 2 bytes of AARG	

	MOVLW	HIGH const1
	MOVWF	M2H
	MOVLW	LOW const1
	MOVWF	M2L	;const1 in M2H

	MOVFW	AARGB3
	SUBWF	M2L
	MOVFW	AARGB2
	SUBWFB	M2H	;const1-result in M2

STOREM12	MOVLW	m12L_EA
	MOVWF	EEWRADDRESS
	MOVFW	M2L
	CALL	EEWRITEW
	MOVFW	M2H
	CALL	EEWRITEWNEXT

CALCDIVATCAL	MOVFW	TMR1HP1
	MOVWF	M1L
	MOVFW	TMR1UP1
	MOVWF	M1H	;P1 RAW SPAN to M1 (M12 still in M2)		

	CALL	MULT16X16	;RAW P1 SPAN x M12 result in AARG

	MOVFW	TMR1HP2
	MOVWF	DIVL
	MOVFW	TMR1UP2
	MOVWF	DIVH	;P2 RAW SPAN to DIV
	
	CALL	UDIV3216L	;((RAW P1 SPAN x M12)/RAW P2 SPAN) - result in lower 2 bytes of AARG	

	MOVLW	LOW const3	;subtract const3 (7.43 x 2^12)= div@cal
	SUBWF	AARGB3
	MOVLW	HIGH const3
	SUBWFB	AARGB2
	
STOREDIVATCAL	MOVLW	caldiv12L_EA
	MOVWF	EEWRADDRESS
	MOVLW	onepdivL	;###q
	BTFSC	TWOPROBE	;###q
	MOVFW	AARGB3
	MOVWF	CURRDIVL
	MOVWF	DIVATCALL
	CALL	EEWRITEW
	MOVLW	onepdivH	;###q
	BTFSC	TWOPROBE	;###q
	MOVFW	AARGB2
	MOVWF	CURRDIVH
	MOVWF	DIVATCALH
	CALL	EEWRITEWNEXT	;store div@cal x 2^12	

	CALL	STOREAVDIV

CALCP1SPAN	MOVFW	TMR1HP1
	MOVWF	M1L
	MOVFW	TMR1UP1
	MOVWF	M1H	;P1 RAW SPAN to M1	

	CLRF	M2L
	MOVLW	HIGH .4096
	MOVWF	M2H	;2^12 to M2
	
	CALL	MULT16X16

;	MOVLW	caldiv12L_EA
;	CALL	EEREADADRW
	MOVFW	DIVATCALL
	MOVWF	DIVL	;###h was remmed
;	CALL	EEREADNEXT
	MOVFW	DIVATCALH
	MOVWF	DIVH
	
	CALL	UDIV3216L	;P1 SPAN = (2^12 x P1 RAW SPAN)/CALDIV12

STOREP1SPAN	MOVLW	P1calspanL_EA
	MOVWF	EEWRADDRESS
	MOVFW	AARGB3
	CALL	EEWRITEW
	MOVFW	AARGB2
	CALL	EEWRITEWNEXT	;store P1 SPAN@cal
	
SAVESPANEND	MOVLW	calstatus_EA
	MOVWF	EEWRADDRESS
	MOVLW	spandone
	MOVWF	CALSTATUS
	CALL	EEWRITEW
			
SAVESPANENDa	BCF	ALARMOP	;###7
	CLRF	FLASHER	;###7 ensure wait
	INCF	FLASHER	;###7 before main
	RETURN

;###q here
;ONEPSPANCAL	MOVFW	P1EMPTYL	;P1 RAW SPAN = P1-P1EMPTY 
;	SUBWF	TMR1HP1	;
;	MOVFW	P1EMPTYH	;
;	SUBWFB	TMR1UP1

;STORERAWSPAN	MOVLW	P1calspanL_EA	;use same location as 2-probe
;	MOVWF	EEWRADDRESS
;	MOVFW	TMR1HP1
;	CALL	EEWRITEW
;	MOVFW	TMR1UP1
;	CALL	EEWRITEWNEXT	;store P1 SPAN@cal

;	GOTO	SAVESPANEND
;###q to here


GETLCAL	BSF	ALARMOP	;###7
	;CLRF	EEWRADDRESS	;###10 here
	;CALL	GETCOMPPROBE
	CALL	GETP2
	CALL	GETP1
	CALL	GETP0	;###10 to here
	
SAVELCAL	MOVLW	calstatus_EA
	MOVWF	EEWRADDRESS	
	MOVLW	emptydone
	MOVWF	CALSTATUS
	CALL	EEWRITEW
	MOVFW	TMR1HP1
	CALL	EEWRITEWNEXT
	MOVFW	TMR1UP1	;save P1E
	CALL	EEWRITEWNEXT		
;###q	BTFSS	TWOPROBE	;###8 HERE
;###q	GOTO	SAVELCALa
	MOVFW	TMR1HP2
	CALL	EEWRITEWNEXT
	MOVFW	TMR1UP2	;save P2E
	CALL	EEWRITEWNEXT	;###8 TO HERE
	MOVFW	TMR1HP0	;###10 here
	CALL	EEWRITEWNEXT
	MOVFW	TMR1UP0	;save P0E
	CALL	EEWRITEWNEXT	;###10 to here

SAVELCALa	BCF	ALARMOP	;###7
	RETURN

STOREAVDIV	MOVLW	currdiv12L_EA
	MOVWF	EEWRADDRESS
	MOVFW	AVDIVL_OVER20
	CALL	EEWRITEW
	MOVFW	AVDIVH_OVER20
	CALL	EEWRITEWNEXT
	RETURN

GETP2	MOVLW	B'11110011'		;both probes disabled
	ANDWF	PORTC
	MOVLW	enref_P2		;set ref/probe2 connection
	IORWF	PORTC
	BSF	GETTINGP2
	BSF	T0WAITSTART
	BTFSC	GETTINGP2
	GOTO	($-1)
	RETURN	
	
TRANSFERP2	MOVFW	TMR1L
	MOVWF	TMR1LP2
	MOVFW	TMR1H
	MOVWF	TMR1HP2	
	MOVFW	TMR1U
	MOVWF	TMR1UP2
	BCF	GETTINGP2
	RETURN


GETP1	MOVLW	B'11110011'		;both probes disabled
	ANDWF	PORTC
	MOVLW	enP1		;set probe connection
	IORWF	PORTC		
	BSF	GETTINGP1
	BSF	T0WAITSTART
	BTFSC	GETTINGP1
	GOTO	($-1)
	RETURN

TRANSFERP1	MOVFW	TMR1L
	MOVWF	TMR1LP1
	MOVFW	TMR1H
	MOVWF	TMR1HP1	
	MOVFW	TMR1U
	MOVWF	TMR1UP1
	BCF	GETTINGP1
	RETURN
	
GETP0	MOVLW	B'11110011'	;###9 here
	ANDWF	PORTC	;both probes de-selected
		
	BSF	GETTINGP0						;;; #DEFINE	GETTINGP0	TASKFLAGS,0 , SET flag
	BSF	T0WAITSTART						;;; #DEFINE	T0WAITSTART	TASKFLAGS,4, set flag
	BTFSC	GETTINGP0
	GOTO	($-1)
	RETURN

TRANSFERP0	MOVFW	TMR1L
	MOVWF	TMR1LP0						;;; TMR1LP0	EQU	39   hold the count of lower P0 but not sure
	MOVFW	TMR1H
	MOVWF	TMR1HP0						;;; TMR1HP0	EQU	38   hold the count of upper P0 but not sure
	MOVFW	TMR1U						;;; not sure which value 
	MOVWF	TMR1UP0
	BCF	GETTINGP0						;;; clear GETTINGP0 flag
	RETURN		;###9 to here


SENDDIAGDATA	BTFSS	SENDPARAMS
	RETURN

	MOVFW	TMR1UP0	;send P0 data
	CALL	TESTSENDTXW
	MOVFW	TMR1HP0
	CALL	TESTSENDTXW
;	MOVFW	TMR1LP0
;	CALL	TESTSENDTXW

	MOVFW	TMR1UP1	;send PROBE1 data
	CALL	TESTSENDTXW
	MOVFW	TMR1HP1
	CALL	TESTSENDTXW
;	MOVFW	TMR1LP1
;	CALL	TESTSENDTXW

	MOVFW	TMR1UP2	;send PROBE2 data
	CALL	TESTSENDTXW
	MOVFW	TMR1HP2
	CALL	TESTSENDTXW
;	MOVFW	TMR1LP2
;	CALL	TESTSENDTXW

	MOVFW	CALCP1H	;send CALCP1
	CALL	TESTSENDTXW	
	MOVFW	CALCP1L
	CALL	TESTSENDTXW
	
	MOVFW	CALCP2H	;send CALCP2
	CALL	TESTSENDTXW	
	MOVFW	CALCP2L
	CALL	TESTSENDTXW

	MOVFW	CURRDIVH	;send DIVISOR
	CALL	TESTSENDTXW	
	MOVFW	CURRDIVL
	CALL	TESTSENDTXW
	
	MOVFW	AVDIVH	;send AVDIV
	CALL	TESTSENDTXW	
	MOVFW	AVDIVL
	CALL	TESTSENDTXW

	BTFSS	DRESISTIVE	;### r here
	GOTO	CONVPWMTOSEND

	CLRF	TEMPHI	;send PR2+1 = R Multiplier
	MOVFW	PR2
	MOVWF	TEMPLO
	MOVLW	.1
	ADDWF	TEMPLO
	CLRW
	ADDWFC	TEMPHI
	
	GOTO	SENDPWM_PR2	;### to here

CONVPWMTOSEND
	BANKSEL	CCPR1L
	LSLF	CCPR1L,W	;put 10-bit PWM into TEMPLO/HI
	MOVWF	TEMPLO
	CLRF	TEMPHI
	RLF	TEMPHI
	LSLF	TEMPLO
	RLF	TEMPHI		
	SWAPF	CCP1CON,W
	ANDLW	B'00000011'
	IORWF	TEMPLO
	BANKSEL	0
	
SENDPWM_PR2	MOVFW	TEMPHI	;send last PWM
	CALL	TESTSENDTXW
	MOVFW	TEMPLO
	CALL	TESTSENDTXW

	MOVFW	DIVUSEDH	;send divisor used in calc ###j
	CALL	TESTSENDTXW
	MOVFW	DIVUSEDL
	CALL	TESTSENDTXW

	MOVLW	0FF
	BTFSS	SAVEINDICATOR
	MOVLW	0
	CALL	TESTSENDTXW
	BCF	SAVEINDICATOR
	
	BCF	SENDPARAMS	;one shot only
	
	RETURN

GETCOMPPROBE	BCF	COMMSBEENON	;clear flag
	CALL	GETP2	
	CALL	GETP1
	CALL	GETP0
	RETURN

	IF (SETFORTEST==1)

GETFALSEDATA	MOVLW	HIGH .2320
	MOVWF	TMR1UP0
	MOVLW	LOW .2320	;negative value
	MOVWF	TMR1HP0
	
INJECT1	BTFSC	TEST,0
	GOTO	INJECT1B
	BTFSC	TEST,1
	GOTO	INJECT1C

INJECT1A	MOVLW	HIGH .28354
	MOVWF	TMR1UP1
	MOVLW	LOW .28354	;30568
	MOVWF	TMR1HP1
	GOTO	INJECT2

INJECT1B	MOVLW	HIGH .34354
	MOVWF	TMR1UP1
	MOVLW	LOW .34354	;30568
	MOVWF	TMR1HP1
	GOTO	INJECT2
	
INJECT1C	MOVLW	HIGH .3354
	MOVWF	TMR1UP1
	MOVLW	LOW .3354	;30568
	MOVWF	TMR1HP1

INJECT2	BTFSC	TEST,4
	GOTO	INJECT2B
	BTFSC	TEST,5
	GOTO	INJECT2C

INJECT2A	MOVLW	HIGH .29477
	MOVWF	TMR1UP2
	MOVLW	LOW .29477	;31561
	MOVWF	TMR1HP2
	RETURN

INJECT2B	MOVLW	HIGH .35477
	MOVWF	TMR1UP2
	MOVLW	LOW .35477	;31561
	MOVWF	TMR1HP2
	RETURN
	
INJECT2C	MOVLW	HIGH .3477
	MOVWF	TMR1UP2
	MOVLW	LOW .3477	;31561
	MOVWF	TMR1HP2
	RETURN

	ENDIF

TWOPROBECALC	
	IF (SETFORTEST==1)
	CALL	GETFALSEDATA
	ENDIF

ONEPDRIFTCALC	MOVFW	TMR1UP0	;entry point for one probe calcs
	MOVWF	P0DFH
	MOVFW	TMR1HP0
	MOVWF	P0DFL	;transfer current P0
	
	BCF	DRIFTNEG	;clear flag
	MOVFW	P0EMPTYL
	SUBWF	P0DFL
	MOVFW	P0EMPTYH
	SUBWFB	P0DFH	;P0-P0_empty
	
	SKPNC		;no carry indicates negative
	GOTO	P1DFCALC
	
P0DFNEGATIVE	BSF	DRIFTNEG
	COMF	P0DFL
	COMF	P0DFH	;complement
	MOVLW	.1
	ADDWF	P0DFL
	CLRW
	ADDWFC	P0DFH	;& add 1
	
P1DFCALC	MOVFW	P1DFL	;constant from EEPROM
	MOVWF	M1L
	MOVFW	P1DFH
	MOVWF	M1H
	
	MOVFW	P0DFL
	MOVWF	M2L
	MOVFW	P0DFH
	MOVWF	M2H
	
	CALL	MULT16X16	;Result - P1df * 2^8 IN AARGB1(H) & AARGB2(L)

;###q	BTFSS	TWOPROBE
;###q	RETURN		;exit if one probe
	
P1CALC	MOVFW	P1EMPTYL	;Current probe - empty = CALCPx
	SUBWF	TMR1HP1,W
	MOVWF	CALCP1L
	MOVFW	P1EMPTYH
	SUBWFB	TMR1UP1,W
	MOVWF	CALCP1H

	SKPNC
	GOTO	P1CALCa
	CLRF	CALCP1H	;if -VE set to zero
	CLRF	CALCP1L
	GOTO	P1_20PCCALC	;###j exit routine

P1CALCa	BTFSS	DRIFTNEG
	GOTO	P1SUBDRIFT
P1ADDDRIFT	MOVFW	AARGB2
	ADDWF	CALCP1L
	MOVFW	AARGB1
	ADDWFC	CALCP1H
	GOTO	P1_20PCCALC	;###j exit routine

P1SUBDRIFT	MOVFW	AARGB2
	SUBWF	CALCP1L
	MOVFW	AARGB1
	SUBWFB	CALCP1H	

	SKPNC
	GOTO	P1_20PCCALC	;###j
	CLRF	CALCP1H	;if -VE set to zero
	CLRF	CALCP1L

P1_20PCCALC	BCF	OVER20PCCSPAN	;###j here
	MOVFW	P1SPANATCALL	;SPAN@CAL/5 (20%)
	MOVWF	AARGB3
	MOVFW	P1SPANATCALH
	MOVWF	AARGB2
	CLRF	AARGB1
	CLRF	AARGB0
	CLRF	DIVH
	MOVLW	.5
	MOVWF	DIVL	
	CALL	UDIV3216L	;result in AARGB2/3 (H/L)
	MOVFW	AARGB3	;subract 20% span from current span
	SUBWF	CALCP1L,W
	MOVFW	AARGB2
	SUBWFB	CALCP1H,W
	SKPNC
	BSF	OVER20PCCSPAN	;###j to here

P2DFCALC	MOVFW	P2DFL	;constant from EEPROM
	MOVWF	M1L
	MOVFW	P2DFH
	MOVWF	M1H
	
	MOVFW	P0DFL
	MOVWF	M2L
	MOVFW	P0DFH
	MOVWF	M2H
	
	CALL	MULT16X16	;Result - P1df/2^8 IN AARGB1(H) & AARGB2(L)
	
P2CALC	MOVFW	P2EMPTYL	;Current probe - empty = CALCPx
	SUBWF	TMR1HP2,W
	MOVWF	CALCP2L
	MOVFW	P2EMPTYH
	SUBWFB	TMR1UP2,W
	MOVWF	CALCP2H

	SKPNC
	GOTO	P2CALCa
	CLRF	CALCP2H	;if -VE set to zero
	CLRF	CALCP2L
	GOTO	P2DFCALCEND	;exit routine

P2CALCa	BTFSS	DRIFTNEG
	GOTO	P2SUBDRIFT
P2ADDDRIFT	MOVFW	AARGB2
	ADDWF	CALCP2L
	MOVFW	AARGB1
	ADDWFC	CALCP2H
	GOTO	P2DFCALCEND

P2SUBDRIFT	MOVFW	AARGB2
	SUBWF	CALCP2L
	MOVFW	AARGB1
	SUBWFB	CALCP2H	

	SKPNC
	GOTO	P2DFCALCEND
	CLRF	CALCP2H	;if -VE set to zero
	CLRF	CALCP2L

P2DFCALCEND	BTFSS	TWOPROBE	;###q
	GOTO	SETCURRDIV	;###q
P2DFCANCENDa	MOVFW	CALCP2H
	IORWF	CALCP2L,W
	BNZ	CALC_CURRDIV

P2ZERO	MOVLW	caldiv12L_EA	;if P2 zero, use div@cal           ;;; IMPORTANT POINT
	CALL	EEREADADRW
	MOVWF	CURRDIVL
	CALL	EEREADNEXT
	MOVWF	CURRDIVH
	GOTO	DIVAVERAGE

CALC_CURRDIV	MOVFW	CALCP1L
	MOVWF	M1L
	MOVFW	CALCP1H
	MOVWF	M1H	;CALCP1 to M1		

	MOVFW	M12L
	MOVWF	M2L
	MOVFW	M12H
	MOVWF	M2H	;M12 in M2	
	
	CALL	MULT16X16	;P1 CALC SPAN x M12 result in AARG

	MOVFW	CALCP2L
	MOVWF	DIVL
	MOVFW	CALCP2H
	MOVWF	DIVH	;P2 CALC SPAN to DIV
	
	CALL	UDIV3216L	;((CALC P1 SPAN x M12)/CALC P2 SPAN) - result in lower 2 bytes of AARG	

	MOVLW	LOW const3	;subtract const3 (7.43 x 2^12)= current div
	SUBWF	AARGB3
	MOVLW	HIGH const3
	SUBWFB	AARGB2
;	Change done 28/10/2021
;
	SKPNC ; jump over next instruction if resulting current divisor is negative
	GOTO SETCURRDIV ; if current divisor is positive, do not use div@cal
SELDIVCAL
	MOVLW caldiv12L_EA ;load div@cal in AARGB3/2. This value is moved to CURDIVH/L later in the code
	CALL EEREADADRW
	MOVWF AARGB3
	CALL EEREADNEXT
	MOVWF AARGB2
	CALL RESETSAVETIME ; since current divisor = div@cal, dont save cur divisor in eeprom. Restart timer used for saving data 
;;;	
SETCURRDIV	MOVLW	onepdivL	;###q
	BTFSC	TWOPROBE	;###q
	MOVFW	AARGB3
	MOVWF	CURRDIVL

	MOVLW	onepdivH	;###q
	BTFSC	TWOPROBE	;###q
	MOVFW	AARGB2
	MOVWF	CURRDIVH	;move result to current div x 2^12	

DIVAVERAGE	BTFSS	OVER20PCCSPAN	;###h altered routine
	CALL	RESETSAVETIME	;set full delay
	BTFSC	OVER20PCCSPAN	;###n
	CALL	DOAVERAGE
			
CALCNEWP1	MOVFW	CALCP1L
	MOVWF	M1L
	MOVFW	CALCP1H
	MOVWF	M1H
	
	MOVLW	LOW .4096
	MOVWF	M2L
	MOVLW	HIGH .4096
	MOVWF	M2H
	
	CALL	MULT16X16	;P1 x 2^12

DIVATCAL97	MOVFW	DIVATCALL	;transfer ###t here
	MOVWF	DIVATCAL97L
	MOVFW	DIVATCALH
	MOVWF	DIVATCAL97H

	RRF	DIVATCAL97H
	RRF	DIVATCAL97L
	RRF	DIVATCAL97H
	RRF	DIVATCAL97L
	RRF	DIVATCAL97H
	RRF	DIVATCAL97L
	RRF	DIVATCAL97H
	RRF	DIVATCAL97L
	RRF	DIVATCAL97H
	RRF	DIVATCAL97L	;divide by 32	
	MOVLW	B'00000111'
	ANDWF	DIVATCAL97H	;remove any wrapped round bits
	
	MOVFW	DIVATCAL97L	;subract from DIVATCAL. Result in DIVATCAL97
	SUBWF	DIVATCALL,W
	MOVWF	DIVATCAL97L
	MOVFW	DIVATCAL97H	
	SUBWFB	DIVATCALH,W
	MOVWF	DIVATCAL97H	;###t to here


TESTFORLOWDIV	BCF	CDIVISLOW
	MOVFW	DIVATCAL97L		;###t
	SUBWF	AVDIVL_OVER20,W
	MOVFW	DIVATCAL97H		;###t
	SUBWFB	AVDIVH_OVER20,W
	SKPC
	BSF	CDIVISLOW		

	MOVFW	AVDIVL_OVER20	;###h alteration ###j altered from just AVDIVL
	BTFSC	CDIVISLOW	;substitute if current div low
	MOVFW	DIVATCAL97L	;###t

	BTFSS	TWOPROBE	;###q
	MOVLW	onepdivL	;###q

	MOVWF	DIVUSEDL
	MOVWF	DIVL

	MOVFW	AVDIVH_OVER20	;###j altered from just AVDIVH	
	BTFSC	CDIVISLOW	;substitute if current div low
	MOVFW	DIVATCAL97H	;###t

	BTFSS	TWOPROBE	;###q
	MOVLW	onepdivH	;###q

	MOVWF	DIVUSEDH
	MOVWF	DIVH
	
	CALL	UDIV3216L	;divide by av12
	
	MOVFW	AARGB3
	MOVWF	PROBE1L		
	MOVFW	AARGB2
	MOVWF	PROBE1H
		
	RETURN

TIMESHIFT	MOVLW	tmr1shift	;shift selectively into upper bytes	
	MOVWF	LOOPCOUNT
	TSTF	LOOPCOUNT
	SKPNZ
	RETURN
	
TSHIFTLOOP	CLRC
	RLF	TMR1L
	RLF	TMR1H
	RLF	TMR1U
	DECFSZ	LOOPCOUNT
	GOTO	TSHIFTLOOP	
	RETURN

ADDTOTIMEp	CLRF	FSR0H
	MOVLW	TIMEL
	MOVWF	FSR0L
	MOVFW	TMR1L
	ADDWF	INDF0
	INCF	FSR0L
	MOVFW	TMR1H
	ADDWFC	INDF0
	INCF	FSR0L
	MOVFW	TMR1U
	ADDWFC	INDF0
	INCF	FSR0L
	CLRW
	ADDWFC	INDF0
	TSTF	INDF0
	SKPNZ
	BSF	SAVEFLAG
	BTFSS	SAVEFLAG	;may have been set elsewhere
	RETURN
	CALL	STOREAVDIV
	BCF	SAVEFLAG
	BSF	SAVEINDICATOR	;for diagnostics
RESETSAVETIME	
	BANKSEL	TIMEUU							;;; TIMEUU	EQU	0A3
	MOVLW	.1								;;; w = 1
	MOVWF	TIMEUU							;;; location 0A3 =  1
	BANKSEL	0								;;; select bank 0
	RETURN

ADDTOTIMEt	CLRF	FSR0H
	MOVLW	TIMEU
	MOVWF	FSR0L
	MOVLW	.1	;add in 65536
	ADDWF	INDF0
	INCF	FSR0L
	CLRW
	ADDWFC	INDF0
	TSTF	INDF0
	SKPNZ
	BSF	SAVEFLAG
	RETURN
	
;cold start *****************************************************************

CSTART    	
	BANKSEL	OSCCON						;;; oscillator controll register, bank selected
	MOVLW	OSCCONset					;;;   OSCCONset = B'01110000' 
	MOVWF	OSCCON	;set clock			;;; 8 or 32 MHz set
	BANKSEL	PORTA						;;; PORTA bank selected 
	
	CLRF	PORTA						;;; PORTA cleared

	IFDEF __16F1823
	CLRF	PORTC						;;; PORTC cleared
	ENDIF
	
	IFDEF __16LF1823
	CLRF	PORTC
	ENDIF

	CALL	SETUP						;;; initialize registers
	CLRF	PORTA						;;; PORTA cleared

	IFDEF __16F1823
	CLRF	PORTC
	ENDIF
	
	IFDEF __16LF1823
	CLRF	PORTC
	ENDIF

	MOVLW     	20          		;clear registers
	MOVWF     	FSR0L
	CLRF	FSR0H
INITLOOP 	CLRW
	MOVWI	FSR0++

	MOVLW     	B'01111111'         	;;; To cheked if reach 0x7FH		;if 0 or 80 reached (80 no. of register)
	ANDWF     	FSR0L,W
	SKPNZ
	BSF	FSR0L,5		;add 20
	MOVLW	B'11110000'
	ANDWF	FSR0H,W		;check for >bank 15
	SKPNZ
	GOTO	INITLOOP
	
	CALL	SETUP
	INCF	FLASHER		;set timer to 1
	BSF	LCALWAIT							;;; flag set 
	CALL	INITMR1							;;; init timer 1, timer 1 ON
	MOVLW	INTCONset						;;; B'11000000'
	MOVWF	INTCON							;;; INTERRUPT CONTROL REGISTER. GIE = 1 Enables all active interrupts
											;;; PEIE = 1; Enables all active peripheral interrupts
	CALL	RESETSAVETIME					;;; General purpose register location 0x0A3 =  1,  select bank 0

;	MOVLW	calstatus_EA
;	MOVWF	EEWRADDRESS	
;	MOVLW	emptydone
;	CALL	EEWRITEW
;	CALL	EEREADSAME
;	MOVWF	CALSTATUS

; 	MOVLW	OPspanH_EA	;get op parameters ###j here (REMMING)
;	CALL	EEREADADRW
;	MOVWF	OPSPANHI

;	BCF	OPREVERSE	;extract reverse flag
;	BTFSC	OPSPANHI,7
;	BSF	OPREVERSE
	
;	MOVLW	B'00000011'
;	ANDWF	OPSPANHI	;remove flag
	
;	CALL	EEREADNEXT
;	MOVWF	OPSPANLO
;	CALL	EEREADNEXT
;	MOVWF	OPOFFSETHI
;	CALL	EEREADNEXT
;	MOVWF	OPOFFSETLO	;###j to here
	
	CALL	AVPRIME									;;; n, currdiv12 RLF 2 timef if n = 2
	
	CALL	GETP0
	CALL	GETCOMPPROBE



;main ***********************************************************************


IDLE	BTFSS	INTDONE
	GOTO	IDLE
	CLRWDT
	BCF	INTDONE 
 
 	MOVLW	type_EA	;get op parameters
	CALL	EEREADADRW
	MOVWF	TYPE
	
 	MOVLW	advanced_EA	;get advanced_EA parameters ###6 HERE
	CALL	EEREADADRW
	MOVWF	ADVANCED
	CALL	COUNTTABLE
	MOVWF	OPTCOUNT	;###6 TO HERE

	CALL	T2SETUP

	BCF	DRESISTIVE	;###3 here
	MOVFW	TYPE	;resistive?
	ANDLW	B'01111000'
	SKPZ
	BSF	DRESISTIVE	;yes ###3 to here

	MOVLW	calstatus_EA
	CALL	EEREADADRW
	MOVWF	CALSTATUS


LCWAITTEST	BTFSS	LCALWAIT
	GOTO	TESTFORHCAL

TESTFORLCAL	MOVFW	PORTC		;###USPVOP
	ANDLW	B'00000010'		;###USBVOP
	
	BNZ	TESTFORHCAL		;if no press, goto HCAL test
	TSTF	FLASHER
	BNZ	SETUNCALOP		;if switch pressed & LCAL not done, output uncalibrated level
	CALL	GETLCAL

	MOVLW	LOW minpwm		;set empty level
	MOVWF	PWML
	MOVLW	HIGH minpwm
	MOVWF	PWMH
	CALL	PWMSET
	BSF	LCJUSTDONE		;avoids HCAL starting from LCAL
	
		
TESTFORHCAL	BCF	LCALWAIT	
	BTFSS	EMPTYCAL
	GOTO	SETUNCALOP		;if LCAL not done
	
	BTFSC	SPANCAL
	GOTO	RPTHCALTEST

	MOVFW	PORTC		;###USPVOP
	ANDLW	B'00000010'		;###USBVOP

	BNZ	CLRJUSTDONE		;if no press, clear JUSTDONE
	BTFSC	LCJUSTDONE
	GOTO	IDLE		;goto idle if key not released
	INCF	SWTIMER
	BTFSC	SWTIMER,7
	CALL	GETSPANCAL
	GOTO	SETMIN		;maintain minimum

CLRJUSTDONE	BCF	LCJUSTDONE
NOSPANCAL	CLRF	SWTIMER
	GOTO	IDLE

RPTHCALTEST	MOVFW	PORTC		;###USPVOP
	ANDLW	B'00000010'		;###USBVOP

	BNZ	MAINCLRTMR		;if no press, goto MAIN		
	
	INCF	SWTIMER
	BTFSS	SWTIMER,7
	GOTO	SETUNCALOP		;revert to uncalibrated value whilst switch is pressed
	BTFSS	HCJUSTDONE
	CALL	GETSPANCAL
	BSF	HCJUSTDONE
	GOTO	MAIN
	
MAINCLRTMR	CLRF	SWTIMER		;clear if no press #
	BCF	HCJUSTDONE
MAIN	BCF	LCALWAIT
	CALL	INTERVALTABLE		;use programmed value	###6
	ANDWF	FLASHER,W		;###6
	SKPZ
	GOTO	IDLE
	CALL	GETCOMPPROBE
	BTFSC	COMMSBEENON
	GOTO	IDLE	;no update if comms interrupt
			
	CALL	MT_DF_M_N_DVC

;###q	BTFSS	TWOPROBE	;###8 HERE
;###q	GOTO	MAINa

	CALL	TWOPROBECALC	

;###q here
;	GOTO	ZEROTEST	;###8 TO HERE

;MAINa	CALL	ONEPDRIFTCALC ;drift factor in AARGB1(H) & AARGB2(L)	

;	MOVFW	P1EMPTYL	;Current probe - empty = PROBE1x
;	SUBWF	TMR1HP1,W
;	MOVWF	PROBE1L
;	MOVFW	P1EMPTYH
;	SUBWFB	TMR1UP1,W
;	MOVWF	PROBE1H
;	SKPNC
;	GOTO	ONEP1CALCa
;	GOTO	ZEROANDEXIT	;if -VE set to zero & #EXIT

;ONEP1CALCa	BTFSS	DRIFTNEG
;	GOTO	P1SUBDRIFT
;P1PLUSDRIFT	MOVFW	AARGB2
;	ADDWF	PROBE1L
;	MOVFW	AARGB1
;	ADDWFC	PROBE1H
;	GOTO	ZEROTEST

;P1MINUSDRIFT	MOVFW	AARGB2
;	SUBWF	PROBE1L
;	MOVFW	AARGB1
;	SUBWFB	PROBE1H	
;	SKPNC
;	GOTO	ZEROTEST

;ZEROANDEXIT	CLRF	PROBE1H	;if -VE set to zero
;	CLRF	PROBE1L		
;###q to here

ZEROTEST	MOVFW	PROBE1L	
	IORWF	PROBE1H,W
	SKPZ
	GOTO	SPANCALC

SETSPANZERO	CLRF	AARGB3
	CLRF	AARGB2
	CALL	GETOPPARAM
	GOTO	REVCHECK

SPANCALC	CALL	GETOPPARAM
	CALL	CALCMAIN	;###r was CALCULATE
	CALL	SPANCHECK

REVCHECK	BTFSC	OPREVERSE
	CALL	REVERSEPWM

ADDLOWOFFSET	MOVFW	OPOFFSETLO
	ADDWF	AARGB3
	MOVFW	OPOFFSETHI
	ADDWFC	AARGB2

	BTFSC	DRESISTIVE
	GOTO	SETDRESPWM

	MOVFW	AARGB2
	MOVWF	PWMH
	MOVFW	AARGB3
	MOVWF	PWML

;GETEEMAX	CLRF	TEMPHI
;	MOVFW	OPSPANLO
;	MOVWF	TEMPLO
;	MOVFW	OPOFFSETLO
;	ADDWF	TEMPLO
;	SKPNC
;	INCF	TEMPHI

;	MOVFW	OPSPANHI
;	ADDWF	TEMPHI
;	MOVFW	OPOFFSETHI
;	ADDWF	TEMPHI

;MAXTEST	MOVFW	TEMPLO		;VAR2-VAR1
;	SUBWF	PWML,W		;Note that Z flag
;	MOVFW	TEMPHI		;is invalid after
;	SKPC			;the subtraction.
;	INCFSZ	TEMPHI,W	
;	SUBWF	PWMH,W
;	SKPC
;	GOTO	ABSMAXTEST

;SETEEMAX	MOVFW	TEMPLO	;set maximum
;	MOVWF	PWML
;	MOVFW	TEMPHI
;	MOVWF	PWMH
	
ABSMAXTEST	MOVFW	PWMH	;check for over 1023
	ANDLW	B'11111100'
	BZ	SETPWM
SETABSMAX	MOVLW	0FF	;set maximum
	MOVWF	PWML
	MOVLW	03
	MOVWF	PWMH
	CALL	PWMSET
	MOVLW	0FE
	MOVWF	PR2
	CALL	CALCPERCENT	;###4
	CALL	SENDDIAGDATA
	GOTO      	IDLE

SETDRESPWM	TSTF	AARGB2	;over 255?
	SKPZ
	GOTO	SETRESMAX	;yes
	
	MOVFW	AARGB3
	ANDLW	B'11111110'	;check for 1 or 0
	SKPNZ				;###r was SKPNZ
	GOTO	SETRESMIN	;for 1/0

;now do the calculation PWM=int(255/resistance multiplier) where resistance multiplier is
;in ACCb(LO/HI) and needs to be stored for later use in ACCd(LO/HI)

;###3	MOVFW	ACCbLO
;###3	MOVWF	ACCdLO
;###3	MOVWF	ACCaLO	;for resistive div
;###3	MOVFW	ACCbHI
;###3	MOVWF	ACCdHI	;store multiplier for later
	
;###3	CALL	RESISTIVEDIV	;get PWM for CCPR1L	

;calculate PR2 = (PWM * CCPR1) -1

;###3	CALL	RESISTIVEMULT	;(PWM * CCPR1)
;###3	DECF	ACCaLO	;-1
	

;###3	MOVFW	ACCbLO
;###3	BANKSEL	CCPR1L
;###3	MOVWF	CCPR1L
;###3	BCF	CCP1CON,4	;PWM (1-255) in ACCbLO
;###3	BCF	CCP1CON,5
;###3	BANKSEL	0
;###3	MOVFW	ACCaLO
;###3	BANKSEL	PR2
;###3	MOVWF	PR2
;###3	BANKSEL	0

	MOVLW	.1	;###3 HERE
	BANKSEL	CCPR1L
	MOVWF	CCPR1L
	BCF	CCP1CON,4	;PWM (1-255) in AARGB3
	BCF	CCP1CON,5
	BANKSEL	0
	DECF	AARGB3,W
	MOVWF	PR2	;###3 TO HERE
	CALL	CALCPERCENT	;###4
	CALL	SENDDIAGDATA
	GOTO	IDLE

SETRESMIN	MOVLW	.100
	BANKSEL	CCPR1L
	MOVWF	CCPR1L
	BCF	CCP1CON,4	;PWM (1-255) in ACCbLO
	BCF	CCP1CON,5
	BANKSEL	0
	MOVLW	.99
	MOVWF	PR2
;	BANKSEL	0
	CALL	CALCPERCENT	;###4
	CALL	SENDDIAGDATA
	GOTO	IDLE

SETRESMAX	MOVLW	.1
	BANKSEL	CCPR1L
	MOVWF	CCPR1L
	BCF	CCP1CON,4	;PWM (1-255) in ACCbLO
	BCF	CCP1CON,5
	BANKSEL	0
	MOVLW	.255	;was 254 ###3
	MOVWF	PR2
;	BANKSEL	0
	CALL	CALCPERCENT	;###4
	CALL	SENDDIAGDATA
	GOTO	IDLE
	


SETPWM	CALL	PWMSET	;normal range
	BANKSEL	0
	MOVLW	0FF
	MOVWF	PR2
;	BANKSEL	0
	CALL	CALCPERCENT	;###4
	CALL	SENDDIAGDATA
	GOTO      	IDLE    

SETMIN	BTFSC	DRESISTIVE
	GOTO	SETRESMIN	;different minimum if resistive
	MOVFW	OPOFFSETLO	;set minimum
	MOVWF	PWML
	MOVFW	OPOFFSETHI
	MOVWF	PWMH
	GOTO	SETPWM

SETUNCALOP	MOVLW	LOW uncalpwm	;set for test
	MOVWF	PWML
	MOVLW	HIGH uncalpwm
	MOVWF	PWMH
	GOTO	SETPWM

		
GETRVOLTAGE			;voltage for resistive op
	BANKSEL	ADCON0
	MOVLW	ADCON0setRV
	MOVWF	ADCON0
ADCMAIN	MOVLW	ADwait	;delay in uSec (20MHz)
	MOVWF	LOOPCOUNT
DELAYLOOP	GOTO	($+1)
	DECFSZ	LOOPCOUNT
	GOTO	DELAYLOOP
	BSF	ADCON0,ADGO	;start conversion
	SLEEP		;workaround for silicon Iss.6
	NOP
	BCF	PIE1,ADIE	;clear interrupt

;	BTFSC	ADCON0,ADGO
;	GOTO	($-1)
	MOVFW	ADRESL
	MOVWF	TEMPLO
	MOVFW	ADRESH
	MOVWF	TEMPHI
	BANKSEL	0
	RETURN

;init eeprom ****************************************************************

; The 12F1822 & 16F1823 have 256 bytes of non-volatile EEPROM, starting at address 0xF000
 

DATAEE    	ORG  0xF000


simdiv	equ	.4301 	;1.05 x 4096

;	DE	0,03,0EC,0,00,00,37,06,LOW simdiv,HIGH simdiv	;simulates programmed (LO-HI)for P1 on emulator 8K

;	DE	0,03,92,0B,0A6,0A,37,06,LOW simdiv,HIGH simdiv	;simulates programmed (LO-HI)for P2 on emulator 8K

;	DE	0,3,.1,1,.2,1,.3,1,.4,1,.5,1,LOW simdiv,HIGH simdiv,.7,1

	IF (SETFORTEST==1)

	DE	0,03,75,69,0A,6E,14,09		;EMPTY CAL
	DE	68,77,49,7B,4E,10,071,0E,5F,80,4E,10	;SPANCAL

	ELSE

	DE	0,0,0,0,0,0,0,0,0,0
	DE	0,0,0,0,0,0,0,0,0,0
	
	ENDIF

	DE	0,0,0,0,0,0,0,0,0,0

	ORG	0xF000 + .32

	DE	0,0,0,0,0,0,0,0,0,0
	DE	0,0,0,0,0,0,0,0
	
;	DE	0,0,0			;DATE
;	DE	.1			;TYPE - V=1, I=2, RA=4, RD1=8, RD2=16, RD3=32, RD4=64
;	DE	(HIGH .1000), LOW .1000		;SPAN (add 128 to high for reverse)	
;	DE	HIGH .0, LOW .0		;OFFSET MIN - 0V, 4mA

;	DE	(.128 + .75)			;ALARM LEVEL H (ON ABOVE 75%)
;	DE	.25			;ALARM LEVEL L (25%)	
;	DE	B'10010111'			;ADVANCED DATA BITS: 7(2probe=1), 5:4(samp.interval), 0:2(count)

	ORG	0xF000 + .50

	DE	LOW defp1df, HIGH defp1df
	DE	LOW defp2df, HIGH defp2df
	DE	defaultn

	END