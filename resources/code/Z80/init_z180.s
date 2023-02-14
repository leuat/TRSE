
mul_8x8:

    sla h  ; optimised 1st iteration
    jr nc,mul_8x8_s1
    ld l,e
mul_8x8_s1:

    add hl,hl
    jr nc,mul_8x8_s2
    add hl,de
mul_8x8_s2:

    add hl,hl
    jr nc,mul_8x8_s3
    add hl,de
mul_8x8_s3:

    add hl,hl
    jr nc,mul_8x8_s4
    add hl,de
mul_8x8_s4:

    add hl,hl
    jr nc,mul_8x8_s5
    add hl,de
mul_8x8_s5:

    add hl,hl
    jr nc,mul_8x8_s6
    add hl,de
mul_8x8_s6:

    add hl,hl
    jr nc,mul_8x8_s7
    add hl,de
mul_8x8_s7:

    add hl,hl
    jr nc,mul_8x8_s8
    add hl,de
mul_8x8_s8:

    ret



mul_16x8:
    add a,a  ; optimised 1st iteration
    jr nc,mul_16x8_s1
    ld h,d
    ld l,e
mul_16x8_s1:

    add hl,hl
    rla
    jr nc,mul_16x8_s2
    add hl,de
    adc a,c
mul_16x8_s2:


    add hl,hl
    rla
    jr nc,mul_16x8_s3
    add hl,de
    adc a,c
mul_16x8_s3:

    add hl,hl
    rla
    jr nc,mul_16x8_s4
    add hl,de
    adc a,c
mul_16x8_s4:

    add hl,hl
    rla
    jr nc,mul_16x8_s5
    add hl,de
    adc a,c
mul_16x8_s5:

    add hl,hl
    rla
    jr nc,mul_16x8_s6
    add hl,de
    adc a,c
mul_16x8_s6:

    add hl,hl
    rla
    jr nc,mul_16x8_s7
    add hl,de
    adc a,c
mul_16x8_s7:

    add hl,hl
    rla
    jr nc,mul_16x8_s8
    add hl,de
    adc a,c
mul_16x8_s8:
    ret


div_16x8
;div_hl_c:
   xor	a
   ld	b, 16
_loop:
   add	hl, hl
   rla
   jr	c, $+5
   cp	c
   jr	c, $+4

   sub	c
   inc	l

   djnz	_loop

   ret


div_16x16:
   ld	hl, 0
   ld	b, 16

_loop16:

   ; oh so slow! sloooow
   push af
   ld a,c
   sla a
   or 1
   ld c,a
   pop af

   rla
   adc	hl, hl
   sbc	hl, de
   jr	nc, $+4
   add	hl, de
   dec	c

   djnz	_loop16

   ret


_Mod_16:
    ld hl,0
    ld a,b
    ld b,8
_Mod_Loop1:
    rla
    adc hl,hl
    sbc hl,de
    jr nc,_Mod_NoAdd1
    add hl,de
_Mod_NoAdd1:
    djnz _Mod_Loop1
    rla
    cpl
    ld b,a
    ld a,c
    ld c,b
    ld b,8
_Mod_Loop2:
    rla
    adc hl,hl
    sbc hl,de
    jr nc,_Mod_NoAdd2
    add hl,de
_Mod_NoAdd2:
    djnz _Mod_Loop2
    rla
    cpl
    ld b,c
    ld c,a
    ret
