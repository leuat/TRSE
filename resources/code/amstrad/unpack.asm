    org $4000
; data FIRST

firmware_RST_jp EQU $38
; dw 0

start   
    di
    call $B909; Disable ROM
     call $B903; Disable UROM
    ld hl,real_start
    ld de,$202
    ld bc,256
    ldir

 ld   hl, (firmware_RST_jp+1)   ;; [5] Obtain pointer to the present interrupt handler
 ex   de, hl                    ;; [1] DE = HL (DE saves present pointer to previous interrupt handler)

 im   1                        ;; [2] Set Interrupt Mode 1 (CPU will jump to 0x38 when a interrupt occurs)
 ld   hl, $C9FB               ;; [3] FB C9 (take into account little endian) => EI : RET

 ld (firmware_RST_jp), hl       ;; [5] Setup new "null interrupt handler" and enable interrupts again
 ;ei                             ;; [1]
 ex   de, hl                    ;; [1] HL = Pointer to previous interrupt handler (return value)

  ld ($200), hl
; push hl
di
    ; Set new stack pointer
    ld sp, $200

    jp $202

real_start
	ld hl,compressed_data
        ld de,$300


        jr LZ4_Version4
LZ4_version_not_supported
LZ4_decompress_error:
;    adc a,1
 ;   ld b,a
  ;  ld c, b
   ; call $BC38

	jp LZ4_decompress_error

LZ4_Version4:
; check version 1.41 magic 


	
	inc		hl
	inc		hl
	inc		hl
	inc		hl
	inc		hl
	inc		hl
	inc		bc


	add		hl,bc
; load low 16 bit of compreesed block size to bc
	ld		c,(hl)
	inc		hl
	ld		b,(hl)
	inc		hl
; check if compressed size <64kb - high 16 bits of compressed size must be 0
	ld		a,(hl)
	cp		0
	jr		nz,LZ4_decompress_error
	inc		hl
	ld		a,(hl)
	cp		0
	jr		nz,LZ4_decompress_error
	inc		hl

; decompress raw lz4 data packet
; on entry hl - start of packed buffer, de - destination buffer, bc - size of packed data
LZ4_decompress_raw:
	push	de							; store original destination pointer
	push	hl							; store start of compressed data source
	add		hl,bc       				; calculate end address of compressed block
	ld		b,h							; move end address of compressed data to bc
	ld		c,l	
	pop		hl							; restore start of compressed data source
	push	bc							; store end address of compessed data
; now hl - start of packed buffer, de - destination, bc - end of packed buffer
	ld		b,0		         			; clear b, c is set later

; get decompression token
LZ4_GetToken:
	xor		a 							; reset c flag for sbc later
	ld		a,(hl)						; read token
	inc		hl
	push	af							; store token
; unpack 4 high bits to get the length of literal
	rlca
	rlca
	rlca
	rlca
; copy literals
	and		#f							; token can be max 15 - mask out unimportant bits
	jr		z,LZ4_skipcalc   			; there is no literals, skip calculation of literal size
	ld		c,a							; set the count for calculation
	cp		#f							; if literal size <15
	jr		nz, LZ4_copyliterals		; copy literal, else
; calculate total literal size by adding contents of following bytes
	push	de							; store destination
	ex		de,hl
; a = size of literal to copy, de=pointer to data to be added
	ld		h,0         				; set hl with size of literal to copy 
	ld		l,a

LZ4_calcloop:
	ld		a,(de)						; get additional literal size to add 
	inc		de
	ld		c,a							; set bc to the length of literal
	add		hl,bc						; add it to the total literal length
	cp		#ff							; if literal=255
	jr		z,LZ4_calcloop				; continue calculating the total literal size
	ld		b,h							; store total literal size to copy in bc
	ld		c,l
	ex		de,hl						; hl now contains current compressed data pointer  
	pop		de							; restore destination to de 

LZ4_copyliterals:
	ldir								; copy literal to destination

LZ4_skipcalc:
; check for end of compressed data
	pop		af							; restore token, carry is cleared because of xor a at the beginning of GetToken
	pop		bc							; restore end address of compressed data 
	push	hl							; store current compressed data pointer 
	sbc		hl, bc						; check if we reached the end of compressed data buffer
	pop		hl							; restore current compressed data pointer
	jr		z,LZ4_decompress_success	; decompression finished
	push	bc							; store end address of compressed data

; Copy Matches
	and		#f							; token can be max 15 - mask out unimportant bits. resets also c flag for sbc later
; get the offset
	ld		c,(hl)
	inc		hl
	ld		b,(hl)						; bc now contains the offset
	inc		hl
	push	hl							; store current compressed data pointer
	push	de							; store destination pointer

	ex		de,hl
	sbc		hl,bc   					; calculate from the offset the new decompressed data source to copy from
; hl contains new copy source, de source ptr

	ld		b,0     					; load bc with the token
	ld		c,a
	cp		#f							; if matchlength <15
	jr nz, LZ4_copymatches				; copy matches. else 

; calculate total matchlength by adding additional bytes
	push	hl							; store current decompressed data source
; a = size of match to copy, de= pointer to data to be added
	ld		h,0     					; set hl with initial matchlength to copy
	ld		l,a
LZ4_calcloop2:
	ld		a,(de)						; get additional matchlength to add
	inc		de
	ld		c,a							; set bc to the matchlength
	add		hl,bc						; add it to the total match length
	cp		#ff							; if matchlength=255
	jr		z,LZ4_calcloop2				; continue calculating the total match length		
	ld		b,h							; store total matchlength to copy in bc
	ld		c,l			 
	pop		hl							; restore current decompressed data source
	pop		af							; set stack to proper position by restoring destination pointer temporarily into af  
	ex		de,hl
	ex		(sp),hl						; update current compressed data pointer on the stack to the new value from de
	ex		de,hl 
	push	af							; restore stack

LZ4_copymatches:
	pop		de							; restore destination pointer
	inc		bc							; add base length of 4 to get the correct size of matchlength 
	inc		bc
	inc		bc
	inc		bc
	ldir								; copy match
	pop		hl							; restore current compressed data source
	jr		LZ4_GetToken				; continue decompression
LZ4_decompress_success:
	pop		hl							; store destination pointer 
	sbc		hl,de						; calculate the number of decompressed bytes 
;	xor		a							; clear exit code


;	call $B900; Enable UROM

;    ld a,0
;    ld b,a
;    ld c, b
;    call $BC38
;loope:
;	jp loope
    ; restore

;    di
;    pop hl
;    im   1                        ;; [2] Set Interrupt Mode 1 (CPU will jump to 0x38 when a interrupt occurs)
;    ld (firmware_RST_jp), hl       ;; Restore
;    ei
    ld hl,[$200]
    di                         ;; [1] Disable interrupts
   ld a, $C3           ;; [2] A = 0xC3, opcode for JP instruction
   ld (firmware_RST_jp), a    ;; [4] Put JP instruction at 0x0038, to create a jump to the pointer at 0x0039
   ld (firmware_RST_jp+1), hl ;; [5] HL = previous interrupt handler pointer (firmware ROM pointer)
 ;  ei                         ;; [1] Reenable interrupts and return
    ld sp,$C000
        jp $300

  org @START
compressed_data:
   incbin "@FILE"

