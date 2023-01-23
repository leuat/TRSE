
procedure PlayHiss();
begin
	
	SOUNDFX_REGISTER := 0;
	if (hissDuration > 0) then 
	begin
		
		SOUNDFX_REGISTER := 253;
		hissDuration -= 1;
	
	end;

end;

procedure PlayFood();
begin
	
	SOUND3_REGISTER := 0;

	if (foodDuration > 0) then
	begin
		
		SOUND3_REGISTER := 220 - foodDuration;
		foodDuration -= 2;
	
end;
	

end;

procedure Sound_Egg(_a: byte);
begin
	
	SOUND3_REGISTER := _a;
	VblWait();
	SOUND3_REGISTER := 0;

end;