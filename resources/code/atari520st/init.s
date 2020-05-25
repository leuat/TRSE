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
