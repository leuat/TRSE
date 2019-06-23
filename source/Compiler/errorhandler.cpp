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

#include "errorhandler.h"

ErrorHandler ErrorHandler::e;

ErrorHandler::ErrorHandler()
{
}

void ErrorHandler::Warning(QString str, int lineNumber) {
    QString v = "<font color=\"#F0E030\">Warning at line " + QString::number(lineNumber)+" : " +str + "</font>";
    m_teOut = m_teOut + v + "<br>";
    //        Message(, WARNING);
    //        throw FatalErrorException(str, lineNumber);
    //Message(str);

    //if (exitOnError)
    //    exit(1);
}
