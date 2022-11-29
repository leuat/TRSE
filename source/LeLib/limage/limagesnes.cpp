#include "limagesnes.h"

LImageSNES::LImageSNES(LColorList::Type t) : LImageAmiga(t,0)
{
    m_width = 128;
    m_height = 128;
    m_qImage = nullptr;
    Initialize(m_width,m_height);
    m_bitMask = 0b1;

    m_scaleX = 1.0f;

    m_colorList.m_bpp = QVector3D(2,2,2);
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
    m_supports.displayCharOperations = true;
//    usePens = true;

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

    m_GUIParams[tabCharset] = "1";

    m_updateCharsetPosition = true;
    m_colorList.m_isCharset = true;

    m_charWidth=16;
    m_charHeight=16;

    m_metaParams.append(new MetaParameter("bitplanes","Bitplane data: 2 (4 colors), 3 (8 colors) or 4 (16 colors). For sprites, select 4 bitplanes (16 colors)",2,2,4));


    InitPens();

    //ForceColor();
}

void LImageSNES::InitPens()
{
    m_colorList.InitSNESPens();
}

void LImageSNES::Initialize(int width, int height)
{
    if (m_width==width && m_height==height && m_qImage!=nullptr)
        return;
    if (m_qImage != nullptr)
        delete m_qImage;

    //    qDebug() << "NEWING "<<m_width << this;
    m_width = width;
    m_height = height;

    m_banks.clear();
    for (int i=0;i<8;i++) {
        auto img = QSharedPointer<QImage>(new QImage(width, height, QImage::Format_ARGB32));
        img->fill(QColor(0,0,0));
        m_banks.append(img);
    }
    SetBank(0);

}

void LImageSNES::SaveBin(QFile &file)
{
    int pal = m_footer.get(LImageFooter::POS_CURRENT_PALETTE);
    m_updatePaletteInternal = true;
    SetPalette(0);
    SetPalette(1);
    SetPalette(0);
    auto qi = m_qImage;
    for (int i=0;i<m_banks.count();i++) {
        SetBank(i);
        LImageQImage::SaveBin(file);
    }
    m_qImage = qi;
    QByteArray data;
    m_colorList.toArray(data);

    file.write(data);
    uchar bpp = m_colorList.m_bpp.x();
    file.write((char*)&bpp,1);
//    qDebug() << Util::numToHex(m_colorList.m_nesPPU[4*2]);
//    for (int i=0;i<16;i++)
  //      qDebug() << Util::numToHex(m_colorList.m_nesPPU[i]);

    SetPalette(pal);

    file.write(m_colorList.m_nesPPU);
    //qDebug() << "C";
    //qDebug() << "D";
}

void LImageSNES::LoadBin(QFile &file)
{
    m_colorList.InitSNES();
    QVector<unsigned char*> d;
    for (int i=0;i<m_banks.count();i++) {
        unsigned char *data = new unsigned char[m_width*m_height];
        file.read((char*)data, m_width*m_height);
        d.append(data);
    }
    uchar size;

    file.read((char*)&size,1);
//    qDebug() << "SIZE READ : "<< QString::number(size);

    if (!file.atEnd()) {
        int cnt = size;
        if (cnt==0) cnt=256;
        QByteArray d = file.read(cnt*3);
//        qDebug() <<QString::number(d[3])<<QString::number(d[4])<<QString::number(d[5]);
//        d.insert(0,(char)0);
//        m_colorList.fromArray(d);
    }
    uchar bpp;
    file.read((char*)&bpp,1);
//    qDebug() << bpp;
    m_colorList.setNoBitplanes(bpp);
    for (int k=0;k<m_banks.count();k++) {
        SetBank(k);
        for (int i=0;i<m_width;i++)
            for (int j=0;j<m_height;j++) {
                setPixel(i,j, d[k][i+j*m_width]);
            }
        delete[] d[k];

    }


    SetBank(0);
    m_colorList.m_nesPPU = file.read(0x100);
//    for (int i=0;i<16;i++)
  //      qDebug() << Util::numToHex(m_colorList.m_nesPPU[i]);
    SetPalette(0);
    m_colorList.UpdateUI();
//    qDebug() << Util::numToHex(m_colorList.m_nesPPU[4*2]);


}

QStringList LImageSNES::getBankNames() {
    QStringList lst;
    for (int i=0;i<m_banks.count(); i++)
        lst<<"Bank "+QString::number(i);
    if (lst.count()==0)
        lst<<"Bank 0";
    return lst;
}

void LImageSNES::SetBank(int bnk) {
    m_footer.set(LImageFooter::POS_CURRENT_BANK,bnk);
    if (bnk<m_banks.count() && bnk>=0)
        m_qImage = m_banks[bnk].get();
}



void LImageSNES::ToQImage(LColorList &lst, QImage &img, double zoom, QPointF center)
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

void LImageSNES::setPixel(int x, int y, unsigned int color)
{
    if (m_qImage==nullptr)
        return;

    QPoint p = getPixelPosition(x,y);

    if (p.x()>=0 && x<m_qImage->width() && p.y()>=0 && y<m_qImage->height())
        m_qImage->setPixel(p.x(),p.y(),QRgb(m_colorList.getPenIndex(color)));
}

void LImageSNES::ExportBin(QFile &file)
{
    int nobp = m_colorList.m_bpp.x();//m_colorList.getNoBitplanes();
    //    qDebug() << nobp;
    auto keep = m_footer.get(LImageFooter::POS_DISPLAY_CHAR);
    m_footer.set(LImageFooter::POS_DISPLAY_CHAR,0);
    QByteArray data;
    int noBanksToExport = fmin(m_exportParams["End"],m_banks.count());
    bool isSprite = m_exportParams["Start"]==1.0;
    auto qi = m_qImage;
    //
    int planes[4] = {1,0,2,3};
    if (isSprite) {
        planes[0]=0;
        planes[1]=1;
        planes[2]=3;
        planes[3]=2;
    }

    for (int bank=0;bank<noBanksToExport;bank++)
    {
        SetBank(bank);
        int curBit = 0;
        data.resize(m_height*m_width/8*nobp);
        data.fill(0);
        int idx = 0;
        // 0123
        // 2301
        // 0132
        // 0231
        // 0213
        // 0321
//        int planes[4] = {1,0,2,3};
//        int planes[4] = {1,0,2,3};

        for (int y=0;y<m_height;y+=8) {
            for (int x=0;x<m_width;x+=8) {
                for (int split = 0;split<nobp/2;split++) {
                    for (int dy=0;dy<8;dy++) {
                        for (int i=0;i<2;i++) {
                            for (int dx=0;dx<8;dx++) {

                                char val = m_qImage->pixel(x+dx,y+dy) ;
                                int tst = (i+(split*2));
                                int bit = CHECK_BIT(val,planes[tst]);
                                if (val!=0)
                                    data[idx] = data[idx] | (bit<<(7-dx));

                                //                          data[idx]=1<<(dy+i+split*2);
                                //
                            }
                            idx++;
                            // qDebug() <<idx<<x<<y<<data.count()<<i<<idx;

                        }
                    }
                }
                //            qDebug() << QString::number(idx);

            }
        }
        file.write(data);

    }
    m_qImage = qi;
    int type = m_exportParams["export1"];
    QByteArray cData;
    QString ff = file.fileName();
    ff = ff.remove(".bin");
    m_colorList.ExportSNESPalette(ff+".pal");

    m_footer.set(LImageFooter::POS_DISPLAY_CHAR,keep);
}

void LImageSNES::ReInitialize()
{
    m_colorList.setNoBitplanes(getMetaParameter("bitplanes")->value);
}

void LImageSNES::SavePalette()
{
    int noCol = pow(2,m_colorList.m_bpp.x());
    int pal = m_footer.get(LImageFooter::POS_CURRENT_PALETTE);
    for (int i=0;i<noCol;i++)  {
        m_colorList.m_nesPPU[pal*noCol +i] = (uchar)m_colorList.getPen(i);
    }

}


void LImageSNES::SetPalette(int pal)
{

    int m_oldPal = m_footer.get(LImageFooter::POS_CURRENT_PALETTE);

    int noCol = pow(2,m_colorList.m_bpp.x());
     //   qDebug() << "OLD " <<noCol<< pal<<m_oldPal;
    if (m_oldPal!=pal && m_updatePaletteInternal && m_firstIgnoreDone) {
        for (int i=0;i<noCol;i++) {
            m_colorList.m_nesPPU[m_oldPal*noCol +i] = (uchar)m_colorList.getPen(i);
              //        qDebug() << " ** " <<Util::numToHex((uchar)m_colorList.getPen(i));
        }
    }
    //     m_colorList.m_nesPPU[m_oldPal*4 +1 +3] = m_colorList.getPen(3);
    //   m_colorList.m_nesPPU[0] = m_colorList.getPen(3);

    for (int i=0;i<noCol;i++) {
        m_colorList.setPen(i,(uchar)m_colorList.m_nesPPU[pal*noCol +i]);
        //   qDebug() << " NEW ** " <<Util::numToHex((uchar)m_colorList.m_nesPPU[pal*noCol +i]);
    }
    m_colorList.m_curPal = pal;
    m_firstIgnoreDone = true;
 //   qDebug() << Util::numToHex(m_colorList.m_nesPPU[4*2]);


}

bool LImageSNES::isNes() {return false;}

bool LImageSNES::isSnes() {return true;}

QStringList LImageSNES::getPaletteNames() {
    QStringList lst;
    int noCol = pow(2,m_colorList.m_bpp.x());
    int noPals = 256/noCol;
    for (int i=0;i<noPals;i++)
        lst<<("Palette "+QString::number(i));
    return lst;

}

QString LImageSNES::GetCurrentDataString() {
    char chr = m_currentChar;
    return "  Character : " + QString(chr) + "  "+ Util::numToHex(m_currentChar) + " (" + QString::number(m_currentChar)+ ")";
}


