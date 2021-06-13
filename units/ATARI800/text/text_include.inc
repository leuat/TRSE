



	procedure DefineScreen();
	var

		y:byte;
	begin

		temp_address_p:=createinteger(peek(^89,0),peek(^88,0));	

		fori y:= 0 to 24 do
		begin
			fillfast(temp_address_p,0,40);
			ytab[y]:=temp_address_p;
			temp_address_p:=temp_address_p+40;					
		end;
	
	end;

	procedure move_to(_text_x: byte, _text_y: byte);
	var

	begin
		poke(^84,0,_text_y);
		poke(^85,0,_text_x);
		poke(^86,0,0);
	end;	


    // put cursor n home position 0,0
    procedure cursor_home();
    begin
        move_to(0,0);
    end;

	procedure cls();
	begin
		//poke(^766,0,1); //shows edit chars
		poke(^82,0,0); // left margin to zero
		DefineScreen();
		cursor_home();
	end;	

	procedure wait_vsync();
	begin
		
	end;
	

	// Put a character at current cursor location
	procedure put_ch(CH:byte);
	begin
		_A:=CH;
		call(^$F6A4);
	end;

	// Clear keyboard buffer
	procedure clear_buffer();
	begin
		//poke(^764,0,255);
	end;


	function get_key():byte;
	var
		ink:byte=0;
	begin
		call(^$F6E2);
		ink:=_A;
		returnvalue((ink));
	end;	
	

	procedure wait_key();
	begin
		get_key();
	end;

	procedure beep();
	begin	
		put_ch(253);
	end;	

	//CURSOR_ON
	procedure cursor_on();
	begin
		poke(^752,0,0);
	end;


	//CURSOR_OFF
	procedure cursor_off();
	begin
		poke(^752,0,1);
	end;


	function get_char_at(_col,_row:byte):byte;
	var
		_char: byte=0;
		_line_addr: pointer;
	begin
		_line_addr:=ytab[_row];
		_char:=_line_addr[_col];

		returnvalue(_char+32);
	end;

	procedure put_char_at(_atx,_aty,_atchar:byte);
	begin

		screen_memory:=ytab[_aty];
		screen_memory[_atx]:=_atchar-32;

	end;

	procedure cursor_return();
	begin	
		put_ch(155);
	end;

	procedure cursor_down();
	begin
		put_ch(29);
	end;

	procedure cursor_graphic();
	begin
	end;

	procedure print_string(in_str: global pointer, CRLF: global byte);
	var
		next_ch: byte;
	begin

		next_ch:=0;
		while(in_str[next_ch]<>0) do
		begin 
			put_ch(in_str[next_ch]);
			inc(next_ch);
		end;
		
		if(CRLF) then
		begin
			cursor_return();
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
 	
	while (_input_key<>155) do
	begin

		_input_key:=txt::get_key();
		
		if(_input_key > 0 and _input_key < 255) then
		begin
			txt::put_ch(_input_key);
			p_input_buffer[bp]:=_input_key;
			bp:=bp+1;
		end;

		if(_input_key = 126) then
		begin
			txt::put_ch(32);			
			txt::put_ch(30);	
			bp:=bp-2;

		end;

		p_input_buffer[bp]:=0;
	end;
	
	txt::cursor_off();
	returnvalue(p_input_buffer);

   end;






