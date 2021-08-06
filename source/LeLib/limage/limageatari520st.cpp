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
