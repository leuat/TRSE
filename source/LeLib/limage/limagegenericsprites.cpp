#include "limagegenericsprites.h"
#include "source/Compiler/syntax.h"

LGenericSprite LImageGenericSprites::m_copy;

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
    return QPoint(m_blockSize*m_items[m_current]->m_width,m_items[m_current]->m_height*m_blockSize);

}

void LImageGenericSprites::AddNew(int w, int h)
{
    auto s = QSharedPointer<LGenericSprite>(new LGenericSprite());
    s->m_blockSize = m_blockSize;
    s->Init(w,h);
    m_items.insert(m_current,s);

//    m_items.append(s);
//    m_current = m_items.length()-1;
    //SetColor(m_extraCols[0],0);
    //SetColor(m_extraCols[1],1);
    //SetColor(m_extraCols[2],2);

}

void LImageGenericSprites::ImportBin(QFile &f)
{
    QByteArray a = f.readAll();
    int cnt = a.length()/64;
    for (int i=0;i<cnt;i++) {
        auto s = QSharedPointer<LGenericSprite>(new LGenericSprite(a,i*64,m_bitMask));
        m_items.append(s);
        m_current = m_items.length()-1;
        //for (int j=0;j<4;j++)
          //  SetColor(m_extraCols[j],j);
    }
}

void LImageGenericSprites::ExportBin(QFile &f)
{

    for (int i=0;i<m_items.count();i++) {
        auto s = ((LGenericSprite*) m_items[i].get());
        QByteArray data = s->m_data.toQByteArray(Syntax::s.m_currentSystem->m_system==AbstractSystem::X16);
//        qDebug() << "Exporting sprite... "<<i << (data.size()/64);;

        f.write(data);
//        f.write(s->m_data.toQByteArray(false));
    }

    QString palName = Util::getFileWithoutEnding(f.fileName()) + ".pal";
    if (QFile::exists(palName))
        QFile::remove(palName);

    m_colorList.ExportVGAPalette(palName);

}

QString LImageGenericSprites::getMetaInfo() {
    QString txt = "Generix NxM blocks of 8x8 pixel sprites.  \n";

    return txt;
}

void LImageGenericSprites::Clear(int val) {
    if (m_current<0 || m_current>=m_items.count())
        return;

    m_items[m_current]->Clear(0);
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
         for (auto li: mc->m_items) {
             LGenericSprite* s=  (LGenericSprite*)li.get();
             auto s2= QSharedPointer<LGenericSprite>(new LGenericSprite());
             s2->Init(s->m_width,s->m_height);
             s2->m_data.CopyFrom(&s->m_data);
             s2->m_data.usePens = s->m_data.usePens;
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
    LGenericSprite* s = (LGenericSprite*)m_items[m_current].get();

    auto p = getPixelPos(x,y);

    s->m_data.setPixel(p.x(),p.y(),color);

}

unsigned int LImageGenericSprites::getPixel(int x, int y)
{
    if (m_current<0)
        return 0;
    if (m_current>=m_items.count())
            return 0 ;


    LGenericSprite* s = (LGenericSprite*)m_items[m_current].get();
//    CharsetImage::setMultiColor(s->m_header[s->HEADER_MULTICOLOR]==(char)1);
    auto p = getPixelPos(x,y);

//    if (rand()%100>98)
  //      qDebug() <<s->getPixel(fx,fy,m_bitMask);

    return s->m_data.getPixel(p.x(),p.y());

}

QByteArray LGenericSprite::ToQByteArray(int mask) {

    return QByteArray();
}

void LImageGenericSprites::Duplicate() {

    LGenericSprite* s = (LGenericSprite*)m_items[m_current].get();
    auto s2= QSharedPointer<LGenericSprite>(new LGenericSprite());
    s2->Init(s->m_width,s->m_height);
    s2->m_data.CopyFrom(&s->m_data);
    m_items.insert(m_current,s2);

}

QPointF LImageGenericSprites::getPixelPos(int x, int y) {
    LGenericSprite* s = (LGenericSprite*)m_items[m_current].get();
    return QPointF(x/(double)m_width*s->m_width*m_blockSize, y/(double)m_height*s->m_height*m_blockSize);

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
    for (auto li : m_items) {

        auto s = qSharedPointerDynamicCast<LGenericSprite>(li);
        uchar sx = s->m_width;
        uchar sy = s->m_height;

        file.write( ( char * )( &sx ), 1 );
        file.write( ( char * )( &sy ), 1 );
//        file.write( ( char * )( &m_data ),  m_charHeight*m_charWidth*12 );
        file.write(s->m_header);


        char* tst = (char*)s->m_data.m_qImage->constBits();
//        for (int i=0;i<sx*sy*8*8;i++)
  //          qDebug().noquote()<<(QString::number(tst[i]));

        file.write( s->m_data.toQByteArray(), sx*sy*m_blockSize*m_blockSize);

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


        auto s = QSharedPointer<LGenericSprite>(new LGenericSprite());
        s->m_blockSize = m_blockSize;
        s->Init(sx,sy);
        s->m_header = file.read(s->HEADER_SIZE);

        QByteArray data = file.read(sx*sy*m_blockSize*m_blockSize);
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
    LGenericSprite* s = (LGenericSprite*)m_items[m_current].get();
    m_copy.Init(s->m_width,s->m_height);
    m_copy.m_data.CopyFrom(&s->m_data);
}

void LImageGenericSprites::PasteChar()
{
    if (m_copy.m_width==0 || m_copy.m_height==0)
        return;
    LGenericSprite* s = (LGenericSprite*)m_items[m_current].get();
    s->Init(m_copy.m_width,m_copy.m_height);
    s->m_data.CopyFrom(&m_copy.m_data);

}


void LImageGenericSprites::InitPens()
{
    for (auto&i : m_items) {
        LGenericSprite* s = ((LGenericSprite*)i.get());
        s->m_data.m_colorList.CopyFrom(&m_colorList);

    }

}

void LImageGenericSprites::ToQImage(LColorList &lst, QImage &img, double zoom, QPointF center)
{
    LGenericSprite* s = ((LGenericSprite*)m_items[m_current].get());
    if (s->m_data.m_qImage==nullptr)
        return;
    img = QImage(s->m_data.m_qImage->width(),s->m_data.m_qImage->height(), QImage::Format_ARGB32);
//    qDebug() << s->m_data.m_qImage->width()<<img.width();
//    exit(1);
    s->m_data.ToQImage(lst,img,zoom,center);
}

int LImageGenericSprites::getGridWidth() {
    return m_items[m_current]->m_width*m_blockSize;
}

int LImageGenericSprites::getGridHeight(){

    return m_items[m_current]->m_height*m_blockSize;
}

QPoint LImageGenericSprites::GetCurrentPosInImage(float x, float y) {
    return QPoint(x/m_width*m_items[m_current]->m_width*m_blockSize,y/m_height*m_items[m_current]->m_height*m_blockSize);
}

QString LImageGenericSprites::getSpriteInfo() {
    auto tx = m_items[m_current]->m_width*m_blockSize;
    auto ty = m_items[m_current]->m_height*m_blockSize;
    return "Block size: ("+QString::number(m_items[m_current]->m_width)+","+QString::number(m_items[m_current]->m_height)+") "+
           "    Pixel size: ("+QString::number(tx) +"px," +QString::number(ty) +"px)";
}



void LImageGenericSprites::ToggleSpriteMulticolor()
{
    LGenericSprite* s = ((LGenericSprite*)m_items[m_current].get());

    s->m_header[s->HEADER_MULTICOLOR]=(s->m_header[s->HEADER_MULTICOLOR]+1)&1;

    InitPens();

}
// Transforms x/y, flips
void LImageGenericSprites::MegaTransform(int flip, int ix, int iy)
{
    auto s = ((LGenericSprite*)m_items[m_current].get());
    int wx = (s->m_width*m_blockSize);
    int wy = (s->m_height*m_blockSize);
    LGenericSprite* n = new LGenericSprite();
    n->Init(s->m_width, s->m_height);
    n->Clear(0);
    n->m_data.CopyFrom(&s->m_data);
//    n->m_header = s->m_header;




/*    float ddx = 0.5;
    float ddy = 0.5;
    if (flip==3) {
        if (ix<0) {
           ddx=-0.05;
        }
        if (iy<0)
           ddy*=-1;
    }*/
    int step=1;
//    if (m_bitMask==0b11) step=2;
//    int keep = m_footer.get(LImageFooter::POS_DISPLAY_CHAR);
    s->m_data.m_footer.set(LImageFooter::POS_DISPLAY_CHAR,0);
    n->m_data.m_footer.set(LImageFooter::POS_DISPLAY_CHAR,0);

    for (float y=0;y<wy;y++) {
        for (float x=0;x<wx;x+=step) {

            double i = ((x));
            double j = ((y));

            double ii = ((int)(x+ix+1024))%wx;
            double jj = ((int)(y+iy+1024))%wy;

            unsigned int u = s->m_data.getPixel(ii,jj);
            if (flip==1)
                n->m_data.m_qImage->setPixel(i,s->m_height*m_blockSize-1-j,u);

            if (flip==0) {
                n->m_data.m_qImage->setPixel(s->m_width*m_blockSize-1-i,j,u);
            }
            if (flip==3) {
                n->m_data.m_qImage->setPixel(i,j,u);



            }

        }

    }
    n->m_data.setPixel(0,0,255);
    m_items[m_current] = QSharedPointer<LGenericSprite>(n);

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


