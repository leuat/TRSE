
var
	const clear1_width:integer = 4;
	const clear1_height:integer = 32;
	const clear1_frames:integer = 24;

procedure ClearSingleBitplaneCopperList();
begin
	
	row:=7+$800+$800*5;
	InitCustomCopperList(); // Message start custom copper list
	AddCopperCommand($0180, 0); // First row black
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
		AddCopperCommand(COPPER_PALETTE_START+y, imgColor[i]); 
		AddCopperCommand(COPPER_PALETTE_START, imgPrev[i]); 
//		AddCopperCommand(COPPER_PALETTE_START, imgColor[i]); 
		
		

  	 	row:=row+$200*4;  	 // Wait until the next row
		

	end;
	// Message end of copper list
	EndCustomCopperList();

end;



interrupt ClearSingleBitplaneVBlank();
begin
	ClearSingleBitplaneCopperList();
	ApplyCopperList();	
	DemoUnit::Update();	
	k:=time;
	for it:=0 to imgN do
	begin
		if (addx=0) then i:=it;
		if (addx=1) then i:=imgN-1-it;
		if (it<k) then
		begin
			DemoUnit::FadeToColor(imgColor[i],dy);
		 	asm("
		 	move.w d0, j
		 	");
			imgColor[i]:=j;
			
			DemoUnit::FadeToColor(imgPrev[i],dy);
		 	asm("
		 	move.w d0, j
		 	");
			imgPrev[i]:=j;
			
			
		end;
	end;
	time+=1;
end;


procedure InitClearSingleBitplane(y,dx,dy,l,addx:global integer);
begin
	for i:=0 to imgN do 
	begin
		imgColor[i]:=dx;
		imgPrev[i]:=l;
	end;
	k:=0;
	time:=0;
	vbirq(ClearSingleBitplaneVBlank());
	
end;



procedure Transition1(color1, color2 : integer);
begin
	vbirq(VBlankEmpty());
	for i:=0 to 16 do
		curColors[i]:=color1;
		
	curColors[1]:=color2;
	setpalette(#curColors,#copper_palette,16);
	ptr:=#scr;
	Graphics::SetupNonInterlacedScreen(ptr,1);
	waitverticalblank();
	ApplyCopperList();
	
	for i:=0 to 52 do begin
		p1:=#scr;
		y:=0;
		waitverticalblank();
		for y1 := 0 to (256/clear1_height) do
		begin
			for j:=0 to 40/clear1_width do
			begin
				k:=j+y1+i-30;
				if (k>0 and k<clear1_frames) then
				begin
					p2:=#clear1 + k*(clear1_width*clear1_height);
					p3:=p1;
					
					asm("
						move.l #31,d3
						move.l p2,a0
						move.l p3,a1
bb1:
						move.l (a0)+,(a1)+
						add.l #36,a1
						dbf d3,bb1
					
					");
					
/*					for l:=0 to clear1_height do
					begin
						memcpyunroll(p2,0,p3,0,4,clear1_width/4);
						p2+=clear1_width;
						p3+=40;
					end;
*/					
				end;
					p1+=clear1_width;
			end;
			p1+=40*(clear1_height-1);
		end;
	end;
	
	for i:=0 to 16 do
		curColors[i]:=$FFF;
		
	curColors[0]:=0;
		
	setpalette(#curColors,#copper_palette,16);

	ApplyCopperList();
	waitverticalblank();
	fill(#scr,$FFFFFFFF,10*256);
		
	
end;
