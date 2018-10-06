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

#ifndef MACHINECODEANALYZER_H
#define MACHINECODEANALYZER_H

#include <QByteArray>
#include <QFile>
#include <QVector>
#include "source/Compiler/assembler/assembler.h"


class MCZP {
 public:
    int m_zp, m_addr;
    MCZP(int zp, int addr) {
        m_zp = zp;
        m_addr = addr;
    }
    MCZP() {

    }
};

class MachineCodeAnalyzer
{
public:
    int startAddr = 0x800;

    QByteArray m_data;
    MachineCodeAnalyzer();
    void Load(QString file);
    QVector<MCZP> m_mczp;
    void ClassifyZP(QVector<MemoryBlock*> blocks);

    void AnalyzeZP();


};

#endif // MACHINECODEANALYZER_H
