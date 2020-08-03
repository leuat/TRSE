
; Set origin in Coleco RAM area
ORG 07000h ; fit common items before the BIOS RAM usage area

TickTimer:    DS 1 ; Signal that 3 frames has elapsed
HalfSecTimer: DS 1 ; Signal that 1/2 second has elapsed
QtrSecTimer:  DS 1 ; Signal that 1/4 second has elapsed
TIME:         DS 2
SEED:	      DS 4
CONTROLLER_BUFFER:	DS 12	;Pointer to hand controller input area
MOVDLY:       DS 10      ; Up to 10 movement timers

ORG 07030h ; avoid Coleco BIOS RAM usage area

; Sprite positions
SPRTBL:       DS 80h
SPRORDER:     DS 1 ; flag to indicate the current sprite write direction
TIMER_TABLE:	    DS 16	;Pointer to timers table (16 timers)
TIMER_DATA_BLOCK:	DS 58	;Pointer to timers table for long timers
                            ;4 bytes * 16 longer than 3 sec timers
VDU_HOOK: DS 4 ; NMI VDU Delayed writes hook


