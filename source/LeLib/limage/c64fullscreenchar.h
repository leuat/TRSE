#ifndef C64FullScreenChar_H
#define C64FullScreenChar_H

#include "multicolorimage.h"
#include "charsetimage.h"

class C64FullScreenChar: public MultiColorImage
{
public:
    QByteArray m_rawData, m_rawColors;
    bool m_isMultiColor = false;

    int m_charWidth = 40;
    int m_charHeight = 25;


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
    void BuildImage();
    CharsetImage* getCharset() override { return m_charset; }

    void setPixel(int x, int y, unsigned int color) override;
    unsigned int getPixel(int x, int y) override;
    void CopyFrom(LImage* mc) override;
};

#endif // CHARSETIMAGE_H
