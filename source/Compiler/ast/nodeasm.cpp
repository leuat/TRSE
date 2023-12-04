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

#include "nodeasm.h"


void NodeAsm::FindPotentialSymbolsInAsmCode(QStringList &lst) {
    lst <<Syntax::s.m_currentSystem->AnalyseForPotentialVariables(m_asm);
}

void NodeAsm::ReplaceInlineAssemblerVariables(Assembler *as, QString var, QString val)
{
    m_outAsm.replace(var,val);
    int cnt = 0;
    while (m_outAsm.contains("{label"+QString::number(cnt)+"}")) {
        QString lbl = "generated_label_"+QString::number(Data::data.genLabel);
        m_outAsm.replace("{label"+QString::number(cnt)+"}",lbl);
        cnt+=1;
        Data::data.genLabel+=1;
    }
//    qDebug() << m_outAsm;
}

void NodeAsm::ResetInlineAssembler()
{
    m_outAsm = m_asm;
}
