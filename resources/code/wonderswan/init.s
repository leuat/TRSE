;--------------------------------------------
;     Wonderswan Registers & Equates v0.2
;              by Orion_ [2008]
;
; http://onorisoft.free.fr/
;
; with help of WStech24.txt by Judge and Dox
;--------------------------------------------

; IO_	mean byte access
; IOw_	mean word access


;-------------
; Rom Header

RH_ROM_4MBITS		equ	0x02
RH_ROM_8MBITS		equ	0x03
RH_ROM_16MBITS		equ	0x04
RH_ROM_32MBITS		equ	0x06
RH_ROM_64MBITS		equ	0x08
RH_ROM_128MBITS		equ	0x09

RH_NO_SRAM		equ	0x00
RH_SRAM_64KBITS		equ	0x01
RH_SRAM_256KBITS	equ	0x02
RH_SRAM_1MBITS		equ	0x03
RH_SRAM_2MBITS		equ	0x04
RH_SRAM_1KBITS		equ	0x10
RH_SRAM_16KBITS		equ	0x20
RH_SRAM_8KBITS		equ	0x50

RH_WS_MONO		equ	0x00
RH_WS_COLOR		equ	0x01

RH_NO_RTC		equ	0x00
RH_RTC			equ	0x01

RH_HORIZONTAL		equ	(0x04 + 0x00)
RH_VERTICAL		equ	(0x04 + 0x01)



;-----------------
; Memory Address

WS_RAM_BASE	equ	0x0000
WS_TILE_BANK	equ	0x2000
WS_STACK	equ	WS_TILE_BANK-2

WSC_TILE_BANK1	equ	0x4000
WSC_TILE_BANK2	equ	0x8000
WSC_RAM_BASE2	equ	0xC000
WSC_PALETTES	equ	0xFE00
WSC_STACK	equ	WSC_PALETTES-2

MAP_SIZE	equ	0x800
SPR_TABLE_SIZE	equ	0x200



;-------------
; Interrupts

INTVEC_HBLANK_TIMER	equ	7
INTVEC_VBLANK_START	equ	6
INTVEC_VBLANK_TIMER	equ	5
INTVEC_DRAWING_LINE	equ	4
INTVEC_SERIAL_RECEIVE	equ	3
INTVEC_RTC_ALARM	equ	2
INTVEC_KEY_PRESS	equ	1
INTVEC_SERIAL_SEND	equ	0



;-----------------
; Useful Defines

%define BG_CHR(tile,pal,bank,hflip,vflip)	(((vflip) << 15) | ((hflip) << 14) | ((bank) << 13) | ((pal) << 9) | (tile))

%macro	ROM_HEADER	6	; Label, Segment, WSType, RomSize, SRamSize, WSSpec
	FILL_UP_SEGMENT %1

	db	0xEA	; jmpf
	dw	0	; Label
	dw	%2	; our segment
	db	0x00

	db	0x42	; Developer ID
	db	%3
	db	0x00	; Cart number
	db	0x00
	db	%4
	db	%5
	db	%6
	db	0x00
	dw	0x0000	; Checksum
%endmacro

%macro FILL_UP_SEGMENT 1 ; label
	times ((64*1024)-16)-$+%1 db 0x00
%endmacro

SCREEN_WIDTH	equ	224
SCREEN_HEIGHT	equ	144
SCREEN_TWIDTH	equ	(SCREEN_WIDTH / 8)
SCREEN_THEIGHT	equ	(SCREEN_HEIGHT / 8)
MAP_WIDTH	equ	256
MAP_HEIGHT	equ	256
MAP_TWIDTH	equ	(MAP_WIDTH / 8)
MAP_THEIGHT	equ	(MAP_HEIGHT / 8)



;-----------------------------------
; I/O Ports and associated equates

IO_DISPLAY_CTRL	equ	0x00
BG_ON		equ	0x01
BG_OFF		equ	0x00
FG_ON		equ	0x02
FG_OFF		equ	0x00
SPR_ON		equ	0x04
SPR_OFF		equ	0x00
SPR_WIN_ON	equ	0x08
SPR_WIN_OFF	equ	0x00
FG_IN_OUT_WIN	equ	0x00
FG_IN_WIN	equ	0x10
FG_OUT_WIN	equ	0x30

IO_BG_PAL	equ	0x01
%define	BG_COLOR(a)	(a)
%define	BG_PAL(a)	(a << 4)

IO_CUR_LINE	equ	0x02
IO_LINE_COMP	equ	0x03

IO_SPR_TABLE	equ	0x04
%define	SPR_TABLE(a)	(a >> 9)	; Sprite Table Address must be 512 bytes aligned !

IO_SPR_START	equ	0x05
IO_SPR_STOP	equ	0x06

IO_FGBG_MAP	equ	0x07
%define	FG_MAP(bank)	((bank & 0xf) << 4)
%define	BG_MAP(bank)	(bank & 0xf)
%define SET_MAP_LOCATIONS(fg_bank, bg_bank) ((fg_bank & 0xf) << 4) | (bg_bank & 0xf)

%define MAP_LOCATION(bank) ((bank & 0xf) << 11)

IO_FG_WIN_X0	equ	0x08
IO_FG_WIN_Y0	equ	0x09
IO_FG_WIN_X1	equ	0x0A
IO_FG_WIN_Y1	equ	0x0B

IO_SPR_WIN_X0	equ	0x0C
IO_SPR_WIN_Y0	equ	0x0D
IO_SPR_WIN_X1	equ	0x0E
IO_SPR_WIN_Y1	equ	0x0F

IO_BG_X		equ	0x10
IO_BG_Y		equ	0x11

IO_FG_X		equ	0x12
IO_FG_Y		equ	0x13

IO_LCD_CTRL	equ	0x14
LCD_ON		equ	0x01
LCD_OFF		equ	0x00

IO_LCD_ICONS	equ	0x15
LCD_ICON_SLEEP	equ	0x01
LCD_ICON_VERTI	equ	0x02
LCD_ICON_HORIZ	equ	0x04
LCD_ICON_DOT1	equ	0x08
LCD_ICON_DOT2	equ	0x10
LCD_ICON_DOT3	equ	0x20

IO_PALSHADE_10	equ	0x1C
IO_PALSHADE_32	equ	0x1D
IO_PALSHADE_54	equ	0x1E
IO_PALSHADE_76	equ	0x1F

IO_WS_PAL_00	equ	0x20
IO_WS_PAL_01	equ	0x21
IO_WS_PAL_10	equ	0x22
IO_WS_PAL_11	equ	0x23
IO_WS_PAL_20	equ	0x24
IO_WS_PAL_21	equ	0x25
IO_WS_PAL_30	equ	0x26
IO_WS_PAL_31	equ	0x27
IO_WS_PAL_40	equ	0x28
IO_WS_PAL_41	equ	0x29
IO_WS_PAL_50	equ	0x2A
IO_WS_PAL_51	equ	0x2B
IO_WS_PAL_60	equ	0x2C
IO_WS_PAL_61	equ	0x2D
IO_WS_PAL_70	equ	0x2E
IO_WS_PAL_71	equ	0x2F
IO_WS_PAL_80	equ	0x30
IO_WS_PAL_81	equ	0x31
IO_WS_PAL_90	equ	0x32
IO_WS_PAL_91	equ	0x33
IO_WS_PAL_A0	equ	0x34
IO_WS_PAL_A1	equ	0x35
IO_WS_PAL_B0	equ	0x36
IO_WS_PAL_B1	equ	0x37
IO_WS_PAL_C0	equ	0x38
IO_WS_PAL_C1	equ	0x39
IO_WS_PAL_D0	equ	0x3A
IO_WS_PAL_D1	equ	0x3B
IO_WS_PAL_E0	equ	0x3C
IO_WS_PAL_E1	equ	0x3D
IO_WS_PAL_F0	equ	0x3E
IO_WS_PAL_F1	equ	0x3F

;-----------------------
; DMA

IOw_DMA_SRC	equ	0x40
IO_DMA_SRC_BANK	equ	0x42
IO_DMA_DST_BANK	equ	0x43
IOw_DMA_DST	equ	0x44
IOw_DMA_SIZE	equ	0x46
IO_DMA_CTRL	equ	0x48
DMA_START	equ	0x80
DMA_CHECK	equ	0x80


%macro DMA_COPY 5; src_adr, src_bank, dest_adr, dest_bank, length
	; src
	mov ax, %1
	out IOw_DMA_SRC, ax
	mov al, %2
	out IO_DMA_SRC_BANK, al

	; dest
	mov ax, %3
	out IOw_DMA_DST, ax
	mov al, %4
	out IO_DMA_DST_BANK, al

	; size
	mov ax, %5
	out IOw_DMA_SIZE, ax

	; start
	mov al, DMA_START
	out IO_DMA_CTRL, al
%endmacro

%macro DMA_COPY 4; src_adr, src_bank, dest_adr (bank 0), length
	DMA_COPY %1, %2, %3, 0x0, %4
%endmacro

%macro DMA_COPY 3; src_adr (bank f), dest_adr (bank 0), length
	DMA_COPY %1, 0xf, %2, 0x0, %3
%endmacro


%macro	DMA_WAIT 0
%%waitDma:
	in al, IO_DMA_CTRL
	and al, DMA_CHECK
	jne %%waitDma
%endmacro


IOw_SNDDMA_SRC	equ	0x4A
IO_SNDDMA_BANK	equ	0x4C
IOw_SNDDMA_SIZE	equ	0x4E
IO_SNDDMA_CTRL	equ	0x52

IO_VIDEO_MODE	equ	0x60
VMODE_16C_CHK	equ	0xE0	; 16 colors per tile chunky mode
VMODE_16C_PLN	equ	0xC0	; 16 colors per tile planar mode
VMODE_4C	equ	0x40	; 4 colors per tile color
VMODE_4C_MONO	equ	0x00	; 4 colors per tile mono
VMODE_CLEANINIT	equ	0x0C	; (?) from FF2

IOw_AUDIO1_FREQ	equ	0x80	; Frequency
IOw_AUDIO2_FREQ	equ	0x82
IOw_AUDIO3_FREQ	equ	0x84
IOw_AUDIO4_FREQ	equ	0x86

IO_AUDIO1_VOL	equ	0x88	; Volume
IO_AUDIO2_VOL	equ	0x89
IO_AUDIO3_VOL	equ	0x8A
IO_AUDIO4_VOL	equ	0x8B

IO_AUDIO_SWEEP_VAL	equ	0x8C
IO_AUDIO_SWEEP_STEP	equ	0x8D

IO_AUDIO_NOISE_CTRL	equ	0x8E
%define	NOISE_TYPE(a)	(a)
NOISE_RESET		equ	0x08
NOISE_ENABLE		equ	0x10

IO_AUDIO_SAMPLE		equ	0x8F	; Sample  location
%define	AUDIO_SAMPLE(a)	(a >> 6)

IO_AUDIO_CTRL	equ	0x90
AUDIO_1_ON	equ	0x01
AUDIO_1_OFF	equ	0x00
AUDIO_2_ON	equ	0x02
AUDIO_2_OFF	equ	0x00
AUDIO_3_ON	equ	0x04
AUDIO_3_OFF	equ	0x00
AUDIO_4_ON	equ	0x08
AUDIO_4_OFF	equ	0x00
AUDIO_2_VOICE	equ	0x20
AUDIO_3_SWEEP	equ	0x40
AUDIO_4_NOISE	equ	0x80

IO_AUDIO_OUTPUT		equ	0x91
AUDIO_OUT_MONO		equ	0x01
AUDIO_OUT_STEREO	equ	0x08
%define	AUDIO_OUT_VOLUME(a)	((a & 0x03) << 1)

IOw_AUDIO_NOISE_CNT	equ	0x92
IO_AUDIO_VOLUME		equ	0x94	; Global Volume (4 bits)

IO_HARDWARE_TYPE	equ	0xA0
WS_COLOR		equ	0x02
WS_MONO			equ	0x00

IO_TIMER_CTRL			equ	0xA2
HBLANK_TIMER_ON			equ	0x01
HBLANK_TIMER_OFF		equ	0x00
HBLANK_TIMER_MODE_ONESHOT	equ	0x00
HBLANK_TIMER_MODE_AUTOPRESET	equ	0x02
VBLANK_TIMER_ON			equ	0x04
VBLANK_TIMER_OFF		equ	0x00
VBLANK_TIMER_MODE_ONESHOT	equ	0x00
VBLANK_TIMER_MODE_AUTOPRESET	equ	0x08

IOw_HBLANK_FREQ	equ	0xA4
IOw_VBLANK_FREQ	equ	0xA6

IO_HBLANK_CNT1	equ	0xA8	; Hblank Counter - 1/12000s
IO_HBLANK_CNT2	equ	0xA9	; Hblank Counter - 1/(12000>>8)s
IO_VBLANK_CNT1	equ	0xAA	; Vblank Counter - 1/75s
IO_VBLANK_CNT2	equ	0xAB	; Vblank Counter - 1/(75>>8)s

IO_INT_BASE	equ	0xB0
INT_BASE	equ	0x20

IO_COMM_DATA	equ	0xB1	; Communication byte

IO_INT_ENABLE		equ	0xB2
INT_HBLANK_TIMER	equ	0x80
INT_VBLANK_START	equ	0x40
INT_VBLANK_TIMER	equ	0x20
INT_DRAWING_LINE	equ	0x10
INT_SERIAL_RECEIVE	equ	0x08
INT_RTC_ALARM		equ	0x04
INT_KEY_PRESS		equ	0x02
INT_SERIAL_SEND		equ	0x01

IO_COMM_DIR		equ	0xB3	; Communication direction
COMM_RECEIVE_INT_GEN	equ	0x80
COMM_SPEED_9600		equ	0x00
COMM_SPEED_38400	equ	0x40
COMM_SEND_INT_GEN	equ	0x20
COMM_SEND_COMPLETE	equ	0x04
COMM_ERROR		equ	0x02
COMM_RECEIVE_COMPLETE	equ	0x01

IO_KEYPAD		equ	0xB5
KEYPAD_READ_ARROWS_V	equ	0x10
KEYPAD_READ_ARROWS_H	equ	0x20
KEYPAD_READ_BUTTONS	equ	0x40
PAD_UP			equ	0x01
PAD_RIGHT		equ	0x02
PAD_DOWN		equ	0x04
PAD_LEFT		equ	0x08
PAD_START		equ	0x02
PAD_A			equ	0x04
PAD_B			equ	0x08

IO_INT_ACK		equ	0xB6	; See IO_INT_ENABLE equates for values

IOw_INTERNAL_EEPROM_DATA	equ	0xBA
IOw_INTERNAL_EEPROM_ADDRESS	equ	0xBC

IOw_INTERNAL_EEPROM_CTRL	equ	0xBE
IEEPROM_INIT			equ	0x80
IEEPROM_PROTECT			equ	0x40
IEEPROM_WRITE			equ	0x20
IEEPROM_READ			equ	0x10
IEEPROM_WRITE_COMPLETE		equ	0x02
IEEPROM_READ_COMPLETE		equ	0x01

IO_ROM_BASE_BANK	equ	0xC0
IO_SRAM_BANK		equ	0xC1
IO_ROM_BANK_SEGMENT2	equ	0xC2
IO_ROM_BANK_SEGMENT3	equ	0xC3

IO_RTC_COMMAND		equ	0xCA
RTC_COMMAND_RESET	equ	0x10
RTC_COMMAND_ALARM	equ	0x12
RTC_COMMAND_SET_TIME	equ	0x14
RTC_COMMAND_GET_TIME	equ	0x15
RTC_COMMAND_ACK		equ	0x80

IO_RTC_DATA	equ	0xCB


; default init sequence
%macro INIT 0
	BITS 16
	
	cli
	cld

	mov ax, 0xF000
	mov ds, ax
	xor ax, ax
	mov es, ax

	; setup stack
	mov bp, cx
	mov ss, ax
	mov sp, WSC_STACK

	; clear Ram
	mov di, 0x0100
	mov cx, 0x7E80
	rep stosw

	out IO_SRAM_BANK,al
%endmacro	

INIT
