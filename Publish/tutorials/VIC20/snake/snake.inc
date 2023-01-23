procedure Snake_Init();
begin
	
	sh := #SCREEN_CHAR_LOC + 44 + 9;
	st := #SCREEN_CHAR_LOC + 44 + 12;
	
	asm
		
		lda #CHAR_SNAKEHEADLEFT ; head
		sta SCREEN_CHAR_LOC + 44 + 9
		lda #CHAR_SNAKEBODYHORIZ ; body
		sta SCREEN_CHAR_LOC + 44 + 10
		sta SCREEN_CHAR_LOC + 44 + 11
		lda #CHAR_SNAKETAILLEFT ; tail
		sta SCREEN_CHAR_LOC + 44 + 12
		
		lda #BLACK
		sta SCREEN_COL_LOC + 44 + 9
		sta SCREEN_COL_LOC + 44 + 10
		sta SCREEN_COL_LOC + 44 + 11
		sta SCREEN_COL_LOC + 44 + 12
		
	end;
	
	tailChar := CHAR_SPACE;
	
end;

procedure Snake_Score();
begin
	
	bcdadd( #score, #scoreadd, 3 );
	UI_DrawScore();

end;


// sh
procedure Snake_Eat();
begin
	
	foodAnim := 0; // clear the anim flag for eat body segment frame (and also stop the tail from growing for a segment)
	
	j := sh[ 0 ];
	
	@ifndef CHEAT
	if ( j <= CHAR_MUSHROOM0 ) then // hit mushroom or self
	begin
		gameMode := GAME_OVER;
		gameOverSound := GAMEOVERSOUNDSTART;
		return;
	end;
	@endif
	
	if ( j = CHAR_SPACE) then return; // no food here
	
	// is there jelly where the head is moving into?
	if ( j = CHAR_JELLY) then
	begin
		bcdadd( #score, #jellyadd, 3 );
		UI_DrawAll();
		foodDuration := SOUND_JELLYDURATION;
		return;
	end;
	
	// there is food where the head is moving into
	foodDuration := SOUND_FOODDURATION;
	
	if ( j <> CHAR_BUG) then food += 1; // keep counter of how many fruit eaten - resets every 3
	foodAnim := 1; // set anim flag so body will show eaten state next time
	numFood -= 1; // how many food left to eat
	
	// flags
	if ( j = CHAR_CHERRY) then flag := flag | FLAG_CHERRY;
	if ( j = CHAR_APPLE) then flag := flag | FLAG_APPLE;
	if ( j = CHAR_GRAPES) then flag := flag | FLAG_GRAPES;

	// on collecting three food, or a bug
	if (food = 3 or j = CHAR_BUG) then
	begin
		if ( flag & FLAG_ALLFOOD = FLAG_ALLFOOD) then
			tailChar := CHAR_JELLY // YAY - poop a jelly!
		else
			tailChar := CHAR_MUSHROOM0; // boo - poop a mushroom
			
		flag := flag & FLAG_CLEARFOOD;
		food := 0;
		
	end
	else
	begin
		tailChar := CHAR_SPACE;
	end;
	
	Snake_Score();
	
end;

procedure Snake_TurnLeft();
begin
	n := bodyTurnLeft[ i ] + foodAnim;
end;
procedure Snake_TurnRight();
begin
	n := bodyTurnRight[ i ] + foodAnim;
end;

procedure Snake_MaskOutOppDir();
begin

	m := sh[ 0 ] & $FE;
	i := (m - CHAR_SNAKEHEADRIGHT) >> 1; // values 0 - 3 rdlu

	// this part is effective when called from game loop, but not from move head 
	if ( i = LRIGHT or i = LLEFT ) then	joy1pressed := joy1pressed & %11100110;
	if ( i = LUP or i = LDOWN ) then		joy1pressed := joy1pressed & %11101001;

end;

// main routine - draws head in new position, taking user input for new direction if needed
procedure Snake_MoveHead();
begin
	
	// get current snake direction
	Snake_MaskOutOppDir(); // stop moving back on yourself
	p1 := sh; // set screenmem pointer to where head is now
	k := i; // k will  be the new direction...
	
	// order is rdlu with r = 0 and u = 3
	if (joy & JOY_RIGHT) then begin k := LRIGHT; m := CHAR_SNAKEHEADRIGHT; end;
	if (joy & JOY_DOWN) then begin k := LDOWN; m := CHAR_SNAKEHEADDOWN; end;
	if (joy & JOY_LEFT) then begin k := LLEFT; m := CHAR_SNAKEHEADLEFT; end;
	if (joy & JOY_UP) then begin k := LUP; m := CHAR_SNAKEHEADUP; end;
	// k = now has new dir 0 - 3
	// m = new head dir
	

	if ( i = LRIGHT ) then
	begin
		// Right
		n := CHAR_SNAKEBODYHORIZ + foodAnim;
		// create turn in body?
		if (k = LUP) then Snake_TurnLeft();
		if (k = LDOWN) then Snake_TurnRight();
		
	end;
	if ( i = LDOWN ) then
	begin
		// Down
		n := CHAR_SNAKEBODYVERT + foodAnim;
		// create turn in body?
		if (k = LRIGHT) then Snake_TurnLeft();
		if (k = LLEFT) then Snake_TurnRight();
		
	end;
	if ( i = LLEFT ) then
	begin
		// Left
		n := CHAR_SNAKEBODYHORIZ + foodAnim;
		// create turn in body?
		if (k = LDOWN) then Snake_TurnLeft();
		if (k = LUP) then Snake_TurnRight();
		
	end;
	if ( i = LUP ) then
	begin
		// Up
		n := CHAR_SNAKEBODYVERT + foodAnim;
		// create turn in body?
		if (k = LLEFT) then Snake_TurnLeft();
		if (k = LRIGHT) then Snake_TurnRight();
		
	end;


	// move head
	if (k = LRIGHT ) then sh := sh + 1;
	if (k = LUP ) then sh := sh - 22;
	if (k = LLEFT ) then sh := sh - 1;
	if (k = LDOWN ) then sh := sh + 22;
	
	if ( sh > #SCREENBOTTOM ) then sh := sh - SCREENSIZE;
	if ( sh < #SCREENTOP ) then sh := sh + SCREENSIZE;
	
	Snake_Eat();
	if (gameMode = GAME_OVER) then return; // don't move into mushroom/you
	
	// anim
	anim := (anim + 1) & 1;
	m := m + anim;
	sh[ 0 ] := m;
	p1 [ 0 ] := n;

	asm
	
; colour the head postion black (as may have eaten food)
	lda sh+1	; address of screen (high byte)
	and #$03
	clc
	adc #$94		; bring up to colour space
	sta colormemory+1
	lda sh
	sta colormemory		
	
	end;
	
	colormemory[ 0 ] := BLACK;

	//Snake_BodyAnim();
	if (anim = 0) then 
	begin
		MemCpyFast( #SRCBODY0, 0, #DSTBODY0, 8 );
		MemCpyFast( #SRCBODY2, 0, #DSTBODY1, 8 );
		MemCpyFast( #SRCJELLY0, 0, #DSTJELLY, 8 );
		
	end else begin
		MemCpyFast( #SRCBODY1, 0, #DSTBODY0, 8 );
		MemCpyFast( #SRCBODY3, 0, #DSTBODY1, 8 );		
		MemCpyFast( #SRCJELLY1, 0, #DSTJELLY, 8 );
		
	end;
	
	// reset joystick value as have now processed it
	joy := 0;
	
end;


procedure Snake_GetPieceAtTail();
begin
	n := st[ 0 ] & $fe; // get the body piece in new spot (remove eat ver)
end;
procedure Snake_TailVerticalAdjust();
begin
	if ( st > #SCREENBOTTOM ) then st := st - SCREENSIZE;
	if ( st < #SCREENTOP ) then st := st + SCREENSIZE;
		
end;

procedure Snake_MoveTail();
begin

	m := st[ 0 ]; // get snake tail char
	j := m; // j holds the final tail char we will later draw
	
	st[ 0 ] := tailChar; // drops a space, jelly or mushroom where the old tail was

	asm
	
	; colour the tail exit - jelly or mushroom
	lda st+1	; address of screen (high byte)
	and #$03
	clc
	adc #$94		; bring up to colour space
	sta colormemory+1
	lda st
	sta colormemory		
	
	end;
	
	if ( tailChar = CHAR_JELLY ) then 
		colormemory[ 0 ] := GREEN
	else
		colormemory[ 0 ] := YELLOW;
	
	tailChar := CHAR_SPACE; // reset/force back to space (clears mushroom or jelly - so only drops one)
	
	
	if ( m = CHAR_SNAKETAILRIGHT ) then 
	begin 
		// right
		st := st + 1;
		Snake_TailVerticalAdjust();
		Snake_GetPieceAtTail();
		if (n = CHAR_SNAKENW) then j := CHAR_SNAKETAILUP;
		if (n = CHAR_SNAKESW) then j := CHAR_SNAKETAILDOWN;
	end;
	
	if ( m = CHAR_SNAKETAILDOWN ) then
	begin
		// down
		st := st + 22;
		Snake_TailVerticalAdjust();
		Snake_GetPieceAtTail();
		if (n = CHAR_SNAKENW) then j := CHAR_SNAKETAILLEFT;
		if (n = CHAR_SNAKENE) then j := CHAR_SNAKETAILRIGHT;
	end;
	
	if ( m = CHAR_SNAKETAILLEFT ) then 
	begin
		// left
		st := st - 1;
		Snake_TailVerticalAdjust();
		Snake_GetPieceAtTail();
		if (n = CHAR_SNAKENE) then j := CHAR_SNAKETAILUP;
		if (n = CHAR_SNAKESE) then j := CHAR_SNAKETAILDOWN;
	end;
	
	if ( m = CHAR_SNAKETAILUP ) then 
	begin
		// up
		st := st - 22;
		Snake_TailVerticalAdjust();
		Snake_GetPieceAtTail();
		if (n = CHAR_SNAKESW) then j := CHAR_SNAKETAILLEFT;
		if (n = CHAR_SNAKESE) then j := CHAR_SNAKETAILRIGHT;
	end;
	
	if ( n >= CHAR_SNAKEHEADRIGHT) then
	begin
		// this can happen when game over, tail shrinks and stops at head
		gameMode := GAME_PRETITLE;
		asm
	
			; colour the tail exit - jelly or mushroom
			lda st+1	; address of screen (high byte)
			and #$03
			clc
			adc #$94		; bring up to colour space
			sta colormemory+1
			lda st
			sta colormemory		
	
		end;
		colormemory[ 0 ] := WHITE;
		j := CHAR_EGG;
		
	end;

	st[ 0 ] := j;	// draw new tail		
	
end;

