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

#ifndef LIMAGESPRITES_H
#define LIMAGESPRITES_H

#include "charsetimage.h"
#include <QPixmap>
#include <QKeyEvent>


class LImageSprites : public CharsetImage
{
public:
    LImageSprites(LColorList::Type t) : CharsetImage(t) {
        m_type = LImage::Type::Sprites;
        m_charCount = 1024;
        m_currentMode = Mode::CHARSET1x1;
        m_currencChar=0;
        //Data::data.currentColor=0;

    }
  //  void ImportBin(QFile& f) override;
   // void ExportBin(QFile& f) override;
    void FromRaw(QByteArray& arr) override;
    void ToRaw(QByteArray& arr) override;


    QPoint getXY(int x, int y);

    void CopyPasteChar(int type);


    bool KeyPress(QKeyEvent *e) override;

    void setPixel(int x, int y, unsigned int color) override;
    unsigned int getPixel(int x, int y) override;
    void FlipVertical() override;
    void FlipHorizontal() override;

    void CopyChar();

    void PasteChar();


};


#endif // LIMAGESPRITES_H
