#include "limageamstradcpc.h"

LImageAmstradCPC::LImageAmstradCPC(LColorList::Type t)  : LImageQImage(t)
{
    SetMode(0);
    m_type = LImage::Type::AmstradCPC;
    m_supports.asmExport = false;
    m_supports.binaryLoad = false   ;
    m_supports.binarySave = false;
    m_supports.flfSave = true;
    m_supports.flfLoad = true;
    m_colorList.m_supportsFooterPen = true;

}

void LImageAmstradCPC::SetMode(int mode)
{
    m_footer.set(LImageFooter::POS_CURRENT_MODE,mode);
    if (mode == 0) {
        Initialize(160,200);
        m_colors = 16;
        m_bpp = 4;
    }
    if (mode == 1) {
        Initialize(320,200);
        m_colors = 4;
        m_bpp = 2;
    }
}
