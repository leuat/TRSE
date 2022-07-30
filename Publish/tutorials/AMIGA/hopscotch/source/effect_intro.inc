var
	xx1,xx2 : integer=0;
/*	const atari_height:integer = 150;
	const atari_stop:integer = 60;
	
	atari_right : array[atari_height+10] of integer = buildtable("-Math.pow(i/20.0,2.0)");
	atari_width1 : array[atari_height+10] of integer = buildtable("2*Math.exp(i*0.015)");
*/


	
procedure toggle();
begin
/*	if (x1=0) then
	begin
		for i:=0 to 16 do 
		begin
			j:=curColors[i];
			k:=(j&$F)>>2;
			k|=(j&$F0)>>2;
			k|=(j&$F00);
//			k:=0;
			curColors[i+16]:=k;
		end;	
		
	end;
	if (x1=1) then
	begin
		for i:=0 to 16 do 
		begin
			j:=curColors[i];
			curColors[i+16]:=j;
		end;	
		
	end;
	setpalette(#curColors,#copper_palette,31);
	*/
	fill(#tarColors,0,32/4*2);
	if (x1=0) then
	begin
		memcpy(#palette, 0, #tarColors, 0, 8,4);
	end;
	if (x1=1) then
	begin
		memcpy(#palette, 0, #tarColors, 32, 8,4);
	end;
	if (x1=2) then
	begin
		memcpy(#palette, 0, #tarColors, 0, 8,4);
		memcpy(#palette, 0, #tarColors, 32, 8,4);
	end;
//	memcpy(#palette, 0, #tarColors, 0, 8,4);
		
	for i:=0 to 16 do
	begin
		waitverticalblank();
		FadeTo(32);
	end;
	
	x1+=1;
	
end;	
	

procedure InitIntro();
begin
	ptr:=#scr;
	fill(#curColors,0,32/4);
	setpalette(#curColors,#copper_palette,31);
	
	Graphics::SetupNonInterlacedScreen(ptr,5);
	
	Compression::Decompress(#image, #scr);
//	Compression::Decompress(#logo, #image);
	Compression::Decompress(#text_logo, bpl5);
	
	
//	memcpy(#palette, 0, #curColors, 0, 16,4);
	x1:=0;
//	toggle();

//	j:=$5000 | $200;
//	poke16(#copper_resolution, 0 , j); // $4000  = bitplanes, $200 = colours
	SetCopperList32(bpl5+320, #copper_bitplane4);



	memcpy(#palette, 0, #tarColors, 32, 8,4);
//	memcpy(#palette, 0, #tarColors, 0, 8,4);
		
	for i:=0 to 16 do
	begin
		waitverticalblank();
		FadeTo(32);
	end;

	
end;

procedure InitCopperListIntro();
begin
	
	row:=7;
	j:=time;
	l:=0;
	y:=$000F;// Color background start
	InitCustomCopperList(); // Message start custom copper list
	AddCopperCommand($0180, 0); // First row black
	for i:=0 to 128 do begin
		// First, copper should wait until raster "row" has been reached
		AddCopperCommand(row, COPPER_WAIT_FOR_RASTER);
		// Next, copper should scroll
//		AddCopperCommand(COPPER_SCROLL, k);
	 	// First color of palette (start ) = background, next is +2, +4 etc
		AddCopperCommand(COPPER_PALETTE_START+4*2, y); 
	 		 	

  /*		k:=sin256[j&63]/16;
  		m:=sin256[(j+32)&63]/16;
  		k:=k;
  		*/
		//k = 4 lower bit sine function     + 4 upper bit sine function (opposite phase)  		

  	 	row:=row+$200;  	 // Wait until the next row
     	j:=j+2;            // Increase sine wobble
     	

		// The next part just decreases the value of the background color "y" 
		// for each 5th line until 0(black) and not lower
       	l:=l+1;            
       	if (l=5) then begin 
       		if (y<>0) then 
	       		y:=y-1;
       		l:=0;
       	end;

	end;
	// Message end of copper list
	 		 	

	EndCustomCopperList();

end;


interrupt VBlankIntro();
begin
	DemoUnit::Update();	
//	InitCopperListIntro();
	ApplyCopperList();
	if (DemoUnit::time&63=0) then toggle();


	
end;