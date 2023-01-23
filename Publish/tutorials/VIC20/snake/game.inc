
// initialise a new game
procedure Game_New();
begin
	
	numFood := 0;
	foodAnim := 0;
	joy := 0;
	flag := 0;
	waveNum := 0;
	roundsPerWave := 2; // start two rounds per wave
	
	numMushrooms := 12;
	numStartFood := 1; // 1
	
	stage[0] := $01;
	
	UI_SetScreenColour();
	
	mu := #SCREENTOP; // mushroom grower pointer
	
end;

procedure Game_SetRandomFood();
begin
	
	n := RND() & 3 + numStartFood;
	numFood += n;
	
end;

//Set the screen memory address to the top of the play area + random value
procedure Game_SetTopOfScreenRND();
begin
	
	j := RND();
	screenmemory := #SCREENTOP + j;
	
end;

// needed to make sure no impossible foods
// Is there a mushroom here?
procedure Game_WipeShroom(pa: global byte);
begin
	
	m := screenmemory[ pa ];
	if (m >= CHAR_MUSHROOM3 and m <= CHAR_MUSHROOM0) then
		screenmemory[ pa ] := CHAR_SPACE;
	
end;

// k = char to draw
// l = colour to draw
// n = number to draw
procedure Game_DrawRndTings();
begin
	
	SOUND3_REGISTER := 0; SOUNDFX_REGISTER := 0;
	
	Game_SetTopOfScreenRND();

	for i := 0 to n do
	begin
		
		while ( screenmemory[ 1 ] <> CHAR_SPACE ) do
		begin
			
			j := RND();
			screenmemory := screenmemory + j;
			if (screenmemory >= #SCREENBOTTOMMINUS1) then Game_SetTopOfScreenRND();

		end;

		SOUND2_REGISTER := 128 + ( j & 63 )<<1; 
		
		screenmemory[ 1 ] := k; // mushroom mushroom
		setcolormemoryaddress();
		colormemory[ 1 ] := l; // colour
		
		// check if a mushroom exists either side, if so, wipe it
		Game_WipeShroom(0);
		Game_WipeShroom(2);
/*		m := screenmemory[ 0 ];
		if (Game_WipeShroom() = 1) then screenmemory[ 0 ] := CHAR_SPACE;
		m := screenmemory[ 2 ];
		if (Game_WipeShroom() = 1) then screenmemory[ 2 ] := CHAR_SPACE;
	*/	
		ticks := 1;
		while ( ticks & 3 <> 0) do
			VblWait();		
		
		SOUND2_REGISTER := 0;
		
	end;


end;

// intentionally - slowly grow mushrooms by
// scanning screen area, do small chunks at a time
procedure Game_GrowShrooms();
begin
	
	for i := 0 to 11 do
	begin
		
		j := mu[ i ];
		if ( j >= CHAR_MUSHROOM2 and j <= CHAR_MUSHROOM0 ) then
			mu[ i ] := j - 1;
		
	end;
	
	mu += 11;
	if (mu > #SCREENBOTTOM) then mu := #SCREENTOP;
	
end;

// draw a lot of mushrooms
procedure Game_DrawRoundStart();
begin
	
	n := numMushrooms;
	k := CHAR_MUSHROOM3;
	l := WHITE;
	Game_DrawRndTings();

end;

procedure Game_DrawNextRound();
begin

	Game_SetRandomFood();
	k := CHAR_CHERRY;
	l := RED;
	Game_DrawRndTings();
	
	Game_SetRandomFood();
	k := CHAR_APPLE;
	l := BLUE; //GREEN;
	Game_DrawRndTings();
	
	Game_SetRandomFood();
	k := CHAR_GRAPES;
	l := PURPLE;
	Game_DrawRndTings();

	Game_SetRandomFood();
	k := CHAR_BUG;
	l := BLACK;
	Game_DrawRndTings();
		
end;

procedure Game_BonusCheck();
begin

	UI_DrawBonus();
	SOUND3_REGISTER := 0;
	
	p1 := #SCREENTOP;
	for i := 0 to 22 do
	begin
		
		for j := 0 to 22 do
		begin
			
			k := p1[ j ];
			
			if (k >=CHAR_MUSHROOM3 and k <= CHAR_MUSHROOM0) then
			begin
				
				p1[ j ] := CHAR_CHECK;
				
				Snake_Score();
				
				SOUNDFX_REGISTER := 150;
				
				l := ticks + 4;
				while ( ticks < l) do
					VblWait();
					
				SOUNDFX_REGISTER := 0;
				
				p1[ j ] := CHAR_SPACE;
				
				VblWait();

			end;
			
		end;
		p1 += 22;
		
	end;

	Long_Wait();

end;

procedure Game_StartWave();
begin

	round := 0;
	
	UI_SetScreenColour();
	UI_DrawNewScreen();

	Snake_Init();

end;

procedure Game_NextRound();
begin
	
	round += 1;
	
	UI_DrawAll();
	
	if ( round >= roundsPerWave) then
	begin
		
		Game_BonusCheck();
		
		bcdadd( #stage, #stageadd, 1 );
		waveNum += 1;
		
		if (numMushrooms < 250) then numMushrooms += 3;
		
		if (waveNum & 1 = 0 and numStartFood < 30) then numStartFood += 2;
		if (waveNum & 1 = 1 and roundsPerWave < 10) then
		begin
			roundsPerWave += 1;
			if (numStartFood > 1) then numStartFood -= 1;
		end;
		
		Game_StartWave(); // draws UI also
		Game_DrawRoundStart();
		
		if (waveNum & 1 = 0 and snakeSpeed > SNAKE_FASTEST) then snakeSpeed -= 1;
		
	end;
	
	Game_DrawNextRound();	
	
end;


