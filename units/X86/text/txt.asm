	CPU 386
	[ORG 0x100]
	; LineNumber: 587
	jmp block1
	; LineNumber: 5
File_fp: dw  0,0
	; LineNumber: 5
File_tp: dw  0,0
	; LineNumber: 6
File_val:	dw	0
	; LineNumber: 6
File_size:	dw	0
	; LineNumber: 6
File_val2:	dw	0
	; LineNumber: 7
File_src: dw  0,0
	; LineNumber: 7
File_dst: dw  0,0
	; LineNumber: 8
File_attr:	db	0
	; LineNumber: 31
File_File_this: dw  0,0
	; LineNumber: 98
	File_file: times 4 db 0
	; LineNumber: 3
Memory_i1:	dw	0
	; LineNumber: 4
Memory_p1: dw  0,0
	; LineNumber: 5
Screen_i:	dw	0
	; LineNumber: 5
Screen_j:	dw	0
	; LineNumber: 5
Screen_k:	dw	0
	; LineNumber: 5
Screen_noBuffers:	dw	0
	; LineNumber: 6
	Screen_f: times 4 db 0
	; LineNumber: 7
Screen_color:	db	0
	; LineNumber: 7
Screen_red:	db	0
	; LineNumber: 7
Screen_green:	db	0
	; LineNumber: 7
Screen_blue:	db	0
	; LineNumber: 9
Screen_buffer1: dw  0,0
	; LineNumber: 9
Screen_buffer2: dw  0,0
	; LineNumber: 9
Screen_buffer3: dw  0,0
	; LineNumber: 9
Screen_buffer4: dw  0,0
	; LineNumber: 9
Screen_buffer5: dw  0,0
	; LineNumber: 12
Screen_source: dw  0,0
	; LineNumber: 12
Screen_dest: dw  0,0
	; LineNumber: 4
txt_i:	db	0
	; LineNumber: 5
txt_CH:	db	0
	; LineNumber: 6
txt_temp_address_p:	dw	0x00
	; LineNumber: 8
txt_Active_Page_Number:	dw	0
	; LineNumber: 10
txt_ytab:	dw 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	dw 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	dw 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	dw 0
	; LineNumber: 15
txt_screen_memory: dw  0,0
	; LineNumber: 16
txt_max_digits:	dw	0
	; LineNumber: 16
txt_next_digit:	dw	0
	; LineNumber: 17
txt_temp_num_p: dw  0,0
	; LineNumber: 18
txt_temp_num:	dw	0
	; LineNumber: 19
txt_temp_i:	db	0x00
	; LineNumber: 20
txt_this_bbc:	db	0x00
	; LineNumber: 21
txt_in_str: dw  0,0
	; LineNumber: 22
txt_CRLF:	db	0
	; LineNumber: 24
txt_o_cx:	db	0
	; LineNumber: 25
txt_o_cy:	db	0
	
; // TRSE's first proper class
	; ***********  Defining procedure : File_File_Open
	;    Procedure type : User-defined procedure
	; LineNumber: 21
	; LineNumber: 20
	; LineNumber: 20
 ; Temp vars section
txt_cls_stringassignstr271	 db "2J",0
 ; Temp vars section ends
File_File_Open_block2:
File_File_Open:
	; LineNumber: 22
	; ****** Inline assembler section
			mov ah, 0x3D
			mov al, [File_attr] ; open attribute: 0 - read-only, 1 - write-only, 2 -read&write
			mov dx, word [File_fp] ; ASCIIZ filename to open
			int 21h
			mov [File_val],ax
			
	; LineNumber: 29
	; Assigning pointer with index, type:INTEGER right is pointer: 0
	mov ax, [File_val]
	les di, [File_File_this]
	mov [es:di+0x0*1],al
	; LineNumber: 30
	ret
	; ***********  Defining procedure : File_File_Create
	;    Procedure type : User-defined procedure
	; LineNumber: 33
	; LineNumber: 32
File_File_Create_block3:
File_File_Create:
	; LineNumber: 34
	; ****** Inline assembler section
			mov ah, 0x3C
			mov cx,0
			mov dx, word [File_fp] ; ASCIIZ filename to open
			int 21h
			mov [File_val],ax
			
	; LineNumber: 41
	ret
	; ***********  Defining procedure : File_File_Read
	;    Procedure type : User-defined procedure
	; LineNumber: 46
	; LineNumber: 45
	; LineNumber: 45
File_File_Read_block4:
File_File_Read:
	; LineNumber: 47
	les di,[File_File_this]
	mov ax, word [es:di + 0x0]
	mov ah,0
	mov [File_val], ax
	; LineNumber: 48
	; ****** Inline assembler section
			cli
			mov ah, 0x3F
			mov bx, [File_val]
			mov cx, [File_size] ; number of bytes to read
			mov dx, [File_dst] ; were to put read data
			push ds
			lds dx,[File_dst]	
			int 21h	
			mov [File_val2],ax
			pop ds
			sti
			
	; LineNumber: 61
	; Assigning pointer with index, type:INTEGER right is pointer: 0
	mov ax, [File_val2]
	mov [es:di+0x2*1],al
	; LineNumber: 62
	ret
	; ***********  Defining procedure : File_File_Write
	;    Procedure type : User-defined procedure
	; LineNumber: 65
	; LineNumber: 64
	; LineNumber: 64
File_File_Write_block5:
File_File_Write:
	; LineNumber: 66
	les di,[File_File_this]
	mov ax, word [es:di + 0x0]
	mov ah,0
	mov [File_val], ax
	; LineNumber: 67
	; ****** Inline assembler section
			cli
			mov ah, 0x40
			mov bx, [File_val]
			mov cx, [File_size] ; number of bytes to read
			mov dx, [File_src] ; were to put read data
			;push ds
			;lds dx,[File_src]	
			int 21h	
			mov [File_val2],ax
			;pop ds
			sti
			
	; LineNumber: 80
	; Assigning pointer with index, type:INTEGER right is pointer: 0
	mov ax, [File_val2]
	mov [es:di+0x2*1],al
	; LineNumber: 81
	ret
	; ***********  Defining procedure : File_File_Close
	;    Procedure type : User-defined procedure
	; LineNumber: 84
File_File_Close:
	; LineNumber: 85
	les di,[File_File_this]
	mov ax, word [es:di + 0x0]
	mov ah,0
	mov [File_val], ax
	; LineNumber: 86
	; ****** Inline assembler section
			mov ah, 0x3E
			mov bx, [File_val] ; file handle
			int 21h	
			
	; LineNumber: 91
	ret
	; ***********  Defining procedure : File_ReadAll
	;    Procedure type : User-defined procedure
	; LineNumber: 103
	; LineNumber: 102
	; LineNumber: 102
File_ReadAll_block7:
File_ReadAll:
	; LineNumber: 104
	; Assigning pointer
	lea si, [File_file]
	mov [File_File_this+2], ds
	mov [File_File_this], si
	mov [File_attr], byte 0x0
	call File_File_Open
	; LineNumber: 105
	; Assigning pointer
	lea si, [File_file]
	mov [File_File_this+2], ds
	mov [File_File_this], si
	mov [File_size], word 0xfa00
	; Assigning pointer
	les di, [File_tp]
	mov [File_dst+2], es
	mov [File_dst], di
	call File_File_Read
	; LineNumber: 106
	; Assigning pointer
	lea si, [File_file]
	mov [File_File_this+2], ds
	mov [File_File_this], si
	call File_File_Close
	; LineNumber: 107
	ret
	; LineNumber: 11
_bx:	dw	0
	; LineNumber: 11
_di:	dw	0
	; LineNumber: 11
_ah:	db	0
	; ***********  Defining procedure : Memory_SetTimer
	;    Procedure type : User-defined procedure
	; LineNumber: 21
	; LineNumber: 20
Memory_ticks:	dw	0
Memory_SetTimer_block8:
Memory_SetTimer:
	; LineNumber: 22
	; ****** Inline assembler section
	cli 
	; LineNumber: 23
	; ****** Inline assembler section
  	  mov al,0x36
  	  out 0x43,al
  	  mov dx,0x40
  	  mov ax,[Memory_ticks]
  	  out 0x40,al
  	  mov al,ah
  	  out 0x40,al
  
	; LineNumber: 32
	; ****** Inline assembler section
	sti 
	; LineNumber: 33
	ret
	; ***********  Defining procedure : Memory_Init
	;    Procedure type : User-defined procedure
	; LineNumber: 37
Memory_Init:
	; LineNumber: 40
	; ****** Inline assembler section
	mov	dx,ss		; Stack segment
	mov	bx,128 / 16 + 1 ; stack size in paragraphs
	add	bx,dx		; BX = end
	mov	ax,es		; ES = PSP(start)
	sub	bx,ax		; BX = new size in paragraphs
	mov	ah,0x4A
	int	21h
	; Allocate some memory up to 1024*2
	mov ah,0x48
	mov bx, 0x2000-0x160-0x3D
	int 0x21
	
	; LineNumber: 56
	ret
	
; // 0x2000
	; ***********  Defining procedure : Memory_GetBlockMem
	;    Procedure type : User-defined procedure
	; LineNumber: 59
	; LineNumber: 58
Memory_blocks:	dw	0
Memory_GetBlockMem_block10:
Memory_GetBlockMem:
	; LineNumber: 62
	; ****** Inline assembler section
	mov ah,0x48
		mov bx, [Memory_blocks]
		int 0x21
		mov [Memory_i1],ax
	
	; LineNumber: 68
	; LineNumber: 73
	mov ax, [Memory_i1]
	mov es,ax
	mov ax, 0x0
	mov di,ax
	ret
	; ***********  Defining procedure : Memory_GetMem
	;    Procedure type : User-defined procedure
	; LineNumber: 76
	; LineNumber: 75
Memory_GetMem_block11:
Memory_GetMem:
	; LineNumber: 78
	
; // Bug?
	mov ax, [Memory_blocks]
	xor dx,dx
	mov bx, 0x4
	div  bx
	mov [Memory_blocks], ax
	; LineNumber: 79
	; Binary clause Simplified: EQUALS
	; Compare with pure num / var optimization
	cmp [Memory_blocks],word 0x0
	jne Memory_GetMem_elsedoneblock15
Memory_GetMem_ConditionalTrueBlock13: ;Main true block ;keep :
	; LineNumber: 78
	mov [Memory_blocks], word 0x1
Memory_GetMem_elsedoneblock15:
	; LineNumber: 84
	; ****** Inline assembler section
	mov ah,0x48
		mov bx, [Memory_blocks]
		int 0x21
		mov [Memory_i1],ax
	
	; LineNumber: 90
	; LineNumber: 97
	mov ax, [Memory_i1]
	mov es,ax
	mov ax, 0x0
	mov di,ax
	ret
	
; //	blocks+=1;
	; ***********  Defining procedure : Memory_Release
	;    Procedure type : User-defined procedure
	; LineNumber: 100
	; LineNumber: 99
Memory_rp: dw  0,0
Memory_Release_block18:
Memory_Release:
	; LineNumber: 103
	; ****** Inline assembler section
	mov ah,0x49
		mov bx, [Memory_rp+2]
		int 0x21
	
	; LineNumber: 108
	ret
	; ***********  Defining procedure : Screen_InitVGABuffers
	;    Procedure type : User-defined procedure
	; LineNumber: 17
	; LineNumber: 16
Screen_InitVGABuffers_block19:
Screen_InitVGABuffers:
	; LineNumber: 18
	; Binary clause Simplified: GREATER
	; Compare with pure num / var optimization
	cmp [Screen_noBuffers],word 0x0
	jbe Screen_InitVGABuffers_elsedoneblock23
Screen_InitVGABuffers_ConditionalTrueBlock21: ;Main true block ;keep :
	; LineNumber: 18
	; Assigning pointer
	; Setting PURE POINTER 0
	mov [Memory_blocks], word 0xfff
	call Memory_GetBlockMem
	; Setting PURE POINTER ends
	mov [Screen_buffer1+2], es
	mov [Screen_buffer1], di
Screen_InitVGABuffers_elsedoneblock23:
	; LineNumber: 20
	; Binary clause Simplified: GREATER
	; Compare with pure num / var optimization
	cmp [Screen_noBuffers],word 0x1
	jbe Screen_InitVGABuffers_elsedoneblock29
Screen_InitVGABuffers_ConditionalTrueBlock27: ;Main true block ;keep :
	; LineNumber: 20
	; Assigning pointer
	; Setting PURE POINTER 0
	mov [Memory_blocks], word 0xfff
	call Memory_GetBlockMem
	; Setting PURE POINTER ends
	mov [Screen_buffer2+2], es
	mov [Screen_buffer2], di
Screen_InitVGABuffers_elsedoneblock29:
	; LineNumber: 22
	; Binary clause Simplified: GREATER
	; Compare with pure num / var optimization
	cmp [Screen_noBuffers],word 0x2
	jbe Screen_InitVGABuffers_elsedoneblock35
Screen_InitVGABuffers_ConditionalTrueBlock33: ;Main true block ;keep :
	; LineNumber: 22
	; Assigning pointer
	; Setting PURE POINTER 0
	mov [Memory_blocks], word 0xfff
	call Memory_GetBlockMem
	; Setting PURE POINTER ends
	mov [Screen_buffer3+2], es
	mov [Screen_buffer3], di
Screen_InitVGABuffers_elsedoneblock35:
	; LineNumber: 24
	; Binary clause Simplified: GREATER
	; Compare with pure num / var optimization
	cmp [Screen_noBuffers],word 0x3
	jbe Screen_InitVGABuffers_elsedoneblock41
Screen_InitVGABuffers_ConditionalTrueBlock39: ;Main true block ;keep :
	; LineNumber: 24
	; Assigning pointer
	; Setting PURE POINTER 0
	mov [Memory_blocks], word 0xfff
	call Memory_GetBlockMem
	; Setting PURE POINTER ends
	mov [Screen_buffer4+2], es
	mov [Screen_buffer4], di
Screen_InitVGABuffers_elsedoneblock41:
	; LineNumber: 26
	; Binary clause Simplified: GREATER
	; Compare with pure num / var optimization
	cmp [Screen_noBuffers],word 0x4
	jbe Screen_InitVGABuffers_elsedoneblock47
Screen_InitVGABuffers_ConditionalTrueBlock45: ;Main true block ;keep :
	; LineNumber: 26
	; Assigning pointer
	; Setting PURE POINTER 0
	mov [Memory_blocks], word 0xfff
	call Memory_GetBlockMem
	; Setting PURE POINTER ends
	mov [Screen_buffer5+2], es
	mov [Screen_buffer5], di
Screen_InitVGABuffers_elsedoneblock47:
	; LineNumber: 28
	ret
	; ***********  Defining procedure : Screen_InitVGABuffersNo21
	;    Procedure type : User-defined procedure
	; LineNumber: 31
	; LineNumber: 30
Screen_InitVGABuffersNo21_block50:
Screen_InitVGABuffersNo21:
	; LineNumber: 32
	; Binary clause Simplified: GREATER
	; Compare with pure num / var optimization
	cmp [Screen_noBuffers],word 0x0
	jbe Screen_InitVGABuffersNo21_elsedoneblock54
Screen_InitVGABuffersNo21_ConditionalTrueBlock52: ;Main true block ;keep :
	; LineNumber: 32
	; Assigning pointer
	; Setting PURE POINTER 0
	mov ax, 0x2000
	mov es,ax
	mov ax, 0x0
	mov di,ax
	; Setting PURE POINTER ends
	mov [Screen_buffer1+2], es
	mov [Screen_buffer1], di
Screen_InitVGABuffersNo21_elsedoneblock54:
	; LineNumber: 34
	; Binary clause Simplified: GREATER
	; Compare with pure num / var optimization
	cmp [Screen_noBuffers],word 0x1
	jbe Screen_InitVGABuffersNo21_elsedoneblock60
Screen_InitVGABuffersNo21_ConditionalTrueBlock58: ;Main true block ;keep :
	; LineNumber: 34
	; Assigning pointer
	; Setting PURE POINTER 0
	mov ax, 0x3000
	mov es,ax
	mov ax, 0x0
	mov di,ax
	; Setting PURE POINTER ends
	mov [Screen_buffer2+2], es
	mov [Screen_buffer2], di
Screen_InitVGABuffersNo21_elsedoneblock60:
	; LineNumber: 36
	; Binary clause Simplified: GREATER
	; Compare with pure num / var optimization
	cmp [Screen_noBuffers],word 0x2
	jbe Screen_InitVGABuffersNo21_elsedoneblock66
Screen_InitVGABuffersNo21_ConditionalTrueBlock64: ;Main true block ;keep :
	; LineNumber: 36
	; Assigning pointer
	; Setting PURE POINTER 0
	mov ax, 0x4000
	mov es,ax
	mov ax, 0x0
	mov di,ax
	; Setting PURE POINTER ends
	mov [Screen_buffer3+2], es
	mov [Screen_buffer3], di
Screen_InitVGABuffersNo21_elsedoneblock66:
	; LineNumber: 38
	; Binary clause Simplified: GREATER
	; Compare with pure num / var optimization
	cmp [Screen_noBuffers],word 0x3
	jbe Screen_InitVGABuffersNo21_elsedoneblock72
Screen_InitVGABuffersNo21_ConditionalTrueBlock70: ;Main true block ;keep :
	; LineNumber: 38
	; Assigning pointer
	; Setting PURE POINTER 0
	mov ax, 0x5000
	mov es,ax
	mov ax, 0x0
	mov di,ax
	; Setting PURE POINTER ends
	mov [Screen_buffer4+2], es
	mov [Screen_buffer4], di
Screen_InitVGABuffersNo21_elsedoneblock72:
	; LineNumber: 40
	; Binary clause Simplified: GREATER
	; Compare with pure num / var optimization
	cmp [Screen_noBuffers],word 0x4
	jbe Screen_InitVGABuffersNo21_elsedoneblock78
Screen_InitVGABuffersNo21_ConditionalTrueBlock76: ;Main true block ;keep :
	; LineNumber: 40
	; Assigning pointer
	; Setting PURE POINTER 0
	mov ax, 0x6000
	mov es,ax
	mov ax, 0x0
	mov di,ax
	; Setting PURE POINTER ends
	mov [Screen_buffer5+2], es
	mov [Screen_buffer5], di
Screen_InitVGABuffersNo21_elsedoneblock78:
	; LineNumber: 42
	ret
	; ***********  Defining procedure : Screen_SetColor
	;    Procedure type : User-defined procedure
	; LineNumber: 45
	; LineNumber: 44
	; LineNumber: 44
	; LineNumber: 44
	; LineNumber: 44
Screen_SetColor_block81:
Screen_SetColor:
	; LineNumber: 47
	; ****** Inline assembler section
	Cli
    mov dx,0x3c8
    mov al,[Screen_color]
    out dx,al
    inc dx
    mov al,[Screen_red]
    out dx,al
    mov al,[Screen_green]
    out dx,al
    mov al,[Screen_blue]
    out dx,al
    Sti
   
	; LineNumber: 61
	ret
	; ***********  Defining procedure : Screen_SetPaletteFromFile
	;    Procedure type : User-defined procedure
	; LineNumber: 69
	; LineNumber: 67
Screen_colbuf:	 times 7680 db 0
	; LineNumber: 68
Screen_tp: dw  0,0
	; LineNumber: 64
Screen_fp: dw  0,0
Screen_SetPaletteFromFile_block82:
Screen_SetPaletteFromFile:
	; LineNumber: 70
	; Assigning pointer
	lea si, [Screen_colbuf]
	mov [Screen_tp+2], ds
	mov [Screen_tp], si
	; LineNumber: 75
	;	handle := File::Open(fp);
; //	File::Read(handle, 0x300, #colbuf);
; //	File::Close(handle);
; //
	; Assigning pointer
	lea si, [Screen_f]
	mov [File_File_this+2], ds
	mov [File_File_this], si
	; Assigning pointer
	les di, [Screen_fp]
	mov [File_fp+2], es
	mov [File_fp], di
	mov [File_attr], byte 0x0
	call File_File_Open
	; LineNumber: 76
	; Assigning pointer
	lea si, [Screen_f]
	mov [File_File_this+2], ds
	mov [File_File_this], si
	mov [File_size], word 0x300
	; Assigning pointer
	lea si, [Screen_colbuf]
	mov [File_dst+2], ds
	mov [File_dst], si
	call File_File_Read
	; LineNumber: 77
	; Assigning pointer
	lea si, [Screen_f]
	mov [File_File_this+2], ds
	mov [File_File_this], si
	call File_File_Close
	; LineNumber: 81
	mov [Screen_i], word 0x0
Screen_SetPaletteFromFile_forloop83:
	; LineNumber: 79
	; LineNumber: 80
	; Assigning pointer with index, type:NADA right is pointer: 1
	les di,[Screen_tp]
	add di,word [Screen_i]
	mov ax, word [es:di]
	mov ah,0
	xor ah,ah
	mov bl, 0x4
	div  bl
	les di, [Screen_tp]
	add di,[Screen_i]
	mov [es:di],al
	; LineNumber: 81
Screen_SetPaletteFromFile_forloopcounter85:
Screen_SetPaletteFromFile_loopstart86:
	add [Screen_i],word 1
	mov ax, 0x300
	cmp ax,word [Screen_i]
	jne Screen_SetPaletteFromFile_forloop83
Screen_SetPaletteFromFile_forloopend84:
Screen_SetPaletteFromFile_loopend87:
	; LineNumber: 82
	mov [Screen_j], word 0x0
	; LineNumber: 88
	mov [Screen_i], word 0x0
Screen_SetPaletteFromFile_forloop90:
	; LineNumber: 84
	; LineNumber: 85
	mov ax, [Screen_i]
	mov [Screen_color], al
	les di,[Screen_tp]
	add di,word [Screen_j]
	mov ax, word [es:di]
	mov ah,0
	mov [Screen_red], al
	les di,[Screen_tp]
	; RHS is pure optimization
	mov ax, [Screen_j]
	; Forcetype IS POINTER: 0
	add  ax, 0x1
	add di,ax
	mov al, byte [es:di]; Is byte
	mov [Screen_green], al
	les di,[Screen_tp]
	; RHS is pure optimization
	mov ax, [Screen_j]
	; Forcetype IS POINTER: 0
	add  ax, 0x2
	add di,ax
	mov al, byte [es:di]; Is byte
	mov [Screen_blue], al
	call Screen_SetColor
	; LineNumber: 86
	; 'a:=a + const'  optimization 
	add [Screen_j], word 0x3
	; LineNumber: 87
Screen_SetPaletteFromFile_forloopcounter92:
Screen_SetPaletteFromFile_loopstart93:
	add [Screen_i],word 1
	mov ax, 0x100
	cmp ax,word [Screen_i]
	jne Screen_SetPaletteFromFile_forloop90
Screen_SetPaletteFromFile_forloopend91:
Screen_SetPaletteFromFile_loopend94:
	; LineNumber: 89
	ret
	; ***********  Defining procedure : Screen_CopyVGABuffer32bit
	;    Procedure type : User-defined procedure
	; LineNumber: 94
	; LineNumber: 93
	; LineNumber: 93
Screen_CopyVGABuffer32bit_block97:
Screen_CopyVGABuffer32bit:
	; LineNumber: 96
	; ****** Inline assembler section
	cld
  	push ds
    les di,[Screen_dest]
    lds si,[Screen_source]
    mov ecx, 16000
    rep movsd
    pop ds
  
	; LineNumber: 104
	ret
	; ***********  Defining procedure : Screen_ClearBuffer32bit
	;    Procedure type : User-defined procedure
	; LineNumber: 108
	; LineNumber: 107
	; LineNumber: 107
_ax:	dw	0
Screen_ClearBuffer32bit_block98:
Screen_ClearBuffer32bit:
	; LineNumber: 110
	; ****** Inline assembler section
	les di,[Screen_dest]
    mov     cx, 16000
    rep stosD
  
	; LineNumber: 114
	ret
	; ***********  Defining procedure : Screen_CopyVGABuffer16bit
	;    Procedure type : User-defined procedure
	; LineNumber: 117
	; LineNumber: 116
	; LineNumber: 116
Screen_CopyVGABuffer16bit_block99:
Screen_CopyVGABuffer16bit:
	; LineNumber: 119
	; ****** Inline assembler section
	push    ds
    les di,[Screen_dest]
    lds si,[Screen_source]
    
    mov     cx, 32000
    Rep MovsW
    pop     ds
  
	; LineNumber: 127
	ret
	; ***********  Defining procedure : Screen_SetMode
	;    Procedure type : User-defined procedure
	; LineNumber: 131
	; LineNumber: 130
Screen_SetMode_block100:
Screen_SetMode:
	; LineNumber: 132
	; ****** Inline assembler section
		mov ax,[Screen_i]
		int 0x10
	
	; LineNumber: 136
	ret
	; ***********  Defining procedure : Screen_PrintChar
	;    Procedure type : User-defined procedure
	; LineNumber: 139
	; LineNumber: 138
Screen_ch:	db	0
Screen_PrintChar_block101:
Screen_PrintChar:
	; LineNumber: 141
	; ****** Inline assembler section
	mov	al,[Screen_ch]
		mov	ah,0x0E
		int	10h
	
	
	; LineNumber: 146
	ret
	; ***********  Defining procedure : Screen_Print
	;    Procedure type : User-defined procedure
	; LineNumber: 150
	; LineNumber: 149
Screen_zp: dw  0,0
Screen_Print_block102:
Screen_Print:
	; LineNumber: 151
Screen_Print_while103:
Screen_Print_loopstart107:
	; Binary clause Simplified: NOTEQUALS
	; Compare with pure num / var optimization
	les di,[Screen_zp]
	mov ax, word [es:di + 0x0]
	mov ah,0
	cmp al,0x0
	je Screen_Print_elsedoneblock106
Screen_Print_ConditionalTrueBlock104: ;Main true block ;keep :
	; LineNumber: 152
	; LineNumber: 153
	les di,[Screen_zp]
	mov ax, word [es:di + 0x0]
	mov ah,0
	mov [Screen_ch], al
	call Screen_PrintChar
	; LineNumber: 154
	; 'a:=a + const'  optimization 
	add [Screen_zp], word 0x1
	; LineNumber: 155
	jmp Screen_Print_while103
Screen_Print_elsedoneblock106:
Screen_Print_loopend108:
	; LineNumber: 156
	ret
	; ***********  Defining procedure : Screen_NewLine
	;    Procedure type : User-defined procedure
	; LineNumber: 159
Screen_NewLine:
	; LineNumber: 160
	mov [Screen_ch], byte 0xa
	call Screen_PrintChar
	; LineNumber: 161
	mov [Screen_ch], byte 0xd
	call Screen_PrintChar
	; LineNumber: 162
	ret
	; ***********  Defining procedure : Screen_PrintAx
	;    Procedure type : User-defined procedure
	; LineNumber: 167
Screen_PrintAx:
	; LineNumber: 169
	; ****** Inline assembler section
	push ds
	pusha
	mov bx,0x192
	mov ds,bx
printAx:		push	cx
			mov	cx,4
.printDigit:		
			rol	ax,4
			push	ax
			movzx	eax,al
			and	al,0xf
			mov	al,[eax+.higits]
			mov	ah,0x0E
			int	10h
			pop	ax
			loop	.printDigit
			pop	cx
			
			popa
			pop ds
			ret
.higits:		db	"0","1","2","3","4","5","6","7"
			db	"8","9","a","b","c","d","e","f"
	; LineNumber: 195
	ret
	; ***********  Defining procedure : Screen_PrintInt
	;    Procedure type : User-defined procedure
	; LineNumber: 199
	; LineNumber: 198
Screen_PrintInt_block113:
Screen_PrintInt:
	; LineNumber: 201
	; ****** Inline assembler section
	mov ax,[Screen_i]
	
	; LineNumber: 203
	call Screen_PrintAx
	; LineNumber: 205
	ret
	
; // PC cursors		        *
; //	
; //	Put a character at current cursor position
; //	
; //	*
	; ***********  Defining procedure : txt_put_ch
	;    Procedure type : User-defined procedure
	; LineNumber: 40
	; LineNumber: 39
txt_put_ch_block114:
txt_put_ch:
	; LineNumber: 41
	; Assigning to register
	mov ah, 0xe
	; LineNumber: 42
	; Assigning to register
	mov al, [txt_CH]
	; LineNumber: 43
	; Assigning to register
	mov bh, 0x0
	; LineNumber: 45
	; ****** Inline assembler section
	    	int 0x10
	    
	; LineNumber: 48
	ret
	;*
; //	
; //	Helper to set up screen pointers etc
; //	
; //	*
	; ***********  Defining procedure : txt_DefineScreen
	;    Procedure type : User-defined procedure
	; LineNumber: 56
txt_DefineScreen:
	; LineNumber: 57
	mov [Screen_i], word 0x1
	call Screen_SetMode
	; LineNumber: 58
	ret
	;*
; //
; //	 Escape sequence
; //    
; //    *
	; ***********  Defining procedure : txt_esc
	;    Procedure type : User-defined procedure
	; LineNumber: 68
txt_esc:
	; LineNumber: 69
	mov [txt_CH], byte 0x1b
	call txt_put_ch
	; LineNumber: 70
	mov [txt_CH], byte 0x5b
	call txt_put_ch
	; LineNumber: 71
	ret
	;*
; //
; //	Place the cursor at 0,0 top left of screen
; //	
; //	*
	; ***********  Defining procedure : txt_cursor_home
	;    Procedure type : User-defined procedure
	; LineNumber: 81
txt_cursor_home:
	; LineNumber: 82
	; Assigning to register
	mov ah, 0x2
	; LineNumber: 83
	; Assigning to register
	mov bh, 0x0
	; LineNumber: 84
	; Assigning to register
	mov dh, 0x0
	; LineNumber: 85
	; Assigning to register
	mov dl, 0x0
	; LineNumber: 86
	; ****** Inline assembler section
	    	int 0x10
	    
	; LineNumber: 90
	ret
	;*
; //	
; //	Place the cursor at X, Y screen position
; //	
; //	*
	; ***********  Defining procedure : txt_move_to
	;    Procedure type : User-defined procedure
	; LineNumber: 98
	; LineNumber: 97
txt__text_x:	db	0
	; LineNumber: 97
txt__text_y:	db	0
txt_move_to_block118:
txt_move_to:
	; LineNumber: 99
	; Assigning to register
	mov ah, 0x2
	; LineNumber: 100
	; Assigning to register
	mov bh, 0x0
	; LineNumber: 101
	; Assigning to register
	mov dh, [txt__text_y]
	; LineNumber: 102
	; Assigning to register
	mov dl, [txt__text_x]
	; LineNumber: 103
	; ****** Inline assembler section
	    	int 0x10
	    
	; LineNumber: 107
	ret
	;*
; //	
; //	Wait for vertical blank(not yet implemented)
; //	
; //	*
	; ***********  Defining procedure : txt_wait_vsync
	;    Procedure type : User-defined procedure
	; LineNumber: 115
txt_wait_vsync:
	; LineNumber: 116
	mov dx, 0x3da
txt_wait_vsync_vblank1120:
	in al, dx
	test al, 8
	je txt_wait_vsync_vblank1120
txt_wait_vsync_vblank2121:
	in al, dx
	test al, 8
	jne txt_wait_vsync_vblank2121
	; LineNumber: 117
	ret
	;*
; //	
; //	Set the text colour(not available on all platforms)	
; //	
; //	*
	; ***********  Defining procedure : txt_text_colour
	;    Procedure type : User-defined procedure
	; LineNumber: 125
	; LineNumber: 124
txt__chosen_text_colour:	db	0
txt_text_colour_block122:
txt_text_colour:
	; LineNumber: 127
	ret
	;*
; //
; //	Get a character input from the keyboard	
; //	
; //	*
	; ***********  Defining procedure : txt_get_key
	;    Procedure type : User-defined procedure
	; LineNumber: 137
txt_get_key:
	; LineNumber: 138
	; Assigning to register
	mov ah, 0x0
	; LineNumber: 140
	; ****** Inline assembler section
	    
	    	int 0x16
	    	mov [txt_CH],AL
	    	
	    
	; LineNumber: 149
	; LineNumber: 149
	mov al, [txt_CH]
	ret
	
; // AL=ascii AH=scan code*
; //
; //	Get a character input from the keyboard	
; //	
; //	*
	; ***********  Defining procedure : txt_get_scancode
	;    Procedure type : User-defined procedure
	; LineNumber: 158
txt_get_scancode:
	; LineNumber: 159
	; Assigning to register
	mov ah, 0x0
	; LineNumber: 161
	; ****** Inline assembler section
	    
	    	int 0x16
	    	mov [txt_CH],AH
	    	
	    
	; LineNumber: 170
	; LineNumber: 170
	mov al, [txt_CH]
	ret
	
; // AL=ascii AH=scan code*
; //	
; //	Wait for a key press	
; //	
; //	*
	; ***********  Defining procedure : txt_wait_key
	;    Procedure type : User-defined procedure
	; LineNumber: 180
txt_wait_key:
	; LineNumber: 181
	call txt_get_key
	; LineNumber: 182
	ret
	;*
; //
; //	Clear the keyboard buffer	
; //	
; //	*
	; ***********  Defining procedure : txt_clear_buffer
	;    Procedure type : User-defined procedure
	; LineNumber: 189
txt_clear_buffer:
	; LineNumber: 191
	ret
	;*
; //    
; //    Get current cursor position to o_cx,o_cy
; //	
; //	*
	; ***********  Defining procedure : txt_get_cursor_position
	;    Procedure type : User-defined procedure
	; LineNumber: 200
txt_get_cursor_position:
	; LineNumber: 201
	; Assigning to register
	mov ah, 0x3
	; LineNumber: 202
	; Assigning to register
	mov bh, 0x0
	; LineNumber: 203
	; ****** Inline assembler section
	    	int 0x10
	    	mov [txt_o_cx],dl
	    	mov [txt_o_cy],dh
	    
	; LineNumber: 208
	ret
	; ***********  Defining procedure : txt_put_char_at
	;    Procedure type : User-defined procedure
	; LineNumber: 216
	; LineNumber: 215
txt__atx:	db	0
	; LineNumber: 215
txt__aty:	db	0
	; LineNumber: 215
txt__atchar:	db	0
txt_put_char_at_block128:
txt_put_char_at:
	; LineNumber: 218
	call txt_get_cursor_position
	; LineNumber: 219
	mov al, [txt__atx]
	mov [txt__text_x], al
	mov al, [txt__aty]
	mov [txt__text_y], al
	call txt_move_to
	; LineNumber: 220
	mov al, [txt__atchar]
	mov [txt_CH], al
	call txt_put_ch
	; LineNumber: 221
	mov al, [txt_o_cx]
	mov [txt__text_x], al
	mov al, [txt_o_cy]
	mov [txt__text_y], al
	call txt_move_to
	; LineNumber: 223
	ret
	;*
; //
; //	Output a string at the current cursor location.
; //	Set Carriage Return on/off	
; //	
; //	*
	; ***********  Defining procedure : txt_print_string
	;    Procedure type : User-defined procedure
	; LineNumber: 234
	; LineNumber: 231
	; LineNumber: 231
txt_print_string_block129:
txt_print_string:
	; LineNumber: 235
	; Assigning pointer
	les di, [txt_in_str]
	mov [Screen_zp+2], es
	mov [Screen_zp], di
	call Screen_Print
	; LineNumber: 237
	; Binary clause Simplified: NOTEQUALS
	; Compare with pure num / var optimization
	cmp [txt_CRLF],byte 0x0
	je txt_print_string_elsedoneblock133
txt_print_string_ConditionalTrueBlock131: ;Main true block ;keep :
	; LineNumber: 238
	; LineNumber: 239
	mov [txt_CH], byte 0xa
	call txt_put_ch
	; LineNumber: 240
	mov [txt_CH], byte 0xd
	call txt_put_ch
	; LineNumber: 241
txt_print_string_elsedoneblock133:
	; LineNumber: 243
	ret
	; ***********  Defining procedure : txt_get_char_at
	;    Procedure type : User-defined procedure
	; LineNumber: 253
	; LineNumber: 252
txt__col:	db	0
	; LineNumber: 252
txt__row:	db	0
txt_get_char_at_block136:
txt_get_char_at:
	; LineNumber: 254
	call txt_get_cursor_position
	; LineNumber: 255
	mov al, [txt__col]
	mov [txt__text_x], al
	mov al, [txt__row]
	mov [txt__text_y], al
	call txt_move_to
	; LineNumber: 256
	; Assigning to register
	mov ah, 0x8
	; LineNumber: 257
	; Assigning to register
	mov bh, 0x0
	; LineNumber: 259
	; ****** Inline assembler section
	    	int 0x10
	    	mov [txt_CH],AL
	    
	; LineNumber: 265
	; LineNumber: 265
	mov al, [txt_o_cx]
	mov [txt__text_x], al
	mov al, [txt_o_cy]
	mov [txt__text_y], al
	call txt_move_to
	; LineNumber: 266
	mov al, [txt_CH]
	ret
	; ***********  Defining procedure : txt_beep
	;    Procedure type : User-defined procedure
	; LineNumber: 277
txt_beep:
	; LineNumber: 279
	mov [txt_CH], byte 0x7
	call txt_put_ch
	; LineNumber: 282
	ret
	;*
; //	
; //	Output a string representation of a decimal number at current cursor position
; //	Set if you want carriage return true/false
; //	
; //	*
	; ***********  Defining procedure : txt_print_dec
	;    Procedure type : User-defined procedure
	; LineNumber: 292
	; LineNumber: 291
txt__in_n:	db	0
	; LineNumber: 291
txt__add_cr:	db	0
txt_print_dec_block138:
txt_print_dec:
	; LineNumber: 294
	mov ax, [txt__in_n]
	mov ah,0 ; forcetype clear high bit
	mov [Screen_i], ax
	call Screen_PrintInt
	; LineNumber: 295
	; Binary clause Simplified: NOTEQUALS
	; Compare with pure num / var optimization
	cmp [txt_CRLF],byte 0x0
	je txt_print_dec_elsedoneblock142
txt_print_dec_ConditionalTrueBlock140: ;Main true block ;keep :
	; LineNumber: 296
	; LineNumber: 297
	mov [txt_CH], byte 0xa
	call txt_put_ch
	; LineNumber: 298
	mov [txt_CH], byte 0xd
	call txt_put_ch
	; LineNumber: 299
txt_print_dec_elsedoneblock142:
	; LineNumber: 302
	ret
	; ***********  Defining procedure : txt_get_page
	;    Procedure type : User-defined procedure
	; LineNumber: 324
txt_get_page:
	; LineNumber: 326
	
; //BIOS Function Read Video mode
	; Assigning to register
	mov ah, 0xf
	; LineNumber: 328
	; ****** Inline assembler section
	int 0x10
				mov 	[txt_Active_Page_Number],bh
	    
	; LineNumber: 334
	ret
	;*
; //	
; //	Hide flashing cursor
; //	
; //	*
	; ***********  Defining procedure : txt_cursor_off
	;    Procedure type : User-defined procedure
	; LineNumber: 344
txt_cursor_off:
	; LineNumber: 346
	; ****** Inline assembler section
	mov ah,0x01
				mov ch,0x20
				int 0x10 
		
	; LineNumber: 353
	ret
	;*
; //	
; //	Show flashing cursor
; //	
; //	*
	; ***********  Defining procedure : txt_cursor_on
	;    Procedure type : User-defined procedure
	; LineNumber: 363
txt_cursor_on:
	; LineNumber: 365
	; ****** Inline assembler section
	mov ah,0x01
				mov ch,0x06
				mov cl,0x07
				int 0x10 
		
	; LineNumber: 373
	ret
	; ***********  Defining procedure : txt_str_compare
	;    Procedure type : User-defined procedure
	; LineNumber: 381
	; LineNumber: 379
txt_si:	db	0x00
	; LineNumber: 380
txt__match:	db	0x00
	; LineNumber: 377
txt_str1: dw  0,0
	; LineNumber: 377
txt_str2: dw  0,0
txt_str_compare_block148:
txt_str_compare:
	; LineNumber: 382
	mov [txt_si], byte 0x0
	; LineNumber: 383
	mov [txt__match], byte 0x0
	; LineNumber: 385
txt_str_compare_while149:
txt_str_compare_loopstart153:
	; Binary clause Simplified: GREATER
	; Compare with pure num / var optimization
	les di,[txt_str1]
	add di,word [txt_si]
	mov ax, word [es:di]
	mov ah,0
	cmp al,0x0
	jbe txt_str_compare_elsedoneblock152
txt_str_compare_localsuccess164: ;keep:
	; ; logical AND, second requirement
	; Binary clause Simplified: NOTEQUALS
	; Compare with pure num / var optimization
	les di,[txt_str1]
	add di,word [txt_si]
	mov ax, word [es:di]
	mov ah,0
	cmp al,0x9b
	je txt_str_compare_elsedoneblock152
txt_str_compare_ConditionalTrueBlock150: ;Main true block ;keep :
	; LineNumber: 386
	; LineNumber: 387
	; Binary clause Simplified: EQUALS
	; Compare two vars optimization
	les di,[txt_str2]
	add di,word [txt_si]
	mov ax, word [es:di]
	mov ah,0
	cmp  txt_str1,al
	jne txt_str_compare_elseblock168
txt_str_compare_ConditionalTrueBlock167: ;Main true block ;keep :
	; LineNumber: 388
	; LineNumber: 389
	mov [txt__match], byte 0x1
	; LineNumber: 391
	jmp txt_str_compare_elsedoneblock169
txt_str_compare_elseblock168:
	; LineNumber: 391
	; LineNumber: 393
	mov [txt__match], byte 0x0
	; LineNumber: 394
txt_str_compare_elsedoneblock169:
	; LineNumber: 396
	; 'a:=a + const'  optimization 
	add [txt_si], byte 0x1
	; LineNumber: 397
	jmp txt_str_compare_while149
txt_str_compare_elsedoneblock152:
txt_str_compare_loopend154:
	; LineNumber: 400
	; LineNumber: 400
	mov al, [txt__match]
	ret
	; ***********  Defining procedure : txt_put_dec_at
	;    Procedure type : User-defined procedure
	; LineNumber: 409
	; LineNumber: 408
txt__natx:	db	0
	; LineNumber: 408
txt__naty:	db	0
	; LineNumber: 408
txt__nat:	db	0
txt_put_dec_at_block174:
txt_put_dec_at:
	; LineNumber: 411
	mov al, [txt__natx]
	mov [txt__text_x], al
	mov al, [txt__naty]
	mov [txt__text_y], al
	call txt_move_to
	; LineNumber: 412
	mov ax, [txt__nat]
	mov ah,0 ; forcetype clear high bit
	mov [Screen_i], ax
	call Screen_PrintInt
	; LineNumber: 414
	ret
	; ***********  Defining procedure : txt_str_len
	;    Procedure type : User-defined procedure
	; LineNumber: 418
	; LineNumber: 417
txt_str_len_block175:
txt_str_len:
	; LineNumber: 420
	mov [txt_i], byte 0x0
	; LineNumber: 423
txt_str_len_while176:
txt_str_len_loopstart180:
	; Binary clause Simplified: NOTEQUALS
	; Compare with pure num / var optimization
	les di,[txt_in_str]
	add di,word [txt_i]
	mov ax, word [es:di]
	mov ah,0
	cmp al,0x0
	je txt_str_len_elsedoneblock179
txt_str_len_ConditionalTrueBlock177: ;Main true block ;keep :
	; LineNumber: 423
	; LineNumber: 425
	
; // get the Str_Len by counting until char is 0
	; 'a:=a + const'  optimization 
	add [txt_i], byte 0x1
	; LineNumber: 426
	jmp txt_str_len_while176
txt_str_len_elsedoneblock179:
txt_str_len_loopend181:
	; LineNumber: 430
	; LineNumber: 431
	mov al, [txt_i]
	ret
	
; // Return
; // print X spaces
	; ***********  Defining procedure : txt_print_space
	;    Procedure type : User-defined procedure
	; LineNumber: 435
	; LineNumber: 434
txt_print_space_block184:
txt_print_space:
	; LineNumber: 437
	mov [txt_i], byte 0x0
	; LineNumber: 442
	mov [txt_i], byte 0x0
txt_print_space_forloop185:
	; LineNumber: 439
	; LineNumber: 440
	mov [txt_CH], byte 0x20
	call txt_put_ch
	; LineNumber: 441
txt_print_space_forloopcounter187:
txt_print_space_loopstart188:
	add [txt_i],byte 1
	mov ax, [txt_max_digits]
	cmp ax,byte [txt_i]
	jne txt_print_space_forloop185
txt_print_space_forloopend186:
txt_print_space_loopend189:
	; LineNumber: 443
	ret
	;*
; //
; //	Output a string at the current cursor location but centered.
; //	Set Carriage Return on/off	
; //	Set the screen width
; //	Set reverse on/off
; //	
; //	*
	; ***********  Defining procedure : txt_print_string_centered
	;    Procedure type : User-defined procedure
	; LineNumber: 454
	; LineNumber: 453
	; LineNumber: 453
	; LineNumber: 453
txt__sc_w:	db	0
txt_print_string_centered_block192:
txt_print_string_centered:
	; LineNumber: 456
	mov [txt_i], byte 0x0
	; LineNumber: 457
	mov [txt_max_digits], word 0x0
	; LineNumber: 460
	
; // Get the length of the string
	call txt_str_len
	mov [txt_i], al
	; LineNumber: 463
	
; // padding should be half of width minus string length
	; RHS is pure optimization
	mov ax, [txt__sc_w]
	mov ah,0 ; forcetype clear high bit
	; Forcetype IS POINTER: 0
	sub  ax, [txt_i]
	xor dx,dx
	mov bx, 0x2
	div  bx
	mov [txt_max_digits], ax
	; LineNumber: 466
	; Binary clause Simplified: GREATER
	; Compare with pure num / var optimization
	cmp [txt_max_digits],word 0x0
	jbe txt_print_string_centered_elseblock195
txt_print_string_centered_localsuccess199: ;keep:
	; ; logical AND, second requirement
	; Binary clause Simplified: LESS
	; Compare with pure num / var optimization
	cmp [txt_i],byte 0x28
	jae txt_print_string_centered_elseblock195
txt_print_string_centered_ConditionalTrueBlock194: ;Main true block ;keep :
	; LineNumber: 466
	; LineNumber: 470
	
; // Is it worth padding?
; // Add the padding
	call txt_print_space
	; LineNumber: 473
	
; // print the string
	call txt_print_string
	; LineNumber: 476
	jmp txt_print_string_centered_elsedoneblock196
txt_print_string_centered_elseblock195:
	; LineNumber: 477
	; LineNumber: 479
	
; // print the string
	call txt_print_string
	; LineNumber: 480
txt_print_string_centered_elsedoneblock196:
	; LineNumber: 484
	ret
	; ***********  Defining procedure : txt_get_string
	;    Procedure type : User-defined procedure
	; LineNumber: 492
	; LineNumber: 488
txt__input_key:	db	0
	; LineNumber: 488
txt_bp:	db	0
	; LineNumber: 489
txt_input_buffer		db	"                                                      "
	db	0
	db	0
	; LineNumber: 490
txt_p_input_buffer: dw  0,0
txt_get_string_block202:
txt_get_string:
	; LineNumber: 494
	mov [txt_bp], byte 0x0
	; LineNumber: 495
	mov [txt__input_key], byte 0x0
	; LineNumber: 496
	; Assigning pointer
	lea si, [txt_input_buffer]
	mov [txt_p_input_buffer+2], ds
	mov [txt_p_input_buffer], si
	; LineNumber: 498
	call txt_cursor_on
	; LineNumber: 500
txt_get_string_while203:
txt_get_string_loopstart207:
	; Binary clause Simplified: NOTEQUALS
	; Compare with pure num / var optimization
	cmp [txt__input_key],byte 0xd
	je txt_get_string_elsedoneblock206
txt_get_string_ConditionalTrueBlock204: ;Main true block ;keep :
	; LineNumber: 501
	; LineNumber: 503
	call txt_get_key
	mov [txt__input_key], al
	; LineNumber: 505
	; Binary clause Simplified: GREATER
	; Compare with pure num / var optimization
	cmp [txt__input_key],byte 0x0
	jbe txt_get_string_elsedoneblock227
txt_get_string_localsuccess229: ;keep:
	; ; logical AND, second requirement
	; Binary clause Simplified: LESS
	; Compare with pure num / var optimization
	cmp [txt__input_key],byte 0xff
	jae txt_get_string_elsedoneblock227
txt_get_string_ConditionalTrueBlock225: ;Main true block ;keep :
	; LineNumber: 506
	; LineNumber: 507
	mov al, [txt__input_key]
	mov [txt_CH], al
	call txt_put_ch
	; LineNumber: 508
	; Assigning pointer with index, type:NADA right is pointer: 0
	mov al, [txt__input_key]
	les di, [txt_p_input_buffer]
	add di,[txt_bp]
	mov [es:di],al
	; LineNumber: 509
	; 'a:=a + const'  optimization 
	add [txt_bp], byte 0x1
	; LineNumber: 510
txt_get_string_elsedoneblock227:
	; LineNumber: 512
	; Binary clause Simplified: EQUALS
	; Compare with pure num / var optimization
	cmp [txt__input_key],byte 0x14
	jne txt_get_string_elsedoneblock234
txt_get_string_ConditionalTrueBlock232: ;Main true block ;keep :
	; LineNumber: 513
	; LineNumber: 514
	mov [txt_CH], byte 0x20
	call txt_put_ch
	; LineNumber: 515
	mov [txt_CH], byte 0x14
	call txt_put_ch
	; LineNumber: 516
	; 'a:=a + const'  optimization 
	sub [txt_bp], byte 0x2
	; LineNumber: 518
txt_get_string_elsedoneblock234:
	; LineNumber: 520
	; Assigning pointer with index, type:NADA right is pointer: 0
	mov al, 0x0
	les di, [txt_p_input_buffer]
	add di,[txt_bp]
	mov [es:di],al
	; LineNumber: 521
	jmp txt_get_string_while203
txt_get_string_elsedoneblock206:
txt_get_string_loopend208:
	; LineNumber: 523
	call txt_cursor_off
	; LineNumber: 525
	; LineNumber: 526
	les di, [txt_p_input_buffer]
	ret
	; ***********  Defining procedure : txt_str_to_dec
	;    Procedure type : User-defined procedure
	; LineNumber: 534
	; LineNumber: 533
txt__in_str: dw  0,0
txt_str_to_dec_block237:
txt_str_to_dec:
	; LineNumber: 536
	mov [txt_next_digit], word 0x0
	; LineNumber: 537
	mov [txt_temp_num], word 0x0
	; LineNumber: 538
	mov [txt_temp_i], byte 0x0
	; LineNumber: 539
	mov [txt_max_digits], word 0x0
	; LineNumber: 541
txt_str_to_dec_while238:
txt_str_to_dec_loopstart242:
	; Binary clause Simplified: GREATER
	; Compare with pure num / var optimization
	les di,[txt__in_str]
	add di,word [txt_max_digits]
	mov ax, word [es:di]
	mov ah,0
	cmp al,0x0
	jbe txt_str_to_dec_elsedoneblock241
txt_str_to_dec_ConditionalTrueBlock239: ;Main true block ;keep :
	; LineNumber: 542
	; LineNumber: 543
	; 'a:=a + const'  optimization 
	add [txt_max_digits], word 0x1
	; LineNumber: 544
	jmp txt_str_to_dec_while238
txt_str_to_dec_elsedoneblock241:
txt_str_to_dec_loopend243:
	; LineNumber: 545
	; 'a:=a + const'  optimization 
	sub [txt_max_digits], word 0x1
	; LineNumber: 561
	mov [txt_temp_i], byte 0x0
txt_str_to_dec_forloop246:
	; LineNumber: 548
	; LineNumber: 550
	; Binary clause Simplified: EQUALS
	; Compare with pure num / var optimization
	les di,[txt__in_str]
	add di,word [txt_temp_i]
	mov ax, word [es:di]
	mov ah,0
	cmp al,0x30
	jne txt_str_to_dec_elseblock263
txt_str_to_dec_ConditionalTrueBlock262: ;Main true block ;keep :
	; LineNumber: 551
	; LineNumber: 552
	mov [txt_next_digit], word 0x0
	; LineNumber: 554
	jmp txt_str_to_dec_elsedoneblock264
txt_str_to_dec_elseblock263:
	; LineNumber: 555
	; LineNumber: 556
	; Damn hard binop
	mov cx, 0x30
	push cx
	les di,[txt__in_str]
	add di,word [txt_temp_i]
	mov ax, word [es:di]
	mov ah,0
	pop cx
	sub  cx,ax ; generic binop when rhs is NOT pointer
	mov ax,cx
	mov [txt_next_digit], ax
	; LineNumber: 557
txt_str_to_dec_elsedoneblock264:
	; LineNumber: 559
	; RHS is pure optimization
	mov ax, [txt_temp_num]
	mov bx, 0xa
	mul  bx
	; Forcetype IS POINTER: 0
	add  ax, [txt_next_digit]
	mov [txt_temp_num], ax
	; LineNumber: 560
txt_str_to_dec_forloopcounter248:
txt_str_to_dec_loopstart249:
	add [txt_temp_i],byte 1
	mov ax, [txt_max_digits]
	cmp ax,byte [txt_temp_i]
	jne txt_str_to_dec_forloop246
txt_str_to_dec_forloopend247:
txt_str_to_dec_loopend250:
	; LineNumber: 563
	; LineNumber: 563
	mov ax, [txt_temp_num]
	ret
	; ***********  Defining procedure : txt_get_dec
	;    Procedure type : User-defined procedure
	; LineNumber: 566
txt_get_dec:
	; LineNumber: 567
	; Assigning pointer
	; Setting PURE POINTER 0
	call txt_get_string
	; Setting PURE POINTER ends
	mov [txt_temp_num_p+2], es
	mov [txt_temp_num_p], di
	; LineNumber: 569
	; LineNumber: 569
	; Assigning pointer
	les di, [txt_temp_num_p]
	mov [txt__in_str+2], es
	mov [txt__in_str], di
	call txt_str_to_dec
	ret
	;*
; //	
; //	Clear screen and initialise pointers 
; //	Required for move_to etc
; //		
; //	*
	; ***********  Defining procedure : txt_cls
	;    Procedure type : User-defined procedure
	; LineNumber: 581
txt_cls:
	; LineNumber: 582
	call txt_esc
	; LineNumber: 583
	; Assigning a string : Screen_zp
	mov si, txt_cls_stringassignstr271
	mov [Screen_zp+2], ds
	mov [Screen_zp], si
	call Screen_Print
	; LineNumber: 584
	call txt_DefineScreen
	; LineNumber: 585
	ret
block1:
	; Ending memory block
