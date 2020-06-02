

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
