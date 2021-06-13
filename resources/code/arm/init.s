.global _start
.align 4
_start:
    b block1

.macro push Rn:req
    str     \Rn, [sp, #-16]
.endm

.macro pop Rn:req
    ldr     \Rn, [sp], #16
.endm

