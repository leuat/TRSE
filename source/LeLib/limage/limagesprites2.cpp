#include "limagesprites2.h"


LImageSprites2::LImageSprites2(LColorList::Type t) : CharsetImage(t) {
    m_type = LImage::Type::Sprites2;
    m_current=0;
    m_bitMask = 0b11;
    m_width = 160;
    m_height = 200;
    m_GUIParams[tabCharset] = "";
    m_GUIParams[tabData] = "";
    m_GUIParams[tabLevels] = "";
    m_GUIParams[tabEffects] = "";

    m_GUIParams[btnEditFullCharset] = "";
    m_GUIParams[tabSprites] ="Sprites";

    m_exportParams.clear();

    AddNew(1,1);
    //Data::data.currentColor=0;

}

void LImageSprites2::AddNew(int x, int y)
{
    LSprite s;
    s.Initialize(x,y);
    m_sprites.append(s);
    m_current = m_sprites.count()-1;
    SetColor(m_extraCols[0],0);
    SetColor(m_extraCols[1],1);
    SetColor(m_extraCols[2],2);

}

void LImageSprites2::ImportBin(QFile &f)
{
    QByteArray a = f.readAll();
    int cnt = a.count()/64;
    for (int i=0;i<cnt;i++) {
        LSprite s(a,i*64,m_bitMask);
        m_sprites.append(s);
        m_current = m_sprites.count()-1;
        for (int j=0;j<4;j++)
            SetColor(m_extraCols[j],j);
    }
}

void LImageSprites2::ExportBin(QFile &f)
{
    for (int i=0;i<m_sprites.count();i++) {
        f.write(m_sprites[i].toQByteArray(m_bitMask));
    }
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
         m_current = mc->m_current;
    }
    else
    {
        LImage::CopyFrom(img);
        return;
    }

}

void LImageSprites2::setPixel(int x, int y, unsigned int color)
{
    if (m_current<0)
        return;

    if (m_current>=m_sprites.count())
            return;


/*    LSprite& s = m_sprites[m_current];
    CharsetImage::setMultiColor(s.m_header[s.HEADER_MULTICOLOR]==(char)1);
*/
    float fx = x/(float)m_width;
    float fy = y/(float)m_height;

    bool fillColor = true;



    for (int i=0;i<3;i++) {
        if (m_extraCols[i]==color)
            fillColor = false;
    }
    if (fillColor)
        m_sprites[m_current].FillColor(color,3);



    m_sprites[m_current].setPixel(fx,fy,color,m_bitMask);

}

unsigned int LImageSprites2::getPixel(int x, int y)
{
    if (m_current<0)
        return 0;
    if (m_current>=m_sprites.count())
            return 0 ;


    LSprite& s = m_sprites[m_current];
    CharsetImage::setMultiColor(s.m_header[s.HEADER_MULTICOLOR]==(char)1);

    float fx = x/(float)m_width;
    float fy = y/(float)m_height;

    return m_sprites[m_current].getPixel(fx,fy,m_bitMask);

}

QByteArray LSprite::toQByteArray(int mask) {
    QByteArray data;


    QVector<QByteArray> grid;

    grid.resize(m_blocksWidth*(m_blocksHeight+1));

    for (int j=0;j<m_blocksHeight;j++) {
        for (int i=0;i<m_blocksWidth;i++) {
            QByteArray a;



/*            000 000 000 000
            000 000 000 000
            000 000 000 000

            000 000 000 000
            000 000 000 000
            000 000 000 000

  */


            int idx = (i*3+j*m_blocksWidth*3*3);
//            qDebug() << idx;
            a.append(grid[i+j*m_blocksWidth]); // Append current grid
            int nxGrid = i+(j+1)*m_blocksWidth;

            int yy=a.count()/3;

            for (int y=0;y<m_pcHeight;y++) {
                for (int j=0;j<8;j++) {
                    if (yy<21)
                        for (int k=0;k<3;k++)

                        a.append(m_data[idx+m_blocksWidth*m_pcWidth*y+k].flipSpriteBit(j,mask));
                    else
                        for (int k=0;k<3;k++)
                        grid[nxGrid].append(m_data[idx+m_blocksWidth*m_pcWidth*y+k].flipSpriteBit(j,mask));
                    yy++;
                }
            }
            a.append((char)0);
            data.append(a);
        }
    }
    data.resize(m_blocksHeight*m_blocksWidth*64);
    for (char& c: data) {
        c = PixelChar::reverse(c);
    }


    return data;
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
    //ix=((int)(ix*8.0)/scale)&((8/scale)-1);
    if (scale==1) {
        ix = (int)(ix*8.0)&7;
    }
    else
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
        file.write(s.m_header);

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
        s.m_header = file.read(s.HEADER_SIZE);

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
    m_current = 0;

    m_extraCols[0] = m_background;
    SetColor(m_extraCols[1],1);
    SetColor(m_extraCols[2],2);


}

void LImageSprites2::SetColor(uchar col, uchar idx)
{
    m_color.c[idx] = col;

    if (idx==0)
        m_background = col;

    if (m_current>=0)

    for (int i=0;i<m_sprites[m_current].m_data.count();i++)
        m_sprites[m_current].m_data[i].c[idx] = col;


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
        Prev();
    if (e->key()==Qt::Key_D)
        Next();


    if (m_current>2500) m_current=0;
    if (m_current>=m_sprites.count())
        m_current=m_sprites.count()-1;
//    m_current = Util::clamp(m_current,0,m_sprites.count()-1);


    SetColor(m_extraCols[0],0);
    SetColor(m_extraCols[1],1);
    SetColor(m_extraCols[2],2);
//   SetColor(m_extraCols[3],3);
    return true;
}

void LImageSprites2::CopyChar()
{
    if (m_current<0) return;
    m_copy = m_sprites[m_current];
}

void LImageSprites2::PasteChar()
{
    if (m_copy.m_data.count()==0)
        return;

//    if (m_sprites[m_current].m_blocksHeight == m_copy.m_blocksHeight)
    m_sprites[m_current]=m_copy;

}

void LImageSprites2::ToQImage(LColorList &lst, QImage &img, float zoom, QPointF center)
{
    LSprite& s = m_sprites[m_current];
    CharsetImage::setMultiColor(s.m_header[s.HEADER_MULTICOLOR]==(char)1);

    img = img.scaled(m_width,m_height);

    MultiColorImage::ToQImage(lst,img,zoom,center);
}

void LImageSprites2::ToggleSpriteMulticolor()
{
    LSprite& s = m_sprites[m_current];

    s.m_header[s.HEADER_MULTICOLOR]=(s.m_header[s.HEADER_MULTICOLOR]+1)&1;


}
// Transforms x/y, flips
void LImageSprites2::MegaTransform(int flip, int ix, int iy)
{
    LSprite s = m_sprites[m_current];
    float wx = (s.m_blocksWidth*s.m_pcWidth*8);
    float wy = (int)((s.m_blocksHeight*s.m_pcHeight*8.0));
    LSprite n;
    n.Initialize(s.m_blocksWidth,s.m_blocksHeight);
    n.m_header = s.m_header;
    for (int i=0;i<4;i++)
        SetColor(m_extraCols[i],i,n);




    float ddx = 0.5;
    float ddy = 0.5;
    if (flip==3) {
        if (ix<0) {
           ddx=-0.05;
        }
        if (iy<0)
           ddy*=-1;
    }

    for (float y=0;y<wy;y++) {
        for (float x=0;x<wx;x++) {

            float i = ((x+ddx)/(wx));
            float j = ((y+ddy)/(wy));

            float ii = ((x+ix+ddx)/(wx));
            float jj = ((y+iy+ddy)/(wy));


            if (jj>=1) jj-=1;
            if (jj<0) jj+=1;
            if (ii>=1) ii-=1;
            if (ii<0) ii+=1;

            unsigned int u = s.getPixel(ii,jj,m_bitMask);
            if (flip==1)
                n.setPixel(i,1-j,u, m_bitMask);
            if (flip==0)
                n.setPixel(1-i,j,u, m_bitMask);
            if (flip==3) {
                n.setPixel(i,j,u, m_bitMask);

            }
        }

    }

    m_sprites[m_current] = n;

}

void LImageSprites2::FlipHorizontal()
{
    MegaTransform(1,0,0);
}

void LImageSprites2::FlipVertical()
{
    MegaTransform(0,0,0);

}

void LImageSprites2::Transform(int x, int y)
{
    MegaTransform(3,x,y);

}

void LImageSprites2::Delete()
{
    if (m_sprites.count()>1) {

        if (m_current>=m_sprites.count())
            m_current = m_sprites.count()-1;
        m_sprites.remove(m_current);
        if (m_current>=m_sprites.count())
            m_current = m_sprites.count()-1;
    }

}

void LImageSprites2::ShiftSprites(int i)
{
    int nxt = m_current+i;
 //   qDebug() << "Cur: " << m_current;
   // qDebug() << "next: " << nxt;
    if (nxt<0)
        return;
    if (nxt>=m_sprites.count())
        return;
    if (m_sprites.count()<=1)
        return;

    LSprite tmp = m_sprites[m_current];
    m_sprites[m_current] = m_sprites[nxt];
    m_sprites[nxt] = tmp;

    m_current = nxt;

}

