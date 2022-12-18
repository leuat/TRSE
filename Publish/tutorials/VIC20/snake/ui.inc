
procedure UI_SetScreenColour();
begin
	SCREEN_BG_COLOR := colrs[ waveNum & 3 ]; // border + background colours
end;

procedure UI_DrawScore();
begin
	
	screenmemory := #SCREEN_CHAR_LOC+2;
	BCDPrint( #score, 3 );

end;

procedure UI_DrawBonus();
begin
	
	asm
		
	ldx #CHAR_BONUS0
	stx $1e00+9
	inx
	stx $1e00+10
	inx
	stx $1e00+11
	inx
	stx $1e00+12
		
	end;
end;


procedure UI_DrawAll();	
begin

	asm

	; score
	lda scoreLabel
	sta $1e00	
	lda scoreLabel+1
	sta $1e00+1	

	; stage
	lda stageLabel
	sta $1e00+9	
	
	lda #129 ; A char
	clc
	adc round
	sta $1e00+12

	; hiscore
	lda hiscoreLabel
	sta $1e00+14	
	lda hiscoreLabel+1
	sta $1e00+15
	
	end;

	UI_DrawScore();
	screenmemory := #SCREEN_CHAR_LOC+10;
	BCDPrint( #stage, 1 );
	screenmemory := #SCREEN_CHAR_LOC+16;
	BCDPrint( #hiscore, 3 );
	
end;

procedure UI_DrawNewScreen();
begin
	
	ClearScreen( CHAR_SPACE, #SCREEN_CHAR_LOC); 	// ^$1e00 - unexpanded screen location
	ClearScreen( BLACK, #SCREEN_COL_LOC); 	// ^$9600 - unexpanded colour location
	UI_DrawAll();

end;

procedure UI7();
begin
	_x := 7;
	Print();	
end;
procedure UI11();
begin
	_x := 11;
	Print();	
end;

procedure UI_DrawTitle();
begin
	
	screenmemory := $1e00 + 5 + 3*22;
	p1 := #title1;
	UI11();	

	screenmemory := $1e00 + 7 + 5*22;
	p1 := #title2;
	UI7();	

	screenmemory := $1e00 + 5 + 7*22;
	p1 := #title3;
	UI11();	


	screenmemory := $1e00 + 7 + 10*22;
	p1 := #startF1;
	UI7();	
	screenmemory := $1e00 + 7 + 12*22;
	p1 := #startF3;
	UI7();	
	screenmemory := $1e00 + 7 + 14*22;
	p1 := #startF5;
	UI7();	
	screenmemory := $1e00 + 7 + 16*22;
	p1 := #startF7;
	UI7();	



	screenmemory := $1e00 + 5 + 19*22;
	p1 := #hewco;
	UI11();	

	screenmemory := $1e00 + 6 + 21*22;
	p1 := #tfw8b;
	_x := 9;
	Print();	
	screenmemory := $1e00 + 3 + 22*22;
	p1 := #TRSE;
	_x := 15;
	Print();	


end;