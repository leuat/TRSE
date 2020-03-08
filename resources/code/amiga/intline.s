; d0=X1  d1=Y1  d2=X2  d3=Y2  d5=Screenwidth  a0=address a6=$dff000
; $dff060=Screenwidth (wrd) $dff072=-$8000 (lng)  $dff044=-1 (lng)
; d4, d5 = Trashed

initLine:

                move.l #-$1,$dff044
                move.l #-$8000,$dff072
                move.w #40,$dff060

        bra continueLine

drawLine:




;	move.w	#$a4a,LineMode
;	lea		Boff(pc), a4
;	lea		$dff000,a6
.waitblut:
;.blitwait:
;	btst	#14,DMACONR2(a6)
;	bne.s	.blitwait
;	move.l	#$ffff8000,BLTBDAT(a6)
;	move.l	#-1,BLTAFWM(a6)
;	move.l	#40,d5
;	move.w	d5,BLTCMOD(a6)






        cmp.w	d1,d3
        bne.b	.ok
        add.w	#1,d1
        rts
.ok:
        cmp.w	d1,d3
        bge.s	line1
        exg		d0,d2
        exg		d1,d3
line1:
        moveq	#0,d4
        move.w	d1,d4
        add.w	d4,d4
;	lea		PlaneWidthTab(pc),a5
;	move.w	(a5,d4.w),d4

        move.w	d1,d4
        muls	d5,d4

        move.w	d0,d5
        add.l	a0,d4
        asr.w	#3,d5
        add.w	d5,d4
        moveq	#0,d5
        sub.w	d1,d3
        sub.w	d0,d2
        bpl.s	line2
        moveq	#1,d5
        neg.w	d2
line2:
        move.w	d3,d1
        add.w	d1,d1
        cmp.w	d2,d1
        dbhi		d3,line3
line3:
        move.w	d3,d1
        sub.w	d2,d1
        bpl.s	line4
        exg		d2,d3
line4:
        addx.w	d5,d5
        add.w	d2,d2
        move.w	d2,d1
        sub.w	d3,d2
        addx.w	d5,d5
        and.w	#15,d0
        ror.w	#4,d0
        or.w	LineMode(pc),d0


waitblt:
        btst		#6,DMACONR2(a6)

        bne.s	waitblt
        move.w	d2,BLTAPTL(a6)
        sub.w	d3,d2
        lsl.w	#6,d3
        addq.w	#2,d3
        move.w	d0,BLTCON0(a6)
        moveq	#0,d0
        move.b	oct(pc,d5.w),d0
        bclr	#1,d0
        move.w	d0,BLTCON1(a6)
        move.l	d4,BLTCPTH(a6)
        move.l	d4,BLTDPTH(a6)
        movem.w	d1/d2,BLTBMOD(a6)
        move.w	d3,BLTSIZE(a6)
out:
        rts

oct:
        dc.l	$3431353,$b4b1757



continueLine:
