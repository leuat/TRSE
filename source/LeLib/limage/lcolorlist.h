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

};

class LColorList : public QObject
{
    Q_OBJECT
private:
    QVector<LColor> m_list;


public:
    enum Type{ C64, C64_ORG, CGA1_LOW, CGA1_HIGH, CGA2_LOW, CGA2_HIGH, UNSUPPORTED, TIFF, VIC20 };

    Type m_type = Type::C64;
    LColorList();
    ~LColorList();

    LColor& get(int i) {
        if (i<m_list.count())
            return m_list[i];
        else return m_list[0];
    }

    static unsigned char TypeToChar(Type t);
    static Type CharToType(unsigned char c);

    QVector<QPushButton*> m_buttonsImport;
    QVector<QPushButton*> m_buttonsEdit;

    Metric* m_metric = nullptr;


    void Initialize(Type t);

    void InitC64_org();
    void InitC64();
    void InitVIC20();
    void InitCGA1_LOW();
    void InitCGA1_HIGH();
    void InitCGA2_LOW();
    void InitCGA2_HIGH();
    void UpdateColors();
    QColor getClosestColor(QColor col);

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
