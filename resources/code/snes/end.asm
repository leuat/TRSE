
.segment "SNESHEADER"
;$00FFC0-$00FFFF

.byte "TRSETRSETRSE         " ;rom name 21 chars
.byte $30  ;LoROM FastROM
.byte $00  ; extra chips in cartridge, 00: no extra RAM; 02: RAM with battery
.byte $08  ; ROM size (2^# in kB)
.byte $00  ; backup RAM size
.byte $01  ; US
.byte $33  ; publisher id
.byte $00  ; ROM revision number
.word $0000  ; checksum of all bytes
.word $0000  ; $FFFF minus checksum

;ffe0 not used
.word $0000
.word $0000

;ffe4 - native mode vectors
.addr return_int  ;cop native **
.addr return_int  ;brk native **
.addr return_int  ;abort native not used *
.addr Screen_nmi ;nmi native
.addr StartBlock800 ;RESET native
.addr return_int ;irq native


;fff0 not used
.word $0000
.word $0000

;fff4 - emulation mode vectors
.addr return_int  ;cop emulation **
.addr $0000 ; not used
.addr $0000  ;abort not used *
.addr Screen_nmi ;nmi emulation
.addr StartBlock800 ;RESET emulation
.addr return_int ;irq/brk emulation **

;* the SNES doesn't use the ABORT vector
;**the programmer could insert COP or BRK as debugging tools
;The SNES boots up in emulation mode, but then immediately
;  will be set in software to native mode
;IRQ_end is just an RTI
;the vectors here need to be in bank 00
;The SNES never looks at the checksum. Some emulators
;will give a warning message, if the checksum is w
