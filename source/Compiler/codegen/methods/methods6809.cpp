#include "methods6809.h"

Methods6809::Methods6809()
{

}



bool Methods6809::Command(QString name)
{
    return m_node->m_procName.toLower() == name.toLower();
}



void Methods6809::Assemble(Assembler *as, AbstractCodeGen* dispatcher) {


    m_codeGen = dispatcher;
    /*    if (Command("Writeln")) {
        as->Writeln();

        m_node->m_params[0]->Accept(dispatcher);
        as->EndWriteln();
    }
*/


    if (Command("AddBreakpoint")) {
        as->Label("trse_breakpoint_"+QString::number(as->m_currentBreakpoint++));
    }
    if (Command("disassemble")) {
        as->Label("trse_disassemble_"+QString::number(as->m_currentBreakpoint++));
    }

    if (Command("Nop"))
        Nop(as);
    if (Command("Loop")) {
        QString lbl = as->NewLabel("loop_internal");

        as->Label(lbl);
        as->Asm("jmp "+lbl);
        as->PopLabel("loop_internal");
        return;
    }
    if (Command("mod"))
        Modulo(as);
    if (Command("mod16"))
        Modulo16(as);


    if (Command("poke"))
        Poke(as);



    if (Command("peek"))
        Peek(as);

    if (Command("memcpy"))
        MemCpy(as,false);

    if (Command("memcpyunroll"))
        MemCpyUnroll(as,false);


    //    as->PopCounter(m_node->m_op.m_lineNumber-1);
}



void Methods6809::Modulo(Assembler *as)
{
    as->Comment("Modulo");

/*
    if (m_node->m_params[0]->isWord(as)) {
//        InitDiv16x8()
        //as->m_internalZP[0]
        Node::flags["div16"] = true;
        LoadVar(as,1);
        as->Asm("sta "+as->m_internalZP[0]);
        as->Asm("sty "+as->m_internalZP[0]+"+1");
        LoadVar(as,0);
        as->Asm("sta "+as->m_internalZP[1]);
        if (m_node->m_params[1]->isWord(as)) {
            as->Asm("sty "+as->m_internalZP[1]+"+1");

        }
        else {
            as->Asm("ldy #0 ; force 16-bit");
            as->Asm("sty "+as->m_internalZP[1]+"+1");
        }
        as->Asm("jsr divide16x8");
        as->Asm("lda "+as->m_internalZP[2]);
        as->Asm("ldy "+as->m_internalZP[2]+"+1");
        return;
    }
    */
    LoadVar(as,1);
    as->Term();
    QString val = as->StoreInTempVar("val");

    LoadVar(as,0);
    as->Term();
//    QString mod = as->StoreInTempVar("modulo");
//    as->Asm("sec");
    QString lbl = as->NewLabel("modulo");
    as->Label(lbl);
    as->Asm("suba "+val);
    as->Asm("bcs "+lbl);
    as->Asm("adca "+val);


    as->PopLabel("modulo");

    as->PopTempVar();

}





void Methods6809::Poke(Assembler* as)
{
    // Optimization : if parameter 1 is zero, drop the ldx / tax
    as->Comment("Poke");
    //m_node->m_params[0]->Accept(as);

    auto bop = NodeFactory::CreateBinop(m_node->m_op,TokenType::PLUS,m_node->m_params[0], m_node->m_params[1]);
    if (!m_node->m_params[2]->isPureNumericOrAddress()) {
        m_node->m_params[2]->Accept(m_codeGen);
        as->Term();
        as->Asm("pshs d");
        bop->Accept(m_codeGen);
        as->Asm("puls d");
        as->Asm("sta ,y");

    }
    else {
        bop->Accept(m_codeGen);
        m_node->m_params[2]->Accept(m_codeGen);
        as->Asm("sta ,y");

    }
}

void Methods6809::Peek(Assembler* as)
{
    // Optimization : if parameter 1 is zero, drop the ldx / tax
    as->Comment("Peek");
    //m_node->m_params[0]->Accept(as);

    auto bop = NodeFactory::CreateBinop(m_node->m_op,TokenType::PLUS,m_node->m_params[0], m_node->m_params[1]);
    bop->Accept(m_codeGen);
    as->Asm("lda ,y");


}


void Methods6809::Modulo16(Assembler *as)
{
    as->Comment("Modulo16");


    //        InitDiv16x8()
    //as->m_internalZP[0]
    LoadVar(as,1);
    as->Asm("sta "+as->m_internalZP[0]);
    as->Asm("sty "+as->m_internalZP[0]+"+1");
    LoadVar(as,0);
    as->Asm("sta "+as->m_internalZP[1]);
    if (m_node->m_params[1]->isWord(as)) {
        as->Asm("sty "+as->m_internalZP[1]+"+1");

    }
    else {
        as->Asm("ldy #0 ; force 16-bit");
        as->Asm("sty "+as->m_internalZP[1]+"+1");
    }
    as->Asm(m_codeGen->getCallSubroutine()+" divide16x8");
    as->Asm("lda "+as->m_internalZP[2]);
    as->Asm("ldy "+as->m_internalZP[2]+"+1");
    return;

}

void Methods6809::MemCpy(Assembler* as, bool isFast)
{
    //as->ClearTerm();
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    QSharedPointer<NodeNumber> num = (QSharedPointer<NodeNumber>)qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumericOrAddress()) {
        ErrorHandler::e.Error("First parameter must be variable or number", m_node->m_op.m_lineNumber);
    }

    QString addr = "";
    if (m_node->m_params[0]->isPureNumericOrAddress())
        addr = m_node->m_params[0]->HexValue();

    if (var!=nullptr)
        addr = var->getValue(as);

    //    QSharedPointer<NodeNumber> num2 = (QSharedPointer<NodeNumber>)qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[1]);

    if (!m_node->m_params[1]->isPureNumericOrAddress()) {
        ErrorHandler::e.Error("Second parameter must be pure numeric", m_node->m_op.m_lineNumber);
    }
    int num2 = m_node->m_params[1]->getValueAsInt(as);
    m_node->RequireAddress(m_node->m_params[0], "MemCpy", m_node->m_op.m_lineNumber);
    m_node->RequireAddress(m_node->m_params[2], "MemCpy", m_node->m_op.m_lineNumber);

    QString ap1 = "";
    QString ap2 = "";
    QString bp1 = "";
    QString bp2 = "";

    QString x = "x";
    if (m_node->m_params[2]->getType(as)==TokenType::POINTER) {
        bp1="(";
        bp2=")";
        x="y";
    }

    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        x="y";
        if (!m_node->m_params[1]->isPureNumeric() || m_node->m_params[1]->getValueAsInt(as)!=0 )
            ErrorHandler::e.Error("When the source data is a pointer, you cannot use an offset. Please set the second parameter to '0'. ", m_node->m_op.m_lineNumber);

    }

    if (!isFast)
        as->Comment("memcpy");
    else
        as->Comment("memcpyfast");

    QString lbl = as->NewLabel("memcpy");
    if (!isFast)
        as->Asm("ld"+x+" #0");
    else {
        if (m_node->m_params[3]->isPureNumericOrAddress()) {
            as->Asm("ld"+x+ " #"+ QString::number((unsigned char)(Util::NumberFromStringHex(m_node->m_params[3]->getValue(as).remove("#"))-1)) );
        }
        else {
            as->Term("ld"+x+" ");
            m_node->m_params[3]->Accept(m_codeGen);
            as->Term();
            as->Asm("de"+x);
        }
    }


    as->Label(lbl);
    //LoadVar(as, 0, "x");

    if (m_node->m_params[0]->getType(as)==TokenType::POINTER)
        as->Asm("lda ("+ addr +"),"+x);
    else {
        QString v = Util::numToHex(num2);
        if (v=="$0") v=""; else v="+ "+v;

        as->Asm("lda " +addr + v + ","+x);
    }
    as->ClearTerm();


    as->Term("sta " + bp1);
    m_node->m_params[2]->Accept(m_codeGen);
    as->Term(bp2 + ","+x, true);


    if (isFast) {
        as->Asm("de"+x+"");
        as->Asm("bpl " + lbl);
    }
    else {
        as->Asm("in"+x+"");
        as->Term("cp"+x+" ");
        // BUG HERE INTEGER
        if (m_node->m_params[3]->isPureNumericOrAddress()) {
            as->Term("#"+Util::numToHex(m_node->m_params[3]->getValueAsInt(as)),true);
        }
        else
            m_node->m_params[3]->Accept(m_codeGen);
        as->Term();

        as->Asm("bne " + lbl);

    }

    as->PopLabel("memcpy");

}

void Methods6809::MemCpyUnroll(Assembler* as, bool isReverse)
{
    //as->ClearTerm();
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    QSharedPointer<NodeNumber> num = (QSharedPointer<NodeNumber>)qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumericOrAddress())
    {
        ErrorHandler::e.Error("First parameter must be variable or number", m_node->m_op.m_lineNumber);
    }
    QString addr = "";
    if (m_node->m_params[0]->isPureNumericOrAddress())
        addr = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr = var->getValue(as);

    //    int num2 = (QSharedPointer<NodeNumber>)qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[1]);
    if (!m_node->m_params[1]->isPureNumericOrAddress()) {
        ErrorHandler::e.Error("Second parameter must be pure numeric", m_node->m_op.m_lineNumber);
    }
    if (!m_node->m_params[3]->isPureNumericOrAddress())
        ErrorHandler::e.Error("Third parameter must be pure numeric", m_node->m_op.m_lineNumber);

    int counter = m_node->m_params[3]->getValueAsInt(as);
    QString ap1 = "";
    QString ap2 = "";
    QString bp1 = "";
    QString bp2 = "";

    if (m_node->m_params[2]->getType(as)==TokenType::POINTER) {
        bp1="(";
        bp2=")";
    }

    int start = 0;
    int end = counter;
    int inc = 1;
    if (isReverse) {
        start = counter-1;
        end = 0;
        inc = -1;
        as->Asm("ldy #" +QString::number(start));
    }
    as->Comment("memcpy unrolled");
    for (int i=start;i!=end;i+=inc) {
        if (m_node->m_params[0]->getType(as)==TokenType::POINTER || m_node->m_params[2]->getType(as)==TokenType::POINTER)
        {
            if (isReverse) {
                as->Asm("dey");

            }
            else {
                if (i==0)
                    as->Asm("ldy #" +QString::number(i));
                else
                    as->Asm("iny");
            }
        }

        if (m_node->m_params[0]->getType(as)==TokenType::POINTER)
            as->Asm("lda ("+ addr +"),y");
        else
            //            as->Asm("lda " +addr +" +  #" + m_node->m_params[1]->getValue(as) + ",y");
            as->Asm("lda " +addr +" +  " + m_node->m_params[1]->getValue(as) + " + " +Util::numToHex(i));

        as->ClearTerm();


        if (m_node->m_params[2]->isPureNumericOrAddress() || m_node->m_params[2]->isReference()) {
            as->Asm("sta " +m_node->m_params[2]->getValue(as) +" +  " +Util::numToHex(i));

        }
        else {
            as->Term("sta " + bp1);
            m_node->m_params[2]->Accept(m_codeGen);
            as->Term(bp2 + ",y", true);
        }
    }

}





void Methods6809::LoadAddress(Assembler *as, int paramNo)
{
    QSharedPointer<Node> node = m_node->m_params[paramNo];

    if (node->isPureNumericOrAddress()) {
        as->Asm("lda #" + Util::numToHex(node->getValueAsInt(as)&0xff));
        as->Asm("ldy #" + Util::numToHex((node->getValueAsInt(as)>>8)&0xff));
        return;
    }
    if (node->isPointer(as) && (!node->hasArrayIndex())) {
        as->Asm("lda "+node->getValue(as));
        as->Asm("ldy "+node->getValue(as)+"+1");
        return;
    }
    as->Asm("lda #<"+node->getValue(as));
    as->Asm("ldy #>"+node->getValue(as));
}

void Methods6809::SaveVar(Assembler *as, int paramNo, QString reg, QString extra)
{

//    qDebug() << "SAVEVAR: " <<  TokenType::getType(m_node->m_params[paramNo]->getType(as));

    if (!(m_node->m_params[paramNo]->isVariable() || m_node->m_params[paramNo]->isAddress()))
        ErrorHandler::e.Error("Parameter "+QString::number(paramNo) + " must be a variable or address", m_node->m_op.m_lineNumber);

    QString vName = m_node->m_params[paramNo]->getValue(as);
    QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[paramNo]);
    as->Term();

    if (v==nullptr || v->m_expr==nullptr) {
//        qDebug() << "SAVEVAR extra " << reg << extra;
        if (reg!="")
            vName+=","+reg;
        as->Asm("sta "+vName);
        return;
    }
    QString pa = "";
    QString pb = "";
    QString x = "x";
    if (v->isPointer(as)) {
        pa = "(";
        pb = ")";
        x = "y";
    }

    // Has expression
    if (v->m_expr->isPure()) {
        as->Asm("ld"+x+" "+v->m_expr->getValue(as));
        as->Asm("sta "+pa+vName+pb + ","+x);
        return;
    }
    // Not pure. Must evaluate
    as->Asm("pha");

    v->m_expr->Accept(m_codeGen);
    as->Term();
    as->Asm("ta"+x);
    as->Asm("pla");
    as->Asm("sta "+pa+vName+pb + ","+x);


/*    as->ClearTerm();
    if (extra=="")
        as->Term("sta ");
    else
        as->Term(extra);

    m_node->m_params[paramNo]->Accept(m_codeGen);

    if (reg!="")
        reg = "," + reg;
    as->Term(reg, true);
*/
}


void Methods6809::LoadVar(Assembler *as, int paramNo, QString reg, QString lda)
{

    QSharedPointer<Node> node = m_node->m_params[paramNo];

/*
        // Override with INLINE parameter
//    qDebug() << node->getValue(as) << m_codeGen->m_inlineParameters.keys();
    if (m_codeGen->m_inlineParameters.contains(node->getValue(as)))
        node = m_codeGen->m_inlineParameters[node->getValue(as)];
*/

   m_node->m_params[paramNo]->Accept(m_codeGen);


}

void Methods6809::LoadVar(Assembler *as, int paramNo)
{
    LoadVar(as, paramNo, "");
}

void Methods6809::SaveVar(Assembler *as, int paramNo)
{
    SaveVar(as, paramNo, "");
}
