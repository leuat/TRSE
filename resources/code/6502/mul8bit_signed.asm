
multiply_8bit_unsigned:
                bcc m8b_next
                    lda S_T1
                    sta m8s_sm1+1
                    sta m8s_sm3+1
                    eor #$ff
                    sta m8s_sm2+1
                    sta m8s_sm4+1
m8b_next:

                ldx S_T2
                sec
m8s_sm1:            lda square1_lo,x
m8s_sm2:            sbc square2_lo,x
                sta S_PRODUCT+0
m8s_sm3:            lda square1_hi,x
m8s_sm4:            sbc square2_hi,x
                sta S_PRODUCT+1

                rts


multiply_8bit_signed:
                jsr multiply_8bit_unsigned

                ; Apply sign (See C=Hacking16 for details).
                lda S_T1
                bpl sign_nxt
                    sec
                    lda S_PRODUCT+1
                    sbc S_T2
                    sta S_PRODUCT+1
sign_nxt:
                lda S_T2
                bpl sign_nxt2
                    sec
                    lda S_PRODUCT+1
                    sbc S_T1
                    sta S_PRODUCT+1
sign_nxt2

                rts
