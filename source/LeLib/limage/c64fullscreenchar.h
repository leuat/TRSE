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
#include "limagecontainer.h"
#include "movieconverter.h"
class C64Screen : public LImageContainerItem {
public:
    QByteArray m_rawData, m_rawColors, m_data;

    void Init(int w, int h)  override {
        m_width = w;
        m_height = h;
        m_rawColors.resize(w*h);
        m_rawData.resize(w*h);
        m_data.resize(16);
        Clear();
    }

    QByteArray ToQByteArray(int b) override {
        QByteArray a;
        a.append(m_rawColors);
        a.append(m_rawData);
        a.append(m_data);
        return a;

    }

    void Clear() override {
    for (int i=0;i<m_rawData.count();i++)
        m_rawData[i] = 0x20;

    for (int i=0;i<m_rawData.count();i++)
        m_rawColors[i] = 0x2;
    }

};


class C64FullScreenChar: public MultiColorImage, public LImageContainer
{
public:


//    QByteArray m_rawData, m_rawColors
  //  bool m_isMultiColor = false;

/*    int m_charWidth = 40;
    int m_charHeight = 25;
*/

    PixelChar m_color;
    C64Screen m_copy;


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
                QString::number(m_items.count()) ;
        return curChar +  screen;
    }



    //    virtual void setMultiColor(bool doSet) override {}

    CharsetImage* getCharset() override { return m_charset; }
    virtual int getContainerCount() override {return m_items.count();}


    bool KeyPress(QKeyEvent *e) override;
    void CopyChar();
    void PasteChar();

    void AddNew(int w, int h) override;

    /*void Prev() override {
        if (m_current>0)
        m_current--;
    }

    void Next() override {
        if (m_current<m_items.count()-1)
        m_current++;
    }
    void Delete() override;

*/
    void setPixel(int x, int y, unsigned int color) override;
    unsigned int getPixel(int x, int y) override;

    void CopyFrom(LImage* mc) override;


    void ExportMovie(QFile &file) override;


};


#endif // CHARSETIMAGE_H
