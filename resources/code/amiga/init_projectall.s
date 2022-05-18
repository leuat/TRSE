     jmp init_pall_cont
pa_czz dc.l 0
pa_cxx dc.l 0
pa_cyy dc.l 0

 
;		move.l pa_vert,a2
;		move.l pa_obj,a0

;		move.l len,d3
call_pall:
		
    move.l #0,d2
    move.l #0,d4


top_pall:

			move.l (a0,d4),d5
			asl.l #8,d5
	
			add.l #4,d4
	
			move.l (a0,d4),d6
			asl.l #8,d6

			add.l #4,d4
	
			move.l (a0,d4),d1
			add.l pa_czz,d1

                        divs.w d1,d5
                        add.l pa_cxx,d5

                        divs.w d1,d6
                        add.l pa_cyy,d6


                        move.w d5,(a2,d2)
                        add.l #2,d2
			
                        move.w d6,(a2,d2)
                        add.l #2,d2
			
                        add.l #4,d4
	

                        dbf d3,top_pall
			rts
init_pall_cont:
