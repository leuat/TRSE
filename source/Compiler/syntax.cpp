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

#include "syntax.h"
#include <QtMath>


Syntax Syntax::s;

Syntax::Syntax()
{
}

void Syntax::Init(System s, QString param)
{
    m_currentSystem = s;
    if (s==VIC20) {
        if (param=="none") {
            m_startAddress = 0x1000;
            m_programStartAddress = 0x1010;
            m_memoryType = 0;
        }
        else
        {
            m_startAddress = 0x1200;
            m_programStartAddress = 0x1210;
            m_memoryType = 1;

       }

    }
    if (s==C64) {
        m_startAddress = 0x0800;
        m_programStartAddress = 0x0810;
    }
    if (s==NES) {
        m_startAddress = 0x8000;
        m_programStartAddress = 0x8000;
    }
    if (s==C128) {
        m_startAddress = 0x01C00;
        m_programStartAddress = 0x1C10;
    }
    if (s==BBCM) {
        m_startAddress = 0x02000;
        m_programStartAddress = 0x2010;
    }

    LoadSyntaxData();

    SetupReservedWords();
    SetupBuiltinFunctions(s);
    SetupKeys();

}





void Syntax::SetupReservedWords()
{
    reservedWords.clear();
    QString currentSystem = StringFromSystem(m_currentSystem).toLower();
    for (QString s: m_syntaxData.split('\n')) {
        s= s.simplified();
        if (s.count()==0) continue;
        if (s.startsWith("#")) continue;
        s=s.replace(" ", "");

        QStringList data = s.split(";");
        if (data[0].toLower()!="r")
            continue;
        QString word = data[1].toLower();
        QString system = data[2].toLower();

        if (system.contains(currentSystem)) {

            reservedWords.append(Token(TokenType::getType(word), word.toUpper()));
        }

     }

}

void Syntax::SetupBuiltinFunctions(System system)
{
    builtInFunctions.clear();



    QString currentSystem = StringFromSystem(m_currentSystem).toLower();

    for (QString s: m_syntaxData.split('\n')) {
        s= s.simplified();
        if (s.count()==0) continue;
        if (s.startsWith("#")) continue;
        s=s.replace(" ", "");

        QStringList data = s.split(";");
        if (data[0].toLower()!="m")
            continue;
        QString method = data[1].toLower();
        QString system = data[2].toLower();
        QStringList params = data[3].toLower().split(",");

        // Build parameter list
        QList<BuiltInFunction::Type> paramList;
        for (QString p: params) {
            if (p=="a")
                paramList << BuiltInFunction::Type::ADDRESS;
            if (p=="b")
                paramList << BuiltInFunction::Type::BYTE;
            if (p=="n")
                paramList << BuiltInFunction::Type::NUMBER;
            if (p=="i")
                paramList << BuiltInFunction::Type::INTEGER;
            if (p=="ib")
                paramList << BuiltInFunction::Type::INTEGER;
            if (p=="s")
                paramList << BuiltInFunction::Type::STRING;
            if (p=="p")
                paramList << BuiltInFunction::Type::PROCEDURE;
        }
        if (system.contains(currentSystem)) {
            builtInFunctions[method] = BuiltInFunction(method, paramList);
        }


    }


}

void Syntax::SetupKeys()
{
    int row[8] = {0b11111110,0b11111101,0b11111011,0b11110111,0b11101111,0b11011111,0b10111111,0b01111111};
    int column[8] = {0b00000001,0b00000010,0b00000100,0b00001000,0b00010000,0b00100000,0b01000000,0b10000000};
    m_c64keys.clear();
    m_c64keys[0x01] = C64Key("A","KEY_A",0x01 , row[1], column[2]);
    m_c64keys[0x02] = C64Key("B","KEY_B",0x02 , row[3], column[4]);
    m_c64keys[0x03] = C64Key("C","KEY_C",0x03 , row[2], column[4]);
    m_c64keys[0x04] = C64Key("D","KEY_D",0x04 , row[2], column[2]);
    m_c64keys[0x05] = C64Key("E","KEY_E",0x05 , row[1], column[6]);
    m_c64keys[0x06] = C64Key("F","KEY_F",0x06 , row[2], column[5]);
    m_c64keys[0x07] = C64Key("G","KEY_G",0x07 , row[3], column[2]);
    m_c64keys[0x08] = C64Key("H","KEY_H",0x08 , row[3], column[5]);
    m_c64keys[0x09] = C64Key("I","KEY_I",0x09 , row[4], column[1]);
    m_c64keys[0x0A] = C64Key("J","KEY_J",0x0A , row[4], column[2]);
    m_c64keys[0x0B] = C64Key("K","KEY_K",0x0B , row[4], column[5]);
    m_c64keys[0x0C] = C64Key("L","KEY_L",0x0C , row[5], column[2]);
    m_c64keys[0x0D] = C64Key("M","KEY_M",0x0D , row[4], column[4]);
    m_c64keys[0x0E] = C64Key("N","KEY_N",0x0E , row[4], column[7]);
    m_c64keys[0x0F] = C64Key("O","KEY_O",0x0F , row[4], column[6]);
    m_c64keys[0x10] = C64Key("P","KEY_P",0x10 , row[5], column[1]);
    m_c64keys[0x11] = C64Key("Q","KEY_Q",0x11 , row[7], column[6]);
    m_c64keys[0x12] = C64Key("R","KEY_R",0x12 , row[2], column[1]);
    m_c64keys[0x13] = C64Key("S","KEY_S",0x13 , row[1], column[5]);
    m_c64keys[0x14] = C64Key("T","KEY_T",0x14 , row[2], column[6]);
    m_c64keys[0x15] = C64Key("U","KEY_U",0x15 , row[3], column[6]);
    m_c64keys[0x16] = C64Key("V","KEY_V",0x16 , row[3], column[7]);
    m_c64keys[0x17] = C64Key("W","KEY_W",0x17 , row[1], column[1]);
    m_c64keys[0x18] = C64Key("X","KEY_X",0x18 , row[2], column[7]);
    m_c64keys[0x19] = C64Key("Y","KEY_Y",0x19 , row[3], column[1]);
    m_c64keys[0x1A] = C64Key("Z","KEY_Z",0x1A , row[1], column[4]);
    m_c64keys[0x30] = C64Key("0","KEY_0",0x30 , row[4], column[3]);
    m_c64keys[0x31] = C64Key("1","KEY_1",0x31 , row[7], column[0]);
    m_c64keys[0x32] = C64Key("2","KEY_2",0x32 , row[7], column[3]);
    m_c64keys[0x33] = C64Key("3","KEY_3",0x33 , row[1], column[0]);
    m_c64keys[0x34] = C64Key("4","KEY_4",0x34 , row[1], column[3]);
    m_c64keys[0x35] = C64Key("5","KEY_5",0x35 , row[2], column[0]);
    m_c64keys[0x36] = C64Key("6","KEY_6",0x36 , row[2], column[3]);
    m_c64keys[0x37] = C64Key("7","KEY_7",0x37 , row[3], column[0]);
    m_c64keys[0x38] = C64Key("8","KEY_8",0x38 , row[3], column[3]);
    m_c64keys[0x39] = C64Key("9","KEY_9",0x39 , row[4], column[0]);

    m_c64keys[0xF1] = C64Key("F1","KEY_F1",0xF1 , row[0], column[4]);
    m_c64keys[0xF3] = C64Key("F3","KEY_F3",0xF3 , row[0], column[5]);
    m_c64keys[0xF5] = C64Key("F5","KEY_F5",0xF5 , row[0], column[6]);
    m_c64keys[0xF7] = C64Key("F7","KEY_F7",0xF7 , row[0], column[3]);

    m_c64keys[0x2b] = C64Key("+","KEY_PLUS",0x2b , row[5], column[0]);
    m_c64keys[0x1c] = C64Key("£","KEY_POUND",0x1c , row[6], column[0]);
    m_c64keys[0xF0] = C64Key("ENTER","KEY_ENTER",0xF0 , row[0], column[1]);
    m_c64keys[0x2a] = C64Key("*","KEY_ASTERIX",0x2a , row[6], column[1]);
    m_c64keys[0x3b] = C64Key(";","KEY_SEMI",0x3b , row[6], column[2]);
    m_c64keys[0xEF] = C64Key("HOME","KEY_HOME",0xEF , row[6], column[3]);
    m_c64keys[0x20] = C64Key("SPACE","KEY_SPACE",0x20 , row[7], column[4]);
    m_c64keys[0xEE] = C64Key("COMMODORE","KEY_COMMODORE",0xEE , row[7], column[5]);
    m_c64keys[0x3a] = C64Key(":","KEY_COLON",0x3a , row[5], column[5]);
    m_c64keys[0x3d] = C64Key("=","KEY_EQUALS",0x3d , row[6], column[5]);

}

void Syntax::LoadSyntaxData()
{
    QFile file(":resources/text/syntax.txt");
    file.open(QFile::ReadOnly | QFile::Text);
    m_syntaxData = file.readAll();
    file.close();


}

bool Syntax::isNumeric(QString s) {
    bool ok;
    int dec = s.toInt(&ok, 10);
    return ok;
}

bool Syntax::isDigit(QString s) {
    // Check if HEX
    return digit.contains(s);

}

bool Syntax::isDigitHex(QString s) {
    // Check if HEX
    return digitAll.contains(s);

}

bool Syntax::isAlnum(QString s) {
    return alnum.contains(s.toLower());
}

bool Syntax::isString(QString s) {
    return alnumString.contains(s.toLower());
}

bool Syntax::isAlpha(QString s) {
    return alpha.contains(s.toLower());
}

Token Syntax::GetID(QString val) {
    for (Token& t: reservedWords)
        if (val.toUpper()==t.m_value) {
            t.m_lineNumber = Pmm::Data::d.lineNumber;
            return t;
        }

    //exit(1);
    return Token(TokenType::ID, val);
}


