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

#ifndef LIMAGECHARSETFIXEDCOLOR_H
#define LIMAGECHARSETFIXEDCOLOR_H

#include "source/LeLib/limage/charsetimage.h"


class LImageCharsetFixedColor : public CharsetImage
{
public:
    LImageCharsetFixedColor(LColorList::Type t);
    QByteArray m_colors;

    void SetColor(uchar col, uchar idx) override;

    void SaveBin(QFile& file) override;
    void LoadBin(QFile& file) override;

//    CharsetImage* getCharset() override { return this; }
//    unsigned int getPixel(int x, int y) override;

 //   void ImportBin(QFile& f) override;
   // void ExportBin(QFile& f) override;
  //  virtual void FromRaw(QByteArray& arr);
    //virtual void ToRaw(QByteArray& arr);
    //void ToQPixMaps(QVector<QPixmap>& map);
  //  virtual void setPixel(int x, int y, unsigned int color) override;


    //void RenderEffect(QMap<QString, float> params) override;

   // void CopyFrom(LImage* mc);
    //bool KeyPress(QKeyEvent *e) override;

    //void setLimitedPixel(int x, int y, unsigned int color);


};

#endif // LIMAGECHARSETFIXEDCOLOR_H
