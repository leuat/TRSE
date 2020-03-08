; Screen address table Low byte / high byte
vbmScrL = $0d ; 20 bytes
;    dc.b 0,0,0,0,0,0,0,0,0,0
;    dc.b 0,0,0,0,0,0,0,0,0,0
vbmScrH = $21 ; 20 bytes
;    dc.b 0,0,0,0,0,0,0,0,0,0
;    dc.b 0,0,0,0,0,0,0,0,0,0
; ends at $35

vbm9000     = $00 ; store $9000 address value
vbm9001     = $01 ; store $9001 address value
vbm9005     = $02 ; store $9005 address value
vbmX        = $03 ; x position
vbmY        = $04 ; y position
vbmI        = $05 ; index
vbmJ        = $06 ; index
vbmT        = $07 ; index
vbmScroll   = $08    ; 16 - character scroll start
vbmNumColumns = $09  ; 20 - number of columns
vbmScrLstart = $0a   ; $00 - start address for bitmap L
vbmScrHstart = $0b   ; $11 - start address for bitmap H
vbmScrHeight = $0c   ; 192 - default height of bitmap modes 0 and 1

; vbmJ = temporarily set to number of dbl height characters per row - 12, 10 or 9
vbmSetDisplayMode

    ; initialise
    lda #16 ; start char
    sta vbmScroll
    lda #$00 ; address L
    sta vbmScrLstart
    lda #$11 ; address H
    sta vbmScrHstart

    lda $9000
    sta vbm9000
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

    lda vbmNumColumns ;#20
    sta $9002	; set screen width to 20, 19, 18 or 17 characters
    cmp #20
    beq vbmSDM_noadjust
    cmp #19
    beq vbmSDM_19Cols
    cmp #18
    beq vbmSDM_18Cols

vbmSDM_17Cols
    lda $9000	; 17 column mode, move horiz position another 4 pixels right to centre
    clc
    adc #3      ; move 12 pixels across
    sta $9000
    jmp vbmSDM_noadjust

vbmSDM_18Cols
    lda $9000	; 18 column mode, move horiz position another 4 pixels right to centre
    clc
    adc #2      ; move 8 pixels across
    sta $9000
    jmp vbmSDM_noadjust

vbmSDM_19Cols
    lda $9000	; 19 column mode, move horiz position another 4 pixels right to centre
    clc
    adc #1      ; move 4 pixels across
    sta $9000

vbmSDM_noadjust

    lda $9001
    sta vbm9001

    ldy vbmScrHeight
    cpy #192
    beq vbmSDM_192Rows
    cpy #176
    beq vbmSDM_176Rows
    cpy #160
    beq vbmSDM_160Rows

vbmSDM_144Rows
    clc
    adc #12      ; move down
    sta $9001	; adjust vertical position

    lda #19		; (9x2) + 1
    sta $9003	; set screen height to 12 double height chars

    jmp vbmSDM_RowsDone

vbmSDM_160Rows
    clc
    adc #8      ; move down
    sta $9001	; adjust vertical position

    lda #21		; (10x2) + 1
    sta $9003	; set screen height to 12 double height chars

    jmp vbmSDM_RowsDone

vbmSDM_176Rows
    clc
    adc #4      ; move down
    sta $9001	; adjust vertical position

    lda #23		; (11x2) + 1
    sta $9003	; set screen height to 12 double height chars

    jmp vbmSDM_RowsDone

vbmSDM_192Rows
    sec
    sbc #1      ; move up
    sta $9001	; adjust vertical position

    lda #25		; (12x2) + 1
    sta $9003	; set screen height to 12 double height chars

vbmSDM_RowsDone

    lda $9005
    sta vbm9005
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
    ldy vbmScrHstart    ; H start address of bitmap - Y reg to retain high byte
    tya
    sta vbmScrH,x
vbmAddrTable2
    lda vbmScrL,x
    inx
    clc
    adc vbmScrHeight    ; #192 ; height of screen in pixels
    bcc vbmNoOverflow2
    iny
vbmNoOverflow2
    sta vbmScrL,x
    tya
    sta vbmScrH,x

    ; Test high byte in Y reg
    cpy #$20             ; address needs to wrap back around?
    bne vbmAddrTable3
    ; reset address to start of memory VIC bitmap
    lda #$00
    sta vbmScrL,x
    ldy #$11
    tya
    sta vbmScrH,x

vbmAddrTable3
    cpx #19		; width in characters-1 ** note for smaller widths these are still calculated
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
    adc vbmNumColumns ; 20 (19, 18 or 17)
    sta screenmemory
    bcc vbmYDrawForLoopOverflow
    inc screenmemory+1

vbmYDrawForLoopOverflow

    inc vbmY
    inc vbmI    ; character
    bne vbmDrawForLoopResetChar
    lda #16     ; reset character back to #16
    sta vbmI

vbmDrawForLoopResetChar

    lda vbmJ ; #12	; comapare 12 rows
    cmp vbmY ;keep
    bne vbmYDrawForLoop ; loop rows

    inc vbmX
    lda vbmNumColumns	; compare 20 columns
    cmp vbmX ;keep
    bne vbmXDrawForLoop	; loop columns
