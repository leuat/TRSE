vbmClear
	; Address of bitmap $1100
        lda #$00 ; 0
        ldx #$11 ; 17
	sta screenmemory
	stx screenmemory+1

	sta vbmX
vbmClearForX

	lda #0
	sta vbmY
vbmClearForY

	lda vbmI ; vbmI contains byte to clear bitmap with
	ldy vbmY
	sta (screenmemory),y
	inc vbmY
        lda vbmScrHeight ; #192    ; reached end of row?
	cmp vbmY ;keep
	bne vbmClearForY

	lda screenmemory

	clc
        adc vbmScrHeight ; #192
	bcc vbmClearXOverflow
	inc screenmemory+1
vbmClearXOverflow
	sta screenmemory
	inc vbmX
        lda vbmNumColumns ; #20 ; reched end of column?
	cmp vbmX ;keep
	bne vbmClearForX
    ; bitmap has been cleared
