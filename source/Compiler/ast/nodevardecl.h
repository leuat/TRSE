#ifndef NODEVARDECL_H
#define NODEVARDECL_H


#include "source/Compiler/token.h"
#include "source/Compiler/pvar.h"
#include "source/Compiler/symboltable.h"
#include "source/Compiler/errorhandler.h"
#include "source/Compiler/ast/node.h"
#include "source/Compiler/ast/nodevar.h"
#include "source/Compiler/ast/nodevartype.h"
#include "source/Compiler/misc/sidfile.h"
#include "source/LeLib/util/util.h"
class NodeVarDecl : public Node {
public:
    Node* m_varNode = nullptr;
    Node* m_typeNode;
    int m_fileSize=0;
    int m_dataSize=0;
    SidFile sid;

    int m_pushedPointers = 0;

    NodeVarDecl(Node* varNode, Node* typeNode);

    void Delete() override;

    int getDataSize() {
        return m_dataSize;
    }

    void IncSid(Assembler* as);

    void IncBin(Assembler* as);

    void InitSid(QString projectDir);

    void DeclarePointer(Assembler*  as);

    QString Build(Assembler* as) override;


    PVar Execute(SymbolTable* symTab, uint lvl) override;
    void ExecuteSym(SymbolTable* symTab) override;


};



#endif // NODEVARDECL_H
