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
#include "source/Compiler/assembler/mos6502/mos6502.h"
#include "source/Compiler/assembler/AsmM68000.h"
#include "source/Compiler/assembler/asmpascal.h"
#include "source/Compiler/assembler/asmx86.h"
#include "source/Compiler/assembler/mos6502/astdispatcher6502.h"
#include "source/Compiler/assembler/astdispatcher68000.h"
#include "source/Compiler/assembler/astdispatcherx86.h"
#include "source/LeLib/util/cinifile.h"
#include <QSharedPointer>
#include "source/Compiler/systems/abstractsystem.h"

class Compiler
{
public:
    QSharedPointer<Node> m_tree = nullptr;
    QSharedPointer<Assembler> m_assembler = nullptr;
    QSharedPointer<AbstractASTDispatcher> m_dispatcher = nullptr;
    Parser m_parser;
    QSharedPointer<Lexer> m_lexer;
    QSharedPointer<CIniFile> m_ini, m_projectIni;
    FatalErrorException recentError;
    Compiler(QSharedPointer<CIniFile> ini, QSharedPointer<CIniFile> pIni);
    Compiler() {};
    ~Compiler();

    bool m_isTRU = false;

    virtual void InitAssemblerAnddispatcher(QSharedPointer<AbstractSystem> system) = 0;
    virtual void Connect() = 0;

    void Parse(QString text, QStringList lst);
    bool Build( QSharedPointer<AbstractSystem> system, QString projDir);
    void CleanupBlockLinenumbers();
    void SaveBuild(QString filename);
    void HandleError(FatalErrorException fe, QString se);
//    void Destroy();
//    void FindLineNumberAndFile(int inLe, QString& file, int& outle);
    void WarningUnusedVariables();
    virtual void CleanupCycleLinenumbers(QString currentFile, QMap<int, int> &ocycles, QMap<int, int> &retcycles) {}

};

#endif // Compiler_H
