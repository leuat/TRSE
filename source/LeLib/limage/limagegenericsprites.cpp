#include "limagegenericsprites.h"


LImageGenericSprites::LImageGenericSprites(LColorList::Type t) : LImageQImage(t) {
    m_type = LImage::Type::GenericSprites;
    m_current=0;
    m_width = 320;
    m_height = 200;
    m_GUIParams[tabCharset] = "";
    m_GUIParams[tabData] = "";
    m_GUIParams[tabLevels] = "";
    m_GUIParams[tabEffects] = "Effects";

    m_GUIParams[btnEditFullCharset] = "";
    m_GUIParams[tabSprites] ="Sprites";

//    m_supports.displayForeground = true;

    m_exportParams.clear();
    m_supports.displayCharOperations = false;
    m_footer.set(LImageFooter::POS_DISPLAY_MULTICOLOR,0);

    AddNew(1,1);
    //Data::data.currentColor=0;

}

QPoint LImageGenericSprites::getActualPixelWidth(){
    return QPoint(8*m_items[m_current]->m_width,m_items[m_current]->m_height*8);

}

void LImageGenericSprites::AddNew(int w, int h)
{
    LGenericSprite* s = new LGenericSprite();
    s->Init(w,h);
    m_items.append(s);
    m_current = m_items.count()-1;
    //SetColor(m_extraCols[0],0);
    //SetColor(m_extraCols[1],1);
    //SetColor(m_extraCols[2],2);

}

void LImageGenericSprites::ImportBin(QFile &f)
{
    QByteArray a = f.readAll();
    int cnt = a.count()/64;
    for (int i=0;i<cnt;i++) {
        LGenericSprite* s = new LGenericSprite(a,i*64,m_bitMask);
        m_items.append(s);
        m_current = m_items.count()-1;
        //for (int j=0;j<4;j++)
          //  SetColor(m_extraCols[j],j);
    }
}

void LImageGenericSprites::ExportBin(QFile &f)
{

    for (int i=0;i<m_items.count();i++) {
        auto s = ((LGenericSprite*) m_items[i]);
        f.write(s->m_data.toQByteArray());
    }
}

QString LImageGenericSprites::getMetaInfo() {
    QString txt = "C64 sprites consist of 64-byte blocks. This editor lets you create both hires (24x21) or multicolor (12x21, 4(3) colour) sprites. \n";
    txt +="\nYou can also create meta-blocks of NxM sprites for larger images. Cycle through sprites by pressing A/D, and use the \"Sprites\" tab to create new (blocks of) sprites, rearrange or perform other operations. \n";
    txt+="\nWhen exported (either by clicking the \"export\" button or by using the TRSE @export command), the sprites are saved seqentially as 64-byte blocks.";

    return txt;
}

void LImageGenericSprites::CopyFrom(LImage *img)
{
    LImageGenericSprites* mc = dynamic_cast<LImageGenericSprites*>(img);
    if (mc!=nullptr)
    {
        m_colorList.CopyFrom(&img->m_colorList);
         //m_extraCols = mc.m_extraCols;
         m_width = mc->m_width;
         m_height = mc->m_height;
         m_bitMask = mc->m_bitMask;
//         m_items = mc->m_items;

         DeleteAll();
         for (LImageContainerItem* li: mc->m_items) {
             LGenericSprite* s= (LGenericSprite*)li;
             LGenericSprite* s2= new LGenericSprite();
             s2->Init(s->m_width,s->m_height);
             s2->m_data.CopyFrom(&s->m_data);
             //*s2 = *s;

             m_items.append(s2);
         }


         m_current = mc->m_current;
    }
    else
    {
        LImage::CopyFrom(img);
        return;
    }

}

void LImageGenericSprites::setPixel(int x, int y, unsigned int color)
{
    if (m_current<0)
        return;

    if (m_current>=m_items.count())
            return;


/*    LGenericSprite& s = m_items[m_current];
    CharsetImage::setMultiColor(s.m_header[s.HEADER_MULTICOLOR]==(char)1);
*/
    LGenericSprite* s = (LGenericSprite*)m_items[m_current];
    auto p = getPixelPos(x,y);

    s->m_data.setPixel(p.x(),p.y(),color);

}

unsigned int LImageGenericSprites::getPixel(int x, int y)
{
    if (m_current<0)
        return 0;
    if (m_current>=m_items.count())
            return 0 ;


    LGenericSprite* s = (LGenericSprite*)m_items[m_current];
//    CharsetImage::setMultiColor(s->m_header[s->HEADER_MULTICOLOR]==(char)1);
    auto p = getPixelPos(x,y);

//    if (rand()%100>98)
  //      qDebug() <<s->getPixel(fx,fy,m_bitMask);

    return s->m_data.getPixel(p.x(),p.y());

}

QByteArray LGenericSprite::ToQByteArray(int mask) {

    return QByteArray();
}

QPointF LImageGenericSprites::getPixelPos(int x, int y) {
    LGenericSprite* s = (LGenericSprite*)m_items[m_current];
    return QPointF(x/(double)m_width*s->m_width*8, y/(double)m_height*s->m_height*8);

}


void LGenericSprite::setPixel(double x, double y, uchar color, uchar bitMask)
{

}

uchar LGenericSprite::getPixel(double x, double y, uchar bitMask)
{
    return 0;
}



void LImageGenericSprites::SaveBin(QFile& file)
{
    uchar cnt = m_items.count();

    file.write( ( char * )( &cnt ), 1 );
    for (LImageContainerItem* li : m_items) {

        LGenericSprite* s = dynamic_cast<LGenericSprite*>(li);
        uchar sx = s->m_width;
        uchar sy = s->m_height;

        file.write( ( char * )( &sx ), 1 );
        file.write( ( char * )( &sy ), 1 );
//        file.write( ( char * )( &m_data ),  m_charHeight*m_charWidth*12 );
        file.write(s->m_header);


        char* tst = (char*)s->m_data.m_qImage->constBits();
//        for (int i=0;i<sx*sy*8*8;i++)
  //          qDebug().noquote()<<(QString::number(tst[i]));

        file.write( s->m_data.toQByteArray(), sx*sy*8*8);

    }


}

void LImageGenericSprites::LoadBin(QFile& file)
{
    m_items.clear();
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


        LGenericSprite* s = new LGenericSprite();

        s->Init(sx,sy);
        s->m_header = file.read(s->HEADER_SIZE);

        QByteArray data = file.read(sx*sy*8*8);
        s->m_data.fromQByteArray(data);
        m_items.append(s);
    }
    m_current = 0;

    //m_extraCols[0] = m_background;
    //SetColor(m_extraCols[1],1);
    //SetColor(m_extraCols[2],2);

}

void LImageGenericSprites::SetColor(uchar col, uchar idx)
{
}

bool LImageGenericSprites::KeyPress(QKeyEvent *e)
{

    if (e->key()==Qt::Key_A)
        Prev();
    if (e->key()==Qt::Key_D)
        Next();

    //SetColor(m_extraCols[0],0);
    //SetColor(m_extraCols[1],1);
    //SetColor(m_extraCols[2],2);
//   SetColor(m_extraCols[3],3);
    return true;
}

void LImageGenericSprites::CopyChar()
{
}

void LImageGenericSprites::PasteChar()
{

}


void LImageGenericSprites::InitPens()
{


}

void LImageGenericSprites::ToQImage(LColorList &lst, QImage &img, double zoom, QPointF center)
{
    LGenericSprite* s = ((LGenericSprite*)m_items[m_current]);
    if (s->m_data.m_qImage==nullptr)
        return;
    img = QImage(s->m_data.m_qImage->width(),s->m_data.m_qImage->height(), QImage::Format_ARGB32);
//    qDebug() << s->m_data.m_qImage->width()<<img.width();
//    exit(1);
    s->m_data.ToQImage(lst,img,zoom,center);
}

int LImageGenericSprites::getGridWidth() {
    return m_items[m_current]->m_width*24;
}

int LImageGenericSprites::getGridHeight(){

    return m_items[m_current]->m_height*21;
}



void LImageGenericSprites::ToggleSpriteMulticolor()
{
    LGenericSprite* s = ((LGenericSprite*)m_items[m_current]);

    s->m_header[s->HEADER_MULTICOLOR]=(s->m_header[s->HEADER_MULTICOLOR]+1)&1;

    InitPens();

}
// Transforms x/y, flips
void LImageGenericSprites::MegaTransform(int flip, int ix, int iy)
{
}

void LImageGenericSprites::FlipHorizontal()
{
    MegaTransform(1,0,0);
}

void LImageGenericSprites::FlipVertical()
{
    MegaTransform(0,0,0);
}

void LImageGenericSprites::Transform(int x, int y)
{
    MegaTransform(3,x,y);

}


