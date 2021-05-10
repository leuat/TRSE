/*
 * Turbo Rascal Syntax error, “;” expected but “BEGIN” (TRSE, Turbo Rascal SE)
 * 8 bit software development IDE for the Commodore 64
 * Copyright (C) 2018  Nicolaas Ervik Groeneboom (nicolaas.groeneboom@gmail.com)
 *
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program (LICENSE.txt).
 *   If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef PARSER_H
#define PARSER_H

#include <QJSEngine>
#include <QProcess>
#include "source/Compiler/ast/ast.h"
#include "source/Compiler/lexer.h"
#include <QRegularExpression>
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
#include "source/LeLib/limage/limageio.h"
#include "source/LeLib/limage/c64fullscreenchar.h"
#ifndef CLI_VERSION
#include "source/trsedocuments/formpaw.h"
#endif
#include "source/LeLib/util/tool.h"
#include "source/LeLib/limage/compression.h"
#include "source/LeLib/util/SimplexNoise.h"
#include "source/LeLib/ttrfile.h"
//#include "source/Compiler/sourcebuilder.h"

class ParserBlock {
public:
    int m_blockID;
    QString pos;
};

class LMacro {
public:
    QString str;
    QStringList params;
    int noParams;
    LMacro() {

    }
    LMacro(QString pstr, QStringList pparams, int no ) {
        str = pstr;
        params = pparams;
        noParams = no;
    }
};


class Parser : public QObject {
    Q_OBJECT
public:
    int m_tick = 0;
    QVector<int> div2s = QVector<int>() <<2 <<4<<8<<16<<32<<64<<128<<256<<512<<1024;
    QVector<ParserBlock> m_parserBlocks;
    static QStringList s_usedTRUs, s_usedTRUNames;
    QMap<QString, LMacro> m_macros;
    QString m_procPrefix = "";
    int m_prevPercent = -1;
//    int m_currentParserBlock=-1;

    bool m_hasBeenApplied = false;
    //bool m_breakSubvar = false;
    QSharedPointer<Node> m_currentProcedureCall;
    QString m_addInitialReferenceToProcedureCall = "";

    QString m_currentClass ="";

    QString m_vicMemoryConfig;
    QString m_currentDir, m_currentFileShort;
    QStringList m_removedProcedures;
    QVector<QStringList> m_obsoleteWarnings;
    QStringList m_monitorCommands;
    QMap<QString, QSharedPointer<Node>> m_procedures;
    QMap<QString, QString> m_preprocessorDefines;
    QStringList m_diskFiles;
    QStringList m_warningsGiven;
    QStringList m_doNotRemoveMethods;
    QString m_initAssembler = "";
    // Preprocessor stuff
    QVector<QString> m_lastKey;
    QVector<bool> m_lastIfdef;


    QVector<QSharedPointer<Node>> m_proceduresOnly;
    QVector<QSharedPointer<Node>> m_mergedProcedures;
    QVector<QString> m_ignoreMethods;
    QSharedPointer<Lexer> m_lexer;
    Token m_currentToken, m_lastStartBlockToken;
    QString m_inCurrentProcedure="main";
    bool m_isTRU = false;
    int PASS_PREPRE = 0;
    int PASS_PRE = 1;
    int PASS_FIRST = 10;
    int PASS_CODE = 2;
    int PASS_OTHER = 4;

    int m_pass = 0, m_acc=0;
    bool m_ignoreAll = false;
    bool m_isRecord = false;
    bool m_removeUnusedDecls = false;
    QStringList  m_initJumps;
    QMap<TokenType::Type, QString> m_typeFlags;
    QSharedPointer<SymbolTable>  m_symTab = nullptr;
    QSharedPointer<CIniFile> m_projectIni, m_settingsIni;
    QSharedPointer<Node> m_tree = nullptr;
    QVector<QSharedPointer<Node>>* m_currentStatementList = nullptr;
    QMap<QString, QSharedPointer<Node>> m_types;

    bool m_doNotPrefix = false;
    QStringList m_doNotPrefixSymbols;

    QString WashVariableName(QString v);
    QString VerifyVariableName(QString v);



    /*
     * Small Node Factory
     *
     * */
    QSharedPointer<NodeNumber> CreateNumber(int i);
    QSharedPointer<NodeVar> CreateVariable(QString v);
    QSharedPointer<Node> CreateBinop(TokenType::Type t, QSharedPointer<Node> left, QSharedPointer<Node> right);
    QSharedPointer<NodeAssign> CreateAssign(QSharedPointer<Node> left, QSharedPointer<Node> right);




    QSharedPointer<Symbol> getSymbol(QSharedPointer<Node> var);

    QVector<QString> m_ignoreBuiltinFunctionTPU;
    static QVector<QSharedPointer<Parser>> m_tpus;


    QStringList getFlags();


    Parser();
    Parser(QSharedPointer<Lexer> l, QSharedPointer<CIniFile> projectFile) {
        m_lexer = l;
        m_projectIni = projectFile;
    }
    ~Parser() {
        Delete();
    }
    QVector<QSharedPointer<MemoryBlock>> m_userBlocks;
    QJSEngine m_jsEngine;

    void Delete();

    void InitObsolete();
    void Eat(TokenType::Type t);
    void VerifyToken(Token t);
    void InitSystemPreprocessors();
    void InitSystemSymbols();
    void HandlePreprocessorInParsing();
    void StripWhiteSpaceBeforeParenthesis();
    void RemoveComments();
    bool PreprocessIncludeFiles();
    void PreprocessAll();
    void PreprocessSingle();
    void PreprocessReplace();
    void PreprocessIfDefs(bool ifdef);
    void PreprocessConstants();
    void ApplyTPUBefore();
    void ApplyTPUAfter(QVector<QSharedPointer<Node>>& declBlock, QVector<QSharedPointer<Node>>& procs);

    int GetParsedIntOld();
    int GetParsedInt(TokenType::Type forceType);

    int getIntVal(Token t);
    int findPage();
    void VerifyTypeSpec(Token& t);

    void RemoveUnusedProcedures();
    void RemoveUnusedSymbols(QSharedPointer<NodeProgram> root);
    QSharedPointer<Node> Parse(bool removeUnusedDecls, QString param, QString globalDefines, bool useLocals);
    void VarDeclarations(QVector<QSharedPointer<Node>>& decl, QString blokName);
    void ProcDeclarations(QVector<QSharedPointer<Node>>& decl, QString blokName);

    QSharedPointer<Node> ApplyClassVariable(QSharedPointer<Node> var);

    QSharedPointer<Node> Variable(bool isSubVar=false);
    QSharedPointer<Node> SubVariable(QString parent,QSharedPointer<Node> parentExpr);
    QSharedPointer<Node> Empty();
    QVector<QSharedPointer<Node>> Record(QString name);
//    QVector<QSharedPointer<Node>> Class(QString name);
//    QSharedPointer<Node> Record();
    QSharedPointer<Node> Case();
    QSharedPointer<Node> AssignStatement();
    QSharedPointer<Node> Statement();
    QVector<QSharedPointer<Node>> StatementList();
    QSharedPointer<Node> CompoundStatement();
    QSharedPointer<Node> Program(QString param);
    QSharedPointer<Node> Factor();
    QSharedPointer<Node> RepeatUntil();
    QSharedPointer<Node> Expr();
    QSharedPointer<Node> Term();
    QSharedPointer<Node> FindProcedure(bool& isAssign,QSharedPointer<Node> parentExpr);
    QSharedPointer<Node> BinaryClause();


    bool isRecord(Token& t);
    bool isClass(Token& t);
    bool nextIsExpr();

    void AppendComment(QSharedPointer<Node> n);

//    void AppendComment(QSharedPointer<Node> n);

    QVector<QSharedPointer<Node>> ConstDeclaration();
    QSharedPointer<Node> TypeDeclaration();
    //QSharedPointer<Node> LogicalClause();
    QSharedPointer<Node> Block(bool useOwnSymTab, QString blockName="");
    QSharedPointer<Node> BlockNoCompound(bool useOwnSymTab, QString blockName="");
    QVector<QSharedPointer<Node>> Parameters(QString blockName);
    QSharedPointer<Node> ForLoop(bool inclusive);
//    QSharedPointer<Node> WhileLoop();
    QSharedPointer<Node> String(bool isCString);

    QSharedPointer<Node> Conditional(bool isWhileLoop=false);
//    QVector<QSharedPointer<Node>> Procedure();
    QVector<QSharedPointer<Node>> Declarations(bool isMain, QString blockName, bool hasBlock = true);
    QVector<QSharedPointer<Node>> VariableDeclarations(QString blockName, bool isProcedureParams = false);
    QSharedPointer<Node> TypeSpec(bool isInProcedure, QStringList varNames);
    QSharedPointer<Node> BuiltinFunction();
    QSharedPointer<Node> Constant();
    QSharedPointer<Node> InlineAssembler();
    QSharedPointer<Node> AssignStatementBetweenObjects(QSharedPointer<Node> left,QSharedPointer<Node> right);
    QStringList m_parserAppendix;


    void HandleExportPalette();
    void HandleSetCompressionWeights();
    void HandleMacro();
    void HandleExecute();
    void HandlePerlinNoise();
    void HandleCallMacro(QString name, bool ignore);
    void HandleExportCompressed();
    void HandleExport();
    void HandleCompress();
    void HandleExportParallaxData();
    void HandleExportBW();
    void HandleBuildPaw();
    void HandleExportPrg2Bin();
    void HandlePBMExport();
    void HandleVBMExport();
    void HandleVBMExportColor();
    void HandleVBMExportChunk();
    void HandleExportFrame();
    void HandleSpriteCompiler();
    void HandleSpritePacker();
    void HandleProjectSettingsPreprocessors();
    void HandleAKGCompiler();
    void HandleUseTPU(QString fileName);
    void Eat();
    void HandleImportChar();
    QStringList BuildTable(int cnt, TokenType::Type type);

    int getParsedNumberOrConstant();

    int findSymbolLineNumber(QString symbol);



    void InitBuiltinFunctions();
    void VerifyInlineSymbols6502(QString s);

    void InitBuiltinFunction(QStringList methodName, QString builtinFunctionName, QString initjump="");

signals:
    void EmitTick(QString val);

};


#endif // PARSER_H
