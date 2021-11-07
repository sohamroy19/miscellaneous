        area odd,code,readonly
        entry
main
        ldr r0, num
        ands r1, r0, #1
        swi &11

num     dcd &432F
        end