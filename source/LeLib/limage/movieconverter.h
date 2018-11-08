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

#ifndef MOVIECONVERTER_H
#define MOVIECONVERTER_H

#include <QString>
#include <QImage>
#include <QDir>
class MovieConverter
{
public:
    MovieConverter();
    uchar SKIP = 255;
    uchar END = 254;

    uchar MSKIP = 0b10000000;
    uchar MEND = 0b01000000;


    QByteArray ConvertImage(QImage& img, int w, int h, float zoom);
    void ConvertRaw(QString dir, QString baseName,  QString fileExtension,QString outFile,int frameSkip, int maxFrames, int width, int height, float zoom);
    void ConvertPacked(QString dir, QString baseName,  QString fileExtension,QString outFile,int frameSkip, int maxFrames, int width, int height, float zoom);
    QByteArray CompressImage(QByteArray prevFrame, QByteArray newFrame, int w, int h, float& compr);


    QByteArray CompressScreen(QByteArray prevFrame, QByteArray newFrame, int w, int h, float & compr);
    QByteArray CompressScreen2(QByteArray prevFrame, QByteArray newFrame, int w, int h, float & compr);
    QByteArray CompressScreen3(QByteArray prevFrame, QByteArray newFrame, int w, int h, float & compr, char, char);

};

#endif // MOVIECONVERTER_H
