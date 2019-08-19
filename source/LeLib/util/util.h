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

#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <random>
#include <QVector3D>
#include <QImage>
#include <QString>
#include <QRgb>
#include <QDirIterator>
#include <QElapsedTimer>
#include <QTextStream>
#include <QDebug>
#include <QElapsedTimer>
#include <QLayout>
#include <QLayoutItem>
#include <QWidget>

//#include "random.h"

#define Q_TIMER_START()  \
{                        \
    QElapsedTimer ttimer; \
    ttimer.start();        \

#define Q_TIMER_ELAPSED(name) \
   qDebug() << (name) << " took "<< QString::number(ttimer.elapsed()/1000.0) << " seconds"; \
}

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif


using namespace std;

class Util {

public:

    static QElapsedTimer globalTimer;

    static void Tokenize(const string& str,
                         vector<string>& tokens,
                         const string& delimiters = " ");




    static QString toString(QStringList lst);
    static unsigned long int Endian_DWord_Conversion(unsigned long int dword)
    {
      return ((dword>>24)&0x000000FF) | ((dword>>8)&0x0000FF00) | ((dword<<8)&0x00FF0000) | ((dword<<24)&0xFF000000);
   }
    static unsigned long int Endian_Word_Conversion(unsigned short dword)
    {
      return ((dword>>24)&0x000000FF) | ((dword>>8)&0x0000FF00) | ((dword<<8)&0x00FF0000) | ((dword<<24)&0xFF000000);
   }

    static void string2char(string s, char* to);
    static string toString(double d, string param);
    static string toString(double d);
    static string toString(int d);
    static bool CancelSignal;
    static const char* read_textfile(string filename);
    static void verify_file(string filename);
    static bool verify_file_bool(string filename);
    static string trim(string s);
    static int VerifyHexAddress(QString s);
    static QString numToHex(int v);
    static QString path;

    static QColor colorScale(QColor& col, int mean, int std);

    static int isEqual(QColor a, QColor b) {
        if (a.red()!=b.red())
            return 0;
        if (a.green()!=b.green())
            return 0;
        if (a.blue()!=b.blue())
            return 0;

        return 1;
    }


    static int getShiftCount(int i);



    static bool QStringIsSimilar(QString a, QString b, qreal percentage, int n, Qt::CaseSensitivity caseSense);

    static QStringList FindFilesOfType(QString dir, QString type);

    static void ConvertFileWithLoadAddress(QString input, QString output, int address);
    static void ConvertFileWithLoadAddress(QString input, QString output);


    static void CopyFile(QString i, QString o);

    static QVector3D abss(QVector3D a);
    static QVector3D maxx(QVector3D a, QVector3D b);

    static int C64StringToInt(QString f) {
        int val;
        bool ok;
        if (f.contains("$")) {
            val = f.replace("$","0x").toInt(&ok,16);
        }
        else
            val = f.toInt(&ok,10);

        return val;

    }

    static bool SameSide(const QVector3D& p1,const QVector3D& p2, const QVector3D& a,const QVector3D& b) {
        QVector3D cp1 = QVector3D::crossProduct(b-a, p1-a);
        QVector3D cp2 = QVector3D::crossProduct(b-a, p2-a);
        return QVector3D::dotProduct (cp1, cp2) >= 0;
//        else return false
    }

    static QString fixFolder(QString folderName) {
        if (folderName[folderName.count()-1]=='\\')
                return folderName;
        if (folderName[folderName.count()-1]=='/')
                return folderName;
        return folderName + "/";
    }

    static std::string c2x(int x, int y) {
        std::string s;
        s = char('A' + y);
        s += std::to_string(x+1);
        return s;
    }

    static float minmax(float v, float a, float b);

    static QString findFileInDirectory(QString search,QString dir, QString extension);
    static QString findFileInSubDirectories(QString file, QString dir, QString extension);
    static QString listFiles(QDir directory, QString searchFile);
    static float floatRandom(const float & min, const float & max) {
        static std::mt19937 generator;
        std::uniform_real_distribution<float> distribution(min, max);
        return distribution(generator);
    }
    static wchar_t* QStringToWchar(QString t) {
        wchar_t* arr = new wchar_t[t.size()+1];
        t.toWCharArray(arr);
        arr[t.size()]=0;
        return arr;
    }

    static void SaveByteArray(QByteArray& data, QString file);

    static QString fromStringList(QStringList lst);

    static int NumberFromStringHex(QString s) {
        int val = 0;
        bool ok = NumberFromStringHex(s,val);
        return val;
    }

    static QString ReplaceWords(QString line, QString word);



    static QString BinopString(QString a);

    static bool NumberFromStringHex(QString s, int& num);

    static QVector3D fromSpherical(float r, float t, float p) {
        return QVector3D( r*sin(t)*cos(p), r*sin(t)*sin(p), r*cos(t)  );
    }

    static QVector3D floor(const QVector3D v) {
        return QVector3D( max(0.0f, v.x()), max(0.0f,v.y()), max(0.0f,v.z())  );
    }

    static QVector3D Rotate2D(QVector3D point, QVector3D center, float angle) {
        QVector3D rot;
        point = point - center;
        rot.setX(point.x()*cos(angle)-point.y()*sin(angle));
        rot.setY(point.y()*cos(angle)+point.x()*sin(angle));
        return rot + center;

    }

    static bool IntersectSphere(QVector3D o, QVector3D d, QVector3D r,QVector3D& isp1,QVector3D& isp2, double& t0, double& t1) {

        r.setX(1.0/(r.x()*r.x()));
        r.setY(1.0/(r.y()*r.y()));
        r.setZ(1.0/(r.z()*r.z()));


        QVector3D rD = QVector3D(d.x()*r.x(), d.y()*r.y(), d.z()*r.z());
        QVector3D rO = QVector3D(o.x()*r.x(), o.y()*r.y(), o.z()*r.z());


        double A = QVector3D::dotProduct(d,rD);
        double B = 2.0*(QVector3D::dotProduct(d, rO));
        double C = QVector3D::dotProduct(o, rO) - 1.0;

        double S = (B*B - 4.0f*A*C);

        if (S<=0) {
            isp1 = QVector3D(0,0,0);
            isp2 = QVector3D(0,0,0);
            t0 = 0;
            t1 = 0;
            return false;
        }

        t0 =  (-B - sqrt(S))/(2.0*A);
        t1 =  (-B + sqrt(S))/(2.0*A);

        isp1 = o+d*t0;
        isp2 = o+d*t1;

        return true;
    }

    static void clearLayout(QLayout* layout, bool deleteWidgets = true)
    {
        while (QLayoutItem* item = layout->takeAt(0))
        {
            if (deleteWidgets)
            {
                if (QWidget* widget = item->widget())
                    widget->deleteLater();
            }
            if (QLayout* childLayout = item->layout())
                clearLayout(childLayout, deleteWidgets);
            delete item;
        }
    }

    static QVector3D fromColor(QColor c) {
        return QVector3D(c.red(), c.green(), c.blue());
    }

    static QColor toColor(QVector3D c) {
        if (c.x()>255) c.setX(255);
        if (c.y()>255) c.setY(255);
        if (c.z()>255) c.setZ(255);
        if (c.x()<0) c.setX(0);
        if (c.y()<0) c.setY(0);
        if (c.z()<0) c.setZ(0);

        return QColor(c.x(),c.y(),c.z());
    }


    static float smoothstep(float edge0, float edge1, float x);
    static float clamp(float val, const float min, const float max);
    static QVector3D clamp(QVector3D val, float min, float max);


    static QColor Gamma(QColor c, float exp, float shift);
    static float ColorLength(QColor& c) {
        return sqrt(c.red()*c.red() + c.green()*c.green() + c.blue()*c.blue());
    }

    static void drawBox(QImage* backImage, QImage* img, int i, int j, int size, QRgb color) {
        int imageSize = img->width();
        QRgb mark = QColor(1,1,1).rgba();
        for (int x=max(0, i-size/2);x<=min(imageSize-1, i+size/2);x++)
            for (int y=max(0, j-size/2);y<=min(imageSize-1, j+size/2);y++) {
                QColor col = QColor::fromRgba(backImage->pixel(x,y));
                if (col.red()==0) {
                    img->setPixel(x,y,color);
                    if (x==i && y== j)
                        backImage->setPixel(x,y,mark);
                }
            }
    }

/*    static QVector<int> ShuffleArray(QVector<int>& a)
    {
        for (int i = a.size(); i > 0; i--)
        {
            int j = rand()%i;// r.Next(i);
            int k = a[j];
            a[j] = a[i - 1];
            a[i - 1]  = k;
        }
        return a;
    }
*/
    template<typename T>
    static QVector<T> ShuffleArray(QVector<T> a)
    {
        for (int i = a.size(); i > 0; i--)
        {
            int j = rand()%i;// r.Next(i);
            T k = a[j];
            a[j] = a[i - 1];
            a[i - 1]  = k;
        }
        return a;
    }

    static QString getFileName(QString dir, QString baseName, QString type);

    static QString loadTextFile(QString filename) {
        QFile file(filename);
        file.open(QIODevice::ReadOnly);
        QTextStream in(&file);
        QString data = in.readAll();
        file.close();
        return data;
    }

    static QString MilisecondToString(int ms) {
//        ms+=1000;
        int ds = ms/100;
        int s = (ms/1000);
        int m = (s/60);
        int h = (m/60);
        int d = h/24;
        ds = ds % 10;
        s = s % 60;
        m = m % 60;
        h = h % 24;
        QString str = "";
        if (d!=0)
            str+= QString::number(d) + "d ";
        if (h!=0)
            str+= QString::number(h) + "h ";
        if (m!=0)
            str+= QString::number(m) + "m ";
        str+= QString::number(s) + "." + QString::number(ds) + "s ";
        return str;
    }
    static QVector3D maxQvector3D(const QVector3D a, const QVector3D b) {
        return QVector3D(max(a.x(), b.x()),max(a.y(), b.y()),max(a.z(), b.z()));
    }

    inline static bool Mollweide(QVector3D& out, float i, float j, float l0, float R, float size) {

/*        float x = 4*R*sqrt(2)*(2*i/(float)size-1);
        float yy = j*2 - size/2;
        float y = R*sqrt(2)*(2*yy/(float)size-1);


        float t = asin(y/(R*sqrt(2.0)));
        out = QVector3D( asin( (2.0*t + sin(2.0*t))/M_PI),l0 + M_PI*x / (2*R*sqrt(2.0)*cos(t)),0  );
        out.setX(out.x()+M_PI/2);
        out.setY(-out.y()*0.5);
        if (out.y()>-M_PI && out.y()<M_PI)
            return true;
*/
        return false;

    }


    static QPoint mapToWindow(QWidget *from, QPoint pt){
      QWidget *wnd = from->window();
      while(from && from!=wnd){
        pt = from->mapToParent(pt);
        from = from->parentWidget();
      }
      return pt;
    }
};


template <typename T>
struct PtrLess // public std::binary_function<bool, const T*, const T*>
{
    bool operator()(const T* a, const T* b) const
    {
        // may want to check that the pointers aren't zero...
        return *a < *b;
    }
};
