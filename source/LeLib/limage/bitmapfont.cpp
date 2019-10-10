#include "bitmapfont.h"

void BitmapFont::Init(int w, int h)
{
    m_image = QImage(QSize(w,h),QImage::Format_ARGB32);
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

void BitmapFont::RenderFont(QString font, int fontSize, QFont::Weight weight, float charsPerLine, float charsPerCol, float startChar)
{
    int i=0;
    QString data = "";
    for (int i=0;i<128;i++)
        data+=(char)(i+startChar);
    float x = 0;
    float y = 0;
    float dx = m_image.width()/(float)charsPerLine;
    float dy = charsPerCol;
    m_image.fill(QColor(0,0,0,255));

    QPainter p;
    p.begin(&m_image);
    p.setPen(QPen(QColor(255,255,255)));

    p.setFont(QFont(font, fontSize, weight));
    int cnt = 0;
//    float cx = dx/2-  fontSize/2;
  //  float cy = dy/2-  fontSize/2;
//    exit(1);
    for (int i=0;i<data.length();i++) {
//        p.drawText(QRect(x, y,fontSize*2, fontSize*2.0), Qt::AlignVCenter | Qt::AlignHCenter,QString(data[i]));
        p.drawText(QRect(x, y,fontSize*2, fontSize*2.0), Qt::AlignLeft,QString(data[i]));
        x=x+dx;
        if (++cnt==charsPerLine) {
            cnt=0;
            x=0;
            y=y+dy;
        }
    }

}
