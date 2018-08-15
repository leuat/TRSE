#include "movieconverter.h"
#include <QFile>
#include <QImage>
#include <QDebug>
#include <math.h>
#include "source/LeLib/limage/lcolorlist.h"
MovieConverter::MovieConverter()
{

}

QByteArray MovieConverter::ConvertImage(QImage &img, int w, int h, float zoom)
{
    QByteArray b;
    b.resize(w*h);

    float wx= img.width()*zoom;
    float wy= img.height()*zoom;
    float sx = img.width()*(1-zoom)/2.0f;
    float sy = img.height()*(1-zoom)/2.0f;

    LColorList lst;
    lst.Initialize(LColorList::C64);

    for (int y=0;y<h;y++)
        for (int x=0;x<w;x++) {
            QColor c = QColor(img.pixel(x/(float)w*wx+sx,y/(float)h*wy+sy));
            int val  =std::max(c.red(),c.green());
            val  =std::max(c.blue(),val);
            val=val/16;
            QColor c64Col = lst.getClosestColor(c);
            uchar idx = lst.getIndex(c64Col);
            //if (idx!=0) qDebug() <<idx;
            val=val | (idx<<4);
            b[y*w + x]=(uchar)val;
        }

    return b;
}

void MovieConverter::Convert(QString dir, QString baseName, QString fileExtension, QString outFile, int frameSkip, int maxFrames, int width, int height, float zoom)
{

    QByteArray buf;
    int cur=1;
    bool done=false;
    int noframes = 0;
    while (!done) {
        QString fname = dir + "/" + baseName + QString::number(cur)+"."+fileExtension;
        qDebug() << fname;
        if (!QFile::exists(fname))
            done=true;
        else {
            QImage img;
            img.load(fname);
            QByteArray b = ConvertImage(img, width, height,zoom);
            buf.append(b);
            cur+=frameSkip;
            noframes++;
            if (noframes>=maxFrames)
                done=true;

        }
    }


    if (buf.length()!=0) {
        QFile f(outFile);
        f.open(QFile::WriteOnly);
        f.write(buf);
        f.close();
    }
    qDebug() << "No frames: " << noframes;
}
