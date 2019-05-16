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
    enum Type {STRING, NUMBER, ADDRESS, BYTE, INTEGER, PROCEDURE};
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
  //  enum System {C64, VIC20, PET, NES, C128, BBCM, AMIGA};
    QString m_syntaxData; // File syntax data
    AbstractSystem::System m_currentSystem;
    int m_startAddress = 0x800;
    int m_programStartAddress = m_startAddress+10;
    int m_memoryType = 0;
    bool m_ignoreSys = false;
    bool m_stripPrg = false;
    Syntax();
    void Init(AbstractSystem::System s,QString p);
    void SetupReservedWords();
    void SetupBuiltinFunctions(AbstractSystem::System s);
    void SetupKeys();
    void LoadSyntaxData();

/*    static System SystemFromString(QString s) {
        if (s.toLower()=="c64")
            return C64;
        if (s.toLower()=="c128")
            return C128;
        if (s.toLower()=="pet")
            return PET;
        if (s.toLower()=="vic20")
            return VIC20;
        if (s.toLower()=="nes")
            return NES;
        if (s.toLower()=="bbcm")
            return BBCM;
        if (s.toLower()=="amiga")
            return AMIGA;
    }

    static QString StringFromSystem(System s) {
        if (s == C64) return "C64";
        if (s == PET) return "PET";
        if (s == VIC20) return "VIC20";
        if (s == NES) return "NES";
        if (s == C128) return "C128";
        if (s == BBCM) return "BBCM";
        if (s == AMIGA) return "AMIGA";
    }
*/
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
