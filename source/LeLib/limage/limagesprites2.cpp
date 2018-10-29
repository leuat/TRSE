#include "limagesprites2.h"


LImageSprites2::LImageSprites2(LColorList::Type t) : CharsetImage(t) {
    m_type = LImage::Type::Sprites2;
    m_currencChar=0;
    m_bitMask = 0b11;
    m_width = 160/4;
    m_height = 200/4;
    m_GUIParams[tabCharset] = "";
    m_GUIParams[tabData] = "";
    m_GUIParams[tabLevels] = "";
    m_GUIParams[tabEffects] = "";

    m_GUIParams[btnEditFullCharset] = "";
    m_GUIParams[tabSprites] ="Sprites";

    m_exportParams.clear();

    AddSprite(1,1);
    //Data::data.currentColor=0;

}

void LImageSprites2::AddSprite(int x, int y)
{
    LSprite s;
    s.Initialize(x,y);
    m_sprites.append(s);
    m_currencChar = m_sprites.count()-1;
    SetColor(m_extraCols[0],0);
    SetColor(m_extraCols[1],1);
    SetColor(m_extraCols[2],2);

}

void LImageSprites2::ImportBin(QFile &f)
{
    QByteArray a = f.readAll();
    int cnt = a.count()/64;
    for (int i=0;i<cnt;i++) {
        LSprite s(a,i*64);
        m_sprites.append(s);
        m_currencChar = m_sprites.count()-1;
        for (int j=0;j<4;j++)
            SetColor(m_extraCols[j],j);
    }
}

void LImageSprites2::ExportBin(QFile &f)
{

}

void LImageSprites2::CopyFrom(LImage *img)
{
    LImageSprites2* mc = dynamic_cast<LImageSprites2*>(img);
    if (mc!=nullptr)
    {
         m_background = mc->m_background;
         m_border = mc->m_border;
         //m_extraCols = mc.m_extraCols;
         m_width = mc->m_width;
         m_height = mc->m_height;
         m_bitMask = mc->m_bitMask;
         for (int i=0;i<4;i++)
             m_extraCols[i] = mc->m_extraCols[i];
         m_sprites = mc->m_sprites;
         m_currencChar = mc->m_currencChar;
    }
    else
    {
        LImage::CopyFrom(img);
        return;
    }

}

void LImageSprites2::setPixel(int x, int y, unsigned int color)
{
    if (m_currencChar<0)
        return;

    if (m_currencChar>=m_sprites.count())
            return;

    float fx = x/(float)m_width;
    float fy = y/(float)m_height;


    m_sprites[m_currencChar].setPixel(fx,fy,color,m_bitMask);

}

unsigned int LImageSprites2::getPixel(int x, int y)
{
    if (m_currencChar<0)
        return 0;
    if (m_currencChar>=m_sprites.count())
            return 0 ;

    float fx = x/(float)m_width;
    float fy = y/(float)m_height;

    return m_sprites[m_currencChar].getPixel(fx,fy,m_bitMask);

}

PixelChar *LSprite::GetSetData(float x, float y, float &ix, float &iy, uchar bitMask)
{
    ix = x*m_blocksWidth*3.0f;
    iy = y*m_blocksHeight*3.0f/24*21.0f;
    if (iy<0 || iy>=m_blocksHeight*3) return nullptr;
    if (ix<0 || ix>=m_blocksWidth*3) return nullptr;
    int scale = 1;
    if (bitMask==0b11) scale =2;
    int v = (int)ix+(int)iy*m_pcHeight*m_blocksWidth;
    if (v<0 || v>=m_data.count())
        return nullptr;
    //PixelChar& pc = m_data[v];
    iy=((int)(iy*8.0));
    if (iy>m_blocksHeight*21)
        return nullptr;
    iy=(int)iy&7;
    ix=((int)(ix*8.0)/scale)&((8/scale)-1);
 //   int ix = x % (8/m_scale);//- (dx*m_charWidth);
    ix =(int)ix*scale;
    return &m_data[v];


}

void LSprite::setPixel(float x, float y, uchar color, uchar bitMask)
{
    float ix, iy;
    PixelChar* pc = GetSetData(x,y,ix,iy, bitMask);
    if (pc==nullptr)
        return;
    pc->set(ix,iy,color,bitMask);

}

uchar LSprite::getPixel(float x, float y, uchar bitMask)
{
    float ix, iy;
    PixelChar* pc = GetSetData(x,y,ix,iy, bitMask);
    if (pc==nullptr)
        return 0;
    return pc->get(ix,iy,bitMask);

}



void LImageSprites2::SaveBin(QFile& file)
{
    file.write( ( char * )( &m_background ),  1 );
    file.write( ( char * )( &m_border ), 1 );
    file.write( ( char * )( &m_extraCols[1] ), 1 );
    file.write( ( char * )( &m_extraCols[2] ), 1 );
    file.write( ( char * )( &m_extraCols[3] ), 1 );

    uchar cnt = m_sprites.count();
    file.write( ( char * )( &cnt ), 1 );
    for (LSprite& s : m_sprites) {
        uchar sx = s.m_blocksWidth;
        uchar sy = s.m_blocksHeight;

        file.write( ( char * )( &sx ), 1 );
        file.write( ( char * )( &sy ), 1 );
//        file.write( ( char * )( &m_data ),  m_charHeight*m_charWidth*12 );

        QByteArray data;
        for (PixelChar& pc: s.m_data) {
                    for (int j=0;j<8;j++)
                        data.append(pc.p[j]);
                    for (int j=0;j<4;j++)
                        data.append(pc.c[j]);
                }

        file.write( data, sx*sy*s.m_pcHeight*s.m_pcWidth*12);

    }

}

void LImageSprites2::LoadBin(QFile& file)
{
    m_sprites.clear();
    file.read( ( char * )( &m_background ),  1 );
    file.read( ( char * )( &m_border ), 1 );
    file.read( ( char * )( &m_extraCols[1] ), 1 );
    file.read( ( char * )( &m_extraCols[2] ), 1 );
    file.read( ( char * )( &m_extraCols[3] ), 1 );
    uchar cnt;
    file.read( ( char * )( &cnt ), 1 );
    for (int i=0;i<cnt;i++) {
        uchar sx,sy;
        file.read( ( char * )( &sx ), 1 );
        file.read( ( char * )( &sy ), 1 );

        LSprite s;
        s.Initialize(sx,sy);

        QByteArray data = file.read(sx*sy*s.m_pcHeight*s.m_pcWidth*12);
        int c=0;
        for (PixelChar& pc: s.m_data) {
            for (int j=0;j<8;j++)
                pc.p[j] = data[12*c+j];
            for (int j=0;j<4;j++)
                pc.c[j] = data[12*c+j+8];
            c++;
        }
        m_sprites.append(s);
    }
    m_currencChar = 0;

    m_extraCols[0] = m_background;
    SetColor(m_extraCols[1],1);
    SetColor(m_extraCols[2],2);


}

void LImageSprites2::SetColor(uchar col, uchar idx)
{
    m_color.c[idx] = col;

    if (idx==0)
        m_background = col;

    if (m_currencChar>=0)
    for (int i=0;i<m_sprites[m_currencChar].m_data.count();i++)
        m_sprites[m_currencChar].m_data[i].c[idx] = col;

    m_extraCols[idx] = col;
}

void LImageSprites2::SetColor(uchar col, uchar idx, LSprite &s)
{

    m_color.c[idx] = col;

    if (idx==0)
        m_background = col;

    for (int i=0;i<s.m_data.count();i++)
        s.m_data[i].c[idx] = col;

    m_extraCols[idx] = col;
}

bool LImageSprites2::KeyPress(QKeyEvent *e)
{

    if (e->key()==Qt::Key_A)
        m_currencChar-=1;
    if (e->key()==Qt::Key_D)
        m_currencChar+=1;

    if (m_currencChar>2500) m_currencChar=0;
    if (m_currencChar>=m_sprites.count())
        m_currencChar=m_sprites.count()-1;
//    m_currencChar = Util::clamp(m_currencChar,0,m_sprites.count()-1);


    SetColor(m_extraCols[0],0);
    SetColor(m_extraCols[1],1);
    SetColor(m_extraCols[2],2);
//   SetColor(m_extraCols[3],3);
    return true;
}

void LImageSprites2::CopyChar()
{
    if (m_currencChar<0) return;
    m_copy = m_sprites[m_currencChar];
}

void LImageSprites2::PasteChar()
{
    if (m_copy.m_data.count()!=0)
        m_sprites[m_currencChar]=m_copy;

}

void LImageSprites2::FlipHorizontal()
{
    LSprite s = m_sprites[m_currencChar];
    float wx = (s.m_blocksWidth*s.m_pcWidth*8);
    float wy = (int)((s.m_blocksHeight*s.m_pcHeight*8.0));
    LSprite n;
    n.Initialize(s.m_blocksWidth,s.m_blocksHeight);
    for (int i=0;i<4;i++)
        SetColor(m_extraCols[i],i,n);

    for (float y=0;y<wy;y++) {
        for (float x=0;x<wx;x++) {
            float i = (x/(wx));
            float j = (y/(wy));
            unsigned int u = s.getPixel(i,j,m_bitMask);
            n.setPixel(i,1-j,u, m_bitMask);
        }

    }

    m_sprites[m_currencChar] = n;

}

void LImageSprites2::FlipVertical()
{
    LSprite s = m_sprites[m_currencChar];
    float wx = (s.m_blocksWidth*s.m_pcWidth*8);
    float wy = (int)((s.m_blocksHeight*s.m_pcHeight*8.0));
    LSprite n;
    n.Initialize(s.m_blocksWidth,s.m_blocksHeight);
    for (int i=0;i<4;i++)
        SetColor(m_extraCols[i],i,n);

    for (float y=0;y<wy;y++) {
        for (float x=0;x<wx;x++) {
            float i = ((x+0.5)/(wx));
            float j = (y/(wy));
            unsigned int u = s.getPixel(i,j,m_bitMask);
            n.setPixel(1-i,j,u, m_bitMask);
        }

    }

    m_sprites[m_currencChar] = n;

}

