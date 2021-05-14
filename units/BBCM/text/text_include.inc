




	procedure screen_mode(selected_mode:byte);
	begin

		_X:=0;
		_Y:=0;
		_A:=22;
		call(OSWRCH);
		_A:=selected_mode; // Again, need to replace with a mode param
		call(OSWRCH);

	end;

	function current_bbc():byte;
	begin
		_A:=0;
		_X:=1;
		call(OSBYTE);
		this_bbc:=_X;
		returnvalue(this_bbc);
	end;

	procedure cls();
	begin

		if(current_bbc()=0) then
		begin
			// Electron
			screen_mode(6);	
		end
		else
		begin
			// if BBC
			screen_mode(7);	
		end;

		_X:=0;
		_Y:=0;
		_A:=12;
		call(OSWRCH);
		_A:=30;
		call(OSWRCH);
		DefineScreen();
		

	end;	

	procedure DefineScreen();
	var

		y:byte;
	begin

		if(ytab[0]<>0) then
		begin
			return;
		end;

		if(current_bbc()=0) then
		begin
			// Electron
			temp_address_p:=$6008;
			fori y:= 0 to 24 do
			begin
				ytab[y]:=temp_address_p;
				temp_address_p:=temp_address_p+49;			
			end;


		end
		else
		begin
			// if BBC
			temp_address_p:=$7C00;
			fori y:= 0 to 24 do
			begin
				ytab[y]:=temp_address_p;
				temp_address_p:=temp_address_p+40;			
			end;
		end;

	end;

	procedure move_to(_text_x: byte, _text_y: byte);
	begin
		_A:=31;
		call(OSWRCH);
		_A:=_text_x;
		call(OSWRCH);
		_A:=_text_y;
		call(OSWRCH);
	end;	

    // put cursor n home position 0,0
    procedure cursor_home();
    begin
        move_to(0,0);
    end;

	procedure wait_vsync();
	begin
		_A:=19;
		call(OSBYTE);
	end;


	procedure cursor_on();
	begin
		
		poke(CRTC_V,0,10);
		poke(CUR_OF,0,0);
	end;
	
	
	//CURSOR_OFF
	procedure cursor_off();
	begin

		poke(CRTC_V,0,10);
		poke(CUR_OF,0,32);

	end;


	//CURSOR Carriage Return
	procedure cursor_return();
	begin

		call(OSNEWL);

	end;

	procedure mode_1();
	begin

		screen_mode(1);

	end;

	procedure red();
	begin
		_A:=17;
		call(OSWRCH);
		_A:=1;			// should use _chosen_text_colour but that doesn't work
		call(OSWRCH);
	end;	

	procedure yellow();
	begin
		_A:=17;
		call(OSWRCH);
		_A:=2;			// should use _chosen_text_colour but that doesn't work
		call(OSWRCH);
	end;	

	procedure text_colour(_chosen_text_colour: byte);
	begin
		_A:=17;
		call(OSWRCH);
		_A:=_chosen_text_colour;	// should use _chosen_text_colour but that doesn't work
		call(OSWRCH);
	end;	

	procedure print_string(in_str: pointer, CRLF: byte = True);
	var
	  	ch: 		byte;
	  	next_ch:	byte;

	begin

	  	next_ch:= 0;

		while(in_str[next_ch]) do
		begin
			ch:=in_str[next_ch];
			_A:=ch;
			call(OSWRCH);
			inc(next_ch);
		end;

		if(CRLF=True) then call(OSNEWL);
	end;

	// Put a character at current cursor location
	procedure put_ch(CH:byte);
	begin
		_A:=CH;
		_X:=0;
		
		if(CH=13) then call(OSNEWL) else call(OSWRCH);
	end;

	// Clear keyboard buffer
	procedure clear_buffer();
	begin
		_X:=0;
		_A:=15;
		call(OSBYTE);
	end;


	function get_key():byte;
	var
	
		_keyp: byte=0;
	begin

		_X:=1;
		_A:=4;
		call(OSBYTE);

		_X:=0;
		_A:=0;		
		call(OSRDCH);

		_keyp:=_A;
		clear_buffer();
		returnvalue(_keyp);
	end;	

	procedure wait_key();
	begin
		while(get_key()=0) do
		begin
			//
		end;	
	end;

	procedure beep();
	var
		beep_string:string=(7,0);
	begin
		print_string(#beep_string,True);
	end;	


	function char_under_cursor():byte;
	var
		r_char: byte=0;
	begin

		_A:=$87;
		call(OSBYTE);
		r_char:=_X;
		returnvalue(r_char);
	end;


	function get_char_at(_col,_row:byte):byte;
	var
		_char: byte=0;
		_line_addr: pointer;
	begin

		if(this_bbc>0) then
		begin
			_line_addr:=ytab[_row];
			_char:=_line_addr[_col];
		end
		else
		begin
			move_to(_col,_row);
			_char:=char_under_cursor();
		end;

		returnvalue(_char);
	end;

	procedure put_char_at(_atx,_aty,_atchar:byte);
	begin

		if(this_bbc>0) then
		begin
			screen_memory:=ytab[_aty];
			screen_memory[_atx]:=_atchar;
		end
		else
		begin
			move_to(_atx,_aty);
			put_ch(_atchar);
		end;
	end;
	
	
   function get_string():integer;
   var
	_input_key, bp: byte;
	input_buffer: string=("                                                                   ",0);
	p_input_buffer: pointer;
	   
   begin
   
	bp:=0;
 	_input_key:=0;
 	p_input_buffer:=#input_buffer;
 	
 	txt::cursor_on();
 	
	while (_input_key<>13) do
	begin

		_input_key:=txt::get_key();
		
		if(_input_key > 0 and _input_key < 255) then
		begin
			txt::put_ch(_input_key);
			p_input_buffer[bp]:=_input_key;
			bp:=bp+1;
		end;

		if(_input_key = 20) then
		begin
			txt::put_ch(32);			
			txt::put_ch(20);	
			bp:=bp-2;

		end;

		p_input_buffer[bp]:=0;
	end;
	
	cursor_off();
	returnvalue(p_input_buffer);

   end;
