#include "token.h"

QString TokenType::types[65] = { "INTEGER", "PLUS", "MINUS", "MUL", "DIV", "LPAREN",
            "RPAREN", "TEOF", "ID", "ASSIGN", "BEGIN",
            "END", "SEMI", "DOT", "VAR", "REAL",
            "PROGRAM", "COLON", "COMMA", "INTEGER_CONST", "REAL_CONST", "PROCEDURE",
            "STRING", "IF", "THEN", "EQUALS", "NOTEQUALS", "GREATER", "LESS",
            "FOR", "TO","DO", "NADA","ADDRESS", "WHILE", "ARRAY", "OF",
            "BYTE", "LBRACKET", "RBRACKET", "INCBIN", "ASM", "INCSID",
            "INTERRUPT", "BITAND", "BITOR", "ELSE", "OR", "AND", "POINTER","AT",
            "INCLUDE", "DEFINE", "PREPROCESSOR", "IFDEF", "ENDIF", "IFNDEF", "OFFPAGE", "ONPAGE",
            "STEP", "UNROLL", "LOOPX", "LOOPY", "CSTRING", "USERDATA"};

Token::Token()
{
    m_lineNumber = Pmm::Data::d.lineNumber;
    m_currentLineText = Pmm::Data::d.currentLineText;

}
