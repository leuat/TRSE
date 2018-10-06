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

#include "lmessage.h"
#include <QDateTime>

LMessage LMessage::lMessage;

LMessage::LMessage(int max)
{
    m_max = max;
}

LMessage::LMessage() {

    m_max = 15;
}

void LMessage::Maintain()
{
    if (m_messages.length()>m_max)
        m_messages.removeFirst();
}

void LMessage::Message(QString s)
{
    m_messages.append(QDateTime::currentDateTime().toString() + " : " +  s);
    Maintain();
}

void LMessage::Error(QString s)
{
    m_messages.append(QDateTime::currentDateTime().toString() + " : <font color=\"red\">" +  s + "</font>");
    Maintain();

}



QString LMessage::Build()
{
    QString m = "";
    for (QString s : m_messages) {
        m = m + s + "<br>";
    }
    return m;
}
