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

#ifndef MULTICOLORIMAGE_H
#define MULTICOLORIMAGE_H

#include <QVector>
#include "source/LeLib/limage/lcolorlist.h"
#include "source/LeLib/limage/limage.h"
#include <QImage>
#include <QByteArray>
//#include <source/effects/demoeffectraytracer.h>

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

class PixelChar {
public:
    PixelChar();
    unsigned char p[8];
    unsigned char c[4];
    unsigned char get(int x, int y, unsigned char bitMask);
    void set(int x, int y, unsigned char color, unsigned char bitMask, unsigned char maxCol, unsigned char minCol);
    void set(int x, int y, unsigned char color, unsigned char bitMask);
    void Clear(unsigned char background);
    QString bitmapToAssembler();
    QString colorMapToAssembler(int i, int j);
    uchar colorMapToNumber(int i, int j);
    QByteArray data();
    uchar flipSpriteBit(int i, int mask);
    QString colorToAssembler();
    QImage toQImage(int size, uchar bmask, LColorList& lst, int scale);
    bool isEmpty();
    bool isEqualBytes(PixelChar& o);
    void Reorganize(unsigned char bitMask, unsigned char Scale,unsigned char minCol, unsigned char maxCol, unsigned char bgCol);
    int Count(unsigned int col, unsigned char bitMask, unsigned char Scale);



    int Compare(PixelChar& other) {
        int l = 0;
        for (int i=0;i<8;i++)
            l+=other.p[i] != p[i];

/*        for (int i=0;i<4;i++)
            l+=other.c[i]!=c[i];
*/
        return l;

    }


    int CompareLength(PixelChar& other) {
        int l = 0;
        for (int i=0;i<8;i++)
            for (int j=0;j<4;j++) {
                char a = (p[i]>>(2*j))&0b11;
                char b = (other.p[i]>>(2*j))&0b11;
                if ( a != b )
                    l++;
                if (a==0 && b!=0) l++;
                if (b==0 && a!=0) l++;

            }
//            if (other.p[i]!=p[i])
  //          l+=other.p[i] != p[i];

        return l;

    }


    static uchar Swap(int a, int b, uchar c);
    static uchar VIC20Swap( uchar c);


    static unsigned char reverse(unsigned char b) {
       b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
       b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
       b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
       return b;
    }
};


/*/class CharPos {
public:
    char m_charIndex, m_colorIndex;
    uint m_x;
    uint m_y;

    CharPos() {}

    CharPos(uint x,uint y, char charIndex, char colorIndex) {
        m_charIndex = charIndex;
        m_x = x;
        m_y = y;
        m_colorIndex = colorIndex;
    }

};
*/







class MultiColorImage  : public LImage
{
public:
  //  (x shift, x count, data, x shift, x count, data)

    QVector<PixelChar> m_organized;
//    QVector<CharPos> m_charPos;
    QByteArray m_outputData;
    int m_Index = 0;
    int m_charWidth=40;
    int m_charHeight=25;

    MultiColorImage(LColorList::Type t);
    PixelChar m_data[40*25];
    PixelChar& getPixelChar(int x, int y);
    virtual void Clear() override;

    int LookUp(PixelChar pc);
    CharsetImage* m_charset = nullptr;
    virtual void setMultiColor(bool doSet) override;

    void CalculateCharIndices();
    int Eat(int start, int add);
    void SaveCharRascal(QString file, QString name);
    void LoadCharset(QString file) override;
    bool isMultiColor() override { return m_bitMask==0b11; }

//    unsigned char m_border=0, m_background=0;

    void setPixel(int x, int y, unsigned int color) override;
    unsigned int getPixel(int x, int y) override;

    void Reorganize();
    bool KeyPress(QKeyEvent *e) override;

    void SaveBin(QFile& f) override;
    void LoadBin(QFile& f) override;

    void ImportKoa(QFile& f) override;
    void ExportKoa(QFile& f) override;

    void FloydSteinbergDither(QImage& img, LColorList& colors, bool dither) override;

    void Initialize(int width, int height) override {}

    void setForeground(unsigned int col) override;
    void setBackground(unsigned int col) override;

    void ToQImage(LColorList& lst, QImage& img, float zoom, QPointF center) override;

    void Release() override {}
    virtual void RenderEffect(QMap<QString, float> params);

    void ApplyToLabel(QLabel* l) override {}

/*    void drawLine(float x0, float y0, float x1, float y1, unsigned int col, int size);

    void Box(int x, int y, unsigned char col, int size);
*/
//    void ToQImage(LColorList& lst, QImage* img, float zoom = 1, QPoint center = QPoint(160,100));
    void fromQImage(QImage* img, LColorList& lst) override;

    void CopyFrom(LImage* mc) override;

    void ForceColorFlattening();

  //  void ExportAsm(QString filename) override;
    void ExportBin(QFile& file) override;
    void ImportBin(QFile& file) override;

    void SetCharSize(int x, int y);

   void ExportCompressed(QString f1, QString f2) override;

   void CompressAndSave(QByteArray& chars, QByteArray& screen, int x0,int x1, int y0, int y1, int& noChars, int compression, int maxChars);

   void SetColor(uchar col, uchar idx) override;


};


#endif // MULTICOLORIMAGE_H
