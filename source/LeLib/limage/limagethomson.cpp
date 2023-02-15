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
    m_data.resize(m_width*m_height/8);
    m_cols.resize(m_width*m_height/4);
    m_data.fill(0);
    m_cols.fill(0);
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

void LImageThomson::setPixel(int x, int y, unsigned int color)
{
    QPoint p = getPixelPosition(x,y);

    int ip = (p.y()*m_width+p.x())/8;
    int cp = (p.y()*m_width+p.x())/8;
    if (ip>m_data.size() || ip<0)
        return;

    uchar c1 = m_cols[cp*2];
    uchar c2 = m_cols[cp*2+1];
    uchar xp = 1<<(7-p.x()&7);
    uchar c = (m_data[ip]);

    if (color!=c1 && color!=c2) {
        //if (!(color==c2 || color==c1))
        {
            uchar cnt = Util::CountBits(c);
//            if (rand()%1000>998)
  //              qDebug() << QString::number(cnt);
            if (cnt>7-cnt)
                 c1 = color;
            else c2 = color;
        }
    }


    if (color == c1) {
        // already cleared
        c = c& ~xp;
    }
    else
    if (color == c2) {
        c |= xp;
    }

    m_cols[cp*2] = c1;
    m_cols[cp*2+1] = c2;
    m_data[ip] = c;

}

unsigned int LImageThomson::getPixel(int x, int y)
{
    QPoint p = getPixelPosition(x,y);

    int ip = (p.y()*m_width+p.x())/8;
    if (ip>m_data.size() || ip<0)
        return 0;

    int cp = (p.y()*m_width+p.x())/8;
    uchar xp = 1<<(7-p.x()&7);
    int add = (m_data[ip] & xp)>>(7-p.x()&7);
    return m_cols[cp*2 +add];

}

void LImageThomson::ExportBin(QFile &ofile)
{


    QString f = ofile.fileName();
    QByteArray data, cols;
/*    for (int y=0;y<m_height;y++) {
        for (int x=0;x<m_width/8;x++) {
            uchar c = 0;
            for (int p=0;p<8;p++) {
                uchar p1 = getPixel(x*8+p,y);
                if (p1!=0) p1=0; else p1=1;
                c |= (p1)<<(7-p);

//                curBit+=1;
            }
            data.append(c);
            uchar c1 = getPixel(x*8+0,y);
            uchar c2 = getPixel(x*8+4,y);
            uchar col = lcols[c1] | (lcols[c2]<<4);
            cols.append(col);

        }
    }
    */
    data.append(m_data);
    for (int i=0;i<m_cols.size()/2;i++) {
        uchar c = lcols[m_cols[i*2]] | (lcols[m_cols[i*2+1]]<<4);
        cols.append(c);
    }




    ofile.write(data);

    QString fData = Util::getFileWithoutEnding(f) + "_color.bin";

    QFile file(fData);
    file.open(QIODevice::WriteOnly);
    file.write( cols );
    file.close();


}

void LImageThomson::LoadBin(QFile &file)
{
    file.read((char*)&m_width,sizeof(int));
    file.read((char*)&m_height,sizeof(int));


    Initialize(m_width,m_height);
    InitPens();

    m_data = file.read(m_width*m_height/8); m_cols = file.read(m_width*m_height/4);

//    LImageQImage::LoadBin(file);

}

void LImageThomson::SaveBin(QFile &file)
{
    file.write((char*)&m_width,sizeof(int));
    file.write((char*)&m_height,sizeof(int));

    file.write(m_data);
    file.write(m_cols);

//    LImageQImage::SaveBin(file);
}

void LImageThomson::CopyFrom(LImage *img) {

    if (m_width!=img->m_width || m_height!=img->m_height) {
        return;
    }

    m_colorList.CopyFrom(&img->m_colorList);
    m_footer = img->m_footer;
    m_scaleX = img->m_scaleX;
    m_currentChar = img->m_currentChar;
    m_charWidthDisplay = img->m_charWidthDisplay;
    m_charWidth = img->m_charWidth;
    LImageThomson* mc = dynamic_cast<LImageThomson*>(img);
    if (mc!=nullptr) {
        m_data = mc->m_data;
        m_cols = mc->m_cols;
    }
    //  m_banks = mc->m_banks;
    //    m_qImage = mc->m_qImage;

}

