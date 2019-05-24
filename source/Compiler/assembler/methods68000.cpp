#include "methods68000.h"

Methods68000::Methods68000()
{

}

void Methods68000::Assemble(Assembler *as, AbstractASTDispatcher *dispatcher)
{
    m_dispatcher = dispatcher;
/*    if (Command("Writeln")) {
        as->Writeln();

        m_node->m_params[0]->Accept(dispatcher);
        as->EndWriteln();
    }
*/

/*    m; InitCustomCopperList; Amiga;
    m; AddCopperCommand; i,i
    m; SkipCopperCommands;i
  */
    if (Command("initcustomcopperlist"))
        as->Asm("lea copper_custom,a5");
    if (Command("endcustomcopperlist"))
        as->Asm("move.l #$fffffffe,(a5)+");
    if (Command("addcoppercommand"))
        AddCopperCommand(as);
    if (Command("skipcoppercommands"))
        SkipCopperCommands(as);

    if (Command("poke8"))
        Poke(as,".b");
    if (Command("poke16"))
        Poke(as,".w");
    if (Command("poke32"))
        Poke(as,".l");

    if (Command("return"))
        as->Asm("rts");

    if (Command("setcopperlist32"))
        SetCopperList32(as);

    if (Command("WaitVerticalBlank")) {
        QString lbl = as->NewLabel("waitVB");
        QString d0 = as->m_regAcc.Get();
        as->Label(lbl);
        as->Asm("move.l VPOSR,"+d0);
        as->Asm("and.l #$1ff00,"+d0);
        as->Asm("cmp.l #300<<8,"+d0);
        as->Asm("bne "+lbl);

        as->m_regAcc.Pop(d0);
        as->PopLabel("waitVB");

    }
    if (Command("ApplyCopperList")) {

        QString a0 = as->m_regMem.Get();
        as->Asm("move.l #cop,"+a0);
        as->Asm("move.l "+a0+",COP1LCH");
        as->m_regMem.Pop(a0);
    }
    if (Command("memcpy"))
        Memcpy(as);
    if (Command("setpalette"))
        Setpalette(as);
    if (Command("ablit"))
        ABlit(as);

}

bool Methods68000::Command(QString name)
{

    return m_node->m_procName.toLower() == name.toLower();

}

void Methods68000::LoadVariable(Assembler* as, QString cmd, Node* n, QString d0)
{
    if (n->getValue()!="") {
        Asm(as,cmd,n->getValue(),d0);
    }
    else {
        n->Accept(m_dispatcher);
        Asm(as,cmd,as->m_varStack.pop(),d0);
    }

}

void Methods68000::LoadAddress(Assembler *as, Node *n, QString d0)
{
    Asm(as,"lea",n->getLiteral(),d0);
}

void Methods68000::Poke(Assembler *as, QString bb)
{
    QString a0 = as->m_regMem.Get();
    as->Comment("Poke command");
    LoadAddress(as,m_node->m_params[0],a0);
   // m_dispatcher->LoadVariable(m_node->m_params[2]);
    m_node->m_params[2]->Accept(m_dispatcher);
    QString val = as->m_varStack.pop();
//    m_node->m_params[2]->Accept(m_dispatcher);
    NodeNumber* num = dynamic_cast<NodeNumber*>(m_node->m_params[1]);
    if (num!=nullptr) {
        if (num->m_val==0) {
            as->Asm("move"+bb+" "+ val +","+"("+a0+")");

            as->m_regMem.Pop(a0);
            return;
        }
    }

/*    if (m_node->m_params[2]->isPureNumeric() || m_node->m_params[2]->isPureVariable()) {

    }
  */
//    m_node->m_params[2]->Accept(m_dispatcher);

//    Asm(as,"move",as->m_varStack.pop(),d0);
//    m_dispatcher->LoadVariable(m_node->m_params[1]);
    m_node->m_params[1]->Accept(m_dispatcher);
    Asm(as,"add.w",as->m_varStack.pop(),a0 + "; "+val);
    as->Asm("move"+bb+" "+ val +","+"("+a0+")");

    as->m_regMem.Pop(a0);

}

void Methods68000::SetCopperList32(Assembler *as)
{
    QString val1 = m_node->m_params[0]->getLiteral();
    QString addr = m_node->m_params[1]->getLiteral();
    QString a0 = as->m_regMem.Get();
    QString d0 = as->m_regAcc.Get();

    Asm(as,"lea",val1,a0);
    Asm(as,"move.l",a0,d0);
    Asm(as,"lea",addr,a0);
    Asm(as,"move.w",d0,"4("+a0+")");
    as->Asm("swap "+d0);
    Asm(as,"move.w",d0,"("+a0+")");

    as->m_regMem.Pop(a0);
    as->m_regAcc.Pop(d0);



 /*   lea    Screens_1,a0
    move.l  a0,d0
    lea        b0h,a0
    move.w    d0,4(a0)
    swap    d0
    move.w    d0,(a0)
;    adda.w    #8,a0
   */
}

void Methods68000::Memcpy(Assembler *as)
{
    QString a0 = as->m_regMem.Get();
    QString a1 = as->m_regMem.Get();
    QString d0 = as->m_regAcc.Get();
    QString lbl = as->NewLabel("memcpy");

    LoadVariable(as, "move.l",m_node->m_params[4], d0);
    LoadVariable(as, "lea", m_node->m_params[0], a0);
    m_node->m_params[1]->Accept(m_dispatcher);
    Asm(as,"add.w",as->m_varStack.pop(), a0);


    LoadVariable(as, "lea",m_node->m_params[2], a1);
    m_node->m_params[3]->Accept(m_dispatcher);
    Asm(as,"add.w",as->m_varStack.pop(), a1);

    as->Label(lbl);
    as->Asm("move.w ("+a0+")+,("+a1+")+");
    as->Asm("dbf "+d0+","+lbl);

    as->m_regMem.Pop(a0);
    as->m_regMem.Pop(a1);
    as->m_regAcc.Pop(d0);
    as->PopLabel("memcpy");
}

void Methods68000::Setpalette(Assembler *as)
{
    QString a0 = as->m_regMem.Get();
    QString a1 = as->m_regMem.Get();
    QString d0 = as->m_regAcc.Get();
    QString lbl = as->NewLabel("setpalette");

    LoadVariable(as, "move.l",m_node->m_params[2], d0);
    LoadVariable(as, "lea", m_node->m_params[0], a0);
    LoadVariable(as, "lea",m_node->m_params[1], a1);
    as->Label(lbl);

    as->Asm("addq.l #2,"+a1+"");
    as->Asm("move.w ("+a0+")+,("+a1+")+");
    as->Asm("dbf "+d0+","+lbl);

    as->m_regMem.Pop(a0);
    as->m_regMem.Pop(a1);
    as->m_regAcc.Pop(d0);
    as->PopLabel("setpalette");

}

void Methods68000::ABlit(Assembler *as)
{
/*    m; ablit; Amiga; a,a, i,i,i,l,i


    ; a0 = source
    ; a1 = dest
    ; d1 = dst x
    ; d2 = dst y
    ; d3 = modulo
    ; d4 = blitter size
//    ; d5 = bltmod
  */

    as->Asm("lea     $dff000,a6 ; Hardware registers");

    LoadAddress(as,m_node->m_params[0], "a0"); // src
    LoadAddress(as,m_node->m_params[1], "a1"); // dst
    as->Asm("move.l #0,d6");
    LoadVariable(as,"move.w",m_node->m_params[2], "d6"); // Offset
    LoadVariable(as,"move.w",m_node->m_params[3], "d1"); // dst x
    LoadVariable(as,"move.w",m_node->m_params[4], "d2"); // dst y
    LoadVariable(as,"move.w",m_node->m_params[5], "d3"); // add
    LoadVariable(as,"move.w",m_node->m_params[6], "d4");
    LoadVariable(as,"move.w",m_node->m_params[7], "BLTAMOD(a6)"); // mod SRC
    LoadVariable(as,"move.w",m_node->m_params[8], "BLTDMOD(a6)"); // mod SRC

    as->Asm("jsr blitter");
}

void Methods68000::AddCopperCommand(Assembler* as)
{
    m_node->m_params[0]->Accept(m_dispatcher);
    Asm(as,"move.w",as->m_varStack.pop(),"(a5)+");
    m_node->m_params[1]->Accept(m_dispatcher);
    Asm(as,"move.w",as->m_varStack.pop(),"(a5)+");

}

void Methods68000::SkipCopperCommands(Assembler *as)
{
    m_node->m_params[0]->Accept(m_dispatcher);
    Asm(as,"add.l",as->m_varStack.pop(),"a5");

}



