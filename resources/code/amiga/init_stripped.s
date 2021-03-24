BPLCON0=                    $100
BPLCON1=                    $102
BPLCON2=                    $104
BLTAMOD:                equ $64
BLTBMOD:                equ $62
BLTCMOD:                equ $60
BLTDMOD:                equ $66
BLTAFWM:                equ $44
BLTCON0:                equ $40
BLTCON1:                equ $42
VHPOSR:                 equ 6
BLTAPTH:                equ $50
BLTAPTL:                equ $52
BLTBPTH:                equ $4c
BLTCPTH:                equ $48
BLTDPTH:                equ $54
BLTSIZE:                equ $58
BLTADAT:                equ $74
BLTBDAT:                equ $72
CIAAPRA         EQU             $bfe001
VPOSR           EQU             $dff004
COP1LCH         EQU             $dff080
DMACONR		EQU		$dff002
ADKCONR		EQU		$dff010
INTENAR		EQU		$dff01c
INTREQR		EQU		$dff01e


CIAAPRA2         EQU             $001
VPOSR2           EQU             $004
COP1LCH2         EQU             $080
DMACONR2		EQU		$002
ADKCONR2		EQU		$010
INTENAR2		EQU		$01c
INTREQR2		EQU		$01e

DMACON		EQU		$dff096
ADKCON		EQU		$dff09e
INTENA		EQU		$dff09a
INTREQ		EQU		$dff09c


DMACON2		EQU		$096
ADKCON2		EQU		$09e
INTENA2		EQU		$09a
INTREQ2		EQU		$09c






init:
