
	ORG $401
	.byte    $0E, $08, $0A, $00, $9E, $20, $28
	.byte   $31,$30,$34,$30
	.byte    $29, $00, $00, $00

	ORG $410

Nibbler
	jmp block84862
scrm	dc.w	 
	org scrm+48
clrm	dc.w	 
	org clrm+48
t	dc.b	
i	dc.b	
r	dc.b	
s	dc.b	
int	dc.w	
p1	= $02
p2	= $04
p3	= $08
head	= $16
tail	= $0B
headc	= $0D
tailc	= $10
tailWait	dc.b	$0
tailGrow	dc.b	$1
animTime	dc.b	$0
moveTime	dc.b	$0
moveTimeReset	dc.b	$b
justMoved	dc.b	$0
numFood	dc.b	$0
gameState	dc.b	$0
stopTime	dc.b	$a
wave	dc.b	$0
levelId	dc.b	$0
time	dc.b	$63
timeTick	dc.b	$0
score	dc.w	$0
lives	dc.b	$5
levelarray	dc.b $00, $01, $02, $03, $04, $05, $06, $07
	dc.b $08, $09
foodarray	dc.b $00, $01, $02, $03, $04, $00, $01, $02
	dc.b $03, $04
colourarray	dc.b $07, $03, $04, $05, $02, $07, $05, $04
	dc.b $03, $02
