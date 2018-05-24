#ifndef CHARSETIMAGE_H
#define CHARSETIMAGE_H

#include "multicolorimage.h"
#include <QPixmap>


class CharsetImage : public MultiColorImage
{
public:
    int m_charCount = 255;
    QByteArray m_rawData;
    bool m_isMultiColor = false;
    PixelChar m_color;
    CharsetImage(LColorList::Type t);

    PixelChar m_copy[16];

    enum Mode{ FULL_IMAGE, CHARSET1x1, CHARSET2x2, CHARSET2x2_REPEAT};

    QString GetCurrentModeString() override {
        if (m_currentMode==CHARSET1x1) return "1x1 charset mode";
        if (m_currentMode==CHARSET2x2) return "2x2 charset mode";
        if (m_currentMode==CHARSET2x2_REPEAT) return "2x2 charset repeat mode";

        return "Full image mode";
    }


    Mode m_currentMode = FULL_IMAGE;


    void SetColor(uchar col, uchar idx) override;

    void SaveBin(QFile& file);
    void LoadBin(QFile& file);

    CharsetImage* getCharset() override { return this; }
    unsigned int getPixel(int x, int y) override;

    void ImportBin(QFile& f) override;
    void ExportBin(QFile& f) override;
    virtual void FromRaw(QByteArray& arr);
    virtual void ToRaw(QByteArray& arr);
    void ToQPixMaps(QVector<QPixmap>& map);
    virtual void setPixel(int x, int y, unsigned int color) override;


    void RenderEffect(QMap<QString, float> params) override;

    void CopyFrom(LImage* mc);
    bool KeyPress(QKeyEvent *e) override;

    void setLimitedPixel(int x, int y, unsigned int color);

    void FlipHorizontal() override;
    void FlipVertical() override;

    void CopyChar() override;
    void PasteChar() override;

};

#endif // CHARSETIMAGE_H
