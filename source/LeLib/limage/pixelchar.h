#ifndef PIXELCHAR_H
#define PIXELCHAR_H

#include <QString>
#include <QImage>
#include <QDebug>
#include "lcolorlist.h"

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

#endif // PIXELCHAR_H
