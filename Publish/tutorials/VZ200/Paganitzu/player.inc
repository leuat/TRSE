// All keys collected. Player hit Keyhole tile
procedure EndLevel();
begin
	if (gameStats.room < NumberOfRooms) then
	begin
		FxPlay(sfxKeyhole);
		// Load next level
		gameStats.room := gameStats.room +1;
		// Dissolve screen
		ClsFx();
		// Copy current room data to grid array
		CopyRoomToGameGrid(gameStats.room);
		DrawRoomTiles();
		// Draw Game HUD
		DrawHUD();
		// Get keys left for level 
		gameStats.keysLeft := levelKeys[gameStats.room-1];
		// Give extra points
		gameStats.score := gameStats.score + gameStats.bonus;
		// Reset bonus timer
		gameStats.bonus := BonusValue;
		// Give extra lives every 3 levels
		case gameStats.room of
			3:
				gameStats.lives := gameStats.lives + 1;
			6:
				gameStats.lives := gameStats.lives + 1;
			9:
				gameStats.lives := gameStats.lives + 1;
			12:
				gameStats.lives := gameStats.lives + 1;
			15:
				gameStats.lives := gameStats.lives + 1;
			18:
				gameStats.lives := gameStats.lives + 1;
		end;
		UpdateHUDNumbers();
	end
	else
	begin
		// Game completed! Well done!
		// Decompress the image to the screen
		Compression::Decompress(#congratsdata, #screen);
		FxPlay(sfxCongrats);
		myp := #mychar;
		mym := #congratsmsg;
		strlen := Functions::SizeOfString(#congratsmsg);
		v := 0;
		strpos := 0;
		Screen::SetPen(1);
		gameStats.congratsShowing := false;
		// Keep titlescreen going until Space or fire pressed
		while (gameStats.congratsShowing = false) do
		begin
			p := Input::GetJoystick(1);
			q := Input::GetJoystick(2);
			if (p = Input::Joy_Fire or (q = Input::Joy_Fire or (Input::GetPressedKey() = Input::Key_SPC))) then
			begin
					gameStats.congratsShowing := true;
			end;
			// Draw new char in the onscreen scroll every 4 pixels
			if (v = 0) then
			begin
				v := 4;
				myp[0] := mym[strpos];
				Font::DrawTextAt(#mychar,31,59,false);
				if (strpos < strlen) then strpos := strpos + 1 else strpos := 0;
			end;
			for u := 0 to 200 do wait(50);	
			// Smooth scroll the message 1 pixel at a time
			Screen::doSmoothScroll(30591,5);
			v := v - 1;
		end;
		v := 0;
		strpos := 0;
		gameStats.gameRunning := false;
		ClsFx2();
	end;
end;

// Move rock if possible
procedure ActionRockTile(x,y : global byte);
begin
	// Has rock moved flag?
	q := 0; // 1 if rock moves
	// Check tile left
	if (playerRecord.lastDirection = 0) then
	begin
		x := x -1;
		p := gameGrid[x+lookupGridY[y]];
	end;
	// Check tile right
	if (playerRecord.lastDirection = 1) then
	begin
		x := x +1;
		p := gameGrid[x+lookupGridY[y]];
	end;
	// Check tile above
	if (playerRecord.lastDirection = 2) then
	begin
		y := y -1;
		p := gameGrid[x+lookupGridY[y]];
	end;
	// Check tile below
	if (playerRecord.lastDirection = 3) then
	begin
		y := y +1;
		p := gameGrid[x+lookupGridY[y]];
	end;
	// Change tile if blank
	if (p = BlankTile) then
	begin
		// Don't push rocks off top of screen
		if (y < 10) then
		begin
			gameGrid[x+lookupGridY[y]] := RockTile;
			q := 1;			
		end;
	end;
	// Change tile if Water
	if (p = WaterTile) then
	begin
		gameGrid[x+lookupGridY[y]] := BlankTile;
		q := 1;
	end;
	if (q = 1) then
	begin
		FxPlay(sfxRock);
	end;
end;

// Player died
procedure KillPlayer();
begin
	// Show gave tile
	DrawSprite(playerRecord.x,playerRecord.y,PlayerTile+4);
	gameStats.lives := gameStats.lives -1;
	UpdateHUDNumbers();
	if (gameStats.lives = 0) then
	begin
		Font::DrawTextAt("                    ",6,25,false);
		Font::DrawTextAt("  G A M E  O V E R  ",6,30,false);
		Font::DrawTextAt("                    ",6,35,false);		
		FxPlay(sfxGameOver);
		ClsFx();
		Screen::Cls(1);
		gameStats.gameRunning := false;
	end
	else
	begin
		FxPlay(sfxDied);
		gameStats.bonus := BonusValue;
		gameStats.keysLeft := levelKeys[gameStats.room-1];
	end;
	v := 0;
	strpos := 0;
end;

// Change a tile to the PlayerTile
procedure ChangeTileToPlayer(x,y : global byte);
begin
	// Change tile to player
	gameGrid[x+lookupGridY[y]] := playerTile;
	q := 1; // Player moved
end;

// Perform some sort of action based on what tile the player is on
function PlayerActionTile(x,y : global byte) : byte;
begin
	r := gameGrid[x+lookupGridY[y]];
	q := 1; // Player has moved
	case r of
		BlankTile:
			begin
				ChangeTileToPlayer(x,y);
				return;
			end;
		DirtTile:
			begin
				ChangeTileToPlayer(x,y);
				return;
			end;
		KeyholeTile:
			begin
				EndLevel();
			end;
		KeyTile:
			begin
				ChangeTileToPlayer(x,y);
				gameStats.keysLeft := gameStats.keysLeft -1;
				gameStats.score := gameStats.score + 1;
				UpdateHUDNumbers();
				if (gameStats.keysLeft = 0) then
				begin
					FxPlay(sfxNoKeys);
				end
				else
				begin
					FxPlay(sfxKey);
				end;
			end;
		DiamondTile:
			begin
				ChangeTileToPlayer(x,y);
				gameStats.score := gameStats.score + 5;
				UpdateHUDNumbers();
				FxPlay(sfxDiamond);
			end;
		RockTile:
			begin
				ActionRockTile(x,y);
			end;
		end;
	PlayerActionTile := q;
end;

// Check if player tile can move to new pos
function CheckTileAt(u,v: global byte) : Boolean;
begin
	r := gameGrid[u+lookupGridY[v]];
	b := false;
	case r of
		BlankTile:
			b := true;
		DirtTile:
			b := true;
		DiamondTile:
			b := true;
		KeyTile:
			b := true;
		RockTile:
			b := true;
		KeyholeTile:
			begin
				if (gameStats.keysLeft = 0) then b := true;
			end;
	end;
	// Can't walk on pipes
	if (r > SpikesShaftTile and (r < DiamondTile)) then
	begin
		b := false;
	end;
	// Player movement sound effect
	if (b = true) then
	begin
		FxPlay(sfxMove);
		if (playerRecord.animationFrame = 0) then
		begin
			playerRecord.animationFrame := 1;
		end
		else
		begin
			playerRecord.animationFrame := 0;
		end;
	end;
	CheckTileAt := b;
end;

// Set tile where player used to be, to blankTile
procedure SetTileToBlank(u,v : global byte);
begin
	gameGrid[u+lookupGridY[v]] := blankTile;
end;

// Reset level 
procedure ResetLevel();
begin
	Font::DrawTextAt("                    ",6,25,false);
	Font::DrawTextAt("  RESETTING LEVEL!  ",6,30,false);
	Font::DrawTextAt("                    ",6,35,false);
	FxPlay(sfxResetLevel);
	// Copy current room data to grid array
	CopyRoomToGameGrid(gameStats.room);
	DrawRoomTiles();
	// Draw Game HUD
	DrawHUD();
	UpdateHUDNumbers();
	// Get keys left for level 
	gameStats.keysLeft := levelKeys[gameStats.room-1];
	// Reset bonus timer
	gameStats.bonus := BonusValue;
end;

// Things to do with player tile
procedure UpdatePlayerTile();
begin
	// Get player input
	p := Input::GetJoystick(1);
	q := Input::GetJoystick(2);
	s := Input::GetPressedKey();	// Scan for key values separately
	// Is player dead already?
	if (playerRecord.isDead = true) then
	begin
		KillPlayer();
		if (gameStats.lives > 0) then
		begin
			// Stop tile actions detecting player above
			playerRecord.x := 0;
			playerRecord.y := 0;
			playerRecord.facingDirection := true;
			playerRecord.isDead := false;
			CopyRoomToGameGrid(gameStats.room);
			DrawRoomTiles();
			return;
		end;
	end;

	// Check for movement
	if (p = Input::Joy_None and (q = Input::Joy_None and (s = Input::Key_None))) then return;
	
	// Reset level 
	if (s = Input::Key_R) then
	begin
		ResetLevel();
	end;
	
	// Up
	if (p = Input::Joy_Up or (q = Input::Joy_Up or (s = Input::Key_Q))) then
	begin
		// Can't move up if at top of screen
		if (playerRecord.y > 0) then
		begin
			if (CheckTileAt(playerRecord.x, playerRecord.y-1) = true) then
			begin
				playerRecord.lastDirection := 2;
				q := PlayerActionTile(playerRecord.x, playerRecord.y-1);
				if (q = 1) then
				begin
					SetTileToBlank(playerRecord.x, playerRecord.y);
					DrawSprite(playerRecord.x, playerRecord.y, BlankTile);
					playerRecord.y := playerRecord.y -1;
					ChangeTileToPlayer(playerRecord.x,playerRecord.y);
				end;
			end;
		end;
	end;
	// Down
	if (p = Input::Joy_Down or (q = Input::Joy_Down or (s = Input::Key_A))) then
	begin
		// Can't move up if at top of screen
		if (playerRecord.y < gridHeight) then
		begin
			if (CheckTileAt(playerRecord.x, playerRecord.y+1) = true) then
			begin
				playerRecord.lastDirection := 3;
				q := PlayerActionTile(playerRecord.x, playerRecord.y+1);
				if (q = 1) then
				begin
					SetTileToBlank(playerRecord.x, playerRecord.y);
					DrawSprite(playerRecord.x, playerRecord.y, BlankTile);
					playerRecord.y := playerRecord.y +1;
					ChangeTileToPlayer(playerRecord.x,playerRecord.y);
				end;
			end;
		end;
	end;
	// Left
	if (p = Input::Joy_Left or (q = Input::Joy_Left or (s = Input::Key_M))) then
	begin
		// Can't move up if at top of screen
		if (playerRecord.x > 0) then
		begin
			if (CheckTileAt(playerRecord.x-1, playerRecord.y) = true) then
			begin
				playerRecord.lastDirection := 0;
				q := PlayerActionTile(playerRecord.x-1, playerRecord.y);
				if (q = 1) then
				begin
					SetTileToBlank(playerRecord.x, playerRecord.y);
					DrawSprite(playerRecord.x, playerRecord.y, BlankTile);
					playerRecord.x := playerRecord.x -1;
					ChangeTileToPlayer(playerRecord.x,playerRecord.y);
					playerRecord.facingDirection := false;
				end;
			end;
		end;
	end;
	// Right
	if (p = Input::Joy_Right or (q = Input::Joy_Right or (s = Input::Key_COMMA))) then
	begin
		// Can't move up if at top of screen
		if (playerRecord.x < gridWidth) then
		begin
			if (CheckTileAt(playerRecord.x+1, playerRecord.y) = true) then
			begin
				playerRecord.lastDirection := 1;
				q := PlayerActionTile(playerRecord.x+1, playerRecord.y);
				if (q = 1) then
				begin
					SetTileToBlank(playerRecord.x, playerRecord.y);
					DrawSprite(playerRecord.x, playerRecord.y, BlankTile);
					playerRecord.x := playerRecord.x +1;
					ChangeTileToPlayer(playerRecord.x,playerRecord.y);
					playerRecord.facingDirection := true;
				end;
			end;
		end;
	end;
	DrawPlayerTile();
end;