#include "limagesnes.h"

LImageSNES::LImageSNES(LColorList::Type t) : LImageAmiga(t,0)
{
    m_width = 128;
    m_height = 128;
    m_qImage = nullptr;
    Initialize(m_width,m_height);
    m_bitMask = 0b1;

    m_scaleX = 1.0f;

    m_colorList.setNoBitplanes(2);

//    m_charWidth = 16;
  //  m_charHeight = 16;


    //m_data = new PixelChar[m_charWidth*m_charHeight];
    m_charWidthDisplay=16;
    m_charHeightDisplay=16;
    m_gridWidthDisplay = 16;

    //    m_double=false;
    Clear();
    m_type = LImage::Type::SNES;
    m_supports.asmExport = true;
    m_supports.binaryLoad = true;
    m_supports.binarySave = true;
    m_supports.nesPalette = true;
    m_supports.koalaImport = false;
    m_supports.koalaExport = false;
    m_supports.flfSave = true;
    m_supports.flfLoad = true;
    m_supports.compressedExport = false;
    m_supports.displayForeground = true;
    m_supports.displayBank = true;
    m_supports.displayCmbColors = true;
    m_supports.displayColors = true;

    m_GUIParams[btnLoadCharset] ="";
    m_GUIParams[btn1x1] = "8x8";
    m_GUIParams[btn2x2] = "16x16";
    m_GUIParams[btn2x2repeat] = "16x16 tiled";
    m_GUIParams[btnCopy] = "Copy";
    m_GUIParams[btnPaste] = "Paste";
    m_GUIParams[btnFlipH] = "Mirror X";
    m_GUIParams[btnFlipV] = "Mirror Y";

    m_GUIParams[tabCharset] = "Charset";
    m_GUIParams[tabData] = "";
    m_GUIParams[tabLevels] = "";
    m_GUIParams[tabEffects] = "Effects";

    m_GUIParams[btnEditFullCharset] = "Char";


    m_GUIParams[col1] = "Background";
    m_GUIParams[col2] = "Color 1";
    m_GUIParams[col3] = "Color 2";
    m_GUIParams[col4] = "Color 3";


    InitPens();

    //ForceColor();
}

void LImageSNES::InitPens()
{
    m_colorList.m_bpp = QVector3D(2,2,2);
    m_colorList.InitSNESPens();
}

void LImageSNES::SaveBin(QFile &file)
{
    int pal = m_footer.get(LImageFooter::POS_CURRENT_PALETTE);
//    qDebug() << "A";
//    if (pal>=2)
  //      SetPalette(pal-1);
  //  qDebug() << "B";

    SetPalette(0);
/*    int noCol = pow(2,m_colorList.m_bpp.x());
        for (int i=0;i<noCol;i++)
            m_colorList.m_nesPPU[pal*noCol +i] = (uchar)m_colorList.getPen(i);
*/

    LImageQImage::SaveBin(file);
    QByteArray data;
    m_colorList.toArray(data);
    file.write(data);
    uchar bpp = m_colorList.m_bpp.x();
    file.write((char*)&bpp,1);
    file.write(m_colorList.m_nesPPU);
    //qDebug() << "C";
    SetPalette(pal);
    //qDebug() << "D";
}

void LImageSNES::LoadBin(QFile &file)
{
    LImageQImage::LoadBin(file);
    uchar size;

    file.read((char*)&size,1);

    if (!file.atEnd()) {
        int cnt = size;
        if (cnt==0) cnt=256;
        QByteArray d = file.read(cnt*3);
//        qDebug() <<QString::number(d[3])<<QString::number(d[4])<<QString::number(d[5]);
//        d.insert(0,(char)0);
        m_colorList.fromArray(d);
    }
    uchar bpp;
    file.read((char*)&bpp,1);
    m_colorList.m_bpp.setX(bpp);
    m_colorList.m_nesPPU = file.read(0x100);

}

void LImageSNES::ToQImage(LColorList &lst, QImage &img, double zoom, QPointF center)
{
#pragma omp parallel for
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

void LImageSNES::setPixel(int x, int y, unsigned int color)
{
    if (m_qImage==nullptr)
        return;

    if (x>=0 && x<m_qImage->width() && y>=0 && y<m_qImage->height())
        m_qImage->setPixel(x,y,QRgb(m_colorList.getPenIndex(color)));
}

void LImageSNES::ExportBin(QFile &file)
{
    int nobp = m_colorList.m_bpp.x();//m_colorList.getNoBitplanes();
    //    qDebug() << nobp;
    QByteArray data;
    data.resize(m_height*m_width/8*nobp);
    data.fill(0);
    int curBit = 0;
    int idx = 0;
    for (int y=0;y<m_height;y+=8) {
        for (int x=0;x<m_width;x+=8) {
            for (int dy=0;dy<8;dy++) {
                for (int i=0;i<nobp;i++) {
                    for (int dx=0;dx<8;dx++) {

                        char val = m_qImage->pixel(x+dx,y+dy);
                        int bit = CHECK_BIT(val,i);
                        if (val!=0)
                            data[idx] |= (bit<<(7-dx));

                    }
                    idx++;
                    // qDebug() <<idx<<x<<y<<data.count()<<i<<idx;

                }
            }
            //            qDebug() << QString::number(idx);

        }
    }
    file.write(data);
    int type = m_exportParams["export1"];
    QByteArray cData;
    QString ff = file.fileName();
    ff = ff.remove(".bin");
    m_colorList.ExportSNESPalette(ff+".pal");

}

void LImageSNES::SetPalette(int pal)
{

    int m_oldPal = m_footer.get(LImageFooter::POS_CURRENT_PALETTE);

    int noCol = pow(2,m_colorList.m_bpp.x());
    //    qDebug() << "OLD " <<noCol<< pal<<m_oldPal;
    if (m_oldPal!=pal && m_updatePaletteInternal && m_firstIgnoreDone) {
        for (int i=0;i<noCol;i++) {
            m_colorList.m_nesPPU[m_oldPal*noCol +i] = (uchar)m_colorList.getPen(i);
            //          qDebug() << " ** " <<Util::numToHex((uchar)m_colorList.getPen(i));
        }
    }
    //     m_colorList.m_nesPPU[m_oldPal*4 +1 +3] = m_colorList.getPen(3);
    //   m_colorList.m_nesPPU[0] = m_colorList.getPen(3);

    for (int i=0;i<noCol;i++) {
        m_colorList.setPen(i,(uchar)m_colorList.m_nesPPU[pal*noCol +i]);
        //   qDebug() << " NEW ** " <<Util::numToHex((uchar)m_colorList.m_nesPPU[pal*noCol +i]);
    }
    m_firstIgnoreDone = true;


}

QStringList LImageSNES::getPaletteNames() {
    QStringList lst;
    int noCol = pow(2,m_colorList.m_bpp.x());
    int noPals = 256/noCol;
    for (int i=0;i<noPals;i++)
        lst<<("Palette "+QString::number(i));
    return lst;

}


