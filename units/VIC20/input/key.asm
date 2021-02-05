 processor 6502
	org $1000
	; Starting new memory block at $1000
StartBlock1000
	.byte    $0, $0E, $08, $0A, $00, $9E, $20
	.byte   $34,$31,$31,$32
	.byte     $00
	; Ending memory block
EndBlock1000
	org $1010
	; Starting new memory block at $1010
StartBlock1010
Key
	; LineNumber: 328
	jmp block1
	; LineNumber: 9
Key_keys	dc.b	 
	org Key_keys+7
	; LineNumber: 10
Key_keysLast	dc.b	 
	org Key_keysLast+7
	; LineNumber: 11
Key_keysPressed	dc.b	 
	org Key_keysPressed+7
	; LineNumber: 14
Key_keyState	dc.b	0
	; LineNumber: 18
Key_temp	dc.b	0
	; LineNumber: 20
Key_KeyRow = $9120
	; LineNumber: 21
Key_KeyRead = $9121
	; LineNumber: 23
Key_keyRows	dc.b $0fe, $0fd, $0fb, $0f7, $0ef, $0df, $0bf, $07f
	; NodeProcedureDecl -1
	; ***********  Defining procedure : Key__GetKeyOffset
	;    Procedure type : User-defined procedure
	; LineNumber: 151
	; LineNumber: 150
	; LineNumber: 150
Key__GetKeyOffset_block2
Key__GetKeyOffset
	; LineNumber: 154
	; ****** Inline assembler section
	
	; pass with _a containing key to check 1, 2, 4, 8, 16, 32, 64, 128
	; and y the row offset 0 - 9
	tax ; temp store key
	
	tya ; multiply row offset by 8 to find starting pos in array
	asl
	asl
	asl ; *8
	sta TEMP_VAR1
	
	; use temporary variable as a pointer
	lda #>Key_screencodes_graphic
	sta TEMP_VAR2+1 ; high byte
	lda #<Key_screencodes_graphic
	sta TEMP_VAR2 ; low byte
	
	clc
	adc TEMP_VAR1
	sta TEMP_VAR2
	bcc getKey_NoOverflow
	inc TEMP_VAR2+1 ; byte overflow, update high byte
	
getKey_NoOverflow
	; then work out the offest 0 - 7
	txa ; the key
	
	ldy #0  ; count the number of shifts
	ldx #8	; test all 8 bits
getKey_testLoop
	lsr ; bit put in carry
	bcs getKey_foundBit
	iny	; counter for next bit to test
	dex	; 
	bne getKey_testLoop
	
getKey_foundBit
	; x has the bit 0-7 or 8 if not found
	cpy #8
	beq getKey_NotFound
	
	lda (TEMP_VAR2),y  ; load screencode + bit offset
	rts 
	
getKey_NotFound
	lda #KEY_NOKEY ; not found
	
	; LineNumber: 206
	rts
	
; // convert bit position to number*
; //Read the full 10 row keyboard matrix 
; //and populate the keyboard flags
; 

	; NodeProcedureDecl -1
	; ***********  Defining procedure : Key_Read
	;    Procedure type : User-defined procedure
	; LineNumber: 216
Key_Read
	; LineNumber: 218
	; Assigning single variable : Key_keyState
	lda #$0
	; Calling storevariable
	sta Key_keyState
	; LineNumber: 247
	
; // row to test
	; Assigning single variable : Key_temp
	; Calling storevariable
	sta Key_temp
Key_Read_forloop4
	; LineNumber: 221
	; LineNumber: 223
	; Assigning single variable : Key_KeyRow
	lda #$7
	; Calling storevariable
	sta Key_KeyRow
	; LineNumber: 225
	; ****** Inline assembler section
		lda Key_KeyRead ; get bits
		eor #$ff ; invert so the below works
		ldx Key_temp
		sta Key_keys,x
		eor Key_keysLast,x
		and Key_keys,x
		sta Key_keysPressed,x
		lda Key_keys,x
		sta Key_keysLast,x
		
		; record if no key pressed
		clc
		adc Key_keyState
		sta Key_keyState
		
		
	; LineNumber: 246
Key_Read_forloopcounter6
Key_Read_loopstart7
	; Compare is onpage
	inc Key_temp
	lda #$7
	cmp Key_temp ;keep
	bne Key_Read_forloop4
Key_Read_loopdone11: ;keep
Key_Read_forloopend5
Key_Read_loopend8
	; LineNumber: 248
	rts
	;*
; //<p>Returns true if the key is being held. Works with multiple keys
; //held at the same time.
; 

	; NodeProcedureDecl -1
	; ***********  Defining procedure : Key_Held
	;    Procedure type : User-defined procedure
	; LineNumber: 254
	; LineNumber: 253
Key_Held_block12
Key_Held
	; LineNumber: 256
	; ****** Inline assembler section
	
	sty Key_temp
	lda Key_keys,x
	and Key_temp 	; contains column 
	
	
	; LineNumber: 264
	rts
	;*
; //<p>Returns true if the key has just been pressed. Works with multiple 
; //keys pressed at the same time.
; 

	; NodeProcedureDecl -1
	; ***********  Defining procedure : Key_Pressed
	;    Procedure type : User-defined procedure
	; LineNumber: 271
	; LineNumber: 270
Key_Pressed_block13
Key_Pressed
	; LineNumber: 273
	; ****** Inline assembler section
	
	sty Key_temp
	lda Key_keysPressed,x
	and Key_temp 	; contains column 
	
	; LineNumber: 281
	rts
	;*
; //Gets a key being held. If multiple keys 
; //are pressed, returns the first key found.
; 

	; NodeProcedureDecl -1
	; ***********  Defining procedure : Key_GetHeld
	;    Procedure type : User-defined procedure
	; LineNumber: 307
	; LineNumber: 292
Key_screencodes_graphic	
	dc.b	$21, $23, $25, $26, 40, 255, 255, 255, 34
	dc.b	$24, $27, $1c, 41, 255, 255, 254, $11, $05
	dc.b	$14, $15, $0f, 70, $37, $39, $17, $12, $19
	dc.b	$09, $10, 255, $38, $2f, $01, $04, $07, $0a
	dc.b	$0c, 255, $34, $36, $13, $06, $08, $0b, $3a
	dc.b	255, $35, $2a, $1a, $03, $02, $0d, $3b, 255
	dc.b	$31, $33, $18, $16, $0e, 44, $3f, 255, $32
	dc.b	$2b, 255, $00, $1d, 255, $3e, 255, $30, $2d
	dc.b	255, $1b, $20, $3c, 255, 255, $2e, $3d 
Key_GetHeld_block14
Key_GetHeld
	; LineNumber: 316
	; Assigning single variable : Key_temp
	lda #$0
	; Calling storevariable
	sta Key_temp
Key_GetHeld_forloop15
	; LineNumber: 310
	; LineNumber: 312
	
; // Look up tables to convert key row / columns to screencodes
; // note: only text, numbers, punctuation and ins/del key are listed
; //       everything else is nokey so not detected in this table
	; Assigning memory location
	; Assigning single variable : $5e
	; Assigning register : _a
	; Load Byte array
	ldx Key_temp
	lda Key_keysPressed,x
	; Assigning register : _y
	ldy Key_temp
	jsr Key__GetKeyOffset
	; Calling storevariable
	sta $5e
	; LineNumber: 313
	; Binary clause Simplified: NOTEQUALS
	; Compare with pure num / var optimization
	cmp #$ff;keep
	beq Key_GetHeld_elsedoneblock31
Key_GetHeld_ConditionalTrueBlock29: ;Main true block ;keep 
	; LineNumber: 312
	lda $5e
	rts
Key_GetHeld_elsedoneblock31
	; LineNumber: 315
Key_GetHeld_forloopcounter17
Key_GetHeld_loopstart18
	; Compare is onpage
	inc Key_temp
	lda #$7
	cmp Key_temp ;keep
	bne Key_GetHeld_forloop15
Key_GetHeld_loopdone34: ;keep
Key_GetHeld_forloopend16
Key_GetHeld_loopend19
	; LineNumber: 318
	
; // no key held!
	lda #$ff
	rts
	; LineNumber: 320
	rts
	; NodeProcedureDecl -1
	; ***********  Defining procedure : Key_SetupScreenCodes
	;    Procedure type : User-defined procedure
	; LineNumber: 323
Key_SetupScreenCodes
	; LineNumber: 325
	; Assigning memory location
	; Assigning single variable : $5e
	; Load Unknown type array
	ldx #$0
	lda Key_screencodes_graphic,x
	; Calling storevariable
	sta $5e
	; LineNumber: 327
	rts
block1
	; End of program
	; Ending memory block
EndBlock1010
