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
#include <QApplication>

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



class Util {

public:

    static QElapsedTimer globalTimer;
    static QMap<QString, long> sm_fileSizes;




    static QString toString(QStringList lst);
    static unsigned long int Endian_DWord_Conversion(unsigned long int dword);
    static unsigned long int Endian_Word_Conversion(unsigned short dword);
    static bool CancelSignal;
    static int VerifyHexAddress(QString s);
    static QString numToHex(long v);
    static QString path;


    static int hexType;

    static QByteArray toQByteArray(QVector<int>& data);

    static QColor colorScale(QColor& col, int mean, int std);

    static int isEqual(QColor a, QColor b);

    static void WriteInt32(QByteArray& ba, int val);
    static void WriteInt16(QByteArray& ba, int val);

    static QStringList fixStringListSplitWithCommaThatContainsStrings(QStringList lst);


    static int getShiftCount(int i);

    static uchar flipBit(uchar val, int bit);

    static QString findFileInDirectories(QString fileName, QStringList dirs);



    static bool QStringIsSimilar(QString a, QString b, qreal percentage, int n, Qt::CaseSensitivity caseSense);

    static QStringList FindFilesOfType(QString dir, QString type);

    static void ConvertFileWithLoadAddress(QString input, QString output, int address);
    static void ConvertFileWithLoadAddress(QString input, QString output);

    static QByteArray loadBinaryFile(QString filename);

    static void CopyFile(QString i, QString o);
    static void CopyFileMSVCBug(QString i, QString o);
    static void CopyFileBytes(QString i, QString o);

    static bool CopyRecursively(QString sourceFolder, QString destFolder);



    static QVector3D abss(QVector3D a);
    static QVector3D maxx(QVector3D a, QVector3D b);

    static int C64StringToInt(QString f);

    static bool SameSide(const QVector3D& p1,const QVector3D& p2, const QVector3D& a,const QVector3D& b);

    static QString fixFolder(QString folderName);

    static std::string c2x(int x, int y);

    static float minmax(float v, float a, float b);

    static QString findFileInDirectory(QString search,QString dir, QString extension);
    static QString findFileInSubDirectories(QString file, QString dir, QString extension);
    static QString listFiles(QDir directory, QString searchFile);
    static float floatRandom(const float & min, const float & max);
    static wchar_t* QStringToWchar(QString t);

    static void SaveByteArray(QByteArray& data, QString file);

    static QString fromStringList(QStringList lst);

    static long NumberFromStringHex(QString s);

    static QString ReplaceWords(QString line, QString word);



    static QString BinopString(QString a);

    static bool NumberFromStringHex(QString s, long& num);

    static QVector3D fromSpherical(float r, float t, float p);

    static QVector3D floor(const QVector3D v);

    static QVector3D Rotate2D(QVector3D point, QVector3D center, float angle);

    static bool IntersectSphere(QVector3D o, QVector3D d, QVector3D r,QVector3D& isp1,QVector3D& isp2, double& t0, double& t1);

    static void clearLayout(QLayout* layout, bool deleteWidgets = true);

    static QVector3D fromColor(QColor c) {
        return QVector3D(c.red(), c.green(), c.blue());
    }

    static QColor toColor(QVector3D c);


    static QString GetSystemPrefix();


    static float smoothstep(float edge0, float edge1, float x);
    static float clamp(float val, const float min, const float max);
    static QVector3D clamp(QVector3D val, float min, float max);


    static QColor Gamma(QColor c, float exp, float shift);
    static float ColorLength(QColor& c);

    static bool isNumber(QString s);

    static void drawBox(QImage* backImage, QImage* img, int i, int j, int size, QRgb color);

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

    static QString getFileWithoutEnding(QString f);

    static QString loadTextFile(QString filename);
    static void SaveTextFile(QString filename, QString text);

    static QString MilisecondToString(int ms);
    static QVector3D maxQvector3D(const QVector3D a, const QVector3D b);

    inline static bool Mollweide(QVector3D& out, float i, float j, float l0, float R, float size);

    static int getInt16(QByteArray& ba, int pos);
    static void setInt16(QByteArray& ba, int pos, int val);
    static void appendInt16(QByteArray& ba, int val);

    static QStringList ByteArrayToHexQStringList(QByteArray& ba);
    static QByteArray HexQStringListToByteArray(QStringList& lst);

    static QPoint mapToWindow(QWidget *from, QPoint pt);

    static QPixmap CreateColorIcon(QColor col, int s);


    static bool fileChanged(QString filename);

    static int CountFilesInAllDirectories(QString dir, QStringList fileTypes);

};


template <typename T>
struct PtrLess // public std::binary_function<bool, const T*, const T*>
{
    bool operator()(const T* a, const T* b) const
    {
        // may want to check that the pointers aren't zero...
        return *a < *b;
    }


    void clearGrid(QLayout* layout);


};
