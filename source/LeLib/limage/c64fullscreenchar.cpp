#include "c64fullscreenchar.h"

C64FullScreenChar::C64FullScreenChar(LColorList::Type t) : MultiColorImage(t)
{
/*    m_charset = new CharsetImage(t);
    QFile f("C:/Users/leuat/OneDrive/Documents/GitHub/pmm/pmm/charsets/test_mc.bin");
    f.open(QIODevice::ReadOnly);
    m_charset->ImportBin(f);*/

    m_width = 320;
    m_height = 200;
    m_scaleX = 1.0f;
    m_bitMask = 0b11;
    m_noColors = 4;
    m_scale = 1;
    m_minCol = 0;
    m_type = LImage::Type::FullScreenChar;

    m_rawData.resize(m_charWidth*m_charHeight);
    m_rawColors.resize(m_charWidth*m_charHeight);
    Clear();

    SetColor(1,1);
    SetColor(2,2);

    m_supports.asmExport = false;
    m_supports.binaryLoad = true;
    m_supports.binarySave = true;
    m_supports.flfSave = false;
    m_supports.flfLoad = true;
    m_supports.asmExport = false;



}

void C64FullScreenChar::SetColor(uchar col, uchar idx)
{
    if (m_charset==nullptr)
        return;
    m_charset->SetColor(col, idx);
    m_extraCols[idx] = col;
}

void C64FullScreenChar::Clear()
{
    for (int i=0;i<m_rawData.count();i++)
        m_rawData[i] = 0x20;

    for (int i=0;i<m_rawData.count();i++)
        m_rawColors[i] = 0x5;

}

void C64FullScreenChar::ImportBin(QFile &f)
{
    m_rawData = f.read(1000);
    m_rawColors = f.read(1000);
    // Shift down
    for (int i=0;i<m_rawColors.count();i++)
        m_rawColors[i] = m_rawColors[i] - 8;
//    FromRaw(m_rawData);

}

void C64FullScreenChar::ExportBin(QFile &f)
{
 //   ToRaw(m_rawData);
    f.write(m_rawData);
/*    for (int i=0;i<m_rawColors.count();i++)
        m_rawColors[i] = m_rawColors[i] + 8;

    f.write(m_rawColors);

    for (int i=0;i<m_rawColors.count();i++)
        m_rawColors[i] = m_rawColors[i] - 8;*/


}

void C64FullScreenChar::BuildImage()
{
    for (int i=0;i<m_charWidth;i++)
        for (int j=0;j<m_charHeight;j++) {
            uchar val = m_rawData[j*m_charWidth + i];

            if (val!=0 && rand()%100==0)
            for (int k=0;k<8;k++)
             m_data[i + j*m_charWidth].p[k] = rand()%255; //m_charset->m_data[val];
        }

}

void C64FullScreenChar::setPixel(int x, int y, unsigned int color)
{
    if (x>=320 || x<0 || y>=200 || y<0)
        return;

    if (m_writeType==Character)
       m_rawData[x/8+ (y/8)*m_charWidth] = m_currencChar;
    if (m_writeType==Color)
       m_rawColors[x/8+ (y/8)*m_charWidth] = color;
    //BuildImage();
}

unsigned int C64FullScreenChar::getPixel(int x, int y)
{
    if (m_charset==nullptr)
        return 0;
    if (x>=320 || x<0 || y>=200 || y<0)
        return 0;
    uchar v = m_rawData[(x/8) + (y/8)*m_charWidth];
    uchar col = m_rawColors[(x/8) + (y/8)*m_charWidth];
    int ix = (x % (8)/2)*2;//- (dx*40);
    int iy = y % 8;//- (dy*25);

 //   return pc.get(m_scale*ix, iy, m_bitMask);



    int pos = v;
//    return m_charset->m_data[pos].get(v + (2*x)&7, v+ y&7,m_bitMask);
    uchar val = m_charset->m_data[pos].get(ix, iy,m_charset->m_bitMask);
    if (val==m_charset->m_data[pos].c[3])
        val = col;


    return val;

}

void C64FullScreenChar::CopyFrom(LImage *mc)
{
    C64FullScreenChar* c = dynamic_cast<C64FullScreenChar*>(mc);
    if (c!=nullptr) {
        if (c->m_charset==nullptr)
            return;

        m_rawData.resize(m_charHeight*m_charWidth);
        m_rawColors.resize(m_charHeight*m_charWidth);
        for (int i=0;i<m_rawData.count();i++) {
            m_rawData[i] = c->m_rawData[i];
            m_rawColors[i] = c->m_rawColors[i];
        }
        m_charset = c->m_charset;
        m_writeType = c->m_writeType;
        m_currencChar = c->m_currencChar;

    }
    else
    LImage::CopyFrom(mc);

}


void C64FullScreenChar::SaveBin(QFile& file)
{
    file.write( ( char * )( &m_background ),  1 );
    file.write( ( char * )( &m_border ), 1 );
    file.write( m_rawColors ,  25*40);
    file.write( m_rawData ,  25*40);


}

void C64FullScreenChar::LoadBin(QFile& file)
{
    file.read( ( char * )( &m_background ),1 );
    file.read( ( char * )( &m_border ), 1);
    m_rawColors.resize(25*40);
    m_rawData.resize(25*40);
    m_rawColors = file.read( 25*40);
    m_rawData = file.read( 25*40);

}

