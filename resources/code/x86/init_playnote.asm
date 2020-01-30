init_playnote:
    jmp init_playnote_cont
player_notes:	dw 0x023a1, 0x021a1, 0x01fbe, 0x01df6, 0x01c47, 0x01ab1, 0x01931, 0x017c7
            dw 0x01672, 0x0152f, 0x013ff, 0x012df, 0x011d0, 0x010d0, 0x0fdf, 0x0efa
            dw 0x0e23, 0x0d58, 0x0c98, 0x0be3, 0x0b39, 0x0a97, 0x09ff, 0x096f
            dw 0x08e8, 0x0868, 0x07ef, 0x077d, 0x0711, 0x06b3, 0x064c, 0x05f1
            dw 0x059c, 0x054b, 0x050c, 0x04b7, 0x01

init_playnote_call:

    mov ah,0
    mov di,ax
    shl di,1 ; Accomodate for word
    mov ax, [player_notes+di]
    push ax

    mov     al, 182         ; Prepare the speaker for the
    out     43h, al         ;  note.
    pop     ax
                        ;  for middle C.
    out     42h, al         ; Output low byte.
    mov     al, ah          ; Output high byte.
    out     42h, al
    in      al, 61h         ; Turn on note(get value from
                        ;  port 61h).
    or      al, 00000011b   ; Set bits 1 and 0.
    out     61h, al         ; Send new value.


    ret
init_playnote_cont:
