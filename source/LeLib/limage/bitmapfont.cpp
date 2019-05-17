#include "bitmapfont.h"

BitmapFont::BitmapFont()
{
    m_image = QImage(QSize(640,400),QImage::Format_ARGB32);
    m_image.fill(QColor(0,0,0,255));

}

void BitmapFont::Test()
{
    for (int j=0;j<m_image.height();j++)
        for (int i=0;i<m_image.width();i++) {
            QColor c(rand()%255, rand()%255, rand()%255);

            m_image.setPixelColor(i,j,c);
        }
}

void BitmapFont::RenderFont(QString font, int fontSize, QFont::Weight weight, float charsPerLine, float charsPerCol)
{
    int i=0;
    QString data = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    float x = 0;
    float y = 2;

    float dx = m_image.width()/(float)charsPerLine;
    float dy = charsPerCol;
    m_image.fill(QColor(0,0,0,255));

    QPainter p;
    p.begin(&m_image);
    p.setPen(QPen(QColor(255,255,255)));

    p.setFont(QFont(font, fontSize, weight));
    int cnt = 0;
    float cx = dx/2-  fontSize/2;
    float cy = dy/2-  fontSize/2;
//    exit(1);
    for (int i=0;i<data.length();i++) {
        p.drawText(QRect(x+cx, y+cy,fontSize*1, fontSize*1.5), Qt::AlignCenter,QString(data[i]));
        x=x+dx;
        if (++cnt==charsPerLine) {
            cnt=0;
            x=0;
            y=y+dy*2;
        }
    }

}
