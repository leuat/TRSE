var
	trse_rot : incbin("data/rot_trse.bin") compressed;

interrupt VBlankPrecalc();

begin
	FadeTo(16);
	begin
		
		x1:=l3+50;
		if (l<>x1) then
		begin
		if (l3<>0) then
		for y1:=128 to 256 do
		begin
			Graphics_putpixel(bpl5,x1,y1);
		end;
		end;
		l := x1;
		//fill(bpl5+40*(128+x1)+8,$FFFFFFFF,l3>>5);
	end;
	curFrame:=((sin256[time&255]>>1)&31);
	begin
	src:=ptr+(40*256*4);
	src+=((16)*64)*curFrame;
	dst:=ptr+40*14+4;
	asm("
		move.l src,a0
		move.l dst,a1
		move.l dst,a2
		move.l dst,a3
		add.l #40*256*0,a1
		add.l #40*256*1,a2
		add.l #40*256*2,a3
		move.w #64,d3
.topp1:		
		move.l (a0),(a1)+
		move.l (a0)+,(a2)+
		move.l (a0),(a1)+
		move.l (a0)+,(a2)+

		move.l (a0)+,(a3)+
		move.l (a0)+,(a3)+
		
		add.l #(40-8),a1
		add.l #(40-8),a2
		add.l #(40-8),a3
		dbf d3,.topp1
	");
	if (time&1=0) then
	curFrame+=1;
	end;	
		
end;
procedure Precalc();
begin
	
	for i:=0 to 32 do tarColors[i]:=0;
	for i:=0 to 16 do
	begin
		FadeTo(32);
		waitverticalblank();
		applycopperlist();
	end;
	ptr:=#scr;
	ClearScreen();
	Graphics::SetupNonInterlacedScreen(ptr,4);
	Compression::Decompress(#octopussy, ptr);
	Compression::Decompress(#trse_rot, ptr+(40*256*4));
		
	fill(#curColors,0,8);
	memcpy(#octopussy_pal,0,#tarColors,0,8,4);	
	tarColors[2]:=$FFF;
	p1:=#tarColors;
	for i:=0 to 8 do
	begin
		j:=p1[i];
		p1[i+8]:=j;
		k:=j;
		k:=(j&$F)>>1;
		k|=(((j>>4)&$F)>>2)<<4;
		k|=(((j>>8)&$F)>>2)<<8;
		p1[i]:=k;
	end;
	bpl5:=ptr+40*256*3;
	fill(bpl5,$FFFFFFFF,10*128);
//	SetColor(1,$FFF);
	l3:=0;
	curFrame:=0;
	vbirq(VBlankPrecalc());
	EnableInterrupt(VB_IRQ);
	InitLines();
	
end;

