init_scanlines_cga:
	jmp init_scanlines_cga_cont

table_cga_scanlines_x:  dw  0  
table_cga_scanlines_y:  dw  0  
table_cga_scanlines:	 times 200 dw 0
table_cga_masks:	db 0x0c0, 0x030, 0x0c, 0x03


init_scanlines_cga_cont:
	mov ax, 0x0
	mov [table_cga_scanlines_x], ax
table_forloop22862:
	mov ax, [table_cga_scanlines_x]
	mov bx, 0x28
	mul  bx
	mov [table_cga_scanlines_y], ax

    mov al,[table_cga_scanlines_x]
    and al,1
    cmp al,1
    jne initscanlines1
    mov ax,[table_cga_scanlines_y]
    add ax,$2000-40
    mov [table_cga_scanlines_y],ax
initscanlines1:

	
	mov ax, [table_cga_scanlines_y]
	push ax
	mov ax, [table_cga_scanlines_x]
	mov di,ax
	shl di,1
	pop ax
	mov [table_cga_scanlines+di], ax
	mov bx,[table_cga_scanlines_x]
	add bx,1
	mov [table_cga_scanlines_x],bx
	mov ax, 0xc8
	cmp ax,bx
	jne table_forloop22862
