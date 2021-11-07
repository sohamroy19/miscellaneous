        area factorial,code,readonly
        entry
main
        ldr r0, num
        mov r1, #1
fact
        mul r1, r0, r1
        subs r0, r0, #1
        bne fact
        
        swi &11

num     dcd 4 ; 4! = 24 = 0x18
        end