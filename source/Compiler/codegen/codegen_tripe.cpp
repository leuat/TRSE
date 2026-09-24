
#include "codegen_tripe.h"

CodeGenTRIPE::CodeGenTRIPE()
{

}

/*
 *
 *  NODE BINOP dispatcher
 *
 *  */



void CodeGenTRIPE::HandleGenericBinop(QSharedPointer<Node> node) {


}

void CodeGenTRIPE::Doublette(QString a, QSharedPointer<Node> b, QString cmd)
{
	QString l = a;
	int pop = 0;

	QString r = "";
	if (b->isPure()) {
		r = TripeValue(b);
	}
	else {
		b->Accept(this);
		r = m_curTemp.last();
		pop++;
	}
	for (int i=0;i<pop;i++)
		m_curTemp.pop();

	as->Asm(cmd+tab+ l+tab+r);

}

void CodeGenTRIPE::Doublette(QSharedPointer<Node> a, QString b, QString cmd)
{
	QString l = "";
	int pop = 0;
	if (a->isPure())
		l = TripeValue(a);
	else {
		a->Accept(this);
		l = m_curTemp.last();
		pop++;
	}

	for (int i=0;i<pop;i++)
		m_curTemp.pop();

	as->Asm(cmd+tab+ l+tab+b);

}

void CodeGenTRIPE::Doublette(QSharedPointer<Node> a, QSharedPointer<Node> b, QString cmd)
{
    QString l = "";
	int pop = 0;
    if (a->isPure())
        l = TripeValue(a);
    else {
        a->Accept(this);
		l = m_curTemp.last();
		pop++;
    }

    QString r = "";
    if (b->isPure()) {
        r = TripeValue(b);
    }
    else {
        b->Accept(this);
		r = m_curTemp.last();
		pop++;
    }
	for (int i=0;i<pop;i++)
		m_curTemp.pop();

	as->Asm(cmd+tab+ l+tab+r);

}
/*
void CodeGenTRIPE::Triplette(QSharedPointer<Node> a, QSharedPointer<Node> b, QSharedPointer<Node> c, QString cmd)
{
	QString l = "";
	int pop = 0;
	if (a->isPure())
		l = TripeValue(a);
	else {
		a->Accept(this);
		l = m_curTemp.pop();
	}

	QString r = "";
	if (b->isPure()) {
		r = TripeValue(b);
	}
	else {
		b->Accept(this);
		r = m_curTemp.pop();
	}

	QString rr = "";
	if (c->isPure()) {
		rr = TripeValue(c);
	}
	else {
		c->Accept(this);
		rr = m_curTemp.pop();
	}
	as->Asm(cmd+"\t"+ l+"\t"+r+"\t"+rr);

}
*/
void CodeGenTRIPE::Triplette(QSharedPointer<Node> a, QSharedPointer<Node> b, QString c, QString cmd)
{
	QString l = "";
	int pop=0;
	if (a->isPure())
		l = TripeValue(a);
	else {
		a->Accept(this);
		l = m_curTemp.last();
		pop++;
	}

	QString r = "";
	if (b->isPure()) {
		r = TripeValue(b);
	}
	else {
		b->Accept(this);
		r = m_curTemp.last();
		pop++;
	}
	for (int i=0;i<pop;i++)
		m_curTemp.pop();

	as->Asm(cmd+tab+ l+tab+r+tab+c);

}

void CodeGenTRIPE::Triplette(QString l, QSharedPointer<Node> b, QString c, QString cmd)
{
	QString r = "";
	if (b->isPure()) {
		r = TripeValue(b);
	}
	else {
		b->Accept(this);
		r = m_curTemp.pop();
	}

	as->Asm(cmd+tab+ l+tab+r+tab+c);

}

void CodeGenTRIPE::ReturnValue(QSharedPointer<NodeProcedureDecl> node) {
//	node->Accept(this);

	//as->Asm("mov " + m_curTemp.pop() + tab + getFunctionName(node));
	Doublette(getFunctionName(node), node->m_returnValue,"mov");
}

void CodeGenTRIPE::WriteCall(Assembler *as, QString call, QSharedPointer<NodeProcedure> node) {
	/*if ((node->m_procedure->m_isFunction))
		as->Asm(call + " " +as->jumpLabel(node->m_procedure->m_procName) + " " + getTempName(node->m_procedure->m_procName+"_ret"));
	else*/
	as->Asm("call " +as->jumpLabel(node->m_procedure->m_procName));
	if (node->m_procedure->m_isFunction)
		auto ret = getFunctionName(node->m_procedure);

//	m_curTemp.pop();

}



void CodeGenTRIPE::dispatch(QSharedPointer<NodeBinOP>node)
{

    node->DispatchConstructor(as,this);

	QString v = getTempName(getType(node));
//	as->Comment("BINOP with tmp " +v + " " + QString::number(m_curTemp.count()));
    if (node->m_left->isWord(as) && !node->m_right->isWord(as))
        node->m_right->setLoadType(TokenType::INTEGER);
    as->ClearTerm();
    as->BinOP(node->m_op.m_type,true);
    QString cmd = as->m_term + "\t"+v;
    as->ClearTerm();

//    as->Asm(s+"\t"+ v+"\t"+ l+"\t"+r);
    Doublette( node->m_left, node->m_right,cmd);
   // return v;


}



/*
 *
 *  NODE NUMBER dispatcher
 *
 *  */

void CodeGenTRIPE::dispatch(QSharedPointer<NodeNumber>node)
{
	node->DispatchConstructor(as,this);

	as->Asm("mov "+getTempName(TokenType::BYTE) + " " +TripeValue(node));
}
/*
 *
 *  SMALL NODE dispatcherS
 *
 *  */






void CodeGenTRIPE::dispatch(QSharedPointer<Node> node)
{
    node->DispatchConstructor(as,this);
    node->m_currentLineNumber = node->m_op.m_lineNumber;


}






void CodeGenTRIPE::dispatch(QSharedPointer<NodeVarType> node)
{
    node->DispatchConstructor(as,this);

}

void CodeGenTRIPE::dispatch(QSharedPointer<NodeBinaryClause> node)
{
    node->DispatchConstructor(as,this);

}

void CodeGenTRIPE::dispatch(QSharedPointer<NodeString> node)
{
    node->DispatchConstructor(as,this);
//    exit(1);
    if (node->m_val.length()>=1 && node->m_val[0].length()>=1) {
        as->ClearTerm();

//        CStringItem it  = ((AsmTRIPE*)as)->m_cstr[QString(node->m_val[0][0]) ];
//        as->Asm("lda #"+QString::number(it.m_screenCode));
        as->Term();
        return;
    }
    as->String(node->m_val,true);
}

void CodeGenTRIPE::DeclarePointer(QSharedPointer<NodeVarDecl> node) {

    QSharedPointer<NodeVarType> t = qSharedPointerDynamicCast<NodeVarType>(node->m_typeNode);
    QString initVal = t->initVal;

//    qDebug() << "POINTER INIT VAL " <<initVal;
    if (initVal=="" && Syntax::s.m_currentSystem->useZeroPagePointers()) {
        initVal = as->PushZeroPointer();
        node->m_pushedPointers++;
//        as->Asm(".data "+Syntax::s.m_currentSystem->getTripePointerType()+":"+Util::numToHex0(m_curZP));
 //       m_curZP+=2;

    }

    QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(node->m_varNode);
//    as->Asm(".data uint64: "+initVal);
	QSharedPointer<Symbol> s = as->m_symTab->Lookup(v->value, node->m_op.m_lineNumber);

//	qDebug() << "Tripe DATA SIZE ptr : " << v->value << TokenType::getType(s->m_arrayType)  << node->m_typeNode->getArrayType(as) << node->getArrayDataSize(as) << node->getArrayType(as) << TokenType::getType(t->m_arrayVarType.m_type);
	QString ptr = "ptr8";
	if (t->m_arrayVarType.m_type==TokenType::INTEGER) ptr="ptr16";
	if (t->m_arrayVarType.m_type==TokenType::LONG) ptr="ptr32";
	as->Asm("decl"+tab+v->value + tab+ptr+":0");

}


void CodeGenTRIPE::dispatch(QSharedPointer<NodeVarDecl> node)
{
    node->DispatchConstructor(as,this);


    QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(node->m_varNode);
    QSharedPointer<NodeVarType> t = qSharedPointerDynamicCast<NodeVarType>(node->m_typeNode);

    QSharedPointer<Appendix> old = as->m_currentBlock;

    if (t->m_flags.contains("wram")) {
        as->m_currentBlock = as->m_wram;
    }

    if (t->m_flags.contains("bank")) {
        QString bnk = t->m_flags[t->m_flags.indexOf("bank")+1];//Banks always placed +1
        if (!as->m_banks.contains(bnk)) {
            as->m_banks[bnk] = QSharedPointer<Appendix>(new Appendix());
            if (Syntax::s.m_currentSystem->m_system==AbstractSystem::MEGA65)
                as->m_banks[bnk]->m_pos = "$0000";
            else
                as->m_banks[bnk]->m_pos = "$4000";
            as->m_banks[bnk]->m_isMainBlock = true;
        }
        as->m_currentBlock = as->m_banks[bnk];
    }
    if (v->m_isGlobal) {
        as->m_currentBlock = nullptr;
        return;
    }


    //    qDebug() << "" <<as->m_currentBlock;
    AbstractCodeGen::dispatch(node);
    if (t->m_op.m_type==TokenType::INCSID || t->m_op.m_type==TokenType::INCNSF) {
//        IncSid(node);
        return;
    }
    //  qDebug() << as->m_currentBlock;
    as->m_currentBlock = old;



}







QString CodeGenTRIPE::getIncbin() {
    return ".incbin";
}

void CodeGenTRIPE::PrintCompare(QSharedPointer<Node> node, QString lblSuccess, QString lblFailed)
{

    QString bcs ="bcs ";
    QString bcc ="bcc ";
    if (node->isSigned(as)) {
       as->Comment("Signed compare");
       bcs = "bpl ";
       bcc = "bmi ";
    }

    if (node->m_op.m_type==TokenType::EQUALS)
        as->Asm("bne " + lblFailed);
    if (node->m_op.m_type==TokenType::NOTEQUALS)
        as->Asm("beq " + lblFailed);
    if (node->m_op.m_type==TokenType::GREATEREQUAL) {
        as->Asm(bcc + lblFailed);
    }
    if (node->m_op.m_type==TokenType::GREATER) {
        as->Asm(bcc + lblFailed);
        as->Asm("beq " + lblFailed);
    }
	if (node->m_op.m_type==TokenType::LESSEQUAL ) {
        as->Asm("beq " + lblSuccess);
        as->Asm(bcs + lblFailed);
    }

    if (node->m_op.m_type==TokenType::LESS)
        as->Asm(bcs + lblFailed);

}
void CodeGenTRIPE::BuildToCmp(QSharedPointer<Node> node)
{
	node->DispatchConstructor(as,this);

	//QString v = getTempName("t_"+getIntType(node)+"_");
/*	if (node->m_left->isWord(as) && !node->m_right->isWord(as))
		node->m_right->setLoadType(TokenType::INTEGER);
	*/
	//    as->Asm(s+"\t"+ v+"\t"+ l+"\t"+r);
	Doublette( node->m_left, node->m_right,"cmp");

}

QString CodeGenTRIPE::TripeNumber(QSharedPointer<Node> node)
{
    QString s = Util::numToHex0(node->getValueAsInt(as));
    QString t = getIntType( node);

    return t+":"+s;

}

QString CodeGenTRIPE::TripeValue(QSharedPointer<Node> node)
{
    if (node->isPureNumeric())
        return TripeNumber(node);
	QString s = "";
	if (node->isReference())
		s="#";
    if (node->isPureVariable())
		return s+node->getValue(as);
	return s+node->getValue(as);

}


void CodeGenTRIPE::BuildConditional(QSharedPointer<Node> node, QString lblSuccess, QString lblFailed, bool page)
{

    as->Comment("Binary clause Simplified: " + node->m_op.getType());
    //    as->Asm("pha"); // Push that baby

    BuildToCmp(node);

    PrintCompare(node, lblSuccess,lblFailed);



}

QString CodeGenTRIPE::getReturn() {
    return "return";
}

QString CodeGenTRIPE::getCallSubroutine() {
	return "call";
}

QString CodeGenTRIPE::ProcedureEndWithoutReturn() {
    return "";

}


QString CodeGenTRIPE::getInitProcedure() {
    return "";
}





void CodeGenTRIPE::Compare(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QSharedPointer<Node> step, bool isLarge, QString loopDone, QString loopNotDone, bool inclusive) {

    Doublette(nodeA->m_left,nodeB,"cmp");
 //   BuildToCmp()
//    PrintCompare(nodeA->m_left, lblSuccess,lblFailed);
	if (isLarge)
		as->Asm("beq "+loopDone);
	else
		as->Asm("bne "+loopNotDone);
}




 //       if (node->m_loopCounter!=0)
   //         ErrorHandler::e.Error("Error: Loop with step other than 1,-1 cannot have loopy/loopx flag");
        // Is word




void CodeGenTRIPE::LoadPointer(QSharedPointer<NodeVar> node) {
	if (node->m_expr== nullptr) {
		as->Asm("mov "+getTempName(TokenType::INTEGER)+ " " +TripeValue(node));
		return;
	}
	as->Comment("LoadPointer");
	TokenType::Type type = node->isWord(as)?TokenType::INTEGER:TokenType::BYTE;
	Triplette(node->getValue(as), node->m_expr,getTempName(type),"load");
/*	node->m_expr->Accept(this);

	QString val = m_curTemp.pop();
	QString idx = getTempName("t_uint8_idx");
	m_curTemp.pop();
	as->Asm("mov "+ idx + " " +val);
	as->Asm("load_p "+TripeValue(node) + " " +idx + " " +getTempName("t_uint8_ret"));
*/

}


/*
 *
 *
 * NodeVar
 *
 *
 *
 *
*/

void CodeGenTRIPE::dispatch(QSharedPointer<NodeVar> node)
{
	as->Comment("::dispatch <NodeVar>");
	LoadVariable(node);

}




void CodeGenTRIPE::LoadByteArray(QSharedPointer<NodeVar> node) {
	as->Comment("::LoadByteArray");

	TokenType::Type type = node->isWord(as)?TokenType::INTEGER:TokenType::BYTE;
	Triplette(node->getValue(as), node->m_expr,getTempName(type),"load");
}

void CodeGenTRIPE::LoadVariable(QSharedPointer<Node> node)
{
	as->Comment("::LoadVariable_2");

    QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(node);
    if (v!=nullptr) {
        LoadVariable(v);
        return;
    }

    QSharedPointer<NodeNumber> num = qSharedPointerDynamicCast<NodeNumber>(node);
    if (num!=nullptr) {
        LoadVariable(num);
        return;
    }

    node->Accept(this);
//    ErrorHandler::e.Error("Uknown variable type!", node->m_op.m_lineNumber);

}


void CodeGenTRIPE::LoadVariable(QSharedPointer<NodeVar> node) {

    /*        if (as->m_symTab->Lookup(value)==nullptr)
                ErrorHandler::e.Error("Could not find variable '" +value +"' for storing.",m_op.m_lineNumber);
    */
    TokenType::Type t = as->m_symTab->Lookup(getValue(node), node->m_op.m_lineNumber)->getTokenType();
    QString type = as->m_symTab->Lookup(getValue(node), node->m_op.m_lineNumber)->m_type;
    if (as->m_symTab->m_records.contains(type))
        t = TokenType::ADDRESS; // Working with a CLASS directly (not pointer)

	as->Comment("::LoadVariable");
    if (node->isStackVariable()) {
//        LoadStackVariable(node);
        return;
    }


//    qDebug() << "LoadVariable: "<<pp;
    if (t==TokenType::ADDRESS || t==TokenType::STRING || t==TokenType::CSTRING || t==TokenType::INCBIN) {
        LoadByteArray(node);
        return;
    }
    if (t==TokenType::POINTER) {
        LoadPointer(node);
        return;
    }

    if (t==TokenType::BYTE) {
		if (node->m_expr!=nullptr) {
            LoadByteArray(node);
			return;
		}
    }
    if (t == TokenType::INTEGER) {
        node->m_isWord = true;
		if (node->m_expr!=nullptr) {
            LoadByteArray(node);
			return;
		}
    }

	as->Asm("mov "+getTempName(TokenType::BYTE) + " " + TripeValue(node));


//    ErrorHandler::e.Error(TokenType::getType(t) + " assignment not supported yet for exp: " + getValue(node));
    return;
}


void CodeGenTRIPE::LoadVariable(QSharedPointer<NodeNumber>node)
{
   as->ClearTerm();
//   qDebug() << "OAD NUMBER";
   as->Comment("::LoadVariable<NodeNumber>");
   if (node->isReference()) {
       as->ClearTerm();
       as->Asm("lda "+node->getValue8bit(as,false));
       as->Asm("ldy "+node->getValue8bit(as,true));
        return;
   }
   as->Term("lda ");
   node->Accept(this);
   as->Term();
}

void CodeGenTRIPE::LoadVariable(QSharedPointer<NodeProcedure> node)
{
    as->Asm("lda #<"+node->m_procedure->m_procName);
    //Disable16bit();
    as->Asm("ldy #>"+node->m_procedure->m_procName);
    //Enable16bit();
}


void CodeGenTRIPE::StoreVariable(QSharedPointer<NodeVar> node) {
	as->Comment("VarNode StoreVariable");
	QString val = "nada";
	if (m_curTemp.size()!=0)
		val = m_curTemp.pop();
	if (node->hasArrayIndex()) {
		Triplette(node->getValue(as), node->m_expr,val,"store");
		return;
	}

	as->Asm("mov	 "+TripeValue(node) + " " +val);
    //          ErrorHandler::e.Error("Could not find variable '" +value +"' for storing.", m_op.m_lineNumber);

/*	as->Term();
	if (node->hasArrayIndex())
		node->m_expr->Accept(this);
	as->Term();
*/
//	Doublette(node,m_curTemp.pop(),"store_p");

//	as->Asm("mov " + node->getValue(as) + " "+m_curTemp.pop());

}


void CodeGenTRIPE::AssignString(QSharedPointer<NodeAssign> node) {


	bool isPointer = node->m_left->isPointer(as);
	QSharedPointer<NodeString> right = qSharedPointerDynamicCast<NodeString>(node->m_right);
	QSharedPointer<NodeVar> left = qSharedPointerDynamicCast<NodeVar>(node->m_left);

	QString str = DefineTempString(right);
	//as->Label(str + "\t.dc \"" + right->m_op.m_value + "\",0");
	//  as->Label(lbl);

		   //    qDebug() << "IS POINTER " << isPointer;
	if (isPointer || left->isStringList(as)) {
		Doublette(left,"#"+str,"mov");
//		StoreVariable(left);
	}
}
/*QString CodeGenTRIPE::BinopTemp( QSharedPointer<Node> node)
{

}
*/


bool CodeGenTRIPE::IsSimpleAssignInteger(QSharedPointer<NodeAssign> node) {
    if (!node->m_left->hasArrayIndex()) {

        if (node->m_right->isPure()) {
//			as->Comment(" Is simple assign integer");

			as->Asm("mov "+TripeValue(node->m_left)+" "+TripeValue(node->m_right));
			return true;
        }
		else
			if (qSharedPointerDynamicCast<NodeBinOP>(node->m_right)!=nullptr) {
//            ErrorHandler::e.Error("BinOp not yet supported",node->m_op.m_lineNumber);
            //QString tempVar = BinopTemp(as,node->m_right);
            node->m_right->Accept(this);
			QString tempVar = "ball";
//			as->Comment("FAIL");
			if (m_curTemp.size()!=0) {
				tempVar = m_curTemp.pop();

				as->Asm("mov "+TripeValue(node->m_left)+" "+tempVar);
			}
			return true;

		}
    }

    return false;
}



bool CodeGenTRIPE::AssignPointer(QSharedPointer<NodeAssign> node) {

    auto var = qSharedPointerDynamicCast<NodeVar>(node->m_left);
	if (var->isPointer(as) && var->hasArrayIndex()) {
        if (node->m_right->isPure() && var->m_expr->isPure()) {
			//as->Comment("store optimized");
			as->Asm("store "+TripeValue(var)+" "+TripeValue(var->m_expr) + " " +TripeValue( node->m_right));

        }
        else {
//            ErrorHandler::e.Error("Tripe: non-pure pointer index not yet supported",node->m_op.m_lineNumber);
			// a[expr]:=b;
			//as->Comment("pop, assign pointer ");
			node->m_right->Accept(this);
			auto val = m_curTemp.last();
			Triplette(var->getValue(as),var->m_expr, val,"store");
			m_curTemp.pop();
			/*
			QString expr = TripeValue(var->m_expr);
			if (!var->m_expr->isPure()) {
				as->Comment("here");
				var->m_expr->Accept(this);
				as->Term();
				QString tempVar = m_curTemp.pop();
				//as->Asm("mov "+TripeValue(var->m_expr)+" "+tempVar);
				qDebug() << "TEMPVAR: " <<tempVar;
				expr = tempVar;
			}
			as->Asm("store_p "+TripeValue(var)+" "+expr + " " +TripeValue( node->m_right));
			*/
		}
		return true;
    }
    return false;

}





void CodeGenTRIPE::dispatch(QSharedPointer<NodeRepeatUntil> node)
{


}

void CodeGenTRIPE::dispatch(QSharedPointer<NodeComment> node)
{
    node->DispatchConstructor(as,this);


}

QString CodeGenTRIPE::resolveTemporaryClassPointer(QString name, int mul, int& res)
{
    return "";
}

TokenType::Type CodeGenTRIPE::getType( QSharedPointer<Node> node) {
    if (node->isWord(as))
		return TokenType::INTEGER;
    if (node->isLong(as))
		return TokenType::LONG;
	return TokenType::BYTE;
}

QString CodeGenTRIPE::getIntType( QSharedPointer<Node> node) {
	if (node->isWord(as))
		return "uint16";
	if (node->isLong(as))
		return "uint32";
	return "uint8";
}

QString CodeGenTRIPE::getIntType(QString type)
{
	type = type.toLower();
	QString t = "uint8";
	if (type=="integer")
		t = "uint16";
	else
	if (type=="long")
		t = "uint32";
	return t;

}

QString CodeGenTRIPE::getTempName(TokenType::Type type)
{
    int i=1;
	QString t = "8";
	QString stype = "uint8";
	if (type==TokenType::INTEGER) {
		t = "16";
		stype = "uint16";
	}
	t+="_";
	QString s = "_r"+t+QString::number(i);
    while (m_curTemp.contains(s))
		s = "_r"+t+QString::number(++i);

    m_curTemp.push(s);
	as->Comment("push");

	QString name = "decl"+tab+s+tab+stype+":0";
    if (!as->m_tempVars.contains(name))
       as->m_tempVars.append(name);

    return s;

}

QString CodeGenTRIPE::getFunctionName(QSharedPointer<NodeProcedureDecl> node)
{
	auto s = node->m_procName+ "_fret";
	m_curTemp.push(s);
	QString name = "decl"+tab+s+tab+getIntType(node->m_returnType->getValue(as))+":0";
	if (!as->m_tempVars.contains(name))
		as->m_tempVars.append(name);
	return s;

}




void CodeGenTRIPE::AssignFromRegister(QSharedPointer<NodeAssign> node)
{
}

void CodeGenTRIPE::AssignToRegister(QSharedPointer<NodeAssign> node)
{
}


void CodeGenTRIPE::OptimizeBinaryClause(QSharedPointer<Node> node)
{
}

void CodeGenTRIPE::dispatch(QSharedPointer<NodeUnaryOp> node)
{

}

void CodeGenTRIPE::dispatch(QSharedPointer<NodeAsm> node) {
	node->DispatchConstructor(as, this);

	as->Asm(".asm");
	QStringList txt;
	if (m_isCurrentlyWithinInline && node->m_outAsm != "") {
		txt = node->m_outAsm.split("\n");
	} else
		txt = node->m_asm.split("\n");
	//    as->Comment("****** Inline assembler section");
	for (QString t : txt) {
		as->Write(t, 0);
	}
	as->Asm(".endasm");
	as->Term();
}



void CodeGenTRIPE::CompareAndJumpIfNotEqualAndIncrementCounter(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QSharedPointer<Node> step, QString lblJump, bool isOffPage, bool isInclusive)
{
    /*    if (!isOffPage)
        SmallLoop(node,qSharedPointerDynamicCast<NodeVar>(nVar->m_left), inclusive);
    else
        LargeLoop(node,qSharedPointerDynamicCast<NodeVar>(nVar->m_left), inclusive);

*/

    if (!isOffPage) {
        QString loopDone = as->NewLabel("loopdone");
//        as->Comment("Compare is onpage");

        IncreaseCounter(step,qSharedPointerDynamicCast<NodeVar>(nodeA->m_left));
        Compare(nodeA, nodeB, step, false, loopDone, lblJump, isInclusive);
        as->PopLabel("loopdone");
        as->Label(loopDone+": ;keep");
        return;
    }


	QString loopDone = as->NewLabel("loopdone");
	QString loopNotDone = as->NewLabel("loopnotdone");

	IncreaseCounter(step,qSharedPointerDynamicCast<NodeVar>(nodeA->m_left));
	Compare(nodeA, nodeB, step, true, loopDone, loopNotDone, isInclusive);
	as->Term();
	as->Label(loopNotDone);
	as->Asm("jump " + lblJump);

	as->Label(loopDone);
	//  Compare(node, var, false, loopDone, nullptr, inclusive);

	as->PopLabel("loopdone");
	as->PopLabel("loopnotdone");



}


void CodeGenTRIPE::CompareAndJumpIfNotEqual(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QString lblJump, bool isOffPage)
{
    Doublette(nodeA,nodeB,"cmp");
    as->Asm("bne " +lblJump);
    return;
}




bool CodeGenTRIPE::StoreVariableSimplified(QSharedPointer<NodeAssign> assignNode)
{
/*	if (assignNode->m_right->isPure() && a)
	as->Comment("Store Variable Simplified");
	assignNode->m_left->Accept(this);
	as->Term();*/
	return false;
}

bool CodeGenTRIPE::StoreStackParameter(QSharedPointer<NodeAssign> n)
{


    return false;

}
