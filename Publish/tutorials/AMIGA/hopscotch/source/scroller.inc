

@define WW 46

var	
	@donotremove jmpPos
	jmpPos : long;
	ended : integer = 0;
   	t2,i2,i3: integer = 0; 
 	done : integer = 0;
   	
// 2 bitplanes
//	orgStart : integer = $2c81;
//	orgStart : integer = $2c81;
//	orgEnd : integer = $2cc1;
	
	@donotremove buf1
	@donotremove buf2
   	
// Sine table
@donotremove text   	
	text_scroller: string = ("ZZZZZZZZZZZZZZZZZZZZZZZZZZZ                      %HAHA DO YOU REMEMEMBER WHENN ALMSTOP WE WELL YES YES HAHAHA    ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ");

//	text_scroller: string = ("ZZZZZZZZZZZZZZZZZZZZZZZZZ                  %HAHA DO YOU REMEMEMBER WHENN YES YES HAHAHA    ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ");
	table2: pointer of integer;
 	dst2,fr : pointer of long;
		
	sine32 : array[256] of byte = buildtable("(Math.sin(i/256.0*2*3.14159)*127+127)/6");
	sine32_2 : array[256] of byte = buildtable("(Math.sin(i/256.0*2*3.14159)*127+127)/2");
	tp : pointer of byte;

	ytab : array[256] of integer = buildtable("i*@WW");


   	t_tab : incbin("data/e3_tab.bin");
   	t_data : incbin("data/e3_data.bin");


  tab : pointer of integer;
  cury,curl,curA,curB : integer;
  tabFrame : array[48] of integer = BuildTable("i*128");
  tabChar : array[48] of integer = BuildTable("i*128*16");
  shifts : array[256*2] of integer;
  shiftsy : array[256*2] of integer;


var
	screen_data_ptr_1 : pointer;


procedure RenderFrame(tf,bb : global integer);
begin
	ptr := bpl5-40*90;

//	ptr += (sine32_2[(tf)&255])*@WW;
	ptr += (sin256[(tf)&255]>>1)*@WW;
		
	if (bb=0) then
	SetCopperList32(ptr, #copper_bitplane0);
		
	if (bb=1) then
	SetCopperList32(ptr, #copper_bitplane2);

end;


procedure RenderLetter2(xp : pointer of long; yp:global integer; tx, charr:integer);
  
begin
	tab := #t_tab;
	dst := #scr;
	dst:=dst+xp;
	
	
	tx:=tx&15;
	tp:=#text_scroller;
	tp:=tp+4;
	j:=tp[charr];
	if (j=$20) then j:=28;
		j:=(j-65)&63;
		
	if (yp>200) then return;
	
	dst := dst + ytab[yp];
	if (j=25) then
	begin	
		asm("
		move.l dst,a0
;		add #@WW*256*1,a0
		
		move.l #0,d0
	rept 74		
		move.l d0,(a0)
		move.l d0,(@WW*1*256)(a0)
		add.l #@WW,a0		
	endr
	
		");
		return;
	end;	


	if (tx=0) then tx:=1;
//	if (tx>=30) then tx:=30;
	tab += tabFrame[tx];
	tab += tabChar[j];
	//dst := #screen_data;
	//dst+=xp;
	
	asm("

		move.l dst,a0
;		add #40*256*1,a0
		move.l tab,a1
		lea t_data,a2
		
		move.l #0,d0
	rept 4		
	
		move.l d0,(a0)
		move.l d0,(@WW*1*256)(a0)
		add.l #@WW,a0		
	endr
topp1:
		rept 62		
		
		move.w (a1)+,d0
		move.l (a2,d0),(a0)
		move.l (4)(a2,d0),(@WW*1*256)(a0)
		add.w #@WW,a0	
	
		endr
		

		move.l #0,d0
		rept 4
		
		move.l d0,(a0)
		move.l d0,(@WW*1*256)(a0)
		add.l #@WW,a0		
		endr
		

	");
end;




procedure InitCopperListScroller();
begin
	
	row:=7;
	j:=time;
	l:=0;
	y:=$000F;// Color background start
	InitCustomCopperList(); // Message start custom copper list
//	AddCopperCommand($0180, 0); // First row black
	AddCopperCommand(row, COPPER_WAIT_FOR_RASTER);
	
	k:=15-(shift*2)&15;
	k:=k<<4;
	
	AddCopperCommand(COPPER_SCROLL, k);
	
	AddCopperCommand(copper_diwstrt, orgStart);
	AddCopperCommand(copper_diwstop, orgEnd);
	
//	AddCopperCommand(copper_diwstrt,#$2c81);
//	AddCopperCommand(copper_diwstop,#$41c0);

	AddCopperCommand(copper_ddfstrt, #$0028);
	AddCopperCommand(copper_ddfstop, #$00D8);
	

	 		 	

	EndCustomCopperList();

end;


procedure UpdateBGColors();
begin
/*	i := sine32[time&255]/16;
	i:=i&$F;
	poke16(#copper_palette,2+4*1,i+(i/2)<<4);	
	i := sine32[time2&255]/16;
	i:=i&$F;
	poke16(#copper_palette,2+4*4,i<<4+(i/2));	
	j := sine32[(time*2)&255]/32+i;
	j:=j&$F;
	
	poke16(#copper_palette,2+4*5,j + (j/2)<<4);	

*/
/*
	poke16(#copper_palette,2+4*1,$222);
	poke16(#copper_palette,2+4*4,$444);
	poke16(#copper_palette,2+4*5,$88A);
	*/
	palette[0]:=0;
	palette[1]:=$222;
	palette[2]:=$444;
	palette[3]:=$88A;


end;


procedure SetColor(color, val:integer);
begin

	poke16(#copper_palette,2+4*color,val);	
	i :=color |%0001;
	poke16(#copper_palette,2+4*i,val);	
	i :=color |%0100;
	poke16(#copper_palette,2+4*i,val);	
	i :=color |%0101;
	poke16(#copper_palette,2+4*i,val);	
	



end;

procedure SetColor2(color, val:global integer);
begin

	tarColors[color]:=val;	
	i :=color |%0001;
	tarColors[i]:=val;	
	i :=color |%0100;
	tarColors[i]:=val;	
	i :=color |%0101;
	tarColors[i]:=val;	
	



end;


procedure Scroll();
begin
	
//	if (time&1=0) then 
		shift:=shift+2;

	if (shift=16) then 
	begin
		txtCnt +=1;
		shift := 0;
	end;

end;


interrupt ScrollerVBlank();
var
 rj:integer;
begin
	DemoUnit::Update();	
	Scroll();
	InitCopperListScroller();
	ApplyCopperList();

	
	
	k:=sine32[time&255]/4+40;
	curA := (txtCnt);
	curB := curA+12;
	
	for i:=curA to curB do
	begin
		shifts[i] := sine32[(i*2-time2)&255];//+sine32[(i*3-time)&255];
		shiftsy[i] := k + sine32_2[(i*4-time*2)&255];//+sine32[(i*3-time)&255];
	end;



	xx:=0-(shift>>2)&$FFFE;
	rj:=txtCnt*4;
	i3:=0;
//	for i2:=11 to $FFFF step $FFFF do
	for i2:=0 to 12 do
	begin	
		curl:=(i2+txtCnt);
		rj := shifts[curl];
		j:=(tp[curl]-65)&63;
//		RenderLetter2(i3+xx,shiftsy[curl]>>1+80,rj,curl);
		if (i3+xx<1024) then 
		RenderLetter2(i3+xx,shiftsy[curl]>>1+80,rj,curl);
		i3+=4;
	end;
	i3:=0;
	

//	UpdateBGColors();
	RenderFrame(t2,0);
	RenderFrame(t2*2,1);
	
	if (DemoUnit::time>48) then
		FadeTo(31);	
		
	if (DemoUnit::time=256+128-64) then
	begin
//		SetColor2(0,$202);
		tarColors[0]:=0;
	end;
	if (DemoUnit::time=DemoUnit::maxTime-16) then
	begin
		fill(#tarColors,0,8);
	end;
	time:=time+1;
	time2+=1;
	if (time&1=0) then time2+=1;
	t2+=1;
	if (time&63=16) then t2+=64;
	t2:=t2&255;

	
	
end;

/*
0000    0
0001    1
0100    4
0101    5
*/

/*
0010    2
1000    8
1010    10

1001    1
1100    4
1101    5

1011    1
1110    4
1111    5

*/



procedure InitScroller();
begin
	for i:=0 to 16 do 
		SetColor(i,$FFF);	
	
	
	Compression::Decompress(#text_logo, #scr);
	src:=#scr;
//	bpl5 := #scr+ 40*256*5;
	dst:=bpl5-8;
	fill(dst,0,10*256);	
	for i:=0 to 256 do
	begin
		memcpy(src,0,dst,0,10,4);
		src+=40;
		dst+=@WW;		
	end;
	
	fill(#scr,0,10*256*5);	
	
	j:=$1000*4 | $200;
	poke16(#copper_resolution, 0 , j); // $4000  = bitplanes, $200 = colours
	poke16(#copper_mod_even,0,0);
	poke16(#copper_mod_odd,0,0);
	ptr:=#scr;
	SetCopperList32(ptr, #copper_bitplane1);
	ptr+=@WW*256;
	SetCopperList32(ptr, #copper_bitplane3);
	shift:=0;
	y:=0;
	curFrame:=0;


	for i:=0 to 16 do
		poke16(#copper_palette,2+4*i,$FFF);


	// 0101
/*			
	poke16(#copper_palette,2+4*0,$00);	
	poke16(#copper_palette,2+4*1,$0095);	
	poke16(#copper_palette,2+4*4,$004F);	
	poke16(#copper_palette,2+4*5,$0AF0);	
*/
	time:=0;
	time2:=0;
	txtCnt:=0;
	fill(#shifts,0,128);
	fill(#shiftsy,0,128);
	fill(#palette,$FFFFFFFF,8);
	fill(#tarColors,$FFFFFFFF,8);
	fill(#curColors,$FFFFFFFF,8);

/*	tarColors[0]:=$FFF;
	tarColors[1]:=$999;
	tarColors[2]:=$012;
	tarColors[3]:=$7f7;

	tarColors[4]:=$FFF;
	tarColors[7]:=$96F;
	tarColors[8]:=$348;
	tarColors[10]:=$29F;
*/	
	SetColor2(0,$FFF);	
	SetColor2(1,$FFF);	
	SetColor2(1,$999);	
	
	SetColor2(2,$012);	
	SetColor2(3,$837);	
	SetColor2(4,$FFF);	
	SetColor2(7,$96F);	
	SetColor2(8,$348);	
	SetColor2(10,$29F);	


	
//	fill(#bpl5,$F0F0,10*256);

end;
