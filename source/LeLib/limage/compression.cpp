#include "compression.h"

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


void Compression::AddBitplaneToData(QByteArray &data, MultiColorImage &img, int xp, int yp, int w, int h, int bpl)
{
    int d= 0;
    for (int y=0;y<h;y+=1)
        for (int bp=0; bp<bpl;bp++) {
            int curBit = pow(2,bp);
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
    qDebug() << "AddBitplaneToData size " <<d << w << h;

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

void Compression::OptimizeScreenAndCharset(QVector<int> &screen, QByteArray &charset, QVector<int> &sOut, QByteArray &cOut, int sw, int sh, int charSize, int compression)
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
    qDebug() << sw << sh << charSize << screens << charset.count()/8;
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
       // qDebug() << "Current screen : " << i ;

        for (int x=0;x<sw*sh;x++) {
            int s = screen[i*sw*sh + x] + charSize*i;


//            qDebug() << "   Current s: " << QString::number(s) ;
            int found = -1;
            int curMin = 8000;
            for (int j=0;j<cOut.count()/8;j++) {
                //int Compression::Compare(QByteArray &a, QByteArray &b, int p1, int p2, int length)
                int res = Compare(cOut, charset,j*8,s*8,8);
                if (res<compression && res<curMin) {
                    found = j;
                    curMin = res;
    //                qDebug() << "Found similar: " << found;
//                    break;
                }
            }
            if (found ==-1) {
                found = cOut.size()/8;
                for (int j=0;j<8;j++)
                    cOut.push_back( charset[s*8+j]  );
      //          qDebug() << "Added new : " << found;
            }
//            qDebug() << "  ADDING : " << QString::number(found) << " vs "  << QString::number(s);

            sOut.append((uchar)found);
        }

    }
    qDebug() << "Final # chars: " << cOut.size()/8;
    qDebug() << "Final # screens: " << sOut.size()/sw/sh;
    qDebug() << "Final # res: " << sum;

}


void Compression::OptimizeScreenAndCharsetGB(QVector<int> &screen, QByteArray &charset, QVector<int> &sOut, QByteArray &cOut, int sw, int sh, int charSize, int compression)
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
    qDebug() << sw << sh << charSize << screens << charset.count()/16;
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
            int curMin = 80000;
            for (int j=0;j<cOut.count()/16;j++) {
                //int Compression::Compare(QByteArray &a, QByteArray &b, int p1, int p2, int length)
                int res = Compare(cOut, charset,j*16,s*16,16);
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


void Compression::SaveCompressedSpriteData(QByteArray &data, QString dataFile, QString tableFile, int address, int compressionLevel)
{
    int noSprites = data.count()/64;
    QByteArray dataOut, tableOut;
    int noSpritesOut = 0;
    int compressed  = 0;
    qDebug() << data.count();
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

int Compression::Compare(QByteArray &a, QByteArray &b, int p1, int p2, int length)
{
    int l = 0;
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


  //      if (a[p1+i]!=b[p2+i]) l++;
    }
    return l;
}

void Compression::OptimizeAndPackCharsetData(QByteArray &dataIn, QByteArray &out, QByteArray &table, int width, int compression, bool invertTable)
{
    out.clear();
    table.clear();

    int cnt = dataIn.count()/width;
    qDebug() << "Width : " << width;
    qDebug() << "Data count : " <<dataIn.count();
    qDebug() << "total rows: " << cnt;
    qDebug() << "total chars x frames: " << (cnt/width);
    qDebug() << "total chars: " << (cnt/32)/width;

    qDebug() << "cnt SHOULD be " << (32*width*6*21);


    for (int i=0;i<cnt;i++) {
        unsigned short currentPointer = 0;
        bool isNew = true;
        for (int j=0;j<out.count()/width;j++) {
            int metric = Compare(dataIn, out, i*width, j*width, width);
            if (metric<compression) {
                currentPointer =j*width;
                isNew=false;
                //break;
            }
        }
        if (isNew) {
            currentPointer = out.count();
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
    int count = inData.count()/1000;
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


