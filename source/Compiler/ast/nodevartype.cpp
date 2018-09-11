#include "nodevartype.h"


NodeVarType::NodeVarType(Token t, QString position, Token arrayVarType, QStringList data):Node() {
    m_op = t;
    value = t.m_value;
    m_arrayVarType = arrayVarType;
    m_data = data;
    m_position = position;

}

NodeVarType::NodeVarType(Token t, QStringList data) : Node() {
    m_op = t;
    value = t.m_value;
    //m_arrayVarType = TokenType::STRING;
    m_data = data;

}

NodeVarType::NodeVarType(Token t, QString filename, QString position) : Node() {
    m_op = t;
    value = t.m_value;
    m_filename = filename;
    m_position = position;


}

NodeVarType::NodeVarType(Token t, QString initvalue) : Node() {
    m_op = t;
    value = t.m_value;
    initVal = initvalue;

}

PVar NodeVarType::Execute(SymbolTable *symTab, uint lvl) {
    Pmm::Data::d.Set(m_op.m_lineNumber, m_op.m_currentLineText);
    level = lvl+1;
    ErrorHandler::e.DebugLow("Calling Vartype",level);
    return PVar();

}
