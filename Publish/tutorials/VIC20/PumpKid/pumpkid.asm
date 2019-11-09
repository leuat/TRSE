 processor 6502

	ORG $1201
	.byte    $0E, $08, $0A, $00, $9E, $20, $28
	.byte   $34,$36,$32,$34
	.byte    $29, $00, $00, $00

	ORG $1210

Pumpkid
	jmp block87167
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
printstring_text77718	dc.b	"@OUT@OF@TIME@@"
	dc.b	0
printstring_text46827	dc.b	"@GOT@BY@WITCH@"
	dc.b	0
printstring_text48000	dc.b	"POISONED@SKULL"
	dc.b	0
printstring_text75413	dc.b	"@GOT@BY@GHOST@"
	dc.b	0
printstring_text50395	dc.b	"@GOT@BY@ZOMBIE@"
	dc.b	0
printstring_text39922	dc.b	"THE@WITCH@RETREATS"
	dc.b	0
printstring_text33464	dc.b	"GET@READY@FOR"
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
EndBlock973
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
	BMI JoySkip63777
	ORA #$02
JoySkip63777
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
	bcc RandomSkip98036
	eor #$4d
RandomSkip98036
	sta Random+1
	eor $9124
	rts
	
	
	; ***********  Defining procedure : PlaySnd1
	;    Procedure type : User-defined procedure
	
vsnd1Time	dc.b	$00
vsnd1	dc.b	
block25765
PlaySnd1
	; Assigning single variable : vsnd1Time
	lda #0
	sta vsnd1Time
	rts
	
	
	; ***********  Defining procedure : PlaySnd2
	;    Procedure type : User-defined procedure
	
vsnd2Time	dc.b	$00
vsnd2	dc.b	
block3595
PlaySnd2
	; Assigning single variable : vsnd2Time
	lda #0
	sta vsnd2Time
	rts
	
	
	; ***********  Defining procedure : PlaySnd3
	;    Procedure type : User-defined procedure
	
vsnd3Time	dc.b	$00
vsnd3	dc.b	
block28807
PlaySnd3
	; Assigning single variable : vsnd3Time
	lda #0
	sta vsnd3Time
	rts
	
	
	; ***********  Defining procedure : PlaySndfx
	;    Procedure type : User-defined procedure
	
vsndfxTime	dc.b	$00
vsndfx	dc.b	
block38754
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
block213
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
	bcc elsedoneblock35666
	beq elsedoneblock35666
ConditionalTrueBlock73882
	; Assigning single variable : vsnd1Time
	dec vsnd1Time
elseblock57539
elsedoneblock35666
	; Binary clause Simplified: EQUALS
	lda vsnd1Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock14149
ConditionalTrueBlock53612
	; Assigning single variable : psnd1
	; WORD optimization: a=a+b
	lda psnd1+0
	
	clc
	adc #2
	bcc WordAdd45669
	inc psnd1+1
WordAdd45669
	sta psnd1+0
elseblock97309
elsedoneblock14149
	rts
	
	
	; ***********  Defining procedure : Snd2Time
	;    Procedure type : User-defined procedure
	
Snd2Time
	; Binary clause Simplified: GREATER
	lda vsnd2Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock48891
	beq elsedoneblock48891
ConditionalTrueBlock66264
	; Assigning single variable : vsnd2Time
	dec vsnd2Time
elseblock88630
elsedoneblock48891
	; Binary clause Simplified: EQUALS
	lda vsnd2Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock78175
ConditionalTrueBlock74449
	; Assigning single variable : psnd2
	; WORD optimization: a=a+b
	lda psnd2+0
	
	clc
	adc #2
	bcc WordAdd94610
	inc psnd2+1
WordAdd94610
	sta psnd2+0
elseblock20377
elsedoneblock78175
	rts
	
	
	; ***********  Defining procedure : Snd3Time
	;    Procedure type : User-defined procedure
	
Snd3Time
	; Binary clause Simplified: GREATER
	lda vsnd3Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock68133
	beq elsedoneblock68133
ConditionalTrueBlock97099
	; Assigning single variable : vsnd3Time
	dec vsnd3Time
elseblock21869
elsedoneblock68133
	; Binary clause Simplified: EQUALS
	lda vsnd3Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock50846
ConditionalTrueBlock56354
	; Assigning single variable : psnd3
	; WORD optimization: a=a+b
	lda psnd3+0
	
	clc
	adc #2
	bcc WordAdd56813
	inc psnd3+1
WordAdd56813
	sta psnd3+0
elseblock20851
elsedoneblock50846
	rts
	
	
	; ***********  Defining procedure : SndfxTime
	;    Procedure type : User-defined procedure
	
SndfxTime
	; Binary clause Simplified: GREATER
	lda vsndfxTime
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock85634
	beq elsedoneblock85634
ConditionalTrueBlock89675
	; Assigning single variable : vsndfxTime
	dec vsndfxTime
elseblock29337
elsedoneblock85634
	; Binary clause Simplified: EQUALS
	lda vsndfxTime
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock44704
ConditionalTrueBlock7328
	; Assigning single variable : psndfx
	; WORD optimization: a=a+b
	lda psndfx+0
	
	clc
	adc #2
	bcc WordAdd18785
	inc psndfx+1
WordAdd18785
	sta psndfx+0
elseblock98105
elsedoneblock44704
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
	bne elsedoneblock80098
ConditionalTrueBlock13027
	; Binary clause Simplified: NOTEQUALS
	lda vsnd1
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock32313
ConditionalTrueBlock46245
	; Assigning single variable : vsnd1
	lda #0
	sta vsnd1
	; Poke
	; Optimization: shift is zero
	sta $900a
elseblock66056
elsedoneblock32313
	rts
elseblock84211
elsedoneblock80098
	; Binary clause: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd1),y
	
	; Compare with pure num / var optimization
	cmp #$20;keep
	; BC done
	bne tempfail6396
binaryclausesuccess92045
	jmp ConditionalTrueBlock12531
tempfail6396
	; Binary clause: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd1),y
	
	; Compare with pure num / var optimization
	cmp #$10;keep
	; BC done
	bne elseblock27002
binaryclausesuccess95020
ConditionalTrueBlock12531
	; Binary clause Simplified: EQUALS
	lda vsnd1Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock78851
ConditionalTrueBlock66343
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
	bne elsedoneblock50244
ConditionalTrueBlock36452
	; Poke
	; Optimization: shift is zero
	lda vsnd1
	sta $900a
elseblock56600
elsedoneblock50244
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd1),y
	
	; Compare with pure num / var optimization
	cmp #$10;keep
	bne elsedoneblock34570
ConditionalTrueBlock45090
	; Poke
	; Optimization: shift is zero
	lda #0
	sta $900a
elseblock7234
elsedoneblock34570
elseblock29098
elsedoneblock78851
	jsr Snd1Time
elseblock27002
elsedoneblock1659
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
	bne elsedoneblock80683
ConditionalTrueBlock61867
	; Binary clause Simplified: EQUALS
	lda vsnd1Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock12476
ConditionalTrueBlock66367
	; Assigning single variable : vsnd1Time
	; Load pointer array
	ldy #$1
	lda (psnd1),y
	
	sta vsnd1Time
elseblock62879
elsedoneblock12476
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
elseblock84950
elsedoneblock80683
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
	bne elsedoneblock9934
ConditionalTrueBlock87458
	; Binary clause Simplified: EQUALS
	lda vsnd1Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock79771
ConditionalTrueBlock90180
	; Assigning single variable : vsnd1Time
	; Load pointer array
	ldy #$1
	lda (psnd1),y
	
	sta vsnd1Time
elseblock85478
elsedoneblock79771
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
	
rightvarAddSub_var18550 = $54
	sta rightvarAddSub_var18550
	lda vsnd1
	sec
	sbc rightvarAddSub_var18550
	
	sta vsnd1
	; Poke
	; Optimization: shift is zero
	sta $900a
	jsr Snd1Time
elseblock95128
elsedoneblock9934
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
	bne elsedoneblock68277
ConditionalTrueBlock77750
	; Binary clause Simplified: NOTEQUALS
	lda vsnd2
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock35136
ConditionalTrueBlock50532
	; Assigning single variable : vsnd2
	lda #0
	sta vsnd2
	; Poke
	; Optimization: shift is zero
	sta $900b
elseblock1519
elsedoneblock35136
	rts
elseblock98506
elsedoneblock68277
	; Binary clause: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd2),y
	
	; Compare with pure num / var optimization
	cmp #$20;keep
	; BC done
	bne tempfail95727
binaryclausesuccess63546
	jmp ConditionalTrueBlock23492
tempfail95727
	; Binary clause: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd2),y
	
	; Compare with pure num / var optimization
	cmp #$10;keep
	; BC done
	bne elseblock29249
binaryclausesuccess11014
ConditionalTrueBlock23492
	; Binary clause Simplified: EQUALS
	lda vsnd2Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock74783
ConditionalTrueBlock54024
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
	bne elsedoneblock68523
ConditionalTrueBlock17216
	; Poke
	; Optimization: shift is zero
	lda vsnd2
	sta $900b
elseblock98569
elsedoneblock68523
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd2),y
	
	; Compare with pure num / var optimization
	cmp #$10;keep
	bne elsedoneblock2181
ConditionalTrueBlock43750
	; Poke
	; Optimization: shift is zero
	lda #0
	sta $900b
elseblock93119
elsedoneblock2181
elseblock85487
elsedoneblock74783
	jsr Snd2Time
elseblock29249
elsedoneblock42362
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
	bne elsedoneblock22750
ConditionalTrueBlock25140
	; Binary clause Simplified: EQUALS
	lda vsnd2Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock24259
ConditionalTrueBlock81702
	; Assigning single variable : vsnd2Time
	; Load pointer array
	ldy #$1
	lda (psnd2),y
	
	sta vsnd2Time
elseblock67051
elsedoneblock24259
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
elseblock71000
elsedoneblock22750
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
	bne elsedoneblock98163
ConditionalTrueBlock27118
	; Binary clause Simplified: EQUALS
	lda vsnd2Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock20965
ConditionalTrueBlock37012
	; Assigning single variable : vsnd2Time
	; Load pointer array
	ldy #$1
	lda (psnd2),y
	
	sta vsnd2Time
elseblock10072
elsedoneblock20965
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
	
rightvarAddSub_var31125 = $54
	sta rightvarAddSub_var31125
	lda vsnd2
	sec
	sbc rightvarAddSub_var31125
	
	sta vsnd2
	; Poke
	; Optimization: shift is zero
	sta $900b
	jsr Snd2Time
elseblock39810
elsedoneblock98163
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
	bne elsedoneblock92876
ConditionalTrueBlock11597
	; Binary clause Simplified: NOTEQUALS
	lda vsnd3
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock52041
ConditionalTrueBlock57130
	; Assigning single variable : vsnd3
	lda #0
	sta vsnd3
	; Poke
	; Optimization: shift is zero
	sta $900c
elseblock63461
elsedoneblock52041
	rts
elseblock79640
elsedoneblock92876
	; Binary clause: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd3),y
	
	; Compare with pure num / var optimization
	cmp #$20;keep
	; BC done
	bne tempfail38574
binaryclausesuccess75890
	jmp ConditionalTrueBlock38425
tempfail38574
	; Binary clause: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd3),y
	
	; Compare with pure num / var optimization
	cmp #$10;keep
	; BC done
	bne elseblock83540
binaryclausesuccess71903
ConditionalTrueBlock38425
	; Binary clause Simplified: EQUALS
	lda vsnd3Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock98918
ConditionalTrueBlock22101
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
	bne elsedoneblock95688
ConditionalTrueBlock78181
	; Poke
	; Optimization: shift is zero
	lda vsnd3
	sta $900c
elseblock52487
elsedoneblock95688
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd3),y
	
	; Compare with pure num / var optimization
	cmp #$10;keep
	bne elsedoneblock38492
ConditionalTrueBlock94185
	; Poke
	; Optimization: shift is zero
	lda #0
	sta $900c
elseblock92747
elsedoneblock38492
elseblock56669
elsedoneblock98918
	jsr Snd3Time
elseblock83540
elsedoneblock88317
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
	bne elsedoneblock66021
ConditionalTrueBlock6667
	; Binary clause Simplified: EQUALS
	lda vsnd3Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock17741
ConditionalTrueBlock73478
	; Assigning single variable : vsnd3Time
	; Load pointer array
	ldy #$1
	lda (psnd3),y
	
	sta vsnd3Time
elseblock51559
elsedoneblock17741
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
elseblock19456
elsedoneblock66021
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
	bne elsedoneblock35180
ConditionalTrueBlock54568
	; Binary clause Simplified: EQUALS
	lda vsnd3Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock43076
ConditionalTrueBlock43820
	; Assigning single variable : vsnd3Time
	; Load pointer array
	ldy #$1
	lda (psnd3),y
	
	sta vsnd3Time
elseblock84654
elsedoneblock43076
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
	
rightvarAddSub_var47567 = $54
	sta rightvarAddSub_var47567
	lda vsnd3
	sec
	sbc rightvarAddSub_var47567
	
	sta vsnd3
	; Poke
	; Optimization: shift is zero
	sta $900c
	jsr Snd3Time
elseblock17972
elsedoneblock35180
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
	bne elsedoneblock4090
ConditionalTrueBlock66376
	; Binary clause Simplified: NOTEQUALS
	lda vsndfx
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock19700
ConditionalTrueBlock18961
	; Assigning single variable : vsndfx
	lda #0
	sta vsndfx
	; Poke
	; Optimization: shift is zero
	sta $900d
elseblock16492
elsedoneblock19700
	rts
elseblock84362
elsedoneblock4090
	; Binary clause: EQUALS
	; Load pointer array
	ldy #$0
	lda (psndfx),y
	
	; Compare with pure num / var optimization
	cmp #$20;keep
	; BC done
	bne tempfail48529
binaryclausesuccess98603
	jmp ConditionalTrueBlock94604
tempfail48529
	; Binary clause: EQUALS
	; Load pointer array
	ldy #$0
	lda (psndfx),y
	
	; Compare with pure num / var optimization
	cmp #$10;keep
	; BC done
	bne elseblock13404
binaryclausesuccess93135
ConditionalTrueBlock94604
	; Binary clause Simplified: EQUALS
	lda vsndfxTime
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock57115
ConditionalTrueBlock90590
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
	bne elsedoneblock37138
ConditionalTrueBlock92453
	; Poke
	; Optimization: shift is zero
	lda vsndfx
	sta $900d
elseblock88308
elsedoneblock37138
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #$0
	lda (psndfx),y
	
	; Compare with pure num / var optimization
	cmp #$10;keep
	bne elsedoneblock33199
ConditionalTrueBlock29709
	; Poke
	; Optimization: shift is zero
	lda #0
	sta $900d
elseblock67994
elsedoneblock33199
elseblock89885
elsedoneblock57115
	jsr SndfxTime
elseblock13404
elsedoneblock57325
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
	bne elsedoneblock21294
ConditionalTrueBlock78963
	; Binary clause Simplified: EQUALS
	lda vsndfxTime
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock14644
ConditionalTrueBlock93787
	; Assigning single variable : vsndfxTime
	; Load pointer array
	ldy #$1
	lda (psndfx),y
	
	sta vsndfxTime
elseblock52097
elsedoneblock14644
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
elseblock68071
elsedoneblock21294
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
	bne elsedoneblock84579
ConditionalTrueBlock16894
	; Binary clause Simplified: EQUALS
	lda vsndfxTime
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock90884
ConditionalTrueBlock4487
	; Assigning single variable : vsndfxTime
	; Load pointer array
	ldy #$1
	lda (psndfx),y
	
	sta vsndfxTime
elseblock55576
elsedoneblock90884
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
	
rightvarAddSub_var27157 = $54
	sta rightvarAddSub_var27157
	lda vsndfx
	sec
	sbc rightvarAddSub_var27157
	
	sta vsndfx
	; Poke
	; Optimization: shift is zero
	sta $900d
	jsr SndfxTime
elseblock59234
elsedoneblock84579
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
block32976
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
	bne elsedoneblock65840
ConditionalTrueBlock54103
	rts
elseblock71690
elsedoneblock65840
	; Binary clause Simplified: EQUALS
	lda musicStep
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock86271
ConditionalTrueBlock72980
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
	bne elsedoneblock5196
ConditionalTrueBlock4061
	; Assigning single variable : musicNote
	lda #0
	sta musicNote
	; Assigning single variable : musicRepeat
	inc musicRepeat
	; Binary clause Simplified: EQUALS
	lda musicRepeat
	; Compare with pure num / var optimization
	cmp #$1;keep
	bne elsedoneblock32555
ConditionalTrueBlock345
	; Poke
	; Optimization: shift is zero
	lda #8
	sta $900e
elseblock20561
elsedoneblock32555
	; Binary clause Simplified: EQUALS
	lda musicRepeat
	; Compare with pure num / var optimization
	cmp #$2;keep
	bne elsedoneblock39582
ConditionalTrueBlock38717
	; Poke
	; Optimization: shift is zero
	lda #5
	sta $900e
elseblock35690
elsedoneblock39582
	; Binary clause Simplified: EQUALS
	lda musicRepeat
	; Compare with pure num / var optimization
	cmp #$3;keep
	bne elsedoneblock60392
ConditionalTrueBlock36938
	; Poke
	; Optimization: shift is zero
	lda #2
	sta $900e
elseblock31017
elsedoneblock60392
elseblock84705
elsedoneblock5196
elseblock82606
elsedoneblock86271
	; Assigning single variable : musicStep
	inc musicStep
	; Binary clause Simplified: EQUALS
	lda musicStep
	; Compare with pure num / var optimization
	cmp #$13;keep
	bne elsedoneblock80259
ConditionalTrueBlock54977
	; Assigning single variable : psnd3
	lda #<noSound
	ldx #>noSound
	sta psnd3
	stx psnd3+1
elseblock33596
elsedoneblock80259
	; Binary clause Simplified: EQUALS
	lda musicStep
	; Compare with pure num / var optimization
	cmp #$14;keep
	bne elsedoneblock86736
ConditionalTrueBlock98455
	; Assigning single variable : musicStep
	lda #0
	sta musicStep
elseblock21832
elsedoneblock86736
	rts
	
	
	; ***********  Defining procedure : UpdateLostChild
	;    Procedure type : User-defined procedure
	
UpdateLostChild
	; Binary clause: EQUALS
	lda moonPos
	; Compare with pure num / var optimization
	cmp #$8;keep
	; BC done
	bne binaryclausefailed83547
binaryclausesuccess56903
	lda #1; success
	jmp binaryclausefinished99226
binaryclausefailed83547
	lda #0 ; failed state
binaryclausefinished99226
	cmp #1
	beq ConditionalTrueBlock90294
	jmp elsedoneblock10059
ConditionalTrueBlock90294
	; Binary clause: EQUALS
	lda lx
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bne binaryclausefailed49646
binaryclausesuccess88786
	lda #1; success
	jmp binaryclausefinished18234
binaryclausefailed49646
	lda #0 ; failed state
binaryclausefinished18234
	cmp #1
	beq ConditionalTrueBlock19502
	jmp elsedoneblock15939
ConditionalTrueBlock19502
	; Binary clause: LESS
	lda level
	; Compare with pure num / var optimization
	cmp #$c;keep
	; BC done
	bcs binaryclausefailed99393
binaryclausesuccess24290
	lda #1; success
	jmp binaryclausefinished66818
binaryclausefailed99393
	lda #0 ; failed state
binaryclausefinished66818
	cmp #1
	beq ConditionalTrueBlock83086
	jmp elsedoneblock95027
ConditionalTrueBlock83086
	; Assigning single variable : i
	lda #0
	sta i
while41147
	; Binary clause: EQUALS
	lda i
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bne binaryclausefailed48430
binaryclausesuccess80879
	lda #1; success
	jmp binaryclausefinished55504
binaryclausefailed48430
	lda #0 ; failed state
binaryclausefinished55504
	cmp #1
	beq ConditionalTrueBlock87786
	jmp elsedoneblock35883
ConditionalTrueBlock87786
	; Assigning single variable : lx
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #19
val_var94232 = $54
	sta val_var94232
	jsr Random
	sec
modulo87758
	sbc val_var94232
	bcs modulo87758
	adc val_var94232
	
	clc
	adc #1
	 ; end add / sub var with constant
	
	sta lx
	; Assigning single variable : ly
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #18
val_var28804 = $54
	sta val_var28804
	jsr Random
	sec
modulo78186
	sbc val_var28804
	bcs modulo78186
	adc val_var28804
	
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
	bne elsedoneblock31636
ConditionalTrueBlock45004
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
elseblock34561
elsedoneblock31636
	jmp while41147
elseblock30701
elsedoneblock35883
elseblock80059
elsedoneblock95027
elseblock65730
elsedoneblock15939
elseblock18902
elsedoneblock10059
	; Binary clause Simplified: EQUALS
	lda moonPos
	; Compare with pure num / var optimization
	cmp #$a;keep
	bne elsedoneblock33786
ConditionalTrueBlock92956
	; Binary clause Simplified: NOTEQUALS
	lda lx
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock77358
ConditionalTrueBlock33776
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
val_var50011 = $54
	sta val_var50011
	jsr Random
	sec
modulo73865
	sbc val_var50011
	bcs modulo73865
	adc val_var50011
	
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
elseblock86284
elsedoneblock77358
elseblock71908
elsedoneblock33786
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
	bne elseblock84216
ConditionalTrueBlock66821
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
	jmp elsedoneblock98202
elseblock84216
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
elsedoneblock98202
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
dtloop51576
	tay
	lda scr,x
	inx
	inx
	clc
	adc #$16
	bcc dtnooverflow3785
	iny
dtnooverflow3785
	sta scr,x
	tya
	sta scr+1,x
	cpx #$2c
	bcc dtloop51576
	; ----------
	; DefineAddressTable address, StartValue, IncrementValue, TableSize
	ldy #>$9400
	lda #<$9400
	ldx #0
	sta clr,x   ; Address of table
	tya
	sta clr+1,x
dtloop34628
	tay
	lda clr,x
	inx
	inx
	clc
	adc #$16
	bcc dtnooverflow1912
	iny
dtnooverflow1912
	sta clr,x
	tya
	sta clr+1,x
	cpx #$2c
	bcc dtloop34628
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
bcdprintloop67711
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
	bpl bcdprintloop67711
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
for79540
	; Assigning single variable : j
	lda #0
	sta j
for36951
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
	bne for36951
forLoopDone65666
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd68569
	inc p1+1
WordAdd68569
	sta p1+0
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd39531
	inc screenmemory+1
WordAdd39531
	sta screenmemory+0
	inc i
	lda #23
	cmp i ;keep
	bne for79540
forLoopDone7246
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
for16633
	; Assigning single variable : j
	lda #0
	sta j
for6221
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
	bne for6221
forLoopDone97874
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd35672
	inc p1+1
WordAdd35672
	sta p1+0
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd86877
	inc screenmemory+1
WordAdd86877
	sta screenmemory+0
	inc i
	lda #23
	cmp i ;keep
	bne for16633
forLoopDone30509
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
for72692
	; Assigning single variable : j
	lda #0
	sta j
for21112
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
	bne for21112
forLoopDone4389
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd60548
	inc p1+1
WordAdd60548
	sta p1+0
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd27093
	inc screenmemory+1
WordAdd27093
	sta screenmemory+0
	inc i
	lda #23
	cmp i ;keep
	bne for72692
forLoopDone58994
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
for91735
	; Assigning single variable : j
	lda #0
	sta j
for12684
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
	bne for12684
forLoopDone2186
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd6126
	inc p1+1
WordAdd6126
	sta p1+0
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd93589
	inc screenmemory+1
WordAdd93589
	sta screenmemory+0
	inc i
	lda #23
	cmp i ;keep
	bne for91735
forLoopDone81409
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
for18475
	; Assigning single variable : j
	lda #0
	sta j
for75226
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
	bne for75226
forLoopDone14007
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd9608
	inc p1+1
WordAdd9608
	sta p1+0
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd10185
	inc screenmemory+1
WordAdd10185
	sta screenmemory+0
	inc i
	lda #23
	cmp i ;keep
	bne for18475
forLoopDone40972
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
for96879
	; Assigning single variable : j
	lda #0
	sta j
for78145
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
	bne for78145
forLoopDone25398
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd86405
	inc p1+1
WordAdd86405
	sta p1+0
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd8628
	inc screenmemory+1
WordAdd8628
	sta screenmemory+0
	inc i
	lda #23
	cmp i ;keep
	bne for96879
forLoopDone61804
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
for4072
	; Assigning single variable : j
	lda #0
	sta j
for67876
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
	bne for67876
forLoopDone81351
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd76224
	inc p1+1
WordAdd76224
	sta p1+0
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd8374
	inc screenmemory+1
WordAdd8374
	sta screenmemory+0
	inc i
	lda #23
	cmp i ;keep
	bne for4072
forLoopDone53384
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
for90538
	; Assigning single variable : j
	lda #0
	sta j
for93721
	; Assigning single variable : p1
	lda #0
	ldy j
	sta (p1),y
	inc j
	lda #22
	cmp j ;keep
	bne for93721
forLoopDone13736
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd72621
	inc p1+1
WordAdd72621
	sta p1+0
	inc i
	lda #23
	cmp i ;keep
	bne for90538
forLoopDone32072
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
for20797
	; Assigning single variable : j
	lda #0
	sta j
for2697
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
	bne for2697
forLoopDone84927
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd38809
	inc p1+1
WordAdd38809
	sta p1+0
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd21450
	inc screenmemory+1
WordAdd21450
	sta screenmemory+0
	inc i
	lda #20
	cmp i ;keep
	bne for20797
forLoopDone32731
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
for13617
	; Assigning single variable : screenmemory
	lda #3
	ldy i
	sta (screenmemory),y
	inc i
	lda #22
	cmp i ;keep
	bne for13617
forLoopDone38678
	; Assigning single variable : i
	lda #22
	sta i
for45745
	; Assigning single variable : screenmemory
	lda #0
	ldy i
	sta (screenmemory),y
	inc i
	lda #66
	cmp i ;keep
	bne for45745
forLoopDone99616
	; Assigning single variable : screenmemory
	lda #8
	ldx #148
	sta screenmemory
	stx screenmemory+1
	; Assigning single variable : i
	lda #0
	sta i
for721
	; Assigning single variable : screenmemory
	lda #2
	ldy i
	sta (screenmemory),y
	inc i
	lda #8
	cmp i ;keep
	bne for721
forLoopDone86813
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
for40220
	; Assigning single variable : x
	lda #1
	sta x
for76077
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy x
	lda (screenmemory),y
	
	; Compare with pure num / var optimization
	cmp #$3a;keep
	bne elsedoneblock58000
ConditionalTrueBlock29524
	; Assigning single variable : px
	lda x
	sta px
	; Assigning single variable : py
	lda y
	sta py
elseblock57860
elsedoneblock58000
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy x
	lda (screenmemory),y
	
	; Compare with pure num / var optimization
	cmp #$20;keep
	bne elsedoneblock97930
ConditionalTrueBlock1111
	; Assigning single variable : wx
	lda x
	sta wx
	; Assigning single variable : wy
	lda y
	sta wy
elseblock53768
elsedoneblock97930
	inc x
	lda #21
	cmp x ;keep
	bne for76077
forLoopDone17444
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd32276
	inc screenmemory+1
WordAdd32276
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
	bne for40220
forLoopDone12148
	rts
	
	
	; ***********  Defining procedure : WitchPickDirection
	;    Procedure type : User-defined procedure
	
WitchPickDirection
	; Binary clause Simplified: NOTEQUALS
	; Modulo
	lda #3
val_var8513 = $54
	sta val_var8513
	jsr Random
	sec
modulo37199
	sbc val_var8513
	bcs modulo37199
	adc val_var8513
	
	; Compare with pure num / var optimization
	cmp #$1;keep
	beq elsedoneblock88114
ConditionalTrueBlock8354
	; Binary clause Simplified: EQUALS
	lda wy
	; Compare with pure num / var optimization
	cmp py;keep
	bne elsedoneblock81342
ConditionalTrueBlock67731
	; Binary clause Simplified: GREATER
	lda wx
	; Compare with pure num / var optimization
	cmp px;keep
	bcc elseblock80636
	beq elseblock80636
ConditionalTrueBlock19076
	; Assigning single variable : wd
	lda #3
	sta wd
	jmp elsedoneblock26722
elseblock80636
	; Assigning single variable : wd
	lda #1
	sta wd
elsedoneblock26722
	rts
elseblock83032
elsedoneblock81342
	; Binary clause Simplified: EQUALS
	lda wx
	; Compare with pure num / var optimization
	cmp px;keep
	bne elsedoneblock76486
ConditionalTrueBlock1695
	; Binary clause Simplified: GREATER
	lda wy
	; Compare with pure num / var optimization
	cmp py;keep
	bcc elseblock46238
	beq elseblock46238
ConditionalTrueBlock11749
	; Assigning single variable : wd
	lda #0
	sta wd
	jmp elsedoneblock55327
elseblock46238
	; Assigning single variable : wd
	lda #2
	sta wd
elsedoneblock55327
	rts
elseblock34942
elsedoneblock76486
elseblock56791
elsedoneblock88114
	; Assigning single variable : wd
	inc wd
	; Binary clause Simplified: GREATER
	lda wd
	; Compare with pure num / var optimization
	cmp #$3;keep
	bcc elsedoneblock14385
	beq elsedoneblock14385
ConditionalTrueBlock17278
	; Assigning single variable : wd
	lda #0
	sta wd
elseblock7084
elsedoneblock14385
	rts
	
	
	; ***********  Defining procedure : UpdateWitch
	;    Procedure type : User-defined procedure
	
UpdateWitch
	; Binary clause: EQUALS
	lda wsteps
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bne binaryclausefailed65737
binaryclausesuccess66194
	lda #1; success
	jmp binaryclausefinished52065
binaryclausefailed65737
	lda #0 ; failed state
binaryclausefinished52065
	cmp #1
	beq ConditionalTrueBlock22445
	jmp elsedoneblock45486
ConditionalTrueBlock22445
	; Binary clause Simplified: EQUALS
	lda wdelay
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock34722
ConditionalTrueBlock32069
	; Assigning single variable : wdelay
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #40
val_var79860 = $54
	sta val_var79860
	jsr Random
	sec
modulo17648
	sbc val_var79860
	bcs modulo17648
	adc val_var79860
	
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
elseblock99319
elsedoneblock34722
	; Assigning single variable : wdelay
	dec wdelay
	; Binary clause Simplified: GREATER
	lda wdelay
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock3352
	beq elsedoneblock3352
ConditionalTrueBlock64967
	rts
elseblock62923
elsedoneblock3352
	jsr WitchPickDirection
	; Assigning single variable : wsteps
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #10
val_var21140 = $54
	sta val_var21140
	jsr Random
	sec
modulo93640
	sbc val_var21140
	bcs modulo93640
	adc val_var21140
	
	clc
	adc #2
	 ; end add / sub var with constant
	
	sta wsteps
	; Binary clause Simplified: GREATER
	lda wdcount
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock35719
	beq elsedoneblock35719
ConditionalTrueBlock65375
	; Assigning single variable : wdcount
	dec wdcount
	; Binary clause Simplified: EQUALS
	lda wdcount
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock96593
ConditionalTrueBlock23427
	; Assigning single variable : wdspeed
	; Load Byte array
	ldx level
	lda lwitch,x
	
	sta wdspeed
elseblock14513
elsedoneblock96593
elseblock99367
elsedoneblock35719
elseblock26448
elsedoneblock45486
	; Binary clause Simplified: NOTEQUALS
	; Modulo
	lda wdspeed
val_var75113 = $54
	sta val_var75113
	lda ticks
	sec
modulo86157
	sbc val_var75113
	bcs modulo86157
	adc val_var75113
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock4838
ConditionalTrueBlock70311
	rts
elseblock78174
elsedoneblock4838
	; Assigning single variable : wsteps
	dec wsteps
	lda wd
	cmp #$0 ;keep
	bne casenext68612
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
	bcs elseblock77971
ConditionalTrueBlock97685
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
	jmp elsedoneblock98256
elseblock77971
	; Assigning single variable : wsteps
	lda #0
	sta wsteps
elsedoneblock98256
	jmp caseend94818
casenext68612
	lda wd
	cmp #$1 ;keep
	bne casenext10544
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
	bcs elseblock44788
ConditionalTrueBlock93245
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
	jmp elsedoneblock93053
elseblock44788
	; Assigning single variable : wsteps
	lda #0
	sta wsteps
elsedoneblock93053
	jmp caseend94818
casenext10544
	lda wd
	cmp #$2 ;keep
	bne casenext14816
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
	bcs elseblock22163
ConditionalTrueBlock19604
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
	jmp elsedoneblock77500
elseblock22163
	; Assigning single variable : wsteps
	lda #0
	sta wsteps
elsedoneblock77500
	jmp caseend94818
casenext14816
	lda wd
	cmp #$3 ;keep
	bne casenext63084
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
	bcc dtnooverflow41629
	iny  ; overflow into high byte
dtnooverflow41629
	
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
	bcc dtnooverflow77122
	iny  ; overflow into high byte
dtnooverflow77122
	
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
	bcs elseblock45266
ConditionalTrueBlock39885
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
	jmp elsedoneblock27197
elseblock45266
	; Assigning single variable : wsteps
	lda #0
	sta wsteps
elsedoneblock27197
	jmp caseend94818
casenext63084
caseend94818
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
	bne elsedoneblock11237
ConditionalTrueBlock9699
	; Binary clause Simplified: EQUALS
	lda cy
	; Compare with pure num / var optimization
	cmp wy;keep
	bne elsedoneblock5128
ConditionalTrueBlock88875
	jsr HitWitch
elseblock84025
elsedoneblock5128
elseblock38311
elsedoneblock11237
	rts
	
	
	; ***********  Defining procedure : Ghost_Respawn
	;    Procedure type : User-defined procedure
	
Ghost_Respawn
	; Binary clause Simplified: GREATEREQUAL
	lda gcurrent
	; Compare with pure num / var optimization
	cmp gNumGhosts;keep
	bcc elsedoneblock29086
ConditionalTrueBlock21100
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
elseblock61686
elsedoneblock29086
	; Assigning single variable : j
	lda #0
	sta j
while72635
	; Binary clause: EQUALS
	lda j
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bne binaryclausefailed51213
binaryclausesuccess78068
	lda #1; success
	jmp binaryclausefinished66951
binaryclausefailed51213
	lda #0 ; failed state
binaryclausefinished66951
	cmp #1
	beq ConditionalTrueBlock81468
	jmp elsedoneblock12280
ConditionalTrueBlock81468
	; Assigning single variable : x
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #20
val_var13768 = $54
	sta val_var13768
	jsr Random
	sec
modulo18393
	sbc val_var13768
	bcs modulo18393
	adc val_var13768
	
	clc
	adc #1
	 ; end add / sub var with constant
	
	sta x
	; Assigning single variable : y
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #18
val_var83855 = $54
	sta val_var83855
	jsr Random
	sec
modulo71823
	sbc val_var83855
	bcs modulo71823
	adc val_var83855
	
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
	bcc dtnooverflow1462
	iny  ; overflow into high byte
dtnooverflow1462
	
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
	bne elsedoneblock14141
ConditionalTrueBlock45178
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
	bcs elseblock96879
ConditionalTrueBlock14816
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
	jmp elsedoneblock74931
elseblock96879
	; Assigning single variable : gmode
	lda #1
	ldx gcurrent
	sta gmode,x
	; Assigning single variable : gx
	lda #0
	sta gx,x
elsedoneblock74931
elseblock98977
elsedoneblock14141
	jmp while72635
elseblock95466
elsedoneblock12280
	rts
	
	
	; ***********  Defining procedure : PlaceAllGhosts
	;    Procedure type : User-defined procedure
	
PlaceAllGhosts
	; Assigning single variable : gcurrent
	lda #0
	sta gcurrent
for71445
	jsr Ghost_Respawn
	; Binary clause Simplified: LESS
	lda gcurrent
	; Compare with pure num / var optimization
	cmp gNumGhosts;keep
	bcs elsedoneblock96152
ConditionalTrueBlock84672
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
for59138
while33850
	; Binary clause Simplified: EQUALS
	lda ticks
	; Compare with pure num / var optimization
	cmp oldTick;keep
	bne elsedoneblock84633
ConditionalTrueBlock13405
	jmp while33850
elseblock47137
elsedoneblock84633
	; Assigning single variable : oldTick
	lda ticks
	sta oldTick
	inc i
	lda #30
	cmp i ;keep
	bne for59138
forLoopDone37590
elseblock75815
elsedoneblock96152
	inc gcurrent
	lda #10
	cmp gcurrent ;keep
	bne for71445
forLoopDone76234
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
for21408
forLoopFix61191
	; Binary clause Simplified: GREATER
	; Load Byte array
	ldx i
	lda gy,x
	
	; Compare with pure num / var optimization
	cmp #$3;keep
	bcc elsedoneblock12596
	beq elsedoneblock12596
ConditionalTrueBlock16996
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
elseblock7764
elsedoneblock12596
	inc i
	lda gNumGhosts
	cmp i ;keep
	beq forLoopDone91606
forLoopNotDone71690
	jmp for21408
forLoopDone91606
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
	bcs ColXConfirmed59815
	cmp #$8
	bcs NoCollision10071
ColXConfirmed59815
	lda y
	clc
	sbc py
	cmp #$fa
	bcs Collision92739
	cmp #$5
	bcs NoCollision10071
Collision92739
	lda #1
	jmp CollisionDone81806
NoCollision10071
	lda #0
CollisionDone81806
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	beq binaryclausefailed86528
binaryclausesuccess59473
	lda #1; success
	jmp binaryclausefinished79322
binaryclausefailed86528
	lda #0 ; failed state
binaryclausefinished79322
	cmp #1
	beq ConditionalTrueBlock5839
	jmp elseblock33428
	jmp elsedoneblock91743
ConditionalTrueBlock5839
	; Binary clause Simplified: EQUALS
	; Load Byte array
	ldx gcurrent
	lda gmode,x
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock35552
ConditionalTrueBlock49016
	; Binary clause Simplified: GREATER
	lda x
	; Compare with pure num / var optimization
	cmp px;keep
	bcc elsedoneblock56256
	beq elsedoneblock56256
ConditionalTrueBlock88789
	; Assigning single variable : x
	dec x
elseblock85480
elsedoneblock56256
	; Binary clause Simplified: LESS
	lda x
	; Compare with pure num / var optimization
	cmp px;keep
	bcs elsedoneblock69207
ConditionalTrueBlock67182
	; Assigning single variable : x
	inc x
elseblock43453
elsedoneblock69207
elseblock82893
elsedoneblock35552
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
	bcc dtnooverflow53361
	iny  ; overflow into high byte
dtnooverflow53361
	
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
	bcc elsedoneblock21787
	beq elsedoneblock21787
ConditionalTrueBlock68295
	; Assigning single variable : x
	lda n
	sta x
elseblock36255
elsedoneblock21787
	; Binary clause Simplified: EQUALS
	; Load Byte array
	ldx gcurrent
	lda gmode,x
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock75076
ConditionalTrueBlock55411
	; Binary clause Simplified: GREATER
	lda y
	; Compare with pure num / var optimization
	cmp py;keep
	bcc elsedoneblock574
	beq elsedoneblock574
ConditionalTrueBlock52352
	; Assigning single variable : y
	dec y
elseblock86772
elsedoneblock574
	; Binary clause Simplified: LESS
	lda y
	; Compare with pure num / var optimization
	cmp py;keep
	bcs elsedoneblock22895
ConditionalTrueBlock85092
	; Assigning single variable : y
	inc y
elseblock90698
elsedoneblock22895
elseblock16929
elsedoneblock75076
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
	bcc dtnooverflow70269
	iny  ; overflow into high byte
dtnooverflow70269
	
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
	bcc elsedoneblock22652
	beq elsedoneblock22652
ConditionalTrueBlock56618
	; Assigning single variable : y
	lda s
	sta y
elseblock87198
elsedoneblock22652
	jmp elsedoneblock91743
elseblock33428
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
	beq elseblock93701
ConditionalTrueBlock2501
	; Assigning single variable : x
	inc x
	jmp elsedoneblock50627
elseblock93701
	; Assigning single variable : x
	dec x
elsedoneblock50627
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
	bcc dtnooverflow47660
	iny  ; overflow into high byte
dtnooverflow47660
	
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
	bcc elsedoneblock82539
	beq elsedoneblock82539
ConditionalTrueBlock46523
	; Assigning single variable : x
	lda n
	sta x
elseblock90979
elsedoneblock82539
	; Binary clause Simplified: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda i
	and #2
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elseblock154
ConditionalTrueBlock14085
	; Assigning single variable : y
	dec y
	jmp elsedoneblock86105
elseblock154
	; Assigning single variable : y
	inc y
elsedoneblock86105
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
	bcc dtnooverflow38761
	iny  ; overflow into high byte
dtnooverflow38761
	
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
	bcc elsedoneblock65029
	beq elsedoneblock65029
ConditionalTrueBlock79269
	; Assigning single variable : y
	lda s
	sta y
elseblock86338
elsedoneblock65029
elsedoneblock91743
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
binary_clause_temp_var79082 = $54
	sta binary_clause_temp_var79082
	; Load Byte array
	ldx gcurrent
	lda gx,x
	
binary_clause_temp_2_var55633 = $56
	sta binary_clause_temp_2_var55633
	lda binary_clause_temp_var79082
	cmp binary_clause_temp_2_var55633;keep
	bne elsedoneblock71971
ConditionalTrueBlock5350
	; Binary clause Simplified: EQUALS
	lda cy
binary_clause_temp_var68146 = $54
	sta binary_clause_temp_var68146
	; Load Byte array
	ldx gcurrent
	lda gy,x
	
binary_clause_temp_2_var80163 = $56
	sta binary_clause_temp_2_var80163
	lda binary_clause_temp_var68146
	cmp binary_clause_temp_2_var80163;keep
	bne elsedoneblock41402
ConditionalTrueBlock55788
	; Assigning single variable : i
	lda gcurrent
	sta i
	jsr HitGhost
elseblock42793
elsedoneblock41402
elseblock40550
elsedoneblock71971
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
	bcc binaryclausefailed91076
	beq binaryclausefailed91076
binaryclausesuccess9817
	lda #1; success
	jmp binaryclausefinished83031
binaryclausefailed91076
	lda #0 ; failed state
binaryclausefinished83031
	cmp #1
	beq ConditionalTrueBlock36432
	jmp elsedoneblock91844
ConditionalTrueBlock36432
	; Assigning single variable : gtime
	lda #0
	sta gtime
	; Binary clause Simplified: LESS
	lda gcurrent
	; Compare with pure num / var optimization
	cmp gNumGhosts;keep
	bcs elsedoneblock43455
ConditionalTrueBlock62421
	; Binary clause Simplified: EQUALS
	; Load Byte array
	ldx gcurrent
	lda gmode,x
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock61823
ConditionalTrueBlock86480
	jsr Ghost_Move
elseblock7757
elsedoneblock61823
	; Binary clause Simplified: GREATER
	; Load Byte array
	ldx gcurrent
	lda gmode,x
	
	; Compare with pure num / var optimization
	cmp #$1;keep
	bcc elsedoneblock72485
	beq elsedoneblock72485
ConditionalTrueBlock98918
	; Assigning single variable : gmode
	ldx gcurrent
	; Optimize byte array dec 
	dec gmode,x
	; Binary clause Simplified: EQUALS
	; Load Byte array
	lda gmode,x
	
	; Compare with pure num / var optimization
	cmp #$1;keep
	bne elsedoneblock4212
ConditionalTrueBlock95556
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
elseblock87079
elsedoneblock4212
elseblock443
elsedoneblock72485
elseblock34205
elsedoneblock43455
	jsr DisplayGhosts
	; Assigning single variable : gcurrent
	inc gcurrent
	; Binary clause Simplified: EQUALS
	lda gcurrent
	; Compare with pure num / var optimization
	cmp #$a;keep
	bne elsedoneblock27440
ConditionalTrueBlock32949
	; Assigning single variable : gcurrent
	lda #0
	sta gcurrent
elseblock83437
elsedoneblock27440
elseblock30235
elsedoneblock91844
	rts
	
	
	; ***********  Defining procedure : SpawnAZombie
	;    Procedure type : User-defined procedure
	
SpawnAZombie
	; Binary clause Simplified: GREATEREQUAL
	lda zNumZombies
	; Compare with pure num / var optimization
	cmp #$a;keep
	bcc elsedoneblock66586
ConditionalTrueBlock30365
	rts
elseblock64182
elsedoneblock66586
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
val_var46291 = $54
	sta val_var46291
	jsr Random
	sec
modulo75571
	sbc val_var46291
	bcs modulo75571
	adc val_var46291
	
	sta t
	; Assigning single variable : y
	lda #3
	sta y
for84542
	; Assigning single variable : x
	lda #1
	sta x
for93785
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
	bne elsedoneblock64014
ConditionalTrueBlock51787
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
	bne elsedoneblock43171
ConditionalTrueBlock69028
	; Assigning single variable : x
	lda #20
	sta x
	; Assigning single variable : y
	lda #21
	sta y
elseblock44533
elsedoneblock43171
	; Assigning single variable : i
	inc i
elseblock14857
elsedoneblock64014
	inc x
	lda #21
	cmp x ;keep
	bne for93785
forLoopDone92498
	inc y
	lda #22
	cmp y ;keep
	bne for84542
forLoopDone34855
	; Binary clause Simplified: GREATER
	lda n
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock35878
	beq elsedoneblock35878
ConditionalTrueBlock45494
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
elseblock26005
elsedoneblock35878
	rts
	
	
	; ***********  Defining procedure : DisplayZombies
	;    Procedure type : User-defined procedure
	
DisplayZombies
	; Assigning single variable : i
	lda #0
	sta i
for37264
forLoopFix13265
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
	bne casenext56217
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
	jmp caseend46768
casenext56217
	lda t
	cmp #$1 ;keep
	bne casenext36829
	; Assigning single variable : screenmemory
	lda #40
	ldy x
	sta (screenmemory),y
	jmp caseend46768
casenext36829
	lda t
	cmp #$2 ;keep
	bne casenext89595
	; Assigning single variable : screenmemory
	lda #41
	ldy x
	sta (screenmemory),y
	jmp caseend46768
casenext89595
	lda t
	cmp #$3 ;keep
	bne casenext53642
	; Assigning single variable : screenmemory
	lda #42
	ldy x
	sta (screenmemory),y
	jmp caseend46768
casenext53642
	; Assigning single variable : screenmemory
	lda #42
	ldy x
	sta (screenmemory),y
caseend46768
	; Assigning single variable : p1
	lda #5
	ldy x
	sta (p1),y
	inc i
	lda zNumZombies
	cmp i ;keep
	beq forLoopDone96828
forLoopNotDone5605
	jmp for37264
forLoopDone96828
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
	beq binaryclausesuccess77963
	bcs binaryclausefailed16576
binaryclausesuccess77963
	lda #1; success
	jmp binaryclausefinished46831
binaryclausefailed16576
	lda #0 ; failed state
binaryclausefinished46831
	cmp #1
	beq ConditionalTrueBlock43767
	jmp elsedoneblock2059
ConditionalTrueBlock43767
	; Binary clause Simplified: GREATER
	lda x
	; Compare with pure num / var optimization
	cmp px;keep
	bcc elsedoneblock84396
	beq elsedoneblock84396
ConditionalTrueBlock45135
	; Assigning single variable : x
	dec x
elseblock64398
elsedoneblock84396
	; Binary clause Simplified: LESS
	lda x
	; Compare with pure num / var optimization
	cmp px;keep
	bcs elsedoneblock50997
ConditionalTrueBlock73627
	; Assigning single variable : x
	inc x
elseblock28187
elsedoneblock50997
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
	bcc dtnooverflow46868
	iny  ; overflow into high byte
dtnooverflow46868
	
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
	bcc elsedoneblock12818
	beq elsedoneblock12818
ConditionalTrueBlock87067
	; Assigning single variable : x
	lda n
	sta x
elseblock87997
elsedoneblock12818
	; Binary clause Simplified: GREATER
	lda y
	; Compare with pure num / var optimization
	cmp py;keep
	bcc elsedoneblock63249
	beq elsedoneblock63249
ConditionalTrueBlock10174
	; Assigning single variable : y
	dec y
elseblock97147
elsedoneblock63249
	; Binary clause Simplified: LESS
	lda y
	; Compare with pure num / var optimization
	cmp py;keep
	bcs elsedoneblock36515
ConditionalTrueBlock24910
	; Assigning single variable : y
	inc y
elseblock72888
elsedoneblock36515
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
	bcc dtnooverflow82196
	iny  ; overflow into high byte
dtnooverflow82196
	
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
	bcc elsedoneblock37120
	beq elsedoneblock37120
ConditionalTrueBlock62565
	; Assigning single variable : y
	lda s
	sta y
elseblock66592
elsedoneblock37120
elseblock36758
elsedoneblock2059
	; Binary clause: EQUALS
	lda t
	; Compare with pure num / var optimization
	cmp #$2;keep
	; BC done
	bne binaryclausefailed96664
binaryclausesuccess23293
	lda #1; success
	jmp binaryclausefinished95173
binaryclausefailed96664
	lda #0 ; failed state
binaryclausefinished95173
	cmp #1
	beq ConditionalTrueBlock47452
	jmp elsedoneblock61981
ConditionalTrueBlock47452
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
	beq elseblock60737
ConditionalTrueBlock38989
	; Assigning single variable : x
	inc x
	jmp elsedoneblock68591
elseblock60737
	; Assigning single variable : x
	dec x
elsedoneblock68591
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
	bcc dtnooverflow70801
	iny  ; overflow into high byte
dtnooverflow70801
	
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
	bcc elsedoneblock32060
	beq elsedoneblock32060
ConditionalTrueBlock2361
	; Assigning single variable : x
	lda n
	sta x
elseblock14926
elsedoneblock32060
	; Binary clause Simplified: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda i
	and #2
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elseblock55869
ConditionalTrueBlock15264
	; Assigning single variable : y
	dec y
	jmp elsedoneblock13714
elseblock55869
	; Assigning single variable : y
	inc y
elsedoneblock13714
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
	bcc dtnooverflow95669
	iny  ; overflow into high byte
dtnooverflow95669
	
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
	bcc elsedoneblock47775
	beq elsedoneblock47775
ConditionalTrueBlock54283
	; Assigning single variable : y
	lda s
	sta y
elseblock1163
elsedoneblock47775
elseblock74817
elsedoneblock61981
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
binary_clause_temp_var91957 = $54
	sta binary_clause_temp_var91957
	; Load Byte array
	ldx zcurrent
	lda zx,x
	
binary_clause_temp_2_var79782 = $56
	sta binary_clause_temp_2_var79782
	lda binary_clause_temp_var91957
	cmp binary_clause_temp_2_var79782;keep
	bne elsedoneblock9618
ConditionalTrueBlock26598
	; Binary clause Simplified: EQUALS
	lda cy
binary_clause_temp_var66483 = $54
	sta binary_clause_temp_var66483
	; Load Byte array
	ldx zcurrent
	lda zy,x
	
binary_clause_temp_2_var33237 = $56
	sta binary_clause_temp_2_var33237
	lda binary_clause_temp_var66483
	cmp binary_clause_temp_2_var33237;keep
	bne elsedoneblock37568
ConditionalTrueBlock52004
	; Assigning single variable : i
	lda zcurrent
	sta i
	jsr HitZombie
elseblock75765
elsedoneblock37568
elseblock60286
elsedoneblock9618
	rts
	
	
	; ***********  Defining procedure : UpdateZombies
	;    Procedure type : User-defined procedure
	
UpdateZombies
	; Binary clause Simplified: EQUALS
	lda zNumZombies
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock64932
ConditionalTrueBlock39205
	rts
elseblock18770
elsedoneblock64932
	; Binary clause Simplified: EQUALS
	; Modulo
	lda ztime
val_var82112 = $54
	sta val_var82112
	lda ticks
	sec
modulo13943
	sbc val_var82112
	bcs modulo13943
	adc val_var82112
	
	; Compare with pure num / var optimization
	cmp #$1;keep
	bne elsedoneblock39784
ConditionalTrueBlock25105
	; Binary clause Simplified: LESS
	lda zcurrent
	; Compare with pure num / var optimization
	cmp zNumZombies;keep
	bcs elsedoneblock96487
ConditionalTrueBlock62652
	; Binary clause Simplified: EQUALS
	; Load Byte array
	ldx zcurrent
	lda zspawn,x
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock47027
ConditionalTrueBlock36486
	jsr Zombie_Move
elseblock53499
elsedoneblock47027
	; Binary clause Simplified: GREATER
	; Load Byte array
	ldx zcurrent
	lda zspawn,x
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock61472
	beq elsedoneblock61472
ConditionalTrueBlock82664
	; Assigning single variable : zspawn
	ldx zcurrent
	; Optimize byte array dec 
	dec zspawn,x
elseblock19486
elsedoneblock61472
elseblock58256
elsedoneblock96487
	jsr DisplayZombies
	; Assigning single variable : zcurrent
	inc zcurrent
	; Binary clause Simplified: EQUALS
	lda zcurrent
	; Compare with pure num / var optimization
	cmp #$a;keep
	bne elsedoneblock4136
ConditionalTrueBlock92290
	; Assigning single variable : zcurrent
	lda #0
	sta zcurrent
elseblock12433
elsedoneblock4136
elseblock16706
elsedoneblock39784
	rts
	
	
	; ***********  Defining procedure : SpawnCandy
	;    Procedure type : User-defined procedure
	
SpawnCandy
	; Assigning single variable : x
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #20
val_var8160 = $54
	sta val_var8160
	jsr Random
	sec
modulo98481
	sbc val_var8160
	bcs modulo98481
	adc val_var8160
	
	clc
	adc #1
	 ; end add / sub var with constant
	
	sta x
	; Assigning single variable : y
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #16
val_var17731 = $54
	sta val_var17731
	jsr Random
	sec
modulo83675
	sbc val_var17731
	bcs modulo83675
	adc val_var17731
	
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
while76671
	; Binary clause Simplified: EQUALS
	lda t
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock23949
ConditionalTrueBlock82427
	; Binary clause Simplified: EQUALS
	; Load Byte array
	; Load pointer array
	ldy x
	lda (screenmemory),y
	
	tax
	lda cMask,x
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock88584
ConditionalTrueBlock79865
	; Assigning single variable : t
	lda #1
	sta t
elseblock34982
elsedoneblock88584
	; Binary clause Simplified: EQUALS
	lda t
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock98415
ConditionalTrueBlock67998
	; Assigning single variable : x
	inc x
	; Binary clause Simplified: GREATER
	lda x
	; Compare with pure num / var optimization
	cmp #$14;keep
	bcc elsedoneblock85143
	beq elsedoneblock85143
ConditionalTrueBlock8780
	; Assigning single variable : x
	lda #1
	sta x
	; Assigning single variable : y
	inc y
	; Binary clause Simplified: GREATER
	lda y
	; Compare with pure num / var optimization
	cmp #$16;keep
	bcc elsedoneblock32892
	beq elsedoneblock32892
ConditionalTrueBlock11016
	; Assigning single variable : y
	lda #4
	sta y
elseblock40448
elsedoneblock32892
elseblock76347
elsedoneblock85143
elseblock44023
elsedoneblock98415
	jmp while76671
elseblock90021
elsedoneblock23949
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
	bne elsedoneblock35607
ConditionalTrueBlock20269
	; Assigning single variable : psnd2
	lda #<sndCAppear
	ldx #>sndCAppear
	sta psnd2
	stx psnd2+1
	; Assigning single variable : vsnd2
	lda #250
	sta vsnd2
	jsr PlaySnd2
elseblock98892
elsedoneblock35607
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
	bcc elsedoneblock25920
	beq elsedoneblock25920
ConditionalTrueBlock81973
	; Assigning single variable : p1
	lda #3
	ldy cx
	sta (p1),y
elseblock32325
elsedoneblock25920
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
for17276
	; Assigning single variable : screenmemory
	lda #0
	ldy i
	sta (screenmemory),y
	inc i
	lda #6
	cmp i ;keep
	bcs for17276
forLoopDone40174
	; Assigning single variable : i
	lda #1
	sta i
for11703
	; Binary clause Simplified: GREATEREQUAL
	lda candy
	; Compare with pure num / var optimization
	cmp i;keep
	bcc elsedoneblock99706
ConditionalTrueBlock60542
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
elseblock30183
elsedoneblock99706
	; Binary clause Simplified: GREATEREQUAL
	lda witchHits
	; Compare with pure num / var optimization
	cmp i;keep
	bcc elsedoneblock5841
ConditionalTrueBlock48604
	; Assigning single variable : screenmemory
	lda #38
	ldy i
	sta (screenmemory),y
	; Assigning single variable : p1
	lda #1
	sta (p1),y
elseblock60642
elsedoneblock5841
	inc i
	lda #6
	cmp i ;keep
	bcs for11703
forLoopDone81876
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
	bne elsedoneblock55719
ConditionalTrueBlock2532
	; Assigning single variable : cx
	inc cx
elseblock64480
elsedoneblock55719
	; Binary clause Simplified: NOTEQUALS
	lda cdx
	; Compare with pure num / var optimization
	cmp #$1;keep
	beq elsedoneblock24865
ConditionalTrueBlock39387
	; Assigning single variable : cx
	dec cx
elseblock27407
elsedoneblock24865
	; Binary clause Simplified: EQUALS
	; Load Byte array
	; Load pointer array
	ldy cx
	lda (screenmemory),y
	
	tax
	lda cMask,x
	
	; Compare with pure num / var optimization
	cmp #$a;keep
	bne elsedoneblock47534
ConditionalTrueBlock55554
	; Assigning single variable : cx
	lda x
	sta cx
	; Binary clause Simplified: LESS
	lda candyscr
	; Compare with pure num / var optimization
	cmp #$4;keep
	bcs elsedoneblock61139
ConditionalTrueBlock86208
	; Assigning single variable : candyscr
	inc candyscr
elseblock24025
elsedoneblock61139
elseblock92267
elsedoneblock47534
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
	bne tempfail67394
binaryclausesuccess23156
	jmp ConditionalTrueBlock996
tempfail67394
	; Binary clause: EQUALS
	lda clife
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bne elseblock66659
binaryclausesuccess21261
ConditionalTrueBlock996
	jsr EndCandy
elseblock66659
elsedoneblock39828
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
	beq binaryclausefailed63655
binaryclausesuccess66615
	lda #1; success
	jmp binaryclausefinished56081
binaryclausefailed63655
	lda #0 ; failed state
binaryclausefinished56081
	cmp #1
	beq ConditionalTrueBlock16418
	jmp elsedoneblock56623
ConditionalTrueBlock16418
	; Binary clause: EQUALS
	lda clife
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bne binaryclausefailed45033
binaryclausesuccess56530
	lda #1; success
	jmp binaryclausefinished29067
binaryclausefailed45033
	lda #0 ; failed state
binaryclausefinished29067
	cmp #1
	beq ConditionalTrueBlock31355
	jmp elsedoneblock76647
ConditionalTrueBlock31355
	; Binary clause: GREATER
	lda candy
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bcc binaryclausefailed68378
	beq binaryclausefailed68378
binaryclausesuccess94404
	lda #1; success
	jmp binaryclausefinished41786
binaryclausefailed68378
	lda #0 ; failed state
binaryclausefinished41786
	cmp #1
	beq ConditionalTrueBlock77679
	jmp elsedoneblock45269
ConditionalTrueBlock77679
	; Assigning single variable : cy
	lda py
	sta cy
	; Binary clause Simplified: EQUALS
	lda pd
	; Compare with pure num / var optimization
	cmp #$3a;keep
	bne elsedoneblock37522
ConditionalTrueBlock67971
	; Assigning single variable : cdx
	lda #255
	sta cdx
elseblock5635
elsedoneblock37522
	; Binary clause Simplified: EQUALS
	lda pd
	; Compare with pure num / var optimization
	cmp #$3d;keep
	bne elsedoneblock33732
ConditionalTrueBlock90267
	; Assigning single variable : cdx
	lda #1
	sta cdx
elseblock92371
elsedoneblock33732
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
	bne elsedoneblock3708
ConditionalTrueBlock37589
	; Assigning single variable : x
	; 8 bit binop
	; Add/sub where right value is constant number
	lda px
	clc
	adc #1
	 ; end add / sub var with constant
	
	sta x
elseblock61764
elsedoneblock3708
	; Binary clause Simplified: NOTEQUALS
	lda cdx
	; Compare with pure num / var optimization
	cmp #$1;keep
	beq elsedoneblock39392
ConditionalTrueBlock98784
	; Assigning single variable : x
	; 8 bit binop
	; Add/sub where right value is constant number
	lda px
	sec
	sbc #1
	 ; end add / sub var with constant
	
	sta x
elseblock54365
elsedoneblock39392
	; Binary clause Simplified: EQUALS
	; Load Byte array
	; Load pointer array
	ldy x
	lda (screenmemory),y
	
	tax
	lda cMask,x
	
	; Compare with pure num / var optimization
	cmp #$a;keep
	bne elseblock73587
ConditionalTrueBlock96983
	; Assigning single variable : cy
	lda #0
	sta cy
	; Assigning single variable : cx
	sta cx
	; Assigning single variable : cdx
	sta cdx
	jmp elsedoneblock28686
elseblock73587
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
elsedoneblock28686
elseblock91893
elsedoneblock45269
elseblock36313
elsedoneblock76647
elseblock7847
elsedoneblock56623
	; Binary clause Simplified: GREATER
	lda clife
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock29954
	beq elsedoneblock29954
ConditionalTrueBlock26759
	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda ticks
	and #1
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock41551
ConditionalTrueBlock36196
	jsr MoveCandy
elseblock91980
elsedoneblock41551
elseblock75155
elsedoneblock29954
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
	beq binaryclausefailed70727
binaryclausesuccess90504
	lda #1; success
	jmp binaryclausefinished63697
binaryclausefailed70727
	lda #0 ; failed state
binaryclausefinished63697
	cmp #1
	beq ConditionalTrueBlock16639
	jmp elsedoneblock92934
ConditionalTrueBlock16639
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
	bcc dtnooverflow45472
	iny  ; overflow into high byte
dtnooverflow45472
	
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
	bcc dtnooverflow45533
	iny  ; overflow into high byte
dtnooverflow45533
	
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
	bcs elsedoneblock79036
ConditionalTrueBlock53964
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
elseblock92107
elsedoneblock79036
	rts
elseblock96211
elsedoneblock92934
	; Binary clause: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1
	and #1
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	beq binaryclausefailed23653
binaryclausesuccess8548
	lda #1; success
	jmp binaryclausefinished79613
binaryclausefailed23653
	lda #0 ; failed state
binaryclausefinished79613
	cmp #1
	beq ConditionalTrueBlock92511
	jmp elsedoneblock80776
ConditionalTrueBlock92511
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
	bcs elsedoneblock55237
ConditionalTrueBlock48205
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
elseblock59602
elsedoneblock55237
	rts
elseblock71502
elsedoneblock80776
	; Binary clause: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1
	and #2
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	beq binaryclausefailed48654
binaryclausesuccess75615
	lda #1; success
	jmp binaryclausefinished16956
binaryclausefailed48654
	lda #0 ; failed state
binaryclausefinished16956
	cmp #1
	beq ConditionalTrueBlock63993
	jmp elsedoneblock47406
ConditionalTrueBlock63993
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
	bcs elsedoneblock25202
ConditionalTrueBlock33355
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
elseblock44824
elsedoneblock25202
	rts
elseblock97243
elsedoneblock47406
	; Binary clause: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1
	and #4
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	beq binaryclausefailed10708
binaryclausesuccess6025
	lda #1; success
	jmp binaryclausefinished67199
binaryclausefailed10708
	lda #0 ; failed state
binaryclausefinished67199
	cmp #1
	beq ConditionalTrueBlock49716
	jmp elsedoneblock35140
ConditionalTrueBlock49716
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
	bcs elsedoneblock70520
ConditionalTrueBlock60228
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
elseblock19216
elsedoneblock70520
	rts
elseblock89876
elsedoneblock35140
	rts
	
	
	; ***********  Defining procedure : UpdatePlayer
	;    Procedure type : User-defined procedure
	
UpdatePlayer
	; Binary clause Simplified: EQUALS
	; Modulo
	lda #2
val_var33206 = $54
	sta val_var33206
	lda ticks
	sec
modulo31257
	sbc val_var33206
	bcs modulo31257
	adc val_var33206
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock65189
ConditionalTrueBlock16668
	; Assigning single variable : pan
	inc pan
elseblock53303
elsedoneblock65189
	; Binary clause Simplified: GREATER
	lda pan
	; Compare with pure num / var optimization
	cmp #$5;keep
	bcc elsedoneblock88208
	beq elsedoneblock88208
ConditionalTrueBlock7354
	; Assigning single variable : pan
	lda #0
	sta pan
elseblock48181
elsedoneblock88208
	; Binary clause Simplified: EQUALS
	lda joy1
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock69030
ConditionalTrueBlock84765
	; Assigning single variable : pstep
	lda #0
	sta pstep
elseblock8940
elsedoneblock69030
	; Binary clause Simplified: GREATER
	lda pstep
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock18657
	beq elsedoneblock18657
ConditionalTrueBlock83412
	; Assigning single variable : pstep
	dec pstep
elseblock19411
elsedoneblock18657
	; Binary clause Simplified: EQUALS
	lda pstep
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock26070
ConditionalTrueBlock6878
	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1
	and #16
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock72783
ConditionalTrueBlock74801
	jsr MovePlayer
	; Binary clause Simplified: GREATER
	lda pstep
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock47335
	beq elsedoneblock47335
ConditionalTrueBlock50359
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
	bne elseblock14815
ConditionalTrueBlock25017
	; Assigning single variable : stepsnd
	lda #30
	sta stepsnd
	jmp elsedoneblock17249
elseblock14815
	; Assigning single variable : stepsnd
	lda #0
	sta stepsnd
elsedoneblock17249
elseblock52226
elsedoneblock47335
elseblock3207
elsedoneblock72783
elseblock9962
elsedoneblock26070
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
	bne elsedoneblock93988
ConditionalTrueBlock11763
	; Binary clause Simplified: LESS
	lda candy
binary_clause_temp_var5062 = $54
	sta binary_clause_temp_var5062
	; 8 bit binop
	; Add/sub where right value is constant number
	lda #6
	sec
	sbc witchHits
	 ; end add / sub var with constant
	
binary_clause_temp_2_var59867 = $56
	sta binary_clause_temp_2_var59867
	lda binary_clause_temp_var5062
	cmp binary_clause_temp_2_var59867;keep
	bcs elsedoneblock91664
ConditionalTrueBlock30180
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
	bcc elsedoneblock72033
	beq elsedoneblock72033
ConditionalTrueBlock43939
	; Assigning single variable : candyscr
	dec candyscr
elseblock24677
elsedoneblock72033
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
elseblock60517
elsedoneblock91664
elseblock1461
elsedoneblock93988
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
	bcc elsedoneblock98702
ConditionalTrueBlock86917
	jsr SpawnAZombie
elseblock22754
elsedoneblock98702
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
for28141
	; Assigning single variable : screenmemory
	lda #0
	ldy i
	sta (screenmemory),y
	inc i
	lda #8
	cmp i ;keep
	bne for28141
forLoopDone71677
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #2
	bcc WordAdd17195
	inc screenmemory+1
WordAdd17195
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
	bne casenext50766
	jsr ClearTime
	clc
	lda #<t11
	adc #0
	ldy #>t11
	sta print_text+0
	sty print_text+1
	ldx #4 ; optimized, look out for bugs
	jsr printstring
	jmp caseend28121
casenext50766
	lda moonPos
	cmp #$6 ;keep
	bne casenext92923
	jsr ClearTime
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	sec
	sbc #2
	bcs WordAdd16090
	dec screenmemory+1
WordAdd16090
	sta screenmemory+0
	clc
	lda #<t12
	adc #0
	ldy #>t12
	sta print_text+0
	sty print_text+1
	ldx #8 ; optimized, look out for bugs
	jsr printstring
	jmp caseend28121
casenext92923
	lda moonPos
	cmp #$8 ;keep
	bne casenext39146
	jsr ClearTime
	clc
	lda #<t01
	adc #0
	ldy #>t01
	sta print_text+0
	sty print_text+1
	ldx #4 ; optimized, look out for bugs
	jsr printstring
	jmp caseend28121
casenext39146
	lda moonPos
	cmp #$a ;keep
	bne casenext73181
	jsr ClearTime
	clc
	lda #<t02
	adc #0
	ldy #>t02
	sta print_text+0
	sty print_text+1
	ldx #4 ; optimized, look out for bugs
	jsr printstring
	jmp caseend28121
casenext73181
	lda moonPos
	cmp #$c ;keep
	bne casenext52701
	jsr ClearTime
	clc
	lda #<t03
	adc #0
	ldy #>t03
	sta print_text+0
	sty print_text+1
	ldx #4 ; optimized, look out for bugs
	jsr printstring
	jmp caseend28121
casenext52701
	lda moonPos
	cmp #$e ;keep
	bne casenext24071
	jsr ClearTime
	clc
	lda #<t04
	adc #0
	ldy #>t04
	sta print_text+0
	sty print_text+1
	ldx #4 ; optimized, look out for bugs
	jsr printstring
	jmp caseend28121
casenext24071
	lda moonPos
	cmp #$10 ;keep
	bne casenext70732
	jsr ClearTime
	clc
	lda #<t05
	adc #0
	ldy #>t05
	sta print_text+0
	sty print_text+1
	ldx #4 ; optimized, look out for bugs
	jsr printstring
	jmp caseend28121
casenext70732
	lda moonPos
	cmp #$12 ;keep
	bne casenext87928
	jsr ClearTime
	clc
	lda #<t06
	adc #0
	ldy #>t06
	sta print_text+0
	sty print_text+1
	ldx #4 ; optimized, look out for bugs
	jsr printstring
	jmp caseend28121
casenext87928
caseend28121
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
	bne elsedoneblock65812
ConditionalTrueBlock80323
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
	bne elsedoneblock13208
ConditionalTrueBlock28221
	; Binary clause Simplified: LESS
	lda candyscr
	; Compare with pure num / var optimization
	cmp #$3;keep
	bcs elsedoneblock91312
ConditionalTrueBlock72935
	jsr SpawnCandy
elseblock68036
elsedoneblock91312
elseblock1014
elsedoneblock13208
	; Binary clause Simplified: EQUALS
	lda cloudPos
	; Compare with pure num / var optimization
	cmp #$16;keep
	bne elsedoneblock76599
ConditionalTrueBlock96038
	; Assigning single variable : cloudPos
	lda #0
	sta cloudPos
	jsr MoveMoon
	jsr DisplayTime
	; Binary clause Simplified: LESS
	lda candyscr
	; Compare with pure num / var optimization
	cmp #$3;keep
	bcs elsedoneblock34307
ConditionalTrueBlock91042
	jsr SpawnCandy
elseblock28026
elsedoneblock34307
elseblock45538
elsedoneblock76599
	; Assigning single variable : screenmemory
	lda #44
	ldx #148
	sta screenmemory
	stx screenmemory+1
	; Assigning single variable : screenmemory
	lda #1
	ldy cloudPos
	sta (screenmemory),y
elseblock36386
elsedoneblock65812
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
while17115
	; Binary clause Simplified: NOTEQUALS
	; Load pointer array
	ldy #$0
	lda (psnd1),y
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock66194
ConditionalTrueBlock72606
	jmp while17115
elseblock85199
elsedoneblock66194
	jsr StopSounds
	; Assigning single variable : sndFlag
	lda #0
	sta sndFlag
while92425
	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1pressed
	and #16
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock83738
ConditionalTrueBlock38184
	; Assigning single variable : x
	lda y
	sta x
	; Assigning single variable : y
	inc y
	; Binary clause Simplified: GREATER
	; Compare with pure num / var optimization
	cmp #$7;keep
	bcc elsedoneblock51074
	beq elsedoneblock51074
ConditionalTrueBlock29541
	; Assigning single variable : y
	lda #1
	sta y
elseblock742
elsedoneblock51074
	jsr callReadJoy1
	; Assigning single variable : t
	lda #0
	sta t
for34812
	; Assigning single variable : p1
	lda x
	ldy t
	sta (p1),y
	; Assigning single variable : x
	inc x
	; Binary clause Simplified: GREATER
	; Compare with pure num / var optimization
	cmp #$7;keep
	bcc elsedoneblock35053
	beq elsedoneblock35053
ConditionalTrueBlock90866
	; Assigning single variable : x
	lda #1
	sta x
elseblock43334
elsedoneblock35053
	inc t
	lda #14
	cmp t ;keep
	bne for34812
forLoopDone76731
	jmp while92425
elseblock20603
elsedoneblock83738
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
	bne elsedoneblock22106
ConditionalTrueBlock24777
	; Binary clause Simplified: EQUALS
	lda cloudPos
	; Compare with pure num / var optimization
	cmp #$15;keep
	bne elsedoneblock10120
ConditionalTrueBlock31761
	; Assigning single variable : gameMode
	lda #1
	sta gameMode
	; Assigning single variable : screenmemory
	lda #48
	ldx #16
	sta screenmemory
	stx screenmemory+1
printstring_call21155
	clc
	lda #<printstring_text77718
	adc #0
	ldy #>printstring_text77718
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
elseblock50635
elsedoneblock10120
elseblock32425
elsedoneblock22106
	; Binary clause: EQUALS
	lda px
	; Compare with pure num / var optimization
	cmp lx;keep
	; BC done
	bne binaryclausefailed9233
binaryclausesuccess8186
	lda #1; success
	jmp binaryclausefinished30889
binaryclausefailed9233
	lda #0 ; failed state
binaryclausefinished30889
	cmp #1
	beq ConditionalTrueBlock56208
	jmp elsedoneblock26941
ConditionalTrueBlock56208
	; Binary clause: EQUALS
	lda py
	; Compare with pure num / var optimization
	cmp ly;keep
	; BC done
	bne binaryclausefailed54327
binaryclausesuccess28721
	lda #1; success
	jmp binaryclausefinished33610
binaryclausefailed54327
	lda #0 ; failed state
binaryclausefinished33610
	cmp #1
	beq ConditionalTrueBlock76256
	jmp elsedoneblock44293
ConditionalTrueBlock76256
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
val_var96164 = $54
	sta val_var96164
	jsr Random
	sec
modulo48738
	sbc val_var96164
	bcs modulo48738
	adc val_var96164
	
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
	bcs elsedoneblock579
ConditionalTrueBlock66810
	; Assigning single variable : lives
	inc lives
elseblock28119
elsedoneblock579
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
elseblock50008
elsedoneblock44293
elseblock82524
elsedoneblock26941
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
	bne elsedoneblock38954
ConditionalTrueBlock50254
	; Binary clause Simplified: EQUALS
	lda py
	; Compare with pure num / var optimization
	cmp wy;keep
	bne elsedoneblock67501
ConditionalTrueBlock91719
	; Assigning single variable : gameMode
	lda #1
	sta gameMode
	; Assigning single variable : screenmemory
	lda #48
	ldx #16
	sta screenmemory
	stx screenmemory+1
printstring_call29128
	clc
	lda #<printstring_text46827
	adc #0
	ldy #>printstring_text46827
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
elseblock16833
elsedoneblock67501
elseblock81397
elsedoneblock38954
	; Binary clause Simplified: EQUALS
	lda pbgchar
	; Compare with pure num / var optimization
	cmp #$26;keep
	bne elsedoneblock54884
ConditionalTrueBlock29707
	; Assigning single variable : gameMode
	lda #1
	sta gameMode
	; Assigning single variable : screenmemory
	lda #48
	ldx #16
	sta screenmemory
	stx screenmemory+1
printstring_call96076
	clc
	lda #<printstring_text48000
	adc #0
	ldy #>printstring_text48000
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
elseblock78354
elsedoneblock54884
	; Assigning single variable : i
	lda #0
	sta i
for1378
	; Binary clause Simplified: EQUALS
	lda px
binary_clause_temp_var63926 = $54
	sta binary_clause_temp_var63926
	; Load Byte array
	ldx i
	lda gx,x
	
binary_clause_temp_2_var58800 = $56
	sta binary_clause_temp_2_var58800
	lda binary_clause_temp_var63926
	cmp binary_clause_temp_2_var58800;keep
	bne elsedoneblock48028
ConditionalTrueBlock35744
	; Binary clause Simplified: EQUALS
	lda py
binary_clause_temp_var5269 = $54
	sta binary_clause_temp_var5269
	; Load Byte array
	ldx i
	lda gy,x
	
binary_clause_temp_2_var51076 = $56
	sta binary_clause_temp_2_var51076
	lda binary_clause_temp_var5269
	cmp binary_clause_temp_2_var51076;keep
	bne elsedoneblock88700
ConditionalTrueBlock67445
	; Assigning single variable : gameMode
	lda #1
	sta gameMode
	; Assigning single variable : screenmemory
	lda #48
	ldx #16
	sta screenmemory
	stx screenmemory+1
printstring_call97502
	clc
	lda #<printstring_text75413
	adc #0
	ldy #>printstring_text75413
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
elseblock19723
elsedoneblock88700
elseblock16064
elsedoneblock48028
	inc i
	lda gNumGhosts
	cmp i ;keep
	bne for1378
forLoopDone75728
	; Binary clause Simplified: GREATER
	lda zNumZombies
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock90255
	beq elsedoneblock90255
ConditionalTrueBlock13566
	; Assigning single variable : i
	lda #0
	sta i
for47997
	; Binary clause Simplified: EQUALS
	lda px
binary_clause_temp_var97656 = $54
	sta binary_clause_temp_var97656
	; Load Byte array
	ldx i
	lda zx,x
	
binary_clause_temp_2_var89473 = $56
	sta binary_clause_temp_2_var89473
	lda binary_clause_temp_var97656
	cmp binary_clause_temp_2_var89473;keep
	bne elsedoneblock51202
ConditionalTrueBlock96268
	; Binary clause Simplified: EQUALS
	lda py
binary_clause_temp_var5468 = $54
	sta binary_clause_temp_var5468
	; Load Byte array
	ldx i
	lda zy,x
	
binary_clause_temp_2_var3342 = $56
	sta binary_clause_temp_2_var3342
	lda binary_clause_temp_var5468
	cmp binary_clause_temp_2_var3342;keep
	bne elsedoneblock10107
ConditionalTrueBlock90824
	; Assigning single variable : gameMode
	lda #1
	sta gameMode
	; Assigning single variable : screenmemory
	lda #48
	ldx #16
	sta screenmemory
	stx screenmemory+1
printstring_call89960
	clc
	lda #<printstring_text50395
	adc #0
	ldy #>printstring_text50395
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
elseblock25092
elsedoneblock10107
elseblock45417
elsedoneblock51202
	inc i
	lda zNumZombies
	cmp i ;keep
	bne for47997
forLoopDone78165
elseblock39794
elsedoneblock90255
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
	bne elsedoneblock13371
ConditionalTrueBlock7084
	rts
elseblock90656
elsedoneblock13371
	; Binary clause Simplified: EQUALS
	lda cx
	; Compare with pure num / var optimization
	cmp wx;keep
	bne elsedoneblock63361
ConditionalTrueBlock61471
	; Binary clause Simplified: EQUALS
	lda cy
	; Compare with pure num / var optimization
	cmp wy;keep
	bne elsedoneblock88046
ConditionalTrueBlock36053
	jsr HitWitch
elseblock11014
elsedoneblock88046
elseblock92419
elsedoneblock63361
	; Assigning single variable : i
	lda #0
	sta i
for39775
	; Binary clause Simplified: EQUALS
	lda cx
binary_clause_temp_var20949 = $54
	sta binary_clause_temp_var20949
	; Load Byte array
	ldx i
	lda gx,x
	
binary_clause_temp_2_var25724 = $56
	sta binary_clause_temp_2_var25724
	lda binary_clause_temp_var20949
	cmp binary_clause_temp_2_var25724;keep
	bne elsedoneblock58031
ConditionalTrueBlock51863
	; Binary clause Simplified: EQUALS
	lda cy
binary_clause_temp_var58288 = $54
	sta binary_clause_temp_var58288
	; Load Byte array
	ldx i
	lda gy,x
	
binary_clause_temp_2_var16792 = $56
	sta binary_clause_temp_2_var16792
	lda binary_clause_temp_var58288
	cmp binary_clause_temp_2_var16792;keep
	bne elsedoneblock44582
ConditionalTrueBlock84547
	jsr HitGhost
elseblock20507
elsedoneblock44582
elseblock90184
elsedoneblock58031
	inc i
	lda gNumGhosts
	cmp i ;keep
	bne for39775
forLoopDone49063
	; Binary clause Simplified: GREATER
	lda zNumZombies
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock99831
	beq elsedoneblock99831
ConditionalTrueBlock48916
	; Assigning single variable : i
	lda #0
	sta i
for24938
	; Binary clause Simplified: EQUALS
	lda cx
binary_clause_temp_var47224 = $54
	sta binary_clause_temp_var47224
	; Load Byte array
	ldx i
	lda zx,x
	
binary_clause_temp_2_var34796 = $56
	sta binary_clause_temp_2_var34796
	lda binary_clause_temp_var47224
	cmp binary_clause_temp_2_var34796;keep
	bne elsedoneblock48851
ConditionalTrueBlock71251
	; Binary clause Simplified: EQUALS
	lda cy
binary_clause_temp_var41439 = $54
	sta binary_clause_temp_var41439
	; Load Byte array
	ldx i
	lda zy,x
	
binary_clause_temp_2_var86688 = $56
	sta binary_clause_temp_2_var86688
	lda binary_clause_temp_var41439
	cmp binary_clause_temp_2_var86688;keep
	bne elsedoneblock9823
ConditionalTrueBlock57875
	jsr HitZombie
elseblock91334
elsedoneblock9823
elseblock98458
elsedoneblock48851
	inc i
	lda zNumZombies
	cmp i ;keep
	bne for24938
forLoopDone88216
elseblock26695
elsedoneblock99831
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
printstring_call25399
	clc
	lda #<printstring_text39922
	adc #0
	ldy #>printstring_text39922
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
while64980
	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1pressed
	and #16
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock7433
ConditionalTrueBlock23728
	; Assigning single variable : x
	lda y
	sta x
	; Assigning single variable : y
	inc y
	; Binary clause Simplified: GREATER
	; Compare with pure num / var optimization
	cmp #$7;keep
	bcc elsedoneblock41122
	beq elsedoneblock41122
ConditionalTrueBlock82985
	; Assigning single variable : y
	lda #1
	sta y
elseblock65320
elsedoneblock41122
	jsr callReadJoy1
	; Assigning single variable : t
	lda #0
	sta t
for48555
	; Assigning single variable : p1
	lda x
	ldy t
	sta (p1),y
	; Assigning single variable : x
	inc x
	; Binary clause Simplified: GREATER
	; Compare with pure num / var optimization
	cmp #$7;keep
	bcc elsedoneblock74413
	beq elsedoneblock74413
ConditionalTrueBlock48460
	; Assigning single variable : x
	lda #1
	sta x
elseblock79957
elsedoneblock74413
	inc t
	lda #18
	cmp t ;keep
	bne for48555
forLoopDone90072
	jmp while64980
elseblock44490
elsedoneblock7433
	rts
	
	
	; ***********  Defining procedure : GameLoop
	;    Procedure type : User-defined procedure
	
GameLoop
while31499
	; Binary clause: EQUALS
	lda gameMode
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bne binaryclausefailed38424
binaryclausesuccess88761
	lda #1; success
	jmp binaryclausefinished51613
binaryclausefailed38424
	lda #0 ; failed state
binaryclausefinished51613
	cmp #1
	beq ConditionalTrueBlock65135
	jmp elsedoneblock86044
ConditionalTrueBlock65135
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
	bne elsedoneblock25125
ConditionalTrueBlock92797
	jsr CollisionPlayer
elseblock85099
elsedoneblock25125
	jsr UpdateCandy
	jsr CollisionCandy
	; Binary clause Simplified: LESSEQUAL
	lda lostchildlevel
	; Compare with pure num / var optimization
	cmp level;keep
	beq ConditionalTrueBlock34841
	bcs elsedoneblock31984
ConditionalTrueBlock34841
	jsr UpdateLostChild
elseblock27132
elsedoneblock31984
	; Binary clause Simplified: EQUALS
	lda witchHits
	; Compare with pure num / var optimization
	cmp #$6;keep
	bne elsedoneblock58875
ConditionalTrueBlock54540
	jsr BeatWitchWait
elseblock75639
elsedoneblock58875
	; Assigning single variable : i
	lda #14
	
	sta i
	; Binary clause Simplified: LESS
	lda moonPos
	; Compare with pure num / var optimization
	cmp #$6;keep
	bcs elsedoneblock60558
ConditionalTrueBlock24744
	; Assigning single variable : i
	lda #12
	
	sta i
elseblock59965
elsedoneblock60558
	; Binary clause Simplified: GREATER
	lda moonPos
	; Compare with pure num / var optimization
	cmp #$11;keep
	bcc elsedoneblock94831
	beq elsedoneblock94831
ConditionalTrueBlock34272
	; Assigning single variable : i
	lda #10
	
	sta i
elseblock60565
elsedoneblock94831
	; Assigning memory location
	; Assigning single variable : $900f
	lda i
	sta $900f
while1993
	; Binary clause Simplified: EQUALS
	lda ticks
	; Compare with pure num / var optimization
	cmp oldTick;keep
	bne elsedoneblock87092
ConditionalTrueBlock97032
	jmp while1993
elseblock58476
elsedoneblock87092
	; Assigning single variable : oldTick
	lda ticks
	sta oldTick
	jmp while31499
elseblock80153
elsedoneblock86044
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
while6200
	; Binary clause Simplified: GREATER
	lda t
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock79826
	beq elsedoneblock79826
ConditionalTrueBlock23009
	; Assigning single variable : t
	dec t
	; Assigning single variable : i
	lda #0
	sta i
for10494
while45483
	; Binary clause Simplified: EQUALS
	lda ticks
	; Compare with pure num / var optimization
	cmp oldTick;keep
	bne elsedoneblock57225
ConditionalTrueBlock16733
	jmp while45483
elseblock19158
elsedoneblock57225
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
	beq elsedoneblock31945
ConditionalTrueBlock69841
	; Assigning single variable : t
	lda #0
	sta t
	; Assigning single variable : i
	lda #29
	sta i
elseblock11243
elsedoneblock31945
	inc i
	lda #30
	cmp i ;keep
	bne for10494
forLoopDone92614
	jmp while6200
elseblock66004
elsedoneblock79826
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
while32221
	; Binary clause Simplified: GREATER
	lda t
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock93346
	beq elsedoneblock93346
ConditionalTrueBlock67166
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
	bne elseblock96777
ConditionalTrueBlock4190
	jsr DrawTitleFrame1
	jmp elsedoneblock54674
elseblock96777
	jsr DrawTitleFrame2
elsedoneblock54674
	; Assigning single variable : t
	dec t
	; Assigning single variable : i
	lda #0
	sta i
for54338
while17949
	; Binary clause Simplified: EQUALS
	lda ticks
	; Compare with pure num / var optimization
	cmp oldTick;keep
	bne elsedoneblock34144
ConditionalTrueBlock81079
	jmp while17949
elseblock57508
elsedoneblock34144
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
	beq elsedoneblock88456
ConditionalTrueBlock29580
	; Assigning single variable : t
	lda #0
	sta t
	; Assigning single variable : i
	lda #29
	sta i
elseblock76024
elsedoneblock88456
	inc i
	lda #30
	cmp i ;keep
	bne for54338
forLoopDone89195
	jmp while32221
elseblock73108
elsedoneblock93346
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
	bcc dtnooverflow86980
	iny  ; overflow into high byte
dtnooverflow86980
	
	sta screenmemory
	sty screenmemory+1
	; ----------
	; BcdPrint address, number
	ldy #$07 ; screen offset
	ldx #0 ; score byte index
bcdprintloop16039
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
	bpl bcdprintloop16039
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
	bcs elseblock3219
ConditionalTrueBlock24663
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
printdecimal99163
	jsr init_printdecimal_div10 
	ora #$30
	sta (screenmemory),y
	dey
	bpl printdecimal99163
	jmp elsedoneblock96338
elseblock3219
printstring_call68115
	clc
	lda #<printstring_text33464
	adc #0
	ldy #>printstring_text33464
	sta print_text+0
	sty print_text+1
	ldx #13 ; optimized, look out for bugs
	jsr printstring
elsedoneblock96338
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
for96855
	; Assigning single variable : screenmemory
	lda #39
	ldy i
	sta (screenmemory),y
	inc i
	lda n
	cmp i ;keep
	bne for96855
forLoopDone83960
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
while30041
	; Binary clause: GREATER
	lda t
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bcc binaryclausefailed65525
	beq binaryclausefailed65525
binaryclausesuccess37287
	lda #1; success
	jmp binaryclausefinished5230
binaryclausefailed65525
	lda #0 ; failed state
binaryclausefinished5230
	cmp #1
	beq ConditionalTrueBlock64038
	jmp elsedoneblock72750
ConditionalTrueBlock64038
	; Assigning single variable : t
	dec t
	; Binary clause: LESS
	lda x
	; Compare with pure num / var optimization
	cmp #$15;keep
	; BC done
	bcs binaryclausefailed76123
binaryclausesuccess68312
	lda #1; success
	jmp binaryclausefinished26141
binaryclausefailed76123
	lda #0 ; failed state
binaryclausefinished26141
	cmp #1
	beq ConditionalTrueBlock72415
	jmp elsedoneblock88788
ConditionalTrueBlock72415
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
	bcs elsedoneblock68547
ConditionalTrueBlock35164
	; Assigning single variable : y
	inc y
elseblock34253
elsedoneblock68547
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
	bne elsedoneblock52421
ConditionalTrueBlock57306
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
elseblock82913
elsedoneblock52421
	; Binary clause Simplified: EQUALS
	lda x
	; Compare with pure num / var optimization
	cmp #$15;keep
	bne elsedoneblock40180
ConditionalTrueBlock12849
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
elseblock12824
elsedoneblock40180
elseblock56941
elsedoneblock88788
	; Binary clause: EQUALS
	lda x
	; Compare with pure num / var optimization
	cmp #$15;keep
	; BC done
	bne binaryclausefailed50119
binaryclausesuccess91530
	lda #1; success
	jmp binaryclausefinished69585
binaryclausefailed50119
	lda #0 ; failed state
binaryclausefinished69585
	cmp #1
	beq ConditionalTrueBlock95480
	jmp elsedoneblock89636
ConditionalTrueBlock95480
	; Binary clause: LESS
	lda n
	; Compare with pure num / var optimization
	cmp #$58;keep
	; BC done
	bcs binaryclausefailed34134
binaryclausesuccess96118
	lda #1; success
	jmp binaryclausefinished93178
binaryclausefailed34134
	lda #0 ; failed state
binaryclausefinished93178
	cmp #1
	beq ConditionalTrueBlock71097
	jmp elsedoneblock34445
ConditionalTrueBlock71097
	; Assigning single variable : pan
	inc pan
	; Binary clause Simplified: GREATER
	lda pan
	; Compare with pure num / var optimization
	cmp #$5;keep
	bcc elsedoneblock23136
	beq elsedoneblock23136
ConditionalTrueBlock82275
	; Assigning single variable : pan
	lda #0
	sta pan
elseblock18569
elsedoneblock23136
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
	bne elsedoneblock2884
ConditionalTrueBlock80329
	; Assigning single variable : s
	lda #61
	sta s
elseblock44198
elsedoneblock2884
	; Binary clause Simplified: EQUALS
	lda n
	; Compare with pure num / var optimization
	cmp #$58;keep
	bne elsedoneblock51751
ConditionalTrueBlock56799
	; Assigning single variable : screenmemory
	lda #0
	pha
	lda n
	lsr
	lsr
	
	tay
	pla
	sta (screenmemory),y
elseblock74058
elsedoneblock51751
	; Binary clause Simplified: EQUALS
	; Modulo
	lda #4
val_var36820 = $54
	sta val_var36820
	lda n
	sec
modulo36451
	sbc val_var36820
	bcs modulo36451
	adc val_var36820
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock29234
ConditionalTrueBlock33399
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
	bne elseblock19633
ConditionalTrueBlock59587
	; Assigning single variable : stepsnd
	lda #30
	sta stepsnd
	jmp elsedoneblock94440
elseblock19633
	; Assigning single variable : stepsnd
	lda #0
	sta stepsnd
elsedoneblock94440
elseblock2363
elsedoneblock29234
elseblock4335
elsedoneblock34445
elseblock44518
elsedoneblock89636
	; Assigning single variable : i
	lda #0
	sta i
for50422
while6287
	; Binary clause Simplified: EQUALS
	lda ticks
	; Compare with pure num / var optimization
	cmp oldTick;keep
	bne elsedoneblock73061
ConditionalTrueBlock11079
	jmp while6287
elseblock43408
elsedoneblock73061
	; Assigning single variable : oldTick
	lda ticks
	sta oldTick
	jsr callReadJoy1
	; Binary clause Simplified: LESS
	lda t
	; Compare with pure num / var optimization
	cmp #$3c;keep
	bcs elsedoneblock25014
ConditionalTrueBlock27193
	; Binary clause Simplified: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1
	and #16
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock71616
ConditionalTrueBlock85815
	; Assigning single variable : t
	lda #0
	sta t
elseblock92561
elsedoneblock71616
elseblock7311
elsedoneblock25014
	inc i
	lda #4
	cmp i ;keep
	bne for50422
forLoopDone94764
	jmp while30041
elseblock56682
elsedoneblock72750
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
val_var68502 = $54
	sta val_var68502
	; 8 bit binop
	; Add/sub where right value is constant number
	lda ticks
	clc
	adc #1
	 ; end add / sub var with constant
	sec
modulo43543
	sbc val_var68502
	bcs modulo43543
	adc val_var68502
	
	sta ticks
	; Binary clause Simplified: EQUALS
	lda sndFlag
	; Compare with pure num / var optimization
	cmp #$1;keep
	bne elsedoneblock18493
ConditionalTrueBlock95933
	jsr UpdateSound
elseblock86952
elsedoneblock18493
	; CloseIRQ
	pla
	tay
	pla
	tax
	pla
	 jmp $eabf     ; return to normal IRQ	
	rti
block87167
	jsr Initialise
	jsr SetupSound
	; Assigning single variable : level
	lda #0
	sta level
while58007
	; Binary clause: NOTEQUALS
	lda #1
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	beq binaryclausefailed54190
binaryclausesuccess15120
	lda #1; success
	jmp binaryclausefinished83603
binaryclausefailed54190
	lda #0 ; failed state
binaryclausefinished83603
	cmp #1
	beq ConditionalTrueBlock30215
	jmp elsedoneblock99373
ConditionalTrueBlock30215
	jsr BlankScreen
	lda level
	cmp #$0 ;keep
	bne casenext99232
	jsr TitleCredits
	jmp caseend91484
casenext99232
	lda level
	cmp #$1 ;keep
	bne casenext62762
	jsr TitleScreen
	jmp caseend91484
casenext62762
	lda level
	cmp #$2 ;keep
	bne casenext33483
	jsr LastScore
	jmp caseend91484
casenext33483
	lda level
	cmp #$3 ;keep
	bne casenext96227
	jsr TitleStory
	jmp caseend91484
casenext96227
	lda level
	cmp #$4 ;keep
	bne casenext52322
	jsr TitleStory2
	jmp caseend91484
casenext52322
	lda level
	cmp #$5 ;keep
	bne casenext40076
	jsr TitleCast
	jmp caseend91484
casenext40076
	lda level
	cmp #$6 ;keep
	bne casenext90210
	jsr TitleHowTo
	jmp caseend91484
casenext90210
caseend91484
	; Binary clause: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1
	and #16
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	beq binaryclausefailed43746
binaryclausesuccess22020
	lda #1; success
	jmp binaryclausefinished50894
binaryclausefailed43746
	lda #0 ; failed state
binaryclausefinished50894
	cmp #1
	beq ConditionalTrueBlock28515
	jmp elsedoneblock67818
ConditionalTrueBlock28515
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
val_var53071 = $54
	sta val_var53071
	jsr Random
	sec
modulo26154
	sbc val_var53071
	bcs modulo26154
	adc val_var53071
	
	clc
	adc #1
	 ; end add / sub var with constant
	
	sta lostchildlevel
while75638
	; Binary clause: GREATER
	lda lives
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bcc binaryclausefailed3038
	beq binaryclausefailed3038
binaryclausesuccess36784
	lda #1; success
	jmp binaryclausefinished37684
binaryclausefailed3038
	lda #0 ; failed state
binaryclausefinished37684
	cmp #1
	beq ConditionalTrueBlock90478
	jmp elsedoneblock25652
ConditionalTrueBlock90478
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
	bne elsedoneblock68506
ConditionalTrueBlock94535
	; Assigning single variable : lives
	dec lives
	; Binary clause Simplified: EQUALS
	lda lives
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elseblock27042
ConditionalTrueBlock49899
	jsr TitleGameOver
	jmp elsedoneblock7046
elseblock27042
	jsr TitleFail
elsedoneblock7046
elseblock81428
elsedoneblock68506
	; Binary clause Simplified: EQUALS
	lda gameMode
	; Compare with pure num / var optimization
	cmp #$2;keep
	bne elsedoneblock65128
ConditionalTrueBlock88827
	; Assigning single variable : level
	inc level
	; Binary clause Simplified: EQUALS
	lda level
	; Compare with pure num / var optimization
	cmp #$d;keep
	bne elseblock72501
ConditionalTrueBlock96074
	jsr TitleGameWon
	; Assigning single variable : lives
	lda #0
	sta lives
	jmp elsedoneblock48762
elseblock72501
	jsr TitleGameNext
	; Assigning single variable : witchHits
	lda #0
	sta witchHits
	; Binary clause Simplified: LESS
	lda lives
	; Compare with pure num / var optimization
	cmp #$5;keep
	bcs elsedoneblock66041
ConditionalTrueBlock19588
	; Binary clause Simplified: EQUALS
	lda level
	; Compare with pure num / var optimization
	cmp #$2;keep
	bne elsedoneblock28384
ConditionalTrueBlock12372
	; Assigning single variable : lives
	inc lives
elseblock23369
elsedoneblock28384
	; Binary clause Simplified: EQUALS
	lda level
	; Compare with pure num / var optimization
	cmp #$5;keep
	bne elsedoneblock42960
ConditionalTrueBlock85327
	; Assigning single variable : lives
	inc lives
elseblock54600
elsedoneblock42960
	; Binary clause Simplified: EQUALS
	lda level
	; Compare with pure num / var optimization
	cmp #$9;keep
	bne elsedoneblock87787
ConditionalTrueBlock23395
	; Assigning single variable : lives
	inc lives
elseblock33013
elsedoneblock87787
	; Binary clause Simplified: EQUALS
	lda level
	; Compare with pure num / var optimization
	cmp #$b;keep
	bne elsedoneblock37233
ConditionalTrueBlock38772
	; Assigning single variable : lives
	inc lives
elseblock88939
elsedoneblock37233
elseblock1609
elsedoneblock66041
elsedoneblock48762
elseblock83852
elsedoneblock65128
	jmp while75638
elseblock54206
elsedoneblock25652
	; Assigning single variable : level
	lda #0
	sta level
elseblock86017
elsedoneblock67818
	; Assigning single variable : level
	inc level
	; Binary clause Simplified: GREATER
	lda level
	; Compare with pure num / var optimization
	cmp #$6;keep
	bcc elsedoneblock68651
	beq elsedoneblock68651
ConditionalTrueBlock14811
	; Assigning single variable : level
	lda #0
	sta level
elseblock38287
elsedoneblock68651
	jmp while58007
elseblock82593
elsedoneblock99373
EndBlock982
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
