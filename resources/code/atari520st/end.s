

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
