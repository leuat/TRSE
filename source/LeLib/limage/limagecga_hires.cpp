#include "limagecga_hires.h"

LImageCGAHires::LImageCGAHires(LColorList::Type t)  : LImageCGA(t)
{
    Initialize(640,200);
    m_scale = 1;
    m_type = LImage::Type::CGA_HIRES;
    m_supports.asmExport = false;
    m_supports.binaryLoad = false;
    m_supports.binarySave = true;
    m_supports.flfSave = true;
    m_supports.flfLoad = true;

    m_supports.displayMC1 = false;
    m_supports.displayMC2 = false;
    m_supports.displayForeground = false;
    m_supports.displayBackground = false;

    m_GUIParams[btnEditFullCharset] = "Char";
    m_supports.displayCharOperations = true;

    m_GUIParams[tabCharset] = "1";
    m_updateCharsetPosition = true;
    m_colorList.m_isCharset = true;

    m_charWidth=80;
    m_charHeight=25;
    m_updateCharsetPosition = true;

    m_supports.displayCharOperations = true;

}

void LImageCGAHires::toCGA(QByteArray &even, QByteArray &odd, QByteArray &evenMask, QByteArray &oddMask, int x1, int y1, int w, int h)
{
    int cur=0;
    char curC=0;
    char curM=0;
    for (int y=0;y<h;y++) {
        for (int x=0;x<w*2;x++) {
            // QColor c = QColor(m_qImage->pixel(x,y));
            //            qDebug() << m_qImage->pixel(x,y);
            char idx = m_qImage->pixel(x+x1,y+y1);//m_colorList.getIndex(c);
            //          qDebug() << QString::number(cur) << c;

            char midx = 0;
            if (idx!=0) midx=0b1;
            curM = curM | (midx<<(7-(curC)));
            cur = cur | (idx<<(7-(curC)));
            curC++;
            if (curC==8) {
                if ((y&1)==0)
                    even.append(cur);
                else odd.append(cur);
                if ((y&1)==0)
                    evenMask.append(curM);
                else oddMask.append(curM);
                curC=0;
                cur=0;
                curM = 0;
            }

        }
    }

}

