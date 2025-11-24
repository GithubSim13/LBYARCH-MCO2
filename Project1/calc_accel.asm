section .data
    k dd 3.6                ; constant for converting kmh to m/s

section .text
    bits 64
    default rel
    global calcAccel

calcAccel:
    test rcx, rcx
    jz done

    movss xmm3, [k]

calcLoop:
    movss xmm0, [rdx]       ; initial v
    movss xmm1, [rdx + 4]   ; final v
    movss xmm2, [rdx + 8]   ; time

    subss xmm1, xmm0
    divss xmm1, xmm3
    divss xmm1, xmm2

    cvtss2si eax, xmm1
    mov [r8], eax

    add rdx, 12
    add r8, 4
    dec rcx
    jnz calcLoop

done:
    ret