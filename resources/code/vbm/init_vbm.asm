; Screen address table Low byte / high byte
vbmScrL
	dc.b 0,0,0,0,0,0,0,0,0,0
	dc.b 0,0,0,0,0,0,0,0,0,0
vbmScrH
	dc.b 0,0,0,0,0,0,0,0,0,0
	dc.b 0,0,0,0,0,0,0,0,0,0

vbmX dc.b 0	; x position
vbmY dc.b 0	; y position
vbmI dc.b 0 ; index	
	
vbmSetDisplayMode

	lda $9000
	cmp #$c;keep
	beq vbmIsPal
	
	lda $9000	; Adjust horizontal position for NTSC
	clc
	adc #1	
	sta $9000
	jmp vbmIsNtsc
	
vbmIsPal

	lda $9000	; Adjust horizontal position for PAL
	clc
	adc #2
	sta $9000

vbmIsNtsc

	lda #25		; (12x2) + 1
	sta $9003	; set screen height to 12 double height chars

	lda #20
	sta $9002	; set screen width to 20 characters

	lda $9001
	sec
	sbc #1
	sta $9001	; adjust vertical position

	lda #%11001100	; 204 - set screen and character to $1000
	sta $9005

	
vbmLayoutCharacters

	; temporary address table for physical character memory
	ldx #0
        lda #<$1000
        ldy #>$1000
	sta vbmScrL,x
        tya
        sta vbmScrH,x
vbmAddrTable1
	lda vbmScrL,x
	inx
	clc
	adc #20		; width
	bcc vbmNoOverflow1
	iny
vbmNoOverflow1
	sta vbmScrL,x
        tya
        sta vbmScrH,x
	cpx #11		; height (12 double height chars)-1
	bcc vbmAddrTable1

	; Layout characters on screen
	lda #16 	; character to start drawing with
	sta vbmI
	lda #0
	sta vbmX
vbmXDrawForLoop
	lda #0
	sta vbmY
vbmYDrawForLoop	
	; Get Address of row
	ldx vbmY;keep
	lda vbmScrL,x   ; Address of table lo
	ldy vbmScrH,x   ; Address of table hi	
	sta screenmemory
	sty screenmemory+1
	
	lda vbmI
	ldy vbmX
	sta (screenmemory),y
	inc vbmI
	inc vbmY
	lda #12	; comapare 12 rows
	cmp vbmY ;keep
	bne vbmYDrawForLoop ; loop rows
	
	inc vbmX
	lda #20	; compare 20 columns
	cmp vbmX ;keep
        bne vbmXDrawForLoop	; loop columns

	; final address table for logical character memory
	; (bitmap x start addresses)
	ldx #0
        lda #<$1100
        ldy #>$1100
	sta vbmScrL,x
        tya
        sta vbmScrH,x
vbmAddrTable2
	lda vbmScrL,x
	inx
	clc
	adc #192 ; height of screen in pixels
	bcc vbmNoOverflow2
	iny
vbmNoOverflow2
	sta vbmScrL,x
        tya
        sta vbmScrH,x
	cpx #19		; width in characters-1
	bcc vbmAddrTable2

