; restore top row of characters so they don't scroll
vbmScrollFixTop

	; screen start
    lda #$00
    ldy #$10
    sta screenmemory
    sty screenmemory+1
    
    ; character start
    lda #16
    sta vbmI
    
	ldy #0
    ldx #20

vbmSFT_loop

	lda vbmI
	sta (screenmemory),y
	clc
	adc #12	// skip 12 chars
	sta vbmI
	
	lda screenmemory
	adc #1
	sta screenmemory

	dex
	bne vbmSFT_loop