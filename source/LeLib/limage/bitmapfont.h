#ifndef BITMAPFONT_H
#define BITMAPFONT_H
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QFont>

class BitmapFont
{
public:
    void Init(int w, int h);

    QImage m_image;

    void Test();
    void RenderFont(QString font, int fontSize, QFont::Weight weight, float charsPerLine, float, float);


};

#endif // BITMAPFONT_H
