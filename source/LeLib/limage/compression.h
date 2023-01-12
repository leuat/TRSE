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
#include "source/LeLib/limage/limageio.h"

class Compression
{
public:
    Compression();

    static const int TYPE_REGULAR = 0;
    static const int TYPE_SSIM = 1;

    void SaveCharset(QString filename, QImage& img, int w, int h, int div);
    void AddScreen(QByteArray& data, QImage& img, int w, int h, char base, int div);
    void AddToDataX(QByteArray& data, MultiColorImage& img,int x, int y, int w, int h);
    void AddToVZ200Data(QByteArray& data, LImage& img,int x, int y, int w, int h);
    void AddTo4PixelData(QByteArray& data, LImage& img,int x, int y, int w, int h);
    void AddSpecialC64bitmapModeToData(QByteArray& data, LImage& img,int x, int y, int w, int h);
    void AddToDataBBCMode5(QByteArray& data, LImage* img,int x, int y, int w, int h);
    void AddToDataVGA(QByteArray& data, LImageQImage& img,int x, int y, int w, int h);
    void AddBitplaneToData(QByteArray& data, MultiColorImage& img,int x, int y, int w, int h, int bpl, int shift);
    void AddAmstradCPCToData(QByteArray& data, LImage* img,int x, int y, int w, int h);
    void AddAtariBitplaneToData(QByteArray& data, MultiColorImage& img,int x, int y, int w, int h);
    void AddSingleAtariBitplaneToData(QByteArray& data, MultiColorImage& img,int x, int y, int w, int h, int bpl_select);

    int CompareSprites(QByteArray& d1,QByteArray& d2, int sprite1, int sprite2);

    void CompressScreenAndCharset(QVector<int>& screen, QByteArray& charset, QVector<int> &sOut, QByteArray& cOut, int sw, int sh, int charSize, int noTargetChar,int bmask);

    static void OptimizeScreenAndCharset(QVector<int>& screen, QByteArray& charset, QVector<int> &sOut, QByteArray& cOut, int sw, int sh, int charSize, double compression,int type, LColorList& lst, int bmask);
    static void OptimizeScreenAndCharsetGB(QVector<int>& screen, QByteArray& charset, QVector<int> &sOut, QByteArray& cOut, int sw, int sh, int charSize, double compression,int type, LColorList& lst, int bmask);

//    static void FrameConverter(QString dir, QString outFile, QVector<int> cols);

    static void SaveSinusScrollerData(MultiColorImage*, int height, int startaddr, QString fname);
    static void SaveSinusScrollerData_OLD(MultiColorImage*, int height, int startaddr, QString fname);


    static int BitplaneCharsetSpritePacker(QByteArray& inData, QByteArray &outData, QVector<int> &arrangement, int x, int y, int w, int h, int compression, int noBitplanes);




    void SaveCompressedSpriteData(QByteArray& data, QString dataFile, QString tableFile, int address, int compressionLevel);

    static double Compare(QByteArray& a, QByteArray& b, int p1, int p2, int length, int type, int bmask);

    void OptimizeAndPackCharsetData(QByteArray& dataIn, QByteArray& out, QByteArray& table, int width, double compression, bool invertTable,int type,LColorList& lst, int bmask);

    void ConvertToC64(bool dither);

    static void GenerateParallaxData(QString inFile, QString outFile, int x0, int y0, int x1, int y1,int p1, int type, int eor);

    void AddCharsetScreen(QByteArray& data, QImage& img, CharsetImage* charset,int w, int h);

    void AddGameboyData(QByteArray &data, MultiColorImage &img, int xp, int yp, int w, int h);

 //   void m_compression.AddCharsetScreen(ba, m_effect->m_img, charset, lua_tonumber(L,1),lua_tonumber(L,2));


    void AddPetsciiScreen(QByteArray& data, QImage& img);
    void AddBinaryScreen(QByteArray& data, QImage& img);

    void SaveCompressedTRM(QByteArray& data, QString file, int compression);

    static void GenerateShiftedCharset(QString inFile, QString outFile, int start, int end);
    static void GenerateRotatedCharset(QString inFile, QString outFile, int start, int end, int dir);

    static void GenerateUnrolledAsm1(QString name, QString outFile, QString inFile, int src, int dst,int width,int height,int shift, int shift2, int shift3);

};


#endif // COMPRESSION_H
