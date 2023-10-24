 org $2000

START:
	org $2000
StartBlock2000:
	; Starting new memory block at $2000
Tiles:
	; LineNumber: 28
	lbra block1
	; LineNumber: 3
Memory_src:	fdb	$00
	; LineNumber: 3
Memory_dst:	fdb	$00
	; LineNumber: 4
Memory_len:	fcb	0
	; LineNumber: 4
Memory_b:	fcb	0
	; LineNumber: 5
Memory_ilen:	fdb	0
	; LineNumber: 4
Screen_p:	fdb	$00
	; LineNumber: 5
Screen_b:	fcb	0
	; LineNumber: 5
Screen_c:	fcb	0
	; LineNumber: 5
Screen_r:	fcb	0
	; LineNumber: 5
Screen_g:	fcb	0
	; LineNumber: 15
Screen_x:	fdb	0
	; LineNumber: 15
Screen_y:	fdb	0
	; LineNumber: 16
Screen_color:	fcb	0
	; LineNumber: 17
Screen_bytetbl1:	fcb $3f,$cf,$f3,$fc
	; LineNumber: 18
Screen_bytetbl2:	fcb $c0,$30,$c,$3
	; LineNumber: 20
Screen_ctab:	fcb $0,$40,$80,$c0
	; LineNumber: 21
Screen_colors:	fcb $0,$55,$aa,$ff
	; LineNumber: 5
Tiles_tlookup2x1:	fdb $00,$04,$08,$0c,$10,$14,$18,$1c
	fdb $20,$24,$28,$2c,$30,$34,$38,$3c
	fdb $40,$44,$48,$4c,$50,$54,$58,$5c
	fdb $60,$64,$68,$6c,$70,$74,$78,$7c
	fdb $200,$204,$208,$20c,$210,$214,$218,$21c
	fdb $220,$224,$228,$22c,$230,$234,$238,$23c
	fdb $240,$244,$248,$24c,$250,$254,$258,$25c
	fdb $260,$264,$268,$26c,$270,$274,$278,$27c
	fdb $400,$404,$408,$40c,$410,$414,$418,$41c
	fdb $420,$424,$428,$42c,$430,$434,$438,$43c
	fdb $440,$444,$448,$44c,$450,$454,$458,$45c
	fdb $460,$464,$468,$46c,$470,$474,$478,$47c
	fdb $600,$604,$608,$60c,$610,$614,$618,$61c
	fdb $620,$624,$628,$62c,$630,$634,$638,$63c
	fdb $640,$644,$648,$64c,$650,$654,$658,$65c
	fdb $660,$664,$668,$66c,$670,$674,$678,$67c
	fdb $800,$804,$808,$80c,$810,$814,$818,$81c
	fdb $820,$824,$828,$82c,$830,$834,$838,$83c
	fdb $840,$844,$848,$84c,$850,$854,$858,$85c
	fdb $860,$864,$868,$86c,$870,$874,$878,$87c
	fdb $a00,$a04,$a08,$a0c,$a10,$a14,$a18,$a1c
	fdb $a20,$a24,$a28,$a2c,$a30,$a34,$a38,$a3c
	fdb $a40,$a44,$a48,$a4c,$a50,$a54,$a58,$a5c
	fdb $a60,$a64,$a68,$a6c,$a70,$a74,$a78,$a7c
	fdb $c00,$c04,$c08,$c0c,$c10,$c14,$c18,$c1c
	fdb $c20,$c24,$c28,$c2c,$c30,$c34,$c38,$c3c
	fdb $c40,$c44,$c48,$c4c,$c50,$c54,$c58,$c5c
	fdb $c60,$c64,$c68,$c6c,$c70,$c74,$c78,$c7c
	fdb $e00,$e04,$e08,$e0c,$e10,$e14,$e18,$e1c
	fdb $e20,$e24,$e28,$e2c,$e30,$e34,$e38,$e3c
	fdb $e40,$e44,$e48,$e4c,$e50,$e54,$e58,$e5c
	fdb $e60,$e64,$e68,$e6c,$e70,$e74,$e78,$e7c
	; LineNumber: 6
Tiles_tab32:	fdb $00,$80,$100,$180,$200,$280,$300,$380
	fdb $400,$480,$500,$580,$600,$680,$700,$780
	fdb $800,$880,$900,$980,$a00,$a80,$b00,$b80
	fdb $c00,$c80,$d00,$d80,$e00,$e80,$f00,$f80
	fdb $1000,$1080,$1100,$1180,$1200,$1280,$1300,$1380
	fdb $1400,$1480,$1500,$1580,$1600,$1680,$1700,$1780
	fdb $1800,$1880,$1900,$1980,$1a00,$1a80,$1b00,$1b80
	fdb $1c00,$1c80,$1d00,$1d80,$1e00,$1e80,$1f00,$1f80
	fdb $2000,$2080,$2100,$2180,$2200,$2280,$2300,$2380
	fdb $2400,$2480,$2500,$2580,$2600,$2680,$2700,$2780
	fdb $2800,$2880,$2900,$2980,$2a00,$2a80,$2b00,$2b80
	fdb $2c00,$2c80,$2d00,$2d80,$2e00,$2e80,$2f00,$2f80
	fdb $3000,$3080,$3100,$3180,$3200,$3280,$3300,$3380
	fdb $3400,$3480,$3500,$3580,$3600,$3680,$3700,$3780
	fdb $3800,$3880,$3900,$3980,$3a00,$3a80,$3b00,$3b80
	fdb $3c00,$3c80,$3d00,$3d80,$3e00,$3e80,$3f00,$3f80
	fdb $4000,$4080,$4100,$4180,$4200,$4280,$4300,$4380
	fdb $4400,$4480,$4500,$4580,$4600,$4680,$4700,$4780
	fdb $4800,$4880,$4900,$4980,$4a00,$4a80,$4b00,$4b80
	fdb $4c00,$4c80,$4d00,$4d80,$4e00,$4e80,$4f00,$4f80
	fdb $5000,$5080,$5100,$5180,$5200,$5280,$5300,$5380
	fdb $5400,$5480,$5500,$5580,$5600,$5680,$5700,$5780
	fdb $5800,$5880,$5900,$5980,$5a00,$5a80,$5b00,$5b80
	fdb $5c00,$5c80,$5d00,$5d80,$5e00,$5e80,$5f00,$5f80
	fdb $6000,$6080,$6100,$6180,$6200,$6280,$6300,$6380
	; LineNumber: 7
Tiles_dst:	fdb	$00
	; LineNumber: 7
Tiles_src:	fdb	$00
	; LineNumber: 7
Tiles_tile:	fdb	$00
	; LineNumber: 8
Tiles_i:	fcb	0
	; ***********  Defining procedure : Memory_Copy
	;    Procedure type : User-defined procedure
	; LineNumber: 8
	; LineNumber: 7
	; LineNumber: 7
	; LineNumber: 7
Memory_Copy_block2:
Memory_Copy:
	; LineNumber: 9
	; ****** Inline assembler section
	ldb Memory_len
	ldx Memory_src
	ldy Memory_dst
memcpy_loop:
	lda ,x+
	sta ,y+
	decb
	bne memcpy_loop
	
	; LineNumber: 19
	rts
end_procedure_Memory_Copy:
	; ***********  Defining procedure : Memory_Copy16
	;    Procedure type : User-defined procedure
	; LineNumber: 21
	; LineNumber: 20
	; LineNumber: 20
	; LineNumber: 20
Memory_ilen2:	fdb	0
Memory_Copy16_block3:
Memory_Copy16:
	; LineNumber: 22
	; ****** Inline assembler section
	
	ldx Memory_ilen2
	ldy Memory_src
	ldu Memory_dst
memcpy_loop16:
	ldd ,y++
	std ,u++
	leax -2,x
	bne memcpy_loop16
	
	; LineNumber: 33
	rts
end_procedure_Memory_Copy16:
	; ***********  Defining procedure : Memory_Fill
	;    Procedure type : User-defined procedure
	; LineNumber: 36
	; LineNumber: 35
	; LineNumber: 35
	; LineNumber: 35
Memory_Fill_block4:
Memory_Fill:
	; LineNumber: 37
	; ****** Inline assembler section
		lda Memory_b
		ldb Memory_len
		ldx Memory_dst
fill_loop:
		sta ,x+
		decb
		bne fill_loop
	
	; LineNumber: 47
	rts
end_procedure_Memory_Fill:
	; ***********  Defining procedure : Memory_Fill16
	;    Procedure type : User-defined procedure
	; LineNumber: 51
	; LineNumber: 50
	; LineNumber: 50
	; LineNumber: 50
Memory_Fill16_block5:
Memory_Fill16:
	; LineNumber: 52
	; ****** Inline assembler section
		lda Memory_b
		ldy Memory_ilen
		ldu Memory_dst
fill16_loop:
		sta ,u+
		leay -1,y
		bne fill16_loop
	
	; LineNumber: 62
	rts
end_procedure_Memory_Fill16:
	; ***********  Defining procedure : Screen_SetBitmapMode
	;    Procedure type : User-defined procedure
	; LineNumber: 38
	; LineNumber: 37
	; LineNumber: 37
Screen_SetBitmapMode_block6:
Screen_SetBitmapMode:
	; LineNumber: 39
	; ****** Inline assembler section
	 ldb #$08
	 stb $FF90
	
	; LineNumber: 43
	; Assigning memory location
	ldb #$80
	; Calling storevariable on generic assign expression
	stb $ff98
	; LineNumber: 44
	; Assigning memory location
	; 8 bit binop
	; Add/sub where right value is constant number
	; 8 bit binop
	; Add/sub where right value is constant number
	ldb #$60
	orb Screen_b ; end add / sub var with constant
	orb Screen_c ; end add / sub var with constant
	; Calling storevariable on generic assign expression
	stb $ff99
	; LineNumber: 45
	; ****** Inline assembler section
	ldd #$EE00
;	MSB =($70000 + addr) / 2048
 	sta $FF9D	
; LSB =(addr / 8) AND $ff	 	
 	stb $FF9E	
	clr $FF9F
	
	; LineNumber: 53
	rts
end_procedure_Screen_SetBitmapMode:
	; ***********  Defining procedure : Screen_PutPixel320
	;    Procedure type : User-defined procedure
	; LineNumber: 56
	; LineNumber: 55
	; LineNumber: 55
	; LineNumber: 55
Screen_PutPixel320_block7:
Screen_PutPixel320:
	; LineNumber: 68
	; ****** Inline assembler section
;pset
; pshs b
	ldx Screen_x
	ldy Screen_y
	ldb Screen_color
	pshs b
	
 ldu #$7000
 tfr y,d
 lda #40
 mul
 leau d,u
 tfr x,d ; * 4
 asra
 rorb
 asra
 rorb
 leau d,u ; pointer to screen byte
 tfr x,d
 andb #$03
 leax Screen_bytetbl1,pcr
 leay Screen_bytetbl2,pcr
 lda ,u
 anda b,x
 sta ,u
 lda ,s
 anda b,y
 ora ,u
 sta ,u
 
 puls b
; puls b,pc	
	
	; LineNumber: 104
	rts
end_procedure_Screen_PutPixel320:
	; ***********  Defining procedure : Screen_PutPixel640
	;    Procedure type : User-defined procedure
	; LineNumber: 107
	; LineNumber: 106
	; LineNumber: 106
	; LineNumber: 106
Screen_PutPixel640_block8:
Screen_PutPixel640:
	; LineNumber: 109
	; ****** Inline assembler section
;pset
; pshs b
	ldx Screen_x
	ldy Screen_y
	ldb Screen_color
 ldu #$7000
 tfr y,d
 lda #160
 mul
 leau d,u
 tfr x,d
 asra
 rorb
 asra
 rorb
 leau d,u ; pointer to screen byte
 tfr x,d
 andb #$03
 leax Screen_bytetbl1,pcr
 leay Screen_bytetbl2,pcr
 lda ,u
 anda b,x
 sta ,u
 lda ,s
 anda b,y
 ora ,u
 sta ,u
; puls b,pc	
	
	; LineNumber: 141
	rts
end_procedure_Screen_PutPixel640:
	; ***********  Defining procedure : Screen_SetColor
	;    Procedure type : User-defined procedure
	; LineNumber: 147
	; LineNumber: 146
	; LineNumber: 146
Screen_SetColor_block9:
Screen_SetColor:
	; LineNumber: 148
	ldb Screen_c
	; Calling storevariable on generic assign expression
	; Storing to a pointer
	pshs b
	; Loading index
	ldb Screen_b
	lda #0
	ldx #$FFB0
	leax d,x
	puls b
	stb ,x
	; LineNumber: 149
	rts
end_procedure_Screen_SetColor:
	; ***********  Defining procedure : Screen_SetColorRGB
	;    Procedure type : User-defined procedure
	; LineNumber: 152
	; LineNumber: 151
	; LineNumber: 151
	; LineNumber: 151
	; LineNumber: 151
Screen_SetColorRGB_block10:
Screen_SetColorRGB:
	; LineNumber: 153
	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	ldb Screen_r
	andb #$2 ; end add / sub var with constant
	; Compare with pure num / var optimization
	cmpb #$2
	lbne Screen_SetColorRGB_elsedoneblock14
Screen_SetColorRGB_ConditionalTrueBlock12: ;Main true block ;keep:
	; LineNumber: 152
	; 8 bit binop
	; Add/sub where right value is constant number
	ldb Screen_r
	orb #$40 ; end add / sub var with constant
	; Calling storevariable on generic assign expression
	stb Screen_r
Screen_SetColorRGB_elsedoneblock14:
	; LineNumber: 154
	; 8 bit binop
	; Add/sub where right value is constant number
	ldb Screen_r
	andb #$fd ; end add / sub var with constant
	; Calling storevariable on generic assign expression
	stb Screen_r
	; LineNumber: 155
	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	ldb Screen_g
	andb #$2 ; end add / sub var with constant
	; Compare with pure num / var optimization
	cmpb #$2
	lbne Screen_SetColorRGB_elsedoneblock20
Screen_SetColorRGB_ConditionalTrueBlock18: ;Main true block ;keep:
	; LineNumber: 154
	; 8 bit binop
	; Add/sub where right value is constant number
	ldb Screen_g
	orb #$20 ; end add / sub var with constant
	; Calling storevariable on generic assign expression
	stb Screen_g
Screen_SetColorRGB_elsedoneblock20:
	; LineNumber: 156
	; 8 bit binop
	; Add/sub where right value is constant number
	ldb Screen_g
	andb #$fd ; end add / sub var with constant
	; Calling storevariable on generic assign expression
	stb Screen_g
	; LineNumber: 157
	; Binary clause Simplified: EQUALS
	; 8 bit binop
	; Add/sub where right value is constant number
	ldb Screen_b
	andb #$2 ; end add / sub var with constant
	; Compare with pure num / var optimization
	cmpb #$2
	lbne Screen_SetColorRGB_elsedoneblock26
Screen_SetColorRGB_ConditionalTrueBlock24: ;Main true block ;keep:
	; LineNumber: 156
	; 8 bit binop
	; Add/sub where right value is constant number
	ldb Screen_b
	orb #$10 ; end add / sub var with constant
	; Calling storevariable on generic assign expression
	stb Screen_b
Screen_SetColorRGB_elsedoneblock26:
	; LineNumber: 158
	; 8 bit binop
	; Add/sub where right value is constant number
	ldb Screen_b
	andb #$fd ; end add / sub var with constant
	; Calling storevariable on generic assign expression
	stb Screen_b
	; LineNumber: 159
	; 8 bit binop
	; Add/sub where right value is constant number
	; 8 bit binop
	; Add/sub where right value is constant number
	ldb Screen_r
	orb Screen_g ; end add / sub var with constant
	orb Screen_b ; end add / sub var with constant
	; Calling storevariable on generic assign expression
	; Storing to a pointer
	pshs b
	; Loading index
	ldb Screen_c
	lda #0
	ldx #$FFB0
	leax d,x
	puls b
	stb ,x
	; LineNumber: 160
	rts
end_procedure_Screen_SetColorRGB:
	; ***********  Defining procedure : Screen_EnableVsync
	;    Procedure type : User-defined procedure
	; LineNumber: 163
Screen_EnableVsync:
	; LineNumber: 164
	; ****** Inline assembler section
  LDA    $FF03 ;* Load A with the value stored at $FF03
  ORA    #$01  ;* Set bit 1 high
  STA    $FF03 ;* Store A at $FF03 - VSYNC is now enabled
  
	; LineNumber: 171
	rts
end_procedure_Screen_EnableVsync:
	; ***********  Defining procedure : Screen_WaitForVsync
	;    Procedure type : User-defined procedure
	; LineNumber: 174
Screen_WaitForVsync:
	; LineNumber: 175
	; ****** Inline assembler section
* Wait for VSYNC to occur
	   lda     $FF02 ;* Acknowledge the VSYNC interrupt
V_VWAIT:
	   lda     $FF03 ;* Loop until Vsync is triggered
       bpl     V_VWAIT ;* If bit 7 is low then keep looping
	; LineNumber: 184
	rts
end_procedure_Screen_WaitForVsync:
	; ***********  Defining procedure : Tiles_SetTileLoc
	;    Procedure type : User-defined procedure
	; LineNumber: 11
	; LineNumber: 10
Tiles_SetTileLoc_block31:
Tiles_SetTileLoc:
	; LineNumber: 13
	rts
end_procedure_Tiles_SetTileLoc:
	; ***********  Defining procedure : Tiles_WriteToScreen
	;    Procedure type : User-defined procedure
	; LineNumber: 17
	; LineNumber: 16
Tiles_xx:	fcb	0
	; LineNumber: 16
Tiles_yy:	fcb	0
	; LineNumber: 16
Tiles_val:	fcb	0
	; LineNumber: 16
Tiles_wx:	fcb	0
	; LineNumber: 16
Tiles_wy:	fcb	0
Tiles_WriteToScreen_block32:
Tiles_WriteToScreen:
	; LineNumber: 18
	; Generic 16 bit op
	; Forcing byte to be integer 
	ldb Tiles_xx
	lda #0
	tfr d,x
	; Store in temp var
	stx Tiles_WriteToScreen_rightvarInteger_var34
	; HandleVarBinopB16bit
	; Loading index
	ldd #$7000
	pshs d
	; Load pointer array
	; Loading index
	ldb Tiles_yy
	lda #0
	; Multiply by two to lookup integer
	aslb
	rola
	ldx #Tiles_tab32
	ldx d,x
	puls d
	leax d,x
	tfr x,d
	; binop:
	addd Tiles_WriteToScreen_rightvarInteger_var34
	tfr d,x
	stx Tiles_dst
	; LineNumber: 19
	;  integer a:=a+b
	; Loading index
	; Load pointer array
	; Loading index
	ldb Tiles_val
	lda #0
	; Multiply by two to lookup integer
	aslb
	rola
	ldx #Tiles_tlookup2x1
	ldd d,x
	; Load pointer array
	; No index load
	ldx Tiles_tile
	leax d,x
	stx Tiles_src
	; LineNumber: 26
	ldb #$0
	; Calling storevariable on generic assign expression
	stb Tiles_i
Tiles_WriteToScreen_forloop36:
	; LineNumber: 21
	; LineNumber: 22
	ldx Tiles_src
	stx Memory_src
	ldx Tiles_dst
	stx Memory_dst
	ldb Tiles_wx
	; Calling storevariable on generic assign expression
	stb Memory_len
	jsr Memory_Copy
	; LineNumber: 23
	;  integer a:=a+b
	; Load pointer array
	; No index load
	ldx Tiles_src
	leax 64,x
	stx Tiles_src
	; LineNumber: 24
	;  integer a:=a+b
	; Load pointer array
	; No index load
	ldx Tiles_dst
	leax 128,x
	stx Tiles_dst
	; LineNumber: 25
Tiles_WriteToScreen_loopstart37:
	; Compare is onpage
	inc Tiles_i
	ldb Tiles_wy
	cmpb Tiles_i ;keep
	lbne Tiles_WriteToScreen_forloop36
Tiles_WriteToScreen_loopdone41: ;keep:
Tiles_WriteToScreen_loopend38:
	; LineNumber: 26
	rts
end_procedure_Tiles_WriteToScreen:
block1:
main_block_begin_:
main_block_end_:
	; End of program
	; Ending memory block at $2000
; based on https://github.com/jmatzen/leventhal-6809/blob/master/6809_MUL16.s
mul16_internal:
        ; use stack
        ldu ,s
        clra
        clrb
        std ,s
        pshs d
        lda	5,s
        ldb	7,s
        mul
        stb	3,s
        sta	2,s
        lda	5,S
        lda	6,S
        mul
        addb	2,S
        stb	2,S
        adca	#0
        sta	1,S
        lda	4,S
        ldb	7,S
        mul
        addb	2,S
        stb	2,S
        adca	1,S
        sta	1,S
        bcc	mul16_skip
        inc	,S
mul16_skip:
        lda	4,S
        ldb	6,S
        mul
        addb	1,S
        adca	,S
        ldx	2,S
        leas	 6,S
        pshs D,X
        jmp	,U
SREM16:
        LDA	#$FF		; INDICATE REMAINDER TO BE RETURNED
        STA	,-S		; SAVE INDICATOR ON STACK
        BRA	CHKSGN		; GO CHECK SIGNS
; SIGNED DIVISION, RETURNS QUOTIENT
;
SDIV16:
        CLR	,-S		; INDICATE QUOTIENT TO BE RETURNED
;
; IF DIVISOR IS NEGATIVE, TAKE ITS ABSOLUTE VALUE AND INDICATE
; THAT QUOTIENT IS NEGATIVE
;
CHKSGN:
        LDD	#0		; INDICATE QUOTIENT, REMAINDER POSITIVE
        PSHS	D		; SAVE INDICATOR ON STACK LEAX 5,8 POINT TO DIVISOR
        TST	,X		; CHECK IF DIVISOR IS POSITIVE
        BPL	CHKDVD		; BRANCH IF DIVISOR IS POSITIVE
        SUBD	,X		; ELSE TAKE ABSOLUTE VALUE OF DIVISOR
        STD	,X
        COM	1,S		; INDICATE QUOTIENT IS NEGATIVE
        BRA	CHKZRO
;
; IF DIVIDEND IS NEGATIVE,
; TAKE ITS ABSOLUTE VALUE,
; INDICATE THAT REMAINDER IS NEGATIVE,
; AND INVERT SIGN OF QUOTIENT
;
CHKDVD: LEAX	2,X		; POINT TO HIGH BYTE OF DIVIDEND
        TST	,X		; CHECK IF DIVIDEND IS POSITIVE
        BPL	CHKZRO		; BRANCH IF DIVIDEND IS POSITIVE
        LDD	#0		; ELSE TAKE ABSOLUTE VALUE OF DIVIDEND
        SUBD	,X
        COM	,S		; INDICATE REMAINDER IS NEGATIVE
        COM	1,S		; INVERT SIGN OF QUOTIENT
;
; UNSIGNED 16-BIT DIVISION, RETURNS QUOTIENT
;
UDIV16: CLR	,-S		; INDICATE QUOTIENT TO BE RETURNED
        BRA	CLRSGN
;
; UNSIGNED 16-BIT DIVISION, RETURNS REMAINDER
;
UREM16:
        LDA	#$FF		; INDICATE REMAINDER TO BE RETURNED
        STA	,-S
;
; UNSIGNED DIVISION, INDICATE QUOTIENT, REMAINDER BOTH POSITIVE
;
CLRSGN:
        LDD	#0		; INDICATE QUOTIENT, REMAINDER POSITIVE
        PSHS	D
;
; CHECK FOR ZERO DIVISOR
; EXIT, INDICATING ERROR, IF FOUND
;
CHKZRO:
        LEAX	5,S		; POINT TO DIVISOR
        LDD	,X		; TEST DIVISOR
        BNE	STRTDV		; BRANCH IF DIVISOR NOT ZERO
        STD	2,X		; DIVISOR IS ZERO, SO MAKE RESULT ZERO
;--	SEC			; INDICATE DIVIDE BY ZERO ERROR
        ORCC	#1
        BRA	EXITDV		; EXIT INDICATING ERROR
;
; DIVIDE UNSIGNED DIVIDEND BY UNSIGNED DIVISOR
; MEMORY ADDRESSES HOLD BOTH DIVIDEND AND QUOTIENT.
; EACH TIME WE SHIFT THE DIVIDEND ONE BIT LEFT,
; WE ALSO SHIFT A BIT OF THE
; QUOTIENT IN FROM THE CARRY AT THE FAR RIGHT
; AT THE END, THE QUOTIENT HAS REPLACED THE DIVIDEND IN MEMORY
; AND THE REMAINDER IS LEFT IN REGISTER D
;
STRTDV: LDD	#0		; EXTEND DIVIDEND TO 32 BITS WITH 0
        LDY	#16		; BIT COUNT = 16
;--	CLC			; START CARRY AT ZERO
        ANDCC	#$FE
;
; SHIFT DIVIDEND LEFT WITH ENTERING AT FAR RIGHT
;
DIV16:
        ROL	3,X		; SHIFT LOW  BYTE OF DIVIDEND
                                ; QUOTIENT BIT ENTERS FROM CARRY
        ROL	2,X		; SHIFT NEXT BYTE OF DIVIDEND
        ROLB			; SHIFT NEXT BYTE OF DIVIDEND
        ROLA			; SHIFT HIGH BYTE OF DIVIDEND
;
; DO A TRIAL SUBTRACTION OF DIVISOR FROM DIVIDEND
; IF DIFFERENCE IS NON-NEGATIVE, SET NEXT BIT OF QUOTIENT.
; PERFORM ACTUAL SUBTRACTION, REPLACING QUOTIENT WITH DIFFERENCE.
; IF DIFFERENCE IS NEGATIVE, CLEAR NEXT BIT OF QUOTIENT
;
        CMPD	,X		; TRIAL SUBTRACTION OF DIVISOR
        BCS	CLRCRY		; BRANCH IF SUBTRACTION FAILS
        SUBD	,X		; TRIAL SUBTRACTION SUCCEEDED,
                                ; SO SUBTRACT DIVISOR FROM
                                ; DIVIDEND
;--	SEC			; SET NEXT BIT OF QUOTIENT TO 1
        ORCC	#1
        BRA	DECCNT
CLRCRY:
;--	CLC			; TRIAL SUBTRACTION FAILED, SO
        ANDCC	#$FE
                                ; SET NEXT BIT OF QUOTIENT TO 0
;
; UPDATE BIT COUNTER
; CONTINUE THROUGH 16 BITS
;
DECCNT: LEAY	-1,Y		; CONTINUE UNTIL ALL BITS DONE
        BNE	DIV16
;
; SHIFT LAST CARRY INTO QUOTIENT
;
        ROL	3,X		; SHIFT LAST CARRY INTO QUOTIENT
        ROL	2,X		; INCLUDING MORE SIGNIFICANT BYTE
;
; SAVE REMAINDER IN STACK
; NEGATE REMAINDER IF INDICATOR SHOWS IT IS NEGATIVE
;
        STD	,X		; SAVE REMAINDER IN STACK
        TST	,S		; CHECK IF REMAINDER IS POSITIVE
        BEQ	TSTQSN
                                ; BRANCH IF REMAINDER IS POSITIVE
        LDD	#0		; ELSE NEGATE IT
        STD	,X		; SAVE NEGATIVE REMAINDER
;
; NEGATE QUOTIENT IF INDICATOR SHOWS IT IS NEGATIVE
;
TSTQSN: TST	1,S		; CHECK IF QUOTIENT IS POSITIVE
        BEQ	TSTRTN		; BRANCH IF QUOTIENT IS POSITIVE LDD #0 ELSE NEGATE IT
        SUBD	7,S
        STD	7,S		; SAVE NEGATIVE QUOTIENT
;
; SAVE QUDTIENT OR REMAINDER, DEPENDING ON FLAG IN STACK
;
TSTRTN:
;--	CLC			; INDICATE NO DIVIDEBYZERO ERROR
        ANDCC	#$FE
        TST	2,S		; TEST QUOTIENT/REMAINDER FLAG
        BEQ	EXITDV		; BRANCH TO RETURN QUOTIENT
        LDD	,X		; REPLACE QUOTIENT WITH REMAINDER
        STD	7,S
; REMOVE PARAMETERS FROM STACK AND EXIT
;
EXITDV:	LDX	3,S		; SAVE RETURN ADDRESS
        LEAS	7,S		; REMOVE PARAMETERS FROM STACK
        JMP	,X		; EXIT TO RETURN ADDRESS
Tiles_WriteToScreen_rightvarInteger_var34:	 fdb	0 
EndBlock2000:
	END START
