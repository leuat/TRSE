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


    m_loadAddress = (unsigned char)(m_blob.at(8))<<8 | (unsigned char)(m_blob.at(8+1))<<0;
    m_initAddress = (unsigned char)(m_blob.at(0xa))<<8 | (unsigned char)(m_blob.at(0xa+1))<<0;
    m_playAddress = (unsigned char)(m_blob.at(0xc))<<8 | (unsigned char)(m_blob.at(0xc+1))<<0;


//    qDebug() << Util::numToHex(m_playAddress);
  //  qDebug() << Util::numToHex(m_playAddress);

    file.close();
}

void SidFile::LoadNSF(QString filename, QString path)
{
    m_path = path;
    m_fileName = filename;
    if (!filename.endsWith(".nsf"))
        ErrorHandler::e.Error("Unable to load '" + filename +"', must be NSF file!");
    QFile file(path + filename);

    if (!QFile::exists(path + filename))
        ErrorHandler::e.Error("Unable to locate '" + filename +"'");

    file.open(QIODevice::ReadOnly);
    m_blob = file.readAll();
    //qDebug() << "sid file size: " << m_blob.count();


    if (!(m_blob.at(0)=='N' && m_blob.at(1)=='E' && m_blob.at(2)=='S' && m_blob.at(3)=='M'))
        ErrorHandler::e.Error("File '" + filename +"' not identified as a NSF file");

    uchar u = m_blob.at(9);
    m_loadAddress = (u&0xff)<<8 | ((uchar)m_blob.at(8))<<0;
    m_initAddress = (uchar)(m_blob.at(0xb))<<8 | ((uchar)m_blob.at(0xa))<<0;
    m_playAddress = (uchar)(m_blob.at(0xd))<<8 | ((uchar)m_blob.at(0xc))<<0;


/*    m_loadAddress+=10;
    m_initAddress+=10;
    m_playAddress+=10;
*/
/*    qDebug() << "MUSIC data: " << Util::numToHex(u);
    qDebug() << "Load address:" << Util::numToHex(m_loadAddress);
    qDebug() << "Init address:" << Util::numToHex(m_initAddress);
    qDebug() << "Play address:" << Util::numToHex(m_playAddress);
*/

  /*  qDebug() << Util::numToHex(m_loadAddress);
    qDebug() << Util::numToHex(m_initAddress);
    qDebug() << Util::numToHex(m_playAddress);*/
//    exit(1);
    file.close();
}

void SidFile::Convert(int headerShift, int newAddress, QString fileEnding, int hsz, bool isOK64)
{
    QString org = m_fileName;
    m_outFile = m_fileName.remove("."+fileEnding);
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
 //   int headerSize = 0x7C + headerShift;
    int headerSize = hsz + headerShift;
    // Should the +2 be included?
    m_blob.remove(0,headerSize);

    // Replace all d400 with newaddress
    //if (org.toLower().trimmed().endsWith(".vsid"))
    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::OK64)
    {
//        qDebug() << "VSID CONVERSION: ";
        for (int i=0;i<m_blob.count()-1;i++) {
            uchar lo = m_blob[i];
            uchar hi = m_blob[i+1];
            if (hi==0xD4) {
//                lo=lo&31;
                if (lo<0x1F) {
                    int addr = 0xFF20+lo;
//                    qDebug() << "SID Moving: " << Util::numToHex((unsigned short)((hi<<8)|lo)) << " to " << Util::numToHex(addr);
                    m_blob[i+1]=(uchar)((addr>>8));
                    m_blob[i]=(uchar)((addr));
  //                  qDebug() << "SID Moved to: " << Util::numToHex((unsigned short)((m_blob[i+1]<<8)|m_blob[i])) << " to " << Util::numToHex(addr);
                }
                 else {
                //    qDebug() << "IG:" << Util::numToHex((hi));
                  //  qDebug() << "IGNORING : " << Util::numToHex((unsigned short)((hi<<8)|lo));
                }
            }
        }


    }


    file.write(m_blob);
    file.close();

}
