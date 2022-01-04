    jmp init_drawline_cga_cont

%macro drawline_set_pixel 3
	push ax
	push bx

	mov ax, %1
        shr  ax, 1
        shr  ax, 1
        mov  di, ax
        shr  di, 1
        shr  di, 1
        mov bx, %2
;        shl  bx, 1
        shr  bx, 1
        and bx, 0xFFFE
        add  di, [bx + table_cga_scanlines]
	and  ax, 3
	mov  bx, table_cga_masks
	xlat
	mov ah, %3
	and  ah, al
	not  al
	and  al, [es:di]
	or   al, ah
        stosb
	pop bx
	pop ax

%endmacro

%macro drawline_absolute 1
	cmp %1, word 0
	jge %%absolute_l
	neg %1
%%absolute_l:

%endmacro

%macro DrawLine2DDY 0
;	local DrawLine2DDY_l1, DrawLine2DDY_lp, DrawLine2DDY_nxt
	mov dx, 1
	mov ax, [pointX1]			; x1
	cmp ax, [pointX2]			; x2
	jbe DrawLine2DDY_l1		; jump if x1 <= x2
	neg dx 					; turn delta to -1
DrawLine2DDY_l1:
	mov ax, [pointY2]			; y2
	shr ax, 1 				; y2 /  2		
	mov [varTemp], ax			; tmp = y2 / 2
	mov ax, [pointX1]			; x1
	mov [varX], ax			; x = x1
	mov ax, [pointY1]			; y1
	mov [varY], ax			; y = y1
	mov bx, [pointY2]			; y2
	sub bx, [pointY1]			; y1 - y2
        drawline_absolute bx			; |y1 - y2|
	mov cx, [pointX2]			; x2
	sub cx, [pointX1]			; x1
        drawline_absolute cx			; |x1 - x2|
	mov ax, [pointY2]			; y2
DrawLine2DDY_lp:
        drawline_set_pixel [varX], [varY], [drawline_color] ; (x,y)
	inc word [varY]				; y++
	cmp word [varTemp], word 0			; tmp
	jge DrawLine2DDY_nxt	; jump if (tmp >= 0)
	add [varTemp], bx 		; tmp = delyaY		bx = (p2Y - p1Y) = deltay
	add [varX], dx 			; x = delta			dx = delta
DrawLine2DDY_nxt:
	sub [varTemp], cx 		; tmp = deltax		cx = abs(p2X - p1X) = daltax
	cmp [varY], ax 			; ax = y2
	jne DrawLine2DDY_lp		; jump if y != y2
        drawline_set_pixel [varX], [varY], [drawline_color]
%endmacro DrawLine2DDY
;---------------------------------------------
; case: DeltaX is bigger than DeltaY		  
; input: p1X p1Y,		            		  
; 		 p2X p2Y,		           		      
;		 Color -> variable                    
; output: line on the screen                  
;---------------------------------------------
%macro DrawLine2DDX 0 
;	local DrawLine2DDX_l1, DrawLine2DDX_lp, DrawLine2DDX_nxt
	mov dx, 1
	mov ax, [pointY1]			; y1
	cmp ax, [pointY2]			
	jbe DrawLine2DDX_l1		; jump if y1 <= y2
	neg dx 					; else turn delta to -1
DrawLine2DDX_l1:
	mov ax, [pointX2]			; y1
	shr ax, 1 				; div by 2		; y1 / 2
	mov [varTemp], ax			; tmp = y1 / 2
	mov ax, [pointX1]			; x1
	mov [varX], ax			; x = x1
	mov ax, [pointY1]			; y1
	mov [varY], ax			; y = y1
	mov bx, [pointX2]			; x2
	sub bx, [pointX1]			; x2 - x1
        drawline_absolute bx			; bx = |x2 - x1|
	mov cx, [pointY2]			; y2
	sub cx, [pointY1]			; y1
        drawline_absolute cx			; cx = |y2 - y1|
	mov ax, [pointX2]			; x2
DrawLine2DDX_lp:
        drawline_set_pixel [varX], [varY], [drawline_color]	; (x,y)
	inc word [varX]				; x++
	cmp [varTemp], word 0			; tmp
	jge DrawLine2DDX_nxt	; jump if tmp >= 0
	add [varTemp], bx 		; tmp = deltax     bx = abs(p2X - p1X) = deltax
	add [varY], dx 			; y <= delta       dx = delta
DrawLine2DDX_nxt:
	sub [varTemp], cx 		; tmp -= deltay    cx = abs(p2Y - p1Y) = deltay
	cmp [varX], ax 			; ax = x2
	jne DrawLine2DDX_lp		; jump if x != x2
        drawline_set_pixel [varX], [varY], [drawline_color]
%endmacro
;---------------------------------------------
; Draws a line
;
; push x1
; push y1f
; push x2
; push y2
; call drawline_DrawLine
;---------------------------------------------


CGA_DrawLine:

	mov [varTemp],word 0
	mov [varY],word 0
	mov [varX],word 0


;	mov ax, 0xb800
;	mov  es, ax

	mov cx, [pointX1]		; x1
	sub cx, [pointX2	]	; x1 - x2
        drawline_absolute cx		; deltaX = |x1 - x2|
	mov bx, [pointY1]	; y1
	sub bx, [pointY2]		; y1 - y2
        drawline_absolute bx		; deltaY = |y1 - y2|
	cmp cx, bx				
	jae @@DrawLine2Dp1 		; jump if deltaX >= deltaY
	mov ax, [pointX1]		; x1
	mov bx, [pointX2]		; x2
	mov cx, [pointY1]		; y1
	mov dx, [pointY2]		; y2
	cmp cx, dx				
	jbe @@DrawLine2DpNxt1 	; jump if point1Y <= point2Y
	xchg ax, bx			 
	xchg cx, dx
@@DrawLine2DpNxt1:
	mov [pointX1], ax		; x1
	mov [pointX2], bx		; x2
	mov [pointY1], cx		; y1
	mov [pointY2], dx		; y2
        DrawLine2DDY
	jmp @@DrawLine2D_exit	; return
@@DrawLine2Dp1:
	mov ax, [pointX1]		; x1
	mov bx, [pointX2]		; x2
	mov cx, [pointY1]		; y1
	mov dx, [pointY2]		; y2
	cmp ax, bx
	jbe @@DrawLine2DpNxt2 	; jump if point1X <= point2X
	xchg ax, bx
	xchg cx, dx
@@DrawLine2DpNxt2:
	mov [pointX1], ax		; x1
	mov [pointX2], bx		; x2
	mov [pointY1], cx		; y1
	mov [pointY2], dx		; y2
	DrawLine2DDX 

@@DrawLine2D_exit:
	ret
	
varTemp dw 0
varY dw 0
varX dw 0
pointX1 dw 0
pointX2 dw 0
pointY1 dw 0
pointY2 dw 0
drawline_color db 0

init_drawline_cga_cont:
