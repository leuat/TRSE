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

#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H


#include <QString>
#include <QDebug>
#include "source/Compiler/data_pmm.h"
#include <stdio.h>
//#include "source/Compiler/ast/node.h"
#include <iostream>
//#include <QTextEdit>
#include <exception>
using namespace std;

class Lexer;


class FatalErrorException: public exception
{
public:
    QString message;
    QString file = "";
    int linenr;
    FatalErrorException() {}
    FatalErrorException(QString msg)
    {
        message = msg;
    }
    FatalErrorException(QString msg, int ln)
    {
        message = msg;
        linenr = ln;
    }
};

class ErrorHandler
{
public:
    ErrorHandler();
    static ErrorHandler e;
    int m_level = 0;

    int ERROR_ONLY = 3;
    int WARNING = 2;
    int DEBUG_HIGH = 1;
    int DEBUG_LOW = 0;

    QString m_teOut;
    QStringList m_warnings;
    bool m_displayWarnings = true;
    Lexer* m_lexer = nullptr;

    void OutputText(QString str) {
        std::cout << str.toStdString() << std::endl;
        m_teOut = m_teOut + str + "\n";
    }

    void Message(QString str, int lvl=0) {
        QString s ="";
        for (int i=0;i<lvl;i++)
            s+=" ";

        s = s + str + "\n";
        //std::cout << s.toStdString();
        m_teOut = m_teOut + s;

    }

    void DebugHigh(QString str, int lvl=0) {
        if (m_level<=DEBUG_HIGH)
            Message(str, lvl);
    }

    void DebugLow(QString str, int lvl=0) {
        if (m_level<=DEBUG_LOW)
            Message(str, lvl);
    }
    bool exitOnError= true;

    void ClearWarnings() {
        m_warnings.clear();
    }

    void Error(QString str, int lineNumber=0) {
        throw FatalErrorException(str, lineNumber);
        //Message(str);

        //if (exitOnError)
        //    exit(1);
    }


    void Warning(QString str, int lineNumber=0);

    void CatchError(FatalErrorException e, QString extraMessage="") {
        Message(extraMessage + e.message);
        if (exitOnError)
            exit(1);
    }





};

#endif // ERRORHANDLER_H
