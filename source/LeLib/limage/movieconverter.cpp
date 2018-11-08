/*
 * Turbo Rascal Syntax error, “;” expected but “BEGIN” (TRSE, Turbo Rascal SE)
 * 8 bit software development IDE for the Commodore 64
 * Copyright (C) 2018  Nicolaas Ervik Groeneboom (nicolaas.groeneboom@gmail.com)
 *
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program (LICENSE.txt).
 *   If not, see <https://www.gnu.org/licenses/>.
*/

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

void MovieConverter::ConvertRaw(QString dir, QString baseName, QString fileExtension, QString outFile, int frameSkip, int maxFrames, int width, int height, float zoom)
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

void MovieConverter::ConvertPacked(QString dir, QString baseName, QString fileExtension, QString outFile, int frameSkip, int maxFrames, int width, int height, float zoom)
{

    QByteArray buf;

    QDir directory(dir);
    qDebug() << dir;
    QStringList images = directory.entryList(QStringList() << "*."+fileExtension,QDir::Files);
    int cur=1;
    int val = 0;
    int lowest = 1E20;
    for (QString img: images) {
        QString first = img.remove(baseName);
        first = first.split(".")[0];
        val = first.toInt();
        qDebug() << val << " " << img;
        if (val<lowest)
            lowest=val;
    }
    cur = lowest;
//    qDebug() << "lowest:" <<lowest;
  //  exit(1);
    bool done=false;
    int noframes = 0;
    int idx=0;
    QByteArray prevFrame, curFrame;
    float compr;
    int kk=0;
    while (!done) {
        QString fname = dir + "/" + baseName + QString::number(cur)+"."+fileExtension;
        qDebug() << fname;
        if (!QFile::exists(fname))
            done=true;
        else {
            QImage img;
            img.load(fname);
            curFrame = ConvertImage(img, width, height,zoom);
            if (idx==0) {
                prevFrame = curFrame;
                buf.append(curFrame);
                idx++;
                noframes++;
                kk=0;
            }

            if (kk==frameSkip) {
                if (idx!=0) {
                    buf.append(CompressImage(prevFrame, curFrame, width,height, compr )    );
                    prevFrame = curFrame;
                    kk=0;
                    noframes++;
                    if (noframes>=maxFrames)
                        done=true;
                }



            }
            cur+=frameSkip;
            kk++;

        }
    }
    qDebug() << "Compression:" << (compr/noframes)*100 << "%";

    if (buf.length()!=0) {
        QFile f(outFile);
        f.open(QFile::WriteOnly);
        f.write(buf);
        f.close();
    }
    qDebug() << "No frames: " << noframes;
}

QByteArray MovieConverter::CompressImage(QByteArray prevFrame, QByteArray newFrame, int w, int h, float&compr)
{
    QByteArray ret;
    uchar skip=0;
    qDebug() << prevFrame.count();
    int c = 0;
    for (int x=0;x<h;x++) {

        for (int y=0;y<w;y++)
        {
            int i=y + x*w;
            prevFrame[i] = prevFrame[i]&0x0F;
            newFrame[i] = newFrame[i]&0x0F;
            char a = prevFrame[i]/2;
            char b = newFrame[i]/2;


            /*if (((uchar)newFrame[i]==SKIP) || ((uchar)newFrame[i]==END)) {
                qDebug() << "WARNING: NEWFRAME SKIP OR END";
                newFrame[i]  = (newFrame[i] & 0x0F) | 4<<4;
            }*/
            //newFrame[i]  = (newFrame[i] & 15) | 4<<4;

            if (abs(a-b)<1) {
                if (skip<254) {
                    skip++;
                    //continue;
                }
                else {
                    ret.append(SKIP);
                    ret.append(skip);
                    c+=2;
                    skip=0;
                    continue;

                }

            }
            else {
                if (skip!=0) {
                    ret.append(SKIP);
                    ret.append(skip);
                    ret.append(newFrame[i]/2);
                    c+=3;
                    skip=0;
                }
                else {
                    ret.append(newFrame[i]/2);
                    c+=1;
                }

            }
        }
        skip+=40-w;
    }
    qDebug() << ret.count();
    qDebug() << "C:" << c/((float)w*h)*100<< "%";
    compr+=c/((float)w*h);
    ret.append(END);
    return ret;
}

QByteArray MovieConverter::CompressScreen(QByteArray prevFrame, QByteArray newFrame, int w, int h, float &compr)
{
    QByteArray ret;
    uchar skip=0;
    qDebug() << prevFrame.count();
    int c = 0;
    for (int x=0;x<h;x++) {

        for (int y=0;y<w;y++)
        {
            int i=2*(y + x*w);
//            prevFrame[i] = prevFrame[i];
//            newFrame[i] = newFrame[i];
            char px1 = prevFrame[i];
            char px2 = newFrame[i];

            char c1 = (prevFrame[i+1] & 0xF);
            char c2 = (newFrame[i+1]& 0xF);

            if (px1==px2 && c1==c2 ) {
                if (skip<254) {
                    skip++;
                    //continue;
                }
                else {
                    ret.append(MSKIP | c2);
                    ret.append(skip);
                    c+=2;
                    skip=0;
                    continue;

                }

            }
            else {
                if (skip!=0) {
                    ret.append(MSKIP | c2);
                    ret.append(skip);
                    ret.append(c2);
                    ret.append(px2);
                    c+=4;
                    skip=0;
                }
                else {
                    ret.append(c2);
                    ret.append(px2);
                    c+=2;
                }

            }
        }
//        skip+=40-w;
    }
    qDebug() << ret.count();
    qDebug() << "C:" << c/((float)w*h)*100<< "%";
    compr+=c/((float)w*h);
    ret.append(MEND);
    return ret;
}

QByteArray MovieConverter::CompressScreen2(QByteArray prevFrame, QByteArray newFrame, int w, int h, float &compr)
{
    QByteArray ret;
    uchar skip=0;
    int c = 0;
    for (int kk=0;kk<2;kk++) {
        for (int x=0;x<h;x++) {
            for (int y=0;y<w;y++)
            {
                int i=2*(y + x*w);
                char px1 = prevFrame[i];
                char px2 = newFrame[i];

                if (kk==1) {
                    px1 = (prevFrame[i+1] & 0xF);
                    px2 = (newFrame[i+1]& 0xF);
                }

                if (px1==px2) {
                    if (skip<255) {
                        skip++;
                        //continue;
                    }
                    else {
                        ret.append(SKIP);
                        ret.append(skip);
                        c+=2;
                        skip=0;
                        continue;

                    }

                }
                else {
                    if (skip!=0) {
                        ret.append(SKIP);
                        ret.append(skip);
                        ret.append(px2);
                        c+=3;
                        skip=0;
                    }
                    else {
                        ret.append(px2);
                        c+=1;
                    }

                }
            }
            //        skip+=40-w;
        }
        //    qDebug() << ret.count();
        //  qDebug() << "C:" << c/((float)w*h)*100<< "%";
        compr+=c/((float)w*h);
        ret.append(END);
        skip=0;
    }
    return ret;
}



QByteArray MovieConverter::CompressScreen3(QByteArray prevFrame, QByteArray newFrame, int w, int h, float &compr, char CEnd, char CSkip)
{
    QByteArray ret;
    uchar skip=0;
    int c = 0;
    uchar cnt = 0;
    for (int kk=0;kk<2;kk++) {
        cnt=1;
        char cur = newFrame[0];
        if (kk==1) {
            cur = (newFrame[1] & 0xF);
        }
        for (int x=0;x<h;x++) {
            for (int y=0;y<w;y++)
            {
                int i=2*(y + x*w);
                char px2 = newFrame[i];

                if (kk==1)
                    px2 = (newFrame[i+1]& 0xF);


                if (cur==px2 && cnt<255) {
                    cnt++;
                }
                else {

                    ret.append(cnt);
                    ret.append(cur);
                    c+=2;

                    cur=px2;
                    cnt=1;
                }
            }
            //        skip+=40-w;
        }
        //    qDebug() << ret.count();
        //  qDebug() << "C:" << c/((float)w*h)*100<< "%";
        ret.append(cnt);
        ret.append(cur);
        c+=2;
        compr+=c/((float)w*h);
        ret.append(CEnd);
        skip=0;
    }
    return ret;
}
