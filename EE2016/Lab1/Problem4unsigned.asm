		.CSEG
		LDI ZL, LOW(NUMS<<1)
		LDI ZH, HIGH(NUMS<<1)
		LDI R16, $06	; numbers of NUMs
		LPM R0, Z+
		DEC R16
start:	LPM R1, Z+
		CP R0, R1		; C set if R1 is larger
		BRCC check		; go to decrement if R0 larger
		MOV R0, R1		; R0 holds larger
check:	DEC R16
		BRNE start		; if counter is not 0, loop
		STS $0060, R0	; store in SRAM
		NOP
NUMS:	.db $D3, $5F, $69, $F9, $23, $45