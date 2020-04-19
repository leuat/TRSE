
SECTION "VBL",ROM0[$0040]		; vblank interrupt handler
        jp	vblank

    SECTION	"Start",ROM0[$100]		; start vector, followed by header data applied by rgbfix.exe
	nop
	jp	start

        SECTION "Code",ROM0[$150]		; code starts here


vblank:
        call	$FF80				; copy OAM mirror table using DMA
        reti



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
