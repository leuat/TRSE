#include "limagetvc.h"

QVector<int> LImageTVC::table160 =  {0b00000000, 0b00000001,0b00000100,0b00000101,
                          0b00010000, 0b00010001,0b00010100,0b00010101,
                          0b01000000, 0b01000001,0b01000100,0b01000101,
                          0b01010000, 0b01010001,0b01010100,0b01010101};

LImageTVC::LImageTVC(LColorList::Type t)  : LImageQImage(t)
{
    m_type = LImage::Type::TVC;
    m_supports.asmExport = false;
    m_supports.binaryLoad = false   ;
    m_supports.binarySave = false;
    m_supports.flfSave = true;
    m_supports.flfLoad = true;
    m_colorList.m_supportsFooterPen = true;
    usePens = true;

    m_metaParams.append(new MetaParameter("colors","Colors",16,0,16));
    m_metaParams.append(new MetaParameter("screen_width","Screen width",128,2,1000));
    m_metaParams.append(new MetaParameter("screen_height","Screen height",240,2,1000));
    m_colors = 16;
    m_width = 128;
    m_height=240;
    SetMode();
    m_colorList.InitPalettePens(m_colors);
    m_supports.displayCharOperations = true;

}


void LImageTVC::SetMode()
{
    m_qImage = nullptr;
    Initialize(m_width,m_height);
    InitPens();
}

/*void LImageTVC::InitPens()
{
    //m_colorList.InitTVC();
//    qDebug() << "HERE INITPENS";
    //  m_colorList.InitPalettePens(m_colors);
}
*/
/*
void LImageTVC::setPixel(int x, int y, unsigned int color)
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
*/
/*
unsigned int LImageTVC::getPixel(int x, int y)
{
    if (m_qImage==nullptr)
        return 0;
    if (x>=0 && x<m_qImage->width() && y>=0 && y<m_qImage->height()) {
        return m_qImage->pixel(x,y);
    }


    return 0;
}
*/
void LImageTVC::ToQImage(LColorList &lst, QImage &img, double zoom, QPointF center)
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

void LImageTVC::InitPens()
{
    m_colorList.InitPalettePens(m_colors);

}
/*
void LImageTVC::OrdererdDither(QImage &img, LColorList &colors, QVector3D strength, QPoint size, float gamma)
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
void LImageTVC::ExportBin(QFile &ofile)
{

    QString f = ofile.fileName();
/*

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
*/
    int y = 0;
    int dy = 0;
    int xw;
/*    if (m_width==320)  xw=80;
    if (m_width==160)  xw=80;
    if (m_width==256)  xw=64;
*/
    QByteArray data;
    if (m_colors==2)
    for (int y=0;y<m_height;y++) {
        for (int x=0;x<m_width/8;x++) {
            char c = 0;
            int curBit = 0;
            for (int p=0;p<8;p++) {
                uchar p1 = getPixel(x*8+p,y)&1;
                c |= (p1&0b01)<<(7-curBit);

                curBit+=1;
            }
            data.append(c);

        }
    }
    if (m_colors==4)
    for (int y=0;y<m_height;y++) {
        for (int x=0;x<m_width/4;x++) {
            char c = 0;
            int curBit = 0;
            for (int p=0;p<4;p++) {
                uchar p1 = getPixel(x*4+p,y)&3;
                c |= (p1&0b01)<<(7-curBit);
                c |= ((p1&0b10)>>1)<<((3-curBit));

                curBit+=1;
            }
            data.append(c);

        }
    }
    if (m_colors==16) {
    for (int y=0;y<m_height;y++) {
        for (int x=0;x<m_width/2;x++) {
            char c = 0;
            int curBit = 0;
            for (int i=0;i<2;i++) {
                auto col = m_colorList.getPenColour(getPixel(x*2+1-i,y));
                if (col.red()!=0) c|=(0b00000100<<i);
                if (col.green()!=0) c|=(0b00010000<<i);
                if (col.blue()!=0) c|=(0b00000001<<i);
                if (col.blue()>0.8 || col.red()>0.8 || col.green()>0.8) c|=(0b01000000<<i);
            }
            // Transfer

            data.append(c);

        }
    }
    }




    ofile.write(data);
}

void LImageTVC::LoadBin(QFile &file)
{
    file.read((char*)&m_width,sizeof(int));
    file.read((char*)&m_height,sizeof(int));
    file.read((char*)&m_colors,sizeof(int));


    Initialize(m_width,m_height);
    InitPens();
    LImageQImage::LoadBin(file);

}

void LImageTVC::SaveBin(QFile &file)
{
    file.write((char*)&m_width,sizeof(int));
    file.write((char*)&m_height,sizeof(int));
    file.write((char*)&m_colors,sizeof(int));

    LImageQImage::SaveBin(file);
}

