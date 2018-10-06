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

#ifndef C64FullScreenChar_H
#define C64FullScreenChar_H

#include "multicolorimage.h"
#include "charsetimage.h"

class C64FullScreenChar: public MultiColorImage
{
public:
    QByteArray m_rawData, m_rawColors;
    bool m_isMultiColor = false;

    int m_charWidth = 40;
    int m_charHeight = 25;


    PixelChar m_color;
    C64FullScreenChar(LColorList::Type t);


//    bool m_charMode = true; // or colorMode


    void SaveBin(QFile &f) override;
    void LoadBin(QFile &f) override;

    void SetColor(uchar col, uchar idx) override;
    void Clear() override;
    void ImportBin(QFile& f) override;
    void ExportBin(QFile& f) override;
//    void FromRaw(QByteArray& arr);
//    void ToRaw(QByteArray& arr);
    void BuildImage();
    CharsetImage* getCharset() override { return m_charset; }

    void setPixel(int x, int y, unsigned int color) override;
    unsigned int getPixel(int x, int y) override;
    void CopyFrom(LImage* mc) override;
};

#endif // CHARSETIMAGE_H
