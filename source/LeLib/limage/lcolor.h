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
#include "source/dialogcolorselect.h"

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

class LPen {
public:
    int m_colorIndex;
    enum Type {FixedSingle, SingleSelect, Dropdown,DropDownExceptAlreadySelected, DisplayAll, DisplayAllExceptAlreadySelected};
    Type m_type = FixedSingle;
    QString m_name;
    int m_and = 0xFFFF;
    QVector3D m_bpp = QVector3D(4,4,4);
    bool m_isHidden = false;
    QVector<int> m_restricted;
    QVector<QSharedPointer<LPen>> *m_pens;
    QVector<LColor>* m_colors;
    LPen() {

    }
    LPen(QVector<QSharedPointer<LPen>> *pens, QVector<LColor> *colors, int cidx) {
        m_colorIndex = cidx;
        m_pens = pens;
        m_colors = colors;
    }

    void Hide(bool hide) {
        m_isHidden = hide;
    }

    LPen(QVector<QSharedPointer<LPen>> *pens, QVector<LColor> *colors,int cidx, QString name, Type type) {
        m_colorIndex = cidx;
        m_name = name;
        m_type = type;
        m_pens = pens;
        m_colors = colors;
    }
    LPen(QVector<QSharedPointer<LPen>> *pens, QVector<LColor> *colors,int cidx, QString name, Type type, QVector3D bpp) {
        m_colorIndex = cidx;
        m_name = name;
        m_type = type;
        m_pens = pens;
        m_bpp = bpp;
        m_colors = colors;
    }
    QWidget* CreateUI(QColor col, int width,int xx,int yy,QVector<LColor>& list);

    QPixmap CreateColorIcon(QColor col, int s);

    QWidget* createButton(QColor col, int index, int width);
    QWidget* createButtonSelect(QColor col, int index, int width);
    QWidget* createComboBox(QColor col,int width, QVector<LColor> &list);
    QWidget* createGrid(QColor col,int width, QVector<LColor> &list);

    void FillComboBox(QComboBox *cmb, QVector<LColor>& list);

    void FillComboBoxRestricted(QComboBox *cmb, QVector<LColor>& list);


    void handleButtonEdit(int val, QPushButton* btn);



    int Get() {
        return m_colorIndex;
    }
};


#endif // LCOLOR_H
