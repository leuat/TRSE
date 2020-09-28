; Copy BC bytes from HL to DE.
z80_copy_mem:
    ld      a,b
    or      c
    ret     z

    ld      a,16
    sub     c
    and     15
    add     a,a
    ld      (z80_lp_entry),a
    jr      $
z80_lp_entry    equ $-1
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
