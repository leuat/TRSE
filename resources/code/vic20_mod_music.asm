shrout = $ffd2   ; kernal addresss 
screen	= $1e00
colors	= $9600

low = $900a
mid = $900b
high	= $900c
noise	= $900d
volume	= $900e


mod_save_x = $70
mod_save_a = $71

pattern_pointer = $19
channel_pointer = $28	; product area for multiplication
workarea 		= $34

;   2014 SYS 4110 
                  ; machine code starts at $100e = #4110 

	jmp main_music


; player variables
order:	.byte	0
pattern:	.byte	0
row:		.byte	0



main_music:
		lda #$7f
		sta $912e     ; disable and acknowledge interrupts
		sta $912d
		sta $911e     ; disable NMIs (Restore key)

		
	ldx #0
		jsr		setup_music
		jmp		skip_order_wrap ; enter in the middle of the music loop
		
halt:	jmp		halt
		
		
setup_music: 	lda		#$08
				sta		volume
				rts
		
; main pattern loop
music:	ldx #0	
			
patloop: stx	row

		txa
		pha	; push x
		
		; at the beginning we point at the first channel
		jsr reset_channel_pointer
		
		ldx #0
channel_loop: stx $26	; we save the channel index to zero page
		txa
		pha	; push x
		
		clc
		lda channel_pointer
		adc row	; row is just 8 bits long variable
		sta workarea
		lda channel_pointer+1
		adc #0	; just apply the carry flag if needed
		sta workarea+1
		
		; row pointer now lies at workarea - workarea+1
                ldy #0
                lda (workarea),y
		ldx $26 ; load the channel index to X
		
		; do nothing if note is 0x00
		cmp #$00		
		beq skip_channel_update
		; 0x01 is notecut
		cmp #$01
		bne skip_notecut
		lda #0
		sta low,x
		jmp end_effects
skip_notecut: cmp #16
		;jmp end_effects
		; branch if less than 15
		bcc effect_speed
		clc
		; branch if more than 32
		cmp #32
		beq loop_label1
		bcs effect_speed
loop_label1:	clc
		sbc #15
                ora volume ; overwrite auxiliary color information like a boss
                sta volume
		;sta screen+6
		jmp skip_channel_update
		
effect_speed:	cmp #$20
	bcc end_effects
	clc
	; branch if more than 64
	cmp #$40
	beq loop_label2
	bcs end_effects	


loop_label2: 
	clc	
	sbc #$20
	asl 
	asl
	asl
	sta ticklength
	
	jmp skip_channel_update
	
		
end_effects:	sta low,x	

		
skip_channel_update:	clc
		
		jsr increase_channel_pointer
		
		pla
		tax	
		inx
		txa
		cmp #4
		bne channel_loop
	
		
		pla
		tax	; pull x

		
;		 jsr delay
				

		rts


music_cont:


		inx
		cpx #64	
		bne patloop
		
		
		; if the current order is over order_amount, then loop
		ldx order
		inx
		cpx order_amount
		bne skip_order_wrap
		ldx #0
skip_order_wrap:	stx order
				
		; load the correct pattern index
		lda orderlist,x
		sta pattern
		
		; store pattern0 pointer in zeropage
reset_pattern_pointer_two:
		ldx #<pattern0
		stx workarea
		ldx #>pattern0
		stx workarea+1
		
		jsr reset_pattern_pointer
		
		
		ldx #0
mult_loop:	cpx pattern	; pattern index
		beq leave_mult

		inx
		
		lda workarea
		; no addition needs to be done since 0x0100 lower byte is 0x00
		sta pattern_pointer ; less significant output byte
		lda	workarea+1
		clc
		adc #1
		sta pattern_pointer+1 ; more significant output byte
		sta workarea+1
		
		jmp mult_loop

leave_mult: jmp music ; } end_while 

; stores pattern0 pointer in zeropage at pattern_pointer
reset_pattern_pointer: ldx #<pattern0
		stx pattern_pointer		
		ldx #>pattern0
		stx pattern_pointer+1
		rts
		
reset_channel_pointer: lda pattern_pointer
		sta channel_pointer
		lda pattern_pointer+1
		sta channel_pointer+1		
		rts

; calculate row pointer by adding offset to pattern_pointer
; save the offsetted value as channel_pointer
increase_channel_pointer:	lda channel_pointer
		adc #64	
		sta channel_pointer
		lda channel_pointer+1
		adc #0	; just apply the carry flag if needed
		sta channel_pointer+1

		rts
		
