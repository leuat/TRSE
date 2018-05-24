#include "charsetimage.h"
#include "source/LeLib/util/util.h"
#include <QKeyEvent>

CharsetImage::CharsetImage(LColorList::Type t) : MultiColorImage(t)
{
    m_width = 320;
    m_height = 200;
    m_scaleX = 1.0f;
    m_bitMask = 0b11;
    m_noColors = 4;
    m_scale = 2;
    m_width = 160;
    m_minCol = 0;
    Clear();
    m_type = LImage::Type::CharMapMulticolor;
    SetColor(1,1);
    SetColor(2,2);
    SetColor(5,3);


    m_supports.asmExport = false;
    m_supports.binaryLoad = true;
    m_supports.binarySave = true;
    m_supports.flfSave = true;
    m_supports.flfLoad = true;
    m_supports.asmExport = false;

    m_supports.displayColors = true;
    m_supports.displayForeground = false;

    m_currencChar=0;
    m_currentMode=Mode::CHARSET2x2;


    m_exportParams["Start"] = 0;
    m_exportParams["End"] = 256;

}

void CharsetImage::SetColor(uchar col, uchar idx)
{
    m_color.c[idx] = col;

    if (idx==0)
        m_background = col;

    for (int i=0;i<40*25;i++)
        m_data[i].c[idx] = col;

    m_extraCols[idx] = col;
}

void CharsetImage::ImportBin(QFile &file)
{
    m_rawData = file.readAll();
    FromRaw(m_rawData);
/*    SetColor(0, m_background); //MULTICOLOR_CHAR_COL +0
    SetColor(1, 2);// MULTICOLOR_CHAR_COL +2
    SetColor(2, 1); // MULTICOLOR_CHAR_COL +1*/
    SetColor(5, 3); // Color that is changeable*/
}

void CharsetImage::ExportBin(QFile &f)
{
    ToRaw(m_rawData);
    f.write(m_rawData);

}


void CharsetImage::SaveBin(QFile& file)
{
    file.write( ( char * )( &m_background ),  1 );
    file.write( ( char * )( &m_border ), 1 );
    file.write( ( char * )( &m_extraCols[1] ), 1 );
    file.write( ( char * )( &m_extraCols[2] ), 1 );
    file.write( ( char * )( &m_extraCols[3] ), 1 );
    file.write( ( char * )( &m_data ),  25*40*12 );

    qDebug() << "SAVBING";

}

void CharsetImage::LoadBin(QFile& file)
{
    file.read( ( char * )( &m_background ),1 );
    file.read( ( char * )( &m_border ), 1);
    file.read( ( char * )( &m_extraCols[1] ), 1 );
    file.read( ( char * )( &m_extraCols[2] ), 1 );
    file.read( ( char * )( &m_extraCols[3] ), 1 );
    file.read( ( char * )( &m_data ),  25*40*12 );

}

unsigned int CharsetImage::getPixel(int x, int y)
{
    if (m_currentMode==FULL_IMAGE)
        return MultiColorImage::getPixel(x,y);


    int bp = 8;
    if (m_currentMode==CHARSET2x2)
        bp=16;

    if (m_currentMode == CHARSET1x1 || m_currentMode == CHARSET2x2) {
        int i = x/320.0*bp;
        int j = y/200.0*bp;
        int shiftx = (m_currencChar*8/m_scale)%320;
        int shifty = (m_currencChar/(int)40)*8;
        return MultiColorImage::getPixel(i+shiftx,j+shifty);
    }
    if (m_currentMode == CHARSET2x2_REPEAT) {
        int i = x/320.0*16*3;
        int j = y/200.0*16*3;
        int shiftx = (m_currencChar*8/m_scale)%320;
        int shifty = (m_currencChar/(int)40)*8;

        int xx = i%8+shiftx;
        int yy = j%16+shifty;

        return MultiColorImage::getPixel(xx,yy);

    }
    return 1;
}


void CharsetImage::FlipVertical()
{
    uint tmp[24*24];
    if (m_currentMode==CHARSET2x2) {
        float i = 160/16.0;
        float j = 200.0/16.0;

        for (int x=0;x<16;x++)
            for (int y=0;y<16;y++) {
                tmp[16*y + x]=getPixel(x*i,y*j+1);
            }
        for (int y=0;y<16;y++)
            for (int x=0;x<16;x++)
            setPixel( x*i ,y*j+1, tmp[16*y + 15-x]);

    }
}

void CharsetImage::FlipHorizontal()
{
    uint tmp[24*24];
    if (m_currentMode==CHARSET2x2) {
        float i = 160/16.0;
        float j = 200.0/16.0;

        for (int x=0;x<16;x++)
            for (int y=0;y<16;y++) {
                tmp[16*y + x]=getPixel(x*i,y*j+1);
            }
        for (int y=0;y<16;y++)
            for (int x=0;x<16;x++)
                setPixel( x*i ,y*j+1, tmp[16*(15-y) +x]);

    }
}


void CharsetImage::FromRaw(QByteArray &arr)
{
    Clear();
    for (int i=0;i<256;i++) {
        PixelChar& pc = m_data[i];
        int idx=i*8;
        for (int j=0;j<8;j++) {
            uchar v = PixelChar::reverse(arr[i*8+j]);
            pc.p[j] = v;
        }
    }
}

void CharsetImage::ToRaw(QByteArray &arr)
{
    int start = m_exportParams["Start"];
    int end = m_exportParams["End"];
    int size = start-end;
    qDebug() << start << " " << end;

    arr.resize(size*8);
    for (int i=start;i<end;i++) {
        PixelChar& pc = m_data[i];
        for (int j=0;j<8;j++)
            arr[(i-start)*8+j] = PixelChar::reverse(pc.p[j]);
    }
}

void CharsetImage::ToQPixMaps(QVector<QPixmap> &map)
{
    map.clear();
    for (int i=0;i<m_charCount;i++) {
        QImage img = m_data[i].toQImage(64, m_bitMask, m_colorList, m_scale);
        QPixmap p = QPixmap::fromImage(img);
        map.append(p);
    }
    /*
    for (int i=0;i<63;i++) {

        int j=i*4;
        QImage img1 = m_data[i].toQImage(j, m_bitMask, m_colorList);
        QImage img2 = m_data[i].toQImage(j+1, m_bitMask, m_colorList);
        QImage img3 = m_data[i].toQImage(j+40, m_bitMask, m_colorList);
        QImage img4 = m_data[i].toQImage(j+41, m_bitMask, m_colorList);

        QImage img = QImage(img1.size()*2, QImage::Format_RGB32);
        int w= img1.size().width();
        int h= img1.size().height();

        for (int i=0;i<w;i++)
            for (int j=0;j<h;j++) {
                img.setPixel(i,j,img1.pixel(i,j));
                img.setPixel(i+w,j,img2.pixel(i,j));
                img.setPixel(i,j+h,img3.pixel(i,j));
                img.setPixel(i+w,j+h,img4.pixel(i,j));
            }


        QPixmap p = QPixmap::fromImage(img);
        map.append(p);
    }
*/
}

void CharsetImage::setPixel(int x, int y, unsigned int color)
{
    if (m_currentMode==FULL_IMAGE) {
        if (x>=m_width || x<0 || y>=m_height || y<0)
            return;
        PixelChar& pc = getPixelChar(x,y);


        int ix = x % (8/m_scale);//- (dx*40);
        int iy = y % 8;//- (dy*25);

        pc.set(m_scale*ix, iy, color, m_bitMask);
        return;
    }

    int bp = 8;
    if (m_currentMode==CHARSET2x2)
        bp=16;

    int xx,yy;
    int shiftx = (m_currencChar*8/m_scale)%320;
    int shifty = (m_currencChar/(int)40)*8;
    if (m_currentMode == CHARSET1x1 || m_currentMode == CHARSET2x2) {
        int i = x/320.0*bp;
        int j = y/200.0*bp;

        if (j<0 || j>=16 || i<0 || i>=16/m_scale)
            return;
        xx = i+shiftx;
        yy = j+shifty;



    }
    if (m_currentMode == CHARSET2x2_REPEAT) {
        int i = x/320.0*16*3;
        int j = y/200.0*16*3;

        if (j<0 || j>=16*3 || i<0 || i>=8*3)
            return;

        xx = i%8+shiftx;
        yy = j%16+shifty;


    }

    setLimitedPixel(xx,yy,color);
//    MultiColorImage::setPixel(xx,yy, color);



}

void CharsetImage::RenderEffect(QMap<QString, float> params)
{


}

void CharsetImage::CopyFrom(LImage *img)
{
   // return LImage::CopyFrom(img);

    CharsetImage* mc = dynamic_cast<CharsetImage*>(img);
    //if ((typeid(*img) == typeid(MultiColorImage)) || (typeid(*img) == typeid(StandardColorImage))
    //        || (typeid(*img) == typeid(CharsetImage)))
    if (mc!=nullptr)
    {
        m_background = mc->m_background;
        m_border = mc->m_border;
        m_currentMode = mc->m_currentMode;
        m_currencChar = mc->m_currencChar;

        m_width = mc->m_width;
        m_height = mc->m_height;
        m_scaleX = mc->m_scaleX;
        m_bitMask = mc->m_bitMask;
        m_noColors = mc->m_noColors;
        m_scale = mc->m_scale;
        m_minCol = mc->m_minCol;

        for (int i=0;i<4;i++)
            m_extraCols[i]  = mc->m_extraCols[i];
        // qDebug() << "COPY FROM";
#pragma omp parallel for
        for(int i=0;i<25*40;i++) {
            for (int j=0;j<8;j++)
                m_data[i].p[j] = mc->m_data[i].p[j];
            for (int j=0;j<4;j++)
                m_data[i].c[j] = mc->m_data[i].c[j];
        }
    }
    else
    {
        MultiColorImage::CopyFrom(img);
        return;
    }


}

bool CharsetImage::KeyPress(QKeyEvent *e)
{
    QPoint dir(0,0);

    int s = 1;

    if (m_currentMode==CHARSET2x2 || m_currentMode==CHARSET2x2_REPEAT)
        s=2;

    if (e->key()==Qt::Key_W)
        m_currencChar-=40*s;
    if (e->key()==Qt::Key_A)
        m_currencChar-=1*s;
    if (e->key()==Qt::Key_S)
        m_currencChar+=40*s;
    if (e->key()==Qt::Key_D)
        m_currencChar+=1*s;

    m_currencChar = Util::clamp(m_currencChar,0,255);


    return true;
}

void CharsetImage::setLimitedPixel(int x, int y, unsigned int color)
{

    if (x>=m_width || x<0 || y>=m_height || y<0)
        return;
    PixelChar& pc = getPixelChar(x,y);

 //   pc.Reorganize(m_bitMask, m_scale,m_minCol, m_noColors);

    int ix = x % (8/m_scale);//- (dx*40);
    int iy = y % 8;//- (dy*25);

    //if (ix==0 || ix == 2 || ix == 4 || ix == 6)
    pc.set(m_scale*ix, iy, color, m_bitMask);

//    qDebug() << color;

}

void CharsetImage::CopyChar()
{
    if (m_currentMode == CHARSET1x1)
        m_copy[0] = m_data[m_currencChar];

    if (m_currentMode == CHARSET2x2 || m_currentMode == CHARSET2x2_REPEAT) {
        m_copy[0] = m_data[m_currencChar];
        m_copy[1] = m_data[m_currencChar+1];
        m_copy[2] = m_data[m_currencChar+40];
        m_copy[3] = m_data[m_currencChar+41];

    }

}

void CharsetImage::PasteChar()
{
    if (m_currentMode == CHARSET1x1)
        m_data[m_currencChar]=m_copy[0];

    if (m_currentMode == CHARSET2x2 || m_currentMode == CHARSET2x2_REPEAT) {
        m_data[m_currencChar] = m_copy[0];
        m_data[m_currencChar+1] = m_copy[1];
        m_data[m_currencChar+40] = m_copy[2];
        m_data[m_currencChar+41] = m_copy[3];

    }

}
