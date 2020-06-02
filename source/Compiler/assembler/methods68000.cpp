#include "methods68000.h"

Methods68000::Methods68000()
{

}

void Methods68000::Assemble(Assembler *as, AbstractASTDispatcher *dispatcher)
{
    m_dispatcher = dispatcher;

    if (Command("InitProjectAllVertices")) {
        as->IncludeFile(":resources/code/amiga/init_projectall.s");
    }
    if (Command("ProjectAllVertices")) {
        ProjectAllVertices(as);
    }

    if (Command("matmul3x3"))
        MatMul(as);

    if (Command("Fill"))
        Fill(as);


    if (Command("initmatmul3x3"))
        as->IncludeFile(":resources/code/amiga/matmul.s");

    if (Command("initmatmulvec"))
        as->IncludeFile(":resources/code/amiga/matmulvec.s");

    if (Command("initmatmulvecnormalz"))
        as->IncludeFile(":resources/code/amiga/matmulvec_norm_z.s");

    if (Command("matmulvec"))
        MatMulVec(as);

    if (Command("matmulvecnormalz"))
        MatMulVecNormalZ(as, true);

    if (Command("matmulvecnormal"))
        MatMulVecNormalZ(as,false);

    if (Command("setrotationx"))
        SetRotation(as,"x");
    if (Command("setrotationy"))
        SetRotation(as,"y");
    if (Command("setrotationz"))
        SetRotation(as,"z");
    if (Command("setidentity"))
        SetRotation(as,"id");


    if (Command("poke8"))
        Poke(as,".b");
    if (Command("poke16"))
        Poke(as,".w");
    if (Command("poke32"))
        Poke(as,".l");


//    if (Command("InitProjectToScreen"))
  //      as->IncludeFile(":resources/code/amiga/simpleproject.s");

    if (Command("ProjectToScreen"))
        ProjectToScreen(as);

    if (Command("return"))
        as->Asm("rts");

    if (Command("pusha")) {
        as->Asm("movem.l d0-d7/a0-a6,-(sp)");
    }
    if (Command("popa")) {
        as->Asm("movem.l (sp)+,d0-d7/a0-a6");
    }
    if (Command("closeirq")) {
        as->Asm("movem.l (sp)+,d0-d7/a0-a6");
        as->Asm("moveq    #0,d0");
    }

    if (Command("WaitVerticalBlank")) {
        if (Syntax::s.m_currentSystem->m_system==AbstractSystem::AMIGA) {
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
        if (Syntax::s.m_currentSystem->m_system==AbstractSystem::ATARI520ST) {
            as->Asm("; Wait for vertical blank");
            as->Asm("move.w  #37,-(a7)   ");
            as->Asm("trap    #14         ");
            as->Asm("addq.l  #2,a7       ");
        }

    }
    if (Command("memcpy"))
        Memcpy(as);
    if (Command("memcpyunroll"))
        MemcpyUnroll(as);




}

bool Methods68000::Command(QString name)
{

    return m_node->m_procName.toLower() == name.toLower();

}

void Methods68000::LoadVariable(Assembler* as, QString cmd, QSharedPointer<Node> n, QString d0)
{
    if (n->getValue(as)!="") {
        Asm(as,cmd,n->getValue(as),d0);
    }
    else {
        n->Accept(m_dispatcher);
        Asm(as,cmd,as->m_varStack.pop(),d0);
    }

}

/*void Methods68000::LoadAddress(Assembler *as, QSharedPointer<Node> n, QString d0)
{
    Asm(as,"lea",n->getLiteral(as),d0);
}
*/
void Methods68000::DrawLine(Assembler *as)
{
/*    ; d0=X1  d1=Y1  d2=X2  d3=Y2  d5=Screenwidth  a0=address a6=$dff000
    move.w dx1,d0
    move.w dx2,d1
    move.w dy1,d2
    move.w dy2,d3
    move.l #40,d5
    lea screenb1,a0
    lea	$dff000,a6

    jsr drawLine
  */
    as->Comment("DrawLine method");
    m_node->m_params[0]->Accept(m_dispatcher);
    Asm(as,"move.l",as->m_varStack.pop(),"d0");
    m_node->m_params[1]->Accept(m_dispatcher);
    Asm(as,"move.l",as->m_varStack.pop(),"d1");
    m_node->m_params[2]->Accept(m_dispatcher);
    Asm(as,"move.l",as->m_varStack.pop(),"d2");
    m_node->m_params[3]->Accept(m_dispatcher);
    Asm(as,"move.l",as->m_varStack.pop(),"d3");
    m_node->m_params[5]->Accept(m_dispatcher);
    Asm(as,"move.w",as->m_varStack.pop(),"d5");
    //m_node->m_params[4]->Accept(m_dispatcher);
    //Asm(as,"lea",as->m_varStack.pop(),"a0");
    m_dispatcher->LoadAddress(m_node->m_params[4]);
    Asm(as,"move.l",as->m_varStack.pop(),"a0");
    as->Asm("lea	$dff000,a6");
    as->Asm("jsr drawLine");


}

void Methods68000::Fill(Assembler *as)
{
    as->Comment("Fill method");
    m_node->m_params[2]->Accept(m_dispatcher);
    Asm(as,"move.l",as->m_varStack.pop(),"d0");
    m_node->m_params[1]->setForceType(TokenType::LONG);
    m_node->m_params[1]->Accept(m_dispatcher);
    Asm(as,"move.l",as->m_varStack.pop(),"d2");
//    m_node->m_params[0]->Accept(m_dispatcher);
    m_dispatcher->LoadAddress(m_node->m_params[0]);
    Asm(as,"move.l",as->m_varStack.pop(),"a0");

    QString lbl = as->NewLabel("fill");
    as->Label(lbl);
    as->Asm("move.l d2,(a0)+");
    as->Asm("dbf d0,"+lbl);

    as->PopLabel("fill");
}

void Methods68000::Poke(Assembler *as, QString bb)
{
    QString a0 = as->m_regMem.Get();
    as->Comment("Poke command");
    //LoadAddress(as,m_node->m_params[0],a0);
    m_dispatcher->LoadAddress(m_node->m_params[0],a0);
   // m_dispatcher->LoadVariable(m_node->m_params[2]);
    m_node->m_params[2]->Accept(m_dispatcher);
    QString val = as->m_varStack.pop();
//    m_node->m_params[2]->Accept(m_dispatcher);
    QSharedPointer<NodeNumber> num = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[1]);
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
    QString val1 = m_node->m_params[0]->getLiteral(as);
    QString addr = m_node->m_params[1]->getLiteral(as);
    QString a0 = as->m_regMem.Get();
    QString d0 = as->m_regAcc.Get();


   // org
  //  Asm(as,"lea",val1,a0);
  //  Asm(as,"move.l",a0,d0);

    as->Comment("setcopperlist32");
    m_dispatcher->LoadAddress(m_node->m_params[0]);
    Asm(as,"move.l",as->m_varStack.pop(),d0);

    Asm(as,"lea",addr,a0);
    Asm(as,"move.w",d0,"6("+a0+")");
    as->Asm("swap "+d0);
    Asm(as,"move.w",d0,"2("+a0+")");

    as->m_regMem.Pop(a0);
    as->m_regAcc.Pop(d0);


}


void Methods68000::Memcpy(Assembler *as)
{
    QString a0 = as->m_regMem.Get();
    QString a1 = as->m_regMem.Get();
    QString d0 = as->m_regAcc.Get();
    QString lbl = as->NewLabel("memcpy");


    if (!m_node->m_params[5]->isPureNumeric())
        ErrorHandler::e.Error("MemCpyUnroll requires parameter 5 to be a constant", m_node->m_op.m_lineNumber);

    int type = m_node->m_params[5]->getValueAsInt(as);
    QString ending = ".b";
    if (type==2) ending=".w";
    if (type==4) ending=".l";



    LoadVariable(as, "move",m_node->m_params[4], d0);
//    LoadVariable(as, "lea", m_node->m_params[0], a0);
    m_dispatcher->LoadAddress(m_node->m_params[0],a0);
 //   m_dispatcher->LoadAddress()
    m_node->m_params[1]->Accept(m_dispatcher);
    Asm(as,"add"+m_dispatcher->getEndType(as,m_node->m_params[1]),as->m_varStack.pop(), a0);


    bool ok = true;
    if (m_node->m_params[3]->isPureNumeric()) {
        if (m_node->m_params[3]->getValueAsInt(as)==0) {
            ok = false;
        }
    }
//    qDebug() << "HERE" << m_dispatcher->getEndType(as,m_node->m_params[3]);
  //  qDebug()  << a1;
    m_dispatcher->LoadAddress(m_node->m_params[2],a1);
     m_node->m_params[3]->Accept(m_dispatcher);
    if (ok)
        Asm(as,"add"+m_dispatcher->getEndType(as,m_node->m_params[3]),as->m_varStack.pop(), a1);

    as->Label(lbl);
    as->Asm("move"+ending+" ("+a0+")+,("+a1+")+");
    as->Asm("dbf "+d0+","+lbl);

    as->m_regMem.Pop(a0);
    as->m_regMem.Pop(a1);
    as->m_regAcc.Pop(d0);
    as->PopLabel("memcpy");
}

void Methods68000::MemcpyUnroll(Assembler *as)
{
    QString a0 = as->m_regMem.Get();
    QString a1 = as->m_regMem.Get();
    QString d0 = as->m_regAcc.Get();
    QString lbl = as->NewLabel("memcpy");

    if (!m_node->m_params[4]->isPureNumeric())
        ErrorHandler::e.Error("MemCpyUnroll requires parameter 4 to be a constant", m_node->m_op.m_lineNumber);

    if (!m_node->m_params[5]->isPureNumeric())
        ErrorHandler::e.Error("MemCpyUnroll requires parameter 5 to be a constant", m_node->m_op.m_lineNumber);

    int cnt = m_node->m_params[4]->getValueAsInt(as);

    int type = m_node->m_params[5]->getValueAsInt(as);
    QString ending = ".b";
    if (type==2) ending=".w";
    if (type==4) ending=".l";

    m_dispatcher->LoadAddress(m_node->m_params[0],a0);
 //   m_dispatcher->LoadAddress()
    m_node->m_params[1]->Accept(m_dispatcher);
    Asm(as,"add"+m_dispatcher->getEndType(as,m_node->m_params[1]),as->m_varStack.pop(), a0);


    m_dispatcher->LoadAddress(m_node->m_params[2],a1);
    bool ok = true;
    if (m_node->m_params[3]->isPureNumeric() && m_node->m_params[3]->getValueAsInt(as)==0)
        ok = false;
    if (ok)
        Asm(as,"add"+m_dispatcher->getEndType(as,m_node->m_params[3]),as->m_varStack.pop(), a1);
    for (int i=0;i<cnt;i++)
        as->Asm("move"+ending+" ("+a0+")+,("+a1+")+");

    as->m_regMem.Pop(a0);
    as->m_regMem.Pop(a1);
    as->m_regAcc.Pop(d0);
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

void Methods68000::ABlit(Assembler *as, bool isFiller)
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
    as->Asm("moveq.l #0,d6");

    as->Asm("lea     $dff000,a6 ; Hardware registers");

    m_dispatcher->LoadAddress(m_node->m_params[0], "a0"); // src
    m_dispatcher->LoadAddress(m_node->m_params[1], "a1"); // dst
    LoadVariable(as,"move.w",m_node->m_params[2], "d6"); // Offset
    LoadVariable(as,"move.w",m_node->m_params[3], "d1"); // dst x
    LoadVariable(as,"move.w",m_node->m_params[4], "d2"); // dst y
    LoadVariable(as,"move.w",m_node->m_params[5], "d3"); // add
    LoadVariable(as,"move.w",m_node->m_params[6], "d4");
    LoadVariable(as,"move.w",m_node->m_params[7], "BLTAMOD(a6)"); // mod SRC
    LoadVariable(as,"move.w",m_node->m_params[9], "BLTBMOD(a6)"); // mod SRC
    LoadVariable(as,"move.w",m_node->m_params[9], "BLTCMOD(a6)"); // mod SRC
    LoadVariable(as,"move.w",m_node->m_params[8], "BLTDMOD(a6)"); // mod SRC

    LoadVariable(as,"move.w",m_node->m_params[10], "d0"); // mod SRC

//    1011
    if (isFiller) {
           // 00001011 $0DFC
//        as->Asm("move.w  #$0DFC,d0 ; Goes in BLTCON0 (scroll first ) minterm");
        as->Asm("move.w  #$0012,BLTCON1(a6) ;    ");
    }
    else {
//        as->Asm("move.w  #$09E0,d0 ; Goes in BLTCON0 (scroll first ) minterm");

        as->Asm("move.w  #0,BLTCON1(a6) ;    issa 0   BLTCON1");
    }


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

void Methods68000::MatMul(Assembler *as)
{
/*    ;move.l mm_res,a0
    ;move.l mm_mat1,a1
    ;move.l mm_mat2,a2
*/
    m_dispatcher->LoadAddress(m_node->m_params[2],"a0");
    m_dispatcher->LoadAddress(m_node->m_params[0],"a1");
    m_dispatcher->LoadAddress(m_node->m_params[1],"a2");
    as->Asm("jsr matmul_call");

}

void Methods68000::SetRotation(Assembler *as, QString mat)
{
    QString a0 = as->m_regMem.Get();
    QString d0 = as->m_regAcc.Get();
    QString d1 = as->m_regAcc.Get();
    QString d2 = as->m_regAcc.Get();
    as->Comment("Set 3x3 matrix multiplication FAST");
    m_dispatcher->LoadAddress(m_node->m_params[0],a0);
    if (mat!="id") {
        as->Asm("");
        m_dispatcher->LoadVariable(m_node->m_params[1]);
        as->Asm("move.l " + as->m_varStack.pop() +","+d0);
        m_dispatcher->LoadVariable(m_node->m_params[2]);
        as->Asm("move.l " + as->m_varStack.pop() +","+d1);
        as->Asm("moveq.l #0,"+d2);
        as->Asm("sub.w "+d1+","+d2);
    }


    if (mat=="x") {
        as->Asm("move.l #128,0("+a0+")");
        as->Asm("move.l "+d0+",16("+a0+")");
        as->Asm("move.l "+d2+",20("+a0+")");
        as->Asm("move.l "+d1+",28("+a0+")");
        as->Asm("move.l "+d0+",32("+a0+")");
    }
    if (mat=="z") {
        as->Asm("move.l #128,32("+a0+")");
        as->Asm("move.l "+d0+",0("+a0+")");
        as->Asm("move.l "+d1+",4("+a0+")");
        as->Asm("move.l "+d2+",12("+a0+")");
        as->Asm("move.l "+d0+",16("+a0+")");
    }
    if (mat=="y") {
        as->Asm("move.l #128,16("+a0+")");
        as->Asm("move.l "+d0+",0("+a0+")");
        as->Asm("move.l "+d2+",8("+a0+")");
        as->Asm("move.l "+d1+",24("+a0+")");
        as->Asm("move.l "+d0+",32("+a0+")");
    }
    if (mat=="id") {
        as->Asm("move.l #128,0("+a0+")");
        as->Asm("move.l #0,4("+a0+")");
        as->Asm("move.l #0,8("+a0+")");
        as->Asm("move.l #0,12("+a0+")");
        as->Asm("move.l #128,16("+a0+")");
        as->Asm("move.l #0,20("+a0+")");
        as->Asm("move.l #0,24("+a0+")");
        as->Asm("move.l #0,28("+a0+")");
        as->Asm("move.l #128,32("+a0+")");
    }

    as->m_regMem.Pop(a0);
    as->m_regAcc.Pop(d0);
    as->m_regAcc.Pop(d1);
    as->m_regAcc.Pop(d2);
}

void Methods68000::MatMulVec(Assembler *as)
{
/*    move.l mm_mat,a0
    move.l mm_vecsIn,a1
    move.l mm_vecsOut,a2

    moveq.l #0,d6
  */
    as->Comment("MatMulVec call");
    m_dispatcher->LoadAddress(m_node->m_params[0],"a0");
    m_dispatcher->LoadAddress(m_node->m_params[1],"a1");
    m_dispatcher->LoadAddress(m_node->m_params[2],"a2");
    as->Asm("moveq.l #0,d6");
    as->Asm("moveq.l #4,d5");
    as->Asm("moveq.l #8,d4");
    m_node->m_params[3]->Accept(m_dispatcher);
//    m_dispatcher->LoadVariable(m_node->m_params[3]);
    as->Asm("move.l "+ as->m_varStack.pop()+",d7");

//    moveq.l #0,d6
    //	move.w index,d6
    as->Asm("jsr call_matmulvec");
}

void Methods68000::MatMulVecNormalZ(Assembler *as, bool isZonly)
{
    as->Comment("MatMulVec normal z only");
    m_dispatcher->LoadAddress(m_node->m_params[0],"a0");
    m_dispatcher->LoadAddress(m_node->m_params[1],"a1");
    m_dispatcher->LoadAddress(m_node->m_params[2],"a2");
    m_node->m_params[3]->Accept(m_dispatcher);
//    m_dispatcher->LoadVariable(m_node->m_params[3]);
    as->Asm("  moveq.l #0,d7");

    as->Asm("move"+m_dispatcher->getEndType(as,m_node->m_params[3])+" "+ as->m_varStack.pop()+",d7");

//    moveq.l #0,d6
    //	move.w index,d6
    if (isZonly)
    as->Asm("jsr matmulvec_normal_z_call");
    else
    as->Asm("jsr matmulvec_normal_call");
}

void Methods68000::ProjectToScreen(Assembler *as)
{
    as->Comment("Project to screen init");
    as->Asm("moveq.l #0,d4");
    m_node->m_params[1]->Accept(m_dispatcher);
    as->Asm("move.w "+as->m_varStack.pop() + ",d4");
    m_dispatcher->LoadAddress(m_node->m_params[0], "a0");


    as->IncludeFile(":resources/code/amiga/simpleproject.s");


    m_node->m_params[2]->Accept(m_dispatcher);
    as->Asm("move.l d5,"+as->m_varStack.pop());
    m_node->m_params[3]->Accept(m_dispatcher);
    as->Asm("move.l d6,"+as->m_varStack.pop());
    m_dispatcher->LoadAddress(m_node->m_params[0], "a0");


//    m_dispatcher->StoreVariable(
/*    move.l ro_obj,a0


    moveq.l #0,d4
    move.w ro_c,d4
*/

}

void Methods68000::EnableInterrupt(Assembler *as)
{
    QString d0 = as->m_regAcc.Get();
    LoadVariable(as,"move",m_node->m_params[0],d0);
    as->Asm("or.w #%1100000000000000,"+d0);
    as->Asm("move.w "+d0+",$dff09a");
    as->m_regAcc.Pop(d0);

//    as->Asm("move.b "+as->m_varStack.pop() + ",d4");
    //    as->Asm("move.b "+m_node->m_params[0]"",d0");
}

void Methods68000::ProjectAllVertices(Assembler *as)
{
    m_dispatcher->LoadVariable(m_node->m_params[3]);
    as->Asm("move.l " + as->m_varStack.pop() +",pa_cxx");
    m_dispatcher->LoadVariable(m_node->m_params[4]);
    as->Asm("move.l " + as->m_varStack.pop() +",pa_cyy");
    m_dispatcher->LoadVariable(m_node->m_params[5]);
    as->Asm("move.l " + as->m_varStack.pop() +",pa_czz");

    m_dispatcher->LoadVariable(m_node->m_params[2]);
    as->Asm("move.l " + as->m_varStack.pop() +",d3");


    m_dispatcher->LoadAddress(m_node->m_params[0], "a0");
    m_dispatcher->LoadAddress(m_node->m_params[1], "a2");
    as->Asm("jsr call_pall");

/*            move.l pa_vert,a2
            move.l pa_obj,a0

            move.l len,d3
  */
}



