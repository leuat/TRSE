;--------------------------------------------
; Coleco ver 1.01 (C) Electric Adventures 2014
;--------------------------------------------
;
; VDP
DATA_PORT: EQU 0BEh ; MSX 098h
CTRL_PORT: EQU 0BFh ; MSX 099h
;
; Video modes
SCRMODE_STANDARD:               EQU 00h
SCRMODE_TEXT:                   EQU 10h
SCRMODE_MULTICOLOR:             EQU 08h
SCRMODE_BITMAP:                 EQU 02h
SCRMODE_BITMAP_TEXT:            EQU 12h
SCRMODE_BITMAP_MULTICOLOR:      EQU 0ah
SCRMODE_BITMAP_TEXT_MULTICOLOR: EQU 1ah
;
; TMS99xxA colours
COLOR_TRANSPARENT:          EQU 00h
COLOR_BLACK:                EQU 01h
COLOR_GREEN:                EQU 02h
COLOR_LIGHT_GREEN:          EQU 03h
COLOR_BLUE:                 EQU 04h
COLOR_LIGHT_BLUE:           EQU 05h
COLOR_DARK_RED:             EQU 06h
COLOR_CYAN:                 EQU 07h
COLOR_RED:                  EQU 08h
COLOR_LIGHT_RED:            EQU 09h
COLOR_YELLOW:               EQU 0ah
COLOR_LIGHT_YELLOW:         EQU 0bh
COLOR_DARK_GREEN:           EQU 0ch
COLOR_MAGENTA:              EQU 0dh
COLOR_GRAY:                 EQU 0eh
COLOR_WHITE:                EQU 0fh

;
; Coleco BIOS Jump Table
RAND_GEN:	    EQU	1ffdh	;Output: 16 bit result in RAND_NUM, HL, A=L
RAND_NUM:	    EQU	73c8h	;2 byte output of last call to RAND_GEN
CALC_OFFSET:    EQU $08c0
LOAD_ASCII:     EQU $1f7f
FILL_VRAM:      EQU $1f82
MODE_1:         EQU $1f85
PUT_VRAM:       EQU $1fbe
WRITE_REGISTER: EQU $1fd9
READ_REGISTER:  EQU $1fdc
WRITE_VRAM:     EQU $1fdf
DECODER:        EQU $1f79
;Timer related calls
INIT_TIMER:     EQU	$1fc7	;**HL=timer table,DE=timer data block
FREE_SIGNAL:	EQU	$1fca	;Note: must not be interrupted
REQUEST_SIGNAL:	EQU	$1fcd
TEST_SIGNAL:	EQU	$1fd0
TIME_MGR:	    EQU	$1fd3
AMERICA:	    EQU	$0069	;Contains how many timer ticks in a second (50/60)
;
; Music/sound effects related
;
SOUND_INIT:	    EQU	1feeh	;B=concurrent voices+effects, HL=song table
SOUND_MAN:	    EQU	1ff4h	;Call during interrupt (late)
PLAY_IT:		EQU	1ff1h	;B=Song number
PLAY_SONGS:	    EQU	1f61h	;Call during interrupt (early)
TURN_OFF_SOUND:	EQU	1fd6h	;No sounds
;
; Coleco Controller related calls and settings
;
POLLER:	            EQU	1febh
;To be added together for CONTROLLER_MAP +0 (player 1), and +1 (player 2)
CONTROLLER_ENABLE:	EQU	80h
KEYPAD_ENABLE:		EQU	10h
ARM_BUTTON_ENABLE:	EQU	8
JOYSTICK_ENABLE:	EQU	2
FIRE_BUTTON_ENABLE:	EQU	1
;Controller Table offsets
PLAYER1:	EQU	0	;Settings (above)
PLAYER2:	EQU	1
FIRE1:	    EQU	2	;Fire button 1 (40h=yes, 0=no)
JOY1:	    EQU	3	;1=N, 2=E, 4=S, 8=W, etc.
SPIN1:	    EQU	4	;counter
ARM1:	    EQU	5	;Arm button 1 (40h=yes, 0=no)
KEYPAD1:	EQU	6	;0-9, '*'=10, '#'=11
FIRE2:	    EQU	7
JOY2:	    EQU	8
SPIN2:	    EQU	9
ARM2:	    EQU	10
KEYPAD2:	EQU	11



StackTop:	        EQU	739fh	;Top of stack, bottom at 7330h (112 bytes in length)
SPRITE_NAME:	    EQU	7030h	;Pointer to sprite name table
        ; max 32 sprites * 4 bytes each = 128 bytes
        ; note: 7020h-702bh are used for music player.
SPRITE_ORDER:	    EQU	7080h	;Pointer to sprite order table
        ; max 32 sprites
WORK_BUFFER:	    EQU	70a0h	;Pointer to graphics work area
        ; up to around 300h max usage

; Any system values
; VRAM DEFAULT TABLES
VRAM_PATTERN:       EQU $0000
VRAM_NAME:          EQU $1800
VRAM_SPRATTR:       EQU $1B00
VRAM_COLOR:         EQU $2000
VRAM_SPRGEN:        EQU $3800


;TickTimer    EQU 7000h ; Signal that 3 frames has elapsed
;HalfSecTimer EQU 7001h ; Signal that 1/2 second has elapsed
;QtrSecTimer  EQU 7002h ; Signal that 1/4 second has elapsed
;TIME         EQU 7003h
;SEED	     EQU 7005h

;CONTROLLER_BUFFER EQU 7009h	;Pointer to hand controller input area

;MOVDLY       EQU 7015h      ; Up to 10 movement timers












    ORG        8000h

    DB       0AAh,055h       ;Cartridge present:  Colecovision logo
;           DB       055h,0AAh       ;Cartridge present:  skip logo, Colecovision logo
    DW       0000           ;Pointer to the sprite name table
    DW       0000           ;Pointer to the sprite order table
    DW       0000           ;Pointer to the working buffer for WR_SPR_NM_TBL
    DW       CONTROLLER_BUFFER ;Pointer to the hand controller input areas
    DW       PRG_START      ;Entry point to the user program


rst_8:
       reti
       nop
rst_10:
       reti
       nop
rst_18:
       JP	RAND_GEN
rst_20:
       reti
       nop
rst_28:
       reti
       nop
rst_30:
       reti
       nop
rst_38:
       reti
       nop

       jp NMI
         db "WHOOOOO      /   MADE WITH TRSE  /2020"

SoundDataCount:   EQU 7
Len_SoundDataArea: EQU  10*SoundDataCount+1 ;7 data areas

SETSCREEN2:
    LD BC,0002h ;Reg 0: Mode 2
    CALL WRITE_REGISTER
    LD BC,0206h        ; Name table 1800h
    CALL WRITE_REGISTER
    LD BC,03ffh        ; Colour table 2000h
    CALL WRITE_REGISTER
    LD BC,0403h        ; Pattern table 0000h
    CALL WRITE_REGISTER
    LD BC,0536h        ; Sprite attribute table 1b00h
    CALL WRITE_REGISTER
    LD BC,0607h        ; Sprite pattern table 3800h
    CALL WRITE_REGISTER
    LD BC,0700h        ; Base colours
    CALL WRITE_REGISTER
    LD BC,01c2h ;Reg 1: Mode 2, 16k, no interrupts, 16x16 sprites
    CALL WRITE_REGISTER
    RET


PRG_START:

    ; set stack pointer
    LD  SP,StackTop ;128 bytes in length at 737fh

    ; Initialise sound
;  LD  B,SoundDataCount  ;Max number of active voices+effects
;  LD  HL,SoundAddrs
;  CALL  SOUND_INIT

    ; initialise clock
 ;   LD  HL,TIMER_TABLE
 ;   LD  DE,TIMER_DATA_BLOCK
 ;   CALL INIT_TIMER

    ; Set screen mode 2,2
  CALL SETSCREEN2

    ;Enable both joysticks, buttons, keypads
    LD  HL,09b9bh
    LD  (CONTROLLER_BUFFER),HL

    ; Seed random numbers with a fixed number (nothing else to use?)
;    LD HL,1967
 ;   CALL SEED_RANDOM

    ;Enable timers
 ;   CALL CREATE_TIMERS
