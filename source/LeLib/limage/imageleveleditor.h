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

#ifndef IMAGELEVELEDITOR_H
#define IMAGELEVELEDITOR_H

#include "multicolorimage.h"
#include "charsetimage.h"
#include <QByteArray>
#include <QVector>
#include <QKeyEvent>
#include "source/LeLib/limage/limagemetachunk.h"

class CharmapLevel {
public:
//    uchar m_col0, m_col1, m_col2;
    QByteArray m_CharData;
    QByteArray m_CharDataHi;
    QByteArray m_ColorData;
    QByteArray m_ExtraData;
    CharmapLevel() {

    }

    void Clear(int val) {
        for (int i=0;i<m_CharData.length();i++) {
            m_CharData[i] = val;
            m_CharDataHi[i] = val;
            m_ColorData[i] = 0x5;
        }
    }



//    virtual bool isMultiColor() { return false; }

    CharmapLevel(int sizeChar, int sizeExtraData) {
        m_CharData.resize(sizeChar);
        m_CharDataHi.resize(sizeChar);
        m_ColorData.resize(sizeChar);
        m_ExtraData.resize(sizeExtraData);
        if (sizeExtraData!=0)
            m_ExtraData.fill(0);
        m_CharData.fill(0);
        m_ColorData.fill(0);

        for (int i=0;i<m_ExtraData.size();i++)
            m_ExtraData[i]=0;
        Clear(0);
    }

    QImage createImage(int size, LColorList& lst, int width, int height, bool hasBorder);

};


class CharmapGlobalData {
private:
    int m_dataSize;
    int m_levelSize;
    int m_totalSize;
    int m_headerSize;

public:
    bool m_useColors=true;
    bool m_is16bit = false;
    int m_width=40, m_height=25;
    int m_sizex, m_sizey;
    int m_colSizex=-1, m_colSizey=-1;
    int m_startx, m_starty;
    int m_extraDataSize;
    int m_dataChunks;
    int m_dataChunkSize;

    int scale = 1;
    bool m_multiChar = true;

    int dataSize() const;
    int levelSize() const;
    int totalSize() const;

    uchar m_displayMultiColor = 1;

    void Calculate()
    {
        if (m_useColors)
        if (m_colSizex <=0 || m_colSizey <=0) {
            m_colSizex = m_sizex;
            m_colSizey = m_sizey;
        }
        m_dataSize = m_width*m_height;

        m_levelSize = m_dataSize + m_extraDataSize;
        if (m_useColors)
            m_levelSize +=m_colSizex*m_colSizey + m_extraDataSize;

//        m_headerSize = 1 + 1 + 1 + 1 + 1 + 1 + 2 + 1;
        m_headerSize = 32;
       // int scale = 1;
        scale = 1;
        if (m_is16bit)
            scale = 2;
        // w/h sx/sy  stx/sty   levelSize
        m_totalSize = m_levelSize*m_sizex*m_sizey*m_is16bit + m_headerSize;
    }



    QByteArray toHeader() {
        QByteArray ba;
        ba.resize(32);
        ba.fill(0);
        ba[0] = (uint)m_sizex;
        ba[1] = (uint)m_sizey;
        ba[2] = (uint)m_width;
        ba[3] = (uint)m_height;
        ba[4] = (uint)m_startx;
        ba[5] = (uint)m_starty;
//        ba[6] = (uchar)(m_levelSize >>8)&0xFF;
//        ba[7] = (uchar)(m_levelSize&0xFF);
        ba[6] = (uint)m_dataChunks;
        ba[7] = (uint)m_dataChunkSize;
        ba[8] = (uint)m_extraDataSize;
        ba[9] = m_useColors==true ? 1:0;
        ba[10] = (uint)m_colSizex;
        ba[11] = (uint)m_colSizey;
        ba[12] = (uint)m_displayMultiColor;
        ba[13] = (uint)m_is16bit;
        return ba;
    }

    void fromHeader(QByteArray ba) {
        m_sizex = (uchar)ba[0];
        m_sizey = (uchar)ba[1];
        m_colSizex = (uchar)ba[10];
        m_colSizey = (uchar)ba[11];

//        m_colSizex = 16;
  //      m_colSizey = 10;


        m_width = (uchar)ba[2];
        m_height = (uchar)ba[3];
        m_startx = (uchar)ba[4];
        m_starty = (uchar)ba[5];

        //m_levelSize = ba[7] + ba[6]<<8;
        m_dataChunks = (uchar)ba[6];
        m_dataChunkSize = (uchar)ba[7];
        m_extraDataSize = (uchar)ba[8];

        m_displayMultiColor = ba[12];

        m_useColors = ((uchar)ba[9]==1);
        m_is16bit = ((uchar)ba[13]==1);

        if (m_useColors)
        if (m_colSizex <=0 || m_colSizey <=0) {
            m_colSizex = m_sizex;
            m_colSizey = m_sizey;
        }
    }



};







class ImageLevelEditor : public MultiColorImage
{

public:
    QPoint m_currentLevelPos = QPoint(0,0);
    CharmapLevel* m_currentLevel = nullptr;
    QVector<CharmapLevel*> m_levels;
    bool first = false;
    bool m_isRightButtonClick = false;
    CharmapLevel* getLevel(int i, int j);
    CharmapGlobalData m_meta;
    QPoint curPos;

    void ExportFrame(QFile& file, int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8) override;

    void SetBank(int bnk) override {
        m_footer.set(LImageFooter::POS_CURRENT_BANK,bnk);
        if (m_charset!=nullptr)
            m_charset->SetBank(bnk);
    }


    void SetLevel(QPoint f);
    ImageLevelEditor(LColorList::Type t);
//    void Initialize(CharmapGlobalData meta);
    void ReInitialize() override;

    void InitPens() override;

    QString GetCurrentDataString() override;

    void setBasePixel(int x, int y) override;

    void BeforeRightButton() override { m_isRightButtonClick = true;}
    void AfterRightButton()  override{m_isRightButtonClick = false;}


    void CtrlLeftShift(int x, int y ) override;
    void ShiftXY(int dx, int dy) override;

    static CharmapLevel m_copyLevel;
    void CopyChar() override;

    void PasteChar() override;


    virtual bool isNes() override {
        if (m_charset!=nullptr)
            return m_charset->isNes();
        return false;
    }
    virtual bool isSnes() override {
        if (m_charset!=nullptr)
            return m_charset->isSnes();
        return false;
    }

    void SetColor(uchar col, uchar idx) override;
    void Clear(int val) override;
    void SaveBin(QFile& f) override;
    void LoadBin(QFile& f) override;
//    void FromRaw(QByteArray& arr);
//    void ToRaw(QByteArray& arr);

    LImage* getCharset() override {
        return m_charset;
    }

    void BuildData(QTableWidget* tbl, QStringList header ) override;
    void StoreData(QTableWidget* tbl ) override;

    bool KeyPress(QKeyEvent *e) override;

    virtual QPoint GetCurrentPosInImage(float x, float y) override {
        return QPoint(x/8,y/8);
    }
    QVector<QPixmap> CreateIcons();

    QString getMetaInfo() override;
    void ExportBin(QFile &file) override;

    void setPixel(int x, int y, unsigned int color) override;
    unsigned int getPixel(int x, int y) override;
    void CopyFrom(LImage* mc) override;
    bool isMultiColor() override { return m_meta.m_displayMultiColor; }

    void onFocus() override;

    void Resize(CharmapGlobalData newMeta);

    bool PixelToPos(int x, float y, int& pos, int w, int h);

    void Fix() override; // Fix data doccuption

    void setMultiColor(bool doSet) override;
    virtual int getGridWidth() override {
        return m_meta.m_width;
    }
    virtual int getGridHeight()  override{
        return m_meta.m_height;
    }


    virtual int getDx() override;
    virtual int getDy() override;

};

#endif // IMAGELEVELEDITOR_H
