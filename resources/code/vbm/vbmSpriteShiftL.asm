; calculate the pre-shifted positions for a sprite character
; p1 = source character
; p2 = destination address for first column
; p3 = destination address for second column
; y = number of rows
; x = shift increment 1 = all 8 positions, 2 = 4 positions, 4 = 2 positions
;
vbmSpriteShiftL
    lda #8
    sta vbmY

	lda #0
	sta vbmT    ; Number of rows rows
vbmSS8_RowLoop
	
	ldy #0

vbmSS8_CopyOriginalLoop
	lda (vbmP1),y	; read 
	sta (vbmP2),y	; copy to 1st column
;	lda #0
;	sta (vbmP3),y	; zero out 2nd column
	iny
	cpy vbmY
	bne vbmSS8_CopyOriginalLoop
		
		
	
	; vbmT to count the number of shifts
	lda vbmT
	cmp #0;keep
	beq vbmSS8_NoFirstShift ; don't need to do anything for first shift (actual position)

	lda #0
	sta vbmI
vbmShiftLoop

	ldy #0
			
vbmSpriteShiftLoop
			
	lda (vbmP2),y	; src 1st column
	clc			; clear carry
	ror
	sta (vbmP2),y	; 1st column
	
;	lda (vbmP3),y	; src 2nd column
;	ror
;	sta (vbmP3),y	; second column	
	
	iny
	cpy	vbmY
	bne vbmSpriteShiftLoop

			
	
	inc vbmI
	lda vbmT
	cmp vbmI ;keep
	bne vbmShiftLoop

vbmSS8_NoFirstShift

	lda vbmP2
	
	clc
	adc vbmY
	bcc vbmSS8_p2overflow
	inc vbmP2+1
vbmSS8_p2overflow
	sta vbmP2

	lda vbmP3
	
	clc
	adc vbmY ; row height
	bcc vbmSS8_p3overflow
	inc vbmP3+1
vbmSS8_p3overflow
	sta vbmP3

	lda vbmT
	clc
	adc vbmX
	
	sta vbmT
   	;lda vbmT
	cmp vbmY;keep
	bcs vbmSS8_Done

	jmp vbmSS8_RowLoop

vbmSS8_Done
	;rts
