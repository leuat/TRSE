#include "limagecoco3.h"


LImageCOCO3::LImageCOCO3(LColorList::Type t)  : LImageQImage(t)
{
    m_type = LImage::Type::COCO3;
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


void LImageCOCO3::SetMode()
{
    m_qImage = nullptr;
    Initialize(m_width,m_height);
    InitPens();
}
void LImageCOCO3::ToQImage(LColorList &lst, QImage &img, double zoom, QPointF center)
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

void LImageCOCO3::InitPens()
{
    m_colorList.InitPalettePens(m_colors);

}
void LImageCOCO3::ExportBin(QFile &ofile)
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
    if (m_colors==4)
    for (int y=0;y<m_height;y++) {
        for (int x=0;x<m_width/4;x++) {
            char c = 0;
            int curBit = 0;
            for (int p=0;p<4;p++) {
                uchar p1 = getPixel(x*4+p,y)&3;
                c |= p1<<(6-2*p);

//                curBit+=1;
            }
            data.append(c);

        }
    }




    ofile.write(data);
}

void LImageCOCO3::LoadBin(QFile &file)
{
    file.read((char*)&m_width,sizeof(int));
    file.read((char*)&m_height,sizeof(int));
    file.read((char*)&m_colors,sizeof(int));


    Initialize(m_width,m_height);
    InitPens();
    LImageQImage::LoadBin(file);

}

void LImageCOCO3::SaveBin(QFile &file)
{
    file.write((char*)&m_width,sizeof(int));
    file.write((char*)&m_height,sizeof(int));
    file.write((char*)&m_colors,sizeof(int));

    LImageQImage::SaveBin(file);
}

