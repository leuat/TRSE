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

#ifndef LIMAGE_H
#define LIMAGE_H

#include <QString>
#include <QImage>
#include "source/LeLib/limage/lcolorlist.h"
#include <QVector3D>
#include <QGraphicsEffect>
#include <QGraphicsBlurEffect>
#include <QPoint>
#include <QFile>
#include <QDebug>
#include <QLabel>
#include <QTableWidget>


class CharsetImage;

class LImageSupports {
public:
    bool binarySave = false;
    bool binaryLoad = false;
    bool flfSave = false;
    bool flfLoad = false;
    bool asmExport = false;
    bool nesPalette = false;
    bool koalaExport = false;
    bool koalaImport = false;
    bool movieExport = false;
    bool compressedExport = false;

    bool editPalette = true;

    bool exportc = false;
    bool importc = false;

    bool displayColors = true;
    bool displayBackground = true;
    bool displayForeground = true;
    bool displayMC1 = true;
    bool displayMC2 = true;



    bool displayTimestamp = false;


};


class LImage
{
public:
    enum Type { QImageBitmap, MultiColorBitmap, HiresBitmap,
                NotSupported, Tiff, CharMapMulticolor, FullScreenChar, LevelEditor, CharmapRegular, CharMapMultiColorFixed,
              Sprites, VIC20_MultiColorbitmap, Sprites2, CGA, AMIGA320x200, AMIGA320x256,
                OK64_256x256,X16_640x480, NES};


    enum WriteType { Color, Character };
    LImage() {}
    LImage(LColorList::Type t);
    ~LImage() {
        Release();
    }
    LImageSupports m_supports;

    static unsigned char TypeToChar(Type t);
    static Type CharToType(unsigned char c);

    enum GUIType { btnLoadCharset, btnEditFullCharset, btn1x1, btn2x2,
                   btn2x2repeat, btnFlipV, btnFlipH, btnCopy, btnPaste,
                   tabLevels, tabCharset, tabData, tabSprites, tabEffects};


    QMap<QString, float> m_exportParams;
    QMap<QString, QString> m_exportParamsComments;

    QMap<GUIType, QString> m_GUIParams;

    bool m_silentExport=false;
    int m_width;
    int m_height;
    float m_scaleX = 0.6f;
    unsigned int m_border=0, m_background=0;
    Type m_type = Type::QImageBitmap;
    WriteType m_writeType = WriteType::Color;
    unsigned char m_bitMask = 0b11;
    unsigned char m_scale = 2;
    unsigned char m_noColors = 4;
    unsigned char m_minCol = 1;
    float m_importScaleX = 1;
    float m_importScaleY = 1;
    unsigned int m_currencChar;


    virtual int GetWidth() {
        return m_width;
    }
    virtual int GetHeight() {
        return m_height;
    }



    virtual void setCurrentChar(int i) {m_currencChar = i;}

    virtual CharsetImage* getCharset() { return nullptr; }

    bool renderPathGrid = false;
    unsigned char m_extraCols[4];

    LColorList m_colorList;

    virtual void FloydSteinbergDither(QImage& img, LColorList& colors, bool dither);
    virtual void OrdererdDither(QImage& img, LColorList& colors, QVector3D strength);


    virtual int getContainerCount() {return 1;}

//    virtual void AddNew(int x, int y) {};

    virtual void RenderEffect(QMap<QString, float> params) {}

    virtual void CopyChar() {}
    virtual void PasteChar() {}

  //  virtual void Delete() {}


    virtual void FlipHorizontal() {}
    virtual void FlipVertical() {}

//    virtual void Next() {}
  //  virtual void Prev() {}

    virtual void Transform(int x, int y) { }

    virtual void Fix() {} // Used to fix problems. Really? Yeah. Character level corruptions etc.

    virtual void Initialize(int width, int height) = 0;

    virtual QString GetCurrentModeString() {
        return "Full image mode";
    }

    virtual QString GetCurrentDataString() {
        return "";
    }

    virtual void Rotate(QPoint center, float angle, float scale, LImage* img);

    virtual void setPixel(int x, int y, unsigned int color) = 0;
    virtual unsigned int getPixel(int x, int y) = 0;
    virtual void SetColor(uchar col, uchar idx) {}

    virtual void LoadCharset(QString file, int skipBytes) {}

    virtual bool KeyPress(QKeyEvent *e) {return false;}
    virtual void SaveBin(QFile &file) = 0;
    virtual void LoadBin(QFile &file) = 0;

    void ApplyColor() {
       SetColor(m_extraCols[0],0);
       SetColor(m_extraCols[1],1);
       SetColor(m_extraCols[2],2);

    }

    virtual void BuildData(QTableWidget* tbl, QStringList header) {}
    virtual void StoreData(QTableWidget* tbl) {}

    virtual QPoint GetCurrentPosInImage(float x, float y) {
        return QPoint(x,y);
    }

    virtual void ExportBin(QFile &file) {}
    virtual void ImportBin(QFile &file) {}

    virtual void ExportCompressed(QString f1, QString f2) {}

    virtual void ExportMovie(QFile &file) {}


    virtual void ExportKoa(QFile &file) {}
    virtual void ImportKoa(QFile &file) {}

    virtual void Release() {}

    virtual void ApplyToLabel(QLabel* l) = 0;

    virtual void setForeground(unsigned int col) { m_border = col; }
    virtual void setBackground(unsigned int col) {m_background = col; }

    virtual void ExportC(QFile& file)  {}
    virtual void ImportC(QFile& file)  {}

    virtual void setMultiColor(bool doSet) {}

    virtual bool isMultiColor() { return false; }
    void drawLine(float x0, float y0, float x1, float y1, unsigned int col, int size);

    void Box(int x, int y, unsigned char col, int size);

    virtual void ToQImage(LColorList& lst, QImage& img, float zoom = 1, QPointF center = QPointF(160,100)) = 0;

    virtual void CopyFrom(LImage* img);

    virtual void SetCurrentType(WriteType wt) {
        m_writeType = wt;
    }

    void CopyTo(LImage* img);

    virtual void Clear() = 0;
    virtual void fromQImage(QImage* img, LColorList& lst) = 0;

    virtual void ExportAsm(QString filename)  { qDebug() << "ASM Write not supported"; }

};


#endif // LIMAGE_H
