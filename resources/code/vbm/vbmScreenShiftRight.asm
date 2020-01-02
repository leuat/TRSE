vbmScreenShiftRight
	;ldx vbmX	; line to start at
vbmRSP_loop
	lda $1100+$E40,x ; to get bit from right
	lsr
	ror $1100,x
	ror $1100+$C0,x
	ror $1100+$180,x
	ror $1100+$240,x
	ror $1100+$300,x
	ror $1100+$3C0,x
	ror $1100+$480,x
	ror $1100+$540,x
	ror $1100+$600,x
	ror $1100+$6C0,x
	ror $1100+$780,x
	ror $1100+$840,x
	ror $1100+$900,x
	ror $1100+$9C0,x
	ror $1100+$A80,x
	ror $1100+$B40,x
	ror $1100+$C00,x
	ror $1100+$CC0,x
	ror $1100+$D80,x
	ror $1100+$E40,x
	
	inx
	cpx vbmY	; line to end at
	bne vbmRSP_loop