

	procedure cls();
	begin
		_A:=147;
		call(^$FFD2);
		DefineScreen();
		

	end;	

	procedure DefineScreen();
	var
	

	begin

		if(ytab[0]<>0) then
		begin
			return;
		end;

		if(temp_address_p=0) then
		begin 
			temp_address_p:=$1E00;
			ClearScreen(1, ^$9600);
		end
		else
		begin
			ClearScreen(1, ^$9400);
		end;

		fori i:= 0 to 23 do
		begin
			fillfast(temp_address_p,32,22);
			ytab[i]:=temp_address_p;
			temp_address_p:=temp_address_p+22;					
		end;


	end;

	procedure move_to(_text_x: byte, _text_y: byte);
	var

	begin
		asm("	clc");
		_Y:=_text_x;
		_X:=_text_y;
		call(^$FFF0)
	end;	


   function ascii_to_petscii(_ASCII_CH:byte):byte;
   begin

      if(_ASCII_CH >=65 and _ASCII_CH<=90) then
      begin
         _ASCII_CH:=_ASCII_CH-64;
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
		_A:=(CH);
		call(^$FFD2);

	end;

	// Clear keyboard buffer
	procedure clear_buffer();
	begin
		
		poke(^198,0,0);
	end;


	function get_key():byte;
	var
		ink:byte=0;
	begin
		_A:=0;
		ink:=0;
		while(ink=0) do
		begin
			ink:=peek(^198,0);
		end;
		call(^$E5CF);
		ink:=_A;
		returnvalue((ink));
	end;	

	procedure wait_key();
	var
		tmp_key:byte=0;
	begin

		tmp_key:=0;
		while(tmp_key=0) do
		begin
			tmp_key:=peek(^198,0);
		end;	

		clear_buffer();
	end;

	procedure beep();
	var

	begin

	end;	

	//CURSOR_ON
	procedure cursor_on();
	begin

		poke(^$CC,0,0);


	end;


	//CURSOR_OFF
	procedure cursor_off();
	begin

		poke(^$CC,0,1);


	end;


	function get_char_at(_col,_row:byte):byte;
	var
		_char: byte=0;
		_line_addr: pointer;
	begin


		_line_addr:=ytab[_row];
		_char:=_line_addr[_col];

		returnvalue(ascii_to_petscii(_char));
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

	procedure print_string(in_str: global pointer, CRLF: global byte);
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






