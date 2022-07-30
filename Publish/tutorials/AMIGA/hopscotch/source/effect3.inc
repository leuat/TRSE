

procedure InitEffect3();
begin
//	Graphics::SetupNonInterlacedScreen(#bpl5,5);
	ptr:=#image;
	ptr:=ptr+40*10;
	Graphics::SetupNonInterlacedScreen(ptr,5);
	
	setpalette(#curColors,#copper_palette,31);
end;


