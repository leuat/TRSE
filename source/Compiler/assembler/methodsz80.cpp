#include "methodsz80.h"

MethodsZ80::MethodsZ80()
{

}

void MethodsZ80::Assemble(Assembler *as, AbstractASTDispatcher *dispatcher)
{
    m_dispatcher = dispatcher;
/*    if (Command("Writeln")) {
        as->Writeln();

        m_node->m_params[0]->Accept(dispatcher);
        as->EndWriteln();
    }
*/
  //   ASTdispatcherZ80* disp = dynamic_cast<ASTdispatcherZ80*>(m_dispatcher);
    as->ClearTerm();
    if (Command("fill"))
        Fill(as);
    else
    if (Command("poke")) {
        Poke(as);
    }
    else
    if (Command("memcpy"))
        MemCpy(as);
    else
        if (Command("waitforverticalblank")) {
            QString lbl = as->NewLabel("vblank");
            as->Label(lbl);
            as->Asm("ld	a,[rLY]");
            as->Asm("cp	$90");
            as->Asm("jr	nz,"+lbl);
            as->PopLabel("vblank");

        }
    else
    if (Command("waitforraster")) {
        QString lbl = as->NewLabel("vblank");
        as->Label(lbl);
        as->Asm("ld	a,[rLY]");
        as->Asm("cp	" +m_node->m_params[0]->getValue(as));
        as->Asm("jr	nz,"+lbl);
        as->PopLabel("vblank");

    }
    else if (Command("enablevblank")) {
        as->Asm("ld	a,IEF_VBLANK");
        as->Asm("ld	[rIE],a");
        as->Asm("ei");

    }
    else if (Command("halt")) {
        QString lbl = as->NewLabel("end");
        as->Label(lbl);
        as->Asm("halt");
        as->Asm(";nop");
        as->Asm("jr "+lbl);
        as->PopLabel("end");
    }
}

bool MethodsZ80::Command(QString name)
{
    return m_node->m_procName.toLower() == name.toLower();
}

void MethodsZ80::LoadVar(Assembler *as, int paramNo)
{
    //    qDebug() << "LOADVAR FORCE WORD  TYPE "<<paramNo <<m_node->m_params[paramNo]->m_builtInFunctionParameterType  <<m_node->m_params[paramNo]->isWord(as);
        if (m_node->m_params[paramNo]->m_builtInFunctionParameterType==BuiltInFunction::INTEGER
                && !m_node->m_params[paramNo]->isWord(as)) {
            m_node->m_params[paramNo]->setForceType(TokenType::INTEGER);
      //      qDebug() << "LOADVAR FORCE WORD "<<paramNo ;
        }

//   qDebug() << "ISN " << qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[paramNo]);
   m_node->m_params[paramNo]->Accept(m_dispatcher);


}

void MethodsZ80::LoadAddress(Assembler *as, int paramNo, QString reg)
{
    if (m_node->m_params[paramNo]->isPure()) {
        as->Asm("ld "+reg+", "+m_node->m_params[paramNo]->getValue(as));
        return;
    }

}

void MethodsZ80::Fill(Assembler *as)
{
    LoadAddress(as, 0);
    as->ClearTerm();
    LoadVar(as,2);
    as->ClearTerm();
    LoadVar(as,1);
    QString lblFill = "."+as->NewLabel("fill");
    QString lblSkip = "."+as->NewLabel("skip");

    as->Asm("inc b");
    as->Asm("inc c");
    as->Asm("jr "+lblSkip);
    as->Label(lblFill);
    as->Asm("ld [hl+],a");
    as->Label(lblSkip);
    as->Asm("dec c");
    as->Asm("jr nz,"+lblFill);
    as->Asm("dec b");
    as->Asm("jr nz,"+lblFill);

    as->PopLabel("fill");
    as->PopLabel("skip");
//    ld	hl,_HRAM			; clear hram
//	ld	c,$80				; a = 0, b = 0 here, so let's save a byte and 4 cycles
   // (ld c,$80 - 2/8 vs ld bc,$80 - 3/12)

/*finit_fill:
;-------------------------------------------------------------------------------
; a - byte to fill with
; hl - destination address
; bc - size of area to fill

    inc	b
    inc	c
    jr	.skip
.fill
    ld	[hl+],a
.skip
    dec	c
    jr	nz,.fill
    dec	b
    jr	nz,.fill
    ret
  */
}

void MethodsZ80::MemCpy(Assembler *as)
{
/*    ; hl - source address
    ; de - destination
    ; bc - size*/
    QString lblCopy = "."+as->NewLabel("copy");
    QString lblSkip = "."+as->NewLabel("skip");
    as->ClearTerm();
    LoadAddress(as,1,"de");
    as->Term();
    LoadAddress(as,0,"hl");
    as->Term();
    LoadVar(as,2);
    as->Asm("inc b");
    as->Asm("inc c");
    as->Asm("jr "+lblSkip);
    as->Label(lblCopy);

    as->Asm("ld	a,[hl+]");
    as->Asm("ld	[de],a");
    as->Asm("inc de");
    as->Label(lblSkip);
    as->Asm("dec c");
    as->Asm("jr	nz,"+lblCopy);
    as->Asm("dec b");
    as->Asm("jr	nz,"+lblCopy);
}

void MethodsZ80::Poke(Assembler *as)
{
    if (m_node->m_params[0]->isPureNumeric() &&
            m_node->m_params[1]->isPureNumeric() &&
            m_node->m_params[1]->getValueAsInt(as)==0) {

        LoadVar(as,2);
        as->Asm("ld ["+m_node->m_params[0]->getValue(as)+"],a");
        return;
    }


    LoadAddress(as,0);
//        as->Asm("ld a,"+m_node->m_params[1]->getValue(as));
    as->Term();
    if (m_node->m_params[1]->isPureNumeric() && m_node->m_params[1]->getValueAsInt(as)==0) {
        LoadVar(as,2);
        as->Asm("ld [hl],a");
        return;
    }
    LoadVar(as,1);
    as->Term();
    as->Asm("ld e,a");
    as->Asm("ld d,0");
    as->Asm("add hl,de");
    LoadVar(as,2);
    as->Asm("ld [hl],a");

}
