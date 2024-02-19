#include "limageagon.h"

LImageAgon::LImageAgon(LColorList::Type t)  : LImageGeneric(t)
{
//    Initialize(320,200);
    m_scale = 1;
    m_type = LImage::Type::AGON;
    m_supports.asmExport = false;
    m_supports.binaryLoad = false;
    m_supports.binarySave = false;
    m_supports.flfSave = true;
    m_supports.flfLoad = true;
    m_supports.asmExport = false;
    m_currentChar = 0;
    m_metaParams.clear();
    m_metaParams.append(new MetaParameter("width","Width",320,2,2000));
    m_metaParams.append(new MetaParameter("height","Height",200,2,2000));
}
QString LImageAgon::getMetaInfo()
{
    m_width = getMetaParameter("width")->value;
    m_height = getMetaParameter("height")->value;

    return "Custom NxM RGBA image type.";
}


void LImageAgon::LoadBin(QFile &file)
{

    ushort width;
    ushort height;
    file.read((char*)&width, 2);
    file.read((char*)&height, 2);
    Initialize(width,height);
    LImageQImage::LoadBin(file);

}

void LImageAgon::SaveBin(QFile &file)
{
    ushort width = m_width;
    ushort height = m_height;
    file.write((char*)&width, 2);
    file.write((char*)&height, 2);
    LImageQImage::SaveBin(file);

}


void LImageAgon::ExportBin(QFile &file)
{
    ushort width = m_width;
    ushort height = m_height;
    ushort tmp = 0;
    tmp=23; file.write((char*)&tmp, 1);
    tmp=0; file.write((char*)&tmp, 1);
    tmp=192; file.write((char*)&tmp, 1);
    tmp=0; file.write((char*)&tmp, 1);

    tmp=23; file.write((char*)&tmp, 1);
    tmp=27; file.write((char*)&tmp, 1);
    tmp=0; file.write((char*)&tmp, 1);
    tmp=0; file.write((char*)&tmp, 1); // id

    tmp=23; file.write((char*)&tmp, 1); // id
    tmp=27; file.write((char*)&tmp, 1); // id
    tmp=1; file.write((char*)&tmp, 1); // id

    file.write((char*)&width, 2);
    file.write((char*)&height, 2);
    LImageQImage::SaveBinRGBA(file);
}
