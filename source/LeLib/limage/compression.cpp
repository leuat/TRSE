#include "compression.h"

Compression::Compression()
{

}

void Compression::SaveCharset(QString filename, QImage& img,int w, int h, int div)
{
    QFile f(filename);
    f.open(QFile::WriteOnly);
    QByteArray data;
    for (int y=0;y<h;y++)
        for (int x=0;x<w;x++) {
            char c = QColor(img.pixel(x,y)).red()/div;
            data.append(c);
        }

    f.write(data);
    f.close();

}

void Compression::AddScreen(QByteArray &data, QImage& img,int w, int h, char base, int div, char ca1, char ca2)
{

    for (int y=0;y<h;y+=1)
        for (int x=0;x<2*w;x+=2) {
            char c1,c2;
//            if (x<m_img.width() && y<m_img.height())
            c1 = QColor(img.pixel(x,y)).red()/div;
  //          if (x+1<m_img.width() && y<m_img.height())
            c2 = QColor(img.pixel(x+1,y)).red()/div;
            char add = 0;
            char c = max(c1,c2);
            //if (c>7) c=7;
            if (c1<1) add=ca1;
            if (c2<1) add=ca2;
//            add=0;
            data.append(c+base+add);
        }

}


void Compression::AddToDataX(QByteArray &data, CharsetImage& img, int xp, int yp, int w, int h)
{
    for (int y=0;y<h;y+=1)
        for (int x=0;x<w;x+=1) {
            int xx = xp+x;
            int yy = yp+y;

            PixelChar& pc = img.m_data[40*(yy/8)+xx];
            data.append(PixelChar::reverse(pc.p[yy&7]));
        }

}

int Compression::CompareSprites(QByteArray &d1, QByteArray& d2, int sprite1, int sprite2)
{
    int l=0;
    for (int i=0;i<63;i++) {
        if (d1[i+sprite1*64] != d2[i+sprite2*64])
            l++;
    }
    return l;
}

void Compression::SaveCompressedSpriteData(QByteArray &data, QString dataFile, QString tableFile, int address, int compressionLevel)
{
    int noSprites = data.count()/64;
    QByteArray dataOut, tableOut;
    int noSpritesOut = 0;
    int compressed  = 0;
    qDebug() << data.count();
    noSpritesOut = 2;
    for (int i=0;i<64;i++) dataOut.append((char)0);
    for (int i=0;i<64;i++) dataOut.append((char)0xFF);
    for (int i=0;i<noSprites;i++) {

        int found = -1;

        for (int j=0;j<noSpritesOut;j++) {
            if (CompareSprites(data,dataOut,i,j)<compressionLevel) {
                found = j;
            }
        }
        if (found!=-1) compressed++;

        if (found==-1) {
            for (int j=0;j<64;j++)
                dataOut.append(data[j+i*64]);

            found = noSpritesOut;
            noSpritesOut++;
            qDebug() << noSpritesOut << " vs " << noSprites;

        }

        tableOut.append(found + address/64);

    }
    qDebug() << "# sprites:" << noSprites;
    qDebug() << "# Compressed sprites:" << noSprites-compressed;
    qDebug() << "Compression level:" << QString::number((1-compressed/(float)noSprites)*100) << "%";
    Util::SaveByteArray(dataOut,dataFile);
    Util::SaveByteArray(tableOut, tableFile);

}

int Compression::Compare(QByteArray &a, QByteArray &b, int p1, int p2, int length)
{
    int l = 0;
    for (int i=0;i<length;i++) {
        if (a[p1+i]!=b[p2+i]) l++;
    }
    return l;
}

void Compression::OptimizeAndPackCharsetData(QByteArray &dataIn, QByteArray &out, QByteArray &table, int width, int compression)
{
    out.clear();
    table.clear();
/*Width :  3
total rows:  4032
total chars x frames:  1344
total chars:  42
cnt SHOULD be  576*/

    int cnt = dataIn.count()/width;
    qDebug() << "Width : " << width;
    qDebug() << "total rows: " << cnt;
    qDebug() << "total chars x frames: " << (cnt/width);
    qDebug() << "total chars: " << (cnt/32)/width;

    qDebug() << "cnt SHOULD be " << (32*width*6*21);


    for (int i=0;i<cnt;i++) {
        unsigned short currentPointer = 0;
        bool isNew = true;
        for (int j=0;j<out.count()/width;j++) {
            int metric = Compare(dataIn, out, i*width, j*width, width);
            if (metric<compression) {
                currentPointer =j*width;
                isNew=false;
                //break;
            }
        }
        if (isNew) {
            currentPointer = out.count();
            for (int j=0;j<width;j++) {
                out.append(dataIn[i*width+j]);
            }
        }
//        qDebug() << isNew << currentPointer;
        char lo = currentPointer&0xff;
        char hi = (currentPointer>>8)&0xff;
//        qDebug() << QString::number(lo) << QString::number(hi);
        table.append(lo);
        table.append(hi);

    }


}


