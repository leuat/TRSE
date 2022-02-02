.orgfill 0x2100
.db	"MN"

.orgfill 0x2102
        jmp	main_block_begin_
.orgfill 0x2108
        jmp	uirq
.orgfill 0x210E
        jmp	uirq
.orgfill 0x2114
        jmp	uirq
.orgfill 0x211A
        jmp	uirq
.orgfill 0x2120
        jmp	uirq
.orgfill 0x2126
        jmp	uirq
.orgfill 0x212C
        jmp	uirq
.orgfill 0x2132
        jmp	uirq
.orgfill 0x2138
        jmp	uirq
.orgfill 0x213E
        jmp	uirq
.orgfill 0x2144
        jmp	uirq
.orgfill 0x214A
        jmp	uirq
.orgfill 0x2150
        jmp	uirq
.orgfill 0x2156
        jmp	uirq
.orgfill 0x215C
        jmp	uirq
.orgfill 0x2162
        jmp	uirq
.orgfill 0x2168
        jmp	uirq
.orgfill 0x216E
        jmp	uirq
.orgfill 0x2174
        jmp	uirq
.orgfill 0x217A
        jmp	uirq
.orgfill 0x2180
        jmp	uirq
.orgfill 0x2186
        jmp	uirq
.orgfill 0x218C
        jmp	uirq
.orgfill 0x2192
        jmp	uirq
.orgfill 0x2198
        jmp	uirq
.orgfill 0x219E
        jmp	uirq



.orgfill 0x21A4
.db	"NINTENDO"
.db	"TRSE"
.db	"TRSE PROGRAM"
.orgfill 0x21Bc
.db	"2P"

.orgfill 0x21D0

uirq:	reti				# unhandled interrupt
