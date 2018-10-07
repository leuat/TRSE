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

#include "machinecodeanalyzer.h"

MachineCodeAnalyzer::MachineCodeAnalyzer()
{
}

void MachineCodeAnalyzer::Load(QString file)
{
    QFile f(file);
    f.open(QFile::ReadOnly);
    m_data = f.readAll();
    f.close();

}

void MachineCodeAnalyzer::ClassifyZP(QVector<MemoryBlock*> blocks)
{
    for (MCZP& m : m_mczp) {
        for (MemoryBlock* b: blocks) {
            if (b->m_type==MemoryBlock::CODE ||b->m_type==MemoryBlock::MUSIC || b->m_type==MemoryBlock::ARRAY)
            if (m.m_addr>=b->m_start && m.m_addr<b->m_end) {
                if (!b->m_zeropages.contains(m.m_zp))
                    b->m_zeropages.append(m.m_zp);
            }
        }

    }
    for (MemoryBlock* b: blocks)
        std::sort(b->m_zeropages.begin(), b->m_zeropages.end(),std::less<int>());
}

void MachineCodeAnalyzer::AnalyzeZP()
{
    int addr=Syntax::s.m_startAddress;
    m_mczp.clear();
    for (int i=0;i<m_data.count()-1;i++) {
        uchar d = m_data[i];
        uchar n = m_data[i+1];
//        if (d==0x81 || d==0x91) {
            if (d==0x85 || d==0xA5) {
//                if (d==0xB1 || d==0x91) {
           m_mczp.append(MCZP(n,addr));
        }
        addr++;
    }
}
