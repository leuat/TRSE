; Copy BC bytes from HL to DE.
z80_copy_mem:
    ld      a,b
    or      c
    ret     z

    ld      a,16
    sub     c
    and     15
    add     a,a
    ld      (z80_copy_lp-1),a
    jr      z80_copy_lp ; will be overwritten
z80_copy_lp:
    ldi
    ldi
    ldi
    ldi

    ldi
    ldi
    ldi
    ldi

    ldi
    ldi
    ldi
    ldi

    ldi
    ldi
    ldi
    ldi

    jp      pe,z80_copy_lp

    ret
