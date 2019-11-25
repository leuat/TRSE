 processor 6502
	ORG $1201
	.byte    $0E, $08, $0A, $00, $9E, $20, $28
	.byte   $34,$36,$32,$34
	.byte    $29, $00, $00, $00
	ORG $1210
Pumpkid
	jmp block73591
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
printstring_text51678	dc.b	"@OUT@OF@TIME@@"
	dc.b	0
printstring_text76701	dc.b	"@GOT@BY@WITCH@"
	dc.b	0
printstring_text48002	dc.b	"POISONED@SKULL"
	dc.b	0
printstring_text97521	dc.b	"@GOT@BY@GHOST@"
	dc.b	0
printstring_text2122	dc.b	"@GOT@BY@ZOMBIE@"
	dc.b	0
printstring_text69111	dc.b	"THE@WITCH@RETREATS"
	dc.b	0
printstring_text29886	dc.b	"GET@READY@FOR"
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
EndBlock10400
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
	BMI JoySkip56002
	ORA #$02
JoySkip56002
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
	bcc RandomSkip89009
	eor #$4d
RandomSkip89009
	sta Random+1
	eor $9124
	rts
	
	
	; ***********  Defining procedure : PlaySnd1
	;    Procedure type : User-defined procedure
	
vsnd1Time	dc.b	$00
vsnd1	dc.b	
block99348
PlaySnd1
	; Assigning single variable : vsnd1Time
	lda #0
	sta vsnd1Time
	rts
	
	
	; ***********  Defining procedure : PlaySnd2
	;    Procedure type : User-defined procedure
	
vsnd2Time	dc.b	$00
vsnd2	dc.b	
block98338
PlaySnd2
	; Assigning single variable : vsnd2Time
	lda #0
	sta vsnd2Time
	rts
	
	
	; ***********  Defining procedure : PlaySnd3
	;    Procedure type : User-defined procedure
	
vsnd3Time	dc.b	$00
vsnd3	dc.b	
block49978
PlaySnd3
	; Assigning single variable : vsnd3Time
	lda #0
	sta vsnd3Time
	rts
	
	
	; ***********  Defining procedure : PlaySndfx
	;    Procedure type : User-defined procedure
	
vsndfxTime	dc.b	$00
vsndfx	dc.b	
block27375
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
block60576
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
	bcc elsedoneblock79192
	beq elsedoneblock79192
ConditionalTrueBlock18111
	; Assigning single variable : vsnd1Time
	dec vsnd1Time
elseblock75205
elsedoneblock79192
	; Binary clause Simplified: EQUALS
	lda vsnd1Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock81870
ConditionalTrueBlock79641
	; Assigning single variable : psnd1
	; WORD optimization: a=a+b
	lda psnd1+0
	
	clc
	adc #2
	bcc WordAdd34281
	inc psnd1+1
WordAdd34281
	sta psnd1+0
elseblock51792
elsedoneblock81870
	rts
	
	
	; ***********  Defining procedure : Snd2Time
	;    Procedure type : User-defined procedure
	
Snd2Time
	; Binary clause Simplified: GREATER
	lda vsnd2Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock97325
	beq elsedoneblock97325
ConditionalTrueBlock44920
	; Assigning single variable : vsnd2Time
	dec vsnd2Time
elseblock71272
elsedoneblock97325
	; Binary clause Simplified: EQUALS
	lda vsnd2Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock2968
ConditionalTrueBlock57119
	; Assigning single variable : psnd2
	; WORD optimization: a=a+b
	lda psnd2+0
	
	clc
	adc #2
	bcc WordAdd32386
	inc psnd2+1
WordAdd32386
	sta psnd2+0
elseblock67788
elsedoneblock2968
	rts
	
	
	; ***********  Defining procedure : Snd3Time
	;    Procedure type : User-defined procedure
	
Snd3Time
	; Binary clause Simplified: GREATER
	lda vsnd3Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock20876
	beq elsedoneblock20876
ConditionalTrueBlock92576
	; Assigning single variable : vsnd3Time
	dec vsnd3Time
elseblock91279
elsedoneblock20876
	; Binary clause Simplified: EQUALS
	lda vsnd3Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock62645
ConditionalTrueBlock4252
	; Assigning single variable : psnd3
	; WORD optimization: a=a+b
	lda psnd3+0
	
	clc
	adc #2
	bcc WordAdd94197
	inc psnd3+1
WordAdd94197
	sta psnd3+0
elseblock60890
elsedoneblock62645
	rts
	
	
	; ***********  Defining procedure : SndfxTime
	;    Procedure type : User-defined procedure
	
SndfxTime
	; Binary clause Simplified: GREATER
	lda vsndfxTime
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock23139
	beq elsedoneblock23139
ConditionalTrueBlock23607
	; Assigning single variable : vsndfxTime
	dec vsndfxTime
elseblock77620
elsedoneblock23139
	; Binary clause Simplified: EQUALS
	lda vsndfxTime
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock86057
ConditionalTrueBlock24553
	; Assigning single variable : psndfx
	; WORD optimization: a=a+b
	lda psndfx+0
	
	clc
	adc #2
	bcc WordAdd74217
	inc psndfx+1
WordAdd74217
	sta psndfx+0
elseblock35856
elsedoneblock86057
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
	bne elsedoneblock17328
ConditionalTrueBlock19887
	; Binary clause Simplified: NOTEQUALS
	lda vsnd1
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock90584
ConditionalTrueBlock95591
	; Assigning single variable : vsnd1
	lda #0
	sta vsnd1
	; Poke
	; Optimization: shift is zero
	sta $900a
elseblock70728
elsedoneblock90584
	rts
elseblock48980
elsedoneblock17328
	; Binary clause: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd1),y
	
	; Compare with pure num / var optimization
	cmp #$20;keep
	; BC done
	bne tempfail83179
binaryclausesuccess41046
	jmp ConditionalTrueBlock86949
tempfail83179
	; Binary clause: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd1),y
	
	; Compare with pure num / var optimization
	cmp #$10;keep
	; BC done
	bne elseblock31620
binaryclausesuccess18198
ConditionalTrueBlock86949
	; Binary clause Simplified: EQUALS
	lda vsnd1Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock56111
ConditionalTrueBlock25166
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
	bne elsedoneblock53328
ConditionalTrueBlock86810
	; Poke
	; Optimization: shift is zero
	lda vsnd1
	sta $900a
elseblock45857
elsedoneblock53328
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd1),y
	
	; Compare with pure num / var optimization
	cmp #$10;keep
	bne elsedoneblock36270
ConditionalTrueBlock64301
	; Poke
	; Optimization: shift is zero
	lda #0
	sta $900a
elseblock60580
elsedoneblock36270
elseblock43786
elsedoneblock56111
	jsr Snd1Time
elseblock31620
elsedoneblock63293
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
	bne elsedoneblock60622
ConditionalTrueBlock57367
	; Binary clause Simplified: EQUALS
	lda vsnd1Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock75293
ConditionalTrueBlock15038
	; Assigning single variable : vsnd1Time
	; Load pointer array
	ldy #$1
	lda (psnd1),y
	
	sta vsnd1Time
elseblock46597
elsedoneblock75293
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
elseblock20297
elsedoneblock60622
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
	bne elsedoneblock85705
ConditionalTrueBlock3147
	; Binary clause Simplified: EQUALS
	lda vsnd1Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock49075
ConditionalTrueBlock3458
	; Assigning single variable : vsnd1Time
	; Load pointer array
	ldy #$1
	lda (psnd1),y
	
	sta vsnd1Time
elseblock36846
elsedoneblock49075
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
	
rightvarAddSub_var98387 = $54
	sta rightvarAddSub_var98387
	lda vsnd1
	sec
	sbc rightvarAddSub_var98387
	
	sta vsnd1
	; Poke
	; Optimization: shift is zero
	sta $900a
	jsr Snd1Time
elseblock45558
elsedoneblock85705
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
	bne elsedoneblock77234
ConditionalTrueBlock2042
	; Binary clause Simplified: NOTEQUALS
	lda vsnd2
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock54818
ConditionalTrueBlock593
	; Assigning single variable : vsnd2
	lda #0
	sta vsnd2
	; Poke
	; Optimization: shift is zero
	sta $900b
elseblock82260
elsedoneblock54818
	rts
elseblock79176
elsedoneblock77234
	; Binary clause: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd2),y
	
	; Compare with pure num / var optimization
	cmp #$20;keep
	; BC done
	bne tempfail8300
binaryclausesuccess91449
	jmp ConditionalTrueBlock99201
tempfail8300
	; Binary clause: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd2),y
	
	; Compare with pure num / var optimization
	cmp #$10;keep
	; BC done
	bne elseblock21929
binaryclausesuccess96019
ConditionalTrueBlock99201
	; Binary clause Simplified: EQUALS
	lda vsnd2Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock30818
ConditionalTrueBlock86569
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
	bne elsedoneblock56419
ConditionalTrueBlock62769
	; Poke
	; Optimization: shift is zero
	lda vsnd2
	sta $900b
elseblock74748
elsedoneblock56419
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd2),y
	
	; Compare with pure num / var optimization
	cmp #$10;keep
	bne elsedoneblock27616
ConditionalTrueBlock63620
	; Poke
	; Optimization: shift is zero
	lda #0
	sta $900b
elseblock72080
elsedoneblock27616
elseblock71840
elsedoneblock30818
	jsr Snd2Time
elseblock21929
elsedoneblock91890
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
	bne elsedoneblock52247
ConditionalTrueBlock77989
	; Binary clause Simplified: EQUALS
	lda vsnd2Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock22356
ConditionalTrueBlock58908
	; Assigning single variable : vsnd2Time
	; Load pointer array
	ldy #$1
	lda (psnd2),y
	
	sta vsnd2Time
elseblock61583
elsedoneblock22356
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
elseblock87373
elsedoneblock52247
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
	bne elsedoneblock33010
ConditionalTrueBlock3028
	; Binary clause Simplified: EQUALS
	lda vsnd2Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock72237
ConditionalTrueBlock96980
	; Assigning single variable : vsnd2Time
	; Load pointer array
	ldy #$1
	lda (psnd2),y
	
	sta vsnd2Time
elseblock61491
elsedoneblock72237
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
	
rightvarAddSub_var43606 = $54
	sta rightvarAddSub_var43606
	lda vsnd2
	sec
	sbc rightvarAddSub_var43606
	
	sta vsnd2
	; Poke
	; Optimization: shift is zero
	sta $900b
	jsr Snd2Time
elseblock10125
elsedoneblock33010
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
	bne elsedoneblock39196
ConditionalTrueBlock42890
	; Binary clause Simplified: NOTEQUALS
	lda vsnd3
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock56409
ConditionalTrueBlock64205
	; Assigning single variable : vsnd3
	lda #0
	sta vsnd3
	; Poke
	; Optimization: shift is zero
	sta $900c
elseblock57250
elsedoneblock56409
	rts
elseblock68694
elsedoneblock39196
	; Binary clause: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd3),y
	
	; Compare with pure num / var optimization
	cmp #$20;keep
	; BC done
	bne tempfail70662
binaryclausesuccess38704
	jmp ConditionalTrueBlock55104
tempfail70662
	; Binary clause: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd3),y
	
	; Compare with pure num / var optimization
	cmp #$10;keep
	; BC done
	bne elseblock25152
binaryclausesuccess55521
ConditionalTrueBlock55104
	; Binary clause Simplified: EQUALS
	lda vsnd3Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock10189
ConditionalTrueBlock49212
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
	bne elsedoneblock97219
ConditionalTrueBlock47906
	; Poke
	; Optimization: shift is zero
	lda vsnd3
	sta $900c
elseblock89052
elsedoneblock97219
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd3),y
	
	; Compare with pure num / var optimization
	cmp #$10;keep
	bne elsedoneblock40478
ConditionalTrueBlock76360
	; Poke
	; Optimization: shift is zero
	lda #0
	sta $900c
elseblock87486
elsedoneblock40478
elseblock47497
elsedoneblock10189
	jsr Snd3Time
elseblock25152
elsedoneblock20817
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
	bne elsedoneblock32024
ConditionalTrueBlock48665
	; Binary clause Simplified: EQUALS
	lda vsnd3Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock52816
ConditionalTrueBlock44840
	; Assigning single variable : vsnd3Time
	; Load pointer array
	ldy #$1
	lda (psnd3),y
	
	sta vsnd3Time
elseblock39142
elsedoneblock52816
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
elseblock8571
elsedoneblock32024
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
	bne elsedoneblock95780
ConditionalTrueBlock4356
	; Binary clause Simplified: EQUALS
	lda vsnd3Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock9436
ConditionalTrueBlock98838
	; Assigning single variable : vsnd3Time
	; Load pointer array
	ldy #$1
	lda (psnd3),y
	
	sta vsnd3Time
elseblock33247
elsedoneblock9436
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
	
rightvarAddSub_var93805 = $54
	sta rightvarAddSub_var93805
	lda vsnd3
	sec
	sbc rightvarAddSub_var93805
	
	sta vsnd3
	; Poke
	; Optimization: shift is zero
	sta $900c
	jsr Snd3Time
elseblock81324
elsedoneblock95780
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
	bne elsedoneblock26099
ConditionalTrueBlock62968
	; Binary clause Simplified: NOTEQUALS
	lda vsndfx
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock83356
ConditionalTrueBlock15824
	; Assigning single variable : vsndfx
	lda #0
	sta vsndfx
	; Poke
	; Optimization: shift is zero
	sta $900d
elseblock61964
elsedoneblock83356
	rts
elseblock20318
elsedoneblock26099
	; Binary clause: EQUALS
	; Load pointer array
	ldy #$0
	lda (psndfx),y
	
	; Compare with pure num / var optimization
	cmp #$20;keep
	; BC done
	bne tempfail64127
binaryclausesuccess28397
	jmp ConditionalTrueBlock42618
tempfail64127
	; Binary clause: EQUALS
	; Load pointer array
	ldy #$0
	lda (psndfx),y
	
	; Compare with pure num / var optimization
	cmp #$10;keep
	; BC done
	bne elseblock74551
binaryclausesuccess97358
ConditionalTrueBlock42618
	; Binary clause Simplified: EQUALS
	lda vsndfxTime
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock95778
ConditionalTrueBlock68913
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
	bne elsedoneblock80817
ConditionalTrueBlock17257
	; Poke
	; Optimization: shift is zero
	lda vsndfx
	sta $900d
elseblock91627
elsedoneblock80817
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #$0
	lda (psndfx),y
	
	; Compare with pure num / var optimization
	cmp #$10;keep
	bne elsedoneblock41861
ConditionalTrueBlock94569
	; Poke
	; Optimization: shift is zero
	lda #0
	sta $900d
elseblock27935
elsedoneblock41861
elseblock45757
elsedoneblock95778
	jsr SndfxTime
elseblock74551
elsedoneblock96912
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
	bne elsedoneblock51218
ConditionalTrueBlock66527
	; Binary clause Simplified: EQUALS
	lda vsndfxTime
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock52638
ConditionalTrueBlock24007
	; Assigning single variable : vsndfxTime
	; Load pointer array
	ldy #$1
	lda (psndfx),y
	
	sta vsndfxTime
elseblock36204
elsedoneblock52638
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
elseblock26911
elsedoneblock51218
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
	bne elsedoneblock59150
ConditionalTrueBlock39641
	; Binary clause Simplified: EQUALS
	lda vsndfxTime
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock65168
ConditionalTrueBlock49041
	; Assigning single variable : vsndfxTime
	; Load pointer array
	ldy #$1
	lda (psndfx),y
	
	sta vsndfxTime
elseblock80322
elsedoneblock65168
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
	
rightvarAddSub_var13674 = $54
	sta rightvarAddSub_var13674
	lda vsndfx
	sec
	sbc rightvarAddSub_var13674
	
	sta vsndfx
	; Poke
	; Optimization: shift is zero
	sta $900d
	jsr SndfxTime
elseblock65560
elsedoneblock59150
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
block21239
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
	bne elsedoneblock65236
ConditionalTrueBlock22258
	rts
elseblock41689
elsedoneblock65236
	; Binary clause Simplified: EQUALS
	lda musicStep
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock99436
ConditionalTrueBlock6765
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
	bne elsedoneblock83717
ConditionalTrueBlock66847
	; Assigning single variable : musicNote
	lda #0
	sta musicNote
	; Assigning single variable : musicRepeat
	inc musicRepeat
	; Binary clause Simplified: EQUALS
	lda musicRepeat
	; Compare with pure num / var optimization
	cmp #$1;keep
	bne elsedoneblock4106
ConditionalTrueBlock85959
	; Poke
	; Optimization: shift is zero
	lda #8
	sta $900e
elseblock10962
elsedoneblock4106
	; Binary clause Simplified: EQUALS
	lda musicRepeat
	; Compare with pure num / var optimization
	cmp #$2;keep
	bne elsedoneblock15118
ConditionalTrueBlock32000
	; Poke
	; Optimization: shift is zero
	lda #5
	sta $900e
elseblock3233
elsedoneblock15118
	; Binary clause Simplified: EQUALS
	lda musicRepeat
	; Compare with pure num / var optimization
	cmp #$3;keep
	bne elsedoneblock82596
ConditionalTrueBlock47085
	; Poke
	; Optimization: shift is zero
	lda #2
	sta $900e
elseblock92841
elsedoneblock82596
elseblock80935
elsedoneblock83717
elseblock61851
elsedoneblock99436
	; Assigning single variable : musicStep
	inc musicStep
	; Binary clause Simplified: EQUALS
	lda musicStep
	; Compare with pure num / var optimization
	cmp #$13;keep
	bne elsedoneblock33268
ConditionalTrueBlock28953
	; Assigning single variable : psnd3
	lda #<noSound
	ldx #>noSound
	sta psnd3
	stx psnd3+1
elseblock4846
elsedoneblock33268
	; Binary clause Simplified: EQUALS
	lda musicStep
	; Compare with pure num / var optimization
	cmp #$14;keep
	bne elsedoneblock29958
ConditionalTrueBlock82335
	; Assigning single variable : musicStep
	lda #0
	sta musicStep
elseblock1293
elsedoneblock29958
	rts
	
	
	; ***********  Defining procedure : UpdateLostChild
	;    Procedure type : User-defined procedure
	
UpdateLostChild
	; Binary clause: EQUALS
	lda moonPos
	; Compare with pure num / var optimization
	cmp #$8;keep
	; BC done
	bne binaryclausefailed91483
binaryclausesuccess42741
	lda #1; success
	jmp binaryclausefinished16380
binaryclausefailed91483
	lda #0 ; failed state
binaryclausefinished16380
	cmp #1
	beq ConditionalTrueBlock3544
	jmp elsedoneblock46129
ConditionalTrueBlock3544
	; Binary clause: EQUALS
	lda lx
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bne binaryclausefailed66374
binaryclausesuccess23787
	lda #1; success
	jmp binaryclausefinished95428
binaryclausefailed66374
	lda #0 ; failed state
binaryclausefinished95428
	cmp #1
	beq ConditionalTrueBlock61557
	jmp elsedoneblock8946
ConditionalTrueBlock61557
	; Binary clause: LESS
	lda level
	; Compare with pure num / var optimization
	cmp #$c;keep
	; BC done
	bcs binaryclausefailed4364
binaryclausesuccess44508
	lda #1; success
	jmp binaryclausefinished78734
binaryclausefailed4364
	lda #0 ; failed state
binaryclausefinished78734
	cmp #1
	beq ConditionalTrueBlock14755
	jmp elsedoneblock98154
ConditionalTrueBlock14755
	; Assigning single variable : i
	lda #0
	sta i
while15501
	; Binary clause: EQUALS
	lda i
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bne binaryclausefailed52352
binaryclausesuccess10207
	lda #1; success
	jmp binaryclausefinished16293
binaryclausefailed52352
	lda #0 ; failed state
binaryclausefinished16293
	cmp #1
	beq ConditionalTrueBlock92616
	jmp elsedoneblock4938
ConditionalTrueBlock92616
	; Assigning single variable : lx
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #19
val_var38578 = $54
	sta val_var38578
	jsr Random
	sec
modulo90808
	sbc val_var38578
	bcs modulo90808
	adc val_var38578
	
	clc
	adc #1
	 ; end add / sub var with constant
	
	sta lx
	; Assigning single variable : ly
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #18
val_var49658 = $54
	sta val_var49658
	jsr Random
	sec
modulo46839
	sbc val_var49658
	bcs modulo46839
	adc val_var49658
	
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
	bne elsedoneblock42790
ConditionalTrueBlock41925
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
elseblock64614
elsedoneblock42790
	jmp while15501
elseblock74771
elsedoneblock4938
elseblock10110
elsedoneblock98154
elseblock135
elsedoneblock8946
elseblock19065
elsedoneblock46129
	; Binary clause Simplified: EQUALS
	lda moonPos
	; Compare with pure num / var optimization
	cmp #$a;keep
	bne elsedoneblock79222
ConditionalTrueBlock50209
	; Binary clause Simplified: NOTEQUALS
	lda lx
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock27730
ConditionalTrueBlock87153
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
val_var61807 = $54
	sta val_var61807
	jsr Random
	sec
modulo38765
	sbc val_var61807
	bcs modulo38765
	adc val_var61807
	
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
elseblock91687
elsedoneblock27730
elseblock36560
elsedoneblock79222
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
	bne elseblock91413
ConditionalTrueBlock5326
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
	jmp elsedoneblock67570
elseblock91413
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
elsedoneblock67570
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
dtloop81740
	tay
	lda scr,x
	inx
	inx
	clc
	adc #$16
	bcc dtnooverflow77644
	iny
dtnooverflow77644
	sta scr,x
	tya
	sta scr+1,x
	cpx #$2c
	bcc dtloop81740
	; ----------
	; DefineAddressTable address, StartValue, IncrementValue, TableSize
	ldy #>$9400
	lda #<$9400
	ldx #0
	sta clr,x   ; Address of table
	tya
	sta clr+1,x
dtloop25163
	tay
	lda clr,x
	inx
	inx
	clc
	adc #$16
	bcc dtnooverflow72554
	iny
dtnooverflow72554
	sta clr,x
	tya
	sta clr+1,x
	cpx #$2c
	bcc dtloop25163
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
bcdprintloop34830
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
	bpl bcdprintloop34830
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
for20275
	; Assigning single variable : j
	lda #0
	sta j
for88123
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
	bne for88123
forLoopDone53609
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd48536
	inc p1+1
WordAdd48536
	sta p1+0
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd8726
	inc screenmemory+1
WordAdd8726
	sta screenmemory+0
	inc i
	lda #23
	cmp i ;keep
	bne for20275
forLoopDone98505
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
for36884
	; Assigning single variable : j
	lda #0
	sta j
for82527
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
	bne for82527
forLoopDone3939
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd5705
	inc p1+1
WordAdd5705
	sta p1+0
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd81082
	inc screenmemory+1
WordAdd81082
	sta screenmemory+0
	inc i
	lda #23
	cmp i ;keep
	bne for36884
forLoopDone62646
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
for84361
	; Assigning single variable : j
	lda #0
	sta j
for77263
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
	bne for77263
forLoopDone82149
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd7215
	inc p1+1
WordAdd7215
	sta p1+0
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd12554
	inc screenmemory+1
WordAdd12554
	sta screenmemory+0
	inc i
	lda #23
	cmp i ;keep
	bne for84361
forLoopDone5442
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
for11669
	; Assigning single variable : j
	lda #0
	sta j
for7211
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
	bne for7211
forLoopDone42448
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd30049
	inc p1+1
WordAdd30049
	sta p1+0
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd97916
	inc screenmemory+1
WordAdd97916
	sta screenmemory+0
	inc i
	lda #23
	cmp i ;keep
	bne for11669
forLoopDone7278
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
for84754
	; Assigning single variable : j
	lda #0
	sta j
for74846
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
	bne for74846
forLoopDone5397
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd86103
	inc p1+1
WordAdd86103
	sta p1+0
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd46184
	inc screenmemory+1
WordAdd46184
	sta screenmemory+0
	inc i
	lda #23
	cmp i ;keep
	bne for84754
forLoopDone50867
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
for58070
	; Assigning single variable : j
	lda #0
	sta j
for22672
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
	bne for22672
forLoopDone17227
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd14218
	inc p1+1
WordAdd14218
	sta p1+0
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd89079
	inc screenmemory+1
WordAdd89079
	sta screenmemory+0
	inc i
	lda #23
	cmp i ;keep
	bne for58070
forLoopDone83063
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
for35138
	; Assigning single variable : j
	lda #0
	sta j
for9000
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
	bne for9000
forLoopDone57552
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd85919
	inc p1+1
WordAdd85919
	sta p1+0
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd70848
	inc screenmemory+1
WordAdd70848
	sta screenmemory+0
	inc i
	lda #23
	cmp i ;keep
	bne for35138
forLoopDone9041
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
for85064
	; Assigning single variable : j
	lda #0
	sta j
for14817
	; Assigning single variable : p1
	lda #0
	ldy j
	sta (p1),y
	inc j
	lda #22
	cmp j ;keep
	bne for14817
forLoopDone60522
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd8858
	inc p1+1
WordAdd8858
	sta p1+0
	inc i
	lda #23
	cmp i ;keep
	bne for85064
forLoopDone26602
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
for94182
	; Assigning single variable : j
	lda #0
	sta j
for64095
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
	bne for64095
forLoopDone81845
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd87231
	inc p1+1
WordAdd87231
	sta p1+0
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd29190
	inc screenmemory+1
WordAdd29190
	sta screenmemory+0
	inc i
	lda #20
	cmp i ;keep
	bne for94182
forLoopDone42479
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
for37366
	; Assigning single variable : screenmemory
	lda #3
	ldy i
	sta (screenmemory),y
	inc i
	lda #22
	cmp i ;keep
	bne for37366
forLoopDone13357
	; Assigning single variable : i
	lda #22
	sta i
for47101
	; Assigning single variable : screenmemory
	lda #0
	ldy i
	sta (screenmemory),y
	inc i
	lda #66
	cmp i ;keep
	bne for47101
forLoopDone84251
	; Assigning single variable : screenmemory
	lda #8
	ldx #148
	sta screenmemory
	stx screenmemory+1
	; Assigning single variable : i
	lda #0
	sta i
for25764
	; Assigning single variable : screenmemory
	lda #2
	ldy i
	sta (screenmemory),y
	inc i
	lda #8
	cmp i ;keep
	bne for25764
forLoopDone53927
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
for40194
	; Assigning single variable : x
	lda #1
	sta x
for30540
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy x
	lda (screenmemory),y
	
	; Compare with pure num / var optimization
	cmp #$3a;keep
	bne elsedoneblock97319
ConditionalTrueBlock49785
	; Assigning single variable : px
	lda x
	sta px
	; Assigning single variable : py
	lda y
	sta py
elseblock82850
elsedoneblock97319
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy x
	lda (screenmemory),y
	
	; Compare with pure num / var optimization
	cmp #$20;keep
	bne elsedoneblock90436
ConditionalTrueBlock12039
	; Assigning single variable : wx
	lda x
	sta wx
	; Assigning single variable : wy
	lda y
	sta wy
elseblock65969
elsedoneblock90436
	inc x
	lda #21
	cmp x ;keep
	bne for30540
forLoopDone34312
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd46249
	inc screenmemory+1
WordAdd46249
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
	bne for40194
forLoopDone22040
	rts
	
	
	; ***********  Defining procedure : WitchPickDirection
	;    Procedure type : User-defined procedure
	
WitchPickDirection
	; Binary clause Simplified: NOTEQUALS
	; Modulo
	lda #3
val_var72471 = $54
	sta val_var72471
	jsr Random
	sec
modulo8567
	sbc val_var72471
	bcs modulo8567
	adc val_var72471
	
	; Compare with pure num / var optimization
	cmp #$1;keep
	beq elsedoneblock17436
ConditionalTrueBlock76789
	; Binary clause Simplified: EQUALS
	lda wy
	; Compare with pure num / var optimization
	cmp py;keep
	bne elsedoneblock97317
ConditionalTrueBlock68172
	; Binary clause Simplified: GREATER
	lda wx
	; Compare with pure num / var optimization
	cmp px;keep
	bcc elseblock48017
	beq elseblock48017
ConditionalTrueBlock59935
	; Assigning single variable : wd
	lda #3
	sta wd
	jmp elsedoneblock80796
elseblock48017
	; Assigning single variable : wd
	lda #1
	sta wd
elsedoneblock80796
	rts
elseblock91032
elsedoneblock97317
	; Binary clause Simplified: EQUALS
	lda wx
	; Compare with pure num / var optimization
	cmp px;keep
	bne elsedoneblock77469
ConditionalTrueBlock441
	; Binary clause Simplified: GREATER
	lda wy
	; Compare with pure num / var optimization
	cmp py;keep
	bcc elseblock89643
	beq elseblock89643
ConditionalTrueBlock29398
	; Assigning single variable : wd
	lda #0
	sta wd
	jmp elsedoneblock47625
elseblock89643
	; Assigning single variable : wd
	lda #2
	sta wd
elsedoneblock47625
	rts
elseblock10834
elsedoneblock77469
elseblock6505
elsedoneblock17436
	; Assigning single variable : wd
	inc wd
	; Binary clause Simplified: GREATER
	lda wd
	; Compare with pure num / var optimization
	cmp #$3;keep
	bcc elsedoneblock37710
	beq elsedoneblock37710
ConditionalTrueBlock67745
	; Assigning single variable : wd
	lda #0
	sta wd
elseblock80713
elsedoneblock37710
	rts
	
	
	; ***********  Defining procedure : UpdateWitch
	;    Procedure type : User-defined procedure
	
UpdateWitch
	; Binary clause: EQUALS
	lda wsteps
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bne binaryclausefailed7687
binaryclausesuccess91297
	lda #1; success
	jmp binaryclausefinished25302
binaryclausefailed7687
	lda #0 ; failed state
binaryclausefinished25302
	cmp #1
	beq ConditionalTrueBlock82239
	jmp elsedoneblock47708
ConditionalTrueBlock82239
	; Binary clause Simplified: EQUALS
	lda wdelay
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock89900
ConditionalTrueBlock68436
	; Assigning single variable : wdelay
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #40
val_var66942 = $54
	sta val_var66942
	jsr Random
	sec
modulo50209
	sbc val_var66942
	bcs modulo50209
	adc val_var66942
	
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
elseblock75927
elsedoneblock89900
	; Assigning single variable : wdelay
	dec wdelay
	; Binary clause Simplified: GREATER
	lda wdelay
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock88640
	beq elsedoneblock88640
ConditionalTrueBlock75461
	rts
elseblock45399
elsedoneblock88640
	jsr WitchPickDirection
	; Assigning single variable : wsteps
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #10
val_var2607 = $54
	sta val_var2607
	jsr Random
	sec
modulo93725
	sbc val_var2607
	bcs modulo93725
	adc val_var2607
	
	clc
	adc #2
	 ; end add / sub var with constant
	
	sta wsteps
	; Binary clause Simplified: GREATER
	lda wdcount
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock68323
	beq elsedoneblock68323
ConditionalTrueBlock53072
	; Assigning single variable : wdcount
	dec wdcount
	; Binary clause Simplified: EQUALS
	lda wdcount
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock84800
ConditionalTrueBlock73520
	; Assigning single variable : wdspeed
	; Load Byte array
	ldx level
	lda lwitch,x
	
	sta wdspeed
elseblock69797
elsedoneblock84800
elseblock99504
elsedoneblock68323
elseblock36498
elsedoneblock47708
	; Binary clause Simplified: NOTEQUALS
	; Modulo
	lda wdspeed
val_var56804 = $54
	sta val_var56804
	lda ticks
	sec
modulo86077
	sbc val_var56804
	bcs modulo86077
	adc val_var56804
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock10615
ConditionalTrueBlock15504
	rts
elseblock80051
elsedoneblock10615
	; Assigning single variable : wsteps
	dec wsteps
	lda wd
	cmp #$0 ;keep
	bne casenext38083
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
	bcs elseblock47665
ConditionalTrueBlock33827
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
	jmp elsedoneblock7158
elseblock47665
	; Assigning single variable : wsteps
	lda #0
	sta wsteps
elsedoneblock7158
	jmp caseend45444
casenext38083
	lda wd
	cmp #$1 ;keep
	bne casenext47180
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
	bcs elseblock2527
ConditionalTrueBlock28137
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
	jmp elsedoneblock92712
elseblock2527
	; Assigning single variable : wsteps
	lda #0
	sta wsteps
elsedoneblock92712
	jmp caseend45444
casenext47180
	lda wd
	cmp #$2 ;keep
	bne casenext72962
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
	bcs elseblock69654
ConditionalTrueBlock7043
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
	jmp elsedoneblock23687
elseblock69654
	; Assigning single variable : wsteps
	lda #0
	sta wsteps
elsedoneblock23687
	jmp caseend45444
casenext72962
	lda wd
	cmp #$3 ;keep
	bne casenext33243
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
	bcc dtnooverflow20281
	iny  ; overflow into high byte
dtnooverflow20281
	
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
	bcc dtnooverflow67070
	iny  ; overflow into high byte
dtnooverflow67070
	
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
	bcs elseblock74078
ConditionalTrueBlock27439
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
	jmp elsedoneblock31407
elseblock74078
	; Assigning single variable : wsteps
	lda #0
	sta wsteps
elsedoneblock31407
	jmp caseend45444
casenext33243
caseend45444
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
	bne elsedoneblock84941
ConditionalTrueBlock16912
	; Binary clause Simplified: EQUALS
	lda cy
	; Compare with pure num / var optimization
	cmp wy;keep
	bne elsedoneblock64897
ConditionalTrueBlock88747
	jsr HitWitch
elseblock18211
elsedoneblock64897
elseblock69144
elsedoneblock84941
	rts
	
	
	; ***********  Defining procedure : Ghost_Respawn
	;    Procedure type : User-defined procedure
	
Ghost_Respawn
	; Binary clause Simplified: GREATEREQUAL
	lda gcurrent
	; Compare with pure num / var optimization
	cmp gNumGhosts;keep
	bcc elsedoneblock88652
ConditionalTrueBlock36291
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
elseblock81562
elsedoneblock88652
	; Assigning single variable : j
	lda #0
	sta j
while20060
	; Binary clause: EQUALS
	lda j
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bne binaryclausefailed49290
binaryclausesuccess28189
	lda #1; success
	jmp binaryclausefinished6255
binaryclausefailed49290
	lda #0 ; failed state
binaryclausefinished6255
	cmp #1
	beq ConditionalTrueBlock33241
	jmp elsedoneblock73022
ConditionalTrueBlock33241
	; Assigning single variable : x
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #20
val_var71506 = $54
	sta val_var71506
	jsr Random
	sec
modulo83469
	sbc val_var71506
	bcs modulo83469
	adc val_var71506
	
	clc
	adc #1
	 ; end add / sub var with constant
	
	sta x
	; Assigning single variable : y
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #18
val_var70873 = $54
	sta val_var70873
	jsr Random
	sec
modulo21698
	sbc val_var70873
	bcs modulo21698
	adc val_var70873
	
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
	bcc dtnooverflow384
	iny  ; overflow into high byte
dtnooverflow384
	
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
	bne elsedoneblock37705
ConditionalTrueBlock54651
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
	bcs elseblock29771
ConditionalTrueBlock0
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
	jmp elsedoneblock91366
elseblock29771
	; Assigning single variable : gmode
	lda #1
	ldx gcurrent
	sta gmode,x
	; Assigning single variable : gx
	lda #0
	sta gx,x
elsedoneblock91366
elseblock28197
elsedoneblock37705
	jmp while20060
elseblock93250
elsedoneblock73022
	rts
	
	
	; ***********  Defining procedure : PlaceAllGhosts
	;    Procedure type : User-defined procedure
	
PlaceAllGhosts
	; Assigning single variable : gcurrent
	lda #0
	sta gcurrent
for96836
	jsr Ghost_Respawn
	; Binary clause Simplified: LESS
	lda gcurrent
	; Compare with pure num / var optimization
	cmp gNumGhosts;keep
	bcs elsedoneblock75918
ConditionalTrueBlock62836
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
for44534
while10898
	; Binary clause Simplified: EQUALS
	lda ticks
	; Compare with pure num / var optimization
	cmp oldTick;keep
	bne elsedoneblock56184
ConditionalTrueBlock44406
	jmp while10898
elseblock6427
elsedoneblock56184
	; Assigning single variable : oldTick
	lda ticks
	sta oldTick
	inc i
	lda #30
	cmp i ;keep
	bne for44534
forLoopDone34724
elseblock93330
elsedoneblock75918
	inc gcurrent
	lda #10
	cmp gcurrent ;keep
	bne for96836
forLoopDone17723
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
for62051
forLoopFix13830
	; Binary clause Simplified: GREATER
	; Load Byte array
	ldx i
	lda gy,x
	
	; Compare with pure num / var optimization
	cmp #$3;keep
	bcc elsedoneblock87241
	beq elsedoneblock87241
ConditionalTrueBlock22968
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
elseblock25748
elsedoneblock87241
	inc i
	lda gNumGhosts
	cmp i ;keep
	beq forLoopDone71121
forLoopNotDone15976
	jmp for62051
forLoopDone71121
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
	bcs ColXConfirmed33930
	cmp #$8
	bcs NoCollision29139
ColXConfirmed33930
	lda y
	clc
	sbc py
	cmp #$fa
	bcs Collision75752
	cmp #$5
	bcs NoCollision29139
Collision75752
	lda #1
	jmp CollisionDone20850
NoCollision29139
	lda #0
CollisionDone20850
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	beq binaryclausefailed1268
binaryclausesuccess68382
	lda #1; success
	jmp binaryclausefinished86550
binaryclausefailed1268
	lda #0 ; failed state
binaryclausefinished86550
	cmp #1
	beq ConditionalTrueBlock68879
	jmp elseblock87862
	jmp elsedoneblock21581
ConditionalTrueBlock68879
	; Binary clause Simplified: EQUALS
	; Load Byte array
	ldx gcurrent
	lda gmode,x
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock67204
ConditionalTrueBlock93938
	; Binary clause Simplified: GREATER
	lda x
	; Compare with pure num / var optimization
	cmp px;keep
	bcc elsedoneblock92574
	beq elsedoneblock92574
ConditionalTrueBlock32763
	; Assigning single variable : x
	dec x
elseblock60218
elsedoneblock92574
	; Binary clause Simplified: LESS
	lda x
	; Compare with pure num / var optimization
	cmp px;keep
	bcs elsedoneblock39333
ConditionalTrueBlock37220
	; Assigning single variable : x
	inc x
elseblock31351
elsedoneblock39333
elseblock59597
elsedoneblock67204
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
	bcc dtnooverflow39531
	iny  ; overflow into high byte
dtnooverflow39531
	
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
	bcc elsedoneblock67418
	beq elsedoneblock67418
ConditionalTrueBlock26375
	; Assigning single variable : x
	lda n
	sta x
elseblock99128
elsedoneblock67418
	; Binary clause Simplified: EQUALS
	; Load Byte array
	ldx gcurrent
	lda gmode,x
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock99643
ConditionalTrueBlock15342
	; Binary clause Simplified: GREATER
	lda y
	; Compare with pure num / var optimization
	cmp py;keep
	bcc elsedoneblock57592
	beq elsedoneblock57592
ConditionalTrueBlock55147
	; Assigning single variable : y
	dec y
elseblock76073
elsedoneblock57592
	; Binary clause Simplified: LESS
	lda y
	; Compare with pure num / var optimization
	cmp py;keep
	bcs elsedoneblock12603
ConditionalTrueBlock95483
	; Assigning single variable : y
	inc y
elseblock18533
elsedoneblock12603
elseblock25723
elsedoneblock99643
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
	bcc dtnooverflow82415
	iny  ; overflow into high byte
dtnooverflow82415
	
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
	bcc elsedoneblock63443
	beq elsedoneblock63443
ConditionalTrueBlock64070
	; Assigning single variable : y
	lda s
	sta y
elseblock8138
elsedoneblock63443
	jmp elsedoneblock21581
elseblock87862
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
	beq elseblock31605
ConditionalTrueBlock57206
	; Assigning single variable : x
	inc x
	jmp elsedoneblock58189
elseblock31605
	; Assigning single variable : x
	dec x
elsedoneblock58189
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
	bcc dtnooverflow16375
	iny  ; overflow into high byte
dtnooverflow16375
	
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
	bcc elsedoneblock17679
	beq elsedoneblock17679
ConditionalTrueBlock32573
	; Assigning single variable : x
	lda n
	sta x
elseblock44745
elsedoneblock17679
	; Binary clause Simplified: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda i
	and #2
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elseblock29328
ConditionalTrueBlock15694
	; Assigning single variable : y
	dec y
	jmp elsedoneblock88747
elseblock29328
	; Assigning single variable : y
	inc y
elsedoneblock88747
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
	bcc dtnooverflow6783
	iny  ; overflow into high byte
dtnooverflow6783
	
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
	bcc elsedoneblock77011
	beq elsedoneblock77011
ConditionalTrueBlock83766
	; Assigning single variable : y
	lda s
	sta y
elseblock70583
elsedoneblock77011
elsedoneblock21581
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
binary_clause_temp_var51141 = $54
	sta binary_clause_temp_var51141
	; Load Byte array
	ldx gcurrent
	lda gx,x
	
binary_clause_temp_2_var69981 = $56
	sta binary_clause_temp_2_var69981
	lda binary_clause_temp_var51141
	cmp binary_clause_temp_2_var69981;keep
	bne elsedoneblock38457
ConditionalTrueBlock92603
	; Binary clause Simplified: EQUALS
	lda cy
binary_clause_temp_var89208 = $54
	sta binary_clause_temp_var89208
	; Load Byte array
	ldx gcurrent
	lda gy,x
	
binary_clause_temp_2_var17500 = $56
	sta binary_clause_temp_2_var17500
	lda binary_clause_temp_var89208
	cmp binary_clause_temp_2_var17500;keep
	bne elsedoneblock10717
ConditionalTrueBlock99310
	; Assigning single variable : i
	lda gcurrent
	sta i
	jsr HitGhost
elseblock71505
elsedoneblock10717
elseblock40480
elsedoneblock38457
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
	bcc binaryclausefailed79967
	beq binaryclausefailed79967
binaryclausesuccess51174
	lda #1; success
	jmp binaryclausefinished77573
binaryclausefailed79967
	lda #0 ; failed state
binaryclausefinished77573
	cmp #1
	beq ConditionalTrueBlock53476
	jmp elsedoneblock71166
ConditionalTrueBlock53476
	; Assigning single variable : gtime
	lda #0
	sta gtime
	; Binary clause Simplified: LESS
	lda gcurrent
	; Compare with pure num / var optimization
	cmp gNumGhosts;keep
	bcs elsedoneblock82832
ConditionalTrueBlock71550
	; Binary clause Simplified: EQUALS
	; Load Byte array
	ldx gcurrent
	lda gmode,x
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock1396
ConditionalTrueBlock18563
	jsr Ghost_Move
elseblock75535
elsedoneblock1396
	; Binary clause Simplified: GREATER
	; Load Byte array
	ldx gcurrent
	lda gmode,x
	
	; Compare with pure num / var optimization
	cmp #$1;keep
	bcc elsedoneblock48753
	beq elsedoneblock48753
ConditionalTrueBlock58171
	; Assigning single variable : gmode
	ldx gcurrent
	; Optimize byte array dec 
	dec gmode,x
	; Binary clause Simplified: EQUALS
	; Load Byte array
	lda gmode,x
	
	; Compare with pure num / var optimization
	cmp #$1;keep
	bne elsedoneblock3333
ConditionalTrueBlock14868
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
elseblock46497
elsedoneblock3333
elseblock53381
elsedoneblock48753
elseblock10925
elsedoneblock82832
	jsr DisplayGhosts
	; Assigning single variable : gcurrent
	inc gcurrent
	; Binary clause Simplified: EQUALS
	lda gcurrent
	; Compare with pure num / var optimization
	cmp #$a;keep
	bne elsedoneblock31593
ConditionalTrueBlock2095
	; Assigning single variable : gcurrent
	lda #0
	sta gcurrent
elseblock83632
elsedoneblock31593
elseblock81230
elsedoneblock71166
	rts
	
	
	; ***********  Defining procedure : SpawnAZombie
	;    Procedure type : User-defined procedure
	
SpawnAZombie
	; Binary clause Simplified: GREATEREQUAL
	lda zNumZombies
	; Compare with pure num / var optimization
	cmp #$a;keep
	bcc elsedoneblock54258
ConditionalTrueBlock79505
	rts
elseblock25162
elsedoneblock54258
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
val_var4437 = $54
	sta val_var4437
	jsr Random
	sec
modulo98164
	sbc val_var4437
	bcs modulo98164
	adc val_var4437
	
	sta t
	; Assigning single variable : y
	lda #3
	sta y
for46821
	; Assigning single variable : x
	lda #1
	sta x
for37585
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
	bne elsedoneblock88974
ConditionalTrueBlock86993
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
	bne elsedoneblock45430
ConditionalTrueBlock45429
	; Assigning single variable : x
	lda #20
	sta x
	; Assigning single variable : y
	lda #21
	sta y
elseblock56278
elsedoneblock45430
	; Assigning single variable : i
	inc i
elseblock88931
elsedoneblock88974
	inc x
	lda #21
	cmp x ;keep
	bne for37585
forLoopDone43470
	inc y
	lda #22
	cmp y ;keep
	bne for46821
forLoopDone15357
	; Binary clause Simplified: GREATER
	lda n
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock7754
	beq elsedoneblock7754
ConditionalTrueBlock75273
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
elseblock32630
elsedoneblock7754
	rts
	
	
	; ***********  Defining procedure : DisplayZombies
	;    Procedure type : User-defined procedure
	
DisplayZombies
	; Assigning single variable : i
	lda #0
	sta i
for31473
forLoopFix23180
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
	bne casenext71754
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
	jmp caseend47869
casenext71754
	lda t
	cmp #$1 ;keep
	bne casenext38641
	; Assigning single variable : screenmemory
	lda #40
	ldy x
	sta (screenmemory),y
	jmp caseend47869
casenext38641
	lda t
	cmp #$2 ;keep
	bne casenext15854
	; Assigning single variable : screenmemory
	lda #41
	ldy x
	sta (screenmemory),y
	jmp caseend47869
casenext15854
	lda t
	cmp #$3 ;keep
	bne casenext46318
	; Assigning single variable : screenmemory
	lda #42
	ldy x
	sta (screenmemory),y
	jmp caseend47869
casenext46318
	; Assigning single variable : screenmemory
	lda #42
	ldy x
	sta (screenmemory),y
caseend47869
	; Assigning single variable : p1
	lda #5
	ldy x
	sta (p1),y
	inc i
	lda zNumZombies
	cmp i ;keep
	beq forLoopDone21737
forLoopNotDone60304
	jmp for31473
forLoopDone21737
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
	beq binaryclausesuccess21785
	bcs binaryclausefailed14682
binaryclausesuccess21785
	lda #1; success
	jmp binaryclausefinished34363
binaryclausefailed14682
	lda #0 ; failed state
binaryclausefinished34363
	cmp #1
	beq ConditionalTrueBlock68905
	jmp elsedoneblock30428
ConditionalTrueBlock68905
	; Binary clause Simplified: GREATER
	lda x
	; Compare with pure num / var optimization
	cmp px;keep
	bcc elsedoneblock73897
	beq elsedoneblock73897
ConditionalTrueBlock27909
	; Assigning single variable : x
	dec x
elseblock72444
elsedoneblock73897
	; Binary clause Simplified: LESS
	lda x
	; Compare with pure num / var optimization
	cmp px;keep
	bcs elsedoneblock41335
ConditionalTrueBlock53524
	; Assigning single variable : x
	inc x
elseblock92039
elsedoneblock41335
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
	bcc dtnooverflow39568
	iny  ; overflow into high byte
dtnooverflow39568
	
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
	bcc elsedoneblock85493
	beq elsedoneblock85493
ConditionalTrueBlock81805
	; Assigning single variable : x
	lda n
	sta x
elseblock91222
elsedoneblock85493
	; Binary clause Simplified: GREATER
	lda y
	; Compare with pure num / var optimization
	cmp py;keep
	bcc elsedoneblock85955
	beq elsedoneblock85955
ConditionalTrueBlock95364
	; Assigning single variable : y
	dec y
elseblock84457
elsedoneblock85955
	; Binary clause Simplified: LESS
	lda y
	; Compare with pure num / var optimization
	cmp py;keep
	bcs elsedoneblock5291
ConditionalTrueBlock96181
	; Assigning single variable : y
	inc y
elseblock42290
elsedoneblock5291
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
	bcc dtnooverflow31720
	iny  ; overflow into high byte
dtnooverflow31720
	
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
	bcc elsedoneblock36842
	beq elsedoneblock36842
ConditionalTrueBlock8891
	; Assigning single variable : y
	lda s
	sta y
elseblock21969
elsedoneblock36842
elseblock42848
elsedoneblock30428
	; Binary clause: EQUALS
	lda t
	; Compare with pure num / var optimization
	cmp #$2;keep
	; BC done
	bne binaryclausefailed81515
binaryclausesuccess11231
	lda #1; success
	jmp binaryclausefinished84762
binaryclausefailed81515
	lda #0 ; failed state
binaryclausefinished84762
	cmp #1
	beq ConditionalTrueBlock55712
	jmp elsedoneblock85290
ConditionalTrueBlock55712
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
	beq elseblock29155
ConditionalTrueBlock59749
	; Assigning single variable : x
	inc x
	jmp elsedoneblock85052
elseblock29155
	; Assigning single variable : x
	dec x
elsedoneblock85052
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
	bcc dtnooverflow41175
	iny  ; overflow into high byte
dtnooverflow41175
	
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
	bcc elsedoneblock1999
	beq elsedoneblock1999
ConditionalTrueBlock46769
	; Assigning single variable : x
	lda n
	sta x
elseblock71652
elsedoneblock1999
	; Binary clause Simplified: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda i
	and #2
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elseblock59288
ConditionalTrueBlock41588
	; Assigning single variable : y
	dec y
	jmp elsedoneblock30740
elseblock59288
	; Assigning single variable : y
	inc y
elsedoneblock30740
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
	bcc dtnooverflow92515
	iny  ; overflow into high byte
dtnooverflow92515
	
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
	bcc elsedoneblock74718
	beq elsedoneblock74718
ConditionalTrueBlock77383
	; Assigning single variable : y
	lda s
	sta y
elseblock18093
elsedoneblock74718
elseblock55937
elsedoneblock85290
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
binary_clause_temp_var72456 = $54
	sta binary_clause_temp_var72456
	; Load Byte array
	ldx zcurrent
	lda zx,x
	
binary_clause_temp_2_var5855 = $56
	sta binary_clause_temp_2_var5855
	lda binary_clause_temp_var72456
	cmp binary_clause_temp_2_var5855;keep
	bne elsedoneblock54839
ConditionalTrueBlock59816
	; Binary clause Simplified: EQUALS
	lda cy
binary_clause_temp_var93392 = $54
	sta binary_clause_temp_var93392
	; Load Byte array
	ldx zcurrent
	lda zy,x
	
binary_clause_temp_2_var37154 = $56
	sta binary_clause_temp_2_var37154
	lda binary_clause_temp_var93392
	cmp binary_clause_temp_2_var37154;keep
	bne elsedoneblock28287
ConditionalTrueBlock65143
	; Assigning single variable : i
	lda zcurrent
	sta i
	jsr HitZombie
elseblock4815
elsedoneblock28287
elseblock25585
elsedoneblock54839
	rts
	
	
	; ***********  Defining procedure : UpdateZombies
	;    Procedure type : User-defined procedure
	
UpdateZombies
	; Binary clause Simplified: EQUALS
	lda zNumZombies
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock87559
ConditionalTrueBlock46657
	rts
elseblock25381
elsedoneblock87559
	; Binary clause Simplified: EQUALS
	; Modulo
	lda ztime
val_var14845 = $54
	sta val_var14845
	lda ticks
	sec
modulo64896
	sbc val_var14845
	bcs modulo64896
	adc val_var14845
	
	; Compare with pure num / var optimization
	cmp #$1;keep
	bne elsedoneblock68082
ConditionalTrueBlock98658
	; Binary clause Simplified: LESS
	lda zcurrent
	; Compare with pure num / var optimization
	cmp zNumZombies;keep
	bcs elsedoneblock89121
ConditionalTrueBlock42061
	; Binary clause Simplified: EQUALS
	; Load Byte array
	ldx zcurrent
	lda zspawn,x
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock63903
ConditionalTrueBlock18038
	jsr Zombie_Move
elseblock93231
elsedoneblock63903
	; Binary clause Simplified: GREATER
	; Load Byte array
	ldx zcurrent
	lda zspawn,x
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock66779
	beq elsedoneblock66779
ConditionalTrueBlock12904
	; Assigning single variable : zspawn
	ldx zcurrent
	; Optimize byte array dec 
	dec zspawn,x
elseblock49343
elsedoneblock66779
elseblock88690
elsedoneblock89121
	jsr DisplayZombies
	; Assigning single variable : zcurrent
	inc zcurrent
	; Binary clause Simplified: EQUALS
	lda zcurrent
	; Compare with pure num / var optimization
	cmp #$a;keep
	bne elsedoneblock42709
ConditionalTrueBlock22711
	; Assigning single variable : zcurrent
	lda #0
	sta zcurrent
elseblock72653
elsedoneblock42709
elseblock33155
elsedoneblock68082
	rts
	
	
	; ***********  Defining procedure : SpawnCandy
	;    Procedure type : User-defined procedure
	
SpawnCandy
	; Assigning single variable : x
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #20
val_var72457 = $54
	sta val_var72457
	jsr Random
	sec
modulo96745
	sbc val_var72457
	bcs modulo96745
	adc val_var72457
	
	clc
	adc #1
	 ; end add / sub var with constant
	
	sta x
	; Assigning single variable : y
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #16
val_var79443 = $54
	sta val_var79443
	jsr Random
	sec
modulo9034
	sbc val_var79443
	bcs modulo9034
	adc val_var79443
	
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
while8008
	; Binary clause Simplified: EQUALS
	lda t
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock14147
ConditionalTrueBlock42220
	; Binary clause Simplified: EQUALS
	; Load Byte array
	; Load pointer array
	ldy x
	lda (screenmemory),y
	
	tax
	lda cMask,x
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock16980
ConditionalTrueBlock60862
	; Assigning single variable : t
	lda #1
	sta t
elseblock53323
elsedoneblock16980
	; Binary clause Simplified: EQUALS
	lda t
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock93661
ConditionalTrueBlock76985
	; Assigning single variable : x
	inc x
	; Binary clause Simplified: GREATER
	lda x
	; Compare with pure num / var optimization
	cmp #$14;keep
	bcc elsedoneblock94750
	beq elsedoneblock94750
ConditionalTrueBlock11973
	; Assigning single variable : x
	lda #1
	sta x
	; Assigning single variable : y
	inc y
	; Binary clause Simplified: GREATER
	lda y
	; Compare with pure num / var optimization
	cmp #$16;keep
	bcc elsedoneblock1248
	beq elsedoneblock1248
ConditionalTrueBlock69510
	; Assigning single variable : y
	lda #4
	sta y
elseblock14688
elsedoneblock1248
elseblock75584
elsedoneblock94750
elseblock10977
elsedoneblock93661
	jmp while8008
elseblock73909
elsedoneblock14147
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
	bne elsedoneblock27897
ConditionalTrueBlock30535
	; Assigning single variable : psnd2
	lda #<sndCAppear
	ldx #>sndCAppear
	sta psnd2
	stx psnd2+1
	; Assigning single variable : vsnd2
	lda #250
	sta vsnd2
	jsr PlaySnd2
elseblock45013
elsedoneblock27897
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
	bcc elsedoneblock67318
	beq elsedoneblock67318
ConditionalTrueBlock80173
	; Assigning single variable : p1
	lda #3
	ldy cx
	sta (p1),y
elseblock15943
elsedoneblock67318
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
for16351
	; Assigning single variable : screenmemory
	lda #0
	ldy i
	sta (screenmemory),y
	inc i
	lda #6
	cmp i ;keep
	bcs for16351
forLoopDone70938
	; Assigning single variable : i
	lda #1
	sta i
for78210
	; Binary clause Simplified: GREATEREQUAL
	lda candy
	; Compare with pure num / var optimization
	cmp i;keep
	bcc elsedoneblock10504
ConditionalTrueBlock41303
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
elseblock54358
elsedoneblock10504
	; Binary clause Simplified: GREATEREQUAL
	lda witchHits
	; Compare with pure num / var optimization
	cmp i;keep
	bcc elsedoneblock70715
ConditionalTrueBlock93921
	; Assigning single variable : screenmemory
	lda #38
	ldy i
	sta (screenmemory),y
	; Assigning single variable : p1
	lda #1
	sta (p1),y
elseblock72484
elsedoneblock70715
	inc i
	lda #6
	cmp i ;keep
	bcs for78210
forLoopDone72005
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
	bne elsedoneblock15359
ConditionalTrueBlock16425
	; Assigning single variable : cx
	inc cx
elseblock82965
elsedoneblock15359
	; Binary clause Simplified: NOTEQUALS
	lda cdx
	; Compare with pure num / var optimization
	cmp #$1;keep
	beq elsedoneblock32709
ConditionalTrueBlock9337
	; Assigning single variable : cx
	dec cx
elseblock61281
elsedoneblock32709
	; Binary clause Simplified: EQUALS
	; Load Byte array
	; Load pointer array
	ldy cx
	lda (screenmemory),y
	
	tax
	lda cMask,x
	
	; Compare with pure num / var optimization
	cmp #$a;keep
	bne elsedoneblock71092
ConditionalTrueBlock12855
	; Assigning single variable : cx
	lda x
	sta cx
	; Binary clause Simplified: LESS
	lda candyscr
	; Compare with pure num / var optimization
	cmp #$4;keep
	bcs elsedoneblock86710
ConditionalTrueBlock76448
	; Assigning single variable : candyscr
	inc candyscr
elseblock24337
elsedoneblock86710
elseblock76630
elsedoneblock71092
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
	bne tempfail3118
binaryclausesuccess68677
	jmp ConditionalTrueBlock42532
tempfail3118
	; Binary clause: EQUALS
	lda clife
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bne elseblock97581
binaryclausesuccess87773
ConditionalTrueBlock42532
	jsr EndCandy
elseblock97581
elsedoneblock46816
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
	beq binaryclausefailed91791
binaryclausesuccess11067
	lda #1; success
	jmp binaryclausefinished3187
binaryclausefailed91791
	lda #0 ; failed state
binaryclausefinished3187
	cmp #1
	beq ConditionalTrueBlock47514
	jmp elsedoneblock69029
ConditionalTrueBlock47514
	; Binary clause: EQUALS
	lda clife
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bne binaryclausefailed82391
binaryclausesuccess14065
	lda #1; success
	jmp binaryclausefinished46868
binaryclausefailed82391
	lda #0 ; failed state
binaryclausefinished46868
	cmp #1
	beq ConditionalTrueBlock81420
	jmp elsedoneblock8081
ConditionalTrueBlock81420
	; Binary clause: GREATER
	lda candy
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bcc binaryclausefailed70051
	beq binaryclausefailed70051
binaryclausesuccess53170
	lda #1; success
	jmp binaryclausefinished52463
binaryclausefailed70051
	lda #0 ; failed state
binaryclausefinished52463
	cmp #1
	beq ConditionalTrueBlock39945
	jmp elsedoneblock70622
ConditionalTrueBlock39945
	; Assigning single variable : cy
	lda py
	sta cy
	; Binary clause Simplified: EQUALS
	lda pd
	; Compare with pure num / var optimization
	cmp #$3a;keep
	bne elsedoneblock71097
ConditionalTrueBlock14692
	; Assigning single variable : cdx
	lda #255
	sta cdx
elseblock31993
elsedoneblock71097
	; Binary clause Simplified: EQUALS
	lda pd
	; Compare with pure num / var optimization
	cmp #$3d;keep
	bne elsedoneblock57714
ConditionalTrueBlock48217
	; Assigning single variable : cdx
	lda #1
	sta cdx
elseblock85554
elsedoneblock57714
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
	bne elsedoneblock37961
ConditionalTrueBlock14514
	; Assigning single variable : x
	; 8 bit binop
	; Add/sub where right value is constant number
	lda px
	clc
	adc #1
	 ; end add / sub var with constant
	
	sta x
elseblock7023
elsedoneblock37961
	; Binary clause Simplified: NOTEQUALS
	lda cdx
	; Compare with pure num / var optimization
	cmp #$1;keep
	beq elsedoneblock53525
ConditionalTrueBlock95285
	; Assigning single variable : x
	; 8 bit binop
	; Add/sub where right value is constant number
	lda px
	sec
	sbc #1
	 ; end add / sub var with constant
	
	sta x
elseblock50853
elsedoneblock53525
	; Binary clause Simplified: EQUALS
	; Load Byte array
	; Load pointer array
	ldy x
	lda (screenmemory),y
	
	tax
	lda cMask,x
	
	; Compare with pure num / var optimization
	cmp #$a;keep
	bne elseblock49318
ConditionalTrueBlock97233
	; Assigning single variable : cy
	lda #0
	sta cy
	; Assigning single variable : cx
	sta cx
	; Assigning single variable : cdx
	sta cdx
	jmp elsedoneblock18593
elseblock49318
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
elsedoneblock18593
elseblock51094
elsedoneblock70622
elseblock92453
elsedoneblock8081
elseblock79315
elsedoneblock69029
	; Binary clause Simplified: GREATER
	lda clife
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock96344
	beq elsedoneblock96344
ConditionalTrueBlock22897
	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda ticks
	and #1
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock32819
ConditionalTrueBlock15453
	jsr MoveCandy
elseblock31998
elsedoneblock32819
elseblock26036
elsedoneblock96344
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
	beq binaryclausefailed6168
binaryclausesuccess35156
	lda #1; success
	jmp binaryclausefinished40114
binaryclausefailed6168
	lda #0 ; failed state
binaryclausefinished40114
	cmp #1
	beq ConditionalTrueBlock52594
	jmp elsedoneblock89199
ConditionalTrueBlock52594
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
	bcc dtnooverflow30719
	iny  ; overflow into high byte
dtnooverflow30719
	
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
	bcc dtnooverflow49758
	iny  ; overflow into high byte
dtnooverflow49758
	
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
	bcs elsedoneblock2235
ConditionalTrueBlock30576
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
elseblock65698
elsedoneblock2235
	rts
elseblock96777
elsedoneblock89199
	; Binary clause: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1
	and #1
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	beq binaryclausefailed39155
binaryclausesuccess25926
	lda #1; success
	jmp binaryclausefinished76396
binaryclausefailed39155
	lda #0 ; failed state
binaryclausefinished76396
	cmp #1
	beq ConditionalTrueBlock87549
	jmp elsedoneblock772
ConditionalTrueBlock87549
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
	bcs elsedoneblock53469
ConditionalTrueBlock63292
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
elseblock16245
elsedoneblock53469
	rts
elseblock93376
elsedoneblock772
	; Binary clause: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1
	and #2
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	beq binaryclausefailed31565
binaryclausesuccess4701
	lda #1; success
	jmp binaryclausefinished4722
binaryclausefailed31565
	lda #0 ; failed state
binaryclausefinished4722
	cmp #1
	beq ConditionalTrueBlock40939
	jmp elsedoneblock73352
ConditionalTrueBlock40939
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
	bcs elsedoneblock33535
ConditionalTrueBlock9427
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
elseblock18104
elsedoneblock33535
	rts
elseblock48054
elsedoneblock73352
	; Binary clause: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1
	and #4
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	beq binaryclausefailed27731
binaryclausesuccess41034
	lda #1; success
	jmp binaryclausefinished72798
binaryclausefailed27731
	lda #0 ; failed state
binaryclausefinished72798
	cmp #1
	beq ConditionalTrueBlock79546
	jmp elsedoneblock31951
ConditionalTrueBlock79546
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
	bcs elsedoneblock20932
ConditionalTrueBlock48838
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
elseblock82097
elsedoneblock20932
	rts
elseblock15439
elsedoneblock31951
	rts
	
	
	; ***********  Defining procedure : UpdatePlayer
	;    Procedure type : User-defined procedure
	
UpdatePlayer
	; Binary clause Simplified: EQUALS
	; Modulo
	lda #2
val_var60409 = $54
	sta val_var60409
	lda ticks
	sec
modulo3447
	sbc val_var60409
	bcs modulo3447
	adc val_var60409
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock23901
ConditionalTrueBlock8731
	; Assigning single variable : pan
	inc pan
elseblock65576
elsedoneblock23901
	; Binary clause Simplified: GREATER
	lda pan
	; Compare with pure num / var optimization
	cmp #$5;keep
	bcc elsedoneblock28133
	beq elsedoneblock28133
ConditionalTrueBlock9709
	; Assigning single variable : pan
	lda #0
	sta pan
elseblock21158
elsedoneblock28133
	; Binary clause Simplified: EQUALS
	lda joy1
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock37423
ConditionalTrueBlock84249
	; Assigning single variable : pstep
	lda #0
	sta pstep
elseblock21467
elsedoneblock37423
	; Binary clause Simplified: GREATER
	lda pstep
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock72205
	beq elsedoneblock72205
ConditionalTrueBlock29393
	; Assigning single variable : pstep
	dec pstep
elseblock54733
elsedoneblock72205
	; Binary clause Simplified: EQUALS
	lda pstep
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock44929
ConditionalTrueBlock15655
	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1
	and #16
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock23094
ConditionalTrueBlock12549
	jsr MovePlayer
	; Binary clause Simplified: GREATER
	lda pstep
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock6070
	beq elsedoneblock6070
ConditionalTrueBlock10538
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
	bne elseblock19869
ConditionalTrueBlock97327
	; Assigning single variable : stepsnd
	lda #30
	sta stepsnd
	jmp elsedoneblock43681
elseblock19869
	; Assigning single variable : stepsnd
	lda #0
	sta stepsnd
elsedoneblock43681
elseblock14208
elsedoneblock6070
elseblock29238
elsedoneblock23094
elseblock1792
elsedoneblock44929
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
	bne elsedoneblock14545
ConditionalTrueBlock69575
	; Binary clause Simplified: LESS
	lda candy
binary_clause_temp_var93914 = $54
	sta binary_clause_temp_var93914
	; 8 bit binop
	; Add/sub where right value is constant number
	lda #6
	sec
	sbc witchHits
	 ; end add / sub var with constant
	
binary_clause_temp_2_var72631 = $56
	sta binary_clause_temp_2_var72631
	lda binary_clause_temp_var93914
	cmp binary_clause_temp_2_var72631;keep
	bcs elsedoneblock71665
ConditionalTrueBlock68313
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
	bcc elsedoneblock30159
	beq elsedoneblock30159
ConditionalTrueBlock65866
	; Assigning single variable : candyscr
	dec candyscr
elseblock97644
elsedoneblock30159
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
elseblock30305
elsedoneblock71665
elseblock24592
elsedoneblock14545
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
	bcc elsedoneblock10953
ConditionalTrueBlock86308
	jsr SpawnAZombie
elseblock81442
elsedoneblock10953
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
for30748
	; Assigning single variable : screenmemory
	lda #0
	ldy i
	sta (screenmemory),y
	inc i
	lda #8
	cmp i ;keep
	bne for30748
forLoopDone16631
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #2
	bcc WordAdd62603
	inc screenmemory+1
WordAdd62603
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
	bne casenext46494
	jsr ClearTime
	clc
	lda #<t11
	adc #0
	ldy #>t11
	sta print_text+0
	sty print_text+1
	ldx #4 ; optimized, look out for bugs
	jsr printstring
	jmp caseend23048
casenext46494
	lda moonPos
	cmp #$6 ;keep
	bne casenext52173
	jsr ClearTime
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	sec
	sbc #2
	bcs WordAdd38844
	dec screenmemory+1
WordAdd38844
	sta screenmemory+0
	clc
	lda #<t12
	adc #0
	ldy #>t12
	sta print_text+0
	sty print_text+1
	ldx #8 ; optimized, look out for bugs
	jsr printstring
	jmp caseend23048
casenext52173
	lda moonPos
	cmp #$8 ;keep
	bne casenext56022
	jsr ClearTime
	clc
	lda #<t01
	adc #0
	ldy #>t01
	sta print_text+0
	sty print_text+1
	ldx #4 ; optimized, look out for bugs
	jsr printstring
	jmp caseend23048
casenext56022
	lda moonPos
	cmp #$a ;keep
	bne casenext6263
	jsr ClearTime
	clc
	lda #<t02
	adc #0
	ldy #>t02
	sta print_text+0
	sty print_text+1
	ldx #4 ; optimized, look out for bugs
	jsr printstring
	jmp caseend23048
casenext6263
	lda moonPos
	cmp #$c ;keep
	bne casenext78400
	jsr ClearTime
	clc
	lda #<t03
	adc #0
	ldy #>t03
	sta print_text+0
	sty print_text+1
	ldx #4 ; optimized, look out for bugs
	jsr printstring
	jmp caseend23048
casenext78400
	lda moonPos
	cmp #$e ;keep
	bne casenext31901
	jsr ClearTime
	clc
	lda #<t04
	adc #0
	ldy #>t04
	sta print_text+0
	sty print_text+1
	ldx #4 ; optimized, look out for bugs
	jsr printstring
	jmp caseend23048
casenext31901
	lda moonPos
	cmp #$10 ;keep
	bne casenext23376
	jsr ClearTime
	clc
	lda #<t05
	adc #0
	ldy #>t05
	sta print_text+0
	sty print_text+1
	ldx #4 ; optimized, look out for bugs
	jsr printstring
	jmp caseend23048
casenext23376
	lda moonPos
	cmp #$12 ;keep
	bne casenext89034
	jsr ClearTime
	clc
	lda #<t06
	adc #0
	ldy #>t06
	sta print_text+0
	sty print_text+1
	ldx #4 ; optimized, look out for bugs
	jsr printstring
	jmp caseend23048
casenext89034
caseend23048
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
	bne elsedoneblock48952
ConditionalTrueBlock3447
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
	bne elsedoneblock76846
ConditionalTrueBlock61018
	; Binary clause Simplified: LESS
	lda candyscr
	; Compare with pure num / var optimization
	cmp #$3;keep
	bcs elsedoneblock79021
ConditionalTrueBlock58507
	jsr SpawnCandy
elseblock71786
elsedoneblock79021
elseblock99076
elsedoneblock76846
	; Binary clause Simplified: EQUALS
	lda cloudPos
	; Compare with pure num / var optimization
	cmp #$16;keep
	bne elsedoneblock36975
ConditionalTrueBlock66900
	; Assigning single variable : cloudPos
	lda #0
	sta cloudPos
	jsr MoveMoon
	jsr DisplayTime
	; Binary clause Simplified: LESS
	lda candyscr
	; Compare with pure num / var optimization
	cmp #$3;keep
	bcs elsedoneblock20277
ConditionalTrueBlock93148
	jsr SpawnCandy
elseblock82521
elsedoneblock20277
elseblock37368
elsedoneblock36975
	; Assigning single variable : screenmemory
	lda #44
	ldx #148
	sta screenmemory
	stx screenmemory+1
	; Assigning single variable : screenmemory
	lda #1
	ldy cloudPos
	sta (screenmemory),y
elseblock34326
elsedoneblock48952
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
while15293
	; Binary clause Simplified: NOTEQUALS
	; Load pointer array
	ldy #$0
	lda (psnd1),y
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock64095
ConditionalTrueBlock98013
	jmp while15293
elseblock89599
elsedoneblock64095
	jsr StopSounds
	; Assigning single variable : sndFlag
	lda #0
	sta sndFlag
while49841
	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1pressed
	and #16
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock45215
ConditionalTrueBlock21897
	; Assigning single variable : x
	lda y
	sta x
	; Assigning single variable : y
	inc y
	; Binary clause Simplified: GREATER
	; Compare with pure num / var optimization
	cmp #$7;keep
	bcc elsedoneblock59401
	beq elsedoneblock59401
ConditionalTrueBlock16081
	; Assigning single variable : y
	lda #1
	sta y
elseblock49449
elsedoneblock59401
	jsr callReadJoy1
	; Assigning single variable : t
	lda #0
	sta t
for4616
	; Assigning single variable : p1
	lda x
	ldy t
	sta (p1),y
	; Assigning single variable : x
	inc x
	; Binary clause Simplified: GREATER
	; Compare with pure num / var optimization
	cmp #$7;keep
	bcc elsedoneblock29626
	beq elsedoneblock29626
ConditionalTrueBlock30430
	; Assigning single variable : x
	lda #1
	sta x
elseblock65141
elsedoneblock29626
	inc t
	lda #14
	cmp t ;keep
	bne for4616
forLoopDone60186
	jmp while49841
elseblock78788
elsedoneblock45215
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
	bne elsedoneblock83828
ConditionalTrueBlock20305
	; Binary clause Simplified: EQUALS
	lda cloudPos
	; Compare with pure num / var optimization
	cmp #$15;keep
	bne elsedoneblock44709
ConditionalTrueBlock12419
	; Assigning single variable : gameMode
	lda #1
	sta gameMode
	; Assigning single variable : screenmemory
	lda #48
	ldx #16
	sta screenmemory
	stx screenmemory+1
printstring_call36101
	clc
	lda #<printstring_text51678
	adc #0
	ldy #>printstring_text51678
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
elseblock84420
elsedoneblock44709
elseblock70918
elsedoneblock83828
	; Binary clause: EQUALS
	lda px
	; Compare with pure num / var optimization
	cmp lx;keep
	; BC done
	bne binaryclausefailed91127
binaryclausesuccess51187
	lda #1; success
	jmp binaryclausefinished85800
binaryclausefailed91127
	lda #0 ; failed state
binaryclausefinished85800
	cmp #1
	beq ConditionalTrueBlock65727
	jmp elsedoneblock22670
ConditionalTrueBlock65727
	; Binary clause: EQUALS
	lda py
	; Compare with pure num / var optimization
	cmp ly;keep
	; BC done
	bne binaryclausefailed35737
binaryclausesuccess1173
	lda #1; success
	jmp binaryclausefinished85439
binaryclausefailed35737
	lda #0 ; failed state
binaryclausefinished85439
	cmp #1
	beq ConditionalTrueBlock10515
	jmp elsedoneblock75640
ConditionalTrueBlock10515
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
val_var40193 = $54
	sta val_var40193
	jsr Random
	sec
modulo89370
	sbc val_var40193
	bcs modulo89370
	adc val_var40193
	
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
	bcs elsedoneblock32286
ConditionalTrueBlock84108
	; Assigning single variable : lives
	inc lives
elseblock19626
elsedoneblock32286
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
elseblock40400
elsedoneblock75640
elseblock73855
elsedoneblock22670
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
	bne elsedoneblock70659
ConditionalTrueBlock99330
	; Binary clause Simplified: EQUALS
	lda py
	; Compare with pure num / var optimization
	cmp wy;keep
	bne elsedoneblock52400
ConditionalTrueBlock56747
	; Assigning single variable : gameMode
	lda #1
	sta gameMode
	; Assigning single variable : screenmemory
	lda #48
	ldx #16
	sta screenmemory
	stx screenmemory+1
printstring_call96798
	clc
	lda #<printstring_text76701
	adc #0
	ldy #>printstring_text76701
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
elseblock39620
elsedoneblock52400
elseblock53718
elsedoneblock70659
	; Binary clause Simplified: EQUALS
	lda pbgchar
	; Compare with pure num / var optimization
	cmp #$26;keep
	bne elsedoneblock6122
ConditionalTrueBlock29085
	; Assigning single variable : gameMode
	lda #1
	sta gameMode
	; Assigning single variable : screenmemory
	lda #48
	ldx #16
	sta screenmemory
	stx screenmemory+1
printstring_call25267
	clc
	lda #<printstring_text48002
	adc #0
	ldy #>printstring_text48002
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
elseblock72754
elsedoneblock6122
	; Assigning single variable : i
	lda #0
	sta i
for49390
	; Binary clause Simplified: EQUALS
	lda px
binary_clause_temp_var17348 = $54
	sta binary_clause_temp_var17348
	; Load Byte array
	ldx i
	lda gx,x
	
binary_clause_temp_2_var73735 = $56
	sta binary_clause_temp_2_var73735
	lda binary_clause_temp_var17348
	cmp binary_clause_temp_2_var73735;keep
	bne elsedoneblock35124
ConditionalTrueBlock15623
	; Binary clause Simplified: EQUALS
	lda py
binary_clause_temp_var72896 = $54
	sta binary_clause_temp_var72896
	; Load Byte array
	ldx i
	lda gy,x
	
binary_clause_temp_2_var49644 = $56
	sta binary_clause_temp_2_var49644
	lda binary_clause_temp_var72896
	cmp binary_clause_temp_2_var49644;keep
	bne elsedoneblock94287
ConditionalTrueBlock77619
	; Assigning single variable : gameMode
	lda #1
	sta gameMode
	; Assigning single variable : screenmemory
	lda #48
	ldx #16
	sta screenmemory
	stx screenmemory+1
printstring_call4939
	clc
	lda #<printstring_text97521
	adc #0
	ldy #>printstring_text97521
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
elseblock5908
elsedoneblock94287
elseblock98077
elsedoneblock35124
	inc i
	lda gNumGhosts
	cmp i ;keep
	bne for49390
forLoopDone92235
	; Binary clause Simplified: GREATER
	lda zNumZombies
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock44015
	beq elsedoneblock44015
ConditionalTrueBlock19368
	; Assigning single variable : i
	lda #0
	sta i
for42425
	; Binary clause Simplified: EQUALS
	lda px
binary_clause_temp_var36254 = $54
	sta binary_clause_temp_var36254
	; Load Byte array
	ldx i
	lda zx,x
	
binary_clause_temp_2_var7565 = $56
	sta binary_clause_temp_2_var7565
	lda binary_clause_temp_var36254
	cmp binary_clause_temp_2_var7565;keep
	bne elsedoneblock27950
ConditionalTrueBlock53086
	; Binary clause Simplified: EQUALS
	lda py
binary_clause_temp_var64276 = $54
	sta binary_clause_temp_var64276
	; Load Byte array
	ldx i
	lda zy,x
	
binary_clause_temp_2_var90811 = $56
	sta binary_clause_temp_2_var90811
	lda binary_clause_temp_var64276
	cmp binary_clause_temp_2_var90811;keep
	bne elsedoneblock54382
ConditionalTrueBlock23463
	; Assigning single variable : gameMode
	lda #1
	sta gameMode
	; Assigning single variable : screenmemory
	lda #48
	ldx #16
	sta screenmemory
	stx screenmemory+1
printstring_call80820
	clc
	lda #<printstring_text2122
	adc #0
	ldy #>printstring_text2122
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
elseblock19319
elsedoneblock54382
elseblock73504
elsedoneblock27950
	inc i
	lda zNumZombies
	cmp i ;keep
	bne for42425
forLoopDone36779
elseblock48997
elsedoneblock44015
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
	bne elsedoneblock62861
ConditionalTrueBlock57432
	rts
elseblock870
elsedoneblock62861
	; Binary clause Simplified: EQUALS
	lda cx
	; Compare with pure num / var optimization
	cmp wx;keep
	bne elsedoneblock74859
ConditionalTrueBlock29946
	; Binary clause Simplified: EQUALS
	lda cy
	; Compare with pure num / var optimization
	cmp wy;keep
	bne elsedoneblock20025
ConditionalTrueBlock7451
	jsr HitWitch
elseblock7710
elsedoneblock20025
elseblock64735
elsedoneblock74859
	; Assigning single variable : i
	lda #0
	sta i
for74350
	; Binary clause Simplified: EQUALS
	lda cx
binary_clause_temp_var58006 = $54
	sta binary_clause_temp_var58006
	; Load Byte array
	ldx i
	lda gx,x
	
binary_clause_temp_2_var94308 = $56
	sta binary_clause_temp_2_var94308
	lda binary_clause_temp_var58006
	cmp binary_clause_temp_2_var94308;keep
	bne elsedoneblock57075
ConditionalTrueBlock41178
	; Binary clause Simplified: EQUALS
	lda cy
binary_clause_temp_var17314 = $54
	sta binary_clause_temp_var17314
	; Load Byte array
	ldx i
	lda gy,x
	
binary_clause_temp_2_var31929 = $56
	sta binary_clause_temp_2_var31929
	lda binary_clause_temp_var17314
	cmp binary_clause_temp_2_var31929;keep
	bne elsedoneblock29924
ConditionalTrueBlock59821
	jsr HitGhost
elseblock69252
elsedoneblock29924
elseblock56569
elsedoneblock57075
	inc i
	lda gNumGhosts
	cmp i ;keep
	bne for74350
forLoopDone10274
	; Binary clause Simplified: GREATER
	lda zNumZombies
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock86147
	beq elsedoneblock86147
ConditionalTrueBlock66925
	; Assigning single variable : i
	lda #0
	sta i
for91511
	; Binary clause Simplified: EQUALS
	lda cx
binary_clause_temp_var14356 = $54
	sta binary_clause_temp_var14356
	; Load Byte array
	ldx i
	lda zx,x
	
binary_clause_temp_2_var47494 = $56
	sta binary_clause_temp_2_var47494
	lda binary_clause_temp_var14356
	cmp binary_clause_temp_2_var47494;keep
	bne elsedoneblock10362
ConditionalTrueBlock65784
	; Binary clause Simplified: EQUALS
	lda cy
binary_clause_temp_var8183 = $54
	sta binary_clause_temp_var8183
	; Load Byte array
	ldx i
	lda zy,x
	
binary_clause_temp_2_var59298 = $56
	sta binary_clause_temp_2_var59298
	lda binary_clause_temp_var8183
	cmp binary_clause_temp_2_var59298;keep
	bne elsedoneblock98616
ConditionalTrueBlock5135
	jsr HitZombie
elseblock16215
elsedoneblock98616
elseblock78322
elsedoneblock10362
	inc i
	lda zNumZombies
	cmp i ;keep
	bne for91511
forLoopDone54879
elseblock91808
elsedoneblock86147
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
printstring_call36241
	clc
	lda #<printstring_text69111
	adc #0
	ldy #>printstring_text69111
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
while42173
	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1pressed
	and #16
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock5777
ConditionalTrueBlock29614
	; Assigning single variable : x
	lda y
	sta x
	; Assigning single variable : y
	inc y
	; Binary clause Simplified: GREATER
	; Compare with pure num / var optimization
	cmp #$7;keep
	bcc elsedoneblock15320
	beq elsedoneblock15320
ConditionalTrueBlock93084
	; Assigning single variable : y
	lda #1
	sta y
elseblock7647
elsedoneblock15320
	jsr callReadJoy1
	; Assigning single variable : t
	lda #0
	sta t
for37449
	; Assigning single variable : p1
	lda x
	ldy t
	sta (p1),y
	; Assigning single variable : x
	inc x
	; Binary clause Simplified: GREATER
	; Compare with pure num / var optimization
	cmp #$7;keep
	bcc elsedoneblock50144
	beq elsedoneblock50144
ConditionalTrueBlock60218
	; Assigning single variable : x
	lda #1
	sta x
elseblock63806
elsedoneblock50144
	inc t
	lda #18
	cmp t ;keep
	bne for37449
forLoopDone23980
	jmp while42173
elseblock40342
elsedoneblock5777
	rts
	
	
	; ***********  Defining procedure : GameLoop
	;    Procedure type : User-defined procedure
	
GameLoop
while93389
	; Binary clause: EQUALS
	lda gameMode
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bne binaryclausefailed96801
binaryclausesuccess91267
	lda #1; success
	jmp binaryclausefinished17837
binaryclausefailed96801
	lda #0 ; failed state
binaryclausefinished17837
	cmp #1
	beq ConditionalTrueBlock5883
	jmp elsedoneblock90934
ConditionalTrueBlock5883
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
	bne elsedoneblock86855
ConditionalTrueBlock91397
	jsr CollisionPlayer
elseblock84931
elsedoneblock86855
	jsr UpdateCandy
	jsr CollisionCandy
	; Binary clause Simplified: LESSEQUAL
	lda lostchildlevel
	; Compare with pure num / var optimization
	cmp level;keep
	beq ConditionalTrueBlock50397
	bcs elsedoneblock31938
ConditionalTrueBlock50397
	jsr UpdateLostChild
elseblock91720
elsedoneblock31938
	; Binary clause Simplified: EQUALS
	lda witchHits
	; Compare with pure num / var optimization
	cmp #$6;keep
	bne elsedoneblock34114
ConditionalTrueBlock3798
	jsr BeatWitchWait
elseblock32008
elsedoneblock34114
	; Assigning single variable : i
	lda #14
	
	sta i
	; Binary clause Simplified: LESS
	lda moonPos
	; Compare with pure num / var optimization
	cmp #$6;keep
	bcs elsedoneblock70041
ConditionalTrueBlock11133
	; Assigning single variable : i
	lda #12
	
	sta i
elseblock22820
elsedoneblock70041
	; Binary clause Simplified: GREATER
	lda moonPos
	; Compare with pure num / var optimization
	cmp #$11;keep
	bcc elsedoneblock68485
	beq elsedoneblock68485
ConditionalTrueBlock94873
	; Assigning single variable : i
	lda #10
	
	sta i
elseblock46461
elsedoneblock68485
	; Assigning memory location
	; Assigning single variable : $900f
	lda i
	sta $900f
while10057
	; Binary clause Simplified: EQUALS
	lda ticks
	; Compare with pure num / var optimization
	cmp oldTick;keep
	bne elsedoneblock13264
ConditionalTrueBlock93889
	jmp while10057
elseblock11553
elsedoneblock13264
	; Assigning single variable : oldTick
	lda ticks
	sta oldTick
	jmp while93389
elseblock94128
elsedoneblock90934
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
while26704
	; Binary clause Simplified: GREATER
	lda t
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock94068
	beq elsedoneblock94068
ConditionalTrueBlock39316
	; Assigning single variable : t
	dec t
	; Assigning single variable : i
	lda #0
	sta i
for60953
while78552
	; Binary clause Simplified: EQUALS
	lda ticks
	; Compare with pure num / var optimization
	cmp oldTick;keep
	bne elsedoneblock75857
ConditionalTrueBlock82565
	jmp while78552
elseblock11183
elsedoneblock75857
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
	beq elsedoneblock61451
ConditionalTrueBlock19847
	; Assigning single variable : t
	lda #0
	sta t
	; Assigning single variable : i
	lda #29
	sta i
elseblock60355
elsedoneblock61451
	inc i
	lda #30
	cmp i ;keep
	bne for60953
forLoopDone80003
	jmp while26704
elseblock1287
elsedoneblock94068
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
while89802
	; Binary clause Simplified: GREATER
	lda t
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock20255
	beq elsedoneblock20255
ConditionalTrueBlock86250
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
	bne elseblock36860
ConditionalTrueBlock37523
	jsr DrawTitleFrame1
	jmp elsedoneblock95949
elseblock36860
	jsr DrawTitleFrame2
elsedoneblock95949
	; Assigning single variable : t
	dec t
	; Assigning single variable : i
	lda #0
	sta i
for16958
while1718
	; Binary clause Simplified: EQUALS
	lda ticks
	; Compare with pure num / var optimization
	cmp oldTick;keep
	bne elsedoneblock10744
ConditionalTrueBlock38010
	jmp while1718
elseblock47734
elsedoneblock10744
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
	beq elsedoneblock11548
ConditionalTrueBlock72484
	; Assigning single variable : t
	lda #0
	sta t
	; Assigning single variable : i
	lda #29
	sta i
elseblock98573
elsedoneblock11548
	inc i
	lda #30
	cmp i ;keep
	bne for16958
forLoopDone16259
	jmp while89802
elseblock21992
elsedoneblock20255
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
	bcc dtnooverflow22661
	iny  ; overflow into high byte
dtnooverflow22661
	
	sta screenmemory
	sty screenmemory+1
	; ----------
	; BcdPrint address, number
	ldy #$07 ; screen offset
	ldx #0 ; score byte index
bcdprintloop21751
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
	bpl bcdprintloop21751
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
	bcs elseblock47254
ConditionalTrueBlock65616
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
printdecimal11815
	jsr init_printdecimal_div10 
	ora #$30
	sta (screenmemory),y
	dey
	bpl printdecimal11815
	jmp elsedoneblock56574
elseblock47254
printstring_call57213
	clc
	lda #<printstring_text29886
	adc #0
	ldy #>printstring_text29886
	sta print_text+0
	sty print_text+1
	ldx #13 ; optimized, look out for bugs
	jsr printstring
elsedoneblock56574
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
for65342
	; Assigning single variable : screenmemory
	lda #39
	ldy i
	sta (screenmemory),y
	inc i
	lda n
	cmp i ;keep
	bne for65342
forLoopDone56982
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
while10641
	; Binary clause: GREATER
	lda t
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bcc binaryclausefailed9847
	beq binaryclausefailed9847
binaryclausesuccess41397
	lda #1; success
	jmp binaryclausefinished7369
binaryclausefailed9847
	lda #0 ; failed state
binaryclausefinished7369
	cmp #1
	beq ConditionalTrueBlock87223
	jmp elsedoneblock36337
ConditionalTrueBlock87223
	; Assigning single variable : t
	dec t
	; Binary clause: LESS
	lda x
	; Compare with pure num / var optimization
	cmp #$15;keep
	; BC done
	bcs binaryclausefailed83632
binaryclausesuccess99262
	lda #1; success
	jmp binaryclausefinished27482
binaryclausefailed83632
	lda #0 ; failed state
binaryclausefinished27482
	cmp #1
	beq ConditionalTrueBlock83380
	jmp elsedoneblock76075
ConditionalTrueBlock83380
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
	bcs elsedoneblock68612
ConditionalTrueBlock59171
	; Assigning single variable : y
	inc y
elseblock70015
elsedoneblock68612
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
	bne elsedoneblock52858
ConditionalTrueBlock89832
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
elseblock98442
elsedoneblock52858
	; Binary clause Simplified: EQUALS
	lda x
	; Compare with pure num / var optimization
	cmp #$15;keep
	bne elsedoneblock36618
ConditionalTrueBlock824
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
elseblock51044
elsedoneblock36618
elseblock33654
elsedoneblock76075
	; Binary clause: EQUALS
	lda x
	; Compare with pure num / var optimization
	cmp #$15;keep
	; BC done
	bne binaryclausefailed54394
binaryclausesuccess67701
	lda #1; success
	jmp binaryclausefinished74241
binaryclausefailed54394
	lda #0 ; failed state
binaryclausefinished74241
	cmp #1
	beq ConditionalTrueBlock57330
	jmp elsedoneblock15407
ConditionalTrueBlock57330
	; Binary clause: LESS
	lda n
	; Compare with pure num / var optimization
	cmp #$58;keep
	; BC done
	bcs binaryclausefailed96502
binaryclausesuccess94190
	lda #1; success
	jmp binaryclausefinished30876
binaryclausefailed96502
	lda #0 ; failed state
binaryclausefinished30876
	cmp #1
	beq ConditionalTrueBlock53801
	jmp elsedoneblock77669
ConditionalTrueBlock53801
	; Assigning single variable : pan
	inc pan
	; Binary clause Simplified: GREATER
	lda pan
	; Compare with pure num / var optimization
	cmp #$5;keep
	bcc elsedoneblock42399
	beq elsedoneblock42399
ConditionalTrueBlock66267
	; Assigning single variable : pan
	lda #0
	sta pan
elseblock93288
elsedoneblock42399
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
	bne elsedoneblock25556
ConditionalTrueBlock14067
	; Assigning single variable : s
	lda #61
	sta s
elseblock96391
elsedoneblock25556
	; Binary clause Simplified: EQUALS
	lda n
	; Compare with pure num / var optimization
	cmp #$58;keep
	bne elsedoneblock13148
ConditionalTrueBlock26144
	; Assigning single variable : screenmemory
	lda #0
	pha
	lda n
	lsr
	lsr
	
	tay
	pla
	sta (screenmemory),y
elseblock14057
elsedoneblock13148
	; Binary clause Simplified: EQUALS
	; Modulo
	lda #4
val_var89930 = $54
	sta val_var89930
	lda n
	sec
modulo74104
	sbc val_var89930
	bcs modulo74104
	adc val_var89930
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock1276
ConditionalTrueBlock98022
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
	bne elseblock93821
ConditionalTrueBlock16503
	; Assigning single variable : stepsnd
	lda #30
	sta stepsnd
	jmp elsedoneblock39906
elseblock93821
	; Assigning single variable : stepsnd
	lda #0
	sta stepsnd
elsedoneblock39906
elseblock32172
elsedoneblock1276
elseblock33911
elsedoneblock77669
elseblock19758
elsedoneblock15407
	; Assigning single variable : i
	lda #0
	sta i
for98840
while60763
	; Binary clause Simplified: EQUALS
	lda ticks
	; Compare with pure num / var optimization
	cmp oldTick;keep
	bne elsedoneblock75956
ConditionalTrueBlock88550
	jmp while60763
elseblock73861
elsedoneblock75956
	; Assigning single variable : oldTick
	lda ticks
	sta oldTick
	jsr callReadJoy1
	; Binary clause Simplified: LESS
	lda t
	; Compare with pure num / var optimization
	cmp #$3c;keep
	bcs elsedoneblock40634
ConditionalTrueBlock10231
	; Binary clause Simplified: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1
	and #16
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock55051
ConditionalTrueBlock2461
	; Assigning single variable : t
	lda #0
	sta t
elseblock2498
elsedoneblock55051
elseblock49866
elsedoneblock40634
	inc i
	lda #4
	cmp i ;keep
	bne for98840
forLoopDone47958
	jmp while10641
elseblock58907
elsedoneblock36337
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
val_var25516 = $54
	sta val_var25516
	; 8 bit binop
	; Add/sub where right value is constant number
	lda ticks
	clc
	adc #1
	 ; end add / sub var with constant
	sec
modulo72005
	sbc val_var25516
	bcs modulo72005
	adc val_var25516
	
	sta ticks
	; Binary clause Simplified: EQUALS
	lda sndFlag
	; Compare with pure num / var optimization
	cmp #$1;keep
	bne elsedoneblock61803
ConditionalTrueBlock14066
	jsr UpdateSound
elseblock62218
elsedoneblock61803
	; CloseIRQ
	pla
	tay
	pla
	tax
	pla
	 jmp $eabf     ; return to normal IRQ	
	rti
block73591
	jsr Initialise
	jsr SetupSound
	; Assigning single variable : level
	lda #0
	sta level
while47172
	; Binary clause: NOTEQUALS
	lda #1
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	beq binaryclausefailed77419
binaryclausesuccess86696
	lda #1; success
	jmp binaryclausefinished14343
binaryclausefailed77419
	lda #0 ; failed state
binaryclausefinished14343
	cmp #1
	beq ConditionalTrueBlock60615
	jmp elsedoneblock4158
ConditionalTrueBlock60615
	jsr BlankScreen
	lda level
	cmp #$0 ;keep
	bne casenext58247
	jsr TitleCredits
	jmp caseend32197
casenext58247
	lda level
	cmp #$1 ;keep
	bne casenext34622
	jsr TitleScreen
	jmp caseend32197
casenext34622
	lda level
	cmp #$2 ;keep
	bne casenext56105
	jsr LastScore
	jmp caseend32197
casenext56105
	lda level
	cmp #$3 ;keep
	bne casenext32459
	jsr TitleStory
	jmp caseend32197
casenext32459
	lda level
	cmp #$4 ;keep
	bne casenext27626
	jsr TitleStory2
	jmp caseend32197
casenext27626
	lda level
	cmp #$5 ;keep
	bne casenext24959
	jsr TitleCast
	jmp caseend32197
casenext24959
	lda level
	cmp #$6 ;keep
	bne casenext77002
	jsr TitleHowTo
	jmp caseend32197
casenext77002
caseend32197
	; Binary clause: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1
	and #16
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	beq binaryclausefailed58371
binaryclausesuccess1822
	lda #1; success
	jmp binaryclausefinished7913
binaryclausefailed58371
	lda #0 ; failed state
binaryclausefinished7913
	cmp #1
	beq ConditionalTrueBlock29412
	jmp elsedoneblock84903
ConditionalTrueBlock29412
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
val_var97786 = $54
	sta val_var97786
	jsr Random
	sec
modulo72460
	sbc val_var97786
	bcs modulo72460
	adc val_var97786
	
	clc
	adc #1
	 ; end add / sub var with constant
	
	sta lostchildlevel
while92613
	; Binary clause: GREATER
	lda lives
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bcc binaryclausefailed99132
	beq binaryclausefailed99132
binaryclausesuccess82211
	lda #1; success
	jmp binaryclausefinished85085
binaryclausefailed99132
	lda #0 ; failed state
binaryclausefinished85085
	cmp #1
	beq ConditionalTrueBlock85844
	jmp elsedoneblock3391
ConditionalTrueBlock85844
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
	bne elsedoneblock84287
ConditionalTrueBlock31585
	; Assigning single variable : lives
	dec lives
	; Binary clause Simplified: EQUALS
	lda lives
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elseblock45028
ConditionalTrueBlock18191
	jsr TitleGameOver
	jmp elsedoneblock8599
elseblock45028
	jsr TitleFail
elsedoneblock8599
elseblock41103
elsedoneblock84287
	; Binary clause Simplified: EQUALS
	lda gameMode
	; Compare with pure num / var optimization
	cmp #$2;keep
	bne elsedoneblock71917
ConditionalTrueBlock46649
	; Assigning single variable : level
	inc level
	; Binary clause Simplified: EQUALS
	lda level
	; Compare with pure num / var optimization
	cmp #$d;keep
	bne elseblock99810
ConditionalTrueBlock14198
	jsr TitleGameWon
	; Assigning single variable : lives
	lda #0
	sta lives
	jmp elsedoneblock37356
elseblock99810
	jsr TitleGameNext
	; Assigning single variable : witchHits
	lda #0
	sta witchHits
	; Binary clause Simplified: LESS
	lda lives
	; Compare with pure num / var optimization
	cmp #$5;keep
	bcs elsedoneblock56579
ConditionalTrueBlock20204
	; Binary clause Simplified: EQUALS
	lda level
	; Compare with pure num / var optimization
	cmp #$2;keep
	bne elsedoneblock4625
ConditionalTrueBlock84232
	; Assigning single variable : lives
	inc lives
elseblock53787
elsedoneblock4625
	; Binary clause Simplified: EQUALS
	lda level
	; Compare with pure num / var optimization
	cmp #$5;keep
	bne elsedoneblock77681
ConditionalTrueBlock89124
	; Assigning single variable : lives
	inc lives
elseblock94209
elsedoneblock77681
	; Binary clause Simplified: EQUALS
	lda level
	; Compare with pure num / var optimization
	cmp #$9;keep
	bne elsedoneblock44598
ConditionalTrueBlock50017
	; Assigning single variable : lives
	inc lives
elseblock27973
elsedoneblock44598
	; Binary clause Simplified: EQUALS
	lda level
	; Compare with pure num / var optimization
	cmp #$b;keep
	bne elsedoneblock85690
ConditionalTrueBlock35992
	; Assigning single variable : lives
	inc lives
elseblock19460
elsedoneblock85690
elseblock59025
elsedoneblock56579
elsedoneblock37356
elseblock64427
elsedoneblock71917
	jmp while92613
elseblock22640
elsedoneblock3391
	; Assigning single variable : level
	lda #0
	sta level
elseblock3489
elsedoneblock84903
	; Assigning single variable : level
	inc level
	; Binary clause Simplified: GREATER
	lda level
	; Compare with pure num / var optimization
	cmp #$6;keep
	bcc elsedoneblock30274
	beq elsedoneblock30274
ConditionalTrueBlock89734
	; Assigning single variable : level
	lda #0
	sta level
elseblock1171
elsedoneblock30274
	jmp while47172
elseblock91447
elsedoneblock4158
EndBlock10409
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
