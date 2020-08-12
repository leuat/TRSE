#ifndef LCOLOR_H
#define LCOLOR_H

#include <QString>
#include <QColor>
#include <math.h>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>
#include <QGridLayout>
#include <QDebug>
#include "source/LeLib/data.h"
#include "source/LeLib/util/util.h"
#include <QComboBox>

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
    bool displayList = true;
    bool ignoreAltColour = false;;

    int m_altColour = -1;

  //  int currentIndex; // used for NES and other fixed-palette stuff
    QString name;
    LColor() {}
    LColor(QColor col, QString n) {
        color = col;
        name = n;
    }

    unsigned short get12BitValue() {
        return color.blue()/16  | (color.green()/16)<<4  | (color.red()/16)<<8;

    }
    unsigned short get9BitValue() {
        return color.blue()/32  | (color.green()/32)<<4  | (color.red()/32)<<8;

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



#endif // LCOLOR_H
