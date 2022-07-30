var
	
	imgy:integer = 0;
	// current height
	const imgS : byte = 8;
	// total height
	const imgN : byte = 31;
	imgp : array[imgN] of integer = buildtable("0");
	imgScroll : array[imgN] of integer = buildtable("0");
	imgPrev : array[imgN] of integer = buildtable("5000");
	imgColor : array[imgN] of integer = buildtable("0");

var
	colors1 : array[64] of integer=buildtable("i/4 | (i/6)<<4  | (i/7.2)<<8"); 
	colors2 : array[64] of integer=buildtable("i/8 | (i/6)<<4  | (i/4.2)<<8"); 
	colors3 : array[64] of integer=buildtable("i/4.2 | (i/7)<<4  | (i/5)<<8"); 
//	colors4 : array[64] of integer=buildtable("(63-i)/4.2 | ((63-i)/8)<<4  | ((63-i)/6)<<8"); 
@donotremove colors4
	

procedure InitImageShowerCopperList();
begin
	InitCustomCopperList(); // Message start custom copper list
	AddCopperCommand(7, COPPER_WAIT_FOR_RASTER);
	AddCopperCommand($0180, 000); // First row black
	AddCopperCommand(COPPER_PALETTE_START+2, $0F0); 

	row:=7+$800+$800*5;
	j:=0;
//	addCopperCommand(COPPER_PALETTE_START+2, 0); 
//		A
	for i:=0 to imgN do begin
		// First, copper should wait until raster "row" has been reached
		if (i=26) then begin
			AddCopperCommand($FFDF, COPPER_WAIT_FOR_RASTER);
			row:=$007;
			j+=$800;
//			continue;
		end
		else
		AddCopperCommand(row, COPPER_WAIT_FOR_RASTER);
		// Next, copper should scroll
		AddCopperCommand(COPPER_PALETTE_START+2, colors[imgColor[i]]); 
//		AddCopperCommand(COPPER_PALETTE_START, imgColor[i]); 
		AddCopperCommand(COPPER_SCROLL, imgScroll[i]<<dx);
	 	// First color of palette (start ) = background, next is +2, +4 etc
//		AddCopperCommand(COPPER_PALETTE_START, y); 
	 	j+=1;


  	 	row:=row+$200*4;  	 // Wait until the next row


	end;
		AddCopperCommand(COPPER_PALETTE_START+2, 0); 
//	// Message end of copper list
	EndCustomCopperList();

end;





procedure UpdateMoveImage();
begin
	for i:=0 to imgN do
	begin
		imgp[i]+=4;
		k:=imgp[i];
		if (k>256 and k<256+256) then
		begin
//			if time&3=0 then
			if (imgColor[i]<>63) then imgColor[i]+=1;
			k:=p2[k-256];
			imgScroll[i]:=(k&15);
		end;		
	end;
	
end;

procedure MoveImage();
begin
	j:=imgy;
//	p1:=bpl5+40*249;// + tab40[imgy];
//	dst:=#scr+40*3+40*249;// + tab40[imgy];
	p1:=bpl5;// + tab40[imgy];
	dst:=#scr+40*4+lshift;// + tab40[imgy];
	for tf:=0 to imgN do
	begin
		i:=tf;//imgN-tf;		
		k:=imgp[i]-256;
//		k:=128;
		if (k<256 and k>0) then
		begin
			k:=p2[k];
			l:=k>>4;
			
			if (l<20 and imgPrev[i]<>l) then
			begin
				imgPrev[i]:=l;
				m:=19-l;
//				if m>5 then m:=5;
				l:=l<<1;
				asm("
					move.w #7,d5
;					moveq #0,d6
					move.w l,d6
					move.l p1,a4
					move.l dst,a5
					
					add.w d6,a5
ltopp1:
					move.w m,d4
ltopp2:
					move.w (a4)+,(a5)+
					dbf d4,ltopp2
					
					add.w d6,a4
					add.w d6,a5
					
					dbf d5,ltopp1								
					
;					sub.w d6,a5
					
				");				
				end;			
		end;
		p1+=40*8;
		dst+=40*8;
		
				
				
	end;
	
end;


interrupt ImageShowerVBlank();
begin
	InitImageShowerCopperList();
//	for i:=0 to 80 do for j:=0 to 10 do k:=1;
	MoveImage();
//	waitverticalblank();
	UpdateMoveImage();		
	DemoUnit::Update();	
	time+=1;
end;

procedure SetupEmptyScreen();
begin
	ptr:=#scr;
	fill(ptr,0,256*10*4);	
	fill(bpl5,0,256*10);	
	Graphics::SetupNonInterlacedScreen(ptr,4);
	
end;

procedure InitImageShower(src,p2:global pointer;lshift:global long;dx,addx:global integer);
begin
	imgy:=0;
	ResetCopperList();
	if (addx=0) then
	for i:=0 to imgN do
	begin
		imgp[i]:=imgN*8 - i*8;
	end;
	if (addx=1) then
	for i:=0 to imgN do
	begin
		imgp[i]:=i*8;
	end;
	for i:=0 to 16 do
		curColors[i]:=0;
		
	curColors[0]:=0;
	curColors[2]:=$FFF;
	curColors[3]:=0;
	
	y1:=30;
	y2:=20;
	y3:=0;
	setpalette(#curColors,#copper_palette,16);
	ApplyCopperList();
	Compression_Decompress(src,bpl5);
//	Compression_Decompress(src,#scr);
		
	
end;