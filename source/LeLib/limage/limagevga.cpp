#include "limagevga.h"

LImageVGA::LImageVGA(LColorList::Type t)  : LImageOK64(t)
{
    Initialize(320,200);
    m_scale = 1;
    m_colorList.m_type = t;
    m_supports.asmExport = false;
    m_supports.binaryLoad = false;
    m_supports.binarySave = true;
    m_supports.flfSave = true;
    m_supports.flfLoad = true;

    m_supports.displayMC1 = false;
    m_supports.displayMC2 = false;
    m_supports.displayForeground = false;
    m_supports.displayBackground = false;
    m_width = 320;
    m_height = 200;
    m_scaleX = 1;
    m_scale = 1;
    m_type = LImage::Type::VGA;

}

