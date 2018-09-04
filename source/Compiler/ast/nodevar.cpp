#include "nodevar.h"


NodeVar::NodeVar(Token t) {
    m_op = t;
    value = t.m_value;
}

NodeVar::NodeVar(Token t, Node *expr) {
    m_op = t;
    value = t.m_value;
    m_expr = expr;
    //  if (m_op.m_type==TokenType::INTEGER)
    //     m_isWord = true;

}

PVar NodeVar::Execute(SymbolTable *symTab, uint lvl) {
    level = lvl+1;
    Pmm::Data::d.Set(m_op.m_lineNumber, m_op.m_currentLineText);
    ErrorHandler::e.DebugLow("Calling Var Node",level);
    /*if (symTab->Lookup(value)==nullptr)
            ErrorHandler::e.Error("Could not find variable '" +value +"'.");

        if (symTab->Lookup(value)->m_value==nullptr)
            ErrorHandler::e.Error("Variable '" +value +"' not initialized before use.");
*/

    PVar v = *symTab->Lookup(value, m_op.m_lineNumber)->m_value;
    return v;

}

TokenType::Type NodeVar::getType(Assembler *as) {
    if (m_forceType!=TokenType::NADA)
        return m_forceType;
    if (as->m_symTab->Lookup(value, m_op.m_lineNumber)!=nullptr)
        return as->m_symTab->Lookup(value, m_op.m_lineNumber)->getTokenType();
    return m_op.m_type;
}

bool NodeVar::DataEquals(Node *other) {
    NodeVar* var = dynamic_cast<NodeVar*>(other);
    if (var==nullptr)
        return false;
    return var->value==value;
}

bool NodeVar::isWord(Assembler *as) {


    return getType(as)==TokenType::INTEGER;
    return m_op.m_type==TokenType::INTEGER;
    Symbol* s = as->m_symTab->Lookup(value, m_op.m_lineNumber);
    qDebug() << "Is word test: " << s;

    return s->m_type.toLower()=="integer";
}

void NodeVar::LoadPointer(Assembler *as) {
    as->Comment("Load pointer array");
    NodeNumber* number = dynamic_cast<NodeNumber*>(m_expr);
    QString m = as->m_term;

    as->ClearTerm();
    if (!LoadXYVarOrNum(as, m_expr,false))
    {

        as->Asm("pha");
        m_expr->Build(as);
        as->Term();
        as->Asm("tay");
        as->Asm("pla");
    }
    if (m=="")
        m="lda ";
    as->Asm(m+  "(" + value+"),y");

}

void NodeVar::AssignPointer(Assembler *as, QString memoryLocation) {

}

bool NodeVar::LoadXYVarOrNum(Assembler *as, Node *node, bool isx) {
    NodeVar* var = dynamic_cast<NodeVar*>(node);
    NodeNumber* num = dynamic_cast<NodeNumber*>(node);
    QString operand = "ldx ";
    if (!isx) operand="ldy ";
    if (var!=nullptr && var->m_expr == nullptr) {
        as->Asm(operand + var->value);
        return true;
    }
    if (num!=nullptr) {
        as->Asm(operand  + num->StringValue());
        return true;
    }
    return false;

}

void NodeVar::LoadByteArray(Assembler *as) {
    // Optimizer: if expression is number, just return direct
    as->Comment("Load Byte array");
    QString m = as->m_term;

    as->ClearTerm();
    if (!LoadXYVarOrNum(as, m_expr,true))
    {
        //       as->Asm("pha");
        m_expr->Build(as);
        as->Term();
        as->Asm("tax");
        //          as->Asm("pla");
    }
    if (m=="")
        m="lda ";
    as->Asm(m+  value+",x");

}

bool NodeVar::isAddress() {
    return true;
}

void NodeVar::LoadVariable(Assembler *as) {

    /*        if (as->m_symTab->Lookup(value)==nullptr)
            ErrorHandler::e.Error("Could not find variable '" +value +"' for storing.",m_op.m_lineNumber);
*/
    TokenType::Type t = as->m_symTab->Lookup(value, m_op.m_lineNumber)->getTokenType();
    if (t==TokenType::ADDRESS || t==TokenType::STRING || t==TokenType::CSTRING) {
        LoadByteArray(as);
        return;
    }
    if (t==TokenType::POINTER) {
        LoadPointer(as);
        return;
    }

    if (t==TokenType::BYTE) {
        if (m_expr!=nullptr)
            LoadByteArray(as);
        else {
            as->Asm("lda " +value);
        }
        return;
    }
    if (t == TokenType::INTEGER) {
        m_isWord = true;
        as->Comment("Integer assignment in nodevar");
        as->Asm("ldy " +value);
        as->Asm("lda " +value+"+1");
        return;
    }
    ErrorHandler::e.Error(TokenType::getType(t) + " assignment not supported yet for exp: " + value);
    return;
}

void NodeVar::StoreVariable(Assembler *as) {
    //        as->Comment("VarNode StoreVariable");
    as->m_symTab->Lookup(value, m_op.m_lineNumber);
    //          ErrorHandler::e.Error("Could not find variable '" +value +"' for storing.", m_op.m_lineNumber);

    // Is array
    if (m_expr != nullptr) {
        NodeNumber* number = dynamic_cast<NodeNumber*>(m_expr);
        if (number!=nullptr && getType(as)!=TokenType::POINTER) { // IS NUMBER optimize}
            as->Asm("sta " + value + "+"+ QString::number(number->m_val));
            return;
        }
        else {
            //if regular array

            NodeVar* var = dynamic_cast<NodeVar*>(m_expr);
            NodeNumber* num = dynamic_cast<NodeNumber*>(m_expr);

            //                qDebug() << "Var name: " << value;
            QString secondReg="x";
            QString pa = "";
            QString pb= "";
            if (getType(as)==TokenType::POINTER) {
                secondReg="y";
                pa="(";
                pb=")";
            }

            // Optimize for number or pure var
            if (m_expr->getType(as)==TokenType::INTEGER_CONST || var!=nullptr) {
                as->ClearTerm();
                as->Term("ld"+secondReg +" ");
                m_expr->Build(as);
                as->Term();
                as->Asm("sta " +pa + value+ pb + "," + secondReg);

                return;
            }
            // Just regular var optimize
            // Regular expression
            as->Asm("pha");
            as->ClearTerm();
            m_expr->Build(as);
            as->Term();
            as->Asm("ta" + secondReg);
            as->Asm("pla");
            as->Asm("sta " +pa + value+pb+","+ secondReg);

        }
        return;
    }
    else {
        // Not array
        if (as->m_symTab->Lookup(value, m_op.m_lineNumber)->getTokenType() == TokenType::BYTE) {

            as->Asm("sta " + value);
            return;
        }
        if (as->m_symTab->Lookup(value, m_op.m_lineNumber)->getTokenType() == TokenType::ADDRESS) {

            as->Asm("sta " + value);
            return;
        }
        if (as->m_symTab->Lookup(value, m_op.m_lineNumber)->getTokenType() == TokenType::INTEGER) {
            as->Asm("sta " + value);
            as->Asm("sty " + value + "+1");
            return;
        }

    }

}

QString NodeVar::Build(Assembler *as) {
    Node::Build(as);


    QString  val = value;
    Pmm::Data::d.lineNumber = m_op.m_lineNumber;
    Symbol* s = as->m_symTab->Lookup(value, m_op.m_lineNumber);
    //        if (s==nullptr) {
    //          ErrorHandler::e.Error("Could not find variable '" + value +"'.\nDid you mispell?", m_op.m_lineNumber);
    //    }
    if (m_expr!=nullptr) {
        LoadVariable(as);

        //LoadByteArray(as);
    }
    else {
        bool isOK = true;
        if (s->getTokenType()==TokenType::INTEGER)
            isOK = false;
        as->Variable(val, isOK);
    }
    return val;
}

void NodeVar::ExecuteSym(SymbolTable *symTab) {
    QString varName = m_op.m_value;
    Symbol* varSymbol = symTab->Lookup(varName, m_op.m_lineNumber);

}
