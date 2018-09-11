#include "nodevararray.h"

NodeVarArray::NodeVarArray(Token token, Node *expr):Node() {
    m_var = new NodeVar(token);
    m_expr = expr;
}

PVar NodeVarArray::Execute(SymbolTable *symTab, uint lvl) {
    return PVar();
}

QString NodeVarArray::Build(Assembler *as) {
    return "";

    m_expr->Build(as);
    as->Asm("tax");
    as->Term("lda ");
    m_var->Build(as);
    as->Term(",x",true);
    qDebug() << "end";
    return "";

}

void NodeVarArray::StoreAcc(Assembler *as) {
    as->Asm("tay");
    m_expr->Build(as);
    as->Asm("tax");
    as->Asm("tya");
    as->Term("sta ");
    m_var->Build(as);
    as->Term(",x",true);



}

void NodeVarArray::ExecuteSym(SymbolTable *symTab) {
    if (m_var!=nullptr)
        m_var->ExecuteSym(symTab);
    if (m_expr!=nullptr)
        m_expr->ExecuteSym(symTab);

}
