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

void LColorList::SetIsMulticolor(bool mult)
{
    m_isMulticolor = mult;
    if (m_list.count()==0)
        return;

    for (int i=0;i<m_list.count();i++)
        m_list[i].ignoreAltColour = false;

    if (m_ignoreSetIsMulti)
        for (int i=0;i<8;i++)
            m_list[i+8].displayList = false;

    if (m_isMulticolor && !m_ignoreSetIsMulti) {
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

}

void LColorList::GeneratePaletteFromQImage(QImage &img)
{
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
    if (m_type == Type::X16) {
        InitOK64();
    }


    m_metric = new LinearMetric();

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
    m_list.append(LColor(QColor(0x6f, 0x4f, 0x25),""));
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

}

void LColorList::InitVIC20()
{
    m_list.clear();


    m_list.append(LColor(QColor(0x0, 0x0, 0x0),""));
    m_list.append(LColor(QColor(0xFF, 0xFF, 0xFF),""));
    m_list.append(LColor(QColor(0x78, 0x29, 0x22),""));
    m_list.append(LColor(QColor(0x87, 0xd6, 0xdd),""));
    m_list.append(LColor(QColor(0xaa, 0x5f, 0xb6),""));
    m_list.append(LColor(QColor(0x55, 0xa0, 0x49),""));
    m_list.append(LColor(QColor(0x40, 0x31, 0x8d),""));
    m_list.append(LColor(QColor(0xbf, 0xce, 0x72),""));
    m_list.append(LColor(QColor(0xaa, 0x74, 0x49),""));
    m_list.append(LColor(QColor(0xea, 0xb4, 0x89),""));
    m_list.append(LColor(QColor(0xb8, 0x69, 0x62),""));
    m_list.append(LColor(QColor(0xc7, 0xff, 0xff),""));
    m_list.append(LColor(QColor(0xea, 0x9f, 0xf6),""));
    m_list.append(LColor(QColor(0x94, 0xe0, 0x89),""));
    m_list.append(LColor(QColor(0x80, 0x71, 0xcc),""));
    m_list.append(LColor(QColor(0xff, 0xff, 0xb2),""));


    for (int i=0;i<8;i++)
        m_list[i].m_altColour = i+8;




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
}
void LColorList::InitCGA1_HIGH()
{
    m_list.clear();
    m_list.append(LColor(QColor(0,0,0),"Black"));
    m_list.append(LColor(QColor(0x55,0xff,0xff),"Cyan"));
    m_list.append(LColor(QColor(0xff,0x55,0xff),"Magenta"));
    m_list.append(LColor(QColor(0xff,0xff,0xff),"Light gray"));
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

/*
//        qDebug() << k+d;
        k=k+4;
        if (k>=64)
        {
            d++;
            k=0;

        }
*/
    }
    m_nesPPU.resize(0x20);
    m_nesPPU.fill(0xF); // fill black
    m_nesPPU[1] = 0x3;
    m_nesPPU[2] = 0x27;
    m_nesPPU[3] = 0x6;


}

void LColorList::InitNES4()
{
    m_list.clear();
    m_list.resize(64);
    m_list.append(LColor(QColor(0,0,255),"Black"));
    m_list.append(LColor(QColor(0,255,0),"Green"));
    m_list.append(LColor(QColor(255,0,255),"Red"));
    m_list.append(LColor(QColor(255,0,255),"Brown"));

}


void LColorList::InitCGA2_LOW()
{
    m_list.clear();
    m_list.append(LColor(QColor(0,0,0),"Black"));
    m_list.append(LColor(QColor(0,0xaa,0x00),"Green"));
    m_list.append(LColor(QColor(0xaa,0x00,0x00),"Red"));
    m_list.append(LColor(QColor(0xaa,0x55,0x00),"Brown"));
}
void LColorList::InitCGA2_HIGH()
{
    m_list.clear();
    m_list.append(LColor(QColor(0,0,0),"Black"));
    m_list.append(LColor(QColor(0x55,0xff,0x55),"Green"));
    m_list.append(LColor(QColor(0xff,0x55,0x55),"Red"));
    m_list.append(LColor(QColor(0xff,0xff,0x55),"Brown"));
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

QColor LColorList::getClosestColor(QColor col, int& winner)
{
    float d = 1E20;
    int i = 0;
//    qDebug() << "WHOO";
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
/*    if (rand()%500==0) {
        qDebug() << "Testing for: " << col.red() << ", " << col.green() << ", " << col.blue();
        qDebug() << "Winner: " <<m_list[winner].color.red() << ", " <<m_list[winner].color.green() << ", " << m_list[winner].color.blue();
    }*/
    return m_list[winner].color;

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


void LColorList::CreateUI(QLayout* ly, int type, QSize windowSize)
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
    int width=40/(max(m/16,1))*(windowSize.width()/(float)1400);
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
/*    QGridLayout* gly = dynamic_cast<QGridLayout*>(ly);
    if (gly!=nullptr) {
        gly->addItem(new QSpacerItem(1,200),maxy,xx);
    }*/
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
