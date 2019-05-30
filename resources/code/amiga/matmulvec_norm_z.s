

	bra matmulvec_norm_z_end

matmulvec_normal_z_call
	moveq.l #0,d6
	moveq.l #1,d5
	moveq.l #2,d4



		
;	mmx:=mm_vecsIn[index];
;	mm_ox:=mmx*mm_mat[0] + mmy*mm_mat[4] + mmz*mm_mat[8];
;	mm_ox:=mm_ox/128;
;	mm_vecsOut[index]:=mm_ox;


matmulvec_norm_z_loop :

		moveq.l #0,d0
		moveq.l #0,d1
		moveq.l #0,d2
		

		move.b (a1,d6),d0
		ext.w d0
		ext.l d0
		move.l 24(a0),d3
		muls.w d3,d0
		move.b (a1,d5),d1
		ext.w d1
		ext.l d1
		move.l 28(a0),d3
		muls.w d3,d1

		move.b (a1,d4),d2
		ext.w d2
		ext.l d2
		move.l 32(a0),d3
		muls.w d3,d2

		add.l d2,d0
		add.l d1,d0
		

;		divs.w #128,d0
		asr #7,d0		

		move.b d0,(a2,d4)
		

		add.l #3,d4
		add.l #3,d5
		add.l #3,d6
		
		dbf d7,matmulvec_norm_z_loop
		rts
matmulvec_norm_z_end:
