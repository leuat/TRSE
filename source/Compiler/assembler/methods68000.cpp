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
    if (Command("poke8"))
        Poke(as,".b");
    if (Command("poke16"))
        Poke(as,".w");
    if (Command("poke32"))
        Poke(as,".l");

    if (Command("setcopperlist32"))
        SetCopperList32(as);

}

bool Methods68000::Command(QString name)
{

    return m_node->m_procName.toLower() == name.toLower();

}

void Methods68000::Poke(Assembler *as, QString bb)
{
    QString a0 = as->m_regMem.Get();
    m_node->m_params[0]->Accept(m_dispatcher);
  //  QString pre = "";
//    if (m_node->m_params[0]->isPureVariable())
    as->Asm("lea "+ as->m_varStack.pop() +","+a0);
    m_node->m_params[1]->Accept(m_dispatcher);

    as->Asm("move"+bb+" "+ as->m_varStack.pop() +","+"("+a0+")");


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



