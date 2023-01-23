
// generate a random number, return in register A
procedure RND();
begin
	
	asm
		
	lda seedhi
	lsr
	rol seedlo
	bcc noeor
	eor #$B4
noeor
	sta seedhi
	eor seedlo
  
	end;
	// result in a
	
end;

procedure VblWait();
Begin
				
	// wait for raster position outside of visible screen area (vertical blank)
	ticks += 1;
	waitforraster(109);
	waitforraster(110); // double wait required because sometimes one frame occurs during same raster position

end;

procedure Long_Wait();
begin
	
	// wait 2 seconds
	for i := 0 to 60 do VblWait();
	
end;

// p1 = string, _x = length
procedure Print();
begin

	asm ;
	
printit
	ldy #0
printitloop
	lda (p1),y

printit_skip
	sta (screenmemory),y
	iny
	dex
	cpx #0
	bne printitloop

	end;
end;