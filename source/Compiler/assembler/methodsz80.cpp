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
    else if (Command("loop")) {
        QString lbl = as->NewLabel("end");
        as->Label(lbl);
        as->Asm("halt");
        as->Asm(";nop");
        as->Asm("jr "+lbl);
        as->PopLabel("end");
    }
    else if (Command("halt")) {
        as->Asm("halt");
    }
    else if (Command("setsprite"))
      SetSprite(as,0);
    else if (Command("initsprite"))
      SetSprite(as,1);
    else if (Command("initspritefromdata"))
      InitSpriteFromData(as,0);

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
        QString v= m_node->m_params[paramNo]->getValue(as);
        if (m_node->m_params[paramNo]->isPointer(as)) {
            as->Asm("ld a,["+v+"]");
            as->Asm("ld "+reg[0]+",a");
            as->Asm("ld a,["+v+"+1]");
            as->Asm("ld "+reg[1]+",a");
            return;
        }


        as->Asm("ld "+reg+", "+v);
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


void MethodsZ80::SetSprite(Assembler *as, int type)
{
//    LoadAddress(as,0);
    if (!m_node->m_params[2]->isPure())
        ErrorHandler::e.Error("Parameter 1 (x) must be pure constant or variable");
    if (!m_node->m_params[1]->isPure())
        ErrorHandler::e.Error("Parameter 2 (y) must be pure constant or variable");

    if (!m_node->m_params[3]->isPureNumeric())
        ErrorHandler::e.Error("Parameter 3 (width) must be constant value!");
    if (!m_node->m_params[4]->isPureNumeric())
        ErrorHandler::e.Error("Parameter 4 (height) must be constant value");


    int x = m_node->m_params[3]->getValueAsInt(as);
    int y = m_node->m_params[4]->getValueAsInt(as);


    QString addr = m_node->m_params[0]->getValue(as);
    as->Comment("SetSprite");
    for (int j=0;j<y;j++) {
        for (int i=0;i<x;i++) {
            int cnt = 4* (x*j+i);
            if (type==0) { // Set sprite
                LoadVar(as,2);
                if (j!=0)
                    as->Asm("add "+QString::number(j*8));
                as->Asm("ld ["+addr + "+"+QString::number(cnt)+" +0 ],a");
                LoadVar(as,1);
                if (i!=0)
                    as->Asm("add "+QString::number(i*8));
                as->Asm("ld ["+addr + "+"+QString::number(cnt)+" +1 ],a");
            }
            if (type==1) { // Init
                int dx = m_node->m_params[1]->getValueAsInt(as);
                int dy = m_node->m_params[2]->getValueAsInt(as);
                int num = (dx+i)+ (dy+j)*16;
                 as->Asm("ld a,"+QString::number(num));
                as->Asm("ld ["+addr + "+"+QString::number(cnt)+" +2 ],a");
            }
        }
    }
}

void MethodsZ80::InitSpriteFromData(Assembler *as, int type)
{
//    LoadAddress(as,0);
    if (!m_node->m_params[2]->isPure())
        ErrorHandler::e.Error("Parameter 2 (start) must be constant/variable");
    if (!m_node->m_params[3]->isPureNumeric())
        ErrorHandler::e.Error("Parameter 3 (length) must be pure constant");

    //    LoadAddress(as,0);
    if (!m_node->m_params[4]->isPureNumeric())
        ErrorHandler::e.Error("Parameter 4 (width) must be pure constant");
    if (!m_node->m_params[5]->isPureNumeric())
        ErrorHandler::e.Error("Parameter 5 (flip x) must be pure constant");

//    int start = m_node->m_params[2]->getValueAsInt(as);
    int length = m_node->m_params[3]->getValueAsInt(as);

    int width = m_node->m_params[4]->getValueAsInt(as);

    int attr = m_node->m_params[5]->getValueAsInt(as);




    QString addr = m_node->m_params[0]->getValue(as);
    as->Comment("InitSpriteFromData");
    LoadAddress(as,1);
    as->Asm("ld d,0");
    m_node->m_params[2]->Accept(m_dispatcher);
//    as->Asm("ld a,["+m_node->m_params[2]->getValue(as)+"]");
    as->Asm("ld e,a");
    as->Asm("add hl,de");
    int x = 0;
    int y =0;
    bool flipx = (attr & 0b00100000)==0b00100000;
    bool flipy = (attr & 0b01000000)==0b01000000;
    for (int i=0;i<length;i++) {
//        int cnt = 4* (i-start);
        int dx = x;
        int dy = y;
        if (flipx) dx = width-1-dx;
        if (flipy) dy = (length)/width-1-dy;
        int cnt = 4* (dx+dy*width);

        as->Asm("ld a,[hl+]");
        as->Asm("ld ["+addr + "+"+QString::number(cnt)+" +2 ],a");
        as->Asm("ld a,"+Util::numToHex(attr));
        as->Asm("ld ["+addr + "+"+QString::number(cnt)+" +3 ],a");
        x++;
        if (x==width) {
            x=0;
            y++;
        }
    }
}
