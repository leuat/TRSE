#include "limagex16.h"


LImageX16::LImageX16(LColorList::Type t)  : LImageOK64(t)
{
    Initialize(640,480);
    m_scale = 1;
    m_type = LImage::Type::X16_640x480;
    m_supports.asmExport = false;
    m_supports.binaryLoad = true;
    m_supports.binarySave = true;
    m_supports.flfSave = true;
    m_supports.flfLoad = true;
    m_supports.asmExport = false;

}

