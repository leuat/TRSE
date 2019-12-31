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
