#include "dispatcherz80.h"


ASTdispatcherZ80::ASTdispatcherZ80()
{
    // Override with Z80 values!
    m_jmp = "jp ";
    m_mov = "ld ";
    m_cmp = "cp ";
    m_jne = "jr nz,";
}


void ASTdispatcherZ80::Handle16bitShift(QSharedPointer<NodeBinOP> node)
{   node->m_left->setForceType(TokenType::NADA);
    node->m_left->Accept(this);
    as->Asm("ld l,a");
    as->Asm("ld h,0");
    if (!node->m_right->isPureNumeric())
        ErrorHandler::e.Error("Only constant 16-bit shifts are supported", node->m_op.m_lineNumber);

    int val = node->m_right->getValueAsInt(as);
    if (node->m_op.m_type == TokenType::SHL) {
        for (int i=0;i<val;i++)
            as->Asm("add hl,hl");
    }
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
        if (node->getType(as)==TokenType::INTEGER) {
            Handle16bitShift(node);
            return;
        }


        node->m_left->Accept(this);
        if (!node->m_right->isPureNumeric())
            ErrorHandler::e.Error("bit shifting on the Z80 must have constant numeric values!", node->m_op.m_lineNumber);
        int num = node->m_right->getValueAsInt(as);
        if (node->m_op.m_type == TokenType::SHR)
            as->Asm("and "+Util::numToHex(0xFF^(int)(pow(2,num)-1)));
        for (int i=0;i<num;i++) {
            if (node->m_op.m_type == TokenType::SHR)
                as->Asm("rrca");
            if (node->m_op.m_type == TokenType::SHL)
                as->Asm("rlca");
        }
        return;
    }

    // Special case if B-node is simple:
    if (node->m_right->isPure() && node->m_left->isPure()) {
        if (node->m_right->isPureVariable())  {
            node->m_right->Accept(this);
            as->Asm("ld b,a");
        }
        else {
            as->Asm("ld b,"+node->m_right->getValue(as));
        }
        node->m_left->Accept(this);
        as->Asm(getBinaryOperation(node)+" a,b");
        return;
    }

    node->m_left->Accept(this);
    as->Asm("push af");
    node->m_right->Accept(this);
    as->Asm("ld b,a");
    as->Asm("pop af");
    as->Asm(getBinaryOperation(node)+" a,b");

//    as->Asm("ld b,a");

/*    as->Comment(";balle binop");
    node->m_left->Accept(this);
    QString bx = getAx(node->m_left);

    PushX();
    node->m_right->Accept(this);

    QString ax = getAx(node->m_right);
    PopX();
    as->BinOP(node->m_op.m_type);

    as->Asm(as->m_term + " " +  bx +"," +ax);
    as->m_term = "";
*/



}



void ASTdispatcherZ80::dispatch(QSharedPointer<NodeBinaryClause> node)
{

}


/*
void ASTdispatcherZ80::dispatch(QSharedPointer<NodeConditional> node)
{

}
*/
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

    if (t->m_flags.contains("sprram"))
        as->m_currentBlock = as->m_sprram;

    if (t->m_flags.contains("bank")) {
        QString bnk = t->m_flags[t->m_flags.indexOf("bank")+1];//Banks always placed +1
        if (!as->m_banks.contains(bnk)) {
            as->m_banks[bnk] = QSharedPointer<Appendix>(new Appendix());
            as->m_banks[bnk]->m_pos = "$4000";
        }
        as->m_currentBlock = as->m_banks[bnk];
    }


//    qDebug() << "BEFORE " <<as->m_currentBlock;
    ASTdispatcherX86::dispatch(node);
  //  qDebug() << as->m_currentBlock;
    as->m_currentBlock = nullptr;
}

void ASTdispatcherZ80::dispatch(QSharedPointer<NodeVar> node)
{
    QString hl =getHL();
    if (node->m_expr!=nullptr) {
/*        ld a, [i]
        ld e,a
        ld d, 0
        ld hl,sine
        add hl,de
        ld a,[hl]


  */
        if (node->m_expr->isPureNumeric() && node->m_expr->getValueAsInt(as)==0) {
            as->Asm("; Optimization : zp[0]");
//            LoadVariable(as,)
//            as->Asm("ld hl,"+node->getValue(as));
            LoadAddress(node);
            as->Asm("ld a,[hl]");
            return;
        }
        node->m_expr->Accept(this);
        as->Asm("ld e,a");
        as->Asm("ld d,0");
  //      as->Asm("ld hl,"+node->getValue(as));
        LoadAddress(node);
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
    if (!node->isWord(as)) {
        if (node->isPureVariable())
            as->Asm("ld a,["+node->getValue(as)+"]");
        else
            as->Asm("ld a,"+node->getValue(as));
        return;
    }
    // Word
    if (node->isPureNumeric())
        as->Asm("ld "+hl+", "+node->getValue(as));
    else
    {
        as->Asm("ld a,["+node->getValue(as)+"]");
        as->Asm("ld "+hl[1]+",a");
        if (node->getOrgType(as)==TokenType::POINTER || node->getOrgType(as)==TokenType::INTEGER) {
            as->Asm("ld a,["+node->getValue(as)+"+1]");
            as->Asm("ld "+hl[0]+",a");
        }
        else {
            as->Asm("xor a,a");
            as->Asm("ld "+hl[0]+",a");
        }

    }
    /*if (as->m_term!="") {
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
*/
}

void ASTdispatcherZ80::dispatch(QSharedPointer<NodeNumber> node)
{
    QString hl = getHL();

    if (!node->isWord(as))
        as->Asm("ld a,"+node->getValue(as));
    else
        as->Asm("ld "+hl+","+node->getValue(as));


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
/*    as->ClearTerm();
    LoadVariable(node->m_b);
    as->Term();
  */
    if (!node->m_b->isPureNumeric()) {
        as->ClearTerm();
        node->m_b->Accept(this);
        as->Term();
        as->Asm("ld c,a");
    }

    as->Asm(m_mov+ax+",["+var+"]");
    as->Asm("add "+ax+",1");
    as->Asm(m_mov+"["+var+"],"+ax);
    if (!node->m_b->isPureNumeric())
        as->Asm(m_cmp+ax+",c");
    else
        as->Asm(m_cmp+ax+"," + node->m_b->getValue(as));

    if (!node->m_forcePage)
        as->Asm(m_jne+lblFor);
    else
        as->Asm("jp nz,"+lblFor);

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
        HandleAssignPointers(node);
        return "";
    }

    if (var->isArrayIndex()) {
        // Is an array

        // Optimization : array[ constant ] := expr
        if (var->m_expr->isPureNumeric()) {
            node->m_right->Accept(this);
            as->Asm("ld [+"+var->getValue(as)+"+"+var->m_expr->getValue(as)+"],a");
            return "";
        }
        // GENERIC expression

        bool rightIsPure = node->m_right->isPure();
        bool isAligned = as->m_symTab->Lookup(var->getValue(as),var->m_op.m_lineNumber)->m_flags.contains("aligned");
        qDebug() << "IS ALIGNED "<< isAligned;

        if (!rightIsPure) {
            node->m_right->Accept(this);
            as->Asm("push af");
        }
        var->m_expr->Accept(this);
        if (isAligned) {
            as->Asm("ld hl," + var->getValue(as));
            as->Asm("add a,l");
            as->Asm("ld l,a");
        }
        else {
            as->Asm("ld e,a");
            as->Asm("ld d,0");
            as->Asm("ld hl," + var->getValue(as));
            as->Asm("add hl,de");
        }
        if (!rightIsPure)
            as->Asm("pop af");
        else
            node->m_right->Accept(this);

        as->Asm("ld [hl],a");



//        ErrorHandler::e.Error("Array indicies not implemented yet");


  /*      node->m_right->Accept(this);
        as->Asm("push a");
        var->m_expr->Accept(this);
        as->Asm("ld d,a");
        if (var->isWord(as))
            as->Asm("shl d,1");
        as->Asm("pop a");
        as->Asm("ld ["+var->getValue(as) + "+di], "+getAx(node->m_left));
        return "";*/
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
            as->Asm("ld b,a");
            as->Asm("ld a,["+var->getValue(as)+"]");
            as->Asm(getBinaryOperation(bop) + " a,b");
            as->Asm("ld ["+var->getValue(as)+"], a");

            return "";
        }
        // Check for a:=a+

    }
    /*    if (node->m_right->isPureVariable()) {
            as->Asm("mov ["+var->getValue(as)+ "],   " +getX86Value(as,node->m_right));
            return "";
        }
    */
    as->Comment("; generic assign ");
    node->m_right->Accept(this);
    as->Comment("; before");

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
    if (bop->m_op.m_type == TokenType::BITAND)
        return "and";
    if (bop->m_op.m_type == TokenType::BITOR)
        return "or";
    if (bop->m_op.m_type == TokenType::XOR)
        return "xor";
/*    if (bop->m_op.m_type == TokenType::DIV)
        return "div";
    if (bop->m_op.m_type == TokenType::MUL)
        return "mul";*/
    ErrorHandler::e.Error("Unknown binary operation : " + bop->m_op.getType(),bop->m_op.m_lineNumber);
//    return " UNKNOWN BINARY OPERATION";
}

void ASTdispatcherZ80::LoadAddress(QSharedPointer<Node> n)
{
    QString hl =getHL();

    if (n->isPointer(as)) {
        as->Asm("ld a,[" +n->getValue(as)+"]");
        as->Asm("ld "+hl[0]+",a");
        as->Asm("ld a,[" +n->getValue(as)+"+1]");
        as->Asm("ld "+hl[1]+",a");
    }
    else as->Asm("ld "+hl+"," +n->getValue(as));
}

void ASTdispatcherZ80::StoreAddress(QSharedPointer<Node> n)
{
    QString hl =getHL();
    as->Asm("ld a,"+hl[0]+"");
    as->Asm("ld ["+n->getValue(as)+"], a");
    as->Asm("ld a,"+hl[1]+"");
    as->Asm("ld ["+n->getValue(as)+"+1], a");

}

void ASTdispatcherZ80::BuildSimple(QSharedPointer<Node> node, QString lblFailed, bool offPage)
{

    as->Comment("Binary clause Simplified: " + node->m_op.getType());
    //    as->Asm("pha"); // Push that baby
    BuildToCmp(node);


    QSharedPointer<NodeConditional> n = qSharedPointerDynamicCast<NodeConditional>(node);
    QString p = "r";
    if (offPage)
        p="p";

    if (node->m_op.m_type==TokenType::EQUALS)
        as->Asm("j"+p+" nz," + lblFailed);
    if (node->m_op.m_type==TokenType::NOTEQUALS)
        as->Asm("j"+p+" z, " + lblFailed);
    if (node->m_op.m_type==TokenType::LESS)
        as->Asm("j"+p+" nc," + lblFailed);
    if (node->m_op.m_type==TokenType::GREATER)
        as->Asm("j"+p+" c, " + lblFailed);



}

void ASTdispatcherZ80::BuildToCmp(QSharedPointer<Node> node)
{
    if (node->m_left->getValue(as)!="") {
        if (node->m_right->isPureNumeric())
        {
            as->Comment("Compare with pure num / var optimization");
            //            TransformVariable(as,"cmp",node->m_left->getValue(as),node->m_right->getValue(as),node->m_left);
            //            TransformVariable(as,"cmp",node->m_left->getValue(as),node->m_right->getValue(as),node->m_left);
            LoadVariable(node->m_left);
            as->Asm("cp a," + node->m_right->getValue(as));

            return;
        } else
        {
            as->Comment("Compare two vars optimization");
            if (node->m_right->isPureVariable()) {
                //QString wtf = as->m_regAcc.Get();
                LoadVariable(node->m_right);
                as->Asm("ld b,a");
                LoadVariable(node->m_left);
                //TransformVariable(as,"move",wtf,qSharedPointerDynamicCast<NodeVar>node->m_left);
                //TransformVariable(as,"cmp",wtf,as->m_varStack.pop());
                as->Asm("cp  a,b");

                return;
            }
/*
            node->m_right->Accept(this);

            as->Asm("cp a," + getAx(node->m_right));

            //            TransformVariable(as,"cmp",qSharedPointerDynamicCast<NodeVar>node->m_left,as->m_varStack.pop());
            return;*/
        }
    }
    node->m_right->Accept(this);
    as->Asm("ld c, a");
    node->m_left->Accept(this);
    as->Term();

    as->Asm("cp a, c");

    //    TransformVariable(as,"cmp",qSharedPointerDynamicCast<NodeVar>node->m_left, as->m_varStack.pop());

    // Perform a full compare : create a temp variable
    //        QString tmpVar = as->m_regAcc.Get();//as->StoreInTempVar("binary_clause_temp");
    //        node->m_right->Accept(this);
    //      as->Asm("cmp " + tmpVar);
    //      as->PopTempVar();


}

void ASTdispatcherZ80::HandleAssignPointers(QSharedPointer<NodeAssign> node)
{
    //        qDebug() << "IS POINTER "<<node->m_right->isPure();
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(node->m_left);

    if (!var->isArrayIndex()) {

        // P := Address / variable
        if (node->m_right->isPure()) {
            LoadAddress(node->m_right);
            StoreAddress(var);
            return;
        }
        // First test for the simplest case: p := p  + const
        QSharedPointer<NodeBinOP> bop = qSharedPointerDynamicCast<NodeBinOP>(node->m_right);
        // P:= (expr);
        if (bop!=nullptr) {
            // Pointer := Pointer BOP expr
            if (bop->m_left->getValue(as)==var->getValue(as)) {

                /*if (bop->m_right->isPure()) {

                    // Simple p1 := p1 + VAL
                    as->ClearTerm();
                    QString lbl = as->NewLabel("pcont");
                    if (bop->m_right->isPureNumeric()) {
                        int lo = bop->m_right->getValueAsInt(as)&0xFF;
                        if (lo!=0) {
                            as->Asm("ld a,["+var->getValue(as)+"+1]");

                            as->Asm(getPlusMinus(bop->m_op)+"a,"+Util::numToHex(lo));
                            as->Asm("ld ["+var->getValue(as)+"+1],a");
                            as->Asm("jr nc,"+lbl);
                            as->Asm("ld a,["+var->getValue(as)+"]");
                            if (bop->m_op.m_type==TokenType::PLUS)
                                as->Asm("inc a");
                            else
                                as->Asm("dec a");
                            as->PopLabel("pcont");

                        }
                        else {
                            as->Asm("ld a,["+var->getValue(as)+"]");

                        }

                        int hi = (bop->m_right->getValueAsInt(as)>>8)&0xFF;
                        if (hi!=0)
                           as->Asm(getPlusMinus(bop->m_op)+"a,"+Util::numToHex(hi));

                        as->Asm("ld ["+var->getValue(as)+"],a");
                        as->Label(lbl);
                        return;
                    }
                    else {
                        as->Asm("ld a,["+bop->m_right->getValue(as)+"]");
                        as->Asm("ld b,a");
                        as->Asm("ld a,["+var->getValue(as)+"+1]");
                        as->Asm(getPlusMinus(bop->m_op)+"a,b");

                    }
                    as->Asm("ld ["+var->getValue(as)+"+1],a");
                    as->Asm("jr nc,"+lbl);
                    as->Asm("ld a,["+var->getValue(as)+"]");
                    if (bop->m_op.m_type==TokenType::PLUS)
                       as->Asm("inc a");
                    else
                        as->Asm("dec a");
                    as->Asm("ld ["+var->getValue(as)+"],a");
                    as->Label(lbl);
                    as->PopLabel("pcont");
                    return;
                }
                */
                as->Comment(";generic pointer P:=P+(expr) add expression");

                bop->setForceType(TokenType::INTEGER);
                if (bop->m_right->isPure()) {
                    m_useNext="de";
                    bop->m_right->Accept(this);
                }
                else {
                    bop->m_right->Accept(this);

                    as->Asm("ld d,h");
                    as->Asm("ld e,l");
                }
                LoadAddress(var); // Load address into hl
                as->Asm(getPlusMinus(bop->m_op)+" hl,de");
                // Store address
                StoreAddress(var);

               return;
            }
            else ErrorHandler::e.Error("Pointers in GBZ80 TRSE do not support this expression",bop->m_op.m_lineNumber);
        }
        // Generic : Doesn't really work
        node->m_right->setForceType(TokenType::POINTER);
        node->m_right->Accept(this);
        StoreAddress(var);


        // Generic case ))
    }
    else
    {
        // HAS array index, so P[ index ] := ..
        // Much simpler. set value.
        if (!node->m_right->isPure()) {
            node->m_right->Accept(this);
            as->Asm("push af");
        }
        LoadAddress(var);
/*        as->Asm("ld a,[p1]");
        as->Asm("ld h,a");
        as->Asm("ld a,[p1+1]");
        as->Asm("ld l,a");*/
        if (var->m_expr->isPureNumeric() && var->m_expr->getValueAsInt(as)==0) {

        }
        else
        {
            as->Asm("xor a");
            as->Asm("ld d,a");
            var->m_expr->Accept(this);
            as->Asm("ld e,a");
            as->Asm("add hl,de");
        }
        if (!node->m_right->isPure()) {
            as->Asm("pop af");
        }
        else node->m_right->Accept(this);

        as->Asm("ld [hl],a"); // Store in pointer

        return;
    }
    ErrorHandler::e.Error("Pointers must be assigned to variables or addresses",node->m_op.m_lineNumber);

}

QString ASTdispatcherZ80::getPlusMinus(Token t) {
    if (t.m_type == TokenType::PLUS) {
        return "add ";
    }
    if (t.m_type == TokenType::MINUS) {
        return "sub ";
    }
    ErrorHandler::e.Error("Only plus / minus are supported for this type of operation.",t.m_lineNumber);
    return "";
}

