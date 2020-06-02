

             move.l  #backup,a0
             move.b  (a0)+,$fffa07           ; restore enable a
             move.b  (a0)+,$fffa13           ; restore mask a
             move.b  (a0)+,$fffa15           ; restore mask b
             move.b  (a0)+,$fffa1b           ; restore timer b control
             move.b  (a0)+,$fffa21           ; restore timer b data
             add.l   #1,a0                   ; make address even

               move.l  old_120,$120              ; restore $70
               move.l  old_70,$70              ; restore $70
               move.l  old_68,$68              ; restore $70



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

;
;   MUSIC PLAYER
;

internal_playmusic:

        move.l  internal_music,a0                 ; pointer to current music data
        moveq.l #0,d0
internal_play
        move.b  d0,$ff8800               ; write to register
        move.b  (a0),$ff8802             ; write music data
        add.l   internal_frames,a0                ; jump to next register in data
        addq.b  #1,d0                    ; next register
        cmp.b   #16,d0                   ; see if last register
        bne     internal_play                      ; if not, write next one

        addq.l  #1,internal_music                 ; next set of registers
        addq.l  #1,internal_play_time             ; 1/50th second play time

        move.l  internal_frames,d0
        move.l  internal_play_time,d1
        cmp.l   d0,d1                    ; see if at end of music file
        bne     internal_no_loop
        sub.l   d0,internal_music                 ; beginning of music data
        move.l  #0,internal_play_time             ; reset play time
internal_no_loop
        rts

; INIT

internal_initmusic:
;    move.l  #ym_file,a0              ; start of ym file
    move.l  12(a0),internal_frames            ; store number of frames

    add.l   #34,a0                   ; beginning of text

int_song_name:
    cmp.b   #0,(a0)+                 ; search for 0
    bne     int_song_name
int_comment
    cmp.b   #0,(a0)+                 ; search for 0
    bne     int_comment
int_song_data
    cmp.b   #0,(a0)+                 ; search for 0
    bne     int_song_data

    move.l  a0,internal_music                 ; skipped 3 zero, store address

    rts



putpixel_slow:
                ; a0 screen address
                ; d0 X coordinate
                ; d1 Y coordinate
                ; d2 colour

                mulu.w  #160,d1                 ; 160 bytes to a scan line
                add.l   d1,a0                   ; add y value to screen memory
                divu.w  #16,d0                  ; number of clusters in low, bit in high
                clr.l   d1                       ; clear d1
                move.w  d0,d1                   ; move cluster part to d1
                mulu.w  #8,d1                   ; 8 bytes to a cluster
                add.l   d1,a0                   ; add the cluster part to screen memory
                clr.w   d0                       ; clear out the cluster value
                swap    d0                       ; put the bit to alter in low part of d0

                ; now a0 points to the first word of the bitplane to use
                ; d0.w holds the bit number to be manipulated in the word

                swap    d2                       ; colour in the high part of d2
                move.w  #%0111111111111111,d1
                ror.w   d0,d1                   ; clear mask prepared

                rept    4                        ; do this 4 times
                lsr.l   #1,d2                   ; shift in the next colour bit
                ror.w   d0,d2                   ; shift colour bit into position
                and.w   d1,(a0)                 ; prepare with mask (bclr)
                or.w    d2,(a0)+                ; or in the colour
                clr.w   d2                       ; clear the old used bit
                endr

                ;rte return form putpixel
                rts
                ; end putpixel


*HOLINE. A horizontal line is drawn from left to right.
* passes: x1=d2.w, y1=d3.w, N=d1.w, colour=d4.w, screen y table:a4.l
* First find the address of the word at which the line starts.
holine_holine:
        lea	hln_tbl,a3	pointer to mask table
        lsl.w	#2,d3		there are y long words before the
        movea.l 0(a4,d3.w),a4	current row address in the table
        move	d2,d5		save x1
        andi	#$fff0,d5	go in steps of 8 bytes
        lsr.w	#1,d5		to point to plane #1 word
        adda.w	d5,a4		at this address
        andi	#$000f,d2	which pixel from the left?
        move	d2,d0		save it
* does the entire line lie within one word?
        subi	#16,d0
        neg	d0		are there more pixels to the word end
        cmp	d1,d0		than we have to draw?
        bmi	holine_long_line	no, so it's a long line
* The line is entirely within one word. Get the mask and draw it.
        move	d1,d0
        bsr	holine_draw_it
        rts			and that's all.
* Complete the 1st word in a long line
holine_long_line:
        sub	d0,d1		number left
        bsr	holine_draw_it
* Now fill all the solid words.
        clr	d0
        not	d0
        move	d1,d2		save number of pixels left to do
        lsr	#4,d2		how many are whole words?
        beq	holine_last_word	none are
* a long stretch of filled words - no need to read the table
        subq	#1,d2		this many full words but one
        move	d0,d3		which are all 1's
        not	d3		or all 0's, depending on the colour
        moveq	#4-1,d5		4 colour planes
        move	d4,d6
        subq	#2,a4
holine_inc_plane:
        addq	#2,a4		offset for next plane
        movea.l a4,a5		save the address
        move	d2,d7		initialise the word count
        lsr.w	#1,d6		next colour bit
        bcc	holine_clr_word
holine_set_word:
        or.w	d0,(a5)
        adda	#8,a5		next word in this plane
        dbra	d7,holine_set_word
        bra	holine_new_plane
holine_clr_word:
        and	d3,(a5)
        adda	#8,a5		next word in this plane
        dbra	d7,holine_clr_word
holine_new_plane:
        dbra	d5,holine_inc_plane	for all the colour planes
        subq	#6,a5		pointer to next plane 1
        movea.l a5,a4		update pointer
* it only remains to do the last word. It will start at pixel 0
holine_last_word:
        andi	#$f,d1		low nibble
        cmpi.w	#0,d1		any todo ?
        beq	holine_holine_end	no - finished.
* In finding the mask,the row offset is zero this time.
        clr	d2		1st pixel at extreme left
        move	d1,d0
        bsr	holine_draw_it
holine_holine_end:
        rts			completely finished

* Draw in a section of a word which starts at pixel a and ends at pixel b
holine_draw_it
        lsl	#5,d2		the mask row offset=a*32
        move	d0,d5		plus
        subq	#1,d5		column
        lsl	#1,d5		offset of (15-b)*2 gives
        add	d5,d2		the total offset
        move.w	0(a3,d2.w),d0	to fetch the mask
        move	d0,d3		and
        not	d3		its 1's compliment
        moveq	#3,d5		4-1 colour planes
        move	d4,d6		save colour
holine_next_plane:
        lsr	#1,d6		is this colour bit set?
        bcc	holine_not_set		no
        or.w	d0,(a4)+	yes, also set the bits
        dbf	d5,holine_next_plane
        rts
holine_not_set and.w	d3,(a4)+	clear the bits
        dbf	d5,holine_next_plane
holine_fil_end rts			finished


holine_hline_lu:
;Set up a look-up table for low resolution horizontal line drawing.
;Each mask in the table is the word to set between pixel a and b.
        lea	hln_tbl,a0	pointer to the table base
        move.w	#16-1,d1	16 rows, d1 is the counter
holine_hloop2	clr.w	d0		new row
        move.w	#16-1,d2	16 columns, d2 is the counter
        bset	d1,d0		set the 1st column bit
holine_hloop3	move.w	d0,(a0)+	next column
        move.w	d0,d3
        lsr.w	#1,d3 shift
        or.w	d3,d0 add	back
        dbra	d2,holine_hloop3	complete this row
        dbra	d1,holine_hloop2	for all rows
        rts

gfx_find_phys:
;A call to the operating System to find the physical screen address
        move.w	#2,-(sp)	xbios _physbase
        trap	#14		xbios call
        addq	#2,sp		tidy stack
;the base address is returned in d0 and saved
        move.l	d0,gfx_phys_screen
        rts


gfx_wrt_phys_tbl:
;Write a look-up table of the addresses pf the start of physical each
;screen row in low resolution. The product 4*y is an offset to row y.
        move.l	gfx_phys_screen,d0	where screen location is kept
        move	#200-1,d1	200 rows
        lea	gfx_phys_tbl_y,a0	where the table is
luloop	move.l	d0,(a0)+	the next row in the table
        add	#160,d0		there are 160 bytes/row
        dbra	d1,luloop	for all rows
        rts





hln_tbl		 ds.w	256	;the masks for filling words
old_resolution:  dc.w    0
old_stack:       dc.l    0
old_screen:      dc.l    0
old_palette:     ds.l    8
old_68:          dc.l    0
old_70:          dc.l    0
old_120:         dc.l    0
backup          ds.b    14
internal_frames: dc.l 0
internal_music : dc.l 0;
internal_play_time: dc.l 0;

gfx_phys_screen	ds.l	1	;the address of the physical screen
gfx_phys_tbl_y	ds.l	200	;pointers to the row y's
