#ifndef TOKEN_H
#define TOKEN_H

#include <QString>
#include "data_pmm.h"


class TokenType {
public:
    enum Type {
        INTEGER, PLUS, MINUS, MUL, DIV, LPAREN,
        RPAREN, TEOF, ID, ASSIGN, BEGIN,
        END, SEMI, DOT, VAR, REAL,
        PROGRAM, COLON, COMMA, INTEGER_CONST,
        REAL_CONST, PROCEDURE, STRING,
        IF, THEN, EQUALS,NOTEQUALS, GREATER, LESS,
        FOR, TO, DO, NADA, ADDRESS, WHILE,
        ARRAY, OF, BYTE, LBRACKET, RBRACKET, INCBIN, ASM, INCSID,
        INTERRUPT, BITAND, BITOR, ELSE, OR, AND, POINTER, AT, INCLUDE, DEFINE, PREPROCESSOR,
        IFDEF, ENDIF, IFNDEF, OFFPAGE, ONPAGE, STEP, UNROLL, LOOPX, LOOPY, CSTRING, USERDATA
    };

    static QString types[];
    static QString getType(Type t) {
        return types[t];
    }

    QString m_value;
    Type m_type;

    TokenType() {}
    TokenType (Type t, QString v) {
        m_type = t;
        m_value = v;
    }
};


class Token
{
public:
    TokenType::Type m_type;
    QString m_value;
    int m_intVal=0;
    int m_lineNumber=0;
    QString m_currentLineText="";
    QString getType();

    Token();


    Token(TokenType::Type t, QString val);
    Token(TokenType::Type t, int val);

};

#endif // TOKEN_H
