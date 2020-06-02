#include "methods68000atari.h"

Methods68000Atari::Methods68000Atari()
{

}


void Methods68000Atari::Assemble(Assembler *as, AbstractASTDispatcher *dispatcher)
{
    Methods68000::Assemble(as,dispatcher);

    if (Command("getscreen")) {
        if (!m_node->m_params[0]->isPureVariable())
            ErrorHandler::e.Error("Parameter must be a pointer in which to store the screen address", m_node->m_lineNumber);
        as->Comment("Getscreen");
        as->Asm("move.w  #2,-(a7)                ; get physbase");
        as->Asm("trap    #14                      ; call XBIOS");
        as->Asm("addq.l  #2,a7                   ; clean up stack");
        as->Asm("move.l  d0,"+m_node->m_params[0]->getValue(as));

    }
    if (Command("waitforkeypress")) {
        as->Asm("move.w  #7,-(a7)");
        as->Asm("trap    #1     ");
        as->Asm("addq.l  #2,a7  ");
    }
    if (Command("keypressed")) {
        as->Asm("move.w #0,d0");
        as->Asm("cmp.b " +m_node->m_params[0]->getValue(as)+",$fffc02");
        QString lbl = as->NewLabel("keypressed");
        as->Asm("bne "+lbl);
        as->Asm("move.w #1,d0");
        as->Label(lbl);
        as->PopLabel("keypressed");

        as->m_varStack.push("d0");
    }
    if (Command("setlores")) {
        as->Asm("move.w  #0,-(a7)                ; low resolution");
        as->Asm("move.l  #-1,-(a7)               ; keep physbase");
        as->Asm("move.l  #-1,-(a7)               ; keep logbase");
        as->Asm("move.w  #5,-(a7)                ; change screen");
        as->Asm("trap    #14");
        as->Asm("add.l   #12,a7");

    }
    if (Command("initmusic")) {
//        ;    move.l  #ym_file,a0              ; start of ym file
        m_dispatcher->LoadAddress(m_node->m_params[0]);
        as->Asm("jsr internal_initmusic");
    }
    if (Command("updatemusic")) {
//        ;    move.l  #ym_file,a0              ; start of ym file
        as->Asm("jsr internal_playmusic");
    }
    if (Command("settimerirq")) {
        QSharedPointer<NodeProcedure> addr1 = qSharedPointerDynamicCast<NodeProcedure>(m_node->m_params[0]);
        QString name = addr1->m_procedure->m_procName;

        as->Asm("clr.b   $fffffa1b                ; disable timer b");
        as->Asm("move.l  #"+name+",$120           ; move in my timer b address");
        as->Asm("bset    #0,$fffffa07            ; turn on timer b in enable a");
        as->Asm("bset    #0,$fffffa13            ; turn on timer b in mask a");
        as->Asm("move.b  "+m_node->m_params[1]->getValue(as) +",$fffffa21  ; scanline");
        as->Asm("move.b  #8,$fffffa1b            ; set timer b to event count mode");

    }
    if (Command("setirq")) {
        QSharedPointer<NodeProcedure> addr1 = qSharedPointerDynamicCast<NodeProcedure>(m_node->m_params[0]);
        QString name = addr1->m_procedure->m_procName;

        as->Asm("move.l #"+name+","+m_node->m_params[1]->getValue(as));
    }
    else
//    m; HLine; ATARI520ST; a, i,i,i

    if (Command("InitGraphics")) {
        as->Asm("bsr gfx_find_phys");
        as->Asm("bsr gfx_wrt_phys_tbl");
        as->Asm("bsr holine_hline_lu");
    }
    else
    if (Command("HLine")) {
        /**HOLINE. A horizontal line is drawn from left to right.
        * passes: x1=d2.w, y1=d3.w, N=d1.w, colour=d4.w, screen y table:a4.l
        * First find the address of the word at which the line starts.
        holine_holine
*/
        m_node->m_params[0]->Accept(m_dispatcher);
        Asm(as,"move.w",as->m_varStack.pop(),"d2");
        m_node->m_params[1]->Accept(m_dispatcher);
        Asm(as,"move.w",as->m_varStack.pop(),"d3");
        m_node->m_params[2]->Accept(m_dispatcher);
        Asm(as,"move.w",as->m_varStack.pop(),"d1");
        m_node->m_params[3]->Accept(m_dispatcher);
        Asm(as,"move.w",as->m_varStack.pop(),"d4");
        as->Asm("lea gfx_phys_tbl_y,a4");
        as->Asm("bsr holine_holine");
    }


}
