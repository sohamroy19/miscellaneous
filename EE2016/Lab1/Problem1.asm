        .CSEG
        LDI ZL, LOW(NUM<<1)
        LDI ZH, HIGH(NUM<<1)
        LDI R16, $00
        LPM R0, Z+
        LPM R1, Z
        ADD R0, R1
        BRCC abc
        LDI R16, $01
abc:    NOP
NUM:    .db $D3, $5F
