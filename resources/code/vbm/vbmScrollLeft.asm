
; reset scroll left position
vbmSL_ResetToStart

	lda #16
	sta vbmScroll
	lda #$00
	sta vbmScrLstart
	lda #$11
	sta vbmScrHstart
	jmp vbmSL_update

; scroll the screen to the left
vbmScrollLeft

	; character map
	lda vbmScroll
	clc
	adc #12 ; 12 dbl height chars per row
	beq vbmSL_ResetToStart ; reached end - reset to start values
	sta vbmScroll
	
	; bitmap column start addresses
	lda vbmScrLstart
	clc
	adc #192 ; 192 pixels per row
	bcc vbmSL_overflow
	inc vbmScrHstart
vbmSL_overflow
	sta vbmScrLstart
	
vbmSL_update

	; update char map and bitmap column addresses
    jsr vbmLayoutCharacters
    jsr vbmCreateColumnAddresses