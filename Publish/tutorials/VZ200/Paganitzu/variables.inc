var
	// Other variables	
	mychar : string = "A";
	myp, mym : pointer;
	strpos, p, q, r, s, u, v, x, y : byte;
	b : boolean;
	tempInt : Integer;
	message : string = "Press 'S' or Fire to start or 'I' for instructions! Welcome to Paganitzu! Help Andre explore the dark caverns underneath Paganitzu temple! Use Joysticks or Keys: QA = Up/Down M, = Left/Right R = Reset Level. Check out BlueBilby.com ... ";
	congratsmsg : string = "You have escaped Paganitzu temple! Well done! The Gods are pleased with you. Head back to civilization with your riches. Press 'Space' or Fire for menu ...";
	strlen : byte;
	
	titledata : incbin("title.bin") compressed;
	congratsdata : incbin("congrats.bin") compressed;
	
	// Records
	gameStats_record = record
		lives : byte;
		bonus : Integer;
		keysLeft : byte;
		room: byte;
		score : Integer;
		gameRunning : boolean;
		congratsShowing : boolean;
		frameDelayMax : byte;
		exitDoorFrame : boolean;
	end;
	gameStats : gameStats_record;
	
	// Constants
	const BlankTile : byte = 0;
	const DirtTile : byte = 1;
	const WallTile : byte = 2;				// These tiles cannot be walked on
	const BrickTile : byte = 3;				//
	const KeyholeTile : byte = 4;				//
	const SpikesShaftTile : byte = 5;			//
	const PipeTile : byte = 6;				//
	const DiamondTile : byte = 13;			// From here on are special tiles
	const KeyTile : byte = 17;
	const SpikesHeadTile : byte = 18;
	const RockTile : byte = 19;
	const WaterTile : byte = 20;				// To here
	const SpiderTile : byte = 21;
	const SnakeTile : byte = 23;
	const SnakeLeftTile : byte = 27;
	const PlayerTile : byte = 31;
	const PlayerLeftTile : byte = 33;
	const SpiderTileLeft : byte = 35;
	const SpiderTileUp : byte = 36;
	const SpiderTileRight : byte = 37;
	const SpiderTileDown : byte = 48;
	const SpiderTileRightScanned : byte = 49;
	const SpiderTileDownScanned : byte = 50;
	const SpikesHeadTileActive : byte = 51;
	
	const sfxMove : byte = 0;
	const sfxDiamond : byte = 1;
	const sfxRock : byte = 2;
	const sfxKeyhole : byte = 3;
	const sfxKey : byte = 4;
	const sfxDied : byte = 5;
	const sfxStart : byte = 6;
	const sfxNoKeys : byte = 7;
	const sfxGameOver : byte = 8;
	const sfxResetLevel : byte = 9;
	const sfxCongrats : byte = 10;
	
	const gridWidth : byte = 13;
	const gridHeight : byte = 8;
	
	const NumberOfRooms : byte = 20;
	const BonusValue : Integer = 300;
	
	// Store current level live data
	gameGrid : array[gridWidth*gridHeight] of byte;
	lookupGridY : array[gridHeight] of Integer = BuildTable("i*GRIDWIDTH");
	lookupScreenY : array[gridHeight] of Integer = BuildTable("i*8");
	lookupScreenX : array[gridWidth] of Integer = BuildTable("i*2");
	
	// Tile records
	diamondTile_record = record
			animationFrame : byte;
			frameCount : byte;
			frameDelay : byte; // Delay before animation starts.
			frameDelayMax : byte;
			points : byte;
	end;
	diamondRecord : diamondTile_record;
	
	spiderTile_record = record
			animationFrame : byte;
			frameCount : byte;
	end;
	spiderRecord : spiderTile_record;
	
	snakeTile_record = record
			animationFrame : byte;
			frameCount : byte;
	end;
	snakeRecord : snakeTile_record;
	
	playerTile_record = record
			animationFrame : byte;
			frameCount : byte;
			x, y : byte;
			lastDirection : byte; // ( 0 = left, 1 = right, 2 = up, 3 = down )
			facingDirection : boolean; // ( false = left, true = right )
			isDead : boolean;
	end;
	playerRecord : playerTile_record;

	// HUD and Tile Data
	HUDData: array[] of byte = (255, 255, 255, 255, 255, 255, 255, 095, 223, 223, 127, 127, 255, 119, 119, 119, 093, 127, 255, 095, 119, 119, 119, 127, 255, 119, 119, 119, 127, 127, 255, 119, 223, 223, 127, 127, 255, 255, 255, 255, 255, 255, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 255, 255, 255, 255, 255, 255, 255, 095, 127, 125, 125, 095, 253, 253, 221, 221, 221, 255, 255, 125, 253, 221, 125, 127, 255, 221, 221, 221, 221, 255, 253, 127, 127, 125, 221, 095, 255, 255, 255, 255, 255, 255, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 255, 255, 255, 255, 255, 255, 253, 253, 093, 221, 095, 095, 253, 255, 125, 221, 253, 255, 253, 255, 125, 221, 127, 127, 253, 255, 125, 221, 255, 223, 253, 093, 095, 125, 093, 127, 255, 255, 255, 255, 255, 255, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 255, 255, 255, 255, 255, 255, 255, 221, 213, 221, 245, 255, 255, 221, 223, 221, 223, 255, 255, 215, 215, 213, 247, 255, 255, 221, 223, 247, 253, 255, 255, 221, 213, 247, 215, 255, 255, 255, 255, 255, 255, 255, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 255, 255, 255, 255, 255, 255, 253, 127, 127, 125, 223, 095, 253, 221, 221, 221, 221, 255, 253, 125, 221, 221, 223, 127, 253, 221, 221, 221, 223, 223, 253, 127, 125, 223, 125, 127, 255, 255, 255, 255, 255, 255, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000);
	TileData: array[] of byte = (000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 068, 068, 021, 085, 081, 068, 021, 085, 021, 084, 085, 021, 021, 084, 068, 017, 042, 168, 213, 086, 213, 086, 213, 086, 213, 086, 213, 086, 213, 086, 063, 252, 170, 170, 149, 085, 149, 085, 149, 085, 170, 170, 085, 149, 085, 149, 085, 149, 042, 168, 215, 214, 223, 246, 223, 246, 215, 214, 215, 214, 213, 086, 063, 252, 017, 016, 017, 016, 017, 016, 017, 016, 017, 016, 017, 016, 017, 016, 017, 016, 000, 000, 000, 000, 255, 255, 085, 085, 085, 085, 255, 255, 000, 000, 000, 000, 013, 112, 013, 112, 013, 112, 013, 112, 013, 112, 013, 112, 013, 112, 013, 112, 013, 112, 013, 112, 253, 112, 085, 112, 085, 112, 255, 192, 000, 000, 000, 000, 013, 112, 013, 112, 013, 127, 013, 085, 013, 085, 003, 255, 000, 000, 000, 000, 000, 000, 000, 000, 003, 255, 013, 085, 013, 085, 013, 127, 013, 112, 013, 112, 000, 000, 000, 000, 255, 192, 085, 112, 085, 112, 253, 112, 013, 112, 013, 112, 013, 112, 013, 112, 253, 127, 085, 085, 085, 085, 253, 127, 013, 112, 013, 112, 002, 192, 010, 240, 042, 252, 170, 255, 255, 170, 063, 168, 015, 160, 003, 128, 002, 192, 010, 240, 042, 252, 170, 255, 085, 170, 021, 168, 005, 160, 001, 128, 001, 192, 005, 240, 021, 252, 085, 255, 255, 085, 063, 084, 015, 080, 003, 064, 002, 064, 010, 080, 042, 084, 170, 085, 255, 170, 063, 168, 015, 160, 003, 128, 252, 000, 204, 000, 255, 000, 003, 192, 000, 240, 003, 060, 000, 012, 000, 048, 034, 032, 034, 032, 170, 168, 017, 016, 017, 016, 017, 016, 017, 016, 085, 084, 015, 240, 063, 124, 255, 223, 255, 255, 255, 255, 251, 255, 062, 252, 015, 240, 042, 168, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 042, 168, 128, 002, 143, 242, 047, 248, 015, 240, 131, 194, 047, 248, 011, 224, 032, 008, 000, 000, 143, 242, 143, 242, 047, 248, 003, 192, 047, 248, 139, 226, 008, 032, 002, 064, 010, 160, 042, 160, 040, 000, 010, 128, 064, 160, 032, 040, 010, 160, 000, 144, 002, 168, 010, 128, 010, 040, 002, 160, 016, 168, 032, 040, 010, 160, 000, 036, 000, 170, 002, 160, 002, 138, 002, 160, 004, 168, 032, 040, 010, 160, 000, 144, 002, 168, 010, 168, 010, 000, 002, 160, 016, 168, 032, 040, 010, 160, 001, 128, 010, 160, 010, 168, 000, 040, 002, 160, 010, 001, 040, 008, 010, 160, 006, 000, 042, 128, 002, 160, 040, 160, 010, 128, 042, 004, 040, 008, 010, 160, 024, 000, 170, 000, 010, 128, 162, 128, 010, 128, 042, 016, 040, 008, 010, 160, 006, 000, 042, 128, 042, 160, 000, 160, 010, 128, 042, 004, 040, 008, 010, 160, 015, 192, 015, 144, 013, 072, 006, 168, 005, 064, 003, 000, 003, 000, 002, 128, 015, 192, 015, 144, 013, 072, 006, 168, 005, 064, 015, 192, 044, 192, 032, 160, 003, 240, 006, 240, 033, 112, 042, 144, 001, 080, 000, 192, 000, 192, 002, 128, 003, 240, 006, 240, 033, 112, 042, 144, 001, 080, 003, 240, 003, 056, 010, 008, 010, 160, 042, 168, 042, 168, 042, 168, 042, 168, 042, 168, 021, 084, 085, 085);
	
	// Levels data
	levelKeys: array[] of byte = (2, 4, 6, 1, 4, 6, 2, 2, 4, 4, 1, 6, 1, 3, 4, 4, 8, 6, 2, 3);
	Room1: array[] of byte = (WallTile, WallTile, WallTile, WallTile, WallTile, WallTile, WallTile, WallTile, BrickTile, BrickTile, BrickTile, PipeTile+1, BrickTile,
							 WallTile, PlayerTile, WallTile, WallTile, WallTile, WallTile, WaterTile, WaterTile, PipeTile, PipeTile+5, BrickTile, PipeTile+1, BrickTile,
							 WallTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, WaterTile, WaterTile, BrickTile, PipeTile+3, PipeTile, PipeTile+2, BrickTile,
							 WallTile, BlankTile, RockTile, WallTile, WallTile, BlankTile, DirtTile, DirtTile, BrickTile, BrickTile, BrickTile, BrickTile, BrickTile,
							 WallTile, BlankTile, BlankTile, BlankTile, SnakeTile, BlankTile, DirtTile, DirtTile, KeyTile, BlankTile, BlankTile, PipeTile+4, PipeTile,
							 WallTile, BlankTile, WallTile, WallTile, WallTile, WallTile,WallTile, WallTile, WallTile, DiamondTile, DiamondTile, PipeTile+3, PipeTile+5,
							 BrickTile, KeyTile, BlankTile, BlankTile, BlankTile, BlankTile, DiamondTile, DiamondTile, DiamondTile, BrickTile, BrickTile, BrickTile, PipeTile+1,
							 BrickTile, BrickTile, BrickTile, BrickTile, KeyholeTile, BrickTile, BrickTile, BrickTile, BrickTile, BrickTile, BrickTile, BrickTile, PipeTile+1);
							 
	Room2: array[] of byte = (BrickTile, BrickTile, BrickTile, BrickTile, BrickTile, BrickTile, PlayerTile, BrickTile, BrickTile, BrickTile, BrickTile, BrickTile, BrickTile,
							 BrickTile, BlankTile, BlankTile, SpiderTileLeft, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, SpiderTileLeft, BlankTile, BlankTile, BrickTile,
 							 BrickTile, BlankTile, BrickTile, KeyTile, BrickTile, BlankTile, DiamondTile, BlankTile, BrickTile, KeyTile, BrickTile, BlankTile, BrickTile,
 							 BrickTile, BlankTile, BrickTile, BrickTile, BrickTile, BlankTile, DiamondTile, BlankTile, BrickTile, BrickTile, BrickTile, BlankTile, BrickTile,
 							 BrickTile, BlankTile, BrickTile, BrickTile, BrickTile, BlankTile, DiamondTile, BlankTile, BrickTile, BrickTile, BrickTile, BlankTile, BrickTile,
 							 BrickTile, BlankTile, BrickTile, KeyTile, BrickTile, BlankTile, DiamondTile, BlankTile, BrickTile, KeyTile, BrickTile, BlankTile, BrickTile,
							 BrickTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BrickTile,
							 BrickTile, BrickTile, BrickTile, BrickTile, BrickTile, BrickTile, KeyholeTile, BrickTile, BrickTile, BrickTile, BrickTile, BrickTile, BrickTile);

	Room3: array[] of byte = (WallTile, WallTile, WallTile, WallTile, WallTile, WallTile, KeyholeTile, WallTile, WallTile, WallTile, WallTile, WallTile, WallTile,  
							 WallTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, KeyTile, BlankTile, BlankTile, WallTile, WallTile, WallTile, WallTile, 
							 WallTile, BlankTile, BrickTile, BlankTile, RockTile, BlankTile, KeyTile, BlankTile, BlankTile, WallTile, WallTile, WallTile, WallTile,
							 WallTile, BlankTile, BrickTile, BlankTile, BlankTile, BlankTile, BlankTile, KeyTile, BlankTile, BlankTile, BlankTile, SnakeTile, WallTile,
							 WallTile, BlankTile, BrickTile, BlankTile, SnakeTile, BlankTile, KeyTile, BlankTile, BlankTile, RockTile, BlankTile, BlankTile, WallTile,
							 WallTile, BlankTile, BrickTile, BrickTile, BlankTile, BlankTile, RockTile, KeyTile, BlankTile, BlankTile, BlankTile, BlankTile, WallTile,
							 WallTile, PlayerTile, BrickTile, BrickTile, BlankTile, KeyTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, SnakeTile, WallTile,
							 WallTile, WaterTile, WaterTile, WaterTile, WaterTile, WaterTile, PipeTile, PipeTile, PipeTile, PipeTile, PipeTile, PipeTile, PipeTile);

	Room4: array[] of byte = (PipeTile+1, BrickTile, BrickTile, KeyholeTile, BrickTile, BrickTile, BrickTile, BrickTile, BrickTile, BrickTile, BrickTile, BrickTile, BrickTile,
							 WaterTile, WaterTile, BrickTile, PlayerTile, WallTile, WallTile, WallTile, WallTile, WallTile, WallTile, WallTile, WallTile, WallTile, 
							 WaterTile, WaterTile, BrickTile, BlankTile, WallTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, WallTile, 
							 BrickTile, BrickTile, BrickTile, BlankTile, RockTile, BlankTile, BlankTile, DiamondTile, DiamondTile, DiamondTile, DiamondTile, BlankTile, WallTile, 
							 BrickTile, BrickTile, BrickTile, WallTile, WallTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, WallTile, 
							 BrickTile, BlankTile, RockTile, RockTile, BlankTile, BlankTile, BlankTile, DiamondTile, DiamondTile, DiamondTile, DiamondTile, BlankTile, WallTile, 
							 BrickTile, BrickTile, KeyTile, BrickTile, WallTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, SpiderTileLeft, BlankTile, WallTile, 
							 BrickTile, BrickTile, BrickTile, BrickTile, WallTile, WallTile, WallTile, WallTile, WallTile, WallTile, WallTile, WallTile, WallTile);
							 
	Room5: array[] of byte = (BrickTile, KeyholeTile, BrickTile, PipeTile+3, PipeTile, PipeTile, PipeTile, PipeTile, PipeTile, WaterTile, PipeTile, PipeTile, PipeTile+5,
							 BrickTile, KeyTile,   WaterTile, WaterTile, BlankTile, BlankTile, BlankTile, RockTile, BlankTile, WaterTile, KeyTile,   KeyTile, PipeTile+1,
							 PipeTile+4, PipeTile, WaterTIle, WaterTile, DiamondTile, BlankTile, BlankTile, RockTile, BlankTile, WaterTile, RockTile, DirtTIle, PipeTile+1,
							 PipeTile+1, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, PipeTile+3, 
							 PipeTile+1, BlankTile, PipeTile+4, PipeTile, WaterTile, WaterTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, WaterTile, PipeTile+5, 
							 PipeTile+2, PlayerTile, PipeTile+1, KeyTile, WaterTile, WaterTile, DirtTile, PipeTile+4, PipeTile, PipeTile, BlankTile, DiamondTile, PipeTile+1, 
							 PipeTile+4, PipeTile, PipeTile+2, BlankTile, BlankTile, BlankTile, SpiderTileLeft, PipeTile+1, BlankTile, BlankTile, BlankTile, DiamondTile, PipeTile+1, 
							 PipeTile+3, PipeTile, PipeTile, PipeTile, PipeTile, PipeTile, PipeTile, PipeTile+2, BrickTile, BrickTile, PipeTile+4, PipeTile, PipeTile+2);
	
	Room6: array[] of byte = (BrickTile, BrickTile, BrickTile, BrickTile, BrickTile, BrickTile, BrickTile, BrickTile, BrickTile, BrickTile, BrickTile, BrickTile, BrickTile,
							 BrickTile, BlankTile, BlankTile, KeyTile,   RockTile,  RockTile,  PlayerTile, RockTile, RockTile,  RockTile,  BlankTile, KeyTile,   BrickTile,
							 BrickTile, RockTile,  BlankTile, RockTile, BlankTile, BlankTile, BlankTile, RockTile, BlankTile, RockTile,  BlankTile, BlankTile, BrickTile,
							 BrickTile, KeyTile,   RockTile,  BlankTile, RockTile, RockTile, RockTile,  BlankTile, BlankTile, RockTile,  RockTile,  RockTile,  BrickTile,
							 BrickTile, RockTile,  BlankTile, RockTile,  RockTile, RockTile,  RockTile,  RockTile,  BlankTile, BlankTile, BlankTile, BlankTile, BrickTile,
							 BrickTile, RockTile,  KeyTile,   RockTile,  KeyTile,   RockTile,  BlankTile, BlankTile,  BlankTile, RockTile,  RockTile,  WaterTile, BrickTile,
							 BrickTile, BlankTile, BlankTile, BlankTile, BlankTile, RockTile,  BlankTile, RockTile,  BlankTile, RockTile,  BlankTile, KeyTile,   KeyholeTile,
							 BrickTile, BrickTile, BrickTile, BrickTile, BrickTile, BrickTile, BrickTile, BrickTile, BrickTile, BrickTile, BrickTile, BrickTile, BrickTile);
	
	Room7: array[] of byte = (WallTile,  WallTile, WallTile, WallTile, WallTile, WallTile, WallTile, WallTile, WallTile, WallTile, WallTile, WallTile, WallTile,  
							 KeyholeTile,DirtTile, DirtTile, DirtTile, DirtTile, DirtTile, DirtTile, DirtTile, DirtTile, DirtTile, DirtTile, DirtTile, WallTile,
							 WallTile,  DirtTile, RockTile, DirtTile, DirtTile, DirtTile, RockTile, DirtTile, DirtTile, DirtTile, DirtTile, DirtTile, WallTile,
							 WallTile,  DirtTile, DirtTile, BlankTile,BlankTile, BlankTile, DirtTile, BlankTile, BlankTile, BlankTile, DirtTile, DirtTile, WallTile,
							 WallTile,  DirtTile, DirtTile, BlankTile, KeyTile, SpiderTileLeft, DirtTile,  BlankTile, KeyTile, SpiderTileLeft, DirtTile, DirtTile, WallTile,
							 WallTile,  DirtTile, DirtTile, DirtTile, DirtTile, DirtTile, DirtTile, DirtTile, DirtTile, DirtTile, DirtTile, DirtTile, WallTile,
							 WallTile,  DirtTile, DirtTile, DirtTile, DirtTile, DirtTile, DirtTile, DirtTile, DirtTile, DirtTile, DirtTile, PlayerTile, WallTile,
							 WallTile,  WallTile, WallTile, WallTile, WallTile, WallTile, WallTile, WallTile, WallTile, WallTile, WallTile, WallTile, WallTile);

	Room8: array[] of byte = (WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  
							 WallTile,  PlayerTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, RockTile, KeyTile, WallTile,
							 WallTile,  BlankTile, BlankTile, RockTile, RockTile, RockTile, RockTile, WallTile, WallTile, WallTile, WallTile, WallTile, WallTile,
							 WallTile,  BlankTile, BlankTile, BlankTile, WaterTile, WallTile, WaterTile, WallTile, DiamondTile, DiamondTile, DiamondTile, DiamondTile, WallTile,
							 WallTile,  BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, KeyTile, WallTile,  BlankTile, BlankTile, BlankTile, SpiderTileLeft, WallTile,
							 WallTile,  WaterTile, WallTile,  WallTile,  WallTile,  WallTile,  WallTile, WallTile, DirtTile, DirtTile, DirtTile, DirtTile, WallTile,
							 WallTile,  BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, KeyholeTile,
							 WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile);
							 
	Room9: array[] of byte = (WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  
							 WallTile,  KeyTile,   DirtTile,  WaterTile, WaterTile, DirtTile, DirtTile, WaterTile, KeyTile, WaterTile, WaterTile, KeyTile, KeyholeTile,
							 WallTile,  RockTile,  DirtTile,  WaterTile, WaterTile, RockTile, DirtTile, WaterTile, DiamondTile, DiamondTile, WaterTile, WaterTile, WallTile,
							 WallTile,  WaterTile, WaterTile, WaterTile, WaterTile, DirtTile, DirtTile, WaterTile, WaterTile, WaterTile, WaterTile, RockTile, WallTile,
							 WallTile,  DiamondTile, WaterTile, WaterTile, WaterTile, RockTile, DirtTile, WaterTile, WaterTile, WaterTile, WaterTile, WaterTile, WallTile,
							 WallTile,  DiamondTile, WaterTile, WaterTile, WaterTile, KeyTile, PlayerTile, WaterTile, RockTile, RockTile, RockTile, RockTile, WallTile,
							 WallTile,  DirtTile, WaterTile, WaterTile, WaterTile, WaterTile, DirtTile, DirtTile, DirtTile, DirtTile, DirtTile, DirtTile, WallTile,
							 WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile);
							 
	Room10: array[] of byte =(WallTile,  BlankTile, BlankTile, WallTile, KeyTile, WallTile, KeyTile, WallTile, KeyTile, WallTile, KeyTile, WallTile, WallTile,  
							 WallTile,  BlankTile, BlankTile, BlankTile,  BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, WallTile,
							 WallTile,  BlankTile, RockTile, BlankTile,  BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, RockTile, BlankTile, BlankTile, WallTile,
							 WallTile,  BlankTile, RockTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, WallTile,
							 WallTile,  BlankTile, BlankTile, WallTile, SpikesHeadTile, WallTile, SpikesHeadTile, WallTile, SpikesHeadTile, WallTile, SpikesHeadTile, DirtTile, WallTile,
							 WallTile,  BlankTile, RockTile, WallTile, SnakeTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, DiamondTile, WallTile,
							 WallTile,  PlayerTile, BlankTile, WallTile, SpiderTileRight, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, DiamondTile, KeyholeTile,
							 WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile);
							 
	Room11: array[] of byte =(WallTile,  KeyholeTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  
							 WallTile,  PlayerTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, DiamondTile, DiamondTile, WallTile,
							 WallTile,  BlankTile, BlankTile, BrickTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BrickTile, DiamondTile, WallTile,
							 WallTile,  BrickTile, BlankTile, BlankTile, SpikesHeadTile, SpikesHeadTile, SpikesHeadTile, SpikesHeadTile, SpikesHeadTile, SpikesHeadTile, BrickTile, DiamondTile, WallTile,
							 WallTile,  BlankTile, BrickTile, BlankTile, BlankTile, BlankTile, BrickTile, KeyTile, DiamondTile, DiamondTile, DiamondTile, DiamondTile, WallTile,
							 WallTile,  BlankTile, BlankTile, BrickTile, BrickTile, DirtTile, BrickTile,  BrickTile, BrickTile, BrickTile, BrickTile, DiamondTile, WallTile,
							 WallTile,  SnakeTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, SpiderTileLeft, BlankTile, BlankTile, BlankTile, BlankTile, WallTile,
							 WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile);

	Room12: array[] of byte =(WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  
							 WallTile,  KeyTile, KeyTile, BrickTile, KeyTile, BlankTile, SnakeTile, BlankTile, KeyTile, BrickTile, DiamondTile, KeyTile, KeyholeTile,
							 WallTile,  BlankTile, BlankTile, BrickTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BrickTile, RockTile, DiamondTile, WallTile,
							 WallTile,  BlankTile, KeyTile, BrickTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BrickTile, DirtTile, DirtTile, WallTile,
							 WallTile,  RockTile, RockTile, BrickTile, SnakeTile, BlankTile, BlankTile, BlankTile, SnakeTile, BrickTile, RockTile, RockTile, WallTile,
							 WallTile,  DirtTile, DirtTile, RockTile, BlankTile, RockTile, BlankTile, RockTile, BlankTile, BlankTile, BlankTile, PlayerTile, WallTile,
							 WallTile,  BlankTile, SpiderTileLeft, BlankTile, DirtTile, BlankTile, BlankTile, BlankTile, BlankTile, BrickTile, BrickTile, BrickTile, WallTile,
							 WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile);

	Room13: array[] of byte =(WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  
							 WallTile,  BlankTile, BlankTile, BlankTile, SpiderTileLeft, BlankTile, BlankTile, PlayerTile, BlankTile, BlankTile, BlankTile, BlankTile, KeyholeTile,
							 WallTile,  BlankTile, BrickTile, BrickTile, BrickTile, BlankTile, DiamondTile, DiamondTile, DiamondTile, BlankTile, BrickTile, BrickTile, WallTile,
							 WallTile,  BlankTile, BlankTile, SpiderTileLeft, BlankTile, BlankTile, DiamondTile, DiamondTile, DiamondTile, BlankTile, DirtTile, BlankTile, WallTile,
							 WallTile,  BlankTile, BrickTile, BrickTile, BrickTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BrickTile, BlankTile, WallTile,
							 WallTile,  BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BrickTile, BrickTile, BrickTile, BrickTile, BrickTile, BlankTile, WallTile,
							 WallTile,  KeyTile,   BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, WallTile,
							 WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile);

	Room14: array[] of byte =(WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  
							 PlayerTile,  BlankTile, BlankTile, SpiderTileLeft, BlankTile, BlankTile, BlankTile, BlankTile, BrickTile, KeyTile, KeyTile, KeyTile, WallTile,
							 WallTile,  BlankTile, BlankTile, BrickTile, BlankTile, DiamondTile, DiamondTile, BlankTile, BrickTile, BlankTile, SpiderTileLeft, BlankTile, WallTile,
							 WallTile,  BlankTile, BlankTile, BrickTile, BlankTile, DiamondTile, DiamondTile, BlankTile, BrickTile, BlankTile, BrickTile, BlankTile, WallTile,
							 WallTile,  BlankTile, BlankTile, BrickTile, BlankTile, DiamondTile, DiamondTile, BlankTile, BrickTile, BlankTile, BrickTile, BlankTile, WallTile,
							 WallTile,  BlankTile, BlankTile, BrickTile, BlankTile, DiamondTile, DiamondTile, BlankTile, BrickTile, BlankTile, BrickTile, BlankTile, KeyholeTile,
							 WallTile,  BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, WallTile,
							 WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile);

	Room15: array[] of byte =(WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  
							 WallTile,  PlayerTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, WallTile,
							 WallTile,  BrickTile, WaterTile, RockTile, BrickTile, BrickTile, BrickTile, BrickTile, BrickTile, BrickTile, RockTile, BlankTile, WallTile,
							 WallTile,  BlankTile, WaterTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, RockTile, BlankTile, BlankTile, BlankTile, KeyholeTile,
							 WallTile,  BlankTile, WaterTile, BlankTile, BlankTile, BlankTile, BrickTile, BrickTile, BrickTile, BrickTile, BrickTile, BrickTile, WallTile,
							 WallTile,  BlankTile, WaterTile, WaterTile, WaterTile, WaterTile, WaterTile, PipeTile, PipeTile, PipeTile+5, KeyTile, KeyTile, WallTile,
 							 WallTile,  SpiderTileLeft, DirtTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, PipeTile+1, KeyTile, KeyTile, WallTile,
							 WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  PipeTile+1,  WallTile,  WallTile,  WallTile);

	Room16: array[] of byte =(WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  
							 WallTile,  BlankTile, SpiderTileLeft, WallTile, BlankTile, SpiderTileLeft, WallTile, BlankTile, SpiderTileLeft, WallTile, BlankTile, SpiderTileLeft, WallTile,
							 WallTile,  KeyTile, BlankTile, WallTile, KeyTile, BlankTile, WallTile, KeyTile, BlankTile, WallTile, KeyTile, BlankTile, WallTile,
							 WallTile,  WallTile,  BlankTile,  WallTile, WallTile,   BlankTile, WallTile,  WallTile, BlankTile, WallTile, WallTile, BlankTile, WallTile,
							 WallTile,  BlankTile, RockTile, BlankTile, BlankTile, RockTile, BlankTile, BlankTile, RockTile, BlankTile, BlankTile, RockTile, WallTile,
							 WallTile,  BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, WallTile,
							 WallTile,  PlayerTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, KeyholeTile,
							 WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile);

	Room17: array[] of byte =(WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  
							 WallTile,  BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, WallTile,
							 WallTile,  BlankTile, KeyTile, BlankTile, SpiderTileLeft, KeyTile, BlankTile, BlankTile,  BlankTile, BlankTile, BlankTile, BlankTile, WallTile,
							 WallTile,  BlankTile, BlankTile, RockTile, RockTile, BlankTile, BlankTile, KeyTile, BlankTile, SpiderTileLeft, KeyTile, BlankTile, WallTile,
							 WallTile,  BlankTile, KeyTile, BlankTile, BlankTile, KeyTile, BlankTile, BlankTile, RockTile, RockTile, BlankTile, BlankTile, WallTile,
							 WallTile,  BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, KeyTile, BlankTile, BlankTile, KeyTile, BlankTile, WallTile,
							 PlayerTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, WallTile,
							 WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  KeyholeTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile);

	Room18: array[] of byte =(WaterTile,  WaterTile, WaterTile, WaterTile, WaterTile, WaterTile, WaterTile, WaterTile, WaterTile, WaterTile, WaterTile, WaterTile, WaterTile,  
							 WaterTile, PlayerTile, RockTile, BlankTile, BlankTile,  WaterTile, WaterTile, BlankTile, BlankTile, BlankTile, KeyTile, WaterTile, WaterTile,
							 WaterTile,   RockTile,  RockTile, WaterTile, WaterTile, RockTile, WaterTile,   RockTile, WaterTile, WaterTile, WaterTile, BlankTile, WaterTile,
							 WaterTile,   RockTile, BlankTile, RockTile, BlankTile, WaterTile, WaterTile, BlankTile, BlankTile, BlankTile, SpiderTileLeft, WaterTile, WaterTile,
							 WaterTile,SpiderTileLeft,KeyTile, WaterTile, RockTile,  BlankTile, WaterTile, BlankTile, WaterTile, WaterTile, WaterTile, BlankTile, KeyholeTile,
							 WaterTile,  BlankTile, WaterTile, WaterTile, WaterTile, KeyTile, WaterTile,    KeyTile, WaterTile, WaterTile, WaterTile, RockTile, WaterTile,
							 WaterTile,  KeyTile,    RockTile, KeyTile,   DirtTile,  WaterTile, WaterTile, RockTile, BlankTile, BlankTile, SpiderTileLeft, DirtTile, WaterTile,
							 WaterTile,  WaterTile, WaterTile, WaterTile, WaterTile, WaterTile, WaterTile, WaterTile, WaterTile, WaterTile, WaterTile, WaterTile, WaterTile);

	Room19: array[] of byte =(WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  KeyholeTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  
							 WallTile,BlankTile,SpiderTileRight,BlankTile, BlankTile, BlankTile, WallTile, BlankTile, WallTile, BlankTile, BlankTile, KeyTile, WallTile,
							 WallTile,  BlankTile, RockTile, BlankTile, RockTile,  BlankTile,  WallTile,  BlankTile,  WallTile, SnakeTile, BlankTile, BlankTile, WallTile,
							 WallTile,  BlankTile, BlankTile, BlankTile, BlankTile, PipeTile+4, WaterTile, BlankTile, WallTile, WaterTile, RockTile, WaterTile, WallTile,
							 WallTile,  BlankTile, BlankTile, BlankTile, RockTile, PipeTile+3, WaterTile, BlankTile, RockTile, BlankTile, BlankTile, BlankTile, WallTile,
							 WallTile, DiamondTile, BlankTile, RockTile, BlankTile,DiamondTile, WallTile, BlankTile, RockTile, BlankTile, RockTile, BlankTile, WallTile,
							 WallTile, PlayerTile,DiamondTile, BlankTile,DiamondTile, BlankTile, WallTile, SnakeTile, BlankTile, BlankTile, BlankTile, KeyTile, WallTile,
							 WallTile, WaterTile, WaterTile, WaterTile, WaterTile, WaterTile, PipeTile+5,  WallTile,  WallTile,  SpikesHeadTile,  WallTile,  WallTile,  WallTile);

	Room20: array[] of byte =(WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  WallTile,  
							 WallTile,  BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, PlayerTile, WallTile,
							 WallTile,  BlankTile, RockTile, DirtTile, BrickTile, BrickTile, BrickTile, BrickTile, BrickTile, BrickTile, BrickTile, DiamondTile, WallTile,
							 WallTile,  KeyTile, BlankTile, BlankTile, BrickTile, SnakeTile, DiamondTile, DiamondTile, DiamondTile, KeyTile, BrickTile, DiamondTile, WallTile,
							 WallTile,  BlankTile, BlankTile, BlankTile, BrickTile, DiamondTile, DiamondTile, DiamondTile, DiamondTile, SnakeTile, BrickTile, DiamondTile, WallTile,
							 WallTile,  BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, KeyTile, BrickTile, BrickTile, BrickTile, BlankTile, WallTile,
							 WallTile,  BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BlankTile, BrickTile, BlankTile, SpiderTileLeft, BlankTile, WallTile,
							 WallTile,SpikesHeadTile,WallTile,SpikesHeadTile,WallTile,WallTile,WallTile,WallTile,WallTile,KeyholeTile,  WallTile,  WallTile,  WallTile);