procedure InitCopperListSlides();
begin
	
	row:=7;
	j:=DemoUnit::time;
	l:=0;
	y:=$000F;// Color background start
	InitCustomCopperList(); // Message start custom copper list
//	AddCopperCommand($0180, 0); // First row black
//	AddCopperCommand(row, COPPER_WAIT_FOR_RASTER);
	
	k:=(scrollx)&15	;
	k:=((15-k)<<4) | k;
	
/*	AddCopperCommand(copper_diwstrt,orgStart+16);
	AddCopperCommand(copper_diwstop,orgEnd-16);
	AddCopperCommand(COPPER_SCROLL, 0);
	AddCopperCommand($F4CF, COPPER_WAIT_FOR_RASTER);
//	AddCopperCommand(COPPER_PALETTE_START+2,$00F0); */

	AddCopperCommand(7, COPPER_WAIT_FOR_RASTER);
	//AddCopperCommand($0180, 000); // First row black
	AddCopperCommand(COPPER_SCROLL, k);
	 		 	

	EndCustomCopperList();

end;


procedure InitClearscreenSlides();
begin
	ptr:=#scr;
	p1:=#scr;
	p2:=#scr+256*40;
	
//	ptr:=#image;
//	ptr:=ptr+40*10;

	Graphics::SetupNonInterlacedScreen(ptr,2);
//	fill(ptr,0,10*256*2);

	curColors[0]:=C_BG;	
	curColors[1]:=C_MEDIUM1;	
	curColors[2]:=C_MEDIUM2;	
	curColors[3]:=C_HIGH1;	
	setpalette(#curColors,#copper_palette,16);

	h:=64;
	state:=0;
	dx:=0;
	scrollx:=0;
	dy:=0;
	x:=180;
	y:=80;
	
	setcopperlist32(#sprite_struct1, #copper_spritestruct0);
	p4:=#sprite_struct1;
	fill(p4,$FFFF,256);	
//	for j:=0 to 64 do p4[j]:=$FFFF;
	
	poke16(#copper_palette,2+1*16,$00);	
	poke16(#copper_palette,2+4*17,$0800);	
	poke16(#copper_palette,2+4*18,$0F16);	
	poke16(#copper_palette,2+4*19,$0FA2);	

end;

procedure RenderClearscreenSlides1Line(w:global integer);
begin
	ptr:=#scr+state*(40*256);
	dx:=sinW[(time2)&255];
//
	x:=dx+h;//mod(dx+h,320);

//	x:=0-dy;
	if (dy<255) then
		Graphics::HlineOR(x,dy,w+2,ptr);
		
	dy+=1;
	
	if (dy>=256) then 
	begin 
		dy:=0;
		h:=mod(h+w,320);
		time2+=128;
		state:=(state+1)&1;		
	end;
	time2+=1;
	
	
	
end;


interrupt VBlankClearscreenSlides();
begin
	InitCopperListSlides();
	ApplyCopperList();
	
	//PlayP61Module();
	setpalette(#curColors,#copper_palette,31);




//	WaitVerticalBlank();	
//	LSPlayer::Play();
	DemoUnit::Update();	
	for i:=0 to 64 do
		RenderClearscreenSlides1Line(16);
		
//	if (DemoUnit::time&3=0) then FadeIn();

	scrollx+=1;
	if (scrollx=80) then
	begin
		h+=8;
		
	end;
	if (scrollx=320) then
	begin
		p1+=40;
		p2-=40;
		scrollx:=0;
		loop();
	end;		
	if (scrollx&15=0) then
	begin
		p1-=2;
		p2+=2;
		
	end;
	SetCopperList32(p1, #copper_bitplane0);
	SetCopperList32(p2, #copper_bitplane1);
	bbb:=DemoUnit::time&127+64;
	bbb:=64;
//	UpdateSpritePos(#sprite_struct1, bbb ,0,0 , 2);	
	UpdateSpritePos(#sprite_struct1, bbb ,44,0, 2);	
//	$DFF142

end;

