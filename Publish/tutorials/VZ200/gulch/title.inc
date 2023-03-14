procedure ResetGame();
begin
	Tiles::SetTileLoc(#tiles);
	level.ReadHeader(#levels);
	Tiles::tile := #tiles;
	p1.lx := 0;
	p1.ly := 0;
	p1.x := 16*16;
	p1.y := 10;
	p1.moveRight := true;
	p1.curSprite := 0;
	UpdateLevel();	
end;


procedure TitleScreen();
begin
	Compression::Decompress(#title, #Screen::screen_char_loc);
	i:=0;
	j:=0;
	k:=0;
	TrtPlayer::Initialize(#introtune);
	while (i<>Input::Key_spc) do
	begin
		i := Input::GetPressedKey();
		if (k=1) then	
			TrtPlayer::Play()
		else 
		begin
			Sound::Play(sinx[j],1);
			if (j>128) then k:=1;
			j:=j+1;
		end;
		Screen::WaitForVerticalBlank();		
	end;
	
end;