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

#ifndef MESSAGES_H
#define MESSAGES_H

#include <QString>
#include <QStringList>
#include <QMap>
#include "source/LeLib/util/cinifile.h"
#include "source/dialogmessage.h"
#include <QPixmap>
#include <QImage>

class Message {
public:
    QString m_message;
    int m_id;
    bool m_ignore;

    enum Severity {Msg, Warning, Error};
    Severity m_severity;

    Message() {}

    Message(int id, Severity sev, QString msg) {
        m_message = msg;
        m_id = id;
        m_ignore = false;
        m_severity = sev;
    }

};


class Messages
{
public:
    static Messages messages;
    Messages();

    int DASM_COMPILER_ERROR = 0;
    int MEMORY_OVERLAP_ERROR = 1;
    int NO_DASM = 2;
    int NO_EMULATOR = 3;
    int NO_PROJECT = 4;
    int ALPHA_WARNING = 5;
    int CHARSET_WARNING = 6;
    int BRANCH_ERROR = 7;
    int NO_EXOMIZER = 8;
    int OLD_SPRITE_FILE = 9;

    QMap<int, Message> m_messages;
    CIniFile* m_iniFile;
    void LoadFromCIni(CIniFile* file);
    void InitMessages();

    void DisplayMessage(int i);


};

#endif // MESSAGES_H
