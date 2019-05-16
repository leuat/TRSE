; exit gracefully - reverse everything done in init
	move.w #$7fff,DMACON
	move.w	olddmareq,DMACON
	move.w #$7fff,INTENA
	move.w	oldintena,INTENA
	move.w #$7fff,INTREQ
	move.w	oldintreq,INTREQ
	move.w #$7fff,ADKCON
	move.w	oldadkcon,ADKCON

	move.l	oldcopper,COP1LCH
	move.l 	gfxbase,a6
	move.l 	oldview,a1
	jsr -222(a6)	; LoadView
	jsr -270(a6)	; WaitTOF
	jsr -270(a6)	; WaitTOF
	move.l	$4,a6
	jsr -138(a6)	; Permit

	; end program
	movem.l (sp)+,d0-d7/a0-a6
	rts

; storage for 32-bit addresses and data
	CNOP 0,4
oldview:	dc.l 0
oldcopper:	dc.l 0
gfxbase:	dc.l 0
frame:          dc.l 0

; storage for 16-bit data
	CNOP 0,4
olddmareq:	dc.w 0
oldintreq:	dc.w 0
oldintena:	dc.w 0
oldadkcon:	dc.w 0

	CNOP 0,4
gfxname: dc.b 'graphics.library',0



  section datachip,data_c

Chip:

    even
cop:
;    dc.w    $0100,$0200
    dc.w    $008e
diwstrt:
    dc.w    $1070,$0090
diwstop:
    dc.w    $2cc1
    dc.w    $0092
ddfstrt:
; 
    dc.w    $0038,$0094
ddfstop:
    dc.w    $00d0
    dc.w    $0108
modeven:
;set bplmodulo here
    dc.w    0,$010a
mododd:
;set bplmodulo here
    dc.w    0




CopSprites:
    
    dc.w $120,0, $122,0
    dc.w $124,0, $126,0
    dc.w $128,0, $12A,0
    dc.w $12C,0, $12E,0
    dc.w $130,0, $132,0
    dc.w $134,0, $136,0
    dc.w $138,0, $13A,0
    dc.w $13C,0, $13E,0

Pal:
    dc.w    $0180, $000
    dc.w    $0182, $fff
    dc.w    $0184, $236
    dc.w    $0186, $ba7
    dc.w    $0188, $836
    dc.w    $018a, $485
    dc.w    $018c, $723
    dc.w    $018e, $6cb
    dc.w    $0190, $246
    dc.w    $0192, $034
    dc.w    $0194, $569
    dc.w    $0196, $444
    dc.w    $0198, $666
    dc.w    $019a, $8d9
    dc.w    $019c, $b56
    dc.w    $019e, $999





    dc.w    $e0
b0h:
    dc.w    0,$e2
b0l:
    dc.w    0,$e4
b1h:
    dc.w    0,$e6
b1l:
    dc.w    0,$e8
b2h:
    dc.w    0,$ea
b2l:
    dc.w    0,$ec
b3h:
    dc.w    0,$ee
b3l:
    dc.w    0,$f0
b4h:
    dc.w    0,$f2
b4l:
    dc.w    0




copbplcon1:
    dc.l    $01020000
    dc.l    $2c01fffe
copbplcon0
; Set bitplane  B
   dc.l    $01004200, $01800111
    dc.l    $2d01fffe, $01800000
    dc.l    $8001fffe, $01002200
    dc.l    $9001fffe, $01001200
    dc.l    $ffdffffe
;    dc.l    $fc01fffe, $0180055a
    dc.l    $0d01fffe, $01000200
    dc.l    $fffffffe


spritepointer:
    blk.b    256






