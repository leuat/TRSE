#include "nodenumber.h"


NodeNumber::NodeNumber(Token op, int val) {
    m_op = op;
    m_val = val;


    if (m_op.m_type!=TokenType::ADDRESS)
        m_op.m_type = TokenType::INTEGER_CONST;

}

PVar NodeNumber::Execute(SymbolTable *symTab, uint lvl) {
    Pmm::Data::d.Set(m_op.m_lineNumber, m_op.m_currentLineText);
    level = lvl +1;
    return PVar(m_val);

}

bool NodeNumber::isAddress() {
    return m_op.m_type == TokenType::ADDRESS;
}

bool NodeNumber::isPureNumeric() {
    return true;
}

bool NodeNumber::isWord(Assembler *as) {
    return (m_val>=256 && m_op.m_type == TokenType::INTEGER_CONST);
}

void NodeNumber::LoadVariable(Assembler *as) {
    as->ClearTerm();
    as->Term("lda ");
    Build(as);
    as->Term();
}

QString NodeNumber::HexValue() {
    return "$" + QString::number((int)m_val,16);
}

bool NodeNumber::DataEquals(Node *other) {
    NodeNumber* var = dynamic_cast<NodeNumber*>(other);
    if (var==nullptr)
        return false;
    return var->m_val == m_val;
}

QString NodeNumber::StringValue() {
    QString val;
    if (m_op.m_type==TokenType::BYTE)
        val = "#"+QString::number((int)m_val);
    if (m_op.m_type==TokenType::INTEGER)
        val = "#"+QString::number((int)m_val);
    if (m_op.m_type==TokenType::INTEGER_CONST)
        val = "#"+QString::number((int)m_val);
    if (m_op.m_type==TokenType::ADDRESS) {
        val = "$" + QString::number((int)m_val,16);
    }
    return val;
}

bool NodeNumber::isMinusOne() {
    if (m_val==255)
        return true;

    return false;
}

bool NodeNumber::isOne() { return m_val==1; }

QString NodeNumber::Build(Assembler *as) {
    Node::Build(as);

    QString val = "";

    /*        if (as->m_symTab->m_constants.contains(m_op.m_value)) {
            m_val = as->m_symTab->m_constants[m_op.m_value]->m_value->m_fVal;
        }
*/


    if (m_op.m_type==TokenType::BYTE)
        val = "#"+QString::number((int)m_val);
    if (m_op.m_type==TokenType::INTEGER)
        val = "#"+QString::number((int)m_val);
    if (m_op.m_type==TokenType::INTEGER_CONST)
        val = "#"+QString::number((int)m_val);
    if (m_op.m_type==TokenType::ADDRESS) {

        val = "$" + QString::number((int)m_val,16);
    }

    if (m_op.m_type==TokenType::INTEGER_CONST && m_val>255) {
        as->Comment("Integer constant assigning");
        int hiBit = ((int)m_val)>>8;
        int loBit = ((int)m_val)&0xff;
        as->ClearTerm();
        as->Asm("ldy #" + QString::number(hiBit) );
        //            as->Asm("tax");
        as->Asm("lda #" + QString::number(loBit) );
        return val;

        //qDebug() << m_op.m_value <<":" << m_val << " : " << hiBit << "  , " << loBit;
        //exit(1);
    }

    // qDebug() << m_op.getType() << " for " << val;;

    if (as->m_term=="")

        as->Term("lda " + val);
    else
        as->Term(val);
    return val;
}
