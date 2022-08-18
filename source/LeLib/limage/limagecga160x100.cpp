#include "limagecga160x100.h"


LImageCGA160x100::LImageCGA160x100(LColorList::Type t)  : LImageQImage(t)
{
    Initialize(160,100);
    m_scale = 1;
    m_type = LImage::Type::CGA160x100;
    m_supports.asmExport = false;
    m_supports.binaryLoad = false;
    m_supports.binarySave = false;
    m_supports.flfSave = true;
    m_supports.flfLoad = true;
    m_supports.asmExport = false;
    m_currentChar = 0;

}

void LImageCGA160x100::ExportBin(QFile &file)
{
    QByteArray d;
    for (int y=0;y<m_height;y++) {
        for (int x=0;x<m_width/2;x++) {
            int d1 = getPixel(x*2,y);
            int d2 = getPixel(x*2+1,y);
            d.append(d2|(d1<<4));
    }
    }
    file.write(d);
}
