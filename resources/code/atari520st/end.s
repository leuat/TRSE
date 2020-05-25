

                move.l  old_stack,-(a7)       ; restore old stack pointer
                move.w  #32,-(a7)             ; back to user mode
                trap    #1                     ; call gemdos
                addq.l  #6,a7                 ; clear stack

                clr.l   -(a7)                  ; clean exit
                trap    #1                     ; call gemdos

old_stack       dc.l 0