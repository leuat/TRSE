 processor 6502

	ORG $1201
	.byte    $0E, $08, $0A, $00, $9E, $20, $28
	.byte   $34,$36,$32,$34
	.byte    $29, $00, $00, $00

	ORG $1210

Pumpkid
	jmp block41
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
printstring_text7672	dc.b	"@OUT@OF@TIME@@"
	dc.b	0
printstring_text12499	dc.b	"@GOT@BY@WITCH@"
	dc.b	0
printstring_text16600	dc.b	"POISONED@SKULL"
	dc.b	0
printstring_text1084	dc.b	"@GOT@BY@GHOST@"
	dc.b	0
printstring_text32618	dc.b	"@GOT@BY@ZOMBIE@"
	dc.b	0
printstring_text22129	dc.b	"THE@WITCH@RETREATS"
	dc.b	0
printstring_text17775	dc.b	"GET@READY@FOR"
	dc.b	0
 ; Temp vars section ends




































































































































































































































































































































































































































































































































EndSymbol
	org $1400
charset
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///pumpkid.bin"
	org $1600
colordata
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///colour.bin"
	org $1640
tcolordata
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///titlescol.bin"
	org $16b4
gameovercolordata
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///gameover-col.bin"
	org $16f4
gamewoncolordata
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///gamewon-col.bin"
	org $1734
gamefailcolordata
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///fail-col.bin"
	org $1774
gamenextcolordata
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///gamenext-col.bin"
	org $17b4
tcolorcrdata
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///credits-col.bin"
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
	BMI JoySkip18467
	ORA #$02
JoySkip18467
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
	bcc RandomSkip6334
	eor #$4d
RandomSkip6334
	sta Random+1
	eor $9124
	rts
	
	
	; ***********  Defining procedure : PlaySnd1
	;    Procedure type : User-defined procedure
	
vsnd1Time	dc.b	$00
vsnd1	dc.b	
block26500
PlaySnd1
	; Assigning single variable : vsnd1Time
	lda #0
	sta vsnd1Time
	rts
	
	
	; ***********  Defining procedure : PlaySnd2
	;    Procedure type : User-defined procedure
	
vsnd2Time	dc.b	$00
vsnd2	dc.b	
block19169
PlaySnd2
	; Assigning single variable : vsnd2Time
	lda #0
	sta vsnd2Time
	rts
	
	
	; ***********  Defining procedure : PlaySnd3
	;    Procedure type : User-defined procedure
	
vsnd3Time	dc.b	$00
vsnd3	dc.b	
block15724
PlaySnd3
	; Assigning single variable : vsnd3Time
	lda #0
	sta vsnd3Time
	rts
	
	
	; ***********  Defining procedure : PlaySndfx
	;    Procedure type : User-defined procedure
	
vsndfxTime	dc.b	$00
vsndfx	dc.b	
block11478
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
block29358
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
	bcc elsedoneblock16827
	beq elsedoneblock16827
ConditionalTrueBlock28145
	; Assigning single variable : vsnd1Time
	dec vsnd1Time
elseblock23281
elsedoneblock16827
	; Binary clause Simplified: EQUALS
	lda vsnd1Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock5436
ConditionalTrueBlock11942
	; Assigning single variable : psnd1
	; WORD optimization: a=a+b
	lda psnd1+0
	
	clc
	adc #2
	bcc WordAdd153
	inc psnd1+1
WordAdd153
	sta psnd1+0
elseblock4827
elsedoneblock5436
	rts
	
	
	; ***********  Defining procedure : Snd2Time
	;    Procedure type : User-defined procedure
	
Snd2Time
	; Binary clause Simplified: GREATER
	lda vsnd2Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock19718
	beq elsedoneblock19718
ConditionalTrueBlock17421
	; Assigning single variable : vsnd2Time
	dec vsnd2Time
elseblock18716
elsedoneblock19718
	; Binary clause Simplified: EQUALS
	lda vsnd2Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock1869
ConditionalTrueBlock14771
	; Assigning single variable : psnd2
	; WORD optimization: a=a+b
	lda psnd2+0
	
	clc
	adc #2
	bcc WordAdd17035
	inc psnd2+1
WordAdd17035
	sta psnd2+0
elseblock11538
elsedoneblock1869
	rts
	
	
	; ***********  Defining procedure : Snd3Time
	;    Procedure type : User-defined procedure
	
Snd3Time
	; Binary clause Simplified: GREATER
	lda vsnd3Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock30333
	beq elsedoneblock30333
ConditionalTrueBlock23811
	; Assigning single variable : vsnd3Time
	dec vsnd3Time
elseblock31322
elsedoneblock30333
	; Binary clause Simplified: EQUALS
	lda vsnd3Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock6868
ConditionalTrueBlock7711
	; Assigning single variable : psnd3
	; WORD optimization: a=a+b
	lda psnd3+0
	
	clc
	adc #2
	bcc WordAdd32757
	inc psnd3+1
WordAdd32757
	sta psnd3+0
elseblock28253
elsedoneblock6868
	rts
	
	
	; ***********  Defining procedure : SndfxTime
	;    Procedure type : User-defined procedure
	
SndfxTime
	; Binary clause Simplified: GREATER
	lda vsndfxTime
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock27529
	beq elsedoneblock27529
ConditionalTrueBlock8723
	; Assigning single variable : vsndfxTime
	dec vsndfxTime
elseblock9741
elsedoneblock27529
	; Binary clause Simplified: EQUALS
	lda vsndfxTime
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock288
ConditionalTrueBlock22190
	; Assigning single variable : psndfx
	; WORD optimization: a=a+b
	lda psndfx+0
	
	clc
	adc #2
	bcc WordAdd19264
	inc psndfx+1
WordAdd19264
	sta psndfx+0
elseblock1842
elsedoneblock288
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
	bne elsedoneblock6729
ConditionalTrueBlock23805
	; Binary clause Simplified: NOTEQUALS
	lda vsnd1
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock11840
ConditionalTrueBlock19954
	; Assigning single variable : vsnd1
	lda #0
	sta vsnd1
	; Poke
	; Optimization: shift is zero
	sta $900a
elseblock18756
elsedoneblock11840
	rts
elseblock15890
elsedoneblock6729
	; Binary clause: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd1),y
	
	; Compare with pure num / var optimization
	cmp #$20;keep
	; BC done
	bne tempfail18636
binaryclausesuccess24767
	jmp ConditionalTrueBlock26308
tempfail18636
	; Binary clause: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd1),y
	
	; Compare with pure num / var optimization
	cmp #$10;keep
	; BC done
	bne elseblock16944
binaryclausesuccess15574
ConditionalTrueBlock26308
	; Binary clause Simplified: EQUALS
	lda vsnd1Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock16941
ConditionalTrueBlock27350
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
	bne elsedoneblock6422
ConditionalTrueBlock24221
	; Poke
	; Optimization: shift is zero
	lda vsnd1
	sta $900a
elseblock18588
elsedoneblock6422
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd1),y
	
	; Compare with pure num / var optimization
	cmp #$10;keep
	bne elsedoneblock900
ConditionalTrueBlock16413
	; Poke
	; Optimization: shift is zero
	lda #0
	sta $900a
elseblock29168
elsedoneblock900
elseblock1150
elsedoneblock16941
	jsr Snd1Time
elseblock16944
elsedoneblock32439
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
	bne elsedoneblock27624
ConditionalTrueBlock17410
	; Binary clause Simplified: EQUALS
	lda vsnd1Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock4596
ConditionalTrueBlock9374
	; Assigning single variable : vsnd1Time
	; Load pointer array
	ldy #$1
	lda (psnd1),y
	
	sta vsnd1Time
elseblock11020
elsedoneblock4596
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
elseblock6359
elsedoneblock27624
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
	bne elsedoneblock8281
ConditionalTrueBlock19668
	; Binary clause Simplified: EQUALS
	lda vsnd1Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock22483
ConditionalTrueBlock14893
	; Assigning single variable : vsnd1Time
	; Load pointer array
	ldy #$1
	lda (psnd1),y
	
	sta vsnd1Time
elseblock24648
elsedoneblock22483
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
	
rightvarAddSub_var14310 = $54
	sta rightvarAddSub_var14310
	lda vsnd1
	sec
	sbc rightvarAddSub_var14310
	
	sta vsnd1
	; Poke
	; Optimization: shift is zero
	sta $900a
	jsr Snd1Time
elseblock24484
elsedoneblock8281
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
	bne elsedoneblock7616
ConditionalTrueBlock9514
	; Binary clause Simplified: NOTEQUALS
	lda vsnd2
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock32609
ConditionalTrueBlock11008
	; Assigning single variable : vsnd2
	lda #0
	sta vsnd2
	; Poke
	; Optimization: shift is zero
	sta $900b
elseblock5844
elsedoneblock32609
	rts
elseblock14309
elsedoneblock7616
	; Binary clause: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd2),y
	
	; Compare with pure num / var optimization
	cmp #$20;keep
	; BC done
	bne tempfail2634
binaryclausesuccess20055
	jmp ConditionalTrueBlock20485
tempfail2634
	; Binary clause: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd2),y
	
	; Compare with pure num / var optimization
	cmp #$10;keep
	; BC done
	bne elseblock3093
binaryclausesuccess22646
ConditionalTrueBlock20485
	; Binary clause Simplified: EQUALS
	lda vsnd2Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock29869
ConditionalTrueBlock18875
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
	bne elsedoneblock21003
ConditionalTrueBlock2600
	; Poke
	; Optimization: shift is zero
	lda vsnd2
	sta $900b
elseblock2510
elsedoneblock21003
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd2),y
	
	; Compare with pure num / var optimization
	cmp #$10;keep
	bne elsedoneblock15255
ConditionalTrueBlock13401
	; Poke
	; Optimization: shift is zero
	lda #0
	sta $900b
elseblock9789
elsedoneblock15255
elseblock28433
elsedoneblock29869
	jsr Snd2Time
elseblock3093
elsedoneblock14343
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
	bne elsedoneblock27088
ConditionalTrueBlock24182
	; Binary clause Simplified: EQUALS
	lda vsnd2Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock2368
ConditionalTrueBlock19976
	; Assigning single variable : vsnd2Time
	; Load pointer array
	ldy #$1
	lda (psnd2),y
	
	sta vsnd2Time
elseblock31329
elsedoneblock2368
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
elseblock10285
elsedoneblock27088
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
	bne elsedoneblock7441
ConditionalTrueBlock3434
	; Binary clause Simplified: EQUALS
	lda vsnd2Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock17437
ConditionalTrueBlock12529
	; Assigning single variable : vsnd2Time
	; Load pointer array
	ldy #$1
	lda (psnd2),y
	
	sta vsnd2Time
elseblock22549
elsedoneblock17437
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
	
rightvarAddSub_var193 = $54
	sta rightvarAddSub_var193
	lda vsnd2
	sec
	sbc rightvarAddSub_var193
	
	sta vsnd2
	; Poke
	; Optimization: shift is zero
	sta $900b
	jsr Snd2Time
elseblock16549
elsedoneblock7441
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
	bne elsedoneblock16105
ConditionalTrueBlock20416
	; Binary clause Simplified: NOTEQUALS
	lda vsnd3
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock24355
ConditionalTrueBlock12263
	; Assigning single variable : vsnd3
	lda #0
	sta vsnd3
	; Poke
	; Optimization: shift is zero
	sta $900c
elseblock4313
elsedoneblock24355
	rts
elseblock28286
elsedoneblock16105
	; Binary clause: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd3),y
	
	; Compare with pure num / var optimization
	cmp #$20;keep
	; BC done
	bne tempfail14018
binaryclausesuccess9905
	jmp ConditionalTrueBlock10808
tempfail14018
	; Binary clause: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd3),y
	
	; Compare with pure num / var optimization
	cmp #$10;keep
	; BC done
	bne elseblock1832
binaryclausesuccess7391
ConditionalTrueBlock10808
	; Binary clause Simplified: EQUALS
	lda vsnd3Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock9314
ConditionalTrueBlock26477
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
	bne elsedoneblock3102
ConditionalTrueBlock17192
	; Poke
	; Optimization: shift is zero
	lda vsnd3
	sta $900c
elseblock13985
elsedoneblock3102
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #$0
	lda (psnd3),y
	
	; Compare with pure num / var optimization
	cmp #$10;keep
	bne elsedoneblock30527
ConditionalTrueBlock4802
	; Poke
	; Optimization: shift is zero
	lda #0
	sta $900c
elseblock4099
elsedoneblock30527
elseblock4414
elsedoneblock9314
	jsr Snd3Time
elseblock1832
elsedoneblock20945
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
	bne elsedoneblock13061
ConditionalTrueBlock11023
	; Binary clause Simplified: EQUALS
	lda vsnd3Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock13064
ConditionalTrueBlock17222
	; Assigning single variable : vsnd3Time
	; Load pointer array
	ldy #$1
	lda (psnd3),y
	
	sta vsnd3Time
elseblock31286
elsedoneblock13064
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
elseblock29972
elsedoneblock13061
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
	bne elsedoneblock14270
ConditionalTrueBlock22413
	; Binary clause Simplified: EQUALS
	lda vsnd3Time
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock21624
ConditionalTrueBlock13694
	; Assigning single variable : vsnd3Time
	; Load pointer array
	ldy #$1
	lda (psnd3),y
	
	sta vsnd3Time
elseblock2695
elsedoneblock21624
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
	
rightvarAddSub_var26576 = $54
	sta rightvarAddSub_var26576
	lda vsnd3
	sec
	sbc rightvarAddSub_var26576
	
	sta vsnd3
	; Poke
	; Optimization: shift is zero
	sta $900c
	jsr Snd3Time
elseblock30974
elsedoneblock14270
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
	bne elsedoneblock22466
ConditionalTrueBlock26302
	; Binary clause Simplified: NOTEQUALS
	lda vsndfx
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock1926
ConditionalTrueBlock18087
	; Assigning single variable : vsndfx
	lda #0
	sta vsndfx
	; Poke
	; Optimization: shift is zero
	sta $900d
elseblock31060
elsedoneblock1926
	rts
elseblock17371
elsedoneblock22466
	; Binary clause: EQUALS
	; Load pointer array
	ldy #$0
	lda (psndfx),y
	
	; Compare with pure num / var optimization
	cmp #$20;keep
	; BC done
	bne tempfail30771
binaryclausesuccess25547
	jmp ConditionalTrueBlock20315
tempfail30771
	; Binary clause: EQUALS
	; Load pointer array
	ldy #$0
	lda (psndfx),y
	
	; Compare with pure num / var optimization
	cmp #$10;keep
	; BC done
	bne elseblock9576
binaryclausesuccess21520
ConditionalTrueBlock20315
	; Binary clause Simplified: EQUALS
	lda vsndfxTime
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock4940
ConditionalTrueBlock30188
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
	bne elsedoneblock6477
ConditionalTrueBlock19357
	; Poke
	; Optimization: shift is zero
	lda vsndfx
	sta $900d
elseblock2324
elsedoneblock6477
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #$0
	lda (psndfx),y
	
	; Compare with pure num / var optimization
	cmp #$10;keep
	bne elsedoneblock14460
ConditionalTrueBlock19801
	; Poke
	; Optimization: shift is zero
	lda #0
	sta $900d
elseblock22850
elsedoneblock14460
elseblock21763
elsedoneblock4940
	jsr SndfxTime
elseblock9576
elsedoneblock30227
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
	bne elsedoneblock31111
ConditionalTrueBlock19405
	; Binary clause Simplified: EQUALS
	lda vsndfxTime
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock8526
ConditionalTrueBlock1626
	; Assigning single variable : vsndfxTime
	; Load pointer array
	ldy #$1
	lda (psndfx),y
	
	sta vsndfxTime
elseblock9357
elsedoneblock8526
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
elseblock6540
elsedoneblock31111
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
	bne elsedoneblock12896
ConditionalTrueBlock23869
	; Binary clause Simplified: EQUALS
	lda vsndfxTime
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock8932
ConditionalTrueBlock4565
	; Assigning single variable : vsndfxTime
	; Load pointer array
	ldy #$1
	lda (psndfx),y
	
	sta vsndfxTime
elseblock6559
elsedoneblock8932
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
	
rightvarAddSub_var12053 = $54
	sta rightvarAddSub_var12053
	lda vsndfx
	sec
	sbc rightvarAddSub_var12053
	
	sta vsndfx
	; Poke
	; Optimization: shift is zero
	sta $900d
	jsr SndfxTime
elseblock29361
elsedoneblock12896
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
block3584
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
	bne elsedoneblock14369
ConditionalTrueBlock16972
	rts
elseblock21457
elsedoneblock14369
	; Binary clause Simplified: EQUALS
	lda musicStep
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock11635
ConditionalTrueBlock2483
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
	bne elsedoneblock12673
ConditionalTrueBlock18443
	; Assigning single variable : musicNote
	lda #0
	sta musicNote
	; Assigning single variable : musicRepeat
	inc musicRepeat
	; Binary clause Simplified: EQUALS
	lda musicRepeat
	; Compare with pure num / var optimization
	cmp #$1;keep
	bne elsedoneblock26740
ConditionalTrueBlock19314
	; Poke
	; Optimization: shift is zero
	lda #8
	sta $900e
elseblock18651
elsedoneblock26740
	; Binary clause Simplified: EQUALS
	lda musicRepeat
	; Compare with pure num / var optimization
	cmp #$2;keep
	bne elsedoneblock7605
ConditionalTrueBlock8759
	; Poke
	; Optimization: shift is zero
	lda #5
	sta $900e
elseblock11192
elsedoneblock7605
	; Binary clause Simplified: EQUALS
	lda musicRepeat
	; Compare with pure num / var optimization
	cmp #$3;keep
	bne elsedoneblock20608
ConditionalTrueBlock3829
	; Poke
	; Optimization: shift is zero
	lda #2
	sta $900e
elseblock23775
elsedoneblock20608
elseblock28296
elsedoneblock12673
elseblock911
elsedoneblock11635
	; Assigning single variable : musicStep
	inc musicStep
	; Binary clause Simplified: EQUALS
	lda musicStep
	; Compare with pure num / var optimization
	cmp #$13;keep
	bne elsedoneblock31627
ConditionalTrueBlock29556
	; Assigning single variable : psnd3
	lda #<noSound
	ldx #>noSound
	sta psnd3
	stx psnd3+1
elseblock25561
elsedoneblock31627
	; Binary clause Simplified: EQUALS
	lda musicStep
	; Compare with pure num / var optimization
	cmp #$14;keep
	bne elsedoneblock29174
ConditionalTrueBlock31240
	; Assigning single variable : musicStep
	lda #0
	sta musicStep
elseblock27813
elsedoneblock29174
	rts
	
	
	; ***********  Defining procedure : UpdateLostChild
	;    Procedure type : User-defined procedure
	
UpdateLostChild
	; Binary clause: EQUALS
	lda moonPos
	; Compare with pure num / var optimization
	cmp #$8;keep
	; BC done
	bne binaryclausefailed1650
binaryclausesuccess21634
	lda #1; success
	jmp binaryclausefinished5662
binaryclausefailed1650
	lda #0 ; failed state
binaryclausefinished5662
	cmp #1
	beq ConditionalTrueBlock8213
	jmp elsedoneblock25824
ConditionalTrueBlock8213
	; Binary clause: EQUALS
	lda lx
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bne binaryclausefailed20710
binaryclausesuccess24774
	lda #1; success
	jmp binaryclausefinished24484
binaryclausefailed20710
	lda #0 ; failed state
binaryclausefinished24484
	cmp #1
	beq ConditionalTrueBlock21416
	jmp elsedoneblock14008
ConditionalTrueBlock21416
	; Binary clause: LESS
	lda level
	; Compare with pure num / var optimization
	cmp #$c;keep
	; BC done
	bcs binaryclausefailed30080
binaryclausesuccess12249
	lda #1; success
	jmp binaryclausefinished10116
binaryclausefailed30080
	lda #0 ; failed state
binaryclausefinished10116
	cmp #1
	beq ConditionalTrueBlock25951
	jmp elsedoneblock18115
ConditionalTrueBlock25951
	; Assigning single variable : i
	lda #0
	sta i
while1528
	; Binary clause: EQUALS
	lda i
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bne binaryclausefailed6064
binaryclausesuccess20415
	lda #1; success
	jmp binaryclausefinished29309
binaryclausefailed6064
	lda #0 ; failed state
binaryclausefinished29309
	cmp #1
	beq ConditionalTrueBlock26679
	jmp elsedoneblock29421
ConditionalTrueBlock26679
	; Assigning single variable : lx
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #19
val_var29873 = $54
	sta val_var29873
	jsr Random
	sec
modulo17124
	sbc val_var29873
	bcs modulo17124
	adc val_var29873
	
	clc
	adc #1
	 ; end add / sub var with constant
	
	sta lx
	; Assigning single variable : ly
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #18
val_var23941 = $54
	sta val_var23941
	jsr Random
	sec
modulo32745
	sbc val_var23941
	bcs modulo32745
	adc val_var23941
	
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
	bne elsedoneblock12268
ConditionalTrueBlock27531
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
elseblock4806
elsedoneblock12268
	jmp while1528
elseblock7864
elsedoneblock29421
elseblock6541
elsedoneblock18115
elseblock13452
elsedoneblock14008
elseblock23992
elsedoneblock25824
	; Binary clause Simplified: EQUALS
	lda moonPos
	; Compare with pure num / var optimization
	cmp #$a;keep
	bne elsedoneblock26630
ConditionalTrueBlock1012
	; Binary clause Simplified: NOTEQUALS
	lda lx
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock7958
ConditionalTrueBlock30114
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
val_var18238 = $54
	sta val_var18238
	jsr Random
	sec
modulo26423
	sbc val_var18238
	bcs modulo26423
	adc val_var18238
	
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
elseblock16439
elsedoneblock7958
elseblock21136
elsedoneblock26630
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
	bne elseblock32498
ConditionalTrueBlock17659
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
	jmp elsedoneblock28486
elseblock32498
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
elsedoneblock28486
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
dtloop12158
	tay
	lda scr,x
	inx
	inx
	clc
	adc #$16
	bcc dtnooverflow12022
	iny
dtnooverflow12022
	sta scr,x
	tya
	sta scr+1,x
	cpx #$2c
	bcc dtloop12158
	; ----------
	; DefineAddressTable address, StartValue, IncrementValue, TableSize
	ldy #>$9400
	lda #<$9400
	ldx #0
	sta clr,x   ; Address of table
	tya
	sta clr+1,x
dtloop1146
	tay
	lda clr,x
	inx
	inx
	clc
	adc #$16
	bcc dtnooverflow18392
	iny
dtnooverflow18392
	sta clr,x
	tya
	sta clr+1,x
	cpx #$2c
	bcc dtloop1146
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
bcdprintloop3925
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
	bpl bcdprintloop3925
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
for29458
	; Assigning single variable : j
	lda #0
	sta j
for20448
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
	bne for20448
forLoopDone540
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd4575
	inc p1+1
WordAdd4575
	sta p1+0
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd8762
	inc screenmemory+1
WordAdd8762
	sta screenmemory+0
	inc i
	lda #23
	cmp i ;keep
	bne for29458
forLoopDone20799
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
for22336
	; Assigning single variable : j
	lda #0
	sta j
for658
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
	bne for658
forLoopDone24520
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd29560
	inc p1+1
WordAdd29560
	sta p1+0
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd15147
	inc screenmemory+1
WordAdd15147
	sta screenmemory+0
	inc i
	lda #23
	cmp i ;keep
	bne for22336
forLoopDone4449
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
for1655
	; Assigning single variable : j
	lda #0
	sta j
for19569
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
	bne for19569
forLoopDone27663
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd24515
	inc p1+1
WordAdd24515
	sta p1+0
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd11521
	inc screenmemory+1
WordAdd11521
	sta screenmemory+0
	inc i
	lda #23
	cmp i ;keep
	bne for1655
forLoopDone26946
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
for15615
	; Assigning single variable : j
	lda #0
	sta j
for12549
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
	bne for12549
forLoopDone330
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd28350
	inc p1+1
WordAdd28350
	sta p1+0
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd14333
	inc screenmemory+1
WordAdd14333
	sta screenmemory+0
	inc i
	lda #23
	cmp i ;keep
	bne for15615
forLoopDone14161
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
for10910
	; Assigning single variable : j
	lda #0
	sta j
for19949
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
	bne for19949
forLoopDone2505
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd10004
	inc p1+1
WordAdd10004
	sta p1+0
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd16337
	inc screenmemory+1
WordAdd16337
	sta screenmemory+0
	inc i
	lda #23
	cmp i ;keep
	bne for10910
forLoopDone28164
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
for28664
	; Assigning single variable : j
	lda #0
	sta j
for9876
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
	bne for9876
forLoopDone28396
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd6249
	inc p1+1
WordAdd6249
	sta p1+0
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd14640
	inc screenmemory+1
WordAdd14640
	sta screenmemory+0
	inc i
	lda #23
	cmp i ;keep
	bne for28664
forLoopDone28652
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
for18819
	; Assigning single variable : j
	lda #0
	sta j
for31450
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
	bne for31450
forLoopDone2609
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd9166
	inc p1+1
WordAdd9166
	sta p1+0
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd16893
	inc screenmemory+1
WordAdd16893
	sta screenmemory+0
	inc i
	lda #23
	cmp i ;keep
	bne for18819
forLoopDone7585
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
for13509
	; Assigning single variable : j
	lda #0
	sta j
for26031
	; Assigning single variable : p1
	lda #0
	ldy j
	sta (p1),y
	inc j
	lda #22
	cmp j ;keep
	bne for26031
forLoopDone19426
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd10422
	inc p1+1
WordAdd10422
	sta p1+0
	inc i
	lda #23
	cmp i ;keep
	bne for13509
forLoopDone17968
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
for8779
	; Assigning single variable : j
	lda #0
	sta j
for28255
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
	bne for28255
forLoopDone474
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #22
	bcc WordAdd26821
	inc p1+1
WordAdd26821
	sta p1+0
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd26122
	inc screenmemory+1
WordAdd26122
	sta screenmemory+0
	inc i
	lda #20
	cmp i ;keep
	bne for8779
forLoopDone1364
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
for21577
	; Assigning single variable : screenmemory
	lda #3
	ldy i
	sta (screenmemory),y
	inc i
	lda #22
	cmp i ;keep
	bne for21577
forLoopDone25605
	; Assigning single variable : i
	lda #22
	sta i
for27594
	; Assigning single variable : screenmemory
	lda #0
	ldy i
	sta (screenmemory),y
	inc i
	lda #66
	cmp i ;keep
	bne for27594
forLoopDone16343
	; Assigning single variable : screenmemory
	lda #8
	ldx #148
	sta screenmemory
	stx screenmemory+1
	; Assigning single variable : i
	lda #0
	sta i
for2481
	; Assigning single variable : screenmemory
	lda #2
	ldy i
	sta (screenmemory),y
	inc i
	lda #8
	cmp i ;keep
	bne for2481
forLoopDone11672
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
for9762
	; Assigning single variable : x
	lda #1
	sta x
for14776
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy x
	lda (screenmemory),y
	
	; Compare with pure num / var optimization
	cmp #$3a;keep
	bne elsedoneblock16828
ConditionalTrueBlock32196
	; Assigning single variable : px
	lda x
	sta px
	; Assigning single variable : py
	lda y
	sta py
elseblock7783
elsedoneblock16828
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy x
	lda (screenmemory),y
	
	; Compare with pure num / var optimization
	cmp #$20;keep
	bne elsedoneblock18703
ConditionalTrueBlock10697
	; Assigning single variable : wx
	lda x
	sta wx
	; Assigning single variable : wy
	lda y
	sta wy
elseblock17612
elsedoneblock18703
	inc x
	lda #21
	cmp x ;keep
	bne for14776
forLoopDone22668
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #22
	bcc WordAdd5545
	inc screenmemory+1
WordAdd5545
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
	bne for9762
forLoopDone17982
	rts
	
	
	; ***********  Defining procedure : WitchPickDirection
	;    Procedure type : User-defined procedure
	
WitchPickDirection
	; Binary clause Simplified: NOTEQUALS
	; Modulo
	lda #3
val_var1670 = $54
	sta val_var1670
	jsr Random
	sec
modulo32528
	sbc val_var1670
	bcs modulo32528
	adc val_var1670
	
	; Compare with pure num / var optimization
	cmp #$1;keep
	beq elsedoneblock26443
ConditionalTrueBlock30238
	; Binary clause Simplified: EQUALS
	lda wy
	; Compare with pure num / var optimization
	cmp py;keep
	bne elsedoneblock25783
ConditionalTrueBlock7213
	; Binary clause Simplified: GREATER
	lda wx
	; Compare with pure num / var optimization
	cmp px;keep
	bcc elseblock10938
	beq elseblock10938
ConditionalTrueBlock24865
	; Assigning single variable : wd
	lda #3
	sta wd
	jmp elsedoneblock1881
elseblock10938
	; Assigning single variable : wd
	lda #1
	sta wd
elsedoneblock1881
	rts
elseblock9860
elsedoneblock25783
	; Binary clause Simplified: EQUALS
	lda wx
	; Compare with pure num / var optimization
	cmp px;keep
	bne elsedoneblock26063
ConditionalTrueBlock28458
	; Binary clause Simplified: GREATER
	lda wy
	; Compare with pure num / var optimization
	cmp py;keep
	bcc elseblock13330
	beq elseblock13330
ConditionalTrueBlock8360
	; Assigning single variable : wd
	lda #0
	sta wd
	jmp elsedoneblock26048
elseblock13330
	; Assigning single variable : wd
	lda #2
	sta wd
elsedoneblock26048
	rts
elseblock2661
elsedoneblock26063
elseblock24237
elsedoneblock26443
	; Assigning single variable : wd
	inc wd
	; Binary clause Simplified: GREATER
	lda wd
	; Compare with pure num / var optimization
	cmp #$3;keep
	bcc elsedoneblock19810
	beq elsedoneblock19810
ConditionalTrueBlock6766
	; Assigning single variable : wd
	lda #0
	sta wd
elseblock1735
elsedoneblock19810
	rts
	
	
	; ***********  Defining procedure : UpdateWitch
	;    Procedure type : User-defined procedure
	
UpdateWitch
	; Binary clause: EQUALS
	lda wsteps
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bne binaryclausefailed5538
binaryclausesuccess6193
	lda #1; success
	jmp binaryclausefinished7492
binaryclausefailed5538
	lda #0 ; failed state
binaryclausefinished7492
	cmp #1
	beq ConditionalTrueBlock7328
	jmp elsedoneblock11369
ConditionalTrueBlock7328
	; Binary clause Simplified: EQUALS
	lda wdelay
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock1840
ConditionalTrueBlock1560
	; Assigning single variable : wdelay
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #40
val_var18805 = $54
	sta val_var18805
	jsr Random
	sec
modulo28791
	sbc val_var18805
	bcs modulo28791
	adc val_var18805
	
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
elseblock15834
elsedoneblock1840
	; Assigning single variable : wdelay
	dec wdelay
	; Binary clause Simplified: GREATER
	lda wdelay
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock21578
	beq elsedoneblock21578
ConditionalTrueBlock13210
	rts
elseblock13549
elsedoneblock21578
	jsr WitchPickDirection
	; Assigning single variable : wsteps
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #10
val_var9277 = $54
	sta val_var9277
	jsr Random
	sec
modulo73
	sbc val_var9277
	bcs modulo73
	adc val_var9277
	
	clc
	adc #2
	 ; end add / sub var with constant
	
	sta wsteps
	; Binary clause Simplified: GREATER
	lda wdcount
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock13826
	beq elsedoneblock13826
ConditionalTrueBlock1620
	; Assigning single variable : wdcount
	dec wdcount
	; Binary clause Simplified: EQUALS
	lda wdcount
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock10380
ConditionalTrueBlock12924
	; Assigning single variable : wdspeed
	; Load Byte array
	ldx level
	lda lwitch,x
	
	sta wdspeed
elseblock9072
elsedoneblock10380
elseblock21497
elsedoneblock13826
elseblock29352
elsedoneblock11369
	; Binary clause Simplified: NOTEQUALS
	; Modulo
	lda wdspeed
val_var14196 = $54
	sta val_var14196
	lda ticks
	sec
modulo12074
	sbc val_var14196
	bcs modulo12074
	adc val_var14196
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock15370
ConditionalTrueBlock6477
	rts
elseblock18503
elsedoneblock15370
	; Assigning single variable : wsteps
	dec wsteps
	lda wd
	cmp #$0 ;keep
	bne casenext19019
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
	bcs elseblock13716
ConditionalTrueBlock16683
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
	jmp elsedoneblock19932
elseblock13716
	; Assigning single variable : wsteps
	lda #0
	sta wsteps
elsedoneblock19932
	jmp caseend12611
casenext19019
	lda wd
	cmp #$1 ;keep
	bne casenext6813
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
	bcs elseblock2599
ConditionalTrueBlock10904
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
	jmp elsedoneblock20136
elseblock2599
	; Assigning single variable : wsteps
	lda #0
	sta wsteps
elsedoneblock20136
	jmp caseend12611
casenext6813
	lda wd
	cmp #$2 ;keep
	bne casenext14387
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
	bcs elseblock241
ConditionalTrueBlock8670
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
	jmp elsedoneblock18882
elseblock241
	; Assigning single variable : wsteps
	lda #0
	sta wsteps
elsedoneblock18882
	jmp caseend12611
casenext14387
	lda wd
	cmp #$3 ;keep
	bne casenext12105
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
	bcc dtnooverflow17095
	iny  ; overflow into high byte
dtnooverflow17095
	
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
	bcc dtnooverflow7296
	iny  ; overflow into high byte
dtnooverflow7296
	
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
	bcs elseblock10178
ConditionalTrueBlock15678
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
	jmp elsedoneblock13579
elseblock10178
	; Assigning single variable : wsteps
	lda #0
	sta wsteps
elsedoneblock13579
	jmp caseend12611
casenext12105
caseend12611
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
	bne elsedoneblock24676
ConditionalTrueBlock2995
	; Binary clause Simplified: EQUALS
	lda cy
	; Compare with pure num / var optimization
	cmp wy;keep
	bne elsedoneblock5055
ConditionalTrueBlock7514
	jsr HitWitch
elseblock10168
elsedoneblock5055
elseblock2678
elsedoneblock24676
	rts
	
	
	; ***********  Defining procedure : Ghost_Respawn
	;    Procedure type : User-defined procedure
	
Ghost_Respawn
	; Binary clause Simplified: GREATEREQUAL
	lda gcurrent
	; Compare with pure num / var optimization
	cmp gNumGhosts;keep
	bcc elsedoneblock2144
ConditionalTrueBlock5651
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
elseblock10986
elsedoneblock2144
	; Assigning single variable : j
	lda #0
	sta j
while26517
	; Binary clause: EQUALS
	lda j
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bne binaryclausefailed4611
binaryclausesuccess2445
	lda #1; success
	jmp binaryclausefinished31947
binaryclausefailed4611
	lda #0 ; failed state
binaryclausefinished31947
	cmp #1
	beq ConditionalTrueBlock14629
	jmp elsedoneblock5874
ConditionalTrueBlock14629
	; Assigning single variable : x
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #20
val_var29975 = $54
	sta val_var29975
	jsr Random
	sec
modulo13489
	sbc val_var29975
	bcs modulo13489
	adc val_var29975
	
	clc
	adc #1
	 ; end add / sub var with constant
	
	sta x
	; Assigning single variable : y
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #18
val_var24750 = $54
	sta val_var24750
	jsr Random
	sec
modulo6149
	sbc val_var24750
	bcs modulo6149
	adc val_var24750
	
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
	bcc dtnooverflow13865
	iny  ; overflow into high byte
dtnooverflow13865
	
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
	bne elsedoneblock27432
ConditionalTrueBlock17282
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
	bcs elseblock19447
ConditionalTrueBlock31351
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
	jmp elsedoneblock24208
elseblock19447
	; Assigning single variable : gmode
	lda #1
	ldx gcurrent
	sta gmode,x
	; Assigning single variable : gx
	lda #0
	sta gx,x
elsedoneblock24208
elseblock27007
elsedoneblock27432
	jmp while26517
elseblock29916
elsedoneblock5874
	rts
	
	
	; ***********  Defining procedure : PlaceAllGhosts
	;    Procedure type : User-defined procedure
	
PlaceAllGhosts
	; Assigning single variable : gcurrent
	lda #0
	sta gcurrent
for30422
	jsr Ghost_Respawn
	; Binary clause Simplified: LESS
	lda gcurrent
	; Compare with pure num / var optimization
	cmp gNumGhosts;keep
	bcs elsedoneblock24961
ConditionalTrueBlock6450
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
for2529
while30032
	; Binary clause Simplified: EQUALS
	lda ticks
	; Compare with pure num / var optimization
	cmp oldTick;keep
	bne elsedoneblock23165
ConditionalTrueBlock3989
	jmp while30032
elseblock9320
elsedoneblock23165
	; Assigning single variable : oldTick
	lda ticks
	sta oldTick
	inc i
	lda #30
	cmp i ;keep
	bne for2529
forLoopDone8805
elseblock14801
elsedoneblock24961
	inc gcurrent
	lda #10
	cmp gcurrent ;keep
	bne for30422
forLoopDone28680
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
for17206
forLoopFix30109
	; Binary clause Simplified: GREATER
	; Load Byte array
	ldx i
	lda gy,x
	
	; Compare with pure num / var optimization
	cmp #$3;keep
	bcc elsedoneblock3684
	beq elsedoneblock3684
ConditionalTrueBlock4015
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
elseblock16626
elsedoneblock3684
	inc i
	lda gNumGhosts
	cmp i ;keep
	beq forLoopDone17984
forLoopNotDone11969
	jmp for17206
forLoopDone17984
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
	bcs ColXConfirmed12259
	cmp #$8
	bcs NoCollision26653
ColXConfirmed12259
	lda y
	clc
	sbc py
	cmp #$fa
	bcs Collision27936
	cmp #$5
	bcs NoCollision26653
Collision27936
	lda #1
	jmp CollisionDone2095
NoCollision26653
	lda #0
CollisionDone2095
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	beq binaryclausefailed24365
binaryclausesuccess7720
	lda #1; success
	jmp binaryclausefinished11874
binaryclausefailed24365
	lda #0 ; failed state
binaryclausefinished11874
	cmp #1
	beq ConditionalTrueBlock12420
	jmp elseblock20710
	jmp elsedoneblock18271
ConditionalTrueBlock12420
	; Binary clause Simplified: EQUALS
	; Load Byte array
	ldx gcurrent
	lda gmode,x
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock5725
ConditionalTrueBlock8976
	; Binary clause Simplified: GREATER
	lda x
	; Compare with pure num / var optimization
	cmp px;keep
	bcc elsedoneblock10080
	beq elsedoneblock10080
ConditionalTrueBlock31202
	; Assigning single variable : x
	dec x
elseblock10508
elsedoneblock10080
	; Binary clause Simplified: LESS
	lda x
	; Compare with pure num / var optimization
	cmp px;keep
	bcs elsedoneblock8546
ConditionalTrueBlock31493
	; Assigning single variable : x
	inc x
elseblock7740
elsedoneblock8546
elseblock18455
elsedoneblock5725
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
	bcc dtnooverflow19097
	iny  ; overflow into high byte
dtnooverflow19097
	
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
	bcc elsedoneblock23400
	beq elsedoneblock23400
ConditionalTrueBlock23335
	; Assigning single variable : x
	lda n
	sta x
elseblock11072
elsedoneblock23400
	; Binary clause Simplified: EQUALS
	; Load Byte array
	ldx gcurrent
	lda gmode,x
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock12481
ConditionalTrueBlock4141
	; Binary clause Simplified: GREATER
	lda y
	; Compare with pure num / var optimization
	cmp py;keep
	bcc elsedoneblock23717
	beq elsedoneblock23717
ConditionalTrueBlock26018
	; Assigning single variable : y
	dec y
elseblock27368
elsedoneblock23717
	; Binary clause Simplified: LESS
	lda y
	; Compare with pure num / var optimization
	cmp py;keep
	bcs elsedoneblock3169
ConditionalTrueBlock3335
	; Assigning single variable : y
	inc y
elseblock12759
elsedoneblock3169
elseblock23588
elsedoneblock12481
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
	bcc dtnooverflow22640
	iny  ; overflow into high byte
dtnooverflow22640
	
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
	bcc elsedoneblock24791
	beq elsedoneblock24791
ConditionalTrueBlock24199
	; Assigning single variable : y
	lda s
	sta y
elseblock29105
elsedoneblock24791
	jmp elsedoneblock18271
elseblock20710
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
	beq elseblock32029
ConditionalTrueBlock24033
	; Assigning single variable : x
	inc x
	jmp elsedoneblock15987
elseblock32029
	; Assigning single variable : x
	dec x
elsedoneblock15987
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
	bcc dtnooverflow11420
	iny  ; overflow into high byte
dtnooverflow11420
	
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
	bcc elsedoneblock31063
	beq elsedoneblock31063
ConditionalTrueBlock32718
	; Assigning single variable : x
	lda n
	sta x
elseblock12244
elsedoneblock31063
	; Binary clause Simplified: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda i
	and #2
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elseblock30470
ConditionalTrueBlock4769
	; Assigning single variable : y
	dec y
	jmp elsedoneblock15005
elseblock30470
	; Assigning single variable : y
	inc y
elsedoneblock15005
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
	bcc dtnooverflow24326
	iny  ; overflow into high byte
dtnooverflow24326
	
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
	bcc elsedoneblock23990
	beq elsedoneblock23990
ConditionalTrueBlock21323
	; Assigning single variable : y
	lda s
	sta y
elseblock7679
elsedoneblock23990
elsedoneblock18271
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
binary_clause_temp_var2088 = $54
	sta binary_clause_temp_var2088
	; Load Byte array
	ldx gcurrent
	lda gx,x
	
binary_clause_temp_2_var12935 = $56
	sta binary_clause_temp_2_var12935
	lda binary_clause_temp_var2088
	cmp binary_clause_temp_2_var12935;keep
	bne elsedoneblock23589
ConditionalTrueBlock28470
	; Binary clause Simplified: EQUALS
	lda cy
binary_clause_temp_var19105 = $54
	sta binary_clause_temp_var19105
	; Load Byte array
	ldx gcurrent
	lda gy,x
	
binary_clause_temp_2_var11807 = $56
	sta binary_clause_temp_2_var11807
	lda binary_clause_temp_var19105
	cmp binary_clause_temp_2_var11807;keep
	bne elsedoneblock18965
ConditionalTrueBlock14790
	; Assigning single variable : i
	lda gcurrent
	sta i
	jsr HitGhost
elseblock24962
elsedoneblock18965
elseblock20183
elsedoneblock23589
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
	bcc binaryclausefailed18805
	beq binaryclausefailed18805
binaryclausesuccess5750
	lda #1; success
	jmp binaryclausefinished8616
binaryclausefailed18805
	lda #0 ; failed state
binaryclausefinished8616
	cmp #1
	beq ConditionalTrueBlock32671
	jmp elsedoneblock12998
ConditionalTrueBlock32671
	; Assigning single variable : gtime
	lda #0
	sta gtime
	; Binary clause Simplified: LESS
	lda gcurrent
	; Compare with pure num / var optimization
	cmp gNumGhosts;keep
	bcs elsedoneblock14697
ConditionalTrueBlock21963
	; Binary clause Simplified: EQUALS
	; Load Byte array
	ldx gcurrent
	lda gmode,x
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock18028
ConditionalTrueBlock24118
	jsr Ghost_Move
elseblock12737
elsedoneblock18028
	; Binary clause Simplified: GREATER
	; Load Byte array
	ldx gcurrent
	lda gmode,x
	
	; Compare with pure num / var optimization
	cmp #$1;keep
	bcc elsedoneblock22795
	beq elsedoneblock22795
ConditionalTrueBlock14091
	; Assigning single variable : gmode
	ldx gcurrent
	; Optimize byte array dec 
	dec gmode,x
	; Binary clause Simplified: EQUALS
	; Load Byte array
	lda gmode,x
	
	; Compare with pure num / var optimization
	cmp #$1;keep
	bne elsedoneblock26790
ConditionalTrueBlock28074
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
elseblock8719
elsedoneblock26790
elseblock20556
elsedoneblock22795
elseblock28135
elsedoneblock14697
	jsr DisplayGhosts
	; Assigning single variable : gcurrent
	inc gcurrent
	; Binary clause Simplified: EQUALS
	lda gcurrent
	; Compare with pure num / var optimization
	cmp #$a;keep
	bne elsedoneblock22974
ConditionalTrueBlock8329
	; Assigning single variable : gcurrent
	lda #0
	sta gcurrent
elseblock16290
elsedoneblock22974
elseblock1457
elsedoneblock12998
	rts
	
	
	; ***********  Defining procedure : SpawnAZombie
	;    Procedure type : User-defined procedure
	
SpawnAZombie
	; Binary clause Simplified: GREATEREQUAL
	lda zNumZombies
	; Compare with pure num / var optimization
	cmp #$a;keep
	bcc elsedoneblock14893
ConditionalTrueBlock31490
	rts
elseblock3239
elsedoneblock14893
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
val_var10681 = $54
	sta val_var10681
	jsr Random
	sec
modulo25903
	sbc val_var10681
	bcs modulo25903
	adc val_var10681
	
	sta t
	; Assigning single variable : y
	lda #3
	sta y
for27005
	; Assigning single variable : x
	lda #1
	sta x
for13874
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
	bne elsedoneblock9458
ConditionalTrueBlock1570
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
	bne elsedoneblock30591
ConditionalTrueBlock31495
	; Assigning single variable : x
	lda #20
	sta x
	; Assigning single variable : y
	lda #21
	sta y
elseblock8044
elsedoneblock30591
	; Assigning single variable : i
	inc i
elseblock8636
elsedoneblock9458
	inc x
	lda #21
	cmp x ;keep
	bne for13874
forLoopDone17226
	inc y
	lda #22
	cmp y ;keep
	bne for27005
forLoopDone27189
	; Binary clause Simplified: GREATER
	lda n
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock6272
	beq elsedoneblock6272
ConditionalTrueBlock7450
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
elseblock12319
elsedoneblock6272
	rts
	
	
	; ***********  Defining procedure : DisplayZombies
	;    Procedure type : User-defined procedure
	
DisplayZombies
	; Assigning single variable : i
	lda #0
	sta i
for1775
forLoopFix10722
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
	bne casenext19047
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
	jmp caseend23258
casenext19047
	lda t
	cmp #$1 ;keep
	bne casenext9658
	; Assigning single variable : screenmemory
	lda #40
	ldy x
	sta (screenmemory),y
	jmp caseend23258
casenext9658
	lda t
	cmp #$2 ;keep
	bne casenext25302
	; Assigning single variable : screenmemory
	lda #41
	ldy x
	sta (screenmemory),y
	jmp caseend23258
casenext25302
	lda t
	cmp #$3 ;keep
	bne casenext18990
	; Assigning single variable : screenmemory
	lda #42
	ldy x
	sta (screenmemory),y
	jmp caseend23258
casenext18990
	; Assigning single variable : screenmemory
	lda #42
	ldy x
	sta (screenmemory),y
caseend23258
	; Assigning single variable : p1
	lda #5
	ldy x
	sta (p1),y
	inc i
	lda zNumZombies
	cmp i ;keep
	beq forLoopDone24271
forLoopNotDone19611
	jmp for1775
forLoopDone24271
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
	beq binaryclausesuccess10498
	bcs binaryclausefailed5869
binaryclausesuccess10498
	lda #1; success
	jmp binaryclausefinished14398
binaryclausefailed5869
	lda #0 ; failed state
binaryclausefinished14398
	cmp #1
	beq ConditionalTrueBlock12361
	jmp elsedoneblock22194
ConditionalTrueBlock12361
	; Binary clause Simplified: GREATER
	lda x
	; Compare with pure num / var optimization
	cmp px;keep
	bcc elsedoneblock13043
	beq elsedoneblock13043
ConditionalTrueBlock25679
	; Assigning single variable : x
	dec x
elseblock28053
elsedoneblock13043
	; Binary clause Simplified: LESS
	lda x
	; Compare with pure num / var optimization
	cmp px;keep
	bcs elsedoneblock17022
ConditionalTrueBlock25834
	; Assigning single variable : x
	inc x
elseblock9850
elsedoneblock17022
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
	bcc dtnooverflow9492
	iny  ; overflow into high byte
dtnooverflow9492
	
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
	bcc elsedoneblock27616
	beq elsedoneblock27616
ConditionalTrueBlock28580
	; Assigning single variable : x
	lda n
	sta x
elseblock15477
elsedoneblock27616
	; Binary clause Simplified: GREATER
	lda y
	; Compare with pure num / var optimization
	cmp py;keep
	bcc elsedoneblock26798
	beq elsedoneblock26798
ConditionalTrueBlock14615
	; Assigning single variable : y
	dec y
elseblock22348
elsedoneblock26798
	; Binary clause Simplified: LESS
	lda y
	; Compare with pure num / var optimization
	cmp py;keep
	bcs elsedoneblock18902
ConditionalTrueBlock2883
	; Assigning single variable : y
	inc y
elseblock1662
elsedoneblock18902
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
	bcc dtnooverflow19770
	iny  ; overflow into high byte
dtnooverflow19770
	
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
	bcc elsedoneblock12686
	beq elsedoneblock12686
ConditionalTrueBlock9273
	; Assigning single variable : y
	lda s
	sta y
elseblock25841
elsedoneblock12686
elseblock16742
elsedoneblock22194
	; Binary clause: EQUALS
	lda t
	; Compare with pure num / var optimization
	cmp #$2;keep
	; BC done
	bne binaryclausefailed23893
binaryclausesuccess20274
	lda #1; success
	jmp binaryclausefinished22630
binaryclausefailed23893
	lda #0 ; failed state
binaryclausefinished22630
	cmp #1
	beq ConditionalTrueBlock1892
	jmp elsedoneblock11267
ConditionalTrueBlock1892
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
	beq elseblock8290
ConditionalTrueBlock17461
	; Assigning single variable : x
	inc x
	jmp elsedoneblock19662
elseblock8290
	; Assigning single variable : x
	dec x
elsedoneblock19662
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
	bcc dtnooverflow18423
	iny  ; overflow into high byte
dtnooverflow18423
	
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
	bcc elsedoneblock28703
	beq elsedoneblock28703
ConditionalTrueBlock16717
	; Assigning single variable : x
	lda n
	sta x
elseblock6640
elsedoneblock28703
	; Binary clause Simplified: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda i
	and #2
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elseblock18245
ConditionalTrueBlock22659
	; Assigning single variable : y
	dec y
	jmp elsedoneblock20386
elseblock18245
	; Assigning single variable : y
	inc y
elsedoneblock20386
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
	bcc dtnooverflow16840
	iny  ; overflow into high byte
dtnooverflow16840
	
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
	bcc elsedoneblock29250
	beq elsedoneblock29250
ConditionalTrueBlock1497
	; Assigning single variable : y
	lda s
	sta y
elseblock27283
elsedoneblock29250
elseblock13698
elsedoneblock11267
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
binary_clause_temp_var18828 = $54
	sta binary_clause_temp_var18828
	; Load Byte array
	ldx zcurrent
	lda zx,x
	
binary_clause_temp_2_var24459 = $56
	sta binary_clause_temp_2_var24459
	lda binary_clause_temp_var18828
	cmp binary_clause_temp_2_var24459;keep
	bne elsedoneblock27999
ConditionalTrueBlock3271
	; Binary clause Simplified: EQUALS
	lda cy
binary_clause_temp_var30614 = $54
	sta binary_clause_temp_var30614
	; Load Byte array
	ldx zcurrent
	lda zy,x
	
binary_clause_temp_2_var19567 = $56
	sta binary_clause_temp_2_var19567
	lda binary_clause_temp_var30614
	cmp binary_clause_temp_2_var19567;keep
	bne elsedoneblock12935
ConditionalTrueBlock3760
	; Assigning single variable : i
	lda zcurrent
	sta i
	jsr HitZombie
elseblock27098
elsedoneblock12935
elseblock4287
elsedoneblock27999
	rts
	
	
	; ***********  Defining procedure : UpdateZombies
	;    Procedure type : User-defined procedure
	
UpdateZombies
	; Binary clause Simplified: EQUALS
	lda zNumZombies
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock7670
ConditionalTrueBlock7746
	rts
elseblock10538
elsedoneblock7670
	; Binary clause Simplified: EQUALS
	; Modulo
	lda ztime
val_var1731 = $54
	sta val_var1731
	lda ticks
	sec
modulo18154
	sbc val_var1731
	bcs modulo18154
	adc val_var1731
	
	; Compare with pure num / var optimization
	cmp #$1;keep
	bne elsedoneblock16760
ConditionalTrueBlock24917
	; Binary clause Simplified: LESS
	lda zcurrent
	; Compare with pure num / var optimization
	cmp zNumZombies;keep
	bcs elsedoneblock10816
ConditionalTrueBlock9385
	; Binary clause Simplified: EQUALS
	; Load Byte array
	ldx zcurrent
	lda zspawn,x
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock1604
ConditionalTrueBlock5427
	jsr Zombie_Move
elseblock23743
elsedoneblock1604
	; Binary clause Simplified: GREATER
	; Load Byte array
	ldx zcurrent
	lda zspawn,x
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock17589
	beq elsedoneblock17589
ConditionalTrueBlock506
	; Assigning single variable : zspawn
	ldx zcurrent
	; Optimize byte array dec 
	dec zspawn,x
elseblock5158
elsedoneblock17589
elseblock23967
elsedoneblock10816
	jsr DisplayZombies
	; Assigning single variable : zcurrent
	inc zcurrent
	; Binary clause Simplified: EQUALS
	lda zcurrent
	; Compare with pure num / var optimization
	cmp #$a;keep
	bne elsedoneblock3818
ConditionalTrueBlock11447
	; Assigning single variable : zcurrent
	lda #0
	sta zcurrent
elseblock40
elsedoneblock3818
elseblock16371
elsedoneblock16760
	rts
	
	
	; ***********  Defining procedure : SpawnCandy
	;    Procedure type : User-defined procedure
	
SpawnCandy
	; Assigning single variable : x
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #20
val_var25089 = $54
	sta val_var25089
	jsr Random
	sec
modulo2503
	sbc val_var25089
	bcs modulo2503
	adc val_var25089
	
	clc
	adc #1
	 ; end add / sub var with constant
	
	sta x
	; Assigning single variable : y
	; 8 bit binop
	; Add/sub where right value is constant number
	; Modulo
	lda #16
val_var30066 = $54
	sta val_var30066
	jsr Random
	sec
modulo26412
	sbc val_var30066
	bcs modulo26412
	adc val_var30066
	
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
while12570
	; Binary clause Simplified: EQUALS
	lda t
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock28641
ConditionalTrueBlock14676
	; Binary clause Simplified: EQUALS
	; Load Byte array
	; Load pointer array
	ldy x
	lda (screenmemory),y
	
	tax
	lda cMask,x
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock13573
ConditionalTrueBlock1825
	; Assigning single variable : t
	lda #1
	sta t
elseblock18718
elsedoneblock13573
	; Binary clause Simplified: EQUALS
	lda t
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock8692
ConditionalTrueBlock7162
	; Assigning single variable : x
	inc x
	; Binary clause Simplified: GREATER
	lda x
	; Compare with pure num / var optimization
	cmp #$14;keep
	bcc elsedoneblock19203
	beq elsedoneblock19203
ConditionalTrueBlock3542
	; Assigning single variable : x
	lda #1
	sta x
	; Assigning single variable : y
	inc y
	; Binary clause Simplified: GREATER
	lda y
	; Compare with pure num / var optimization
	cmp #$16;keep
	bcc elsedoneblock18721
	beq elsedoneblock18721
ConditionalTrueBlock17939
	; Assigning single variable : y
	lda #4
	sta y
elseblock21138
elsedoneblock18721
elseblock29608
elsedoneblock19203
elseblock1717
elsedoneblock8692
	jmp while12570
elseblock25395
elsedoneblock28641
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
	bne elsedoneblock27700
ConditionalTrueBlock6724
	; Assigning single variable : psnd2
	lda #<sndCAppear
	ldx #>sndCAppear
	sta psnd2
	stx psnd2+1
	; Assigning single variable : vsnd2
	lda #250
	sta vsnd2
	jsr PlaySnd2
elseblock25981
elsedoneblock27700
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
	bcc elsedoneblock15137
	beq elsedoneblock15137
ConditionalTrueBlock684
	; Assigning single variable : p1
	lda #3
	ldy cx
	sta (p1),y
elseblock24016
elsedoneblock15137
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
for2944
	; Assigning single variable : screenmemory
	lda #0
	ldy i
	sta (screenmemory),y
	inc i
	lda #6
	cmp i ;keep
	bcs for2944
forLoopDone821
	; Assigning single variable : i
	lda #1
	sta i
for24643
	; Binary clause Simplified: GREATEREQUAL
	lda candy
	; Compare with pure num / var optimization
	cmp i;keep
	bcc elsedoneblock13452
ConditionalTrueBlock27759
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
elseblock700
elsedoneblock13452
	; Binary clause Simplified: GREATEREQUAL
	lda witchHits
	; Compare with pure num / var optimization
	cmp i;keep
	bcc elsedoneblock11798
ConditionalTrueBlock12448
	; Assigning single variable : screenmemory
	lda #38
	ldy i
	sta (screenmemory),y
	; Assigning single variable : p1
	lda #1
	sta (p1),y
elseblock5227
elsedoneblock11798
	inc i
	lda #6
	cmp i ;keep
	bcs for24643
forLoopDone17615
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
	bne elsedoneblock32629
ConditionalTrueBlock12170
	; Assigning single variable : cx
	inc cx
elseblock6862
elsedoneblock32629
	; Binary clause Simplified: NOTEQUALS
	lda cdx
	; Compare with pure num / var optimization
	cmp #$1;keep
	beq elsedoneblock29400
ConditionalTrueBlock6923
	; Assigning single variable : cx
	dec cx
elseblock27085
elsedoneblock29400
	; Binary clause Simplified: EQUALS
	; Load Byte array
	; Load pointer array
	ldy cx
	lda (screenmemory),y
	
	tax
	lda cMask,x
	
	; Compare with pure num / var optimization
	cmp #$a;keep
	bne elsedoneblock5767
ConditionalTrueBlock9412
	; Assigning single variable : cx
	lda x
	sta cx
	; Binary clause Simplified: LESS
	lda candyscr
	; Compare with pure num / var optimization
	cmp #$4;keep
	bcs elsedoneblock20542
ConditionalTrueBlock27734
	; Assigning single variable : candyscr
	inc candyscr
elseblock25190
elsedoneblock20542
elseblock15765
elsedoneblock5767
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
	bne tempfail24254
binaryclausesuccess18108
	jmp ConditionalTrueBlock15621
tempfail24254
	; Binary clause: EQUALS
	lda clife
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bne elseblock30424
binaryclausesuccess12493
ConditionalTrueBlock15621
	jsr EndCandy
elseblock30424
elsedoneblock22597
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
	beq binaryclausefailed17117
binaryclausesuccess20620
	lda #1; success
	jmp binaryclausefinished1334
binaryclausefailed17117
	lda #0 ; failed state
binaryclausefinished1334
	cmp #1
	beq ConditionalTrueBlock2438
	jmp elsedoneblock10819
ConditionalTrueBlock2438
	; Binary clause: EQUALS
	lda clife
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bne binaryclausefailed6238
binaryclausesuccess2047
	lda #1; success
	jmp binaryclausefinished27671
binaryclausefailed6238
	lda #0 ; failed state
binaryclausefinished27671
	cmp #1
	beq ConditionalTrueBlock8964
	jmp elsedoneblock18866
ConditionalTrueBlock8964
	; Binary clause: GREATER
	lda candy
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bcc binaryclausefailed24272
	beq binaryclausefailed24272
binaryclausesuccess12294
	lda #1; success
	jmp binaryclausefinished26160
binaryclausefailed24272
	lda #0 ; failed state
binaryclausefinished26160
	cmp #1
	beq ConditionalTrueBlock27311
	jmp elsedoneblock1405
ConditionalTrueBlock27311
	; Assigning single variable : cy
	lda py
	sta cy
	; Binary clause Simplified: EQUALS
	lda pd
	; Compare with pure num / var optimization
	cmp #$3a;keep
	bne elsedoneblock10471
ConditionalTrueBlock146
	; Assigning single variable : cdx
	lda #255
	sta cdx
elseblock4229
elsedoneblock10471
	; Binary clause Simplified: EQUALS
	lda pd
	; Compare with pure num / var optimization
	cmp #$3d;keep
	bne elsedoneblock16366
ConditionalTrueBlock29421
	; Assigning single variable : cdx
	lda #1
	sta cdx
elseblock22688
elsedoneblock16366
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
	bne elsedoneblock8982
ConditionalTrueBlock22860
	; Assigning single variable : x
	; 8 bit binop
	; Add/sub where right value is constant number
	lda px
	clc
	adc #1
	 ; end add / sub var with constant
	
	sta x
elseblock2208
elsedoneblock8982
	; Binary clause Simplified: NOTEQUALS
	lda cdx
	; Compare with pure num / var optimization
	cmp #$1;keep
	beq elsedoneblock7887
ConditionalTrueBlock11979
	; Assigning single variable : x
	; 8 bit binop
	; Add/sub where right value is constant number
	lda px
	sec
	sbc #1
	 ; end add / sub var with constant
	
	sta x
elseblock1897
elsedoneblock7887
	; Binary clause Simplified: EQUALS
	; Load Byte array
	; Load pointer array
	ldy x
	lda (screenmemory),y
	
	tax
	lda cMask,x
	
	; Compare with pure num / var optimization
	cmp #$a;keep
	bne elseblock22795
ConditionalTrueBlock17540
	; Assigning single variable : cy
	lda #0
	sta cy
	; Assigning single variable : cx
	sta cx
	; Assigning single variable : cdx
	sta cdx
	jmp elsedoneblock7266
elseblock22795
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
elsedoneblock7266
elseblock32657
elsedoneblock1405
elseblock19154
elsedoneblock18866
elseblock12600
elsedoneblock10819
	; Binary clause Simplified: GREATER
	lda clife
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock18135
	beq elsedoneblock18135
ConditionalTrueBlock26134
	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda ticks
	and #1
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock6097
ConditionalTrueBlock5174
	jsr MoveCandy
elseblock2512
elsedoneblock6097
elseblock32053
elsedoneblock18135
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
	beq binaryclausefailed8182
binaryclausesuccess6120
	lda #1; success
	jmp binaryclausefinished28569
binaryclausefailed8182
	lda #0 ; failed state
binaryclausefinished28569
	cmp #1
	beq ConditionalTrueBlock21292
	jmp elsedoneblock27530
ConditionalTrueBlock21292
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
	bcc dtnooverflow6292
	iny  ; overflow into high byte
dtnooverflow6292
	
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
	bcc dtnooverflow14962
	iny  ; overflow into high byte
dtnooverflow14962
	
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
	bcs elsedoneblock13194
ConditionalTrueBlock12527
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
elseblock26807
elsedoneblock13194
	rts
elseblock20237
elsedoneblock27530
	; Binary clause: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1
	and #1
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	beq binaryclausefailed22511
binaryclausesuccess4235
	lda #1; success
	jmp binaryclausefinished7954
binaryclausefailed22511
	lda #0 ; failed state
binaryclausefinished7954
	cmp #1
	beq ConditionalTrueBlock18913
	jmp elsedoneblock19664
ConditionalTrueBlock18913
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
	bcs elsedoneblock23817
ConditionalTrueBlock15127
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
elseblock27577
elsedoneblock23817
	rts
elseblock6693
elsedoneblock19664
	; Binary clause: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1
	and #2
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	beq binaryclausefailed28029
binaryclausesuccess16211
	lda #1; success
	jmp binaryclausefinished30211
binaryclausefailed28029
	lda #0 ; failed state
binaryclausefinished30211
	cmp #1
	beq ConditionalTrueBlock28400
	jmp elsedoneblock21476
ConditionalTrueBlock28400
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
	bcs elsedoneblock26528
ConditionalTrueBlock21321
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
elseblock20785
elsedoneblock26528
	rts
elseblock6558
elsedoneblock21476
	; Binary clause: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1
	and #4
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	beq binaryclausefailed12796
binaryclausesuccess7262
	lda #1; success
	jmp binaryclausefinished10312
binaryclausefailed12796
	lda #0 ; failed state
binaryclausefinished10312
	cmp #1
	beq ConditionalTrueBlock9270
	jmp elsedoneblock9375
ConditionalTrueBlock9270
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
	bcs elsedoneblock29556
ConditionalTrueBlock32557
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
elseblock14248
elsedoneblock29556
	rts
elseblock7452
elsedoneblock9375
	rts
	
	
	; ***********  Defining procedure : UpdatePlayer
	;    Procedure type : User-defined procedure
	
UpdatePlayer
	; Binary clause Simplified: EQUALS
	; Modulo
	lda #2
val_var32331 = $54
	sta val_var32331
	lda ticks
	sec
modulo75
	sbc val_var32331
	bcs modulo75
	adc val_var32331
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock23332
ConditionalTrueBlock13293
	; Assigning single variable : pan
	inc pan
elseblock24099
elsedoneblock23332
	; Binary clause Simplified: GREATER
	lda pan
	; Compare with pure num / var optimization
	cmp #$5;keep
	bcc elsedoneblock11738
	beq elsedoneblock11738
ConditionalTrueBlock15026
	; Assigning single variable : pan
	lda #0
	sta pan
elseblock14066
elsedoneblock11738
	; Binary clause Simplified: EQUALS
	lda joy1
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock19695
ConditionalTrueBlock441
	; Assigning single variable : pstep
	lda #0
	sta pstep
elseblock3380
elsedoneblock19695
	; Binary clause Simplified: GREATER
	lda pstep
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock1088
	beq elsedoneblock1088
ConditionalTrueBlock12628
	; Assigning single variable : pstep
	dec pstep
elseblock6329
elsedoneblock1088
	; Binary clause Simplified: EQUALS
	lda pstep
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock7679
ConditionalTrueBlock18569
	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1
	and #16
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock17860
ConditionalTrueBlock12306
	jsr MovePlayer
	; Binary clause Simplified: GREATER
	lda pstep
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock32719
	beq elsedoneblock32719
ConditionalTrueBlock6188
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
	bne elseblock32686
ConditionalTrueBlock11146
	; Assigning single variable : stepsnd
	lda #30
	sta stepsnd
	jmp elsedoneblock16589
elseblock32686
	; Assigning single variable : stepsnd
	lda #0
	sta stepsnd
elsedoneblock16589
elseblock30227
elsedoneblock32719
elseblock10635
elsedoneblock17860
elseblock666
elsedoneblock7679
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
	bne elsedoneblock28042
ConditionalTrueBlock32677
	; Binary clause Simplified: LESS
	lda candy
binary_clause_temp_var3873 = $54
	sta binary_clause_temp_var3873
	; 8 bit binop
	; Add/sub where right value is constant number
	lda #6
	sec
	sbc witchHits
	 ; end add / sub var with constant
	
binary_clause_temp_2_var4327 = $56
	sta binary_clause_temp_2_var4327
	lda binary_clause_temp_var3873
	cmp binary_clause_temp_2_var4327;keep
	bcs elsedoneblock31522
ConditionalTrueBlock7846
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
	bcc elsedoneblock24450
	beq elsedoneblock24450
ConditionalTrueBlock31896
	; Assigning single variable : candyscr
	dec candyscr
elseblock12472
elsedoneblock24450
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
elseblock27043
elsedoneblock31522
elseblock2403
elsedoneblock28042
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
	bcc elsedoneblock8695
ConditionalTrueBlock16824
	jsr SpawnAZombie
elseblock17929
elsedoneblock8695
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
for26067
	; Assigning single variable : screenmemory
	lda #0
	ldy i
	sta (screenmemory),y
	inc i
	lda #8
	cmp i ;keep
	bne for26067
forLoopDone4640
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #2
	bcc WordAdd10041
	inc screenmemory+1
WordAdd10041
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
	bne casenext4860
	jsr ClearTime
	clc
	lda #<t11
	adc #0
	ldy #>t11
	sta print_text+0
	sty print_text+1
	ldx #4 ; optimized, look out for bugs
	jsr printstring
	jmp caseend12534
casenext4860
	lda moonPos
	cmp #$6 ;keep
	bne casenext27692
	jsr ClearTime
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	sec
	sbc #2
	bcs WordAdd5117
	dec screenmemory+1
WordAdd5117
	sta screenmemory+0
	clc
	lda #<t12
	adc #0
	ldy #>t12
	sta print_text+0
	sty print_text+1
	ldx #8 ; optimized, look out for bugs
	jsr printstring
	jmp caseend12534
casenext27692
	lda moonPos
	cmp #$8 ;keep
	bne casenext3943
	jsr ClearTime
	clc
	lda #<t01
	adc #0
	ldy #>t01
	sta print_text+0
	sty print_text+1
	ldx #4 ; optimized, look out for bugs
	jsr printstring
	jmp caseend12534
casenext3943
	lda moonPos
	cmp #$a ;keep
	bne casenext26925
	jsr ClearTime
	clc
	lda #<t02
	adc #0
	ldy #>t02
	sta print_text+0
	sty print_text+1
	ldx #4 ; optimized, look out for bugs
	jsr printstring
	jmp caseend12534
casenext26925
	lda moonPos
	cmp #$c ;keep
	bne casenext2850
	jsr ClearTime
	clc
	lda #<t03
	adc #0
	ldy #>t03
	sta print_text+0
	sty print_text+1
	ldx #4 ; optimized, look out for bugs
	jsr printstring
	jmp caseend12534
casenext2850
	lda moonPos
	cmp #$e ;keep
	bne casenext23623
	jsr ClearTime
	clc
	lda #<t04
	adc #0
	ldy #>t04
	sta print_text+0
	sty print_text+1
	ldx #4 ; optimized, look out for bugs
	jsr printstring
	jmp caseend12534
casenext23623
	lda moonPos
	cmp #$10 ;keep
	bne casenext21346
	jsr ClearTime
	clc
	lda #<t05
	adc #0
	ldy #>t05
	sta print_text+0
	sty print_text+1
	ldx #4 ; optimized, look out for bugs
	jsr printstring
	jmp caseend12534
casenext21346
	lda moonPos
	cmp #$12 ;keep
	bne casenext10238
	jsr ClearTime
	clc
	lda #<t06
	adc #0
	ldy #>t06
	sta print_text+0
	sty print_text+1
	ldx #4 ; optimized, look out for bugs
	jsr printstring
	jmp caseend12534
casenext10238
caseend12534
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
	bne elsedoneblock3751
ConditionalTrueBlock11728
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
	bne elsedoneblock1219
ConditionalTrueBlock8363
	; Binary clause Simplified: LESS
	lda candyscr
	; Compare with pure num / var optimization
	cmp #$3;keep
	bcs elsedoneblock424
ConditionalTrueBlock12137
	jsr SpawnCandy
elseblock2397
elsedoneblock424
elseblock4651
elsedoneblock1219
	; Binary clause Simplified: EQUALS
	lda cloudPos
	; Compare with pure num / var optimization
	cmp #$16;keep
	bne elsedoneblock29271
ConditionalTrueBlock31993
	; Assigning single variable : cloudPos
	lda #0
	sta cloudPos
	jsr MoveMoon
	jsr DisplayTime
	; Binary clause Simplified: LESS
	lda candyscr
	; Compare with pure num / var optimization
	cmp #$3;keep
	bcs elsedoneblock6798
ConditionalTrueBlock1293
	jsr SpawnCandy
elseblock28892
elsedoneblock6798
elseblock10382
elsedoneblock29271
	; Assigning single variable : screenmemory
	lda #44
	ldx #148
	sta screenmemory
	stx screenmemory+1
	; Assigning single variable : screenmemory
	lda #1
	ldy cloudPos
	sta (screenmemory),y
elseblock1267
elsedoneblock3751
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
while8493
	; Binary clause Simplified: NOTEQUALS
	; Load pointer array
	ldy #$0
	lda (psnd1),y
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock13774
ConditionalTrueBlock29074
	jmp while8493
elseblock1549
elsedoneblock13774
	jsr StopSounds
	; Assigning single variable : sndFlag
	lda #0
	sta sndFlag
while24069
	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1pressed
	and #16
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock16833
ConditionalTrueBlock29218
	; Assigning single variable : x
	lda y
	sta x
	; Assigning single variable : y
	inc y
	; Binary clause Simplified: GREATER
	; Compare with pure num / var optimization
	cmp #$7;keep
	bcc elsedoneblock6923
	beq elsedoneblock6923
ConditionalTrueBlock30345
	; Assigning single variable : y
	lda #1
	sta y
elseblock12351
elsedoneblock6923
	jsr callReadJoy1
	; Assigning single variable : t
	lda #0
	sta t
for2097
	; Assigning single variable : p1
	lda x
	ldy t
	sta (p1),y
	; Assigning single variable : x
	inc x
	; Binary clause Simplified: GREATER
	; Compare with pure num / var optimization
	cmp #$7;keep
	bcc elsedoneblock4949
	beq elsedoneblock4949
ConditionalTrueBlock11245
	; Assigning single variable : x
	lda #1
	sta x
elseblock14578
elsedoneblock4949
	inc t
	lda #14
	cmp t ;keep
	bne for2097
forLoopDone27844
	jmp while24069
elseblock26865
elsedoneblock16833
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
	bne elsedoneblock28356
ConditionalTrueBlock8624
	; Binary clause Simplified: EQUALS
	lda cloudPos
	; Compare with pure num / var optimization
	cmp #$15;keep
	bne elsedoneblock27089
ConditionalTrueBlock28410
	; Assigning single variable : gameMode
	lda #1
	sta gameMode
	; Assigning single variable : screenmemory
	lda #48
	ldx #16
	sta screenmemory
	stx screenmemory+1
printstring_call12529
	clc
	lda #<printstring_text7672
	adc #0
	ldy #>printstring_text7672
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
elseblock17132
elsedoneblock27089
elseblock17204
elsedoneblock28356
	; Binary clause: EQUALS
	lda px
	; Compare with pure num / var optimization
	cmp lx;keep
	; BC done
	bne binaryclausefailed12628
binaryclausesuccess26947
	lda #1; success
	jmp binaryclausefinished21979
binaryclausefailed12628
	lda #0 ; failed state
binaryclausefinished21979
	cmp #1
	beq ConditionalTrueBlock18277
	jmp elsedoneblock13976
ConditionalTrueBlock18277
	; Binary clause: EQUALS
	lda py
	; Compare with pure num / var optimization
	cmp ly;keep
	; BC done
	bne binaryclausefailed16769
binaryclausesuccess27956
	lda #1; success
	jmp binaryclausefinished14709
binaryclausefailed16769
	lda #0 ; failed state
binaryclausefinished14709
	cmp #1
	beq ConditionalTrueBlock13245
	jmp elsedoneblock14062
ConditionalTrueBlock13245
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
val_var13465 = $54
	sta val_var13465
	jsr Random
	sec
modulo16016
	sbc val_var13465
	bcs modulo16016
	adc val_var13465
	
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
	bcs elsedoneblock5190
ConditionalTrueBlock16497
	; Assigning single variable : lives
	inc lives
elseblock18335
elsedoneblock5190
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
elseblock25128
elsedoneblock14062
elseblock5553
elsedoneblock13976
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
	bne elsedoneblock13143
ConditionalTrueBlock23388
	; Binary clause Simplified: EQUALS
	lda py
	; Compare with pure num / var optimization
	cmp wy;keep
	bne elsedoneblock2997
ConditionalTrueBlock940
	; Assigning single variable : gameMode
	lda #1
	sta gameMode
	; Assigning single variable : screenmemory
	lda #48
	ldx #16
	sta screenmemory
	stx screenmemory+1
printstring_call10485
	clc
	lda #<printstring_text12499
	adc #0
	ldy #>printstring_text12499
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
elseblock27799
elsedoneblock2997
elseblock18675
elsedoneblock13143
	; Binary clause Simplified: EQUALS
	lda pbgchar
	; Compare with pure num / var optimization
	cmp #$26;keep
	bne elsedoneblock18682
ConditionalTrueBlock29613
	; Assigning single variable : gameMode
	lda #1
	sta gameMode
	; Assigning single variable : screenmemory
	lda #48
	ldx #16
	sta screenmemory
	stx screenmemory+1
printstring_call5033
	clc
	lda #<printstring_text16600
	adc #0
	ldy #>printstring_text16600
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
elseblock32612
elsedoneblock18682
	; Assigning single variable : i
	lda #0
	sta i
for31864
	; Binary clause Simplified: EQUALS
	lda px
binary_clause_temp_var19750 = $54
	sta binary_clause_temp_var19750
	; Load Byte array
	ldx i
	lda gx,x
	
binary_clause_temp_2_var21337 = $56
	sta binary_clause_temp_2_var21337
	lda binary_clause_temp_var19750
	cmp binary_clause_temp_2_var21337;keep
	bne elsedoneblock30624
ConditionalTrueBlock30712
	; Binary clause Simplified: EQUALS
	lda py
binary_clause_temp_var28290 = $54
	sta binary_clause_temp_var28290
	; Load Byte array
	ldx i
	lda gy,x
	
binary_clause_temp_2_var1898 = $56
	sta binary_clause_temp_2_var1898
	lda binary_clause_temp_var28290
	cmp binary_clause_temp_2_var1898;keep
	bne elsedoneblock27505
ConditionalTrueBlock4931
	; Assigning single variable : gameMode
	lda #1
	sta gameMode
	; Assigning single variable : screenmemory
	lda #48
	ldx #16
	sta screenmemory
	stx screenmemory+1
printstring_call21447
	clc
	lda #<printstring_text1084
	adc #0
	ldy #>printstring_text1084
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
elseblock22197
elsedoneblock27505
elseblock26984
elsedoneblock30624
	inc i
	lda gNumGhosts
	cmp i ;keep
	bne for31864
forLoopDone2734
	; Binary clause Simplified: GREATER
	lda zNumZombies
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock29855
	beq elsedoneblock29855
ConditionalTrueBlock18725
	; Assigning single variable : i
	lda #0
	sta i
for10700
	; Binary clause Simplified: EQUALS
	lda px
binary_clause_temp_var24312 = $54
	sta binary_clause_temp_var24312
	; Load Byte array
	ldx i
	lda zx,x
	
binary_clause_temp_2_var10144 = $56
	sta binary_clause_temp_2_var10144
	lda binary_clause_temp_var24312
	cmp binary_clause_temp_2_var10144;keep
	bne elsedoneblock8314
ConditionalTrueBlock19095
	; Binary clause Simplified: EQUALS
	lda py
binary_clause_temp_var19808 = $54
	sta binary_clause_temp_var19808
	; Load Byte array
	ldx i
	lda zy,x
	
binary_clause_temp_2_var9290 = $56
	sta binary_clause_temp_2_var9290
	lda binary_clause_temp_var19808
	cmp binary_clause_temp_2_var9290;keep
	bne elsedoneblock21856
ConditionalTrueBlock18200
	; Assigning single variable : gameMode
	lda #1
	sta gameMode
	; Assigning single variable : screenmemory
	lda #48
	ldx #16
	sta screenmemory
	stx screenmemory+1
printstring_call979
	clc
	lda #<printstring_text32618
	adc #0
	ldy #>printstring_text32618
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
elseblock30643
elsedoneblock21856
elseblock30906
elsedoneblock8314
	inc i
	lda zNumZombies
	cmp i ;keep
	bne for10700
forLoopDone19402
elseblock8996
elsedoneblock29855
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
	bne elsedoneblock6336
ConditionalTrueBlock28642
	rts
elseblock16339
elsedoneblock6336
	; Binary clause Simplified: EQUALS
	lda cx
	; Compare with pure num / var optimization
	cmp wx;keep
	bne elsedoneblock1436
ConditionalTrueBlock18129
	; Binary clause Simplified: EQUALS
	lda cy
	; Compare with pure num / var optimization
	cmp wy;keep
	bne elsedoneblock15579
ConditionalTrueBlock12516
	jsr HitWitch
elseblock14481
elsedoneblock15579
elseblock21358
elsedoneblock1436
	; Assigning single variable : i
	lda #0
	sta i
for1771
	; Binary clause Simplified: EQUALS
	lda cx
binary_clause_temp_var6648 = $54
	sta binary_clause_temp_var6648
	; Load Byte array
	ldx i
	lda gx,x
	
binary_clause_temp_2_var26804 = $56
	sta binary_clause_temp_2_var26804
	lda binary_clause_temp_var6648
	cmp binary_clause_temp_2_var26804;keep
	bne elsedoneblock19891
ConditionalTrueBlock1052
	; Binary clause Simplified: EQUALS
	lda cy
binary_clause_temp_var19176 = $54
	sta binary_clause_temp_var19176
	; Load Byte array
	ldx i
	lda gy,x
	
binary_clause_temp_2_var5137 = $56
	sta binary_clause_temp_2_var5137
	lda binary_clause_temp_var19176
	cmp binary_clause_temp_2_var5137;keep
	bne elsedoneblock32668
ConditionalTrueBlock31441
	jsr HitGhost
elseblock8872
elsedoneblock32668
elseblock565
elsedoneblock19891
	inc i
	lda gNumGhosts
	cmp i ;keep
	bne for1771
forLoopDone24541
	; Binary clause Simplified: GREATER
	lda zNumZombies
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock31015
	beq elsedoneblock31015
ConditionalTrueBlock5745
	; Assigning single variable : i
	lda #0
	sta i
for28856
	; Binary clause Simplified: EQUALS
	lda cx
binary_clause_temp_var25167 = $54
	sta binary_clause_temp_var25167
	; Load Byte array
	ldx i
	lda zx,x
	
binary_clause_temp_2_var17420 = $56
	sta binary_clause_temp_2_var17420
	lda binary_clause_temp_var25167
	cmp binary_clause_temp_2_var17420;keep
	bne elsedoneblock6450
ConditionalTrueBlock13101
	; Binary clause Simplified: EQUALS
	lda cy
binary_clause_temp_var22632 = $54
	sta binary_clause_temp_var22632
	; Load Byte array
	ldx i
	lda zy,x
	
binary_clause_temp_2_var20189 = $56
	sta binary_clause_temp_2_var20189
	lda binary_clause_temp_var22632
	cmp binary_clause_temp_2_var20189;keep
	bne elsedoneblock30833
ConditionalTrueBlock23865
	jsr HitZombie
elseblock24696
elsedoneblock30833
elseblock5610
elsedoneblock6450
	inc i
	lda zNumZombies
	cmp i ;keep
	bne for28856
forLoopDone23116
elseblock18018
elsedoneblock31015
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
printstring_call13817
	clc
	lda #<printstring_text22129
	adc #0
	ldy #>printstring_text22129
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
while6753
	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1pressed
	and #16
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock10600
ConditionalTrueBlock10679
	; Assigning single variable : x
	lda y
	sta x
	; Assigning single variable : y
	inc y
	; Binary clause Simplified: GREATER
	; Compare with pure num / var optimization
	cmp #$7;keep
	bcc elsedoneblock31065
	beq elsedoneblock31065
ConditionalTrueBlock8510
	; Assigning single variable : y
	lda #1
	sta y
elseblock30946
elsedoneblock31065
	jsr callReadJoy1
	; Assigning single variable : t
	lda #0
	sta t
for5946
	; Assigning single variable : p1
	lda x
	ldy t
	sta (p1),y
	; Assigning single variable : x
	inc x
	; Binary clause Simplified: GREATER
	; Compare with pure num / var optimization
	cmp #$7;keep
	bcc elsedoneblock24696
	beq elsedoneblock24696
ConditionalTrueBlock32106
	; Assigning single variable : x
	lda #1
	sta x
elseblock31302
elsedoneblock24696
	inc t
	lda #18
	cmp t ;keep
	bne for5946
forLoopDone32715
	jmp while6753
elseblock18613
elsedoneblock10600
	rts
	
	
	; ***********  Defining procedure : GameLoop
	;    Procedure type : User-defined procedure
	
GameLoop
while415
	; Binary clause: EQUALS
	lda gameMode
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bne binaryclausefailed23849
binaryclausesuccess28026
	lda #1; success
	jmp binaryclausefinished24075
binaryclausefailed23849
	lda #0 ; failed state
binaryclausefinished24075
	cmp #1
	beq ConditionalTrueBlock6645
	jmp elsedoneblock6354
ConditionalTrueBlock6645
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
	bne elsedoneblock20128
ConditionalTrueBlock17774
	jsr CollisionPlayer
elseblock2884
elsedoneblock20128
	jsr UpdateCandy
	jsr CollisionCandy
	; Binary clause Simplified: LESSEQUAL
	lda lostchildlevel
	; Compare with pure num / var optimization
	cmp level;keep
	beq ConditionalTrueBlock11714
	bcs elsedoneblock24168
ConditionalTrueBlock11714
	jsr UpdateLostChild
elseblock20149
elsedoneblock24168
	; Binary clause Simplified: EQUALS
	lda witchHits
	; Compare with pure num / var optimization
	cmp #$6;keep
	bne elsedoneblock1697
ConditionalTrueBlock23504
	jsr BeatWitchWait
elseblock1105
elsedoneblock1697
	; Assigning single variable : i
	lda #14
	
	sta i
	; Binary clause Simplified: LESS
	lda moonPos
	; Compare with pure num / var optimization
	cmp #$6;keep
	bcs elsedoneblock32097
ConditionalTrueBlock2343
	; Assigning single variable : i
	lda #12
	
	sta i
elseblock26133
elsedoneblock32097
	; Binary clause Simplified: GREATER
	lda moonPos
	; Compare with pure num / var optimization
	cmp #$11;keep
	bcc elsedoneblock10252
	beq elsedoneblock10252
ConditionalTrueBlock1954
	; Assigning single variable : i
	lda #10
	
	sta i
elseblock930
elsedoneblock10252
	; Assigning memory location
	; Assigning single variable : $900f
	lda i
	sta $900f
while12488
	; Binary clause Simplified: EQUALS
	lda ticks
	; Compare with pure num / var optimization
	cmp oldTick;keep
	bne elsedoneblock26072
ConditionalTrueBlock24870
	jmp while12488
elseblock31754
elsedoneblock26072
	; Assigning single variable : oldTick
	lda ticks
	sta oldTick
	jmp while415
elseblock30175
elsedoneblock6354
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
while19975
	; Binary clause Simplified: GREATER
	lda t
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock8938
	beq elsedoneblock8938
ConditionalTrueBlock30260
	; Assigning single variable : t
	dec t
	; Assigning single variable : i
	lda #0
	sta i
for21547
while21276
	; Binary clause Simplified: EQUALS
	lda ticks
	; Compare with pure num / var optimization
	cmp oldTick;keep
	bne elsedoneblock10684
ConditionalTrueBlock31506
	jmp while21276
elseblock32702
elsedoneblock10684
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
	beq elsedoneblock20311
ConditionalTrueBlock8250
	; Assigning single variable : t
	lda #0
	sta t
	; Assigning single variable : i
	lda #29
	sta i
elseblock13475
elsedoneblock20311
	inc i
	lda #30
	cmp i ;keep
	bne for21547
forLoopDone3318
	jmp while19975
elseblock20514
elsedoneblock8938
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
while11520
	; Binary clause Simplified: GREATER
	lda t
	; Compare with pure num / var optimization
	cmp #$0;keep
	bcc elsedoneblock27892
	beq elsedoneblock27892
ConditionalTrueBlock26101
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
	bne elseblock31785
ConditionalTrueBlock8381
	jsr DrawTitleFrame1
	jmp elsedoneblock522
elseblock31785
	jsr DrawTitleFrame2
elsedoneblock522
	; Assigning single variable : t
	dec t
	; Assigning single variable : i
	lda #0
	sta i
for15713
while27065
	; Binary clause Simplified: EQUALS
	lda ticks
	; Compare with pure num / var optimization
	cmp oldTick;keep
	bne elsedoneblock31496
ConditionalTrueBlock19278
	jmp while27065
elseblock4198
elsedoneblock31496
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
	beq elsedoneblock23202
ConditionalTrueBlock7547
	; Assigning single variable : t
	lda #0
	sta t
	; Assigning single variable : i
	lda #29
	sta i
elseblock12121
elsedoneblock23202
	inc i
	lda #30
	cmp i ;keep
	bne for15713
forLoopDone19919
	jmp while11520
elseblock24798
elsedoneblock27892
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
	bcc dtnooverflow31654
	iny  ; overflow into high byte
dtnooverflow31654
	
	sta screenmemory
	sty screenmemory+1
	; ----------
	; BcdPrint address, number
	ldy #$07 ; screen offset
	ldx #0 ; score byte index
bcdprintloop19359
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
	bpl bcdprintloop19359
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
	bcs elseblock23960
ConditionalTrueBlock23391
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
printdecimal1675
	jsr init_printdecimal_div10 
	ora #$30
	sta (screenmemory),y
	dey
	bpl printdecimal1675
	jmp elsedoneblock30510
elseblock23960
printstring_call30499
	clc
	lda #<printstring_text17775
	adc #0
	ldy #>printstring_text17775
	sta print_text+0
	sty print_text+1
	ldx #13 ; optimized, look out for bugs
	jsr printstring
elsedoneblock30510
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
for25790
	; Assigning single variable : screenmemory
	lda #39
	ldy i
	sta (screenmemory),y
	inc i
	lda n
	cmp i ;keep
	bne for25790
forLoopDone31256
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
while20967
	; Binary clause: GREATER
	lda t
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bcc binaryclausefailed31622
	beq binaryclausefailed31622
binaryclausesuccess27395
	lda #1; success
	jmp binaryclausefinished13208
binaryclausefailed31622
	lda #0 ; failed state
binaryclausefinished13208
	cmp #1
	beq ConditionalTrueBlock9515
	jmp elsedoneblock23495
ConditionalTrueBlock9515
	; Assigning single variable : t
	dec t
	; Binary clause: LESS
	lda x
	; Compare with pure num / var optimization
	cmp #$15;keep
	; BC done
	bcs binaryclausefailed12729
binaryclausesuccess12438
	lda #1; success
	jmp binaryclausefinished9734
binaryclausefailed12729
	lda #0 ; failed state
binaryclausefinished9734
	cmp #1
	beq ConditionalTrueBlock26291
	jmp elsedoneblock10929
ConditionalTrueBlock26291
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
	bcs elsedoneblock24348
ConditionalTrueBlock28134
	; Assigning single variable : y
	inc y
elseblock22972
elsedoneblock24348
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
	bne elsedoneblock13179
ConditionalTrueBlock27437
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
elseblock20695
elsedoneblock13179
	; Binary clause Simplified: EQUALS
	lda x
	; Compare with pure num / var optimization
	cmp #$15;keep
	bne elsedoneblock6576
ConditionalTrueBlock10961
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
elseblock21907
elsedoneblock6576
elseblock31988
elsedoneblock10929
	; Binary clause: EQUALS
	lda x
	; Compare with pure num / var optimization
	cmp #$15;keep
	; BC done
	bne binaryclausefailed5032
binaryclausesuccess13896
	lda #1; success
	jmp binaryclausefinished17508
binaryclausefailed5032
	lda #0 ; failed state
binaryclausefinished17508
	cmp #1
	beq ConditionalTrueBlock22972
	jmp elsedoneblock24196
ConditionalTrueBlock22972
	; Binary clause: LESS
	lda n
	; Compare with pure num / var optimization
	cmp #$58;keep
	; BC done
	bcs binaryclausefailed5169
binaryclausesuccess1570
	lda #1; success
	jmp binaryclausefinished29562
binaryclausefailed5169
	lda #0 ; failed state
binaryclausefinished29562
	cmp #1
	beq ConditionalTrueBlock22638
	jmp elsedoneblock4059
ConditionalTrueBlock22638
	; Assigning single variable : pan
	inc pan
	; Binary clause Simplified: GREATER
	lda pan
	; Compare with pure num / var optimization
	cmp #$5;keep
	bcc elsedoneblock2064
	beq elsedoneblock2064
ConditionalTrueBlock6890
	; Assigning single variable : pan
	lda #0
	sta pan
elseblock1711
elsedoneblock2064
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
	bne elsedoneblock30787
ConditionalTrueBlock14767
	; Assigning single variable : s
	lda #61
	sta s
elseblock21754
elsedoneblock30787
	; Binary clause Simplified: EQUALS
	lda n
	; Compare with pure num / var optimization
	cmp #$58;keep
	bne elsedoneblock2815
ConditionalTrueBlock2088
	; Assigning single variable : screenmemory
	lda #0
	pha
	lda n
	lsr
	lsr
	
	tay
	pla
	sta (screenmemory),y
elseblock10273
elsedoneblock2815
	; Binary clause Simplified: EQUALS
	; Modulo
	lda #4
val_var20454 = $54
	sta val_var20454
	lda n
	sec
modulo25893
	sbc val_var20454
	bcs modulo25893
	adc val_var20454
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elsedoneblock13830
ConditionalTrueBlock19269
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
	bne elseblock30336
ConditionalTrueBlock22999
	; Assigning single variable : stepsnd
	lda #30
	sta stepsnd
	jmp elsedoneblock18829
elseblock30336
	; Assigning single variable : stepsnd
	lda #0
	sta stepsnd
elsedoneblock18829
elseblock11886
elsedoneblock13830
elseblock8245
elsedoneblock4059
elseblock628
elsedoneblock24196
	; Assigning single variable : i
	lda #0
	sta i
for10365
while8061
	; Binary clause Simplified: EQUALS
	lda ticks
	; Compare with pure num / var optimization
	cmp oldTick;keep
	bne elsedoneblock28366
ConditionalTrueBlock6344
	jmp while8061
elseblock20259
elsedoneblock28366
	; Assigning single variable : oldTick
	lda ticks
	sta oldTick
	jsr callReadJoy1
	; Binary clause Simplified: LESS
	lda t
	; Compare with pure num / var optimization
	cmp #$3c;keep
	bcs elsedoneblock3554
ConditionalTrueBlock9107
	; Binary clause Simplified: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1
	and #16
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	beq elsedoneblock3059
ConditionalTrueBlock19029
	; Assigning single variable : t
	lda #0
	sta t
elseblock25316
elsedoneblock3059
elseblock5268
elsedoneblock3554
	inc i
	lda #4
	cmp i ;keep
	bne for10365
forLoopDone20747
	jmp while20967
elseblock23542
elsedoneblock23495
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
val_var22342 = $54
	sta val_var22342
	; 8 bit binop
	; Add/sub where right value is constant number
	lda ticks
	clc
	adc #1
	 ; end add / sub var with constant
	sec
modulo17780
	sbc val_var22342
	bcs modulo17780
	adc val_var22342
	
	sta ticks
	; Binary clause Simplified: EQUALS
	lda sndFlag
	; Compare with pure num / var optimization
	cmp #$1;keep
	bne elsedoneblock17686
ConditionalTrueBlock30406
	jsr UpdateSound
elseblock28295
elsedoneblock17686
	; CloseIRQ
	pla
	tay
	pla
	tax
	pla
	 jmp $eabf     ; return to normal IRQ	
	rti
block41
	jsr Initialise
	jsr SetupSound
	; Assigning single variable : level
	lda #0
	sta level
while15718
	; Binary clause: NOTEQUALS
	lda #1
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	beq binaryclausefailed17187
binaryclausesuccess23961
	lda #1; success
	jmp binaryclausefinished7863
binaryclausefailed17187
	lda #0 ; failed state
binaryclausefinished7863
	cmp #1
	beq ConditionalTrueBlock23035
	jmp elsedoneblock28428
ConditionalTrueBlock23035
	jsr BlankScreen
	lda level
	cmp #$0 ;keep
	bne casenext19251
	jsr TitleCredits
	jmp caseend28599
casenext19251
	lda level
	cmp #$1 ;keep
	bne casenext12081
	jsr TitleScreen
	jmp caseend28599
casenext12081
	lda level
	cmp #$2 ;keep
	bne casenext28103
	jsr LastScore
	jmp caseend28599
casenext28103
	lda level
	cmp #$3 ;keep
	bne casenext15238
	jsr TitleStory
	jmp caseend28599
casenext15238
	lda level
	cmp #$4 ;keep
	bne casenext17464
	jsr TitleStory2
	jmp caseend28599
casenext17464
	lda level
	cmp #$5 ;keep
	bne casenext6572
	jsr TitleCast
	jmp caseend28599
casenext6572
	lda level
	cmp #$6 ;keep
	bne casenext16922
	jsr TitleHowTo
	jmp caseend28599
casenext16922
caseend28599
	; Binary clause: NOTEQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda joy1
	and #16
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	beq binaryclausefailed17598
binaryclausesuccess5398
	lda #1; success
	jmp binaryclausefinished31077
binaryclausefailed17598
	lda #0 ; failed state
binaryclausefinished31077
	cmp #1
	beq ConditionalTrueBlock21260
	jmp elsedoneblock25343
ConditionalTrueBlock21260
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
val_var11888 = $54
	sta val_var11888
	jsr Random
	sec
modulo14463
	sbc val_var11888
	bcs modulo14463
	adc val_var11888
	
	clc
	adc #1
	 ; end add / sub var with constant
	
	sta lostchildlevel
while29053
	; Binary clause: GREATER
	lda lives
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	bcc binaryclausefailed12312
	beq binaryclausefailed12312
binaryclausesuccess14883
	lda #1; success
	jmp binaryclausefinished3232
binaryclausefailed12312
	lda #0 ; failed state
binaryclausefinished3232
	cmp #1
	beq ConditionalTrueBlock3505
	jmp elsedoneblock21267
ConditionalTrueBlock3505
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
	bne elsedoneblock6209
ConditionalTrueBlock32589
	; Assigning single variable : lives
	dec lives
	; Binary clause Simplified: EQUALS
	lda lives
	; Compare with pure num / var optimization
	cmp #$0;keep
	bne elseblock19601
ConditionalTrueBlock3954
	jsr TitleGameOver
	jmp elsedoneblock13940
elseblock19601
	jsr TitleFail
elsedoneblock13940
elseblock17683
elsedoneblock6209
	; Binary clause Simplified: EQUALS
	lda gameMode
	; Compare with pure num / var optimization
	cmp #$2;keep
	bne elsedoneblock11983
ConditionalTrueBlock26623
	; Assigning single variable : level
	inc level
	; Binary clause Simplified: EQUALS
	lda level
	; Compare with pure num / var optimization
	cmp #$d;keep
	bne elseblock8079
ConditionalTrueBlock29073
	jsr TitleGameWon
	; Assigning single variable : lives
	lda #0
	sta lives
	jmp elsedoneblock8709
elseblock8079
	jsr TitleGameNext
	; Assigning single variable : witchHits
	lda #0
	sta witchHits
	; Binary clause Simplified: LESS
	lda lives
	; Compare with pure num / var optimization
	cmp #$5;keep
	bcs elsedoneblock29343
ConditionalTrueBlock7475
	; Binary clause Simplified: EQUALS
	lda level
	; Compare with pure num / var optimization
	cmp #$2;keep
	bne elsedoneblock15941
ConditionalTrueBlock2510
	; Assigning single variable : lives
	inc lives
elseblock10396
elsedoneblock15941
	; Binary clause Simplified: EQUALS
	lda level
	; Compare with pure num / var optimization
	cmp #$5;keep
	bne elsedoneblock15948
ConditionalTrueBlock4020
	; Assigning single variable : lives
	inc lives
elseblock23355
elsedoneblock15948
	; Binary clause Simplified: EQUALS
	lda level
	; Compare with pure num / var optimization
	cmp #$9;keep
	bne elsedoneblock31173
ConditionalTrueBlock31825
	; Assigning single variable : lives
	inc lives
elseblock9228
elsedoneblock31173
	; Binary clause Simplified: EQUALS
	lda level
	; Compare with pure num / var optimization
	cmp #$b;keep
	bne elsedoneblock13739
ConditionalTrueBlock31238
	; Assigning single variable : lives
	inc lives
elseblock13991
elsedoneblock13739
elseblock28815
elsedoneblock29343
elsedoneblock8709
elseblock11472
elsedoneblock11983
	jmp while29053
elseblock15741
elsedoneblock21267
	; Assigning single variable : level
	lda #0
	sta level
elseblock9298
elsedoneblock25343
	; Assigning single variable : level
	inc level
	; Binary clause Simplified: GREATER
	lda level
	; Compare with pure num / var optimization
	cmp #$6;keep
	bcc elsedoneblock22490
	beq elsedoneblock22490
ConditionalTrueBlock17054
	; Assigning single variable : level
	lda #0
	sta level
elseblock29877
elsedoneblock22490
	jmp while15718
elseblock7285
elsedoneblock28428
EndBlock43
	org $4800
titleScrn
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///compressed//titleScrn_c.bin"
	org $4877
gameChars
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///compressed//gameChars_c.bin"
	org $49e4
titleChars
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///compressed//titleChars_c.bin"
	org $4b40
scr00
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///compressed//scr00_c.bin"
	org $4ba7
gameoverChars
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///compressed//gameoverChars_c.bin"
	org $4d31
gamewonChars
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///compressed//gamewonChars_c.bin"
	org $4ec8
scrgameover
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///compressed//scrgameover_c.bin"
	org $4f54
scrgamewon
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///compressed//scrgamewon_c.bin"
	org $4fe5
titlestory
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///compressed//titlestory_c.bin"
	org $5097
titlecast
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///compressed//titlecast_c.bin"
	org $512b
scrhowto
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///compressed//scrhowto_c.bin"
	org $51d1
titlestory2
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///compressed//titlestory2_c.bin"
	org $5275
failchars
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///compressed//failchars_c.bin"
	org $5411
failscr
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///compressed//failscr_c.bin"
	org $549d
scrheader
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///compressed//scrheader_c.bin"
	org $54cd
scr01
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///compressed//scr01_c.bin"
	org $552a
scr02
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///compressed//scr02_c.bin"
	org $558e
scr03
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///compressed//scr03_c.bin"
	org $5603
scr04
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///compressed//scr04_c.bin"
	org $566e
scr05
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///compressed//scr05_c.bin"
	org $56d3
scr06
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///compressed//scr06_c.bin"
	org $5739
scr07
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///compressed//scr07_c.bin"
	org $579c
scr08
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///compressed//scr08_c.bin"
	org $5820
scr09
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///compressed//scr09_c.bin"
	org $5886
scr10
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///compressed//scr10_c.bin"
	org $58f0
scr11
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///compressed//scr11_c.bin"
	org $5960
scr12
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///compressed//scr12_c.bin"
	org $59d2
scr13
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///compressed//scr13_c.bin"
	org $5a39
gamenextchars
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///compressed//gamenextchars_c.bin"
	org $5b93
gamenextscr
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///compressed//gamenextscr_c.bin"
	org $5bf6
lastscorescr
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///compressed//lastscorescr_c.bin"
	org $5c36
creditschar
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///compressed//creditschar_c.bin"
	org $5d0c
creditscr
	incbin "C:/OneDrive/Documents/GitHub/build-TRSE-Desktop_Qt_5_13_0_MSVC2017_64bit-Release/tutorials/VIC20/PumpKid///compressed//creditscr_c.bin"
