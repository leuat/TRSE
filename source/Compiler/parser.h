#ifndef PARSER_H
#define PARSER_H

#include "source/Compiler/ast/ast.h"
#include "source/Compiler/lexer.h"

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
#include "source/Compiler/ast/nodebinaryclause.h".h"




class Parser {
public:

    QMap<QString, Node*> m_procedures;
    QMap<QString, QString> m_preprocessorDefines;
    QVector<Node*> m_proceduresOnly;
    Lexer* m_lexer;
    Token m_currentToken;
    int m_pass = 0;
    SymbolTable* m_symTab;
    Parser();
    Parser(Lexer* l) {
        m_lexer = l;
    }
    ~Parser() {
        Delete();
    }
    QVector<MemoryBlock> m_userBlocks;

    void Delete();

    void Eat(TokenType::Type t);
    void VerifyToken(Token t);

    void HandlePreprocessorInParsing();
    void Preprocess();
    void PreprocessReplace();
    void PreprocessIfDefs(bool ifdef);

    int findPage();

    Node* Parse();
    Node* Variable();
    Node* Empty();
    Node* AssignStatement();
    Node* Statement();
    QVector<Node*> StatementList();
    Node* CompoundStatement();
    Node* Program();
    Node* Factor();
    Node* Expr();
    Node* Term();
    Node* FindProcedure();
    Node* BinaryClause();
    //Node* LogicalClause();
    Node* Block(bool useOwnSymTab);
    QVector<Node*> Parameters();
    Node* ForLoop();
//    Node* WhileLoop();
    Node* String();

    Node* Conditional(bool isWhileLoop=false);
//    QVector<Node*> Procedure();
    QVector<Node*> Declarations(bool isMain);
    QVector<Node*> VariableDeclarations();
    Node* TypeSpec();
    Node* BuiltinFunction();
    Node* Constant();
    Node* InlineAssembler();
    void Eat();


    int findSymbolLineNumber(QString symbol);


    void InitBuiltinFunctions();


    void InitBuiltinFunction(QStringList methodName, QString builtinFunctionName);

};


#endif // PARSER_H
