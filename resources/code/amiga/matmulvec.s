;a0 = mm_mat
;a1 = mm_vecsIn
;a2 = mm_vecsOut
;d6 = index


;	mmx:=mm_vecsIn[index];
;	mmy:=mm_vecsIn[index+4];
;	mmz:=mm_vecsIn[index+8];

;	mm_ox:=mmx*mm_mat[0] + mmy*mm_mat[4] + mmz*mm_mat[8];
;	mm_oy:=mmx*mm_mat[12] + mmy*mm_mat[16] + mmz*mm_mat[20];
;	mm_oz:=mmx*mm_mat[24] + mmy*mm_mat[28] + mmz*mm_mat[32];

;	mm_ox:=mm_ox/128;
;	mm_oy:=mm_oy/128;
;	mm_oz:=mm_oz/128;

;	mm_vecsOut[index]:=mm_ox;
;	mm_vecsOut[index+4]:=mm_oy;
;	mm_vecsOut[index+8]:=mm_oz;

	bra call_matmulvec_cont
call_matmulvec:


matmulvec_loop

		;a0 = mm_mat
		
;	mmx:=mm_vecsIn[index];
;	mm_ox:=mmx*mm_mat[0] + mmy*mm_mat[4] + mmz*mm_mat[8];
;	mm_ox:=mm_ox/128;
;	mm_vecsOut[index]:=mm_ox;


		move.l (a1,d6),d0
		move.l 0(a0),d3
		muls.w d3,d0
		move.l (a1,d5),d1
		move.l 4(a0),d3
		muls.w d3,d1

		move.l (a1,d4),d2
		move.l 8(a0),d3
		muls.w d3,d2

		add.l d2,d0
		add.l d1,d0
		divs.w #128,d0
		move.l d0,(a2,d6)



		move.l (a1,d6),d0
		move.l 12(a0),d3
		muls.w d3,d0
		move.l (a1,d5),d1
		move.l 16(a0),d3
		muls.w d3,d1

		move.l (a1,d4),d2
		move.l 20(a0),d3
		muls.w d3,d2

		add.l d2,d0
		add.l d1,d0
		divs.w #128,d0
		move.l d0,(a2,d5)




		move.l (a1,d6),d0
		move.l 24(a0),d3
		muls.w d3,d0
		move.l (a1,d5),d1
		move.l 28(a0),d3
		muls.w d3,d1

		move.l (a1,d4),d2
		move.l 32(a0),d3
		muls.w d3,d2

		add.l d2,d0
		add.l d1,d0
		divs.w #128,d0
		move.l d0,(a2,d4)


		add.l #12,d6
		add.l #12,d5
		add.l #12,d4

		dbf d7,matmulvec_loop


		rts
call_matmulvec_cont:
