; restore bottom row of characters so they don't scroll
vbmScrollFixBottom

	; screen start
    lda #$DC
    ldy #$10
    sta screenmemory
    sty screenmemory+1
    
    ; character start
    lda #27
    sta vbmI
    
	ldy #0
    ldx #20

vbmSFB_loop

	lda vbmI
	sta (screenmemory),y
	clc
	adc #12	// skip 12 chars
	sta vbmI
	
	lda screenmemory
	adc #1
	sta screenmemory

	dex
	bne vbmSFB_loop