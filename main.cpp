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


#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QSettings>
#include "trc.h"
/*
void ConvertPerlin(QString input, QString out, float div) {
    QImage img;
    img.load(input);

    QByteArray a;
    int xw = img.width()/div;
    int xh = img.height()/div;
    a.resize(xw*xh);

//    qDebug() << "Converting file: " << xw;

    for (int x=0;x<xw;x++)
        for (int y=0;y<xh;y++) {

            a[x + y*xw]=QColor(img.pixel(x*div, y*div)).red();
//            if (rand()%100>98)
  //              qDebug() << a[x + y*xw];
        }

    QFile f(out);
    f.open(QFile::WriteOnly);
    f.write(a);
    f.close();
}


void SineTable(QString fn) {
    QFile f(fn);
    f.open(QFile::WriteOnly);
    QByteArray b;
    for (int i=0;i<256;i++)
        b.append(sin((i/256.0)*3.14159265*2)*127+128);
    f.write(b);
    f.close();

}

void SineTablei(QString fn) {
    QFile f(fn);
    f.open(QFile::WriteOnly);
    QByteArray b;
    for (int i=0;i<256;i++) {
        b.append(sin((i/256.0)*3.14159265*2)*127+128);
    }
    f.write(b);
    f.close();

}


void RandTable(QString fn) {
    QFile f(fn);
    f.open(QFile::WriteOnly);
    QByteArray b;
    for (int i=0;i<256;i++)
        b.append(rand()%0xFF);
    f.write(b);
    f.close();

}

void TanTable(QString fn) {
    QFile f(fn);
    f.open(QFile::WriteOnly);
    QByteArray b;
    float ss = 0.8;
    float div = 1.0f;
    for (int i=0;i<256;i++) {
//        b.append(1+tan((i/256.0)*3.14159265/2 + 3.14159)*127+128);
//      b.append(1+tan((i/256.0)*PI/2.83)*127+128);
        float v = tan( ((i/256.0)*PI*ss+PI/2.0 + (PI/2.0*(1-ss)/1) ) )*0.4;
        if (v>div) v=div;
        if (v<-div) v=-div;
        v=v/div;
//        qDebug() << i<< " : " << v;
      b.append((v+1)*127+128);
}
//    for (int i=0;i<256;i++)
  //      qDebug() << QString::number(i)  << " : " << QString::number(b[i]);

    f.write(b);
    f.close();

}

void ColumnTab()
{
    int k=0xFF;
    QByteArray columntab;
    columntab.resize(256);
    for (int i=0;i<256;i++) {
        if (i==0xff-0x80)  k=0x70;
        if (i==0xff-0x40)  k=0x60;
        if (i==0xff-0x20)  k=0x50;
        if (i==0xff-0x10)  k=0x40;
        if (i==0xff-0x08)  k=0x30;
        if (i==0xff-0x04)  k=0x20;
        if (i==0xff-0x02)  k=0x10;
        if (i==0xff-0x01) k=0x00;

        columntab[i]=k/16;
    }
    QFile f("columntab.bin");
    f.open(QFile::WriteOnly);
    f.write(columntab);
    f.close();

}
*/
/*

void CircleAndAtan(QString f1, QString f2, int w, int h) {
    QByteArray b1,b2;
    for (int j=0;j<h;j++)
        for (int i=0;i<w;i++) {
            float y = (j-h/2)/(float)h;
            float x = (i-w/2)/(float)w;
            float s = sqrt(x*x+y*y);
            b1.append((char)(s*15.0));
            float s2 = atan2(x,y);
            b2.append((char)(s2*15.0));

        }

    Util::SaveByteArray(b1,f1);
    Util::SaveByteArray(b2,f2);
}


void TestSSIM() {
    LImageQImage ia, ib;
    ia.LoadQImage("/home/leuat/Pictures/jupiter1.jpg");
    ib.LoadQImage("/home/leuat/Pictures/jupiter1.jpg");
    qDebug() <<" Self : " <<ia.CalcSSIM(&ib);
    ib.LoadQImage("/home/leuat/Pictures/jupiter2.jpg");
    qDebug() <<" jup 2 : " <<ia.CalcSSIM(&ib);
    ib.LoadQImage("/home/leuat/Pictures/skullsanta.jpeg");
    qDebug() <<" santa : " <<ia.CalcSSIM(&ib);
    ib.LoadQImage("/home/leuat/Pictures/avail.png");
    qDebug() <<" avail : " <<ia.CalcSSIM(&ib);
}


//https://www.c64-wiki.com/wiki/Commodore_Plus/4
*/

void fixCurrentDir(QString execFile) {
    QFileInfo exec(execFile);
    QDir::setCurrent(exec.absoluteDir().absolutePath());
}

int main(int argc, char *argv[])
{
    if (argc>=2) {
        if (QString(argv[1])=="-cli") {
            ClascExec ras(argc, argv);
            return ras.Perform();
        }
    }


    QApplication a(argc, argv);
    a.setOrganizationDomain("lemonspawn.com");
    a.setApplicationName("TRSE");
    QString oldCurDir = QDir::currentPath();
    fixCurrentDir(QString(argv[0]));
    a.setStyle(QStyleFactory::create("Fusion"));
    MainWindow w;
    for (int i=0; i<argc;i++)
        w.m_commandParams+=QString(argv[i]);
    w.show();
    w.AfterStart(oldCurDir);
    w.RestoreSettings();

    return a.exec();
}
