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

#include "nodevartype.h"


NodeVarType::NodeVarType(Token t, QString position, Token arrayVarType, QStringList data):Node() {
    m_op = t;
    value = t.m_value;
    m_arrayVarType = arrayVarType;
    m_data = data;
    m_position = position;

}

NodeVarType::NodeVarType(Token t, QStringList data) : Node() {
    m_op = t;
    value = t.m_value;
    //m_arrayVarType = TokenType::STRING;
    m_data = data;

}

NodeVarType::NodeVarType(Token t, QString filename, QString position) : Node() {
    m_op = t;
    value = t.m_value;
    m_filename = filename;
    m_position = position;
    m_arrayVarType.m_type = TokenType::BYTE;

}

NodeVarType::NodeVarType(Token t, QString initvalue) : Node() {
    m_op = t;
    value = t.m_value;
    initVal = initvalue;

}

void NodeVarType::VerifyFlags(bool isProcedureParameter) {
    for (QString f : m_flags) {
        if (isProcedureParameter) {
            if (!Syntax::s.m_currentSystem->m_allowedProcedureTypeFlags.contains(f))
                ErrorHandler::e.Error("Procedure type '"+value+"' on this system does not support the flag '"+f+"'",m_op.m_lineNumber);
        }
        else {
            if (!Syntax::s.m_currentSystem->m_allowedGlobalTypeFlags.contains(f))
                ErrorHandler::e.Error("Globally defined type '"+value+"' on this system does not support the flag '"+f+"'",m_op.m_lineNumber);
        }
        if (f=="no_term") {
            // only works for strings
            if (m_op.m_type!=TokenType::STRING)
                ErrorHandler::e.Error("Type flag 'no_term' is only allowed for strings.",m_op.m_lineNumber);
        }
    }
    // Individual type tests

}
