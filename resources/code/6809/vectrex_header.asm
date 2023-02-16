    fcc      "g GCE 1983"
    fcb      $80
    fcb      $FD,$0D
    fcb      $F8,$50,$20,$BA
    fcc      "TURBORASCAL "
    fcb      $80
    fcb      0

;ASSUME dpr:$C8		;SETDP on other systems $D0= Hardware Regs $C8=Ram
    lda #$C8
    tfr a,dp
