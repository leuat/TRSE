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
#include <QVector3D>
#include <QGraphicsEffect>
#include <QGraphicsBlurEffect>
#include <QPoint>
#include <QFile>
#include <QDebug>
#include <QLabel>
#include <QTableWidget>
#include "source/LeLib/limage/limagefooter.h"
#include "source/LeLib/limage/lcolorlist.h"
// Footer contains image-state specific data

#define CHECK_BIT(var,pos) (((var) & (1<<(pos)))>>pos)

class CharsetImage;

class MetaParameter {
public:
    QString name;
    QString text;
    float value;
    float pmin;
    float pmax;
    MetaParameter(QString n, QString t, float val, float mi, float ma) {
        name = n;
        text = t;
        value = val;
        pmin = mi;
        pmax = ma;
    }
};


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
    bool tilestamp = false;
    bool editPalette = true;
    bool displayColors = true;
    bool displayCmbColors = true;

    bool exportc = false;
    bool importc = false;
    bool d800_limit = false;
    bool displayBackground = true;
    bool displayForeground = true;
    bool displayMC1 = true;
    bool displayMC2 = true;
    bool displayBank = false;

    bool displayDefaultClearButton = false;
    bool displayCharOperations = false;

    bool displayTimestamp = false;


};



class LImage : public QObject
{
Q_OBJECT


public:
    enum Type { QImageBitmap, MultiColorBitmap, HiresBitmap,
                NotSupported, Tiff, CharMapMulticolor, FullScreenChar, LevelEditor, CharmapRegular, CharMapMultiColorFixed,
              Sprites, VIC20_MultiColorbitmap, Sprites2, CGA, AMIGA320x200, AMIGA320x256, ATARI320x200,
                OK64_256x256,X16_640x480, NES, LMetaChunk, LevelEditorNES, SpritesNES, GAMEBOY, LevelEditorGameboy, HybridCharset,
                AmstradCPC, AmstradCPCGeneric, BBC, VGA, Spectrum, SNES,LevelEditorSNES,VZ200, CustomC64, JDH8, LImageGeneric, GenericSprites, CGA160x100,
                AmstradSprites, SNESGeneric, TIM, TVC, COCO3, THOMSON, TIMG, LevelEditorGeneric
              };


    enum WriteType { Color, Character };
    LImage() {}
    LImage(LColorList::Type t);
    virtual ~LImage() {
        Release();
    }
    QString m_exportMessage="";
    LImageSupports m_supports;

    LImageFooter m_footer;
    static unsigned char TypeToChar(Type t);
    static QString TypeToString(Type t);
    static Type CharToType(unsigned char c);

    enum GUIType { btnLoadCharset, btnEditFullCharset, btn1x1, btn2x2,
                   btn2x2repeat, btnFlipV, btnFlipH, btnCopy, btnPaste,
                   tabLevels, tabCharset, tabData, tabSprites, tabEffects, col1,col2,col3,col4};


    QMap<QString, float> m_exportParams;
    QMap<QString, QString> m_exportParamsComments;
    QVector<int> m_ignoreValues;

    QMap<GUIType, QString> m_GUIParams;


    virtual bool isNes() {return false;}
    virtual bool isSnes() {return false;}
    bool m_returnActualColor = true;
    float m_importScale = 1.0;
    QVector<MetaParameter*> m_metaParams;
    MetaParameter* getMetaParameter(QString name);
    virtual QString getMetaInfo() { return "";}

    virtual void InitAfterFooter() {}
    virtual LColorList::Type getColorType() {
        return m_colorList.m_type;
    }
    bool m_fixMultiColorYSave = false;

    virtual QByteArray getDirArt() { return QByteArray();}
    virtual QByteArray getBinaryExportData() { return QByteArray();}

    virtual void CopyImageData(LImage* img);
    virtual int getCharWidthDisplay();
    virtual int getCharHeightDisplay();

    int m_charHeightDisplay = 25;
    int m_charWidthDisplay = 40;

    int m_gridWidthDisplay = 40;


    virtual QStringList getBankNames();
    bool m_updatePaletteInternal = true;
    virtual void SetBank(int bnk) {
        m_footer.set(LImageFooter::POS_CURRENT_BANK,bnk);
    }

    virtual void onFocus()  {

    }

    virtual void ReInitialize() {

    }

    virtual int getBackground() {
        return 0;
    }

    virtual void ExportFrame(QFile& file, int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8) {}

    virtual void PBMExport(QFile& file, int p1, int p2, int p3, int p4) {}

    virtual void VBMExport(QFile& file, int p1, int p2, int p3, int p4) {}
    virtual void VBMExportColor(QFile& file, int p1, int p2, int p3, int p4) {}
    virtual void VBMExportChunk(QFile& file, int p1, int p2, int p3, int p4) {}
    virtual void VBMCompileChunk(QTextStream& f, QString procName, QString pointerName, QString asmOperation, int p1, int p2, int p3, int p4) {}

    virtual void CopySingleChar(LImage* src, int srcChar, int dstChar) {qDebug() << "CopySingleChar not implemented";}

    virtual void ExportBlackWhite(QFile& file, int p1, int p2, int p3, int p4, int type) {}


    virtual void SetForceD800Color(int val) {

    }

    virtual QStringList SpriteCompiler(QString name, QString currentDir, QString src, QString dst, int x, int y, int w, int h, QString pparam) {return QStringList();}

    virtual void BeforeRightButton() {}
    virtual void AfterRightButton() {}
    virtual void CtrlLeftShift(int x, int y );

    int m_constrainDisplay = -1;
    bool m_silentExport=false;
    int m_width;
    int m_height;
    bool m_isHybridTemp;

    int m_forceD800Color = -1;


    void PushHybrid();
    void PopHybrid();

    float m_scaleX = 1.0f;

//    unsigned int m_border=0, m_background=0;

    Type m_type = Type::QImageBitmap;
    WriteType m_classvariableType = WriteType::Color;
    unsigned char m_bitMask = 0b11;
    unsigned char m_scale = 2;
    unsigned char m_noColors = 4;
    unsigned char m_minCol = 1;
    double m_importScaleX = 1;
    double m_importScaleY = 1;
    bool m_clearWithCurrentChar = false;
    unsigned int m_currentChar;
    // Updates charset position in editor
    bool m_updateCharsetPosition = false;
    bool m_forcePaintColorAndChar = true;

    static QPoint m_copySize;
    static uchar m_copy[];
    QPoint m_basePixel;

    virtual int getGridWidth() {
//        return getCharWidthDisplay();
        return m_gridWidthDisplay;
    }
    virtual int getGridHeight() {
        return getCharHeightDisplay();
    }

//    enum Mode{ FULL_IMAGE, CHARSET1x1, CHARSET2x2, CHARSET2x2_REPEAT};
  //  Mode m_currentMode = FULL_IMAGE;
    QString GetCurrentModeString();


//    Mode m_copyFromMode;

    static bool m_hasCopy;// = false;


    virtual int GetWidth() {
        return m_width;
    }
    virtual int GetHeight() {
        return m_height;
    }

    virtual QPoint getActualPixelWidth() {
        return QPoint(m_width *m_scale,m_height);

    }


    virtual void SpritePacker(LImage* in, QByteArray& rawDataOut, QByteArray& sprData, int x, int y, int w, int h, int comp, int& noChars) {
        qDebug() << "LImage::SpritePacker not implemented for this image type : " <<m_type;
    }
    int m_charWidth=40;
    int m_charHeight=25;

    void ToQPixMaps(QVector<QPixmap> &map)
    {
        map.clear();
        for (int i=0;i<m_charWidth*m_charHeight;i++) {
            map.append(ToQPixMap(i));
        }
    }
    virtual QPixmap ToQPixMap(int chr) {return QPixmap();}

    virtual void setCurrentChar(int i) {m_currentChar = i;}

    virtual LImage* getCharset() { return nullptr; }

    bool renderPathGrid = false;
//    unsigned char m_extraCols[4];

    LColorList m_colorList;


    void PerformConstrainColours(QVector<int>& cols) {
        m_colorList.ConstrainColours(cols);
    }


    virtual void SetHybridMode(bool checked) {
        m_colorList.m_isHybridMode = checked;

    }

    virtual void ConstrainColours(QVector<int>& cols) {


    }


    virtual void FloydSteinbergDither(QImage& img, LColorList& colors, bool dither, double strength);
    virtual void OrdererdDither(QImage& img, LColorList& colors, QVector3D strength, QPoint size,float gamma);

    virtual void InitPens();

    virtual int getContainerCount() {return 1;}

//    virtual void AddNew(int x, int y) {};

    virtual void RenderEffect(QMap<QString, float> params) {}

    virtual void CopyChar();

    void SetGridSize(QString size);
    QString getGridSize();

    virtual void PasteChar();

  //  virtual void Delete() {}


    virtual void FlipHorizontal();
    virtual void FlipVertical();

//    virtual void Next() {}
  //  virtual void Prev() {}

    virtual void Transform(int x, int y);

    virtual void Fix() {} // Used to fix problems. Really? Yeah. Character level corruptions etc.

    virtual void Initialize(int width, int height) = 0;


    virtual QString GetCurrentDataString() {
        return "";
    }

    virtual void Rotate(QPoint center, float angle, float scale, LImage* img);

    virtual void setPixel(int x, int y, unsigned int color) = 0;
    virtual void setBasePixel(int x, int y) {}
    virtual unsigned int getPixel(int x, int y) = 0;
    virtual void SetColor(uchar col, uchar idx) {}

    virtual void LoadCharset(QString file, int skipBytes) {

    }

    virtual bool KeyPress(QKeyEvent *e) {return false;}
    virtual void SaveBin(QFile &file) = 0;
    virtual void LoadBin(QFile &file) = 0;

  /*  virtual void ApplyColor() {
       SetColor(m_extraCols[0],0);
       SetColor(m_extraCols[1],1);
       SetColor(m_extraCols[2],2);

    }
*/
    virtual void SetPalette(int pal) {}

    virtual void BuildData(QTableWidget* tbl, QStringList header) {}
    virtual void StoreData(QTableWidget* tbl) {}

    virtual QPoint GetCurrentPosInImage(float x, float y) {
        return QPoint(x,y);
    }
    virtual QString getSpriteInfo() { return "";}

    virtual void ExportSubregion(QString outfile,int x, int y, int w, int h,int type);

    virtual void ExportBin(QFile &file) {}
    virtual void ImportBin(QFile &file) {}

    virtual QStringList getPaletteNames() {
        return QStringList() <<"Tiles 1"<<"Tiles 2"<<"Tiles 3"<<"Tiles 4"<<"Sprites 1"<<"Sprites 2"<<"Sprites 3"<<"Sprites 4";;
    }


    virtual void ExportRGB8Palette(QString filename);

    virtual void ExportCompressed(QString f1, QString f2, QString f3) {}

    virtual void ExportMovie(QFile &file) {}

    virtual void Invert() {}

    virtual void ExportKoa(QFile &file) {}
    virtual void ImportKoa(QFile &file) {}

    virtual void Release() {}

    virtual void ApplyToLabel(QLabel* l) = 0;

    virtual void setBackground(unsigned int col) {  }

    virtual void ExportC(QFile& file)  {}
    virtual void ImportC(QFile& file)  {}

    virtual void setMultiColor(bool doSet) {}
    void RBox(int x0, int y0, int x1, int y1, unsigned char col, int size);

    virtual bool isMultiColor() { return false; }
    void drawLine(float x0, float y0, float x1, float y1, unsigned int col, int size);
    void drawCircle(float x0, float y0, float r, float r0, unsigned int col);

    bool usePens = false;

    void Box(int x, int y, unsigned char col, int size);

    virtual void ToQImage(LColorList& lst, QImage& img, double zoom = 1, QPointF center = QPointF(160,100)) = 0;

    virtual void SavePalette() {}

    virtual void CopyFrom(LImage* img);

    virtual void Duplicate() {}

    virtual void SaveCurrentPaletteToPPU();

    virtual int getCharAtPos(QPoint p, float zoom, QPointF center) { return 0;}

    virtual void SetCurrentType(WriteType wt) {
        m_classvariableType = wt;
    }

    void CopyTo(LImage* img);
    virtual void FromLImageQImage(LImage* other) {}
    virtual void CreateMagicalCharset() {}

    virtual void Clear(int val=0);
    virtual void fromQImage(QImage* img, LColorList& lst) = 0;

    virtual void ExportAsm(QString filename)  { qDebug() << "ASM Write not supported"; }
    virtual void ShiftXY(int x, int y);

    void EnsureSystemColours();

    virtual int getDx();
    virtual int getDy();

signals:
    void emitImportRom();
};


#endif // LIMAGE_H
