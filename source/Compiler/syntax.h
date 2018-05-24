#ifndef SYNTAX_H
#define SYNTAX_H

#include <QString>
#include <QVector>
#include "token.h"
#include <QDebug>
#include <QMap>
#include <QList>
#include "pvar.h"
#include "errorhandler.h"



class C64Key {
public:
    QString m_name;
    QString m_key;
    unsigned char m_value;
    unsigned char m_row, m_column;
    C64Key() {}
    C64Key(QString name, QString key, int value, int row, int column) {
        m_name = name;
        m_key = key;
        m_value = value;
        m_row = row;
        m_column = column;

    }

};



class BuiltInFunction {
public:
    enum Type {STRING, INTEGER, REAL};
    QString m_name;
    QList<Type> m_params;

    BuiltInFunction() {}
    bool m_initFunction = false;
//    QVector<PVar> m_params;

    BuiltInFunction(QString name, QList<BuiltInFunction::Type> params, bool initFunction = false) {
        m_name = name;
        m_params = params;
        m_initFunction = initFunction;
    }
};

class Syntax
{
public:
    QVector<Token> reservedWords;
    //QVector<BuiltInFunction> builtinFunctions;
    QMap<QString, BuiltInFunction> builtInFunctions;
    Syntax();
    void SetupReservedWords();
    void SetupBuiltinFunctions();
    void SetupKeys();
    QString puredigit = "0123456789^";
    QString digit = "^0123456789$%";
    QString digitAll = "^0123456789$%ABCDEFabcdef";
    QString alpha = "abcdefghijklmnopqrstuvwxyz_";
    QString alnum =alpha+digit;
    QString alnumString =alpha+digit+ " ;:æøå!#¤%&/()=.,-+*";
    uint lineNumber = 0;
    void SetupConstants();


    QMap<unsigned char, C64Key> m_c64keys;

    static Syntax s;

    bool isNumeric(QString s) {
        bool ok;
        int dec = s.toInt(&ok, 10);
        return ok;
    }

    bool isDigit(QString s) {
        // Check if HEX
        return digit.contains(s);

    }
    bool isDigitHex(QString s) {
        // Check if HEX
        return digitAll.contains(s);

    }
    bool isAlnum(QString s) {
        return alnum.contains(s.toLower());
    }
    bool isString(QString s) {
        return alnumString.contains(s.toLower());
    }

    bool isAlpha(QString s) {
        return alpha.contains(s.toLower());
    }

    Token GetID(QString val) {
        for (Token& t: reservedWords)
            if (val.toUpper()==t.m_value) {
                t.m_lineNumber = Pmm::Data::d.lineNumber;
                return t;
            }

        //exit(1);
        return Token(TokenType::ID, val);
    }
};

#endif // SYNTAX_H
