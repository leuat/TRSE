        ORG $FFFA        ; set address to 6507 Interrupt Vectors 
        dc.w $F000 ; NMI
        dc.w $F000 ; RESET
        dc.w $F000 ; IRQ
