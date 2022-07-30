procedure InitCopperListErings();
begin
	if (DemoUnit::time>=DemoUnit::maxTime) then
		return;
	
	i:=DemoUnit::time&15;
	ptr:=#src+(40*256*i);
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
//	if (i=2) then
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

interrupt CallErings();
begin
//	InitCopperListErings();
//	ApplyCopperList();	
	asm("
		move.l p2,a2
		jsr (a2)
	");
	DemoUnit::Update();
	
end;
