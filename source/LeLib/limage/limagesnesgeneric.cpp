#include "limagesnesgeneric.h"


LImageSnesGeneric::LImageSnesGeneric(LColorList::Type t) : LImageSNES(t)
{
    m_type = LImage::Type::SNESGeneric;

    //m_metaParams.append(new MetaParameter("bitplanes","Bitplane data: 2 (4 colors), 3 (8 colors) or 4 (16 colors). For sprites, select 4 bitplanes (16 colors)",2,2,4));
    m_metaParams.append(new MetaParameter("sizex","Width in 8 pixel chunks, ie 16 = 256 pixels, 32=512",16,1,64));
    m_metaParams.append(new MetaParameter("sizey","Height in 8 pixel chunks, ie 16 = 256 pixels, 32=512",16,1,64));


 //   InitPens();

    //ForceColor();
}

void LImageSnesGeneric::SaveBin(QFile &file)
{
    ushort width = m_width/8;
    ushort height = m_height/8;
    file.write((char*)&width, 2);
    file.write((char*)&height, 2);
    LImageSNES::SaveBin(file);

}

void LImageSnesGeneric::LoadBin(QFile &file)
{
    ushort width;
    ushort height;
    file.read((char*)&width, 2);
    file.read((char*)&height, 2);
    Initialize(width*8,height*8);

    m_charWidthDisplay=width;
    m_charHeightDisplay=height;
    m_gridWidthDisplay = width;


    LImageSNES::LoadBin(file);

}

void LImageSnesGeneric::ReInitialize()
{
    m_colorList.setNoBitplanes(getMetaParameter("bitplanes")->value);

//    Initialize(getMetaParameter("sizex")->value*8,getMetaParameter("sizey")->value*8);
    Initialize(getMetaParameter("sizex")->value*8,getMetaParameter("sizey")->value*8);

    m_charWidthDisplay=m_width/8;
    m_charHeightDisplay=m_height/8;
    m_gridWidthDisplay = m_width/8;

}
