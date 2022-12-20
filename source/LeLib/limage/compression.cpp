#include "compression.h"
#include "source/LeLib/limage/limageamstradcpc.h"
#include "source/LeLib/limage/limagebbc.h"

Compression::Compression()
{

}

void Compression::SaveCharset(QString filename, QImage& img,int w, int h, int div)
{
    QFile f(filename);
    f.open(QFile::WriteOnly);
    QByteArray data;
    for (int y=0;y<h;y++)
        for (int x=0;x<w;x++) {
            char c = QColor(img.pixel(x,y)).red()/div;
            data.append(c);
        }

    f.write(data);
    f.close();

}

/*void Compression::AddScreenComplicated(QByteArray &data, QImage& img,int w, int h, char base, int div, char ca1, char ca2)
{

    for (int y=0;y<h;y+=1)
        for (int x=0;x<2*w;x+=2) {
            char c1,c2;
//            if (x<m_img.width() && y<m_img.height())
            c1 = QColor(img.pixel(x,y)).red()/div;
  //          if (x+1<m_img.width() && y<m_img.height())
            c2 = QColor(img.pixel(x+1,y)).red()/div;
            char add = 0;
            char c = max(c1,c2);
            //if (c>7) c=7;
            if (c1<1) add=ca1;
            if (c2<1) add=ca2;
//            add=0;
            data.append(c+base+add);
        }

}
*/
void Compression::AddScreen(QByteArray &data, QImage& img,int w, int h, char base, int div)
{

    for (int y=0;y<h;y+=1)
        for (int x=0;x<w;x+=1) {
            char c1;
            c1 = QColor(img.pixel(x,y)).red()/div;
            data.append(c1+base);
        }

}



void Compression::AddToDataX(QByteArray &data, MultiColorImage& img, int xp, int yp, int w, int h)
{
    for (int y=0;y<h;y+=1)
        for (int x=0;x<w;x+=1) {
            int xx = xp+x;
            int yy = yp+y;

            PixelChar& pc = img.m_data[40*(yy/8)+xx];
            data.append(PixelChar::reverse(pc.p[yy&7]));
        }

}

void Compression::AddToVZ200Data(QByteArray &data, LImage &img, int xp, int yp, int w, int h)
{
    for (int y=0;y<h;y+=1)
        for (int x=0;x<w;x+=4) {
            uchar c = 0;
            int xx = xp+x;
            int yy = yp+y;
            for (int j=0;j<4;j++) {
                uchar v = img.getPixel(xx+j,yy);
                c=c|(v<<(6-2*j));
            }
            data.append(c);

//            PixelChar& pc = img.m_data[40*(yy/8)+xx];
  //          data.append(PixelChar::reverse(pc.p[yy&7]));
        }

}

void Compression::AddToDataBBCMode5(QByteArray &data, LImage *img, int xp, int yp, int w, int h)
{
    for (int y=0;y<h;y+=1)
        for (int x=0;x<w;x+=4) {
            int xx = xp+x;
            int yy = yp+y;

//            uchar val  = img.m_colorList.getIndex(QColor(img.getPixel(xx,yy)));
            uchar pixel1  = img->getPixel(xx,yy);
            uchar pixel2  = img->getPixel(xx+1,yy);
            uchar pixel3  = img->getPixel(xx+2,yy);
            uchar pixel4  = img->getPixel(xx+3,yy);
            uchar c = LImageBBC::tablemode5[pixel4] |
                    LImageBBC::tablemode5[pixel3]<<1 |
                    LImageBBC::tablemode5[pixel2]<<2 |
                    LImageBBC::tablemode5[pixel1]<<3;

//            qDebug() << QString::number(val) << QColor(img.getPixel(xx,yy));
            data.append(c);
        }

}


void Compression::AddToDataVGA(QByteArray &data, LImageQImage& img, int xp, int yp, int w, int h)
{
    for (int y=0;y<h;y+=1)
        for (int x=0;x<w;x+=1) {
            int xx = xp+x;
            int yy = yp+y;

//            uchar val  = img.m_colorList.getIndex(QColor(img.getPixel(xx,yy)));
            uchar val  = QColor(img.getPixel(xx,yy)).blue();
//            qDebug() << QString::number(val) << QColor(img.getPixel(xx,yy));
            data.append(val);
        }

}


void Compression::AddBitplaneToData(QByteArray &data, MultiColorImage &img, int xp, int yp, int w, int h, int bpl, int shift)
{
    int d= 0;
    for (int y=0;y<h;y+=1)
        for (int bp=0; bp<bpl;bp++) {
            int curBit = pow(2,bp+shift);
//            curBit=curBit;


        for (int x=0;x<w;x++) {
            int xx = xp+x;
            int yy = yp+y;
            char c = 0;
            for (int i=0;i<8;i++) {
                int col = img.getPixel(xx*8+i,yy);
                if ((col & curBit)==curBit )
                    c = c | (0x1<<(7-i));
                //0101
            }
            data.append(c);
            d++;
//            PixelChar& pc = img.m_data[40*(yy/8)+xx];
  //          data.append(PixelChar::reverse(pc.p[yy&7]));
        }
     }
//    qDebug() << "AddBitplaneToData size " <<d << w << h;

}



void Compression::AddAmstradCPCToData(QByteArray &data, LImage *img, int xx, int yy, int w, int h)
{

    int d= 0;
    for (int y=0;y<h;y+=1) {

        char c = 0;
        for (int x=0;x<w;x++) {
             int curBit = x&1;

             int pixel = img->getPixel(x+xx,y+yy);
             c|= LImageAmstradCPC::table160[pixel]<<(1-curBit);

             if (curBit==1)
             {
                data.append(LImageAmstradCPC::AmstradCrazySwap(c));
                d++;
                c = 0;
             }
     }
    }
//    qDebug() << "AddCPC data size and width : " <<d << w << h;
}

void Compression::AddAtariBitplaneToData(QByteArray &data, MultiColorImage &img, int xp, int yp, int w, int h)
{
    int d= 0;
    int bpl = 4;
    // 4*(16/8)*4
    for (int y=0;y<h;y+=1)
//            curBit=curBit;


        for (int x=0;x<w;x++) {

            for (int bp=0; bp<bpl;bp++) {
                int curBit = pow(2,bp);


            int xx = xp+x;
            int yy = yp+y;

            unsigned int c = 0;
            for (int i=0;i<16;i++) {
                int col = img.getPixel(xx*16+i,yy);
                if ((col & curBit)==curBit )
                    c = c | (0b1<<(15-i));
                //0101
            }
            // Test
//            if (x!=0)
  //              c=0;
            data.append((char)((c>>8)&255));
            data.append((char)(c&255));
            d+=2;
//            PixelChar& pc = img.m_data[40*(yy/8)+xx];
  //          data.append(PixelChar::reverse(pc.p[yy&7]));
        }
     }
    qDebug() << "AddAtariBitplaneToData size " <<d << w << h;

}

void Compression::AddSingleAtariBitplaneToData(QByteArray &data, MultiColorImage &img, int xp, int yp, int w, int h, int bpl_select)
{
    int d= 0;
    int bpl = 4;
    // 4*(16/8)*4
    for (int y=0;y<h;y+=1)
//            curBit=curBit;


        for (int x=0;x<w;x++) {

            for (int bp=0; bp<bpl;bp++) {
                int curBit = pow(2,bp);


            int xx = xp+x;
            int yy = yp+y;

            unsigned int c = 0;
            for (int i=0;i<16;i++) {
                int col = img.getPixel(xx*16+i,yy);
                if ((col & curBit)==curBit )
                    c = c | (0b1<<(15-i));
                //0101
            }
            // Test
//            if (x!=0)
  //              c=0;
            if (bp == bpl_select) {
                data.append((char)((c>>8)&255));
                data.append((char)(c&255));
            }
            d+=2;
//            PixelChar& pc = img.m_data[40*(yy/8)+xx];
  //          data.append(PixelChar::reverse(pc.p[yy&7]));
        }
     }
    qDebug() << "AddAtariBitplaneToData size " <<d << w << h;

}


void Compression::AddGameboyData(QByteArray &data, MultiColorImage &img, int xp, int yp, int w, int h)
{
    int d= 0;
//            curBit=curBit;

    for (int ymain=0;ymain<h/8;ymain++)
    for (int x=0;x<w;x++) {
        for (int y=0;y<8;y+=1) {

            for (int bp=0; bp<2;bp++) {

                int curBit = pow(2,bp);

                int xx = xp+x;
                int yy = yp+y+ymain*8;
                char c = 0;
                for (int i=0;i<8;i++) {
                    int col = img.getPixel(xx*8+i,yy);
                    if ((col & curBit)==curBit )
                        c = c | (0x1<<(7-i));
                    //0101
                }
                data.append(c);

                d++;
                //            PixelChar& pc = img.m_data[40*(yy/8)+xx];
                //          data.append(PixelChar::reverse(pc.p[yy&7]));
            }
        }
    }
    //    qDebug() << "AddBitplaneToData size " <<d << w << h;

}



int Compression::CompareSprites(QByteArray &d1, QByteArray& d2, int sprite1, int sprite2)
{
    int l=0;
    for (int i=0;i<63;i++) {
        if (d1[i+sprite1*64] != d2[i+sprite2*64])
            l++;
    }
    return l;
}

void Compression::CompressScreenAndCharset(QVector<int> &screen, QByteArray &charset, QVector<int> &sOut, QByteArray &cOut, int sw, int sh, int charSize, int noTargetChar, int bmask)
{
    cOut.clear();
    sOut.clear();
    sOut = screen;
    cOut = charset;
    if (sw==0 || sh==0)
        return;
    int screens = screen.size()/sw/sh;
//    QByteArray curCharset = charset;

//    cOut.insert(0,32,0);
/*    for (int i=0;i<8;i++) {
        cOut[i+0] = 0;
        cOut[i+8] = 0xFF;
        cOut[i+16] = 0b10101010;
        cOut[i+24] = 0b01010101;
    }
*/

    int curChars = cOut.length()/8;

    double compression = 0.03;
    int pass = 0;
//    for (int i=0;i<sOut.count();i++)
 //       qDebug() << "Sout : " << i << sOut[i];
    // First, make everything unique
    for (int i=0;i<screens;i++) {
        int k = charSize*i;
        for (int j=0;j<sw*sh;j++) {
            sOut [j + k] += k;
        }
    }
    while (curChars>=noTargetChar) {
        qDebug() << "Cur chars " <<curChars << " with compression " << compression;
        // What to do: go through every char and check every other that is the same
        for (int i=0;i<cOut.length()/8;i++) {
       //     int found = -1;
            QVector<int> found;
            double curMin = 1E5;
            for (int j=i+1;j<cOut.length()/8;j++) {
                double res = Compare(cOut, cOut,j*8,i*8,8, 1, bmask);
                if (res<compression) {
                    found.append(j);
//                    curMin = res;
                 }
            }

            int cur=0;
            for (auto f : found) {
                f = f-cur;// Collaps characters:
                cur++;
                cOut.remove(f*8,8); // Removed
//                found = found &0xFF;// noTargetChar;
               // int add = 0;
                for (int j=0;j<sOut.length();j++) {
                    if (sOut[j]==f) sOut[j] = i;
                    if (sOut[j]>f) sOut[j]--;

                }
            }
        }
        pass++;
        curChars = cOut.length()/8;
        qDebug() << "After pass " << pass <<  "  : " << curChars;
        if (compression ==0) compression = 0.01;
        compression = compression * 1.05;
        // After pass :
    }
//    screen = sOut;
}

void Compression::OptimizeScreenAndCharset(QVector<int> &screen, QByteArray &charset, QVector<int> &sOut, QByteArray &cOut, int sw, int sh, int charSize, double compression, int type, LColorList& lst, int bmask)
{
    cOut.clear();
    sOut.clear();
    if (sw==0 || sh==0)
        return;
    int screens = screen.size()/sw/sh;
/*    for (int i=0;i<screens;i++) {
        for (int x=0;x<sw*sh;x++)
            screen[i*sw*sh +x] = screen[i*sw*sh +x]+ charSize*i;
    }*/
//    qDebug() << sw << sh << charSize << screens << charset.count()/8 << compression;
    int sum = 0;
    cOut.resize(32);
    for (int i=0;i<8;i++) {
        cOut[i+0] = 0;
        cOut[i+8] = 0xFF;
        cOut[i+16] = 0b10101010;
        cOut[i+24] = 0b01010101;
    }


//    for (int i=0;i<charset.count();i++)
  //      qDebug() << QString::number(charset[i]);

    // All screens are set up. Start compressing!
    for (int i=0;i<screens;i++) {
        qDebug() << "Current screen : " << i ;

        for (int x=0;x<sw*sh;x++) {
            int s = screen[i*sw*sh + x] + charSize*i;


  //          qDebug() << "   Current s: " << QString::number(s) << " with compression type " <<type ;
            int found = -1;
            double curMin = 8000;
            for (int j=0;j<cOut.length()/8;j++) {
             //   int Compression::Compare(QByteArray &a, QByteArray &b, int p1, int p2, int length)
                double res = Compare(cOut, charset,j*8,s*8,8, type, bmask);
//                double res = 1;
                if (res<compression && res<curMin) {


                    found = j;
                    curMin = res;
    //                qDebug() << "Found similar: " << found;
//                    break;
                }
            }
//            qDebug() << "FOUND " << found;
            if (found ==-1) {
                found = cOut.size()/8;
                for (int j=0;j<8;j++)
                    cOut.push_back( charset[s*8+j]  );
            //    qDebug() << "Added new : " << found;
            }
//            qDebug() << "  ADDING : " << QString::number(found) << " vs "  << QString::number(s);

            sOut.append((uchar)found);
        }

    }
    qDebug() << "Final # chars: " << cOut.size()/8;
    qDebug() << "Final # screens: " << sOut.size()/sw/sh;
    qDebug() << "Final # res: " << sum;

}


void Compression::OptimizeScreenAndCharsetGB(QVector<int> &screen, QByteArray &charset, QVector<int> &sOut, QByteArray &cOut, int sw, int sh, int charSize, double compression,int type,LColorList& lst, int bmask)
{
    cOut.clear();
    sOut.clear();
    if (sw==0 || sh==0)
        return;
    int screens = screen.size()/sw/sh;
/*    for (int i=0;i<screens;i++) {
        for (int x=0;x<sw*sh;x++)
            screen[i*sw*sh +x] = screen[i*sw*sh +x]+ charSize*i;
    }*/
    qDebug() << sw << sh << charSize << screens << charset.length()/16 << compression;
    int sum = 0;
    cOut.resize(64);
    for (int i=0;i<8;i++) {
        cOut[i+8*0] = 0;
        cOut[i+8*1] = 0;

        cOut[i+8*2] = 0xFF;
        cOut[i+8*3] = 0xFF;

        cOut[i+8*4] = 0xFF;
        cOut[i+8*5] = 0x00;

        cOut[i+8*6] = 0x00;
        cOut[i+8*7] = 0xFF;

    }

//    for (int i=0;i<charset.count();i++)
  //      qDebug() << QString::number(charset[i]);

    // All screens are set up. Start compressing!
    for (int i=0;i<screens;i++) {
       // qDebug() << "Current screen : " << i ;

        for (int x=0;x<sw*sh;x++) {
            int s = screen[i*sw*sh + x] + charSize*i;


//            qDebug() << "   Current s: " << QString::number(s) ;
            int found = -1;
            double curMin = 80000;
            for (int j=0;j<cOut.length()/16;j++) {
                //int Compression::Compare(QByteArray &a, QByteArray &b, int p1, int p2, int length)
                double res = Compare(cOut, charset,j*16,s*16,16,type, bmask);
                if (res<compression && res<curMin) {
                    found = j;
                    curMin = res;
    //                qDebug() << "Found similar: " << found;
//                    break;
                }
            }
            if (found ==-1) {
                found = cOut.size()/16;
                for (int j=0;j<16;j++)
                    cOut.push_back( charset[s*16+j]  );
      //          qDebug() << "Added new : " << found;
            }
//            qDebug() << "  ADDING : " << QString::number(found) << " vs "  << QString::number(s);

            sOut.append((uchar)found);
        }

    }
    qDebug() << "Final # chars: " << cOut.size()/16;
    qDebug() << "Final # screens: " << sOut.size()/sw/sh;
    qDebug() << "Final # res: " << sum;

}

void Compression::SaveSinusScrollerData_OLD(MultiColorImage* mc, int height, int startaddr, QString fname)
{
}



int Compression::BitplaneCharsetSpritePacker(QByteArray& inData, QByteArray &outData, QVector<int> &arrangement,
                                              int x, int y, int w, int h, int compression, int noBitplanes) {
    int cur = 0;
    outData.clear();
    int foundChars = 0;
    int totalChars = 0;
 //   qDebug() << arrangement.size();
    for (int j=0;j<h;j++)
        for (int i=0;i<w;i++) {
            // Lookup in array
            int ii =(i+(j)*w)*noBitplanes*8;
//            qDebug() << "Currently on "<<ii<<i<<j << " with total size" << totalChars << " found chars" << foundChars;
/*            QByteArray org;
            for (int i=0;i<noBitplanes;i++)
                org.append(ii + i);
*/
            //double Compression::Compare(QByteArray &a, QByteArray &b, int p1, int p2, int length, int type, int bmask)
            int found = -1;
            double best = 1E30;
            for (int k=0;k<totalChars;k++) {
                double res = Compare(inData, outData,ii,k*noBitplanes*8,8*noBitplanes, TYPE_REGULAR, 1);
//                qDebug() << res;
                if (res<compression) {
                    if (res<best) {
                        found = k;
                        best = res;
                    }
//                    break;
                }

            }

            if (found==-1) {
                // Copy and add data
                for (int k=0;k<noBitplanes*8;k++) {
                    outData.append( inData[ii+k]);
                }

                arrangement.append(totalChars);
                totalChars+=1;

            }
            else {
                arrangement.append(found);
                foundChars +=1;
            }
        }
//    qDebug() << "Compression::BitplaneCharsetSpritePacker found / total " << foundChars << totalChars << " Arrangement size " << arrangement.size();
    return totalChars;
}


void Compression::SaveSinusScrollerData(MultiColorImage* mc, int height, int startaddr, QString fname)
{
    QString dfname = fname;
    if (QFile::exists(dfname+"_data.bin"))
        QFile::remove(dfname+"_data.bin");

    QFile f(dfname+".bin");
    f.open(QIODevice::WriteOnly);

    mc->ExportBin(f);
    f.close();
    LImageIO::Save(dfname+".flf",mc);

    QFile f2(dfname+"_unroll.ras");
    f2.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream s(&f2);

    QVector<int> addr;
    QVector<int> inv;
    QVector<int> iny;
    QVector<int> inyBefore;
    QVector<int> single;
    for (int x=0;x<40;x++) {
        uchar c = 0;
        uchar d = 0;
        int y = 0;

        while (c==0 && d==0) {
            int pos = y*40 + x;

            c = mc->m_data[pos].c[1];
            d = mc->m_data[pos].c[2];
            y++;
        }
        y--;
        // here we start
        uchar oc = 0;
        uchar od = 0;
        bool done = false;
        bool t = false;
        int cnt = 0;
        int rows = 0;
        int alt = 0;
        uchar cval = 1;
        int countdown = 0;

        int addy=0;
        y*=8;
        while (!done && y<200) {
            int pos = ((int)y/8)*40+x;
            oc = mc->m_data[pos-40].c[1];
            od = mc->m_data[pos-40].c[2];
            c = mc->m_data[pos].c[1];
            d = mc->m_data[pos].c[2];
            int sng = 0;
            bool cross = false;
            int iyb = 0;
            int orgVal = cval;
            bool ldone = false;
            for (int xx=0;xx<1;xx++) {
                uchar val = mc->getPixel(x*4+xx,y);
                //                if (cval!=val && val!=0 && val>cval) {
                if (val!=cval && val!=0) {
                    cval = val;
                    cross = true;

                }
            }


            int iv = 0;
//            if (isUniform)
  //              addy=1;
            if (cross) {
                alt++;
                addy=1;
            }
//            qDebug() <<addy <<y;
//            if (rows==0) addy=1;
            if (c==0 && d!=0) {
                sng=2;
            }
            if (c!=0 && d==0) {
                sng=1;
            }

            int inyAfter = 0;

            if ((y&7)==0) {
//                qDebug() << "Append "<<y;
                if (addy==1 && c==oc) {
                    addy=0; inyAfter=1;
                }
                iny.append(inyAfter);
                inyBefore.append(addy);
                inv.append(0);
                single.append(sng);
                cnt+=addy + inyAfter;
                addr.append(pos+startaddr);
                rows++;


  //              if (x==2)
    //                  qDebug() << "Y:" <<y << " - " <<QString::number(c) << QString::number(d)<< "- "<<QString::number(nc) << QString::number(nd) <<  " - " << addy << cnt;
//                      qDebug() << "Y:" <<y << " - " <<QString::number(c) << QString::number(d) <<  " - " << addy << cnt << cval;


                addy = 0;
            }
            if (cnt==8)
                done =true;


            oc = c;
            od = d;


            y++;

        }
    //    qDebug() << "DONE";
        // Last one
/*        int pos = y*40+x;
        addr.append(pos+startaddr);
        cnt++;
        iny.append(1);
*/
        int add = 1;
        if (cnt!=8) {
            qDebug() << "CNT NOT 8 for x="<<x<<cnt<<8-cnt;
            add = 8-cnt;

        }
        iny[iny.count()-1]=add;

//        qDebug() << cnt << rows;

    }
//    int i=0;
  /*  for (int j=0;j<2;j++) {*/
//    s<<"procedure paint_unroll"+QString::number(j)+"();\n";
    s<<"procedure paint_unroll();\n";
    s<<"begin\n";
    s<<" asm(\"\n";

/*    if (j==0) s<<" ldy #0\n";
    if (j==1)
        s<<"\tldy $72\n";
        s<<

*/
    s<<"\tldy #0\n";
    int cnt = 0;
    for (int i=0;i<addr.length();i++) {
        if (inyBefore[i]!=0) {
            for (int j=0;j<inyBefore[i];j++)
                s<<"\tiny\n";
            cnt+=inyBefore[i];
            if (cnt>=256) {
                s<<"\tinc zp+1 \n";
                s<<"\tinc zp2+1\n";
                cnt -=256;
            }
        }
        if (single[i]==0) {
            s<<"\tlda (zp2),y\n";
            s<<"\tora (zp),y\n";
        }
        if (single[i]==2) {
            s<<"\tlda (zp2),y\n";
        }
        if (single[i]==1) {
            s<<"\tlda (zp),y\n";
        }
        s<<"\tsta $"+QString::number(addr[i],16)+"\n";
        if (iny[i]!=0) {
            for (int j=0;j<iny[i];j++)
                s<<"\tiny\n";
            cnt+=iny[i];
            if (cnt>=256) {
                s<<"\tinc zp+1 \n";
                s<<"\tinc zp2+1\n";
                cnt = 0;
            }
        }
  //      i++;


    }
//    if (j==0)
  //      s<<"\tsty $72\n";
    s<<" \");\n";
    s<<"end;\n";
    //}

    f2.close();

}


void Compression::SaveCompressedSpriteData(QByteArray &data, QString dataFile, QString tableFile, int address, int compressionLevel)
{
    int noSprites = data.length()/64;
    QByteArray dataOut, tableOut;
    int noSpritesOut = 0;
    int compressed  = 0;
    qDebug() << data.length();
    noSpritesOut = 2;
    for (int i=0;i<64;i++) dataOut.append((char)0);
    for (int i=0;i<64;i++) dataOut.append((char)0xFF);
    for (int i=0;i<noSprites;i++) {

        int found = -1;

        for (int j=0;j<noSpritesOut;j++) {
            if (CompareSprites(data,dataOut,i,j)<compressionLevel) {
                found = j;
            }
        }
        if (found!=-1) compressed++;

        if (found==-1) {
            for (int j=0;j<64;j++)
                dataOut.append(data[j+i*64]);

            found = noSpritesOut;
            noSpritesOut++;
            qDebug() << noSpritesOut << " vs " << noSprites;

        }

        tableOut.append(found + address/64);

    }
    qDebug() << "# sprites:" << noSprites;
    qDebug() << "# Compressed sprites:" << noSprites-compressed;
    qDebug() << "Compression level:" << QString::number((1-compressed/(float)noSprites)*100) << "%";
    Util::SaveByteArray(dataOut,dataFile);
    Util::SaveByteArray(tableOut, tableFile);

}

double Compression::Compare(QByteArray &a, QByteArray &b, int p1, int p2, int length, int type, int bmask)
{
    int l = 0;
    if (type==TYPE_REGULAR) {
        for (int i=0;i<length;i++) {
            for (int j=0;j<8;j++) {
                uchar k1 =(a[p1+i]>>(j)) & 0b1;
                uchar k2 =(b[p2+i]>>(j)) & 0b1 ;
                if (k1!=k2) {
                    l++;
                    //                if ((i&1)==0)
                    //                    qDebug() << "SHOULD NOT HAPPEN";
                }
            }
        }
//        if (rand()%100>98) qDebug() << "TYPE: " << l;
        return l;

    }

   if (type==TYPE_SSIM) {
       PixelCharSSIM pc1,pc2;
       for (int j=0;j<8;j++) {
           pc1.p[j] = a[p1+j];
           pc2.p[j] = b[p2+j];
       }
       pc1.m_lastBitmask = 1;//bmask;
       pc2.m_lastBitmask = 1;//bmask;
//       double l = 1;
       double l = pc1.CalcSSIM(&pc2);
       return l;

   }
   qDebug() << "UKNOWN COMPRESSION TYPE";
//    exit(1);


  //      if (a[p1+i]!=b[p2+i]) l++;
  //  }
    return -1;
}

void Compression::OptimizeAndPackCharsetData(QByteArray &dataIn, QByteArray &out, QByteArray &table, int width, double compression, bool invertTable,int type, LColorList& lst, int bmask)
{
    out.clear();
    table.clear();

    int cnt = dataIn.length()/width;
    qDebug() << "Width : " << width ;
    qDebug() << "Data count : " <<dataIn.length();
    qDebug() << "total rows: " << cnt;
    qDebug() << "total chars x frames: " << (cnt/width);
    qDebug() << "total chars: " << (cnt/32)/width;
    qDebug() << "Compression : " << compression;

    qDebug() << "cnt SHOULD be " << (32*width*6*21);


    for (int i=0;i<cnt;i++) {
        unsigned short currentPointer = 0;
        bool isNew = true;
        for (int j=0;j<out.length()/width;j++) {
            double metric = Compare(dataIn, out, i*width, j*width, width, type, bmask);
            if (metric<compression) {
                currentPointer =j*width;
                isNew=false;
                //break;
            }
        }
        if (isNew) {
            currentPointer = out.length();
            for (int j=0;j<width;j++) {
                out.append(dataIn[i*width+j]);
            }
        }
//        qDebug() << isNew << currentPointer;
        char lo = currentPointer&0xff;
        char hi = (currentPointer>>8)&0xff;
//        qDebug() << QString::number(lo) << QString::number(hi);
        if (invertTable) {

            table.append(hi);
            table.append(lo);
        }
        else {
        table.append(lo);
        table.append(hi);
        }

    }


}

void Compression::GenerateParallaxData(QString inFile, QString outFile, int x0, int y0, int x1, int y1, int p1, int type, int eor)
{
    LImage* imgIn = (LImage*)LImageIO::Load(inFile);
    if (imgIn==nullptr) {
        qDebug() << "File not found / not c64 fullscreen : " << inFile;
        return;
    }
    C64FullScreenChar* fs = (C64FullScreenChar*)imgIn;
    if (fs!=nullptr)
        imgIn->LoadCharset(fs->m_charsetFilename,0);

    outFile = outFile.split(".")[0];
    QVector<int> screenData;
    QByteArray charData;
    int compType = 0;
    for (int scroll=0;scroll<8;scroll+=2) {
        QSharedPointer<CharsetImage> c1 = QSharedPointer<CharsetImage>(new CharsetImage(imgIn->m_colorList.m_type));
 //       c1->setMultiColor(false)
        c1->m_width = 320;
        c1->m_bitMask = 0b1;
        c1->m_noColors = 4;
        c1->m_scale = 1;
        c1->m_minCol = 0;

        c1->m_width = 320;
        c1->m_bitMask = 0b11;
        c1->m_noColors = 4;
        c1->m_scale = 1;
        c1->m_minCol = 0;


//        c1->setMultiColor(false);
//        c1->m_charWidth = imgIn->m_charWidth;
  //      c1->m_width = imgIn->m_width;
        // Paint data!
        for (int y=0;y<imgIn->m_height;y++)
            for (int x=0;x<imgIn->m_width;x++) {
                int i = imgIn->getPixel(x/2,y);
                if (i!=0)
                    c1->setPixel((x+scroll)%(c1->m_width),y, 1);
            }
        int noChars;
        QVector<int> tmpI;
        QByteArray ba;
        c1->CompressAndSave(ba, tmpI, x0,x1,y0,y1,
                            noChars,p1,  256, compType,true);
        charData.append(ba);
        screenData.append(tmpI);
//        qDebug() << "NO chars frame " << scroll << " : " << noChars <<imgIn->m_charHeight << screenData.count();
        //qDebug() << charData;

        if (scroll==2)
            LImageIO::Save(outFile+"_test.flf",c1.get());

    }
    // Type set
    if (type!=0) {
        QByteArray n;
        for (char c: charData) {
            char r = 0;
            for (int i=0;i<4;i++) {
                char k = (c>>(i*2))&0b11;
                if (k==0b11)
                    k = type;
                r = r | k<<(2*i);

            }
            n.append(r ^eor);
        }
        charData = n;
    }
//    qDebug() << type << QString::number(eor,2) <;

    QString fsOut = outFile+"_screens.bin";
    QString fcOut = outFile+"_charset.bin";
    QVector<int> sOut;
    QByteArray cOut;
/*    OptimizeScreenAndCharset(screenData, charData, sOut, cOut,  x1-x0,y1-y0,
                             256,p1, compType,imgIn->m_colorList,imgIn->m_bitMask);
                             */
//    int sw, int sh, int charSize, double compression, int type, LColorList& lst, int bmask//
/*    Util::SaveByteArray(cOut, fcOut);
    QByteArray d = Util::toQByteArray(sOut);
    Util::SaveByteArray(d, fsOut);
*/
//    qDebug() << "Parallax no chars : " << charData.count()/8 << " vs ORG " << charData.count()/8 << "  ";

     Util::SaveByteArray(charData, fcOut);
     QByteArray d = Util::toQByteArray(screenData);
     Util::SaveByteArray(d, fsOut);


//    void Compression::OptimizeScreenAndCharset(QVector<int> &screen, QByteArray &charset, QVector<int> &sOut, QByteArray &cOut, int sw, int sh, int charSize, double compression, int type,LColorList& lst, int bmask)


//    qDebug() << "WHOO";
}

void Compression::AddCharsetScreen(QByteArray &data, QImage &img, CharsetImage *charset, int w, int h)
{


}

void Compression::AddPetsciiScreen(QByteArray &data, QImage &img)
{

    QByteArray d;
//    qDebug() << QString::number(img.width());
    int pixelw = img.width()/80;
    int pixelH = img.height()/50;
    QColor bg(QColor(0,0,0));
    int cnt = 0;
    unsigned cc = 0;
    unsigned char petTable[16]= {0x20, 0x7e, 0x7C, 0xE2,
                                    0x6C, 0x7f, 0xE1,  0xFb,
                                0x7b, 0x61, 0xff, 0xec,
                                0x62, 0xfc, 0xfe, 0xA0};

    for (int y=0;y<25;y++)
        for (int x=0;x<40;x++) {
            int xx = x/40.0*(float)img.width();
            int yy = y/25.0*(float)img.height();



            int i1 =  !Util::isEqual(img.pixelColor(xx,yy),bg) <<0 |
                      !Util::isEqual(img.pixelColor(xx+pixelw,yy),bg) <<1 |
                      !Util::isEqual(img.pixelColor(xx,yy+pixelH),bg) <<3 |
                      !Util::isEqual(img.pixelColor(xx+pixelw,yy+pixelH),bg) <<2;

            int i2 =  !Util::isEqual(img.pixelColor(xx,yy),bg) <<0;
            i2=i2*0xA0;
//            d.append(petTable[i1]);
            d.append(i2);
//            qDebug() << i1 << img.pixelColor(xx,yy);


/*            if (cnt==0) {
                cc=i1;
            }
            else {
                cc = cc | i1<<4;
                d.append(cc);

            }
*/
//            cnt=(cnt+1)&1;
//            unsigned char i1 =
        }

    data.append(d);
}


void Compression::AddBinaryScreen(QByteArray &data, QImage &img)
{

    QByteArray d;
//    qDebug() << QString::number(img.width());
    int pixelw = img.width()/80;
    int pixelH = img.height()/50;
    QColor bg(QColor(0,0,0));
    int cnt = 0;
    unsigned cc = 0;

    for (int y=0;y<25;y++)
        for (int x=0;x<40;x++) {
            int xx = x/40.0*(float)img.width();
            int yy = y/25.0*(float)img.height();

            int i1 =  !Util::isEqual(img.pixelColor(xx,yy),bg) <<0;

            if (cnt==0) {
                cc=i1;
            }
            else {
                cc = cc | i1<<cnt;

            }

            cnt=(cnt+1);
            if (cnt==8) {
                cnt = 0;
                d.append(cc);

            }
//            unsigned char i1 =
        }

    data.append(d);
}

void Compression::SaveCompressedTRM(QByteArray& inData, QString fileName, int c)
{
    int count = inData.length()/1000;
    qDebug() << "count screens: " << count;
    MovieConverter mc;
    float compr = 0;
    char endChar = 0x56;
    char skipChar = 0x1;


    QByteArray header;


    header.append(40);
    header.append(25);
    header.append((unsigned char)count);
    header.append((char)c);
    header.append((char)0);
    header.append(endChar);
    header.append(skipChar);

//    file.write(header);



    QByteArray data;

    data.append(header);
    for (int i=0;i<count-1;i++) {

        QByteArray s1, s2;
        for (int j=0;j<1000;j++) {
            s1.append(inData[i*1000 + j]);
            s2.append(inData[(i+1)*1000 + j]);
        }

        if (c==3)
        data.append(mc.CompressScreen3(s1, s2,
                40, 25,compr,
                endChar ,skipChar, false));

        if (c==2)
        data.append(mc.CompressScreen2(s1, s2,
                40, 25,compr,
                endChar ,skipChar,false));

    }
   Util::SaveByteArray(data,fileName) ;

}

void Compression::GenerateShiftedCharset(QString inFile, QString outFile, int start, int end)
{
    LImage* imgIn = (LImage*)LImageIO::Load(inFile);
    MultiColorImage* mc = dynamic_cast<MultiColorImage*>(imgIn);
    if (imgIn==nullptr || mc==nullptr) {
        qDebug() << "File not found / not c64 fullscreen : " << inFile;
        return;
    }
    QByteArray charData;
    int compType = 0;
    QSharedPointer<CharsetImage> t = QSharedPointer<CharsetImage>(new CharsetImage(imgIn->m_colorList.m_type));
    t->Initialize(320,200);
    t->setMultiColor(false);
    for (int i=start;i<end;i++) {
        for (int j=0;j<8;j++)
            t->m_data[i*2+1].p[j] = mc->m_data[i].p[j];

    }
    for (int scroll=0;scroll<8;scroll+=2) {
        QSharedPointer<CharsetImage> c1 = QSharedPointer<CharsetImage>(new CharsetImage(imgIn->m_colorList.m_type));
        //       c1->setMultiColor(false)
        c1->m_width = 320;
        c1->m_bitMask = 0b1;
        c1->m_noColors = 2;
        c1->m_scale = 1;
        c1->m_minCol = 0;


        //        c1->setMultiColor(false);
        //        c1->m_charWidth = imgIn->m_charWidth;
        //      c1->m_width = imgIn->m_width;
        // Paint data!
        for (int y=0;y<mc->m_height;y++)
            for (int x=0;x<mc->m_width;x++) {
                int i = t->getPixel(x,y);
                if (i!=0)
                    c1->setPixel((x-scroll)%(c1->m_width),y, 1);
            }

        for (int i=0;i<(end-start)*2;i++) {
            for (int j=0;j<8;j++)
                charData.append(PixelChar::reverse(c1->m_data[i].p[j]));
        }

    }
    Util::SaveByteArray(charData, outFile);
}

void Compression::GenerateRotatedCharset(QString inFile, QString outFile, int start, int end, int dir)
{
    LImage* imgIn = (LImage*)LImageIO::Load(inFile);
    MultiColorImage* mc = dynamic_cast<MultiColorImage*>(imgIn);
    if (imgIn==nullptr || mc==nullptr) {
        qDebug() << "File not found / not c64 fullscreen : " << inFile;
        return;
    }
    QByteArray charData;
    int compType = 0;
    QSharedPointer<CharsetImage> t = QSharedPointer<CharsetImage>(new CharsetImage(imgIn->m_colorList.m_type));
    t->Initialize(320,200);
    t->setMultiColor(false);
    for (int i=start;i<end;i++) {
        for (int j=0;j<8;j++)
            for (int k=0;k<8;k++) {
                int x = j;
                int y= k;
                if (dir==1)  {
                    x = 7-k;
                    y = j;
                }
                if (dir==2)  {
                    y = 7-k;
                    x = 7-j;
                }
                if (dir==3)  {
                    x = k;
                    y = 7-j;
                }
                unsigned char pp = (mc->m_data[i].p[y]>>x) & 1;
                if (pp!=0) {
                    unsigned int f = ~(1 << j);
                    PixelChar& pc = t->m_data[i];
                    pc.p[k] &= f;
                    // Add

                    pc.p[k] |= 1<<j;
                }

            }
    }
    for (int i=0;i<(end-start);i++) {
        for (int j=0;j<8;j++)
            charData.append(PixelChar::reverse(t->m_data[i].p[j]));
    }
    Util::SaveByteArray(charData, outFile);

}

void Compression::GenerateUnrolledAsm1(QString name, QString outFile, QString inFile, int src, int dst, int width, int height, int shift1, int shift2, int shift3)
{
    QString out="procedure "+name+"();\n";

    out+="begin\n";
    out+="\tasm(\"\n";
/*    uchar c = Util::MultiCharMask(0b10000001);
    QString s = QString::number(c,2);
    qDebug() << s;
*/
    if (!QFile::exists(inFile))
    {
        ErrorHandler::e.Error("Generate unrolled asm code: could not find file "+inFile);
    }
    QByteArray img = Util::loadBinaryFile(inFile);
    for (int j=0;j<height;j++) {
        for (int i=0;i<width;i++) {

            int pos1 = i*8+j*320+shift1;
            int pos2 = i*8+j*320+shift1+4;
            uchar org1=0;
            uchar org2=0;
            if (pos1<img.size())
                org1 = img[pos1];
            if (pos2<img.size())
                org2 = img[pos2];
            uchar m1 = ~Util::MultiCharMask(org1);
            uchar m2 = ~Util::MultiCharMask(org2);
//            if (m1!=0)
  //              qDebug() << (int)m1;
            // 1000 org
            // 1100 mask
            // 0101 val
            // 1101

            if (org1==0) {
                out +="\t lda "+Util::numToHex(src)+"+ "+Util::numToHex(i)+"+ "+ Util::numToHex(shift2) +"+"+Util::numToHex(j*160)+"\n";
                out += "\t sta "+Util::numToHex(dst)+"+"+Util::numToHex(i*8)+"+"+ Util::numToHex(shift1) +"+"+Util::numToHex(j*320)+"\n";
            }
            else if (m1!=0){
                out +="\t lda "+Util::numToHex(src)+"+ "+Util::numToHex(i)+"+ "+ Util::numToHex(shift2) +"+"+Util::numToHex(j*160)+"\n";
                out+="\t and #"+Util::numToHex((uchar)m1)+"\n";
//                out+="\t ora #"+Util::numToHex((uchar)org1)+"\n";
                out += "\t sta "+Util::numToHex(dst)+"+"+Util::numToHex(i*8)+"+"+ Util::numToHex(shift1) +"+"+Util::numToHex(j*320)+"\n";

            }
            if (org2==0) {
                out += "\t lda "+Util::numToHex(src)+"+"+Util::numToHex(i)+" +"+ Util::numToHex(shift3) +" +"+Util::numToHex(j*160)+"\n";
                out += "\t sta "+Util::numToHex(dst)+"+"+Util::numToHex(i*8)+" +"+ Util::numToHex(shift1+4) +" +"+Util::numToHex(j*320)+"\n";
            }
            else  if (m2!=0)
            {
                out += "\t lda "+Util::numToHex(src)+"+"+Util::numToHex(i)+" +"+ Util::numToHex(shift3) +" +"+Util::numToHex(j*160)+"\n";
                out+="\t and #"+Util::numToHex((uchar)m2)+"\n";
  //              out+="\t ora #"+Util::numToHex((uchar)org2)+"\n";
                out += "\t sta "+Util::numToHex(dst)+"+"+Util::numToHex(i*8)+" +"+ Util::numToHex(shift1+4) +" +"+Util::numToHex(j*320)+"\n";

            }

/*                   sta @SDST+[i*8] +[j*320]


                   lda $8000+[i]+80+[j*160]
                   sta @SDST+4+[i*8]+[j*320]
*/
        }
    }

    out+="\t\");\n";
    out+="end;";
    Util::SaveTextFile(outFile,out);

}

/*
void Compression::FrameConverter(QString dir, QString outFile, QVector<int> cols) {

}
*/
