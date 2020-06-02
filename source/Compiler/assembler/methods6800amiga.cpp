#include "methods6800amiga.h"

Methods68000Amiga::Methods68000Amiga()
{

}

void Methods68000Amiga::Assemble(Assembler *as, AbstractASTDispatcher *dispatcher)
{
    Methods68000::Assemble(as,dispatcher);

    if (Command("waitforblitter")) {
        QString lbl = as->NewLabel("waitforblitter");
        as->Label(lbl);
        as->Asm("btst	#14,DMACONR");
        as->Asm("bne.s	"+lbl);
        as->PopLabel("waitforblitter");
    }

    if (Command("vbirq")) {
        QSharedPointer<NodeProcedure> addr = qSharedPointerDynamicCast<NodeProcedure>(m_node->m_params[0]);
        QString name = addr->m_procedure->m_procName;

        as->Asm("move.l #"+name+",$6c.w");
    }

    if (Command("initcustomcopperlist"))
        as->Asm("lea copper_custom,a5");

    if (Command("endcustomcopperlist"))
        as->Asm("move.l #$fffffffe,(a5)+");

    if (Command("InitP61PlayerInternal")) {
        as->Asm("jsr init_p1_cont");
        as->IncludeFile(":resources/code/amiga/init_p61_player.s");
        as->IncludeFile(":resources/code/amiga/p61-play.i");
        as->Label("init_p1_cont");
    }
    if (Command("InitP61Module")) {
        as->Asm("movem.l	d0-a6,-(sp)");
        m_dispatcher->LoadAddress(m_node->m_params[0],"a0");
        //as->Asm("lea Module1,a0
        as->Asm("sub.l a1,a1");
        as->Asm("sub.l a2,a2");
        as->Asm("moveq #0,d0");
        as->Asm("lea $DFF000,a6");

        as->Asm("jsr P61_Init");
        as->Asm("movem.l (sp)+,d0-a6");

    }
    if (Command("PlayP61Module")) {
        as->Asm("movem.l d0-a6,-(sp)");
        as->Asm("lea $DFF000,a6");

        as->Asm("jsr P61_Music");
        as->Asm("movem.l (sp)+,d0-a6");

    }

    if (Command("addcoppercommand"))
        AddCopperCommand(as);

    if (Command("skipcoppercommands"))
        SkipCopperCommands(as);

    if (Command("EnableInterrupt"))
        EnableInterrupt(as);

    if (Command("InitPoly"))
        as->IncludeFile(":resources/code/amiga/poly.s");

    if (Command("InitLine"))
        as->IncludeFile(":resources/code/amiga/intline.s");



    if (Command("DrawLine"))
        DrawLine(as);




    if (Command("setcopperlist32"))
        SetCopperList32(as);


    if (Command("ApplyCopperList")) {

        QString a0 = as->m_regMem.Get();
        as->Asm("move.l #cop,"+a0);
        as->Asm("move.l "+a0+",COP1LCH");
        as->m_regMem.Pop(a0);
    }


    if (Command("setpalette"))
        Setpalette(as);
    if (Command("ablit"))
        ABlit(as, false);
    if (Command("fblit"))
        ABlit(as, true);



}
