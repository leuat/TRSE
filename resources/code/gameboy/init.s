
SECTION "VBL",ROM0[$0040]		; vblank interrupt handler
        jp	internal_vblank

SECTION "TIMER",ROM0[$0050]		; timer interrupt handler
        jp	internal_timer

    SECTION	"Start",ROM0[$100]		; start vector, followed by header data applied by rgbfix.exe
	nop
	jp	start

        SECTION "Code",ROM0[$150]		; code starts here


internal_vblank:
        push af
        push bc
        push de
        push hl
        call	$FF80				; copy OAM mirror table using DMA
        pop hl
        pop de
        pop bc
        pop af
        reti

internal_timer:
        push af
        push bc
        push de
        push hl
        call	$FF90				; copy OAM mirror table using DMA
        pop hl
        pop de
        pop bc
        pop af
        reti


; Copies 8 bytes on HBLank
internal_copy_hblank8
    ld a, [de]
    inc de
    ld b, a

    ld a, [de]
    inc de
    ld c, a

waitfor_hblank_internal:
    ld a,[$FF41]
    and 3
    cp 0
    jr nz, waitfor_hblank_internal
    ; 1
    ld a,b
    ld [hl+],a
    ; 2
    ld a,c
    ld [hl+],a
    ; 3
    ld a,[de]
    ld [hl+],a
    inc de
    ; 4
    ld a,[de]
    ld [hl+],a
    inc de
    ; 5
    ld a,[de]
    ld [hl+],a
    inc de
    ; 6

    ld a,[de]
    ld [hl+],a
    inc de
    ; 7

    ld a,[de]
    ld [hl+],a
    inc de
    ; 8

    ld a,[de]
    ld [hl+],a
    inc de
    ret



; Copies 8 bytes on HBLank
internal_copy_hblank_exp


waitfor_hblank_internal_exp:
    ld a,[$FF41]
    and 3
    cp 0
    jr nz, waitfor_hblank_internal_exp
    ; 1
    ld a,[de]
    ld [hl+],a
    inc e


    ret





default_DMAData:
        db $03e, $0c0, $0e0, $046, $03e, $028, $03d, $020, $0fd,   $0cd,   $00, $00, $0c9

default_DMATimer:
        db $0cd,   $00, $00, $0c9

start:
	di					; disable interrupts
	ld	sp,$E000			; setup stack

.wait_vbl					; wait for vblank to properly disable lcd
	ld	a,[rLY]	
	cp	$90
	jr	nz,.wait_vbl

	xor	a
	ld	[rIF],a				; reset important registers
	ld	[rLCDC],a
	ld	[rSTAT],a
	ld	[rSCX],a
	ld	[rSCY],a
	ld	[rLYC],a
	ld	[rIE],a

	ld	hl,_RAM                         ; clear ram (fill with a which is 0 here)
	ld	bc,$2000-2			; watch out for stack ;)
	call	finit_fill

	ld	hl,_HRAM			; clear hram
	ld	c,$80				; a = 0, b = 0 here, so let's save a byte and 4 cycles (ld c,$80 - 2/8 vs ld bc,$80 - 3/12)
	call	finit_fill

	ld	hl,_VRAM			; clear vram
	ld	b,$18				; a = 0, bc should be $1800; c = 0 here, so..
	call	finit_fill

        ld	a,LCDCF_ON | LCDCF_BG9800 | LCDCF_OBJ8 | LCDCF_OBJON | LCDCF_WINOFF | LCDCF_BGON
                                        ; lcd setup: tiles at $8000, map at $9800, 8x8 sprites (disabled), no window, etc.
        ld	[rLCDC],a			; enable lcd



	jp init_cont
;-------------------------------------------------------------------------------
finit_fill:
;-------------------------------------------------------------------------------
; a - byte to fill with
; hl - destination address
; bc - size of area to fill

	inc	b
	inc	c
	jr	.skip
.fill
	ld	[hl+],a
.skip
	dec	c
	jr	nz,.fill
	dec	b
	jr	nz,.fill
	ret


init_cont:
