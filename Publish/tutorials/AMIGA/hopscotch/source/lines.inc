var  
   	loo,t,sx,sy,jj,time,tt: integer = 0; 
   	ll:long;
   	@donotremove LineMode
   	ii,ij,k2:long;
	curBuf : integer = 0;
	
//	sin256 : array[256] of byte = buildtable("Math.sin256(i/256.0*2*3.14159)*127 +127");
	
	p1l : pointer of long;
   	pi : pointer of integer;

	screen,offscreen: pointer of long;


	// Rotation matrices  3 x 3 = 9 longs
	rotmat,rotX,rotY,rotZ,rotmatA : array[9] of long;

	  	
	// 40 = 320/8 = bytes per bitplane
   	@define width 40 
   	@define height 256
   	@define bitplanes 1
   	
	@define bitmapFont_noCharactersInColumns 10
	@define bitmapFont_characterHeight 24


	const maxlFrames : integer = 256-64;


	l_time_tab : array[maxlFrames] of byte = buildtable("i/(256-64)*255.0");

	const size : byte = 8;
	
	vcount: long = size*size;
	const max : integer = size*size+1;
	vertices_T : array[3*max] of long;
	vertices : array[3*max] of long;
	const lcount : integer = (size-1)*(size-1)*2;
	LineList : array[lcount*2] of integer;
//	PVert : array[2*max*maxlFrames] of long;
	PVert : pointer of integer;
	orgVert : pointer of long;
	zpos : long = 488;
	cnt:integer;
//	LineMode: integer = $a4a;




	
   	
/*  	lineBuf : array[@width*@height] of byte chipmem;
	  	
  	buffer : array[@width*@height] of byte chipmem;
   	screenb1 : array[@width*@height*@bitplanes] of byte chipmem;
   	screenb2 : array[@width*@height*@bitplanes] of byte chipmem;

*/

	screenb1, screenb2 : pointer of byte;
 
/*
	Performs the rendering of the scene usin256g lines. Will probably be 
	converted to a TRSE method later on. 

*/




procedure RenderObjectLines(ro_list:pointer of integer,ro_count : integer);
var
	ro_ri : integer = 0;
	ro_x1,ro_x2, ro_y1, ro_y2 : integer = 0;
	ro_z1,ro_tmp:byte;
	ro_idx,ro_nidx : integer = 0;
	
begin

	ro_idx:=0;
	tt:=0;
	asm(" 
		move.l #-$1,$dff044 ;; last word first word mask
		move.l #-$8000,$dff072
		move.w #40,$dff060
		");
		
	for ro_ri:=0 to ro_count do begin
			i:=ro_list[ro_idx];
			j:=ro_list[ro_idx+1];
			
			ro_x1:=PVert[i];
			ro_y1:=PVert[i+1];
			
			ro_x2:=PVert[j];
			ro_y2:=PVert[j+1];
			
			DrawLine(ro_x1,ro_y1,ro_x2,ro_y2, screen, 40);
	
		ro_idx:=ro_idx+2; // two shorts per line
	end;
	return;
	asm("
	
LineMode: 	dc.w	$a4a
	");

end;




/*
	Method that sets cosin256e and sin256e of a given theta in 0-255

*/
procedure SetCosin256(scs:integer);
var
   cost,sin256t,tmp : long =0;
	scs_t:  integer = 0;
	ms : long;
	v:integer;
begin
	scs:=scs&255;
	v := sin256[scs];
	sin256t:=v-128;	
	
	scs_t:=scs+64;	
	scs_t:=scs_t&255;
	v := sin256[scs_t];
	cost:=v-128;	


end;

/*
	Sets up the rotation matrix
	Rx(t1) * Ry(t2) * Rz(t3) of the three Euler angles t1,t2 and t3
*/

procedure SetupRotMat(rto3D_t1,rto3D_t2,rto3D_t3 : integer);
begin
	SetCosin256(rto3D_t1);
	SetRotationX(#rotX,cost,sin256t);
	SetCosin256(rto3D_t2);
	SetRotationY(#rotY,cost,sin256t);
	SetCosin256(rto3D_t3);
	SetRotationZ(#rotZ,cost,sin256t);

	// Built-in matmul rulez	
	MatMul3x3(#rotX,#rotY,#rotmatA);
	MatMul3x3(#rotmatA,#rotZ,#rotmat);
	
//	setidentity(rotmat);
end;




/*
	Flips rendering buffer and updates copper list & pointers
*/
procedure FlipBuffers();
begin
	if (curBuf=0) then begin
		screen:=screenb1;
		offscreen:=screenb2;

	end else begin
		screen:=screenb2;
		offscreen:=screenb1;
	end;
	
	SetCopperList32(offscreen,#copper_bitplane0);

	curBuf:=(curBuf+1)&1;

end;



procedure UpdateGrid();
begin
	p1l:=#vertices;
	k2:=1;
	for i:=0 to size do
	begin
		for j:=0 to size do
		begin
			t:=(i+j)<<4;
			t:=t+time<<3;
			t:=t&255;
			k:=sin256[t];
			l2:=k/2;
			
			p1l[k2]:=l2+j;
			k2:=k2+3;

		end;

	end;


end;



procedure InitGrid();
begin
	k2:=0;
	
	p1l:=#vertices;
	
	for i:=0 to size do
	begin
		sx:=0;
		sy:=0;
		sx:=i-size/2;
		for j:=0 to size do
		begin
			sy:=j-size/2;

			p1l[k2]:=sx*32;
			k2:=k2+1;
			
			p1l[k2]:=100;
			k2:=k2+1;
			
			p1l[k2]:=sy*32;
			k2:=k2+1;

		end;

	end;
	k2:=0;
	m:=size-1;

	pi:=#LineList;

	for i:=0 to m do
	begin
		
		for j:=0 to m do
		begin
			l:=i*size + j;
			l:=l<<1;			
			pi[k2]:=l;
			k2:=k2+1;
			l:=i*size + j+1;
			l:=l<<1;			
			pi[k2]:=l;
			k2:=k2+1;
	
		

			l:=i*size + j;
			pi[k2]:=l<<1;
			k2:=k2+1;
			l:=(i+1)*size + j;
			l:=l<<1;			

			pi[k2]:=l;
			k2:=k2+1;
		

		end;
	end;
	
end;


/*
	Initializing method. Most TRSE "Init"-methods will be performed automatically in later versions.

*/
procedure PerformInit();
begin
	
	//	lineBuf, buffer, screenb1, screenb2 : pointer of byte;
	screenb1:=#scr;//+256*40;
	screenb2:=screenb1+256*40*5;
	ptr:=#scr;	
	
	Graphics::SetupNonInterlacedScreen(#scr, 5);
	fill(#curColors,0,8);
	memcpy(#spagh_pal,0,#tarColors,0,8,4);	
	setpalette(#curColors,#copper_palette,32);
	Compression::Decompress(#spagh, ptr);
//	applycopperlist();
//	loop();
//	InitLine();



end;


procedure ExecuteLines();
begin
	PerformInit();
	p1:=ptr;//+(256*40);
	memcpy(p1,0,bplX,0,10*256,4);
	memcpy(bplX,0,screenb1,0,10*256,4);
	memcpy(bplX,0,screenb2,0,10*256,4);
//	fill(screenb1,0,40);
//	fill(screenb2,0,40);
	FlipBuffers();
//	Compression_Decompress(#woman1,bplX);
//	fill(#scr,0,10*256*5);
	PVert:=orgVert;
	while (DemoUnit::time<DemoUnit::maxTime) do begin
		
		RenderObjectLines(#LineList,lcount);		
		PVert+=lcount*2*2;
		curFrame+=1;
		if (curFrame=maxlFrames) then
		begin
			curFrame:=0;
			PVert:=orgVert;
		end;
		waitverticalblank();		
		FlipBuffers();
		
		//setpalette(#curColors,#copper_palette,31);
		//fill(screen,0,10*201);
		memcpy(bplX,0,screen,0*40,10*251,4);
		FadeTo(32);
		if (DemoUnit::time=DemoUnit::maxTime-16) then fill(#tarColors,0,8);
		/*asm("
			move.l screen, a0
			move.w #10,d3
@topp:
			rept 230
			move.l #0,(a0)+
			endr
			dbf d3,@topp
		");*/
		
		DemoUnit::Update();
	end;
	fill(#scr,0,10*256*7);
end;

procedure InitLines();
begin
	
	InitGrid();	
	InitMatmul3x3();
	InitMatmulVec();
	InitMatmulVecNormalZ();
	InitProjectAllVertices();	
//	ablit(logo_image,screen,0, 0,0,  160,(100*$40) + 20,0,120);
//	memcpy(bpl5,0,bplX,0,10*252,4);

//	WaitVerticalBlank();	
	i:=0;
	screen:=screenb1;
	offscreen:=screenb2;
	
	SetIdentity(#rotX);	
	SetIdentity(#rotY);	
	SetIdentity(#rotZ);	
//	ApplyCopperList();
	time:=0;
	setLineType(0);
	curFrame:=0;	
	orgVert:=#scr+256*40*8;
	PVert:=orgVert;
	x2:=0;
	for l3:=0 to maxlFrames do
	begin
		time := l_time_tab[x2];
		UpdateGrid();	
		SetupRotMat((time/3)&255,time&255,(time/2)&255);
		SetupRotMat(0,(92+time)&255,(111-time)&255);
		MatmulVec(#rotmat, #vertices, #vertices_T, vcount);
		ProjectAllVertices(#vertices_T,PVert, vcount, 160,140,zpos);
		PVert+=lcount*2*2;
		x2+=1;
	end;
	
		

end;

