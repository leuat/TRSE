#include "limageok64.h"

LImageOK64::LImageOK64()
{

}

LImageOK64::LImageOK64(LColorList::Type t)  : LImage(t)
{
    Initialize(256,256);
    m_scale = 1;
    m_type = LImage::Type::OK64_256x256;
    m_supports.asmExport = false;
    m_supports.binaryLoad = true;
    m_supports.binarySave = true;
    m_supports.flfSave = true;
    m_supports.flfLoad = true;
    m_supports.asmExport = false;

}

void LImageOK64::Initialize(int width, int height)
{

    m_width = width;
    m_height = height;
    m_data.resize(width*height);
    m_data.fill(0);


}

void LImageOK64::setPixel(int x, int y, unsigned int color)
{
    if (x<0 || x>=m_width) return;
    if (y<0 || y>=m_height) return;
    m_data[x+y*m_width] = (uchar)color;
}

unsigned int LImageOK64::getPixel(int x, int y)
{
    if (x<0 || x>=m_width) return 0;
    if (y<0 || y>=m_height) return 0;
    return (uchar)m_data[x+y*m_width];

}

void LImageOK64::SaveBin(QFile &f)
{
    f.write(m_data);
    f.write(m_colorList.toArray());

}

void LImageOK64::LoadBin(QFile &f)
{
    m_data = f.read(m_width*m_height);
    uchar size;
    f.read((char*)&size,1);
    size = 0;
    if (!f.atEnd()) {
        QByteArray d = f.read(256*3);
        d[0]=0;
        //d.insert(0,256);
        m_colorList.fromArray(d);
    }

}

void LImageOK64::ToQImage(LColorList &lst, QImage &img, double zoom, QPointF center)
{
    for (int i=0;i<m_width;i++)
        for (int j=0;j<m_height;j++) {

            float xp = floor(((i-center.x())*zoom)+ center.x());
            float yp = floor(((j-center.y())*zoom) + center.y());


            unsigned int col = 0;
            if (xp>=0 && xp<m_width && yp>=0 && yp<m_height)
                col = getPixel(xp,yp);
            // Has transparency?
 //           if (rand()%100>97)
   //         qDebug() << col;
            QColor scol = lst.get(col).color;
            QRgb rgbCol = (scol).rgb();
           img.setPixel(i,j,rgbCol);
        }
    //return img;

}

void LImageOK64::fromQImage(QImage *img, LColorList &lst)
{

}

void LImageOK64::Release()
{

}

void LImageOK64::ExportBin(QFile &file)
{
    file.write(m_data);
    QString n = file.fileName().remove(".bin");
    QByteArray b = m_colorList.toArray();
    b.remove(0,1);
    Util::SaveByteArray(b,n+".pal");

}

void LImageOK64::ImportBin(QFile &file)
{
    m_data = file.readAll();
}

/*void LImageAmiga::SaveBin(QFile &file)
{
    LImageQImage::SaveBin(file);
    file.write(m_colorList.toArray());
}

void LImageAmiga::LoadBin(QFile &file)
{
    LImageQImage::LoadBin(file);
    uchar size;

    file.read((char*)&size,1);

    if (!file.atEnd()) {
        QByteArray d = file.read(size*3);
        d.insert(0,size);
        m_colorList.fromArray(d);
    }
}


*/
