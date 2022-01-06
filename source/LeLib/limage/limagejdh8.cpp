#include "limagejdh8.h"

LImageJDH8::LImageJDH8(LColorList::Type t)  : LImageQImage(t){
    Initialize(256,240);
    m_scale = 1;
    m_type = LImage::Type::JDH8;
    m_supports.asmExport = false;
    m_supports.binaryLoad = false;
    m_supports.binarySave = true;
    m_supports.flfSave = true;
    m_supports.flfLoad = true;

    m_supports.displayMC1 = false;
    m_supports.displayMC2 = false;
    m_supports.displayForeground = false;
    m_supports.displayBackground = false;

}

void LImageJDH8::ExportBin(QFile &file)
{
    QString s = "";
    int x = m_width/8;
    for (int j=0;j<m_height;j++) {
        s+="@db ";
        for (int i=0;i<x;i++) {
            uchar c = 0;
            for (int k=0;k<8;k++) {
                auto p = getPixel(i*8+k,j);
                c|=p<<k;
            }
            s+="0x"+QString::number(c,16);
            if (i!=x-1)
                s+=", ";
        }
        s+="\n";
    }
    QString f = file.fileName();

    QString filenameBase = Util::getFileWithoutEnding(f);

    Util::SaveTextFile(filenameBase + ".asm",s);
}

/*void LImageJDH8::LoadBin(QFile &file)
{
    Initialize(256,240);
    qDebug() << "HERE";
    LImageQImage::LoadBin(file);

}
*/
