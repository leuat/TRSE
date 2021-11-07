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

#ifndef Compiler_H
#define Compiler_H

#include "../parser.h"
#include "source/Compiler/assembler/asm6502.h"
#include "source/Compiler/assembler/asm68000.h"
#include "source/Compiler/assembler/asmx86.h"
#include "source/Compiler/assembler/asmz80.h"
#include "source/Compiler/assembler/asmarm.h"
#include "source/Compiler/codegen/codegen_6502.h"
#include "source/Compiler/codegen/codegen_m68k.h"
#include "source/Compiler/codegen/codegen_arm.h"
#include "source/Compiler/codegen/codegen_x86.h"
#include "source/Compiler/codegen/codegen_z80.h"
#include "source/Compiler/codegen/codegen_tripe.h"
#include "source/LeLib/util/cinifile.h"
#include <QSharedPointer>
#include "source/Compiler/systems/abstractsystem.h"
#include "source/OrgAsm/orgasm.h"
/*
 *
 *
 *
 *  Main compiler class.
 *
 *  Main objects:
 *   - Parser
 *   - Lexer
 *   - Assembler
 *   - Code generator
 *
 *  Main methods:
 *   - Parse
 *   - Build
 *
 *
 *
 *
 *
 *
 */
class Compiler : public QObject
{
    Q_OBJECT

public:
    QSharedPointer<Node> m_tree = nullptr;
    QSharedPointer<Assembler> m_assembler = nullptr;
    QSharedPointer<AbstractCodeGen> m_codeGen = nullptr;

    Parser m_parser;
    QSharedPointer<Lexer> m_lexer;
    QSharedPointer<CIniFile> m_ini, m_projectIni;
    FatalErrorException recentError;
    bool m_isTRU = false;



    Compiler(QSharedPointer<CIniFile> ini, QSharedPointer<CIniFile> pIni);
    Compiler() {}
    virtual ~Compiler();

    void Parse(QString text, QStringList lst, QString fname);
    bool Build( QSharedPointer<AbstractSystem> system, QString projDir);
    void CleanupBlockLinenumbers();
    virtual void CleanupCycleLinenumbers(QString currentFile, QMap<int, int> &ocycles, QMap<int, int> &retcycles, bool isCycles=true) {}
    virtual bool SetupMemoryAnalyzer(QString filename, Orgasm* orgAsm = nullptr) { return true;}

    void SaveBuild(QString filename);
    void ApplyOptions(QMap<QString,QStringList>& opt);

private:


    virtual void InitAssemblerAnddispatcher(QSharedPointer<AbstractSystem> system) = 0;
    virtual void Connect() = 0;


    void HandleError(FatalErrorException fe, QString se);
//    void Destroy();
//    void FindLineNumberAndFile(int inLe, QString& file, int& outle);
    void WarningUnusedVariables();


public:
signals:
    void EmitTick(QString val);
    void emitRequestSystemChange(QString val);
public slots:
    void AcceptDispatcherTick(QString val);
    void AcceptRequestSystemChange(QString val);
};

#endif // Compiler_H
