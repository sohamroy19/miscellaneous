		.CSEG
		LDI ZL, LOW(NUM<<1)
		LDI ZH, HIGH(NUM<<1)
		LPM R2, Z+
		LPM R3, Z
		MUL R2, R3
abc:	NOP
NUM:	.db $D3, $5F