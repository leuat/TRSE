#ifndef LIMAGESPRITES_H
#define LIMAGESPRITES_H

#include "charsetimage.h"
#include <QPixmap>
#include <QKeyEvent>


class LImageSprites : public CharsetImage
{
public:
    LImageSprites(LColorList::Type t) : CharsetImage(t) {
        m_type = LImage::Type::Sprites;
        m_charCount = 1024;
        m_currentMode = Mode::CHARSET1x1;
        m_currencChar=0;
        //Data::data.currentColor=0;

    }
  //  void ImportBin(QFile& f) override;
   // void ExportBin(QFile& f) override;
    void FromRaw(QByteArray& arr) override;
    void ToRaw(QByteArray& arr) override;

    QPoint getXY(int x, int y);

    bool KeyPress(QKeyEvent *e) override;

    void setPixel(int x, int y, unsigned int color) override;
    unsigned int getPixel(int x, int y) override;
    void FlipVertical() override;
    void FlipHorizontal() override;

    void CopyChar();

    void PasteChar();


};


#endif // LIMAGESPRITES_H
