/*
	This method is called when the player leaves the screen and a new room needs to be painted. 

*/
procedure UpdateLevel();
begin
	// For some reason rendering to screen seems to crash when performed right after a
	// vertical blank.. hence the wait... Jason you know anything about this?	
	Screen::Cls(1);
	Screen::WaitForVerticalBlank();
	for i:=0 to 200 do wait(190);
	// Renders the current level at location lx,ly. Height of each block is 8, width is 4 bytes
	level.RenderLevel(p1.lx,p1.ly, 4,8);
	// Print out the text
	Screen::SetPen(2);
	Font::SetFont(#Font::font1);
	Font::DrawTextAt(levelTxt[p1.lx+p1.ly<<2],2,64-8,true);
	// Copy the level to the backbuffer for sprite XORing
	memcpy(#Screen::screen_char_loc, #buf, 2048);
end;
/*

	Will move the player in the direction of dx,dy if not detecting a pixel in the background 
	buffer. 
*/
procedure VerifyMove(cx,cy,dx, dy : integer);
begin
	xx := (p1.x+cx*8+16*8)/(16);
	yy := (p1.y+cy*4+8*8)/(16);
	
	ok:=true;
	
	if (dx<>0) then
	for j:=0 to 4 step 2 do
	begin	
	//	if Screen::getPixel(#buf, xx,yy+j-4)<>background then ok:=false;
	end;
	if (dy<>0) then
	begin
		for j:=0 to 16 step 4 do
		begin	
			if Screen::getPixel(#buf, xx+j-16,yy)<>background then ok:=false;
//			Screen::PutPixel(xx+j-16,yy);
		end;
		if (not ok and ddy<30000) then p1.onGround:=true else p1.onGround:=false;
	end;
	
//	ok := true;
	
	if (ok) then
	begin
		p1.x += dx/2;
		p1.y += dy/2;
		
	end;
	
end;


/*
	Updates the player movement

*/
procedure UpdatePlayer();
begin
	xx := p1.x/16;
	yy := p1.y/16;
	
	// Draw the player
	if (not p1.onGround) then
		player_sprite4(0, #screen, xx,yy)
	else
	if (p1.moveRight) then
	begin
		if (p1.curSprite=0) then
			player_sprite0(0, #screen, xx,yy)
		else
			player_sprite1(0, #screen, xx,yy)
	end
	else
	begin
		if (p1.curSprite=0) then
			player_sprite2(0, #screen, xx,yy)
		else
			player_sprite3(0, #screen, xx,yy)
		
	end;
/*	Sprite::SetData(#sprites);
	Sprite::SetSize(4,16);
	Sprite::DrawAt(p1.x/16,p1.y/16,0,true);
*/
	// Get key
	i := Input::GetPressedKey();
	// Move left/right
	isMoving := false;
	if (i = Input::Key_D) then begin VerifyMove(16,16,24,0);p1.moveRight:=true; isMoving := true;   end;
	if (i = Input::Key_A) then begin VerifyMove(16,16,65536-24,0);p1.moveRight:=false; isMoving:=true; end;
	// Jump up
	if (i = Input::Key_W and p1.onGround) then 
	begin
		ddy:=65505;//VerifyMove(0,65536-24);
		Sound::Play(100,4);

	end;
	// Check jump/falling down
	ccy:=16;
	if (ddy>30000) then ccy:=65536-16;
	VerifyMove(16,ccy,0,ddy);
	
	
	// redraw map wen player is at the edge of the screen	
	xx := p1.x>>4;
	yy := p1.y>>4;
	
	if (xx>150) then if (p1.lx<>0) then begin p1.lx-=1; p1.x:=111*16; xx:=111; UpdateLevel(); end else begin p1.x:=2; xx:=2; end;
	if (xx>112) then if (p1.lx<level.sizex) then begin p1.lx+=1; p1.x:=1; xx:=1; UpdateLevel(); end else begin p1.x:=111*16;xx:=111;end;
	if (yy>70) then if (p1.lx<>0) then begin p1.ly-=1; p1.y:=55*16; yy:=55; UpdateLevel(); end else begin p1.y:=2; yy:=2; end;
	if (yy>64-8) then if (p1.ly<level.sizey) then begin p1.ly+=1; p1.y:=1; yy:=1; UpdateLevel(); end else begin p1.y:=55*16;yy:=55;end;
	if (not p1.onGround) then
	begin
		ddy+=1;
	end;

	
		
	// Update sprite counter
	if (isMoving) then
		p1.nextSprite+=1;
	if (p1.nextSprite = spriteSpeed) then
	begin
		p1.nextSprite:=0;
		p1.curSprite:=(p1.curSprite+1)&1;
		if (p1.onGround) then
		Sound::Play(1000,1);
	end;
		
end;
