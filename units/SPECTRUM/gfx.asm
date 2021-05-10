	CPU 
	 org $6000
CRAM EQU $5800
FALSE EQU $00
INIT_MUSIC EQU $00
NIL EQU $00
PLAY_MUSIC EQU $00
SCREEN EQU $c000
STOP_MUSIC EQU $00
TRUE EQU $01
VRAM EQU $4000
	jp block1
Screen_ytab:	dw $4000, $4100, $4200, $4300, $4400, $4500, $4600, $4700
	dw $4020, $4120, $4220, $4320, $4420, $4520, $4620, $4720
	dw $4040, $4140, $4240, $4340, $4440, $4540, $4640, $4740
	dw $4060, $4160, $4260, $4360, $4460, $4560, $4660, $4760
	dw $4080, $4180, $4280, $4380, $4480, $4580, $4680, $4780
	dw $40a0, $41a0, $42a0, $43a0, $44a0, $45a0, $46a0, $47a0
	dw $40c0, $41c0, $42c0, $43c0, $44c0, $45c0, $46c0, $47c0
	dw $40e0, $41e0, $42e0, $43e0, $44e0, $45e0, $46e0, $47e0
	dw $4800, $4900, $4a00, $4b00, $4c00, $4d00, $4e00, $4f00
	dw $4820, $4920, $4a20, $4b20, $4c20, $4d20, $4e20, $4f20
	dw $4840, $4940, $4a40, $4b40, $4c40, $4d40, $4e40, $4f40
	dw $4860, $4960, $4a60, $4b60, $4c60, $4d60, $4e60, $4f60
	dw $4880, $4980, $4a80, $4b80, $4c80, $4d80, $4e80, $4f80
	dw $48a0, $49a0, $4aa0, $4ba0, $4ca0, $4da0, $4ea0, $4fa0
	dw $48c0, $49c0, $4ac0, $4bc0, $4cc0, $4dc0, $4ec0, $4fc0
	dw $48e0, $49e0, $4ae0, $4be0, $4ce0, $4de0, $4ee0, $4fe0
	dw $5000, $5100, $5200, $5300, $5400, $5500, $5600, $5700
	dw $5020, $5120, $5220, $5320, $5420, $5520, $5620, $5720
	dw $5040, $5140, $5240, $5340, $5440, $5540, $5640, $5740
	dw $5060, $5160, $5260, $5360, $5460, $5560, $5660, $5760
	dw $5080, $5180, $5280, $5380, $5480, $5580, $5680, $5780
	dw $50a0, $51a0, $52a0, $53a0, $54a0, $55a0, $56a0, $57a0
	dw $50c0, $51c0, $52c0, $53c0, $54c0, $55c0, $56c0, $57c0
	dw $50e0, $51e0, $52e0, $53e0, $54e0, $55e0, $56e0, $57e0
	dw $5800
Screen_ytab_color:	dw $5800, $5820, $5840, $5860, $5880, $58a0, $58c0, $58e0
	dw $5900, $5920, $5940, $5960, $5980, $59a0, $59c0, $59e0
	dw $5a00, $5a20, $5a40, $5a60, $5a80, $5aa0, $5ac0, $5ae0
Screen_tabpow:	db $080, $040, $020, $010, $08, $04, $02, $01
Screen_i:	db	0
Screen_j:	db	0
Screen_x:	db	0
Screen_y:	db	0
Screen_x2:	db	0
Screen_y2:	db	0
Screen_zp: dw  0
Gfx_xc:	db	0
Gfx_yc:	db	0
Gfx_rc:	db	0
	; NodeProcedureDecl -1
	; ***********  Defining procedure : Screen_BorderColor
	;    Procedure type : User-defined procedure
Screen__a:	db	0
 ; Temp vars section
 ; Temp vars section ends
Screen_BorderColor_block2:
Screen_BorderColor:
	; ****** Inline assembler section
		ld bc,#fe
        out(c),a
	
	ret
	; NodeProcedureDecl -1
	; ***********  Defining procedure : Screen_ClearScreen
	;    Procedure type : User-defined procedure
Screen_ClearScreen_block3:
Screen_ClearScreen:
	; Loading param 2 
	ld bc,$1800
	; Loading param 0 
	ld hl,$4000
	; Loading param 1 
	ld a,[Screen_x]
	inc b
	inc c
	jr .Screen_ClearScreen_skip5
.Screen_ClearScreen_fill4:
	ld [hl],a
	inc hl
.Screen_ClearScreen_skip5:
	dec c
	jr nz,.Screen_ClearScreen_fill4
	dec b
	jr nz,.Screen_ClearScreen_fill4
	; Loading param 2 
	ld bc,$300
	; Loading param 0 
	ld hl,$5800
	; Loading param 1 
	ld a,[Screen_y]
	inc b
	inc c
	jr .Screen_ClearScreen_skip7
.Screen_ClearScreen_fill6:
	ld [hl],a
	inc hl
.Screen_ClearScreen_skip7:
	dec c
	jr nz,.Screen_ClearScreen_fill6
	dec b
	jr nz,.Screen_ClearScreen_fill6
	ret
	; NodeProcedureDecl -1
	; ***********  Defining procedure : Screen_SetColor
	;    Procedure type : User-defined procedure
Screen_SetColor_block8:
Screen_SetColor:
	; Generic assign 16-bit pointer
	ld a,[Screen_y]
	and $f8
	rrca
	rrca
	rrca
	ld e,a ; variable is 8-bit
	ld d,0
	ld hl,Screen_ytab_color
	add hl,de
	add hl,de
	ld a,[hl]
	; LoadVar Testing if 'Screen_ytab_color' is word : 1
	ld e,a
	inc hl
	ld a,[hl]
	ld d,a
	ex de,hl
	; Store 16-bit address
	ld [Screen_zp],hl
	; ;general 8-bit expression for the index
	ld d,0
	ld a,[Screen_x]
	and $f8
	rrca
	rrca
	rrca
	ld e,a
	add hl,de
	ld a,[Screen_i]
	ld [hl],a
	ret
	; NodeProcedureDecl -1
	; ***********  Defining procedure : Screen_PutPixel
	;    Procedure type : User-defined procedure
Screen_PutPixel_block9:
Screen_PutPixel:
	; ****** Inline assembler section
	push de
	push hl
	push bc
	push af
	ld      hl,Screen_tabpow
    ld      a,(Screen_x)
    and     7       ; x mod 8
    ld      b,0
    ld      c,a
    add     hl,bc
    ld      a,(hl)
    ld      e,a     ; e contains one bit set
    ld      ix,Screen_ytab
    ld      a,(Screen_y)
    ld      c,a
    add     ix,bc
    add     ix,bc
    ld      a,(ix)      ; table lookup
    ld      l,a
    inc     ix
    ld      a,(ix)      ; table lookup
    ld      h,a
    ld      a,(Screen_x)
    srl     a
    srl     a
    srl     a           ; x/8.
    or      l
    ld      l,a         ; + x/8.
    ld      a,(hl)
    or      e           ; or = superposition mode.
    ld(hl),a      ; set the pixel.	
    pop af
	pop bc
	pop hl
	pop de	
		
	ret
	; NodeProcedureDecl -1
	; ***********  Defining procedure : Screen_ClearDefaultIRQ
	;    Procedure type : User-defined procedure
Screen_pos: dw  0
Screen_intPos: dw  0
Screen_ClearDefaultIRQ_block10:
Screen_ClearDefaultIRQ:
	; ****** Inline assembler section
        ;Initializes IM2, gets rid of the system interrupt.
        di
        ld hl,[Screen_pos]	;HL points to the IM2 Lookup table.
        ld a,h
        ld i,a		;Set I= high(pos).
        im 2		;Switch to vectorized interrupts.
        
	ld a, $0
	ld [Screen_i], a
Screen_ClearDefaultIRQ_forloop11:
	ld a,[Screen_intPos+1]
	push af
	ld hl,[Screen_pos]
	pop af
	ld [hl],a
	ld a,[Screen_intPos]
	push af
	ld hl,[Screen_pos]
	; ; index is pure number optimization
	ld de,$1
	add hl,de
	pop af
	ld [hl],a
	; ;generic pointer/integer P:=P+(expr) add expression
	; RHS is pure 
	ld de,$2
	ld hl,[Screen_pos]
	add  hl,de
	ld [Screen_pos],hl
Screen_ClearDefaultIRQ_forloopcounter13:
Screen_ClearDefaultIRQ_loopstart14:
	ld a,[Screen_i]
	add a,1
	ld [Screen_i],a
	cp $100
	jr nz,Screen_ClearDefaultIRQ_forloop11
Screen_ClearDefaultIRQ_forloopend12:
Screen_ClearDefaultIRQ_loopend15:
	ld hl,[Screen_intPos]
	ld a,$fb
	ld [hl],a
	ld hl,[Screen_intPos]
	; ; index is pure number optimization
	ld de,$1
	add hl,de
	ld a,$c9
	ld [hl],a
	; ****** Inline assembler section
 ei
	ret
	; NodeProcedureDecl -1
	; ***********  Defining procedure : Gfx_Circle
	;    Procedure type : User-defined procedure
Gfx_Circle_block18:
Gfx_Circle:
	; ****** Inline assembler section
    ;;(b,c) stores the current arc point coordinates(x,y)
    ld      a,(Gfx_rc)
    ld      b,a     ; x = r
    ld      c,0     ; y = 0
    ld      hl,0    ; error = 0(I need 16 bits)
    ld      a,(Gfx_xc)
    ld      d,a
    ld      a,(Gfx_yc)
    ld      e,a     ;(d,e) contains the center coordinates(xc, yc)
    ld      a,b
    and     a
    jr      z,outloop   ; if radius = 0, we exit
loop:
    ld      a,d
    add     a,b
    ld(Screen_x),a       ; xo + x
    ld      a,e
    add     a,c
    ld(Screen_y),a       ; yo + y
    call    Screen_PutPixel        ;(xo + x, yo + y)
    ld      a,e
    sub     a,c
    ld(Screen_y),a       ; yo - y
    call    Screen_PutPixel        ;(xo + x, yo + y)
    ld      a,d
    sub     b
    ld(Screen_x),a       ; xo - x
    call    Screen_PutPixel        ;(xo + x, yo + y)
    ld      a,e
    add     a,c
    ld(Screen_y),a       ; yo + y
    call    Screen_PutPixel        ;(xo + x, yo + y)
    ;; ---------------------
    ld      a,d
    add     a,c
    ld(Screen_x),a       ; xo + y
    ld      a,e
    add     a,b
    ld(Screen_y),a       ; yo + x
    call    Screen_PutPixel        ;(xo + x, yo + y)
    ld      a,e
    sub     b
    ld(Screen_y),a       ; yo - x
    call    Screen_PutPixel        ;(xo + x, yo + y)
    ld      a,d
    sub     c
    ld(Screen_x),a       ; xo - y
    call    Screen_PutPixel        ;(xo + x, yo + y)
    ld      a,e
    add     a,b
    ld(Screen_y),a       ; yo + x
    call    Screen_PutPixel        ;(xo + x, yo + y)
    ;; -----------------------
    push    bc
    ld      b,0
    add     hl,bc
    add     hl,bc
    inc     hl      ; error += 1 + 2*y
    pop     bc
    inc     c       ; y++
    push    hl
    push    bc
    ld      c,b
    scf
    ccf                 ; clear carry flag
    sbc     hl,bc       ; error - x
    dec     hl
    bit     7,h         ; is error - x <= 0 ?
    pop     bc
    pop     hl
    jr      nz,skip1    ; if error - x <= 0, skip
    push    bc
    ld      c,b
    scf
    ccf
    sbc     hl,bc
    sbc     hl,bc
    inc     hl
    pop     bc      ; error += 1 - 2*x
    dec     b       ; x--
skip1:
    ld      a,b
    cp      c       ; if y >= x, then exit
    jr      nc,loop
outloop:
	
	ret
block1:
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
	; Ending memory block
	end
