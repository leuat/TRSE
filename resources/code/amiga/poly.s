*******************************************
*** Draw flat filled Polygons		***
*******************************************

; $VER: POLYFILLER FLAT v3.o, Wed, 24-Mar-2oo4
;	(c)oded by StingRay/[S]carab^Scoopex
;
; derived from my texture-mapper, no division table used
; maximum 3 divisions per polygon, writes 4 pixels at once
;

; d0-d2: coords
;    d3: color
;    a4: surface
;    a6: ptr to object structure


CLIPX_MAX  = 40
CLIPX_MIN  = 0
CLIPY_MAX  = 200
CLIPY_MIN  = 0


DRAW_FLAT
	moveq	#0,d5
	move.b	d3,d5		; ......cc
	cmp.w	#P_T50,SURF_FLAGS(a4)
	beq.b	.nolc
	lsl.w	#8,d5		; ....cc00
	move.b	d3,d5		; ....cccc
	move.w	d5,d3
	swap	d5		; cccc....
	move.w	d3,d5		; cccccccc

.nolc	cmp.w	d0,d2
	bge.b	.ok1
	exg.l	d0,d2
.ok1	cmp.w	d1,d2
	bge.b	.ok2
	exg.l	d1,d2
.ok2	cmp.w	d0,d1
	bge.b	.ok3
	exg.l	d0,d1
.ok3	
	
	move.b	OBJ_CLIP(a6),d7
	lea	.VARS(pc),a6
	movem.l	d0-d2,.X1(a6)	; x/y 1-3
	move.b	d7,.CLIP(a6)	; clipping status
	move.w	SURF_FLAGS(a4),.FLAGS(a6)


	move.w	d2,d7
	sub.w	d0,d7		; y3-y1
	beq.w	.exit
	move.l	d2,d6
	sub.l	d0,d6		; x3-x1
	asr.l	#8,d6
	ext.l	d7
	; LEUAT changed from divs.l to divs.w
	divs.w	d7,d6
	
	move.l	d6,.DXDY2(a6)


	move.w	d1,d7
	sub.w	d0,d7		; y2-y1
	bne.b	.noflat

; xend for 2nd section = x1
	move.w	.X1(a6),d1
	ext.l	d1
	lsl.l	#8,d1
	bra.w	.part2


.noflat	move.w	.X2(a6),d1
	sub.w	.X1(a6),d1
	ext.l	d7
	ext.l	d1
	lsl.l	#8,d1
	; LEUAT changed from divs.l to divs.w

	divs.w	d7,d1
	move.l	d1,.DXDY1(a6)


	*** Draw top part of triangle ***

	move.w	.X1(a6),d0
	move.w	d0,d1

	move.l	.DXDY2(a6),a2
	move.l	.DXDY1(a6),a3

	tst.b	.CLIP(a6)
	beq.b	.nclip
	move.w	.Y1(a6),a5
	move.w	.Y2(a6),d7
	bsr.b	.draw
	bra.b	.part2
	
.nclip	move.w	.Y2(a6),d7
	sub.w	.Y1(a6),d7
	move.l	ENG_CHUNKYBUFFER(pc),a0
	move.w	.Y1(a6),d6
	mulu.w	#CHUNKYX,d6
	add.l	d6,a0
	bsr.b	.draw

	*** Draw bottom part of triangle ***

.part2	move.w	.Y3(a6),d7
	sub.w	.Y2(a6),d7
	beq.w	.exit
	move.w	.X3(a6),d0
	sub.w	.X2(a6),d0
	ext.l	d7
	ext.l	d0
	lsl.l	#8,d0
		; LEUAT changed from divs.l to divs.w

	divs.w	d7,d0
	move.l	d0,.DXDY1(a6)

	move.w	.X2(a6),d0
	move.l	.DXDY1(a6),a2
	move.l	.DXDY2(a6),a3

	tst.b	.CLIP(a6)
	beq.b	.nclip2
	move.w	.Y2(a6),a5
	move.w	.Y3(a6),d7
	bra.b	.draw2

.nclip2	move.l	ENG_CHUNKYBUFFER(pc),a0
	move.w	.Y2(a6),d6
	mulu.w	#CHUNKYX,d6
	add.l	d6,a0
	bra.b	.draw2

*******************************************
*** NON-CLIPPED DRAW	NON-TRANSPARENT	***
*******************************************

; d0: x1
; d1: x2
; d5: color
; d7: height
; a0: chunkybuffer + yoffset
; a2: DXDY2 (DXDY1 for the 2nd section)
; a3: DXDY1 (DXDY2 for the 2nd section)

.draw	ext.l	d1
	lsl.l	#8,d1
.draw2	ext.l	d0
	lsl.l	#8,d0

	subq.w	#1,d7				; adapt "dbf"
	cmp.l	a2,a3
	bge.b	.swap

	move.l	d0,a2				; x left
	move.l	d1,a1				; x right
	move.l	.DXDY1(a6),d6			; delta xleft
	move.l	.DXDY2(a6),a4			; delta xright
	bsr.b	.go
	move.l	a1,d1
.exit	rts

.swap	move.l	d1,a2				; x left
	move.l	d0,a1				; x right
	move.l	.DXDY2(a6),d6			; delta xleft
	move.l	.DXDY1(a6),a4			; delta xright
	bsr.b	.go
	move.l	a2,d1
	rts
	
	CNOP	0,4
.go	tst.b	.CLIP(a6)
	bne.w	.cl_go
	cmp.w	#P_T50,.FLAGS(a6)
	beq.b	.trans
.loopY	move.l	a2,d0		; x start
	move.l	a1,d1		; x end
	lsr.l	#8,d0
	lsr.l	#8,d1
	sub.w	d0,d1		; delta X = width of scanline
	ble.w	.noX
	lea	(a0,d0.w),a5

	lsr.w	#1,d1
	bcc.b	.nobyte
	move.b	d5,(a5)+

.nobyte	lsr.w	#1,d1
	bcc.b	.noword
	move.w	d5,(a5)+

.noword	subq.w	#1,d1
	bmi.b	.nolong
.loopX	move.l	d5,(a5)+
	dbf	d1,.loopX

.nolong

.noX	add.l	a4,a1
	add.l	d6,a2
	lea	CHUNKYX(a0),a0
	dbf	d7,.loopY
	rts


*******************************************
*** NON-CLIPPED DRAW	50% TRANSPARENT	***
*******************************************

.trans
.tloopY	move.l	a2,d0		; x start
	move.l	a1,d1		; x end
	lsr.l	#8,d0
	lsr.l	#8,d1
	sub.w	d0,d1		; delta X = width of scanline
	ble.b	.tnoX
	lea	(a0,d0.w),a5
.tloopX	moveq	#0,d4
	move.b	(a5),d4
	add.w	d5,d4
	lsr.w	#1,d4
	move.b	d4,(a5)+
	subq.w	#1,d1
	bne.b	.tloopX
.tnox	add.l	a4,a1
	add.l	d6,a2
	lea	CHUNKYX(a0),a0
	dbf	d7,.tloopY
	rts


*******************************************
*** CLIPPED DRAW	NON-TRANSPARENT	***
*******************************************

; d0: x1
; d1: x2
; d5: color
; d7: y1
; a5: y2
; a2: DXDY2 (DXDY1 for the 2nd section)
; a3: DXDY1 (DXDY2 for the 2nd section)

	CNOP	0,4
.cl_go
.cl_loopY
	cmp.w	#P_T50,.FLAGS(a6)
	beq.b	.cltrans
	cmp.w	a5,d7
	blt.b	.cl_exit

	move.w	a5,d0
	cmp.w	#CLIPY_MAX,d0
	bgt.b	.cl_exit
	cmp.w	#CLIPY_MIN,d0
	blt.b	.cl_noX
	mulu.w	#CHUNKYX,d0
	move.l	ENG_CHUNKYBUFFER(pc),a0
	add.l	d0,a0
	

	move.l	a2,d0		; x start
	move.l	a1,d1		; x end
	lsr.l	#8,d0
	lsr.l	#8,d1


; LEUAT removed following section
;	movem.w	([ENG_CLIPX_TABPTR,pc],a5.w*4),d2/d3
;.cl_cl	cmp.w	d3,d0
;	bgt.b	.cl_noX
;	cmp.w	d2,d1
;	blt.b	.cl_noX
;	cmp.w	d3,d1
;	ble.b	.cl_xmaxok
;	move.w	d3,d1
;.cl_xmaxok
;	cmp.w	d2,d0
;	bge.b	.cl_xminok
;	move.w	d2,d0
;.cl_xminok



;	cmp.w	#CLIPX_MAX,d0
;	bgt.b	.cl_noX
;	cmp.w	#CLIPX_MIN,d1
;	blt.b	.cl_noX
;	cmp.w	#CLIPX_MAX,d1
;	ble.b	.cl_xmaxok
;	move.w	#CLIPX_MAX,d1
;.cl_xmaxok
;	cmp.w	#CLIPX_MIN,d0
;	bge.b	.cl_xminok
;	moveq	#CLIPX_MIN,d0	; x1 = CLIPX_MIN
;.cl_xminok

	sub.w	d0,d1		; delta X = width of scanline
	ble.w	.cl_nox
	add.w	d0,a0
	lsr.w	#1,d1
	bcc.b	.cl_nob
	move.b	d5,(a0)+

.cl_nob	lsr.w	#1,d1
	bcc.b	.cl_now
	move.w	d5,(a0)+

.cl_now	subq.w	#1,d1
	bmi.b	.cl_nol
.cl_loopX
	move.l	d5,(a0)+
	dbf	d1,.cl_loopX
.cl_nol

.cl_noX	add.l	a4,a1
	add.l	d6,a2

	addq.w	#1,a5
	bra.b	.cl_loopY
.cl_exit
	rts

*******************************************
*** CLIPPED DRAW	50% TRANSPARENT	***
*******************************************

.cltrans
.clt_loopY
	cmp.w	a5,d7
	blt.b	.cl_exit

	move.w	a5,d0
	cmp.w	#CLIPY_MAX,d0
	bgt.b	.cl_exit
	cmp.w	#CLIPY_MIN,d0
	blt.b	.clt_noX
	mulu.w	#CHUNKYX,d0
	move.l	ENG_CHUNKYBUFFER(pc),a0
	add.l	d0,a0
	

	move.l	a2,d0		; x start
	move.l	a1,d1		; x end
	lsr.l	#8,d0
	lsr.l	#8,d1


	cmp.w	#CLIPX_MAX,d0
	bgt.b	.clt_noX
	cmp.w	#CLIPX_MIN,d1
	blt.b	.clt_noX
	cmp.w	#CLIPX_MAX,d1
	ble.b	.clt_xmaxok
	move.w	#CLIPX_MAX,d1
.clt_xmaxok
	cmp.w	#CLIPX_MIN,d0
	bge.b	.clt_xminok
	moveq	#CLIPX_MIN,d0	; x1 = CLIPX_MIN
.clt_xminok

	sub.w	d0,d1		; delta X = width of scanline
	ble.w	.clt_nox
	add.w	d0,a0
	
.clt_loopX
	moveq	#0,d4
	move.b	(a0),d4
	add.w	d5,d4
	lsr.w	#1,d4
	move.b	d4,(a0)+
	subq.w	#1,d1
	bne.b	.clt_loopX

.clt_noX
	add.l	a4,a1
	add.l	d6,a2

	addq.w	#1,a5
	bra.b	.clt_loopY


.VARS	RSRESET
.X1	rs.w	1
.Y1	rs.w	1
.X2	rs.w	1
.Y2	rs.w	1
.X3	rs.w	1
.Y3	rs.w	1
.DXDY1	rs.l	1
.DXDY2	rs.l	1
.CLIP	rs.w	1
.FLAGS	rs.w	1
.SIZE	rs.b	0
	dcb.b	.SIZE
