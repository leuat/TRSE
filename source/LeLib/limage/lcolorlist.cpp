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

#include "lcolorlist.h"
#include <QDebug>
#include <QPalette>
#include <QSignalMapper>
#include "source/LeLib/util/util.h"
LColorList::LColorList()
{
}


LColorList::~LColorList()
{
    if (m_metric)
        delete m_metric;
}

LColor &LColorList::get(int i) {
    if (i<m_list.count() && i>=0)
        return m_list[i];
    if (m_list.count()!=0) return m_list[0];
    return m_black;
}

QVector<int> LColorList::getPenList()
{
    QVector<int> lst;
    for (int i=0;i<m_pens.count();i++)
        lst.append(m_pens.at(i)->m_colorIndex);
    return lst;
}

void LColorList::SetIsMulticolor(bool mult)
{
//    m_isMulticolor = mult;
    if (m_list.count()==0)
        return;

    for (int i=0;i<m_list.count();i++)
        m_list[i].ignoreAltColour = false;

    if (m_ignoreSetIsMulti)
        for (int i=0;i<8;i++)
            m_list[i+8].displayList = false;

    if (mult && !m_ignoreSetIsMulti) {
        if (m_type==LColorList::C64 || m_type==LColorList::VIC20) {
            for (int i=0;i<8;i++) {
                m_list[i].ignoreAltColour = true;
            }
            if (m_multicolors.count()>=1) {
                if (m_multicolors[0]<m_list.count() && m_multicolors[0]>8)
                m_list[ m_multicolors[0]&7].ignoreAltColour = false;
            }
            if (m_multicolors.count()>=2) {
  //              qDebug() << m_multicolors[2];

                if (m_multicolors[1]<m_list.count() && m_multicolors[1]>8)
                m_list[ m_multicolors[1]&7].ignoreAltColour = false;
            }
        }
    }
}

void LColorList::SetMulticolor(int index, int col)
{
    int i = index-1;
    if (i<0) return;
    if (i>=m_multicolors.count()) {
        m_multicolors.resize(i+1);
    }
    m_multicolors[i] = col;
    if (m_layout!=nullptr)
        CreateUI(m_layout,m_currentType,m_windowSize);

}
void LColorList::UpdateUI()
{
    CreateUI(m_layout,m_currentType,m_windowSize);
}


void LColorList::SetPPUColors(char c1, int idx)
{
    m_nesPPU[1+m_curPal*4+idx] = c1;
}

unsigned char LColorList::TypeToChar(LColorList::Type t)
{
    if (t==C64)
        return 0;
    if (t==C64_ORG)
        return 1;
    if (t==CGA1_LOW)
        return 2;
  if (t==CGA1_HIGH)
      return 3;
  if (t==CGA2_LOW)
      return 4;
  if (t==CGA2_HIGH)
      return 5;
  if (t==VIC20)
      return 6;
  if (t==PICO8)
      return 7;
  if (t==OK64)
      return 8;
  if (t==X16)
      return 9;
  if (t==NES)
      return 10;
  if (t==AMSTRADCPC)
      return 11;
  if (t==BBC)
      return 12;
  if (t==VGA)
      return 13;
  if (t==SPECTRUM)
      return 14;

  return 255;
}

LColorList::Type LColorList::CharToType(unsigned char c)
{
    if (c==0)
        return C64;
    if (c==1)
        return C64_ORG;
    if (c==2)
        return CGA1_LOW;
    if (c==3)
        return CGA1_HIGH;
    if (c==4)
        return CGA2_LOW;
    if (c==5)
        return CGA2_HIGH;
    if (c==6)
        return VIC20;
    if (c==7)
        return PICO8;
    if (c==8)
        return OK64;
    if (c==9)
        return X16;
    if (c==10)
        return NES;
    if (c==11)
        return AMSTRADCPC;
    if (c==12)
        return BBC;
    if (c==13)
        return VGA;
    if (c==14)
        return SPECTRUM;

    return UNSUPPORTED;

}

void LColorList::ConstrainTo(int max)
{
    for (int i=0;i<m_list.count();i++) {
        m_list[i].inUse = i<=max;
    }
}

void LColorList::SetGreyscale(QVector3D base, bool inverted)
{
    for (int i=0;i<m_list.count();i++) {
        float scale = (i+1)/((float)m_list.count());
        if (inverted) scale = 1-scale;
        QVector3D col = base*scale*255;
        m_list[i].color = Util::toColor(col);
    }
}

void LColorList::SetTwoColors(QVector3D base1, QVector3D base2)
{
    int cnt = m_list.count();
    int splt = cnt/3;
    int cur = 0;
    for (int i=0;i<splt;i++) {
        float scale = (i+1)/((float)splt);
        QVector3D col = base1*scale;
        m_list[cur].color = Util::toColor(col);
        col = (base2*scale + base1*(1-scale));
        if (cur+splt<cnt)
            m_list[cur+splt].color = Util::toColor(col);
        col = (base2*(1-scale));
        if (cur+2*splt<cnt)
            m_list[cur+2*splt].color = Util::toColor(col);

        cur++;
    }

}

void LColorList::EnableColors(QVector<int> &cols)
{
    for (int i=0;i<m_list.count();i++) {
        if (cols.contains(i))
            m_list[i].inUse = true;
        else
            m_list[i].inUse = false;

    }
    m_enabledColors = cols;

}

void LColorList::EnableAllColors(bool val)
{
    m_enabledColors.clear();
    for (int i=0;i<m_list.count();i++) {
            m_list[i].inUse = val;
            if (val)
                m_enabledColors.append(i);
    }
}

bool sortColors(const QVector3D &s1, const QVector3D &s2)
{
    return s1.x()+s1.y()+s1.z() < s2.x()+s2.y()+s2.z();
}
void LColorList::GeneratePaletteFromQImage(QImage &img)
{
    if (img.format()==QImage::Format_Indexed8) {
//        qDebug() << "INDEXED colors : "<<img.colorTable().count();
        for (int i=0;i<img.colorTable().count();i++) {
            if (i<m_list.count())
              m_list[i].color = QColor(img.colorTable()[i]);
        }

        return;
    }


    QVector<QVector3D> m_colorList;
    for (int i=0;i<10000;i++) {
            int x  = rand()%img.width();
            int y  = rand()%img.height();
            QVector3D c1 = Util::fromColor(img.pixelColor(x,y));
            bool isNew = true;
            for (QVector3D& o : m_colorList)
                if ((o-c1).lengthSquared()<0.001)
                    isNew = false;
            if (isNew)
                m_colorList.append(c1);
//        qDebug() << y ;
    }

/*    for (int y=0;y<img.height();y++) {
        for (int x=0;x<img.width();x++) {
            QVector3D c1 = Util::fromColor(img.pixelColor(x,y));
            bool isNew = true;
            for (QVector3D& o : m_colorList)
                if ((o-c1).lengthSquared()<0.001)
                    isNew = false;
            if (isNew)
                m_colorList.append(c1);
        }
        qDebug() << y ;
    }
  */

    qDebug() << " # unique colours : " << m_colorList.count();

    int reducedCount = m_list.count();
    float distance = 1;
    while (m_colorList.count()>reducedCount) {
        QVector<QVector3D> m_newList;
        for (int i=0;i<m_colorList.count();i++) {
            bool isNew = true;
            for (QVector3D& o : m_newList)
                if ((o-m_colorList[i]).length()<distance)
                    isNew = false;

            if (isNew)
                m_newList.append(m_colorList[i]);

        }
  //      qDebug() << "Reduced from " << m_colorList.count() << " to " << m_newList.count();
        m_colorList = m_newList;
        distance=distance+1;
    }

    // Sort colors
   std::sort(m_colorList.begin(), m_colorList.end(), sortColors);
    for (int i=0;i<m_colorList.count();i++) {
        m_list[i].color = Util::toColor(m_colorList[i]);
    }



}

int LColorList::getNoBitplanes() {
    if (m_list.count()==2) return 1;
    if (m_list.count()==4) return 2;
    if (m_list.count()==8) return 3;
    if (m_list.count()==16) return 4;
    if (m_list.count()==32) return 5;
    if (m_list.count()==64) return 6;
    if (m_list.count()==128) return 7;
    if (m_list.count()==256) return 8;
    return 8;
}

void LColorList::setNoBitplanes(int bpl)
{
    m_list.resize(pow(2,bpl));
    DefaultPen(LPen::SingleSelect);
}

QByteArray LColorList::toArray()
{
    QByteArray data;
    data.resize(m_list.count()*3+1);
    data[0] = m_list.count();//(char)getNoBitplanes();
    int i = 0;
    for (LColor c: m_list) {
        data[3*i+0+1] = (uchar)c.color.red();
        data[3*i+1+1] = (uchar)c.color.green();
        data[3*i+2+1] = (uchar)c.color.blue();
        i++;

    }

    return data;
}

void LColorList::fromArray(QByteArray &d)
{
    int size = (uchar)d[0];
    int shift = 1;
  //  qDebug() << size;
    if (size==0) {
        size = 256;
        shift = 0;
//        qDebug() << "HERE";
    }
    m_list.clear();
 //    setNoBitplanes(size);
    for (int i=0;i<size;i++) {
        QColor col((unsigned char)d[3*i+0+shift],
                (unsigned char)d[3*i+1+shift],
                (unsigned char)d[3*i+2+shift]);
        m_list.append(LColor(col,""));
    }
}

void LColorList::Initialize(Type t)
{
    m_type = t;


    if (m_type == Type::C64)
        InitC64();
    if (m_type == Type::C64_ORG)
        InitC64_org();
    if (m_type == Type::CGA1_HIGH)
        InitCGA1_HIGH();
    if (m_type == Type::CGA1_LOW)
        InitCGA1_LOW();
    if (m_type == Type::CGA2_HIGH)
        InitCGA2_HIGH();
    if (m_type == Type::CGA2_LOW)
        InitCGA2_LOW();
    if (m_type == Type::TIFF)
        InitC64();
    if (m_type == Type::VIC20)
        InitVIC20();
    if (m_type == Type::PICO8)
        InitPICO8();
    if (m_type == Type::OK64)
        InitOK64();
    if (m_type == Type::NES)
        InitNES();
    if (m_type == Type::X16)
        InitOK64();
    if (m_type == Type::AMSTRADCPC)
        InitAmstradCPC();
    if (m_type == Type::BBC)
        InitBBC(16);
    if (m_type == Type::VGA)
        InitVGA();
    if (m_type == Type::SPECTRUM)
        InitSPECTRUM();




    m_metric = new LinearMetric();

}

void LColorList::SetC64SpritePen(bool m_isMulticolor) {
    QVector<int> oldList = getPenList();
    // Make sure old data is kept!
    for (int i=0;i<4;i++) {
        if (i>=oldList.count())
            oldList.append(i);

    }
    m_pens.clear();
//    qDebug()<< "OLD LIST " <<oldList;

        m_pens.append(QSharedPointer<LPen>(new LPen(&m_pens,&m_list,oldList[0],"Background",LPen::Dropdown)));
        m_pens.append(QSharedPointer<LPen>(new LPen(&m_pens,&m_list,oldList[1],"Multicolor 1",LPen::Dropdown)));
        m_pens.append(QSharedPointer<LPen>(new LPen(&m_pens,&m_list,oldList[2],"Multicolor 2",LPen::Dropdown)));

        m_pens.append(QSharedPointer<LPen>(new LPen(&m_pens,&m_list,oldList[3],"Free colour",LPen::DisplayAll)));

    if (!m_isMulticolor) {
        m_pens[1]->Hide(true);
        m_pens[2]->Hide(true);
    }
}


void LColorList::SetC64Pens(bool m_isMulticolor, bool m_isCharset)
{
    QVector<int> oldList = getPenList();
//    qDebug() << "In SetC64Pens "<<oldList<<this;
    // Make sure old data is kept!
    for (int i=0;i<4;i++) {
        if (i>=oldList.count())
            oldList.append(i);
    }
 //   qDebug()<< "OLD LIST " <<oldList;
    m_pens.clear();
    LPen::Type type = LPen::DisplayAllExceptAlreadySelected;

    if ((m_isMulticolor && m_isCharset) || m_isHybridMode)
        type = LPen::DisplayAll;


    if (m_type==C64) {
        m_pens.append(QSharedPointer<LPen>(new LPen(&m_pens,&m_list,oldList[0],"Background",LPen::Dropdown)));
        m_pens.append(QSharedPointer<LPen>(new LPen(&m_pens,&m_list,oldList[1],"Multicolor 1",LPen::Dropdown)));
        m_pens.append(QSharedPointer<LPen>(new LPen(&m_pens,&m_list,oldList[2],"Multicolor 2",LPen::Dropdown)));

        m_pens.append(QSharedPointer<LPen>(new LPen(&m_pens,&m_list,oldList[3],"Free colour",type)));
    }
    if (m_type==VIC20) {
        m_pens.append(QSharedPointer<LPen>(new LPen(&m_pens,&m_list,oldList[0],"Background",LPen::Dropdown)));
        m_pens.append(QSharedPointer<LPen>(new LPen(&m_pens,&m_list,oldList[1],"AUX",LPen::Dropdown)));
        m_pens.append(QSharedPointer<LPen>(new LPen(&m_pens,&m_list,oldList[2],"Border",LPen::Dropdown)));
        m_pens.append(QSharedPointer<LPen>(new LPen(&m_pens,&m_list,oldList[3],"Char colour",type)));
    }


    if (!m_isMulticolor && !m_isHybridMode) {
        m_pens[1]->Hide(true);
        m_pens[2]->Hide(true);
    }
    else { // IS multicolor
        if (m_type == VIC20 && !m_isHybridMode) {
            m_pens[3]->m_restricted = QVector<int>() << 8<<9<<10<<11<<12<<13<<14<<15;
            m_pens[3]->m_and =0x7;
        }

        if (m_type==C64) {
            if (m_isCharset && !m_isHybridMode && !m_isLevelEditor ) {
                m_pens[3]->m_restricted = QVector<int>() << 8<<9<<10<<11<<12<<13<<14<<15;
                m_pens[3]->m_and =0x7;

            }
            if (!m_isCharset && m_isMulticolor) {
                m_pens[1]->Hide(true);
                m_pens[2]->Hide(true);
                //m_pens[3]->m_and =0x255;

            }
        }
 //       if (m_type == C64)

    }

}

void LColorList::SetVIC20Pens(bool m_isMulticolor)
{
    QVector<int> oldList = getPenList();
    // Make sure old data is kept!
    for (int i=0;i<4;i++) {
        if (i>=oldList.count())
            oldList.append(i);
    }
    m_pens.clear();
    LPen::Type type = LPen::DisplayAllExceptAlreadySelected;

    if ((m_isMulticolor && m_isCharset) || m_isHybridMode)
        type = LPen::DisplayAll;


    bool allZero = true;
    for (auto v : oldList)
        if (v!=0)
            allZero = false;

//    qDebug() << oldList;
    if (allZero)
        return;

    m_pens.append(QSharedPointer<LPen>(new LPen(&m_pens,&m_list,oldList[0],"Background",LPen::Dropdown)));
    m_pens.append(QSharedPointer<LPen>(new LPen(&m_pens,&m_list,oldList[1],"AUX",LPen::Dropdown)));
//    qDebug() << "BORDER:  "<<oldList[1];
    m_pens.append(QSharedPointer<LPen>(new LPen(&m_pens,&m_list,oldList[2],"Border",LPen::Dropdown)));
    m_pens.append(QSharedPointer<LPen>(new LPen(&m_pens,&m_list,oldList[3],"Char colour",type)));




    if (!m_isMulticolor && !m_isHybridMode) {
        m_pens[1]->Hide(true);
        m_pens[2]->Hide(true);
    }
    else { // IS multicolor
        if (m_type == VIC20 && !m_isHybridMode) {
            //m_pens[3]->m_restricted = QVector<int>() << 8<<9<<10<<11<<12<<13<<14<<15;
            m_pens[3]->m_and =0x7;
        }
 //       if (m_type == C64)

    }

}

void LColorList::InitNESPens()
{
    QVector<int> oldList = getPenList();
    // Make sure old data is kept!
    for (int i=0;i<4;i++) {
        if (i>=oldList.count())
            oldList.append(i);
    }
    m_pens.clear();
    m_pens.append(QSharedPointer<LPen>(new LPen(&m_pens,&m_list,oldList[0],"Color 0",LPen::Dropdown)));
    m_pens.append(QSharedPointer<LPen>(new LPen(&m_pens,&m_list,oldList[1],"Color 1",LPen::Dropdown)));
    m_pens.append(QSharedPointer<LPen>(new LPen(&m_pens,&m_list,oldList[2],"Color 2",LPen::Dropdown)));
    m_pens.append(QSharedPointer<LPen>(new LPen(&m_pens,&m_list,oldList[3],"Color 3",LPen::Dropdown)));

}

QPixmap LColorList::CreateColorIcon(int col, int s)
{
    QImage img(s,s,QImage::Format_RGB32);
    int c2 = col;
    if (m_list[col].m_altColour!=-1 && (!m_list[col].ignoreAltColour))
        c2 = m_list[col].m_altColour;
    if (!m_list[col].color.isValid())
        m_list[col].color = QColor(0,0,0);


    for (int y=0;y<s;y++)
    for (int x=0;x<s;x++) {
        if (s-1-y>x)
            img.setPixelColor(x,y, m_list[col].color);
        else
            img.setPixelColor(x,y, m_list[c2].color);


    }
    return QPixmap::fromImage(img);

}

void LColorList::CopyFrom(LColorList *other)
{

    m_list.resize(other->m_list.count());
    for (int i=0;i<m_list.count();i++)
        m_list[i] = other->m_list[i];

    m_pens.resize(other->m_pens.count());
    for (int i=0;i<m_pens.count();i++) {
        m_pens[i] = other->m_pens[i];
    }
    m_isLevelEditor = other->m_isLevelEditor;


}

void LColorList::CopyFromKeep(LColorList *other)
{

    m_isLevelEditor = other->m_isLevelEditor;

    m_list.resize(other->m_list.count());
    for (int i=0;i<m_list.count();i++)
        m_list[i] = other->m_list[i];

    QVector<int> vals;
    for (auto p:m_pens)
        vals<< p->m_colorIndex;
    m_pens.resize(other->m_pens.count());
    for (int i=0;i<m_pens.count();i++) {
        m_pens[i] = other->m_pens[i];
        m_pens[i]->m_colorIndex = vals[i];
    }



}

void LColorList::InitC64_org()
{
    m_list.clear();
    m_list.append(LColor(QColor(0,0,0),""));
    m_list.append(LColor(QColor(255,255,255),""));
    m_list.append(LColor(QColor(136,0,0),""));
    m_list.append(LColor(QColor(170,255,238),""));
    m_list.append(LColor(QColor(204,68,204),""));
    m_list.append(LColor(QColor(0,204,85),""));
    m_list.append(LColor(QColor(0,0,170),""));
    m_list.append(LColor(QColor(238,238,119),""));
    m_list.append(LColor(QColor(221,136,85),""));
    m_list.append(LColor(QColor(102,68,0),""));
    m_list.append(LColor(QColor(255,119,119),""));
    m_list.append(LColor(QColor(51,51,51),""));
    m_list.append(LColor(QColor(119,119,119),""));
    m_list.append(LColor(QColor(170,255,102),""));
    m_list.append(LColor(QColor(0,136,255),""));
    m_list.append(LColor(QColor(187,187,198),""));
    DefaultPen(LPen::FixedSingle);
}

void LColorList::InitC64()
{
    m_list.clear();
    m_list.append(LColor(QColor(0x0, 0x0, 0x0),""));
    m_list.append(LColor(QColor(0xFF, 0xFF, 0xFF),""));
    m_list.append(LColor(QColor(0x68, 0x37, 0x2b),""));
    m_list.append(LColor(QColor(0x70, 0xa4, 0xb2),""));
    m_list.append(LColor(QColor(0x6f, 0x3d, 0x86),""));
    m_list.append(LColor(QColor(0x58, 0x8d, 0x43),""));
    m_list.append(LColor(QColor(0x35, 0x28, 0x79),""));
    m_list.append(LColor(QColor(0xb8, 0xc7, 0x6F),""));
 //   m_list.append(LColor(QColor(0x6f, 0x4f, 0x25),"")); // INTERNAL ORANGE
    m_list.append(LColor(QColor(0x90, 0x5f, 0x25),"")); /// ALT ORANGE
    m_list.append(LColor(QColor(0x43, 0x39, 0x00),""));
    m_list.append(LColor(QColor(0x9a, 0x67, 0x59),""));
    m_list.append(LColor(QColor(0x44, 0x44, 0x44),""));
    m_list.append(LColor(QColor(0x6c, 0x6c, 0x6c),""));
    m_list.append(LColor(QColor(0x9a, 0xd2, 0x84),""));
    m_list.append(LColor(QColor(0x6c, 0x5e, 0xb5),""));
    m_list.append(LColor(QColor(0x95, 0x95, 0x95),""));



    for (int i=0;i<8;i++)
        m_list[i].m_altColour = i+8;


    m_background = m_list[0];
//    DefaultPen();
    m_pens.clear();

}

void LColorList::InitSPECTRUM()
{
    m_list.clear();
    m_list.append(LColor(QColor(0x0, 0x0, 0x0),""));

    m_list.append(LColor(QColor(0x00, 0x22, 0xc7),""));

    m_list.append(LColor(QColor(0xd6, 0x28, 0x16),""));

    m_list.append(LColor(QColor(0xd4, 0x33, 0xc7),""));

    m_list.append(LColor(QColor(0x0, 0xc5, 0x25),""));

    m_list.append(LColor(QColor(0x0, 0xc7, 0xc9),""));

    m_list.append(LColor(QColor(0xcc, 0xc8, 0x2a),""));

    m_list.append(LColor(QColor(0xca, 0xca, 0xca),""));

    m_list.append(LColor(QColor(0x0, 0x0, 0x0),""));
    m_list.append(LColor(QColor(0x0, 0x2b, 0xfb),""));
    m_list.append(LColor(QColor(0xff, 0x33, 0x1c),""));
    m_list.append(LColor(QColor(0xff, 0x40, 0xfc),""));
    m_list.append(LColor(QColor(0x0, 0xf9, 0x2f),""));
    m_list.append(LColor(QColor(0x0, 0xfb, 0xfe),""));
    m_list.append(LColor(QColor(0xff, 0xfc, 0x36),""));
    m_list.append(LColor(QColor(0xff, 0xff, 0xff),""));

//    m_background = m_list[0];
  //  DefaultPen(LPen::FixedSingle);
    QVector<int> oldList = getPenList();
    if (oldList.count()<=2)
        oldList <<0<<1;
    m_pens.clear();
    m_pens.append(QSharedPointer<LPen>(new LPen(&m_pens,&m_list,oldList[0],"Background",LPen::Dropdown)));
    auto type = LPen::DisplayAll;
    m_pens.append(QSharedPointer<LPen>(new LPen(&m_pens,&m_list,oldList[1],"Free colour",type)));

}

void LColorList::InitC64Multicolor()
{

}

void LColorList::InitPICO8()
{
    m_list.clear();
    m_list.append(LColor(QColor(0x0, 0x0, 0x0),""));
    m_list.append(LColor(QColor(0x1d, 0x2b, 0x53),""));
    m_list.append(LColor(QColor(0x7e, 0x25, 0x53),""));
    m_list.append(LColor(QColor(0x0, 0x87, 0x51),""));
    m_list.append(LColor(QColor(0xab, 0x52, 0x36),""));
    m_list.append(LColor(QColor(0x5f, 0x57, 0x4f),""));
    m_list.append(LColor(QColor(0xc2, 0xc3, 0xc7),""));
    m_list.append(LColor(QColor(0xff, 0xf1, 0xe8),""));
    m_list.append(LColor(QColor(0xff, 0x0, 0x4d),""));
    m_list.append(LColor(QColor(0xff, 0xa3, 0x0),""));
    m_list.append(LColor(QColor(0xff, 0xec, 0x27),""));
    m_list.append(LColor(QColor(0x0, 0xe4, 0x36),""));
    m_list.append(LColor(QColor(0x29, 0xad, 0xff),""));
    m_list.append(LColor(QColor(0x83, 0x76, 0x9c),""));
    m_list.append(LColor(QColor(0xff, 0x77, 0xa8),""));
    m_list.append(LColor(QColor(0xff, 0xcc, 0xaa),""));

    m_background = m_list[0];
    DefaultPen(LPen::FixedSingle);

}

void LColorList::InitVIC20()
{
    m_list.clear();

    // updated with vice colours
    m_list.append(LColor(QColor(0x0, 0x0, 0x0),""));    // black
    m_list.append(LColor(QColor(0xFF, 0xFF, 0xFF),"")); // white ffffff
    m_list.append(LColor(QColor(0xb8, 0x21, 0x12),"")); // red b82212
    m_list.append(LColor(QColor(0x70, 0xf2, 0xfb),"")); // cyan 70f2fb
    m_list.append(LColor(QColor(0xad, 0x25, 0xff),"")); // purple ad25ff
    m_list.append(LColor(QColor(0x7b, 0xef, 0x00),"")); // green 7bef00
    m_list.append(LColor(QColor(0x3b, 0x24, 0xff),"")); // blue 3b24ff
    m_list.append(LColor(QColor(0xf8, 0xff, 0x00),"")); // yellow f8ff00
    m_list.append(LColor(QColor(0xcd, 0x6f, 0x00),"")); // orange cd6f00
    m_list.append(LColor(QColor(0xff, 0xc5, 0x51),"")); // light orange ffc551
    m_list.append(LColor(QColor(0xff, 0x9b, 0x91),"")); // pink ff9b91
    m_list.append(LColor(QColor(0xab, 0xff, 0xff),"")); // light cyan abffff
    m_list.append(LColor(QColor(0xff, 0x92, 0xff),"")); // light purple ff92ff
    m_list.append(LColor(QColor(0xbd, 0xff, 0x3c),"")); // light green bdff3c
    m_list.append(LColor(QColor(0xa1, 0x90, 0xff),"")); // light blue a190ff
    m_list.append(LColor(QColor(0xff, 0xff, 0x49),"")); // light yellow ffff49


    for (int i=0;i<8;i++)
        m_list[i].m_altColour = i+8;

//    DefaultPen(LPen::FixedSingle);



 /*   m_list.append(LColor(QColor(0x0, 0x0, 0x0),""));
    m_list.append(LColor(QColor(0xFF, 0xFF, 0xFF),""));
    m_list.append(LColor(QColor(103, 55, 43),""));
    m_list.append(LColor(QColor(112,165 ,177 ),""));
    m_list.append(LColor(QColor(111, 61, 133),""));
    m_list.append(LColor(QColor(88, 140, 67),""));
    m_list.append(LColor(QColor(52, 40, 121),""));
    m_list.append(LColor(QColor(183, 198,110 ),""));
    m_list.append(LColor(QColor(111, 79, 37),""));
    m_list.append(LColor(QColor(66, 57, 0),""));
    m_list.append(LColor(QColor(153, 102, 89),""));
    m_list.append(LColor(QColor(68, 68, 68),""));
    m_list.append(LColor(QColor(108, 108, 108),""));
    m_list.append(LColor(QColor(154, 209, 131),""));
    m_list.append(LColor(QColor(107, 94, 181),""));
    m_list.append(LColor(QColor(150,150 ,150 ),""));*/
    m_background = m_list[0];

}

void LColorList::InitCGA1_LOW()
{
    m_list.clear();
    m_list.append(LColor(QColor(0,0,0),"Black"));
    m_list.append(LColor(QColor(0,0xaa,0xaa),"Cyan"));
    m_list.append(LColor(QColor(0xaa,0x00,0xaa),"Magenta"));
    m_list.append(LColor(QColor(0xaa,0xaa,0xaa),"Light gray"));
    DefaultPen(LPen::FixedSingle);
}
void LColorList::InitCGA1_HIGH()
{
    m_list.clear();
    m_list.append(LColor(QColor(0,0,0),"Black"));
    m_list.append(LColor(QColor(0x55,0xff,0xff),"Cyan"));
    m_list.append(LColor(QColor(0xff,0x55,0xff),"Magenta"));
    m_list.append(LColor(QColor(0xff,0xff,0xff),"Light gray"));
    DefaultPen(LPen::FixedSingle);
}

void LColorList::InitOK64()
{
    m_list.clear();
    float s = 1; // saturation
    for (int i=0;i<256;i++) {
        int b = (i&0b11100000);
        int g = (i&0b00011000)<<3;
        int r = (i&0b00000111)<<5;

        int c = (r+g+b)/3;
        r = Util::minmax(c+(r-c)*s,0,255);
        g = Util::minmax(c+(g-c)*s,0,255);
        b = Util::minmax(c+(b-c)*s,0,255);

        m_list.append(LColor(QColor(r,g,b),""+QString::number((i))));
    }
    DefaultPen(LPen::FixedSingle);

}

void LColorList::InitVGA()
{
    m_list.clear();
    float s = 1; // saturation
    for (int i=0;i<256;i++) {
        int b = (i&0b11100000);
        int g = (i&0b00011000)<<3;
        int r = (i&0b00000111)<<5;

        int c = (r+g+b)/3;
        r = Util::minmax(c+(r-c)*s,0,255);
        g = Util::minmax(c+(g-c)*s,0,255);
        b = Util::minmax(c+(b-c)*s,0,255);

        m_list.append(LColor(QColor(r,g,b),""+QString::number((i))));
    }
    DefaultPen(LPen::FixedSingle);

}

void LColorList::InitNES()
{
    m_list.clear();
    m_list.resize(64);
//    LoadFromFile(":resources/palette/nes.pal");
    QStringList pal = Util::loadTextFile(":resources/palette/nes.txt").split("\n");
    int i=3;
    int k = 0;
    int d = 0;

    for (int l=0;l<64;l++) {
        int idx = i+k*4;
        if (idx<pal.count()) {

            QString s = pal[idx];
            m_list[l] = (LColor(QColor(Util::NumberFromStringHex(QString("$"+s.mid(2,2))),
                                       Util::NumberFromStringHex(QString("$"+s.mid(4,2))),
                                       Util::NumberFromStringHex(QString("$"+s.mid(6,2)))
                                       ),"Color"));
        }
        k++;
        if (k>=16) {
            k=0;
            i--;
        }

    }
    m_nesPPU.resize(0x20);
    m_nesPPU.fill(0xF); // fill black
    m_nesPPU[1] = 0x3;
    m_nesPPU[2] = 0x27;
    m_nesPPU[3] = 0x6;

    m_pens.clear();
/*    for (int i=0;i<4;i++) {
        m_pens.append(QSharedPointer<LPen>(new LPen(&m_pens, m_nesPPU[i])));
    }
*/
//    DefaultPen();

}

void LColorList::InitNES4()
{
    m_list.clear();
    m_list.resize(64);
    m_list.append(LColor(QColor(0,0,255),"Black"));
    m_list.append(LColor(QColor(0,255,0),"Green"));
    m_list.append(LColor(QColor(255,0,255),"Red"));
    m_list.append(LColor(QColor(255,0,255),"Brown"));


    DefaultPen(LPen::FixedSingle);

}


void LColorList::InitCGA2_LOW()
{
    m_list.clear();
    m_list.append(LColor(QColor(0,0,0),"Black"));
    m_list.append(LColor(QColor(0,0xaa,0x00),"Green"));
    m_list.append(LColor(QColor(0xaa,0x00,0x00),"Red"));
    m_list.append(LColor(QColor(0xaa,0x55,0x00),"Brown"));
    DefaultPen(LPen::FixedSingle);
}
void LColorList::InitCGA2_HIGH()
{
    m_list.clear();
    m_list.append(LColor(QColor(0,0,0),"Black"));
    m_list.append(LColor(QColor(0x55,0xff,0x55),"Green"));
    m_list.append(LColor(QColor(0xff,0x55,0x55),"Red"));
    m_list.append(LColor(QColor(0xff,0xff,0x55),"Brown"));
    DefaultPen(LPen::FixedSingle);
}

void LColorList::InitAmstradCPC()
{
    m_list.clear();
    m_list.append(LColor(QColor(0,0,0),"Black"));
    m_list.append(LColor(QColor(0x0,0x0,0x80),"Blue"));
    m_list.append(LColor(QColor(0x0,0x0,0xFF),"Bright Blue"));
    m_list.append(LColor(QColor(0x80,0x0,0x0),"Red"));
    m_list.append(LColor(QColor(0x80,0x0,0x80),"Magenta"));
    m_list.append(LColor(QColor(0x80,0x0,0xFF),"Mauve"));
    m_list.append(LColor(QColor(0xFF,0x0,0x0),"Bright Red"));
    m_list.append(LColor(QColor(0xFF,0x0,0x80),"Purple"));
    m_list.append(LColor(QColor(0xFF,0x0,0x80),"Bright Magenta"));
    m_list.append(LColor(QColor(0x0,0x80,0x0),"Green"));

    m_list.append(LColor(QColor(0x0,0x80,0x80),"Cyan"));
    m_list.append(LColor(QColor(0x0,0x80,0xFF),"Sky Blue"));
    m_list.append(LColor(QColor(0x80,0x80,0x0),"Yellow"));
    m_list.append(LColor(QColor(0x80,0x80,0x80),"White"));
    m_list.append(LColor(QColor(0x80,0x80,0xFF),"Pastel blue"));
    m_list.append(LColor(QColor(0xFF,0x80,0x0),"Orange"));
    m_list.append(LColor(QColor(0xFF,0x80,0x80),"Pink"));
    m_list.append(LColor(QColor(0xff,0x80,0xFF),"Pastel Magenta"));
    m_list.append(LColor(QColor(0x0,0xFF,0x0),"Bright Green"));
    m_list.append(LColor(QColor(0x0,0xFF,0x80),"Sea green"));

    m_list.append(LColor(QColor(0x0,0xFF,0xFF),"Bright Cyan"));
    m_list.append(LColor(QColor(0x80,0xFF,0x0),"Lime"));
    m_list.append(LColor(QColor(0x80,0xFF,0x80),"Pastel Green"));
    m_list.append(LColor(QColor(0x80,0xFF,0xFF),"Pastel Cyan"));
    m_list.append(LColor(QColor(0xFF,0xFF,0x0),"Bright Yellow"));
    m_list.append(LColor(QColor(0xFF,0xFF,0x80),"Pastel Yellow"));
    m_list.append(LColor(QColor(0xFF,0xFF,0xFF),"Bright White"));
    //qDebug() << "Constructor called";
    InitPalettePens(16);
}

void LColorList::InitBBC(int noPens)
{
    m_list.clear();
    m_list.append(LColor(QColor(0,0,0),"Black"));
    m_list.append(LColor(QColor(0xFF,0x0,0x0),"Red"));
    m_list.append(LColor(QColor(0x00,0xFF,0x0),"Green"));
    m_list.append(LColor(QColor(0xFF,0xFF,0x0),"Yellow"));
    m_list.append(LColor(QColor(0x00,0x0,0xFF),"Blue"));
    m_list.append(LColor(QColor(0xFF,0x0,0xFF),"Magenta"));
    m_list.append(LColor(QColor(0x00,0xFF,0xFF),"Cyan"));
    m_list.append(LColor(QColor(0xFF,0xFF,0xFF),"White"));
    m_pens.clear();
    for (int i=0;i<noPens;i++) {
        m_pens.append(QSharedPointer<LPen>(new LPen(&m_pens,&m_list,i&7,"",LPen::Dropdown, m_bpp)));
    }

}

void LColorList::InitPalettePens(int cnt)
{
    if (m_pens.count()==cnt)
        return;
    m_pens.clear();
//    qDebug() << "IniTPalettePens called";
//    m_pens.append(LPen(&m_pens, 0,m_,LPen::Fixed));
    for (int i=0;i<16;i++) {
        m_pens.append(QSharedPointer<LPen>(new LPen(&m_pens,&m_list,i,"",LPen::Dropdown, m_bpp)));
    }

}


void LColorList::LoadFromFile(QString fileName)
{

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }
    QByteArray blob = file.readAll();
    m_list.clear();
    for (int i=0;i<blob.size()/3;i++) {
        m_list.append(LColor(QColor((unsigned char)blob[3*i],
                             (unsigned char)blob[3*i+1],
                             (unsigned char)blob[3*i+2]),"Color"+QString::number(i)));
//        qDebug() << "WHOO "<<i;

    }
    file.close();

}

void LColorList::DefaultPen(LPen::Type type = LPen::FixedSingle)
{
    m_pens.clear();

//    m_pens.append(LPen(&m_pens, 0,m_,LPen::Fixed));
    for (int i=0;i<m_list.count();i++) {
        m_pens.append(QSharedPointer<LPen>(new LPen(&m_pens,&m_list,i,"",type, m_bpp)));
    }

}

void LColorList::DefaultPen(LPen::Type type, int cnt)
{
    m_pens.clear();

//    m_pens.append(LPen(&m_pens, 0,m_,LPen::Fixed));
    for (int i=0;i<cnt;i++) {
        m_pens.append(QSharedPointer<LPen>(new LPen(&m_pens,&m_list,i,"",type, m_bpp)));
    }

}

QColor LColorList::getClosestColor(QColor col, int& winner)
{

    float d = 1E20;
    //    qDebug() << "WHOO";
    winner = 0;
    if (m_selectClosestFromPen) {

        for (int i=0;i<m_pens.count();i++) {
            //qDebug() << "Metric:";

            int pen = m_pens[i]->Get();

            if (pen>=m_list.count())
                continue;

            if (!m_list[pen].inUse)
                continue;


            float v = m_metric->getDistance(m_list[pen].color, col);
            //qDebug() << "end:";
            if (v<d) {
                d = v;
                winner = pen;
            }
        }
        return m_list[winner].color;
    }
      else
      {
        for (int i=0;i<m_list.count();i++) {
            //qDebug() << "Metric:";

            if (!m_list[i].inUse)
                continue;

            float v = m_metric->getDistance(m_list[i].color, col);
            //qDebug() << "end:";
            if (v<d) {
                d = v;
                winner = i;
            }
        }
        return m_list[winner].color;

      }

}

int LColorList::getPen(int pcol) {


//    qDebug() << "WTF "<< pcol << m_pens.count();// <<m_pens[pcol]->m_colorIndex;
//    if (pcol>8) qDebug() << pcol;
    if (pcol<m_pens.count() && pcol>=0)
        return m_pens[pcol]->Get();
    return 0;
}

int LColorList::getPenIndex(int pcol)
{
//    qDebug() << "Pens pcol " << pcol;

   // if (pcol!=0)
   // qDebug() << "Pens count " << m_pens.count() << getPen(pcol);
    for (int i=0;i<m_pens.count();i++) {
//        if (pcol!=0)
  //          qDebug() << "CUR "<<i<<getPen(i) <<pcol;
        if (getPen(i)==pcol)
            return i;
    }
    return 0;
}

void LColorList::setPen(int pcol, int colorIndex) {
    if (pcol<m_pens.count() && pcol>=0)
        m_pens[pcol]->m_colorIndex = colorIndex;
}

QColor LColorList::getPenColour(int pcol) {
    int idx = getPen(pcol);
    if (idx<m_list.count() && idx>=0)
        return m_list[idx].color;

    return Qt::black;
}

void LColorList::ExportAmigaPalette(QString filename)
{
    QByteArray data;
    for (LColor l: m_list) {
        unsigned short d = l.get12BitValue();
        //        qDebug() << QString::number(d,16);
        data.append((char)((d>>8)&0xFF));
        data.append((char)(d&0xFF));
    }
    Util::SaveByteArray(data, filename);
}

void LColorList::ExportAtariSTPalette(QString filename)
{
    QByteArray data;
    for (LColor l: m_list) {
        unsigned short d = l.get9BitValue();
        //        qDebug() << QString::number(d,16);
        data.append((char)((d>>8)&0xFF));
        data.append((char)(d&0xFF));
    }
    Util::SaveByteArray(data, filename);
}

void LColorList::PenToFooter(LImageFooter *footer)
{
    for (int i=0;i<m_pens.count();i++) {
        footer->set(LImageFooter::POS_PEN_START + i,getPen(i));
    }
}

void LColorList::FooterToPen(LImageFooter *footer)
{
    if (m_supportsFooterPen)
    for (int i=0;i<m_pens.count();i++) {
        uchar val =footer->get(LImageFooter::POS_PEN_START + i);
        if (m_pens[i]->m_type!=LPen::FixedSingle)
            if (val!=0) {
                setPen(i,val);
            }
    }

}

void LColorList::ConstrainColours(QVector<int> &cols) {
    if (m_supportsFooterPen)
    for (int i=0;i<m_list.count();i++) {
        ConstrainColour(i,cols.contains(i));
//        qDebug() << "const" << !cols.contains(i);
    }

}

void LColorList::ConstrainColour(int pen, bool constrain) {
    // Temporary FIX UP
    m_list[ pen].displayList = constrain;
}


void LColorList::UpdateCommodoreRestrictions(uchar* extraCols)
{
    for (int i=0;i<m_list.count();i++) {
        if (i<8)
            m_list[i].displayList = true;
        else
            m_list[i].displayList = false;

    }
    for (int i=0;i<4;i++)
        if (extraCols[i]<m_list.count())
            m_list[extraCols[i]].displayList=true;

}


void LColorList::FillComboBox(QComboBox *cmb)
{
    cmb->clear();
    for (int i=0;i<m_list.count();i++) {
        QPixmap pixmap(16,16);
        pixmap.fill(m_list[i].color);
        QIcon icon(pixmap);
        cmb->addItem(icon,Util::numToHex(i));
    }
}

void LColorList::FillComboBoxRestricted(QComboBox *cmb, int a, int b)
{
    cmb->clear();
    for (int i=a;i<b;i++) {
        QPixmap pixmap(16,16);
        pixmap.fill(m_list[i].color);
        QIcon icon(pixmap);
        cmb->addItem(icon,Util::numToHex(i));
    }

}

int LColorList::getIndex(QColor c)
{
    for (int i=0;i<m_list.count();i++) {
//        qDebug() << "   Testing: " << c << m_list[i].color;
        if (m_list[i].color == c) {
//            qDebug() << "found" << i <<  c << m_list[i].color;
            return i;
        }
    }
    return 0;
}

void LColorList::CreateUI(QLayout *ly, int type)
{
    CreateUI(ly, type, QSize(1980,1024));
}

void LColorList::CreateUI(QLayout* ly, int type, QSize windowSize) {
    if (type==0) {
        CreateUIOld(ly,type,windowSize);
        return;
    }
    //qDebug() <<"LColorList::CreateUI";
    m_windowSize = windowSize;
    if (ly==nullptr)
        return;

    m_currentType = type;
    m_layout = ly;
    Util::clearLayout(ly, true);
    int m = m_pens.count();
    int width=60/(std::max(m/16,1))*(windowSize.height()/(float)1600);
//    qDebug() << width;
    if (m>200) {
        width = 24*(windowSize.height()/(float)1600);;
    }
//    width = (windowSize.height()/(float)140);
    int xx=0, yy=0;
    int maxy=0;
    int cur = 0;
//    ly->setSpacing(0);
//    qDebug() << "*************************";
//    ly->setSpacing(0);
    for (int i=0;i<m_pens.count();i++) {
        //qDebug() << "COL " <<m_pens[i].m_colorIndex;
        if (m_pens[i]->m_isHidden)
            continue;


        QWidget* widget = nullptr;
//        qDebug() << "PEN COLOR:  " << getPen(i);
        widget = m_pens[i]->CreateUI(getPenColour(i),width,xx,yy, m_list);

        if (widget!=nullptr) {

            QGridLayout* gly = dynamic_cast<QGridLayout*>(ly);
            if (gly!=nullptr) {
                gly->addWidget(widget,yy,xx);
                gly->setVerticalSpacing(0);
            }
            else
                ly->addWidget(widget);


            yy++;
            cur++;
            maxy++;
            if (yy==16) {
                yy=0;
                xx++;
                maxy=17;
            }
        }
        //}
    }
    ly->addItem(new QSpacerItem(0,maxy,QSizePolicy::Expanding,QSizePolicy::Expanding));

}



/*
 *
 *  DEPRECATED
 *
 *
 **/
void LColorList::CreateUIOld(QLayout* ly, int type, QSize windowSize)
{
    m_windowSize = windowSize;
    if (ly==nullptr)
        return;

    m_currentType = type;
    m_layout = ly;
    Util::clearLayout(ly, true);
    int m=0;
    for (int i=0;i<m_list.count();i++)
        if (m_list[i].displayList)
            m++;


    m_buttonsEdit.clear();
    m_buttonsImport.clear();
//    m_buttons.clear();
    int xx=0, yy=0;
    int width=40/(std::max(m/16,1))*(windowSize.width()/(float)1400);
//    qDebug() << width;
    if (m>200) {
        width = 16*(windowSize.width()/(float)1400);;
    }
//    if (m_list.count())

    int maxy=0;
    int cur = 0;

    for(int j=0; j<m_list.count(); j++)
    {
        if (!m_list[j].displayList)
            continue;

        QPushButton *b = new QPushButton();
        //b->setGeometry(0,0,40,40);

//        qDebug() <<j<<m_list[j].m_altColour <<m_list[j].ignoreAltColour;

        QPalette p;
//        p.setColor(QPalette::Button, m_list[j].color);
  //      p.setColor(QPalette::Window, m_list[j].color);
        QString txtCol = QString::number(m_list[j].color.red()) + ", ";
        txtCol += QString::number(m_list[j].color.green()) + ", ";
        txtCol += QString::number(m_list[j].color.blue());


        b->setFlat(true);
        QPixmap pm = CreateColorIcon(j,width);




        b->setAutoFillBackground(true);

//        p.set
        p.setBrush(b->backgroundRole(), QBrush(pm));


//        b->setStyleSheet("background-color: rgb("+txtCol + "); color: rgb(0, 0, 0)");
//        b->setIcon(CreateColorIcon(j,width));
        b->setMaximumWidth(width);
        b->setMinimumWidth(width);
        b->setMaximumHeight(width);
        b->setMinimumHeight(width);
        b->setPalette(p);
        if (type==1)
        for (int k=0;k<m_multicolors.count();k++)
            if (m_multicolors[k]==j){
                if (k!=2)
                    b->setText("" + QString::number(k+1));
                else
                    b->setText("X");
            }
//        b->setStyleSheet("padding: 0px;");
        if (type==0) {
            QObject::connect( b, &QPushButton::clicked,  [=](){ handleButtonImport(j);} );

        }
        if (type==1) {
            QObject::connect( b, &QPushButton::clicked,  [=](){ handleButtonEdit(j,cur,b);} );
        }
        //QObject::connect( b, &QPushButton::clicked,  colorValueChanged );


        QGridLayout* gly = dynamic_cast<QGridLayout*>(ly);
        if (gly!=nullptr) {
            gly->addWidget(b,yy,xx);
        }
        else
            ly->addWidget(b);

        if (type==0)
            m_buttonsImport.append(b);
        else
            m_buttonsEdit.append(b);





        yy++;
        cur++;
        maxy++;
        if (yy==16) {
            yy=0;
            xx++;
            maxy=17;
        }
    }
}

void LColorList::handleButtonEdit(int val, int data, QPushButton* btn)
{
/*    for (int i=0;i<m_buttonsEdit.count();i++)
        m_buttonsEdit[i]->setText("");
    //if (data<m_buttonsEdit.count())
//    qDebug() << data;
        m_buttonsEdit[data]->setText("X");*/
    Data::data.currentColor = val;
    Data::data.currentIsColor=true;
    SetMulticolor(3,val);
    QPoint p = (QCursor::pos() - btn->mapToGlobal(QPoint(0,0)));
    QPointF fp = QPointF(p.x()/(float)btn->width(),
            p.y()/(float)btn->height());
//    qDebug() << (QCursor::pos() - btn->mapToGlobal(QPoint(0,0))) << btn->mapToGlobal(QPoint(0,0))  <<m_buttonsEdit[data]->cursor().pos();
//    qDebug() <<fp;
    // Select alternative colour
    if (!m_list[val].ignoreAltColour)
    if (m_list[val].m_altColour!=-1)
        if (1-fp.y()<fp.x())
            Data::data.currentColor = m_list[val].m_altColour;

}

void LColorList::handleButtonImport(int data)
 {
    //qDebug() << data;
    m_list[data].inUse=!m_list[data].inUse;
    if (m_list[data].inUse)
        m_buttonsImport[data]->setText(" ");
    else {
        m_buttonsImport[data]->setText("X ");
   }

    //Data::data.redrawInput = true;
    emit colorValueChanged();
 }

void LColorList::fromArrayList(QVector<int> colors) {
    m_list.resize(colors.size()/3);
    int c = 0;
    for (LColor &l: m_list) {
        l.color = QColor(colors[c],colors[c+1],colors[c+2]);
        c+=3;
    }
}
