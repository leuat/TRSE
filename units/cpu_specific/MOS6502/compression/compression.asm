 processor 6502
	org $1100
	; Starting new memory block at $1100
StartBlock1100
	; LineNumber: 148
	jmp block1
	; LineNumber: 3
Compression_endd	dc.w	0
	; LineNumber: 3
Compression_count	dc.w	0
	; LineNumber: 4
Compression_src	= $E0
	; LineNumber: 4
Compression_dst	=     $E2
	; NodeProcedureDecl -1
	; ***********  Defining procedure : Compression_Decompress
	;    Procedure type : User-defined procedure
	; LineNumber: 23
	; LineNumber: 22
	; LineNumber: 22
Compression_Decompress_block2
Compression_Decompress
	; LineNumber: 24
	; Assigning single variable : Compression_src
	lda Compression_src
	clc
	adc #$07
	sta Compression_src+0
	; Optimization : A := A op 8 bit - var and bvar are the same - perform inc
	bcc Compression_Decompress_WordAdd3
	inc Compression_src+1
Compression_Decompress_WordAdd3
	; LineNumber: 26
	
; // Look up count and end
	; Assigning single variable : Compression_count
	ldy #0
	; Load pointer array
	lda (Compression_src),y
	pha
	iny
	lda (Compression_src),y
	tay
	pla
	; Calling storevariable
	sta Compression_count
	sty Compression_count+1
	; LineNumber: 27
	; Assigning single variable : Compression_src
	lda Compression_src
	clc
	adc #$04
	sta Compression_src+0
	; Optimization : A := A op 8 bit - var and bvar are the same - perform inc
	bcc Compression_Decompress_WordAdd4
	inc Compression_src+1
Compression_Decompress_WordAdd4
	; LineNumber: 28
	; Assigning single variable : Compression_endd
	ldy #0
	; INTEGER optimization: a=b+c 
	lda Compression_src
	clc
	adc Compression_count
	sta Compression_endd+0
	lda Compression_src+1
	adc Compression_count+1
	sta Compression_endd+1
	; LineNumber: 29
	; ****** Inline assembler section
;unpacker variables, no need to change these
	lda $10
	sta Compression_count
	lda $10
	sta Compression_count+1
parsetoken:
	jsr	getsrc
	pha
	lsr
	lsr
	lsr
	lsr
	beq	copymatches
	jsr	buildcount
	tax
	jsr	docopy
	lda	Compression_src
	cmp	Compression_endd
	lda	Compression_src+1
	sbc	Compression_endd+1
	bcs	done
copymatches
	jsr	getsrc
	sta	delta_d
	jsr	getsrc
	sta	delta_d+1
	pla
	and	#$0f
	jsr	buildcount
	clc
	adc	#4
	tax
	bcc	c_b1
	inc	Compression_count+1
c_b1:
	lda	Compression_src+1
	pha
	lda	Compression_src
	pha
	sec
	lda	Compression_dst
	sbc	delta_d
	sta	Compression_src
	lda	Compression_dst+1
	sbc	delta_d+1
	sta	Compression_src+1
	jsr	docopy
	pla
	sta	Compression_src
	pla
	sta	Compression_src+1
	jmp	parsetoken
done
	pla
	rts
docopy
	jsr	getput
	dex
	bne	docopy
	dec	Compression_count+1
	bne	docopy
	rts
buildcount
	ldx	#1
	stx	Compression_count+1
	cmp	#$0f
	bne	c_q1
c_m1	
	sta	Compression_count
	jsr	getsrc
	tax
	clc
	adc	Compression_count
	bcc	c_b2
	inc	Compression_count+1
c_b2
	inx
	beq	c_m1
c_q1
	rts
getput
	jsr	getsrc
putdst:
	sta (Compression_dst),y
	inc	Compression_dst
	bne	c_p1
	inc	Compression_dst+1
c_p1:
	rts
getsrc:
	lda(Compression_src),y
	inc	Compression_src
	bne	c_p2
	inc	Compression_src+1
c_p2:
	ret 
delta_d:	db	$B4
	
	; LineNumber: 146
	rts
block1
	; End of program
	; Ending memory block
EndBlock1100
