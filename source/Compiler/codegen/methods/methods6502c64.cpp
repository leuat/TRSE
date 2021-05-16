#include "methods6502c64.h"

Methods6502C64::Methods6502C64()
{

}

void Methods6502C64::Assemble(Assembler *as, AbstractCodeGen *dispenser)
{
    Methods6502::Assemble(as,dispenser);

    if (Command("waitforverticalblank"))
        WaitForVerticalBlank(as);
    else
    if (Command("fld"))
        FLD(as);
    else
    if (Command("jammer"))
        Jammer(as);


}

void Methods6502C64::WaitForVerticalBlank(Assembler *as)
{
    QString l1 = as->NewLabel("verticalblank1");
    QString l2 = as->NewLabel("verticalblank2");

    as->Label(l1);
    as->Asm("bit $D011");
    as->Asm("bpl "+l1);
    as->Label(l2);
    as->Asm("bit $D011");
    as->Asm("bmi "+l2);


    as->PopLabel("verticalblank1");
    as->PopLabel("verticalblank2");
}

void Methods6502C64::Jammer(Assembler *as)
{
    as->Comment("Jammer");
    QString lbl = as->NewLabel("jammer");
    m_node->m_params[0]->Accept(m_dispatcher);
    as->Term();
    //;sta     $7000
    as->Asm("cmp $d012");

    as->Asm("bcs "+lbl);
    as->Asm("lda #$02");
    as->Asm("sta     $0400");
    m_node->m_params[1]->Accept(m_dispatcher);
    as->Term();
    as->Asm("sta     $d020");
    as->Asm("sta     $d021");

    as->Asm("jmp     *");
    as->Label(lbl);
    as->PopLabel("jammer");
}

void Methods6502C64::FLD(Assembler *as)
{

    QSharedPointer<NodeNumber> num = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[1]);
    if (num==nullptr)
        ErrorHandler::e.Error("FLD: last parameter required to be pure constant number (0 or 1)");

    int val = num->m_val;

    QString lbl = as->NewLabel("fld");
    as->Comment("FLD effect");
    m_node->m_params[0]->Accept(m_dispatcher);
    as->Term();
    as->Asm("tax");

    as->Label(lbl);
    as->Asm("lda $d012 ; Wait for beginning of next line");
    as->Asm("cmp $d012");
    as->Asm("beq *-3");

    as->Asm("clc ; Do one line of FLD");
    as->Asm("lda $d011");
    as->Asm("adc #1");
    as->Asm("and #7");
    if (val==0)
        as->Asm("ora #$18");
    else
        as->Asm("ora #$38");
    as->Asm("sta $d011");

    as->Asm("dex ; Decrease counter");
    as->Asm("bne " + lbl);

    as->PopLabel("fld");
}
