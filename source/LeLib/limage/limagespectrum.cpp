#include "limagespectrum.h"

LImageSpectrum::LImageSpectrum(LColorList::Type t) : LImageQImage(t)
{
    m_width = 256;
    m_height = 192;
    Initialize(m_width,m_height);
    m_scaleX = 1.0f;
    m_noColors = 8;
    m_scale = 1;
    m_type = LImage::Type::Spectrum;
    m_colorList.m_supportsFooterPen = true;

    m_supports.koalaExport = false;
    m_supports.koalaImport = false;
    m_supports.asmExport = false;
    m_supports.binaryLoad = true;
    m_supports.binarySave = true;
    m_supports.flfSave = true;
    m_supports.flfLoad = true;
    m_supports.asmExport = false;
    m_colorList.m_selectClosestFromPen = false;
/*    m_GUIParams[col2] = "";
    m_GUIParams[col3] = "Multicolor 1";
    m_GUIParams[col4] = "Multicolor 2";
  */
    m_currentChar=0;
    //m_currentMode=Mode::FULL_IMAGE;
    m_exportParams.clear();
/*    m_exportParams["Start"] = 0;
    m_exportParams["End"] = 256;
    m_exportParams["IncludeColors"] = 0;
    m_exportParams["VIC20mode"] = 0;
  */
    m_supports.displayCharOperations = true;

    m_colorList.InitSPECTRUM();
    //    InitPens();

}

void LImageSpectrum::setPixel(int x, int y, unsigned int color)
{
    if (m_qImage==nullptr)
        return;

    if (x>=0 && x<m_qImage->width() && y>=0 && y<m_qImage->height()) {
        m_qImage->setPixel(x,y,QRgb(color));
        if (!m_ignoreConstraints)
            MaintainSpectrumAttribute(x,y);
    }


}

void LImageSpectrum::InitPens()
{
    m_colorList.setPen(1,Data::data.currentColor);
    m_colorList.InitSPECTRUM();
}

void LImageSpectrum::MaintainSpectrumAttribute(int x, int y)
{
    int ix = x-(x&7);
    int iy = y-(y&7);
    int bg = m_colorList.getPen(0);
    int fg = m_colorList.getPen(1);
    bg &= 0b11110111;
    bg |=((fg) & 0b1000);
 //   if (rand()%100>98)
   //     qDebug() << bg << fg;
    for (int j=0;j<8;j++)
        for (int i=0;i<8;i++) {
            int c = m_qImage->pixel(i+ix,j+iy);

            if (!(c==bg || c==fg))
                m_qImage->setPixel(ix+i,iy+j,QRgb(bg));
        }
}

bool sortFunc(const QPoint& lhs, const QPoint& rhs)
{
  return lhs.y() > rhs.y();
}
void LImageSpectrum::OrdererdDither(QImage &img, LColorList &colors, QVector3D strength, QPoint size, float gamma)
{
    m_ignoreConstraints = true;
    m_colorList.m_selectClosestFromPen = false;
    LImage::OrdererdDither(img,colors,strength,size,gamma);
    for (int iy=0;iy<m_height;iy+=8 )
        for (int ix=0;ix<m_width;ix+=8) {
            QVector<QPoint> winners;
            winners.resize(16);
            for (int i=0;i<winners.count();i++)
                winners[i] = QPoint(i,0);

            for (int j=0;j<8;j++) {
                for (int i=0;i<8;i++) {
                    int w = m_qImage->pixel(i+ix,j+iy);
                    if (w<winners.count())
                        winners[w].setY(winners[w].y()+1);
                }
            }
            std::sort(winners.begin(), winners.end(),sortFunc);
            int c1 = winners[0].x();
            int c2 = winners[1].x();

            c2 &= 0b11110111;
            c2 |=((c1) & 0b1000);

            // afterwards, make sure that both adhere to the winner's brightness:

            m_colorList.EnableColors(QVector<int>() <<c1 <<c2);
            for (int j=0;j<8;j++) {
                for (int i=0;i<8;i++) {
                    int c = m_qImage->pixel(i+ix,j+iy);
                    QColor col = m_colorList.m_list[c].color;
                    int winnerColor = -1;
                    m_colorList.getClosestColor(col,winnerColor);
                    setPixel(i+ix,j+iy,winnerColor);
                }
            }


        }

    m_colorList.EnableAllColors(true);


    m_ignoreConstraints = false;

}

void LImageSpectrum::LoadBin(QFile &file)
{
    m_ignoreConstraints=true;
    LImageQImage::LoadBin(file);
    m_ignoreConstraints=false;
}

void LImageSpectrum::ExportBin(QFile &ofile)
{
    QByteArray data;
    data.resize(32*192);
    QByteArray cols;
    QVector<int> cols1, cols2;
    for (int y=0;y<m_height;y+=8)
        for (int x=0;x<m_width;x+=8) {
            int c1,c2,b;
            getColors(x,y,c1,c2,b);
            uchar col = (c2&7) | (c1&7)<<3;
            col|=(b)<<6;
            cols.append(col);
            cols1.append(c1);
            cols2.append(c2);
        }



    for (int y=0;y<m_height;y++) {
        int pos = (y&0b00111000)*4 | ((y&7) | (y&0b11000000)/8)*0x100;
        for (int x=0;x<m_width;x+=8) {

            uchar b = 0;
            int c2 = cols2[(y/8)*32+x/8];
            for (int dx=0;dx<8;dx++) {
                uchar c = getPixel(x+dx,y);
                if (c==c2) b|=1<<(7-dx);

            }
            data[pos + x/8]=b;

        }
    }

    ofile.write(data);
    ofile.write(cols);
    // Take care of color data!

    ofile.close();

}

void LImageSpectrum::getColors(int x, int y, int& c1, int& c2, int &b)
{
    int ix = x-(x&7);
    int iy = y-(y&7);
    c1 = -1;
    c2 = -1;
    b=0;
    int cntA = 0;
    int cntB = 0;
    for (int j=0;j<8;j++)
        for (int i=0;i<8;i++) {
            int c = m_qImage->pixel(i+ix,j+iy);
            if (c!=c1 && c!=c2) {
                if (c1==-1)
                    c1 =c;
                else c2=c ;

            }
            if (c==c1)
                cntA++;
            else cntB++;

        }
    if (cntA>cntB)
        b = (c1&0b1000)>>3;
    else b= (c2&0b1000)>>3;


}

