#include "limagecga.h"

LImageCGA::LImageCGA(LColorList::Type t)  : LImageQImage(t)
{
    Initialize(320,200);
    m_scale = 1;
    m_type = LImage::Type::CGA;
    m_supports.asmExport = false;
    m_supports.binaryLoad = false;
    m_supports.binarySave = true;
    m_supports.flfSave = true;
    m_supports.flfLoad = true;

}

void LImageCGA::ExportBin(QFile &file)
{
    QByteArray even, odd;
    int cur=0;
    char curC=0;
    for (int y=0;y<200;y++) {
        for (int x=0;x<320;x++) {
           // QColor c = QColor(m_qImage->pixel(x,y));
//            qDebug() << m_qImage->pixel(x,y);
            char idx = m_qImage->pixel(x,y);//m_colorList.getIndex(c);
  //          qDebug() << QString::number(cur) << c;
            cur = cur | (idx<<(6-(curC*2)));
            curC++;
            if (curC==4) {
                if ((y&1)==0)
                    even.append(cur);
                else odd.append(cur);
                curC=0;
                cur=0;
            }
        }
    }
    for (int i=0;i<192;i++) even.append((char)0);
    file.write(even);

    file.write(odd);
    qDebug() << "Written CGA file";
}
