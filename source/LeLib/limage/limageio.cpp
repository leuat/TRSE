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

#include "limageio.h"


LImageIO::LImageIO()
{

}

QString LImageIO::m_fileExtension = "flf";
QString LImageIO::m_ID = "FLUFF64";

bool LImageIO::Save(QString filename, LImage* img)
{
    QString headerID = LImageIO::m_ID;// + m_version;
    QByteArray array = headerID.toLocal8Bit();
    char* header = array.data();
    unsigned char version[sizeof(int)];
    memcpy(version, &Data::data.flfVersion, sizeof(int));

    QFile file( filename);
    if( !file.open( QFile::WriteOnly ) )
        return false;

    unsigned char imageType = LImage::TypeToChar(img->m_type);

    unsigned char colorType = LColorList::TypeToChar(img->m_colorList.m_type);

    // 7 + 4 + 1 + 1 = 13

    file.write( ( char * )( header ), array.count() );
    file.write( ( char * )( version ),sizeof(float) );
    file.write( ( char * )( &imageType ),1 );
    file.write( ( char * )( &colorType ),1 );
    img->SaveBin(file);
    img->m_colorList.PenToFooter(&img->m_footer);
    img->m_footer.Save(file);
    file.close();
    return true;
}

LImage* LImageIO::Load(QString filename)
{

//    return LImageFactory::Create(LImage::HiresBitmap,LColorList::C64);

    QString headerID = "fluff64";// + m_version;
    QByteArray array = headerID.toLocal8Bit();
    char* header = array.data();
    //memcpy(Data::data.version, &f, sizeof(f));

    QFile file( filename);
    if( !file.open( QFile::ReadOnly ) )
        return nullptr;


    int version;
    unsigned char imageType;
    unsigned char paletteType;

    file.read( ( char * )( header ), array.count() );
    file.read( ( char * )( &version ),sizeof(int) );
    file.read( ( char * )( &imageType ),1);
    file.read( ( char * )( &paletteType ),1);


    LImage* img = LImageFactory::Create(LImage::CharToType(imageType), LColorList::CharToType(paletteType));
    if (img==nullptr)
        return nullptr;

    img->LoadBin(file);
    img->m_footer.Load(file);
    img->m_colorList.FooterToPen(&img->m_footer);
    img->InitAfterFooter();
    file.close();
    return img;
}

