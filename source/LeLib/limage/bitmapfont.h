#ifndef BITMAPFONT_H
#define BITMAPFONT_H
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QFont>

class BitmapFont
{
public:
    BitmapFont();

    QImage m_image;

    void Test();
    void RenderFont(QString font, int fontSize, QFont::Weight weight, float charsPerLine, float);


};

#endif // BITMAPFONT_H
