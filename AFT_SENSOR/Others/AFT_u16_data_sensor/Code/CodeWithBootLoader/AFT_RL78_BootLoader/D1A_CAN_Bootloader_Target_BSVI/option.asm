OPT CSEG OPT_BYTE
DB 0EFH ; Does not use interval interrupt of watchdog timer,
	; Enables watchdog timer operation,
	; Window open period of watchdog timer is 50%,
	; Overflow time of watchdog timer is 29/fIL,
	; Stops watchdog timer operation during HALT/STOP mode
DB 0FFH ; Select 2.75 V for VLVIL
	; Select rising edge 2.92 V, falling edge 2.86 V for VLVIH
	; Select the interrupt & reset mode as the LVD operation mode
	;stops clock monitoring
DB 0E8H ; Select the HS (high speed main) mode as the flash operation mode
	;and 32 MHz as the frequency of the high-speed on-chip oscillator
DB 084H ; Enables on-chip debug operation, does not erase flash memory
	;data when security ID authorization fails
	
	end