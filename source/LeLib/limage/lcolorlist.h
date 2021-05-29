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
//#ifndef CLI_VERSION
#include <QComboBox>
#include <QPushButton>
#include <QLayout>
#include <QSharedPointer>
//#endif
#include <QIcon>
#include <QPixmap>
#include "source/LeLib/data.h"
#include <math.h>
//#include <QStringLiteral>
#include <QLabel>

#include "lcolor.h"
#include "lpen.h"
#include "limagefooter.h"

class LColorList : public QObject
{
    Q_OBJECT
private:

    QVector<int> m_multicolors;
    int m_currentType = 0;
    QVector<QSharedPointer<LPen>> m_pens;
    bool firstTime=true;

public:
//    uchar m_nesCols[4];    // OBSOLETE REWRITE
//    bool m_isMulticolor = true; // OBSOLETE REWRITE
    QVector<LColor> m_list;
    bool m_isCharset = false;
    bool m_isHybridMode = false;
    bool m_supportsFooterPen = false;
    bool m_isLevelEditor = false;
    QVector3D m_bpp = QVector3D(8,8,8);
    enum Type{ NES, C64, C64_ORG, CGA1_LOW, CGA1_HIGH, CGA2_LOW, CGA2_HIGH, UNSUPPORTED, TIFF, VIC20, PICO8,OK64,X16, AMSTRADCPC, BBC, VGA, SPECTRUM };
    bool m_selectClosestFromPen = true;
    QVector<int> m_enabledColors;
    QByteArray m_customPalette;
    Type m_type = Type::C64;
    LColorList();
    ~LColorList();

    QByteArray m_nesPPU;
    int m_curPal = 0;
    bool m_ignoreSetIsMulti = false;
    LColor& get(int i);
    QColor m_cblack = QColor(0,0,0,255);

    LColor m_black = LColor(m_cblack,"black");

    QVector<int> getPenList();

    void SetIsMulticolor(bool mult);

    void SetMulticolor(int index, int col);
    void fromArrayList(QVector<int> colors);

    void SetPPUColors(char c1, int idx);
    QSize m_windowSize;
    void InitPalettePens(int cnt);

    static unsigned char TypeToChar(Type t);
    static Type CharToType(unsigned char c);

    QVector<QPushButton*> m_buttonsImport;
    QVector<QPushButton*> m_buttonsEdit;
    int m_constrainDisplay = -1;

    void ConstrainTo(int max);
    void SetGreyscale(QVector3D base, bool inverted);
    void SetTwoColors(QVector3D base1, QVector3D base2);
    Metric* m_metric = nullptr;
    void EnableColors(QVector<int>& cols);
    void EnableAllColors(bool val);

    void CreateUIOld(QLayout* ly, int type, QSize windowSize);

    void GeneratePaletteFromQImage(QImage& img);

    int getNoBitplanes();
    void setNoBitplanes(int bpl);
    QByteArray toArray();
    void fromArray(QByteArray& d);

    void Initialize(Type t);

    void SetC64Pens(bool m_isMulticolor, bool m_isCharset);
    void SetVIC20Pens(bool m_isMulticolor);
    void SetC64SpritePen(bool m_isMulticolor);

    void InitNESPens();


    QPixmap CreateColorIcon(int col, int s);

    void CopyFrom(LColorList* other);
    void CopyFromKeep(LColorList* other);
    void InitC64_org();
    void InitC64();
    void InitSPECTRUM();
    void InitC64Multicolor();
    void InitPICO8();
    void InitVIC20();
    void InitCGA1_LOW();
    void InitCGA1_HIGH();
    void InitOK64();
    void InitVGA();
    void InitNES();
    void InitNES4();
    void InitCGA2_LOW();
    void InitCGA2_HIGH();
    void InitAmstradCPC();
    void InitBBC(int noColors);
    void UpdateColors();
    void LoadFromFile(QString fileName);

    int count() {
        return m_pens.count();
    }
    void DefaultPen(LPen::Type type);
    void DefaultPen(LPen::Type type, int cnt);

    QColor getClosestColor(QColor col, int& winner);

    int getPen(int pcol);
    int getPenIndex(int pcol);
    void setPen(int pcol, int colorIndex);

    QColor getPenColour(int pcol);

    void ExportAmigaPalette(QString filename);
    void ExportAtariSTPalette(QString filename);


    void PenToFooter(LImageFooter* footer);
    void FooterToPen(LImageFooter* footer);

    void ConstrainColours(QVector<int>& cols);

    void ConstrainColour(int pen, bool constrain);
    // Scheduled to be deprecated
    void UpdateCommodoreRestrictions(uchar* extraCols);


    void FillComboBox(QComboBox* cmb);
    void FillComboBoxRestricted(QComboBox* cmb, int a, int b);

    LColor m_background;

    int getIndex(QColor c);

    void handleButtonEdit(int val, int data, QPushButton* btn);

    QLayout* m_layout = nullptr;
    void CreateUI(QLayout* ly, int type);
    void CreateUI(QLayout* ly, int type, QSize windowSize);
    void UpdateUI();
public slots:
    void handleButtonImport(int data);


signals:
    void colorValueChanged();

};

#endif // LCOLORLIST_H
