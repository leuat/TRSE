#include "limageamstradgeneric.h"


LImageAmstradGeneric::LImageAmstradGeneric(LColorList::Type t)  : LImageAmstradCPC(t)
{
    m_type = LImage::Type::AmstradCPCGeneric;
    m_supports.asmExport = false;
    m_supports.binaryLoad = false   ;
    m_supports.binarySave = false;
    m_supports.flfSave = true;
    m_supports.flfLoad = true;
    m_colorList.m_supportsFooterPen = true;

    m_metaParams.append(new MetaParameter("mode","Mode",0,3,3));
    m_metaParams.append(new MetaParameter("screen_width","Screen width",160,2,1000));
    m_metaParams.append(new MetaParameter("screen_height","Screen height",200,2,1000));
    SetMode();

}

void LImageAmstradGeneric::SetMode()
{
    //m_footer.set(LImageFooter::POS_CHARSET_,getMetaParameter("screen_height")->value);
//    m_qImage = nullptr;
    Initialize(m_width,m_height);
    if (m_mode == 0) {
        m_colors = 16;
        m_bpp = 4;
        m_scaleX = 2;
    }
    if (m_mode == 1) {
        m_colors = 4;
        m_scaleX = 1;
        m_bpp = 2;
    }
    if (m_mode == 2) {
        m_colors = 2;
        m_bpp = 1;
        m_scaleX = 0.5;
    }

    InitPens();
}

void LImageAmstradGeneric::ReInitialize()
{
    m_width = getMetaParameter("screen_width")->value;
    m_height = getMetaParameter("screen_height")->value;
    m_mode = getMetaParameter("mode")->value;
    m_qImage = nullptr;
    SetMode();
}

void LImageAmstradGeneric::LoadBin(QFile &file)
{
    file.read((char*)&m_width,sizeof(int));
    file.read((char*)&m_height,sizeof(int));
    file.read((char*)&m_mode,sizeof(int));
    getMetaParameter("screen_width")->value = m_width;
    getMetaParameter("screen_height")->value = m_height;
    getMetaParameter("mode")->value = m_mode;
    ReInitialize();
    LImageAmstradCPC::LoadBin(file);
}

void LImageAmstradGeneric::SaveBin(QFile &file)
{
    file.write((char*)&m_width,sizeof(int));
    file.write((char*)&m_height,sizeof(int));
    file.write((char*)&m_mode,sizeof(int));
//    qDebug() << m_width << m_height << m_qImage->width() <<m_qImage->height();

    LImageAmstradCPC::SaveBin(file);

}
