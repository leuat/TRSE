 processor 6502
	ORG $1201
	.byte    $0E, $08, $0A, $00, $9E, $20, $28
	.byte   $34,$36,$32,$34
	.byte    $29, $00, $00, $00
	ORG $1210
Pumpkid
	jmp block6219
credits		dc.b	"CREATED BY ANDY H - HEWCO.UK 2019"
	dc.b	0
packedScreens	dc.b $077, $048, $0e4, $049, $040, $04b, $0a7, $04b
	dc.b $031, $04d, $0c8, $04e, $054, $04f, $0e5, $04f
	dc.b $097, $050, $02b, $051, $0d1, $051, $075, $052
	dc.b $011, $054, $09d, $054, $0cd, $054, $02a, $055
	dc.b $08e, $055, $03, $056, $06e, $056, $0d3, $056
	dc.b $039, $057, $09c, $057, $020, $058, $086, $058
	dc.b $0f0, $058, $060, $059, $0d2, $059, $039, $05a
	dc.b $093, $05b, $0f6, $05b, $036, $05c, $0c, $05d
	dc.b $062, $05d
scr	dc.w	 
	org scr+46
clr	dc.w	 
	org clr+46
cMask	dc.b $00, $00, $00, $00, $00, $00, $00, $00
	dc.b $00, $00, $00, $00, $00, $00, $00, $00
	dc.b $00, $00, $00, $00, $00, $00, $00, $00
	dc.b $00, $00, $00, $0a, $0a, $0a, $0a, $0a
	dc.b $05, $05, $05, $05, $05, $05, $03, $03
	dc.b $05, $05, $05, $00, $00, $00, $02, $02
	dc.b $00, $00, $00, $00, $00, $00, $00, $00
	dc.b $00, $00, $01, $01, $01, $01, $01, $01
	dc.b 
i	dc.b	$00
j	dc.b	$00
x	dc.b	$00
y	dc.b	$00
t	dc.b	$00
n	dc.b	$00
s	dc.b	$00
e	dc.b	$00
w	dc.b	$00
p1	= $02
psnd1	= $04
psnd2	= $08
psnd3	= $16
psndfx	= $0B
sndFlag	dc.b	$01
ticks	dc.b	$00
oldTick	dc.b	$00
gameTimer	dc.b	$00
GAMETIMERMAX	dc.b	$32
gameMode	dc.b	$00
moonPos	dc.b	$02
cloudPos	dc.b	$00
level	dc.b	$00
lives	dc.b	$05
score	dc.b $00, $00, $00, $00
scCandy	dc.b $01, $00, $00, $00
scMoon	dc.b $00, $01, $00, $00
scZombie	dc.b $010, $00, $00, $00
scGhost	dc.b $05, $00, $00, $00
scWitch	dc.b $00, $010, $00, $00
t11		dc.b	"11PM"
	dc.b	0
t12		dc.b	"MIDNIGHT"
	dc.b	0
t01		dc.b	"1@AM"
	dc.b	0
t02		dc.b	"2@AM"
	dc.b	0
t03		dc.b	"3@AM"
	dc.b	0
t04		dc.b	"4@AM"
	dc.b	0
t05		dc.b	"5@AM"
	dc.b	0
t06		dc.b	"6@AM"
	dc.b	0
px	dc.b	$00
py	dc.b	$00
pan	dc.b	$00
pd	dc.b	$00
panFrames	dc.b $00, $00, $01, $02, $02, $01
pstep	dc.b	$00
pbgchar	dc.b	$00
pbgcol	dc.b	$00
wx	dc.b	$00
wy	dc.b	$00
wd	dc.b	$00
wbgchar	dc.b	$00
wbgcol	dc.b	$00
wsteps	dc.b	$00
wdelay	dc.b	$00
wdspeed	dc.b	$00
wdcount	dc.b	$00
 ; Temp vars section
printstring_text78491	dc.b	"@OUT@OF@TIME@@"
	dc.b	0
printstring_text11577	dc.b	"@GOT@BY@WITCH@"
	dc.b	0
printstring_text5751	dc.b	"POISONED@SKULL"
	dc.b	0
printstring_text53694	dc.b	"@GOT@BY@GHOST@"
	dc.b	0
printstring_text57771	dc.b	"@GOT@BY@ZOMBIE@"
	dc.b	0
printstring_text94828	dc.b	"THE@WITCH@RETREATS"
	dc.b	0
printstring_text13606	dc.b	"GET@READY@FOR"
	dc.b	0
 ; Temp vars section ends
EndSymbol
	org $1400
charset
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///pumpkid.bin"
	org $1600
colordata
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///colour.bin"
	org $1640
tcolordata
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///titlescol.bin"
	org $16b4
gameovercolordata
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///gameover-col.bin"
	org $16f4
gamewoncolordata
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///gamewon-col.bin"
	org $1734
gamefailcolordata
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///fail-col.bin"
	org $1774
gamenextcolordata
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///gamenext-col.bin"
	org $17b4
tcolorcrdata
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///credits-col.bin"
	org $1818
gNumGhosts	dc.b	$06
gx	dc.b	 
	org gx+10
gy	dc.b	 
	org gy+10
gbgchar	dc.b	 
	org gbgchar+10
gbgcol	dc.b	 
	org gbgcol+10
gmode	dc.b	 
	org gmode+10
gtime	dc.b	
gcurrent	dc.b	$00
zNumZombies	dc.b	$00
zx	dc.b	 
	org zx+10
zy	dc.b	 
	org zy+10
zbgchar	dc.b	 
	org zbgchar+10
zbgcol	dc.b	 
	org zbgcol+10
ztime	dc.b	$0a
zcurrent	dc.b	$00
zspawn	dc.b	 
	org zspawn+10
cx	dc.b	$00
cy	dc.b	$00
cdx	dc.b	$00
clife	dc.b	$00
cbgchar	dc.b	$00
cbgcol	dc.b	$00
candy	dc.b	$00
candyscr	dc.b	$00
witchHits	dc.b	$00
lghosts	dc.b $03, $04, $04, $05, $05, $06, $06, $07
	dc.b $07, $08, $09, $0a, $0a
lwitch	dc.b $07, $07, $07, $07, $06, $06, $06, $05
	dc.b $05, $05, $05, $04, $04
lzombie	dc.b $0b, $0a, $09, $08, $08, $07, $07, $06
	dc.b $06, $05, $05, $04, $04
lostchildlevel	dc.b	$00
lx	dc.b	$00
ly	dc.b	$00
stepsnd	dc.b	$00
EndBlock52
	org $1900
	
	
	; ***********  Defining procedure : init_decrunch
	;    Procedure type : User-defined procedure
	
init_decrunch
	jmp end_init_decrunch
;EXOD_LITERAL_SEQUENCES_NOT_USED = 1
; -------------------------------------------------------------------
; zero page addresses used
; -------------------------------------------------------------------
;exod_zp_len_lo = $a7
;exod_zp_src_lo  = $ae
;exod_zp_src_hi  = exod_zp_src_lo + 1
;exod_zp_bits_hi = $fc
;exod_zp_bitbuf  = $fd
exod_zp_len_lo = $47
exod_zp_src_lo  = $48
exod_zp_src_hi  = exod_zp_src_lo + 1
exod_zp_bits_hi = $4a
exod_zp_bitbuf  = $4b
exod_zp_dest_lo = exod_zp_bitbuf + 1	; dest addr lo
exod_zp_dest_hi = exod_zp_bitbuf + 2	; dest addr hi
exod_tabl_bi = exod_decrunch_table
exod_tabl_lo = exod_decrunch_table + 52
exod_tabl_hi = exod_decrunch_table + 104
; -------------------------------------------------------------------
; no code below this comment has to be modified in order to generate
; a working decruncher of this source file.
; However, you may want to relocate the tables last in the file to a
; more suitable address.
; -------------------------------------------------------------------
; -------------------------------------------------------------------
; jsr this label to decrunch, it will in turn init the tables and
; call the decruncher
; no constraints on register content, however the
; decimal flag has to be #0 (it almost always is, otherwise do a cld)
exod_decrunch:
; -------------------------------------------------------------------
; init zeropage, x and y regs. (12 bytes)
;
	ldy #0
	ldx #3
exod_init_zp:
	jsr exod_get_crunched_byte
        sta exod_zp_bitbuf-1,x
	dex
	bne exod_init_zp
; -------------------------------------------------------------------
; calculate tables (50 bytes)
; x and y must be #0 when entering
;
exod_nextone:
	inx
	tya
	and #$0f
	beq exod_shortcut		; starta på ny sekvens
	txa			; this clears reg a
	lsr			; and sets the carry flag
        ldx exod_tabl_bi-1,y
exod_rolle:
	rol
	rol exod_zp_bits_hi
	dex
	bpl exod_rolle		; c = 0 after this (rol exod_zp_bits_hi)
	adc exod_tabl_lo-1,y
	tax
	lda exod_zp_bits_hi
	adc exod_tabl_hi-1,y
exod_shortcut:
	sta exod_tabl_hi,y
	txa
	sta exod_tabl_lo,y
	ldx #4
	jsr exod_get_bits		; clears x-reg.
	sta exod_tabl_bi,y
	iny
	cpy #52
	bne exod_nextone
	ldy #0
	beq exod_begin
; -------------------------------------------------------------------
; get bits (29 bytes)
;
; args:
;   x = number of bits to get
; returns:
;   a = #bits_lo
;   x = #0
;   c = 0
;   z = 1
;   exod_zp_bits_hi = #bits_hi
; notes:
;   y is untouched
; -------------------------------------------------------------------
exod_get_bits:
	lda #$00
	sta exod_zp_bits_hi
	cpx #$01
	bcc exod_bits_done
exod_bits_next:
	lsr exod_zp_bitbuf
	bne exod_ok
	pha
exod_literal_get_byte:
	jsr exod_get_crunched_byte
	bcc exod_literal_byte_gotten
	ror
	sta exod_zp_bitbuf
	pla
exod_ok:
	rol
	rol exod_zp_bits_hi
	dex
	bne exod_bits_next
exod_bits_done:
	rts
; -------------------------------------------------------------------
; main copy loop (18(16) bytes)
;
exod_copy_next_hi:
	dex
	dec exod_zp_dest_hi
	dec exod_zp_src_hi
exod_copy_next:
	dey
	IFNCONST EXOD_LITERAL_SEQUENCES_NOT_USED
	bcc exod_literal_get_byte
	ENDIF
	lda (exod_zp_src_lo),y
exod_literal_byte_gotten:
	sta (exod_zp_dest_lo),y
exod_copy_start:
	tya
	bne exod_copy_next
exod_begin:
	txa
	bne exod_copy_next_hi
; -------------------------------------------------------------------
; decruncher entry point, needs calculated tables (21(13) bytes)
; x and y must be #0 when entering
;
	IFNCONST EXOD_LITERAL_SEQUENCES_NOT_USED
	inx
	jsr exod_get_bits
	tay
	bne exod_literal_start1
	ELSE
	dey
	ENDIF
exod_begin2:
	inx
	jsr exod_bits_next
	lsr
	iny
	bcc exod_begin2
	IFCONST EXOD_LITERAL_SEQUENCES_NOT_USED
	beq exod_literal_start
	ENDIF
	cpy #$11
	IFNCONST EXOD_LITERAL_SEQUENCES_NOT_USED
	bcc exod_sequence_start
	beq exod_bits_done
; -------------------------------------------------------------------
; literal sequence handling (13(2) bytes)
;
	ldx #$10
	jsr exod_get_bits
exod_literal_start1:
	sta <exod_zp_len_lo
	ldx <exod_zp_bits_hi
	ldy #0
	bcc exod_literal_start
exod_sequence_start:
	ELSE
	bcs exod_bits_done
	ENDIF
; -------------------------------------------------------------------
; calulate length of sequence (exod_zp_len) (11 bytes)
;
	ldx exod_tabl_bi - 1,y
	jsr exod_get_bits
	adc exod_tabl_lo - 1,y	; we have now calculated exod_zp_len_lo
	sta exod_zp_len_lo
; -------------------------------------------------------------------
; now do the hibyte of the sequence length calculation (6 bytes)
	lda exod_zp_bits_hi
	adc exod_tabl_hi - 1,y	; c = 0 after this.
	pha
; -------------------------------------------------------------------
; here we decide what offset table to use (20 bytes)
; x is 0 here
;
	bne exod_nots123
	ldy exod_zp_len_lo
	cpy #$04
	bcc exod_size123
exod_nots123:
	ldy #$03
exod_size123:
	ldx exod_tabl_bit - 1,y
	jsr exod_get_bits
	adc exod_tabl_off - 1,y	; c = 0 after this.
	tay			; 1 <= y <= 52 here
; -------------------------------------------------------------------
; Here we do the dest_lo -= len_lo subtraction to prepare exod_zp_dest
; but we do it backwards:	a - b == (b - a - 1) ^ ~0 (C-syntax)
; (16(16) bytes)
	lda exod_zp_len_lo
exod_literal_start:			; literal enters here with y = 0, c = 1
	sbc exod_zp_dest_lo
	bcc exod_noborrow
	dec exod_zp_dest_hi
exod_noborrow:
	eor #$ff
	sta exod_zp_dest_lo
	cpy #$01		; y < 1 then literal
	IFNCONST EXOD_LITERAL_SEQUENCES_NOT_USED
	bcc exod_pre_copy
	ELSE
	bcc exod_literal_get_byte
	ENDIF
; -------------------------------------------------------------------
; calulate absolute offset (exod_zp_src) (27 bytes)
;
	ldx exod_tabl_bi,y
	jsr exod_get_bits;
	adc exod_tabl_lo,y
	bcc exod_skipcarry
	inc exod_zp_bits_hi
	clc
exod_skipcarry:
	adc exod_zp_dest_lo
	sta exod_zp_src_lo
	lda exod_zp_bits_hi
	adc exod_tabl_hi,y
	adc exod_zp_dest_hi
	sta exod_zp_src_hi
; -------------------------------------------------------------------
; prepare for copy loop (8(6) bytes)
;
	pla
	tax
	IFNCONST EXOD_LITERAL_SEQUENCES_NOT_USED
	sec
exod_pre_copy:
	ldy <exod_zp_len_lo
	jmp exod_copy_start
	ELSE
	ldy <exod_zp_len_lo
	bcc exod_copy_start
	ENDIF
; -------------------------------------------------------------------
; two small static tables (6(6) bytes)
;
exod_tabl_bit:
	.byte 2,4,4
exod_tabl_off:
	.byte 48,32,16
; -------------------------------------------------------------------
; end of decruncher
; -------------------------------------------------------------------
; -------------------------------------------------------------------
; this 156 byte table area may be relocated. It may also be clobbered
; by other data between decrunches.
; -------------------------------------------------------------------
exod_decrunch_table:
	.byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.byte 0,0,0,0,0,0,0,0,0,0,0,0
; -------------------------------------------------------------------
; end of decruncher
; -------------------------------------------------------------------
exod_get_crunched_byte:
	lda opbase + 1
	bne nowrap
	dec opbase + 2
nowrap:	dec opbase + 1
	;; change the $ffff to point to the byte immediately following the last
	;; byte of the crunched file data (mem command)
opbase:	
	lda $FFFF
	rts
end_init_decrunch
	rts
	
	
	; ***********  Defining procedure : init_viairq
	;    Procedure type : User-defined procedure
	
init_via_irq:
  ldx #0       ; wait for this raster line (times 2)
A0_vic_raster:
  cpx $9004
  bne A0_vic_raster        ; at this stage, the inaccuracy is 7 clock cycles
                ; the processor is in this place 2 to 9 cycles
                ; after $9004 has changed
  ldy #9
  bit $24
A1_vic_raster:
  ldx $9004
  txa
  bit $24
  ldx #24
  dex
  bne *-1       ; first spend some time (so that the whole
  cmp $9004     ; loop will be 2 raster lines)
  bcs *+2       ; save one cycle if $9004 changed too late
  dey
  bne A1_vic_raster
                ; now it is fully synchronized
                ; 6 cycles have passed since last $9004 change
                ; and we are on line 2(28+9)=74
;initialize the timers
  lda #$40      ; enable Timer A free run of both VIAs
  sta $911b
  sta $912b
        ; 312*71-2 = $568
timers_vic_raster:
  lda #$56
  ldx #$86
  sta $9116     ; load the timer low byte latches
  sta $9126
  ldy #7        ; make a little delay to get the raster effect to the
  dey           ; right place
  bne *-1
  nop
  nop
  stx $9125     ; start the IRQ timer A
                ; 6560-101: 65 cycles from $9004 change
                ; 6561-101: 77 cycles from $9004 change
  ldy #10       ; spend some time (1+5*9+4=55 cycles)
  dey           ; before starting the reference timer
  bne *-1
  stx $9115     ; start the reference timer
pointers_vic_raster:
  lda #00     ; set the raster IRQ routine pointer
  sta $314
  lda #00
  sta $315
  lda #$c0
  sta $912e     ; enable Timer A underflow interrupts
  rts
	rts
	
	
	; ***********  Defining procedure : initbcd
	;    Procedure type : Built-in function
	;    Requires initialization : no
	
	; ----------
	; BCD support
	; Commands for manipulating and displaying BCD numbers
	; 
	; a = digit to display
	; y = screen offset
bcdplotdigit
	clc
	adc #48 ; 0 screen code
	sta (screenmemory),y
	dey
	rts
	
	
	; ***********  Defining procedure : initjoy1
	;    Procedure type : Built-in function
	;    Requires initialization : no
	
	; ----------
	; ReadJoy1 and ReadJoy2 (on supported platforms)
	; populates joy1 and joy1pressed which can be tested by AND-ing with the following constants:
;JOY_DOWN  = %00000100
;JOY_UP    = %00000010
;JOY_LEFT  = %00001000
;JOY_RIGHT = %00000001
;JOY_FIRE  = %00010000
VIC20_PORTACASS = $911F
VIC20_PORTBVIA2 = $9120  ; Port B 6522 2 value (joystick)
VIC20_PORTBVIA2d = $9122 ; Port B 6522 2 direction (joystick)
joy1 .byte 0
joy1last .byte 0
joy1pressed .byte 0
callReadJoy1
	LDA VIC20_PORTACASS
	EOR #$FF
	AND #$3C
	LDX #$7F
	SEI
	STX VIC20_PORTBVIA2d
	LDY VIC20_PORTBVIA2
	BMI JoySkip28624
	ORA #$02
JoySkip28624
	LDX #$FF
	STX VIC20_PORTBVIA2d
	CLI
	LSR
	STA joy1
	eor joy1last
	and joy1
	sta joy1pressed
	lda joy1
	sta joy1last
	rts
	
	
	; ***********  Defining procedure : initprintdecimal
	;    Procedure type : Built-in function
	;    Requires initialization : no
	
ipd_div_lo dc.b 0
ipd_div_hi dc.b 0
init_printdecimal_div10
	ldx #$11
	lda #$00
	clc
init_printdecimal_loop
	rol
	cmp #$0A
	bcc init_printdecimal_skip
	sbc #$0A
init_printdecimal_skip
	rol ipd_div_lo
	rol ipd_div_hi
	dex
	bne init_printdecimal_loop
	rts
	
	
	; ***********  Defining procedure : initprintstring
	;    Procedure type : User-defined procedure
	
print_text = $4c
print_number_text .dc "    ",0
printstring
	ldy #0
printstringloop
	lda (print_text),y
	cmp #0 ;keep
	beq printstring_done
	cmp #64
	bcc printstring_skip
	sec
	sbc #64
printstring_skip
	sta (screenmemory),y
	iny
	dex
	cpx #0
	beq printstring_done
	jmp printstringloop
printstring_done
	rts
	
	
	; ***********  Defining procedure : initrandom256
	;    Procedure type : Built-in function
	;    Requires initialization : no
	
	; init random256
Random
	lda #$01
	asl
	bcc RandomSkip51528
	eor #$4d
RandomSkip51528
	sta Random+1
	eor $9124
	rts
	
	
	; ***********  Defining procedure : PlaySnd1
	;    Procedure type : User-defined procedure
	
vsnd1Time	dc.b	$00
vsnd1	dc.b	
block32871
PlaySnd1
	; Assigning single variable : vsnd1Time
	lda #0
	sta vsnd1Time
	rts
	
	
	; ***********  Defining procedure : PlaySnd2
	;    Procedure type : User-defined procedure
	
vsnd2Time	dc.b	$00
vsnd2	dc.b	
block5732
PlaySnd2
	; Assigning single variable : vsnd2Time
	lda #0
	sta vsnd2Time
	rts
	
	
	; ***********  Defining procedure : PlaySnd3
	;    Procedure type : User-defined procedure
	
vsnd3Time	dc.b	$00
vsnd3	dc.b	
block48829
PlaySnd3
	; Assigning single variable : vsnd3Time
	lda #0
	sta vsnd3Time
	rts
	
	
	; ***********  Defining procedure : PlaySndfx
	;    Procedure type : User-defined procedure
	
vsndfxTime	dc.b	$00
vsndfx	dc.b	
block9503
PlaySndfx
	; Assigning single variable : vsndfxTime
	lda #0
	sta vsndfxTime
	rts
	
	
	; ***********  Defining procedure : SetupSound
	;    Procedure type : User-defined procedure
	
noSound	dc.b	$00
sndGhostAppear	dc.b $081, $07, $020, $02, $043, $04, $00
sndPumpkidFootsteps	dc.b $04f, $03, $00
sndWitchDive	dc.b $041, $023, $042, $08, $041, $06, $020, $0c
	dc.b $083, $019, $00
sndWitchSpell	dc.b $08f, $04, $042, $0c, $041, $02, $00
sndDead	dc.b $041, $0a, $010, $03, $041, $08, $010, $03
	dc.b $041, $06, $00
sndPickup	dc.b $042, $04, $010, $03, $08f, $04, $00
sndCAppear	dc.b $020, $01, $010, $01, $020, $01, $00
sndShoot	dc.b $020, $01, $010, $01, $041, $0a, $00
sndGrave	dc.b $020, $0d, $010, $05, $089, $01, $020, $0c
	dc.b $010, $05, $089, $01, $020, $0c, $00
sndExplode	dc.b $010, $02, $020, $05, $044, $04, $00
sndMNoteShort	dc.b $041, $01, $081, $01, $041, $01, $081, $01
	dc.b $020, $01, $041, $01, $081, $01, $041, $01
	dc.b $081, $01, $020, $02, $041, $01, $081, $01
	dc.b $041, $01, $081, $01, $020, $01, $041, $01
	dc.b $081, $01, $041, $01, $081, $01, $020, $02
	dc.b $00
sndSWitch	dc.b $020, $01, $00
block30019
SetupSound
	jsr StopSounds
	; Poke
	; Optimization: shift is zero
	lda #15
	sta $900e
	rts
	
	
	; ***********  Defining procedure : StopSounds
	;    Procedure type : User-defined procedure
	
StopSounds
	; Assigning single variable : vsnd1
	lda #0
	sta vsnd1
	; Assigning single variable : vsnd1Time
	sta vsnd1Time
	; Assigning single variable : psnd1
	lda #<noSound
	ldx #>noSound
	sta psnd1
	stx psnd1+1
	; Poke
	; Optimization: shift is zero
	lda #0
	sta $900a
	; Assigning single variable : vsnd2
	sta vsnd2
	; Assigning single variable : vsnd2Time
	sta vsnd2Time
	; Assigning single variable : psnd2
	lda #<noSound
	sta psnd2
	stx psnd2+1
	; Poke
	; Optimization: shift is zero
	lda #0
	sta $900b
	; Assigning single variable : vsnd3
	sta vsnd3
	; Assigning single variable : vsnd3Time
	sta vsnd3Time
	; Assigning single variable : psnd3
	lda #<noSound
	sta psnd3
	stx psnd3+1
	; Poke
	; Optimization: shift is zero
	lda #0
	sta $900c
	; Assigning single variable : vsndfx
	sta vsndfx
	; Assigning single variable : vsndfxTime
	sta vsndfxTime
	; Assigning single variable : psndfx
	lda #<noSound
	sta psndfx
	stx psndfx+1
	; Poke
	; Optimization: shift is zero
	lda #0
	sta $900d
	rts
	
	
	; ***********  Defining procedure : Snd1Time
	;    Procedure type : User-defined procedure
	
Snd1Time
	; Binary clause Simplified: GREATER
	lda vsnd1Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock18149
	beq elsedoneblock18149
ConditionalTrueBlock86715
	; Assigning single variable : vsnd1Time
	dec vsnd1Time
elseblock26340
elsedoneblock18149
	; Binary clause Simplified: EQUALS
	lda vsnd1Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock93451
ConditionalTrueBlock2245
	; Assigning single variable : psnd1
	; WORD optimization: a=a+b
	lda psnd1+0
	
	clc
	adc #2
	bcc WordAdd97488
	inc psnd1+1
WordAdd97488
	sta psnd1+0
elseblock22846
elsedoneblock93451
	rts
	
	
	; ***********  Defining procedure : Snd2Time
	;    Procedure type : User-defined procedure
	
Snd2Time
	; Binary clause Simplified: GREATER
	lda vsnd2Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock65193
	beq elsedoneblock65193
ConditionalTrueBlock69841
	; Assigning single variable : vsnd2Time
	dec vsnd2Time
elseblock92350
elsedoneblock65193
	; Binary clause Simplified: EQUALS
	lda vsnd2Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock36987
ConditionalTrueBlock70124
	; Assigning single variable : psnd2
	; WORD optimization: a=a+b
	lda psnd2+0
	
	clc
	adc #2
	bcc WordAdd22227
	inc psnd2+1
WordAdd22227
	sta psnd2+0
elseblock24914
elsedoneblock36987
	rts
	
	
	; ***********  Defining procedure : Snd3Time
	;    Procedure type : User-defined procedure
	
Snd3Time
	; Binary clause Simplified: GREATER
	lda vsnd3Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock52551
	beq elsedoneblock52551
ConditionalTrueBlock81936
	; Assigning single variable : vsnd3Time
	dec vsnd3Time
elseblock51432
elsedoneblock52551
	; Binary clause Simplified: EQUALS
	lda vsnd3Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock76121
ConditionalTrueBlock75407
	; Assigning single variable : psnd3
	; WORD optimization: a=a+b
	lda psnd3+0
	
	clc
	adc #2
	bcc WordAdd61237
	inc psnd3+1
WordAdd61237
	sta psnd3+0
elseblock1474
elsedoneblock76121
	rts
	
	
	; ***********  Defining procedure : SndfxTime
	;    Procedure type : User-defined procedure
	
SndfxTime
	; Binary clause Simplified: GREATER
	lda vsndfxTime
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock66143
	beq elsedoneblock66143
ConditionalTrueBlock65818
	; Assigning single variable : vsndfxTime
	dec vsndfxTime
elseblock94428
elsedoneblock66143
	; Binary clause Simplified: EQUALS
	lda vsndfxTime
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock64443
ConditionalTrueBlock18776
	; Assigning single variable : psndfx
	; WORD optimization: a=a+b
	lda psndfx+0
	
	clc
	adc #2
	bcc WordAdd18606
	inc psndfx+1
WordAdd18606
	sta psndfx+0
elseblock22404
elsedoneblock64443
	rts
	
	
	; ***********  Defining procedure : UpdateSound1
	;    Procedure type : User-defined procedure
	
UpdateSound1
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd1),y
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock70688
ConditionalTrueBlock44818
	; Binary clause Simplified: NOTEQUALS
	lda vsnd1
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock6725
ConditionalTrueBlock95499
	; Assigning single variable : vsnd1
	lda #0
	sta vsnd1
	; Poke
	; Optimization: shift is zero
	sta $900a
elseblock89772
elsedoneblock6725
	rts
elseblock35128
elsedoneblock70688
	; Binary clause: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd1),y
	
	; Compare with pure num / var optimization
	cmp #$20;keep
	; BC done
	bne tempfail3465
binaryclausesuccess73416
	jmp ConditionalTrueBlock68139
tempfail3465
	; Binary clause: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd1),y
	
	; Compare with pure num / var optimization
	cmp #$10;keep
	; BC done
	bne elseblock2954
binaryclausesuccess13258
ConditionalTrueBlock68139
	; Binary clause Simplified: EQUALS
	lda vsnd1Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock62600
ConditionalTrueBlock42062
	; Assigning single variable : vsnd1Time
	; Load pointer array
	ldy #$1
	lda (psnd1),y
	
	sta vsnd1Time
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd1),y
	
	; Compare with pure num / var optimization
	cmp #$20;keep
	bne elsedoneblock48899
ConditionalTrueBlock57199
	; Poke
	; Optimization: shift is zero
	lda vsnd1
	sta $900a
elseblock87981
elsedoneblock48899
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd1),y
	
	; Compare with pure num / var optimization
	cmp #$10;keep
	bne elsedoneblock87190
ConditionalTrueBlock72813
	; Poke
	; Optimization: shift is zero
	lda #0
	sta $900a
elseblock39668
elsedoneblock87190
elseblock5624
elsedoneblock62600
	jsr Snd1Time
elseblock2954
elsedoneblock69786
	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	; Load pointer array
	ldy #$0
	lda (psnd1),y
	
	and #128
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$80;keep
	bne elsedoneblock22090
ConditionalTrueBlock65084
	; Binary clause Simplified: EQUALS
	lda vsnd1Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock3348
ConditionalTrueBlock6887
	; Assigning single variable : vsnd1Time
	; Load pointer array
	ldy #$1
	lda (psnd1),y
	
	sta vsnd1Time
elseblock89412
elsedoneblock3348
	; Assigning single variable : vsnd1
	; 8 bit binop
	; Add/sub where right value is constant number
	; 8 bit binop
	; Add/sub where right value is constant number
	; Load pointer array
	ldy #$0
	lda (psnd1),y
	
	sec
	sbc #128
	 ; end add / sub var with constant
	
	clc
	adc vsnd1
	 ; end add / sub var with constant
	
	sta vsnd1
	; Poke
	; Optimization: shift is zero
	sta $900a
	jsr Snd1Time
elseblock11340
elsedoneblock22090
	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	; Load pointer array
	ldy #$0
	lda (psnd1),y
	
	and #64
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$40;keep
	bne elsedoneblock66342
ConditionalTrueBlock2336
	; Binary clause Simplified: EQUALS
	lda vsnd1Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock39811
ConditionalTrueBlock29904
	; Assigning single variable : vsnd1Time
	; Load pointer array
	ldy #$1
	lda (psnd1),y
	
	sta vsnd1Time
elseblock55939
elsedoneblock39811
	; Assigning single variable : vsnd1
	; 8 bit binop
	; Add/sub right value is variable/expression
	; 8 bit binop
	; Add/sub where right value is constant number
	; Load pointer array
	ldy #$0
	lda (psnd1),y
	
	sec
	sbc #64
	 ; end add / sub var with constant
	
rightvarAddSub_var11705 = $54
	sta rightvarAddSub_var11705
	lda vsnd1
	sec
	sbc rightvarAddSub_var11705
	
	sta vsnd1
	; Poke
	; Optimization: shift is zero
	sta $900a
	jsr Snd1Time
elseblock25210
elsedoneblock66342
	rts
	
	
	; ***********  Defining procedure : UpdateSound2
	;    Procedure type : User-defined procedure
	
UpdateSound2
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd2),y
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock96658
ConditionalTrueBlock29150
	; Binary clause Simplified: NOTEQUALS
	lda vsnd2
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock73850
ConditionalTrueBlock11972
	; Assigning single variable : vsnd2
	lda #0
	sta vsnd2
	; Poke
	; Optimization: shift is zero
	sta $900b
elseblock7672
elsedoneblock73850
	rts
elseblock65984
elsedoneblock96658
	; Binary clause: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd2),y
	
	; Compare with pure num / var optimization
	cmp #$20;keep
	; BC done
	bne tempfail62754
binaryclausesuccess54259
	jmp ConditionalTrueBlock39299
tempfail62754
	; Binary clause: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd2),y
	
	; Compare with pure num / var optimization
	cmp #$10;keep
	; BC done
	bne elseblock6640
binaryclausesuccess28202
ConditionalTrueBlock39299
	; Binary clause Simplified: EQUALS
	lda vsnd2Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock22842
ConditionalTrueBlock36784
	; Assigning single variable : vsnd2Time
	; Load pointer array
	ldy #$1
	lda (psnd2),y
	
	sta vsnd2Time
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd2),y
	
	; Compare with pure num / var optimization
	cmp #$20;keep
	bne elsedoneblock17567
ConditionalTrueBlock72890
	; Poke
	; Optimization: shift is zero
	lda vsnd2
	sta $900b
elseblock99754
elsedoneblock17567
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd2),y
	
	; Compare with pure num / var optimization
	cmp #$10;keep
	bne elsedoneblock73788
ConditionalTrueBlock14500
	; Poke
	; Optimization: shift is zero
	lda #0
	sta $900b
elseblock38046
elsedoneblock73788
elseblock2021
elsedoneblock22842
	jsr Snd2Time
elseblock6640
elsedoneblock6042
	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	; Load pointer array
	ldy #$0
	lda (psnd2),y
	
	and #128
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$80;keep
	bne elsedoneblock5363
ConditionalTrueBlock73303
	; Binary clause Simplified: EQUALS
	lda vsnd2Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock95436
ConditionalTrueBlock49157
	; Assigning single variable : vsnd2Time
	; Load pointer array
	ldy #$1
	lda (psnd2),y
	
	sta vsnd2Time
elseblock3729
elsedoneblock95436
	; Assigning single variable : vsnd2
	; 8 bit binop
	; Add/sub where right value is constant number
	; 8 bit binop
	; Add/sub where right value is constant number
	; Load pointer array
	ldy #$0
	lda (psnd2),y
	
	sec
	sbc #128
	 ; end add / sub var with constant
	
	clc
	adc vsnd2
	 ; end add / sub var with constant
	
	sta vsnd2
	; Poke
	; Optimization: shift is zero
	sta $900b
	jsr Snd2Time
elseblock3033
elsedoneblock5363
	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	; Load pointer array
	ldy #$0
	lda (psnd2),y
	
	and #64
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$40;keep
	bne elsedoneblock60028
ConditionalTrueBlock70460
	; Binary clause Simplified: EQUALS
	lda vsnd2Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock6403
ConditionalTrueBlock32002
	; Assigning single variable : vsnd2Time
	; Load pointer array
	ldy #$1
	lda (psnd2),y
	
	sta vsnd2Time
elseblock90428
elsedoneblock6403
	; Assigning single variable : vsnd2
	; 8 bit binop
	; Add/sub right value is variable/expression
	; 8 bit binop
	; Add/sub where right value is constant number
	; Load pointer array
	ldy #$0
	lda (psnd2),y
	
	sec
	sbc #64
	 ; end add / sub var with constant
	
rightvarAddSub_var17647 = $54
	sta rightvarAddSub_var17647
	lda vsnd2
	sec
	sbc rightvarAddSub_var17647
	
	sta vsnd2
	; Poke
	; Optimization: shift is zero
	sta $900b
	jsr Snd2Time
elseblock26304
elsedoneblock60028
	rts
	
	
	; ***********  Defining procedure : UpdateSound3
	;    Procedure type : User-defined procedure
	
UpdateSound3
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd3),y
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock82134
ConditionalTrueBlock95151
	; Binary clause Simplified: NOTEQUALS
	lda vsnd3
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock76335
ConditionalTrueBlock36429
	; Assigning single variable : vsnd3
	lda #0
	sta vsnd3
	; Poke
	; Optimization: shift is zero
	sta $900c
elseblock95343
elsedoneblock76335
	rts
elseblock22535
elsedoneblock82134
	; Binary clause: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd3),y
	
	; Compare with pure num / var optimization
	cmp #$20;keep
	; BC done
	bne tempfail26057
binaryclausesuccess73168
	jmp ConditionalTrueBlock7971
tempfail26057
	; Binary clause: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd3),y
	
	; Compare with pure num / var optimization
	cmp #$10;keep
	; BC done
	bne elseblock16949
binaryclausesuccess90358
ConditionalTrueBlock7971
	; Binary clause Simplified: EQUALS
	lda vsnd3Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock2726
ConditionalTrueBlock65100
	; Assigning single variable : vsnd3Time
	; Load pointer array
	ldy #$1
	lda (psnd3),y
	
	sta vsnd3Time
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd3),y
	
	; Compare with pure num / var optimization
	cmp #$20;keep
	bne elsedoneblock39214
ConditionalTrueBlock41312
	; Poke
	; Optimization: shift is zero
	lda vsnd3
	sta $900c
elseblock61886
elsedoneblock39214
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd3),y
	
	; Compare with pure num / var optimization
	cmp #$10;keep
	bne elsedoneblock9610
ConditionalTrueBlock14412
	; Poke
	; Optimization: shift is zero
	lda #0
	sta $900c
elseblock59479
elsedoneblock9610
elseblock4346
elsedoneblock2726
	jsr Snd3Time
elseblock16949
elsedoneblock60289
	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	; Load pointer array
	ldy #$0
	lda (psnd3),y
	
	and #128
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$80;keep
	bne elsedoneblock56620
ConditionalTrueBlock6355
	; Binary clause Simplified: EQUALS
	lda vsnd3Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock25849
ConditionalTrueBlock606
	; Assigning single variable : vsnd3Time
	; Load pointer array
	ldy #$1
	lda (psnd3),y
	
	sta vsnd3Time
elseblock72260
elsedoneblock25849
	; Assigning single variable : vsnd3
	; 8 bit binop
	; Add/sub where right value is constant number
	; 8 bit binop
	; Add/sub where right value is constant number
	; Load pointer array
	ldy #$0
	lda (psnd3),y
	
	sec
	sbc #128
	 ; end add / sub var with constant
	
	clc
	adc vsnd3
	 ; end add / sub var with constant
	
	sta vsnd3
	; Poke
	; Optimization: shift is zero
	sta $900c
	jsr Snd3Time
elseblock47641
elsedoneblock56620
	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	; Load pointer array
	ldy #$0
	lda (psnd3),y
	
	and #64
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$40;keep
	bne elsedoneblock34945
ConditionalTrueBlock35217
	; Binary clause Simplified: EQUALS
	lda vsnd3Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock45729
ConditionalTrueBlock49314
	; Assigning single variable : vsnd3Time
	; Load pointer array
	ldy #$1
	lda (psnd3),y
	
	sta vsnd3Time
elseblock34471
elsedoneblock45729
	; Assigning single variable : vsnd3
	; 8 bit binop
	; Add/sub right value is variable/expression
	; 8 bit binop
	; Add/sub where right value is constant number
	; Load pointer array
	ldy #$0
	lda (psnd3),y
	
	sec
	sbc #64
	 ; end add / sub var with constant
	
rightvarAddSub_var23618 = $54
	sta rightvarAddSub_var23618
	lda vsnd3
	sec
	sbc rightvarAddSub_var23618
	
	sta vsnd3
	; Poke
	; Optimization: shift is zero
	sta $900c
	jsr Snd3Time
elseblock48518
elsedoneblock34945
	rts
	
	
	; ***********  Defining procedure : UpdateSoundfx
	;    Procedure type : User-defined procedure
	
UpdateSoundfx
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #$0
	lda (psndfx),y
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock31233
ConditionalTrueBlock11388
	; Binary clause Simplified: NOTEQUALS
	lda vsndfx
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock28794
ConditionalTrueBlock68363
	; Assigning single variable : vsndfx
	lda #0
	sta vsndfx
	; Poke
	; Optimization: shift is zero
	sta $900d
elseblock92622
elsedoneblock28794
	rts
elseblock33074
elsedoneblock31233
	; Binary clause: EQUALS
	; Load pointer array
	ldy #$0
	lda (psndfx),y
	
	; Compare with pure num / var optimization
	cmp #$20;keep
	; BC done
	bne tempfail39862
binaryclausesuccess35379
	jmp ConditionalTrueBlock17462
tempfail39862
	; Binary clause: EQUALS
	; Load pointer array
	ldy #$0
	lda (psndfx),y
	
	; Compare with pure num / var optimization
	cmp #$10;keep
	; BC done
	bne elseblock56682
binaryclausesuccess92685
ConditionalTrueBlock17462
	; Binary clause Simplified: EQUALS
	lda vsndfxTime
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock79207
ConditionalTrueBlock24176
	; Assigning single variable : vsndfxTime
	; Load pointer array
	ldy #$1
	lda (psndfx),y
	
	sta vsndfxTime
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #$0
	lda (psndfx),y
	
	; Compare with pure num / var optimization
	cmp #$20;keep
	bne elsedoneblock63291
ConditionalTrueBlock41805
	; Poke
	; Optimization: shift is zero
	lda vsndfx
	sta $900d
elseblock19934
elsedoneblock63291
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #$0
	lda (psndfx),y
	
	; Compare with pure num / var optimization
	cmp #$10;keep
	bne elsedoneblock28993
ConditionalTrueBlock33589
	; Poke
	; Optimization: shift is zero
	lda #0
	sta $900d
elseblock43768
elsedoneblock28993
elseblock33483
elsedoneblock79207
	jsr SndfxTime
elseblock56682
elsedoneblock39390
	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	; Load pointer array
	ldy #$0
	lda (psndfx),y
	
	and #128
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$80;keep
	bne elsedoneblock26717
ConditionalTrueBlock44822
	; Binary clause Simplified: EQUALS
	lda vsndfxTime
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock78179
ConditionalTrueBlock27814
	; Assigning single variable : vsndfxTime
	; Load pointer array
	ldy #$1
	lda (psndfx),y
	
	sta vsndfxTime
elseblock34713
elsedoneblock78179
	; Assigning single variable : vsndfx
	; 8 bit binop
	; Add/sub where right value is constant number
	; 8 bit binop
	; Add/sub where right value is constant number
	; Load pointer array
	ldy #$0
	lda (psndfx),y
	
	sec
	sbc #128
	 ; end add / sub var with constant
	
	clc
	adc vsndfx
	 ; end add / sub var with constant
	
	sta vsndfx
	; Poke
	; Optimization: shift is zero
	sta $900d
	jsr SndfxTime
elseblock76982
elsedoneblock26717
	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	; Load pointer array
	ldy #$0
	lda (psndfx),y
	
	and #64
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$40;keep
	bne elsedoneblock35710
ConditionalTrueBlock27088
	; Binary clause Simplified: EQUALS
	lda vsndfxTime
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock49291
ConditionalTrueBlock66328
	; Assigning single variable : vsndfxTime
	; Load pointer array
	ldy #$1
	lda (psndfx),y
	
	sta vsndfxTime
elseblock925
elsedoneblock49291
	; Assigning single variable : vsndfx
	; 8 bit binop
	; Add/sub right value is variable/expression
	; 8 bit binop
	; Add/sub where right value is constant number
	; Load pointer array
	ldy #$0
	lda (psndfx),y
	
	sec
	sbc #64
	 ; end add / sub var with constant
	
rightvarAddSub_var77217 = $54
	sta rightvarAddSub_var77217
	lda vsndfx
	sec
	sbc rightvarAddSub_var77217
	
	sta vsndfx
	; Poke
	; Optimization: shift is zero
	sta $900d
	jsr SndfxTime
elseblock32919
elsedoneblock35710
	rts
	
	
	; ***********  Defining procedure : UpdateSound
	;    Procedure type : User-defined procedure
	
UpdateSound
	jsr UpdateSound1
	jsr UpdateSound2
	jsr UpdateSound3
	jsr UpdateSoundfx
	rts
	
	
	; ***********  Defining procedure : SetupMusic
	;    Procedure type : User-defined procedure
	
tuneTitle	dc.b $0db, $0e1, $0e7, $0e8, $0e1, $0e7, $0d7
musicStep	dc.b	$00
musicNote	dc.b	$00
musicRepeat	dc.b	$00
block96963
SetupMusic
	; Assigning single variable : musicNote
	lda #0
	sta musicNote
	; Assigning single variable : musicStep
	sta musicStep
	; Assigning single variable : musicRepeat
	sta musicRepeat
	; Poke
	; Optimization: shift is zero
	lda #11
	sta $900e
	rts
	
	
	; ***********  Defining procedure : UpdateTitleMusic
	;    Procedure type : User-defined procedure
	
UpdateTitleMusic
	; Binary clause Simplified: EQUALS
	lda musicRepeat
	; Compare with pure num / var optimization
	cmp #$4;keep
	bne elsedoneblock14904
ConditionalTrueBlock63858
	rts
elseblock78130
elsedoneblock14904
	; Binary clause Simplified: EQUALS
	lda musicStep
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock13073
ConditionalTrueBlock89685
	; Assigning single variable : psnd3
	lda #<sndMNoteShort
	ldx #>sndMNoteShort
	sta psnd3
	stx psnd3+1
	; Assigning single variable : vsnd3
	; Load Byte array
	ldx musicNote
	lda tuneTitle,x
	
	sta vsnd3
	jsr PlaySnd3
	; Assigning single variable : musicNote
	inc musicNote
	; Binary clause Simplified: EQUALS
	lda musicNote
	; Compare with pure num / var optimization
	cmp #$7;keep
	bne elsedoneblock57522
ConditionalTrueBlock53714
	; Assigning single variable : musicNote
	lda #0
	sta musicNote
	; Assigning single variable : musicRepeat
	inc musicRepeat
	; Binary clause Simplified: EQUALS
	lda musicRepeat
	; Compare with pure num / var optimization
	cmp #$1;keep
	bne elsedoneblock27335
ConditionalTrueBlock49122
	; Poke
	; Optimization: shift is zero
	lda #8
	sta $900e
elseblock54154
elsedoneblock27335
	; Binary clause Simplified: EQUALS
	lda musicRepeat
	; Compare with pure num / var optimization
	cmp #$2;keep
	bne elsedoneblock11776
ConditionalTrueBlock2747
	; Poke
	; Optimization: shift is zero
	lda #5
	sta $900e
elseblock91171
elsedoneblock11776
	; Binary clause Simplified: EQUALS
	lda musicRepeat
	; Compare with pure num / var optimization
	cmp #$3;keep
	bne elsedoneblock9933
ConditionalTrueBlock21703
	; Poke
	; Optimization: shift is zero
	lda #2
	sta $900e
elseblock14653
elsedoneblock9933
elseblock86059
elsedoneblock57522
elseblock4789
elsedoneblock13073
	; Assigning single variable : musicStep
	inc musicStep
	; Binary clause Simplified: EQUALS
	lda musicStep
	; Compare with pure num / var optimization
	cmp #$13;keep
	bne elsedoneblock48720
ConditionalTrueBlock62806
	; Assigning single variable : psnd3
	lda #<noSound
	ldx #>noSound
	sta psnd3
	stx psnd3+1
elseblock15797
elsedoneblock48720
	; Binary clause Simplified: EQUALS
	lda musicStep
	; Compare with pure num / var optimization
	cmp #$14;keep
	bne elsedoneblock76376
ConditionalTrueBlock42698
	; Assigning single variable : musicStep
	lda #0
	sta musicStep
elseblock10991
elsedoneblock76376
	rts
	
	
	; ***********  Defining procedure : UpdateLostChild
	;    Procedure type : User-defined procedure
	
UpdateLostChild
	; Binary clause: EQUALS
	lda moonPos
	; Compare with pure num / var optimization
	cmp #$8;keep
	; BC done
	bne binaryclausefailed87334
binaryclausesuccess48159
	lda #1; success
	jmp binaryclausefinished88421
binaryclausefailed87334
	lda #0 ; failed state
binaryclausefinished88421
	cmp #1
	beq ConditionalTrueBlock18189
	jmp elsedoneblock52506
ConditionalTrueBlock18189
	; Binary clause: EQUALS
	lda lx
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bne binaryclausefailed74295
binaryclausesuccess4313
	lda #1; success
	jmp binaryclausefinished1877
binaryclausefailed74295
	lda #0 ; failed state
binaryclausefinished1877
	cmp #1
	beq ConditionalTrueBlock1354
	jmp elsedoneblock78762
ConditionalTrueBlock1354
	; Binary clause: LESS
	lda level
	; Compare with pure num / var optimization
	cmp #$c;keep
	; BC done
	bcs binaryclausefailed61956
binaryclausesuccess32475
	lda #1; success
	jmp binaryclausefinished65981
binaryclausefailed61956
	lda #0 ; failed state
binaryclausefinished65981
	cmp #1
	beq ConditionalTrueBlock94949
	jmp elsedoneblock65155
ConditionalTrueBlock94949
	; Assigning single variable : i
	lda #0
	sta i
while68011
	; Binary clause: EQUALS
	lda i
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bne binaryclausefailed38957
binaryclausesuccess76125
	lda #1; success
	jmp binaryclausefinished25062
binaryclausefailed38957
	lda #0 ; failed state
binaryclausefinished25062
	cmp #1
	beq ConditionalTrueBlock88967
	jmp elsedoneblock90192
ConditionalTrueBlock88967
	; Assigning single variable : lx
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #19
val_var66797 = $54
	sta val_var66797
	jsr Random
	sec
modulo12496
	sbc val_var66797
	bcs modulo12496
	adc val_var66797
	
	clc
	adc #1
	 ; end add / sub var with constant
	
	sta lx
	; Assigning single variable : ly
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #18
val_var93418 = $54
	sta val_var93418
	jsr Random
	sec
modulo93141
	sbc val_var93418
	bcs modulo93141
	adc val_var93418
	
	clc
	adc #4
	 ; end add / sub var with constant
	
	sta ly
	; Assigning single variable : screenmemory
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	asl ; *2
	tax
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	
	sta screenmemory
	sty screenmemory+1
	; Binary clause Simplified: EQUALS
	; Load Byte array
	; Load pointer array
	ldy lx
	lda (screenmemory),y
	
	tax
	lda cMask,x
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock25393
ConditionalTrueBlock75474
	; Assigning single variable : i
	lda #1
	sta i
	; Assigning single variable : screenmemory
	lda #39
	ldy lx
	sta (screenmemory),y
	; Assigning single variable : p1
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda ly
	asl ; *2
	tax
	lda clr,x   ; Address of table lo
	ldy clr+1,x   ; Address of table hi
	
	sta p1
	sty p1+1
	; Assigning single variable : p1
	lda #7
	ldy lx
	sta (p1),y
elseblock96980
elsedoneblock25393
	jmp while68011
elseblock46912
elsedoneblock90192
elseblock99355
elsedoneblock65155
elseblock91761
elsedoneblock78762
elseblock71559
elsedoneblock52506
	; Binary clause Simplified: EQUALS
	lda moonPos
	; Compare with pure num / var optimization
	cmp #$a;keep
	bne elsedoneblock52160
ConditionalTrueBlock69559
	; Binary clause Simplified: NOTEQUALS
	lda lx
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock10431
ConditionalTrueBlock32557
	; Assigning single variable : screenmemory
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda ly
	asl ; *2
	tax
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	
	sta screenmemory
	sty screenmemory+1
	; Assigning single variable : lostchildlevel
	; 8 bit binop
	; Add/sub where right value is constant number
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #2
val_var63638 = $54
	sta val_var63638
	jsr Random
	sec
modulo38270
	sbc val_var63638
	bcs modulo38270
	adc val_var63638
	
	clc
	adc #1
	 ; end add / sub var with constant
	
	clc
	adc lostchildlevel
	 ; end add / sub var with constant
	
	sta lostchildlevel
	; Assigning single variable : screenmemory
	lda #0
	ldy lx
	sta (screenmemory),y
	; Assigning single variable : lx
	sta lx
	; Assigning single variable : ly
	sta ly
elseblock52493
elsedoneblock10431
elseblock6898
elsedoneblock52160
	rts
	
	
	; ***********  Defining procedure : Initialise
	;    Procedure type : User-defined procedure
	
Initialise
	lda #$7f
	sta $912e ; disable and acknowledge interrupts
	sta $912d
	; Binary clause Simplified: EQUALS
	lda $9000
	; Compare with pure num / var optimization
	cmp #$c;keep
	bne elseblock2671
ConditionalTrueBlock7829
	lda #<vbl
	sta pointers_vic_raster+1
	lda #>vbl
	sta pointers_vic_raster+6
	lda #134
	sta timers_vic_raster+1
	lda #86
	sta timers_vic_raster+3
	jsr init_via_irq
	; Assigning single variable : GAMETIMERMAX
	lda #25
	sta GAMETIMERMAX
	jmp elsedoneblock96180
elseblock2671
	lda #<vbl
	sta pointers_vic_raster+1
	lda #>vbl
	sta pointers_vic_raster+6
	lda #67
	sta timers_vic_raster+1
	lda #66
	sta timers_vic_raster+3
	jsr init_via_irq
	; Assigning single variable : GAMETIMERMAX
	lda #28
	sta GAMETIMERMAX
elsedoneblock96180
	; Assigning memory location
	; Assigning single variable : $900f
	lda #8
	sta $900f
screenmemory =  $fe
	lda $9005
	and #%00001111
	ora #%11000000
	sta $9005
	and #%11110000
	ora #%00001101
	sta $9005
	; ----------
	; DefineAddressTable address, StartValue, IncrementValue, TableSize
	ldy #>$1000
	lda #<$1000
	ldx #0
	sta scr,x   ; Address of table
	tya
	sta scr+1,x
dtloop43024
	tay
	lda scr,x
	inx
	inx
	clc
	adc #$16
	bcc dtnooverflow37088
	iny
dtnooverflow37088
	sta scr,x
	tya
	sta scr+1,x
	cpx #$2c
	bcc dtloop43024
	; ----------
	; DefineAddressTable address, StartValue, IncrementValue, TableSize
	ldy #>$9400
	lda #<$9400
	ldx #0
	sta clr,x   ; Address of table
	tya
	sta clr+1,x
dtloop49154
	tay
	lda clr,x
	inx
	inx
	clc
	adc #$16
	bcc dtnooverflow52386
	iny
dtnooverflow52386
	sta clr,x
	tya
	sta clr+1,x
	cpx #$2c
	bcc dtloop49154
	rts
	
	
	; ***********  Defining procedure : DisplayScore
	;    Procedure type : User-defined procedure
	
DisplayScore
	; Assigning single variable : screenmemory
	lda #0
	ldx #16
	sta screenmemory
	stx screenmemory+1
	; ----------
	; BcdPrint address, number
	ldy #$07 ; screen offset
	ldx #0 ; score byte index
bcdprintloop58232
	lda score,x
	inx
	pha
	and #$0f
	jsr bcdplotdigit
	pla
	lsr
	lsr
	lsr
	lsr
	jsr bcdplotdigit
	bpl bcdprintloop58232
	rts
	
	
	; ***********  Defining procedure : ColourTitleScreen
	;    Procedure type : User-defined procedure
	
ColourTitleScreen
	; Assigning single variable : screenmemory
	lda #0
	ldx #16
	sta screenmemory
	stx screenmemory+1
	; Assigning single variable : p1
	ldx #148
	sta p1
	stx p1+1
	; Assigning single variable : i
	sta i
for55710
	; Assigning single variable : j
	lda #0
	sta j
for22351
	; Assigning single variable : p1
	; Load Unknown type array
	; Load pointer array
	ldy j
	lda (screenmemory),y
	
	tax
	lda tcolordata,x
	
	sta (p1),y
	inc j
	lda #22
	cmp j ;keep
	bne for22351
forLoopDone57464
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd87330
	inc p1+1
WordAdd87330
	sta p1+0
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd12086
	inc screenmemory+1
WordAdd12086
	sta screenmemory+0
	inc i
	lda #23
	cmp i ;keep
	bne for55710
forLoopDone29649
	rts
	
	
	; ***********  Defining procedure : ColourCreditsScreen
	;    Procedure type : User-defined procedure
	
ColourCreditsScreen
	; Assigning single variable : screenmemory
	lda #0
	ldx #16
	sta screenmemory
	stx screenmemory+1
	; Assigning single variable : p1
	ldx #148
	sta p1
	stx p1+1
	; Assigning single variable : i
	sta i
for31349
	; Assigning single variable : j
	lda #0
	sta j
for6773
	; Assigning single variable : p1
	; Load Unknown type array
	; Load pointer array
	ldy j
	lda (screenmemory),y
	
	tax
	lda tcolorcrdata,x
	
	sta (p1),y
	inc j
	lda #22
	cmp j ;keep
	bne for6773
forLoopDone81519
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd95354
	inc p1+1
WordAdd95354
	sta p1+0
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd90244
	inc screenmemory+1
WordAdd90244
	sta screenmemory+0
	inc i
	lda #23
	cmp i ;keep
	bne for31349
forLoopDone12722
	rts
	
	
	; ***********  Defining procedure : ColourTitleScreen2
	;    Procedure type : User-defined procedure
	
ColourTitleScreen2
	; Assigning single variable : screenmemory
	lda #0
	ldx #16
	sta screenmemory
	stx screenmemory+1
	; Assigning single variable : p1
	ldx #148
	sta p1
	stx p1+1
	; Assigning single variable : i
	sta i
for10139
	; Assigning single variable : j
	lda #0
	sta j
for26799
	; Assigning single variable : p1
	; Load Unknown type array
	; Load pointer array
	ldy j
	lda (screenmemory),y
	
	tax
	lda colordata,x
	
	sta (p1),y
	inc j
	lda #22
	cmp j ;keep
	bne for26799
forLoopDone54819
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd40435
	inc p1+1
WordAdd40435
	sta p1+0
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd91087
	inc screenmemory+1
WordAdd91087
	sta screenmemory+0
	inc i
	lda #23
	cmp i ;keep
	bne for10139
forLoopDone5404
	rts
	
	
	; ***********  Defining procedure : ColourFailScreen
	;    Procedure type : User-defined procedure
	
ColourFailScreen
	; Assigning single variable : screenmemory
	lda #0
	ldx #16
	sta screenmemory
	stx screenmemory+1
	; Assigning single variable : p1
	ldx #148
	sta p1
	stx p1+1
	; Assigning single variable : i
	sta i
for669
	; Assigning single variable : j
	lda #0
	sta j
for66704
	; Assigning single variable : p1
	; Load Unknown type array
	; Load pointer array
	ldy j
	lda (screenmemory),y
	
	tax
	lda gamefailcolordata,x
	
	sta (p1),y
	inc j
	lda #22
	cmp j ;keep
	bne for66704
forLoopDone2557
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd23403
	inc p1+1
WordAdd23403
	sta p1+0
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd18816
	inc screenmemory+1
WordAdd18816
	sta screenmemory+0
	inc i
	lda #23
	cmp i ;keep
	bne for669
forLoopDone85205
	rts
	
	
	; ***********  Defining procedure : ColourGameOverScreen
	;    Procedure type : User-defined procedure
	
ColourGameOverScreen
	; Assigning single variable : screenmemory
	lda #0
	ldx #16
	sta screenmemory
	stx screenmemory+1
	; Assigning single variable : p1
	ldx #148
	sta p1
	stx p1+1
	; Assigning single variable : i
	sta i
for821
	; Assigning single variable : j
	lda #0
	sta j
for64045
	; Assigning single variable : p1
	; Load Unknown type array
	; Load pointer array
	ldy j
	lda (screenmemory),y
	
	tax
	lda gameovercolordata,x
	
	sta (p1),y
	inc j
	lda #22
	cmp j ;keep
	bne for64045
forLoopDone23787
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd75551
	inc p1+1
WordAdd75551
	sta p1+0
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd42589
	inc screenmemory+1
WordAdd42589
	sta screenmemory+0
	inc i
	lda #23
	cmp i ;keep
	bne for821
forLoopDone34447
	rts
	
	
	; ***********  Defining procedure : ColourGameWonScreen
	;    Procedure type : User-defined procedure
	
ColourGameWonScreen
	; Assigning single variable : screenmemory
	lda #0
	ldx #16
	sta screenmemory
	stx screenmemory+1
	; Assigning single variable : p1
	ldx #148
	sta p1
	stx p1+1
	; Assigning single variable : i
	sta i
for50353
	; Assigning single variable : j
	lda #0
	sta j
for15921
	; Assigning single variable : p1
	; Load Unknown type array
	; Load pointer array
	ldy j
	lda (screenmemory),y
	
	tax
	lda gamewoncolordata,x
	
	sta (p1),y
	inc j
	lda #22
	cmp j ;keep
	bne for15921
forLoopDone98480
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd5994
	inc p1+1
WordAdd5994
	sta p1+0
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd86354
	inc screenmemory+1
WordAdd86354
	sta screenmemory+0
	inc i
	lda #23
	cmp i ;keep
	bne for50353
forLoopDone65417
	rts
	
	
	; ***********  Defining procedure : ColourGameNextScreen
	;    Procedure type : User-defined procedure
	
ColourGameNextScreen
	; Assigning single variable : screenmemory
	lda #0
	ldx #16
	sta screenmemory
	stx screenmemory+1
	; Assigning single variable : p1
	ldx #148
	sta p1
	stx p1+1
	; Assigning single variable : i
	sta i
for8437
	; Assigning single variable : j
	lda #0
	sta j
for91608
	; Assigning single variable : p1
	; Load Unknown type array
	; Load pointer array
	ldy j
	lda (screenmemory),y
	
	tax
	lda gamenextcolordata,x
	
	sta (p1),y
	inc j
	lda #22
	cmp j ;keep
	bne for91608
forLoopDone1745
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd67422
	inc p1+1
WordAdd67422
	sta p1+0
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd1171
	inc screenmemory+1
WordAdd1171
	sta screenmemory+0
	inc i
	lda #23
	cmp i ;keep
	bne for8437
forLoopDone25165
	rts
	
	
	; ***********  Defining procedure : BlankScreen
	;    Procedure type : User-defined procedure
	
BlankScreen
	; Assigning single variable : p1
	lda #0
	ldx #148
	sta p1
	stx p1+1
	; Assigning single variable : i
	sta i
for48569
	; Assigning single variable : j
	lda #0
	sta j
for20172
	; Assigning single variable : p1
	lda #0
	ldy j
	sta (p1),y
	inc j
	lda #22
	cmp j ;keep
	bne for20172
forLoopDone74089
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd3566
	inc p1+1
WordAdd3566
	sta p1+0
	inc i
	lda #23
	cmp i ;keep
	bne for48569
forLoopDone58925
	rts
	
	
	; ***********  Defining procedure : ColourScreen
	;    Procedure type : User-defined procedure
	
ColourScreen
	; Assigning single variable : screenmemory
	lda #66
	ldx #16
	sta screenmemory
	stx screenmemory+1
	; Assigning single variable : p1
	ldx #148
	sta p1
	stx p1+1
	; Assigning single variable : i
	lda #0
	sta i
for55812
	; Assigning single variable : j
	lda #0
	sta j
for95568
	; Assigning single variable : p1
	; Load Unknown type array
	; Load pointer array
	ldy j
	lda (screenmemory),y
	
	tax
	lda colordata,x
	
	sta (p1),y
	inc j
	lda #22
	cmp j ;keep
	bne for95568
forLoopDone86141
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd82639
	inc p1+1
WordAdd82639
	sta p1+0
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd82437
	inc screenmemory+1
WordAdd82437
	sta screenmemory+0
	inc i
	lda #20
	cmp i ;keep
	bne for55812
forLoopDone63370
	rts
	
	
	; ***********  Defining procedure : ColourHeader
	;    Procedure type : User-defined procedure
	
ColourHeader
	; Assigning single variable : screenmemory
	lda #0
	ldx #148
	sta screenmemory
	stx screenmemory+1
	; Assigning single variable : i
	sta i
for63198
	; Assigning single variable : screenmemory
	lda #3
	ldy i
	sta (screenmemory),y
	inc i
	lda #22
	cmp i ;keep
	bne for63198
forLoopDone69939
	; Assigning single variable : i
	lda #22
	sta i
for26513
	; Assigning single variable : screenmemory
	lda #0
	ldy i
	sta (screenmemory),y
	inc i
	lda #66
	cmp i ;keep
	bne for26513
forLoopDone65128
	; Assigning single variable : screenmemory
	lda #8
	ldx #148
	sta screenmemory
	stx screenmemory+1
	; Assigning single variable : i
	lda #0
	sta i
for11804
	; Assigning single variable : screenmemory
	lda #2
	ldy i
	sta (screenmemory),y
	inc i
	lda #8
	cmp i ;keep
	bne for11804
forLoopDone52346
	rts
	
	
	; ***********  Defining procedure : FindStartPos
	;    Procedure type : User-defined procedure
	
FindStartPos
	; Assigning single variable : screenmemory
	
	; ----------
	; AddressTable address, xoffset, yoffset
	ldx #6
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	
	sta screenmemory
	sty screenmemory+1
	; Assigning single variable : y
	lda #3
	sta y
for90495
	; Assigning single variable : x
	lda #1
	sta x
for33292
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy x
	lda (screenmemory),y
	
	; Compare with pure num / var optimization
	cmp #$3a;keep
	bne elsedoneblock99492
ConditionalTrueBlock20083
	; Assigning single variable : px
	lda x
	sta px
	; Assigning single variable : py
	lda y
	sta py
elseblock14800
elsedoneblock99492
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy x
	lda (screenmemory),y
	
	; Compare with pure num / var optimization
	cmp #$20;keep
	bne elsedoneblock5735
ConditionalTrueBlock22939
	; Assigning single variable : wx
	lda x
	sta wx
	; Assigning single variable : wy
	lda y
	sta wy
elseblock12763
elsedoneblock5735
	inc x
	lda #21
	cmp x ;keep
	bne for33292
forLoopDone32540
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd2606
	inc screenmemory+1
WordAdd2606
	sta screenmemory+0
	; Assigning single variable : pbgchar
	lda #0
	sta pbgchar
	; Assigning single variable : pbgcol
	sta pbgcol
	; Assigning single variable : wbgchar
	sta wbgchar
	; Assigning single variable : wbgcol
	sta wbgcol
	inc y
	lda #23
	cmp y ;keep
	bne for90495
forLoopDone48860
	rts
	
	
	; ***********  Defining procedure : WitchPickDirection
	;    Procedure type : User-defined procedure
	
WitchPickDirection
	; Binary clause Simplified: NOTEQUALS
	; Modulo
	lda #3
val_var76505 = $54
	sta val_var76505
	jsr Random
	sec
modulo63179
	sbc val_var76505
	bcs modulo63179
	adc val_var76505
	
	; Compare with pure num / var optimization
	cmp #$1;keep
	beq elsedoneblock82751
ConditionalTrueBlock52251
	; Binary clause Simplified: EQUALS
	lda wy
	; Compare with pure num / var optimization
	cmp py;keep
	bne elsedoneblock71472
ConditionalTrueBlock68098
	; Binary clause Simplified: GREATER
	lda wx
	; Compare with pure num / var optimization
	cmp px;keep
	bcc elseblock89877
	beq elseblock89877
ConditionalTrueBlock65159
	; Assigning single variable : wd
	lda #3
	sta wd
	jmp elsedoneblock61900
elseblock89877
	; Assigning single variable : wd
	lda #1
	sta wd
elsedoneblock61900
	rts
elseblock94425
elsedoneblock71472
	; Binary clause Simplified: EQUALS
	lda wx
	; Compare with pure num / var optimization
	cmp px;keep
	bne elsedoneblock18107
ConditionalTrueBlock62035
	; Binary clause Simplified: GREATER
	lda wy
	; Compare with pure num / var optimization
	cmp py;keep
	bcc elseblock81763
	beq elseblock81763
ConditionalTrueBlock6797
	; Assigning single variable : wd
	lda #0
	sta wd
	jmp elsedoneblock63633
elseblock81763
	; Assigning single variable : wd
	lda #2
	sta wd
elsedoneblock63633
	rts
elseblock55951
elsedoneblock18107
elseblock64349
elsedoneblock82751
	; Assigning single variable : wd
	inc wd
	; Binary clause Simplified: GREATER
	lda wd
	; Compare with pure num / var optimization
	cmp #$3;keep
	bcc elsedoneblock32100
	beq elsedoneblock32100
ConditionalTrueBlock85185
	; Assigning single variable : wd
	lda #0
	sta wd
elseblock20897
elsedoneblock32100
	rts
	
	
	; ***********  Defining procedure : UpdateWitch
	;    Procedure type : User-defined procedure
	
UpdateWitch
	; Binary clause: EQUALS
	lda wsteps
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bne binaryclausefailed28826
binaryclausesuccess97565
	lda #1; success
	jmp binaryclausefinished6518
binaryclausefailed28826
	lda #0 ; failed state
binaryclausefinished6518
	cmp #1
	beq ConditionalTrueBlock91313
	jmp elsedoneblock52634
ConditionalTrueBlock91313
	; Binary clause Simplified: EQUALS
	lda wdelay
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock16908
ConditionalTrueBlock87134
	; Assigning single variable : wdelay
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #40
val_var93842 = $54
	sta val_var93842
	jsr Random
	sec
modulo48432
	sbc val_var93842
	bcs modulo48432
	adc val_var93842
	
	clc
	adc #15
	 ; end add / sub var with constant
	
	sta wdelay
	; Assigning single variable : psndfx
	lda #<sndSWitch
	ldx #>sndSWitch
	sta psndfx
	stx psndfx+1
	; Assigning single variable : vsndfx
	lda #254
	sta vsndfx
	jsr PlaySndfx
elseblock51546
elsedoneblock16908
	; Assigning single variable : wdelay
	dec wdelay
	; Binary clause Simplified: GREATER
	lda wdelay
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock18834
	beq elsedoneblock18834
ConditionalTrueBlock39807
	rts
elseblock49810
elsedoneblock18834
	jsr WitchPickDirection
	; Assigning single variable : wsteps
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #10
val_var95815 = $54
	sta val_var95815
	jsr Random
	sec
modulo27984
	sbc val_var95815
	bcs modulo27984
	adc val_var95815
	
	clc
	adc #2
	 ; end add / sub var with constant
	
	sta wsteps
	; Binary clause Simplified: GREATER
	lda wdcount
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock94465
	beq elsedoneblock94465
ConditionalTrueBlock52324
	; Assigning single variable : wdcount
	dec wdcount
	; Binary clause Simplified: EQUALS
	lda wdcount
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock73879
ConditionalTrueBlock5360
	; Assigning single variable : wdspeed
	; Load Byte array
	ldx level
	lda lwitch,x
	
	sta wdspeed
elseblock64088
elsedoneblock73879
elseblock11382
elsedoneblock94465
elseblock95879
elsedoneblock52634
	; Binary clause Simplified: NOTEQUALS
	; Modulo
	lda wdspeed
val_var83162 = $54
	sta val_var83162
	lda ticks
	sec
modulo56942
	sbc val_var83162
	bcs modulo56942
	adc val_var83162
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock17135
ConditionalTrueBlock22924
	rts
elseblock86628
elsedoneblock17135
	; Assigning single variable : wsteps
	dec wsteps
	lda wd
	cmp #$0 ;keep
	bne casenext54163
	; Assigning single variable : screenmemory
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	; 8 bit binop
	; Add/sub where right value is constant number
	lda wy
	sec
	sbc #1
	 ; end add / sub var with constant
	asl ; *2
	tax
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	
	sta screenmemory
	sty screenmemory+1
	; Assigning single variable : p1
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	; 8 bit binop
	; Add/sub where right value is constant number
	lda wy
	sec
	sbc #1
	 ; end add / sub var with constant
	asl ; *2
	tax
	lda clr,x   ; Address of table lo
	ldy clr+1,x   ; Address of table hi
	
	sta p1
	sty p1+1
	; Assigning single variable : i
	lda #32
	sta i
	; Binary clause Simplified: LESS
	; Load Byte array
	; Load pointer array
	ldy wx
	lda (screenmemory),y
	
	tax
	lda cMask,x
	
	; Compare with pure num / var optimization
	cmp #$5;keep
	bcs elseblock26234
ConditionalTrueBlock66487
	; Assigning single variable : screenmemory
	lda wbgchar
	pha
	; 8 bit binop
	; Add/sub where right value is constant number
	lda wx
	clc
	adc #22
	 ; end add / sub var with constant
	
	tay
	pla
	sta (screenmemory),y
	; Assigning single variable : p1
	lda wbgcol
	pha
	; 8 bit binop
	; Add/sub where right value is constant number
	lda wx
	clc
	adc #22
	 ; end add / sub var with constant
	
	tay
	pla
	sta (p1),y
	; Assigning single variable : wy
	dec wy
	; Assigning single variable : wbgchar
	; Load pointer array
	ldy wx
	lda (screenmemory),y
	
	sta wbgchar
	; Assigning single variable : wbgcol
	; Load pointer array
	lda (p1),y
	
	sta wbgcol
	jmp elsedoneblock83332
elseblock26234
	; Assigning single variable : wsteps
	lda #0
	sta wsteps
elsedoneblock83332
	jmp caseend1996
casenext54163
	lda wd
	cmp #$1 ;keep
	bne casenext51633
	; Assigning single variable : screenmemory
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda wy
	asl ; *2
	tax
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	
	sta screenmemory
	sty screenmemory+1
	; Assigning single variable : p1
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda wy
	asl ; *2
	tax
	lda clr,x   ; Address of table lo
	ldy clr+1,x   ; Address of table hi
	
	sta p1
	sty p1+1
	; Assigning single variable : i
	lda #33
	
	sta i
	; Binary clause Simplified: LESS
	; Load Byte array
	; Load pointer array
	; LDA stuff
	; 8 bit binop
	; Add/sub where right value is constant number
	lda wx
	clc
	adc #1
	 ; end add / sub var with constant
	
	tay
	lda (screenmemory),y
	
	tax
	lda cMask,x
	
	; Compare with pure num / var optimization
	cmp #$5;keep
	bcs elseblock32854
ConditionalTrueBlock85213
	; Assigning single variable : screenmemory
	lda wbgchar
	ldy wx
	sta (screenmemory),y
	; Assigning single variable : p1
	lda wbgcol
	sta (p1),y
	; Assigning single variable : wx
	inc wx
	; Assigning single variable : wbgchar
	; Load pointer array
	ldy wx
	lda (screenmemory),y
	
	sta wbgchar
	; Assigning single variable : wbgcol
	; Load pointer array
	lda (p1),y
	
	sta wbgcol
	jmp elsedoneblock66382
elseblock32854
	; Assigning single variable : wsteps
	lda #0
	sta wsteps
elsedoneblock66382
	jmp caseend1996
casenext51633
	lda wd
	cmp #$2 ;keep
	bne casenext98738
	; Assigning single variable : screenmemory
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda wy
	asl ; *2
	tax
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	
	sta screenmemory
	sty screenmemory+1
	; Assigning single variable : p1
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda wy
	asl ; *2
	tax
	lda clr,x   ; Address of table lo
	ldy clr+1,x   ; Address of table hi
	
	sta p1
	sty p1+1
	; Assigning single variable : i
	lda #32
	sta i
	; Binary clause Simplified: LESS
	; Load Byte array
	; Load pointer array
	; LDA stuff
	; 8 bit binop
	; Add/sub where right value is constant number
	lda wx
	clc
	adc #22
	 ; end add / sub var with constant
	
	tay
	lda (screenmemory),y
	
	tax
	lda cMask,x
	
	; Compare with pure num / var optimization
	cmp #$5;keep
	bcs elseblock89168
ConditionalTrueBlock16465
	; Assigning single variable : screenmemory
	lda wbgchar
	ldy wx
	sta (screenmemory),y
	; Assigning single variable : p1
	lda wbgcol
	sta (p1),y
	; Assigning single variable : wy
	inc wy
	; Assigning single variable : wbgchar
	; Load pointer array
	; LDA stuff
	; 8 bit binop
	; Add/sub where right value is constant number
	lda wx
	clc
	adc #22
	 ; end add / sub var with constant
	
	tay
	lda (screenmemory),y
	
	sta wbgchar
	; Assigning single variable : wbgcol
	; Load pointer array
	; LDA stuff
	; 8 bit binop
	; Add/sub where right value is constant number
	lda wx
	clc
	adc #22
	 ; end add / sub var with constant
	
	tay
	lda (p1),y
	
	sta wbgcol
	jmp elsedoneblock68681
elseblock89168
	; Assigning single variable : wsteps
	lda #0
	sta wsteps
elsedoneblock68681
	jmp caseend1996
casenext98738
	lda wd
	cmp #$3 ;keep
	bne casenext48977
	; Assigning single variable : screenmemory
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda wy
	asl ; *2
	tax
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	; xoffset is complex
	sta $4c
	; 8 bit binop
	; Add/sub where right value is constant number
	lda wx
	sec
	sbc #1
	 ; end add / sub var with constant
	clc
	adc $4c
	bcc dtnooverflow65014
	iny  ; overflow into high byte
dtnooverflow65014
	
	sta screenmemory
	sty screenmemory+1
	; Assigning single variable : p1
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda wy
	asl ; *2
	tax
	lda clr,x   ; Address of table lo
	ldy clr+1,x   ; Address of table hi
	; xoffset is complex
	sta $4c
	; 8 bit binop
	; Add/sub where right value is constant number
	lda wx
	sec
	sbc #1
	 ; end add / sub var with constant
	clc
	adc $4c
	bcc dtnooverflow15464
	iny  ; overflow into high byte
dtnooverflow15464
	
	sta p1
	sty p1+1
	; Assigning single variable : i
	lda #32
	sta i
	; Binary clause Simplified: LESS
	; Load Byte array
	; Load pointer array
	ldy #$0
	lda (screenmemory),y
	
	tax
	lda cMask,x
	
	; Compare with pure num / var optimization
	cmp #$5;keep
	bcs elseblock14107
ConditionalTrueBlock48346
	; Assigning single variable : screenmemory
	lda wbgchar
	ldy #1
	sta (screenmemory),y
	; Assigning single variable : p1
	lda wbgcol
	sta (p1),y
	; Assigning single variable : wx
	dec wx
	; Assigning single variable : wbgchar
	; Load pointer array
	ldy #$0
	lda (screenmemory),y
	
	sta wbgchar
	; Assigning single variable : wbgcol
	; Load pointer array
	lda (p1),y
	
	sta wbgcol
	jmp elsedoneblock63072
elseblock14107
	; Assigning single variable : wsteps
	lda #0
	sta wsteps
elsedoneblock63072
	jmp caseend1996
casenext48977
caseend1996
	; Assigning single variable : screenmemory
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda wy
	asl ; *2
	tax
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	
	sta screenmemory
	sty screenmemory+1
	; Assigning single variable : p1
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda wy
	asl ; *2
	tax
	lda clr,x   ; Address of table lo
	ldy clr+1,x   ; Address of table hi
	
	sta p1
	sty p1+1
	; Assigning single variable : screenmemory
	lda i
	ldy wx
	sta (screenmemory),y
	; Assigning single variable : p1
	lda #5
	sta (p1),y
	; Binary clause Simplified: EQUALS
	lda cx
	; Compare with pure num / var optimization
	cmp wx;keep
	bne elsedoneblock73439
ConditionalTrueBlock37836
	; Binary clause Simplified: EQUALS
	lda cy
	; Compare with pure num / var optimization
	cmp wy;keep
	bne elsedoneblock4697
ConditionalTrueBlock49764
	jsr HitWitch
elseblock73567
elsedoneblock4697
elseblock58793
elsedoneblock73439
	rts
	
	
	; ***********  Defining procedure : Ghost_Respawn
	;    Procedure type : User-defined procedure
	
Ghost_Respawn
	; Binary clause Simplified: GREATEREQUAL
	lda gcurrent
	; Compare with pure num / var optimization
	cmp gNumGhosts;keep
	bcc elsedoneblock17457
ConditionalTrueBlock47433
	; Assigning single variable : gmode
	lda #0
	ldx gcurrent
	sta gmode,x
	; Assigning single variable : gx
	lda #1
	sta gx,x
	; Assigning single variable : gy
	sta gy,x
	rts
elseblock85490
elsedoneblock17457
	; Assigning single variable : j
	lda #0
	sta j
while80529
	; Binary clause: EQUALS
	lda j
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bne binaryclausefailed65169
binaryclausesuccess47227
	lda #1; success
	jmp binaryclausefinished19170
binaryclausefailed65169
	lda #0 ; failed state
binaryclausefinished19170
	cmp #1
	beq ConditionalTrueBlock85523
	jmp elsedoneblock85292
ConditionalTrueBlock85523
	; Assigning single variable : x
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #20
val_var11520 = $54
	sta val_var11520
	jsr Random
	sec
modulo39563
	sbc val_var11520
	bcs modulo39563
	adc val_var11520
	
	clc
	adc #1
	 ; end add / sub var with constant
	
	sta x
	; Assigning single variable : y
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #18
val_var39221 = $54
	sta val_var39221
	jsr Random
	sec
modulo93660
	sbc val_var39221
	bcs modulo93660
	adc val_var39221
	
	clc
	adc #4
	 ; end add / sub var with constant
	
	sta y
	; Assigning single variable : screenmemory
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	asl ; *2
	tax
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	
	sta screenmemory
	sty screenmemory+1
	; Assigning single variable : p1
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	; 8 bit binop
	; Add/sub where right value is constant number
	lda y
	sec
	sbc #1
	 ; end add / sub var with constant
	asl ; *2
	tax
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	; xoffset is complex
	sta $4c
	; 8 bit binop
	; Add/sub where right value is constant number
	lda x
	sec
	sbc #1
	 ; end add / sub var with constant
	clc
	adc $4c
	bcc dtnooverflow68616
	iny  ; overflow into high byte
dtnooverflow68616
	
	sta p1
	sty p1+1
	; Assigning single variable : n
	; Load Byte array
	; Load pointer array
	ldy #$1
	lda (p1),y
	
	tax
	lda cMask,x
	
	sta n
	; Assigning single variable : w
	; Load Byte array
	; Load pointer array
	ldy #$16
	lda (p1),y
	
	tax
	lda cMask,x
	
	sta w
	; Assigning single variable : e
	; Load Byte array
	; Load pointer array
	ldy #$18
	lda (p1),y
	
	tax
	lda cMask,x
	
	sta e
	; Assigning single variable : s
	; Load Byte array
	; Load pointer array
	ldy #$2d
	lda (p1),y
	
	tax
	lda cMask,x
	
	sta s
	; Assigning single variable : i
	; 8 bit binop
	; Add/sub where right value is constant number
	; 8 bit binop
	; Add/sub where right value is constant number
	; 8 bit binop
	; Add/sub where right value is constant number
	; 8 bit binop
	; Add/sub where right value is constant number
	; Load Byte array
	; Load pointer array
	ldy #$17
	lda (p1),y
	
	tax
	lda cMask,x
	
	clc
	adc n
	 ; end add / sub var with constant
	
	clc
	adc s
	 ; end add / sub var with constant
	
	clc
	adc e
	 ; end add / sub var with constant
	
	clc
	adc w
	 ; end add / sub var with constant
	
	sta i
	; Binary clause Simplified: EQUALS
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock84292
ConditionalTrueBlock28223
	; Assigning single variable : p1
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda y
	asl ; *2
	tax
	lda clr,x   ; Address of table lo
	ldy clr+1,x   ; Address of table hi
	
	sta p1
	sty p1+1
	; Assigning single variable : j
	lda #1
	sta j
	; Assigning single variable : gx
	lda x
	ldx gcurrent
	sta gx,x
	; Assigning single variable : gy
	lda y
	sta gy,x
	; Assigning single variable : gbgchar
	; Load pointer array
	ldy x
	lda (screenmemory),y
	
	sta gbgchar,x
	; Assigning single variable : gbgcol
	; Load pointer array
	lda (p1),y
	
	sta gbgcol,x
	; Binary clause Simplified: LESS
	lda gcurrent
	; Compare with pure num / var optimization
	cmp gNumGhosts;keep
	bcs elseblock62173
ConditionalTrueBlock41009
	; Assigning single variable : gmode
	lda #0
	ldx gcurrent
	sta gmode,x
	; Assigning single variable : screenmemory
	lda #36
	ldy x
	sta (screenmemory),y
	; Assigning single variable : p1
	lda #1
	sta (p1),y
	jmp elsedoneblock89503
elseblock62173
	; Assigning single variable : gmode
	lda #1
	ldx gcurrent
	sta gmode,x
	; Assigning single variable : gx
	lda #0
	sta gx,x
elsedoneblock89503
elseblock55382
elsedoneblock84292
	jmp while80529
elseblock81690
elsedoneblock85292
	rts
	
	
	; ***********  Defining procedure : PlaceAllGhosts
	;    Procedure type : User-defined procedure
	
PlaceAllGhosts
	; Assigning single variable : gcurrent
	lda #0
	sta gcurrent
for18481
	jsr Ghost_Respawn
	; Binary clause Simplified: LESS
	lda gcurrent
	; Compare with pure num / var optimization
	cmp gNumGhosts;keep
	bcs elsedoneblock79117
ConditionalTrueBlock72525
	; Assigning single variable : psnd3
	lda #<sndGhostAppear
	ldx #>sndGhostAppear
	sta psnd3
	stx psnd3+1
	; Assigning single variable : vsnd3
	lda #200
	sta vsnd3
	jsr PlaySnd3
	; Assigning single variable : i
	lda #0
	sta i
for39833
while23446
	; Binary clause Simplified: EQUALS
	lda ticks
	; Compare with pure num / var optimization
	cmp oldTick;keep
	bne elsedoneblock13605
ConditionalTrueBlock85374
	jmp while23446
elseblock27458
elsedoneblock13605
	; Assigning single variable : oldTick
	lda ticks
	sta oldTick
	inc i
	lda #30
	cmp i ;keep
	bne for39833
forLoopDone86128
elseblock82894
elsedoneblock79117
	inc gcurrent
	lda #10
	cmp gcurrent ;keep
	bne for18481
forLoopDone3807
	; Assigning single variable : gcurrent
	lda #0
	sta gcurrent
	rts
	
	
	; ***********  Defining procedure : DisplayGhosts
	;    Procedure type : User-defined procedure
	
DisplayGhosts
	; Assigning single variable : i
	lda #0
	sta i
for38989
forLoopFix42348
	; Binary clause Simplified: GREATER
	; Load Byte array
	ldx i
	lda gy,x
	
	; Compare with pure num / var optimization
	cmp #$3;keep
	bcc elsedoneblock32538
	beq elsedoneblock32538
ConditionalTrueBlock72497
	; Assigning single variable : screenmemory
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	; Load Byte array
	ldx i
	lda gy,x
	
	asl ; *2
	tax
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	
	sta screenmemory
	sty screenmemory+1
	; Assigning single variable : p1
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	; Load Byte array
	ldx i
	lda gy,x
	
	asl ; *2
	tax
	lda clr,x   ; Address of table lo
	ldy clr+1,x   ; Address of table hi
	
	sta p1
	sty p1+1
	; Assigning single variable : screenmemory
	; 8 bit binop
	; Add/sub where right value is constant number
	; 8 bit binop
	; Add/sub where right value is constant number
	; 8 bit binop
	; Add/sub where right value is constant number
	lda gcurrent
	clc
	adc i
	 ; end add / sub var with constant
	
	lsr
	
	and #1
	 ; end add / sub var with constant
	
	clc
	adc #36
	 ; end add / sub var with constant
	
	; Load Byte array
	ldx i
	ldy gx,x
	
	sta (screenmemory),y
	; Assigning single variable : p1
	lda #1
	; Load Byte array
	ldy gx,x
	
	sta (p1),y
elseblock80569
elsedoneblock32538
	inc i
	lda gNumGhosts
	cmp i ;keep
	beq forLoopDone43461
forLoopNotDone30017
	jmp for38989
forLoopDone43461
	rts
	
	
	; ***********  Defining procedure : Ghost_Move
	;    Procedure type : User-defined procedure
	
Ghost_Move
	; Assigning single variable : x
	; Load Byte array
	ldx gcurrent
	lda gx,x
	
	sta x
	; Assigning single variable : y
	; Load Byte array
	lda gy,x
	
	sta y
	; Assigning single variable : n
	lda x
	sta n
	; Assigning single variable : s
	lda y
	sta s
	; Assigning single variable : screenmemory
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	asl ; *2
	tax
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	
	sta screenmemory
	sty screenmemory+1
	; Assigning single variable : p1
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda y
	asl ; *2
	tax
	lda clr,x   ; Address of table lo
	ldy clr+1,x   ; Address of table hi
	
	sta p1
	sty p1+1
	; Assigning single variable : screenmemory
	; Load Byte array
	ldx gcurrent
	lda gbgchar,x
	
	ldy x
	sta (screenmemory),y
	; Assigning single variable : p1
	; Load Byte array
	lda gbgcol,x
	
	sta (p1),y
	; Binary clause: NOTEQUALS
	; ----------
	; IsOverlapping collision  =  x1,y1, x2,y2, width, height
	lda x
	clc
	sbc px
	cmp #$f7
	bcs ColXConfirmed40353
	cmp #$8
	bcs NoCollision49299
ColXConfirmed40353
	lda y
	clc
	sbc py
	cmp #$fa
	bcs Collision92724
	cmp #$5
	bcs NoCollision49299
Collision92724
	lda #1
	jmp CollisionDone754
NoCollision49299
	lda #0
CollisionDone754
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	beq binaryclausefailed46350
binaryclausesuccess37710
	lda #1; success
	jmp binaryclausefinished13236
binaryclausefailed46350
	lda #0 ; failed state
binaryclausefinished13236
	cmp #1
	beq ConditionalTrueBlock41707
	jmp elseblock75743
	jmp elsedoneblock34400
ConditionalTrueBlock41707
	; Binary clause Simplified: EQUALS
	; Load Byte array
	ldx gcurrent
	lda gmode,x
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock86023
ConditionalTrueBlock6158
	; Binary clause Simplified: GREATER
	lda x
	; Compare with pure num / var optimization
	cmp px;keep
	bcc elsedoneblock65375
	beq elsedoneblock65375
ConditionalTrueBlock18017
	; Assigning single variable : x
	dec x
elseblock57806
elsedoneblock65375
	; Binary clause Simplified: LESS
	lda x
	; Compare with pure num / var optimization
	cmp px;keep
	bcs elsedoneblock77377
ConditionalTrueBlock8978
	; Assigning single variable : x
	inc x
elseblock15265
elsedoneblock77377
elseblock35164
elsedoneblock86023
	; Assigning single variable : p1
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda y
	asl ; *2
	tax
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	clc
	adc x
	bcc dtnooverflow17669
	iny  ; overflow into high byte
dtnooverflow17669
	
	sta p1
	sty p1+1
	; Binary clause Simplified: GREATER
	; Load Byte array
	; Load pointer array
	ldy #$0
	lda (p1),y
	
	tax
	lda cMask,x
	
	; Compare with pure num / var optimization
	cmp #$1;keep
	bcc elsedoneblock94536
	beq elsedoneblock94536
ConditionalTrueBlock59134
	; Assigning single variable : x
	lda n
	sta x
elseblock52833
elsedoneblock94536
	; Binary clause Simplified: EQUALS
	; Load Byte array
	ldx gcurrent
	lda gmode,x
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock89260
ConditionalTrueBlock67697
	; Binary clause Simplified: GREATER
	lda y
	; Compare with pure num / var optimization
	cmp py;keep
	bcc elsedoneblock87096
	beq elsedoneblock87096
ConditionalTrueBlock95367
	; Assigning single variable : y
	dec y
elseblock50212
elsedoneblock87096
	; Binary clause Simplified: LESS
	lda y
	; Compare with pure num / var optimization
	cmp py;keep
	bcs elsedoneblock73950
ConditionalTrueBlock26095
	; Assigning single variable : y
	inc y
elseblock78674
elsedoneblock73950
elseblock83316
elsedoneblock89260
	; Assigning single variable : p1
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda y
	asl ; *2
	tax
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	clc
	adc x
	bcc dtnooverflow27208
	iny  ; overflow into high byte
dtnooverflow27208
	
	sta p1
	sty p1+1
	; Binary clause Simplified: GREATER
	; Load Byte array
	; Load pointer array
	ldy #$0
	lda (p1),y
	
	tax
	lda cMask,x
	
	; Compare with pure num / var optimization
	cmp #$1;keep
	bcc elsedoneblock99242
	beq elsedoneblock99242
ConditionalTrueBlock12851
	; Assigning single variable : y
	lda s
	sta y
elseblock10525
elsedoneblock99242
	jmp elsedoneblock34400
elseblock75743
	; Assigning single variable : i
	jsr Random
	
	sta i
	; Binary clause Simplified: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	and #1
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elseblock84169
ConditionalTrueBlock27934
	; Assigning single variable : x
	inc x
	jmp elsedoneblock84795
elseblock84169
	; Assigning single variable : x
	dec x
elsedoneblock84795
	; Assigning single variable : p1
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda y
	asl ; *2
	tax
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	clc
	adc x
	bcc dtnooverflow27351
	iny  ; overflow into high byte
dtnooverflow27351
	
	sta p1
	sty p1+1
	; Binary clause Simplified: GREATER
	; Load Byte array
	; Load pointer array
	ldy #$0
	lda (p1),y
	
	tax
	lda cMask,x
	
	; Compare with pure num / var optimization
	cmp #$1;keep
	bcc elsedoneblock85900
	beq elsedoneblock85900
ConditionalTrueBlock41320
	; Assigning single variable : x
	lda n
	sta x
elseblock62297
elsedoneblock85900
	; Binary clause Simplified: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda i
	and #2
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elseblock48401
ConditionalTrueBlock86974
	; Assigning single variable : y
	dec y
	jmp elsedoneblock94114
elseblock48401
	; Assigning single variable : y
	inc y
elsedoneblock94114
	; Assigning single variable : p1
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda y
	asl ; *2
	tax
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	clc
	adc x
	bcc dtnooverflow50052
	iny  ; overflow into high byte
dtnooverflow50052
	
	sta p1
	sty p1+1
	; Binary clause Simplified: GREATER
	; Load Byte array
	; Load pointer array
	ldy #$0
	lda (p1),y
	
	tax
	lda cMask,x
	
	; Compare with pure num / var optimization
	cmp #$1;keep
	bcc elsedoneblock1433
	beq elsedoneblock1433
ConditionalTrueBlock11625
	; Assigning single variable : y
	lda s
	sta y
elseblock43682
elsedoneblock1433
elsedoneblock34400
	; Assigning single variable : gx
	lda x
	ldx gcurrent
	sta gx,x
	; Assigning single variable : gy
	lda y
	sta gy,x
	; Assigning single variable : screenmemory
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	asl ; *2
	tax
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	
	sta screenmemory
	sty screenmemory+1
	; Assigning single variable : p1
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda y
	asl ; *2
	tax
	lda clr,x   ; Address of table lo
	ldy clr+1,x   ; Address of table hi
	
	sta p1
	sty p1+1
	; Assigning single variable : gbgchar
	; Load pointer array
	ldy x
	lda (screenmemory),y
	
	ldx gcurrent
	sta gbgchar,x
	; Assigning single variable : gbgcol
	; Load pointer array
	lda (p1),y
	
	sta gbgcol,x
	; Binary clause Simplified: EQUALS
	lda cx
binary_clause_temp_var80589 = $54
	sta binary_clause_temp_var80589
	; Load Byte array
	ldx gcurrent
	lda gx,x
	
binary_clause_temp_2_var39013 = $56
	sta binary_clause_temp_2_var39013
	lda binary_clause_temp_var80589
	cmp binary_clause_temp_2_var39013;keep
	bne elsedoneblock30960
ConditionalTrueBlock30151
	; Binary clause Simplified: EQUALS
	lda cy
binary_clause_temp_var74759 = $54
	sta binary_clause_temp_var74759
	; Load Byte array
	ldx gcurrent
	lda gy,x
	
binary_clause_temp_2_var47036 = $56
	sta binary_clause_temp_2_var47036
	lda binary_clause_temp_var74759
	cmp binary_clause_temp_2_var47036;keep
	bne elsedoneblock96984
ConditionalTrueBlock3766
	; Assigning single variable : i
	lda gcurrent
	sta i
	jsr HitGhost
elseblock73990
elsedoneblock96984
elseblock97235
elsedoneblock30960
	rts
	
	
	; ***********  Defining procedure : UpdateGhosts
	;    Procedure type : User-defined procedure
	
UpdateGhosts
	; Assigning single variable : gtime
	inc gtime
	; Binary clause: GREATER
	lda gtime
	; Compare with pure num / var optimization
	cmp #$14;keep
	; BC done
	bcc binaryclausefailed36335
	beq binaryclausefailed36335
binaryclausesuccess92631
	lda #1; success
	jmp binaryclausefinished6892
binaryclausefailed36335
	lda #0 ; failed state
binaryclausefinished6892
	cmp #1
	beq ConditionalTrueBlock37209
	jmp elsedoneblock82700
ConditionalTrueBlock37209
	; Assigning single variable : gtime
	lda #0
	sta gtime
	; Binary clause Simplified: LESS
	lda gcurrent
	; Compare with pure num / var optimization
	cmp gNumGhosts;keep
	bcs elsedoneblock82313
ConditionalTrueBlock33007
	; Binary clause Simplified: EQUALS
	; Load Byte array
	ldx gcurrent
	lda gmode,x
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock70210
ConditionalTrueBlock62251
	jsr Ghost_Move
elseblock77724
elsedoneblock70210
	; Binary clause Simplified: GREATER
	; Load Byte array
	ldx gcurrent
	lda gmode,x
	
	; Compare with pure num / var optimization
	cmp #$1;keep
	bcc elsedoneblock90626
	beq elsedoneblock90626
ConditionalTrueBlock3193
	; Assigning single variable : gmode
	ldx gcurrent
	; Optimize byte array dec 
	dec gmode,x
	; Binary clause Simplified: EQUALS
	; Load Byte array
	lda gmode,x
	
	; Compare with pure num / var optimization
	cmp #$1;keep
	bne elsedoneblock72784
ConditionalTrueBlock62795
	jsr Ghost_Respawn
	; Assigning single variable : psnd3
	lda #<sndGhostAppear
	ldx #>sndGhostAppear
	sta psnd3
	stx psnd3+1
	; Assigning single variable : vsnd3
	lda #200
	sta vsnd3
	jsr PlaySnd3
elseblock99826
elsedoneblock72784
elseblock25834
elsedoneblock90626
elseblock19353
elsedoneblock82313
	jsr DisplayGhosts
	; Assigning single variable : gcurrent
	inc gcurrent
	; Binary clause Simplified: EQUALS
	lda gcurrent
	; Compare with pure num / var optimization
	cmp #$a;keep
	bne elsedoneblock573
ConditionalTrueBlock33882
	; Assigning single variable : gcurrent
	lda #0
	sta gcurrent
elseblock78155
elsedoneblock573
elseblock41238
elsedoneblock82700
	rts
	
	
	; ***********  Defining procedure : SpawnAZombie
	;    Procedure type : User-defined procedure
	
SpawnAZombie
	; Binary clause Simplified: GREATEREQUAL
	lda zNumZombies
	; Compare with pure num / var optimization
	cmp #$a;keep
	bcc elsedoneblock99067
ConditionalTrueBlock97946
	rts
elseblock39366
elsedoneblock99067
	; Assigning single variable : n
	lda #0
	sta n
	; Assigning single variable : s
	sta s
	; Assigning single variable : i
	sta i
	; Assigning single variable : t
	; Modulo
	lda #12
val_var73790 = $54
	sta val_var73790
	jsr Random
	sec
modulo78408
	sbc val_var73790
	bcs modulo78408
	adc val_var73790
	
	sta t
	; Assigning single variable : y
	lda #3
	sta y
for77290
	; Assigning single variable : x
	lda #1
	sta x
for43294
	; Assigning single variable : screenmemory
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda y
	asl ; *2
	tax
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	
	sta screenmemory
	sty screenmemory+1
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy x
	lda (screenmemory),y
	
	; Compare with pure num / var optimization
	cmp #$1b;keep
	bne elsedoneblock46286
ConditionalTrueBlock83852
	; Assigning single variable : n
	lda x
	sta n
	; Assigning single variable : s
	lda y
	sta s
	; Binary clause Simplified: EQUALS
	lda i
	; Compare with pure num / var optimization
	cmp t;keep
	bne elsedoneblock74729
ConditionalTrueBlock84200
	; Assigning single variable : x
	lda #20
	sta x
	; Assigning single variable : y
	lda #21
	sta y
elseblock80878
elsedoneblock74729
	; Assigning single variable : i
	inc i
elseblock97508
elsedoneblock46286
	inc x
	lda #21
	cmp x ;keep
	bne for43294
forLoopDone43370
	inc y
	lda #22
	cmp y ;keep
	bne for77290
forLoopDone80297
	; Binary clause Simplified: GREATER
	lda n
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock42772
	beq elsedoneblock42772
ConditionalTrueBlock33179
	; Assigning single variable : zx
	lda n
	ldx zNumZombies
	sta zx,x
	; Assigning single variable : zy
	lda s
	sta zy,x
	; Assigning single variable : zbgchar
	lda #0
	sta zbgchar,x
	; Assigning single variable : zbgcol
	sta zbgcol,x
	; Assigning single variable : zspawn
	lda #5
	sta zspawn,x
	; Assigning single variable : zNumZombies
	inc zNumZombies
	; Assigning single variable : psnd2
	lda #<sndGrave
	ldx #>sndGrave
	sta psnd2
	stx psnd2+1
	; Assigning single variable : vsnd2
	lda #128
	sta vsnd2
	jsr PlaySnd2
elseblock46007
elsedoneblock42772
	rts
	
	
	; ***********  Defining procedure : DisplayZombies
	;    Procedure type : User-defined procedure
	
DisplayZombies
	; Assigning single variable : i
	lda #0
	sta i
for5904
forLoopFix54833
	; Assigning single variable : screenmemory
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	; Load Byte array
	ldx i
	lda zy,x
	
	asl ; *2
	tax
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	
	sta screenmemory
	sty screenmemory+1
	; Assigning single variable : p1
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	; Load Byte array
	ldx i
	lda zy,x
	
	asl ; *2
	tax
	lda clr,x   ; Address of table lo
	ldy clr+1,x   ; Address of table hi
	
	sta p1
	sty p1+1
	; Assigning single variable : t
	; Load Byte array
	ldx i
	lda zspawn,x
	
	sta t
	; Assigning single variable : x
	; Load Byte array
	lda zx,x
	
	sta x
	lda t
	cmp #$0 ;keep
	bne casenext53150
	; Assigning single variable : screenmemory
	; 8 bit binop
	; Add/sub where right value is constant number
	; 8 bit binop
	; Add/sub where right value is constant number
	jsr Random
	
	and #1
	 ; end add / sub var with constant
	
	clc
	adc #34
	 ; end add / sub var with constant
	
	ldy x
	sta (screenmemory),y
	jmp caseend82271
casenext53150
	lda t
	cmp #$1 ;keep
	bne casenext23381
	; Assigning single variable : screenmemory
	lda #40
	ldy x
	sta (screenmemory),y
	jmp caseend82271
casenext23381
	lda t
	cmp #$2 ;keep
	bne casenext38287
	; Assigning single variable : screenmemory
	lda #41
	ldy x
	sta (screenmemory),y
	jmp caseend82271
casenext38287
	lda t
	cmp #$3 ;keep
	bne casenext54328
	; Assigning single variable : screenmemory
	lda #42
	ldy x
	sta (screenmemory),y
	jmp caseend82271
casenext54328
	; Assigning single variable : screenmemory
	lda #42
	ldy x
	sta (screenmemory),y
caseend82271
	; Assigning single variable : p1
	lda #5
	ldy x
	sta (p1),y
	inc i
	lda zNumZombies
	cmp i ;keep
	beq forLoopDone11410
forLoopNotDone70448
	jmp for5904
forLoopDone11410
	rts
	
	
	; ***********  Defining procedure : Zombie_Move
	;    Procedure type : User-defined procedure
	
Zombie_Move
	; Assigning single variable : x
	; Load Byte array
	ldx zcurrent
	lda zx,x
	
	sta x
	; Assigning single variable : n
	sta n
	; Assigning single variable : y
	; Load Byte array
	lda zy,x
	
	sta y
	; Assigning single variable : s
	sta s
	; Assigning single variable : screenmemory
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda y
	asl ; *2
	tax
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	
	sta screenmemory
	sty screenmemory+1
	; Assigning single variable : p1
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda y
	asl ; *2
	tax
	lda clr,x   ; Address of table lo
	ldy clr+1,x   ; Address of table hi
	
	sta p1
	sty p1+1
	; Assigning single variable : screenmemory
	; Load Byte array
	ldx zcurrent
	lda zbgchar,x
	
	ldy x
	sta (screenmemory),y
	; Assigning single variable : p1
	; Load Byte array
	lda zbgcol,x
	
	sta (p1),y
	; Assigning single variable : t
	; 8 bit binop
	; Add/sub where right value is constant number
	jsr Random
	
	and #3
	 ; end add / sub var with constant
	
	sta t
	; Binary clause: LESSEQUAL
	; Compare with pure num / var optimization
	cmp #$1;keep
	; BC done
	beq binaryclausesuccess61120
	bcs binaryclausefailed92300
binaryclausesuccess61120
	lda #1; success
	jmp binaryclausefinished99950
binaryclausefailed92300
	lda #0 ; failed state
binaryclausefinished99950
	cmp #1
	beq ConditionalTrueBlock2345
	jmp elsedoneblock94861
ConditionalTrueBlock2345
	; Binary clause Simplified: GREATER
	lda x
	; Compare with pure num / var optimization
	cmp px;keep
	bcc elsedoneblock60056
	beq elsedoneblock60056
ConditionalTrueBlock46550
	; Assigning single variable : x
	dec x
elseblock41549
elsedoneblock60056
	; Binary clause Simplified: LESS
	lda x
	; Compare with pure num / var optimization
	cmp px;keep
	bcs elsedoneblock83458
ConditionalTrueBlock65914
	; Assigning single variable : x
	inc x
elseblock21924
elsedoneblock83458
	; Assigning single variable : p1
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda y
	asl ; *2
	tax
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	clc
	adc x
	bcc dtnooverflow658
	iny  ; overflow into high byte
dtnooverflow658
	
	sta p1
	sty p1+1
	; Binary clause Simplified: GREATER
	; Load Byte array
	; Load pointer array
	ldy #$0
	lda (p1),y
	
	tax
	lda cMask,x
	
	; Compare with pure num / var optimization
	cmp #$1;keep
	bcc elsedoneblock53319
	beq elsedoneblock53319
ConditionalTrueBlock31554
	; Assigning single variable : x
	lda n
	sta x
elseblock99281
elsedoneblock53319
	; Binary clause Simplified: GREATER
	lda y
	; Compare with pure num / var optimization
	cmp py;keep
	bcc elsedoneblock85506
	beq elsedoneblock85506
ConditionalTrueBlock96499
	; Assigning single variable : y
	dec y
elseblock24763
elsedoneblock85506
	; Binary clause Simplified: LESS
	lda y
	; Compare with pure num / var optimization
	cmp py;keep
	bcs elsedoneblock26701
ConditionalTrueBlock11305
	; Assigning single variable : y
	inc y
elseblock59970
elsedoneblock26701
	; Assigning single variable : p1
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda y
	asl ; *2
	tax
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	clc
	adc x
	bcc dtnooverflow80129
	iny  ; overflow into high byte
dtnooverflow80129
	
	sta p1
	sty p1+1
	; Binary clause Simplified: GREATER
	; Load Byte array
	; Load pointer array
	ldy #$0
	lda (p1),y
	
	tax
	lda cMask,x
	
	; Compare with pure num / var optimization
	cmp #$1;keep
	bcc elsedoneblock72417
	beq elsedoneblock72417
ConditionalTrueBlock10012
	; Assigning single variable : y
	lda s
	sta y
elseblock40186
elsedoneblock72417
elseblock51514
elsedoneblock94861
	; Binary clause: EQUALS
	lda t
	; Compare with pure num / var optimization
	cmp #$2;keep
	; BC done
	bne binaryclausefailed22835
binaryclausesuccess43058
	lda #1; success
	jmp binaryclausefinished43483
binaryclausefailed22835
	lda #0 ; failed state
binaryclausefinished43483
	cmp #1
	beq ConditionalTrueBlock78979
	jmp elsedoneblock80237
ConditionalTrueBlock78979
	; Assigning single variable : i
	jsr Random
	
	sta i
	; Binary clause Simplified: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	and #1
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elseblock25315
ConditionalTrueBlock58053
	; Assigning single variable : x
	inc x
	jmp elsedoneblock99213
elseblock25315
	; Assigning single variable : x
	dec x
elsedoneblock99213
	; Assigning single variable : p1
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda y
	asl ; *2
	tax
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	clc
	adc x
	bcc dtnooverflow35680
	iny  ; overflow into high byte
dtnooverflow35680
	
	sta p1
	sty p1+1
	; Binary clause Simplified: GREATER
	; Load Byte array
	; Load pointer array
	ldy #$0
	lda (p1),y
	
	tax
	lda cMask,x
	
	; Compare with pure num / var optimization
	cmp #$1;keep
	bcc elsedoneblock71564
	beq elsedoneblock71564
ConditionalTrueBlock58641
	; Assigning single variable : x
	lda n
	sta x
elseblock71006
elsedoneblock71564
	; Binary clause Simplified: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda i
	and #2
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elseblock85752
ConditionalTrueBlock131
	; Assigning single variable : y
	dec y
	jmp elsedoneblock30242
elseblock85752
	; Assigning single variable : y
	inc y
elsedoneblock30242
	; Assigning single variable : p1
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda y
	asl ; *2
	tax
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	clc
	adc x
	bcc dtnooverflow17824
	iny  ; overflow into high byte
dtnooverflow17824
	
	sta p1
	sty p1+1
	; Binary clause Simplified: GREATER
	; Load Byte array
	; Load pointer array
	ldy #$0
	lda (p1),y
	
	tax
	lda cMask,x
	
	; Compare with pure num / var optimization
	cmp #$1;keep
	bcc elsedoneblock61002
	beq elsedoneblock61002
ConditionalTrueBlock34463
	; Assigning single variable : y
	lda s
	sta y
elseblock6071
elsedoneblock61002
elseblock34589
elsedoneblock80237
	; Assigning single variable : zx
	lda x
	ldx zcurrent
	sta zx,x
	; Assigning single variable : zy
	lda y
	sta zy,x
	; Assigning single variable : screenmemory
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	asl ; *2
	tax
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	
	sta screenmemory
	sty screenmemory+1
	; Assigning single variable : p1
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda y
	asl ; *2
	tax
	lda clr,x   ; Address of table lo
	ldy clr+1,x   ; Address of table hi
	
	sta p1
	sty p1+1
	; Assigning single variable : zbgchar
	; Load pointer array
	ldy x
	lda (screenmemory),y
	
	ldx zcurrent
	sta zbgchar,x
	; Assigning single variable : zbgcol
	; Load pointer array
	lda (p1),y
	
	sta zbgcol,x
	; Binary clause Simplified: EQUALS
	lda cx
binary_clause_temp_var33766 = $54
	sta binary_clause_temp_var33766
	; Load Byte array
	ldx zcurrent
	lda zx,x
	
binary_clause_temp_2_var838 = $56
	sta binary_clause_temp_2_var838
	lda binary_clause_temp_var33766
	cmp binary_clause_temp_2_var838;keep
	bne elsedoneblock28656
ConditionalTrueBlock99174
	; Binary clause Simplified: EQUALS
	lda cy
binary_clause_temp_var54461 = $54
	sta binary_clause_temp_var54461
	; Load Byte array
	ldx zcurrent
	lda zy,x
	
binary_clause_temp_2_var65135 = $56
	sta binary_clause_temp_2_var65135
	lda binary_clause_temp_var54461
	cmp binary_clause_temp_2_var65135;keep
	bne elsedoneblock30959
ConditionalTrueBlock2942
	; Assigning single variable : i
	lda zcurrent
	sta i
	jsr HitZombie
elseblock84284
elsedoneblock30959
elseblock11544
elsedoneblock28656
	rts
	
	
	; ***********  Defining procedure : UpdateZombies
	;    Procedure type : User-defined procedure
	
UpdateZombies
	; Binary clause Simplified: EQUALS
	lda zNumZombies
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock113
ConditionalTrueBlock83598
	rts
elseblock99356
elsedoneblock113
	; Binary clause Simplified: EQUALS
	; Modulo
	lda ztime
val_var68232 = $54
	sta val_var68232
	lda ticks
	sec
modulo92331
	sbc val_var68232
	bcs modulo92331
	adc val_var68232
	
	; Compare with pure num / var optimization
	cmp #$1;keep
	bne elsedoneblock32110
ConditionalTrueBlock19962
	; Binary clause Simplified: LESS
	lda zcurrent
	; Compare with pure num / var optimization
	cmp zNumZombies;keep
	bcs elsedoneblock25413
ConditionalTrueBlock45820
	; Binary clause Simplified: EQUALS
	; Load Byte array
	ldx zcurrent
	lda zspawn,x
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock63193
ConditionalTrueBlock9439
	jsr Zombie_Move
elseblock59619
elsedoneblock63193
	; Binary clause Simplified: GREATER
	; Load Byte array
	ldx zcurrent
	lda zspawn,x
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock76850
	beq elsedoneblock76850
ConditionalTrueBlock61770
	; Assigning single variable : zspawn
	ldx zcurrent
	; Optimize byte array dec 
	dec zspawn,x
elseblock69528
elsedoneblock76850
elseblock74446
elsedoneblock25413
	jsr DisplayZombies
	; Assigning single variable : zcurrent
	inc zcurrent
	; Binary clause Simplified: EQUALS
	lda zcurrent
	; Compare with pure num / var optimization
	cmp #$a;keep
	bne elsedoneblock55148
ConditionalTrueBlock76229
	; Assigning single variable : zcurrent
	lda #0
	sta zcurrent
elseblock24031
elsedoneblock55148
elseblock80744
elsedoneblock32110
	rts
	
	
	; ***********  Defining procedure : SpawnCandy
	;    Procedure type : User-defined procedure
	
SpawnCandy
	; Assigning single variable : x
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #20
val_var69906 = $54
	sta val_var69906
	jsr Random
	sec
modulo91568
	sbc val_var69906
	bcs modulo91568
	adc val_var69906
	
	clc
	adc #1
	 ; end add / sub var with constant
	
	sta x
	; Assigning single variable : y
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #16
val_var53954 = $54
	sta val_var53954
	jsr Random
	sec
modulo62508
	sbc val_var53954
	bcs modulo62508
	adc val_var53954
	
	clc
	adc #4
	 ; end add / sub var with constant
	
	sta y
	; Assigning single variable : t
	lda #0
	sta t
	; Assigning single variable : screenmemory
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda y
	asl ; *2
	tax
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	
	sta screenmemory
	sty screenmemory+1
while21698
	; Binary clause Simplified: EQUALS
	lda t
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock52470
ConditionalTrueBlock34303
	; Binary clause Simplified: EQUALS
	; Load Byte array
	; Load pointer array
	ldy x
	lda (screenmemory),y
	
	tax
	lda cMask,x
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock82649
ConditionalTrueBlock41138
	; Assigning single variable : t
	lda #1
	sta t
elseblock99958
elsedoneblock82649
	; Binary clause Simplified: EQUALS
	lda t
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock10720
ConditionalTrueBlock38146
	; Assigning single variable : x
	inc x
	; Binary clause Simplified: GREATER
	lda x
	; Compare with pure num / var optimization
	cmp #$14;keep
	bcc elsedoneblock83348
	beq elsedoneblock83348
ConditionalTrueBlock15033
	; Assigning single variable : x
	lda #1
	sta x
	; Assigning single variable : y
	inc y
	; Binary clause Simplified: GREATER
	lda y
	; Compare with pure num / var optimization
	cmp #$16;keep
	bcc elsedoneblock23221
	beq elsedoneblock23221
ConditionalTrueBlock64303
	; Assigning single variable : y
	lda #4
	sta y
elseblock46387
elsedoneblock23221
elseblock1842
elsedoneblock83348
elseblock9428
elsedoneblock10720
	jmp while21698
elseblock987
elsedoneblock52470
	; Assigning single variable : p1
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda y
	asl ; *2
	tax
	lda clr,x   ; Address of table lo
	ldy clr+1,x   ; Address of table hi
	
	sta p1
	sty p1+1
	; Assigning single variable : screenmemory
	lda #46
	ldy x
	sta (screenmemory),y
	; Assigning single variable : p1
	lda #3
	sta (p1),y
	; Assigning single variable : candyscr
	inc candyscr
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd2),y
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock870
ConditionalTrueBlock29462
	; Assigning single variable : psnd2
	lda #<sndCAppear
	ldx #>sndCAppear
	sta psnd2
	stx psnd2+1
	; Assigning single variable : vsnd2
	lda #250
	sta vsnd2
	jsr PlaySnd2
elseblock30386
elsedoneblock870
	rts
	
	
	; ***********  Defining procedure : EndCandy
	;    Procedure type : User-defined procedure
	
EndCandy
	; Assigning single variable : p1
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda cy
	asl ; *2
	tax
	lda clr,x   ; Address of table lo
	ldy clr+1,x   ; Address of table hi
	
	sta p1
	sty p1+1
	; Binary clause Simplified: GREATER
	lda cy
	; Compare with pure num / var optimization
	cmp #$3;keep
	bcc elsedoneblock8766
	beq elsedoneblock8766
ConditionalTrueBlock68824
	; Assigning single variable : p1
	lda #3
	ldy cx
	sta (p1),y
elseblock19117
elsedoneblock8766
	; Assigning single variable : cx
	lda #0
	sta cx
	; Assigning single variable : cy
	sta cy
	; Assigning single variable : clife
	sta clife
	rts
	
	
	; ***********  Defining procedure : DrawCandyCounter
	;    Procedure type : User-defined procedure
	
DrawCandyCounter
	; Assigning single variable : screenmemory
	lda #15
	ldx #16
	sta screenmemory
	stx screenmemory+1
	; Assigning single variable : p1
	ldx #148
	sta p1
	stx p1+1
	; Assigning single variable : i
	lda #1
	sta i
for97893
	; Assigning single variable : screenmemory
	lda #0
	ldy i
	sta (screenmemory),y
	inc i
	lda #6
	cmp i ;keep
	bcs for97893
forLoopDone60186
	; Assigning single variable : i
	lda #1
	sta i
for14057
	; Binary clause Simplified: GREATEREQUAL
	lda candy
	; Compare with pure num / var optimization
	cmp i;keep
	bcc elsedoneblock98817
ConditionalTrueBlock84506
	; Assigning single variable : screenmemory
	lda #46
	pha
	; 8 bit binop
	; Add/sub where right value is constant number
	lda #7
	sec
	sbc i
	 ; end add / sub var with constant
	
	tay
	pla
	sta (screenmemory),y
	; Assigning single variable : p1
	lda #3
	pha
	; 8 bit binop
	; Add/sub where right value is constant number
	lda #7
	sec
	sbc i
	 ; end add / sub var with constant
	
	tay
	pla
	sta (p1),y
elseblock3163
elsedoneblock98817
	; Binary clause Simplified: GREATEREQUAL
	lda witchHits
	; Compare with pure num / var optimization
	cmp i;keep
	bcc elsedoneblock38893
ConditionalTrueBlock51352
	; Assigning single variable : screenmemory
	lda #38
	ldy i
	sta (screenmemory),y
	; Assigning single variable : p1
	lda #1
	sta (p1),y
elseblock88549
elsedoneblock38893
	inc i
	lda #6
	cmp i ;keep
	bcs for14057
forLoopDone75260
	rts
	
	
	; ***********  Defining procedure : MoveCandy
	;    Procedure type : User-defined procedure
	
MoveCandy
	; Assigning single variable : screenmemory
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda cy
	asl ; *2
	tax
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	
	sta screenmemory
	sty screenmemory+1
	; Assigning single variable : p1
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda cy
	asl ; *2
	tax
	lda clr,x   ; Address of table lo
	ldy clr+1,x   ; Address of table hi
	
	sta p1
	sty p1+1
	; Assigning single variable : screenmemory
	lda cbgchar
	ldy cx
	sta (screenmemory),y
	; Assigning single variable : p1
	lda cbgcol
	sta (p1),y
	; Assigning single variable : x
	lda cx
	sta x
	; Binary clause Simplified: EQUALS
	lda cdx
	; Compare with pure num / var optimization
	cmp #$1;keep
	bne elsedoneblock89944
ConditionalTrueBlock57692
	; Assigning single variable : cx
	inc cx
elseblock57302
elsedoneblock89944
	; Binary clause Simplified: NOTEQUALS
	lda cdx
	; Compare with pure num / var optimization
	cmp #$1;keep
	beq elsedoneblock14526
ConditionalTrueBlock66256
	; Assigning single variable : cx
	dec cx
elseblock1101
elsedoneblock14526
	; Binary clause Simplified: EQUALS
	; Load Byte array
	; Load pointer array
	ldy cx
	lda (screenmemory),y
	
	tax
	lda cMask,x
	
	; Compare with pure num / var optimization
	cmp #$a;keep
	bne elsedoneblock66796
ConditionalTrueBlock93054
	; Assigning single variable : cx
	lda x
	sta cx
	; Binary clause Simplified: LESS
	lda candyscr
	; Compare with pure num / var optimization
	cmp #$4;keep
	bcs elsedoneblock82346
ConditionalTrueBlock38157
	; Assigning single variable : candyscr
	inc candyscr
elseblock92203
elsedoneblock82346
elseblock53939
elsedoneblock66796
	; Assigning single variable : cbgchar
	; Load pointer array
	ldy cx
	lda (screenmemory),y
	
	sta cbgchar
	; Assigning single variable : cbgcol
	; Load pointer array
	lda (p1),y
	
	sta cbgcol
	; Assigning single variable : screenmemory
	; 8 bit binop
	; Add/sub where right value is constant number
	; 8 bit binop
	; Add/sub where right value is constant number
	lda ticks
	lsr
	
	and #1
	 ; end add / sub var with constant
	
	clc
	adc #46
	 ; end add / sub var with constant
	
	sta (screenmemory),y
	; Assigning single variable : p1
	lda #7
	sta (p1),y
	; Assigning single variable : clife
	dec clife
	; Binary clause: EQUALS
	lda cx
	; Compare with pure num / var optimization
	cmp x;keep
	; BC done
	bne tempfail78440
binaryclausesuccess82584
	jmp ConditionalTrueBlock75927
tempfail78440
	; Binary clause: EQUALS
	lda clife
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bne elseblock86252
binaryclausesuccess36010
ConditionalTrueBlock75927
	jsr EndCandy
elseblock86252
elsedoneblock17553
	rts
	
	
	; ***********  Defining procedure : UpdateCandy
	;    Procedure type : User-defined procedure
	
UpdateCandy
	; Binary clause: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1pressed
	and #16
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	beq binaryclausefailed62638
binaryclausesuccess98187
	lda #1; success
	jmp binaryclausefinished49476
binaryclausefailed62638
	lda #0 ; failed state
binaryclausefinished49476
	cmp #1
	beq ConditionalTrueBlock95513
	jmp elsedoneblock52277
ConditionalTrueBlock95513
	; Binary clause: EQUALS
	lda clife
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bne binaryclausefailed66768
binaryclausesuccess97797
	lda #1; success
	jmp binaryclausefinished30540
binaryclausefailed66768
	lda #0 ; failed state
binaryclausefinished30540
	cmp #1
	beq ConditionalTrueBlock82199
	jmp elsedoneblock85770
ConditionalTrueBlock82199
	; Binary clause: GREATER
	lda candy
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bcc binaryclausefailed42587
	beq binaryclausefailed42587
binaryclausesuccess62196
	lda #1; success
	jmp binaryclausefinished51295
binaryclausefailed42587
	lda #0 ; failed state
binaryclausefinished51295
	cmp #1
	beq ConditionalTrueBlock9341
	jmp elsedoneblock47223
ConditionalTrueBlock9341
	; Assigning single variable : cy
	lda py
	sta cy
	; Binary clause Simplified: EQUALS
	lda pd
	; Compare with pure num / var optimization
	cmp #$3a;keep
	bne elsedoneblock22256
ConditionalTrueBlock71166
	; Assigning single variable : cdx
	lda #255
	sta cdx
elseblock84129
elsedoneblock22256
	; Binary clause Simplified: EQUALS
	lda pd
	; Compare with pure num / var optimization
	cmp #$3d;keep
	bne elsedoneblock75429
ConditionalTrueBlock44718
	; Assigning single variable : cdx
	lda #1
	sta cdx
elseblock12055
elsedoneblock75429
	; Assigning single variable : screenmemory
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda cy
	asl ; *2
	tax
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	
	sta screenmemory
	sty screenmemory+1
	; Assigning single variable : p1
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda cy
	asl ; *2
	tax
	lda clr,x   ; Address of table lo
	ldy clr+1,x   ; Address of table hi
	
	sta p1
	sty p1+1
	; Binary clause Simplified: EQUALS
	lda cdx
	; Compare with pure num / var optimization
	cmp #$1;keep
	bne elsedoneblock56737
ConditionalTrueBlock22715
	; Assigning single variable : x
	; 8 bit binop
	; Add/sub where right value is constant number
	lda px
	clc
	adc #1
	 ; end add / sub var with constant
	
	sta x
elseblock72878
elsedoneblock56737
	; Binary clause Simplified: NOTEQUALS
	lda cdx
	; Compare with pure num / var optimization
	cmp #$1;keep
	beq elsedoneblock75805
ConditionalTrueBlock13882
	; Assigning single variable : x
	; 8 bit binop
	; Add/sub where right value is constant number
	lda px
	sec
	sbc #1
	 ; end add / sub var with constant
	
	sta x
elseblock13308
elsedoneblock75805
	; Binary clause Simplified: EQUALS
	; Load Byte array
	; Load pointer array
	ldy x
	lda (screenmemory),y
	
	tax
	lda cMask,x
	
	; Compare with pure num / var optimization
	cmp #$a;keep
	bne elseblock47069
ConditionalTrueBlock33257
	; Assigning single variable : cy
	lda #0
	sta cy
	; Assigning single variable : cx
	sta cx
	; Assigning single variable : cdx
	sta cdx
	jmp elsedoneblock40881
elseblock47069
	; Assigning single variable : cbgchar
	lda pbgchar
	sta cbgchar
	; Assigning single variable : cbgcol
	lda #1
	sta cbgcol
	; Assigning single variable : cx
	lda px
	sta cx
	; Assigning single variable : clife
	lda #6
	sta clife
	; Assigning single variable : candy
	dec candy
	jsr DrawCandyCounter
	; Assigning single variable : psndfx
	lda #<sndShoot
	ldx #>sndShoot
	sta psndfx
	stx psndfx+1
	; Assigning single variable : vsndfx
	lda #254
	sta vsndfx
	jsr PlaySndfx
elsedoneblock40881
elseblock93053
elsedoneblock47223
elseblock77140
elsedoneblock85770
elseblock93554
elsedoneblock52277
	; Binary clause Simplified: GREATER
	lda clife
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock92118
	beq elsedoneblock92118
ConditionalTrueBlock70879
	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda ticks
	and #1
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock97219
ConditionalTrueBlock41976
	jsr MoveCandy
elseblock51304
elsedoneblock97219
elseblock85889
elsedoneblock92118
	rts
	
	
	; ***********  Defining procedure : MovePlayer
	;    Procedure type : User-defined procedure
	
MovePlayer
	; Binary clause: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1
	and #8
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	beq binaryclausefailed14403
binaryclausesuccess66131
	lda #1; success
	jmp binaryclausefinished40418
binaryclausefailed14403
	lda #0 ; failed state
binaryclausefinished40418
	cmp #1
	beq ConditionalTrueBlock35706
	jmp elsedoneblock26508
ConditionalTrueBlock35706
	; Assigning single variable : pd
	lda #58
	sta pd
	; Assigning single variable : screenmemory
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda py
	asl ; *2
	tax
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	; xoffset is complex
	sta $4c
	; 8 bit binop
	; Add/sub where right value is constant number
	lda px
	sec
	sbc #1
	 ; end add / sub var with constant
	clc
	adc $4c
	bcc dtnooverflow87377
	iny  ; overflow into high byte
dtnooverflow87377
	
	sta screenmemory
	sty screenmemory+1
	; Assigning single variable : p1
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda py
	asl ; *2
	tax
	lda clr,x   ; Address of table lo
	ldy clr+1,x   ; Address of table hi
	; xoffset is complex
	sta $4c
	; 8 bit binop
	; Add/sub where right value is constant number
	lda px
	sec
	sbc #1
	 ; end add / sub var with constant
	clc
	adc $4c
	bcc dtnooverflow86656
	iny  ; overflow into high byte
dtnooverflow86656
	
	sta p1
	sty p1+1
	; Binary clause Simplified: LESS
	; Load Byte array
	; Load pointer array
	ldy #$0
	lda (screenmemory),y
	
	tax
	lda cMask,x
	
	; Compare with pure num / var optimization
	cmp #$a;keep
	bcs elsedoneblock4852
ConditionalTrueBlock19944
	; Assigning single variable : screenmemory
	lda pbgchar
	ldy #1
	sta (screenmemory),y
	; Assigning single variable : p1
	lda pbgcol
	sta (p1),y
	; Assigning single variable : px
	dec px
	; Assigning single variable : pbgchar
	; Load pointer array
	ldy #$0
	lda (screenmemory),y
	
	sta pbgchar
	; Assigning single variable : pbgcol
	; Load pointer array
	lda (p1),y
	
	sta pbgcol
	; Assigning single variable : pstep
	lda #5
	sta pstep
elseblock62069
elsedoneblock4852
	rts
elseblock90212
elsedoneblock26508
	; Binary clause: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1
	and #1
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	beq binaryclausefailed17809
binaryclausesuccess32074
	lda #1; success
	jmp binaryclausefinished48473
binaryclausefailed17809
	lda #0 ; failed state
binaryclausefinished48473
	cmp #1
	beq ConditionalTrueBlock66026
	jmp elsedoneblock68784
ConditionalTrueBlock66026
	; Assigning single variable : pd
	lda #61
	sta pd
	; Assigning single variable : screenmemory
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda py
	asl ; *2
	tax
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	
	sta screenmemory
	sty screenmemory+1
	; Assigning single variable : p1
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda py
	asl ; *2
	tax
	lda clr,x   ; Address of table lo
	ldy clr+1,x   ; Address of table hi
	
	sta p1
	sty p1+1
	; Binary clause Simplified: LESS
	; Load Byte array
	; Load pointer array
	; LDA stuff
	; 8 bit binop
	; Add/sub where right value is constant number
	lda px
	clc
	adc #1
	 ; end add / sub var with constant
	
	tay
	lda (screenmemory),y
	
	tax
	lda cMask,x
	
	; Compare with pure num / var optimization
	cmp #$a;keep
	bcs elsedoneblock38020
ConditionalTrueBlock59772
	; Assigning single variable : screenmemory
	lda pbgchar
	ldy px
	sta (screenmemory),y
	; Assigning single variable : p1
	lda pbgcol
	sta (p1),y
	; Assigning single variable : px
	inc px
	; Assigning single variable : pbgchar
	; Load pointer array
	ldy px
	lda (screenmemory),y
	
	sta pbgchar
	; Assigning single variable : pbgcol
	; Load pointer array
	lda (p1),y
	
	sta pbgcol
	; Assigning single variable : pstep
	lda #5
	sta pstep
elseblock55866
elsedoneblock38020
	rts
elseblock16123
elsedoneblock68784
	; Binary clause: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1
	and #2
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	beq binaryclausefailed60396
binaryclausesuccess61591
	lda #1; success
	jmp binaryclausefinished56298
binaryclausefailed60396
	lda #0 ; failed state
binaryclausefinished56298
	cmp #1
	beq ConditionalTrueBlock83445
	jmp elsedoneblock44815
ConditionalTrueBlock83445
	; Assigning single variable : screenmemory
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	; 8 bit binop
	; Add/sub where right value is constant number
	lda py
	sec
	sbc #1
	 ; end add / sub var with constant
	asl ; *2
	tax
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	
	sta screenmemory
	sty screenmemory+1
	; Assigning single variable : p1
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	; 8 bit binop
	; Add/sub where right value is constant number
	lda py
	sec
	sbc #1
	 ; end add / sub var with constant
	asl ; *2
	tax
	lda clr,x   ; Address of table lo
	ldy clr+1,x   ; Address of table hi
	
	sta p1
	sty p1+1
	; Binary clause Simplified: LESS
	; Load Byte array
	; Load pointer array
	ldy px
	lda (screenmemory),y
	
	tax
	lda cMask,x
	
	; Compare with pure num / var optimization
	cmp #$a;keep
	bcs elsedoneblock95150
ConditionalTrueBlock10762
	; Assigning single variable : screenmemory
	lda pbgchar
	pha
	; 8 bit binop
	; Add/sub where right value is constant number
	lda px
	clc
	adc #22
	 ; end add / sub var with constant
	
	tay
	pla
	sta (screenmemory),y
	; Assigning single variable : p1
	lda pbgcol
	pha
	; 8 bit binop
	; Add/sub where right value is constant number
	lda px
	clc
	adc #22
	 ; end add / sub var with constant
	
	tay
	pla
	sta (p1),y
	; Assigning single variable : py
	dec py
	; Assigning single variable : pbgchar
	; Load pointer array
	ldy px
	lda (screenmemory),y
	
	sta pbgchar
	; Assigning single variable : pbgcol
	; Load pointer array
	lda (p1),y
	
	sta pbgcol
	; Assigning single variable : pstep
	lda #5
	sta pstep
elseblock17267
elsedoneblock95150
	rts
elseblock20943
elsedoneblock44815
	; Binary clause: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1
	and #4
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	beq binaryclausefailed48250
binaryclausesuccess94814
	lda #1; success
	jmp binaryclausefinished20263
binaryclausefailed48250
	lda #0 ; failed state
binaryclausefinished20263
	cmp #1
	beq ConditionalTrueBlock6240
	jmp elsedoneblock25259
ConditionalTrueBlock6240
	; Assigning single variable : screenmemory
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda py
	asl ; *2
	tax
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	
	sta screenmemory
	sty screenmemory+1
	; Assigning single variable : p1
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda py
	asl ; *2
	tax
	lda clr,x   ; Address of table lo
	ldy clr+1,x   ; Address of table hi
	
	sta p1
	sty p1+1
	; Binary clause Simplified: LESS
	; Load Byte array
	; Load pointer array
	; LDA stuff
	; 8 bit binop
	; Add/sub where right value is constant number
	lda px
	clc
	adc #22
	 ; end add / sub var with constant
	
	tay
	lda (screenmemory),y
	
	tax
	lda cMask,x
	
	; Compare with pure num / var optimization
	cmp #$a;keep
	bcs elsedoneblock93624
ConditionalTrueBlock95765
	; Assigning single variable : screenmemory
	lda pbgchar
	ldy px
	sta (screenmemory),y
	; Assigning single variable : p1
	lda pbgcol
	sta (p1),y
	; Assigning single variable : py
	inc py
	; Assigning single variable : pbgchar
	; Load pointer array
	; LDA stuff
	; 8 bit binop
	; Add/sub where right value is constant number
	lda px
	clc
	adc #22
	 ; end add / sub var with constant
	
	tay
	lda (screenmemory),y
	
	sta pbgchar
	; Assigning single variable : pbgcol
	; Load pointer array
	; LDA stuff
	; 8 bit binop
	; Add/sub where right value is constant number
	lda px
	clc
	adc #22
	 ; end add / sub var with constant
	
	tay
	lda (p1),y
	
	sta pbgcol
	; Assigning single variable : pstep
	lda #5
	sta pstep
elseblock86865
elsedoneblock93624
	rts
elseblock95572
elsedoneblock25259
	rts
	
	
	; ***********  Defining procedure : UpdatePlayer
	;    Procedure type : User-defined procedure
	
UpdatePlayer
	; Binary clause Simplified: EQUALS
	; Modulo
	lda #2
val_var9831 = $54
	sta val_var9831
	lda ticks
	sec
modulo54228
	sbc val_var9831
	bcs modulo54228
	adc val_var9831
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock47988
ConditionalTrueBlock89325
	; Assigning single variable : pan
	inc pan
elseblock22591
elsedoneblock47988
	; Binary clause Simplified: GREATER
	lda pan
	; Compare with pure num / var optimization
	cmp #$5;keep
	bcc elsedoneblock313
	beq elsedoneblock313
ConditionalTrueBlock23969
	; Assigning single variable : pan
	lda #0
	sta pan
elseblock62697
elsedoneblock313
	; Binary clause Simplified: EQUALS
	lda joy1
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock28386
ConditionalTrueBlock93916
	; Assigning single variable : pstep
	lda #0
	sta pstep
elseblock18732
elsedoneblock28386
	; Binary clause Simplified: GREATER
	lda pstep
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock48473
	beq elsedoneblock48473
ConditionalTrueBlock59385
	; Assigning single variable : pstep
	dec pstep
elseblock94814
elsedoneblock48473
	; Binary clause Simplified: EQUALS
	lda pstep
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock7396
ConditionalTrueBlock46977
	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1
	and #16
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock3604
ConditionalTrueBlock21414
	jsr MovePlayer
	; Binary clause Simplified: GREATER
	lda pstep
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock32247
	beq elsedoneblock32247
ConditionalTrueBlock66146
	; Assigning single variable : psnd1
	lda #<sndPumpkidFootsteps
	ldx #>sndPumpkidFootsteps
	sta psnd1
	stx psnd1+1
	; Assigning single variable : vsnd1
	; 8 bit binop
	; Add/sub where right value is constant number
	lda #160
	clc
	adc stepsnd
	 ; end add / sub var with constant
	
	sta vsnd1
	jsr PlaySnd1
	; Binary clause Simplified: EQUALS
	lda stepsnd
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elseblock81616
ConditionalTrueBlock1568
	; Assigning single variable : stepsnd
	lda #30
	sta stepsnd
	jmp elsedoneblock49966
elseblock81616
	; Assigning single variable : stepsnd
	lda #0
	sta stepsnd
elsedoneblock49966
elseblock30872
elsedoneblock32247
elseblock97161
elsedoneblock3604
elseblock81337
elsedoneblock7396
	; Assigning single variable : screenmemory
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda py
	asl ; *2
	tax
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	
	sta screenmemory
	sty screenmemory+1
	; Assigning single variable : p1
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda py
	asl ; *2
	tax
	lda clr,x   ; Address of table lo
	ldy clr+1,x   ; Address of table hi
	
	sta p1
	sty p1+1
	; Assigning single variable : screenmemory
	; 8 bit binop
	; Add/sub where right value is constant number
	; Load Byte array
	ldx pan
	lda panFrames,x
	
	clc
	adc pd
	 ; end add / sub var with constant
	
	ldy px
	sta (screenmemory),y
	; Assigning single variable : p1
	lda #2
	sta (p1),y
	; Binary clause Simplified: EQUALS
	; Load Byte array
	ldx pbgchar
	lda cMask,x
	
	; Compare with pure num / var optimization
	cmp #$2;keep
	bne elsedoneblock41751
ConditionalTrueBlock18466
	; Binary clause Simplified: LESS
	lda candy
binary_clause_temp_var66392 = $54
	sta binary_clause_temp_var66392
	; 8 bit binop
	; Add/sub where right value is constant number
	lda #6
	sec
	sbc witchHits
	 ; end add / sub var with constant
	
binary_clause_temp_2_var88018 = $56
	sta binary_clause_temp_2_var88018
	lda binary_clause_temp_var66392
	cmp binary_clause_temp_2_var88018;keep
	bcs elsedoneblock56396
ConditionalTrueBlock33518
	; Assigning single variable : pbgchar
	lda #0
	sta pbgchar
	; Assigning single variable : candy
	inc candy
	jsr DrawCandyCounter
	; Binary clause Simplified: GREATER
	lda candyscr
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock97004
	beq elsedoneblock97004
ConditionalTrueBlock44543
	; Assigning single variable : candyscr
	dec candyscr
elseblock64825
elsedoneblock97004
	; ----------
	; BcdAdd address, address, number
	sed
	clc
	lda score
	adc scCandy
	sta score
	lda score+$01
	adc scCandy+$01
	sta score+$01
	lda score+$02
	adc scCandy+$02
	sta score+$02
	lda score+$03
	adc scCandy+$03
	sta score+$03
	cld
	jsr DisplayScore
	; Assigning single variable : psnd3
	lda #<sndPickup
	ldx #>sndPickup
	sta psnd3
	stx psnd3+1
	; Assigning single variable : vsnd3
	lda #230
	sta vsnd3
	jsr PlaySnd3
elseblock93662
elsedoneblock56396
elseblock42196
elsedoneblock41751
	rts
	
	
	; ***********  Defining procedure : SetupTimer
	;    Procedure type : User-defined procedure
	
SetupTimer
	; Assigning single variable : gameTimer
	lda #0
	sta gameTimer
	; Assigning single variable : moonPos
	lda #2
	sta moonPos
	; Assigning single variable : cloudPos
	lda #0
	sta cloudPos
	; Assigning single variable : screenmemory
	lda #44
	ldx #148
	sta screenmemory
	stx screenmemory+1
	; Assigning single variable : screenmemory
	lda #1
	ldy cloudPos
	sta (screenmemory),y
	; Assigning single variable : screenmemory
	lda #22
	sta screenmemory
	stx screenmemory+1
	; Assigning single variable : screenmemory
	lda #7
	ldy moonPos
	sta (screenmemory),y
	rts
	
	
	; ***********  Defining procedure : MoveMoon
	;    Procedure type : User-defined procedure
	
MoveMoon
	; Assigning single variable : p1
	lda #22
	ldx #148
	sta p1
	stx p1+1
	; Assigning single variable : p1
	lda #0
	ldy moonPos
	sta (p1),y
	; Assigning single variable : moonPos
	inc moonPos
	; Assigning single variable : p1
	lda #7
	ldy moonPos
	sta (p1),y
	; Binary clause Simplified: GREATEREQUAL
	lda moonPos
	; Compare with pure num / var optimization
	cmp #$6;keep
	bcc elsedoneblock15304
ConditionalTrueBlock60908
	jsr SpawnAZombie
elseblock56387
elsedoneblock15304
	; ----------
	; BcdAdd address, address, number
	sed
	clc
	lda score
	adc scMoon
	sta score
	lda score+$01
	adc scMoon+$01
	sta score+$01
	lda score+$02
	adc scMoon+$02
	sta score+$02
	lda score+$03
	adc scMoon+$03
	sta score+$03
	cld
	jsr DisplayScore
	rts
	
	
	; ***********  Defining procedure : ClearTime
	;    Procedure type : User-defined procedure
	
ClearTime
	; Assigning single variable : i
	lda #0
	sta i
for73032
	; Assigning single variable : screenmemory
	lda #0
	ldy i
	sta (screenmemory),y
	inc i
	lda #8
	cmp i ;keep
	bne for73032
forLoopDone16347
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #2
	bcc WordAdd65366
	inc screenmemory+1
WordAdd65366
	sta screenmemory+0
	rts
	
	
	; ***********  Defining procedure : DisplayTime
	;    Procedure type : User-defined procedure
	
DisplayTime
	; Assigning single variable : screenmemory
	lda #8
	ldx #16
	sta screenmemory
	stx screenmemory+1
	lda moonPos
	cmp #$4 ;keep
	bne casenext48050
	jsr ClearTime
	clc
	lda #<t11
	adc #0
	ldy #>t11
	sta print_text+0
	sty print_text+1
	ldx #4 ; optimized, look out for bugs
	jsr printstring
	jmp caseend38326
casenext48050
	lda moonPos
	cmp #$6 ;keep
	bne casenext77950
	jsr ClearTime
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	sec
	sbc #2
	bcs WordAdd55229
	dec screenmemory+1
WordAdd55229
	sta screenmemory+0
	clc
	lda #<t12
	adc #0
	ldy #>t12
	sta print_text+0
	sty print_text+1
	ldx #8 ; optimized, look out for bugs
	jsr printstring
	jmp caseend38326
casenext77950
	lda moonPos
	cmp #$8 ;keep
	bne casenext20804
	jsr ClearTime
	clc
	lda #<t01
	adc #0
	ldy #>t01
	sta print_text+0
	sty print_text+1
	ldx #4 ; optimized, look out for bugs
	jsr printstring
	jmp caseend38326
casenext20804
	lda moonPos
	cmp #$a ;keep
	bne casenext70847
	jsr ClearTime
	clc
	lda #<t02
	adc #0
	ldy #>t02
	sta print_text+0
	sty print_text+1
	ldx #4 ; optimized, look out for bugs
	jsr printstring
	jmp caseend38326
casenext70847
	lda moonPos
	cmp #$c ;keep
	bne casenext77108
	jsr ClearTime
	clc
	lda #<t03
	adc #0
	ldy #>t03
	sta print_text+0
	sty print_text+1
	ldx #4 ; optimized, look out for bugs
	jsr printstring
	jmp caseend38326
casenext77108
	lda moonPos
	cmp #$e ;keep
	bne casenext19677
	jsr ClearTime
	clc
	lda #<t04
	adc #0
	ldy #>t04
	sta print_text+0
	sty print_text+1
	ldx #4 ; optimized, look out for bugs
	jsr printstring
	jmp caseend38326
casenext19677
	lda moonPos
	cmp #$10 ;keep
	bne casenext87854
	jsr ClearTime
	clc
	lda #<t05
	adc #0
	ldy #>t05
	sta print_text+0
	sty print_text+1
	ldx #4 ; optimized, look out for bugs
	jsr printstring
	jmp caseend38326
casenext87854
	lda moonPos
	cmp #$12 ;keep
	bne casenext26210
	jsr ClearTime
	clc
	lda #<t06
	adc #0
	ldy #>t06
	sta print_text+0
	sty print_text+1
	ldx #4 ; optimized, look out for bugs
	jsr printstring
	jmp caseend38326
casenext26210
caseend38326
	rts
	
	
	; ***********  Defining procedure : UpdateTimer
	;    Procedure type : User-defined procedure
	
UpdateTimer
	; Assigning single variable : gameTimer
	inc gameTimer
	; Binary clause Simplified: EQUALS
	lda gameTimer
	; Compare with pure num / var optimization
	cmp GAMETIMERMAX;keep
	bne elsedoneblock61689
ConditionalTrueBlock84471
	; Assigning single variable : gameTimer
	lda #0
	sta gameTimer
	; Assigning single variable : screenmemory
	lda #44
	ldx #148
	sta screenmemory
	stx screenmemory+1
	; Assigning single variable : screenmemory
	lda #0
	ldy cloudPos
	sta (screenmemory),y
	; Assigning single variable : cloudPos
	inc cloudPos
	; Binary clause Simplified: EQUALS
	lda cloudPos
	; Compare with pure num / var optimization
	cmp #$b;keep
	bne elsedoneblock19019
ConditionalTrueBlock53696
	; Binary clause Simplified: LESS
	lda candyscr
	; Compare with pure num / var optimization
	cmp #$3;keep
	bcs elsedoneblock20752
ConditionalTrueBlock17565
	jsr SpawnCandy
elseblock55662
elsedoneblock20752
elseblock61811
elsedoneblock19019
	; Binary clause Simplified: EQUALS
	lda cloudPos
	; Compare with pure num / var optimization
	cmp #$16;keep
	bne elsedoneblock72296
ConditionalTrueBlock80396
	; Assigning single variable : cloudPos
	lda #0
	sta cloudPos
	jsr MoveMoon
	jsr DisplayTime
	; Binary clause Simplified: LESS
	lda candyscr
	; Compare with pure num / var optimization
	cmp #$3;keep
	bcs elsedoneblock40322
ConditionalTrueBlock27367
	jsr SpawnCandy
elseblock36665
elsedoneblock40322
elseblock36404
elsedoneblock72296
	; Assigning single variable : screenmemory
	lda #44
	ldx #148
	sta screenmemory
	stx screenmemory+1
	; Assigning single variable : screenmemory
	lda #1
	ldy cloudPos
	sta (screenmemory),y
elseblock19439
elsedoneblock61689
	rts
	
	
	; ***********  Defining procedure : CollisionWait
	;    Procedure type : User-defined procedure
	
CollisionWait
	; Assigning memory location
	; Assigning single variable : $900f
	lda #10
	
	sta $900f
	; Assigning single variable : p1
	lda #48
	ldx #148
	sta p1
	stx p1+1
	jsr callReadJoy1
	jsr callReadJoy1
	; Assigning single variable : joy1pressed
	lda #0
	sta joy1pressed
	; Assigning single variable : x
	lda #1
	sta x
	; Assigning single variable : y
	sta y
while91977
	; Binary clause Simplified: NOTEQUALS
	; Load pointer array
	ldy #$0
	lda (psnd1),y
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock84908
ConditionalTrueBlock92061
	jmp while91977
elseblock34271
elsedoneblock84908
	jsr StopSounds
	; Assigning single variable : sndFlag
	lda #0
	sta sndFlag
while94996
	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1pressed
	and #16
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock15749
ConditionalTrueBlock40066
	; Assigning single variable : x
	lda y
	sta x
	; Assigning single variable : y
	inc y
	; Binary clause Simplified: GREATER
	; Compare with pure num / var optimization
	cmp #$7;keep
	bcc elsedoneblock87186
	beq elsedoneblock87186
ConditionalTrueBlock8254
	; Assigning single variable : y
	lda #1
	sta y
elseblock55630
elsedoneblock87186
	jsr callReadJoy1
	; Assigning single variable : t
	lda #0
	sta t
for19287
	; Assigning single variable : p1
	lda x
	ldy t
	sta (p1),y
	; Assigning single variable : x
	inc x
	; Binary clause Simplified: GREATER
	; Compare with pure num / var optimization
	cmp #$7;keep
	bcc elsedoneblock65509
	beq elsedoneblock65509
ConditionalTrueBlock89992
	; Assigning single variable : x
	lda #1
	sta x
elseblock82424
elsedoneblock65509
	inc t
	lda #14
	cmp t ;keep
	bne for19287
forLoopDone81776
	jmp while94996
elseblock58780
elsedoneblock15749
	; Assigning single variable : sndFlag
	lda #1
	sta sndFlag
	; Assigning single variable : psndfx
	lda #<sndShoot
	ldx #>sndShoot
	sta psndfx
	stx psndfx+1
	; Assigning single variable : vsndfx
	lda #254
	sta vsndfx
	jsr PlaySndfx
	rts
	
	
	; ***********  Defining procedure : CollisionPlayer
	;    Procedure type : User-defined procedure
	
CollisionPlayer
	; Binary clause Simplified: EQUALS
	lda moonPos
	; Compare with pure num / var optimization
	cmp #$13;keep
	bne elsedoneblock2720
ConditionalTrueBlock66083
	; Binary clause Simplified: EQUALS
	lda cloudPos
	; Compare with pure num / var optimization
	cmp #$15;keep
	bne elsedoneblock90371
ConditionalTrueBlock28651
	; Assigning single variable : gameMode
	lda #1
	sta gameMode
	; Assigning single variable : screenmemory
	lda #48
	ldx #16
	sta screenmemory
	stx screenmemory+1
printstring_call45032
	clc
	lda #<printstring_text78491
	adc #0
	ldy #>printstring_text78491
	sta print_text+0
	sty print_text+1
	ldx #14 ; optimized, look out for bugs
	jsr printstring
	; Assigning single variable : psnd1
	lda #<sndDead
	ldx #>sndDead
	sta psnd1
	stx psnd1+1
	; Assigning single variable : vsnd1
	lda #160
	sta vsnd1
	jsr PlaySnd1
	jsr CollisionWait
	rts
elseblock14398
elsedoneblock90371
elseblock17027
elsedoneblock2720
	; Binary clause: EQUALS
	lda px
	; Compare with pure num / var optimization
	cmp lx;keep
	; BC done
	bne binaryclausefailed30058
binaryclausesuccess80437
	lda #1; success
	jmp binaryclausefinished22328
binaryclausefailed30058
	lda #0 ; failed state
binaryclausefinished22328
	cmp #1
	beq ConditionalTrueBlock10541
	jmp elsedoneblock51503
ConditionalTrueBlock10541
	; Binary clause: EQUALS
	lda py
	; Compare with pure num / var optimization
	cmp ly;keep
	; BC done
	bne binaryclausefailed92905
binaryclausesuccess56675
	lda #1; success
	jmp binaryclausefinished10196
binaryclausefailed92905
	lda #0 ; failed state
binaryclausefinished10196
	cmp #1
	beq ConditionalTrueBlock14375
	jmp elsedoneblock93259
ConditionalTrueBlock14375
	; Assigning single variable : screenmemory
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda ly
	asl ; *2
	tax
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	
	sta screenmemory
	sty screenmemory+1
	; Assigning single variable : screenmemory
	lda #0
	ldy lx
	sta (screenmemory),y
	; Assigning single variable : pbgchar
	sta pbgchar
	; Assigning single variable : lostchildlevel
	; 8 bit binop
	; Add/sub where right value is constant number
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #2
val_var19639 = $54
	sta val_var19639
	jsr Random
	sec
modulo49319
	sbc val_var19639
	bcs modulo49319
	adc val_var19639
	
	clc
	adc #1
	 ; end add / sub var with constant
	
	clc
	adc lostchildlevel
	 ; end add / sub var with constant
	
	sta lostchildlevel
	; Assigning single variable : lx
	lda #0
	sta lx
	; Assigning single variable : ly
	sta ly
	; Binary clause Simplified: LESS
	lda lives
	; Compare with pure num / var optimization
	cmp #$a;keep
	bcs elsedoneblock73960
ConditionalTrueBlock87454
	; Assigning single variable : lives
	inc lives
elseblock22667
elsedoneblock73960
	; ----------
	; BcdAdd address, address, number
	sed
	clc
	lda score
	adc scWitch
	sta score
	lda score+$01
	adc scWitch+$01
	sta score+$01
	lda score+$02
	adc scWitch+$02
	sta score+$02
	lda score+$03
	adc scWitch+$03
	sta score+$03
	cld
	jsr DisplayScore
	; Assigning single variable : psnd3
	lda #<sndPickup
	ldx #>sndPickup
	sta psnd3
	stx psnd3+1
	; Assigning single variable : vsnd3
	lda #230
	sta vsnd3
	jsr PlaySnd3
elseblock37023
elsedoneblock93259
elseblock76021
elsedoneblock51503
	; Assigning single variable : screenmemory
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda py
	asl ; *2
	tax
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	
	sta screenmemory
	sty screenmemory+1
	; Binary clause Simplified: EQUALS
	lda px
	; Compare with pure num / var optimization
	cmp wx;keep
	bne elsedoneblock28875
ConditionalTrueBlock32038
	; Binary clause Simplified: EQUALS
	lda py
	; Compare with pure num / var optimization
	cmp wy;keep
	bne elsedoneblock88131
ConditionalTrueBlock38228
	; Assigning single variable : gameMode
	lda #1
	sta gameMode
	; Assigning single variable : screenmemory
	lda #48
	ldx #16
	sta screenmemory
	stx screenmemory+1
printstring_call44230
	clc
	lda #<printstring_text11577
	adc #0
	ldy #>printstring_text11577
	sta print_text+0
	sty print_text+1
	ldx #14 ; optimized, look out for bugs
	jsr printstring
	; Assigning single variable : psnd1
	lda #<sndDead
	ldx #>sndDead
	sta psnd1
	stx psnd1+1
	; Assigning single variable : vsnd1
	lda #160
	sta vsnd1
	jsr PlaySnd1
	jsr CollisionWait
	rts
elseblock77125
elsedoneblock88131
elseblock95397
elsedoneblock28875
	; Binary clause Simplified: EQUALS
	lda pbgchar
	; Compare with pure num / var optimization
	cmp #$26;keep
	bne elsedoneblock10947
ConditionalTrueBlock18190
	; Assigning single variable : gameMode
	lda #1
	sta gameMode
	; Assigning single variable : screenmemory
	lda #48
	ldx #16
	sta screenmemory
	stx screenmemory+1
printstring_call63007
	clc
	lda #<printstring_text5751
	adc #0
	ldy #>printstring_text5751
	sta print_text+0
	sty print_text+1
	ldx #14 ; optimized, look out for bugs
	jsr printstring
	; Assigning single variable : psnd1
	lda #<sndDead
	ldx #>sndDead
	sta psnd1
	stx psnd1+1
	; Assigning single variable : vsnd1
	lda #160
	sta vsnd1
	jsr PlaySnd1
	jsr CollisionWait
	rts
elseblock61690
elsedoneblock10947
	; Assigning single variable : i
	lda #0
	sta i
for99083
	; Binary clause Simplified: EQUALS
	lda px
binary_clause_temp_var90306 = $54
	sta binary_clause_temp_var90306
	; Load Byte array
	ldx i
	lda gx,x
	
binary_clause_temp_2_var1418 = $56
	sta binary_clause_temp_2_var1418
	lda binary_clause_temp_var90306
	cmp binary_clause_temp_2_var1418;keep
	bne elsedoneblock5611
ConditionalTrueBlock58976
	; Binary clause Simplified: EQUALS
	lda py
binary_clause_temp_var51477 = $54
	sta binary_clause_temp_var51477
	; Load Byte array
	ldx i
	lda gy,x
	
binary_clause_temp_2_var94218 = $56
	sta binary_clause_temp_2_var94218
	lda binary_clause_temp_var51477
	cmp binary_clause_temp_2_var94218;keep
	bne elsedoneblock82353
ConditionalTrueBlock31367
	; Assigning single variable : gameMode
	lda #1
	sta gameMode
	; Assigning single variable : screenmemory
	lda #48
	ldx #16
	sta screenmemory
	stx screenmemory+1
printstring_call86226
	clc
	lda #<printstring_text53694
	adc #0
	ldy #>printstring_text53694
	sta print_text+0
	sty print_text+1
	ldx #14 ; optimized, look out for bugs
	jsr printstring
	; Assigning single variable : psnd1
	lda #<sndDead
	ldx #>sndDead
	sta psnd1
	stx psnd1+1
	; Assigning single variable : vsnd1
	lda #160
	sta vsnd1
	jsr PlaySnd1
	jsr CollisionWait
	rts
elseblock6647
elsedoneblock82353
elseblock21738
elsedoneblock5611
	inc i
	lda gNumGhosts
	cmp i ;keep
	bne for99083
forLoopDone24150
	; Binary clause Simplified: GREATER
	lda zNumZombies
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock73924
	beq elsedoneblock73924
ConditionalTrueBlock7964
	; Assigning single variable : i
	lda #0
	sta i
for87633
	; Binary clause Simplified: EQUALS
	lda px
binary_clause_temp_var58289 = $54
	sta binary_clause_temp_var58289
	; Load Byte array
	ldx i
	lda zx,x
	
binary_clause_temp_2_var58790 = $56
	sta binary_clause_temp_2_var58790
	lda binary_clause_temp_var58289
	cmp binary_clause_temp_2_var58790;keep
	bne elsedoneblock56181
ConditionalTrueBlock29793
	; Binary clause Simplified: EQUALS
	lda py
binary_clause_temp_var1666 = $54
	sta binary_clause_temp_var1666
	; Load Byte array
	ldx i
	lda zy,x
	
binary_clause_temp_2_var50231 = $56
	sta binary_clause_temp_2_var50231
	lda binary_clause_temp_var1666
	cmp binary_clause_temp_2_var50231;keep
	bne elsedoneblock26130
ConditionalTrueBlock25327
	; Assigning single variable : gameMode
	lda #1
	sta gameMode
	; Assigning single variable : screenmemory
	lda #48
	ldx #16
	sta screenmemory
	stx screenmemory+1
printstring_call16454
	clc
	lda #<printstring_text57771
	adc #0
	ldy #>printstring_text57771
	sta print_text+0
	sty print_text+1
	ldx #15 ; optimized, look out for bugs
	jsr printstring
	; Assigning single variable : psnd1
	lda #<sndDead
	ldx #>sndDead
	sta psnd1
	stx psnd1+1
	; Assigning single variable : vsnd1
	lda #160
	sta vsnd1
	jsr PlaySnd1
	jsr CollisionWait
	rts
elseblock91009
elsedoneblock26130
elseblock35587
elsedoneblock56181
	inc i
	lda zNumZombies
	cmp i ;keep
	bne for87633
forLoopDone7603
elseblock59305
elsedoneblock73924
	rts
	
	
	; ***********  Defining procedure : HitWitch
	;    Procedure type : User-defined procedure
	
HitWitch
	; Assigning single variable : wbgchar
	lda #38
	sta wbgchar
	; Assigning single variable : wbgcol
	lda #7
	sta wbgcol
	; Assigning single variable : wdelay
	lda #50
	sta wdelay
	; Assigning single variable : wsteps
	lda #0
	sta wsteps
	; Assigning single variable : wdcount
	lda #7
	sta wdcount
	; Assigning single variable : wdspeed
	lda #2
	sta wdspeed
	; Assigning single variable : witchHits
	inc witchHits
	jsr DrawCandyCounter
	; Assigning single variable : screenmemory
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda cy
	asl ; *2
	tax
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	
	sta screenmemory
	sty screenmemory+1
	; Assigning single variable : screenmemory
	lda wbgchar
	ldy cx
	sta (screenmemory),y
	; Assigning single variable : p1
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda cy
	asl ; *2
	tax
	lda clr,x   ; Address of table lo
	ldy clr+1,x   ; Address of table hi
	
	sta p1
	sty p1+1
	; Assigning single variable : p1
	lda wbgcol
	ldy cx
	sta (p1),y
	jsr EndCandy
	; ----------
	; BcdAdd address, address, number
	sed
	clc
	lda score
	adc scWitch
	sta score
	lda score+$01
	adc scWitch+$01
	sta score+$01
	lda score+$02
	adc scWitch+$02
	sta score+$02
	lda score+$03
	adc scWitch+$03
	sta score+$03
	cld
	jsr DisplayScore
	; Assigning single variable : psndfx
	lda #<sndExplode
	ldx #>sndExplode
	sta psndfx
	stx psndfx+1
	; Assigning single variable : vsndfx
	lda #160
	sta vsndfx
	jsr PlaySndfx
	rts
	
	
	; ***********  Defining procedure : HitGhost
	;    Procedure type : User-defined procedure
	
HitGhost
	; Assigning single variable : gbgchar
	lda #0
	ldx i
	sta gbgchar,x
	; Assigning single variable : gbgcol
	sta gbgcol,x
	; Assigning single variable : gmode
	lda #15
	sta gmode,x
	; Assigning single variable : gx
	lda #1
	sta gx,x
	; Assigning single variable : gy
	sta gy,x
	; Assigning single variable : screenmemory
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda cy
	asl ; *2
	tax
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	
	sta screenmemory
	sty screenmemory+1
	; Assigning single variable : screenmemory
	lda #0
	ldy cx
	sta (screenmemory),y
	jsr EndCandy
	; ----------
	; BcdAdd address, address, number
	sed
	clc
	lda score
	adc scGhost
	sta score
	lda score+$01
	adc scGhost+$01
	sta score+$01
	lda score+$02
	adc scGhost+$02
	sta score+$02
	lda score+$03
	adc scGhost+$03
	sta score+$03
	cld
	jsr DisplayScore
	; Assigning single variable : psndfx
	lda #<sndExplode
	ldx #>sndExplode
	sta psndfx
	stx psndfx+1
	; Assigning single variable : vsndfx
	lda #160
	sta vsndfx
	jsr PlaySndfx
	rts
	
	
	; ***********  Defining procedure : HitZombie
	;    Procedure type : User-defined procedure
	
HitZombie
	; Assigning single variable : zspawn
	lda #10
	ldx i
	sta zspawn,x
	; Assigning single variable : screenmemory
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda cy
	asl ; *2
	tax
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	
	sta screenmemory
	sty screenmemory+1
	; Assigning single variable : screenmemory
	lda #42
	ldy cx
	sta (screenmemory),y
	jsr EndCandy
	; ----------
	; BcdAdd address, address, number
	sed
	clc
	lda score
	adc scZombie
	sta score
	lda score+$01
	adc scZombie+$01
	sta score+$01
	lda score+$02
	adc scZombie+$02
	sta score+$02
	lda score+$03
	adc scZombie+$03
	sta score+$03
	cld
	jsr DisplayScore
	; Assigning single variable : psndfx
	lda #<sndExplode
	ldx #>sndExplode
	sta psndfx
	stx psndfx+1
	; Assigning single variable : vsndfx
	lda #160
	sta vsndfx
	jsr PlaySndfx
	rts
	
	
	; ***********  Defining procedure : CollisionCandy
	;    Procedure type : User-defined procedure
	
CollisionCandy
	; Binary clause Simplified: EQUALS
	lda clife
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock97917
ConditionalTrueBlock78847
	rts
elseblock62975
elsedoneblock97917
	; Binary clause Simplified: EQUALS
	lda cx
	; Compare with pure num / var optimization
	cmp wx;keep
	bne elsedoneblock91953
ConditionalTrueBlock30758
	; Binary clause Simplified: EQUALS
	lda cy
	; Compare with pure num / var optimization
	cmp wy;keep
	bne elsedoneblock9428
ConditionalTrueBlock66092
	jsr HitWitch
elseblock87849
elsedoneblock9428
elseblock91586
elsedoneblock91953
	; Assigning single variable : i
	lda #0
	sta i
for77822
	; Binary clause Simplified: EQUALS
	lda cx
binary_clause_temp_var65979 = $54
	sta binary_clause_temp_var65979
	; Load Byte array
	ldx i
	lda gx,x
	
binary_clause_temp_2_var91647 = $56
	sta binary_clause_temp_2_var91647
	lda binary_clause_temp_var65979
	cmp binary_clause_temp_2_var91647;keep
	bne elsedoneblock26409
ConditionalTrueBlock2050
	; Binary clause Simplified: EQUALS
	lda cy
binary_clause_temp_var10076 = $54
	sta binary_clause_temp_var10076
	; Load Byte array
	ldx i
	lda gy,x
	
binary_clause_temp_2_var5979 = $56
	sta binary_clause_temp_2_var5979
	lda binary_clause_temp_var10076
	cmp binary_clause_temp_2_var5979;keep
	bne elsedoneblock67455
ConditionalTrueBlock94215
	jsr HitGhost
elseblock38993
elsedoneblock67455
elseblock62918
elsedoneblock26409
	inc i
	lda gNumGhosts
	cmp i ;keep
	bne for77822
forLoopDone45482
	; Binary clause Simplified: GREATER
	lda zNumZombies
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock55895
	beq elsedoneblock55895
ConditionalTrueBlock35811
	; Assigning single variable : i
	lda #0
	sta i
for27303
	; Binary clause Simplified: EQUALS
	lda cx
binary_clause_temp_var469 = $54
	sta binary_clause_temp_var469
	; Load Byte array
	ldx i
	lda zx,x
	
binary_clause_temp_2_var27459 = $56
	sta binary_clause_temp_2_var27459
	lda binary_clause_temp_var469
	cmp binary_clause_temp_2_var27459;keep
	bne elsedoneblock82669
ConditionalTrueBlock58825
	; Binary clause Simplified: EQUALS
	lda cy
binary_clause_temp_var77207 = $54
	sta binary_clause_temp_var77207
	; Load Byte array
	ldx i
	lda zy,x
	
binary_clause_temp_2_var47408 = $56
	sta binary_clause_temp_2_var47408
	lda binary_clause_temp_var77207
	cmp binary_clause_temp_2_var47408;keep
	bne elsedoneblock81851
ConditionalTrueBlock83190
	jsr HitZombie
elseblock1665
elsedoneblock81851
elseblock17357
elsedoneblock82669
	inc i
	lda zNumZombies
	cmp i ;keep
	bne for27303
forLoopDone99088
elseblock91975
elsedoneblock55895
	rts
	
	
	; ***********  Defining procedure : BeatWitchWait
	;    Procedure type : User-defined procedure
	
BeatWitchWait
	; Assigning single variable : gameMode
	lda #2
	sta gameMode
	; Assigning memory location
	; Assigning single variable : $900f
	lda #8
	sta $900f
	; Assigning single variable : screenmemory
	lda #46
	ldx #16
	sta screenmemory
	stx screenmemory+1
printstring_call82206
	clc
	lda #<printstring_text94828
	adc #0
	ldy #>printstring_text94828
	sta print_text+0
	sty print_text+1
	ldx #18 ; optimized, look out for bugs
	jsr printstring
	; Assigning single variable : p1
	lda #46
	ldx #148
	sta p1
	stx p1+1
	jsr callReadJoy1
	jsr callReadJoy1
	; Assigning single variable : joy1pressed
	lda #0
	sta joy1pressed
	; Assigning single variable : x
	lda #1
	sta x
	; Assigning single variable : y
	sta y
while87760
	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1pressed
	and #16
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock31354
ConditionalTrueBlock10085
	; Assigning single variable : x
	lda y
	sta x
	; Assigning single variable : y
	inc y
	; Binary clause Simplified: GREATER
	; Compare with pure num / var optimization
	cmp #$7;keep
	bcc elsedoneblock87307
	beq elsedoneblock87307
ConditionalTrueBlock46290
	; Assigning single variable : y
	lda #1
	sta y
elseblock53642
elsedoneblock87307
	jsr callReadJoy1
	; Assigning single variable : t
	lda #0
	sta t
for35013
	; Assigning single variable : p1
	lda x
	ldy t
	sta (p1),y
	; Assigning single variable : x
	inc x
	; Binary clause Simplified: GREATER
	; Compare with pure num / var optimization
	cmp #$7;keep
	bcc elsedoneblock42789
	beq elsedoneblock42789
ConditionalTrueBlock80300
	; Assigning single variable : x
	lda #1
	sta x
elseblock10471
elsedoneblock42789
	inc t
	lda #18
	cmp t ;keep
	bne for35013
forLoopDone15638
	jmp while87760
elseblock10268
elsedoneblock31354
	rts
	
	
	; ***********  Defining procedure : GameLoop
	;    Procedure type : User-defined procedure
	
GameLoop
while63904
	; Binary clause: EQUALS
	lda gameMode
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bne binaryclausefailed77773
binaryclausesuccess86128
	lda #1; success
	jmp binaryclausefinished63558
binaryclausefailed77773
	lda #0 ; failed state
binaryclausefinished63558
	cmp #1
	beq ConditionalTrueBlock40579
	jmp elsedoneblock64227
ConditionalTrueBlock40579
	jsr UpdateTimer
	jsr callReadJoy1
	jsr UpdatePlayer
	jsr CollisionPlayer
	jsr UpdateWitch
	jsr UpdateGhosts
	jsr UpdateZombies
	; Binary clause Simplified: EQUALS
	lda gameMode
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock90546
ConditionalTrueBlock87989
	jsr CollisionPlayer
elseblock35591
elsedoneblock90546
	jsr UpdateCandy
	jsr CollisionCandy
	; Binary clause Simplified: LESSEQUAL
	lda lostchildlevel
	; Compare with pure num / var optimization
	cmp level;keep
	beq ConditionalTrueBlock70159
	bcs elsedoneblock61679
ConditionalTrueBlock70159
	jsr UpdateLostChild
elseblock69532
elsedoneblock61679
	; Binary clause Simplified: EQUALS
	lda witchHits
	; Compare with pure num / var optimization
	cmp #$6;keep
	bne elsedoneblock18509
ConditionalTrueBlock66429
	jsr BeatWitchWait
elseblock41497
elsedoneblock18509
	; Assigning single variable : i
	lda #14
	
	sta i
	; Binary clause Simplified: LESS
	lda moonPos
	; Compare with pure num / var optimization
	cmp #$6;keep
	bcs elsedoneblock11058
ConditionalTrueBlock44146
	; Assigning single variable : i
	lda #12
	
	sta i
elseblock16336
elsedoneblock11058
	; Binary clause Simplified: GREATER
	lda moonPos
	; Compare with pure num / var optimization
	cmp #$11;keep
	bcc elsedoneblock16220
	beq elsedoneblock16220
ConditionalTrueBlock96827
	; Assigning single variable : i
	lda #10
	
	sta i
elseblock19332
elsedoneblock16220
	; Assigning memory location
	; Assigning single variable : $900f
	lda i
	sta $900f
while90048
	; Binary clause Simplified: EQUALS
	lda ticks
	; Compare with pure num / var optimization
	cmp oldTick;keep
	bne elsedoneblock41991
ConditionalTrueBlock61927
	jmp while90048
elseblock9802
elsedoneblock41991
	; Assigning single variable : oldTick
	lda ticks
	sta oldTick
	jmp while63904
elseblock90591
elsedoneblock64227
	rts
	
	
	; ***********  Defining procedure : SetUpGameLevel
	;    Procedure type : User-defined procedure
	
SetUpGameLevel
	jsr BlankScreen
	; 8 bit binop
	; Add/sub where right value is constant number
	lda level
	clc
	adc #15
	 ; end add / sub var with constant
	
	asl
	tax
	; ; Decrunch from index
	lda packedScreens,x
	sta opbase+1
	inx
	lda packedScreens,x
	sta opbase+2
	jsr exod_decrunch
	lda #14
	asl
	tax
	; ; Decrunch from index
	lda packedScreens,x
	sta opbase+1
	inx
	lda packedScreens,x
	sta opbase+2
	jsr exod_decrunch
	jsr ColourScreen
	jsr ColourHeader
	jsr DisplayTime
	jsr DrawCandyCounter
	; Assigning memory location
	; Assigning single variable : $900f
	lda #12
	
	sta $900f
	jsr SetupTimer
	; Assigning single variable : gameMode
	lda #0
	sta gameMode
	; Assigning single variable : zNumZombies
	sta zNumZombies
	; Assigning single variable : ztime
	; Load Byte array
	ldx level
	lda lzombie,x
	
	sta ztime
	; Assigning single variable : cx
	lda #0
	sta cx
	; Assigning single variable : cy
	sta cy
	; Assigning single variable : cdx
	sta cdx
	; Assigning single variable : lx
	sta lx
	; Assigning single variable : ly
	sta ly
	; Assigning single variable : candy
	sta candy
	; Assigning single variable : candyscr
	sta candyscr
	; Assigning single variable : clife
	sta clife
	jsr DrawCandyCounter
	jsr DisplayScore
	; Assigning single variable : wdspeed
	; Load Byte array
	ldx level
	lda lwitch,x
	
	sta wdspeed
	; Assigning single variable : wdcount
	lda #0
	sta wdcount
	; Assigning single variable : pd
	lda #58
	sta pd
	; Assigning single variable : wd
	lda #3
	sta wd
	jsr FindStartPos
	; Assigning single variable : gNumGhosts
	; Load Byte array
	ldx level
	lda lghosts,x
	
	sta gNumGhosts
	jsr PlaceAllGhosts
	rts
	
	
	; ***********  Defining procedure : DrawTitleFrame1
	;    Procedure type : User-defined procedure
	
DrawTitleFrame1
	; Assigning single variable : screenmemory
	lda #14
	ldy #0
	sta (screenmemory),y
	; Assigning single variable : screenmemory
	lda #15
	ldy #1
	sta (screenmemory),y
	rts
	
	
	; ***********  Defining procedure : DrawTitleFrame2
	;    Procedure type : User-defined procedure
	
DrawTitleFrame2
	; Assigning single variable : screenmemory
	lda #30
	ldy #0
	sta (screenmemory),y
	; Assigning single variable : screenmemory
	lda #31
	ldy #1
	sta (screenmemory),y
	rts
	
	
	; ***********  Defining procedure : ScreenPause
	;    Procedure type : User-defined procedure
	
ScreenPause
while16204
	; Binary clause Simplified: GREATER
	lda t
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock80685
	beq elsedoneblock80685
ConditionalTrueBlock77538
	; Assigning single variable : t
	dec t
	; Assigning single variable : i
	lda #0
	sta i
for80957
while42227
	; Binary clause Simplified: EQUALS
	lda ticks
	; Compare with pure num / var optimization
	cmp oldTick;keep
	bne elsedoneblock2944
ConditionalTrueBlock92983
	jmp while42227
elseblock34455
elsedoneblock2944
	; Assigning single variable : oldTick
	lda ticks
	sta oldTick
	jsr callReadJoy1
	; Binary clause Simplified: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1pressed
	and #16
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock202
ConditionalTrueBlock92819
	; Assigning single variable : t
	lda #0
	sta t
	; Assigning single variable : i
	lda #29
	sta i
elseblock96683
elsedoneblock202
	inc i
	lda #30
	cmp i ;keep
	bne for80957
forLoopDone4018
	jmp while16204
elseblock63593
elsedoneblock80685
	rts
	
	
	; ***********  Defining procedure : TitleScreen
	;    Procedure type : User-defined procedure
	
TitleScreen
	jsr callReadJoy1
	lda #2
	asl
	tax
	; ; Decrunch from index
	lda packedScreens,x
	sta opbase+1
	inx
	lda packedScreens,x
	sta opbase+2
	jsr exod_decrunch
	lda #0
	asl
	tax
	; ; Decrunch from index
	lda packedScreens,x
	sta opbase+1
	inx
	lda packedScreens,x
	sta opbase+2
	jsr exod_decrunch
	jsr ColourTitleScreen
	jsr SetupMusic
	; Assigning memory location
	; Assigning single variable : $900f
	lda #8
	sta $900f
	; Assigning single variable : screenmemory
	lda #135
	ldx #17
	sta screenmemory
	stx screenmemory+1
	; Assigning single variable : t
	lda #20
	sta t
while50128
	; Binary clause Simplified: GREATER
	lda t
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock94223
	beq elsedoneblock94223
ConditionalTrueBlock62649
	; Assigning single variable : j
	inc j
	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda j
	and #1
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$1;keep
	bne elseblock10799
ConditionalTrueBlock43493
	jsr DrawTitleFrame1
	jmp elsedoneblock40584
elseblock10799
	jsr DrawTitleFrame2
elsedoneblock40584
	; Assigning single variable : t
	dec t
	; Assigning single variable : i
	lda #0
	sta i
for86034
while96382
	; Binary clause Simplified: EQUALS
	lda ticks
	; Compare with pure num / var optimization
	cmp oldTick;keep
	bne elsedoneblock90800
ConditionalTrueBlock86058
	jmp while96382
elseblock21820
elsedoneblock90800
	; Assigning single variable : oldTick
	lda ticks
	sta oldTick
	jsr UpdateTitleMusic
	jsr callReadJoy1
	; Binary clause Simplified: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1pressed
	and #16
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock46276
ConditionalTrueBlock16246
	; Assigning single variable : t
	lda #0
	sta t
	; Assigning single variable : i
	lda #29
	sta i
elseblock91412
elsedoneblock46276
	inc i
	lda #30
	cmp i ;keep
	bne for86034
forLoopDone34660
	jmp while50128
elseblock97783
elsedoneblock94223
	; Poke
	; Optimization: shift is zero
	lda #15
	sta $900e
	rts
	
	
	; ***********  Defining procedure : TitleCredits
	;    Procedure type : User-defined procedure
	
TitleCredits
	lda #31
	asl
	tax
	; ; Decrunch from index
	lda packedScreens,x
	sta opbase+1
	inx
	lda packedScreens,x
	sta opbase+2
	jsr exod_decrunch
	lda #32
	asl
	tax
	; ; Decrunch from index
	lda packedScreens,x
	sta opbase+1
	inx
	lda packedScreens,x
	sta opbase+2
	jsr exod_decrunch
	jsr ColourCreditsScreen
	; Assigning single variable : t
	lda #10
	sta t
	jsr ScreenPause
	rts
	
	
	; ***********  Defining procedure : TitleStory
	;    Procedure type : User-defined procedure
	
TitleStory
	lda #1
	asl
	tax
	; ; Decrunch from index
	lda packedScreens,x
	sta opbase+1
	inx
	lda packedScreens,x
	sta opbase+2
	jsr exod_decrunch
	lda #8
	asl
	tax
	; ; Decrunch from index
	lda packedScreens,x
	sta opbase+1
	inx
	lda packedScreens,x
	sta opbase+2
	jsr exod_decrunch
	jsr ColourTitleScreen2
	; Assigning single variable : t
	lda #20
	sta t
	jsr ScreenPause
	rts
	
	
	; ***********  Defining procedure : TitleStory2
	;    Procedure type : User-defined procedure
	
TitleStory2
	lda #1
	asl
	tax
	; ; Decrunch from index
	lda packedScreens,x
	sta opbase+1
	inx
	lda packedScreens,x
	sta opbase+2
	jsr exod_decrunch
	lda #11
	asl
	tax
	; ; Decrunch from index
	lda packedScreens,x
	sta opbase+1
	inx
	lda packedScreens,x
	sta opbase+2
	jsr exod_decrunch
	jsr ColourTitleScreen2
	; Assigning single variable : t
	lda #20
	sta t
	jsr ScreenPause
	rts
	
	
	; ***********  Defining procedure : TitleCast
	;    Procedure type : User-defined procedure
	
TitleCast
	jsr BlankScreen
	lda #1
	asl
	tax
	; ; Decrunch from index
	lda packedScreens,x
	sta opbase+1
	inx
	lda packedScreens,x
	sta opbase+2
	jsr exod_decrunch
	lda #9
	asl
	tax
	; ; Decrunch from index
	lda packedScreens,x
	sta opbase+1
	inx
	lda packedScreens,x
	sta opbase+2
	jsr exod_decrunch
	jsr ColourTitleScreen2
	; Assigning single variable : t
	lda #20
	sta t
	jsr ScreenPause
	rts
	
	
	; ***********  Defining procedure : TitleHowTo
	;    Procedure type : User-defined procedure
	
TitleHowTo
	jsr BlankScreen
	lda #1
	asl
	tax
	; ; Decrunch from index
	lda packedScreens,x
	sta opbase+1
	inx
	lda packedScreens,x
	sta opbase+2
	jsr exod_decrunch
	lda #10
	asl
	tax
	; ; Decrunch from index
	lda packedScreens,x
	sta opbase+1
	inx
	lda packedScreens,x
	sta opbase+2
	jsr exod_decrunch
	jsr ColourTitleScreen2
	; Assigning single variable : t
	lda #20
	sta t
	jsr ScreenPause
	rts
	
	
	; ***********  Defining procedure : LastScore
	;    Procedure type : User-defined procedure
	
LastScore
	jsr BlankScreen
	lda #1
	asl
	tax
	; ; Decrunch from index
	lda packedScreens,x
	sta opbase+1
	inx
	lda packedScreens,x
	sta opbase+2
	jsr exod_decrunch
	lda #30
	asl
	tax
	; ; Decrunch from index
	lda packedScreens,x
	sta opbase+1
	inx
	lda packedScreens,x
	sta opbase+2
	jsr exod_decrunch
	; Assigning single variable : screenmemory
	
	; ----------
	; AddressTable address, xoffset, yoffset
	ldx #24
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	clc
	adc #$7
	bcc dtnooverflow87891
	iny  ; overflow into high byte
dtnooverflow87891
	
	sta screenmemory
	sty screenmemory+1
	; ----------
	; BcdPrint address, number
	ldy #$07 ; screen offset
	ldx #0 ; score byte index
bcdprintloop20567
	lda score,x
	inx
	pha
	and #$0f
	jsr bcdplotdigit
	pla
	lsr
	lsr
	lsr
	lsr
	jsr bcdplotdigit
	bpl bcdprintloop20567
	jsr ColourTitleScreen2
	; Assigning single variable : t
	lda #5
	sta t
	jsr ScreenPause
	rts
	
	
	; ***********  Defining procedure : LevelStart
	;    Procedure type : User-defined procedure
	
LevelStart
	jsr BlankScreen
	lda #1
	asl
	tax
	; ; Decrunch from index
	lda packedScreens,x
	sta opbase+1
	inx
	lda packedScreens,x
	sta opbase+2
	jsr exod_decrunch
	lda #3
	asl
	tax
	; ; Decrunch from index
	lda packedScreens,x
	sta opbase+1
	inx
	lda packedScreens,x
	sta opbase+2
	jsr exod_decrunch
	lda #14
	asl
	tax
	; ; Decrunch from index
	lda packedScreens,x
	sta opbase+1
	inx
	lda packedScreens,x
	sta opbase+2
	jsr exod_decrunch
	jsr ColourScreen
	jsr ColourHeader
	; Assigning single variable : screenmemory
	lda #134
	ldx #16
	sta screenmemory
	stx screenmemory+1
	; Binary clause Simplified: LESS
	lda level
	; Compare with pure num / var optimization
	cmp #$c;keep
	bcs elseblock12109
ConditionalTrueBlock78009
	; Assigning single variable : i
	; 8 bit binop
	; Add/sub where right value is constant number
	lda #12
	sec
	sbc level
	 ; end add / sub var with constant
	
	sta i
	ldy #0
	lda i
	sta ipd_div_lo
	sty ipd_div_hi
	ldy #1 ; optimized, look out for bugs
printdecimal17223
	jsr init_printdecimal_div10 
	ora #$30
	sta (screenmemory),y
	dey
	bpl printdecimal17223
	jmp elsedoneblock81019
elseblock12109
printstring_call44298
	clc
	lda #<printstring_text13606
	adc #0
	ldy #>printstring_text13606
	sta print_text+0
	sty print_text+1
	ldx #13 ; optimized, look out for bugs
	jsr printstring
elsedoneblock81019
	jsr DisplayScore
	; Assigning single variable : screenmemory
	lda #30
	ldx #17
	sta screenmemory
	stx screenmemory+1
	; Assigning single variable : n
	; 8 bit binop
	; Add/sub where right value is constant number
	lda lives
	clc
	adc #5
	 ; end add / sub var with constant
	
	sta n
	; Assigning single variable : i
	lda #5
	sta i
for43016
	; Assigning single variable : screenmemory
	lda #39
	ldy i
	sta (screenmemory),y
	inc i
	lda n
	cmp i ;keep
	bne for43016
forLoopDone20758
	; Assigning single variable : n
	dec n
	; Assigning single variable : x
	lda #0
	sta x
	; Assigning single variable : y
	lda #7
	sta y
	; Assigning single variable : screenmemory
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	asl ; *2
	tax
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	
	sta screenmemory
	sty screenmemory+1
	; Assigning single variable : p1
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda y
	asl ; *2
	tax
	lda clr,x   ; Address of table lo
	ldy clr+1,x   ; Address of table hi
	
	sta p1
	sty p1+1
	; Assigning single variable : pan
	lda #0
	sta pan
	; Assigning single variable : psnd1
	lda #<sndWitchDive
	ldx #>sndWitchDive
	sta psnd1
	stx psnd1+1
	; Assigning single variable : vsnd1
	lda #255
	sta vsnd1
	jsr PlaySnd1
	; Assigning single variable : t
	lda #80
	sta t
while96228
	; Binary clause: GREATER
	lda t
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bcc binaryclausefailed21008
	beq binaryclausefailed21008
binaryclausesuccess30123
	lda #1; success
	jmp binaryclausefinished15005
binaryclausefailed21008
	lda #0 ; failed state
binaryclausefinished15005
	cmp #1
	beq ConditionalTrueBlock2073
	jmp elsedoneblock39626
ConditionalTrueBlock2073
	; Assigning single variable : t
	dec t
	; Binary clause: LESS
	lda x
	; Compare with pure num / var optimization
	cmp #$15;keep
	; BC done
	bcs binaryclausefailed19640
binaryclausesuccess54062
	lda #1; success
	jmp binaryclausefinished50768
binaryclausefailed19640
	lda #0 ; failed state
binaryclausefinished50768
	cmp #1
	beq ConditionalTrueBlock69586
	jmp elsedoneblock42428
ConditionalTrueBlock69586
	; Assigning single variable : screenmemory
	lda #0
	ldy x
	sta (screenmemory),y
	; Assigning single variable : x
	inc x
	; Binary clause Simplified: LESS
	lda y
	; Compare with pure num / var optimization
	cmp #$c;keep
	bcs elsedoneblock66829
ConditionalTrueBlock66036
	; Assigning single variable : y
	inc y
elseblock8675
elsedoneblock66829
	; Assigning single variable : screenmemory
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda y
	asl ; *2
	tax
	lda scr,x   ; Address of table lo
	ldy scr+1,x   ; Address of table hi
	
	sta screenmemory
	sty screenmemory+1
	; Assigning single variable : p1
	
	; ----------
	; AddressTable address, xoffset, yoffset
	; yoffset is complex
	lda y
	asl ; *2
	tax
	lda clr,x   ; Address of table lo
	ldy clr+1,x   ; Address of table hi
	
	sta p1
	sty p1+1
	; Assigning single variable : screenmemory
	lda #33
	ldy x
	sta (screenmemory),y
	; Assigning single variable : p1
	lda #5
	sta (p1),y
	; Binary clause Simplified: EQUALS
	lda x
	; Compare with pure num / var optimization
	cmp n;keep
	bne elsedoneblock72699
ConditionalTrueBlock37215
	; Assigning single variable : screenmemory
	lda #62
	pha
	; 8 bit binop
	; Add/sub where right value is constant number
	lda x
	clc
	adc #22
	 ; end add / sub var with constant
	
	tay
	pla
	sta (screenmemory),y
	; Assigning single variable : p1
	lda #2
	pha
	; 8 bit binop
	; Add/sub where right value is constant number
	lda x
	clc
	adc #22
	 ; end add / sub var with constant
	
	tay
	pla
	sta (p1),y
	; Assigning single variable : psnd3
	lda #<sndWitchSpell
	ldx #>sndWitchSpell
	sta psnd3
	stx psnd3+1
	; Assigning single variable : vsnd3
	lda #195
	sta vsnd3
	jsr PlaySnd3
elseblock78726
elsedoneblock72699
	; Binary clause Simplified: EQUALS
	lda x
	; Compare with pure num / var optimization
	cmp #$15;keep
	bne elsedoneblock994
ConditionalTrueBlock9246
	; Assigning single variable : screenmemory
	lda #0
	ldy x
	sta (screenmemory),y
	; Assigning single variable : screenmemory
	lda #30
	ldx #17
	sta screenmemory
	stx screenmemory+1
	; Assigning single variable : p1
	ldx #149
	sta p1
	stx p1+1
	; Assigning single variable : pd
	lda #61
	sta pd
	; Assigning single variable : n
	lda n
	asl
	asl
	
	sta n
elseblock87660
elsedoneblock994
elseblock15215
elsedoneblock42428
	; Binary clause: EQUALS
	lda x
	; Compare with pure num / var optimization
	cmp #$15;keep
	; BC done
	bne binaryclausefailed24081
binaryclausesuccess49294
	lda #1; success
	jmp binaryclausefinished60707
binaryclausefailed24081
	lda #0 ; failed state
binaryclausefinished60707
	cmp #1
	beq ConditionalTrueBlock79145
	jmp elsedoneblock54353
ConditionalTrueBlock79145
	; Binary clause: LESS
	lda n
	; Compare with pure num / var optimization
	cmp #$58;keep
	; BC done
	bcs binaryclausefailed12977
binaryclausesuccess86264
	lda #1; success
	jmp binaryclausefinished57817
binaryclausefailed12977
	lda #0 ; failed state
binaryclausefinished57817
	cmp #1
	beq ConditionalTrueBlock23315
	jmp elsedoneblock5917
ConditionalTrueBlock23315
	; Assigning single variable : pan
	inc pan
	; Binary clause Simplified: GREATER
	lda pan
	; Compare with pure num / var optimization
	cmp #$5;keep
	bcc elsedoneblock71275
	beq elsedoneblock71275
ConditionalTrueBlock57142
	; Assigning single variable : pan
	lda #0
	sta pan
elseblock15709
elsedoneblock71275
	; Assigning single variable : screenmemory
	lda #0
	pha
	lda n
	lsr
	lsr
	
	tay
	pla
	sta (screenmemory),y
	; Assigning single variable : n
	inc n
	; Assigning single variable : screenmemory
	; 8 bit binop
	; Add/sub where right value is constant number
	; Load Byte array
	ldx pan
	lda panFrames,x
	
	clc
	adc pd
	 ; end add / sub var with constant
	
	pha
	lda n
	lsr
	lsr
	
	tay
	pla
	sta (screenmemory),y
	; Assigning single variable : p1
	lda #2
	pha
	lda n
	lsr
	lsr
	
	tay
	pla
	sta (p1),y
	; Assigning single variable : s
	inc s
	; Binary clause Simplified: EQUALS
	lda s
	; Compare with pure num / var optimization
	cmp #$40;keep
	bne elsedoneblock78849
ConditionalTrueBlock42902
	; Assigning single variable : s
	lda #61
	sta s
elseblock52101
elsedoneblock78849
	; Binary clause Simplified: EQUALS
	lda n
	; Compare with pure num / var optimization
	cmp #$58;keep
	bne elsedoneblock62720
ConditionalTrueBlock44267
	; Assigning single variable : screenmemory
	lda #0
	pha
	lda n
	lsr
	lsr
	
	tay
	pla
	sta (screenmemory),y
elseblock11548
elsedoneblock62720
	; Binary clause Simplified: EQUALS
	; Modulo
	lda #4
val_var36066 = $54
	sta val_var36066
	lda n
	sec
modulo52777
	sbc val_var36066
	bcs modulo52777
	adc val_var36066
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock33213
ConditionalTrueBlock33907
	; Assigning single variable : psnd1
	lda #<sndPumpkidFootsteps
	ldx #>sndPumpkidFootsteps
	sta psnd1
	stx psnd1+1
	; Assigning single variable : vsnd1
	; 8 bit binop
	; Add/sub where right value is constant number
	lda #160
	clc
	adc stepsnd
	 ; end add / sub var with constant
	
	sta vsnd1
	jsr PlaySnd1
	; Binary clause Simplified: EQUALS
	lda stepsnd
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elseblock50063
ConditionalTrueBlock40404
	; Assigning single variable : stepsnd
	lda #30
	sta stepsnd
	jmp elsedoneblock43281
elseblock50063
	; Assigning single variable : stepsnd
	lda #0
	sta stepsnd
elsedoneblock43281
elseblock37271
elsedoneblock33213
elseblock49115
elsedoneblock5917
elseblock12843
elsedoneblock54353
	; Assigning single variable : i
	lda #0
	sta i
for48628
while88229
	; Binary clause Simplified: EQUALS
	lda ticks
	; Compare with pure num / var optimization
	cmp oldTick;keep
	bne elsedoneblock36857
ConditionalTrueBlock35786
	jmp while88229
elseblock78198
elsedoneblock36857
	; Assigning single variable : oldTick
	lda ticks
	sta oldTick
	jsr callReadJoy1
	; Binary clause Simplified: LESS
	lda t
	; Compare with pure num / var optimization
	cmp #$3c;keep
	bcs elsedoneblock21992
ConditionalTrueBlock17140
	; Binary clause Simplified: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1
	and #16
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock1876
ConditionalTrueBlock52742
	; Assigning single variable : t
	lda #0
	sta t
elseblock89423
elsedoneblock1876
elseblock86254
elsedoneblock21992
	inc i
	lda #4
	cmp i ;keep
	bne for48628
forLoopDone10116
	jmp while96228
elseblock69818
elsedoneblock39626
	rts
	
	
	; ***********  Defining procedure : TitleFail
	;    Procedure type : User-defined procedure
	
TitleFail
	jsr BlankScreen
	lda #12
	asl
	tax
	; ; Decrunch from index
	lda packedScreens,x
	sta opbase+1
	inx
	lda packedScreens,x
	sta opbase+2
	jsr exod_decrunch
	lda #13
	asl
	tax
	; ; Decrunch from index
	lda packedScreens,x
	sta opbase+1
	inx
	lda packedScreens,x
	sta opbase+2
	jsr exod_decrunch
	jsr ColourFailScreen
	; Assigning memory location
	; Assigning single variable : $900f
	lda #8
	sta $900f
	; Assigning single variable : t
	lda #10
	sta t
	jsr ScreenPause
	rts
	
	
	; ***********  Defining procedure : TitleGameOver
	;    Procedure type : User-defined procedure
	
TitleGameOver
	jsr BlankScreen
	lda #4
	asl
	tax
	; ; Decrunch from index
	lda packedScreens,x
	sta opbase+1
	inx
	lda packedScreens,x
	sta opbase+2
	jsr exod_decrunch
	lda #6
	asl
	tax
	; ; Decrunch from index
	lda packedScreens,x
	sta opbase+1
	inx
	lda packedScreens,x
	sta opbase+2
	jsr exod_decrunch
	jsr ColourGameOverScreen
	; Assigning memory location
	; Assigning single variable : $900f
	lda #8
	sta $900f
	; Assigning single variable : t
	lda #10
	sta t
	jsr ScreenPause
	rts
	
	
	; ***********  Defining procedure : TitleGameWon
	;    Procedure type : User-defined procedure
	
TitleGameWon
	jsr BlankScreen
	lda #5
	asl
	tax
	; ; Decrunch from index
	lda packedScreens,x
	sta opbase+1
	inx
	lda packedScreens,x
	sta opbase+2
	jsr exod_decrunch
	lda #7
	asl
	tax
	; ; Decrunch from index
	lda packedScreens,x
	sta opbase+1
	inx
	lda packedScreens,x
	sta opbase+2
	jsr exod_decrunch
	jsr ColourGameWonScreen
	; Assigning memory location
	; Assigning single variable : $900f
	lda #8
	sta $900f
	; Assigning single variable : t
	lda #100
	sta t
	jsr ScreenPause
	rts
	
	
	; ***********  Defining procedure : TitleGameNext
	;    Procedure type : User-defined procedure
	
TitleGameNext
	jsr BlankScreen
	lda #28
	asl
	tax
	; ; Decrunch from index
	lda packedScreens,x
	sta opbase+1
	inx
	lda packedScreens,x
	sta opbase+2
	jsr exod_decrunch
	lda #29
	asl
	tax
	; ; Decrunch from index
	lda packedScreens,x
	sta opbase+1
	inx
	lda packedScreens,x
	sta opbase+2
	jsr exod_decrunch
	jsr ColourGameNextScreen
	; Assigning memory location
	; Assigning single variable : $900f
	lda #8
	sta $900f
	; Assigning single variable : t
	lda #10
	sta t
	jsr ScreenPause
	rts
	
	
	; ***********  Defining procedure : vbl
	;    Procedure type : User-defined procedure
	
vbl
	; StartIRQ
	pha
	txa
	pha
	tya
	pha
	; Assigning single variable : ticks
	; Modulo
	lda #100
val_var89571 = $54
	sta val_var89571
	; 8 bit binop
	; Add/sub where right value is constant number
	lda ticks
	clc
	adc #1
	 ; end add / sub var with constant
	sec
modulo46575
	sbc val_var89571
	bcs modulo46575
	adc val_var89571
	
	sta ticks
	; Binary clause Simplified: EQUALS
	lda sndFlag
	; Compare with pure num / var optimization
	cmp #$1;keep
	bne elsedoneblock15945
ConditionalTrueBlock25357
	jsr UpdateSound
elseblock24773
elsedoneblock15945
	; CloseIRQ
	pla
	tay
	pla
	tax
	pla
	 jmp $eabf     ; return to normal IRQ	
	rti
block6219
	jsr Initialise
	jsr SetupSound
	; Assigning single variable : level
	lda #0
	sta level
while22186
	; Binary clause: NOTEQUALS
	lda #1
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	beq binaryclausefailed56446
binaryclausesuccess62271
	lda #1; success
	jmp binaryclausefinished69022
binaryclausefailed56446
	lda #0 ; failed state
binaryclausefinished69022
	cmp #1
	beq ConditionalTrueBlock79531
	jmp elsedoneblock60530
ConditionalTrueBlock79531
	jsr BlankScreen
	lda level
	cmp #$0 ;keep
	bne casenext2039
	jsr TitleCredits
	jmp caseend1478
casenext2039
	lda level
	cmp #$1 ;keep
	bne casenext3419
	jsr TitleScreen
	jmp caseend1478
casenext3419
	lda level
	cmp #$2 ;keep
	bne casenext22439
	jsr LastScore
	jmp caseend1478
casenext22439
	lda level
	cmp #$3 ;keep
	bne casenext23537
	jsr TitleStory
	jmp caseend1478
casenext23537
	lda level
	cmp #$4 ;keep
	bne casenext75451
	jsr TitleStory2
	jmp caseend1478
casenext75451
	lda level
	cmp #$5 ;keep
	bne casenext681
	jsr TitleCast
	jmp caseend1478
casenext681
	lda level
	cmp #$6 ;keep
	bne casenext40296
	jsr TitleHowTo
	jmp caseend1478
casenext40296
caseend1478
	; Binary clause: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1
	and #16
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	beq binaryclausefailed56880
binaryclausesuccess62161
	lda #1; success
	jmp binaryclausefinished23201
binaryclausefailed56880
	lda #0 ; failed state
binaryclausefinished23201
	cmp #1
	beq ConditionalTrueBlock13081
	jmp elsedoneblock52512
ConditionalTrueBlock13081
	; Assigning single variable : psndfx
	lda #<sndShoot
	ldx #>sndShoot
	sta psndfx
	stx psndfx+1
	; Assigning single variable : vsndfx
	lda #254
	sta vsndfx
	jsr PlaySndfx
	; Assigning single variable : lives
	lda #5
	sta lives
	; Assigning single variable : score
	; ----------
	; BcdSub address, address, number
	sed
	sec
	lda score
	sbc score
	sta score
	lda score+$01
	sbc score+$01
	sta score+$01
	lda score+$02
	sbc score+$02
	sta score+$02
	lda score+$03
	sbc score+$03
	sta score+$03
	cld
	
	sta score
	; Assigning single variable : level
	lda #0
	sta level
	; Assigning single variable : witchHits
	sta witchHits
	; Assigning single variable : lostchildlevel
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #3
val_var39441 = $54
	sta val_var39441
	jsr Random
	sec
modulo66229
	sbc val_var39441
	bcs modulo66229
	adc val_var39441
	
	clc
	adc #1
	 ; end add / sub var with constant
	
	sta lostchildlevel
while85046
	; Binary clause: GREATER
	lda lives
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bcc binaryclausefailed38558
	beq binaryclausefailed38558
binaryclausesuccess24136
	lda #1; success
	jmp binaryclausefinished85923
binaryclausefailed38558
	lda #0 ; failed state
binaryclausefinished85923
	cmp #1
	beq ConditionalTrueBlock50266
	jmp elsedoneblock27320
ConditionalTrueBlock50266
	; 
	; ****** Inline assembler section
					lda $9004
					sta Random+1
				
	
	jsr SetupTimer
	jsr LevelStart
	jsr SetUpGameLevel
	jsr GameLoop
	; Binary clause Simplified: EQUALS
	lda gameMode
	; Compare with pure num / var optimization
	cmp #$1;keep
	bne elsedoneblock38867
ConditionalTrueBlock38783
	; Assigning single variable : lives
	dec lives
	; Binary clause Simplified: EQUALS
	lda lives
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elseblock16029
ConditionalTrueBlock68681
	jsr TitleGameOver
	jmp elsedoneblock11703
elseblock16029
	jsr TitleFail
elsedoneblock11703
elseblock95160
elsedoneblock38867
	; Binary clause Simplified: EQUALS
	lda gameMode
	; Compare with pure num / var optimization
	cmp #$2;keep
	bne elsedoneblock28807
ConditionalTrueBlock25765
	; Assigning single variable : level
	inc level
	; Binary clause Simplified: EQUALS
	lda level
	; Compare with pure num / var optimization
	cmp #$d;keep
	bne elseblock13027
ConditionalTrueBlock41876
	jsr TitleGameWon
	; Assigning single variable : lives
	lda #0
	sta lives
	jmp elsedoneblock84211
elseblock13027
	jsr TitleGameNext
	; Assigning single variable : witchHits
	lda #0
	sta witchHits
	; Binary clause Simplified: LESS
	lda lives
	; Compare with pure num / var optimization
	cmp #$5;keep
	bcs elsedoneblock68189
ConditionalTrueBlock34697
	; Binary clause Simplified: EQUALS
	lda level
	; Compare with pure num / var optimization
	cmp #$2;keep
	bne elsedoneblock1064
ConditionalTrueBlock77468
	; Assigning single variable : lives
	inc lives
elseblock49265
elsedoneblock1064
	; Binary clause Simplified: EQUALS
	lda level
	; Compare with pure num / var optimization
	cmp #$5;keep
	bne elsedoneblock76362
ConditionalTrueBlock31249
	; Assigning single variable : lives
	inc lives
elseblock97406
elsedoneblock76362
	; Binary clause Simplified: EQUALS
	lda level
	; Compare with pure num / var optimization
	cmp #$9;keep
	bne elsedoneblock91800
ConditionalTrueBlock57917
	; Assigning single variable : lives
	inc lives
elseblock69538
elsedoneblock91800
	; Binary clause Simplified: EQUALS
	lda level
	; Compare with pure num / var optimization
	cmp #$b;keep
	bne elsedoneblock95020
ConditionalTrueBlock92045
	; Assigning single variable : lives
	inc lives
elseblock4310
elsedoneblock95020
elseblock22386
elsedoneblock68189
elsedoneblock84211
elseblock3595
elsedoneblock28807
	jmp while85046
elseblock55284
elsedoneblock27320
	; Assigning single variable : level
	lda #0
	sta level
elseblock51498
elsedoneblock52512
	; Assigning single variable : level
	inc level
	; Binary clause Simplified: GREATER
	lda level
	; Compare with pure num / var optimization
	cmp #$6;keep
	bcc elsedoneblock67046
	beq elsedoneblock67046
ConditionalTrueBlock29098
	; Assigning single variable : level
	lda #0
	sta level
elseblock78851
elsedoneblock67046
	jmp while22186
elseblock95655
elsedoneblock60530
EndBlock61
	org $4800
titleScrn
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///compressed//titleScrn_c.bin"
	org $4877
gameChars
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///compressed//gameChars_c.bin"
	org $49e4
titleChars
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///compressed//titleChars_c.bin"
	org $4b40
scr00
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///compressed//scr00_c.bin"
	org $4ba7
gameoverChars
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///compressed//gameoverChars_c.bin"
	org $4d31
gamewonChars
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///compressed//gamewonChars_c.bin"
	org $4ec8
scrgameover
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///compressed//scrgameover_c.bin"
	org $4f54
scrgamewon
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///compressed//scrgamewon_c.bin"
	org $4fe5
titlestory
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///compressed//titlestory_c.bin"
	org $5097
titlecast
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///compressed//titlecast_c.bin"
	org $512b
scrhowto
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///compressed//scrhowto_c.bin"
	org $51d1
titlestory2
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///compressed//titlestory2_c.bin"
	org $5275
failchars
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///compressed//failchars_c.bin"
	org $5411
failscr
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///compressed//failscr_c.bin"
	org $549d
scrheader
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///compressed//scrheader_c.bin"
	org $54cd
scr01
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///compressed//scr01_c.bin"
	org $552a
scr02
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///compressed//scr02_c.bin"
	org $558e
scr03
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///compressed//scr03_c.bin"
	org $5603
scr04
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///compressed//scr04_c.bin"
	org $566e
scr05
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///compressed//scr05_c.bin"
	org $56d3
scr06
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///compressed//scr06_c.bin"
	org $5739
scr07
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///compressed//scr07_c.bin"
	org $579c
scr08
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///compressed//scr08_c.bin"
	org $5820
scr09
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///compressed//scr09_c.bin"
	org $5886
scr10
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///compressed//scr10_c.bin"
	org $58f0
scr11
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///compressed//scr11_c.bin"
	org $5960
scr12
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///compressed//scr12_c.bin"
	org $59d2
scr13
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///compressed//scr13_c.bin"
	org $5a39
gamenextchars
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///compressed//gamenextchars_c.bin"
	org $5b93
gamenextscr
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///compressed//gamenextscr_c.bin"
	org $5bf6
lastscorescr
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///compressed//lastscorescr_c.bin"
	org $5c36
creditschar
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///compressed//creditschar_c.bin"
	org $5d0c
creditscr
	incbin "/home/leuat/code/TRSE/tutorials/VIC20/PumpKid///compressed//creditscr_c.bin"
