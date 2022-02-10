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
#include "nodefactory.h"

QMap<QString, bool> NodeBuiltinMethod::m_isInitialized;




void NodeBuiltinMethod::VerifyParams(Assembler* as)
{
    if (m_function==nullptr)
        return;

    QString error = "Error: "+m_procName +" requires parameter ";

    for (int p=0;p<m_params.count();p++) {
        m_params[p]->m_builtInFunctionParameterType = m_function->m_params[p];
//        qDebug() << "NodeBuiltinMethod " << m_function->m_params[p] << m_procName;
        QString cp = QString::number(p+1);

        if (m_params[p]->isPureNumericOrAddress() && !m_params[p]->isPureVariable()) {
            // Collapse here.. willl that work?
            int val = m_params[p]->getValueAsInt(as);

/*            auto num = qSharedPointerDynamicCast<NodeNumber>(m_params[p]);
            if (num!=nullptr) {
                qDebug() <<"IS NUMBER " <<Util::numToHex(num->m_val);
                qDebug() << (m_params[p]->m_op.getType())<<m_procName <<Util::numToHex(val)<<m_params[p]->getValue(as);
            }*/
            bool isAddress = m_params[p]->isAddress();
            bool isRef = m_params[p]->isReference();
            auto n = NodeFactory::CreateNumber(m_params[p]->m_op,val);
            n->setReference(isRef);

            if (isAddress) {
                n->m_op.m_type = TokenType::ADDRESS;
                m_params[p] = n;

            }
//                n->m_op.m_type = TokenType::INTEGER_CONST;
  //          else
    //            n->m_op.m_
//            qDebug() << isAddress << Util::numToHex(val) <<n->isAddress();
        }

        if (m_function->m_params[p]==BuiltInFunction::ADDRESS) {
//            qDebug() << m_procedure->m_procName << m_op.m_isReference;
            //qDebug() <<" ARHJ " << m_params[p]->getValue(as);
            if (!m_params[p]->isAddress()) {
//                qDebug() << "TYPE: " << m_params[p]->m_op.getType() <<m_params[p]->getValue(as);
//                ErrorHandler::e.Error(error + cp + " to be an address. Did you forget a ^?", m_op.m_lineNumber);
            }
            // REMEMBER
//            m_params[p]->setReference(true);
            m_params[p]->VerifyReferences(as);
        }
        if (m_function->m_params[p]==BuiltInFunction::NUMBER) {
            if (!m_params[p]->isPureNumericOrAddress())
                ErrorHandler::e.Error(error + cp + " to be pure numeric", m_op.m_lineNumber);
        }
        if (m_function->m_params[p]==BuiltInFunction::PROCEDURE) {
            if (qSharedPointerDynamicCast<NodeProcedure>(m_params[p])==nullptr)
                ErrorHandler::e.Error(error + cp + " to be a procedure", m_op.m_lineNumber);
        }

        if (qSharedPointerDynamicCast<NodeVar>(m_params[p])!=nullptr) {
                QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(m_params[p]);
                v->ExecuteSym(as->m_symTab);

                if (m_function->m_params[p]==BuiltInFunction::BYTE) {
                    if (v->isWord(as))
                        ErrorHandler::e.Warning("Method '"+m_procName+"' requires byte value for parameter "+QString::number(p+1)+", but an integer is provided. Might yield incorrect result. ",m_op.m_lineNumber);
                }
                if (m_function->m_params[p]==BuiltInFunction::INTEGER || m_function->m_params[p]==BuiltInFunction::BYTE) {
                    if (v->isPointer(as)) {
                        if (!v->hasArrayIndex())
                        ErrorHandler::e.Warning("Method '"+m_procName+"' requires a byte/integer value for parameter "+QString::number(p+1)+", but a pointer is provided. Might yield incorrect result. ",m_op.m_lineNumber);
                    }
                }


//            if (dynamic_cast<QSharedPointer<NodeProcedure>>(m_params[p])==nullptr)
  //              ErrorHandler::e.Error(error + cp + " to be a procedure", m_op.m_lineNumber);
        }
    }
}

void NodeBuiltinMethod::ReplaceInline(Assembler* as,QMap<QString, QSharedPointer<Node> > &inp)
{
    for (int i=0;i<m_params.count();i++) {
        auto n = m_params[i];
 //       qDebug() << "Testing parameter pureness " <<n->isPure();
        if (n->isPure()) {
//            qDebug() << n->getValue(as);
            for (QString k: inp.keys())
            if (n->getValue(as)==k) {
                m_params[i] = inp[k];
            }

        }
        else
            ErrorHandler::e.Error("TRSE currently does not support inline parameters to be in expressions in built-in methods. Please bug the developer about this", m_op.m_lineNumber);
//        n->ReplaceInline(as,inp);
    }
}


