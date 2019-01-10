RESET:
  SEI          ; disable IRQs
;  CLD          ; disable decimal mode
  LDX #$40  
  STX $4017    ; disable APU frame IRQ
  LDX #$FF  
  TXS          ; Set up stack
  INX          ; now X = 0
  STX $2000    ; disable NMI
  STX $2001    ; disable rendering
  STX $4010    ; disable DMC IRQs

vblankwait1:       ; First wait for vblank to make sure PPU is ready
  BIT $2002
  BPL vblankwait1

clrmem:
  LDA #$00
  STA $0000,x
  STA $0100,x
  STA $0400,x
  STA $0500,x
  STA $0600,x
  STA $0700,x
  LDA #$FE
  STA $0300,x
  INX
  BNE clrmem


   
vblankwait2:      ; Second wait for vblank, PPU is ready after this
  BIT $2002
  BPL vblankwait2

