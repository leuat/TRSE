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
#include <QKeyEvent>

class C64Screen {
public:
    QByteArray m_rawData, m_rawColors, m_data;

    void Init(int x, int y, int d) {
        m_rawColors.resize(x*y);
        m_rawData.resize(x*y);
        m_data.resize(d);
        Clear();
    }

    void Clear() {
    for (int i=0;i<m_rawData.count();i++)
        m_rawData[i] = 0x20;

    for (int i=0;i<m_rawData.count();i++)
        m_rawColors[i] = 0x2;
    }

};


class C64FullScreenChar: public MultiColorImage
{
public:

    QVector<C64Screen> m_screens;

//    QByteArray m_rawData, m_rawColors;
  //  bool m_isMultiColor = false;

/*    int m_charWidth = 40;
    int m_charHeight = 25;
*/

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
    QString GetCurrentDataString() override {
        if (m_current<0) return "";
        QString curChar =  "  Character : " + Util::numToHex(m_currencChar);
        QString screen = "  Screen : " + QString::number(m_current) + "/" +
                QString::number(m_screens.count()) ;
        return curChar +  screen;
    }



    //    virtual void setMultiColor(bool doSet) override {}

    CharsetImage* getCharset() override { return m_charset; }
    virtual int getContainerCount() override {return m_screens.count();}


    bool KeyPress(QKeyEvent *e) override;

    void AddNew(int, int) override {
        C64Screen s;
        s.Init(m_charWidth,m_charHeight, 16);
        m_screens.append(s);
        m_current = m_screens.count()-1;
    }

    void Prev() override {
        if (m_current>0)
        m_current--;
    }

    void Next() override {
        if (m_current<m_screens.count()-1)
        m_current++;
    }

    void setPixel(int x, int y, unsigned int color) override;
    unsigned int getPixel(int x, int y) override;
    void CopyFrom(LImage* mc) override;
    void Delete() override;


};


#endif // CHARSETIMAGE_H
