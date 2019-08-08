 processor 6502

	ORG $401
	.byte    $0E, $08, $0A, $00, $9E, $20, $28
	.byte   $31,$30,$34,$30
	.byte    $29, $00, $00, $00

	ORG $410

Nibbler
	jmp block89383
scrm	dc.w	 
	org scrm+48
clrm	dc.w	 
	org clrm+48
t	dc.b	
i	dc.b	
r	dc.b	
s	dc.b	
int	dc.w	
p1	= $02
p2	= $04
p3	= $08
head	= $16
tail	= $0B
headc	= $0D
tailc	= $10
tailWait	dc.b	$0
tailGrow	dc.b	$1
animTime	dc.b	$0
moveTime	dc.b	$0
moveTimeReset	dc.b	$b
justMoved	dc.b	$0
numFood	dc.b	$0
gameState	dc.b	$0
stopTime	dc.b	$a
wave	dc.b	$0
levelId	dc.b	$0
time	dc.b	$63
timeTick	dc.b	$0
score	dc.w	$0
lives	dc.b	$5
levelarray	dc.b $00, $01, $02, $03, $04, $05, $06, $07
	dc.b $08, $09
foodarray	dc.b $00, $01, $02, $03, $04, $00, $01, $02
	dc.b $03, $04
colourarray	dc.b $07, $03, $04, $05, $02, $07, $05, $04
	dc.b $03, $02
	
	
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
	rts
	
	
	; ***********  Defining procedure : initprintstring
	;    Procedure type : User-defined procedure
	
print_text = $4c
print_number_text .dc "    ",0
printstring
	ldy #0
printstringloop
	lda (print_text),y
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
	
	
	; ***********  Defining procedure : ClearTheScreen
	;    Procedure type : User-defined procedure
	
 ; Temp vars section
 ; Temp vars section ends
ClearTheScreen

	; Clear screen with offset
	lda #0
	ldx #$FD
clearloop92777
	sta $0000+$1dff,x
	sta $00FD+$1dff,x
	dex
	bne clearloop92777
	; Clear screen with offset
	lda #1
	ldx #$FD
clearloop36915
	sta $0000+$95ff,x
	sta $00FD+$95ff,x
	dex
	bne clearloop36915

	rts
	
	
	; ***********  Defining procedure : ScreenSetup
	;    Procedure type : User-defined procedure
	
ScreenSetup

screenmemory =  $fe
	jsr ClearTheScreen
	lda $9005
	and #%11110000
	ora #%00001111
	sta $9005
	; Assigning memory location (poke replacement)
	; Assigning single variable : $9002
	lda #147
	
	sta $9002
	; Assigning memory location (poke replacement)
	; Assigning single variable : $9003
	lda #48
	sta $9003
	; Assigning memory location (poke replacement)
	; Assigning single variable : $9000
	; 8 bit binop
	; Add/sub where right value is constant number
	; Peek
	lda $9000 + $0
	
	clc
	adc #2
	 ; end add / sub var with constant
	
	sta $9000
	; Assigning memory location (poke replacement)
	; Assigning single variable : $9001
	; 8 bit binop
	; Add/sub where right value is constant number
	; Peek
	lda $9001 + $0
	
	sec
	sbc #2
	 ; end add / sub var with constant
	
	sta $9001
	; Assigning memory location (poke replacement)
	; Assigning single variable : $900e
	lda #12
	sta $900e
	; Assigning memory location (poke replacement)
	; Assigning single variable : $900f
	lda #8
	sta $900f
	; ----------
	; DefineAddressTable address, StartValue, IncrementValue, TableSize
	ldy #>$1e00
	lda #<$1e00
	ldx #0
	sta scrm,x   ; Address of table
	tya
	sta scrm+1,x
dtloop38335
	tay
	lda scrm,x
	inx
	inx
	clc
	adc #$13
	bcc dtnooverflow85386
	iny
dtnooverflow85386
	sta scrm,x
	tya
	sta scrm+1,x
	cpx #$30
	bcc dtloop38335
	; ----------
	; DefineAddressTable address, StartValue, IncrementValue, TableSize
	ldy #>$9600
	lda #<$9600
	ldx #0
	sta clrm,x   ; Address of table
	tya
	sta clrm+1,x
dtloop60492
	tay
	lda clrm,x
	inx
	inx
	clc
	adc #$13
	bcc dtnooverflow16649
	iny
dtnooverflow16649
	sta clrm,x
	tya
	sta clrm+1,x
	cpx #$30
	bcc dtloop60492

	rts
	
	
	; ***********  Defining procedure : SetupPlayer
	;    Procedure type : User-defined procedure
	
SetupPlayer

	; Assigning single variable : head
	lda #154
	ldx #31
	sta head
	stx head+1
	; Assigning single variable : tail
	lda #149
	ldx #31
	sta tail
	stx tail+1
	; Assigning single variable : headc
	lda #154
	ldx #151
	sta headc
	stx headc+1
	; Assigning single variable : tailc
	lda #149
	ldx #151
	sta tailc
	stx tailc+1

	rts
	
	
	; ***********  Defining procedure : SetupJoystick
	;    Procedure type : User-defined procedure
	
	jmp block2362
JoyResult	dc.b	$0
block2362
SetupJoystick

	; 
	; ****** Inline assembler section


PORTACASS       = $911F
PORTBVIA2       = $9120      ; Port B 6522 2 value (joystick)
PORTBVIA2d      = $9122      ; Port B 6522 2 direction (joystick)


	
	

	rts
	
	
	; ***********  Defining procedure : ReadJoystick
	;    Procedure type : User-defined procedure
	
ReadJoystick

	; 
	; ****** Inline assembler section


JoyRead      
                LDA PORTACASS
                EOR #$FF
                AND #$3C
                LDX #$7F
                SEI
                STX PORTBVIA2d
                LDY PORTBVIA2
                BMI @jump
                ORA #$02
@jump           LDX #$FF
                STX PORTBVIA2d
                CLI
                LSR
                STA JoyResult
	
	

	rts
	
	
	; ***********  Defining procedure : WaitForNRasters
	;    Procedure type : User-defined procedure
	
	jmp block68690
numberOfRastersToWait	dc.b	$1
block68690
WaitForNRasters

	; Assigning single variable : s
	lda #1
	sta s
for20059
	; wait for raster
	ldx #130
	cpx $9004
	bne *-3
	inc s
	lda numberOfRastersToWait
	cmp s
	bcs for20059
forLoopDone13926

	rts
	
	
	; ***********  Defining procedure : PlaySFX
	;    Procedure type : User-defined procedure
	
	jmp block83426
hissDuration	dc.b	$0
block83426
PlaySFX

	; Binary clause Simplified: EQUALS
	lda hissDuration
	; Compare with pure num / var optimization
	bne elsedoneblock95368
ConditionalTrueBlock55736

	rts

elseblock5211
elsedoneblock95368
	; Binary clause Simplified: EQUALS
	lda hissDuration
	; Compare with pure num / var optimization
	cmp #1
	bne elsedoneblock65123
ConditionalTrueBlock21530

	; Assigning memory location (poke replacement)
	; Assigning single variable : $900d
	lda #0
	sta $900d
	; Assigning single variable : hissDuration
	sta hissDuration
	rts

elseblock22862
elsedoneblock65123
	; Assigning memory location (poke replacement)
	; Assigning single variable : $900d
	lda #253
	sta $900d
	; Assigning single variable : hissDuration
	dec hissDuration

	rts
	
	
	; ***********  Defining procedure : PlaySND1
	;    Procedure type : User-defined procedure
	
	jmp block13929
snd1Freq	dc.b	$0
block13929
PlaySND1

	; Binary clause Simplified: GREATER
	lda snd1Freq
	; Compare with pure num / var optimization
	sbc #0
	cmp #130
	bcc elsedoneblock33069
ConditionalTrueBlock34022
	; Assigning single variable : snd1Freq
	dec snd1Freq
elseblock23058
elsedoneblock33069
	; Assigning memory location (poke replacement)
	; Assigning single variable : $900a
	lda snd1Freq
	sta $900a

	rts
	
	
	; ***********  Defining procedure : PlaySND2
	;    Procedure type : User-defined procedure
	
	jmp block18456
snd2Freq	dc.b	$0
block18456
PlaySND2

	; Binary clause Simplified: EQUALS
	lda snd2Freq
	; Compare with pure num / var optimization
	cmp #240
	bne elsedoneblock77373
ConditionalTrueBlock78042

	; Assigning single variable : snd2Freq
	lda #0
	sta snd2Freq
	; Assigning memory location (poke replacement)
	; Assigning single variable : $900b
	sta $900b
	rts

elseblock76229
elsedoneblock77373
	; Binary clause Simplified: NOTEQUALS
	lda snd2Freq
	; Compare with pure num / var optimization
	beq elsedoneblock94324
ConditionalTrueBlock98537
	; Assigning single variable : snd2Freq
	; Optimizer: a = a +/- b
	lda snd2Freq
	clc
	adc #10
	sta snd2Freq
elseblock75198
elsedoneblock94324
	; Assigning memory location (poke replacement)
	; Assigning single variable : $900b
	lda snd2Freq
	sta $900b

	rts
	
	
	; ***********  Defining procedure : DrawLevelFrame
	;    Procedure type : User-defined procedure
	
DrawLevelFrame

	; Assigning single variable : p1
	lda #76
	ldx #30
	sta p1
	stx p1+1
	; Assigning single variable : p2
	lda #162
	ldx #31
	sta p2
	stx p2+1
	; Assigning single variable : i
	lda #1
	sta i
for3526

	; Assigning single variable : p1
	lda #18
	ldy i
	sta (p1),y
	; Assigning single variable : p2
	ldy i
	sta (p2),y

	inc i
	lda #17
	cmp i
	bcs for3526
forLoopDone68980
	; Assigning single variable : p1
	lda #20
	ldy #0
	sta (p1),y
	; Assigning single variable : p1
	lda #21
	ldy #18
	sta (p1),y
	; Assigning single variable : p2
	lda #22
	ldy #0
	sta (p2),y
	; Assigning single variable : p2
	lda #23
	ldy #18
	sta (p2),y
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #19
	bcc WordAdd41873
	inc p1+1
WordAdd41873
	sta p1+0
	; Assigning single variable : i
	lda #1
	sta i
for6862

	; Assigning single variable : p1
	lda #19
	ldy #0
	sta (p1),y
	; Assigning single variable : p1
	ldy #18
	sta (p1),y
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #19
	bcc WordAdd20925
	inc p1+1
WordAdd20925
	sta p1+0

	inc i
	lda #17
	cmp i
	bcs for6862
forLoopDone97281
	; Assigning single variable : p1
	lda #76
	ldx #150
	sta p1
	stx p1+1
	; Assigning single variable : t
	lda #0
	sta t
for77084

	; Assigning single variable : i
	lda #0
	sta i
for53895

	; Assigning single variable : p1
	; Load Byte array
	ldx levelId
	lda colourarray,x
	
	ldy i
	sta (p1),y

	inc i
	lda #18
	cmp i
	bcs for53895
forLoopDone545
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #19
	bcc WordAdd33367
	inc p1+1
WordAdd33367
	sta p1+0

	inc t
	lda #18
	cmp t
	bcs for77084
forLoopDone25857

	rts
	
	
	; ***********  Defining procedure : DrawLevelMaze
	;    Procedure type : User-defined procedure
	
	jmp block15434
level0	dc.b $00, $075, $054, $075, $05, $07d, $040, $05f
	dc.b $00, $07d, $01, $075, $054, $075, $05, $07d
	dc.b $00, $040, $055
level1	dc.b $00, $07d, $01, $077, $050, $077, $00, $0df
	dc.b $00, $07f, $01, $07d, $04, $055, $051, $05f
	dc.b $00, $04, $00
level2	dc.b $00, $055, $00, $055, $00, $055, $00, $055
	dc.b $00, $055, $00, $055, $00, $055, $00, $055
	dc.b $00, $00, $00
level3	dc.b $00, $075, $04, $075, $040, $05d, $011, $055
	dc.b $04, $055, $011, $05d, $040, $075, $04, $075
	dc.b $00, $055, $055
level4	dc.b $00, $05d, $011, $075, $05, $075, $041, $05f
	dc.b $00, $05f, $041, $075, $05, $075, $011, $05d
	dc.b $00, $00, $00
level5	dc.b $00, $07d, $00, $07d, $045, $07d, $040, $05f
	dc.b $040, $075, $05, $075, $054, $075, $05, $07d
	dc.b $00, $054, $05
level6	dc.b $00, $07d, $01, $077, $050, $077, $00, $0ff
	dc.b $00, $07f, $01, $07d, $04, $075, $051, $077
	dc.b $00, $04, $00
level7	dc.b $00, $055, $04, $055, $00, $0d5, $00, $077
	dc.b $00, $055, $00, $055, $010, $055, $00, $055
	dc.b $010, $010, $04
level8	dc.b $04, $075, $04, $075, $040, $05d, $010, $057
	dc.b $05, $057, $010, $05d, $040, $075, $04, $075
	dc.b $00, $050, $05
level9	dc.b $00, $055, $011, $075, $05, $075, $044, $05f
	dc.b $00, $05f, $044, $075, $00, $077, $015, $057
	dc.b $00, $00, $00
block15434
DrawLevelMaze

	; Assigning single variable : p1
	lda #<level0
	ldx #>level0
	sta p1
	stx p1+1
	; Assigning single variable : r
	; Load Byte array
	ldx levelId
	lda levelarray,x
	
	sta r
	; Binary clause Simplified: NOTEQUALS
	; Compare with pure num / var optimization
	beq elsedoneblock71087
ConditionalTrueBlock44043
	; Assigning single variable : i
	lda #1
	sta i
for12399
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #19
	bcc WordAdd47739
	inc p1+1
WordAdd47739
	sta p1+0
	inc i
	lda r
	cmp i
	bcs for12399
forLoopDone49676
elseblock13750
elsedoneblock71087
	; Assigning single variable : p2
	lda #95
	ldx #30
	sta p2
	stx p2+1
	; Assigning single variable : i
	lda #1
	sta i
for10012
forLoopFix5771

	; Assigning single variable : t
	lda #1
	sta t
for98927

	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda #8
	sec
	sbc t
	 ; end add / sub var with constant
	
	tax
	lda #1
shiftbit18149
	cpx #0
	beq shiftbitdone47796
	asl
	dex
	jmp shiftbit18149
shiftbitdone47796
bitmask_var723 = $54
	sta bitmask_var723
	; Load pointer array
	ldy #0
	lda (p1),y
	
	and bitmask_var723
	cmp bitmask_var723
	bne getbit_false86715
	lda #1
	jmp getbit_done26340
getbit_false86715
	lda #0
getbit_done26340
	
	; Compare with pure num / var optimization
	cmp #1
	bne elsedoneblock63368
ConditionalTrueBlock30019

	; Assigning single variable : p2
	lda #28
	ldy t
	sta (p2),y
	; Assigning single variable : p2
	pha
	; 8 bit binop
	; Add/sub where right value is constant number
	lda #18
	sec
	sbc t
	 ; end add / sub var with constant
	
	tay
	pla
	sta (p2),y

elseblock58270
elsedoneblock63368

	inc t
	lda #8
	cmp t
	bcs for98927
forLoopDone5732
	; Assigning single variable : p2
	; WORD optimization: a=a+b
	lda p2+0
	
	clc
	adc #19
	bcc WordAdd2245
	inc p2+1
WordAdd2245
	sta p2+0
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #1
	bcc WordAdd22846
	inc p1+1
WordAdd22846
	sta p1+0

	inc i
	lda #17
	cmp i
	bcc forLoopDone34481
forLoopNotDone90675
	jmp for10012
forLoopDone34481
	; Assigning single variable : p2
	lda #95
	ldx #30
	sta p2
	stx p2+1
	; Assigning single variable : t
	lda #1
	sta t
for93451

	; Assigning single variable : i
	lda #1
	sta i
for36029

	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda #8
	sec
	sbc i
	 ; end add / sub var with constant
	
	tax
	lda #1
shiftbit67917
	cpx #0
	beq shiftbitdone69917
	asl
	dex
	jmp shiftbit67917
shiftbitdone69917
bitmask_var66996 = $54
	sta bitmask_var66996
	; Load pointer array
	ldy #0
	lda (p1),y
	
	and bitmask_var66996
	cmp bitmask_var66996
	bne getbit_false70688
	lda #1
	jmp getbit_done97369
getbit_false70688
	lda #0
getbit_done97369
	
	; Compare with pure num / var optimization
	cmp #1
	bne elsedoneblock44818
ConditionalTrueBlock36840
	; Assigning single variable : p2
	lda #28
	ldy #9
	sta (p2),y
elseblock2904
elsedoneblock44818
	; Assigning single variable : p2
	; WORD optimization: a=a+b
	lda p2+0
	
	clc
	adc #19
	bcc WordAdd87743
	inc p2+1
WordAdd87743
	sta p2+0

	inc i
	lda #8
	cmp i
	bcs for36029
forLoopDone14613
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #1
	bcc WordAdd59470
	inc p1+1
WordAdd59470
	sta p1+0

	inc t
	lda #2
	cmp t
	bcs for93451
forLoopDone68858
	jsr RenderMazeShapes

	rts
	
	
	; ***********  Defining procedure : DrawLevelFood
	;    Procedure type : User-defined procedure
	
	jmp block12183
foodLevel0	dc.b $022, $00, $088, $00, $02, $00, $090, $01
	dc.b $010, $00, $020, $00, $01, $08a, $00, $080
	dc.b $00
foodLevel1	dc.b $022, $00, $088, $00, $02, $00, $090, $01
	dc.b $010, $00, $020, $00, $01, $0a, $080, $080
	dc.b $00
foodLevel2	dc.b $022, $00, $088, $00, $02, $00, $090, $01
	dc.b $010, $00, $020, $00, $01, $08a, $080, $00
	dc.b $00
foodLevel3	dc.b $021, $02, $021, $00, $0a1, $02, $00, $00
	dc.b $09, $00, $00, $02, $081, $08, $021, $00
	dc.b $00
foodLevel4	dc.b $00, $00, $028, $081, $08, $00, $022, $00
	dc.b $011, $00, $022, $00, $08, $081, $028, $00
	dc.b $00
block12183
DrawLevelFood

	; Assigning single variable : numFood
	lda #0
	sta numFood
	; Assigning single variable : p1
	lda #<foodLevel0
	ldx #>foodLevel0
	sta p1
	stx p1+1
	; Assigning single variable : r
	; Load Byte array
	ldx levelId
	lda foodarray,x
	
	sta r
	; Binary clause Simplified: NOTEQUALS
	; Compare with pure num / var optimization
	beq elsedoneblock6725
ConditionalTrueBlock95499
	; Assigning single variable : i
	lda #1
	sta i
for8542
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #17
	bcc WordAdd28804
	inc p1+1
WordAdd28804
	sta p1+0
	inc i
	lda r
	cmp i
	bcs for8542
forLoopDone39507
elseblock89772
elsedoneblock6725
	; Assigning single variable : p2
	lda #95
	ldx #30
	sta p2
	stx p2+1
	; Assigning single variable : p3
	ldx #150
	sta p3
	stx p3+1
	; Assigning single variable : i
	lda #1
	sta i
for76348
forLoopFix90071

	; Assigning single variable : t
	lda #1
	sta t
for84930
forLoopFix87190

	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda #8
	sec
	sbc t
	 ; end add / sub var with constant
	
	tax
	lda #1
shiftbit79107
	cpx #0
	beq shiftbitdone17445
	asl
	dex
	jmp shiftbit79107
shiftbitdone17445
bitmask_var19756 = $54
	sta bitmask_var19756
	; Load pointer array
	ldy #0
	lda (p1),y
	
	and bitmask_var19756
	cmp bitmask_var19756
	bne getbit_false55542
	lda #1
	jmp getbit_done55936
getbit_false55542
	lda #0
getbit_done55936
	
	; Compare with pure num / var optimization
	cmp #1
	bne elsedoneblock27684
ConditionalTrueBlock11340

	; Assigning single variable : p2
	lda #1
	ldy t
	sta (p2),y
	; Assigning single variable : p2
	pha
	; 8 bit binop
	; Add/sub where right value is constant number
	lda #18
	sec
	sbc t
	 ; end add / sub var with constant
	
	tay
	pla
	sta (p2),y
	; Assigning single variable : p3
	lda #6
	ldy t
	sta (p3),y
	; Assigning single variable : p3
	pha
	; 8 bit binop
	; Add/sub where right value is constant number
	lda #18
	sec
	sbc t
	 ; end add / sub var with constant
	
	tay
	pla
	sta (p3),y
	; Assigning single variable : numFood
	; Optimizer: a = a +/- b
	lda numFood
	clc
	adc #2
	sta numFood

elseblock22090
elsedoneblock27684

	inc t
	lda #7
	cmp t
	bcc forLoopDone81095
forLoopNotDone52926
	jmp for84930
forLoopDone81095
	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	; Load pointer array
	ldy #0
	lda (p1),y
	
	and #1
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #1
	bne elsedoneblock3348
ConditionalTrueBlock6887

	; Assigning single variable : p2
	lda #1
	ldy #9
	sta (p2),y
	; Assigning single variable : p3
	lda #6
	ldy #9
	sta (p3),y
	; Assigning single variable : numFood
	inc numFood

elseblock89412
elsedoneblock3348
	; Assigning single variable : p3
	; WORD optimization: a=a+b
	lda p3+0
	
	clc
	adc #19
	bcc WordAdd62009
	inc p3+1
WordAdd62009
	sta p3+0
	; Assigning single variable : p2
	; WORD optimization: a=a+b
	lda p2+0
	
	clc
	adc #19
	bcc WordAdd2336
	inc p2+1
WordAdd2336
	sta p2+0
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #1
	bcc WordAdd25210
	inc p1+1
WordAdd25210
	sta p1+0

	inc i
	lda #17
	cmp i
	bcc forLoopDone973
forLoopNotDone87131
	jmp for76348
forLoopDone973

	rts
	
	
	; ***********  Defining procedure : RenderMazeShapes
	;    Procedure type : User-defined procedure
	
	jmp block66342
mazeTranslate	dc.b $01c, $020, $01e, $017, $01d, $016, $012, $019
	dc.b $01f, $013, $015, $01b, $014, $01a, $018, $00
	dc.b 
block66342
RenderMazeShapes

	; Assigning single variable : p1
	lda #76
	ldx #30
	sta p1
	stx p1+1
	; Assigning single variable : t
	lda #1
	sta t
for67587
forLoopFix69633

	; Assigning single variable : i
	lda #1
	sta i
for22604
forLoopFix70888

	; Assigning single variable : r
	lda #0
	sta r
	; Assigning single variable : s
	; 8 bit binop
	; Add/sub where right value is constant number
	lda i
	clc
	adc #19
	 ; end add / sub var with constant
	
	sta s
	; Binary clause: EQUALS
	; Load pointer array
	ldy s
	lda (p1),y
	
	; Compare with pure num / var optimization
	cmp #28
	; BC done
	bne binaryclausefailed78341
	lda #1; success
	jmp binaryclausefinished63970
binaryclausefailed78341
	lda #0 ; failed state
binaryclausefinished63970
	cmp #1
	beq ConditionalTrueBlock20853
	jmp elsedoneblock7722
ConditionalTrueBlock20853

	; Binary clause Simplified: NOTEQUALS
	; Load pointer array
	ldy i
	lda (p1),y
	
	; Compare with pure num / var optimization
	beq elsedoneblock49517
ConditionalTrueBlock95511
	inc r
elseblock93069
elsedoneblock49517
	; Binary clause Simplified: NOTEQUALS
	; Load pointer array
	; LDA stuff
	; 8 bit binop
	; Add/sub where right value is constant number
	lda i
	clc
	adc #18
	 ; end add / sub var with constant
	
	tay
	lda (p1),y
	
	; Compare with pure num / var optimization
	beq elsedoneblock6506
ConditionalTrueBlock74112
	; Assigning single variable : r
	; Optimizer: a = a +/- b
	lda r
	clc
	adc #2
	sta r
elseblock89300
elsedoneblock6506
	; Binary clause Simplified: NOTEQUALS
	; Load pointer array
	; LDA stuff
	; 8 bit binop
	; Add/sub where right value is constant number
	lda i
	clc
	adc #20
	 ; end add / sub var with constant
	
	tay
	lda (p1),y
	
	; Compare with pure num / var optimization
	beq elsedoneblock34154
ConditionalTrueBlock31489
	; Assigning single variable : r
	; Optimizer: a = a +/- b
	lda r
	clc
	adc #4
	sta r
elseblock81032
elsedoneblock34154
	; Binary clause Simplified: NOTEQUALS
	; Load pointer array
	; LDA stuff
	; 8 bit binop
	; Add/sub where right value is constant number
	lda i
	clc
	adc #38
	 ; end add / sub var with constant
	
	tay
	lda (p1),y
	
	; Compare with pure num / var optimization
	beq elsedoneblock92538
ConditionalTrueBlock6141
	; Assigning single variable : r
	; Optimizer: a = a +/- b
	lda r
	clc
	adc #8
	sta r
elseblock13412
elsedoneblock92538
	; Assigning single variable : p1
	; Load Byte array
	ldx r
	lda mazeTranslate,x
	
	ldy s
	sta (p1),y
	; Binary clause Simplified: EQUALS
	lda i
	; Compare with pure num / var optimization
	cmp #1
	bne elsedoneblock62760
ConditionalTrueBlock11956
	; Binary clause Simplified: NOTEQUALS
	lda r
	; Compare with pure num / var optimization
	beq elsedoneblock32621
ConditionalTrueBlock1535
	; Assigning single variable : p1
	lda #26
	pha
	; 8 bit binop
	; Add/sub where right value is constant number
	lda i
	clc
	adc #18
	 ; end add / sub var with constant
	
	tay
	pla
	sta (p1),y
elseblock20152
elsedoneblock32621
elseblock62844
elsedoneblock62760
	; Binary clause Simplified: EQUALS
	lda i
	; Compare with pure num / var optimization
	cmp #17
	bne elsedoneblock34673
ConditionalTrueBlock50109
	; Binary clause Simplified: NOTEQUALS
	lda r
	; Compare with pure num / var optimization
	beq elsedoneblock57793
ConditionalTrueBlock99355
	; Assigning single variable : p1
	lda #27
	pha
	; 8 bit binop
	; Add/sub where right value is constant number
	lda i
	clc
	adc #20
	 ; end add / sub var with constant
	
	tay
	pla
	sta (p1),y
elseblock65155
elsedoneblock57793
elseblock59631
elsedoneblock34673
	; Binary clause Simplified: EQUALS
	lda t
	; Compare with pure num / var optimization
	cmp #1
	bne elsedoneblock23286
ConditionalTrueBlock82933
	; Binary clause Simplified: NOTEQUALS
	lda r
	; Compare with pure num / var optimization
	beq elsedoneblock9954
ConditionalTrueBlock10050
	; Assigning single variable : p1
	lda #24
	ldy i
	sta (p1),y
elseblock84100
elsedoneblock9954
elseblock98823
elsedoneblock23286
	; Binary clause Simplified: EQUALS
	lda t
	; Compare with pure num / var optimization
	cmp #17
	bne elsedoneblock23919
ConditionalTrueBlock35624
	; Binary clause Simplified: NOTEQUALS
	lda r
	; Compare with pure num / var optimization
	beq elsedoneblock61956
ConditionalTrueBlock26343
	; Assigning single variable : p1
	lda #25
	pha
	; 8 bit binop
	; Add/sub where right value is constant number
	lda i
	clc
	adc #38
	 ; end add / sub var with constant
	
	tay
	pla
	sta (p1),y
elseblock19304
elsedoneblock61956
elseblock42659
elsedoneblock23919

elseblock60008
elsedoneblock7722

	inc i
	lda #17
	cmp i
	bcc forLoopDone34225
forLoopNotDone26815
	jmp for22604
forLoopDone34225
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #19
	bcc WordAdd53666
	inc p1+1
WordAdd53666
	sta p1+0

	inc t
	lda #17
	cmp t
	bcc forLoopDone89685
forLoopNotDone4789
	jmp for67587
forLoopDone89685

	rts
	
	
	; ***********  Defining procedure : ColourTime
	;    Procedure type : User-defined procedure
	
	jmp block68011
timeColor	dc.b	
block68011
ColourTime

	; Assigning single variable : p1
	lda #68
	ldx #150
	sta p1
	stx p1+1
	; Assigning single variable : i
	lda #0
	sta i
for88967
	; Assigning single variable : p1
	lda timeColor
	ldy i
	sta (p1),y
	inc i
	lda #7
	cmp i
	bcs for88967
forLoopDone90192

	rts
	
	
	; ***********  Defining procedure : DrawUITextLabels
	;    Procedure type : User-defined procedure
	
	jmp block80902
plyrString		dc.b	33
	dc.b	34
	dc.b	35
	dc.b	36
	dc.b	0
leftString		dc.b	37
	dc.b	38
	dc.b	0
timeString		dc.b	39
	dc.b	40
	dc.b	41
	dc.b	42
	dc.b	0
waveString		dc.b	58
	dc.b	59
	dc.b	60
	dc.b	61
	dc.b	0
block80902
DrawUITextLabels

	; Assigning single variable : screenmemory
	lda #0
	ldx #30
	sta screenmemory
	stx screenmemory+1
	clc
	lda #<plyrString
	adc #0
	ldy #>plyrString
	sta print_text+0
	sty print_text+1
	ldx #4 ; optimized, look out for bugs
	jsr printstring
	; Assigning single variable : screenmemory
	lda #48
	ldy #10
	sta (screenmemory),y
	; Assigning single variable : screenmemory
	lda #14
	ldx #30
	sta screenmemory
	stx screenmemory+1
	clc
	lda #<leftString
	adc #0
	ldy #>leftString
	sta print_text+0
	sty print_text+1
	ldx #2 ; optimized, look out for bugs
	jsr printstring
	; Assigning single variable : screenmemory
	lda #69
	ldx #30
	sta screenmemory
	stx screenmemory+1
	clc
	lda #<timeString
	adc #0
	ldy #>timeString
	sta print_text+0
	sty print_text+1
	ldx #4 ; optimized, look out for bugs
	jsr printstring
	; Assigning single variable : screenmemory
	lda #48
	ldy #6
	sta (screenmemory),y
	; Assigning single variable : screenmemory
	lda #187
	ldx #31
	sta screenmemory
	stx screenmemory+1
	clc
	lda #<waveString
	adc #0
	ldy #>waveString
	sta print_text+0
	sty print_text+1
	ldx #4 ; optimized, look out for bugs
	jsr printstring

	rts
	
	
	; ***********  Defining procedure : DrawUIBonus
	;    Procedure type : User-defined procedure
	
	jmp block92377
bonusString		dc.b	39
	dc.b	40
	dc.b	41
	dc.b	42
	dc.b	0
block92377
DrawUIBonus

	; Assigning single variable : timeColor
	lda #5
	sta timeColor
	jsr ColourTime
	; Assigning single variable : p1
	lda #68
	ldx #30
	sta p1
	stx p1+1
	; Assigning single variable : t
	lda #0
	sta t
	; Assigning single variable : i
	lda #43
	sta i
for45487

	; Assigning single variable : p1
	lda i
	ldy t
	sta (p1),y
	; Assigning single variable : t
	inc t

	inc i
	lda #47
	cmp i
	bcs for45487
forLoopDone34573

	rts
	
	
	; ***********  Defining procedure : DrawUIScore
	;    Procedure type : User-defined procedure
	
DrawUIScore

	; Assigning single variable : screenmemory
	lda #5
	ldx #30
	sta screenmemory
	stx screenmemory+1
	ldy #0
	; integer assignment NodeVar
	ldy score+1 ; Next one ; optimized, look out for bugs
	lda score
	sta ipd_div_lo
	sty ipd_div_hi
	ldy #4 ; optimized, look out for bugs
printdecimal76125
	jsr init_printdecimal_div10 
	ora #$30
	sta (screenmemory),y
	dey
	bpl printdecimal76125

	rts
	
	
	; ***********  Defining procedure : DrawUILives
	;    Procedure type : User-defined procedure
	
DrawUILives

	; Assigning single variable : screenmemory
	lda #17
	ldx #30
	sta screenmemory
	stx screenmemory+1
	ldy #0
	lda lives
	sta ipd_div_lo
	sty ipd_div_hi
	ldy #1 ; optimized, look out for bugs
printdecimal66797
	jsr init_printdecimal_div10 
	ora #$30
	sta (screenmemory),y
	dey
	bpl printdecimal66797

	rts
	
	
	; ***********  Defining procedure : DrawUITime
	;    Procedure type : User-defined procedure
	
DrawUITime

	; Assigning single variable : screenmemory
	lda #73
	ldx #30
	sta screenmemory
	stx screenmemory+1
	ldy #0
	lda time
	sta ipd_div_lo
	sty ipd_div_hi
	ldy #1 ; optimized, look out for bugs
printdecimal93418
	jsr init_printdecimal_div10 
	ora #$30
	sta (screenmemory),y
	dey
	bpl printdecimal93418

	rts
	
	
	; ***********  Defining procedure : DrawUIWave
	;    Procedure type : User-defined procedure
	
DrawUIWave

	; Assigning single variable : screenmemory
	lda #192
	ldx #31
	sta screenmemory
	stx screenmemory+1
	ldy #0
	lda wave
	sta ipd_div_lo
	sty ipd_div_hi
	ldy #1 ; optimized, look out for bugs
printdecimal48153
	jsr init_printdecimal_div10 
	ora #$30
	sta (screenmemory),y
	dey
	bpl printdecimal48153

	rts
	
	
	; ***********  Defining procedure : MoveNibblerHead
	;    Procedure type : User-defined procedure
	
MoveNibblerHead

	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #0
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #2
	bne elsedoneblock43485
ConditionalTrueBlock96980

	jsr MoveNibblerHeadUp
	rts

elseblock25393
elsedoneblock43485
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #0
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #3
	bne elsedoneblock6898
ConditionalTrueBlock64029

	jsr MoveNibblerHeadRight
	rts

elseblock69559
elsedoneblock6898
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #0
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #4
	bne elsedoneblock11641
ConditionalTrueBlock17719

	jsr MoveNibblerHeadDown
	rts

elseblock84280
elsedoneblock11641
	jsr MoveNibblerHeadLeft

	rts
	
	
	; ***********  Defining procedure : MoveNibblerTail
	;    Procedure type : User-defined procedure
	
MoveNibblerTail

	; Binary clause Simplified: NOTEQUALS
	lda tailWait
	; Compare with pure num / var optimization
	beq elsedoneblock34140
ConditionalTrueBlock27583

	; Assigning single variable : tailWait
	dec tailWait
	rts

elseblock28789
elsedoneblock34140
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #0
	lda (tail),y
	
	; Compare with pure num / var optimization
	cmp #14
	bne elsedoneblock10431
ConditionalTrueBlock32557

	; Assigning single variable : tail
	lda #0
	ldy #0
	sta (tail),y
	; Assigning single variable : tail
	; WORD optimization: a=a+b
	lda tail+0
	
	sec
	sbc #19
	bcs WordAdd43024
	dec tail+1
WordAdd43024
	sta tail+0
	; Assigning single variable : tailc
	; WORD optimization: a=a+b
	lda tailc+0
	
	sec
	sbc #19
	bcs WordAdd37088
	dec tailc+1
WordAdd37088
	sta tailc+0
	; Assigning single variable : tailc
	lda #7
	ldy #0
	sta (tailc),y
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #0
	lda (tail),y
	
	; Compare with pure num / var optimization
	cmp #11
	bne elsedoneblock58232
ConditionalTrueBlock52386

	; Assigning single variable : tail
	lda #17
	ldy #0
	sta (tail),y
	rts

elseblock10569
elsedoneblock58232
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #0
	lda (tail),y
	
	; Compare with pure num / var optimization
	cmp #10
	bne elsedoneblock9663
ConditionalTrueBlock30

	; Assigning single variable : tail
	lda #15
	ldy #0
	sta (tail),y
	rts

elseblock98433
elsedoneblock9663
	; Assigning single variable : tail
	lda #14
	ldy #0
	sta (tail),y
	rts

elseblock52493
elsedoneblock10431
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #0
	lda (tail),y
	
	; Compare with pure num / var optimization
	cmp #15
	bne elsedoneblock22351
ConditionalTrueBlock29649

	; Assigning single variable : tail
	lda #0
	ldy #0
	sta (tail),y
	; Assigning single variable : tail
	; WORD optimization: a=a+b
	lda tail+0
	
	clc
	adc #1
	bcc WordAdd6773
	inc tail+1
WordAdd6773
	sta tail+0
	; Assigning single variable : tailc
	; WORD optimization: a=a+b
	lda tailc+0
	
	clc
	adc #1
	bcc WordAdd39643
	inc tailc+1
WordAdd39643
	sta tailc+0
	; Assigning single variable : tailc
	lda #7
	ldy #0
	sta (tailc),y
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #0
	lda (tail),y
	
	; Compare with pure num / var optimization
	cmp #13
	bne elsedoneblock90244
ConditionalTrueBlock86742

	; Assigning single variable : tail
	lda #14
	ldy #0
	sta (tail),y
	rts

elseblock95354
elsedoneblock90244
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #0
	lda (tail),y
	
	; Compare with pure num / var optimization
	cmp #11
	bne elsedoneblock36353
ConditionalTrueBlock52063

	; Assigning single variable : tail
	lda #16
	ldy #0
	sta (tail),y
	rts

elseblock77418
elsedoneblock36353
	; Assigning single variable : tail
	lda #15
	ldy #0
	sta (tail),y
	rts

elseblock91499
elsedoneblock22351
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #0
	lda (tail),y
	
	; Compare with pure num / var optimization
	cmp #16
	bne elsedoneblock26799
ConditionalTrueBlock5404

	; Assigning single variable : tail
	lda #0
	ldy #0
	sta (tail),y
	; Assigning single variable : tail
	; WORD optimization: a=a+b
	lda tail+0
	
	clc
	adc #19
	bcc WordAdd66704
	inc tail+1
WordAdd66704
	sta tail+0
	; Assigning single variable : tailc
	; WORD optimization: a=a+b
	lda tailc+0
	
	clc
	adc #19
	bcc WordAdd96911
	inc tailc+1
WordAdd96911
	sta tailc+0
	; Assigning single variable : tailc
	lda #7
	ldy #0
	sta (tailc),y
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #0
	lda (tail),y
	
	; Compare with pure num / var optimization
	cmp #13
	bne elsedoneblock18816
ConditionalTrueBlock69829

	; Assigning single variable : tail
	lda #17
	ldy #0
	sta (tail),y
	rts

elseblock23403
elsedoneblock18816
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #0
	lda (tail),y
	
	; Compare with pure num / var optimization
	cmp #12
	bne elsedoneblock46579
ConditionalTrueBlock3605

	; Assigning single variable : tail
	lda #15
	ldy #0
	sta (tail),y
	rts

elseblock2443
elsedoneblock46579
	; Assigning single variable : tail
	lda #16
	ldy #0
	sta (tail),y
	rts

elseblock42733
elsedoneblock26799
	; Assigning single variable : tail
	lda #0
	ldy #0
	sta (tail),y
	; Assigning single variable : tail
	; WORD optimization: a=a+b
	lda tail+0
	
	sec
	sbc #1
	bcs WordAdd73378
	dec tail+1
WordAdd73378
	sta tail+0
	; Assigning single variable : tailc
	; WORD optimization: a=a+b
	lda tailc+0
	
	sec
	sbc #1
	bcs WordAdd34447
	dec tailc+1
WordAdd34447
	sta tailc+0
	; Assigning single variable : tailc
	lda #7
	ldy #0
	sta (tailc),y
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #0
	lda (tail),y
	
	; Compare with pure num / var optimization
	cmp #12
	bne elsedoneblock23787
ConditionalTrueBlock64045

	; Assigning single variable : tail
	lda #14
	ldy #0
	sta (tail),y
	rts

elseblock74882
elsedoneblock23787
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #0
	lda (tail),y
	
	; Compare with pure num / var optimization
	cmp #10
	bne elsedoneblock99426
ConditionalTrueBlock21386

	; Assigning single variable : tail
	lda #16
	ldy #0
	sta (tail),y
	rts

elseblock50353
elsedoneblock99426
	; Assigning single variable : tail
	lda #17
	ldy #0
	sta (tail),y

	rts
	
	
	; ***********  Defining procedure : EatFood
	;    Procedure type : User-defined procedure
	
EatFood

	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #0
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #1
	bne elsedoneblock65417
ConditionalTrueBlock77107

	; Assigning single variable : snd2Freq
	lda #200
	sta snd2Freq
	; Assigning single variable : tailWait
	; Optimizer: a = a +/- b
	lda tailWait
	clc
	adc tailGrow
	sta tailWait
	; Assigning single variable : tailGrow
	inc tailGrow
	; Binary clause Simplified: EQUALS
	lda tailGrow
	; Compare with pure num / var optimization
	cmp #4
	bne elsedoneblock96295
ConditionalTrueBlock67422
	; Assigning single variable : tailGrow
	lda #1
	sta tailGrow
elseblock1171
elsedoneblock96295
	; Binary clause Simplified: LESS
	lda numFood
	; Compare with pure num / var optimization
	cmp #8
	bcs elsedoneblock46571
ConditionalTrueBlock25676
	; Assigning single variable : tailWait
	; Optimizer: a = a +/- b
	lda tailWait
	clc
	adc #2
	sta tailWait
elseblock26652
elsedoneblock46571
	; Assigning single variable : stopTime
	inc stopTime
	; Binary clause Simplified: GREATEREQUAL
	lda stopTime
	; Compare with pure num / var optimization
	cmp #10
	bcc elsedoneblock74089
ConditionalTrueBlock20172
	; Assigning single variable : stopTime
	lda #10
	sta stopTime
elseblock13091
elsedoneblock74089
	; Assigning single variable : score
	ldy #0
	; WORD optimization: a=a+b
	lda score+0
	
	clc
	adc #1
	bcc WordAdd98878
	inc score+1
WordAdd98878
	sta score+0
	; Assigning single variable : numFood
	dec numFood
	jsr DrawUIScore

elseblock46092
elsedoneblock65417

	rts
	
	
	; ***********  Defining procedure : DecStopTime
	;    Procedure type : User-defined procedure
	
DecStopTime

	; Binary clause Simplified: NOTEQUALS
	lda stopTime
	; Compare with pure num / var optimization
	beq elsedoneblock53333
ConditionalTrueBlock85196
	dec stopTime
elseblock62124
elsedoneblock53333

	rts
	
	
	; ***********  Defining procedure : NibblerControl
	;    Procedure type : User-defined procedure
	
NibblerControl

	jsr ReadJoystick
	; Binary clause Simplified: EQUALS
	lda $c5
	; Compare with pure num / var optimization
	cmp #48
	bne elsedoneblock65115
ConditionalTrueBlock56794
	; Assigning single variable : JoyResult
	; 8 bit binop
	; Add/sub where right value is constant number
	lda JoyResult
	ora #2
	 ; end add / sub var with constant
	
	sta JoyResult
elseblock95568
elsedoneblock65115
	; Binary clause Simplified: EQUALS
	lda $c5
	; Compare with pure num / var optimization
	cmp #17
	bne elsedoneblock63198
ConditionalTrueBlock82437
	; Assigning single variable : JoyResult
	; 8 bit binop
	; Add/sub where right value is constant number
	lda JoyResult
	ora #4
	 ; end add / sub var with constant
	
	sta JoyResult
elseblock44263
elsedoneblock63198
	; Binary clause Simplified: EQUALS
	lda $c5
	; Compare with pure num / var optimization
	cmp #52
	bne elsedoneblock65128
ConditionalTrueBlock26513
	; Assigning single variable : JoyResult
	; 8 bit binop
	; Add/sub where right value is constant number
	lda JoyResult
	ora #8
	 ; end add / sub var with constant
	
	sta JoyResult
elseblock91631
elsedoneblock65128
	; Binary clause Simplified: EQUALS
	lda $c5
	; Compare with pure num / var optimization
	cmp #13
	bne elsedoneblock98138
ConditionalTrueBlock52346
	; Assigning single variable : JoyResult
	; 8 bit binop
	; Add/sub where right value is constant number
	lda JoyResult
	ora #1
	 ; end add / sub var with constant
	
	sta JoyResult
elseblock34683
elsedoneblock98138
	; Assigning single variable : justMoved
	lda #0
	sta justMoved
	; Binary clause: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda JoyResult
	and #2
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #2
	; BC done
	bne binaryclausefailed71472
	lda #1; success
	jmp binaryclausefinished4978
binaryclausefailed71472
	lda #0 ; failed state
binaryclausefinished4978
	cmp #1
	beq ConditionalTrueBlock36421
	jmp elsedoneblock87226
ConditionalTrueBlock36421

	; Binary clause: NOTEQUALS
	; Load pointer array
	ldy #0
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #2
	; BC done
	beq binaryclausefailed6629
	lda #1; success
	jmp binaryclausefinished89569
binaryclausefailed6629
	lda #0 ; failed state
binaryclausefinished89569
	cmp #1
	beq ConditionalTrueBlock64797
	jmp elsedoneblock24547
ConditionalTrueBlock64797
	; Binary clause: NOTEQUALS
	; Load pointer array
	ldy #0
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #4
	; BC done
	beq binaryclausefailed97565
	lda #1; success
	jmp binaryclausefinished51794
binaryclausefailed97565
	lda #0 ; failed state
binaryclausefinished51794
	cmp #1
	beq ConditionalTrueBlock96366
	jmp elsedoneblock67395
ConditionalTrueBlock96366

	; Assigning single variable : p1
	
	; HandleVarBinopB16bit here
	ldy #0
	lda #19
rightvarInteger_var51546 = $54
	sta rightvarInteger_var51546
	sty rightvarInteger_var51546+1
	
	lda head+1
	sec
	sbc rightvarInteger_var51546+1
	tay
	lda head
	sec
	sbc rightvarInteger_var51546
	bcs wordAdd87134
	dey
wordAdd87134
	
	sta p1
	sty p1+1
	; Binary clause Simplified: GREATEREQUAL
	; Load pointer array
	ldy #0
	lda (p1),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcc elsedoneblock84303
ConditionalTrueBlock99853
	rts
elseblock33062
elsedoneblock84303
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #0
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #3
	bne elsedoneblock49810
ConditionalTrueBlock96534
	; Assigning single variable : head
	lda #13
	ldy #0
	sta (head),y
elseblock39807
elsedoneblock49810
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #0
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #5
	bne elsedoneblock48696
ConditionalTrueBlock95815
	; Assigning single variable : head
	lda #12
	ldy #0
	sta (head),y
elseblock27984
elsedoneblock48696
	; Assigning single variable : hissDuration
	lda #2
	sta hissDuration
	; Assigning single variable : snd1Freq
	lda #0
	sta snd1Freq
	; Assigning single variable : head
	lda p1
	ldx p1+1
	sta head
	stx head+1
	jsr EatFood
	; Assigning single variable : head
	lda #2
	ldy #0
	sta (head),y
	; Assigning single variable : headc
	; WORD optimization: a=a+b
	lda headc+0
	
	sec
	sbc #19
	bcs WordAdd94465
	dec headc+1
WordAdd94465
	sta headc+0
	; Assigning single variable : headc
	lda #5
	ldy #0
	sta (headc),y
	; Assigning single variable : justMoved
	lda #1
	sta justMoved

elseblock81515
elsedoneblock67395
elseblock16748
elsedoneblock24547
	rts

elseblock68972
elsedoneblock87226
	; Binary clause: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda JoyResult
	and #4
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #4
	; BC done
	bne binaryclausefailed77052
	lda #1; success
	jmp binaryclausefinished65220
binaryclausefailed77052
	lda #0 ; failed state
binaryclausefinished65220
	cmp #1
	beq ConditionalTrueBlock19716
	jmp elsedoneblock69343
ConditionalTrueBlock19716

	; Binary clause: NOTEQUALS
	; Load pointer array
	ldy #0
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #4
	; BC done
	beq binaryclausefailed44854
	lda #1; success
	jmp binaryclausefinished52366
binaryclausefailed44854
	lda #0 ; failed state
binaryclausefinished52366
	cmp #1
	beq ConditionalTrueBlock31287
	jmp elsedoneblock19540
ConditionalTrueBlock31287
	; Binary clause: NOTEQUALS
	; Load pointer array
	ldy #0
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #2
	; BC done
	beq binaryclausefailed3807
	lda #1; success
	jmp binaryclausefinished79759
binaryclausefailed3807
	lda #0 ; failed state
binaryclausefinished79759
	cmp #1
	beq ConditionalTrueBlock1281
	jmp elsedoneblock41973
ConditionalTrueBlock1281

	; Assigning single variable : p1
	
	; HandleVarBinopB16bit here
	ldy #0
	lda #19
rightvarInteger_var82894 = $54
	sta rightvarInteger_var82894
	sty rightvarInteger_var82894+1
	
	lda head+1
	clc
	adc rightvarInteger_var82894+1
	tay
	lda head
	clc
	adc rightvarInteger_var82894
	bcc wordAdd72525
	iny
wordAdd72525
	
	sta p1
	sty p1+1
	; Binary clause Simplified: GREATEREQUAL
	; Load pointer array
	ldy #0
	lda (p1),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcc elsedoneblock21090
ConditionalTrueBlock90158
	rts
elseblock14636
elsedoneblock21090
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #0
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #3
	bne elsedoneblock18865
ConditionalTrueBlock32973
	; Assigning single variable : head
	lda #11
	ldy #0
	sta (head),y
elseblock1937
elsedoneblock18865
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #0
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #5
	bne elsedoneblock5400
ConditionalTrueBlock69409
	; Assigning single variable : head
	lda #10
	ldy #0
	sta (head),y
elseblock88863
elsedoneblock5400
	; Assigning single variable : hissDuration
	lda #2
	sta hissDuration
	; Assigning single variable : snd1Freq
	lda #0
	sta snd1Freq
	; Assigning single variable : head
	lda p1
	ldx p1+1
	sta head
	stx head+1
	jsr EatFood
	; Assigning single variable : head
	lda #4
	ldy #0
	sta (head),y
	; Assigning single variable : headc
	; WORD optimization: a=a+b
	lda headc+0
	
	clc
	adc #19
	bcc WordAdd78004
	inc headc+1
WordAdd78004
	sta headc+0
	; Assigning single variable : headc
	lda #5
	ldy #0
	sta (headc),y
	; Assigning single variable : justMoved
	lda #1
	sta justMoved

elseblock31742
elsedoneblock41973
elseblock32983
elsedoneblock19540
	rts

elseblock25361
elsedoneblock69343
	; Binary clause: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda JoyResult
	and #8
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #8
	; BC done
	bne binaryclausefailed25139
	lda #1; success
	jmp binaryclausefinished33694
binaryclausefailed25139
	lda #0 ; failed state
binaryclausefinished33694
	cmp #1
	beq ConditionalTrueBlock7458
	jmp elsedoneblock26127
ConditionalTrueBlock7458

	; Binary clause: NOTEQUALS
	; Load pointer array
	ldy #0
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #5
	; BC done
	beq binaryclausefailed67697
	lda #1; success
	jmp binaryclausefinished83316
binaryclausefailed67697
	lda #0 ; failed state
binaryclausefinished83316
	cmp #1
	beq ConditionalTrueBlock74782
	jmp elsedoneblock90523
ConditionalTrueBlock74782
	; Binary clause: NOTEQUALS
	; Load pointer array
	ldy #0
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #3
	; BC done
	beq binaryclausefailed27208
	lda #1; success
	jmp binaryclausefinished93630
binaryclausefailed27208
	lda #0 ; failed state
binaryclausefinished93630
	cmp #1
	beq ConditionalTrueBlock87570
	jmp elsedoneblock52986
ConditionalTrueBlock87570

	; Assigning single variable : p1
	
	; HandleVarBinopB16bit here
	ldy #0
	lda #1
rightvarInteger_var99242 = $54
	sta rightvarInteger_var99242
	sty rightvarInteger_var99242+1
	
	lda head+1
	sec
	sbc rightvarInteger_var99242+1
	tay
	lda head
	sec
	sbc rightvarInteger_var99242
	bcs wordAdd10525
	dey
wordAdd10525
	
	sta p1
	sty p1+1
	; Binary clause Simplified: GREATEREQUAL
	; Load pointer array
	ldy #0
	lda (p1),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcc elsedoneblock32676
ConditionalTrueBlock14447
	rts
elseblock71161
elsedoneblock32676
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #0
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #4
	bne elsedoneblock54931
ConditionalTrueBlock90563
	; Assigning single variable : head
	lda #13
	ldy #0
	sta (head),y
elseblock81135
elsedoneblock54931
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #0
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #2
	bne elsedoneblock36688
ConditionalTrueBlock62297
	; Assigning single variable : head
	lda #11
	ldy #0
	sta (head),y
elseblock85900
elsedoneblock36688
	; Assigning single variable : hissDuration
	lda #2
	sta hissDuration
	; Assigning single variable : snd1Freq
	lda #0
	sta snd1Freq
	; Assigning single variable : head
	lda p1
	ldx p1+1
	sta head
	stx head+1
	jsr EatFood
	; Assigning single variable : head
	lda #5
	ldy #0
	sta (head),y
	; Assigning single variable : headc
	; WORD optimization: a=a+b
	lda headc+0
	
	sec
	sbc #1
	bcs WordAdd86974
	dec headc+1
WordAdd86974
	sta headc+0
	; Assigning single variable : headc
	lda #5
	ldy #0
	sta (headc),y
	; Assigning single variable : justMoved
	lda #1
	sta justMoved

elseblock55567
elsedoneblock52986
elseblock80282
elsedoneblock90523
	rts

elseblock66356
elsedoneblock26127
	; Binary clause: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda JoyResult
	and #1
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #1
	; BC done
	bne binaryclausefailed4513
	lda #1; success
	jmp binaryclausefinished58628
binaryclausefailed4513
	lda #0 ; failed state
binaryclausefinished58628
	cmp #1
	beq ConditionalTrueBlock94114
	jmp elsedoneblock43428
ConditionalTrueBlock94114

	; Binary clause: NOTEQUALS
	; Load pointer array
	ldy #0
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #3
	; BC done
	beq binaryclausefailed39366
	lda #1; success
	jmp binaryclausefinished99067
binaryclausefailed39366
	lda #0 ; failed state
binaryclausefinished99067
	cmp #1
	beq ConditionalTrueBlock44856
	jmp elsedoneblock27699
ConditionalTrueBlock44856
	; Binary clause: NOTEQUALS
	; Load pointer array
	ldy #0
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #5
	; BC done
	beq binaryclausefailed27834
	lda #1; success
	jmp binaryclausefinished73949
binaryclausefailed27834
	lda #0 ; failed state
binaryclausefinished73949
	cmp #1
	beq ConditionalTrueBlock73790
	jmp elsedoneblock77290
ConditionalTrueBlock73790

	; Assigning single variable : p1
	
	; HandleVarBinopB16bit here
	ldy #0
	lda #1
rightvarInteger_var49164 = $54
	sta rightvarInteger_var49164
	sty rightvarInteger_var49164+1
	
	lda head+1
	clc
	adc rightvarInteger_var49164+1
	tay
	lda head
	clc
	adc rightvarInteger_var49164
	bcc wordAdd26901
	iny
wordAdd26901
	
	sta p1
	sty p1+1
	; Binary clause Simplified: GREATEREQUAL
	; Load pointer array
	ldy #0
	lda (p1),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcc elsedoneblock21955
ConditionalTrueBlock17043
	rts
elseblock27572
elsedoneblock21955
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #0
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #4
	bne elsedoneblock42496
ConditionalTrueBlock13256
	; Assigning single variable : head
	lda #12
	ldy #0
	sta (head),y
elseblock13640
elsedoneblock42496
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #0
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #2
	bne elsedoneblock13846
ConditionalTrueBlock53510
	; Assigning single variable : head
	lda #10
	ldy #0
	sta (head),y
elseblock7428
elsedoneblock13846
	; Assigning single variable : hissDuration
	lda #2
	sta hissDuration
	; Assigning single variable : snd1Freq
	lda #0
	sta snd1Freq
	; Assigning single variable : head
	lda p1
	ldx p1+1
	sta head
	stx head+1
	jsr EatFood
	; Assigning single variable : head
	lda #3
	ldy #0
	sta (head),y
	; Assigning single variable : headc
	; WORD optimization: a=a+b
	lda headc+0
	
	clc
	adc #1
	bcc WordAdd43294
	inc headc+1
WordAdd43294
	sta headc+0
	; Assigning single variable : headc
	lda #5
	ldy #0
	sta (headc),y
	; Assigning single variable : justMoved
	lda #1
	sta justMoved

elseblock78408
elsedoneblock77290
elseblock12110
elsedoneblock27699
	rts

elseblock13069
elsedoneblock43428

	rts
	
	
	; ***********  Defining procedure : MoveNibblerHeadUp
	;    Procedure type : User-defined procedure
	
MoveNibblerHeadUp

	; Assigning single variable : p1
	
	; HandleVarBinopB16bit here
	ldy #0
	lda #19
rightvarInteger_var67250 = $54
	sta rightvarInteger_var67250
	sty rightvarInteger_var67250+1
	
	lda head+1
	sec
	sbc rightvarInteger_var67250+1
	tay
	lda head
	sec
	sbc rightvarInteger_var67250
	bcs wordAdd64556
	dey
wordAdd64556
	
	sta p1
	sty p1+1
	; Binary clause Simplified: LESS
	; Load pointer array
	ldy #0
	lda (p1),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcs elsedoneblock34319
ConditionalTrueBlock79885

	; Assigning single variable : head
	lda #8
	ldy #0
	sta (head),y
	; Assigning single variable : head
	lda p1
	ldx p1+1
	sta head
	stx head+1
	jsr EatFood
	; Assigning single variable : head
	lda #2
	ldy #0
	sta (head),y
	; Assigning single variable : headc
	; WORD optimization: a=a+b
	lda headc+0
	
	sec
	sbc #19
	bcs WordAdd67324
	dec headc+1
WordAdd67324
	sta headc+0
	; Assigning single variable : headc
	lda #5
	ldy #0
	sta (headc),y
	rts

elseblock43253
elsedoneblock34319
	; Assigning single variable : p2
	
	; HandleVarBinopB16bit here
	ldy #0
	lda #1
rightvarInteger_var4896 = $54
	sta rightvarInteger_var4896
	sty rightvarInteger_var4896+1
	
	lda head+1
	sec
	sbc rightvarInteger_var4896+1
	tay
	lda head
	sec
	sbc rightvarInteger_var4896
	bcs wordAdd82719
	dey
wordAdd82719
	
	sta p2
	sty p2+1
	; Binary clause Simplified: LESS
	; Load pointer array
	ldy #1
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcs elsedoneblock98530
ConditionalTrueBlock26644
	; Binary clause Simplified: LESS
	; Load pointer array
	ldy #0
	lda (p2),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcs elsedoneblock24758
ConditionalTrueBlock74729

	; Binary clause Simplified: EQUALS
	lda snd1Freq
	; Compare with pure num / var optimization
	bne elsedoneblock57902
ConditionalTrueBlock69440
	; Assigning single variable : snd1Freq
	lda #180
	sta snd1Freq
elseblock65626
elsedoneblock57902
	jsr DecStopTime
	inc tailWait
	rts

elseblock34107
elsedoneblock24758
elseblock43203
elsedoneblock98530
	jsr DecStopTime
	; Binary clause Simplified: LESS
	; Load pointer array
	ldy #1
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcs elsedoneblock55804
ConditionalTrueBlock10946

	; Assigning single variable : hissDuration
	lda #2
	sta hissDuration
	; Assigning single variable : head
	lda #10
	ldy #0
	sta (head),y
	; Assigning single variable : head
	; WORD optimization: a=a+b
	lda head+0
	
	clc
	adc #1
	bcc WordAdd11410
	inc head+1
WordAdd11410
	sta head+0
	jsr EatFood
	; Assigning single variable : head
	lda #3
	ldy #0
	sta (head),y
	; Assigning single variable : headc
	; WORD optimization: a=a+b
	lda headc+0
	
	clc
	adc #1
	bcc WordAdd70448
	inc headc+1
WordAdd70448
	sta headc+0
	; Assigning single variable : headc
	lda #5
	ldy #0
	sta (headc),y
	rts

elseblock67123
elsedoneblock55804
	; Binary clause Simplified: LESS
	; Load pointer array
	ldy #0
	lda (p2),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcs elsedoneblock82271
ConditionalTrueBlock62069

	; Assigning single variable : hissDuration
	lda #2
	sta hissDuration
	; Assigning single variable : head
	lda #11
	ldy #0
	sta (head),y
	; Assigning single variable : head
	lda p2
	ldx p2+1
	sta head
	stx head+1
	jsr EatFood
	; Assigning single variable : head
	lda #5
	ldy #0
	sta (head),y
	; Assigning single variable : headc
	; WORD optimization: a=a+b
	lda headc+0
	
	sec
	sbc #1
	bcs WordAdd11556
	dec headc+1
WordAdd11556
	sta headc+0
	; Assigning single variable : headc
	lda #5
	ldy #0
	sta (headc),y
	rts

elseblock71000
elsedoneblock82271
	; Assigning single variable : gameState
	lda #1
	sta gameState
	; Assigning single variable : headc
	lda #2
	ldy #0
	sta (headc),y

	rts
	
	
	; ***********  Defining procedure : MoveNibblerHeadRight
	;    Procedure type : User-defined procedure
	
MoveNibblerHeadRight

	; Binary clause Simplified: LESS
	; Load pointer array
	ldy #1
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcs elsedoneblock74951
ConditionalTrueBlock54328

	; Assigning single variable : head
	lda #6
	ldy #0
	sta (head),y
	; Assigning single variable : head
	; WORD optimization: a=a+b
	lda head+0
	
	clc
	adc #1
	bcc WordAdd2345
	inc head+1
WordAdd2345
	sta head+0
	jsr EatFood
	; Assigning single variable : head
	lda #3
	ldy #0
	sta (head),y
	; Assigning single variable : headc
	; WORD optimization: a=a+b
	lda headc+0
	
	clc
	adc #1
	bcc WordAdd51514
	inc headc+1
WordAdd51514
	sta headc+0
	; Assigning single variable : headc
	lda #5
	ldy #0
	sta (headc),y
	rts

elseblock14462
elsedoneblock74951
	; Assigning single variable : p1
	
	; HandleVarBinopB16bit here
	ldy #0
	lda #19
rightvarInteger_var23182 = $54
	sta rightvarInteger_var23182
	sty rightvarInteger_var23182+1
	
	lda head+1
	sec
	sbc rightvarInteger_var23182+1
	tay
	lda head
	sec
	sbc rightvarInteger_var23182
	bcs wordAdd94861
	dey
wordAdd94861
	
	sta p1
	sty p1+1
	; Assigning single variable : p2
	
	; HandleVarBinopB16bit here
	ldy #0
	lda #19
rightvarInteger_var5807 = $54
	sta rightvarInteger_var5807
	sty rightvarInteger_var5807+1
	
	lda head+1
	clc
	adc rightvarInteger_var5807+1
	tay
	lda head
	clc
	adc rightvarInteger_var5807
	bcc wordAdd25972
	iny
wordAdd25972
	
	sta p2
	sty p2+1
	; Binary clause Simplified: LESS
	; Load pointer array
	ldy #0
	lda (p1),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcs elsedoneblock47294
ConditionalTrueBlock98129
	; Binary clause Simplified: LESS
	; Load pointer array
	ldy #0
	lda (p2),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcs elsedoneblock93485
ConditionalTrueBlock15055

	; Binary clause Simplified: EQUALS
	lda snd1Freq
	; Compare with pure num / var optimization
	bne elsedoneblock75789
ConditionalTrueBlock99950
	; Assigning single variable : snd1Freq
	lda #180
	sta snd1Freq
elseblock61120
elsedoneblock75789
	jsr DecStopTime
	inc tailWait
	rts

elseblock11310
elsedoneblock93485
elseblock10911
elsedoneblock47294
	jsr DecStopTime
	; Binary clause Simplified: LESS
	; Load pointer array
	ldy #0
	lda (p1),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcs elsedoneblock47042
ConditionalTrueBlock60056

	; Assigning single variable : hissDuration
	lda #2
	sta hissDuration
	; Assigning single variable : head
	lda #13
	ldy #0
	sta (head),y
	; Assigning single variable : head
	lda p1
	ldx p1+1
	sta head
	stx head+1
	jsr EatFood
	; Assigning single variable : head
	lda #2
	ldy #0
	sta (head),y
	; Assigning single variable : headc
	; WORD optimization: a=a+b
	lda headc+0
	
	sec
	sbc #19
	bcs WordAdd83458
	dec headc+1
WordAdd83458
	sta headc+0
	; Assigning single variable : headc
	lda #5
	ldy #0
	sta (headc),y
	rts

elseblock20737
elsedoneblock47042
	; Binary clause Simplified: LESS
	; Load pointer array
	ldy #0
	lda (p2),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcs elsedoneblock38264
ConditionalTrueBlock88109

	; Assigning single variable : hissDuration
	lda #2
	sta hissDuration
	; Assigning single variable : head
	lda #11
	ldy #0
	sta (head),y
	; Assigning single variable : head
	lda p2
	ldx p2+1
	sta head
	stx head+1
	jsr EatFood
	; Assigning single variable : head
	lda #4
	ldy #0
	sta (head),y
	; Assigning single variable : headc
	; WORD optimization: a=a+b
	lda headc+0
	
	clc
	adc #19
	bcc WordAdd59216
	inc headc+1
WordAdd59216
	sta headc+0
	; Assigning single variable : headc
	lda #5
	ldy #0
	sta (headc),y
	rts

elseblock658
elsedoneblock38264
	; Assigning single variable : gameState
	lda #1
	sta gameState
	; Assigning single variable : headc
	lda #2
	ldy #0
	sta (headc),y

	rts
	
	
	; ***********  Defining procedure : MoveNibblerHeadDown
	;    Procedure type : User-defined procedure
	
MoveNibblerHeadDown

	; Binary clause Simplified: LESS
	; Load pointer array
	ldy #19
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcs elsedoneblock85506
ConditionalTrueBlock96499

	; Assigning single variable : head
	lda #9
	ldy #0
	sta (head),y
	; Assigning single variable : head
	; WORD optimization: a=a+b
	lda head+0
	
	clc
	adc #19
	bcc WordAdd59970
	inc head+1
WordAdd59970
	sta head+0
	jsr EatFood
	; Assigning single variable : head
	lda #4
	ldy #0
	sta (head),y
	; Assigning single variable : headc
	; WORD optimization: a=a+b
	lda headc+0
	
	clc
	adc #19
	bcc WordAdd26701
	inc headc+1
WordAdd26701
	sta headc+0
	; Assigning single variable : headc
	lda #5
	ldy #0
	sta (headc),y
	rts

elseblock24763
elsedoneblock85506
	; Assigning single variable : p2
	
	; HandleVarBinopB16bit here
	ldy #0
	lda #1
rightvarInteger_var52111 = $54
	sta rightvarInteger_var52111
	sty rightvarInteger_var52111+1
	
	lda head+1
	sec
	sbc rightvarInteger_var52111+1
	tay
	lda head
	sec
	sbc rightvarInteger_var52111
	bcs wordAdd88777
	dey
wordAdd88777
	
	sta p2
	sty p2+1
	; Binary clause Simplified: LESS
	; Load pointer array
	ldy #1
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcs elsedoneblock40186
ConditionalTrueBlock35328
	; Binary clause Simplified: LESS
	; Load pointer array
	ldy #0
	lda (p2),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcs elsedoneblock45107
ConditionalTrueBlock41042

	; Binary clause Simplified: EQUALS
	lda snd1Freq
	; Compare with pure num / var optimization
	bne elsedoneblock59617
ConditionalTrueBlock71894
	; Assigning single variable : snd1Freq
	lda #180
	sta snd1Freq
elseblock46894
elsedoneblock59617
	jsr DecStopTime
	inc tailWait
	rts

elseblock46632
elsedoneblock45107
elseblock10012
elsedoneblock40186
	jsr DecStopTime
	; Binary clause Simplified: LESS
	; Load pointer array
	ldy #1
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcs elsedoneblock58053
ConditionalTrueBlock58082

	; Assigning single variable : hissDuration
	lda #2
	sta hissDuration
	; Assigning single variable : head
	lda #12
	ldy #0
	sta (head),y
	; Assigning single variable : head
	; WORD optimization: a=a+b
	lda head+0
	
	clc
	adc #1
	bcc WordAdd27998
	inc head+1
WordAdd27998
	sta head+0
	jsr EatFood
	; Assigning single variable : head
	lda #3
	ldy #0
	sta (head),y
	; Assigning single variable : headc
	; WORD optimization: a=a+b
	lda headc+0
	
	clc
	adc #1
	bcc WordAdd35680
	inc headc+1
WordAdd35680
	sta headc+0
	; Assigning single variable : headc
	lda #5
	ldy #0
	sta (headc),y
	rts

elseblock78337
elsedoneblock58053
	; Binary clause Simplified: LESS
	; Load pointer array
	ldy #0
	lda (p2),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcs elsedoneblock71564
ConditionalTrueBlock58641

	; Assigning single variable : hissDuration
	lda #2
	sta hissDuration
	; Assigning single variable : head
	lda #13
	ldy #0
	sta (head),y
	; Assigning single variable : head
	lda p2
	ldx p2+1
	sta head
	stx head+1
	jsr EatFood
	; Assigning single variable : head
	lda #5
	ldy #0
	sta (head),y
	; Assigning single variable : headc
	; WORD optimization: a=a+b
	lda headc+0
	
	sec
	sbc #1
	bcs WordAdd131
	dec headc+1
WordAdd131
	sta headc+0
	; Assigning single variable : headc
	lda #5
	ldy #0
	sta (headc),y
	rts

elseblock71006
elsedoneblock71564
	; Assigning single variable : gameState
	lda #1
	sta gameState
	; Assigning single variable : headc
	lda #2
	ldy #0
	sta (headc),y

	rts
	
	
	; ***********  Defining procedure : MoveNibblerHeadLeft
	;    Procedure type : User-defined procedure
	
MoveNibblerHeadLeft

	; Assigning single variable : p1
	
	; HandleVarBinopB16bit here
	ldy #0
	lda #1
rightvarInteger_var80709 = $54
	sta rightvarInteger_var80709
	sty rightvarInteger_var80709+1
	
	lda head+1
	sec
	sbc rightvarInteger_var80709+1
	tay
	lda head
	sec
	sbc rightvarInteger_var80709
	bcs wordAdd30242
	dey
wordAdd30242
	
	sta p1
	sty p1+1
	; Binary clause Simplified: LESS
	; Load pointer array
	ldy #0
	lda (p1),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcs elsedoneblock97756
ConditionalTrueBlock53825

	; Assigning single variable : head
	lda #7
	ldy #0
	sta (head),y
	; Assigning single variable : head
	lda p1
	ldx p1+1
	sta head
	stx head+1
	jsr EatFood
	; Assigning single variable : head
	lda #5
	ldy #0
	sta (head),y
	; Assigning single variable : headc
	; WORD optimization: a=a+b
	lda headc+0
	
	sec
	sbc #1
	bcs WordAdd94080
	dec headc+1
WordAdd94080
	sta headc+0
	; Assigning single variable : headc
	lda #5
	ldy #0
	sta (headc),y
	rts

elseblock17824
elsedoneblock97756
	; Assigning single variable : p1
	
	; HandleVarBinopB16bit here
	ldy #0
	lda #19
rightvarInteger_var4485 = $54
	sta rightvarInteger_var4485
	sty rightvarInteger_var4485+1
	
	lda head+1
	sec
	sbc rightvarInteger_var4485+1
	tay
	lda head
	sec
	sbc rightvarInteger_var4485
	bcs wordAdd28906
	dey
wordAdd28906
	
	sta p1
	sty p1+1
	; Assigning single variable : p2
	
	; HandleVarBinopB16bit here
	ldy #0
	lda #19
rightvarInteger_var86989 = $54
	sta rightvarInteger_var86989
	sty rightvarInteger_var86989+1
	
	lda head+1
	clc
	adc rightvarInteger_var86989+1
	tay
	lda head
	clc
	adc rightvarInteger_var86989
	bcc wordAdd53491
	iny
wordAdd53491
	
	sta p2
	sty p2+1
	; Binary clause Simplified: LESS
	; Load pointer array
	ldy #0
	lda (p1),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcs elsedoneblock98388
ConditionalTrueBlock11544
	; Binary clause Simplified: LESS
	; Load pointer array
	ldy #0
	lda (p2),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcs elsedoneblock113
ConditionalTrueBlock83598

	; Binary clause Simplified: EQUALS
	lda snd1Freq
	; Compare with pure num / var optimization
	bne elsedoneblock11886
ConditionalTrueBlock77440
	; Assigning single variable : snd1Freq
	lda #180
	sta snd1Freq
elseblock69451
elsedoneblock11886
	jsr DecStopTime
	inc tailWait
	rts

elseblock99356
elsedoneblock113
elseblock28656
elsedoneblock98388
	jsr DecStopTime
	; Binary clause Simplified: LESS
	; Load pointer array
	ldy #0
	lda (p1),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcs elsedoneblock86946
ConditionalTrueBlock45289

	; Assigning single variable : hissDuration
	lda #2
	sta hissDuration
	; Assigning single variable : head
	lda #12
	ldy #0
	sta (head),y
	; Assigning single variable : head
	lda p1
	ldx p1+1
	sta head
	stx head+1
	jsr EatFood
	; Assigning single variable : head
	lda #2
	ldy #0
	sta (head),y
	; Assigning single variable : headc
	; WORD optimization: a=a+b
	lda headc+0
	
	sec
	sbc #19
	bcs WordAdd42850
	dec headc+1
WordAdd42850
	sta headc+0
	; Assigning single variable : headc
	lda #5
	ldy #0
	sta (headc),y
	rts

elseblock53489
elsedoneblock86946
	; Binary clause Simplified: LESS
	; Load pointer array
	ldy #0
	lda (p2),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcs elsedoneblock3868
ConditionalTrueBlock42043

	; Assigning single variable : hissDuration
	lda #2
	sta hissDuration
	; Assigning single variable : head
	lda #10
	ldy #0
	sta (head),y
	; Assigning single variable : head
	lda p2
	ldx p2+1
	sta head
	stx head+1
	jsr EatFood
	; Assigning single variable : head
	lda #4
	ldy #0
	sta (head),y
	; Assigning single variable : headc
	; WORD optimization: a=a+b
	lda headc+0
	
	clc
	adc #19
	bcc WordAdd46676
	inc headc+1
WordAdd46676
	sta headc+0
	; Assigning single variable : headc
	lda #5
	ldy #0
	sta (headc),y
	rts

elseblock7986
elsedoneblock3868
	; Assigning single variable : gameState
	lda #1
	sta gameState
	; Assigning single variable : headc
	lda #2
	ldy #0
	sta (headc),y

	rts
	
	
	; ***********  Defining procedure : AnimateNibblerBody
	;    Procedure type : User-defined procedure
	
AnimateNibblerBody

	; 
	; ****** Inline assembler section

	; scroll vertical body up
	ldy $1c38+8
	ldx #0
Scroll1tA26758
	lda $1c39+8,x
	sta $1c38+8,x
	inx
	cpx #7
	bne Scroll1tA26758
	sty $1c3f+8
	
	; scroll vertical body down
	ldy $1c47+8
	ldx #7
Scroll2tA26758
	lda $1c40+7,x
	sta $1c41+7,x
	dex
	cpx #0
	bne Scroll2tA26758
	sty $1c40+8

	; scroll horizontal body right
	ldx #0
RightBitshiftA26758
	lda $1c28+8,x
	lsr
	ror $1c28+8,x
	inx
	cpx #8
	bne RightBitshiftA26758
	
	; scroll horizontal body left
	ldx #0
LeftBitshiftA16895
	lda $1c30+8,x
	asl
	rol $1c30+8,x
	inx
	cpx #8
	bne LeftBitshiftA16895
	
	

	rts
	
	
	; ***********  Defining procedure : TitleScreen
	;    Procedure type : User-defined procedure
	
	jmp block8253
titleText		dc.b	214
	dc.b	201
	dc.b	195
	dc.b	0
hewcoText		dc.b	200
	dc.b	197
	dc.b	215
	dc.b	195
	dc.b	207
	dc.b	28
	dc.b	213
	dc.b	203
	dc.b	63
	dc.b	50
	dc.b	48
	dc.b	49
	dc.b	57
	dc.b	0
rockText		dc.b	210
	dc.b	207
	dc.b	195
	dc.b	203
	dc.b	28
	dc.b	207
	dc.b	204
	dc.b	193
	dc.b	63
	dc.b	49
	dc.b	57
	dc.b	56
	dc.b	50
	dc.b	0
titlegfx	dc.b $0a, $06, $0b, $02, $010, $00, $00, $010
	dc.b $00, $00, $010, $00, $00, $00, $00, $00
	dc.b $00, $08, $00, $09, $08, $09, $05, $0b
	dc.b $09, $05, $0b, $09, $0a, $07, $0b, $0a
	dc.b $06, $03, $08, $00, $09, $08, $09, $00
	dc.b $08, $09, $00, $08, $09, $09, $06, $0d
	dc.b $08, $00, $00, $08, $00, $09, $08, $09
	dc.b $00, $08, $09, $00, $08, $09, $09, $00
	dc.b $00, $08, $00, $00, $0e, $00, $04, $0e
	dc.b $0c, $06, $0d, $0c, $06, $0d, $04, $0c
	dc.b $06, $03, $0e, $00, $01
block8253
TitleScreen

	; Assigning single variable : screenmemory
	lda #20
	ldx #30
	sta screenmemory
	stx screenmemory+1
	clc
	lda #<titleText
	adc #0
	ldy #>titleText
	sta print_text+0
	sty print_text+1
	ldx #11 ; optimized, look out for bugs
	jsr printstring
	; Assigning single variable : screenmemory
	lda #39
	ldx #30
	sta screenmemory
	stx screenmemory+1
	; Assigning single variable : p1
	ldx #150
	sta p1
	stx p1+1
	; Assigning single variable : r
	lda #0
	sta r
	; Assigning single variable : i
	sta i
for28215

	; Assigning single variable : t
	lda #0
	sta t
for92331

	; Assigning single variable : screenmemory
	; Load Byte array
	ldx r
	lda titlegfx,x
	
	ldy t
	sta (screenmemory),y
	; Assigning single variable : p1
	lda #5
	ldy t
	sta (p1),y
	; Assigning single variable : r
	inc r

	inc t
	lda #16
	cmp t
	bcs for92331
forLoopDone13522
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #19
	bcc WordAdd74446
	inc screenmemory+1
WordAdd74446
	sta screenmemory+0
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #19
	bcc WordAdd25413
	inc p1+1
WordAdd25413
	sta p1+0

	inc i
	lda #4
	cmp i
	bcs for28215
forLoopDone54595
	; Assigning single variable : screenmemory
	lda #127
	ldx #31
	sta screenmemory
	stx screenmemory+1
	clc
	lda #<rockText
	adc #0
	ldy #>rockText
	sta print_text+0
	sty print_text+1
	ldx #13 ; optimized, look out for bugs
	jsr printstring
	; Assigning single variable : screenmemory
	lda #165
	ldx #31
	sta screenmemory
	stx screenmemory+1
	clc
	lda #<hewcoText
	adc #0
	ldy #>hewcoText
	sta print_text+0
	sty print_text+1
	ldx #13 ; optimized, look out for bugs
	jsr printstring
	; Assigning single variable : screenmemory
	lda #253
	ldx #30
	sta screenmemory
	stx screenmemory+1
	ldy #0
	; integer assignment NodeVar
	ldy score+1 ; Next one ; optimized, look out for bugs
	lda score
	sta ipd_div_lo
	sty ipd_div_hi
	ldy #5 ; optimized, look out for bugs
printdecimal20424
	jsr init_printdecimal_div10 
	ora #$30
	sta (screenmemory),y
	dey
	bpl printdecimal20424
	; Assigning single variable : screenmemory
	lda #48
	ldy #6
	sta (screenmemory),y

	rts
	
	
	; ***********  Defining procedure : InstructionScreen
	;    Procedure type : User-defined procedure
	
	jmp block76250
textInst0	dc.b $089, $08e, $093, $094, $092, $095, $083, $094
	dc.b $089, $08f, $08e, $093, $04e
textInst1	dc.b $083, $08c, $085, $081, $092, $00, $085, $081
	dc.b $083, $088, $00, $08d, $081, $09a, $085, $012
	dc.b 
textInst2	dc.b $082, $085, $086, $08f, $092, $085, $00, $094
	dc.b $089, $08d, $085, $093, $00, $095, $090, $038
	dc.b 
textInst3	dc.b $084, $08f, $08e, $0a7, $094, $00, $08c, $085
	dc.b $094, $00, $08e, $089, $082, $082, $08c, $085
	dc.b $092, $017
textInst4	dc.b $082, $089, $094, $085, $00, $089, $094, $093
	dc.b $085, $08c, $086, $036
textInst5	dc.b $08e, $089, $082, $082, $08c, $085, $092, $00
	dc.b $084, $08f, $085, $093, $00, $08e, $08f, $094
	dc.b $014
textInst6	dc.b $093, $094, $08f, $090, $00, $081, $094, $00
	dc.b $083, $08f, $092, $08e, $085, $092, $093, $039
	dc.b 
textInst7	dc.b $08e, $089, $082, $082, $08c, $085, $092, $00
	dc.b $087, $092, $08f, $097, $093, $014
textInst8	dc.b $097, $088, $085, $08e, $00, $089, $094, $00
	dc.b $085, $081, $094, $093, $060
textInstd	dc.b $096, $089, $083, $00, $096, $085, $092, $093
	dc.b $089, $08f, $08e, $014
textInste	dc.b $082, $099, $00, $081, $08e, $084, $099, $00
	dc.b $088, $0fe
block76250
InstructionScreen

	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy i
	lda (p1),y
	
	; Compare with pure num / var optimization
	cmp #254
	bne elsedoneblock56618
ConditionalTrueBlock67744
	rts
elseblock71794
elsedoneblock56618
	; Binary clause Simplified: NOTEQUALS
	; Load pointer array
	ldy i
	lda (p1),y
	
	; Compare with pure num / var optimization
	beq elsedoneblock9439
ConditionalTrueBlock46647
	; Binary clause Simplified: LESS
	; Load pointer array
	ldy i
	lda (p1),y
	
	; Compare with pure num / var optimization
	cmp #100
	bcs elsedoneblock55148
ConditionalTrueBlock76229

	; Assigning single variable : p2
	; WORD optimization: a=a+b
	lda p2+0
	
	clc
	; Load pointer array
	ldy i
	adc (p1),y
	
	bcc WordAdd69906
	inc p2+1
WordAdd69906
	sta p2+0
	; Assigning single variable : r
	lda #0
	sta r
	; Assigning single variable : i
	inc i

elseblock24031
elsedoneblock55148
elseblock99267
elsedoneblock9439
	; Assigning single variable : p2
	; Load pointer array
	ldy i
	lda (p1),y
	
	ldy r
	sta (p2),y
	; Assigning single variable : i
	inc i
	; Assigning single variable : r
	inc r

	rts
	
	
	; ***********  Defining procedure : TitleSequence
	;    Procedure type : User-defined procedure
	
	jmp block91568
scrnNo	dc.b	
scrnDelay	dc.b	
block91568
TitleSequence

	; Assigning single variable : scrnNo
	lda #0
	sta scrnNo
while53954
	; Binary clause: EQUALS
	lda #1
	; Compare with pure num / var optimization
	cmp #1
	; BC done
	bne binaryclausefailed10062
	lda #1; success
	jmp binaryclausefinished75739
binaryclausefailed10062
	lda #0 ; failed state
binaryclausefinished75739
	cmp #1
	beq ConditionalTrueBlock62508
	jmp elsedoneblock21698
ConditionalTrueBlock62508

	jsr ClearTheScreen
	; Assigning single variable : scrnNo
	inc scrnNo
	; Binary clause Simplified: EQUALS
	lda scrnNo
	; Compare with pure num / var optimization
	cmp #3
	bne elsedoneblock30334
ConditionalTrueBlock4735
	; Assigning single variable : scrnNo
	lda #1
	sta scrnNo
elseblock82315
elsedoneblock30334
	; Binary clause Simplified: EQUALS
	lda scrnNo
	; Compare with pure num / var optimization
	cmp #1
	bne elsedoneblock87964
ConditionalTrueBlock62507
	jsr TitleScreen
elseblock17546
elsedoneblock87964
	; Assigning single variable : p1
	lda #<textInst0
	ldx #>textInst0
	sta p1
	stx p1+1
	; Assigning single variable : p2
	lda #0
	ldx #30
	sta p2
	stx p2+1
	; Assigning single variable : i
	sta i
	; Assigning single variable : r
	lda #3
	sta r
	; Assigning single variable : numberOfRastersToWait
	lda #15
	sta numberOfRastersToWait
	; Assigning single variable : scrnDelay
	lda #0
	sta scrnDelay
while60703
	; Binary clause Simplified: NOTEQUALS
	lda scrnDelay
	; Compare with pure num / var optimization
	cmp #200
	beq elsedoneblock1842
ConditionalTrueBlock67037

	; Binary clause Simplified: EQUALS
	lda scrnNo
	; Compare with pure num / var optimization
	cmp #2
	bne elsedoneblock83858
ConditionalTrueBlock19117
	jsr InstructionScreen
elseblock8766
elsedoneblock83858
	; Binary clause Simplified: EQUALS
	lda scrnNo
	; Compare with pure num / var optimization
	cmp #1
	bne elsedoneblock75109
ConditionalTrueBlock40259
	jsr AnimateNibblerBody
elseblock60186
elsedoneblock75109
	jsr ReadJoystick
	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda JoyResult
	and #16
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #16
	bne elsedoneblock43702
ConditionalTrueBlock64030
	rts
elseblock34495
elsedoneblock43702
	; Binary clause Simplified: NOTEQUALS
	lda $c5
	; Compare with pure num / var optimization
	cmp #64
	beq elsedoneblock44874
ConditionalTrueBlock97813
	rts
elseblock40492
elsedoneblock44874
	jsr WaitForNRasters
	; Assigning single variable : scrnDelay
	inc scrnDelay

	jmp while60703
elseblock15033
elsedoneblock1842

	jmp while53954
elseblock44369
elsedoneblock21698

	rts
	
	
	; ***********  Defining procedure : GameStarting
	;    Procedure type : User-defined procedure
	
GameStarting

	jsr SetupPlayer
	; Assigning single variable : numberOfRastersToWait
	lda #30
	sta numberOfRastersToWait
	; Assigning single variable : r
	lda #0
	sta r
while68394
	; Binary clause Simplified: LESS
	lda r
	; Compare with pure num / var optimization
	cmp #6
	bcs elsedoneblock3163
ConditionalTrueBlock40403

	; Assigning single variable : tail
	lda #62
	ldy r
	sta (tail),y
	; Assigning single variable : tailc
	lda #1
	ldy r
	sta (tailc),y
	; Assigning single variable : r
	inc r
	; Assigning memory location (poke replacement)
	; Assigning single variable : $900b
	; 8 bit binop
	; Add/sub where right value is constant number
	; Right is PURE NUMERIC
	; 8 bit mul of power 2
	
	lda r
	
	asl
	asl
	
	clc
	adc #170
	 ; end add / sub var with constant
	
	sta $900b
	jsr WaitForNRasters

	jmp while68394
elseblock84506
elsedoneblock3163
	; Assigning memory location (poke replacement)
	; Assigning single variable : $900c
	lda #220
	sta $900c
	jsr WaitForNRasters
	; Assigning memory location (poke replacement)
	; Assigning single variable : $900c
	lda #0
	sta $900c
	; Assigning single variable : r
	sta r
while88339
	; Binary clause Simplified: LESS
	lda r
	; Compare with pure num / var optimization
	cmp #6
	bcs elsedoneblock88549
ConditionalTrueBlock17934

	; Assigning single variable : tail
	lda #6
	ldy r
	sta (tail),y
	; Assigning single variable : tailc
	ldy r
	sta (tailc),y
	; Binary clause Simplified: EQUALS
	lda r
	; Compare with pure num / var optimization
	bne elsedoneblock62327
ConditionalTrueBlock14526
	; Assigning single variable : tail
	lda #15
	ldy r
	sta (tail),y
elseblock6748
elsedoneblock62327
	; Binary clause Simplified: EQUALS
	lda r
	; Compare with pure num / var optimization
	cmp #5
	bne elsedoneblock38445
ConditionalTrueBlock66796
	; Assigning single variable : tail
	lda #3
	ldy r
	sta (tail),y
elseblock49809
elsedoneblock38445
	; Assigning single variable : r
	inc r
	; Assigning memory location (poke replacement)
	; Assigning single variable : $900b
	; 8 bit binop
	; Add/sub where right value is constant number
	; Right is PURE NUMERIC
	; 8 bit mul of power 2
	
	lda r
	
	asl
	asl
	
	clc
	adc #175
	 ; end add / sub var with constant
	
	sta $900b
	jsr WaitForNRasters

	jmp while88339
elseblock51352
elsedoneblock88549
	; Assigning memory location (poke replacement)
	; Assigning single variable : $900c
	lda #230
	sta $900c
	jsr WaitForNRasters
	; Assigning memory location (poke replacement)
	; Assigning single variable : $900c
	lda #0
	sta $900c
	; Assigning single variable : r
	sta r
while64680
	; Binary clause Simplified: LESS
	lda r
	; Compare with pure num / var optimization
	cmp #6
	bcs elsedoneblock16032
ConditionalTrueBlock91002

	; Assigning single variable : tailc
	lda #5
	ldy r
	sta (tailc),y
	; Binary clause Simplified: EQUALS
	lda r
	; Compare with pure num / var optimization
	bne elsedoneblock78440
ConditionalTrueBlock17553
	; Assigning single variable : tailc
	lda #7
	ldy r
	sta (tailc),y
elseblock65871
elsedoneblock78440
	; Assigning single variable : r
	inc r
	; Assigning memory location (poke replacement)
	; Assigning single variable : $900b
	; 8 bit binop
	; Add/sub where right value is constant number
	; Right is PURE NUMERIC
	; 8 bit mul of power 2
	
	lda r
	
	asl
	asl
	
	clc
	adc #180
	 ; end add / sub var with constant
	
	sta $900b
	jsr WaitForNRasters

	jmp while64680
elseblock82912
elsedoneblock16032
	; Assigning memory location (poke replacement)
	; Assigning single variable : $900b
	lda #0
	sta $900b
	; Assigning memory location (poke replacement)
	; Assigning single variable : $900c
	lda #240
	sta $900c
	jsr WaitForNRasters
	; Assigning memory location (poke replacement)
	; Assigning single variable : $900c
	lda #0
	sta $900c

	rts
	
	
	; ***********  Defining procedure : CountBonus
	;    Procedure type : User-defined procedure
	
CountBonus

	jsr DrawUIBonus
while36010
	; Binary clause Simplified: NOTEQUALS
	lda time
	; Compare with pure num / var optimization
	beq elsedoneblock98337
ConditionalTrueBlock97111

	; Assigning single variable : time
	dec time
	; Assigning single variable : score
	ldy #0
	; WORD optimization: a=a+b
	lda score+0
	
	clc
	adc #1
	bcc WordAdd78661
	inc score+1
WordAdd78661
	sta score+0
	jsr DrawUIScore
	jsr DrawUITime
	; Assigning memory location (poke replacement)
	; Assigning single variable : $900c
	lda #150
	sta $900c
	; Assigning single variable : numberOfRastersToWait
	lda #5
	sta numberOfRastersToWait
	jsr WaitForNRasters
	; Assigning memory location (poke replacement)
	; Assigning single variable : $900c
	lda #0
	sta $900c
	jsr WaitForNRasters

	jmp while36010
elseblock84148
elsedoneblock98337

	rts
	
	
	; ***********  Defining procedure : LoseLife
	;    Procedure type : User-defined procedure
	
LoseLife

	; Assigning memory location (poke replacement)
	; Assigning single variable : $900d
	lda #170
	sta $900d
	; Assigning single variable : tailWait
	lda #0
	sta tailWait
	; Assigning single variable : numberOfRastersToWait
	lda #10
	sta numberOfRastersToWait
	; Assigning single variable : r
	lda #0
	sta r
while7074
	; Binary clause Simplified: EQUALS
	lda r
	; Compare with pure num / var optimization
	bne elsedoneblock71754
ConditionalTrueBlock81324

	jsr MoveNibblerTail
	jsr WaitForNRasters
	; Binary clause Simplified: EQUALS
	lda head
	
binary_clause_temp_var3183 = $54
	sta binary_clause_temp_var3183
	lda tail
	
binary_clause_temp_2_var23786 = $56
	sta binary_clause_temp_2_var23786
	lda binary_clause_temp_var3183
	cmp binary_clause_temp_2_var23786
	bne elsedoneblock12754
ConditionalTrueBlock89693
	; Binary clause Simplified: EQUALS
	lda head+1
	
binary_clause_temp_var24046 = $54
	sta binary_clause_temp_var24046
	lda tail+1
	
binary_clause_temp_2_var3630 = $56
	sta binary_clause_temp_2_var3630
	lda binary_clause_temp_var24046
	cmp binary_clause_temp_2_var3630
	bne elsedoneblock36636
ConditionalTrueBlock52167
	; Assigning single variable : r
	lda #1
	sta r
elseblock87534
elsedoneblock36636
elseblock21825
elsedoneblock12754

	jmp while7074
elseblock8342
elsedoneblock71754
	; Assigning single variable : head
	lda #0
	ldy #0
	sta (head),y
	; Assigning single variable : lives
	dec lives
	jsr DrawUILives
	; Assigning memory location (poke replacement)
	; Assigning single variable : $900d
	lda #0
	sta $900d

	rts
	
	
	; ***********  Defining procedure : ScrollScreenDown
	;    Procedure type : User-defined procedure
	
ScrollScreenDown

	; Assigning single variable : numberOfRastersToWait
	lda #10
	sta numberOfRastersToWait
	; Assigning single variable : r
	lda #1
	sta r
	; Assigning single variable : i
	sta i
for71125

	; Assigning memory location (poke replacement)
	; Assigning single variable : $9001
	; 8 bit binop
	; Add/sub where right value is constant number
	; Peek
	lda $9001 + $0
	
	clc
	adc r
	 ; end add / sub var with constant
	
	sta $9001
	; Assigning single variable : r
	inc r
	jsr WaitForNRasters

	inc i
	lda #20
	cmp i
	bcs for71125
forLoopDone3157

	rts
	
	
	; ***********  Defining procedure : ScrollScreenUp
	;    Procedure type : User-defined procedure
	
ScrollScreenUp

	; Assigning single variable : numberOfRastersToWait
	lda #10
	sta numberOfRastersToWait
	; Assigning single variable : r
	lda #20
	sta r
	; Assigning single variable : i
	lda #1
	sta i
for43658

	; Assigning memory location (poke replacement)
	; Assigning single variable : $9001
	; 8 bit binop
	; Add/sub where right value is constant number
	; Peek
	lda $9001 + $0
	
	sec
	sbc r
	 ; end add / sub var with constant
	
	sta $9001
	; Assigning single variable : r
	dec r
	jsr WaitForNRasters

	inc i
	lda #20
	cmp i
	bcs for43658
forLoopDone33764

	rts
	
	
	; ***********  Defining procedure : GameLoop
	;    Procedure type : User-defined procedure
	
GameLoop

	; Assigning single variable : r
	lda #0
	sta r
while11564
	; Binary clause: EQUALS
	lda r
	; Compare with pure num / var optimization
	; BC done
	bne binaryclausefailed98187
	lda #1; success
	jmp binaryclausefinished34017
binaryclausefailed98187
	lda #0 ; failed state
binaryclausefinished34017
	cmp #1
	beq ConditionalTrueBlock75986
	jmp elsedoneblock93503
ConditionalTrueBlock75986

	; Binary clause Simplified: EQUALS
	lda moveTime
	; Compare with pure num / var optimization
	bne elsedoneblock19072
ConditionalTrueBlock77140

	; Assigning single variable : moveTime
	lda moveTimeReset
	sta moveTime
	jsr NibblerControl
	; Binary clause Simplified: EQUALS
	lda justMoved
	; Compare with pure num / var optimization
	bne elsedoneblock20488
ConditionalTrueBlock72944
	jsr MoveNibblerHead
elseblock94528
elsedoneblock20488
	jsr MoveNibblerTail
	; Binary clause Simplified: EQUALS
	lda gameState
	; Compare with pure num / var optimization
	cmp #1
	bne elsedoneblock85544
ConditionalTrueBlock86034
	rts
elseblock22037
elsedoneblock85544
	; Binary clause Simplified: EQUALS
	lda numFood
	; Compare with pure num / var optimization
	bne elsedoneblock72606
ConditionalTrueBlock10655
	rts
elseblock47247
elsedoneblock72606

elseblock85770
elsedoneblock19072
	dec moveTime
	inc animTime
	; Binary clause Simplified: EQUALS
	lda animTime
	; Compare with pure num / var optimization
	cmp #5
	bne elsedoneblock98403
ConditionalTrueBlock96083

	jsr AnimateNibblerBody
	; Assigning single variable : animTime
	lda #0
	sta animTime
	jsr PlaySND1
	jsr PlaySND2
	jsr PlaySFX

elseblock13002
elsedoneblock98403
	inc timeTick
	; Binary clause Simplified: EQUALS
	lda timeTick
	; Compare with pure num / var optimization
	cmp #60
	bne elsedoneblock39021
ConditionalTrueBlock95892

	; Assigning single variable : timeTick
	lda #0
	sta timeTick
	; Binary clause Simplified: EQUALS
	lda stopTime
	; Compare with pure num / var optimization
	bne elsedoneblock89390
ConditionalTrueBlock20958
	; Assigning single variable : timeTick
	lda #55
	sta timeTick
elseblock90057
elsedoneblock89390
	dec time
	jsr DrawUITime
	; Binary clause Simplified: EQUALS
	lda time
	; Compare with pure num / var optimization
	cmp #12
	bne elsedoneblock97797
ConditionalTrueBlock66768
	; Assigning single variable : timeColor
	lda #2
	sta timeColor
	jsr ColourTime
elseblock30540
elsedoneblock97797
	; Binary clause Simplified: EQUALS
	lda time
	; Compare with pure num / var optimization
	bne elsedoneblock93829
ConditionalTrueBlock93053

	; Assigning single variable : gameState
	lda #1
	sta gameState
	rts

elseblock47223
elsedoneblock93829

elseblock75205
elsedoneblock39021
	; wait for raster
	ldx #130
	cpx $9004
	bne *-3

	jmp while11564
elseblock86052
elsedoneblock93503

	rts
	
	
	; ***********  Defining procedure : ResetVars
	;    Procedure type : User-defined procedure
	
ResetVars

	; Assigning single variable : time
	lda #99
	sta time
	; Assigning single variable : stopTime
	lda #10
	sta stopTime
	; Assigning single variable : moveTime
	lda moveTimeReset
	sta moveTime
	; Assigning single variable : animTime
	lda #1
	sta animTime
	; Assigning single variable : tailGrow
	sta tailGrow
	; Assigning single variable : tailWait
	lda #0
	sta tailWait

	rts
block89383

	jsr ScreenSetup
	jsr SetupJoystick
while1533
	; Binary clause: EQUALS
	lda #1
	; Compare with pure num / var optimization
	cmp #1
	; BC done
	bne binaryclausefailed54228
	lda #1; success
	jmp binaryclausefinished8251
binaryclausefailed54228
	lda #0 ; failed state
binaryclausefinished8251
	cmp #1
	beq ConditionalTrueBlock59316
	jmp elsedoneblock50259
ConditionalTrueBlock59316

	jsr TitleSequence
	; Assigning single variable : wave
	lda #0
	sta wave
	; Assigning single variable : levelId
	sta levelId
	; Assigning single variable : lives
	lda #5
	sta lives
	; Assigning single variable : score
	ldy #0
	lda #0
	sta score
	sty score+1
	; Assigning single variable : numFood
	sta numFood
	; Assigning single variable : moveTimeReset
	lda #11
	sta moveTimeReset
while23969
	; Binary clause: NOTEQUALS
	lda lives
	; Compare with pure num / var optimization
	; BC done
	beq binaryclausefailed57224
	lda #1; success
	jmp binaryclausefinished66146
binaryclausefailed57224
	lda #0 ; failed state
binaryclausefinished66146
	cmp #1
	beq ConditionalTrueBlock62697
	jmp elsedoneblock9859
ConditionalTrueBlock62697

	jsr ScrollScreenDown
	; Assigning single variable : timeColor
	lda #1
	sta timeColor
	jsr ColourTime
	; Binary clause Simplified: EQUALS
	lda numFood
	; Compare with pure num / var optimization
	bne elsedoneblock53661
ConditionalTrueBlock42916

	jsr ClearTheScreen
	jsr DrawUITextLabels
	jsr DrawUIScore
	jsr DrawUILives
	jsr DrawLevelFrame
	jsr DrawLevelMaze
	jsr DrawLevelFood
	; Assigning single variable : wave
	inc wave
	; Assigning single variable : levelId
	inc levelId
	; Binary clause Simplified: EQUALS
	lda levelId
	; Compare with pure num / var optimization
	cmp #10
	bne elsedoneblock20696
ConditionalTrueBlock46870
	; Assigning single variable : levelId
	lda #0
	sta levelId
elseblock66289
elsedoneblock20696
	jsr DrawUIWave
	; Binary clause Simplified: EQUALS
	lda levelId
	; Compare with pure num / var optimization
	cmp #2
	bne elsedoneblock74863
ConditionalTrueBlock15424
	; Assigning single variable : moveTimeReset
	dec moveTimeReset
elseblock66332
elsedoneblock74863
	; Binary clause Simplified: EQUALS
	lda levelId
	; Compare with pure num / var optimization
	cmp #4
	bne elsedoneblock88018
ConditionalTrueBlock91847
	; Assigning single variable : lives
	inc lives
elseblock66392
elsedoneblock88018
	; Binary clause Simplified: EQUALS
	lda levelId
	; Compare with pure num / var optimization
	cmp #8
	bne elsedoneblock43983
ConditionalTrueBlock64825
	; Assigning single variable : moveTimeReset
	dec moveTimeReset
elseblock97004
elsedoneblock43983
	; Binary clause Simplified: EQUALS
	lda levelId
	; Compare with pure num / var optimization
	cmp #10
	bne elsedoneblock56387
ConditionalTrueBlock52878
	; Assigning single variable : lives
	inc lives
elseblock60908
elsedoneblock56387
	; Binary clause Simplified: LESS
	lda moveTimeReset
	; Compare with pure num / var optimization
	cmp #5
	bcs elsedoneblock55351
ConditionalTrueBlock95651
	; Assigning single variable : moveTimeReset
	lda #5
	sta moveTimeReset
elseblock73032
elsedoneblock55351

elseblock43847
elsedoneblock53661
	jsr DrawUITime
	jsr DrawUILives
	jsr ScrollScreenUp
	jsr GameStarting
	jsr GameLoop
	; Assigning single variable : snd1Freq
	lda #0
	sta snd1Freq
	; Assigning memory location (poke replacement)
	; Assigning single variable : $900a
	sta $900a
	; Assigning memory location (poke replacement)
	; Assigning single variable : $900b
	sta $900b
	; Assigning memory location (poke replacement)
	; Assigning single variable : $900d
	sta $900d
	; Binary clause Simplified: EQUALS
	lda numFood
	; Compare with pure num / var optimization
	bne elsedoneblock48050
ConditionalTrueBlock72743
	jsr CountBonus
elseblock38326
elsedoneblock48050
	; Binary clause Simplified: EQUALS
	lda gameState
	; Compare with pure num / var optimization
	cmp #1
	bne elsedoneblock55229
ConditionalTrueBlock77950
	jsr LoseLife
elseblock20498
elsedoneblock55229
	jsr ResetVars
	; Assigning single variable : gameState
	lda #0
	sta gameState

	jmp while23969
elseblock313
elsedoneblock9859

	jmp while1533
elseblock56510
elsedoneblock50259

EndSymbol
	org $1c00
charset
	incbin "/home/leuat/code/TRSE/Publish/tutorials/VIC20/VicNibbler///bin/chars.bin"
