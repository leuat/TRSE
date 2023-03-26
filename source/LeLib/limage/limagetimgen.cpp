#include "limagetimgen.h"


LImageTIMGen::LImageTIMGen(LColorList::Type t)  : LImageTIM(t)
{
    m_metaParams.append(new MetaParameter("screen_width","Screen width",256,2,2048));
    m_metaParams.append(new MetaParameter("screen_height","Screen height",128,2,2048));
    m_type = TIMG;

}


void LImageTIMGen::LoadBin(QFile &file)
{
    file.read((char*)&m_width,sizeof(int));
    file.read((char*)&m_height,sizeof(int));


    Initialize(m_width,m_height);
    m_charWidth=m_width/8;
    m_charHeight=m_height/8;

    LImageQImage::LoadBin(file);

}

void LImageTIMGen::SaveBin(QFile &file)
{
    file.write((char*)&m_width,sizeof(int));
    file.write((char*)&m_height,sizeof(int));

    LImageQImage::SaveBin(file);
}

void LImageTIMGen::ReInitialize()
{
    m_width = getMetaParameter("screen_width")->value;
    m_height = getMetaParameter("screen_height")->value;
    m_qImage = nullptr;
    m_charWidth=m_width/8;
    m_charHeight=m_height/8;

    Initialize(m_width,m_height);
    m_charWidth=m_width/8;
    m_charHeight=m_height/8;

}
