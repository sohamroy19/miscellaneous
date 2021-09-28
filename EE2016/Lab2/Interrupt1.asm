.org $0000
rjmp reset

.org $0002
rjmp int1_ISR

.org $0100

reset:
        ;Loading stack pointer address
        LDI R16, $70
        OUT SPL, R16
        LDI R16, $00
        OUT SPH, R16
        
        ;Interface port B pin0 to be output
        ;so to view LED blinking
        LDI R16, $01
        OUT DDRB, R16

        LDI R16, $00
        OUT DDRD, R16

        ;Set MCUCR register to enable low level interrupt
        OUT MCUCR, R16

        ;Set GICR register to enable interrupt 1
        LDI R16, $80
        OUT GICR, R16

        LDI R16, $00
        OUT PORTB, R16

        SEI

ind_loop:   rjmp ind_loop

int1_ISR:   IN R16, SREG    ; push status register to stack
            PUSH R16

            LDI R16, $0A
            MOV R0, R16

            ;Modify below loops to make LED blink for 1 sec
      c1:   LDI R16, $01
            OUT PORTB, R16

            LDI R16, $63    ; =99
      a1:   LDI R17, $63    ; =99
      a2:   LDI R18, $21    ; =33
      a3:   DEC R18
            BRNE a3         ; 3*33 - 1 = 98
            DEC R17
            BRNE a2         ; 102*99 - 1 = 10097
            DEC R16
            BRNE a1         ; 10101*99 - 1 = 999998
            ;Considering contributions by LDI and OUT operations,
            ;exactly 1000000 cycles are consumed between the 2 OUT commands = 1 sec

            LDI R16, $00
            OUT PORTB, R16

            LDI R16, $63    ; =99
      b1:   LDI R17, $63    ; =99
      b2:   LDI R18, $21    ; =33
      b3:   DEC R18
            BRNE b3         ; 3*33 - 1 = 98
            DEC R17
            BRNE b2         ; 102*99 - 1 = 10097
            DEC R16
            BRNE b1         ; 10101*99 - 1 = 999998
            ;Considering contributions by LDI and OUT operations,
            ;exactly 1000000 cycles are consumed between the 2 OUT commands = 1 sec

            DEC R0
            BRNE c1         ; outer loop, 10 times

            POP R16         ; restore status register from stack
            OUT SREG, R16

            RETI