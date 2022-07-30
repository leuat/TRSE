
procedure RenderCreditsFrame(tf,bb,dx : global integer);
begin
	ptr := bpl5-40*90;
	ptr+=dx*2;

	ptr += (sine32_2[(tf)&255])*40;
		
	if (bb = 0) then
		SetCopperList32(ptr, #copper_bitplane0);
		
	if (bb = 1) then
		SetCopperList32(ptr, #copper_bitplane1);

	if (bb=2) then
		SetCopperList32(ptr, #copper_bitplane2);

	if (bb=3) then
		SetCopperList32(ptr, #copper_bitplane3);

end;

interrupt CreditsVBlank();
begin
	DemoUnit::Update();	
	ApplyCopperList();

	RenderCreditsFrame(time*2,1,0);
//	RenderZiggFrame(time*3,2,0);
	
	time+=1;
	t2+=1;
	if (t2=40) then t2:=0;
end;



procedure InitCredits();
begin
	ptr:=#scr;
	
end;