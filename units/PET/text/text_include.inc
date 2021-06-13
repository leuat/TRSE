

	procedure cls();
	begin
		_A:=147;
		call(^$FFD2);
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

		temp_address_p:=$8000;

		fori y:= 0 to 24 do
		begin
			ytab[y]:=temp_address_p;
			temp_address_p:=temp_address_p+40;			
		end;
	end;

	procedure move_to(_text_x: byte, _text_y: byte);
	var
		temp_p: pointer;
	begin
		temp_p:=ytab[_text_y];
		poke(^196,0,lo(temp_p));  // LSB
		poke(^197,0,hi(temp_p)); // MSB
		poke(^198,0,_text_x);
		call(^57471)
	end;	


   function ascii_to_petscii(_ASCII_CH:byte):byte;
   begin

      if(_ASCII_CH >=65 and _ASCII_CH<=90) then
      begin
         _ASCII_CH:=_ASCII_CH+32;
      end
      else
      begin
         if(_ASCII_CH >=97 and _ASCII_CH<=122) then
         begin
            _ASCII_CH:=_ASCII_CH-32;
         end;
      end;

      if(_ASCII_CH>=127) then
      begin
         _ASCII_CH:=_ASCII_CH-128;
      end;

      if(_ASCII_CH=64) then
      begin
         _ASCII_CH:=0;
      end;

      returnvalue(_ASCII_CH);

   end;

    // put cursor n home position 0,0
    procedure cursor_home();
    begin
        move_to(0,0);
    end;

	procedure wait_vsync();
	begin


	end;

	// Put a character at current cursor location
	procedure put_ch(CH:byte);
	begin
		_A:=CH;
		call(^$FFD2);

	end;

	// Clear keyboard buffer
	procedure clear_buffer();
	begin
		_X:=0;
		poke(^$9E,0,0);
		poke(^$26F,0,0);
	end;


	function get_key():byte;
	var
	
		_keyp: byte=0;
	begin
		_keyp:=0;

		// Read the buffer
		_keyp:=peek(^$26F,0);
		clear_buffer();

		returnvalue(_keyp);
	end;	

	procedure wait_key();
	var
		tmp_key_count:byte=0;
	begin

		
		while(tmp_key_count=0) do
		begin
			tmp_key_count:=peek(^$9E,0);
		end;	

		tmp_key_count:=0;
		clear_buffer();
	end;

	procedure beep();
	var

	begin

	end;	

	//CURSOR_ONN
	procedure cursor_on();
	begin

		poke(^$A7,0,0);


	end;


	//CURSOR_OFF
	procedure cursor_off();
	begin

		poke(^$A7,0,1);


	end;


	function get_char_at(_col,_row:byte):byte;
	var
		_char: byte=0;
		_line_addr: pointer;
	begin


		_line_addr:=ytab[_row];
		_char:=_line_addr[_col];

		returnvalue(_char);
	end;

	procedure put_char_at(_atx,_aty,_atchar:byte);
	begin

		screen_memory:=ytab[_aty];
		screen_memory[_atx]:=ascii_to_petscii(_atchar);

	end;

	procedure cursor_return();
	begin

		_A:=13;
		call(^$FFD2);

	end;

	procedure cursor_down();
	begin

		_A:=17;
		call(^$FFD2);

	end;

	procedure cursor_graphic();
	begin

		_A:=142;
		call(^$FFD2);

	end;

	procedure print_string(in_str: global pointer, CRLF: global byte = True);
	var
		next_ch: byte;
	begin

		next_ch:=0;
		while(in_str[next_ch]<>0) do
		begin 
			_A:=in_str[next_ch];
			call(^$FFD2);
			inc(next_ch);
		end;
		
		if(CRLF) then
		begin
			cursor_return();
			//cursor_down();
		end;
	end;
	
	

   function get_string():integer;
   var
	_input_key, bp: byte;
	input_buffer: string=("                                                      ",0);
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
	
	txt::cursor_off();
	returnvalue(p_input_buffer);

   end;






