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

#include "nodeproceduredecl.h"
#include "nodebuiltinmethod.h"
#include "source/Compiler/syntax.h"


NodeProcedureDecl::NodeProcedureDecl(Token t, QString m):Node() {
    m_op = t;
    m_procName = m;
    m_type=0;
}

NodeProcedureDecl::NodeProcedureDecl(Token t, QString m, QVector<QSharedPointer<Node> > paramDecl, QSharedPointer<Node> block, int type) : Node() {
    m_op = t;

    m_procName = m;
    m_block = block;
    m_paramDecl = paramDecl;
    if (block!=nullptr) {
        QSharedPointer<NodeBlock> b = qSharedPointerDynamicCast<NodeBlock>(block);
        for (int i=0;i<m_paramDecl.count();i++)
            b->m_decl.append(m_paramDecl[i]);
    }
    m_type=type;

}

void NodeProcedureDecl::AppendBlock(QSharedPointer<Node> block)
{
    QSharedPointer<NodeBlock> b = qSharedPointerDynamicCast<NodeBlock>(block);
    if (b==nullptr)
        return;
    for (int i=0;i<m_paramDecl.count();i++)
        b->m_decl.append(m_paramDecl[i]);
    m_block = block;
}


void NodeProcedureDecl::SetParametersValue(QVector<PVar> &lst) {
    if (lst.count()!=m_paramDecl.count())
        ErrorHandler::e.Error("Incorrect number of parameters calling procedure '" +
                              m_procName +"'. Requires "+QString::number(m_paramDecl.count()) +
                              " parameters but is called with "+QString::number(lst.count()));

    for (int i=0;i<m_paramDecl.count();i++) {
        QSharedPointer<NodeVarDecl> vd = qSharedPointerDynamicCast<NodeVarDecl>(m_paramDecl[i]);
        QSharedPointer<NodeVar> v= qSharedPointerDynamicCast<NodeVar>(vd->m_varNode);
        QString name = v->value;
        PVar val = lst[i];
        qSharedPointerDynamicCast<NodeBlock>(m_block)->SetParameter(name, val);

    }
    //        ((QSharedPointer<NodeBlock>)m_block)->SetParameters(lst, names);
}


void NodeProcedureDecl::ExecuteSym(QSharedPointer<SymbolTable>  symTab) {


//    m_block->ExecuteSym(symTab);

    auto decls = m_paramDecl;
    int stackPos = 0;

    for (auto& nv:decls) {
        auto nv2 = qSharedPointerDynamicCast<NodeVarDecl>(nv);
        auto var = qSharedPointerDynamicCast<NodeVar>(nv2->m_varNode);
        auto typeNode = qSharedPointerDynamicCast<NodeVarType>(nv2->m_typeNode);
        QSharedPointer<Symbol> varSymbol = symTab->Lookup(var->value,var->m_op.m_lineNumber);
        if (varSymbol  && varSymbol->m_isStackVariable) {
            stackPos +=varSymbol->m_size;
//            qDebug() << "EHHH";

        }
    }

    for (auto& nv:decls) {
        auto nv2 = qSharedPointerDynamicCast<NodeVarDecl>(nv);
        auto var = qSharedPointerDynamicCast<NodeVar>(nv2->m_varNode);
        auto typeNode = qSharedPointerDynamicCast<NodeVarType>(nv2->m_typeNode);
        QSharedPointer<Symbol> varSymbol = symTab->Lookup(var->value,var->m_op.m_lineNumber);
        if (varSymbol  && varSymbol->m_isStackVariable) {
            varSymbol->m_isStackVariable = true;
            stackPos-=varSymbol->m_size;
            varSymbol->m_stackPos = stackPos;
//            qDebug() << "setting pos : "<<varSymbol->m_stackPos<<varSymbol->m_name;

        }
    }
/*    for (auto& nv:decls) {
        nv->ExecuteSym(symTab);
    }
*/

}
