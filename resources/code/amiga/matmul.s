	bra matmul_end	

matmul_call:


		move.l #128,d4
	;double c11 = a.M11 * b.M11 + a.M12 * b.M21 + a.M13 * b.M31;
		
		move.l 0(a1),d1
		move.l 0(a2),d5
		muls.w d5,d1
		
		move.l 1*4+0*12(a1),d2
		move.l 0*4+1*12(a2),d5
		muls.w d5,d2
		
		move.l 2*4+0*12(a1),d3
		move.l 0*4+2*12(a2),d5
		muls.w d5,d3
		
		add.l d2,d1
		add.l d3,d1
		
		divs.w d4,d1
		move.l d1,0(a0)

;        double c12 = a.M11 * b.M12 + a.M12 * b.M22 + a.M13 * b.M32;


		move.l 0*4+0*12(a1),d1
		move.l 1*4+0*12(a2),d5
		muls.w d5,d1
		
		move.l 1*4+0*12(a1),d2
		move.l 1*4+1*12(a2),d5
		muls.w d5,d2
		
		move.l 2*4+0*12(a1),d3
		move.l 1*4+2*12(a2),d5
		muls.w d5,d3
		
		add.l d2,d1
		add.l d3,d1
		divs.w d4,d1
		move.l d1,1*4+0*12(a0)

;        double c13 = a.M11 * b.M13 + a.M12 * b.M23 + a.M13 * b.M33;

		
		move.l 0*4+0*12(a1),d1
		move.l 2*4+0*12(a2),d5
		muls.w d5,d1
		
		move.l 1*4+0*12(a1),d2
		move.l 2*4+1*12(a2),d5
		muls.w d5,d2
		
		move.l 2*4+0*12(a1),d3
		move.l 2*4+2*12(a2),d5
		muls.w d5,d3
		
		add.l d2,d1
		add.l d3,d1
		divs.w d4,d1
		move.l d1,2*4+0*12(a0)
		

;        double c21 = a.M21 * b.M11 + a.M22 * b.M21 + a.M23 * b.M31;

		move.l 0*4+1*12(a1),d1
		move.l 0*4+0*12(a2),d5
		muls.w d5,d1
		
		move.l 1*4+1*12(a1),d2
		move.l 0*4+1*12(a2),d5
		muls.w d5,d2
		
		move.l 2*4+1*12(a1),d3
		move.l 0*4+2*12(a2),d5
		muls.w d5,d3
		
		add.l d2,d1
		add.l d3,d1
		divs.w d4,d1
		move.l d1,0*4+1*12(a0)

;        double c22 = a.M21 * b.M12 + a.M22 * b.M22 + a.M23 * b.M32;

		move.l 0*4+1*12(a1),d1
		move.l 1*4+0*12(a2),d5
		muls.w d5,d1
		
		move.l 1*4+1*12(a1),d2
		move.l 1*4+1*12(a2),d5
		muls.w d5,d2
		
		move.l 2*4+1*12(a1),d3
		move.l 1*4+2*12(a2),d5
		muls.w d5,d3
		
		add.l d2,d1
		add.l d3,d1
		divs.w d4,d1
		move.l d1,1*4+1*12(a0)
		
;        double c23 = a.M21 * b.M13 + a.M22 * b.M23 + a.M23 * b.M33;
 

		move.l 0*4+1*12(a1),d1
		move.l 2*4+0*12(a2),d5
		muls.w d5,d1
		
		move.l 1*4+1*12(a1),d2
		move.l 2*4+1*12(a2),d5
		muls.w d5,d2
		
		move.l 2*4+1*12(a1),d3
		move.l 2*4+2*12(a2),d5
		muls.w d5,d3
		
		add.l d2,d1
		add.l d3,d1
		divs.w d4,d1
		move.l d1,2*4+1*12(a0)

;         double c31 = a.M31 * b.M11 + a.M32 * b.M21 + a.M33 * b.M31;

		move.l 0*4+2*12(a1),d1
		move.l 0*4+0*12(a2),d5
		muls.w d5,d1
		move.l 1*4+2*12(a1),d2
		move.l 0*4+1*12(a2),d5
		muls.w d5,d2
		move.l 2*4+2*12(a1),d3
		move.l 0*4+2*12(a2),d5
		muls.w d5,d3
		add.l d2,d1
		add.l d3,d1
		divs.w d4,d1
		move.l d1,0*4+2*12(a0)

;  double c32 = a.M31 * b.M12 + a.M32 * b.M22 + a.M33 * b.M32;

		move.l 0*4+2*12(a1),d1
		move.l 1*4+0*12(a2),d5
		muls.w d5,d1
		move.l 1*4+2*12(a1),d2
		move.l 1*4+1*12(a2),d5
		muls.w d5,d2
		move.l 2*4+2*12(a1),d3
		move.l 1*4+2*12(a2),d5
		muls.w d5,d3
		add.l d2,d1
		add.l d3,d1
		divs.w d4,d1
		move.l d1,1*4+2*12(a0)

;         double c33 = a.M31 * b.M13 + a.M32 * b.M23 + a.M33 * b.M33;

		move.l 1*4+2*12(a1),d1
		move.l 2*4+1*12(a2),d5
		muls.w d5,d1
		move.l 1*4+2*12(a1),d2
		move.l 2*4+1*12(a2),d5
		muls.w d5,d2
		move.l 2*4+2*12(a1),d3
		move.l 2*4+2*12(a2),d5
		muls.w d5,d3
		add.l d2,d1
		add.l d3,d1
		divs.w d4,d1
		move.l d1,2*4+2*12(a0)


		rts

matmul_end
