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

void TanTable(QString fn) {
    QFile f(fn);
    f.open(QFile::WriteOnly);
    QByteArray b;
    float ss = 0.8;
    float PI = 3.14159265;
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

void fixCurrentDir(QString execFile) {
    QStringList al = execFile.split(QDir::separator());
    al.removeLast();
    QString dir = QDir::separator();
    for (QString s : al)
        dir +=s+QDir::separator();

    QDir::setCurrent(dir);

}


int main(int argc, char *argv[])
{
//    ConvertAllObjs();
    QApplication a(argc, argv);
    QString oldCurDir = QDir::currentPath();
    fixCurrentDir(QString(argv[0]));
    a.setStyle(QStyleFactory::create("Fusion"));
    MainWindow w;
    for (int i=0;i<argc;i++)
        w.m_commandParams+=QString(argv[i]);
    w.showMaximized();
    w.AfterStart(oldCurDir);

    return a.exec();
}
