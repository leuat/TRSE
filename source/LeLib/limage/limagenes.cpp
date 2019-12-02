#include "limagenes.h"


LImageNES::LImageNES(LColorList::Type t) : CharsetImage(t)
{
    m_width = 128;
    m_height = 128;
    m_bitMask = 0b1;

    m_scaleX = 1.0f;
    m_noColors = 4;
    m_scale = 2;


    m_charWidth = 32;
    m_charHeight = 16;

    //m_data = new PixelChar[m_charWidth*m_charHeight];

    Clear();
    m_type = LImage::Type::NES;
    m_supports.asmExport = true;
    m_supports.binaryLoad = true;
    m_supports.binarySave = true;
    m_supports.nesPalette = true;
    m_supports.koalaImport = false;
    m_supports.koalaExport = false;
    m_supports.flfSave = true;
    m_supports.flfLoad = true;
    m_supports.compressedExport = true;
    m_supports.displayForeground = true;

    m_GUIParams[btnLoadCharset] ="";
    m_GUIParams[btn1x1] = "";
    m_GUIParams[btn2x2] = "";
    m_GUIParams[btn2x2repeat] = "";
    m_GUIParams[btnCopy] = "";
    m_GUIParams[btnPaste] = "";
    m_GUIParams[btnFlipH] = "";
    m_GUIParams[btnFlipV] = "";

    m_GUIParams[tabCharset] = "";
    m_GUIParams[tabData] = "";
    m_GUIParams[tabLevels] = "";
    m_GUIParams[tabEffects] = "Effects";

    m_GUIParams[btnEditFullCharset] = "";

    m_exportParams["StartX"] = 0;
    m_exportParams["EndX"] = m_charWidth;
    m_exportParams["StartY"] = 0;
    m_exportParams["EndY"] = m_charHeight;
    m_exportParams["Compression"] = 0;



    for (int i=0;i<4;i++)
        m_extraCols[i] = 0;

    ForceColor();
}

void LImageNES::ImportBin(QFile &file)
{
    QByteArray data = file.readAll();
    int j=0;
    for (int i=0;i<m_charWidth*m_charHeight;i++) {
        for (int k=0;k<8;k++)
//            m_data[i].p[k] = data[j+k];//PixelChar::reverse(data[j+k]);
            m_data[i].p[k] = PixelChar::reverse(data[j+k]);
        j+=8;
    }
    ForceColor();
}

void LImageNES::setMultiColor(bool doSet)
{
    ForceColor();
}

void LImageNES::ForceColor()
{
/*    SetColor(0xF,0);
    SetColor(0x17,1);
    SetColor(0x28,2);
    SetColor(0x5,3);
*/
}

void LImageNES::setForeground(unsigned int col)
{
//    m_cols[1] = col;
    SetColor(col, 1);
 //   qDebug() << "HERE";
}


unsigned int LImageNES::getPixel(int x, int y)
{
    if (x>=m_width || x<0 || y>=m_height || y<0)
        return 0;

    int r = x/(float)8;
    x=x+r*8;

    PixelChar& pc1 = getPixelChar((x/2),y);
    PixelChar& pc2 = getPixelChar((x/2)+4,y);

    int ix = x %8;//- (dx*m_charWidth);
    int iy = y % 8;//- (dy*m_charHeight);

    x = ix;
    y = iy;

    if (x<0 || x>=16 || y<0 || y>=8)
        return 0;


    unsigned char pp = (((pc1.p[y])>>x) & 0b1) | (((pc2.p[y])>>x) & 0b1)*2;

    return m_cols[pp];


//    return pc.get(m_scale*ix, iy, m_bitMask);

}

void LImageNES::setPixel(int x, int y, unsigned int col)
{

}

void LImageNES::SetColor(uchar col, uchar idx)
{

    m_cols[idx] = col;
   // qDebug() << "Setting idx " << idx << " to  "<< QString::number(col);

}

void LImageNES::CopyFrom(LImage *img)
{
    LImageNES* n = dynamic_cast<LImageNES*>(img);
    for (int i=0;i<4;i++)
        m_cols[i] = n->m_cols[i];
    CharsetImage::CopyFrom(img);
}
