var
	@donotremove ro_tmp
	@donotremove ro_z1	
	r3d : incbin("data/object.bin") compressed;

@export "images/endlogo.flf" "images/endlogo.bin" 256 1
	endlogo:incbin("images/endlogo.bin") compressed;
	endlogo_pal: incbin("images/endlogo.pal");
	sprites:incbin("data/hearts_data.bin");
//	sprites:incbin("data/sprites_data.bin");
	

/*
// Updates the X/Y position of a sprite
procedure UpdateSpritePos(spo : pointer of byte; u_cx, u_cy, u_height:byte);
begin
	spo[1]:=u_cx; // X
	spo[0]:=u_cy; // Y 
	spo[2]:=u_cy+u_height; // Y+height
	spo[3]:=0;
end;
*/

	
procedure InitCopperList3D();
var 
	ik,jk,pk : integer;
begin
	if (DemoUnit::time>=DemoUnit::maxTime) then
		return;
	

	row:=7+$200*21;
	y:=$000F;// Color background start
	InitCustomCopperList(); // Message start custom copper list
//	ik:=sin256[(DemoUnit::time)&255];
	ik:=DemoUnit::time;
			
	pk:=0;	
	for ii:=0 to 64 do begin
		// First, copper should wait until raster "row" has been reached
//		jk:=tab_i_scale[ii]/2 + sin256[ik&255]>>2;
//		pp:=offscreen+tab160[jk&255];
//		if (pk=jk) then pp:=screen;
		pk:=jk;
		AddCopperCommand(row, COPPER_WAIT_FOR_RASTER);


		AddCopperCommand(#COPPER_PALETTE_START+2+14,colors3[ik&127]);
	
/*		if (ii<>0) then
		begin
			Graphics::AddCopperCommandBitplane(0, pp);
		end;
*/		
		ik+=1; 
		row+=$200;
	end;
	// Message end of copper list
	EndCustomCopperList();

end;


procedure InitLinePoly();
begin
	asm(" 
		move.w	#$a4a,LineMode
		move.l #-$1,$dff044 ;; last word first word mask
		move.l #-$8000,$dff072
		move.w #40,$dff060 ; modulo! SET!
		move.w #2,line_mode
		
 		");


end;


procedure FlipBuffers3D();
begin
	if (curBuf=0) then begin
		screen:=screenb1;
		offscreen:=screenb2;

	end else begin
		screen:=screenb2;
		offscreen:= screenb1;
	end;
	
	SetCopperList32(offscreen-40*40+4, #copper_bitplane3);
	//SetCopperList32(offscreen+40*256, #copper_bitplane1);
//	ss:=offscreen+40*256;
//	SetCopperList32(ss, #copper_bitplane1);

	curBuf:=(curBuf+1)&1;

end;
	

procedure Render3D();
var
	ymax:integer;
	
begin
	ro_count := p2[0];
	p2+=2;
//	ss:=linebuf;
	dst:=screen;		
	ss:=linebuf;
	
//	ro_count:=4;
//	ss:=screen; // Draw every second 
//	ij:=(time>>5);
	time+=1;
	m:=time>>2;
	for ro_ri:=0 to ro_count do begin
		rx1 := p2[0];		
		ry1 := p2[1];		
		rx2 := p2[2];		
		ry2 := p2[3];		
		rx3 := p2[4];		
		ry3 := p2[5];		
		rx4 := p2[6];		
		ry4 := p2[7];		

			
/*		rx1 := 20;
		ry1 := 10;
		rx2 := 100;
		ry2 := 20;
		rx3 := 50;
		ry3 := 100;*/
		
		if (((p2[8])&1)=0) then
		begin
		waitforblitter();
		InitLinePoly();
		DrawLine(rx1,ry1,rx2,ry2,ss,40);
		DrawLine(rx2,ry2,rx3,ry3,ss,40);
		DrawLine(rx3,ry3,rx4,ry4,ss,40);
		DrawLine(rx1,ry1,rx4,ry4,ss,40);
		/*
		Graphics_putpixel(ss,rx3,ry3);
		Graphics_putpixel(ss,rx2,ry2);
		Graphics_putpixel(ss,rx1,ry1);
		*/
		//Graphics_putpixel(ss,i,i);
			
		yy:=ry1-1;
		if (yy>ry2) then yy:=ry2-1;
		if (yy>ry3) then yy:=ry3-1;
		if (yy>ry4) then yy:=ry4-1;

		ymax:=ry1+1;
		if (ymax<ry2) then ymax:=ry2+1;
		if (ymax<ry3) then ymax:=ry3+1;
		if (ymax<ry4) then ymax:=ry4+1;
		dh := ymax-yy;
	
	
	
	
	h:=tab40[yy];	
	w:=h+tab40[dh];
	w1:=yy+dh;
	k:=tab64[dh]+10;
	l:=tab64[dh]+16;
	
//	fill(dst,$F0F0,10*10);

//			ablit(p2, screen, 0, x1>>3, y1,  40,  (16*$40)+(2), 0,40-4, 0,m);
	
		waitforblitter();
		fblit(ss,bplX, w,  0, w1,   40, l,   8,8,8,$09F0);
//		fblit(ss,bplX, w,  0, w1,   40, l,   0,0,0,$09F0);
		//if (p2[8]&1=1) then dst+=40*256;
//		k:=dh*$40+10;
//		ablit(bplX, dst, h,  0, yy,  40, k,    0,40,40,$0DFC);
		waitforblitter();
		ablit(bplX, dst, h+10,  10, yy,  40, k,    20,20,20,Graphics::BLT_ABD_OR);
		waitforblitter();
//		p3:=dst+40*256;
//		ablit(ss, p3, h+10,  10, yy,  40, k,    20,20,20,Graphics::BLT_ABD_OR);
//		ablit(bplX, dst, 0,  0, 0,  40, 200*$40+20,    0,0,0,$0DFC);
//		memcpy(bplX,0,dst,0,10*245,4);
//		ablit(ss, dst, h,  0, yy,  40, k,    0,00,00,$0DFC);
		waitforblitter();

		InitLinePoly();

		DrawLine(rx1,ry1,rx2,ry2,ss,40);
		DrawLine(rx2,ry2,rx3,ry3,ss,40);
		DrawLine(rx3,ry3,rx4,ry4,ss,40);
		DrawLine(rx1,ry1,rx4,ry4,ss,40);
		end;		
		
		p2+=(2*4*2+2);
	end;
	curFrame+=1;
	if (curFrame=128) then 
	begin
		p2:=p4;
		curFrame:=0;
	end;
//	poke16(#bltcon1,0);

//	fblit(ptr, dst, w,  0,w1,   40, k,  0,  0,0,$09F0);
	

end;





procedure Init3DStuff();
begin
	Graphics::SetupNonInterlacedScreen(#scr, 4);
	fill(#scr,0,10*256*5);
	ptr:=#scr;
	screenb1:=#scr+(256*40*4);
	screenb2:=screenb1+(40*256);
	linebuf:=ptr+(40*256*6);
	bplX := ptr+(40*256*7);
	p4:=ptr+40*256*8;
	p2:=p4;
//	pp:=ptr+40*256*9;
	ResetCopperList();
	Compression_Decompress(#endlogo,#scr);
	Compression_Decompress(#r3d,p4);
//	Compression_Decompress(#sprites,pp);
//	memcpy(#spagh_pal,0,#tarColors,0,8,4);
	FlipBuffers3D();
	memcpy(#endlogo_pal,0,#tarColors,0,4,4);
	time:=0;
	tarColors[1+8]:=$F49;
	curFrame:=0;
	tarColors[16]:=00;
	tarColors[17]:=$804;
	tarColors[18]:=$F1A;
	tarColors[19]:=$F9E;

end;


procedure Update3DStuff();
begin
	p1:=p4;
	while (DemoUnit::time<DemoUnit::maxTime) do
	begin
//		fill(linebuf,0,10*250);
		Render3D();
		waitverticalblank();
		FlipBuffers3D();
//		fill(screen,0,10*200);
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
/*			asm("
			move.l screen,a0
			add.l #40*256,a0
			move.w #245,d5
			add.l #40*10,a0
.topp2:
			add.l #1*4,a0
			rept 8
			move.l #$0000,(a0)+
			endr
			add.l #1*4,a0
			dbf d5,.topp2			
			
		");		
*/
//		tarColors[1]:=DemoUnit::time;
	end;		
end;


procedure UpdateHeartSprites();
begin
	// Calculate some funky sines
/*	xb:=sin256[time&255]>>1;
	yb:=sin256[(time+64)&255]>>1;
	xb:=xb+64;
	yb:=yb+64;
*/
	// Find frame in data	
	j:=(time&63)<<7; // 64 frames in total, 128 bytes per frame (<<7)
	// Copy over 32*4=128 bytes of data
	xb:=154;
	yb:=80 + sin256[time&255]>>4;
	UpdateSpritePos(#sprite_struct1, xb,yb, 32,0);
	
	memcpy(#sprites,j,#sprite_struct1,4,32,4);
	
//	fill(#sprite_struct1+4,$F0F0,32);
	for i:=0 to 8 do tarColors[i+8]:=15-i*2;
	setcopperlist32(#sprite_struct1, #copper_spritestruct0);
end;


interrupt VBlank3DStuff();
begin
	ApplyCopperList();
	UpdateHeartSprites();
	FadeTo(32);	
/*	poke16(#copper_palette,2+4*16,$00);	
	poke16(#copper_palette,2+4*17,$0804);	
	poke16(#copper_palette,2+4*18,$0F1A);	
	poke16(#copper_palette,2+4*19,$0F9E);	*/
//	FlipBuffers3D();
	if (DemoUnit::time=DemoUnit::maxTime-16) then fill(#tarColors,0,14);
	DemoUnit::Update();
//	InitCopperList3D();
//	setcopperlist32(#sprite_struct1, #copper_spritestruct0);
	time+=1;
end;



