
;    org $FFFA     ;first of the three vectors starts here
;  org $BFF8     ;first of the three vectors starts here
  .word NMI        ;when an NMI happens (once per frame if enabled) the 
                   ;processor will jump to the label NMI:
  .word RESET      ;when the processor first turns on or is reset, it will jump
                   ;to the label RESET:
  .word IRQ          ;external interrupt IRQ is not used in this tutorial
