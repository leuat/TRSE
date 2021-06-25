



	procedure cls();
	begin
		DefineScreen();
		poke(^$C00F,0,1);
		call(^$FC58);
		poke(^$C30F,0,$0E);
	end;

	// set x, y of cursor
	procedure move_to(_text_x:byte; _text_y:byte);
	begin
		poke(^$24,0,_text_x);
		poke(^$25,0,_text_y);
		call(^$FC22);
	end;

	// put cursor n home position 0,0
	procedure cursor_home();
	begin
		move_to(0,0);
	end;


	// Put a character at current cursor location
	procedure put_ch(CH:byte);
	begin
		togglebit(CH,7,1);
		_A:=CH;
		call(^$FDED);
	end;

	// Get a key press
	function get_key():byte;
	var
		_keyp:byte;
	begin
		_keyp:=0;
		while(_keyp<128) do
		begin
			_keyp:=peek(^$C000,0);					
		end;
		
		clear_buffer();	

		returnvalue(_keyp-128);
	end;

	procedure wait_key();
	begin
		while(get_key()=0) do
		begin
			//
		end;	
	end;

	// Clear keyboard buffer
	procedure clear_buffer();
	begin
		peek(^$C010,0);
	end;


	// Print a line to screen
	procedure print_string(in_str:global pointer, CRLF: global byte = True);
	var
		_chx,_this_ch:byte;
	begin

		for _chx:=0 to 255 do
		begin
			_this_ch:=in_str[_chx];
			if(_this_ch>0) then 
			begin
				put_ch(_this_ch+128);
			end
			else
			begin
				break;
			end;
		end;

		if(CRLF) then
		begin
			put_ch(13);
			
		end;

	end;

	procedure beep();
	var
	begin
		call(^$FF3A);
	end;


	//CURSOR_OFF
	procedure cursor_off();
	begin

		poke(^$C30F,0,$06);
		
	end;

	//CURSOR_ON
	procedure cursor_on();
	begin

		poke(^$C30F,0,$05);
		
	end;

	procedure DefineScreen();
	var
		
		temp_address: array[25] of integer;
		y:byte;
	begin

		if(ytab[0]<>0) then
		begin
			return;
		end;

		temp_address_p:=1024;

		fori y:= 0 to 24 do
		begin

			temp_address[y]:=temp_address_p;
			
			//hole
			if(mod(y+1,3)=0) then
			begin
				temp_address_p:=temp_address_p+48;
			end
			else
			begin
				temp_address_p:=temp_address_p+40;			
			end;


		end;


		ytab[0]:=temp_address[0];
		ytab[1]:=temp_address[3];
		ytab[2]:=temp_address[6];
		ytab[3]:=temp_address[9];
		ytab[4]:=temp_address[12];
		ytab[5]:=temp_address[15];
		ytab[6]:=temp_address[18];
		ytab[7]:=temp_address[21];

		
		ytab[8]:=temp_address[1];
		ytab[9]:=temp_address[4];
		ytab[10]:=temp_address[7];
		ytab[11]:=temp_address[10];
		ytab[12]:=temp_address[13];
		ytab[13]:=temp_address[16];
		ytab[14]:=temp_address[19];
		ytab[15]:=temp_address[22];

		
		ytab[16]:=temp_address[2];
		ytab[17]:=temp_address[5];
		ytab[18]:=temp_address[8];
		ytab[19]:=temp_address[11];
		ytab[20]:=temp_address[14];
		ytab[21]:=temp_address[17];
		ytab[22]:=temp_address[20];
		ytab[23]:=temp_address[23];
	end;


	function get_char_at(_col,_row:byte):byte;
	var
		_char: byte=0;
		_line_addr: pointer;
	begin


		_line_addr:=ytab[_row];
		_char:=_line_addr[_col];

		returnvalue(togglebit(_char,7,0));
	end;

	procedure put_char_at(_atx,_aty,_atchar:byte);
	begin

		screen_memory:=ytab[_aty];
		screen_memory[_atx]:=togglebit(_atchar,7,1);

	end;
	
	procedure cursor_return();
	begin
		put_ch(13);
	end;
	
   function get_string():integer;
   var
	_input_key, bp: byte;
	input_buffer: string=("                                                                                              ",0);
	p_input_buffer: pointer;
	   
   begin
   
	bp:=0;
 	_input_key:=0;
 	p_input_buffer:=#input_buffer;
 	
 	cursor_on();
 	
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
