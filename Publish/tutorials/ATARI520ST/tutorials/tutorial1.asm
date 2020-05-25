    clr.l   -(a7)                  ; clear stack
                move.w  #32,-(a7)             ; prepare for super mode
                trap    #1                     ; call gemdos
                addq.l  #6,a7                 ; clear up stack
                move.l  d0,old_stack          ; backup old stack pointer

	jmp block142
i	dc.w	0
j	dc.w	0
s dc.l 0
	 	CNOP 0,4
block142
	; ****** Inline assembler section

	move.w  #$777,$ffff8240       ; red background color
	
	; ****** Inline assembler section

	move.w  #2,-(a7)                ; get physbase
    trap    #14                      ; call XBIOS
    addq.l  #2,a7                   ; clean up stack
    move.l  d0,s                   ; a0 points to screen
    move.l  d0,a3                   ; a0 points to screen
    

    move.l  #8000,d3
loope
    move.l  d3,(a3)+
    dbf     d3,loope

	
	
; //		s:=s+80;		
	; Assigning single variable : j
	move.w #$0,j ; Simple a:=b optimization
forloop143
	 	CNOP 0,4
	; Assigning single variable : i
	move.w #$0,i ; Simple a:=b optimization
forloop145
	 	CNOP 0,4
	; ****** Inline assembler section
;here
	; ****** Inline assembler section

	moveq.l #0,d1
	move.w i,d1          ; Loadvar regular end
	move.l s,a0
	move.w i,(a0)
			

			
	
; //			s[i]:=i;
	; Assigning single variable : s
	add.l #$2,s ; Optimization: simple A := A op Const ADD SUB OR AND
	add #1,i
	move #$50,d0
	cmp i,d0
	bne forloop145
	add #1,j
	move #$64,d0
	cmp j,d0
	bne forloop143
	; ****** Inline assembler section

	  move.w  #7,-(a7)              ; wait for a keypress
      trap    #1                     ; call gemdos
      addq.l  #2,a7                 ; clear up stack
	


                move.l  old_stack,-(a7)       ; restore old stack pointer
                move.w  #32,-(a7)             ; back to user mode
                trap    #1                     ; call gemdos
                addq.l  #6,a7                 ; clear stack

                clr.l   -(a7)                  ; clean exit
                trap    #1                     ; call gemdos

old_stack       dc.l 0
	Section ChipRAM,Data_c
 	CNOP 0,4
