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

#include "token.h"

QString TokenType::types[no_types] = {
            "NONE", "INTEGER", "PLUS", "MINUS", "MUL", "DIV", "LPAREN",
            "RPAREN", "TEOF", "ID", "ASSIGN", "BEGIN",
            "END", "SEMI", "DOT", "VAR", "REAL",
            "PROGRAM", "COLON", "COMMA", "INTEGER_CONST", "REAL_CONST", "PROCEDURE",
            "STRING", "IF", "THEN", "EQUALS", "NOTEQUALS", "GREATER", "LESS",
            "FOR", "FORI", "TO","DO", "NADA","ADDRESS", "WHILE", "ARRAY", "OF",
            "BYTE", "LBRACKET", "RBRACKET", "INCBIN", "ASM", "INCSID",
            "INTERRUPT", "BITAND", "BITOR", "ELSE", "OR", "AND", "POINTER","AT",
            "INCLUDE", "DEFINE", "PREPROCESSOR", "IFDEF", "ENDIF", "IFNDEF", "OFFPAGE", "ONPAGE",
            "STEP", "UNROLL", "LOOPX", "LOOPY", "CSTRING", "USERDATA", "STARTBLOCK", "ENDBLOCK",
            "IGNOREMETHOD", "ERROR", "WEDGE", "USE", "INCNSF", "STARTASSEMBLER", "LONG", "CHIPMEM",
            "GREATEREQUAL", "LESSEQUAL","DONOTREMOVE", "CASE", "WORD", "SHR", "SHL", "XOR", "REPEAT",
            "UNTIL", "CONST", "COMMENT", "ABSOLUTE", "LENGTH", "RECORD", "UNIT", "GLOBAL",
            "WRAM", "HRAM", "ALIGNED", "BANK", "SPRRAM","INLINE", "BUILDTABLE", "USES", "FUNCTION", "COMPRESSED",
            "PURE", "PURE_VARIABLE", "PURE_NUMBER", "NO_TERM"

};


QString Token::getType() {
    return TokenType::types[m_type];
}

Token::Token()
{
    m_lineNumber = Pmm::Data::d.lineNumber;
    m_currentLineText = Pmm::Data::d.currentLineText;

}

QString Token::getNumAsHexString()
{

    QString position = m_value;
    if (position=="")
        position="$"+QString::number(m_intVal,16);

    return position;
}

Token::Token(TokenType::Type t, QString val) {
    m_value = val;
    m_type = t;
    m_lineNumber = Pmm::Data::d.lineNumber;
    m_currentLineText = Pmm::Data::d.currentLineText;
}

Token::Token(TokenType::Type t, long val) {
    m_intVal = val;
    m_type = t;
    m_lineNumber = Pmm::Data::d.lineNumber;
    m_currentLineText = Pmm::Data::d.currentLineText;

}

TokenType::Type TokenType::getType(QString s) {
    //      exit(1);
    for (int i=0;i<no_types;i++)
        if (types[i].toLower()==s.toLower()) {
            return (Type)i;
        }

    return NONE;
}
