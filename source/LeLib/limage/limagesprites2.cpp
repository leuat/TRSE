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
    m_GUIParams[tabEffects] = "Effects";

    m_GUIParams[btnEditFullCharset] = "";
    m_GUIParams[tabSprites] ="Sprites";

//    m_supports.displayForeground = true;
//    m_GUIParams[col3] = "Multicolor 2";
  //  m_GUIParams[col4] = "Multicolor 1";

    m_exportParams.clear();
    m_supports.displayCharOperations = false;
    m_footer.set(LImageFooter::POS_DISPLAY_MULTICOLOR,0);

    AddNew(1,1);
    //Data::data.currentColor=0;

}

QPoint LImageSprites2::getActualPixelWidth(){
    return QPoint(24*m_items[m_current]->m_width,m_items[m_current]->m_height*21);

}

void LImageSprites2::AddNew(int w, int h)
{
    LSprite* s = new LSprite();
    s->Init(w,h);
    m_items.append(QSharedPointer<LSprite>(s));
    m_current = m_items.count()-1;
    //SetColor(m_extraCols[0],0);
    //SetColor(m_extraCols[1],1);
    //SetColor(m_extraCols[2],2);

}

void LImageSprites2::ImportBin(QFile &f)
{
    QByteArray a = f.readAll();
    int cnt = a.length()/64;
    for (int i=0;i<cnt;i++) {
        LSprite* s = new LSprite(a,i*64,m_bitMask);
        m_items.append(QSharedPointer<LSprite>(s));
        m_current = m_items.count()-1;
        //for (int j=0;j<4;j++)
          //  SetColor(m_extraCols[j],j);
    }
}

void LImageSprites2::ExportBin(QFile &f)
{

    for (int i=0;i<m_items.count();i++) {
        m_bitMask = ((LSprite*) m_items[i].get())->m_header[LSprite::HEADER_MULTICOLOR];
        if (m_bitMask ==0) m_bitMask = 0b1; else m_bitMask=0b11;
//        qDebug() << m_bitMask;
        f.write(m_items[i]->ToQByteArray(m_bitMask));
    }
}

QString LImageSprites2::getMetaInfo() {
    QString txt = "C64 sprites consist of 64-byte blocks. This editor lets you create both hires (24x21) or multicolor (12x21, 4(3) colour) sprites. \n";
    txt +="\nYou can also create meta-blocks of NxM sprites for larger images. Cycle through sprites by pressing A/D, and use the \"Sprites\" tab to create new (blocks of) sprites, rearrange or perform other operations. \n";
    txt+="\nWhen exported (either by clicking the \"export\" button or by using the TRSE @export command), the sprites are saved seqentially as 64-byte blocks.";

    return txt;
}

void LImageSprites2::CopyFrom(LImage *img)
{
    LImageSprites2* mc = dynamic_cast<LImageSprites2*>(img);
    if (mc!=nullptr)
    {
        m_colorList.CopyFrom(&img->m_colorList);
         //m_extraCols = mc.m_extraCols;
         m_width = mc->m_width;
         m_height = mc->m_height;
         m_bitMask = mc->m_bitMask;
//         m_items = mc->m_items;

         DeleteAll();
         for (auto li: mc->m_items) {
             LSprite* s= (LSprite*)li.get();
             LSprite* s2= new LSprite();
             *s2 = *s;

             m_items.append(QSharedPointer<LSprite>(s2));
         }


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

    if (m_current>=m_items.count())
            return;


/*    LSprite& s = m_items[m_current];
    CharsetImage::setMultiColor(s.m_header[s.HEADER_MULTICOLOR]==(char)1);
*/
    float fx = x/(float)m_width;
    float fy = y/(float)m_height;

    bool fillColor = true;



    for (int i=0;i<3;i++) {
        if (m_colorList.getPen(i)==color)
            fillColor = false;
    }




    if (fillColor)
        ((LSprite*)m_items[m_current].get())->FillColor(color,3);



    ((LSprite*)m_items[m_current].get())->setPixel(fx,fy,color,m_bitMask);

}

unsigned int LImageSprites2::getPixel(int x, int y)
{
    if (m_current<0)
        return 0;
    if (m_current>=m_items.count())
            return 0 ;


    LSprite* s = (LSprite*)m_items[m_current].get();
//    CharsetImage::setMultiColor(s->m_header[s->HEADER_MULTICOLOR]==(char)1);

    double fx = x/(double)m_width;
    double fy = y/(double)m_height;

//    if (rand()%100>98)
  //      qDebug() <<s->getPixel(fx,fy,m_bitMask);

    return s->getPixel(fx,fy,m_bitMask);

}

QByteArray LSprite::ToQByteArray(int mask) {
    QByteArray data;

    QVector<QByteArray> grid;

    grid.resize(m_width*(m_height+1));

    for (int j=0;j<m_height;j++) {
        for (int i=0;i<m_width;i++) {
            QByteArray a;



/*            000 000 000 000
            000 000 000 000
            000 000 000 000

            000 000 000 000
            000 000 000 000
            000 000 000 000

  */


            int idx = (i*3+j*m_width*3*3);
//            qDebug() << idx;
            a.append(grid[i+j*m_width]); // Append current grid
            int nxGrid = i+(j+1)*m_width;

            int yy=a.length()/3;

            for (int y=0;y<m_pcHeight;y++) {
                for (int j=0;j<8;j++) {
                    if (yy<21)
                        for (int k=0;k<3;k++)

                        a.append(m_data[idx+m_width*m_pcWidth*y+k].flipSpriteBit(j,mask));
                    else
                        for (int k=0;k<3;k++)
                        grid[nxGrid].append(m_data[idx+m_width*m_pcWidth*y+k].flipSpriteBit(j,mask));
                    yy++;
                }
            }
            a.append((char)0);
            data.append(a);
        }
    }
    data.resize(m_height*m_width*64);
    for (char& c: data) {
        c = PixelChar::reverse(c);
    }


    return data;
}

PixelChar *LSprite::GetSetData(double x, double y, double &ix, double &iy, uchar bitMask)
{
    ix = x*m_width*3.0f;
    iy = y*m_height*3.0f/24*21.0f;
    if (iy<0 || iy>=m_height*3) return nullptr;
    if (ix<0 || ix>=m_width*3) return nullptr;
    int scale = 1;
    if (bitMask==0b11) scale =2;
    int v = (int)ix+(int)iy*m_pcHeight*m_width;
    if (v<0 || v>=m_data.count())
        return nullptr;
    //PixelChar& pc = m_data[v];
    iy=((int)(iy*8.0));
    if (iy>m_height*21)
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

void LSprite::setPixel(double x, double y, uchar color, uchar bitMask)
{
    double ix, iy;
    PixelChar* pc = GetSetData(x,y,ix,iy, bitMask);
    if (pc==nullptr)
        return;
    pc->set(ix,iy,color,bitMask);

}

uchar LSprite::getPixel(double x, double y, uchar bitMask)
{
    double ix, iy;
    PixelChar* pc = GetSetData(x,y,ix,iy, bitMask);
    if (pc==nullptr)
        return 0;
    return pc->get(ix,iy,bitMask);

}



void LImageSprites2::SaveBin(QFile& file)
{
    SavePensBin(file);
    uchar cnt = m_items.count();

    file.write( ( char * )( &cnt ), 1 );
    for (auto li : m_items) {

        auto s = qSharedPointerDynamicCast<LSprite>(li);
        uchar sx = s->m_width;
        uchar sy = s->m_height;

        file.write( ( char * )( &sx ), 1 );
        file.write( ( char * )( &sy ), 1 );
//        file.write( ( char * )( &m_data ),  m_charHeight*m_charWidth*12 );
        file.write(s->m_header);

        QByteArray data;
        for (PixelChar& pc: s->m_data) {
                    for (int j=0;j<8;j++)
                        data.append(pc.p[j]);
                    for (int j=0;j<4;j++)
                        data.append(pc.c[j]);
                }

        file.write( data, sx*sy*s->m_pcHeight*s->m_pcWidth*12);

    }

}

void LImageSprites2::LoadBin(QFile& file)
{
    m_items.clear();
    LoadPensBin(file);
/*    file.read( ( char * )( &m_background ),  1 );
    file.read( ( char * )( &m_border ), 1 );
    file.read( ( char * )( &m_extraCols[1] ), 1 );
    file.read( ( char * )( &m_extraCols[2] ), 1 );
    file.read( ( char * )( &m_extraCols[3] ), 1 );*/
    uchar cnt;
    file.read( ( char * )( &cnt ), 1 );
    for (int i=0;i<cnt;i++) {
        uchar sx,sy;
        file.read( ( char * )( &sx ), 1 );
        file.read( ( char * )( &sy ), 1 );


        LSprite* s = new LSprite();

        s->Init(sx,sy);
        s->m_header = file.read(s->HEADER_SIZE);

        QByteArray data = file.read(sx*sy*s->m_pcHeight*s->m_pcWidth*12);
        int c=0;
        for (PixelChar& pc: s->m_data) {
            for (int j=0;j<8;j++)
                pc.p[j] = data[12*c+j];
            for (int j=0;j<4;j++)
                pc.c[j] = data[12*c+j+8];
            c++;
        }
        m_items.append(QSharedPointer<LSprite>(s));
    }
    m_current = 0;

    //m_extraCols[0] = m_background;
    //SetColor(m_extraCols[1],1);
    //SetColor(m_extraCols[2],2);


}

void LImageSprites2::SetColor(uchar col, uchar idx)
{
    m_color.c[idx] = col;

//    if (idx==0)
  //      m_background = col;

//    qDebug() << "SETCOLOR index " +QString::number(idx) << "  " << QString::number(col)  << "   " <<QString::number(m_current);
    //if (m_current>=0 && idx==3)

    {

        for (int i=0;i<((LSprite*)m_items[m_current].get())->m_data.count();i++)
            ((LSprite*)m_items[m_current].get())->m_data[i].c[idx] = col;



    }
    //m_extraCols[idx] = col;
}

void LImageSprites2::SetColor(uchar col, uchar idx, LSprite &s)
{

    m_color.c[idx] = col;

//    if (idx==0)
  //      m_background = col;

    for (int i=0;i<s.m_data.count();i++)
        s.m_data[i].c[idx] = col;

    //m_extraCols[idx] = col;
}

bool LImageSprites2::KeyPress(QKeyEvent *e)
{

    if (e->key()==Qt::Key_A)
        Prev();
    if (e->key()==Qt::Key_D)
        Next();


    if (m_current>2500) m_current=0;
    if (m_current>=m_items.count())
        m_current=m_items.count()-1;
//    m_current = Util::clamp(m_current,0,m_items.count()-1);


    //SetColor(m_extraCols[0],0);
    //SetColor(m_extraCols[1],1);
    //SetColor(m_extraCols[2],2);
//   SetColor(m_extraCols[3],3);
    return true;
}

void LImageSprites2::CopyChar()
{
    if (m_current<0) return;
    m_copy = *((LSprite*)m_items[m_current].get());
}

void LImageSprites2::PasteChar()
{
    if (m_copy.m_data.count()==0)
        return;

//    if (m_items[m_current].m_height == m_copy.m_height)
    *((LSprite*)m_items[m_current].get())=m_copy;

}


void LImageSprites2::InitPens()
{
    LSprite* s = ((LSprite*)m_items[m_current].get());
    m_colorList.SetC64SpritePen(true);
    for (int i=0;i<s->m_data.count();i++) {
        s->m_data[i].c[0] = m_colorList.getPen(0);
        s->m_data[i].c[1] = m_colorList.getPen(1);
        s->m_data[i].c[2] = m_colorList.getPen(2);
    }



}

void LImageSprites2::ToQImage(LColorList &lst, QImage &img, double zoom, QPointF center)
{
    LSprite* s = ((LSprite*)m_items[m_current].get());
    CharsetImage::setMultiColor(s->m_header[s->HEADER_MULTICOLOR]==(char)1);

    img = img.scaled(m_width,m_height);

    MultiColorImage::ToQImage(lst,img,zoom,center);
}

int LImageSprites2::getGridWidth() {
    return m_items[m_current]->m_width*24;
}

int LImageSprites2::getGridHeight(){

    return m_items[m_current]->m_height*21;
}



void LImageSprites2::ToggleSpriteMulticolor()
{
    LSprite* s = ((LSprite*)m_items[m_current].get());

    s->m_header[s->HEADER_MULTICOLOR]=(s->m_header[s->HEADER_MULTICOLOR]+1)&1;

    InitPens();

}
// Transforms x/y, flips
void LImageSprites2::MegaTransform(int flip, int ix, int iy)
{
    LSprite* s = ((LSprite*)m_items[m_current].get());
    double wx = (s->m_width*s->m_pcWidth*8);
    double wy = (int)((s->m_height*s->m_pcHeight*8.0));
    LSprite n;
    n.Init(s->m_width, s->m_height);
    n.m_data  =s->m_data;
    n.m_header = s->m_header;
    n.m_pcHeight = s->m_pcHeight;
    n.m_pcWidth = s->m_pcWidth;




    float ddx = 0.5;
    float ddy = 0.5;
    if (flip==3) {
        if (ix<0) {
           ddx=-0.05;
        }
        if (iy<0)
           ddy*=-1;
    }
    int step=1;
    if (m_bitMask==0b11) step=2;

    for (float y=0;y<wy;y++) {
        for (float x=0;x<wx;x+=step) {

            double i = ((x+ddx)/(wx));
            double j = ((y+ddy)/(wy));

            double ii = ((x+ix+ddx)/(wx));
            double jj = ((y+iy+ddy)/(wy));


            if (jj>=1) jj-=1;
            if (jj<0) jj+=1;
            if (ii>=1) ii-=1;
            if (ii<0) ii+=1;

            unsigned int u = s->getPixel(ii,jj,m_bitMask);

            if (flip==1)
                n.setPixel(i,1-j,u, m_bitMask);

            if (flip==0)
                n.setPixel(1-i,j,u, m_bitMask);
            if (flip==3) {
                n.setPixel(i,j,u, m_bitMask);

            }
        }

    }

    m_items[m_current] = QSharedPointer<LSprite>(new LSprite(n));

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

void LImageSprites2::OrdererdDither(QImage &img, LColorList &colors, QVector3D strength, QPoint size, float gamma)
{
    int height  =std::min(img.height(), m_height);
    int width  =std::min(img.width(), m_width);
    QMatrix4x4 bayer4x4 = QMatrix4x4(0,8,2,10,  12,4,14,6, 3,11,1,9, 15,7,13,5);
    bayer4x4 = bayer4x4*1/16.0*strength.x();

    /*    QMatrix4x4 bayer4x4 = QMatrix4x4(0,2,0,0,  3,1,0,0, 0,0,0,0, 0,0,0,0);
       bayer4x4 = bayer4x4*1/4.0*strength.x();
   */

    QVector<QPoint> hist;
    hist.resize(16);

    //  QElapsedTimer timer;
    //    timer.start();
//    LSprite* s = ((LSprite*)m_items[m_current]);

    float w = m_width;//s->m_width*12*m_scale;
    float h = m_height;//s->m_height*21;


    for (int y=0;y<h;y++) {
        for (int x=0;x<w;x++) {

            //            color.R = color.R + bayer8x8[x % 8, y % 8] * GAP / 65;

            double dx = x/(double)w*img.width();
            double dy = y/(double)h*img.height();
            int xx = (dx-img.width()/2.0)*m_importScaleX + img.width()/2.0;
            int yy = (dy-img.height()/2.0)*m_importScaleY + img.height()/2.0;


            QColor color = QColor(img.pixel(xx,yy));
            int yp = y + x%(int)strength.y();
            int xp = x + y%(int)strength.z();
            color.setRed(std::min((float)pow(color.red(),gamma) + bayer4x4(xp % size.x(),yp % size.y()),255.0f));
            color.setGreen(std::min((float)pow(color.green(),gamma) + bayer4x4(xp % size.x(),yp % size.y()),255.0f));
            color.setBlue(std::min((float)pow(color.blue(),gamma) + bayer4x4(xp % size.x(),yp % size.y()),255.0f));

            int winner = 0;
            QColor newPixel = colors.getClosestColor(color, winner);
            //            PixelChar& pc = getPixelChar(x,y);
            setPixel(x,y,winner);

        }
    }
    //qDebug() << "The slow operation took" << timer.elapsed() << "milliseconds";

}


