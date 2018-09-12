#include "nodeblock.h"

void NodeBlock::SetParameter(QString name, PVar var) {
    Symbol* s = m_symTab->Lookup(name, m_op.m_lineNumber);
    //        if (s==nullptr)
    //          ErrorHandler::e.Error("Could not find variable :" + name,m_op.m_lineNumber);

    s->m_value = new PVar(var);
}

void NodeBlock::Delete() {
    Node::Delete();

    if (m_useOwnSymTab && m_symTab!=nullptr) {
        m_symTab->Delete();
    }
    if (m_compoundStatement!=nullptr) {
        m_compoundStatement->Delete();
        m_compoundStatement = nullptr;
    }
    for (Node* n : m_decl) {
        n->Delete();
        delete n;
    }
    m_decl.clear();

}

void NodeBlock::PopZeroPointers(Assembler *as) {
    for (Node* n: m_decl) {
        NodeVarDecl* nv = dynamic_cast<NodeVarDecl*>(n);
        if (nv!=nullptr) {
            for (int i=0;i<nv->m_pushedPointers;i++)
                as->PopZeroPointer();
        }
    }

}

QString NodeBlock::Build(Assembler *as) {
    //as->VarDeclHeader();
    Node::Build(as);
    // In case memory block is acive
    as->EndMemoryBlock();
    as->PushBlock(m_currentLineNumber);



    QString label = as->NewLabel("block");
    as->Asm("jmp " + label);
    bool blockLabel = false;
    bool blockProcedure = false;
    if (m_decl.count()!=0) {
        //           as->PushBlock(m_decl[0]->m_op.m_lineNumber-1);
    }
    for (Node* n: m_decl) {
        // Print label at end of vardecl
        if (dynamic_cast<NodeVarDecl*>(n)==nullptr) {
/*            if (!blockLabel)
                as->Label(label);
            blockLabel = true;
*/
            if (!blockProcedure) // Print label at end of vardecl
            {
                if (n->m_op.m_lineNumber!=0) {
                    //                      as->PopBlock(n->m_op.m_lineNumber);
                    blockProcedure = true;
                    //   qDebug() << "pop" << n->m_op.m_lineNumber << " " << TokenType::getType(n->getType(as));
                }

            }

        }
        //if (dynamic_cast<NodeProcedureDecl*>(n)==nullptr)
        //qDebug() << "VarDeclBuild:" ;
        n->Build(as);
    }
    as->VarDeclEnds();
    as->PushCounter();
    as->EndMemoryBlock();
    if (!blockLabel)
        as->Label(label);
    if (m_compoundStatement!=nullptr)
        m_compoundStatement->Build(as);


    as->PopCounter(m_op.m_lineNumber-1);
    as->PopBlock(m_currentLineNumber);

    PopZeroPointers(as);

    //        as->PopAllZeroPointers();

    //        qDebug() << "ln:" << m_currentLineNumber;
    //qDebug() << "Adding at linenumber: " << m_op.m_lineNumber << "  cycles " << m_cycleCounter;
    return "";

}

PVar NodeBlock::Execute(SymbolTable *symTab, uint lvl) {
    Pmm::Data::d.Set(m_op.m_lineNumber, m_op.m_currentLineText);
    level = lvl+1;
    ErrorHandler::e.DebugLow("Calling BlockNode",level);

    for (Node* n: m_decl)
        n->Execute(m_symTab, level);
    ErrorHandler::e.DebugLow("Calling Compound" ,level);
    m_compoundStatement->Execute(m_symTab, level);
    Symbol* s = m_symTab->Lookup("return", m_op.m_lineNumber);
    if (s->m_value)
        return *s->m_value;
    return PVar();

}

void NodeBlock::ExecuteSym(SymbolTable *symTab) {
    m_useOwnSymTab = false;

    if (m_useOwnSymTab) {
        if (m_symTab == nullptr)
            m_symTab = new SymbolTable();
    }
    else m_symTab = symTab;

    for (Node* n: m_decl)
    {
        n->ExecuteSym(m_symTab);
    }
    m_compoundStatement->ExecuteSym(m_symTab);
}
