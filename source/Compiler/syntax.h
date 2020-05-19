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
#include "source/Compiler/systems/abstractsystem.h"

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
    enum Type {STRING, NUMBER, ADDRESS, BYTE, INTEGER, PROCEDURE, LONG, IGNOREPARAM};
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

// Singleton
class Syntax
{
private:
    Syntax();
public:
    QVector<Token> reservedWords, reservedWordsFjong;
    QStringList m_illegaVariableNames;
    //QVector<BuiltInFunction> builtinFunctions;
    QMap<QString, BuiltInFunction> builtInFunctions, builtinFunctionsFjong;
  //  enum System {C64, VIC20, PET, NES, C128, BBCM, AMIGA};
    QString m_syntaxData; // File syntax data
    QString m_systemString;

    QMap<QString, bool> m_reservedWordsRegularFont;

    QSharedPointer<AbstractSystem> m_currentSystem;
    bool m_ignoreSys = false;
    bool m_stripPrg = false;
    void Init(AbstractSystem::System s, QSharedPointer<CIniFile> m_ini, QSharedPointer<CIniFile> m_proj);
    void SetupReservedWords(QVector<Token>& list, QString id, bool ignoreSystem);
    void SetupIllegalVariables();
    void SetupBuiltinFunctions(QMap<QString, BuiltInFunction>& lst, AbstractSystem::System s, QString id, bool ignoreSystem);
    void SetupKeys();
    void LoadSyntaxData();

    QString puredigit = "0123456789^";
    QString digit = "^0123456789$%";
    QString digitAll = "^0123456789$%ABCDEFabcdef";
    QString alpha = "abcdefghijklmnopqrstuvwxyz_";
    QString alnum =alpha+digit;
    QString alnumString =alpha+digit+ " ;:æøå!#¤%&/()=.,-+*";
    QString binop = digitAll + " +-*/";
    uint lineNumber = 0;
    void SetupConstants();


    QMap<unsigned char, C64Key> m_c64keys;

    static Syntax s;

    bool isNumeric(QString s);

    bool isDigit(QString s);
    bool isDigitHex(QString s);
    bool isAlnum(QString s);
    bool isString(QString s);

    bool isAlpha(QString s);

    Token GetID(QString val);
};

#endif // SYNTAX_H
