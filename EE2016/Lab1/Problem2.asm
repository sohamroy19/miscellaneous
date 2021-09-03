        .CSEG
        LDI ZL, LOW(NUM1<<1)
        LDI ZH, HIGH(NUM1<<1)
        LDI R16, $00
        LPM R0, Z+
        LPM R1, Z
        LDI ZL, LOW(NUM2<<1)
        LDI ZH, HIGH(NUM2<<1)
        LPM R2, Z+
        LPM R3, Z
        ADD R0, R2
        ADC R1, R3
        BRCC abc
        LDI R16, $01
abc:    NOP
NUM1:    .db $12, $34
NUM2:    .db $56, $78