    clr.l   -(a7)                  ; clear stack
                move.w  #32,-(a7)             ; prepare for super mode
                trap    #1                     ; call gemdos
                addq.l  #6,a7                 ; clear up stack
                move.l  d0,old_stack          ; backup old stack pointer
                ; Save old palette
                move.l  #old_palette,a0         ; put backup address in a0
                movem.l $ffff8240,d0-d7         ; all palettes in d0-d7
                movem.l d0-d7,(a0)              ; move data into old_palette

                move.w  #2,-(a7)                ; get physbase
                trap    #14
                addq.l  #2,a7
                move.l  d0,old_screen           ; save old screen address

                move.w  #4,-(a7)                ; get resolution
                trap    #14
                addq.l  #2,a7
                move.w  d0,old_resolution       ; save resolution
	jmp block24
i	dc.l	0
j	dc.l	0
k	dc.l	0
t	dc.w	0
val	dc.l	0
s dc.l 0
	 	CNOP 0,4
image
	incbin "/home/leuat/code/TRSE/tutorials/ATARI520ST/tutorials///images/image1.bin"
	 	CNOP 0,4
image_pal
	incbin "/home/leuat/code/TRSE/tutorials/ATARI520ST/tutorials///images/image1.pal"
	; ***********  Defining procedure : SetLoRes
	;    Procedure type : User-defined procedure
	 	CNOP 0,4
SetLoRes
	; ****** Inline assembler section

	
                move.w  #0,-(a7)                ; low resolution
                move.l  #-1,-(a7)               ; keep physbase
                move.l  #-1,-(a7)               ; keep logbase
                move.w  #5,-(a7)                ; change screen
                trap    #14
                add.l   #12,a7

	
	rts
	 	CNOP 0,4
block24
	jsr SetLoRes
	move #$f,d0
	move.l #image_pal,a0
	add.w #$0,a0
	move.l #-$7dc0,a1
memcpy26
	move.w (a0)+,(a1)+
	dbf d0,memcpy26
	; Getscreen
	move.w  #2,-(a7)                ; get physbase
	trap    #14                      ; call XBIOS
	addq.l  #2,a7                   ; clean up stack
	move.l  d0,s
	move #$1f40,d0
	move.l #image,a1
	add.w #$0,a1
	move.l s,a0
memcpy27
	move.l (a1)+,(a0)+
	dbf d0,memcpy27
	; ****** Inline assembler section

	  move.w  #7,-(a7)              ; wait for a keypress
      trap    #1                     ; call gemdos
      addq.l  #2,a7                 ; clear up stack
      
	
	
; // Wait for keypress
	; Assigning single variable : t
	move.w #$0,t ; Simple a:=b optimization
	; Assigning single variable : j
	move.l #$0,j ; Simple a:=b optimization
forloop28
	 	CNOP 0,4
	; Assigning single variable : k
	move.l #$0,k ; Simple a:=b optimization
	
; //			s:=s+2;
	; Assigning single variable : i
	move.l #$0,i ; Simple a:=b optimization
forloop30
	 	CNOP 0,4
	; Assigning single variable : s
	; Assign: Regular, forcetype INTEGER
	; StoreVar : 0
	; Storevar START getting new unused var #$5500
	move.l k,d1          ; Loadvar regular end
	move.l s,a0
	move.w #$5500,(a0,d1)
	; Assigning single variable : k
	add.l #$2,k ; Optimization: simple A := A op Const ADD SUB OR AND
	; Assigning single variable : s
	; Assign: Regular, forcetype INTEGER
	; BOP NEW register: d0
	moveq #0,d0
	move.w t,d0     ; BOP move
	add.w #$ff73,d0
	; StoreVar : 0
	; Storevar START getting new unused var d0
	move.l k,d2          ; Loadvar regular end
	move.l s,a0
	move.w d0,(a0,d2)
	; Assigning single variable : k
	add.l #$2,k ; Optimization: simple A := A op Const ADD SUB OR AND
	; Assigning single variable : s
	; Assign: Regular, forcetype INTEGER
	; StoreVar : 0
	; Storevar START getting new unused var #$ffff
	move.l k,d2          ; Loadvar regular end
	move.l s,a0
	move.w #$ffff,(a0,d2)
	; Assigning single variable : k
	add.l #$2,k ; Optimization: simple A := A op Const ADD SUB OR AND
	; Assigning single variable : val
	move.l #$ffff,val ; Simple a:=b optimization
	; Assigning single variable : s
	; Assign: Regular, forcetype INTEGER
	; StoreVar : 0
	; Storevar START getting new unused var val
	move.l k,d2          ; Loadvar regular end
	move.l s,a0
	move.w val,(a0,d2)
	; Assigning single variable : k
	add.l #$2,k ; Optimization: simple A := A op Const ADD SUB OR AND
	; Assigning single variable : t
	add.w #$1,t ; Optimization: simple A := A op Const ADD SUB OR AND
	add #1,i
	move #$28,d1
	cmp i,d1
	bne forloop30
	; Assigning single variable : t
	add.w #$100,t ; Optimization: simple A := A op Const ADD SUB OR AND
	; Assigning single variable : s
	add.l #$a0,s ; Optimization: simple A := A op Const ADD SUB OR AND
	add #1,j
	move #$c8,d1
	cmp j,d1
	bne forloop28
	; ****** Inline assembler section

	  move.w  #7,-(a7)              ; wait for a keypress
      trap    #1                     ; call gemdos
      addq.l  #2,a7                 ; clear up stack
      
	


               move.w  old_resolution,d0       ; res in d0
               move.w  d0,-(a7)                ; push resolution
               move.l  old_screen,d0           ; screen in d0
               move.l  d0,-(a7)                ; push physbase
               move.l  d0,-(a7)                ; push logbase
               move.w  #5,-(a7)                ; change screen
               trap    #14
               add.l   #12,a7
;* end resolution and screen adress restore

;* restores the old palette
               move.l  #old_palette,a0         ; palette pointer in a0
               movem.l (a0),d0-d7              ; move palette data
               movem.l d0-d7,$ffff8240         ; smack palette in

                move.l  old_stack,-(a7)       ; restore old stack pointer
                move.w  #32,-(a7)             ; back to user mode
                trap    #1                     ; call gemdos
                addq.l  #6,a7                 ; clear stack

                clr.l   -(a7)                  ; clean exit
                trap    #1                     ; call gemdos

                rts



old_resolution:  dc.w    0
old_stack:       dc.l    0
old_screen:      dc.l    0
old_palette:     ds.l    8
	Section ChipRAM,Data_c
 	CNOP 0,4
