#include "limageamiga4.h"

#define CHECK_BIT(var,pos) (((var) & (1<<(pos)))>>pos)

LImageAmiga::LImageAmiga(LColorList::Type t, int type)  : LImageQImage(t)
{
    if (type==0) {
        m_height = 200;
        Initialize(320,200);
        m_type = LImage::Type::AMIGA320x200;
    }
    if (type==1) {
        m_height = 256;
        Initialize(320,256);
        m_type = LImage::Type::AMIGA320x256;
    }
    m_scale = 1;
    m_supports.asmExport = false;
    m_supports.binaryLoad = false;
    m_supports.binarySave = true;
    m_supports.flfSave = true;
    m_supports.flfLoad = true;

}

void LImageAmiga::ExportBin(QFile &file)
{
    int nobp = m_colorList.getNoBitplanes();
//    qDebug() << nobp;
    QVector<QByteArray> data;
    data.resize(nobp);
    for (int i=0;i<nobp;i++) {
        data[i].resize(m_height*40);
        data[i].fill(0);
    }



    int curBit = 0;
    int idx = 0;
    for (int y=0;y<m_height;y++) {
        for (int x=0;x<320;x++) {

            char val = m_qImage->pixel(x,y);

            for (int i=0;i<nobp;i++) {
                int bit = CHECK_BIT(val,i);
                if (val!=0)
//                qDebug() << QString::number(bit) << " from : " << QString::number(val) << "," <<QString::number(i); ;
                data[i][idx] = data[i][idx] | (bit<<(7-curBit));
             }
            curBit++;
            if (curBit==8) {
                curBit=0;
                idx++;
            }
//            qDebug() << QString::number(idx);

        }
    }

    QByteArray cData;
    for (int y=0;y<m_height;y++) {
        for (int i=0;i<nobp;i++)
            for (int j=0;j<40;j++)
                cData.append(data[i][j+y*40]);

    }
    file.write(cData);
//    for (int i=0;i<nobp;i++)
  //      file.write(data[i]);
    qDebug() << "Written AMIGA file";
    int i=0;
    for (auto c : m_colorList.m_list) {
        int val = c.get12BitValue();
        val = val | (0x01800000+i*0x00020000);
        QString out = "move.l #"+Util::numToHex(val) + ",(a6)+";
        qDebug().noquote() << out	;

        i=i+1;
    }

}

void LImageAmiga::SaveBin(QFile &file)
{
    LImageQImage::SaveBin(file);
    file.write(m_colorList.toArray());
}

void LImageAmiga::LoadBin(QFile &file)
{
    LImageQImage::LoadBin(file);
    char size;
    file.read((char*)&size,1);
    if (!file.atEnd()) {
        QByteArray d = file.read(size*3);
        d.insert(0,size);
        m_colorList.fromArray(d);
    }
}

