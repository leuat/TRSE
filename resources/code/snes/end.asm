.segment "VECTORS"
.word 0, 0        ;Native mode vectors
.word return_int  ;COP
.word return_int  ;BRK
.word return_int  ;ABORT
.word Screen_nmi         ;NMI
.word StartBlock800       ;RST
.word return_int  ;IRQ

.word 0, 0        ;Emulation mode vectors
.word return_int  ;COP
.word 0
.word return_int  ;ABORT
.word Screen_nmi         ;NMI
.word StartBlock800       ;RST
.word return_int  ;IRQ
