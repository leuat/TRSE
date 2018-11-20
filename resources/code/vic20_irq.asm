init_via_irq:

  ldx #0       ; wait for this raster line (times 2)
A0_vic_raster:
  cpx $9004
  bne A0_vic_raster        ; at this stage, the inaccuracy is 7 clock cycles
                ; the processor is in this place 2 to 9 cycles
                ; after $9004 has changed
  ldy #9
  bit $24
A1_vic_raster:
  ldx $9004
  txa
  bit $24

  ldx #24

  dex
  bne *-1       ; first spend some time (so that the whole
  cmp $9004     ; loop will be 2 raster lines)
  bcs *+2       ; save one cycle if $9004 changed too late
  dey
  bne A1_vic_raster
                ; now it is fully synchronized
                ; 6 cycles have passed since last $9004 change
                ; and we are on line 2(28+9)=74

;initialize the timers
  lda #$40      ; enable Timer A free run of both VIAs
  sta $911b
  sta $912b


        ; 312*71-2 = $568
timers_vic_raster:
  lda #$56
  ldx #$86
  sta $9116     ; load the timer low byte latches
  sta $9126

  ldy #7        ; make a little delay to get the raster effect to the
  dey           ; right place
  bne *-1
  nop
  nop


  stx $9125     ; start the IRQ timer A
                ; 6560-101: 65 cycles from $9004 change
                ; 6561-101: 77 cycles from $9004 change
  ldy #10       ; spend some time (1+5*9+4=55 cycles)
  dey           ; before starting the reference timer
  bne *-1
  stx $9115     ; start the reference timer

pointers_vic_raster:
  lda #00     ; set the raster IRQ routine pointer
  sta $314
  lda #00
  sta $315
  lda #$c0
  sta $912e     ; enable Timer A underflow interrupts

  rts
