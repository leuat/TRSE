#ifndef LPEN_H
#define LPEN_H

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
#include "source/LeLib/limage/lcolor.h"
#include <QComboBox>
#include "source/dialogcolorselect.h"

class LPen {
public:
    int m_colorIndex;
    enum Type {FixedSingle, SingleSelect, Dropdown,DropDownExceptAlreadySelected, DisplayAll, DisplayAllExceptAlreadySelected};
    Type m_type = FixedSingle;
    QString m_name;
    int m_dataType = 0;
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

#endif // LPEN_H
