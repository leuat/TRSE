    clr.l   -(a7)                  ; clear stack
                move.w  #32,-(a7)             ; prepare for super mode
                trap    #1                     ; call gemdos
                addq.l  #6,a7                 ; clear up stack
                move.l  d0,old_stack          ; backup old stack pointer

