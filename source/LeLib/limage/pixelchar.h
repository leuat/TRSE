#ifndef PIXELCHAR_H
#define PIXELCHAR_H

#include <QString>
#include <QImage>
#include <QDebug>
#include "lcolorlist.h"
#include "ssim.h"

class PixelChar {// : public SSIM {
public:
    PixelChar();
    unsigned char p[8];
    unsigned char c[4];
    static int m_lastBitmask;
    unsigned char get(int x, int y, unsigned char bitMask);
    void set(int x, int y, unsigned char color, unsigned char bitMask, unsigned char maxCol, unsigned char minCol, int forceD800Col=-1);
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
    void Reorganize(unsigned char bitMask, unsigned char Scale,unsigned char minCol, unsigned char maxCol, unsigned char bgCol, unsigned char forceD800Color);
    int Count(unsigned int col, unsigned char bitMask, unsigned char Scale);

    void ForceBackgroundColor(int col, int swapcol);
    void swapMCBits(uchar b1, uchar b2);

    bool isPure();

    int Compare(PixelChar& other);
    void Invert();

    int CompareLength(PixelChar& other);

    int CompareLength2(PixelChar& other);

    int CompareLength3(PixelChar& other);
    double CompareLengthSSIM(PixelChar& other);

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


class PixelCharSSIM : public PixelChar, public SSIM {


        double  getWidth() override {
            if (m_lastBitmask==1)
                return 8;
            return 4;
        }
        double  getHeight() override {
            return 8;
        }

        double  getL() override {
            return 4;
        }

        double  getVal(int x, int y) override;


};

#endif // PIXELCHAR_H
