vic20_playsid_init
		lda	#0
		sta	used_freq
		sta	used_freq+1
		sta	used_freq+2
		sta	used_freq+3
		
		ldx	#$0e
.repeter05
		lda	Ld404,x
		beq	.next
		lda	Ld400,x
		ldy	Ld401,x
		jsr	L4200B
.cont		
		pha
		lda		Ld404,x
		and		#$80
		beq		.o
		pla
		lda		#4
		pha
		
.o		pla		
		cmp		#1
		bne		.a
		sty		$900a
		sta		used_freq
		jmp		.z
.a		cmp		#2
		bne		.b
		sty		$900b
		sta		used_freq+1
		jmp		.z
.b		cmp		#3
		bne		.c
		sty		$900c
		sta		used_freq+2
		jmp		.z
.c		cmp		#4
		bne		.d
		sty		$900d
		sta		used_freq+3
.d
.z		
.next	
		txa
		sec
		sbc	#$07
		tax
		bpl .repeter05

;-----------

		ldx	#3
.repeter10
		lda	used_freq,x
		bne	.finsi05
		sta	$900a,x
.finsi05		
		dex
		bpl	.repeter10
		
		rts

;----------------------------------

L4200B	
		sta		L40
		sty		L41
		ldy		#0
.repeter05 
		sec
		lda		c64notes_range_lo,y
		sbc		L40
		lda		c64notes_range_hi,y
		sbc		L41
		bcs		.break				; freq table entry >= note ?
		iny
		cpy		#7
		bne		.repeter05
.break
		dey
		cpy		#$ff
		beq		.exit
		cpy		#6
		bcc		.cont
.exit	lda		#0
		rts
.cont	lda		vic_regs,y
		sta		_reg+1
		lda		c64notes_ptr,y
		tay
.repeter10 
		sec
		lda		c64notes_lo,y
		sbc		L40
		lda		c64notes_hi,y
		sbc		L41
		bcs		.break2				; c64notes >= freq ?
		iny
		cpy		#62
		bne		.repeter10
.break2
		lda		vic20notes,y
		tay
_reg		lda		#$ff
		rts

c64notes_range_lo
								!by		<536             ; 13 C_1
								!by		<1072            ; 25 C_2
								!by		<2145            ; 37 C_3
								!by		<4291            ; 49 C_4
								!by		<8583            ; 61 C_5
								!by		<17167           ; 73 C_6
								!by		<19269
c64notes_range_hi
								!by		>536             ; 13 C_1
								!by		>1072            ; 25 C_2
								!by		>2145            ; 37 C_3
								!by		>4291            ; 49 C_4
								!by		>8583            ; 61 C_5
								!by		>17167           ; 73 C_6
								!by		>19269

c64notes_ptr					!by		0,12,24,36,48,60
vic_regs						!by		1,2,2,3,3,3

c64notes_lo
								!by		<536             ; 13 C_1
								!by		<568             ; 14 CS1
								!by		<602             ; 15 D_1
								!by		<637             ; 16 DS1
								!by		<675             ; 17 E_1
								!by		<716             ; 18 F_1
								!by		<758             ; 19 FS1
								!by		<803             ; 20 G_1
								!by		<851             ; 21 GS1
								!by		<902             ; 22 A_1
								!by		<955             ; 23 AS1
								!by		<1012            ; 24 B_1

								!by		<1072            ; 25 C_2
								!by		<1136            ; 26 CS2
								!by		<1204            ; 27 D_2
								!by		<1275            ; 28 DS2
								!by		<1351            ; 29 E_2
								!by		<1432            ; 30 F_2
								!by		<1517            ; 31 FS2
								!by		<1607            ; 32 G_2
								!by		<1703            ; 33 GS2
								!by		<1804            ; 34 A_2
								!by		<1911            ; 35 AS2
								!by		<2025            ; 36 B_2

								!by		<2145            ; 37 C_3
								!by		<2273            ; 38 CS3
								!by		<2408            ; 39 D_3
								!by		<2551            ; 40 DS3
								!by		<2703            ; 41 E_3
								!by		<2864            ; 42 F_3
								!by		<3034            ; 43 FS3
								!by		<3215            ; 44 G_3
								!by		<3406            ; 45 GS3
								!by		<3608            ; 46 A_3
								!by		<3823            ; 47 AS3
								!by		<4050            ; 48 B_3

								!by		<4291            ; 49 C_4
								!by		<4547            ; 50 CS4
								!by		<4817            ; 51 D_4
								!by		<5103            ; 52 DS4
								!by		<5407            ; 53 E_4
								!by		<5728            ; 54 F_4
								!by		<6069            ; 55 FS4
								!by		<6430            ; 56 G_4
								!by		<6812            ; 57 GS4
								!by		<7217            ; 58 A_4
								!by		<7647            ; 59 AS4
								!by		<8101            ; 60 B_4

								!by		<8583            ; 61 C_5
								!by		<9094            ; 62 CS5
								!by		<9634            ; 63 D_5
								!by		<10207           ; 64 DS5
								!by		<10814           ; 65 E_5
								!by		<11457           ; 66 F_5
								!by		<12139           ; 67 FS5
								!by		<12860           ; 68 G_5
								!by		<13625           ; 69 GS5
								!by		<14435           ; 70 A_5
								!by		<15294           ; 71 AS5
								!by		<16203           ; 72 B_5

								!by		<17167           ; 73 C_6
								!by		<18188           ; 74 CS6
								
c64notes_hi
								!by		>536             ; C_1
								!by		>568             ; CS1
								!by		>602             ; D_1
								!by		>637             ; DS1
								!by		>675             ; E_1
								!by		>716             ; F_1
								!by		>758             ; FS1
								!by		>803             ; G_1
								!by		>851             ; GS1
								!by		>902             ; A_1
								!by		>955             ; AS1
								!by		>1012            ; B_1

								!by		>1072            ; C_2
								!by		>1136            ; CS2
								!by		>1204            ; D_2
								!by		>1275            ; DS2
								!by		>1351            ; E_2
								!by		>1432            ; F_2
								!by		>1517            ; FS2
								!by		>1607            ; G_2
								!by		>1703            ; GS2
								!by		>1804            ; A_2
								!by		>1911            ; AS2
								!by		>2025            ; B_2

								!by		>2145            ; C_3
								!by		>2273            ; CS3
								!by		>2408            ; D_3
								!by		>2551            ; DS3
								!by		>2703            ; E_3
								!by		>2864            ; F_3
								!by		>3034            ; FS3
								!by		>3215            ; G_3
								!by		>3406            ; GS3
								!by		>3608            ; A_3
								!by		>3823            ; AS3
								!by		>4050            ; B_3

								!by		>4291            ; C_4
								!by		>4547            ; CS4
								!by		>4817            ; D_4
								!by		>5103            ; DS4
								!by		>5407            ; E_4
								!by		>5728            ; F_4
								!by		>6069            ; FS4
								!by		>6430            ; G_4
								!by		>6812            ; GS4
								!by		>7217            ; A_4
								!by		>7647            ; AS4
								!by		>8101            ; B_4

								!by		>8583            ; C_5
								!by		>9094            ; CS5
								!by		>9634            ; D_5
								!by		>10207           ; DS5
								!by		>10814           ; E_5
								!by		>11457           ; F_5
								!by		>12139           ; FS5
								!by		>12860           ; G_5
								!by		>13625           ; GS5
								!by		>14435           ; A_5
								!by		>15294           ; AS5
								!by		>16203           ; B_5

								!by		>17167           ; C_6
								!by		>18188           ; CS6
vic20notes
								!by		135			; 00 - C_0 - octave 1 c64
								!by		143			; 01 - CS0
								!by		147			; 02 - D_0
								!by		151			; 03 - DS0
								!by		159			; 04 - E_0
								!by		163			; 05 - F_0
								!by		167			; 06 - FS0
								!by		175			; 07 - G_0
								!by		179			; 08 - GS0
								!by		183			; 09 - A_0
								!by		187			; 10 - AS0
								!by		191			; 11 - B_0
								!by		135			; 12 - C_0 - octave 2 c64
								!by		143			; 13 - CS0
								!by		147			; 14 - D_0
								!by		151			; 15 - DS0
								!by		159			; 16 - E_0
								!by		163			; 17 - F_0
								!by		167			; 18 - FS0
								!by		175			; 19 - G_0
								!by		179			; 20 - GS0
								!by		183			; 21 - A_0
								!by		187			; 22 - AS0
								!by		191			; 23 - B_0
								!by		195			; 24 - C_1 - octave 3 c64
								!by		199			; 25 - CS1
								!by		201			; 26 - D_1
								!by		203			; 27 - DS1
								!by		207			; 28 - E_1
								!by		209			; 29 - F_1
								!by		212			; 30 - FS1
								!by		215			; 31 - G_1
								!by		217			; 32 - GS1
								!by		219			; 33 - A_1
								!by		221			; 34 - AS1
								!by		223			; 35 - B_1
								!by		195			; 36 - C_1 - octave 4 c64
								!by		199			; 37 - CS1
								!by		201			; 38 - D_1
								!by		203			; 39 - DS1
								!by		207			; 40 - E_1
								!by		209			; 41 - F_1
								!by		212			; 42 - FS1
								!by		215			; 43 - G_1
								!by		217			; 44 - GS1
								!by		219			; 45 - A_1
								!by		221			; 46 - AS1
								!by		223			; 47 - B_1
								!by		225			; 48 - C_2 - octave 5 c64
								!by		227			; 49 - CS2
								!by		228			; 50 - D_2
								!by		229			; 51 - DS2
								!by		231			; 52 - E_2
								!by		232			; 53 - F_2
								!by		233			; 54 - FS2
								!by		235			; 55 - G_2
								!by		236			; 56 - GS2
								!by		237			; 57 - A_2
								!by		238			; 58 - AS2
								!by		239			; 59 - B_2
								!by		240			; 60 - C_3 - octave 6 c64
								!by		241         ; 61 - CS3

ZeroPageSpace	=	$40

L40		=	ZeroPageSpace
L41		=	ZeroPageSpace+1

Ld400	=	SIDREG
Ld401	=	SIDREG+1
Ld404	=	SIDREG+4
