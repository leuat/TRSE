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

#ifndef SIDFILE_H
#define SIDFILE_H

#include <QString>
#include <QFile>
#include <QByteArray>
#include "source/Compiler/errorhandler.h"
#include "source/LeLib/util/util.h"
#include "source/Compiler/syntax.h"

class SidFile
{
public:
    SidFile();
    QByteArray m_blob, m_stripped;
    unsigned short m_loadAddress, m_initAddress, m_playAddress;
    QString m_outFile, m_fileName, m_path;

    void Load(QString filename, QString path);
    void Convert(int headerShift, int newAddress);
};

#endif // SID_H

