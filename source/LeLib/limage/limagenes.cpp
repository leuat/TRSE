#include "limagenes.h"


LImageNES::LImageNES(LColorList::Type t) : CharsetImage(t)
{
    m_width = 256;
    m_height = 256;
    m_bitMask = 0b1;

    m_scaleX = 1.0f;
    m_noColors = 4;
    m_scale = 2;


    m_charWidth = 32;
    m_charHeight = 32;


    //m_data = new PixelChar[m_charWidth*m_charHeight];
    m_charWidthDisplay=16;
    m_charHeightDisplay=16;
    m_gridWidthDisplay = 16;

//    m_double=false;
    Clear();
    m_type = LImage::Type::NES;
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

    m_exportParams["StartX"] = 0;
    m_exportParams["EndX"] = m_charWidth;
    m_exportParams["StartY"] = 0;
    m_exportParams["EndY"] = m_charHeight;
    m_exportParams["Compression"] = 0;

    m_GUIParams[col1] = "Background";
    m_GUIParams[col2] = "Color 1";
    m_GUIParams[col3] = "Color 2";
    m_GUIParams[col4] = "Color 3";


    InitPens();

    //ForceColor();
}

void LImageNES::ImportBin(QFile &file)
{
    QByteArray data = file.readAll();
    int j=0;
    for (int i=0;i<m_charWidth*m_charHeight;i++) {
        for (int k=0;k<8;k++)
            m_data[i].p[k] = PixelChar::reverse(data[j+k]);
        j+=8;
    }
    ForceColor();
}

void LImageNES::ExportBin(QFile &file)
{
    QByteArray data;
    int j=0;
    for (int i=0;i<m_charWidth*m_charHeight;i++) {
        for (int k=0;k<8;k++)
            data.append( PixelChar::reverse(m_data[i].p[k]));
//        data.append( m_data[i].p[k]);
    }
    file.write(data);


    QString f = file.fileName();

    QString of = Util::getFileWithoutEnding(f) + ".pal";
    if (QFile::exists(of))
        QFile::remove(of);
    QFile wf(of);
    wf.open(QFile::WriteOnly);
    QByteArray ba = m_colorList.m_nesPPU;

    for (int i=0;i<8;i++) {
        ba[i*4+0] = m_colorList.m_nesPPU[0];
        ba[i*4+1] = m_colorList.m_nesPPU[i*4+2];
        ba[i*4+2] = m_colorList.m_nesPPU[i*4+1];
        ba[i*4+3] = m_colorList.m_nesPPU[i*4+3];
    }

    wf.write(ba);
    wf.close();

    QByteArray d2;
    d2.append(data);
    d2.append(ba);
    Util::SaveByteArray(d2,Util::getFileWithoutEnding(f) + ".chr");

}

void LImageNES::setMultiColor(bool doSet)
{
    ForceColor();
}

void LImageNES::ForceColor()
{
}



void LImageNES::SaveBin(QFile &file)
{
    file.write( ( char * )( &m_data ),  m_charWidth*m_charHeight*12 );
    file.write(m_colorList.m_nesPPU);

}


bool LImageNES::KeyPress(QKeyEvent *e)
{
    CharsetImage::KeyPress(e);

    if (e->key()==Qt::Key_0 ) { Data::data.currentColor = m_colorList.getPen(3);}
    if (e->key()==Qt::Key_1 ) { Data::data.currentColor = m_colorList.getPen(0);}
    if (e->key()==Qt::Key_2 ) { Data::data.currentColor = m_colorList.getPen(1);}
    if (e->key()==Qt::Key_3 ) { Data::data.currentColor = m_colorList.getPen(2);}
    return true;
}

void LImageNES::LoadBin(QFile &file)
{
    file.read( ( char * )( &m_data ),  m_charWidth*m_charHeight*12 );

    m_colorList.m_nesPPU = file.read(0x20);
/*    m_colorList.m_nesCols[0] = m_colorList.m_nesPPU[0];
    m_colorList.m_nesCols[1] = m_colorList.m_nesPPU[1];
    m_colorList.m_nesCols[2] = m_colorList.m_nesPPU[2];
    m_colorList.m_nesCols[3] = m_colorList.m_nesPPU[3];
*/
/*    for (int i=0;i<4;i++)
        m_colorList.setPen(i,m_colorList.m_nesPPU[i]);
*/
//    SetPalette(0);
    for (PixelChar& pc: m_data)
        for (int i=0;i<4;i++)
            pc.c[i] = m_colorList.getPen(i);


/*
    m_extraCols[0] = m_colorList.m_nesCols[3];;//m_colorList.m_nesCols[3];
    m_extraCols[1] = m_colorList.m_nesCols[0];
    m_extraCols[2] = m_colorList.m_nesCols[1];
    m_extraCols[3] = m_colorList.m_nesCols[2];
    m_background = m_extraCols[0];
 //   m_background = m_colorList.m_nesCols[1];

    qDebug() << "COLS IN ORDER "<< m_colorList.m_nesCols[0] << m_colorList.m_nesCols[1] <<m_colorList.m_nesCols[2] <<m_colorList.m_nesCols[3];
*/
//    m_background = m_colorList.m_nesCols[3];;
}

QPixmap LImageNES::ToQPixMap(int chr)
{
    //QImage img = m_data[chr].toQImage(64, m_bitMask, m_colorList, m_scale);
    int sz = 32;
    QImage img = QImage(sz,sz,QImage::Format_RGB32);
//    qDebug() << (chr*8)%16;
    int xx = 0;
    int yy = m_footer.get(LImageFooter::POS_CURRENT_BANK)*16;


    int c= 0;
    while (c<chr) {
        if (xx++>=15) {
            xx=0;
            yy++;
        }
        c++;
    }
    xx*=8;
    yy*=8;
    bool isDouble = m_double;
    m_double = false;
    int m = !m_footer.isFullscreen();
    m_footer.set(LImageFooter::POS_DISPLAY_CHAR,0);
    for (int i=0;i<sz;i++)
        for (int j=0;j<sz;j++)
            img.setPixel(i,j,m_colorList.get(getPixel(
                                                 (int)(i/(float)sz*8)+xx,
                                                 (int)(j/(float)sz*8)+yy)
                         ).color.rgb());

    m_double = isDouble;
    m_footer.set(LImageFooter::POS_DISPLAY_CHAR,m);
    return QPixmap::fromImage(img);

}

void LImageNES::SetPalette(int pal)
{
//    m_colorList.setPen(2-1, m_colorList.)
//    if (pal==m_oldPal)
  //      return;

    if (m_charset!=nullptr) {
//        m_charset->SetPalette(pal);
        return;
    }

    int m_oldPal = m_footer.get(LImageFooter::POS_CURRENT_PALETTE);
    if (m_oldPal!=pal && m_updatePaletteInternal) {
        m_colorList.m_nesPPU[m_oldPal*4 +1 +0] = m_colorList.getPen(2-1);
        m_colorList.m_nesPPU[m_oldPal*4 +1 +1] = m_colorList.getPen(2-0);
        m_colorList.m_nesPPU[m_oldPal*4 +1 +2] = m_colorList.getPen(2-2);
        m_colorList.m_nesPPU[m_oldPal*4 +1 +3] = m_colorList.getPen(3);
        m_colorList.m_nesPPU[0] = m_colorList.getPen(3);
//        if (rand()%100>98)
  //      qDebug() << "Setting 0 : " << QString::number(m_colorList.m_nesPPU[0]);
    }
//     m_colorList.m_nesPPU[m_oldPal*4 +1 +3] = m_colorList.getPen(3);
  //   m_colorList.m_nesPPU[0] = m_colorList.getPen(3);


     m_colorList.setPen(2-1,m_colorList.m_nesPPU[pal*4 +1 +0]);
     m_colorList.setPen(2-0,m_colorList.m_nesPPU[pal*4 +1 +1]);
     m_colorList.setPen(2-2,m_colorList.m_nesPPU[pal*4 +1 +2]);
//     m_colorList.setPen(3,m_colorList.m_nesPPU[pal*4 +1 +3]);
     m_colorList.setPen(3,m_colorList.m_nesPPU[0]);


}

bool LImageNES::getXY(QPoint& xy,QPoint& p1, QPoint& p2)
{
    int x = xy.x();
    int y = xy.y();
    if (m_double) {
        x=x/2;
        y=y/2;
    }

        if (m_footer.get(LImageFooter::POS_DISPLAY_CHAR)==1) {

        int cx = m_footer.get(LImageFooter::POS_CURRENT_DISPLAY_X);
        int cy = m_footer.get(LImageFooter::POS_CURRENT_DISPLAY_Y);

        int sx = (m_currentChar%m_charWidth)*8;
        int sy = (m_currentChar/m_charWidth)*16;

        if (!m_footer.get(LImageFooter::POS_CURRENT_DISPLAY_REPEAT)) {
            x = (x / (float)m_width)*16*cx+sx;
            y = (y / (float)m_height)*16*cy+sy;
        }
        else
        {
            x = (int)(x / ((float)m_width)*3*16*cx)%(8*cx)+sx;
            y = (int)(y / ((float)m_height)*3*16*cy)%(8*cy)+sy;

        }
    }


    int r = x/(float)8;
    x=x+r*8;
    if (m_double)
        y=y+128*m_footer.get(LImageFooter::POS_CURRENT_BANK);

//    m_pc1 = &getPixelChar((x/2),y);
  //  m_pc2 = &getPixelChar((x/2)+4,y);

    p1 = QPoint(x/2,y);
    p2 = QPoint((x/2)+4,y);


    int ix = x %8;//- (dx*m_charWidth);
    int iy = y % 8;//- (dy*m_charHeight);

    x = ix;
    y = iy;

    if (x<0 || x>=16 || y<0 || y>=8)
        return false;


    xy = QPoint(x,y);
    return true;
}

void LImageNES::InitPens()
{
    m_colorList.InitNESPens();
}

PixelChar &LImageNES::getPixelChar(int x, int y)
{
    int dx = x/(8/m_scale);
    int dy = y/8;
    int i = Util::clamp(dx + m_charWidth*dy,0,m_charWidth*m_charHeight);
    return m_data[i];
}

QString LImageNES::getMetaInfo() {
    QString txt = "The NES CHR image is your standard 8kb sprite/tileset (2x256 8x8 pixels) for any 40kb NES project. \n\n";
    txt+="Data are stored in 2 bitplanes, allowing for a total of 4 colours. The NES CHR consists of 2 \"pages\", where one contains sprite data while the other tile data (can be set programmatically on the NES).\n\n ";
    txt+="Each of these pages consists of 256 8x8 pixel characters that are usually combined into 16x16 or larger blocks. \n\n";
    txt+="From a palette of 54 unique colors, you can define 8 attribute palettes of 4 colours each (4 for sprites, 4 for tiles). This palette file is exported ";
    txt+="as a binary file, and can be used directly TRSE's built-in SetPalette NES method. ";
    return txt;
}

void LImageNES::CompressAndSave(QByteArray &chardata, QVector<int> &screen, int x0, int x1, int y0, int y1, int &noChars, double compression, int maxChars, int type, bool AddChars=true) {

    QVector<PixelChar> charsA;
    QVector<PixelChar> charsB;
    //    QByteArray data;
    QVector<int> data;
    int sx = x1-x0;
    int sy = y1-y0;
    data.resize(sx*sy);
    data.fill(0x0);
    noChars = 0;
    int w = m_charWidth/2;
    qDebug() << "W " <<m_width <<m_height << w << m_charWidth << sx;;

    for (int j=0;j<sy;j++) {

        for (int i=0;i<sx;i++)
        {
            PixelChar& pc1 = m_data[(i+x0 + (j+y0)*w)*2+0];
            PixelChar& pc2 = m_data[(i+x0 + (j+y0)*w)*2+1];
            int pi = 0;
            bool found = false;
            int cur = 1E5;
            for (int cc=0;cc<charsA.count();cc++) {
                //                if (found)
                //                  break;
                int metric = pc1.CompareLength3(charsA[cc]) + pc2.CompareLength3(charsB[cc]);
                //                int metric = pc.Compare(p);
                if (metric <=compression && metric<cur ) {
                    data[i + j*sx] = pi;
                    cur=metric;
                    found = true;
                    //                    break;

                }
                pi++;
                //                if (pi>255) exit(1);
            }
            if (!found) {
                charsA.append(pc1);
                charsB.append(pc2);
                data[i + j*sx] = charsA.count()-1;
                noChars++;

            }
            //        data[i]=i;
        }
    }

    //QFile f(outFile);
    //f.open(QFile::WriteOnly);
    QByteArray out;
    for (int i=0;i<maxChars;i++) {
        if (i<charsA.count()) {
            for (int j=0;j<8;j++) {
 //               out.append(PixelChar::reverse(charsA[i].p[j]));
//                out.append(PixelChar::reverse(charsB[i].p[j]));
                out.append(PixelChar::reverse(charsA[i].p[j]));
                out.append(PixelChar::reverse(charsB[i].p[j]));
//                qDebug() << "CHR : " << Util::numToHex(charsA[i].p[j]) << Util::numToHex(charsB[i].p[j]);
            }
        }
        else
            for (int j=0;j<16;j++)
                out.append((char)0);

    }
    //    qDebug() << data.count();
    //out.append(data);
    chardata.append(out);
    screen.append(data);
    /*    f.write(out);
       f.write(data);

       f.close();
   */

//    qDebug() << "CHARDATA : " << chardata.count()/16;
//    delete ni;

    // return out;


}



unsigned int LImageNES::getPixel(int x, int y)
{
    if (x>=m_width || x<0 || y>=m_height || y<0)
        return m_colorList.getPen(0);

    QPoint xy = QPoint(x,y);

    QPoint p1, p2;
    if (!getXY(xy, p1, p2)) {
        return m_colorList.getPen(0);
    }

    PixelChar& m_pc1 = getPixelChar(p1.x(), p1.y());
    PixelChar& m_pc2 = getPixelChar(p2.x(), p2.y());

    unsigned char pp = 3-((((m_pc1.p[xy.y()])>>xy.x()) & 0b1) | (((m_pc2.p[xy.y()])>>xy.x()) & 0b1)*2);

    return m_colorList.getPen(pp);


    //    return pc.get(m_scale*ix, iy, m_bitMask);

}

void LImageNES::setPixel(int x, int y, unsigned int col)
{
    if (x>=m_width || x<0 || y>=m_height || y<0)
        return;

    QPoint xy = QPoint(x,y);

    QPoint p1, p2;
    if (!getXY(xy, p1, p2))
        return;

    PixelChar& m_pc1 = getPixelChar(p1.x(), p1.y());
    PixelChar& m_pc2 = getPixelChar(p2.x(), p2.y());

    int j=0;
    for (int i=0;i<4;i++) {
        //        if (rand()%1000>988)
        //          qDebug() << "LImageNes::SETPIXEL " <<Util::numToHex(m_colorList.m_nesCols[i]) << Util::numToHex(i);
        if (m_colorList.getPen(i)==col)
            j=3-i;
    }

    m_pc1.p[xy.y()] &= ~(1<<xy.x());
    m_pc2.p[xy.y()] &= ~(1<<xy.x());
    m_pc1.p[xy.y()] |= (j&1)<<xy.x();
    m_pc2.p[xy.y()] |= ((j&3)>>1)<<xy.x();
    //return m_colorList.m_nesCols[pp];


}

void LImageNES::SetColor(uchar col, uchar idx)
{
/*    int li = idx;
    if (li==0)
        li=3;
    else
        if (li==3)
            li=0;


    m_colorList.setPen(li,col);//.m_nesCols[li] = col;
    if (idx!=0)
        m_colorList.SetPPUColors(col, idx-1);


    else m_colorList.m_nesPPU[0]=col;
*/
    //    if (rand()%100>98)
    //      qDebug() << "SETTING COLOR : " << idx <<  "   WITH COLOR " <<Util::numToHex(col);

    // qDebug() << "Setting idx " << idx << " to  "<< QString::number(col);

}

void LImageNES::CopyFrom(LImage *img)
{
    CharsetImage::CopyFrom(img);

    LImageNES* n = dynamic_cast<LImageNES*>(img);
    m_colorList.CopyFrom(&img->m_colorList);

    m_footer.m_data = img->m_footer.m_data;
    //    qDebug() << m_footer.isFullscreen() << img->m_footer.isFullscreen();

}

void LImageNES::CopySingleChar(LImage *src, int srcChar, int dstChar)
{
    LImageNES* other = dynamic_cast<LImageNES*>(src);
    if (other==nullptr) {
        qDebug() << "LImageNES::CopySingleChar only works with other NES images";
        return;
    }
    PixelChar& m_s1 = other->m_data[srcChar*2];
    PixelChar& m_s2 = other->m_data[srcChar*2+1];

    PixelChar& m_d1 = m_data[dstChar*2];
    PixelChar& m_d2 = m_data[dstChar*2+1];

//    qDebug() << "HERE";

    for (int i=0;i<8;i++) {
        m_d1.p[i] = m_s1.p[i];
        m_d2.p[i] = m_s2.p[i];
    }

}

