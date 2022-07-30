procedure FlipBuffersPath();
begin
	if (curBuf=0) then begin
		screen:=screenb1;
		offscreen:=screenb2;

	end else begin
		screen:=screenb2;
		offscreen:=screenb1;
	end;
	
	SetCopperList32(offscreen,#copper_bitplane4);

	curBuf:=(curBuf+1)&1;

end;




procedure UnpackFonts();
begin
	src:=#font;
	for i:=0 to 26 do
	begin
		for j:=0 to 32 do
		begin
			for k:=0 to 16 do
			begin
				dst[0]:=src[0];
				dst[1]:=0;
				dst+=4;
				src+=2;
			end;
		end;			
	end;
end;

procedure InitPaths();
var
	paths: incbin("data/rawxy.bin");
	text: string="                                                                                                                                MY EYES ARE HURTING AGAIN         WONDERING WHERE TO REST MY GASE          TOO MUCH TOO MUCH                                                                                 ";
	fonttab : array[32] of integer = buildtable("i*2*16*32*2");
begin
	ptr:=#scr;
	screenb1:=#scr+40*256*4;
	screenb2:=#scr+40*256*5;
	Graphics::SetupNonInterlacedScreen(ptr, 5);
//	fill(#scr,0,10*256*6);
//	fill(#scr + 0,$F0,10*256);
//	fill(ptr,0,10*256*6);	
	fill(#curColors,0,8);
	//fill(#tarColors,0,8);
	//for i:=1 to 15 do
	//	tarColors[i]:=$FFF;
	setpalette(#curColors,#copper_palette,31);
	p3:=ptr+40*256*6;
	Compression_Decompress(#bpl_spiral,p3);
	curFrame:=0;
	FlipBuffersPath();	
	
	memcpy(#image_tent_pal,0,#tarColors,0,8,4);
	tarColors[4]:=$823;
	tarColors[5]:=$A73;
	p1:=#tarColors;
	for i:=0 to 16 do
	begin
		p1[i+16]:=$FFF;
	end;
//	p3+=40*256*8;
	dst:=#scr+40*256*(6+8);
	UnpackFonts();
	src:=#scr+40*256*(6+8);
	l:=0;
end;


procedure DrawPath();
begin
	p1:=#paths;
	j:=12-shift*3+20*12;
	
	// Setup blitter
	asm("
	moveq.l #0,d6
	lea     $dff000,a6 ; Hardware registers
	move.w #$0,d6
	moveq #0,d0
	move.w x1,d0     ; BOP move
	move.w #$28,d3
	move.w #$40*15+2,d4
	move.w #$0,BLTAMOD(a6)
	move.w #$24,BLTBMOD(a6)
	move.w #$0,BLTCMOD(a6)
	move.w #$24,BLTDMOD(a6)
	move.w  #0,BLTCON1(a6) ;    issa 0   BLTCON1
	move.w #$0,d6
	moveq #0,d0
	move.w  #$0,BLTCON1(a6) ;    issa 0   BLTCON1
	
	
	");

	for i:=20 to 120 do
	begin

		x1:=p1[j];
		y1:=p1[j+1];
		y2:=p1[j+2];
		if (y2=0) then y2:=1;
//		if (i>length(text)) then break;
		k:=pbb[i]&255;
		if (k<>$20) then
		begin
			p2:=src+fonttab[k-65];
			p2+=(tab64[y2]);	
		
//			Graphics::Blit16pix(p2,screen,x1,y1);		
			asm("
			
				
			");
//			ablit(p2, screen, 0, x1>>3, y1,  40,  (16*$40)+(2), 0,40-4, 0,m);
			asm("
	move.l p2,a0
	move.l screen,a1
	move.w x1,d1     ; BOP move
	lsr.w #$3,d1
	move.w y1,d2

	move.w x1,d0
	and.w #15,d0
	ror.w #4,d0
	or.w #$DFC,d0

	
	jsr blitter


			
			
			");
		end;
		j+=4*3;
//		Graphics::putpixel(offscreen,x1,y1);
	end;
	shift+=1;
	if (shift=4) then
	begin
		shift:=0;
		pbb+=1;
	end;
		
	Graphics::Blit16pixMask(#mask,screen,138,172);		

	
end;

interrupt VBlankPaths();
begin
	FadeTo(32);
	val:=1;
	if (DemoUnit::time=DemoUnit::maxTime-16) then
		fill(#tarColors,0,16);
		
	
	p4:=p3 + (40*256)*curFrame;
	SetCopperList32(p4,#copper_bitplane0);
	p4:=p3 + (40*256)*((curFrame+1)&7);
	SetCopperList32(p4,#copper_bitplane2);
	if (DemoUnit::time&1=0) then
	begin
		if (curFrame = 0) then
		 curFrame:=7
		else
			curFrame-=1;
	end;
	
	
		
	DemoUnit::Update();
end;

procedure UpdatePaths();
begin
	pbb:=#text;
	
	shift:=0;
	
	
	
	while (DemoUnit::time<DemoUnit::maxTime) do 
	begin

		// Clear screen
		asm("
			move.l screen,a0
			move.w #245,d5
			add.l #40*10,a0
.topp:
			add.l #1*4,a0
			rept 8
			move.l #$0000,(a0)+
			endr
			add.l #1*4,a0
			dbf d5,.topp			
			
		");		
//		fill(screen+40*10,0,10*246);
//		memcpy(bplX,0,screen,0,10*250,4);	


//		waitverticalblank();
		
		DrawPath();
//		while (val<>1) do begin end;
//		val:=0;
		waitverticalblank();
		
		FlipBuffersPath();
		time+=1;
	end;

end;