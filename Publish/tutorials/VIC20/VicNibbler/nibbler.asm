 processor 6502

	ORG $401
	.byte    $0E, $08, $0A, $00, $9E, $20, $28
	.byte   $31,$30,$34,$30
	.byte    $29, $00, $00, $00

	ORG $410

Nibbler
	jmp block41
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
levelarray	dc.b $0, $1, $2, $3, $4, $5, $6, $7
	dc.b $8, $9
foodarray	dc.b $0, $1, $2, $3, $4, $0, $1, $2
	dc.b $3, $4
colourarray	dc.b $7, $3, $4, $5, $2, $7, $5, $4
	dc.b $3, $2
	
	
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
clearloop6334
	sta $0000+$1dff,x
	sta $00FD+$1dff,x
	dex
	bne clearloop6334
	; Clear screen with offset
	lda #1
	ldx #$FD
clearloop26500
	sta $0000+$95ff,x
	sta $00FD+$95ff,x
	dex
	bne clearloop26500

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
dtloop15724
	tay
	lda scrm,x
	inx
	inx
	clc
	adc #$13
	bcc dtnooverflow11478
	iny
dtnooverflow11478
	sta scrm,x
	tya
	sta scrm+1,x
	cpx #$30
	bcc dtloop15724
	; ----------
	; DefineAddressTable address, StartValue, IncrementValue, TableSize
	ldy #>$9600
	lda #<$9600
	ldx #0
	sta clrm,x   ; Address of table
	tya
	sta clrm+1,x
dtloop29358
	tay
	lda clrm,x
	inx
	inx
	clc
	adc #$13
	bcc dtnooverflow26962
	iny
dtnooverflow26962
	sta clrm,x
	tya
	sta clrm+1,x
	cpx #$30
	bcc dtloop29358

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
	
	jmp block5705
JoyResult	dc.b	$0
block5705
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
	
	jmp block23281
numberOfRastersToWait	dc.b	$1
block23281
WaitForNRasters

	; Assigning single variable : s
	lda #1
	sta s
for16827
	; wait for raster
	ldx #130
	cpx $9004
	bne *-3
	inc s
	lda numberOfRastersToWait
	cmp s
	bcs for16827
forLoopDone491

	rts
	
	
	; ***********  Defining procedure : PlaySFX
	;    Procedure type : User-defined procedure
	
	jmp block11942
hissDuration	dc.b	$0
block11942
PlaySFX

	; Binary clause Simplified: EQUALS
	lda hissDuration
	; Compare with pure num / var optimization
	bne elsedoneblock14604
ConditionalTrueBlock5436

	rts

elsedoneblock14604
	; Binary clause Simplified: EQUALS
	lda hissDuration
	; Compare with pure num / var optimization
	cmp #1
	bne elsedoneblock18716
ConditionalTrueBlock12382

	; Assigning memory location (poke replacement)
	; Assigning single variable : $900d
	lda #0
	sta $900d
	; Assigning single variable : hissDuration
	sta hissDuration
	rts

elsedoneblock18716
	; Assigning memory location (poke replacement)
	; Assigning single variable : $900d
	lda #253
	sta $900d
	; Assigning single variable : hissDuration
	dec hissDuration

	rts
	
	
	; ***********  Defining procedure : PlaySND1
	;    Procedure type : User-defined procedure
	
	jmp block5447
snd1Freq	dc.b	$0
block5447
PlaySND1

	; Binary clause Simplified: GREATER
	lda snd1Freq
	; Compare with pure num / var optimization
	sbc #0
	cmp #130
	bcc elsedoneblock1869
ConditionalTrueBlock14771
	; Assigning single variable : snd1Freq
	dec snd1Freq
elsedoneblock1869
	; Assigning memory location (poke replacement)
	; Assigning single variable : $900a
	lda snd1Freq
	sta $900a

	rts
	
	
	; ***********  Defining procedure : PlaySND2
	;    Procedure type : User-defined procedure
	
	jmp block26299
snd2Freq	dc.b	$0
block26299
PlaySND2

	; Binary clause Simplified: EQUALS
	lda snd2Freq
	; Compare with pure num / var optimization
	cmp #240
	bne elsedoneblock23811
ConditionalTrueBlock9894

	; Assigning single variable : snd2Freq
	lda #0
	sta snd2Freq
	; Assigning memory location (poke replacement)
	; Assigning single variable : $900b
	sta $900b
	rts

elsedoneblock23811
	; Binary clause Simplified: NOTEQUALS
	lda snd2Freq
	; Compare with pure num / var optimization
	beq elsedoneblock7711
ConditionalTrueBlock4664
	; Assigning single variable : snd2Freq
	; Optimizer: a = a +/- b
	lda snd2Freq
	clc
	adc #10
	sta snd2Freq
elsedoneblock7711
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
for27644

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
	bcs for27644
forLoopDone32757
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
	bcc WordAdd12859
	inc p1+1
WordAdd12859
	sta p1+0
	; Assigning single variable : i
	lda #1
	sta i
for8723

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
	bcc WordAdd3035
	inc p1+1
WordAdd3035
	sta p1+0

	inc i
	lda #17
	cmp i
	bcs for8723
forLoopDone778
	; Assigning single variable : p1
	lda #76
	ldx #150
	sta p1
	stx p1+1
	; Assigning single variable : t
	lda #0
	sta t
for22190

	; Assigning single variable : i
	lda #0
	sta i
for23805

	; Assigning single variable : p1
	; Load Byte array
	ldx levelId
	lda colourarray,x
	
	ldy i
	sta (p1),y

	inc i
	lda #18
	cmp i
	bcs for23805
forLoopDone6729
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #19
	bcc WordAdd15350
	inc p1+1
WordAdd15350
	sta p1+0

	inc t
	lda #18
	cmp t
	bcs for22190
forLoopDone22648

	rts
	
	
	; ***********  Defining procedure : DrawLevelMaze
	;    Procedure type : User-defined procedure
	
	jmp block15006
level0	dc.b $0, $75, $54, $75, $5, $7d, $40, $5f
	dc.b $0, $7d, $1, $75, $54, $75, $5, $7d
	dc.b $0, $40, $55
level1	dc.b $0, $7d, $1, $77, $50, $77, $0, $df
	dc.b $0, $7f, $1, $7d, $4, $55, $51, $5f
	dc.b $0, $4, $0
level2	dc.b $0, $55, $0, $55, $0, $55, $0, $55
	dc.b $0, $55, $0, $55, $0, $55, $0, $55
	dc.b $0, $0, $0
level3	dc.b $0, $75, $4, $75, $40, $5d, $11, $55
	dc.b $4, $55, $11, $5d, $40, $75, $4, $75
	dc.b $0, $55, $55
level4	dc.b $0, $5d, $11, $75, $5, $75, $41, $5f
	dc.b $0, $5f, $41, $75, $5, $75, $11, $5d
	dc.b $0, $0, $0
level5	dc.b $0, $7d, $0, $7d, $45, $7d, $40, $5f
	dc.b $40, $75, $5, $75, $54, $75, $5, $7d
	dc.b $0, $54, $5
level6	dc.b $0, $7d, $1, $77, $50, $77, $0, $ff
	dc.b $0, $7f, $1, $7d, $4, $75, $51, $77
	dc.b $0, $4, $0
level7	dc.b $0, $55, $4, $55, $0, $d5, $0, $77
	dc.b $0, $55, $0, $55, $10, $55, $0, $55
	dc.b $10, $10, $4
level8	dc.b $4, $75, $4, $75, $40, $5d, $10, $57
	dc.b $5, $57, $10, $5d, $40, $75, $4, $75
	dc.b $0, $50, $5
level9	dc.b $0, $55, $11, $75, $5, $75, $44, $5f
	dc.b $0, $5f, $44, $75, $0, $77, $15, $57
	dc.b $0, $0, $0
block15006
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
	beq elsedoneblock19629
ConditionalTrueBlock24393
	; Assigning single variable : i
	lda #1
	sta i
for26308
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #19
	bcc WordAdd5537
	inc p1+1
WordAdd5537
	sta p1+0
	inc i
	lda r
	cmp i
	bcs for26308
forLoopDone24626
elsedoneblock19629
	; Assigning single variable : p2
	lda #95
	ldx #30
	sta p2
	stx p2+1
	; Assigning single variable : i
	lda #1
	sta i
for21538
forLoopFix24767

	; Assigning single variable : t
	lda #1
	sta t
for12052

	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda #8
	sec
	sbc t
	 ; end add / sub var with constant
	
	tax
	lda #1
shiftbit24946
	cpx #0
	beq shiftbitdone27506
	asl
	dex
	jmp shiftbit24946
shiftbitdone27506
bitmask_var13030 = $54
	sta bitmask_var13030
	; Load pointer array
	ldy #0
	lda (p1),y
	
	and bitmask_var13030
	cmp bitmask_var13030
	bne getbit_false18588
	lda #1
	jmp getbit_done6422
getbit_false18588
	lda #0
getbit_done6422
	
	; Compare with pure num / var optimization
	cmp #1
	bne elsedoneblock9758
ConditionalTrueBlock8909

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

elsedoneblock9758

	inc t
	lda #8
	cmp t
	bcs for12052
forLoopDone27753
	; Assigning single variable : p2
	; WORD optimization: a=a+b
	lda p2+0
	
	clc
	adc #19
	bcc WordAdd29168
	inc p2+1
WordAdd29168
	sta p2+0
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #1
	bcc WordAdd900
	inc p1+1
WordAdd900
	sta p1+0

	inc i
	lda #17
	cmp i
	bcc forLoopDone23655
forLoopNotDone15574
	jmp for21538
forLoopDone23655
	; Assigning single variable : p2
	lda #95
	ldx #30
	sta p2
	stx p2+1
	; Assigning single variable : t
	lda #1
	sta t
for32591

	; Assigning single variable : i
	lda #1
	sta i
for3728

	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda #8
	sec
	sbc i
	 ; end add / sub var with constant
	
	tax
	lda #1
shiftbit32609
	cpx #0
	beq shiftbitdone14989
	asl
	dex
	jmp shiftbit32609
shiftbitdone14989
bitmask_var32702 = $54
	sta bitmask_var32702
	; Load pointer array
	ldy #0
	lda (p1),y
	
	and bitmask_var32702
	cmp bitmask_var32702
	bne getbit_false11008
	lda #1
	jmp getbit_done5844
getbit_false11008
	lda #0
getbit_done5844
	
	; Compare with pure num / var optimization
	cmp #1
	bne elsedoneblock30303
ConditionalTrueBlock31556
	; Assigning single variable : p2
	lda #28
	ldy #9
	sta (p2),y
elsedoneblock30303
	; Assigning single variable : p2
	; WORD optimization: a=a+b
	lda p2+0
	
	clc
	adc #19
	bcc WordAdd20485
	inc p2+1
WordAdd20485
	sta p2+0

	inc i
	lda #8
	cmp i
	bcs for3728
forLoopDone20600
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #1
	bcc WordAdd3093
	inc p1+1
WordAdd3093
	sta p1+0

	inc t
	lda #2
	cmp t
	bcs for32591
forLoopDone18127
	jsr RenderMazeShapes

	rts
	
	
	; ***********  Defining procedure : DrawLevelFood
	;    Procedure type : User-defined procedure
	
	jmp block14343
foodLevel0	dc.b $22, $0, $88, $0, $2, $0, $90, $1
	dc.b $10, $0, $20, $0, $1, $8a, $0, $80
	dc.b $0
foodLevel1	dc.b $22, $0, $88, $0, $2, $0, $90, $1
	dc.b $10, $0, $20, $0, $1, $a, $80, $80
	dc.b $0
foodLevel2	dc.b $22, $0, $88, $0, $2, $0, $90, $1
	dc.b $10, $0, $20, $0, $1, $8a, $80, $0
	dc.b $0
foodLevel3	dc.b $21, $2, $21, $0, $a1, $2, $0, $0
	dc.b $9, $0, $0, $2, $81, $8, $21, $0
	dc.b $0
foodLevel4	dc.b $0, $0, $28, $81, $8, $0, $22, $0
	dc.b $11, $0, $22, $0, $8, $81, $28, $0
	dc.b $0
block14343
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
	beq elsedoneblock9503
ConditionalTrueBlock1587
	; Assigning single variable : i
	lda #1
	sta i
for19589
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #17
	bcc WordAdd23622
	inc p1+1
WordAdd23622
	sta p1+0
	inc i
	lda r
	cmp i
	bcs for19589
forLoopDone27157
elsedoneblock9503
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
for18538
forLoopFix2510

	; Assigning single variable : t
	lda #1
	sta t
for14688
forLoopFix9832

	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda #8
	sec
	sbc t
	 ; end add / sub var with constant
	
	tax
	lda #1
shiftbit10555
	cpx #0
	beq shiftbitdone3434
	asl
	dex
	jmp shiftbit10555
shiftbitdone3434
bitmask_var16549 = $54
	sta bitmask_var16549
	; Load pointer array
	ldy #0
	lda (p1),y
	
	and bitmask_var16549
	cmp bitmask_var16549
	bne getbit_false28692
	lda #1
	jmp getbit_done21425
getbit_false28692
	lda #0
getbit_done21425
	
	; Compare with pure num / var optimization
	cmp #1
	bne elsedoneblock31329
ConditionalTrueBlock17189

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

elsedoneblock31329

	inc t
	lda #7
	cmp t
	bcc forLoopDone30932
forLoopNotDone4169
	jmp for14688
forLoopDone30932
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
	bne elsedoneblock21718
ConditionalTrueBlock30145

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

elsedoneblock21718
	; Assigning single variable : p3
	; WORD optimization: a=a+b
	lda p3+0
	
	clc
	adc #19
	bcc WordAdd12423
	inc p3+1
WordAdd12423
	sta p3+0
	; Assigning single variable : p2
	; WORD optimization: a=a+b
	lda p2+0
	
	clc
	adc #19
	bcc WordAdd16279
	inc p2+1
WordAdd16279
	sta p2+0
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #1
	bcc WordAdd25996
	inc p1+1
WordAdd25996
	sta p1+0

	inc i
	lda #17
	cmp i
	bcc forLoopDone21003
forLoopNotDone26869
	jmp for18538
forLoopDone21003

	rts
	
	
	; ***********  Defining procedure : RenderMazeShapes
	;    Procedure type : User-defined procedure
	
	jmp block16687
mazeTranslate	dc.b $1c, $20, $1e, $17, $1d, $16, $12, $19
	dc.b $1f, $13, $15, $1b, $14, $1a, $18, $0
	dc.b 
block16687
RenderMazeShapes

	; Assigning single variable : p1
	lda #76
	ldx #30
	sta p1
	stx p1+1
	; Assigning single variable : t
	lda #1
	sta t
for12529
forLoopFix750

	; Assigning single variable : i
	lda #1
	sta i
for303
forLoopFix1515

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
	bne binaryclausefailed26630
	lda #1; success
	jmp binaryclausefinished24114
binaryclausefailed26630
	lda #0 ; failed state
binaryclausefinished24114
	cmp #1
	beq ConditionalTrueBlock14700
	jmp elsedoneblock26264
ConditionalTrueBlock14700

	; Binary clause Simplified: NOTEQUALS
	; Load pointer array
	ldy i
	lda (p1),y
	
	; Compare with pure num / var optimization
	beq elsedoneblock21293
ConditionalTrueBlock23556
	inc r
elsedoneblock21293
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
	beq elsedoneblock248
ConditionalTrueBlock25345
	; Assigning single variable : r
	; Optimizer: a = a +/- b
	lda r
	clc
	adc #2
	sta r
elsedoneblock248
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
	beq elsedoneblock7958
ConditionalTrueBlock30114
	; Assigning single variable : r
	; Optimizer: a = a +/- b
	lda r
	clc
	adc #4
	sta r
elsedoneblock7958
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
	beq elsedoneblock7479
ConditionalTrueBlock1052
	; Assigning single variable : r
	; Optimizer: a = a +/- b
	lda r
	clc
	adc #8
	sta r
elsedoneblock7479
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
	bne elsedoneblock32498
ConditionalTrueBlock10866
	; Binary clause Simplified: NOTEQUALS
	lda r
	; Compare with pure num / var optimization
	beq elsedoneblock29458
ConditionalTrueBlock3925
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
elsedoneblock29458
elsedoneblock32498
	; Binary clause Simplified: EQUALS
	lda i
	; Compare with pure num / var optimization
	cmp #17
	bne elsedoneblock14600
ConditionalTrueBlock27830
	; Binary clause Simplified: NOTEQUALS
	lda r
	; Compare with pure num / var optimization
	beq elsedoneblock18397
ConditionalTrueBlock9567
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
elsedoneblock18397
elsedoneblock14600
	; Binary clause Simplified: EQUALS
	lda t
	; Compare with pure num / var optimization
	cmp #1
	bne elsedoneblock24230
ConditionalTrueBlock13851
	; Binary clause Simplified: NOTEQUALS
	lda r
	; Compare with pure num / var optimization
	beq elsedoneblock792
ConditionalTrueBlock1655
	; Assigning single variable : p1
	lda #24
	ldy i
	sta (p1),y
elsedoneblock792
elsedoneblock24230
	; Binary clause Simplified: EQUALS
	lda t
	; Compare with pure num / var optimization
	cmp #17
	bne elsedoneblock26946
ConditionalTrueBlock23146
	; Binary clause Simplified: NOTEQUALS
	lda r
	; Compare with pure num / var optimization
	beq elsedoneblock12570
ConditionalTrueBlock20528
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
elsedoneblock12570
elsedoneblock26946

elsedoneblock26264

	inc i
	lda #17
	cmp i
	bcc forLoopDone25269
forLoopNotDone24937
	jmp for303
forLoopDone25269
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #19
	bcc WordAdd19557
	inc p1+1
WordAdd19557
	sta p1+0

	inc t
	lda #17
	cmp t
	bcc forLoopDone25205
forLoopNotDone4975
	jmp for12529
forLoopDone25205

	rts
	
	
	; ***********  Defining procedure : ColourTime
	;    Procedure type : User-defined procedure
	
	jmp block28962
timeColor	dc.b	
block28962
ColourTime

	; Assigning single variable : p1
	lda #68
	ldx #150
	sta p1
	stx p1+1
	; Assigning single variable : i
	lda #0
	sta i
for14161
	; Assigning single variable : p1
	lda timeColor
	ldy i
	sta (p1),y
	inc i
	lda #7
	cmp i
	bcs for14161
forLoopDone12549

	rts
	
	
	; ***********  Defining procedure : DrawUITextLabels
	;    Procedure type : User-defined procedure
	
	jmp block330
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
block330
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
	
	jmp block1278
bonusString		dc.b	39
	dc.b	40
	dc.b	41
	dc.b	42
	dc.b	0
block1278
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
for12393

	; Assigning single variable : p1
	lda i
	ldy t
	sta (p1),y
	; Assigning single variable : t
	inc t

	inc i
	lda #47
	cmp i
	bcs for12393
forLoopDone30714

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
printdecimal19949
	jsr init_printdecimal_div10 
	ora #$30
	sta (screenmemory),y
	dey
	bpl printdecimal19949

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
printdecimal2505
	jsr init_printdecimal_div10 
	ora #$30
	sta (screenmemory),y
	dey
	bpl printdecimal2505

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
printdecimal10004
	jsr init_printdecimal_div10 
	ora #$30
	sta (screenmemory),y
	dey
	bpl printdecimal10004

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
printdecimal2623
	jsr init_printdecimal_div10 
	ora #$30
	sta (screenmemory),y
	dey
	bpl printdecimal2623

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
	bne elsedoneblock19281
ConditionalTrueBlock25608

	jsr MoveNibblerHeadUp
	rts

elsedoneblock19281
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #0
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #3
	bne elsedoneblock9876
ConditionalTrueBlock28652

	jsr MoveNibblerHeadRight
	rts

elsedoneblock9876
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #0
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #4
	bne elsedoneblock29174
ConditionalTrueBlock6249

	jsr MoveNibblerHeadDown
	rts

elsedoneblock29174
	jsr MoveNibblerHeadLeft

	rts
	
	
	; ***********  Defining procedure : MoveNibblerTail
	;    Procedure type : User-defined procedure
	
MoveNibblerTail

	; Binary clause Simplified: NOTEQUALS
	lda tailWait
	; Compare with pure num / var optimization
	beq elsedoneblock5565
ConditionalTrueBlock20289

	; Assigning single variable : tailWait
	dec tailWait
	rts

elsedoneblock5565
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #0
	lda (tail),y
	
	; Compare with pure num / var optimization
	cmp #14
	bne elsedoneblock2609
ConditionalTrueBlock31450

	; Assigning single variable : tail
	lda #0
	ldy #0
	sta (tail),y
	; Assigning single variable : tail
	; WORD optimization: a=a+b
	lda tail+0
	
	sec
	sbc #19
	bcs WordAdd28510
	dec tail+1
WordAdd28510
	sta tail+0
	; Assigning single variable : tailc
	; WORD optimization: a=a+b
	lda tailc+0
	
	sec
	sbc #19
	bcs WordAdd10422
	dec tailc+1
WordAdd10422
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
	bne elsedoneblock23552
ConditionalTrueBlock8779

	; Assigning single variable : tail
	lda #17
	ldy #0
	sta (tail),y
	rts

elsedoneblock23552
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #0
	lda (tail),y
	
	; Compare with pure num / var optimization
	cmp #10
	bne elsedoneblock1364
ConditionalTrueBlock5764

	; Assigning single variable : tail
	lda #15
	ldy #0
	sta (tail),y
	rts

elsedoneblock1364
	; Assigning single variable : tail
	lda #14
	ldy #0
	sta (tail),y
	rts

elsedoneblock2609
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #0
	lda (tail),y
	
	; Compare with pure num / var optimization
	cmp #15
	bne elsedoneblock26821
ConditionalTrueBlock474

	; Assigning single variable : tail
	lda #0
	ldy #0
	sta (tail),y
	; Assigning single variable : tail
	; WORD optimization: a=a+b
	lda tail+0
	
	clc
	adc #1
	bcc WordAdd9762
	inc tail+1
WordAdd9762
	sta tail+0
	; Assigning single variable : tailc
	; WORD optimization: a=a+b
	lda tailc+0
	
	clc
	adc #1
	bcc WordAdd5967
	inc tailc+1
WordAdd5967
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
	bne elsedoneblock17223
ConditionalTrueBlock4415

	; Assigning single variable : tail
	lda #14
	ldy #0
	sta (tail),y
	rts

elsedoneblock17223
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #0
	lda (tail),y
	
	; Compare with pure num / var optimization
	cmp #11
	bne elsedoneblock958
ConditionalTrueBlock29486

	; Assigning single variable : tail
	lda #16
	ldy #0
	sta (tail),y
	rts

elsedoneblock958
	; Assigning single variable : tail
	lda #15
	ldy #0
	sta (tail),y
	rts

elsedoneblock26821
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #0
	lda (tail),y
	
	; Compare with pure num / var optimization
	cmp #16
	bne elsedoneblock17692
ConditionalTrueBlock18700

	; Assigning single variable : tail
	lda #0
	ldy #0
	sta (tail),y
	; Assigning single variable : tail
	; WORD optimization: a=a+b
	lda tail+0
	
	clc
	adc #19
	bcc WordAdd9220
	inc tail+1
WordAdd9220
	sta tail+0
	; Assigning single variable : tailc
	; WORD optimization: a=a+b
	lda tailc+0
	
	clc
	adc #19
	bcc WordAdd31111
	inc tailc+1
WordAdd31111
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
	bne elsedoneblock8925
ConditionalTrueBlock23856

	; Assigning single variable : tail
	lda #17
	ldy #0
	sta (tail),y
	rts

elsedoneblock8925
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #0
	lda (tail),y
	
	; Compare with pure num / var optimization
	cmp #12
	bne elsedoneblock7534
ConditionalTrueBlock15969

	; Assigning single variable : tail
	lda #15
	ldy #0
	sta (tail),y
	rts

elsedoneblock7534
	; Assigning single variable : tail
	lda #16
	ldy #0
	sta (tail),y
	rts

elsedoneblock17692
	; Assigning single variable : tail
	lda #0
	ldy #0
	sta (tail),y
	; Assigning single variable : tail
	; WORD optimization: a=a+b
	lda tail+0
	
	sec
	sbc #1
	bcs WordAdd31068
	dec tail+1
WordAdd31068
	sta tail+0
	; Assigning single variable : tailc
	; WORD optimization: a=a+b
	lda tailc+0
	
	sec
	sbc #1
	bcs WordAdd21991
	dec tailc+1
WordAdd21991
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
	bne elsedoneblock7727
ConditionalTrueBlock7783

	; Assigning single variable : tail
	lda #14
	ldy #0
	sta (tail),y
	rts

elsedoneblock7727
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #0
	lda (tail),y
	
	; Compare with pure num / var optimization
	cmp #10
	bne elsedoneblock11027
ConditionalTrueBlock17612

	; Assigning single variable : tail
	lda #16
	ldy #0
	sta (tail),y
	rts

elsedoneblock11027
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
	bne elsedoneblock26443
ConditionalTrueBlock30238

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
	bne elsedoneblock702
ConditionalTrueBlock13877
	; Assigning single variable : tailGrow
	lda #1
	sta tailGrow
elsedoneblock702
	; Binary clause Simplified: LESS
	lda numFood
	; Compare with pure num / var optimization
	cmp #8
	bcs elsedoneblock8023
ConditionalTrueBlock8823
	; Assigning single variable : tailWait
	; Optimizer: a = a +/- b
	lda tailWait
	clc
	adc #2
	sta tailWait
elsedoneblock8023
	; Assigning single variable : stopTime
	inc stopTime
	; Binary clause Simplified: GREATEREQUAL
	lda stopTime
	; Compare with pure num / var optimization
	cmp #10
	bcc elsedoneblock31310
ConditionalTrueBlock2327
	; Assigning single variable : stopTime
	lda #10
	sta stopTime
elsedoneblock31310
	; Assigning single variable : score
	ldy #0
	; WORD optimization: a=a+b
	lda score+0
	
	clc
	adc #1
	bcc WordAdd7926
	inc score+1
WordAdd7926
	sta score+0
	; Assigning single variable : numFood
	dec numFood
	jsr DrawUIScore

elsedoneblock26443

	rts
	
	
	; ***********  Defining procedure : DecStopTime
	;    Procedure type : User-defined procedure
	
DecStopTime

	; Binary clause Simplified: NOTEQUALS
	lda stopTime
	; Compare with pure num / var optimization
	beq elsedoneblock2258
ConditionalTrueBlock32528
	dec stopTime
elsedoneblock2258

	rts
	
	
	; ***********  Defining procedure : NibblerControl
	;    Procedure type : User-defined procedure
	
NibblerControl

	jsr ReadJoystick
	; Binary clause Simplified: EQUALS
	lda $c5
	; Compare with pure num / var optimization
	cmp #48
	bne elsedoneblock4472
ConditionalTrueBlock2742
	; Assigning single variable : JoyResult
	; 8 bit binop
	; Add/sub where right value is constant number
	lda JoyResult
	ora #2
	 ; end add / sub var with constant
	
	sta JoyResult
elsedoneblock4472
	; Binary clause Simplified: EQUALS
	lda $c5
	; Compare with pure num / var optimization
	cmp #17
	bne elsedoneblock14867
ConditionalTrueBlock20718
	; Assigning single variable : JoyResult
	; 8 bit binop
	; Add/sub where right value is constant number
	lda JoyResult
	ora #4
	 ; end add / sub var with constant
	
	sta JoyResult
elsedoneblock14867
	; Binary clause Simplified: EQUALS
	lda $c5
	; Compare with pure num / var optimization
	cmp #52
	bne elsedoneblock28614
ConditionalTrueBlock9257
	; Assigning single variable : JoyResult
	; 8 bit binop
	; Add/sub where right value is constant number
	lda JoyResult
	ora #8
	 ; end add / sub var with constant
	
	sta JoyResult
elsedoneblock28614
	; Binary clause Simplified: EQUALS
	lda $c5
	; Compare with pure num / var optimization
	cmp #13
	bne elsedoneblock20807
ConditionalTrueBlock26063
	; Assigning single variable : JoyResult
	; 8 bit binop
	; Add/sub where right value is constant number
	lda JoyResult
	ora #1
	 ; end add / sub var with constant
	
	sta JoyResult
elsedoneblock20807
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
	bne binaryclausefailed31163
	lda #1; success
	jmp binaryclausefinished16683
binaryclausefailed31163
	lda #0 ; failed state
binaryclausefinished16683
	cmp #1
	beq ConditionalTrueBlock6365
	jmp elsedoneblock7586
ConditionalTrueBlock6365

	; Binary clause: NOTEQUALS
	; Load pointer array
	ldy #0
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #2
	; BC done
	beq binaryclausefailed11191
	lda #1; success
	jmp binaryclausefinished5973
binaryclausefailed11191
	lda #0 ; failed state
binaryclausefinished5973
	cmp #1
	beq ConditionalTrueBlock25452
	jmp elsedoneblock4954
ConditionalTrueBlock25452
	; Binary clause: NOTEQUALS
	; Load pointer array
	ldy #0
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #4
	; BC done
	beq binaryclausefailed16955
	lda #1; success
	jmp binaryclausefinished23183
binaryclausefailed16955
	lda #0 ; failed state
binaryclausefinished23183
	cmp #1
	beq ConditionalTrueBlock5651
	jmp elsedoneblock2144
ConditionalTrueBlock5651

	; Assigning single variable : p1
	
	; HandleVarBinopB16bit here
	ldy #0
	lda #19
rightvarInteger_var16097 = $54
	sta rightvarInteger_var16097
	sty rightvarInteger_var16097+1
	
	lda head+1
	sec
	sbc rightvarInteger_var16097+1
	tay
	lda head
	sec
	sbc rightvarInteger_var16097
	bcs wordAdd30180
	dey
wordAdd30180
	
	sta p1
	sty p1+1
	; Binary clause Simplified: GREATEREQUAL
	; Load pointer array
	ldy #0
	lda (p1),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcc elsedoneblock9261
ConditionalTrueBlock27065
	rts
elsedoneblock9261
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #0
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #3
	bne elsedoneblock31947
ConditionalTrueBlock14140
	; Assigning single variable : head
	lda #13
	ldy #0
	sta (head),y
elsedoneblock31947
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #0
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #5
	bne elsedoneblock6149
ConditionalTrueBlock13489
	; Assigning single variable : head
	lda #12
	ldy #0
	sta (head),y
elsedoneblock6149
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
	bcs WordAdd22214
	dec headc+1
WordAdd22214
	sta headc+0
	; Assigning single variable : headc
	lda #5
	ldy #0
	sta (headc),y
	; Assigning single variable : justMoved
	lda #1
	sta justMoved

elsedoneblock2144
elsedoneblock4954
	rts

elsedoneblock7586
	; Binary clause: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda JoyResult
	and #4
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #4
	; BC done
	bne binaryclausefailed15529
	lda #1; success
	jmp binaryclausefinished14788
binaryclausefailed15529
	lda #0 ; failed state
binaryclausefinished14788
	cmp #1
	beq ConditionalTrueBlock27007
	jmp elsedoneblock8896
ConditionalTrueBlock27007

	; Binary clause: NOTEQUALS
	; Load pointer array
	ldy #0
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #4
	; BC done
	beq binaryclausefailed31647
	lda #1; success
	jmp binaryclausefinished3181
binaryclausefailed31647
	lda #0 ; failed state
binaryclausefinished3181
	cmp #1
	beq ConditionalTrueBlock11969
	jmp elsedoneblock21617
ConditionalTrueBlock11969
	; Binary clause: NOTEQUALS
	; Load pointer array
	ldy #0
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #2
	; BC done
	beq binaryclausefailed13713
	lda #1; success
	jmp binaryclausefinished20968
binaryclausefailed13713
	lda #0 ; failed state
binaryclausefinished20968
	cmp #1
	beq ConditionalTrueBlock10987
	jmp elsedoneblock20493
ConditionalTrueBlock10987

	; Assigning single variable : p1
	
	; HandleVarBinopB16bit here
	ldy #0
	lda #19
rightvarInteger_var12079 = $54
	sta rightvarInteger_var12079
	sty rightvarInteger_var12079+1
	
	lda head+1
	clc
	adc rightvarInteger_var12079+1
	tay
	lda head
	clc
	adc rightvarInteger_var12079
	bcc wordAdd27216
	iny
wordAdd27216
	
	sta p1
	sty p1+1
	; Binary clause Simplified: GREATEREQUAL
	; Load pointer array
	ldy #0
	lda (p1),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcc elsedoneblock12933
ConditionalTrueBlock17040
	rts
elsedoneblock12933
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #0
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #3
	bne elsedoneblock2095
ConditionalTrueBlock26653
	; Assigning single variable : head
	lda #11
	ldy #0
	sta (head),y
elsedoneblock2095
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #0
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #5
	bne elsedoneblock8976
ConditionalTrueBlock26835
	; Assigning single variable : head
	lda #10
	ldy #0
	sta (head),y
elsedoneblock8976
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
	bcc WordAdd4071
	inc headc+1
WordAdd4071
	sta headc+0
	; Assigning single variable : headc
	lda #5
	ldy #0
	sta (headc),y
	; Assigning single variable : justMoved
	lda #1
	sta justMoved

elsedoneblock20493
elsedoneblock21617
	rts

elsedoneblock8896
	; Binary clause: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda JoyResult
	and #8
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #8
	; BC done
	bne binaryclausefailed29033
	lda #1; success
	jmp binaryclausefinished2088
binaryclausefailed29033
	lda #0 ; failed state
binaryclausefinished2088
	cmp #1
	beq ConditionalTrueBlock13559
	jmp elsedoneblock5602
ConditionalTrueBlock13559

	; Binary clause: NOTEQUALS
	; Load pointer array
	ldy #0
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #5
	; BC done
	beq binaryclausefailed24245
	lda #1; success
	jmp binaryclausefinished1338
binaryclausefailed24245
	lda #0 ; failed state
binaryclausefinished1338
	cmp #1
	beq ConditionalTrueBlock15993
	jmp elsedoneblock24962
ConditionalTrueBlock15993
	; Binary clause: NOTEQUALS
	; Load pointer array
	ldy #0
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #3
	; BC done
	beq binaryclausefailed21630
	lda #1; success
	jmp binaryclausefinished23224
binaryclausefailed21630
	lda #0 ; failed state
binaryclausefinished23224
	cmp #1
	beq ConditionalTrueBlock31339
	jmp elsedoneblock24604
ConditionalTrueBlock31339

	; Assigning single variable : p1
	
	; HandleVarBinopB16bit here
	ldy #0
	lda #1
rightvarInteger_var24474 = $54
	sta rightvarInteger_var24474
	sty rightvarInteger_var24474+1
	
	lda head+1
	sec
	sbc rightvarInteger_var24474+1
	tay
	lda head
	sec
	sbc rightvarInteger_var24474
	bcs wordAdd26737
	dey
wordAdd26737
	
	sta p1
	sty p1+1
	; Binary clause Simplified: GREATEREQUAL
	; Load pointer array
	ldy #0
	lda (p1),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcc elsedoneblock3855
ConditionalTrueBlock27372
	rts
elsedoneblock3855
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #0
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #4
	bne elsedoneblock7075
ConditionalTrueBlock27606
	; Assigning single variable : head
	lda #13
	ldy #0
	sta (head),y
elsedoneblock7075
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #0
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #2
	bne elsedoneblock12779
ConditionalTrueBlock30432
	; Assigning single variable : head
	lda #11
	ldy #0
	sta (head),y
elsedoneblock12779
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
	bcs WordAdd20667
	dec headc+1
WordAdd20667
	sta headc+0
	; Assigning single variable : headc
	lda #5
	ldy #0
	sta (headc),y
	; Assigning single variable : justMoved
	lda #1
	sta justMoved

elsedoneblock24604
elsedoneblock24962
	rts

elsedoneblock5602
	; Binary clause: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda JoyResult
	and #1
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #1
	; BC done
	bne binaryclausefailed9458
	lda #1; success
	jmp binaryclausefinished25967
binaryclausefailed9458
	lda #0 ; failed state
binaryclausefinished25967
	cmp #1
	beq ConditionalTrueBlock24125
	jmp elsedoneblock12737
ConditionalTrueBlock24125

	; Binary clause: NOTEQUALS
	; Load pointer array
	ldy #0
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #3
	; BC done
	beq binaryclausefailed22194
	lda #1; success
	jmp binaryclausefinished23699
binaryclausefailed22194
	lda #0 ; failed state
binaryclausefinished23699
	cmp #1
	beq ConditionalTrueBlock11531
	jmp elsedoneblock26819
ConditionalTrueBlock11531
	; Binary clause: NOTEQUALS
	; Load pointer array
	ldy #0
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #5
	; BC done
	beq binaryclausefailed10844
	lda #1; success
	jmp binaryclausefinished13049
binaryclausefailed10844
	lda #0 ; failed state
binaryclausefinished13049
	cmp #1
	beq ConditionalTrueBlock4042
	jmp elsedoneblock23941
ConditionalTrueBlock4042

	; Assigning single variable : p1
	
	; HandleVarBinopB16bit here
	ldy #0
	lda #1
rightvarInteger_var26363 = $54
	sta rightvarInteger_var26363
	sty rightvarInteger_var26363+1
	
	lda head+1
	clc
	adc rightvarInteger_var26363+1
	tay
	lda head
	clc
	adc rightvarInteger_var26363
	bcc wordAdd24065
	iny
wordAdd24065
	
	sta p1
	sty p1+1
	; Binary clause Simplified: GREATEREQUAL
	; Load pointer array
	ldy #0
	lda (p1),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcc elsedoneblock29731
ConditionalTrueBlock28773
	rts
elsedoneblock29731
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #0
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #4
	bne elsedoneblock7103
ConditionalTrueBlock14398
	; Assigning single variable : head
	lda #12
	ldy #0
	sta (head),y
elsedoneblock7103
	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy #0
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #2
	bne elsedoneblock31088
ConditionalTrueBlock13043
	; Assigning single variable : head
	lda #10
	ldy #0
	sta (head),y
elsedoneblock31088
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
	bcc WordAdd9850
	inc headc+1
WordAdd9850
	sta headc+0
	; Assigning single variable : headc
	lda #5
	ldy #0
	sta (headc),y
	; Assigning single variable : justMoved
	lda #1
	sta justMoved

elsedoneblock23941
elsedoneblock26819
	rts

elsedoneblock12737

	rts
	
	
	; ***********  Defining procedure : MoveNibblerHeadUp
	;    Procedure type : User-defined procedure
	
MoveNibblerHeadUp

	; Assigning single variable : p1
	
	; HandleVarBinopB16bit here
	ldy #0
	lda #19
rightvarInteger_var21911 = $54
	sta rightvarInteger_var21911
	sty rightvarInteger_var21911+1
	
	lda head+1
	sec
	sbc rightvarInteger_var21911+1
	tay
	lda head
	sec
	sbc rightvarInteger_var21911
	bcs wordAdd7240
	dey
wordAdd7240
	
	sta p1
	sty p1+1
	; Binary clause Simplified: LESS
	; Load pointer array
	ldy #0
	lda (p1),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcs elsedoneblock27616
ConditionalTrueBlock28580

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
	bcs WordAdd14615
	dec headc+1
WordAdd14615
	sta headc+0
	; Assigning single variable : headc
	lda #5
	ldy #0
	sta (headc),y
	rts

elsedoneblock27616
	; Assigning single variable : p2
	
	; HandleVarBinopB16bit here
	ldy #0
	lda #1
rightvarInteger_var26798 = $54
	sta rightvarInteger_var26798
	sty rightvarInteger_var26798+1
	
	lda head+1
	sec
	sbc rightvarInteger_var26798+1
	tay
	lda head
	sec
	sbc rightvarInteger_var26798
	bcs wordAdd22348
	dey
wordAdd22348
	
	sta p2
	sty p2+1
	; Binary clause Simplified: LESS
	; Load pointer array
	ldy #1
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcs elsedoneblock2883
ConditionalTrueBlock31635
	; Binary clause Simplified: LESS
	; Load pointer array
	ldy #0
	lda (p2),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcs elsedoneblock13028
ConditionalTrueBlock2003

	; Binary clause Simplified: EQUALS
	lda snd1Freq
	; Compare with pure num / var optimization
	bne elsedoneblock26610
ConditionalTrueBlock29681
	; Assigning single variable : snd1Freq
	lda #180
	sta snd1Freq
elsedoneblock26610
	jsr DecStopTime
	inc tailWait
	rts

elsedoneblock13028
elsedoneblock2883
	jsr DecStopTime
	; Binary clause Simplified: LESS
	; Load pointer array
	ldy #1
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcs elsedoneblock16840
ConditionalTrueBlock6459

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
	bcc WordAdd20274
	inc head+1
WordAdd20274
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
	bcc WordAdd12993
	inc headc+1
WordAdd12993
	sta headc+0
	; Assigning single variable : headc
	lda #5
	ldy #0
	sta (headc),y
	rts

elsedoneblock16840
	; Binary clause Simplified: LESS
	; Load pointer array
	ldy #0
	lda (p2),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcs elsedoneblock8290
ConditionalTrueBlock32203

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
	bcs WordAdd31151
	dec headc+1
WordAdd31151
	sta headc+0
	; Assigning single variable : headc
	lda #5
	ldy #0
	sta (headc),y
	rts

elsedoneblock8290
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
	bcs elsedoneblock28703
ConditionalTrueBlock16717

	; Assigning single variable : head
	lda #6
	ldy #0
	sta (head),y
	; Assigning single variable : head
	; WORD optimization: a=a+b
	lda head+0
	
	clc
	adc #1
	bcc WordAdd18245
	inc head+1
WordAdd18245
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
	bcc WordAdd20386
	inc headc+1
WordAdd20386
	sta headc+0
	; Assigning single variable : headc
	lda #5
	ldy #0
	sta (headc),y
	rts

elsedoneblock28703
	; Assigning single variable : p1
	
	; HandleVarBinopB16bit here
	ldy #0
	lda #19
rightvarInteger_var32765 = $54
	sta rightvarInteger_var32765
	sty rightvarInteger_var32765+1
	
	lda head+1
	sec
	sbc rightvarInteger_var32765+1
	tay
	lda head
	sec
	sbc rightvarInteger_var32765
	bcs wordAdd32651
	dey
wordAdd32651
	
	sta p1
	sty p1+1
	; Assigning single variable : p2
	
	; HandleVarBinopB16bit here
	ldy #0
	lda #19
rightvarInteger_var16840 = $54
	sta rightvarInteger_var16840
	sty rightvarInteger_var16840+1
	
	lda head+1
	clc
	adc rightvarInteger_var16840+1
	tay
	lda head
	clc
	adc rightvarInteger_var16840
	bcc wordAdd19601
	iny
wordAdd19601
	
	sta p2
	sty p2+1
	; Binary clause Simplified: LESS
	; Load pointer array
	ldy #0
	lda (p1),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcs elsedoneblock29250
ConditionalTrueBlock1497
	; Binary clause Simplified: LESS
	; Load pointer array
	ldy #0
	lda (p2),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcs elsedoneblock12935
ConditionalTrueBlock3760

	; Binary clause Simplified: EQUALS
	lda snd1Freq
	; Compare with pure num / var optimization
	bne elsedoneblock21555
ConditionalTrueBlock20538
	; Assigning single variable : snd1Freq
	lda #180
	sta snd1Freq
elsedoneblock21555
	jsr DecStopTime
	inc tailWait
	rts

elsedoneblock12935
elsedoneblock29250
	jsr DecStopTime
	; Binary clause Simplified: LESS
	; Load pointer array
	ldy #0
	lda (p1),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcs elsedoneblock15433
ConditionalTrueBlock5752

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
	bcs WordAdd2401
	dec headc+1
WordAdd2401
	sta headc+0
	; Assigning single variable : headc
	lda #5
	ldy #0
	sta (headc),y
	rts

elsedoneblock15433
	; Binary clause Simplified: LESS
	; Load pointer array
	ldy #0
	lda (p2),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcs elsedoneblock30548
ConditionalTrueBlock12759

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
	bcc WordAdd12933
	inc headc+1
WordAdd12933
	sta headc+0
	; Assigning single variable : headc
	lda #5
	ldy #0
	sta (headc),y
	rts

elsedoneblock30548
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
	bcs elsedoneblock13694
ConditionalTrueBlock3690

	; Assigning single variable : head
	lda #9
	ldy #0
	sta (head),y
	; Assigning single variable : head
	; WORD optimization: a=a+b
	lda head+0
	
	clc
	adc #19
	bcc WordAdd15248
	inc head+1
WordAdd15248
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
	bcc WordAdd3879
	inc headc+1
WordAdd3879
	sta headc+0
	; Assigning single variable : headc
	lda #5
	ldy #0
	sta (headc),y
	rts

elsedoneblock13694
	; Assigning single variable : p2
	
	; HandleVarBinopB16bit here
	ldy #0
	lda #1
rightvarInteger_var11149 = $54
	sta rightvarInteger_var11149
	sty rightvarInteger_var11149+1
	
	lda head+1
	sec
	sbc rightvarInteger_var11149+1
	tay
	lda head
	sec
	sbc rightvarInteger_var11149
	bcs wordAdd11647
	dey
wordAdd11647
	
	sta p2
	sty p2+1
	; Binary clause Simplified: LESS
	; Load pointer array
	ldy #1
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcs elsedoneblock25598
ConditionalTrueBlock669
	; Binary clause Simplified: LESS
	; Load pointer array
	ldy #0
	lda (p2),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcs elsedoneblock13930
ConditionalTrueBlock10048

	; Binary clause Simplified: EQUALS
	lda snd1Freq
	; Compare with pure num / var optimization
	bne elsedoneblock17589
ConditionalTrueBlock506
	; Assigning single variable : snd1Freq
	lda #180
	sta snd1Freq
elsedoneblock17589
	jsr DecStopTime
	inc tailWait
	rts

elsedoneblock13930
elsedoneblock25598
	jsr DecStopTime
	; Binary clause Simplified: LESS
	; Load pointer array
	ldy #1
	lda (head),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcs elsedoneblock3818
ConditionalTrueBlock11447

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
	bcc WordAdd2503
	inc head+1
WordAdd2503
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
	bcc WordAdd30066
	inc headc+1
WordAdd30066
	sta headc+0
	; Assigning single variable : headc
	lda #5
	ldy #0
	sta (headc),y
	rts

elsedoneblock3818
	; Binary clause Simplified: LESS
	; Load pointer array
	ldy #0
	lda (p2),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcs elsedoneblock14676
ConditionalTrueBlock6840

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
	bcs WordAdd17651
	dec headc+1
WordAdd17651
	sta headc+0
	; Assigning single variable : headc
	lda #5
	ldy #0
	sta (headc),y
	rts

elsedoneblock14676
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
rightvarInteger_var20083 = $54
	sta rightvarInteger_var20083
	sty rightvarInteger_var20083+1
	
	lda head+1
	sec
	sbc rightvarInteger_var20083+1
	tay
	lda head
	sec
	sbc rightvarInteger_var20083
	bcs wordAdd8187
	dey
wordAdd8187
	
	sta p1
	sty p1+1
	; Binary clause Simplified: LESS
	; Load pointer array
	ldy #0
	lda (p1),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcs elsedoneblock18686
ConditionalTrueBlock23390

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
	bcs WordAdd25502
	dec headc+1
WordAdd25502
	sta headc+0
	; Assigning single variable : headc
	lda #5
	ldy #0
	sta (headc),y
	rts

elsedoneblock18686
	; Assigning single variable : p1
	
	; HandleVarBinopB16bit here
	ldy #0
	lda #19
rightvarInteger_var30862 = $54
	sta rightvarInteger_var30862
	sty rightvarInteger_var30862+1
	
	lda head+1
	sec
	sbc rightvarInteger_var30862+1
	tay
	lda head
	sec
	sbc rightvarInteger_var30862
	bcs wordAdd24277
	dey
wordAdd24277
	
	sta p1
	sty p1+1
	; Assigning single variable : p2
	
	; HandleVarBinopB16bit here
	ldy #0
	lda #19
rightvarInteger_var3565 = $54
	sta rightvarInteger_var3565
	sty rightvarInteger_var3565+1
	
	lda head+1
	clc
	adc rightvarInteger_var3565+1
	tay
	lda head
	clc
	adc rightvarInteger_var3565
	bcc wordAdd26424
	iny
wordAdd26424
	
	sta p2
	sty p2+1
	; Binary clause Simplified: LESS
	; Load pointer array
	ldy #0
	lda (p1),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcs elsedoneblock14428
ConditionalTrueBlock832
	; Binary clause Simplified: LESS
	; Load pointer array
	ldy #0
	lda (p2),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcs elsedoneblock7590
ConditionalTrueBlock9310

	; Binary clause Simplified: EQUALS
	lda snd1Freq
	; Compare with pure num / var optimization
	bne elsedoneblock4998
ConditionalTrueBlock4712
	; Assigning single variable : snd1Freq
	lda #180
	sta snd1Freq
elsedoneblock4998
	jsr DecStopTime
	inc tailWait
	rts

elsedoneblock7590
elsedoneblock14428
	jsr DecStopTime
	; Binary clause Simplified: LESS
	; Load pointer array
	ldy #0
	lda (p1),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcs elsedoneblock28047
ConditionalTrueBlock8692

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
	bcs WordAdd24115
	dec headc+1
WordAdd24115
	sta headc+0
	; Assigning single variable : headc
	lda #5
	ldy #0
	sta (headc),y
	rts

elsedoneblock28047
	; Binary clause Simplified: LESS
	; Load pointer array
	ldy #0
	lda (p2),y
	
	; Compare with pure num / var optimization
	cmp #2
	bcs elsedoneblock2645
ConditionalTrueBlock25858

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
	bcc WordAdd14272
	inc headc+1
WordAdd14272
	sta headc+0
	; Assigning single variable : headc
	lda #5
	ldy #0
	sta (headc),y
	rts

elsedoneblock2645
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
	
	jmp block21862
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
titlegfx	dc.b $a, $6, $b, $2, $10, $0, $0, $10
	dc.b $0, $0, $10, $0, $0, $0, $0, $0
	dc.b $0, $8, $0, $9, $8, $9, $5, $b
	dc.b $9, $5, $b, $9, $a, $7, $b, $a
	dc.b $6, $3, $8, $0, $9, $8, $9, $0
	dc.b $8, $9, $0, $8, $9, $9, $6, $d
	dc.b $8, $0, $0, $8, $0, $9, $8, $9
	dc.b $0, $8, $9, $0, $8, $9, $9, $0
	dc.b $0, $8, $0, $0, $e, $0, $4, $e
	dc.b $c, $6, $d, $c, $6, $d, $4, $c
	dc.b $6, $3, $e, $0, $1
block21862
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
for2480

	; Assigning single variable : t
	lda #0
	sta t
for9078

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
	bcs for9078
forLoopDone20169
	; Assigning single variable : screenmemory
	; WORD optimization: a=a+b
	lda screenmemory+0
	
	clc
	adc #19
	bcc WordAdd27478
	inc screenmemory+1
WordAdd27478
	sta screenmemory+0
	; Assigning single variable : p1
	; WORD optimization: a=a+b
	lda p1+0
	
	clc
	adc #19
	bcc WordAdd17939
	inc p1+1
WordAdd17939
	sta p1+0

	inc i
	lda #4
	cmp i
	bcs for2480
forLoopDone20134
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
printdecimal1677
	jsr init_printdecimal_div10 
	ora #$30
	sta (screenmemory),y
	dey
	bpl printdecimal1677
	; Assigning single variable : screenmemory
	lda #48
	ldy #6
	sta (screenmemory),y

	rts
	
	
	; ***********  Defining procedure : InstructionScreen
	;    Procedure type : User-defined procedure
	
	jmp block21575
textInst0	dc.b $89, $8e, $93, $94, $92, $95, $83, $94
	dc.b $89, $8f, $8e, $93, $4e
textInst1	dc.b $83, $8c, $85, $81, $92, $0, $85, $81
	dc.b $83, $88, $0, $8d, $81, $9a, $85, $12
	dc.b 
textInst2	dc.b $82, $85, $86, $8f, $92, $85, $0, $94
	dc.b $89, $8d, $85, $93, $0, $95, $90, $38
	dc.b 
textInst3	dc.b $84, $8f, $8e, $a7, $94, $0, $8c, $85
	dc.b $94, $0, $8e, $89, $82, $82, $8c, $85
	dc.b $92, $17
textInst4	dc.b $82, $89, $94, $85, $0, $89, $94, $93
	dc.b $85, $8c, $86, $36
textInst5	dc.b $8e, $89, $82, $82, $8c, $85, $92, $0
	dc.b $84, $8f, $85, $93, $0, $8e, $8f, $94
	dc.b $14
textInst6	dc.b $93, $94, $8f, $90, $0, $81, $94, $0
	dc.b $83, $8f, $92, $8e, $85, $92, $93, $39
	dc.b 
textInst7	dc.b $8e, $89, $82, $82, $8c, $85, $92, $0
	dc.b $87, $92, $8f, $97, $93, $14
textInst8	dc.b $97, $88, $85, $8e, $0, $89, $94, $0
	dc.b $85, $81, $94, $93, $60
textInstd	dc.b $96, $89, $83, $0, $96, $85, $92, $93
	dc.b $89, $8f, $8e, $14
textInste	dc.b $82, $99, $0, $81, $8e, $84, $99, $0
	dc.b $88, $fe
block21575
InstructionScreen

	; Binary clause Simplified: EQUALS
	; Load pointer array
	ldy i
	lda (p1),y
	
	; Compare with pure num / var optimization
	cmp #254
	bne elsedoneblock7961
ConditionalTrueBlock25981
	rts
elsedoneblock7961
	; Binary clause Simplified: NOTEQUALS
	; Load pointer array
	ldy i
	lda (p1),y
	
	; Compare with pure num / var optimization
	beq elsedoneblock24016
ConditionalTrueBlock25095
	; Binary clause Simplified: LESS
	; Load pointer array
	ldy i
	lda (p1),y
	
	; Compare with pure num / var optimization
	cmp #100
	bcs elsedoneblock23041
ConditionalTrueBlock119

	; Assigning single variable : p2
	; WORD optimization: a=a+b
	lda p2+0
	
	clc
	; Load pointer array
	ldy i
	adc (p1),y
	
	bcc WordAdd6915
	inc p2+1
WordAdd6915
	sta p2+0
	; Assigning single variable : r
	lda #0
	sta r
	; Assigning single variable : i
	inc i

elsedoneblock23041
elsedoneblock24016
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
	
	jmp block8072
scrnNo	dc.b	
scrnDelay	dc.b	
block8072
TitleSequence

	; Assigning single variable : scrnNo
	lda #0
	sta scrnNo
while17929
	; Binary clause: EQUALS
	lda #1
	; Compare with pure num / var optimization
	cmp #1
	; BC done
	bne binaryclausefailed25808
	lda #1; success
	jmp binaryclausefinished4632
binaryclausefailed25808
	lda #0 ; failed state
binaryclausefinished4632
	cmp #1
	beq ConditionalTrueBlock841
	jmp elsedoneblock17615
ConditionalTrueBlock841

	jsr ClearTheScreen
	; Assigning single variable : scrnNo
	inc scrnNo
	; Binary clause Simplified: EQUALS
	lda scrnNo
	; Compare with pure num / var optimization
	cmp #3
	bne elsedoneblock28074
ConditionalTrueBlock13787
	; Assigning single variable : scrnNo
	lda #1
	sta scrnNo
elsedoneblock28074
	; Binary clause Simplified: EQUALS
	lda scrnNo
	; Compare with pure num / var optimization
	cmp #1
	bne elsedoneblock16798
ConditionalTrueBlock1988
	jsr TitleScreen
elsedoneblock16798
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
while16321
	; Binary clause Simplified: NOTEQUALS
	lda scrnDelay
	; Compare with pure num / var optimization
	cmp #200
	beq elsedoneblock7056
ConditionalTrueBlock30104

	; Binary clause Simplified: EQUALS
	lda scrnNo
	; Compare with pure num / var optimization
	cmp #2
	bne elsedoneblock26124
ConditionalTrueBlock18738
	jsr InstructionScreen
elsedoneblock26124
	; Binary clause Simplified: EQUALS
	lda scrnNo
	; Compare with pure num / var optimization
	cmp #1
	bne elsedoneblock14489
ConditionalTrueBlock10951
	jsr AnimateNibblerBody
elsedoneblock14489
	jsr ReadJoystick
	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	lda JoyResult
	and #16
	 ; end add / sub var with constant
	
	; Compare with pure num / var optimization
	cmp #16
	bne elsedoneblock14855
ConditionalTrueBlock17013
	rts
elsedoneblock14855
	; Binary clause Simplified: NOTEQUALS
	lda $c5
	; Compare with pure num / var optimization
	cmp #64
	beq elsedoneblock13543
ConditionalTrueBlock25402
	rts
elsedoneblock13543
	jsr WaitForNRasters
	; Assigning single variable : scrnDelay
	inc scrnDelay

	jmp while16321
elsedoneblock7056

	jmp while17929
elsedoneblock17615

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
while15275
	; Binary clause Simplified: LESS
	lda r
	; Compare with pure num / var optimization
	cmp #6
	bcs elsedoneblock10761
ConditionalTrueBlock21528

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

	jmp while15275
elsedoneblock10761
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
while32729
	; Binary clause Simplified: LESS
	lda r
	; Compare with pure num / var optimization
	cmp #6
	bcs elsedoneblock2451
ConditionalTrueBlock17107

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
	bne elsedoneblock20654
ConditionalTrueBlock5471
	; Assigning single variable : tail
	lda #15
	ldy r
	sta (tail),y
elsedoneblock20654
	; Binary clause Simplified: EQUALS
	lda r
	; Compare with pure num / var optimization
	cmp #5
	bne elsedoneblock6766
ConditionalTrueBlock21229
	; Assigning single variable : tail
	lda #3
	ldy r
	sta (tail),y
elsedoneblock6766
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

	jmp while32729
elsedoneblock2451
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
while26632
	; Binary clause Simplified: LESS
	lda r
	; Compare with pure num / var optimization
	cmp #6
	bcs elsedoneblock20666
ConditionalTrueBlock7470

	; Assigning single variable : tailc
	lda #5
	ldy r
	sta (tailc),y
	; Binary clause Simplified: EQUALS
	lda r
	; Compare with pure num / var optimization
	bne elsedoneblock28317
ConditionalTrueBlock4629
	; Assigning single variable : tailc
	lda #7
	ldy r
	sta (tailc),y
elsedoneblock28317
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

	jmp while26632
elsedoneblock20666
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
while25452
	; Binary clause Simplified: NOTEQUALS
	lda time
	; Compare with pure num / var optimization
	beq elsedoneblock11833
ConditionalTrueBlock3154

	; Assigning single variable : time
	dec time
	; Assigning single variable : score
	ldy #0
	; WORD optimization: a=a+b
	lda score+0
	
	clc
	adc #1
	bcc WordAdd6738
	inc score+1
WordAdd6738
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

	jmp while25452
elsedoneblock11833

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
while25776
	; Binary clause Simplified: EQUALS
	lda r
	; Compare with pure num / var optimization
	bne elsedoneblock16320
ConditionalTrueBlock21098

	jsr MoveNibblerTail
	jsr WaitForNRasters
	; Binary clause Simplified: EQUALS
	lda head
	
binary_clause_temp_var8272 = $54
	sta binary_clause_temp_var8272
	lda tail
	
binary_clause_temp_2_var11434 = $56
	sta binary_clause_temp_2_var11434
	lda binary_clause_temp_var8272
	cmp binary_clause_temp_2_var11434
	bne elsedoneblock12212
ConditionalTrueBlock23775
	; Binary clause Simplified: EQUALS
	lda head+1
	
binary_clause_temp_var20308 = $54
	sta binary_clause_temp_var20308
	lda tail+1
	
binary_clause_temp_2_var12157 = $56
	sta binary_clause_temp_2_var12157
	lda binary_clause_temp_var20308
	cmp binary_clause_temp_2_var12157
	bne elsedoneblock12946
ConditionalTrueBlock25563
	; Assigning single variable : r
	lda #1
	sta r
elsedoneblock12946
elsedoneblock12212

	jmp while25776
elsedoneblock16320
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
for6464

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
	bcs for6464
forLoopDone4346

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
for25318

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
	bcs for25318
forLoopDone16292

	rts
	
	
	; ***********  Defining procedure : GameLoop
	;    Procedure type : User-defined procedure
	
GameLoop

	; Assigning single variable : r
	lda #0
	sta r
while27123
	; Binary clause: EQUALS
	lda r
	; Compare with pure num / var optimization
	; BC done
	bne binaryclausefailed16366
	lda #1; success
	jmp binaryclausefinished12033
binaryclausefailed16366
	lda #0 ; failed state
binaryclausefinished12033
	cmp #1
	beq ConditionalTrueBlock6461
	jmp elsedoneblock31461
ConditionalTrueBlock6461

	; Binary clause Simplified: EQUALS
	lda moveTime
	; Compare with pure num / var optimization
	bne elsedoneblock22860
ConditionalTrueBlock25329

	; Assigning single variable : moveTime
	lda moveTimeReset
	sta moveTime
	jsr NibblerControl
	; Binary clause Simplified: EQUALS
	lda justMoved
	; Compare with pure num / var optimization
	bne elsedoneblock17843
ConditionalTrueBlock12258
	jsr MoveNibblerHead
elsedoneblock17843
	jsr MoveNibblerTail
	; Binary clause Simplified: EQUALS
	lda gameState
	; Compare with pure num / var optimization
	cmp #1
	bne elsedoneblock12937
ConditionalTrueBlock18140
	rts
elsedoneblock12937
	; Binary clause Simplified: EQUALS
	lda numFood
	; Compare with pure num / var optimization
	bne elsedoneblock21817
ConditionalTrueBlock9551
	rts
elsedoneblock21817

elsedoneblock22860
	dec moveTime
	inc animTime
	; Binary clause Simplified: EQUALS
	lda animTime
	; Compare with pure num / var optimization
	cmp #5
	bne elsedoneblock25296
ConditionalTrueBlock27530

	jsr AnimateNibblerBody
	; Assigning single variable : animTime
	lda #0
	sta animTime
	jsr PlaySND1
	jsr PlaySND2
	jsr PlaySFX

elsedoneblock25296
	inc timeTick
	; Binary clause Simplified: EQUALS
	lda timeTick
	; Compare with pure num / var optimization
	cmp #60
	bne elsedoneblock31744
ConditionalTrueBlock5809

	; Assigning single variable : timeTick
	lda #0
	sta timeTick
	; Binary clause Simplified: EQUALS
	lda stopTime
	; Compare with pure num / var optimization
	bne elsedoneblock1048
ConditionalTrueBlock26946
	; Assigning single variable : timeTick
	lda #55
	sta timeTick
elsedoneblock1048
	dec time
	jsr DrawUITime
	; Binary clause Simplified: EQUALS
	lda time
	; Compare with pure num / var optimization
	cmp #12
	bne elsedoneblock1127
ConditionalTrueBlock7954
	; Assigning single variable : timeColor
	lda #2
	sta timeColor
	jsr ColourTime
elsedoneblock1127
	; Binary clause Simplified: EQUALS
	lda time
	; Compare with pure num / var optimization
	bne elsedoneblock23817
ConditionalTrueBlock15127

	; Assigning single variable : gameState
	lda #1
	sta gameState
	rts

elsedoneblock23817

elsedoneblock31744
	; wait for raster
	ldx #130
	cpx $9004
	bne *-3

	jmp while27123
elsedoneblock31461

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
block41

	jsr ScreenSetup
	jsr SetupJoystick
while28400
	; Binary clause: EQUALS
	lda #1
	; Compare with pure num / var optimization
	cmp #1
	; BC done
	bne binaryclausefailed1325
	lda #1; success
	jmp binaryclausefinished28313
binaryclausefailed1325
	lda #0 ; failed state
binaryclausefinished28313
	cmp #1
	beq ConditionalTrueBlock6558
	jmp elsedoneblock10935
ConditionalTrueBlock6558

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
while22312
	; Binary clause: NOTEQUALS
	lda lives
	; Compare with pure num / var optimization
	; BC done
	beq binaryclausefailed24942
	lda #1; success
	jmp binaryclausefinished7104
binaryclausefailed24942
	lda #0 ; failed state
binaryclausefinished7104
	cmp #1
	beq ConditionalTrueBlock24618
	jmp elsedoneblock18396
ConditionalTrueBlock24618

	jsr ScrollScreenDown
	; Assigning single variable : timeColor
	lda #1
	sta timeColor
	jsr ColourTime
	; Binary clause Simplified: EQUALS
	lda numFood
	; Compare with pure num / var optimization
	bne elsedoneblock11262
ConditionalTrueBlock26641

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
	bne elsedoneblock1014
ConditionalTrueBlock893
	; Assigning single variable : levelId
	lda #0
	sta levelId
elsedoneblock1014
	jsr DrawUIWave
	; Binary clause Simplified: EQUALS
	lda levelId
	; Compare with pure num / var optimization
	cmp #2
	bne elsedoneblock1293
ConditionalTrueBlock11495
	; Assigning single variable : moveTimeReset
	dec moveTimeReset
elsedoneblock1293
	; Binary clause Simplified: EQUALS
	lda levelId
	; Compare with pure num / var optimization
	cmp #4
	bne elsedoneblock8493
ConditionalTrueBlock11299
	; Assigning single variable : lives
	inc lives
elsedoneblock8493
	; Binary clause Simplified: EQUALS
	lda levelId
	; Compare with pure num / var optimization
	cmp #8
	bne elsedoneblock24069
ConditionalTrueBlock9314
	; Assigning single variable : moveTimeReset
	dec moveTimeReset
elsedoneblock24069
	; Binary clause Simplified: EQUALS
	lda levelId
	; Compare with pure num / var optimization
	cmp #10
	bne elsedoneblock13252
ConditionalTrueBlock15238
	; Assigning single variable : lives
	inc lives
elsedoneblock13252
	; Binary clause Simplified: LESS
	lda moveTimeReset
	; Compare with pure num / var optimization
	cmp #5
	bcs elsedoneblock6231
ConditionalTrueBlock23088
	; Assigning single variable : moveTimeReset
	lda #5
	sta moveTimeReset
elsedoneblock6231

elsedoneblock11262
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
	bne elsedoneblock15258
ConditionalTrueBlock12368
	jsr CountBonus
elsedoneblock15258
	; Binary clause Simplified: EQUALS
	lda gameState
	; Compare with pure num / var optimization
	cmp #1
	bne elsedoneblock17723
ConditionalTrueBlock18238
	jsr LoseLife
elsedoneblock17723
	jsr ResetVars
	; Assigning single variable : gameState
	lda #0
	sta gameState

	jmp while22312
elsedoneblock18396

	jmp while28400
elsedoneblock10935

EndSymbol
	org $1c00
charset
	incbin "C:/OneDrive/Documents/Vic/Dev/Nibbler/releases/TRSEsource///bin\chars.bin"
