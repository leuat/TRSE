#include "limageamstradcpc.h"

QVector<int> LImageAmstradCPC::table160 =  {0b00000000, 0b00000001,0b00000100,0b00000101,
                          0b00010000, 0b00010001,0b00010100,0b00010101,
                          0b01000000, 0b01000001,0b01000100,0b01000101,
                          0b01010000, 0b01010001,0b01010100,0b01010101};

LImageAmstradCPC::LImageAmstradCPC(LColorList::Type t)  : LImageQImage(t)
{
    m_type = LImage::Type::AmstradCPC;
    m_supports.asmExport = false;
    m_supports.binaryLoad = false   ;
    m_supports.binarySave = false;
    m_supports.flfSave = true;
    m_supports.flfLoad = true;
    m_colorList.m_supportsFooterPen = true;

/*    m_metaParams.append(new MetaParameter("mode","Mode",0,3,3));
    m_metaParams.append(new MetaParameter("screen_width","Screen width",160,2,1000));
    m_metaParams.append(new MetaParameter("screen_height","Screen height",200,2,1000));*/
    m_mode = 0;
    SetMode();

}

uchar LImageAmstradCPC::AmstradCrazySwap(uchar c) {
//    return ((c&0b11000000) | ((c&0b00000011)<<2) | ((c&0b00001100)<<2) | ((c&0b00110000)>>4));
    return (((c&0b00000011)<<6) | ((c&0b00001100)<<0) | ((c&0b00110000)<<0) | ((c&0b11000000)>>6));
}

void LImageAmstradCPC::SetMode()
{
    m_qImage = nullptr;
    if (m_mode == 0) {
        Initialize(160,200);
        m_colors = 16;
        m_bpp = 4;
        m_scaleX = 2;
    }
    if (m_mode == 1) {
        Initialize(320,200);
        m_colors = 4;
        m_scaleX = 1;
        m_bpp = 2;
    }
    if (m_mode == 2) {
        Initialize(640,200);
        m_colors = 1;
        m_bpp = 1;
        m_scaleX = 0.5;
    }

    InitPens();
}

void LImageAmstradCPC::InitPens()
{
    //m_colorList.InitAmstradCPC();
//    qDebug() << "HERE INITPENS";
    //  m_colorList.InitPalettePens(m_colors);
}


void LImageAmstradCPC::setPixel(int x, int y, unsigned int color)
{
    if (m_qImage==nullptr)
        return;

//    if (rand()%100>98)
  //      qDebug() << m_width;
//    int val = m_colorList.getPen(color);


//    if (rand()%100>98 && color!=0)
  //  qDebug() << "SET PIXEL "<< QString::number(m_colorList.getPenIndex(color)) << QString::number(color) << val;

    if (x>=0 && x<m_qImage->width() && y>=0 && y<m_qImage->height())
        m_qImage->setPixel(x,y,QRgb(m_colorList.getPenIndex(color)));
}
/*
unsigned int LImageAmstradCPC::getPixel(int x, int y)
{
    if (m_qImage==nullptr)
        return 0;
    if (x>=0 && x<m_qImage->width() && y>=0 && y<m_qImage->height()) {
        return m_qImage->pixel(x,y);
    }


    return 0;
}
*/
void LImageAmstradCPC::ToQImage(LColorList &lst, QImage &img, double zoom, QPointF center)
{
//#pragma omp parallel for
    for (int i=0;i<m_width;i++)
        for (int j=0;j<m_height;j++) {

            float xp = ((i-center.x())*zoom)+ center.x();
            float yp = ((j-center.y())*zoom) + center.y();

            unsigned int pen = getPixel(xp,yp);// % 16;
            unsigned int col = m_colorList.getPen(pen);

            //            img->setPixel(i,j,QRgb(col));
            img.setPixel(i,j,lst.get(col).color.rgb());
        }
    //return img;
}
/*
void LImageAmstradCPC::OrdererdDither(QImage &img, LColorList &colors, QVector3D strength, QPoint size, float gamma)
{
    int height  =min(img.height(), m_height);
    int width  =min(img.width(), m_width);
    QMatrix4x4 bayer4x4 = QMatrix4x4(0,8,2,10,  12,4,14,6, 3,11,1,9, 15,7,13,5);
    bayer4x4 = bayer4x4*1/16.0*strength.x();

//    qDebug() << "COLORS " << colors.getPen(7);
    for (int y=0;y<height;y++) {
        for (int x=0;x<width;x++) {

            //            color.R = color.R + bayer8x8[x % 8, y % 8] * GAP / 65;

            int xx = (x-img.width()/2)*m_importScaleX + img.width()/2;
            int yy = (y-img.height()/2)*m_importScaleY + img.height()/2;


            QColor color = QColor(img.pixel(xx,yy));
            int yp = y + x%(int)strength.y();
            int xp = x + y%(int)strength.z();
            color.setRed(min((float)pow(color.red(),gamma) + bayer4x4(xp % size.x(),yp % size.y()),255.0f));
            color.setGreen(min((float)pow(color.green(),gamma) + bayer4x4(xp % size.x(),yp % size.y()),255.0f));
            color.setBlue(min((float)pow(color.blue(),gamma) + bayer4x4(xp % size.x(),yp % size.y()),255.0f));

            int winner = 0;

            QColor newPixel = colors.getClosestColor(color, winner);
            //int c = m_colorList.getIndex(newPixel);
            setPixel(x,y,colors.getPen(winner));

        }
    }


}
*/
void LImageAmstradCPC::ExportBin(QFile &ofile)
{

    QString f = ofile.fileName();

    QString filenameBase = Util::getFileWithoutEnding(f);

    QString fColor = filenameBase + "_palette.bin";

    if (QFile::exists(fColor))
        QFile::remove(fColor);

    QByteArray palette,data;

    QVector<int> lst = m_colorList.getPenList();
    for (auto i : lst)
        palette.append(((unsigned char)i));
//    qDebug() << lst;

    Util::SaveByteArray(palette,fColor);

    int y = 0;
    int dy = 0;
    int xw;
/*    if (m_width==320)  xw=80;
    if (m_width==160)  xw=80;
    if (m_width==256)  xw=64;
*/
    for (int i=0;i<m_height;i++) {
        char c = 0;
        int curBit = 0;
        for (int x=0;x<m_width;x++) {
            //int pixel = ((dy+y)/10)&15;
            int pixel = getPixel(x,y+dy);
//            qDebug() <<curBit << pixel << x;
            //pixel = 4;
            c|=table160[pixel]<<(1-curBit);


            curBit+=1;

            if (curBit>=2) {
                curBit=0;
                data.append((AmstradCrazySwap(c)));
//                data.append(c);
                c=0;
            }


        }
        y=y+8;

        if (y>=m_height) {
            y=0;
            dy++;
            for (int i=0;i<0x30;i++) // Extra data to align to $0, $0800, $1000 etc
                data.append((char)0);
        }

    }




    ofile.write(data);
}

void LImageAmstradCPC::LoadBin(QFile &file)
{
    m_qImage = new QImage(m_width, m_height, QImage::Format_ARGB32);
    unsigned char *temp_data = new unsigned char[m_width*m_height];
    file.read((char*)temp_data, m_width*m_height);
    // Perform actual copy of data)
    for (int i=0;i<m_width;i++)
        for (int j=0;j<m_height;j++) {
            setPixel(i,j, temp_data[i+j*m_width]);
        }
    delete[] temp_data;

}

