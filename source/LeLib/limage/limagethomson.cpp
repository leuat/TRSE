#include "limagethomson.h"


LImageThomson::LImageThomson(LColorList::Type t)  : LImageQImage(t)
{
    m_type = LImage::Type::THOMSON;
    m_supports.asmExport = false;
    m_supports.binaryLoad = false   ;
    m_supports.binarySave = false;
    m_supports.flfSave = true;
    m_supports.flfLoad = true;
    m_colorList.m_supportsFooterPen = true;
    usePens = true;

    m_metaParams.append(new MetaParameter("screen_width","Screen width",320,2,1000));
    m_metaParams.append(new MetaParameter("screen_height","Screen height",200,2,1000));
    m_width = 320;
    m_height=200;
    SetMode();
    m_supports.displayCharOperations = true;
    m_GUIParams[tabCharset] = "1";
    m_updateCharsetPosition = true;
    m_colorList.m_isCharset = true;

    m_charWidth=40;
    m_charHeight=25;
    m_updateCharsetPosition = true;

    m_supports.displayCharOperations = true;

}


void LImageThomson::SetMode()
{
    m_qImage = nullptr;
    Initialize(m_width,m_height);
    InitPens();
}
void LImageThomson::ToQImage(LColorList &lst, QImage &img, double zoom, QPointF center)
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

void LImageThomson::ExportBin(QFile &ofile)
{

    QString f = ofile.fileName();
    QByteArray data;
    for (int y=0;y<m_height;y++) {
        for (int x=0;x<m_width/8;x++) {
            uchar c = 0;
            for (int p=0;p<8;p++) {
                uchar p1 = getPixel(x*8+p,y);
                if (p1!=0) p1=0; else p1=1;
                c |= (p1)<<(7-p);

//                curBit+=1;
            }
            data.append(c);
            if (c!=0)
                qDebug() << Util::numToHex(c);
        }
    }




    ofile.write(data);
}

void LImageThomson::LoadBin(QFile &file)
{
    file.read((char*)&m_width,sizeof(int));
    file.read((char*)&m_height,sizeof(int));


    Initialize(m_width,m_height);
    InitPens();
    LImageQImage::LoadBin(file);

}

void LImageThomson::SaveBin(QFile &file)
{
    file.write((char*)&m_width,sizeof(int));
    file.write((char*)&m_height,sizeof(int));

    LImageQImage::SaveBin(file);
}

