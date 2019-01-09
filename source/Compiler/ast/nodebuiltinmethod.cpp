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

#include "nodebuiltinmethod.h"
#include "nodenumber.h"
#include "nodevar.h"
#include "nodestring.h"
#include "nodeprocedure.h"

QMap<QString, bool> NodeBuiltinMethod::m_isInitialized;




void NodeBuiltinMethod::VerifyParams(Assembler* as)
{
    if (m_function==nullptr)
        return;

    QString error = "Error: "+m_procName +" requires parameter ";

    for (int p=0;p<m_params.count();p++) {
        QString cp = QString::number(p+1);
        if (m_function->m_params[p]==BuiltInFunction::ADDRESS) {
            if (!m_params[p]->isAddress()) {
     //           qDebug() << "TYPE: " << m_params[p]->m_op.getType();
                ErrorHandler::e.Error(error + cp + " to be an address. Did you forget a ^?", m_op.m_lineNumber);
            }
        }
        if (m_function->m_params[p]==BuiltInFunction::NUMBER) {
            if (!m_params[p]->isPureNumeric())
                ErrorHandler::e.Error(error + cp + " to be pure numeric", m_op.m_lineNumber);
        }
        if (m_function->m_params[p]==BuiltInFunction::PROCEDURE) {
            if (dynamic_cast<NodeProcedure*>(m_params[p])==nullptr)
                ErrorHandler::e.Error(error + cp + " to be a procedure", m_op.m_lineNumber);
        }
        if (dynamic_cast<NodeVar*>(m_params[p])!=nullptr) {
                NodeVar* v = dynamic_cast<NodeVar*>(m_params[p]);
                v->ExecuteSym(as->m_symTab);
//            if (dynamic_cast<NodeProcedure*>(m_params[p])==nullptr)
  //              ErrorHandler::e.Error(error + cp + " to be a procedure", m_op.m_lineNumber);
        }
    }
}

