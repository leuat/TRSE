#include "dispatcherz80.h"


ASTdispatcherZ80::ASTdispatcherZ80()
{
    // Override with Z80 values!
    m_jmp = "jp ";
    m_mov = "ld ";
    m_cmp = "cp ";
    m_jne = "jr nz,";
}

void ASTdispatcherZ80::dispatch(QSharedPointer<NodeBinOP>node)
{
    if (node->m_left->isWord(as))
        node->m_right->setForceType(TokenType::INTEGER);
    if (node->m_right->isWord(as))
        node->m_left->setForceType(TokenType::INTEGER);


    if (node->isPureNumeric()) {
        as->Asm("ld "+getAx(node)+", " + node->getValue(as) + "; binop is pure numeric");
        return;
    }
    if (node->isPureVariable()) {
        as->Asm("ld "+getAx(node)+", [" + node->getValue(as)+"] ; binop is pure variable");
        return;
    }
/*    if (!node->m_left->isPure() && node->m_right->isPure()) {
        QSharedPointer<Node> t = node->m_right;
        node->m_right = node->m_left;
        node->m_left = t;
        qDebug() << "SWITCH";
    }
*/
    if (node->m_op.m_type == TokenType::MUL || node->m_op.m_type == TokenType::DIV) {
        node->m_left->Accept(this);
        QString bx = getAx(node->m_left);

        PushX();
        if (node->m_op.m_type == TokenType::DIV) {
//            node->m_right->setForceType(TokenType::BYTE);
//            as->Asm("cwd");
        }
        node->m_right->Accept(this);

        QString ax = getAx(node->m_right);
        PopX();
        as->BinOP(node->m_op.m_type);

        as->Asm(as->m_term + " " +  ax);
        as->m_term = "";
        return;
    }

    if (node->m_op.m_type == TokenType::SHR || node->m_op.m_type == TokenType::SHL) {
        node->m_left->Accept(this);
        if (!node->m_right->isPureNumeric())
            ErrorHandler::e.Error("bit shifting on the Z80 must have constant numeric values!", node->m_op.m_lineNumber);
        int num = node->m_right->getValueAsInt(as);
        if (node->m_op.m_type == TokenType::SHR)
            as->Asm("and "+Util::numToHex(0xFF^(int)(pow(2,num)-1)));
        for (int i=0;i<num;i++) {
            if (node->m_op.m_type == TokenType::SHR)
                as->Asm("rrca");
        }
        return;
    }

    node->m_left->Accept(this);
    QString bx = getAx(node->m_left);

    PushX();
    node->m_right->Accept(this);

    QString ax = getAx(node->m_right);
    PopX();
    as->BinOP(node->m_op.m_type);

    as->Asm(as->m_term + " " +  bx +"," +ax);
    as->m_term = "";



}



void ASTdispatcherZ80::dispatch(QSharedPointer<NodeBinaryClause> node)
{

}



void ASTdispatcherZ80::dispatch(QSharedPointer<NodeConditional> node)
{

}
/*
void ASTdispatcherZ80::dispatch(QSharedPointer<NodeForLoop> node)
{

}
*/
void ASTdispatcherZ80::dispatch(QSharedPointer<NodeVarDecl> node)
{
    QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(node->m_varNode);
    QSharedPointer<NodeVarType> t = qSharedPointerDynamicCast<NodeVarType>(node->m_typeNode);

    if (t->m_flags.contains("hram")) {
        as->m_currentBlock = as->m_hram;
//        qDebug() << "declaring in HRAM";
    }
    if (t->m_flags.contains("wram"))
        as->m_currentBlock = as->m_wram;


//    qDebug() << "BEFORE " <<as->m_currentBlock;
    ASTdispatcherX86::dispatch(node);
  //  qDebug() << as->m_currentBlock;
    as->m_currentBlock = nullptr;
}

void ASTdispatcherZ80::dispatch(QSharedPointer<NodeVar> node)
{
    QString ending = "]";
    if (node->m_expr!=nullptr) {
/*        ld a, [i]
        ld e,a
        ld d, 0
        ld hl,sine
        add hl,de
        ld a,[hl]
  */    node->m_expr->Accept(this);
        as->Asm("ld e,a");
        as->Asm("ld d,0");
        as->Asm("ld hl,"+node->getValue(as));
        as->Asm("add hl,de");
        as->Asm("ld a,[hl]");
        return;



/*        as->Asm("mov ah,0");
        node->m_expr->Accept(this);
        as->Asm("mov di,ax");
        if (node->getArrayType(as)==TokenType::INTEGER)
            as->Asm("shl di,1 ; Accomodate for word");
        ending = "+di]";*/

    }

    if (as->m_term!="") {
        as->m_term +=node->getValue(as);
        return;
    }


    QString ax = getAx(node);

    if (ax[0]!='a') {
        as->Asm("ld a,[" + node->getValue(as)+ending);
        as->Asm("ld "+ax[1]+",a");

    }
    else
        as->Asm("ld "+ax[0]+", [" + node->getValue(as)+ending);
    //    if (node->isArrayIndex())
    //        qDebug() << TokenType::getType(node->getArrayType(as));
    if (node->m_forceType==TokenType::INTEGER) {
        bool accomodate = false;
        if (node->isArrayIndex()) {
            if (node->getArrayType(as)!=TokenType::INTEGER) {
                accomodate = true;
            }
        }
        else
            if (node->getOrgType(as)!=TokenType::INTEGER)
                accomodate = true;

        if (accomodate) {
            if (!(node->getOrgType(as)!=TokenType::INTEGER))
                as->Asm("ld a,[" + node->getValue(as)+"+1"+ending);
            else
                as->Asm("ld a,0");
            as->Asm("ld "+ax[0]+",a");
        }
    }
    //    qDebug() << "ORG " <<TokenType::getType(node->getOrgType(as)) << "   : " << node->getValue(as);
    //  qDebug() << "FT " <<TokenType::getType(node->m_forceType);

}

void ASTdispatcherZ80::dispatch(QSharedPointer<NodeNumber> node)
{
    QString ax = getAx(node);
    if (as->m_term!="") {
        as->m_term +=node->getValue(as);
        return;
    }

    as->Asm("ld "+ax+", " + node->getValue(as));
}

void ASTdispatcherZ80::dispatch(QSharedPointer<NodeForLoop> node)
{
    node->DispatchConstructor(as);


    //QString m_currentVar = ((NodeAssign*)m_a)->m_
    QSharedPointer<NodeAssign> nVar = qSharedPointerDynamicCast<NodeAssign>(node->m_a);


    if (nVar==nullptr)
        ErrorHandler::e.Error("Index must be variable", node->m_op.m_lineNumber);

    QString var = qSharedPointerDynamicCast<NodeVar>(nVar->m_left)->getValue(as);//  m_a->Build(as);
    //    qDebug() << "Starting for";
    node->m_a->Accept(this);
    //  qDebug() << "accepted";

    //    LoadVariable(node->m_a);
    //  TransformVariable()
    //QString to = m_b->Build(as);
    QString to = "";
    if (node->m_b->isPure())
        //    if (qSharedPointerDynamicCast<NodeNumber>(node->m_b) != nullptr)
        to = node->m_b->getValue(as);
    //  if (qSharedPointerDynamicCast<NodeVar>(node->m_b) != nullptr)
    //    to = (qSharedPointerDynamicCast<NodeVar>node->m_b)->getValue(as);

    //    as->m_stack["for"].push(var);
    QString lblFor =as->NewLabel("forloop");
    as->Label(lblFor);
    //    qDebug() << "end for";


    if (nVar->m_left->isWord(as))
        node->m_b->setForceType(TokenType::INTEGER);

    node->m_block->Accept(this);
    QString ax = getA(nVar->m_left);
    as->Asm(m_mov+ax+",["+var+"]");
    as->Asm("add "+ax+",1");
    as->Asm(m_mov+"["+var+"],"+ax);
    PushX();
    as->ClearTerm();
    LoadVariable(node->m_b);
    as->Term();
    as->Asm(m_cmp+ax+","+getA(node->m_b));
    PopX();

    as->Asm(m_jne+lblFor);

    as->PopLabel("forloop");

}



QString ASTdispatcherZ80::AssignVariable(QSharedPointer<NodeAssign> node)
{

    if (node->m_left->isWord(as)) {
        node->m_right->setForceType(TokenType::INTEGER);
    }




    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(node->m_left);
    as->ClearTerm();

    if (qSharedPointerDynamicCast<NodeNumber>(node->m_left)!=nullptr) {
        node->m_right->Accept(this);
        as->Term();
        as->Asm("ld ["+node->m_left->getValue(as)+"],a");
        return "";

    }


    if (var->isPointer(as)) {
        /*       if (node->m_right->isPureVariable()) {
            as->Asm("lea si, ["+node->m_right->getValue(as)+"]");
            as->Asm("mov ["+var->getValue(as)+"], ds");
            as->Asm("mov ["+var->getValue(as)+"+2], si");
            return "";
        }
        else{
            node->m_right->Accept(this);

            as->Asm("mov ["+var->getValue(as)+"], ax");
            as->Asm("mov ["+var->getValue(as)+"+2], bx");
        }
        */
        ErrorHandler::e.Error("Pointers not implemented",node->m_op.m_lineNumber);
        return "";
    }

    if (var->isArrayIndex()) {
        // Is an array
        node->m_right->Accept(this);
        as->Asm("push a");
        var->m_expr->Accept(this);
        as->Asm("ld d,a");
        if (var->isWord(as))
            as->Asm("shl d,1");
        as->Asm("pop a");
        as->Asm("ld ["+var->getValue(as) + "+di], "+getAx(node->m_left));
        return "";
    }

    //    if (var->getValue())
    // Simple a:=b;
    QString type ="byte";
    if (var->isWord(as))
        type = "word";

    if (node->m_right->isPureNumeric()) {
        as->Asm("ld "+getAx(node) + ", "+node->m_right->getValue(as));
        as->Asm("ld ["+var->getValue(as)+ "], "+getAx(node));
        return "";
    }
    // Check for a:=a+2;
    QSharedPointer<NodeBinOP> bop =  qSharedPointerDynamicCast<NodeBinOP>(node->m_right);
    // as->Comment("Testing for : a:=a+ expr " + QString::number(bop!=nullptr));
    // if (bop!=nullptr)
    //  as->Comment(TokenType::getType(bop->getType(as)));
    if (bop!=nullptr && (bop->m_op.m_type==TokenType::PLUS || bop->m_op.m_type==TokenType::MINUS)) {
        //      as->Comment("PREBOP searching for "+var->getValue(as));
        if (bop->ContainsVariable(as,var->getValue(as))) {
            // We are sure that a:=a ....
            // first, check if a:=a + number
            //            as->Comment("In BOP");
            if (bop->m_right->isPureNumeric()) {
                as->Comment("'a:=a + const'  optimization ");
                //as->Asm(getBinaryOperation(bop) + " ["+var->getValue(as)+"], "+type + " "+bop->m_right->getValue(as));
                as->Asm("ld a,["+var->getValue(as)+"]");
                as->Asm(getBinaryOperation(bop) + " " +bop->m_right->getValue(as));
                as->Asm("ld ["+var->getValue(as)+"], a");
                return "";
            }
            as->Comment("'a:=a + expression'  optimization ");
            bop->m_right->Accept(this);
            as->Asm(getBinaryOperation(bop) + " ["+var->getValue(as)+"], "+getAx(var));
            return "";
        }
        // Check for a:=a+

    }
    /*    if (node->m_right->isPureVariable()) {
            as->Asm("mov ["+var->getValue(as)+ "],   " +getX86Value(as,node->m_right));
            return "";
        }
    */

    node->m_right->Accept(this);

    as->Asm("ld ["+qSharedPointerDynamicCast<NodeVar>(node->m_left)->getValue(as) + "], "+getAx(node->m_left));
    return "";
}

QString ASTdispatcherZ80::getAx(QSharedPointer<Node> n) {
    QString a = m_regs[m_lvl];
    if (n->m_forceType==TokenType::INTEGER)
        return "bc";
    if (n->getType(as)==TokenType::INTEGER)
        return "bc";
    if (n->getType(as)==TokenType::ADDRESS)
        return "hl";
    if (n->getType(as)==TokenType::INTEGER_CONST)
        if (n->isWord(as))
            return "bc";
    //        if (n->isPureNumeric())
    //          if (n->getValue()
    return a;

}

QString ASTdispatcherZ80::getA(QSharedPointer<Node> n) {
    QString a = m_regs[m_lvl];
    return a;

}

QString ASTdispatcherZ80::getX86Value(Assembler *as, QSharedPointer<Node> n) {
    if (n->isPureVariable())
        return "["+n->getValue(as)+"]";
    return n->getValue(as);

}

QString ASTdispatcherZ80::getBinaryOperation(QSharedPointer<NodeBinOP> bop) {
    if (bop->m_op.m_type == TokenType::PLUS)
        return "add";
    if (bop->m_op.m_type == TokenType::MINUS)
        return "sub";
    if (bop->m_op.m_type == TokenType::DIV)
        return "div";
    if (bop->m_op.m_type == TokenType::MUL)
        return "mul";
    return " UNKNOWN BINARY OPERATION";
}

