
; reset scroll right position
vbmSR_ResetToEnd

	lda #244
	sta vbmScroll
	lda #$40
	sta vbmScrLstart
	lda #$1F
	sta vbmScrHstart
	jmp vbmSR_update

; scroll the screen to the right
vbmScrollRight

	; character map
	lda vbmScroll
	sec
	sbc #12 ; 12 dbl height chars per row
	cmp #4
	beq vbmSR_ResetToEnd ; reached start - reset to end values
	sta vbmScroll
	
	; bitmap column start addresses
	lda vbmScrLstart
	sec
	sbc #192 ; 192 pixels per row
	bcs vbmSR_overflow
	dec vbmScrHstart
vbmSR_overflow
	sta vbmScrLstart
	
vbmSR_update

	; update char map and bitmap column addresses
    jsr vbmLayoutCharacters
    jsr vbmCreateColumnAddresses