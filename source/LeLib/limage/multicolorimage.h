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
#include <QMatrix4x4>
#include "source/LeLib/limage/lcolorlist.h"
#include "source/LeLib/limage/limage.h"
#include <QImage>
#include <QtMath>
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

    void ForceBackgroundColor(int col, int swapcol);

    int Compare(PixelChar& other);
    void Invert();

    int CompareLength(PixelChar& other);

    int CompareLength2(PixelChar& other);

    int CompareLength3(PixelChar& other);

    static uchar SwapColor(uchar data, uchar c1, uchar c2);


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
//    QVector<PixelChar> m_data;
    PixelChar m_data[40*25*2];
 //   QVector<PixelChar> m_data;
    virtual PixelChar& getPixelChar(int x, int y);
//    virtual void Clear() override;

    int LookUp(PixelChar pc);
    QString m_charsetFilename ="";

    CharsetImage* m_charset = nullptr;
    virtual void setMultiColor(bool doSet) override;
    void ForceBackgroundColor(int col, int swapCol);


    void AppendSaveBinCharsetFilename(QFile& file);
    void LoadBinCharsetFilename(QFile& file);

    void SwapChars(int p1, int p2);

    void UpdateColorList();

    void Invert() override;


    void CalculateCharIndices();
    int Eat(int start, int add);
    void SaveCharRascal(QString file, QString name);
    void LoadCharset(QString file, int skipBttes) override;
    bool isMultiColor() override { return m_bitMask==0b11; }

    virtual int getCharAtPos(QPoint p, float zoom, QPointF center) override;


    void onFocus() override;

//    unsigned char m_border=0, m_background=0;

    void setPixel(int x, int y, unsigned int color) override;
    unsigned int getPixel(int x, int y) override;

    void Reorganize();
    virtual bool KeyPress(QKeyEvent *e) override;

    void SaveBin(QFile& f) override;
    void LoadBin(QFile& f) override;


    virtual int charWidthDisplay() {
        return m_charWidth;
    }

    void ImportKoa(QFile& f) override;
    void ExportKoa(QFile& f) override;

    virtual QString getMetaInfo() override;


    void FloydSteinbergDither(QImage& img, LColorList& colors, bool dither) override;
    void OrdererdDither(QImage& img, LColorList& colors, QVector3D strength, int size, float gamma) override;

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
    void ForceExportBin(QFile& file);
    void ImportBin(QFile& file) override;
    void VBMExport(QFile& file, int p1, int p2, int p3, int p4) override;

    void VBMExportChunk(QFile& file, int p1, int p2, int p3, int p4) override;

    void SetCharSize(int x, int y);

   void ExportCompressed(QString f1, QString f2) override;

   virtual void CompressAndSave(QByteArray& chars, QVector<int>& screen, int x0,int x1, int y0, int y1, int& noChars, int compression, int maxChars);

   void SetColor(uchar col, uchar idx) override;


};


#endif // MULTICOLORIMAGE_H
