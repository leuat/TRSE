                clr.l   -(a7)                  ; clear stack
                ; into super mode
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
                ; must happen AFTER super
                move.l  $70,old_70              ; backup $70
                move.l  $68,old_68              ; backup $68
                move.l  $120,old_120              ; backup $68

              move.l  #backup,a0              ; get ready with backup space
              move.b  $fffa07,(a0)+           ; backup enable a
              move.b  $fffa13,(a0)+           ; backup mask a
              move.b  $fffa15,(a0)+           ; backup mask b
              move.b  $fffa1b,(a0)+           ; backup timer b control
              move.b  $fffa21,(a0)+           ; backup timer b data
              add.l   #1,a0                   ; make address even
