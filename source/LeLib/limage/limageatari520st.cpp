#include "limageatari520st.h"

#define CHECK_BIT(var,pos) (((var) & (1<<(pos)))>>pos)


LImageAtari520ST::LImageAtari520ST(LColorList::Type t, int type)  : LImageAmiga(t,type)
{
    /*    if (type==0) {
        m_height = 256;
        Initialize(320,256);
        m_type = LImage::Type::AMIGA320x200;
    }
    if (type==1) {
        m_height = 256;
        Initialize(320,256);
        m_type = LImage::Type::AMIGA320x256;
    }*/
    m_type = LImage::Type::ATARI320x200;
    Initialize(320,200);
    m_scale = 1;
    m_colorList.setNoBitplanes(4);
    m_supports.asmExport = false;
    m_supports.binaryLoad = false;
    m_supports.binarySave = true;
    m_supports.flfSave = true;
    m_supports.flfLoad = true;

}

void LImageAtari520ST::InitPens() {
    m_colorList.m_bpp = QVector3D(3,3,3);
    m_colorList.DefaultPen(LPen::SingleSelect);
}

void LImageAtari520ST::ExportBin(QFile &file)
{
    m_height  = 200;

    int nobp = m_colorList.getNoBitplanes();
    //    qDebug() << nobp;
    QByteArray data;
//    data.resize(320*200/2); // 4 bitplanes
  //  data.fill(0);


    int curBit = 0;
    int idx = 0;
    QVector<int> is;
    is.resize(4);
    is.fill(0);
    for (int y=0;y<m_height;y++) {
        for (int x=0;x<320;x++) {

            char val = m_qImage->pixel(x,y);

            for (int i=0;i<nobp;i++) {
                int bit = CHECK_BIT(val,i);
                //if (val!=0)
                    //                qDebug() << QString::number(bit) << " from : " << QString::number(val) << "," <<QString::number(i); ;
                is[i] = is[i] | (bit<<(15-curBit));
                //data[4*idx+i] = data[i][idx] | (bit<<(7-curBit));
             }
            curBit++;
            if (curBit==16) {
                curBit=0;
                idx++;
                for (int i=0;i<nobp;i++) {
                    data.append((is[i]>>8)&0xFF);
                    data.append(is[i]&0xFF);
                }
                is.fill(0);
                // Append to data
            }
            //            qDebug() << QString::number(idx);

        }
    }

  //  QByteArray cData;
/*    for (int y=0;y<m_height;y++) {
        for (int i=0;i<nobp;i++)
            for (int j=0;j<40;j++)
                cData.append(data[i][j+y*40]);

    }
    */
    file.write(data);
    //    for (int i=0;i<nobp;i++)
    //      file.write(data[i]);
//    qDebug() << "Written AMIGA file";
    int i=0;
/*    for (auto c : m_colorList.m_list) {
        int val = c.get12BitValue();
        val = val | (0x01800000+i*0x00020000);
        QString out = "move.l #"+Util::numToHex(val) + ",(a6)+";
        //        qDebug().noquote() << out	;

        i=i+1;
    }
*/
    QString ff = file.fileName();
    ff = ff.remove(".bin");
    m_colorList.ExportAtariSTPalette(ff+".pal");
}

void LImageAtari520ST::SaveBin(QFile &file)
{
    m_height  = 200;
    LImageQImage::SaveBin(file);
    QByteArray data;
    m_colorList.toArray(data);
    file.write(data);
}

void LImageAtari520ST::LoadBin(QFile &file)
{
    m_height  = 200;
    LImageQImage::LoadBin(file);
    uchar size;

    file.read((char*)&size,1);

    if (!file.atEnd()) {
        QByteArray d = file.read(size*3);
        d.insert(0,size);
        m_colorList.fromArray(d);
    }
}





QStringList LImageAtari520ST::SpriteCompiler(QString name, QString currentDir, QString src, QString dst, int xp, int yp, int w, int h, QString pparam)
{
    QString sprf = currentDir + name;

    QString p = pparam;

    dst = p+"_dst";
    QString xx = p+"_x";
    QString yy = p+"_y";
    QString s = "";

    for (int i=0;i<16;i++) {
        PrintShift(p + "_"+QString::number(i),s,xp,yp,w,h,15-i);
    }

    s+= "procedure "+p+"("+dst+":^byte, "+xx+","+yy+":integer);\n";
    s+="begin\n";
    s+="\t"+dst+"+=toPointer("+yy+"*160);\n";
    s+="\t"+dst+"+=toPointer(("+xx+">>1)&$FFF8);\n";
    s+="\tasm(\" move.l "+dst+",a0\");\n";
    s+="\t"+xx+"&=15;\n";
//    s+="\t"+xx+":=1;\n";
    for (int i=0;i<16;i++) {
        s+="\tif ("+xx+"="+QString::number(i)+") then "+p+"_"+QString::number(i)+"();\n";
    }
/*    s+="\tasm(\"\n";
    //    qDebug() << nobp;
    QByteArray data;
    //    data.resize(320*200/2); // 4 bitplanes
    //  data.fill(0);


    int curBit = 0;
    int idx = 0;
    QVector<int> is;
    is.resize(4);
    is.fill(0);

/*    s+="\tmove.l "+dst+",a0\n";
    s+="\tmoveq #0,d0\n";
    s+="\tmoveq #0,d1\n";
    s+="\tmove.w "+yy+",d0\n";
    s+="\tmulu.w #160,d0\n";
//    s+="\tadd.w "+xx+",d0\n";
    s+="\tmove.w "+xx+",d1\n";
    s+="\tlsr #1,d1\n";
    s+="\tand #$FFF8,d1\n";
    s+="\tadd.l d0,a0\n";
    s+="\tadd.l d1,a0\n";
*/


  //  s+="\t\");\n";
    s+="end;\n";
    Util::SaveTextFile(sprf,s);
    return QStringList();
}

void LImageAtari520ST::PrintShift(QString name, QString &s, int xp, int yp, int w, int h, int shift)
{
    int nobp = m_colorList.getNoBitplanes();

    int curBit = 0;
    int idx = 0;
    QVector<int> is;
    is.resize(4);
    is.fill(0);
    s+= "procedure "+name+"();\n";
    s+="begin\n";
    s+="\tasm(\"\n";

    for (int y=0;y<h;y++) {
        for (int x=0;x<w*8;x++) {
            char val = m_qImage->pixel(x+xp*8+shift,y+yp);
            for (int i=0;i<nobp;i++) {
                int bit = CHECK_BIT(val,i);
                is[i] = is[i] | (bit<<(15-curBit));
                //data[4*idx+i] = data[i][idx] | (bit<<(7-curBit));
            }
            curBit++;
            if (curBit==16) {
                curBit=0;
                idx++;
//                int pos = y*160 + (x>>5)*16;
                int pos = y*160 + (x>>4)*8;

                for (int i=0;i<nobp;i++) {
                    //                    is[i] = x+y;
                    if (is[i]!=0) {
//                        qDebug() << pos+i*2;;
                        s+="\tor.w #"+QString::number(is[i])+","+QString::number(pos+i*2)+"(a0)\n";
                    }
                    //data.append((is[i]>>8)&0xFF);
                    //data.append(is[i]&0xFF);
                }
                is.fill(0);
                // Append to data
            }
            //            qDebug() << QString::number(idx);

        }
    }
    s+="\t\");\n";
    s+="end;\n";

}
