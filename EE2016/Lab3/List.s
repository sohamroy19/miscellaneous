        area list_concat,code,readonly
        entry
main
        ldr r0, =list
        mov r1, #4
        mov r2, #0
loop
        ldrb r3, [r0], #1
        and r3, r3, #&F
        lsl r2, #4
        orr r2, r2, r3
        subs r1, r1, #1
        bne loop
        
        ldr r3, result
        str r2, [r3]
        swi &11

list    dcb &01
        dcb &02
        dcb &03
        dcb &04
result  dcd &40000000
        end