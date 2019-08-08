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

#ifndef LCOLORLIST_H
#define LCOLORLIST_H

#include <QColor>
#include <QVector>
#include <QString>
#include <QComboBox>
#include <QPushButton>
#include <QIcon>
#include <QPixmap>
#include <QLayout>
#include "source/LeLib/data.h"
#include <math.h>
class Metric {
public:
    virtual float getDistance(QColor& a, QColor& b) = 0;
};

class LinearMetric: public Metric {
public:
    float getDistance(QColor& a, QColor& b) override {
        return sqrt(1+ pow(a.red() - b.red(), 2) +pow(a.green() - b.green(), 2) +pow(a.blue() - b.blue(), 2)  );
    }
};


class LColor {
public:
    QColor color;
    bool inUse = true;
    QString name;
    LColor() {}
    LColor(QColor col, QString n) {
        color = col;
        name = n;
    }

    unsigned short get12BitValue() {
        return color.blue()/16  | (color.green()/16)<<4  | (color.red()/16)<<8;

    }
    QString toRGB8() {
        return QString::number(color.red())+"," + QString::number(color.green()) +","+ QString::number(color.blue());
    }
    QString toRGB4() {
        return QString::number(color.red()/16)+"," + QString::number(color.green()/16) +","+ QString::number(color.blue()/16);
    }

    void fromRGB8(QString s) {
        QStringList d = s.simplified().trimmed().split(",");
        if (d.count()==3) {
            color.setRed( d[0].toInt() );
            color.setGreen( d[1].toInt() );
            color.setBlue( d[2].toInt() );
        }
    }

    void fromRGB4(QString s) {
        QStringList d = s.simplified().trimmed().split(",");
        if (d.count()==3) {
            color.setRed( d[0].toInt()*16 );
            color.setGreen( d[1].toInt()*16 );
            color.setBlue( d[2].toInt()*16 );
        }
    }

};

class LColorList : public QObject
{
    Q_OBJECT
private:


public:
    QVector<LColor> m_list;

    enum Type{ C64, C64_ORG, CGA1_LOW, CGA1_HIGH, CGA2_LOW, CGA2_HIGH, UNSUPPORTED, TIFF, VIC20, PICO8 };

    Type m_type = Type::C64;
    LColorList();
    ~LColorList();

    LColor& get(int i);
    QColor m_cblack = QColor(0,0,0,255);
    LColor m_black = LColor(m_cblack,"black");

    static unsigned char TypeToChar(Type t);
    static Type CharToType(unsigned char c);

    QVector<QPushButton*> m_buttonsImport;
    QVector<QPushButton*> m_buttonsEdit;


    void SetGreyscale(QVector3D base, bool inverted);
    void SetTwoColors(QVector3D base1, QVector3D base2);
    Metric* m_metric = nullptr;
    void EnableColors(QVector<int>& cols);

    void GeneratePaletteFromQImage(QImage& img);

    int getNoBitplanes();
    void setNoBitplanes(int bpl);
    QByteArray toArray();
    void fromArray(QByteArray& d);

    void Initialize(Type t);

    void CopyFrom(LColorList* other);
    void InitC64_org();
    void InitC64();
    void InitPICO8();
    void InitVIC20();
    void InitCGA1_LOW();
    void InitCGA1_HIGH();
    void InitCGA2_LOW();
    void InitCGA2_HIGH();
    void UpdateColors();
    QColor getClosestColor(QColor col, int& winner);

    void ExportAmigaPalette(QString filename);

    void FillComboBox(QComboBox* cmb);

    LColor m_background;

    int getIndex(QColor c);

    void handleButtonEdit(int data);

    void CreateUI(QLayout* ly, int type);

public slots:
    void handleButtonImport(int data);


signals:
    void colorValueChanged();

};

#endif // LCOLORLIST_H
