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

#include "sidfile.h"
#include <QDebug>
SidFile::SidFile()
{

}

void SidFile::Load(QString filename, QString path)
{
    m_path = path;
    m_fileName = filename;
    if (!filename.endsWith(".sid"))
        ErrorHandler::e.Error("Unable to load '" + filename +"', must be sid file!");
    QFile file(path + filename);

    if (!QFile::exists(path + filename))
        ErrorHandler::e.Error("Unable to locate '" + filename +"'");

    file.open(QIODevice::ReadOnly);
    m_blob = file.readAll();
    //qDebug() << "sid file size: " << m_blob.count();


    if (!(m_blob.at(0)=='P' && m_blob.at(1)=='S' && m_blob.at(2)=='I' && m_blob.at(3)=='D'))
        ErrorHandler::e.Error("File '" + filename +"' not identified as a SID file");

    m_loadAddress = m_blob.at(8)<<8 | m_blob.at(8+1)<<0;
    m_initAddress = m_blob.at(0xa)<<8 | m_blob.at(0xa+1)<<0;
    m_playAddress = m_blob.at(0xc)<<8 | m_blob.at(0xc+1)<<0;


    file.close();
}

void SidFile::Convert(int headerShift)
{
    m_outFile = m_fileName.remove(".sid");
    QStringList l = m_fileName.split("/");
    QString last = l.last();
//    qDebug() << "last: " << last;
    l.removeLast();


    m_outFile = "_" + last + ".dat";
    QString f="";
    for (QString& s : l)
        f+=s+"/";
    m_outFile=f+m_outFile;


    if (QFile::exists(m_path + m_outFile))
        QFile::remove(m_path + m_outFile);

    QFile file(m_path + m_outFile);
    file.open(QIODevice::WriteOnly);
    int headerSize = 0x7C + headerShift;
    // Should the +2 be included? FUCK!
    m_blob.remove(0,headerSize);
    file.write(m_blob);
    file.close();

}
