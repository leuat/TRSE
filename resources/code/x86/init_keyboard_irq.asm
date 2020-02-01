	jmp init_keyboard_irq_cont
currentKey dw 0
init_keyboard_irq:
	cli                     ; disable interrupts
    push ax 
    xor   ax,ax

    in al, 64h              ; Read keyboard status port
    cmp al, 10b
    je @@endd                ; not input

    in      al, 060h        ; read scan code 

    cmp al, 80h
    ja @@keyReleased

    ; Handle key presse events
;    __fifo_write al         ; save it in buffer
	mov [currentKey],al


    jmp @@endd
@@keyReleased:
    ; Handle key release events
    mov ah,0
    mov [currentKey],ah

@@endd:
    ; Send End-Of-Interrupt signal to the 8259 Interrupt Controller
    push ax
    mov al,20h
    out 20h,al
    pop ax

    pop ax 
    sti                     ; enable interrupts
    iret
init_keyboard_irq_cont:
