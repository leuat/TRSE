#ifndef NODEFACTORY_H
#define NODEFACTORY_H

#include "source/Compiler/ast/nodebinop.h"
#include "source/Compiler/ast/nodenumber.h"
#include "source/Compiler/ast/nodestring.h"
#include "source/Compiler/ast/nodeunaryop.h"
#include "source/Compiler/ast/nodecompound.h"
#include "source/Compiler/ast/nodevar.h"
#include "source/Compiler/ast/nodeassign.h"
#include "source/Compiler/ast/nodevardecl.h"
#include "source/Compiler/ast/nodeblock.h"
#include "source/Compiler/ast/nodeprogram.h"
#include "source/Compiler/ast/nodevartype.h"
#include "source/Compiler/ast/nodeproceduredecl.h"
#include "source/Compiler/ast/nodeprocedure.h"
#include "source/Compiler/ast/nodeconditional.h"
#include "source/Compiler/ast/nodeforloop.h"
#include "source/Compiler/ast/nodebuiltinmethod.h"
#include "source/Compiler/ast/nodewhileloop.h"
#include "source/Compiler/ast/nodeasm.h"
#include "source/Compiler/ast/node.h"
#include "source/Compiler/ast/nodebinaryclause.h"
#include "source/Compiler/ast/noderepeatuntil.h"
#include "source/Compiler/ast/nodecase.h"
#include "source/Compiler/ast/nodecontrolstatement.h"


class NodeFactory
{
public:
    NodeFactory();
    /*
     * Small Node Factory
     *
     * */
    static QSharedPointer<NodeNumber> CreateNumber(Token t,int i);
    static QSharedPointer<NodeVar> CreateVariable(Token t,QString v);
    static QSharedPointer<Node> CreateBinop(Token t,TokenType::Type type, QSharedPointer<Node> left, QSharedPointer<Node> right);
    static QSharedPointer<NodeAssign> CreateAssign(Token t,QSharedPointer<Node> left, QSharedPointer<Node> right);



};

#endif // NODEFACTORY_H
