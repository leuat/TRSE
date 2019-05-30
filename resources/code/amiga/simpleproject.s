
;init_projecttoscreen
;	bra projecttoscreen_end


;ro_x1 dc.l 0
;ro_x2 dc.l 0
;ro_y1 dc.l 0
;ro_y2 dc.l 0

;call_projecttoscreen


	move.l (a0,d4),d5
	asl.l #8,d5
;	muls.w #256,d5	
	
	add.l #4,d4
	
	move.l (a0,d4),d6
	asl.l #8,d6
;	muls.w #256,d6	

	add.l #4,d4
	
	move.l (a0,d4),d1
	add.l czz,d1

	divs d1,d5
	add.l cxx,d5
;	move.l d5,ro_x1

	divs d1,d6
	add.l cyy,d6
;	move.l d6,ro_y1

;	rts
;projecttoscreen_end