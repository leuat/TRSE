


procedure InitCopperListTorus();
@define RROW 105
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


procedure MoveProximaStuff();
begin
	asm("
	move.l bpl5,a0
	add.l #40*255+40*80,a0
	
	move.l a0,a1
	sub.l #80,a1
	move.l $F0F0F0F0,d0	
	rept 128
	move.l (a1)+,(a0)+
	move.w (a1)+,(a0)+
	move.b (a1)+,(a0)+
	sub.l #40+7,a0
	sub.l #40+7,a1
	
	endr
	
	");	
end;


procedure InitTorus();
begin
	fill(#palette,0,32/4);
	setpalette(#palette,#copper_palette,31);
	applycopperlist();
	ptr:=#scr;
	//ptr:=ptr+40*10;
	fill(ptr+40*256*4,0,10*256);
	bpl5:=ptr+40*256*5;
	Graphics::SetupNonInterlacedScreen(ptr,5);
//	Graphics::SetupDefaultScreen(ptr,5);
	yy:=0;
	x1:=$248;
//	fill(ptr,%101010101010,40*256*3);
	Compression::Decompress(#torusimage, ptr);
	
	Compression::Decompress(#prxlogo, bpl5+40*80);
//	memcpy(#torusimage,0, ptr,0, 40*256*4,4);	


//	setpalette(#image_palette,#copper_palette,16);
	p1:=#palette;
	k:=2;
	for i:=0 to 16 do
	begin
		j:=$FFF;
		p1[i+16]:=j;

	end;
//	InitCopperListTorus();	
end;		

procedure UpdateTorus();
begin
	p1:=#palette;
	p2:=#curColors;

	for i:=0 to 16 do
	begin
		l:=i;
/*		if (DemoUnit::time>128) then
			if ((DemoUnit::time>>5)&1=0) then l+=16;		
			
			*/
		j:=p2[l];
//		if (k<>0) then
		if (DemoUnit::time<DemoUnit::maxTime-16) then 
		begin
			
		k:=mod(i+DemoUnit::time,15);
//	
//		if (DemoUnit::time&31=0) then 
		if (DemoUnit::time>128) then
		begin
			if (k=13) then j:=C_MAIN;

/*		if ((DemoUnit::time)&47=0) then 
		if (DemoUnit::time>256) then
			if (i=2) then j:=$9AF;
*/
/*		if ((DemoUnit::time+8)&15=0) then 
		if (DemoUnit::time>256) then
			if (i=8) then j:=$F3F;
*/
		end
		else 
		begin 
		k:=mod(i-DemoUnit::time,15);
//		if (k<>0) then
//		if (j=0) then
		if (DemoUnit::time>0) then
			if (k=11) then j:=x1;
		end;
		//if (i=12 and (DemoUnit::time+16)&63=0) then j:=$7E5;
//		if (i=1 and (DemoUnit::time+38)&53=0) then j:=$F;
			p2[l]:=j;
		end;
		
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
//		if (i<16) then 
		begin
//			if (DemoUnit::time&1=1) then
			begin
				DemoUnit::FadeToColor(j,$000);
				asm("
					move.w d0,j
				");
				DemoUnit::FadeToColor(j,$000);
				asm("
					move.w d0,j
				");
			end;
			p2[i]:=j;
		end;
/*		else
		begin
//			if (DemoUnit::time&1=1) then
			begin
				DemoUnit::FadeToColor(j,$FFF);
				asm("
					move.w d0,j
				");
				DemoUnit::FadeToColor(j,$FFF);
				asm("
					move.w d0,j
				");
			end;
			p2[i]:=j;
		end;*/

		
//		if (k<>0) then
			p1[k]:=j;
	end;
	p1[0]:=0;
	setpalette(#palette,#copper_palette,31);
//	FadeTo(16);
//	WaitVerticalBlank();	
//	LSPlayer::Play();
	InitCopperListTorus();
	ApplyCopperList();
	SetCopperList32(bpl5+(sin256[yy]*40)-168*40, #copper_bitplane4);
//	SetCopperList32(bpl5+(sin256[yy]*40)-176*40, #copper_bitplane4);
	if (DemoUnit::time>240) then MoveProximaStuff();
	if (DemoUnit::time>DemoUnit::maxTime-128) then 
	begin
		if (yy<>0) then yy-=1;
//		if (x1<>0) then x1-=1;
	end
	else
	begin
		DemoUnit::FadeToColor(x1,$58A);
		asm("
			move.w d0,x1
		");
		
		if (DemoUnit::time>64) then if (yy<64) then yy+=1;

	end;
	
end;

interrupt VBlankTorus();
begin
	//PlayP61Module();
//	setpalette(#curColors,#copper_palette,31);
	UpdateTorus();
	DemoUnit::Update();
//	InitCopperList();
//	if (DemoUnit::time&3=0) then FadeIn();


end;
