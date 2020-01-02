#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <QString>
#include <QByteArray>
#include <QFile>
#include "source/LeLib/limage/multicolorimage.h"
#include "source/LeLib/limage/limageqimage.h"
#include "source/LeLib/limage/charsetimage.h"
#include "source/LeLib/limage/c64fullscreenchar.h"
#include <QImage>
#include <math.h>
#include <QtMath>
#include "source/LeLib/limage/charsetimage.h"

class Compression
{
public:
    Compression();

    void SaveCharset(QString filename, QImage& img, int w, int h, int div);
    void AddScreen(QByteArray& data, QImage& img, int w, int h, char base, int div);
    void AddToDataX(QByteArray& data, MultiColorImage& img,int x, int y, int w, int h);
    void AddToDataVGA(QByteArray& data, LImageQImage& img,int x, int y, int w, int h);
    void AddBitplaneToData(QByteArray& data, MultiColorImage& img,int x, int y, int w, int h, int bpl);

    int CompareSprites(QByteArray& d1,QByteArray& d2, int sprite1, int sprite2);

    void OptimizeScreenAndCharset(QVector<int>& screen, QByteArray& charset, QVector<int> &sOut, QByteArray& cOut, int sw, int sh, int charSize, int compression);

    void SaveCompressedSpriteData(QByteArray& data, QString dataFile, QString tableFile, int address, int compressionLevel);

    int Compare(QByteArray& a, QByteArray& b, int p1, int p2, int length);

    void OptimizeAndPackCharsetData(QByteArray& dataIn, QByteArray& out, QByteArray& table, int width, int compression, bool invertTable);

    void ConvertToC64(bool dither);

    void AddCharsetScreen(QByteArray& data, QImage& img, CharsetImage* charset,int w, int h);

 //   void m_compression.AddCharsetScreen(ba, m_effect->m_img, charset, lua_tonumber(L,1),lua_tonumber(L,2));


    void AddPetsciiScreen(QByteArray& data, QImage& img);
    void AddBinaryScreen(QByteArray& data, QImage& img);

    void SaveCompressedTRM(QByteArray& data, QString file, int compression);
};

#endif // COMPRESSION_H
