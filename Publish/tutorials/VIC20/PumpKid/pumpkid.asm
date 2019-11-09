 processor 6502
	ORG $1201
	.byte    $0E, $08, $0A, $00, $9E, $20, $28
	.byte   $34,$36,$32,$34
	.byte    $29, $00, $00, $00
	ORG $1210
Pumpkid
	jmp block89383
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
printstring_text84454	dc.b	"@OUT@OF@TIME@@"
	dc.b	0
printstring_text92190	dc.b	"@GOT@BY@WITCH@"
	dc.b	0
printstring_text24221	dc.b	"POISONED@SKULL"
	dc.b	0
printstring_text3165	dc.b	"@GOT@BY@GHOST@"
	dc.b	0
printstring_text73924	dc.b	"@GOT@BY@ZOMBIE@"
	dc.b	0
printstring_text45482	dc.b	"THE@WITCH@RETREATS"
	dc.b	0
printstring_text17140	dc.b	"GET@READY@FOR"
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
EndBlock34
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
	BMI JoySkip30886
	ORA #$02
JoySkip30886
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
	bcc RandomSkip92777
	eor #$4d
RandomSkip92777
	sta Random+1
	eor $9124
	rts
	
	
	; ***********  Defining procedure : PlaySnd1
	;    Procedure type : User-defined procedure
	
vsnd1Time	dc.b	$00
vsnd1	dc.b	
block36915
PlaySnd1
	; Assigning single variable : vsnd1Time
	lda #0
	sta vsnd1Time
	rts
	
	
	; ***********  Defining procedure : PlaySnd2
	;    Procedure type : User-defined procedure
	
vsnd2Time	dc.b	$00
vsnd2	dc.b	
block47793
PlaySnd2
	; Assigning single variable : vsnd2Time
	lda #0
	sta vsnd2Time
	rts
	
	
	; ***********  Defining procedure : PlaySnd3
	;    Procedure type : User-defined procedure
	
vsnd3Time	dc.b	$00
vsnd3	dc.b	
block38335
PlaySnd3
	; Assigning single variable : vsnd3Time
	lda #0
	sta vsnd3Time
	rts
	
	
	; ***********  Defining procedure : PlaySndfx
	;    Procedure type : User-defined procedure
	
vsndfxTime	dc.b	$00
vsndfx	dc.b	
block85386
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
block60492
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
	bcc elsedoneblock20059
	beq elsedoneblock20059
ConditionalTrueBlock90027
	; Assigning single variable : vsnd1Time
	dec vsnd1Time
elseblock68690
elsedoneblock20059
	; Binary clause Simplified: EQUALS
	lda vsnd1Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock55736
ConditionalTrueBlock83426
	; Assigning single variable : psnd1
	; WORD optimization: a=a+b
	lda psnd1+0
	
	clc
	adc #2
	bcc WordAdd56429
	inc psnd1+1
WordAdd56429
	sta psnd1+0
elseblock89172
elsedoneblock55736
	rts
	
	
	; ***********  Defining procedure : Snd2Time
	;    Procedure type : User-defined procedure
	
Snd2Time
	; Binary clause Simplified: GREATER
	lda vsnd2Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock74067
	beq elsedoneblock74067
ConditionalTrueBlock22862
	; Assigning single variable : vsnd2Time
	dec vsnd2Time
elseblock65123
elsedoneblock74067
	; Binary clause Simplified: EQUALS
	lda vsnd2Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock33069
ConditionalTrueBlock34022
	; Assigning single variable : psnd2
	; WORD optimization: a=a+b
	lda psnd2+0
	
	clc
	adc #2
	bcc WordAdd75011
	inc psnd2+1
WordAdd75011
	sta psnd2+0
elseblock23058
elsedoneblock33069
	rts
	
	
	; ***********  Defining procedure : Snd3Time
	;    Procedure type : User-defined procedure
	
Snd3Time
	; Binary clause Simplified: GREATER
	lda vsnd3Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock44919
	beq elsedoneblock44919
ConditionalTrueBlock77373
	; Assigning single variable : vsnd3Time
	dec vsnd3Time
elseblock84421
elsedoneblock44919
	; Binary clause Simplified: EQUALS
	lda vsnd3Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock64370
ConditionalTrueBlock94324
	; Assigning single variable : psnd3
	; WORD optimization: a=a+b
	lda psnd3+0
	
	clc
	adc #2
	bcc WordAdd68980
	inc psnd3+1
WordAdd68980
	sta psnd3+0
elseblock98315
elsedoneblock64370
	rts
	
	
	; ***********  Defining procedure : SndfxTime
	;    Procedure type : User-defined procedure
	
SndfxTime
	; Binary clause Simplified: GREATER
	lda vsndfxTime
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock6996
	beq elsedoneblock6996
ConditionalTrueBlock6862
	; Assigning single variable : vsndfxTime
	dec vsndfxTime
elseblock99170
elsedoneblock6996
	; Binary clause Simplified: EQUALS
	lda vsndfxTime
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock60336
ConditionalTrueBlock77084
	; Assigning single variable : psndfx
	; WORD optimization: a=a+b
	lda psndfx+0
	
	clc
	adc #2
	bcc WordAdd61313
	inc psndfx+1
WordAdd61313
	sta psndfx+0
elseblock36327
elsedoneblock60336
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
	bne elsedoneblock545
ConditionalTrueBlock53895
	; Binary clause Simplified: NOTEQUALS
	lda vsnd1
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock93584
ConditionalTrueBlock47178
	; Assigning single variable : vsnd1
	lda #0
	sta vsnd1
	; Poke
	; Optimization: shift is zero
	sta $900a
elseblock95788
elsedoneblock93584
	rts
elseblock19582
elsedoneblock545
	; Binary clause: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd1),y
	
	; Compare with pure num / var optimization
	cmp #$20;keep
	; BC done
	bne tempfail8117
binaryclausesuccess5771
	jmp ConditionalTrueBlock12399
tempfail8117
	; Binary clause: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd1),y
	
	; Compare with pure num / var optimization
	cmp #$10;keep
	; BC done
	bne elseblock99932
binaryclausesuccess90675
ConditionalTrueBlock12399
	; Binary clause Simplified: EQUALS
	lda vsnd1Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock79497
ConditionalTrueBlock4567
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
	bne elsedoneblock26340
ConditionalTrueBlock59708
	; Poke
	; Optimization: shift is zero
	lda vsnd1
	sta $900a
elseblock86715
elsedoneblock26340
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd1),y
	
	; Compare with pure num / var optimization
	cmp #$10;keep
	bne elsedoneblock22846
ConditionalTrueBlock42618
	; Poke
	; Optimization: shift is zero
	lda #0
	sta $900a
elseblock2245
elsedoneblock22846
elseblock77856
elsedoneblock79497
	jsr Snd1Time
elseblock99932
elsedoneblock95060
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
	bne elsedoneblock37764
ConditionalTrueBlock92379
	; Binary clause Simplified: EQUALS
	lda vsnd1Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock73743
ConditionalTrueBlock36987
	; Assigning single variable : vsnd1Time
	; Load pointer array
	ldy #$1
	lda (psnd1),y
	
	sta vsnd1Time
elseblock75856
elsedoneblock73743
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
elseblock97488
elsedoneblock37764
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
	bne elsedoneblock51432
ConditionalTrueBlock9859
	; Binary clause Simplified: EQUALS
	lda vsnd1Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock73793
ConditionalTrueBlock61237
	; Assigning single variable : vsnd1Time
	; Load pointer array
	ldy #$1
	lda (psnd1),y
	
	sta vsnd1Time
elseblock8235
elsedoneblock73793
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
	
rightvarAddSub_var66143 = $54
	sta rightvarAddSub_var66143
	lda vsnd1
	sec
	sbc rightvarAddSub_var66143
	
	sta vsnd1
	; Poke
	; Optimization: shift is zero
	sta $900a
	jsr Snd1Time
elseblock81936
elsedoneblock51432
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
	bne elsedoneblock22404
ConditionalTrueBlock39529
	; Binary clause Simplified: NOTEQUALS
	lda vsnd2
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock67917
ConditionalTrueBlock70688
	; Assigning single variable : vsnd2
	lda #0
	sta vsnd2
	; Poke
	; Optimization: shift is zero
	sta $900b
elseblock97369
elsedoneblock67917
	rts
elseblock18776
elsedoneblock22404
	; Binary clause: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd2),y
	
	; Compare with pure num / var optimization
	cmp #$20;keep
	; BC done
	bne tempfail21801
binaryclausesuccess73730
	jmp ConditionalTrueBlock87743
tempfail21801
	; Binary clause: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd2),y
	
	; Compare with pure num / var optimization
	cmp #$10;keep
	; BC done
	bne elseblock59470
binaryclausesuccess11305
ConditionalTrueBlock87743
	; Binary clause Simplified: EQUALS
	lda vsnd2Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock48522
ConditionalTrueBlock79444
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
	bne elsedoneblock973
ConditionalTrueBlock47468
	; Poke
	; Optimization: shift is zero
	lda vsnd2
	sta $900b
elseblock90071
elsedoneblock973
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd2),y
	
	; Compare with pure num / var optimization
	cmp #$10;keep
	bne elsedoneblock58660
ConditionalTrueBlock8933
	; Poke
	; Optimization: shift is zero
	lda #0
	sta $900b
elseblock45894
elsedoneblock58660
elseblock48626
elsedoneblock48522
	jsr Snd2Time
elseblock59470
elsedoneblock12183
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
	bne elsedoneblock52959
ConditionalTrueBlock48899
	; Binary clause Simplified: EQUALS
	lda vsnd2Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock43376
ConditionalTrueBlock22090
	; Assigning single variable : vsnd2Time
	; Load pointer array
	ldy #$1
	lda (psnd2),y
	
	sta vsnd2Time
elseblock27684
elsedoneblock43376
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
elseblock52996
elsedoneblock52959
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
	bne elsedoneblock69179
ConditionalTrueBlock17445
	; Binary clause Simplified: EQUALS
	lda vsnd2Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock19301
ConditionalTrueBlock67587
	; Assigning single variable : vsnd2Time
	; Load pointer array
	ldy #$1
	lda (psnd2),y
	
	sta vsnd2Time
elseblock78206
elsedoneblock19301
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
	
rightvarAddSub_var75321 = $54
	sta rightvarAddSub_var75321
	lda vsnd2
	sec
	sbc rightvarAddSub_var75321
	
	sta vsnd2
	; Poke
	; Optimization: shift is zero
	sta $900b
	jsr Snd2Time
elseblock19756
elsedoneblock69179
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
	bne elsedoneblock29904
ConditionalTrueBlock44599
	; Binary clause Simplified: NOTEQUALS
	lda vsnd3
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock89224
ConditionalTrueBlock96658
	; Assigning single variable : vsnd3
	lda #0
	sta vsnd3
	; Poke
	; Optimization: shift is zero
	sta $900c
elseblock63920
elsedoneblock89224
	rts
elseblock17721
elsedoneblock29904
	; Binary clause: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd3),y
	
	; Compare with pure num / var optimization
	cmp #$20;keep
	; BC done
	bne tempfail48142
binaryclausesuccess45884
	jmp ConditionalTrueBlock54081
tempfail48142
	; Binary clause: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd3),y
	
	; Compare with pure num / var optimization
	cmp #$10;keep
	; BC done
	bne elseblock45630
binaryclausesuccess23205
ConditionalTrueBlock54081
	; Binary clause Simplified: EQUALS
	lda vsnd3Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock1961
ConditionalTrueBlock62504
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
	bne elsedoneblock48036
ConditionalTrueBlock49958
	; Poke
	; Optimization: shift is zero
	lda vsnd3
	sta $900c
elseblock10498
elsedoneblock48036
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd3),y
	
	; Compare with pure num / var optimization
	cmp #$10;keep
	bne elsedoneblock32133
ConditionalTrueBlock83303
	; Poke
	; Optimization: shift is zero
	lda #0
	sta $900c
elseblock33333
elsedoneblock32133
elseblock90613
elsedoneblock1961
	jsr Snd3Time
elseblock45630
elsedoneblock40084
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
	bne elsedoneblock90368
ConditionalTrueBlock17567
	; Binary clause Simplified: EQUALS
	lda vsnd3Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock10253
ConditionalTrueBlock5363
	; Assigning single variable : vsnd3Time
	; Load pointer array
	ldy #$1
	lda (psnd3),y
	
	sta vsnd3Time
elseblock12497
elsedoneblock10253
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
elseblock51746
elsedoneblock90368
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
	bne elsedoneblock45975
ConditionalTrueBlock61152
	; Binary clause Simplified: EQUALS
	lda vsnd3Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock66748
ConditionalTrueBlock88027
	; Assigning single variable : vsnd3Time
	; Load pointer array
	ldy #$1
	lda (psnd3),y
	
	sta vsnd3Time
elseblock78050
elsedoneblock66748
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
	
rightvarAddSub_var4794 = $54
	sta rightvarAddSub_var4794
	lda vsnd3
	sec
	sbc rightvarAddSub_var4794
	
	sta vsnd3
	; Poke
	; Optimization: shift is zero
	sta $900c
	jsr Snd3Time
elseblock13996
elsedoneblock45975
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
	bne elsedoneblock32002
ConditionalTrueBlock71043
	; Binary clause Simplified: NOTEQUALS
	lda vsndfx
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock71692
ConditionalTrueBlock82134
	; Assigning single variable : vsndfx
	lda #0
	sta vsndfx
	; Poke
	; Optimization: shift is zero
	sta $900d
elseblock4339
elsedoneblock71692
	rts
elseblock1039
elsedoneblock32002
	; Binary clause: EQUALS
	; Load pointer array
	ldy #$0
	lda (psndfx),y
	
	; Compare with pure num / var optimization
	cmp #$20;keep
	; BC done
	bne tempfail26309
binaryclausesuccess95425
	jmp ConditionalTrueBlock55629
tempfail26309
	; Binary clause: EQUALS
	; Load pointer array
	ldy #$0
	lda (psndfx),y
	
	; Compare with pure num / var optimization
	cmp #$10;keep
	; BC done
	bne elseblock60049
binaryclausesuccess6367
ConditionalTrueBlock55629
	; Binary clause Simplified: EQUALS
	lda vsndfxTime
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock64524
ConditionalTrueBlock30690
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
	bne elsedoneblock2647
ConditionalTrueBlock28946
	; Poke
	; Optimization: shift is zero
	lda vsndfx
	sta $900d
elseblock32290
elsedoneblock2647
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #$0
	lda (psndfx),y
	
	; Compare with pure num / var optimization
	cmp #$10;keep
	bne elsedoneblock30857
ConditionalTrueBlock99631
	; Poke
	; Optimization: shift is zero
	lda #0
	sta $900d
elseblock18593
elsedoneblock30857
elseblock31626
elsedoneblock64524
	jsr SndfxTime
elseblock60049
elsedoneblock90964
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
	bne elsedoneblock93512
ConditionalTrueBlock39214
	; Binary clause Simplified: EQUALS
	lda vsndfxTime
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock98987
ConditionalTrueBlock56620
	; Assigning single variable : vsndfxTime
	; Load pointer array
	ldy #$1
	lda (psndfx),y
	
	sta vsndfxTime
elseblock25433
elsedoneblock98987
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
elseblock88355
elsedoneblock93512
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
	bne elsedoneblock56856
ConditionalTrueBlock87770
	; Binary clause Simplified: EQUALS
	lda vsndfxTime
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock28458
ConditionalTrueBlock90783
	; Assigning single variable : vsndfxTime
	; Load pointer array
	ldy #$1
	lda (psndfx),y
	
	sta vsndfxTime
elseblock36873
elsedoneblock28458
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
	
rightvarAddSub_var4289 = $54
	sta rightvarAddSub_var4289
	lda vsndfx
	sec
	sbc rightvarAddSub_var4289
	
	sta vsndfx
	; Poke
	; Optimization: shift is zero
	sta $900d
	jsr SndfxTime
elseblock27284
elsedoneblock56856
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
block26607
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
	bne elsedoneblock45729
ConditionalTrueBlock49314
	rts
elseblock34471
elsedoneblock45729
	; Binary clause Simplified: EQUALS
	lda musicStep
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock11388
ConditionalTrueBlock89438
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
	bne elsedoneblock74580
ConditionalTrueBlock85404
	; Assigning single variable : musicNote
	lda #0
	sta musicNote
	; Assigning single variable : musicRepeat
	inc musicRepeat
	; Binary clause Simplified: EQUALS
	lda musicRepeat
	; Compare with pure num / var optimization
	cmp #$1;keep
	bne elsedoneblock75105
ConditionalTrueBlock77560
	; Poke
	; Optimization: shift is zero
	lda #8
	sta $900e
elseblock67818
elsedoneblock75105
	; Binary clause Simplified: EQUALS
	lda musicRepeat
	; Compare with pure num / var optimization
	cmp #$2;keep
	bne elsedoneblock19934
ConditionalTrueBlock88711
	; Poke
	; Optimization: shift is zero
	lda #5
	sta $900e
elseblock41805
elsedoneblock19934
	; Binary clause Simplified: EQUALS
	lda musicRepeat
	; Compare with pure num / var optimization
	cmp #$3;keep
	bne elsedoneblock43768
ConditionalTrueBlock93614
	; Poke
	; Optimization: shift is zero
	lda #2
	sta $900e
elseblock33589
elsedoneblock43768
elseblock82338
elsedoneblock74580
elseblock58025
elsedoneblock11388
	; Assigning single variable : musicStep
	inc musicStep
	; Binary clause Simplified: EQUALS
	lda musicStep
	; Compare with pure num / var optimization
	cmp #$13;keep
	bne elsedoneblock76982
ConditionalTrueBlock76882
	; Assigning single variable : psnd3
	lda #<noSound
	ldx #>noSound
	sta psnd3
	stx psnd3+1
elseblock44822
elsedoneblock76982
	; Binary clause Simplified: EQUALS
	lda musicStep
	; Compare with pure num / var optimization
	cmp #$14;keep
	bne elsedoneblock86593
ConditionalTrueBlock11574
	; Assigning single variable : musicStep
	lda #0
	sta musicStep
elseblock30126
elsedoneblock86593
	rts
	
	
	; ***********  Defining procedure : UpdateLostChild
	;    Procedure type : User-defined procedure
	
UpdateLostChild
	; Binary clause: EQUALS
	lda moonPos
	; Compare with pure num / var optimization
	cmp #$8;keep
	; BC done
	bne binaryclausefailed67857
binaryclausesuccess99181
	lda #1; success
	jmp binaryclausefinished81494
binaryclausefailed67857
	lda #0 ; failed state
binaryclausefinished81494
	cmp #1
	beq ConditionalTrueBlock27814
	jmp elsedoneblock78179
ConditionalTrueBlock27814
	; Binary clause: EQUALS
	lda lx
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bne binaryclausefailed40528
binaryclausesuccess28398
	lda #1; success
	jmp binaryclausefinished30639
binaryclausefailed40528
	lda #0 ; failed state
binaryclausefinished30639
	cmp #1
	beq ConditionalTrueBlock15720
	jmp elsedoneblock87982
ConditionalTrueBlock15720
	; Binary clause: LESS
	lda level
	; Compare with pure num / var optimization
	cmp #$c;keep
	; BC done
	bcs binaryclausefailed66127
binaryclausesuccess65421
	lda #1; success
	jmp binaryclausefinished98382
binaryclausefailed66127
	lda #0 ; failed state
binaryclausefinished98382
	cmp #1
	beq ConditionalTrueBlock92393
	jmp elsedoneblock33890
ConditionalTrueBlock92393
	; Assigning single variable : i
	lda #0
	sta i
while79334
	; Binary clause: EQUALS
	lda i
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bne binaryclausefailed76217
binaryclausesuccess1173
	lda #1; success
	jmp binaryclausefinished65154
binaryclausefailed76217
	lda #0 ; failed state
binaryclausefinished65154
	cmp #1
	beq ConditionalTrueBlock42439
	jmp elsedoneblock88421
ConditionalTrueBlock42439
	; Assigning single variable : lx
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #19
val_var86506 = $54
	sta val_var86506
	jsr Random
	sec
modulo39851
	sbc val_var86506
	bcs modulo39851
	adc val_var86506
	
	clc
	adc #1
	 ; end add / sub var with constant
	
	sta lx
	; Assigning single variable : ly
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #18
val_var76364 = $54
	sta val_var76364
	jsr Random
	sec
modulo24790
	sbc val_var76364
	bcs modulo24790
	adc val_var76364
	
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
	bne elsedoneblock73537
ConditionalTrueBlock23172
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
elseblock70037
elsedoneblock73537
	jmp while79334
elseblock87334
elsedoneblock88421
elseblock28577
elsedoneblock33890
elseblock52433
elsedoneblock87982
elseblock34713
elsedoneblock78179
	; Binary clause Simplified: EQUALS
	lda moonPos
	; Compare with pure num / var optimization
	cmp #$a;keep
	bne elsedoneblock78341
ConditionalTrueBlock55856
	; Binary clause Simplified: NOTEQUALS
	lda lx
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock34154
ConditionalTrueBlock31489
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
val_var34969 = $54
	sta val_var34969
	jsr Random
	sec
modulo90636
	sbc val_var34969
	bcs modulo90636
	adc val_var34969
	
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
elseblock81032
elsedoneblock34154
elseblock46590
elsedoneblock78341
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
	bne elseblock62760
ConditionalTrueBlock62844
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
	jmp elsedoneblock6649
elseblock62760
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
elsedoneblock6649
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
dtloop22326
	tay
	lda scr,x
	inx
	inx
	clc
	adc #$16
	bcc dtnooverflow13886
	iny
dtnooverflow13886
	sta scr,x
	tya
	sta scr+1,x
	cpx #$2c
	bcc dtloop22326
	; ----------
	; DefineAddressTable address, StartValue, IncrementValue, TableSize
	ldy #>$9400
	lda #<$9400
	ldx #0
	sta clr,x   ; Address of table
	tya
	sta clr+1,x
dtloop60183
	tay
	lda clr,x
	inx
	inx
	clc
	adc #$16
	bcc dtnooverflow46039
	iny
dtnooverflow46039
	sta clr,x
	tya
	sta clr+1,x
	cpx #$2c
	bcc dtloop60183
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
bcdprintloop1535
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
	bpl bcdprintloop1535
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
for32621
	; Assigning single variable : j
	lda #0
	sta j
for74295
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
	bne for74295
forLoopDone4313
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd53198
	inc p1+1
WordAdd53198
	sta p1+0
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd94949
	inc screenmemory+1
WordAdd94949
	sta screenmemory+0
	inc i
	lda #23
	cmp i ;keep
	bne for32621
forLoopDone95735
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
for65155
	; Assigning single variable : j
	lda #0
	sta j
for85677
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
	bne for85677
forLoopDone93245
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd72761
	inc p1+1
WordAdd72761
	sta p1+0
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd33990
	inc screenmemory+1
WordAdd33990
	sta screenmemory+0
	inc i
	lda #23
	cmp i ;keep
	bne for65155
forLoopDone13171
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
for10050
	; Assigning single variable : j
	lda #0
	sta j
for25844
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
	bne for25844
forLoopDone51238
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd74976
	inc p1+1
WordAdd74976
	sta p1+0
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd25383
	inc screenmemory+1
WordAdd25383
	sta screenmemory+0
	inc i
	lda #23
	cmp i ;keep
	bne for10050
forLoopDone23919
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
for63133
	; Assigning single variable : j
	lda #0
	sta j
for90192
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
	bne for90192
forLoopDone80902
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd88131
	inc p1+1
WordAdd88131
	sta p1+0
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd65002
	inc screenmemory+1
WordAdd65002
	sta screenmemory+0
	inc i
	lda #23
	cmp i ;keep
	bne for63133
forLoopDone88967
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
for49207
	; Assigning single variable : j
	lda #0
	sta j
for76125
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
	bne for76125
forLoopDone66797
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd93418
	inc p1+1
WordAdd93418
	sta p1+0
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd93141
	inc screenmemory+1
WordAdd93141
	sta screenmemory+0
	inc i
	lda #23
	cmp i ;keep
	bne for49207
forLoopDone38957
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
for71726
	; Assigning single variable : j
	lda #0
	sta j
for6898
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
	bne for6898
forLoopDone34562
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd17719
	inc p1+1
WordAdd17719
	sta p1+0
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd84280
	inc screenmemory+1
WordAdd84280
	sta screenmemory+0
	inc i
	lda #23
	cmp i ;keep
	bne for71726
forLoopDone64029
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
for18902
	; Assigning single variable : j
	lda #0
	sta j
for32557
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
	bne for32557
forLoopDone10431
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd24220
	inc p1+1
WordAdd24220
	sta p1+0
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd85905
	inc screenmemory+1
WordAdd85905
	sta screenmemory+0
	inc i
	lda #23
	cmp i ;keep
	bne for18902
forLoopDone42723
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
for49613
	; Assigning single variable : j
	lda #0
	sta j
for58743
	; Assigning single variable : p1
	lda #0
	ldy j
	sta (p1),y
	inc j
	lda #22
	cmp j ;keep
	bne for58743
forLoopDone13899
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd37088
	inc p1+1
WordAdd37088
	sta p1+0
	inc i
	lda #23
	cmp i ;keep
	bne for49613
forLoopDone2671
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
for52386
	; Assigning single variable : j
	lda #0
	sta j
for67279
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
	bne for67279
forLoopDone29649
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd22351
	inc p1+1
WordAdd22351
	sta p1+0
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd7339
	inc screenmemory+1
WordAdd7339
	sta screenmemory+0
	inc i
	lda #20
	cmp i ;keep
	bne for52386
forLoopDone9663
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
for51742
	; Assigning single variable : screenmemory
	lda #3
	ldy i
	sta (screenmemory),y
	inc i
	lda #22
	cmp i ;keep
	bne for51742
forLoopDone12086
	; Assigning single variable : i
	lda #22
	sta i
for31349
	; Assigning single variable : screenmemory
	lda #0
	ldy i
	sta (screenmemory),y
	inc i
	lda #66
	cmp i ;keep
	bne for31349
forLoopDone50186
	; Assigning single variable : screenmemory
	lda #8
	ldx #148
	sta screenmemory
	stx screenmemory+1
	; Assigning single variable : i
	lda #0
	sta i
for95011
	; Assigning single variable : screenmemory
	lda #2
	ldy i
	sta (screenmemory),y
	inc i
	lda #8
	cmp i ;keep
	bne for95011
forLoopDone74133
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
for6773
	; Assigning single variable : x
	lda #1
	sta x
for95960
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy x
	lda (screenmemory),y
	
	; Compare with pure num / var optimization
	cmp #$3a;keep
	bne elsedoneblock74882
ConditionalTrueBlock32700
	; Assigning single variable : px
	lda x
	sta px
	; Assigning single variable : py
	lda y
	sta py
elseblock64045
elsedoneblock74882
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy x
	lda (screenmemory),y
	
	; Compare with pure num / var optimization
	cmp #$20;keep
	bne elsedoneblock50353
ConditionalTrueBlock42589
	; Assigning single variable : wx
	lda x
	sta wx
	; Assigning single variable : wy
	lda y
	sta wy
elseblock21386
elsedoneblock50353
	inc x
	lda #21
	cmp x ;keep
	bne for95960
forLoopDone61528
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd64794
	inc screenmemory+1
WordAdd64794
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
	bne for6773
forLoopDone63610
	rts
	
	
	; ***********  Defining procedure : WitchPickDirection
	;    Procedure type : User-defined procedure
	
WitchPickDirection
	; Binary clause Simplified: NOTEQUALS
	; Modulo
	lda #3
val_var3050 = $54
	sta val_var3050
	jsr Random
	sec
modulo85196
	sbc val_var3050
	bcs modulo85196
	adc val_var3050
	
	; Compare with pure num / var optimization
	cmp #$1;keep
	beq elsedoneblock65417
ConditionalTrueBlock77107
	; Binary clause Simplified: EQUALS
	lda wy
	; Compare with pure num / var optimization
	cmp py;keep
	bne elsedoneblock78499
ConditionalTrueBlock4213
	; Binary clause Simplified: GREATER
	lda wx
	; Compare with pure num / var optimization
	cmp px;keep
	bcc elseblock69939
	beq elseblock69939
ConditionalTrueBlock63590
	; Assigning single variable : wd
	lda #3
	sta wd
	jmp elsedoneblock6202
elseblock69939
	; Assigning single variable : wd
	lda #1
	sta wd
elsedoneblock6202
	rts
elseblock65186
elsedoneblock78499
	; Binary clause Simplified: EQUALS
	lda wx
	; Compare with pure num / var optimization
	cmp px;keep
	bne elsedoneblock52346
ConditionalTrueBlock11804
	; Binary clause Simplified: GREATER
	lda wy
	; Compare with pure num / var optimization
	cmp py;keep
	bcc elseblock20356
	beq elseblock20356
ConditionalTrueBlock65725
	; Assigning single variable : wd
	lda #0
	sta wd
	jmp elsedoneblock90952
elseblock20356
	; Assigning single variable : wd
	lda #2
	sta wd
elsedoneblock90952
	rts
elseblock94571
elsedoneblock52346
elseblock46092
elsedoneblock65417
	; Assigning single variable : wd
	inc wd
	; Binary clause Simplified: GREATER
	lda wd
	; Compare with pure num / var optimization
	cmp #$3;keep
	bcc elsedoneblock17603
	beq elsedoneblock17603
ConditionalTrueBlock68111
	; Assigning single variable : wd
	lda #0
	sta wd
elseblock75883
elsedoneblock17603
	rts
	
	
	; ***********  Defining procedure : UpdateWitch
	;    Procedure type : User-defined procedure
	
UpdateWitch
	; Binary clause: EQUALS
	lda wsteps
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bne binaryclausefailed2606
binaryclausesuccess84523
	lda #1; success
	jmp binaryclausefinished30164
binaryclausefailed2606
	lda #0 ; failed state
binaryclausefinished30164
	cmp #1
	beq ConditionalTrueBlock82339
	jmp elsedoneblock35344
ConditionalTrueBlock82339
	; Binary clause Simplified: EQUALS
	lda wdelay
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock16339
ConditionalTrueBlock82751
	; Assigning single variable : wdelay
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #40
val_var65625 = $54
	sta val_var65625
	jsr Random
	sec
modulo61079
	sbc val_var65625
	bcs modulo61079
	adc val_var65625
	
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
elseblock42530
elsedoneblock16339
	; Assigning single variable : wdelay
	dec wdelay
	; Binary clause Simplified: GREATER
	lda wdelay
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock68484
	beq elsedoneblock68484
ConditionalTrueBlock62547
	rts
elseblock22407
elsedoneblock68484
	jsr WitchPickDirection
	; Assigning single variable : wsteps
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #10
val_var4919 = $54
	sta val_var4919
	jsr Random
	sec
modulo21931
	sbc val_var4919
	bcs modulo21931
	adc val_var4919
	
	clc
	adc #2
	 ; end add / sub var with constant
	
	sta wsteps
	; Binary clause Simplified: GREATER
	lda wdcount
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock15494
	beq elsedoneblock15494
ConditionalTrueBlock20528
	; Assigning single variable : wdcount
	dec wdcount
	; Binary clause Simplified: EQUALS
	lda wdcount
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock51320
ConditionalTrueBlock51988
	; Assigning single variable : wdspeed
	; Load Byte array
	ldx level
	lda lwitch,x
	
	sta wdspeed
elseblock7394
elsedoneblock51320
elseblock88427
elsedoneblock15494
elseblock79174
elsedoneblock35344
	; Binary clause Simplified: NOTEQUALS
	; Modulo
	lda wdspeed
val_var94695 = $54
	sta val_var94695
	lda ticks
	sec
modulo49010
	sbc val_var94695
	bcs modulo49010
	adc val_var94695
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock86463
ConditionalTrueBlock93018
	rts
elseblock58752
elsedoneblock86463
	; Assigning single variable : wsteps
	dec wsteps
	lda wd
	cmp #$0 ;keep
	bne casenext56142
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
	bcs elseblock82853
ConditionalTrueBlock4978
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
	jmp elsedoneblock86966
elseblock82853
	; Assigning single variable : wsteps
	lda #0
	sta wsteps
elsedoneblock86966
	jmp caseend63179
casenext56142
	lda wd
	cmp #$1 ;keep
	bne casenext22272
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
	bcs elseblock61900
ConditionalTrueBlock89877
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
	jmp elsedoneblock72553
elseblock61900
	; Assigning single variable : wsteps
	lda #0
	sta wsteps
elsedoneblock72553
	jmp caseend63179
casenext22272
	lda wd
	cmp #$2 ;keep
	bne casenext62035
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
	bcs elseblock13861
ConditionalTrueBlock29154
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
	jmp elsedoneblock2906
elseblock13861
	; Assigning single variable : wsteps
	lda #0
	sta wsteps
elsedoneblock2906
	jmp caseend63179
casenext62035
	lda wd
	cmp #$3 ;keep
	bne casenext76784
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
	bcc dtnooverflow6797
	iny  ; overflow into high byte
dtnooverflow6797
	
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
	bcc dtnooverflow81763
	iny  ; overflow into high byte
dtnooverflow81763
	
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
	bcs elseblock46560
ConditionalTrueBlock10115
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
	jmp elsedoneblock80381
elseblock46560
	; Assigning single variable : wsteps
	lda #0
	sta wsteps
elsedoneblock80381
	jmp caseend63179
casenext76784
caseend63179
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
	bne elsedoneblock91313
ConditionalTrueBlock38329
	; Binary clause Simplified: EQUALS
	lda cy
	; Compare with pure num / var optimization
	cmp wy;keep
	bne elsedoneblock6616
ConditionalTrueBlock36626
	jsr HitWitch
elseblock37892
elsedoneblock6616
elseblock73349
elsedoneblock91313
	rts
	
	
	; ***********  Defining procedure : Ghost_Respawn
	;    Procedure type : User-defined procedure
	
Ghost_Respawn
	; Binary clause Simplified: GREATEREQUAL
	lda gcurrent
	; Compare with pure num / var optimization
	cmp gNumGhosts;keep
	bcc elsedoneblock67395
ConditionalTrueBlock96366
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
elseblock81515
elsedoneblock67395
	; Assigning single variable : j
	lda #0
	sta j
while47776
	; Binary clause: EQUALS
	lda j
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bne binaryclausefailed39807
binaryclausesuccess18834
	lda #1; success
	jmp binaryclausefinished49810
binaryclausefailed39807
	lda #0 ; failed state
binaryclausefinished49810
	cmp #1
	beq ConditionalTrueBlock73847
	jmp elsedoneblock85026
ConditionalTrueBlock73847
	; Assigning single variable : x
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #20
val_var53596 = $54
	sta val_var53596
	jsr Random
	sec
modulo95815
	sbc val_var53596
	bcs modulo95815
	adc val_var53596
	
	clc
	adc #1
	 ; end add / sub var with constant
	
	sta x
	; Assigning single variable : y
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #18
val_var27984 = $54
	sta val_var27984
	jsr Random
	sec
modulo48696
	sbc val_var27984
	bcs modulo48696
	adc val_var27984
	
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
	bcc dtnooverflow11382
	iny  ; overflow into high byte
dtnooverflow11382
	
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
	bne elsedoneblock25361
ConditionalTrueBlock99451
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
	bcs elseblock86628
ConditionalTrueBlock22924
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
	jmp elsedoneblock17135
elseblock86628
	; Assigning single variable : gmode
	lda #1
	ldx gcurrent
	sta gmode,x
	; Assigning single variable : gx
	lda #0
	sta gx,x
elsedoneblock17135
elseblock19716
elsedoneblock25361
	jmp while47776
elseblock29613
elsedoneblock85026
	rts
	
	
	; ***********  Defining procedure : PlaceAllGhosts
	;    Procedure type : User-defined procedure
	
PlaceAllGhosts
	; Assigning single variable : gcurrent
	lda #0
	sta gcurrent
for1996
	jsr Ghost_Respawn
	; Binary clause Simplified: LESS
	lda gcurrent
	; Compare with pure num / var optimization
	cmp gNumGhosts;keep
	bcs elsedoneblock52623
ConditionalTrueBlock65740
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
for66067
while81690
	; Binary clause Simplified: EQUALS
	lda ticks
	; Compare with pure num / var optimization
	cmp oldTick;keep
	bne elsedoneblock7629
ConditionalTrueBlock85292
	jmp while81690
elseblock38147
elsedoneblock7629
	; Assigning single variable : oldTick
	lda ticks
	sta oldTick
	inc i
	lda #30
	cmp i ;keep
	bne for66067
forLoopDone80529
elseblock88411
elsedoneblock52623
	inc gcurrent
	lda #10
	cmp gcurrent ;keep
	bne for1996
forLoopDone14107
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
for82140
forLoopFix62957
	; Binary clause Simplified: GREATER
	; Load Byte array
	ldx i
	lda gy,x
	
	; Compare with pure num / var optimization
	cmp #$3;keep
	bcc elsedoneblock36997
	beq elsedoneblock36997
ConditionalTrueBlock78721
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
elseblock34826
elsedoneblock36997
	inc i
	lda gNumGhosts
	cmp i ;keep
	beq forLoopDone19204
forLoopNotDone31287
	jmp for82140
forLoopDone19204
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
	bcs ColXConfirmed50614
	cmp #$8
	bcs NoCollision32973
ColXConfirmed50614
	lda y
	clc
	sbc py
	cmp #$fa
	bcs Collision1937
	cmp #$5
	bcs NoCollision32973
Collision1937
	lda #1
	jmp CollisionDone18865
NoCollision32973
	lda #0
CollisionDone18865
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	beq binaryclausefailed63748
binaryclausesuccess76620
	lda #1; success
	jmp binaryclausefinished43421
binaryclausefailed63748
	lda #0 ; failed state
binaryclausefinished43421
	cmp #1
	beq ConditionalTrueBlock39221
	jmp elseblock93660
	jmp elsedoneblock77883
ConditionalTrueBlock39221
	; Binary clause Simplified: EQUALS
	; Load Byte array
	ldx gcurrent
	lda gmode,x
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock52786
ConditionalTrueBlock5400
	; Binary clause Simplified: GREATER
	lda x
	; Compare with pure num / var optimization
	cmp px;keep
	bcc elsedoneblock73165
	beq elsedoneblock73165
ConditionalTrueBlock10
	; Assigning single variable : x
	dec x
elseblock64901
elsedoneblock73165
	; Binary clause Simplified: LESS
	lda x
	; Compare with pure num / var optimization
	cmp px;keep
	bcs elsedoneblock86238
ConditionalTrueBlock40926
	; Assigning single variable : x
	inc x
elseblock50732
elsedoneblock86238
elseblock1832
elsedoneblock52786
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
	bcc dtnooverflow72000
	iny  ; overflow into high byte
dtnooverflow72000
	
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
	bcc elsedoneblock42348
	beq elsedoneblock42348
ConditionalTrueBlock65457
	; Assigning single variable : x
	lda n
	sta x
elseblock73832
elsedoneblock42348
	; Binary clause Simplified: EQUALS
	; Load Byte array
	ldx gcurrent
	lda gmode,x
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock80569
ConditionalTrueBlock26169
	; Binary clause Simplified: GREATER
	lda y
	; Compare with pure num / var optimization
	cmp py;keep
	bcc elsedoneblock66788
	beq elsedoneblock66788
ConditionalTrueBlock33021
	; Assigning single variable : y
	dec y
elseblock1485
elsedoneblock66788
	; Binary clause Simplified: LESS
	lda y
	; Compare with pure num / var optimization
	cmp py;keep
	bcs elsedoneblock45325
ConditionalTrueBlock39634
	; Assigning single variable : y
	inc y
elseblock10647
elsedoneblock45325
elseblock72497
elsedoneblock80569
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
	bcc dtnooverflow91694
	iny  ; overflow into high byte
dtnooverflow91694
	
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
	bcc elsedoneblock45063
	beq elsedoneblock45063
ConditionalTrueBlock96630
	; Assigning single variable : y
	lda s
	sta y
elseblock64191
elsedoneblock45063
	jmp elsedoneblock77883
elseblock93660
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
	beq elseblock31492
ConditionalTrueBlock87142
	; Assigning single variable : x
	inc x
	jmp elsedoneblock930
elseblock31492
	; Assigning single variable : x
	dec x
elsedoneblock930
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
	bcc dtnooverflow27308
	iny  ; overflow into high byte
dtnooverflow27308
	
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
	bcc elsedoneblock38644
	beq elsedoneblock38644
ConditionalTrueBlock29780
	; Assigning single variable : x
	lda n
	sta x
elseblock61708
elsedoneblock38644
	; Binary clause Simplified: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda i
	and #2
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elseblock41087
ConditionalTrueBlock96522
	; Assigning single variable : y
	dec y
	jmp elsedoneblock76925
elseblock41087
	; Assigning single variable : y
	inc y
elsedoneblock76925
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
	bcc dtnooverflow34492
	iny  ; overflow into high byte
dtnooverflow34492
	
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
	bcc elsedoneblock51530
	beq elsedoneblock51530
ConditionalTrueBlock30296
	; Assigning single variable : y
	lda s
	sta y
elseblock98986
elsedoneblock51530
elsedoneblock77883
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
binary_clause_temp_var25139 = $54
	sta binary_clause_temp_var25139
	; Load Byte array
	ldx gcurrent
	lda gx,x
	
binary_clause_temp_2_var33694 = $56
	sta binary_clause_temp_2_var33694
	lda binary_clause_temp_var25139
	cmp binary_clause_temp_2_var33694;keep
	bne elsedoneblock68448
ConditionalTrueBlock77589
	; Binary clause Simplified: EQUALS
	lda cy
binary_clause_temp_var35236 = $54
	sta binary_clause_temp_var35236
	; Load Byte array
	ldx gcurrent
	lda gy,x
	
binary_clause_temp_2_var25015 = $56
	sta binary_clause_temp_2_var25015
	lda binary_clause_temp_var35236
	cmp binary_clause_temp_2_var25015;keep
	bne elsedoneblock90523
ConditionalTrueBlock74782
	; Assigning single variable : i
	lda gcurrent
	sta i
	jsr HitGhost
elseblock80282
elsedoneblock90523
elseblock7654
elsedoneblock68448
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
	bcc binaryclausefailed50212
	beq binaryclausefailed50212
binaryclausesuccess33934
	lda #1; success
	jmp binaryclausefinished87096
binaryclausefailed50212
	lda #0 ; failed state
binaryclausefinished87096
	cmp #1
	beq ConditionalTrueBlock49299
	jmp elsedoneblock754
ConditionalTrueBlock49299
	; Assigning single variable : gtime
	lda #0
	sta gtime
	; Binary clause Simplified: LESS
	lda gcurrent
	; Compare with pure num / var optimization
	cmp gNumGhosts;keep
	bcs elsedoneblock73950
ConditionalTrueBlock26095
	; Binary clause Simplified: EQUALS
	; Load Byte array
	ldx gcurrent
	lda gmode,x
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock43428
ConditionalTrueBlock94114
	jsr Ghost_Move
elseblock13069
elsedoneblock43428
	; Binary clause Simplified: GREATER
	; Load Byte array
	ldx gcurrent
	lda gmode,x
	
	; Compare with pure num / var optimization
	cmp #$1;keep
	bcc elsedoneblock1433
	beq elsedoneblock1433
ConditionalTrueBlock11625
	; Assigning single variable : gmode
	ldx gcurrent
	; Optimize byte array dec 
	dec gmode,x
	; Binary clause Simplified: EQUALS
	; Load Byte array
	lda gmode,x
	
	; Compare with pure num / var optimization
	cmp #$1;keep
	bne elsedoneblock86229
ConditionalTrueBlock87799
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
elseblock28447
elsedoneblock86229
elseblock43682
elsedoneblock1433
elseblock78674
elsedoneblock73950
	jsr DisplayGhosts
	; Assigning single variable : gcurrent
	inc gcurrent
	; Binary clause Simplified: EQUALS
	lda gcurrent
	; Compare with pure num / var optimization
	cmp #$a;keep
	bne elsedoneblock80589
ConditionalTrueBlock93799
	; Assigning single variable : gcurrent
	lda #0
	sta gcurrent
elseblock90528
elsedoneblock80589
elseblock92724
elsedoneblock754
	rts
	
	
	; ***********  Defining procedure : SpawnAZombie
	;    Procedure type : User-defined procedure
	
SpawnAZombie
	; Binary clause Simplified: GREATEREQUAL
	lda zNumZombies
	; Compare with pure num / var optimization
	cmp #$a;keep
	bcc elsedoneblock47194
ConditionalTrueBlock73990
	rts
elseblock96984
elsedoneblock47194
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
val_var35776 = $54
	sta val_var35776
	jsr Random
	sec
modulo86384
	sbc val_var35776
	bcs modulo86384
	adc val_var35776
	
	sta t
	; Assigning single variable : y
	lda #3
	sta y
for7071
	; Assigning single variable : x
	lda #1
	sta x
for95038
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
	bne elsedoneblock1662
ConditionalTrueBlock19353
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
	bne elsedoneblock23090
ConditionalTrueBlock52481
	; Assigning single variable : x
	lda #20
	sta x
	; Assigning single variable : y
	lda #21
	sta y
elseblock87195
elsedoneblock23090
	; Assigning single variable : i
	inc i
elseblock82313
elsedoneblock1662
	inc x
	lda #21
	cmp x ;keep
	bne for95038
forLoopDone92631
	inc y
	lda #22
	cmp y ;keep
	bne for7071
forLoopDone59963
	; Binary clause Simplified: GREATER
	lda n
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock27682
	beq elsedoneblock27682
ConditionalTrueBlock37736
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
elseblock96008
elsedoneblock27682
	rts
	
	
	; ***********  Defining procedure : DisplayZombies
	;    Procedure type : User-defined procedure
	
DisplayZombies
	; Assigning single variable : i
	lda #0
	sta i
for35733
forLoopFix64401
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
	bne casenext80456
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
	jmp caseend38589
casenext80456
	lda t
	cmp #$1 ;keep
	bne casenext62795
	; Assigning single variable : screenmemory
	lda #40
	ldy x
	sta (screenmemory),y
	jmp caseend38589
casenext62795
	lda t
	cmp #$2 ;keep
	bne casenext72784
	; Assigning single variable : screenmemory
	lda #41
	ldy x
	sta (screenmemory),y
	jmp caseend38589
casenext72784
	lda t
	cmp #$3 ;keep
	bne casenext43860
	; Assigning single variable : screenmemory
	lda #42
	ldy x
	sta (screenmemory),y
	jmp caseend38589
casenext43860
	; Assigning single variable : screenmemory
	lda #42
	ldy x
	sta (screenmemory),y
caseend38589
	; Assigning single variable : p1
	lda #5
	ldy x
	sta (p1),y
	inc i
	lda zNumZombies
	cmp i ;keep
	beq forLoopDone37945
forLoopNotDone18325
	jmp for35733
forLoopDone37945
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
	beq binaryclausesuccess13256
	bcs binaryclausefailed86733
binaryclausesuccess13256
	lda #1; success
	jmp binaryclausefinished53542
binaryclausefailed86733
	lda #0 ; failed state
binaryclausefinished53542
	cmp #1
	beq ConditionalTrueBlock45290
	jmp elsedoneblock97946
ConditionalTrueBlock45290
	; Binary clause Simplified: GREATER
	lda x
	; Compare with pure num / var optimization
	cmp px;keep
	bcc elsedoneblock28258
	beq elsedoneblock28258
ConditionalTrueBlock15859
	; Assigning single variable : x
	dec x
elseblock79136
elsedoneblock28258
	; Binary clause Simplified: LESS
	lda x
	; Compare with pure num / var optimization
	cmp px;keep
	bcs elsedoneblock43294
ConditionalTrueBlock80297
	; Assigning single variable : x
	inc x
elseblock10875
elsedoneblock43294
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
	bcc dtnooverflow67250
	iny  ; overflow into high byte
dtnooverflow67250
	
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
	bcc elsedoneblock34319
	beq elsedoneblock34319
ConditionalTrueBlock79885
	; Assigning single variable : x
	lda n
	sta x
elseblock43253
elsedoneblock34319
	; Binary clause Simplified: GREATER
	lda y
	; Compare with pure num / var optimization
	cmp py;keep
	bcc elsedoneblock4896
	beq elsedoneblock4896
ConditionalTrueBlock67324
	; Assigning single variable : y
	dec y
elseblock82719
elsedoneblock4896
	; Binary clause Simplified: LESS
	lda y
	; Compare with pure num / var optimization
	cmp py;keep
	bcs elsedoneblock96746
ConditionalTrueBlock98530
	; Assigning single variable : y
	inc y
elseblock29729
elsedoneblock96746
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
	bcc dtnooverflow55594
	iny  ; overflow into high byte
dtnooverflow55594
	
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
	bcc elsedoneblock97508
	beq elsedoneblock97508
ConditionalTrueBlock38858
	; Assigning single variable : y
	lda s
	sta y
elseblock83852
elsedoneblock97508
elseblock12505
elsedoneblock97946
	; Binary clause: EQUALS
	lda t
	; Compare with pure num / var optimization
	cmp #$2;keep
	; BC done
	bne binaryclausefailed29501
binaryclausesuccess11410
	lda #1; success
	jmp binaryclausefinished54833
binaryclausefailed29501
	lda #0 ; failed state
binaryclausefinished54833
	cmp #1
	beq ConditionalTrueBlock73513
	jmp elsedoneblock49730
ConditionalTrueBlock73513
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
	beq elseblock71000
ConditionalTrueBlock62069
	; Assigning single variable : x
	inc x
	jmp elsedoneblock82271
elseblock71000
	; Assigning single variable : x
	dec x
elsedoneblock82271
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
	bcc dtnooverflow11556
	iny  ; overflow into high byte
dtnooverflow11556
	
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
	bcc elsedoneblock14462
	beq elsedoneblock14462
ConditionalTrueBlock69388
	; Assigning single variable : x
	lda n
	sta x
elseblock54328
elsedoneblock14462
	; Binary clause Simplified: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda i
	and #2
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elseblock81423
ConditionalTrueBlock77259
	; Assigning single variable : y
	dec y
	jmp elsedoneblock2345
elseblock81423
	; Assigning single variable : y
	inc y
elsedoneblock2345
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
	bcc dtnooverflow25972
	iny  ; overflow into high byte
dtnooverflow25972
	
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
	bcc elsedoneblock10911
	beq elsedoneblock10911
ConditionalTrueBlock6657
	; Assigning single variable : y
	lda s
	sta y
elseblock98129
elsedoneblock10911
elseblock57344
elsedoneblock49730
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
binary_clause_temp_var49833 = $54
	sta binary_clause_temp_var49833
	; Load Byte array
	ldx zcurrent
	lda zx,x
	
binary_clause_temp_2_var848 = $56
	sta binary_clause_temp_2_var848
	lda binary_clause_temp_var49833
	cmp binary_clause_temp_2_var848;keep
	bne elsedoneblock69078
ConditionalTrueBlock67137
	; Binary clause Simplified: EQUALS
	lda cy
binary_clause_temp_var54589 = $54
	sta binary_clause_temp_var54589
	; Load Byte array
	ldx zcurrent
	lda zy,x
	
binary_clause_temp_2_var37283 = $56
	sta binary_clause_temp_2_var37283
	lda binary_clause_temp_var54589
	cmp binary_clause_temp_2_var37283;keep
	bne elsedoneblock11310
ConditionalTrueBlock98623
	; Assigning single variable : i
	lda zcurrent
	sta i
	jsr HitZombie
elseblock15055
elsedoneblock11310
elseblock37126
elsedoneblock69078
	rts
	
	
	; ***********  Defining procedure : UpdateZombies
	;    Procedure type : User-defined procedure
	
UpdateZombies
	; Binary clause Simplified: EQUALS
	lda zNumZombies
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock63042
ConditionalTrueBlock26555
	rts
elseblock24893
elsedoneblock63042
	; Binary clause Simplified: EQUALS
	; Modulo
	lda ztime
val_var78979 = $54
	sta val_var78979
	lda ticks
	sec
modulo34589
	sbc val_var78979
	bcs modulo34589
	adc val_var78979
	
	; Compare with pure num / var optimization
	cmp #$1;keep
	bne elsedoneblock46550
ConditionalTrueBlock75789
	; Binary clause Simplified: LESS
	lda zcurrent
	; Compare with pure num / var optimization
	cmp zNumZombies;keep
	bcs elsedoneblock14995
ConditionalTrueBlock35247
	; Binary clause Simplified: EQUALS
	; Load Byte array
	ldx zcurrent
	lda zspawn,x
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock3441
ConditionalTrueBlock97218
	jsr Zombie_Move
elseblock64225
elsedoneblock3441
	; Binary clause Simplified: GREATER
	; Load Byte array
	ldx zcurrent
	lda zspawn,x
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock46894
	beq elsedoneblock46894
ConditionalTrueBlock64286
	; Assigning single variable : zspawn
	ldx zcurrent
	; Optimize byte array dec 
	dec zspawn,x
elseblock71894
elsedoneblock46894
elseblock34853
elsedoneblock14995
	jsr DisplayZombies
	; Assigning single variable : zcurrent
	inc zcurrent
	; Binary clause Simplified: EQUALS
	lda zcurrent
	; Compare with pure num / var optimization
	cmp #$a;keep
	bne elsedoneblock78337
ConditionalTrueBlock43058
	; Assigning single variable : zcurrent
	lda #0
	sta zcurrent
elseblock58082
elsedoneblock78337
elseblock53428
elsedoneblock46550
	rts
	
	
	; ***********  Defining procedure : SpawnCandy
	;    Procedure type : User-defined procedure
	
SpawnCandy
	; Assigning single variable : x
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #20
val_var48617 = $54
	sta val_var48617
	jsr Random
	sec
modulo85314
	sbc val_var48617
	bcs modulo85314
	adc val_var48617
	
	clc
	adc #1
	 ; end add / sub var with constant
	
	sta x
	; Assigning single variable : y
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #16
val_var27998 = $54
	sta val_var27998
	jsr Random
	sec
modulo35680
	sbc val_var27998
	bcs modulo35680
	adc val_var27998
	
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
while58641
	; Binary clause Simplified: EQUALS
	lda t
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock16035
ConditionalTrueBlock71006
	; Binary clause Simplified: EQUALS
	; Load Byte array
	; Load pointer array
	ldy x
	lda (screenmemory),y
	
	tax
	lda cMask,x
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock57257
ConditionalTrueBlock32110
	; Assigning single variable : t
	lda #1
	sta t
elseblock34702
elsedoneblock57257
	; Binary clause Simplified: EQUALS
	lda t
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock36299
ConditionalTrueBlock11886
	; Assigning single variable : x
	inc x
	; Binary clause Simplified: GREATER
	lda x
	; Compare with pure num / var optimization
	cmp #$14;keep
	bcc elsedoneblock79269
	beq elsedoneblock79269
ConditionalTrueBlock96619
	; Assigning single variable : x
	lda #1
	sta x
	; Assigning single variable : y
	inc y
	; Binary clause Simplified: GREATER
	lda y
	; Compare with pure num / var optimization
	cmp #$16;keep
	bcc elsedoneblock25413
	beq elsedoneblock25413
ConditionalTrueBlock45820
	; Assigning single variable : y
	lda #4
	sta y
elseblock74446
elsedoneblock25413
elseblock63925
elsedoneblock79269
elseblock95171
elsedoneblock36299
	jmp while58641
elseblock71564
elsedoneblock16035
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
	bne elsedoneblock36448
ConditionalTrueBlock20424
	; Assigning single variable : psnd2
	lda #<sndCAppear
	ldx #>sndCAppear
	sta psnd2
	stx psnd2+1
	; Assigning single variable : vsnd2
	lda #250
	sta vsnd2
	jsr PlaySnd2
elseblock76250
elsedoneblock36448
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
	bcc elsedoneblock99267
	beq elsedoneblock99267
ConditionalTrueBlock64872
	; Assigning single variable : p1
	lda #3
	ldy cx
	sta (p1),y
elseblock46647
elsedoneblock99267
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
for88709
	; Assigning single variable : screenmemory
	lda #0
	ldy i
	sta (screenmemory),y
	inc i
	lda #6
	cmp i ;keep
	bcs for88709
forLoopDone22254
	; Assigning single variable : i
	lda #1
	sta i
for69528
	; Binary clause Simplified: GREATEREQUAL
	lda candy
	; Compare with pure num / var optimization
	cmp i;keep
	bcc elsedoneblock52470
ConditionalTrueBlock34303
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
elseblock987
elsedoneblock52470
	; Binary clause Simplified: GREATEREQUAL
	lda witchHits
	; Compare with pure num / var optimization
	cmp i;keep
	bcc elsedoneblock91440
ConditionalTrueBlock33726
	; Assigning single variable : screenmemory
	lda #38
	ldy i
	sta (screenmemory),y
	; Assigning single variable : p1
	lda #1
	sta (p1),y
elseblock8003
elsedoneblock91440
	inc i
	lda #6
	cmp i ;keep
	bcs for69528
forLoopDone62508
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
	bne elsedoneblock69027
ConditionalTrueBlock12376
	; Assigning single variable : cx
	inc cx
elseblock37541
elsedoneblock69027
	; Binary clause Simplified: NOTEQUALS
	lda cdx
	; Compare with pure num / var optimization
	cmp #$1;keep
	beq elsedoneblock68399
ConditionalTrueBlock48226
	; Assigning single variable : cx
	dec cx
elseblock31765
elsedoneblock68399
	; Binary clause Simplified: EQUALS
	; Load Byte array
	; Load pointer array
	ldy cx
	lda (screenmemory),y
	
	tax
	lda cMask,x
	
	; Compare with pure num / var optimization
	cmp #$a;keep
	bne elsedoneblock20688
ConditionalTrueBlock19358
	; Assigning single variable : cx
	lda x
	sta cx
	; Binary clause Simplified: LESS
	lda candyscr
	; Compare with pure num / var optimization
	cmp #$4;keep
	bcs elsedoneblock41306
ConditionalTrueBlock49492
	; Assigning single variable : candyscr
	inc candyscr
elseblock12644
elsedoneblock41306
elseblock80532
elsedoneblock20688
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
	bne tempfail69976
binaryclausesuccess56387
	jmp ConditionalTrueBlock77949
tempfail69976
	; Binary clause: EQUALS
	lda clife
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bne elseblock76898
binaryclausesuccess64626
ConditionalTrueBlock77949
	jsr EndCandy
elseblock76898
elsedoneblock86386
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
	beq binaryclausefailed63290
binaryclausesuccess66994
	lda #1; success
	jmp binaryclausefinished20160
binaryclausefailed63290
	lda #0 ; failed state
binaryclausefinished20160
	cmp #1
	beq ConditionalTrueBlock17458
	jmp elsedoneblock63181
ConditionalTrueBlock17458
	; Binary clause: EQUALS
	lda clife
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bne binaryclausefailed93026
binaryclausesuccess46711
	lda #1; success
	jmp binaryclausefinished52861
binaryclausefailed93026
	lda #0 ; failed state
binaryclausefinished52861
	cmp #1
	beq ConditionalTrueBlock51142
	jmp elsedoneblock45488
ConditionalTrueBlock51142
	; Binary clause: GREATER
	lda candy
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bcc binaryclausefailed26503
	beq binaryclausefailed26503
binaryclausesuccess90868
	lda #1; success
	jmp binaryclausefinished12661
binaryclausefailed26503
	lda #0 ; failed state
binaryclausefinished12661
	cmp #1
	beq ConditionalTrueBlock52368
	jmp elsedoneblock84821
ConditionalTrueBlock52368
	; Assigning single variable : cy
	lda py
	sta cy
	; Binary clause Simplified: EQUALS
	lda pd
	; Compare with pure num / var optimization
	cmp #$3a;keep
	bne elsedoneblock58962
ConditionalTrueBlock35500
	; Assigning single variable : cdx
	lda #255
	sta cdx
elseblock38155
elsedoneblock58962
	; Binary clause Simplified: EQUALS
	lda pd
	; Compare with pure num / var optimization
	cmp #$3d;keep
	bne elsedoneblock62638
ConditionalTrueBlock77717
	; Assigning single variable : cdx
	lda #1
	sta cdx
elseblock52958
elsedoneblock62638
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
	bne elsedoneblock19072
ConditionalTrueBlock77140
	; Assigning single variable : x
	; 8 bit binop
	; Add/sub where right value is constant number
	lda px
	clc
	adc #1
	 ; end add / sub var with constant
	
	sta x
elseblock85770
elsedoneblock19072
	; Binary clause Simplified: NOTEQUALS
	lda cdx
	; Compare with pure num / var optimization
	cmp #$1;keep
	beq elsedoneblock60494
ConditionalTrueBlock41882
	; Assigning single variable : x
	; 8 bit binop
	; Add/sub where right value is constant number
	lda px
	sec
	sbc #1
	 ; end add / sub var with constant
	
	sta x
elseblock77610
elsedoneblock60494
	; Binary clause Simplified: EQUALS
	; Load Byte array
	; Load pointer array
	ldy x
	lda (screenmemory),y
	
	tax
	lda cMask,x
	
	; Compare with pure num / var optimization
	cmp #$a;keep
	bne elseblock11053
ConditionalTrueBlock37953
	; Assigning single variable : cy
	lda #0
	sta cy
	; Assigning single variable : cx
	sta cx
	; Assigning single variable : cdx
	sta cdx
	jmp elsedoneblock15802
elseblock11053
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
elsedoneblock15802
elseblock44477
elsedoneblock84821
elseblock13990
elsedoneblock45488
elseblock35668
elsedoneblock63181
	; Binary clause Simplified: GREATER
	lda clife
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock19986
	beq elsedoneblock19986
ConditionalTrueBlock91614
	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda ticks
	and #1
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock5107
ConditionalTrueBlock22037
	jsr MoveCandy
elseblock85544
elsedoneblock5107
elseblock60591
elsedoneblock19986
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
	beq binaryclausefailed39021
binaryclausesuccess95191
	lda #1; success
	jmp binaryclausefinished72835
binaryclausefailed39021
	lda #0 ; failed state
binaryclausefinished72835
	cmp #1
	beq ConditionalTrueBlock72606
	jmp elsedoneblock58697
ConditionalTrueBlock72606
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
	bcc dtnooverflow27495
	iny  ; overflow into high byte
dtnooverflow27495
	
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
	bcc dtnooverflow84488
	iny  ; overflow into high byte
dtnooverflow84488
	
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
	bcs elsedoneblock10336
ConditionalTrueBlock47983
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
elseblock88323
elsedoneblock10336
	rts
elseblock71149
elsedoneblock58697
	; Binary clause: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1
	and #1
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	beq binaryclausefailed19361
binaryclausesuccess6240
	lda #1; success
	jmp binaryclausefinished18295
binaryclausefailed19361
	lda #0 ; failed state
binaryclausefinished18295
	cmp #1
	beq ConditionalTrueBlock33302
	jmp elsedoneblock54091
ConditionalTrueBlock33302
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
	bcs elsedoneblock93053
ConditionalTrueBlock3375
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
elseblock9341
elsedoneblock93053
	rts
elseblock79465
elsedoneblock54091
	; Binary clause: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1
	and #2
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	beq binaryclausefailed3580
binaryclausesuccess14780
	lda #1; success
	jmp binaryclausefinished52774
binaryclausefailed3580
	lda #0 ; failed state
binaryclausefinished52774
	cmp #1
	beq ConditionalTrueBlock11558
	jmp elsedoneblock1533
ConditionalTrueBlock11558
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
	bcs elsedoneblock27019
ConditionalTrueBlock77479
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
elseblock26763
elsedoneblock27019
	rts
elseblock82152
elsedoneblock1533
	; Binary clause: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1
	and #4
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	beq binaryclausefailed71166
binaryclausesuccess22256
	lda #1; success
	jmp binaryclausefinished84129
binaryclausefailed71166
	lda #0 ; failed state
binaryclausefinished84129
	cmp #1
	beq ConditionalTrueBlock27746
	jmp elsedoneblock74917
ConditionalTrueBlock27746
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
	bcs elsedoneblock54887
ConditionalTrueBlock75429
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
elseblock85608
elsedoneblock54887
	rts
elseblock50254
elsedoneblock74917
	rts
	
	
	; ***********  Defining procedure : UpdatePlayer
	;    Procedure type : User-defined procedure
	
UpdatePlayer
	; Binary clause Simplified: EQUALS
	; Modulo
	lda #2
val_var835 = $54
	sta val_var835
	lda ticks
	sec
modulo13882
	sbc val_var835
	bcs modulo13882
	adc val_var835
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock63628
ConditionalTrueBlock72878
	; Assigning single variable : pan
	inc pan
elseblock56737
elsedoneblock63628
	; Binary clause Simplified: GREATER
	lda pan
	; Compare with pure num / var optimization
	cmp #$5;keep
	bcc elsedoneblock78685
	beq elsedoneblock78685
ConditionalTrueBlock74317
	; Assigning single variable : pan
	lda #0
	sta pan
elseblock79422
elsedoneblock78685
	; Binary clause Simplified: EQUALS
	lda joy1
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock28399
ConditionalTrueBlock48707
	; Assigning single variable : pstep
	lda #0
	sta pstep
elseblock48623
elsedoneblock28399
	; Binary clause Simplified: GREATER
	lda pstep
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock30607
	beq elsedoneblock30607
ConditionalTrueBlock92118
	; Assigning single variable : pstep
	dec pstep
elseblock31527
elsedoneblock30607
	; Binary clause Simplified: EQUALS
	lda pstep
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock62074
ConditionalTrueBlock75412
	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1
	and #16
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock75737
ConditionalTrueBlock3002
	jsr MovePlayer
	; Binary clause Simplified: GREATER
	lda pstep
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock10762
	beq elsedoneblock10762
ConditionalTrueBlock85193
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
	bne elseblock69538
ConditionalTrueBlock65223
	; Assigning single variable : stepsnd
	lda #30
	sta stepsnd
	jmp elsedoneblock91742
elseblock69538
	; Assigning single variable : stepsnd
	lda #0
	sta stepsnd
elsedoneblock91742
elseblock33817
elsedoneblock10762
elseblock89735
elsedoneblock75737
elseblock2908
elsedoneblock62074
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
	bne elsedoneblock39467
ConditionalTrueBlock94814
	; Binary clause Simplified: LESS
	lda candy
binary_clause_temp_var90579 = $54
	sta binary_clause_temp_var90579
	; 8 bit binop
	; Add/sub where right value is constant number
	lda #6
	sec
	sbc witchHits
	 ; end add / sub var with constant
	
binary_clause_temp_2_var35338 = $56
	sta binary_clause_temp_2_var35338
	lda binary_clause_temp_var90579
	cmp binary_clause_temp_2_var35338;keep
	bcs elsedoneblock28386
ConditionalTrueBlock93916
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
	bcc elsedoneblock82857
	beq elsedoneblock82857
ConditionalTrueBlock7396
	; Assigning single variable : candyscr
	dec candyscr
elseblock74418
elsedoneblock82857
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
elseblock18732
elsedoneblock28386
elseblock867
elsedoneblock39467
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
	bcc elsedoneblock74635
ConditionalTrueBlock1425
	jsr SpawnAZombie
elseblock66707
elsedoneblock74635
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
for51605
	; Assigning single variable : screenmemory
	lda #0
	ldy i
	sta (screenmemory),y
	inc i
	lda #8
	cmp i ;keep
	bne for51605
forLoopDone1653
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #2
	bcc WordAdd35281
	inc screenmemory+1
WordAdd35281
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
	bne casenext74276
	jsr ClearTime
	clc
	lda #<t11
	adc #0
	ldy #>t11
	sta print_text+0
	sty print_text+1
	ldx #4 ; optimized, look out for bugs
	jsr printstring
	jmp caseend4039
casenext74276
	lda moonPos
	cmp #$6 ;keep
	bne casenext18064
	jsr ClearTime
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	sec
	sbc #2
	bcs WordAdd76022
	dec screenmemory+1
WordAdd76022
	sta screenmemory+0
	clc
	lda #<t12
	adc #0
	ldy #>t12
	sta print_text+0
	sty print_text+1
	ldx #8 ; optimized, look out for bugs
	jsr printstring
	jmp caseend4039
casenext18064
	lda moonPos
	cmp #$8 ;keep
	bne casenext57360
	jsr ClearTime
	clc
	lda #<t01
	adc #0
	ldy #>t01
	sta print_text+0
	sty print_text+1
	ldx #4 ; optimized, look out for bugs
	jsr printstring
	jmp caseend4039
casenext57360
	lda moonPos
	cmp #$a ;keep
	bne casenext6577
	jsr ClearTime
	clc
	lda #<t02
	adc #0
	ldy #>t02
	sta print_text+0
	sty print_text+1
	ldx #4 ; optimized, look out for bugs
	jsr printstring
	jmp caseend4039
casenext6577
	lda moonPos
	cmp #$c ;keep
	bne casenext7433
	jsr ClearTime
	clc
	lda #<t03
	adc #0
	ldy #>t03
	sta print_text+0
	sty print_text+1
	ldx #4 ; optimized, look out for bugs
	jsr printstring
	jmp caseend4039
casenext7433
	lda moonPos
	cmp #$e ;keep
	bne casenext46939
	jsr ClearTime
	clc
	lda #<t04
	adc #0
	ldy #>t04
	sta print_text+0
	sty print_text+1
	ldx #4 ; optimized, look out for bugs
	jsr printstring
	jmp caseend4039
casenext46939
	lda moonPos
	cmp #$10 ;keep
	bne casenext77694
	jsr ClearTime
	clc
	lda #<t05
	adc #0
	ldy #>t05
	sta print_text+0
	sty print_text+1
	ldx #4 ; optimized, look out for bugs
	jsr printstring
	jmp caseend4039
casenext77694
	lda moonPos
	cmp #$12 ;keep
	bne casenext21414
	jsr ClearTime
	clc
	lda #<t06
	adc #0
	ldy #>t06
	sta print_text+0
	sty print_text+1
	ldx #4 ; optimized, look out for bugs
	jsr printstring
	jmp caseend4039
casenext21414
caseend4039
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
	bne elsedoneblock65069
ConditionalTrueBlock25421
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
	bne elsedoneblock41751
ConditionalTrueBlock18466
	; Binary clause Simplified: LESS
	lda candyscr
	; Compare with pure num / var optimization
	cmp #$3;keep
	bcs elsedoneblock1443
ConditionalTrueBlock83088
	jsr SpawnCandy
elseblock45772
elsedoneblock1443
elseblock42196
elsedoneblock41751
	; Binary clause Simplified: EQUALS
	lda cloudPos
	; Compare with pure num / var optimization
	cmp #$16;keep
	bne elsedoneblock48901
ConditionalTrueBlock72886
	; Assigning single variable : cloudPos
	lda #0
	sta cloudPos
	jsr MoveMoon
	jsr DisplayTime
	; Binary clause Simplified: LESS
	lda candyscr
	; Compare with pure num / var optimization
	cmp #$3;keep
	bcs elsedoneblock57620
ConditionalTrueBlock66332
	jsr SpawnCandy
elseblock74863
elsedoneblock57620
elseblock62426
elsedoneblock48901
	; Assigning single variable : screenmemory
	lda #44
	ldx #148
	sta screenmemory
	stx screenmemory+1
	; Assigning single variable : screenmemory
	lda #1
	ldy cloudPos
	sta (screenmemory),y
elseblock10069
elsedoneblock65069
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
while66392
	; Binary clause Simplified: NOTEQUALS
	; Load pointer array
	ldy #$0
	lda (psnd1),y
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock49222
ConditionalTrueBlock88018
	jmp while66392
elseblock62900
elsedoneblock49222
	jsr StopSounds
	; Assigning single variable : sndFlag
	lda #0
	sta sndFlag
while97004
	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1pressed
	and #16
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock98447
ConditionalTrueBlock43983
	; Assigning single variable : x
	lda y
	sta x
	; Assigning single variable : y
	inc y
	; Binary clause Simplified: GREATER
	; Compare with pure num / var optimization
	cmp #$7;keep
	bcc elsedoneblock17808
	beq elsedoneblock17808
ConditionalTrueBlock10512
	; Assigning single variable : y
	lda #1
	sta y
elseblock64576
elsedoneblock17808
	jsr callReadJoy1
	; Assigning single variable : t
	lda #0
	sta t
for16255
	; Assigning single variable : p1
	lda x
	ldy t
	sta (p1),y
	; Assigning single variable : x
	inc x
	; Binary clause Simplified: GREATER
	; Compare with pure num / var optimization
	cmp #$7;keep
	bcc elsedoneblock53220
	beq elsedoneblock53220
ConditionalTrueBlock37114
	; Assigning single variable : x
	lda #1
	sta x
elseblock15612
elsedoneblock53220
	inc t
	lda #14
	cmp t ;keep
	bne for16255
forLoopDone4415
	jmp while97004
elseblock26950
elsedoneblock98447
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
	bne elsedoneblock84471
ConditionalTrueBlock98941
	; Binary clause Simplified: EQUALS
	lda cloudPos
	; Compare with pure num / var optimization
	cmp #$15;keep
	bne elsedoneblock26572
ConditionalTrueBlock26423
	; Assigning single variable : gameMode
	lda #1
	sta gameMode
	; Assigning single variable : screenmemory
	lda #48
	ldx #16
	sta screenmemory
	stx screenmemory+1
printstring_call49946
	clc
	lda #<printstring_text84454
	adc #0
	ldy #>printstring_text84454
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
elseblock45941
elsedoneblock26572
elseblock90107
elsedoneblock84471
	; Binary clause: EQUALS
	lda px
	; Compare with pure num / var optimization
	cmp lx;keep
	; BC done
	bne binaryclausefailed55662
binaryclausesuccess91334
	lda #1; success
	jmp binaryclausefinished20752
binaryclausefailed55662
	lda #0 ; failed state
binaryclausefinished20752
	cmp #1
	beq ConditionalTrueBlock19519
	jmp elsedoneblock73769
ConditionalTrueBlock19519
	; Binary clause: EQUALS
	lda py
	; Compare with pure num / var optimization
	cmp ly;keep
	; BC done
	bne binaryclausefailed36665
binaryclausesuccess46386
	lda #1; success
	jmp binaryclausefinished40322
binaryclausefailed36665
	lda #0 ; failed state
binaryclausefinished40322
	cmp #1
	beq ConditionalTrueBlock80396
	jmp elsedoneblock72296
ConditionalTrueBlock80396
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
val_var91977 = $54
	sta val_var91977
	jsr Random
	sec
modulo92061
	sbc val_var91977
	bcs modulo92061
	adc val_var91977
	
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
	bcs elsedoneblock86766
ConditionalTrueBlock84908
	; Assigning single variable : lives
	inc lives
elseblock22500
elsedoneblock86766
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
elseblock36404
elsedoneblock72296
elseblock27016
elsedoneblock73769
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
	bne elsedoneblock70038
ConditionalTrueBlock31400
	; Binary clause Simplified: EQUALS
	lda py
	; Compare with pure num / var optimization
	cmp wy;keep
	bne elsedoneblock98952
ConditionalTrueBlock27851
	; Assigning single variable : gameMode
	lda #1
	sta gameMode
	; Assigning single variable : screenmemory
	lda #48
	ldx #16
	sta screenmemory
	stx screenmemory+1
printstring_call68864
	clc
	lda #<printstring_text92190
	adc #0
	ldy #>printstring_text92190
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
elseblock53814
elsedoneblock98952
elseblock67935
elsedoneblock70038
	; Binary clause Simplified: EQUALS
	lda pbgchar
	; Compare with pure num / var optimization
	cmp #$26;keep
	bne elsedoneblock64672
ConditionalTrueBlock55630
	; Assigning single variable : gameMode
	lda #1
	sta gameMode
	; Assigning single variable : screenmemory
	lda #48
	ldx #16
	sta screenmemory
	stx screenmemory+1
printstring_call89325
	clc
	lda #<printstring_text24221
	adc #0
	ldy #>printstring_text24221
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
elseblock87186
elsedoneblock64672
	; Assigning single variable : i
	lda #0
	sta i
for19389
	; Binary clause Simplified: EQUALS
	lda px
binary_clause_temp_var29326 = $54
	sta binary_clause_temp_var29326
	; Load Byte array
	ldx i
	lda gx,x
	
binary_clause_temp_2_var39195 = $56
	sta binary_clause_temp_2_var39195
	lda binary_clause_temp_var29326
	cmp binary_clause_temp_2_var39195;keep
	bne elsedoneblock10541
ConditionalTrueBlock78491
	; Binary clause Simplified: EQUALS
	lda py
binary_clause_temp_var16538 = $54
	sta binary_clause_temp_var16538
	; Load Byte array
	ldx i
	lda gy,x
	
binary_clause_temp_2_var58132 = $56
	sta binary_clause_temp_2_var58132
	lda binary_clause_temp_var16538
	cmp binary_clause_temp_2_var58132;keep
	bne elsedoneblock72592
ConditionalTrueBlock78520
	; Assigning single variable : gameMode
	lda #1
	sta gameMode
	; Assigning single variable : screenmemory
	lda #48
	ldx #16
	sta screenmemory
	stx screenmemory+1
printstring_call73348
	clc
	lda #<printstring_text3165
	adc #0
	ldy #>printstring_text3165
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
elseblock39012
elsedoneblock72592
elseblock39234
elsedoneblock10541
	inc i
	lda gNumGhosts
	cmp i ;keep
	bne for19389
forLoopDone72147
	; Binary clause Simplified: GREATER
	lda zNumZombies
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock22328
	beq elsedoneblock22328
ConditionalTrueBlock12582
	; Assigning single variable : i
	lda #0
	sta i
for91243
	; Binary clause Simplified: EQUALS
	lda px
binary_clause_temp_var31367 = $54
	sta binary_clause_temp_var31367
	; Load Byte array
	ldx i
	lda zx,x
	
binary_clause_temp_2_var6647 = $56
	sta binary_clause_temp_2_var6647
	lda binary_clause_temp_var31367
	cmp binary_clause_temp_2_var6647;keep
	bne elsedoneblock73855
ConditionalTrueBlock92008
	; Binary clause Simplified: EQUALS
	lda py
binary_clause_temp_var53694 = $54
	sta binary_clause_temp_var53694
	; Load Byte array
	ldx i
	lda zy,x
	
binary_clause_temp_2_var18446 = $56
	sta binary_clause_temp_2_var18446
	lda binary_clause_temp_var53694
	cmp binary_clause_temp_2_var18446;keep
	bne elsedoneblock51477
ConditionalTrueBlock33503
	; Assigning single variable : gameMode
	lda #1
	sta gameMode
	; Assigning single variable : screenmemory
	lda #48
	ldx #16
	sta screenmemory
	stx screenmemory+1
printstring_call59305
	clc
	lda #<printstring_text73924
	adc #0
	ldy #>printstring_text73924
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
elseblock18967
elsedoneblock51477
elseblock83202
elsedoneblock73855
	inc i
	lda zNumZombies
	cmp i ;keep
	bne for91243
forLoopDone21738
elseblock30058
elsedoneblock22328
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
	bne elsedoneblock22215
ConditionalTrueBlock23015
	rts
elseblock17566
elsedoneblock22215
	; Binary clause Simplified: EQUALS
	lda cx
	; Compare with pure num / var optimization
	cmp wx;keep
	bne elsedoneblock30757
ConditionalTrueBlock74495
	; Binary clause Simplified: EQUALS
	lda cy
	; Compare with pure num / var optimization
	cmp wy;keep
	bne elsedoneblock4183
ConditionalTrueBlock2788
	jsr HitWitch
elseblock53046
elsedoneblock4183
elseblock50352
elsedoneblock30757
	; Assigning single variable : i
	lda #0
	sta i
for63043
	; Binary clause Simplified: EQUALS
	lda cx
binary_clause_temp_var92857 = $54
	sta binary_clause_temp_var92857
	; Load Byte array
	ldx i
	lda gx,x
	
binary_clause_temp_2_var70797 = $56
	sta binary_clause_temp_2_var70797
	lda binary_clause_temp_var92857
	cmp binary_clause_temp_2_var70797;keep
	bne elsedoneblock43989
ConditionalTrueBlock11917
	; Binary clause Simplified: EQUALS
	lda cy
binary_clause_temp_var74771 = $54
	sta binary_clause_temp_var74771
	; Load Byte array
	ldx i
	lda gy,x
	
binary_clause_temp_2_var39334 = $56
	sta binary_clause_temp_2_var39334
	lda binary_clause_temp_var74771
	cmp binary_clause_temp_2_var39334;keep
	bne elsedoneblock82280
ConditionalTrueBlock20799
	jsr HitGhost
elseblock32288
elsedoneblock82280
elseblock85191
elsedoneblock43989
	inc i
	lda gNumGhosts
	cmp i ;keep
	bne for63043
forLoopDone35205
	; Binary clause Simplified: GREATER
	lda zNumZombies
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock6181
	beq elsedoneblock6181
ConditionalTrueBlock71220
	; Assigning single variable : i
	lda #0
	sta i
for64335
	; Binary clause Simplified: EQUALS
	lda cx
binary_clause_temp_var96585 = $54
	sta binary_clause_temp_var96585
	; Load Byte array
	ldx i
	lda zx,x
	
binary_clause_temp_2_var86216 = $56
	sta binary_clause_temp_2_var86216
	lda binary_clause_temp_var96585
	cmp binary_clause_temp_2_var86216;keep
	bne elsedoneblock42157
ConditionalTrueBlock86872
	; Binary clause Simplified: EQUALS
	lda cy
binary_clause_temp_var36872 = $54
	sta binary_clause_temp_var36872
	; Load Byte array
	ldx i
	lda zy,x
	
binary_clause_temp_2_var29832 = $56
	sta binary_clause_temp_2_var29832
	lda binary_clause_temp_var36872
	cmp binary_clause_temp_2_var29832;keep
	bne elsedoneblock22172
ConditionalTrueBlock71192
	jsr HitZombie
elseblock71083
elsedoneblock22172
elseblock38230
elsedoneblock42157
	inc i
	lda zNumZombies
	cmp i ;keep
	bne for64335
forLoopDone40215
elseblock40845
elsedoneblock6181
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
printstring_call22806
	clc
	lda #<printstring_text45482
	adc #0
	ldy #>printstring_text45482
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
while64091
	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1pressed
	and #16
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock62918
ConditionalTrueBlock48586
	; Assigning single variable : x
	lda y
	sta x
	; Assigning single variable : y
	inc y
	; Binary clause Simplified: GREATER
	; Compare with pure num / var optimization
	cmp #$7;keep
	bcc elsedoneblock23556
	beq elsedoneblock23556
ConditionalTrueBlock74969
	; Assigning single variable : y
	lda #1
	sta y
elseblock53810
elsedoneblock23556
	jsr callReadJoy1
	; Assigning single variable : t
	lda #0
	sta t
for66317
	; Assigning single variable : p1
	lda x
	ldy t
	sta (p1),y
	; Assigning single variable : x
	inc x
	; Binary clause Simplified: GREATER
	; Compare with pure num / var optimization
	cmp #$7;keep
	bcc elsedoneblock27397
	beq elsedoneblock27397
ConditionalTrueBlock23840
	; Assigning single variable : x
	lda #1
	sta x
elseblock55018
elsedoneblock27397
	inc t
	lda #18
	cmp t ;keep
	bne for66317
forLoopDone32192
	jmp while64091
elseblock2050
elsedoneblock62918
	rts
	
	
	; ***********  Defining procedure : GameLoop
	;    Procedure type : User-defined procedure
	
GameLoop
while99815
	; Binary clause: EQUALS
	lda gameMode
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bne binaryclausefailed28585
binaryclausesuccess30773
	lda #1; success
	jmp binaryclausefinished88837
binaryclausefailed28585
	lda #0 ; failed state
binaryclausefinished88837
	cmp #1
	beq ConditionalTrueBlock20440
	jmp elsedoneblock12400
ConditionalTrueBlock20440
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
	bne elsedoneblock57509
ConditionalTrueBlock71361
	jsr CollisionPlayer
elseblock44861
elsedoneblock57509
	jsr UpdateCandy
	jsr CollisionCandy
	; Binary clause Simplified: LESSEQUAL
	lda lostchildlevel
	; Compare with pure num / var optimization
	cmp level;keep
	beq ConditionalTrueBlock29519
	bcs elsedoneblock66066
ConditionalTrueBlock29519
	jsr UpdateLostChild
elseblock18446
elsedoneblock66066
	; Binary clause Simplified: EQUALS
	lda witchHits
	; Compare with pure num / var optimization
	cmp #$6;keep
	bne elsedoneblock58825
ConditionalTrueBlock27241
	jsr BeatWitchWait
elseblock55366
elsedoneblock58825
	; Assigning single variable : i
	lda #14
	
	sta i
	; Binary clause Simplified: LESS
	lda moonPos
	; Compare with pure num / var optimization
	cmp #$6;keep
	bcs elsedoneblock35625
ConditionalTrueBlock24657
	; Assigning single variable : i
	lda #12
	
	sta i
elseblock38724
elsedoneblock35625
	; Binary clause Simplified: GREATER
	lda moonPos
	; Compare with pure num / var optimization
	cmp #$11;keep
	bcc elsedoneblock469
	beq elsedoneblock469
ConditionalTrueBlock22269
	; Assigning single variable : i
	lda #10
	
	sta i
elseblock99531
elsedoneblock469
	; Assigning memory location
	; Assigning single variable : $900f
	lda i
	sta $900f
while36989
	; Binary clause Simplified: EQUALS
	lda ticks
	; Compare with pure num / var optimization
	cmp oldTick;keep
	bne elsedoneblock81851
ConditionalTrueBlock83190
	jmp while36989
elseblock1665
elsedoneblock81851
	; Assigning single variable : oldTick
	lda ticks
	sta oldTick
	jmp while99815
elseblock17184
elsedoneblock12400
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
while94828
	; Binary clause Simplified: GREATER
	lda t
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock10268
	beq elsedoneblock10268
ConditionalTrueBlock87760
	; Assigning single variable : t
	dec t
	; Assigning single variable : i
	lda #0
	sta i
for80262
while67578
	; Binary clause Simplified: EQUALS
	lda ticks
	; Compare with pure num / var optimization
	cmp oldTick;keep
	bne elsedoneblock40579
ConditionalTrueBlock7681
	jmp while67578
elseblock63904
elsedoneblock40579
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
	beq elsedoneblock13094
ConditionalTrueBlock50707
	; Assigning single variable : t
	lda #0
	sta t
	; Assigning single variable : i
	lda #29
	sta i
elseblock28311
elsedoneblock13094
	inc i
	lda #30
	cmp i ;keep
	bne for80262
forLoopDone42789
	jmp while94828
elseblock10085
elsedoneblock10268
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
while53914
	; Binary clause Simplified: GREATER
	lda t
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock5279
	beq elsedoneblock5279
ConditionalTrueBlock47329
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
	bne elseblock61679
ConditionalTrueBlock69532
	jsr DrawTitleFrame1
	jmp elsedoneblock88723
elseblock61679
	jsr DrawTitleFrame2
elsedoneblock88723
	; Assigning single variable : t
	dec t
	; Assigning single variable : i
	lda #0
	sta i
for41497
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
	beq elsedoneblock64442
ConditionalTrueBlock54525
	; Assigning single variable : t
	lda #0
	sta t
	; Assigning single variable : i
	lda #29
	sta i
elseblock72463
elsedoneblock64442
	inc i
	lda #30
	cmp i ;keep
	bne for41497
forLoopDone76737
	jmp while53914
elseblock79703
elsedoneblock5279
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
	bcc dtnooverflow18245
	iny  ; overflow into high byte
dtnooverflow18245
	
	sta screenmemory
	sty screenmemory+1
	; ----------
	; BcdPrint address, number
	ldy #$07 ; screen offset
	ldx #0 ; score byte index
bcdprintloop61366
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
	bpl bcdprintloop61366
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
	bcs elseblock76169
ConditionalTrueBlock88777
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
printdecimal27092
	jsr init_printdecimal_div10 
	ora #$30
	sta (screenmemory),y
	dey
	bpl printdecimal27092
	jmp elsedoneblock15115
elseblock76169
printstring_call72612
	clc
	lda #<printstring_text17140
	adc #0
	ldy #>printstring_text17140
	sta print_text+0
	sty print_text+1
	ldx #13 ; optimized, look out for bugs
	jsr printstring
elsedoneblock15115
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
for44365
	; Assigning single variable : screenmemory
	lda #39
	ldy i
	sta (screenmemory),y
	inc i
	lda n
	cmp i ;keep
	bne for44365
forLoopDone59131
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
while67715
	; Binary clause: GREATER
	lda t
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bcc binaryclausefailed60053
	beq binaryclausefailed60053
binaryclausesuccess7526
	lda #1; success
	jmp binaryclausefinished63907
binaryclausefailed60053
	lda #0 ; failed state
binaryclausefinished63907
	cmp #1
	beq ConditionalTrueBlock73533
	jmp elsedoneblock272
ConditionalTrueBlock73533
	; Assigning single variable : t
	dec t
	; Binary clause: LESS
	lda x
	; Compare with pure num / var optimization
	cmp #$15;keep
	; BC done
	bcs binaryclausefailed74069
binaryclausesuccess22951
	lda #1; success
	jmp binaryclausefinished89791
binaryclausefailed74069
	lda #0 ; failed state
binaryclausefinished89791
	cmp #1
	beq ConditionalTrueBlock6847
	jmp elsedoneblock47287
ConditionalTrueBlock6847
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
	bcs elsedoneblock36868
ConditionalTrueBlock69004
	; Assigning single variable : y
	inc y
elseblock27597
elsedoneblock36868
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
	bne elsedoneblock57460
ConditionalTrueBlock95389
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
elseblock4949
elsedoneblock57460
	; Binary clause Simplified: EQUALS
	lda x
	; Compare with pure num / var optimization
	cmp #$15;keep
	bne elsedoneblock19419
ConditionalTrueBlock55112
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
elseblock61727
elsedoneblock19419
elseblock78673
elsedoneblock47287
	; Binary clause: EQUALS
	lda x
	; Compare with pure num / var optimization
	cmp #$15;keep
	; BC done
	bne binaryclausefailed48415
binaryclausesuccess89888
	lda #1; success
	jmp binaryclausefinished77583
binaryclausefailed48415
	lda #0 ; failed state
binaryclausefinished77583
	cmp #1
	beq ConditionalTrueBlock95508
	jmp elsedoneblock15260
ConditionalTrueBlock95508
	; Binary clause: LESS
	lda n
	; Compare with pure num / var optimization
	cmp #$58;keep
	; BC done
	bcs binaryclausefailed71054
binaryclausesuccess72763
	lda #1; success
	jmp binaryclausefinished62189
binaryclausefailed71054
	lda #0 ; failed state
binaryclausefinished62189
	cmp #1
	beq ConditionalTrueBlock54739
	jmp elsedoneblock57022
ConditionalTrueBlock54739
	; Assigning single variable : pan
	inc pan
	; Binary clause Simplified: GREATER
	lda pan
	; Compare with pure num / var optimization
	cmp #$5;keep
	bcc elsedoneblock5510
	beq elsedoneblock5510
ConditionalTrueBlock86358
	; Assigning single variable : pan
	lda #0
	sta pan
elseblock23856
elsedoneblock5510
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
	bne elsedoneblock72483
ConditionalTrueBlock96467
	; Assigning single variable : s
	lda #61
	sta s
elseblock93946
elsedoneblock72483
	; Binary clause Simplified: EQUALS
	lda n
	; Compare with pure num / var optimization
	cmp #$58;keep
	bne elsedoneblock73515
ConditionalTrueBlock85907
	; Assigning single variable : screenmemory
	lda #0
	pha
	lda n
	lsr
	lsr
	
	tay
	pla
	sta (screenmemory),y
elseblock6138
elsedoneblock73515
	; Binary clause Simplified: EQUALS
	; Modulo
	lda #4
val_var67344 = $54
	sta val_var67344
	lda n
	sec
modulo99634
	sbc val_var67344
	bcs modulo99634
	adc val_var67344
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock67764
ConditionalTrueBlock18916
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
	bne elseblock68735
ConditionalTrueBlock21496
	; Assigning single variable : stepsnd
	lda #30
	sta stepsnd
	jmp elsedoneblock12660
elseblock68735
	; Assigning single variable : stepsnd
	lda #0
	sta stepsnd
elsedoneblock12660
elseblock88883
elsedoneblock67764
elseblock39282
elsedoneblock57022
elseblock26865
elsedoneblock15260
	; Assigning single variable : i
	lda #0
	sta i
for83296
while70387
	; Binary clause Simplified: EQUALS
	lda ticks
	; Compare with pure num / var optimization
	cmp oldTick;keep
	bne elsedoneblock53683
ConditionalTrueBlock35742
	jmp while70387
elseblock84537
elsedoneblock53683
	; Assigning single variable : oldTick
	lda ticks
	sta oldTick
	jsr callReadJoy1
	; Binary clause Simplified: LESS
	lda t
	; Compare with pure num / var optimization
	cmp #$3c;keep
	bcs elsedoneblock49563
ConditionalTrueBlock84829
	; Binary clause Simplified: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1
	and #16
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock23315
ConditionalTrueBlock27022
	; Assigning single variable : t
	lda #0
	sta t
elseblock68717
elsedoneblock23315
elseblock14641
elsedoneblock49563
	inc i
	lda #4
	cmp i ;keep
	bne for83296
forLoopDone37835
	jmp while67715
elseblock51186
elsedoneblock272
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
val_var29205 = $54
	sta val_var29205
	; 8 bit binop
	; Add/sub where right value is constant number
	lda ticks
	clc
	adc #1
	 ; end add / sub var with constant
	sec
modulo58826
	sbc val_var29205
	bcs modulo58826
	adc val_var29205
	
	sta ticks
	; Binary clause Simplified: EQUALS
	lda sndFlag
	; Compare with pure num / var optimization
	cmp #$1;keep
	bne elsedoneblock87191
ConditionalTrueBlock64947
	jsr UpdateSound
elseblock43364
elsedoneblock87191
	; CloseIRQ
	pla
	tay
	pla
	tax
	pla
	 jmp $eabf     ; return to normal IRQ	
	rti
block89383
	jsr Initialise
	jsr SetupSound
	; Assigning single variable : level
	lda #0
	sta level
while2056
	; Binary clause: NOTEQUALS
	lda #1
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	beq binaryclausefailed82164
binaryclausesuccess14592
	lda #1; success
	jmp binaryclausefinished13373
binaryclausefailed82164
	lda #0 ; failed state
binaryclausefinished13373
	cmp #1
	beq ConditionalTrueBlock33461
	jmp elsedoneblock67971
ConditionalTrueBlock33461
	jsr BlankScreen
	lda level
	cmp #$0 ;keep
	bne casenext65220
	jsr TitleCredits
	jmp caseend84545
casenext65220
	lda level
	cmp #$1 ;keep
	bne casenext941
	jsr TitleScreen
	jmp caseend84545
casenext941
	lda level
	cmp #$2 ;keep
	bne casenext65898
	jsr LastScore
	jmp caseend84545
casenext65898
	lda level
	cmp #$3 ;keep
	bne casenext10192
	jsr TitleStory
	jmp caseend84545
casenext10192
	lda level
	cmp #$4 ;keep
	bne casenext51948
	jsr TitleStory2
	jmp caseend84545
casenext51948
	lda level
	cmp #$5 ;keep
	bne casenext44529
	jsr TitleCast
	jmp caseend84545
casenext44529
	lda level
	cmp #$6 ;keep
	bne casenext13271
	jsr TitleHowTo
	jmp caseend84545
casenext13271
caseend84545
	; Binary clause: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1
	and #16
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	beq binaryclausefailed54180
binaryclausesuccess62701
	lda #1; success
	jmp binaryclausefinished79851
binaryclausefailed54180
	lda #0 ; failed state
binaryclausefinished79851
	cmp #1
	beq ConditionalTrueBlock76803
	jmp elsedoneblock15741
ConditionalTrueBlock76803
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
val_var33998 = $54
	sta val_var33998
	jsr Random
	sec
modulo77981
	sbc val_var33998
	bcs modulo77981
	adc val_var33998
	
	clc
	adc #1
	 ; end add / sub var with constant
	
	sta lostchildlevel
while87453
	; Binary clause: GREATER
	lda lives
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bcc binaryclausefailed45979
	beq binaryclausefailed45979
binaryclausesuccess77956
	lda #1; success
	jmp binaryclausefinished15147
binaryclausefailed45979
	lda #0 ; failed state
binaryclausefinished15147
	cmp #1
	beq ConditionalTrueBlock64098
	jmp elsedoneblock91486
ConditionalTrueBlock64098
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
	bne elsedoneblock7767
ConditionalTrueBlock68632
	; Assigning single variable : lives
	dec lives
	; Binary clause Simplified: EQUALS
	lda lives
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elseblock59543
ConditionalTrueBlock60054
	jsr TitleGameOver
	jmp elsedoneblock7750
elseblock59543
	jsr TitleFail
elsedoneblock7750
elseblock37257
elsedoneblock7767
	; Binary clause Simplified: EQUALS
	lda gameMode
	; Compare with pure num / var optimization
	cmp #$2;keep
	bne elsedoneblock92036
ConditionalTrueBlock55423
	; Assigning single variable : level
	inc level
	; Binary clause Simplified: EQUALS
	lda level
	; Compare with pure num / var optimization
	cmp #$d;keep
	bne elseblock95632
ConditionalTrueBlock83539
	jsr TitleGameWon
	; Assigning single variable : lives
	lda #0
	sta lives
	jmp elsedoneblock62380
elseblock95632
	jsr TitleGameNext
	; Assigning single variable : witchHits
	lda #0
	sta witchHits
	; Binary clause Simplified: LESS
	lda lives
	; Compare with pure num / var optimization
	cmp #$5;keep
	bcs elsedoneblock71392
ConditionalTrueBlock15648
	; Binary clause Simplified: EQUALS
	lda level
	; Compare with pure num / var optimization
	cmp #$2;keep
	bne elsedoneblock22246
ConditionalTrueBlock52074
	; Assigning single variable : lives
	inc lives
elseblock64326
elsedoneblock22246
	; Binary clause Simplified: EQUALS
	lda level
	; Compare with pure num / var optimization
	cmp #$5;keep
	bne elsedoneblock38558
ConditionalTrueBlock96618
	; Assigning single variable : lives
	inc lives
elseblock4586
elsedoneblock38558
	; Binary clause Simplified: EQUALS
	lda level
	; Compare with pure num / var optimization
	cmp #$9;keep
	bne elsedoneblock95160
ConditionalTrueBlock33177
	; Assigning single variable : lives
	inc lives
elseblock38783
elsedoneblock95160
	; Binary clause Simplified: EQUALS
	lda level
	; Compare with pure num / var optimization
	cmp #$b;keep
	bne elsedoneblock79842
ConditionalTrueBlock6912
	; Assigning single variable : lives
	inc lives
elseblock10236
elsedoneblock79842
elseblock51108
elsedoneblock71392
elsedoneblock62380
elseblock79988
elsedoneblock92036
	jmp while87453
elseblock84447
elsedoneblock91486
	; Assigning single variable : level
	lda #0
	sta level
elseblock26965
elsedoneblock15741
	; Assigning single variable : level
	inc level
	; Binary clause Simplified: GREATER
	lda level
	; Compare with pure num / var optimization
	cmp #$6;keep
	bcc elsedoneblock68681
	beq elsedoneblock68681
ConditionalTrueBlock46552
	; Assigning single variable : level
	lda #0
	sta level
elseblock4242
elsedoneblock68681
	jmp while2056
elseblock70878
elsedoneblock67971
EndBlock43
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
