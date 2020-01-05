 processor 6502
	ORG $1201
	.byte    $0E, $08, $0A, $00, $9E, $20, $28
	.byte   $34,$36,$32,$34
	.byte    $29, $00, $00, $00
	ORG $1210
TileMap
	jmp block41
 ; Temp vars section
 ; Temp vars section ends
EndSymbol
	org $2000
i	dc.b	
message	
	dc.b	$10, $12, $05, $13, $13, $20, $20, $31, $20
	dc.b	$0f, $12, $20, $32, 0
	
	
	; ***********  Defining procedure : initVbm
	;    Procedure type : User-defined procedure
	
	; Initialise the core VBM (Vic20 Bitmap Mode) library
	; Created by Andy H - Hewco.uk for use in Turbo Rascal
	; See help to get started, all commmands begin with 'vbm'
; Screen address table Low byte / high byte
vbmScrL
    dc.b 0,0,0,0,0,0,0,0,0,0
    dc.b 0,0,0,0,0,0,0,0,0,0
vbmScrH
    dc.b 0,0,0,0,0,0,0,0,0,0
    dc.b 0,0,0,0,0,0,0,0,0,0
vbmX        dc.b 0 ; x position
vbmY        dc.b 0 ; y position
vbmI        dc.b 0 ; index
vbmJ        dc.b 0 ; index
vbmT        dc.b 0 ; index
vbmScroll   dc.b 16       ; character scroll start
vbmScrLstart dc.b $00   ; start address for bitmap L
vbmScrHstart dc.b $11   ; start address for bitmap H
vbmNumColumns dc.b 20        ; number of columns
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
    lda vbmNumColumns ;#20
    sta $9002	; set screen width to 20 characters
    cmp #20
    beq vbmSDM_noadjust
    lda $9000	; 19 column mode, move horiz position another 4 pixels right to centre
    clc
    adc #1
    sta $9000
vbmSDM_noadjust
    lda $9001
    sec
    sbc #1
    sta $9001	; adjust vertical position
    lda #%11001100	; 204 - set screen and character to $1000
    sta $9005
    jsr vbmLayoutCharacters
    jsr vbmCreateColumnAddresses
    rts
;; Write column start addresses
vbmCreateColumnAddresses
    ; address table for logical character memory
    ; (bitmap x start addresses)
    ldx #0
    lda vbmScrLstart    ; L start address of bitmap
    sta vbmScrL,x
    ldy vbmScrHstart    ; H start address of bitmap
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
    cpy #$20             ; address needs to wrap back around?
    bne vbmAddrTable3
    ; reset address to start of memory VIC bitmap
    lda #$00
    sta vbmScrL,x
    ldy #$11
    tya
    sta vbmScrH,x
vbmAddrTable3
    cpx #19		; width in characters-1
    bcc vbmAddrTable2
    rts
;; Draw character map to screen to form bitmap layout
vbmLayoutCharacters
    ; Layout characters on screen
    lda vbmScroll 	; character to start drawing with (16)
    sta vbmI            ; character
    lda #0
    sta vbmX
vbmXDrawForLoop
    lda #0
    sta vbmY
    ; screen start
    lda #$00
    ldy #$10
    sta screenmemory
    sty screenmemory+1
vbmYDrawForLoop
    lda vbmI
    ldy vbmX
    sta (screenmemory),y
    ; add 20 for next row (or 19)
    lda screenmemory
    clc
    adc vbmNumColumns ; 20
    sta screenmemory
    bcc vbmYDrawForLoopOverflow
    inc screenmemory+1
vbmYDrawForLoopOverflow
    inc vbmY
    inc vbmI    ; character
    bne vbmDrawForLoopResetChar
    lda #16     ; reset character
    sta vbmI
vbmDrawForLoopResetChar
    lda #12	; comapare 12 rows
    cmp vbmY ;keep
    bne vbmYDrawForLoop ; loop rows
    inc vbmX
    lda vbmNumColumns	; compare 20 columns
    cmp vbmX ;keep
    bne vbmXDrawForLoop	; loop columns
	rts
	
	
	; ***********  Defining procedure : initVbmClear
	;    Procedure type : User-defined procedure
	
	; VBM Clear bitmap routine
vbmClear
	; Address of bitmap $1100
	lda #0
	ldx #17
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
	lda #192    ; reached end of row?
	cmp vbmY ;keep
	bne vbmClearForY
	lda screenmemory
	clc
	adc #192
	bcc vbmClearXOverflow
	inc screenmemory+1
vbmClearXOverflow
	sta screenmemory
	inc vbmX
	lda #20 ; reched end of column?
	cmp vbmX ;keep
	bne vbmClearForX
    ; bitmap has been cleared
	rts
	
	
	; ***********  Defining procedure : initVbmDrawText
	;    Procedure type : User-defined procedure
	
	; Draw text characters to the bitmap using a zero terminated CSTRING
	; CSTRING    = $4c
	; Font chars = $4e
	; Temp addr  = $50 - used to calculate char address
vbmDrawText
vbmDTX_Xloop
	; calculate next screen memory position
	ldx vbmX
	lda vbmScrL,x   ; Address of table lo
	ldy vbmScrH,x   ; Address of table hi
	clc
	adc vbmY		; Add Y offset
	bcc vbmDTX_NSP_NoOverflow
	iny
vbmDTX_NSP_NoOverflow
	sta screenmemory
	sty screenmemory+1
vbmDTX_GetCharNum
	; convert text number (0-255) * 8 = memory offset
	ldy #0
	lda ($4c),y		; get char from current position in CSTRING
	bne vbmDTX_NotEnd
	rts ; if =0, we are end of the cstring
vbmDTX_NotEnd
	sta $50
	sty $50+1
	asl $50
	rol $50+1 ;x2
	asl $50
	rol $50+1 ;x4
	asl $50
	rol $50+1 ;x8
	lda $50
	clc
	adc $4e  ; add tile low address
	sta $50
	lda $50+1
	adc $4e+1 ; add tile high address
	sta $50+1
vbmDTX_DrawChar
	; y reg is ZERO from ldy #0 in GetTileNum
	lda ($50),y
	sta (screenmemory),y
	iny
	lda ($50),y
	sta (screenmemory),y
	iny
	lda ($50),y
	sta (screenmemory),y
	iny
	lda ($50),y
	sta (screenmemory),y
	iny
	lda ($50),y
	sta (screenmemory),y
	iny
	lda ($50),y
	sta (screenmemory),y
	iny
	lda ($50),y
	sta (screenmemory),y
	iny
	lda ($50),y
	sta (screenmemory),y
vbmDTX_NextChar
	clc
	inc $4c  ; low byte
	bne vbmDTX_NTM_NoOverflow
	inc $4c+1  ; high byte
vbmDTX_NTM_NoOverflow
	; next x pos on screen
	inc vbmX
	lda #20   ; 0-19 columns, 20 means exceeded right of screen
	cmp vbmX  ; has x pos exceeded?
	bne vbmDTX_Xloop  ; no, draw next char
	; yes, set x back to 0 and inc vbmY by line height (pixels)
	lda #0
	sta vbmX
	lda vbmY
	clc
	adc vbmI
	sta vbmY
	jmp vbmDTX_Xloop
	rts
	
	
	; ***********  Defining procedure : initVbmDrawTileMap
	;    Procedure type : User-defined procedure
	
	; Draw tile characters to the bitmap using a tilemap array (columns,rows)
	; Tilemap    = $4c
	; Tile chars = $4e
	; Temp addr  = $50 - used to calculate tile address
vbmDrawTileMap
vbmDTM_Xloop
	; calculate next screen memory position
	ldx vbmX
	lda vbmScrL,x   ; Address of table lo
	ldy vbmScrH,x   ; Address of table hi
	clc
	adc vbmY		; Add Y offset
	bcc vbmDTM_NSP_NoOverflow
	iny
vbmDTM_NSP_NoOverflow
	sta screenmemory
	sty screenmemory+1
vbmDTM_GetTileNum
	; convert tile number (0-255) * 8 = memory offset
	ldy #0
	lda ($4c),y		; get tile from current position on tilemap
	sta $50
	sty $50+1
	asl $50
	rol $50+1 ;x2
	asl $50
	rol $50+1 ;x4
	asl $50
	rol $50+1 ;x8
	lda $50
	clc
	adc $4e  ; add tile low address
	sta $50
	lda $50+1
	adc $4e+1 ; add tile high address
	sta $50+1
vbmDTM_DrawTile
	; y reg is ZERO from ldy #0 in GetTileNum
	lda ($50),y
	sta (screenmemory),y
	iny
	lda ($50),y
	sta (screenmemory),y
	iny
	lda ($50),y
	sta (screenmemory),y
	iny
	lda ($50),y
	sta (screenmemory),y
	iny
	lda ($50),y
	sta (screenmemory),y
	iny
	lda ($50),y
	sta (screenmemory),y
	iny
	lda ($50),y
	sta (screenmemory),y
	iny
	lda ($50),y
	sta (screenmemory),y
vbmDTM_NextTileMap
	clc
	inc $4c  ; low byte
	bne vbmDTM_NTM_NoOverflow
	inc $4c+1  ; high byte
vbmDTM_NTM_NoOverflow
	; next x pos on screen
	inc vbmX
	lda vbmI   ; tilemap end x pos
	cmp vbmX  ; have we reached this?
	bne vbmDTM_Xloop  ; no, draw next column
	; yes, set x back to start x and inc vbmY by 8 rows (pixels)
	lda vbmT
	sta vbmX
	; check if y pos at end and loop if not
	lda vbmY
	clc
	adc #8
	sta vbmY
	lda vbmJ   ; tilemap end Y pos in pixels
	cmp vbmY  ; have we reached this?
	bcc vbmDTM_Done
	jmp vbmDTM_Xloop
vbmDTM_Done
	rts
block41
screenmemory =  $fe
	; Set special display mode for VBM bitmap graphics
	lda #20
	sta vbmNumColumns
	jsr vbmSetDisplayMode
	; Clear VBM bitmap
	lda #0
	sta vbmI ; byte to clear bitmap with
	jsr vbmClear
	lda #$93
	sta screenmemory+1
	lda #$ff
	sta screenmemory
	lda #5
	ldy #241 ; colour mem to clear (stops at zero so +1)
vbmCC_loop18467
	sta (screenmemory),y
	dey
	bne vbmCC_loop18467
	; Assigning memory location
	; Assigning single variable : $900f
	lda #13
	
	sta $900f
	; Draw 8x8 text to the bitmap
	; Text to use:
	lda #<message
	sta $4c
	lda #>message
	sta $4c+1
	; Font characters to use:
	lda #<font
	sta $4e
	lda #>font
	sta $4e+1
	lda #3
	sta vbmX ; x position
	lda #100
	sta vbmY ; y position in pixels
	lda #8
	sta vbmI ; line height in pixels
	jsr vbmDrawText
while6334
	; Binary clause: NOTEQUALS
	lda #1
	; Compare with pure num / var optimization
	cmp #$0;keep
	; BC done
	beq binaryclausefailed23281
binaryclausesuccess9961
	lda #1; success
	jmp binaryclausefinished16827
binaryclausefailed23281
	lda #0 ; failed state
binaryclausefinished16827
	cmp #1
	beq ConditionalTrueBlock26500
	jmp elsedoneblock15724
ConditionalTrueBlock26500
	; Assigning single variable : i
	lda 197
	
	sta i
	cmp #$0 ;keep
	bne casenext11942
	; Using a tilemap, draw a screenfull of tiles to the bitmap
	; Tilemap to use:
	lda #<map
	sta $4c
	lda #>map
	sta $4c+1
	; Tile characters to use:
	lda #<tiles
	sta $4e
	lda #>tiles
	sta $4e+1
	lda #0
	sta vbmX ; x start position
	sta vbmT ; x start position reset
	sta vbmY ; y start position in pixels
	lda #20
	sta vbmI ; x end position
	lda #184
	sta vbmJ ; y end position in pixels
	jsr vbmDrawTileMap
	jmp caseend2995
casenext11942
	lda i
	cmp #$32 ;keep
	bne casenext5436
	; Clear VBM bitmap
	lda #0
	sta vbmI ; byte to clear bitmap with
	jsr vbmClear
	; Using a tilemap, draw a screenfull of tiles to the bitmap
	; Tilemap to use:
	lda #<map10
	sta $4c
	lda #>map10
	sta $4c+1
	; Tile characters to use:
	lda #<tiles
	sta $4e
	lda #>tiles
	sta $4e+1
	lda #5
	sta vbmX ; x start position
	sta vbmT ; x start position reset
	lda #100
	sta vbmY ; y start position in pixels
	lda #15
	sta vbmI ; x end position
	lda #172
	sta vbmJ ; y end position in pixels
	jsr vbmDrawTileMap
	jmp caseend2995
casenext5436
caseend2995
	; wait for raster
	ldx #100 ; optimized, look out for bugs
	cpx $9004
	bne *-3
	jmp while6334
elseblock19169
elsedoneblock15724
EndBlock218
	org $4000
tiles
	incbin "C:/Users/ovine/OneDrive/Documents/GitHub/TRSE/Publish/tutorials/VIC20/tutorials///resources/charsets/tiles.bin"
	org $4050
map
	incbin "C:/Users/ovine/OneDrive/Documents/GitHub/TRSE/Publish/tutorials/VIC20/tutorials///resources/charsets/tilemap.bin"
	org $4230
map10
	incbin "C:/Users/ovine/OneDrive/Documents/GitHub/TRSE/Publish/tutorials/VIC20/tutorials///resources/charsets/tilemap10x10.bin"
	org $4294
font
	incbin "C:/Users/ovine/OneDrive/Documents/GitHub/TRSE/Publish/tutorials/VIC20/tutorials///resources/charsets/font8x8.bin"
