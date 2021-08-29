#ifndef LIMAGEVZ200_H
#define LIMAGEVZ200_H

#include "limageqimage.h"

class LImageVZ200 : public LImageQImage
{
public:
    LImageVZ200(LColorList::Type t)  : LImageQImage(t)
    {
        Initialize(128,64);
        m_scale = 1;
        m_type = LImage::VZ200;

        m_supports.asmExport = false;
        m_supports.binaryLoad = false;
        m_supports.binarySave = false;
        m_supports.flfSave = true;
        m_supports.flfLoad = true;
        m_supports.asmExport = false;
        m_currentChar = 0;

    }


    void ExportBin(QFile &file) override;


};

#endif // LIMAGEVZ200_H
