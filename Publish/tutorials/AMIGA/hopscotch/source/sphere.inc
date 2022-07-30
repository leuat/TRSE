


procedure InitCopperListSphere();
begin
	if (DemoUnit::time>=DemoUnit::maxTime) then
		return;
	

	row:=7+$200*21;
	j:=DemoUnit::time;
	l:=0;
	y:=$000F;// Color background start
	InitCustomCopperList(); // Message start custom copper list
//	AddCopperCommand($0180, 0); // First row black
	p1:=ptr;
	shift:=0;
	k:=DemoUnit::time*2;
//	p2:=ptr+40*256*6;
//	fill(p2,$FFFFFFFF,40/4);
//	for i:=0 to 140 do begin
/*	DemoUnit::FadeToColor(x2,0);
	asm("
		move.w d0,x2
	");
	if ((DemoUnit::time+16)&63=0) then x2:=$78F;
	
			AddCopperCommand(COPPER_PALETTE_START+4*2,x2); 
			*/

			
			
	for i:=0 to @RROW do begin
		// First, copper should wait until raster "row" has been reached
		j:=tab_i_scale[i] + sin256[k&255]>>3+shift;
		p1:=ptr+tab160[j&255];
//		if (i<>@RROW) then
		AddCopperCommand(row, COPPER_WAIT_FOR_RASTER);
//		if (i<=@RROW) then
//		else	
//			AddCopperCommand(COPPER_PALETTE_START,i<<4); 
/*		if (j>220) then 
		begin	
			p1:=p2; // black
			Graphics::AddCopperCommandBitplane(0, p2);
			Graphics::AddCopperCommandBitplane(1, p2);
			Graphics::AddCopperCommandBitplane(2, p2);
			Graphics::AddCopperCommandBitplane(3, p2);
		end*/
//		else
		if (i<>0) then
		begin
			Graphics::AddCopperCommandBitplane(0, p1);
			p1+=40*256;
			Graphics::AddCopperCommandBitplane(1, p1);
			p1+=40*256;
			Graphics::AddCopperCommandBitplane(2, p1);
			p1+=40*256;
			Graphics::AddCopperCommandBitplane(3, p1);	
		end;
  	 	row:=row+$200;  	 // Wait until the next row
		
/*		if (i=@RROW) then 
		begin
			//AddCopperCommand(COPPER_PALETTE_START,$0F0); 
			//AddCopperCommand($F4CF, COPPER_WAIT_FOR_RASTER);
			//AddCopperCommand($0BDF, COPPER_WAIT_FOR_RASTER);
			//AddCopperCommand(COPPER_PALETTE_START,$F0F); 
			
//			AddCopperCommand(COPPER_PALETTE_START,$F0F); 
//	AddCopperCommand($0BDF, COPPER_WAIT_FOR_RASTER);
			@define SHH 10
			row-=$200*@SHH;
			shift:=-@SHH;
			k-=@SHH;
		end;
//		if (i>=@RROW) then
//			AddCopperCommand(COPPER_PALETTE_START,i);
		*/
		k+=2;     	
	end;
	// Message end of copper list
	EndCustomCopperList();

end;


procedure InitSphere();
begin
	ptr:=#scr;
	//ptr:=ptr+40*10;
	fill(ptr+40*256*4,0,10*256);
	bpl5:=ptr+40*256*5;
	Graphics::SetupNonInterlacedScreen(ptr,5);
//	Graphics::SetupDefaultScreen(ptr,5);
	fill(#palette,0,32/4);
	setpalette(#palette,#copper_palette,31);
	applycopperlist();
	yy:=0;
	x1:=0;
//	fill(ptr,%101010101010,40*256*3);
	Compression::Decompress(#sphereimage, ptr);
/*	for i:=0 to 4 do
	begin
		j:=4-i;		
		p1:=ptr+(40*256*(j-1));
		p2:=ptr+(40*300*j);
		memcpy(p1,0,p2,0,10*256,4);
		fill((p2+40*256),0,10*44);
		
	end;
	ptr+= 40*300;	


	bpl5:=ptr+40*300*4;*/
//	SetCopperList32(bpl5-(160*40), #copper_bitplane4);
	fill(bpl5-40*100,0,10*256*3);
	Compression::Decompress(#greets, bpl5+40*80);
//	memcpy(#torusimage,0, ptr,0, 40*256*4,4);	
	y2:=0;

//	setpalette(#image_palette,#copper_palette,16);
	p1:=#palette;
	k:=2;
	for i:=0 to 16 do
	begin
		j:=$FFF;
		p1[i+16]:=j;
//		k:=(i)/4+11;
//		j:=k + (k)<<4 + (k)<<8;
//		p1[i+16]:=j;

	end;
//	InitCopperListTorus();	
end;		

procedure UpdateSphere();
begin
	p1:=#palette;
	p2:=#curColors;

	for i:=0 to 16 do
	begin
		j:=p2[i];
//		if (k<>0) then
		if (DemoUnit::time<DemoUnit::maxTime-16) then 
		begin

			
			k:=mod(i+DemoUnit::time,15);
	//		if (k<>0) then
			l:=(DemoUnit::time>>2)&15;
			if (l<6) then
			begin
				if (DemoUnit::time>0) then
					if (k=11) then j:=x1 + (x1/4)<<4 + (x1/2)<<8;
			end
			else
			begin
				k:=mod(i-DemoUnit::time,15);
					
				if (DemoUnit::time>0) then
					if (k=4) then j:=(x1/2) + (x1/2)<<4 + (x1)<<8;
			end;
		end;
			
		//if (i=12 and (DemoUnit::time+16)&63=0) then j:=$7E5;
//		if (i=1 and (DemoUnit::time+38)&53=0) then j:=$F;
		p2[i]:=j;
	end;
	for i:=0 to 16 do
	begin
//		if (DemoUnit::time&15=i) then j:=$FFF;
		k:=i;		
//		k:=(i+sin256[DemoUnit::time&255]>>8-DemoUnit::time)&15;
//		k:=i;		
//		if (DemoUnit::time&1=1) then 
//			k:=15-k;
			
		j:=p2[i];
	//	if (j<>0) then 
		begin
//			if (DemoUnit::time&1=1) then
			begin
				DemoUnit::FadeToColor(j,$000);
				asm("
					move.w d0,j
				");
/*				DemoUnit::FadeToColor(j,$000);
				asm("
					move.w d0,j
				");*/
			end;
			p2[i]:=j;
		end;
//		if (k<>0) then
			p1[k]:=j;
	end;
	p1[0]:=0;
	setpalette(#palette,#copper_palette,31);
	p1:=bpl5+y2*40;
	if (DemoUnit::time>64) then 
		y2+=2;
	p1:=p1-(200*40);
	SetCopperList32(p1, #copper_bitplane4);
	
	InitCopperListSphere();
	ApplyCopperList();
	if (DemoUnit::time>DemoUnit::maxTime-16) then 
	begin
//		if (yy<>0) then yy-=1;
		if (x1<>0) then x1-=1;
	end
	else
	begin
		if (x1<$9) then x1+=1;
//		if (DemoUnit::time>64) then if (yy<64) then yy+=1;

	end;
	
end;

interrupt VBlankSphere();
begin
	//PlayP61Module();
//	setpalette(#curColors,#copper_palette,31);
	UpdateSphere();
	DemoUnit::Update();
//	InitCopperList();
//	if (DemoUnit::time&3=0) then FadeIn();


end;
