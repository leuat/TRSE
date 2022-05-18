	times ((64*1024)-16-($ - $$)) db 0
	
	db 0xEA
    dw main_block_begin_
	dw 0xf000 		;// segment (last one)
	db 0x00
	
	db 0x42			;// dev ID
	db 0x1			;// system: 0=ws, 1=ws-color
	db 0x0			;// cart id 0=developer
	db 0x0			;// unknown
	db 0x2			;// ROM Size (2=4MBit)
	db 0x0			;// SRAM Size
	db 0x4			;// additional stuff (second bit must be set)
	db 0x0			;// RTC on/off
	
	dw 0x0000			;// checksum
	
