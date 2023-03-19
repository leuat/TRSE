#include "codegen_chip8.h"


CodeGenChip8::CodeGenChip8()
{

}
void CodeGenChip8::dispatch(QSharedPointer<NodeProgram> node){

	AbstractCodeGen::dispatch(node);
	QString injected_assembly=" \
	Inject_ptr: dw 0 \n\
	Inject_save_p2: db 0,0,0 \n\
	Inject_LoadPointer: \n\
		LD I, Inject_ptr \n\
		LD V1, [I] \n\
		LD I, Inject_save_p2 \n\
		LD [I], V2 \n\
		LD V2, #10 \n\
		SUBN V2, V0 \n\
		SE VF, 0 \n\
		RET \n\
		LD I, Inject_save_p2 \n\
		LD V2, [I] \n\
		ADD V0, #A0 \n\
		LD I, Inject_load_inst \n\
		LD [I], V1 \n\
	Inject_load_inst: \n\
		LD I, 0 \n\
		RET";
	#if 0
	if (m_system==XO_CHIP){
		injected_assembly+=" \
		LD I, Inject_ptr \n\
		LD V1, [I] \n\
		LD I, XO_CHIP_load_inst \n\
		LD [I], V1 \n\
		dw #f000 \n\
		XO_CHIP_load_inst: \n\
		dw 0 \n\
		RET\n";
	}
	#endif
	as->Asm(injected_assembly);
}
void CodeGenChip8::PrintBop(TokenType::Type type, QString x0, QString x1)
{
	if (type==TokenType::Type::PLUS)
		as->Asm("add "+x0+","+x1);
	else if (type==TokenType::Type::MINUS)
		as->Asm("sub "+x0+","+x1);
/*    if (type==TokenType::Type::MUL)
		Binop("mul",x0,x1,value);
	if (type==TokenType::Type::DIV)
		Binop("div",x0,x1,value);*/
	else if (type==TokenType::Type::BITOR)
		as->Asm("or "+x0+","+x1);
	else if (type==TokenType::Type::XOR)
		as->Asm("xor "+x0+","+x1);
	else if (type==TokenType::Type::BITAND)
		as->Asm("and "+x0+","+x1);
	else if (type==TokenType::Type::SHL || type==TokenType::Type::SHR){
		QString op = (type==TokenType::Type::SHR)?"SHR ":"SHL ";
		QString loop_lbl = as->NewLabel("shift_loop");
		QString end_lbl = as->NewLabel("shift_end");
		QString zero_lbl = as->NewLabel("shift_zero");
		QString scratch = getReg();
		as->Asm("LD "+scratch+", 8");
		as->Asm("SUBN "+scratch+", "+x1);
		as->Asm("SE VF, 0");
		as->Asm("JP "+zero_lbl);
		as->Label(loop_lbl);
		as->Asm("SNE "+x1+",0");
		as->Asm("JP "+end_lbl);
		as->Asm("ADD "+x1+",-1");
		as->Asm(op+x0+","+x0);
		as->Asm("JP "+ loop_lbl);
		as->Label(zero_lbl);
		as->Asm("LD "+x0+",0");
		as->Label(end_lbl);
	} 

}

void CodeGenChip8::PrintBop16(TokenType::Type type, QString x0_hi, QString x0_lo, QString x1_hi, QString x1_lo)
{   
	if (type==TokenType::Type::PLUS){
		as->Asm("add "+x0_lo+","+x1_lo);
		as->Asm("add "+x0_hi+", VF");
		as->Asm("add "+x0_hi+","+ x1_hi);
	} else if (type==TokenType::Type::MINUS){
		as->Asm("sub "+x0_lo+","+x1_lo);
		as->Asm("sub "+x0_hi+", VF");
		as->Asm("sub "+x0_hi+","+ x1_hi);
	} else if (type==TokenType::Type::BITOR){
		as->Asm("or "+x0_lo+","+x1_lo);
		as->Asm("or "+x0_hi+","+ x1_hi);
	} else if (type==TokenType::Type::XOR){
		as->Asm("xor "+x0_lo+","+x1_lo);
		as->Asm("xor "+x0_hi+","+ x1_hi);
	} else if (type==TokenType::Type::BITAND){
		as->Asm("and "+x0_lo+","+x1_lo);
		as->Asm("and "+x0_hi+","+ x1_hi);
	} else if (type==TokenType::Type::SHL || type==TokenType::Type::SHR){
		QString loop_lbl = as->NewLabel("shift_loop");
		QString end_lbl = as->NewLabel("shift_end");
		QString zero_lbl = as->NewLabel("shift_zero");
		as->Asm("SE "+x1_hi+", 0");
		as->Asm("JP "+zero_lbl);
		QString scratch = getReg();
		as->Asm("LD "+scratch+", 8");
		as->Asm("SUBN "+scratch+", "+x1_lo);
		as->Asm("SE VF, 0");
		as->Asm("JP "+zero_lbl);
		as->Label(loop_lbl);
		as->Asm("SNE "+x1_lo+",0");
		as->Asm("JP "+end_lbl);
		as->Asm("ADD "+x1_lo+",-1");
		if (type==TokenType::Type::SHL){
			as->Asm("SHL "+x0_hi+","+x0_hi);
			as->Asm("SHL "+x0_lo+","+x0_lo);
			as->Asm("ADD "+x0_hi+",VF");
		} else {
			as->Asm("SHR "+x0_lo+","+x0_lo);
			as->Asm("SHR "+x0_hi+","+x0_hi);
			as->Asm("SE VF, 0");
			as->Asm("ADD "+x0_lo+",#80");

		}
		as->Asm("JP "+ loop_lbl);
		as->Label(zero_lbl);
		as->Asm("LD "+x0_lo+",0");
		as->Asm("LD "+x0_hi+",0");
		as->Label(end_lbl);
	}

}

void CodeGenChip8::dispatch(QSharedPointer<NodeBinOP> node)
{

	as->Comment("Binary operation of type: "+TokenType::getType(node->m_op.m_type));
	auto right_imm = qSharedPointerDynamicCast<NodeNumber>(node->m_right);
	auto left_imm = qSharedPointerDynamicCast<NodeNumber>(node->m_left);
	if (node->isWord(as)) {
		auto workaround = node->getLoadType();
		node->setLoadType(TokenType::NADA);

		as->Comment("accept right");
		node->m_right->Accept(this);
		QString c = getReg(); PushReg();
		QString d = getReg(); PushReg();
		
		if (!node->m_right->isWord(as)){
			as->Asm("LD "+d+",V0");
			as->Asm("LD "+c+",0");
		} else {
			as->Asm("LD "+d+",V1");
			as->Asm("LD "+c+",V0");

		}

		as->Comment("accept left");
		node->m_left->Accept(this);
		if (!node->m_left->isWord(as)){
			castToWord("V0", "V1");
		}

		PrintBop16(node->m_op.m_type,"V0","V1",c,d);
		PopReg(); PopReg();
		node->setLoadType(workaround);

	} else if (node->m_op.m_type == TokenType::Type::PLUS || node->m_op.m_type == TokenType::Type::MINUS){
		QString negate = node->m_op.m_type==TokenType::Type::MINUS?"-":"";
		if (right_imm!=nullptr) {
			node->m_left->Accept(this);
			as->Asm("ADD V0, "+negate+right_imm->StringValue());
		} else if (left_imm!=nullptr) {
			node->m_right->Accept(this);
			as->Asm("ADD V0, "+negate+left_imm->StringValue());

		} else goto register_register_binop;

	} else {
	register_register_binop:
		node->m_right->Accept(this);
		QString b = getReg(); PushReg();
		as->Asm("LD "+b+",V0");
		node->m_left->Accept(this);
		PrintBop(node->m_op.m_type,"V0",b);
		PopReg();

	}
}

void CodeGenChip8::dispatch(QSharedPointer<NodeNumber>node)
{
	if (!node->isWord(as))
		as->Asm("ld V0, " + node->getValue(as));
	else
	{
		QString imm = node->getValue(as);
		as->Asm("ld V0, " + imm+">>8" );
		as->Asm("ld V1, " + imm+"&#ff" );

	}
}

void CodeGenChip8::dispatch(QSharedPointer<NodeVar> node)
{
	if (m_inlineParameters.contains(node->value)) {
		m_inlineParameters[node->value]->Accept(this);
		return;
	}
	if (node->isReference()) {
		as->Comment("Load Reference");
		as->Asm("LD v0, "+node->getValue(as)+">>8");
		as->Asm("LD v1, "+node->getValue(as)+"&#ff");
		return;

	} else if (node->m_expr!=nullptr) {
		if (node->isPointer(as)) {
			as->Comment("Load Pointer");
			//I could make it support words, but that's for later
			QString index;
			const auto node_m_expr_number = qSharedPointerDynamicCast<NodeNumber>(node->m_expr);
			const bool has_index=node->hasArrayIndex() || (node_m_expr_number != nullptr && node_m_expr_number->m_val != 0) ;
			if (has_index) {
				node->m_expr->setLoadType(TokenType::BYTE); 
				node->m_expr->Accept(this);
				index=getReg();
				as->Asm("LD "+index+", V0");
			}
			as->Asm("ld I,"+node->getValue(as));
			as->Asm("LD V1, [I]");
			as->Asm("LD I, Inject_ptr");
			as->Asm("LD [I], V1");
			as->Asm("call Inject_loadPointer");
			if (has_index) as->Asm("ADD I, "+ index);

			if (node->getArrayType(as)==TokenType::INTEGER) {
				if (has_index) as->Asm("ADD I, " + index);
				as->Asm("LD V1, [I]");
			} else {
				as->Asm("LD V0, [I]");
			}


			return;

		} else {
			as->Comment("Load Array");
			QString index;
			const auto node_m_expr_number = qSharedPointerDynamicCast<NodeNumber>(node->m_expr);
			const bool has_index=node->hasArrayIndex() || (node_m_expr_number != nullptr && node_m_expr_number->m_val != 0) ;
			if (has_index) {
				node->m_expr->setLoadType(TokenType::BYTE);
				node->m_expr->Accept(this);
				index = getReg();
				as->Asm("LD "+index+", V0");

			} 
			as->Asm("LD I, "+node->getValue(as));
			if (has_index) as->Asm("ADD I, "+index);
			if (node->getArrayType(as)==TokenType::INTEGER) {
				if (has_index) as->Asm("ADD I, "+index);
				as->Asm("LD V1, [I]");
			} else {
				as->Asm("LD V0, [I]");

			}
			return;
		}
	} else if (node->isWord(as)) {
		as->Comment("Load Word");
		as->Asm("ld I,"+node->getValue(as));
		as->Asm("LD V1, [I]");
		
	}
	else {
		as->Comment("Load Byte");
		as->Asm("ld I,"+node->getValue(as));
		as->Asm("ld V0,[I]");
		

	}
	



}


void CodeGenChip8::dispatch(QSharedPointer<NodeString> node)
{

}



void CodeGenChip8::dispatch(QSharedPointer<NodeVarType> node)
{

}

void CodeGenChip8::dispatch(QSharedPointer<NodeBinaryClause> node)
{

}


void CodeGenChip8::dispatch(QSharedPointer<Node> node)
{

}

void CodeGenChip8::dispatch(QSharedPointer<NodeRepeatUntil> node)
{
	ErrorHandler::e.Error("Repeat-until not implemented yet", node->m_op.m_lineNumber);

}
void CodeGenChip8::dispatch(QSharedPointer<NodeComment> node)
{

}

void CodeGenChip8::StoreVariable(QSharedPointer<NodeVar> n)
{

}

bool CodeGenChip8::StoreVariableSimplified(QSharedPointer<NodeAssign> node)
{
	if (node->m_right->isPure() && !node->m_left->isPointer(as) && !node->m_left->hasArrayIndex()) {
		as->Comment("Store variable simplified");
		node->m_right->Accept(this);
		if (node->m_left->isWord(as) && node->m_right->getOrgType(as)==TokenType::BYTE)
			castToWord("V0","V1");
		else if ((!node->m_left->isWord(as)) && node->m_right->getOrgType(as)==TokenType::INTEGER)
			castToByte("V0","V1");
		str(node->m_left);

		return true;
	}
	return false;

}

void CodeGenChip8::LoadVariable(QSharedPointer<NodeVar> n)
{
	n->Accept(this);
}

QString CodeGenChip8::LoadAddress(QSharedPointer<Node> n)
{
	QString reg = PushReg();
	as->Asm("ld "+reg+","+n->getValue(as));
	return reg;
}

QString CodeGenChip8::LoadAddress(QSharedPointer<Node> n, QString reg)
{
	return "";
}

void CodeGenChip8::LoadVariable(QSharedPointer<NodeProcedure> node)
{
	ErrorHandler::e.Error("Procedure address not implemented yet! Please bug the developer", node->m_op.m_lineNumber);
}

void CodeGenChip8::LoadPointer(QSharedPointer<Node> n)
{
}

void CodeGenChip8::LoadVariable(QSharedPointer<Node> n)
{
	n->Accept(this);

}

void CodeGenChip8::LoadVariable(QSharedPointer<NodeNumber>n)
{

}





QString CodeGenChip8::getReg(int dd) {
	if (m_lvl+dd>=m_regs.count()) {
		qDebug() << "ERROR CodeGenChip8 reg stack error shouldn't happen!";
		exit(1);
	}
	return m_regs[m_lvl+dd];

}



void CodeGenChip8::str(QSharedPointer<Node> var)
{
	as->Comment("Str");
	if (var->isWord(as)) {
		as->Asm("LD I, "+var->getValue(as));
		as->Asm("LD [I], V1");

	}
	else {
		as->Asm("ld I,"+var->getValue(as));
		
		as->Asm("ld [I], V0");

	}
}

QString CodeGenChip8::PushReg() {
	if (m_lvl==12)
		ErrorHandler::e.Error("Error in Chip8 dispatcher PopReg : trying to push regstack from max");

	m_lvl++;
	return m_regs[m_lvl];
}

void CodeGenChip8::PopReg() {
	if (m_lvl==0)
		ErrorHandler::e.Error("Error in Chip8 dispatcher PopReg : trying to pop regstack from zero");
	m_lvl--;
}






void CodeGenChip8::AssignString(QSharedPointer<NodeAssign> node) {
	//TODO: Convert to Chip8

	QSharedPointer<NodeString> right = qSharedPointerDynamicCast<NodeString>(node->m_right);
	QSharedPointer<NodeVar> left = qSharedPointerDynamicCast<NodeVar>(node->m_left);
	QString str = as->NewLabel("stringassignstr");
	QString lblCpy=as->NewLabel("stringassigncpy");
	bool isPointer = left->isPointer(as);
	QString strAssign = str + "\t db \"" + right->m_op.m_value + "\",0";
	as->m_tempVars<<strAssign;
	if (isPointer) {
		as->Asm("mw si, "+str+"");
		as->Asm("mw ["+left->getValue(as)+"+2], ds");
		as->Asm("mw ["+left->getValue(as)+"], si");

	}
	else {
		as->Comment("String copy!");
		if (left->isPointer(as))
			as->Asm("les di,["+left->getValue(as)+"]");
		else
			as->Asm("mw di,"+left->getValue(as)+"");
		as->Term();

		as->Asm("push ds");
		as->Asm("pop es");
		as->Asm("mw si,"+str);
		as->Asm("mw cx, "+Util::numToHex(right->m_op.m_value.length()+2));
		as->Asm("rep movsb");
	}
	as->PopLabel("stringassignstr");
	as->PopLabel("stringassigncpy");
}

bool CodeGenChip8::AssignPointer(QSharedPointer<NodeAssign> node)
{
	auto var = qSharedPointerDynamicCast<NodeVar>(node->m_left);
	if (var != nullptr){
		if (var->isPointer(as)){
			if (node->m_right->isReference() || node->m_right->isPointer(as)) {
				as->Comment("Store Reference/Pointer");

				node->m_right->Accept(this);
				as->Asm("LD I, "+var->getValue(as));
				as->Asm("LD [I], V1");
				return true;

			} else  {
				// Storing p[i] := something;
				as->Comment("storing to pointer");
				as->Comment("loading expression:");
				node->m_right->Accept(this);
				QString rvalue_save_hi = getReg(); PushReg();
				as->Asm("LD "+rvalue_save_hi+", V0");
				QString rvalue_save_lo;
				if (var->m_expr->getArrayType(as)==TokenType::INTEGER){
					rvalue_save_lo = getReg(); PushReg();
					as->Asm("LD "+rvalue_save_lo+", V1");
				}
				const auto var_m_expr_num = qSharedPointerDynamicCast<NodeNumber>(var->m_expr);
				const bool has_index = var->hasArrayIndex() || (var_m_expr_num != nullptr && var_m_expr_num->m_val != 0);
				QString index;
				if (has_index) {
					var->m_expr->setLoadType(TokenType::BYTE);
					var->m_expr->Accept(this);
					index=getReg(); PushReg();
					as->Asm("LD "+index+",V0");
				}

				as->Comment("load value:");
				as->Asm("LD I, "+var->getValue(as));
				as->Asm("LD V1, [I]");
				as->Asm("LD I, Inject_ptr");
				as->Asm("LD [I], V1");
				as->Asm("call Inject_loadPointer");
				if (has_index) {
					as->Asm("add I,"+index); 
					if (var->m_expr->getArrayType(as)==TokenType::INTEGER) {
						as->Comment("store word with index  to pointer");
						as->Asm("add I,"+index); 
						as->Asm("LD V0,"+rvalue_save_hi);
						as->Asm("LD V1,"+rvalue_save_lo);
						as->Asm("LD [I], V1");
						PopReg();
					}
					else{
						as->Comment("store byte with index  to pointer");
						as->Asm("LD V0,"+rvalue_save_hi);
						as->Asm("LD [I], V0");
					}
					PopReg();PopReg();
				} else {
					if (var->isWord(as)) {
						as->Comment("store word to pointer");
						as->Asm("LD V0,"+rvalue_save_hi);
						as->Asm("LD V1,"+rvalue_save_lo);
						as->Asm("LD [I], V1");
					} else {
						as->Comment("store byte to pointer");
						as->Asm("LD V0,"+rvalue_save_hi);
						as->Asm("LD [I], V0");

					}
					PopReg();PopReg();

				}
				return true;
			} 
		} else {
			as->Comment("storing to non pointer");
			return false;

		}
	}
	return false;
}

void CodeGenChip8::GenericAssign(QSharedPointer<NodeAssign> node)
{
	as->Comment("Generic assign");
	node->m_right->Accept(this);
	str(node->m_left);

}

bool CodeGenChip8::IsAssignPointerWithIndex(QSharedPointer<NodeAssign> node)
{
	// Set pointer value
	return false;
}

bool CodeGenChip8::IsAssignArrayWithIndex(QSharedPointer<NodeAssign> node)
{
	return false;
}

bool CodeGenChip8::IsSimpleIncDec(QSharedPointer<NodeAssign> node)
{
	return false;
}

bool CodeGenChip8::IsSimpleAssignPointer(QSharedPointer<NodeAssign> node)
{
	auto var = node->m_left;
	if (var->isPointer(as) && !var->hasArrayIndex()) {

		node->m_right->VerifyReferences(as);
		if (!node->m_right->isReference())
			if (!node->m_right->isPointer(as))
				if (node->m_right->isByte(as)) {
					ErrorHandler::e.Error("Trying to assign a non-pointer / non-reference / non-long to pointer '"+var->getValue(as)+"'",var->m_op.m_lineNumber);
				}

		as->Comment("Assigning pointer");

		node->m_right->Accept(this);
		str(node->m_left);
		return true;

	}
	return false;

}

void CodeGenChip8::OptimizeBinaryClause(QSharedPointer<Node> node)
{

}

void CodeGenChip8::AssignFromRegister(QSharedPointer<NodeAssign> node)
{
	ErrorHandler::e.Error("Cannot (yet) assign variables from registers on the Chip8",node->m_op.m_lineNumber);
}

void CodeGenChip8::AssignToRegister(QSharedPointer<NodeAssign> node)
{
	QString vname = getValue(node->m_left);
	vname = vname.toLower();
	if (!node->m_right->isPure())
		ErrorHandler::e.Error("When assigning registers, RHS needs to be pure numeric or variable",node->m_op.m_lineNumber);

	QString reg = vname.remove(0,1);
//        as->Comment("Assigning register : " + vname);

	as->Asm("ld "+reg+", "+getChip8Value(node->m_right));
	return;

}


void CodeGenChip8::DeclarePointer(QSharedPointer<NodeVarDecl> node)
{
	QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(node->m_varNode);
	QSharedPointer<NodeVarType> t = qSharedPointerDynamicCast<NodeVarType>(node->m_typeNode);
	as->Write(v->getValue(as)+ ": \n\tdw  0",0);

	as->m_symTab->Lookup(v->getValue(as), node->m_op.m_lineNumber)->m_arrayType=t->m_arrayVarType.m_type;

}




void CodeGenChip8::BuildConditional(QSharedPointer<Node> node,  QString lblSuccess, QString lblFailed, bool page)
{

	as->Comment("Binary clause Simplified: " + node->m_op.getType());



	auto left_imm = qSharedPointerDynamicCast<NodeNumber>(node->m_left);
	auto right_imm = qSharedPointerDynamicCast<NodeNumber>(node->m_right);
	if (node->isWord(as)){

		if (node->m_op.m_type==TokenType::EQUALS){

			if (left_imm && right_imm){
				if (left_imm->m_val != right_imm->m_val)
					as->Asm("jp "+as->jumpLabel(lblFailed));
				return;
				
			} else if (left_imm) { 
				node->m_right->Accept(this);
				as->Asm("sne V0," + left_imm->StringValue()+">>8");
				as->Asm("se V1,"+ left_imm->StringValue()+"&#ff");
			} else if (right_imm) {
				node->m_left->Accept(this);
				as->Asm("sne V0," + right_imm->StringValue()+">>8");
				as->Asm("se V1,"+ right_imm->StringValue()+"&#ff");
			} else {
				QString  bx_hi, bx_lo; 
				bx_hi = getReg(); PushReg();
				bx_lo = getReg(); PushReg();
				node->m_right->Accept(this);
				as->Asm("LD "+bx_hi+",V0");
				as->Asm("LD "+bx_lo+",V1");
				node->m_left->Accept(this);
				as->Asm("sne V0," + bx_hi);
				as->Asm("se V1,"+ bx_lo);
				PopReg(); PopReg();
			}

		} else if (node->m_op.m_type==TokenType::NOTEQUALS) {

			if (left_imm && right_imm){
				if (left_imm->m_val == right_imm->m_val)
					as->Asm("jp "+as->jumpLabel(lblFailed));
				return;
				
			} else if (left_imm) { 
				node->m_right->Accept(this);
				as->Asm("se V0," + left_imm->StringValue()+">>8");
				as->Asm("sne V1,"+ left_imm->StringValue()+"&#ff");
			} else if (right_imm) {
				node->m_left->Accept(this);
				as->Asm("se V0," + right_imm->StringValue()+">>8");
				as->Asm("sne V1,"+ right_imm->StringValue()+"&#ff");
			} else {
				QString bx_hi, bx_lo; 
				bx_hi = getReg(); PushReg();
				bx_lo = getReg();
				node->m_right->Accept(this);
				as->Asm("LD "+bx_hi+",V0");
				as->Asm("LD "+bx_lo+",V1");
				node->m_left->Accept(this);
				as->Asm("se V0," + bx_hi);
				as->Asm("sne V1,"+ bx_lo);
				PopReg(); 
			}
		} else {

			QString bx_hi, bx_lo; 
			node->m_right->Accept(this);
			bx_hi = getReg(); PushReg();
			bx_lo = getReg(); PushReg();
			as->Asm("LD "+bx_hi+",V0");
			as->Asm("LD "+bx_lo+",V1");
			node->m_left->Accept(this);
			if (node->m_op.m_type==TokenType::GREATEREQUAL){
				as->Asm("subn "+bx_hi+",V0");
				as->Asm("sne VF, 0");
				as->Asm("jp "+as->jumpLabel(lblSuccess));
				as->Asm("subn V0,"+bx_hi);
				as->Asm("sne VF, 0");
				as->Asm("jp "+as->jumpLabel(lblFailed));
				as->Asm("subn "+bx_lo+",V1");
				as->Asm("sne VF, 0");

				


			} else if (node->m_op.m_type==TokenType::GREATER){
				as->Asm("subn "+bx_hi+",V0");
				as->Asm("sne VF, 0");
				as->Asm("jp "+as->jumpLabel(lblSuccess));
				as->Asm("subn V0,"+bx_hi);
				as->Asm("sne VF, 0");
				as->Asm("jp "+as->jumpLabel(lblFailed));
				as->Asm("subn V1,"+bx_lo);
				as->Asm("se VF, 0");


			} else if (node->m_op.m_type==TokenType::LESSEQUAL){
				as->Asm("subn "+bx_hi+",V0");
				as->Asm("sne VF, 0");
				as->Asm("jp "+as->jumpLabel(lblFailed));
				as->Asm("subn V0,"+bx_hi);
				as->Asm("sne VF, 0");
				as->Asm("jp "+as->jumpLabel(lblSuccess));
				as->Asm("subn V1,"+bx_lo);
				//(bx > ax) 
				as->Asm("sne VF, 0");


			} else if (node->m_op.m_type==TokenType::LESS){
				as->Asm("subn "+bx_hi+",V0");
				as->Asm("sne VF, 0");
				as->Asm("jp "+as->jumpLabel(lblFailed));
				as->Asm("subn V0,"+bx_hi);
				as->Asm("sne VF, 0");
				as->Asm("jp "+as->jumpLabel(lblSuccess));
				as->Asm("subn "+bx_lo+",V1");
				//!(ax > bx) 
				as->Asm("se VF, 0");
			}
			PopReg();  PopReg();
		}
	} else {
		

		if (node->m_op.m_type==TokenType::EQUALS){

			if (right_imm && left_imm){
				if (left_imm->m_val != right_imm->m_val) 
					as->Asm("jp "+as->jumpLabel(lblFailed));
				return;

			} else if (right_imm){
				node->m_left->Accept(this);
				as->Asm("se V0," + right_imm->StringValue());
			}else if (left_imm){
				node->m_right->Accept(this);
				as->Asm("se V0," + left_imm->StringValue());
			} else {
				node->m_right->Accept(this);
				QString bx= getReg();
				as->Asm("ld "+bx+", V0");
				node->m_left->Accept(this);
				as->Asm("se V0," + bx);
			}   
		
		}else if (node->m_op.m_type==TokenType::NOTEQUALS) {
			if (right_imm && left_imm){
				if (left_imm->m_val == right_imm->m_val) 
					as->Asm("jp "+as->jumpLabel(lblFailed));
				return;
			} else if (right_imm) {
				node->m_left->Accept(this);
				as->Asm("sne V0," + right_imm->StringValue());
			} else if (left_imm) {
				node->m_right->Accept(this);
				as->Asm("sne V0," + left_imm->StringValue());
			} else {
				node->m_right->Accept(this);
				QString bx= getReg();
				as->Asm("ld "+bx+", V0");
				node->m_left->Accept(this);
				as->Asm("sne V0," + bx);
			}
		} else {
			node->m_right->Accept(this);
			QString bx = getReg();
			as->Asm("LD "+bx+", V0");

			node->m_left->Accept(this);
			if (node->m_op.m_type==TokenType::GREATEREQUAL){
				as->Asm("subn "+bx+",V0");
				//(ax >= bx) 
				as->Asm("sne VF, 0");


			} else if (node->m_op.m_type==TokenType::GREATER){
				as->Asm("subn V0,"+bx);
				//!(bx >= ax) 
				as->Asm("se VF, 0");


			} else if (node->m_op.m_type==TokenType::LESSEQUAL){
				as->Asm("subn V0,"+bx);
				//(bx >= ax) 
				as->Asm("sne VF, 0");


			} else if (node->m_op.m_type==TokenType::LESS){
				as->Asm("subn "+bx+",V0");
				//!(ax >= bx) 
				as->Asm("se VF, 0");
			}
		}
	}
	as->Asm("jp "+as->jumpLabel(lblFailed));



}


void CodeGenChip8::CompareAndJumpIfNotEqualAndIncrementCounter(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QSharedPointer<Node> step, QString lblJump, bool isOffPage, bool isInclusive)
{

	QString loopDone = as->NewLabel("loopdone");
	as->Comment("Compare is onpage");

	IncreaseCounter(step,qSharedPointerDynamicCast<NodeVar>(nodeA->m_left));
	Compare(nodeA, nodeB, step, false, loopDone, lblJump, isInclusive);
	as->PopLabel("loopdone");
	as->Label(loopDone+": ;keep");
	return;


#if 0
	if (step!=nullptr)
		ErrorHandler::e.Error("For loops currently don't support step",nodeA->m_op.m_lineNumber);

//    QString x1 = BinopVariableNumber(nodeA->m_left,"1", TokenType::Type::PLUS);

	// variable
	auto n = nodeA->m_left;
	if (n->isWord(as)) {
		ldr(n);
		as->Asm("; 16 bit add not implemented");
		str(n);
	}
	else {
		ldr(n);
		as->Asm("add "+getReg()+",1");
		str(n);
	}


	if (n->isWord(as)) {
 /*       PushReg();
		PushReg();
		nodeB->Accept(this);
		PopReg();
		PopReg();


		as->Asm("eq16 a,b,c,d");*/
		as->Comment("; 16 bit counter not implemented yet");
	}
	else {
		PushReg();
		nodeB->Accept(this);
		QString b = getReg();
		PopReg();
		QString a = getReg();

		as->Asm("se "+a+","+b);

	}
	as->Asm("jp "+lblJump);

#endif
}
void CodeGenChip8::CompareAndJumpIfNotEqual(QSharedPointer<Node> nodeA, QSharedPointer<Node> nodeB, QString lblJump, bool isOffPage)
{
	#if 0
	if (nodeA->isWord(as)) nodeB->setLoadType(TokenType::INTEGER);
	LoadVariable(nodeA);
	QString ax = getReg();
	PushReg();
	LoadVariable(nodeB);
	QString bx = getReg();
	PopReg();
	as->Asm(m_cmp+ax+","+bx);
	as->Asm(m_jne+lblJump);
	#endif 
}

QString CodeGenChip8::getReturn() { return "ret";}

void CodeGenChip8::castToWord(QString hi, QString lo){
	as->Comment("Cast to Word");
	as->Asm("LD "+lo+","+hi);
	as->Asm("LD "+hi+",0");
}
void CodeGenChip8::castToByte(QString hi, QString lo){
	as->Comment("Cast to Byte");
	as->Asm("LD "+hi+","+lo);
}