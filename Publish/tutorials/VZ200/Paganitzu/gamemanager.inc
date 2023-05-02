// Reset Player and game stats
procedure ResetGameStats();
begin
	gameStats.room := 1;
	gameStats.score := 0;
	gameStats.bonus := BonusValue;
	gameStats.keysLeft := levelKeys[gameStats.room-1];
	gameStats.lives := 3;
	gameStats.exitDoorFrame := false;
	
	diamondRecord.animationFrame := 0;
	diamondRecord.frameCount := 3;
	diamondRecord.frameDelayMax := 30;
	diamondRecord.frameDelay := diamondRecord.frameDelayMax;
	diamondRecord.points := 1;
	
	spiderRecord.animationFrame := 0;
	spiderRecord.frameCount := 1;
	
	snakeRecord.animationFrame := 0;
	snakeRecord.frameCount := 3;
	
	playerRecord.animationFrame := 0;
	playerRecord.frameCount := 2;
	playerRecord.lastDirection := 1;
	playerRecord.facingDirection := true;
	playerRecord.isDead := false;
end;

// Move spikes if player above
procedure DrawSpikesHeadTile(x,y : global byte);
begin
	if (u = SpikesHeadTileActive) then
	begin
		DrawSprite(x, y, SpikesHeadTile);
	end
	else
	begin
		DrawSprite(x, y, u);	
	end;
	if (u = SpikesHeadTile) then
	begin
		// Only activate if player above
		if (playerRecord.x <> x) then return;
		// Player standing on spikes
		if (gameGrid[x+lookupGridY[y-1]] = PlayerTile) then
		begin
			playerRecord.isDead := true;
		end;
		// Check tiles between player and spikehead
		for p := y-1 to playerRecord.y step -1 do
		begin
			if (gameGrid[x+lookupGridY[p]] > BlankTile) then
			begin
				return;
			end;
		end;
		// Player is above spikes
		u := gameGrid[x+lookupGridY[y-1]];
		if (u = BlankTile) then
		begin
			gameGrid[x+lookupGridY[y-1]] := SpikesHeadTileActive;
			gameGrid[x+lookupGridY[y]] := SpikesShaftTile;
		end;
	end
	else
	begin
		// Keep movin' on up!
		u := gameGrid[x+lookupGridY[y-1]];	
		if (u = PlayerTile) then
		begin
			playerRecord.isDead := true;
		end;	
		if (u = BlankTile) then
		begin
			gameGrid[x+lookupGridY[y-1]] := SpikesHeadTileActive;
			gameGrid[x+lookupGridY[y]] := SpikesShaftTile;
		end;
	end;

end;

// Check if snake spits poison at player
procedure SnakeAttackPlayer(x, y: global byte);
begin
	if (y <> playerRecord.y) then return;
	p := 0; // Non-zero if non-blank tile found between snake and player
	if (x > playerRecord.x) then
	begin
		for q := playerRecord.x to x do
		begin
			if (gameGrid[q+lookupGridY[y]] > BlankTile) then
			begin
				p := 1;
			end;
			if (gameGrid[q+lookupGridY[y]] = SnakeTile or (gameGrid[q+lookupGridY[y]] = PlayerTile)) then
			begin
				p := 0;
			end;
		end;
	end
	else
	begin
		for q := x to playerRecord.x do
		begin
			if (gameGrid[q+lookupGridY[y]] > BlankTile) then
			begin
				p := 1;
			end;
			if (gameGrid[q+lookupGridY[y]] = SnakeTile or (gameGrid[q+lookupGridY[y]] = PlayerTile)) then
			begin
				p := 0;
			end;
		end;			
	end;
	if (p = 0) then
	begin
		// Spit poison at player
		if (x > playerRecord.x) then
		begin
			s := lookupScreenX[playerRecord.x+1]-2;
			tempInt := 32*lookupScreenY[y]+64;
			for q := lookupScreenX[x]-1 to s step -1 do
			begin
				Screen::Poke(28672+q+tempInt,255);
				for p := 0 to 50 do begin wait(200); end;
				Screen::Poke(28672+q+tempInt,0);				
			end;
			playerRecord.isDead := true;
		end
		else
		begin
			s := lookupScreenX[playerRecord.x]+1;
			tempInt := 32*lookupScreenY[y]+64;
			for q := lookupScreenX[x]+2 to s do
			begin
				Screen::Poke(28672+q+tempInt,255);
				for p := 0 to 50 do begin wait(200); end;
				Screen::Poke(28672+q+tempInt,0);				
			end;
			playerRecord.isDead := true;
		end;
	end;
end;

// Draw Snake tile
procedure DrawSnakeTile();
begin
	// Set direction snake faces, based on player location
	gameGrid[x+lookupGridY[y]] := SnakeTile;
	if (x > playerRecord.x) then
	begin
		gameGrid[x+lookupGridY[y]] := SnakeLeftTile;
	end;
	// Player is at same y-value as snake
	SnakeAttackPlayer(x,y);
	v := snakeRecord.animationFrame;
	if (gameGrid[x+lookupGridY[y]] = SnakeTile) then
	begin
		DrawSprite(x, y, u+v);
	end
	else
	begin
		DrawSprite(x, y, u+v);
	end;	
end;

// Check if player adjacent to spider
procedure CheckPlayerOrBlocked();
begin
	if (u = PlayerTile) then
	begin
		playerRecord.isDead := true;
	end;
	// Check Spider surrounding tiles
	if (u > BlankTile) then
	begin
		v := v +1;
	end;
end;

// Make tile diamond
procedure MakeTileDiamond(p,q : global byte);
begin
	gameGrid[p+lookupGridY[q]] := DiamondTile;
end;

// Turn tiles into diamonds
procedure TurnTilesToDiamonds(p,q,u : global byte);
begin
	case u of
		RockTile:
			MakeTileDiamond(p,q);
		DirtTile:
			MakeTileDiamond(p,q);
	end;
	// Pipes
	if (u > SpikesShaftTile and (u < DiamondTile)) then
	begin
		MakeTileDiamond(p,q);
	end;
end;

// Check if tiles can be changed
procedure CheckSpiderDiamonds(x,y : global byte);
begin
	u := gameGrid[x-1+lookupGridY[y]];
	TurnTilesToDiamonds(x-1,y,u);
	u := gameGrid[x+1+lookupGridY[y]];
	TurnTilesToDiamonds(x+1,y,u);
	u := gameGrid[x+lookupGridY[y-1]];
	TurnTilesToDiamonds(x,y-1,u);
	u := gameGrid[x+lookupGridY[y+1]];
	TurnTilesToDiamonds(x,y+1,u);
end;

// Check for player 
procedure CheckPlayerSequence(x,y : global byte);
begin
	// Check for player adjacent or blocked
	v := 0; // if v = 4 then all directions blocked
	u := gameGrid[x-1+lookupGridY[y]];
	CheckPlayerOrBlocked();
	u := gameGrid[x+1+lookupGridY[y]];
	CheckPlayerOrBlocked();
	u := gameGrid[x+lookupGridY[y-1]];
	CheckPlayerOrBlocked();
	u := gameGrid[x+lookupGridY[y+1]];
	CheckPlayerOrBlocked();	
end;

// Draw Spider tile
procedure DrawSpiderTile(x,y : global byte);
begin
	// Check if player adjacent
	CheckPlayerSequence(x,y);
	// Blocked
	if (v = 4) then
	begin
		// Turn spider into diamond
		gameGrid[x+lookupGridY[y]] := DiamondTile;
		// Check around spider to turn to diamonds
		CheckSpiderDiamonds(x,y);
		return;
	end;
	// Move spider
	u := gameGrid[x+lookupGridY[y]];
	// Spider is 'facing' left
	if (u = SpiderTileLeft) then
	begin
		u := gameGrid[x+lookupGridY[y+1]];
		if (u = BlankTile) then
		begin
			gameGrid[x+lookupGridY[y]] := BlankTile;
			gameGrid[x+lookupGridY[y+1]] := SpiderTileDownScanned;
		end
		else
		begin
			u := gameGrid[x-1+lookupGridY[y]];
			if (u = BlankTile) then
			begin
				gameGrid[x+lookupGridY[y]] := BlankTile;
				gameGrid[x-1+lookupGridY[y]] := SpiderTileLeft;
			end
			else
			begin
				gameGrid[x+lookupGridY[y]] := SpiderTileRight;
			end;
		end;
	end;
	// Spider is 'facing' down.
	if (u = SpiderTileDown) then
	begin
		u := gameGrid[x+1+lookupGridY[y]];
		if (u = BlankTile) then
		begin
			gameGrid[x+lookupGridY[y]] := BlankTile;
			gameGrid[x+1+lookupGridY[y]] := SpiderTileRightScanned;
		end
		else
		begin
			u := gameGrid[x+lookupGridY[y+1]];
			if (u = BlankTile) then
			begin
				gameGrid[x+lookupGridY[y]] := BlankTile;
				gameGrid[x+lookupGridY[y+1]] := SpiderTileDownScanned;
			end
			else
			begin
				gameGrid[x+lookupGridY[y]] := SpiderTileUp;
			end;
		end;
	end;
	// Spider is 'facing' right
	if (u = SpiderTileRight) then
	begin
		u := gameGrid[x+lookupGridY[y-1]];
		if (u = BlankTile and (y > 0)) then
		begin
			gameGrid[x+lookupGridY[y]] := BlankTile;
			gameGrid[x+lookupGridY[y-1]] := SpiderTileUp;
		end
		else
		begin
			u := gameGrid[x+1+lookupGridY[y]];
			if (u = BlankTile) then
			begin
				gameGrid[x+lookupGridY[y]] := BlankTile;
				gameGrid[x+1+lookupGridY[y]] := SpiderTileRightScanned;
			end
			else
			begin
				gameGrid[x+lookupGridY[y]] := SpiderTileLeft;
			end;
		end;
	end;
	// Spider is 'facing' up.
	if (u = SpiderTileUp) then
	begin
		u := gameGrid[x-1+lookupGridY[y]];
		if (u = BlankTile) then
		begin
			gameGrid[x+lookupGridY[y]] := BlankTile;
			gameGrid[x-1+lookupGridY[y]] := SpiderTileLeft;
		end
		else
		begin
			if (y = 0) then
			begin
				gameGrid[x+lookupGridY[y+1]] := BlankTile;
				gameGrid[x+lookupGridY[y]] := SpiderTileDown;
			end
			else
			begin
				u := gameGrid[x+lookupGridY[y-1]];
				if (u = BlankTile) then
				begin
					gameGrid[x+lookupGridY[y]] := BlankTile;
					gameGrid[x+lookupGridY[y-1]] := SpiderTileUp;
				end
				else
				begin
					gameGrid[x+lookupGridY[y]] := SpiderTileRightScanned;
				end;
			end;
		end;
	end;
	v := spiderRecord.animationFrame;
	DrawSprite(x, y, SpiderTile+v);
end;

// Update diamond tile state
procedure UpdateDiamondTile();
begin
	// Update animation
	if (diamondRecord.frameDelay > 1) then
	begin
		diamondRecord.frameDelay := diamondRecord.frameDelay -1;
		return;
	end
	else
	begin
		if (diamondRecord.animationFrame < diamondRecord.frameCount) then
		begin
			diamondRecord.animationFrame := diamondRecord.animationFrame +1;
		end
		else
		begin
			diamondRecord.animationFrame := 0;
			diamondRecord.frameDelay := diamondRecord.frameDelayMax;
		end;
	end;
end;

// Update snake tile state - TODO: Shoot player.
procedure UpdateSnakeTile();
begin
	begin
		if (snakeRecord.animationFrame < snakeRecord.frameCount) then
		begin
			snakeRecord.animationFrame := snakeRecord.animationFrame +1;
		end
		else
		begin
			snakeRecord.animationFrame := 0;
		end;
	end;
end;

// Update snake tile state - TODO: Move spider.
procedure UpdateSpiderTile();
begin
	begin
		if (spiderRecord.animationFrame < spiderRecord.frameCount) then
		begin
			spiderRecord.animationFrame := spiderRecord.animationFrame +1;
		end
		else
		begin
			spiderRecord.animationFrame := 0;
		end;
	end;
end;

// Draw room tiles to screen
procedure DrawRoomTiles();
begin
	// Update animation frame for all tiles
	// Scan through grid to update states and change sprites
	for y := 0 to gridHeight do
	begin
		for x := 0 to gridWidth do
		begin
			u := gameGrid[x+lookupGridY[y]];
			case u of
				PlayerTile:
					begin
						playerRecord.x := x;
						playerRecord.y := y;
						DrawPlayerTile();
					end;
				DiamondTile:
					begin
						v := diamondRecord.animationFrame;
						DrawSprite(x, y, u+v);
					end;
				SpiderTileLeft:
					begin
						DrawSpiderTile(x,y);					
					end;
				SpiderTileUp:
					begin
						DrawSpiderTile(x,y);					
					end;
				SpiderTileRight:
					begin
						DrawSpiderTile(x,y);
					end;
				SpiderTileDown:
					begin
						DrawSpiderTile(x,y);
					end;
				SnakeTile:
					begin
						DrawSnakeTile();
					end;
				SnakeLeftTile:
					begin
						DrawSnakeTile();
					end;
				SpiderTileDownScanned:
					begin
						gameGrid[x+lookupGridY[y]] := SpiderTileDown;
					end;
				SpikesHeadTile:
					begin
						DrawSpikesHeadTile(x,y);
					end;
				SpikesHeadTileActive:
					begin
						DrawSpikesHeadTile(x,y);
					end;
				KeyholeTile:
					begin
						UpdateExitDoor(x,y);
					end;
			else
			begin
				if (u <> SpiderTileRightScanned) then
				begin
					DrawSprite(x, y, u);					
				end;
			end;
			// Reset SpiderTileRightScanned
			if (y > 0) then
			begin
				u := gameGrid[x+lookupGridY[y-1]];
				if (u = SpiderTileRightScanned) then
				begin
					gameGrid[x+lookupGridY[y-1]] := SpiderTileRight;
				end;
			end;
		end;
	end;	
end;

// Decrease bonus value
procedure UpdateBonus();
begin
	if (gameStats.bonus > 0) then
	begin
		gameStats.bonus := gameStats.bonus -1;
		UpdateBonusHUD();
	end;
end;