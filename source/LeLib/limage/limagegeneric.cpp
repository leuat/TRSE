#include "limagegeneric.h"

LImageGeneric::LImageGeneric(LColorList::Type t)  : LImageQImage(t)
{
//    Initialize(320,200);
    m_scale = 1;
    m_type = LImage::Type::LImageGeneric;
    m_supports.asmExport = false;
    m_supports.binaryLoad = false;
    m_supports.binarySave = false;
    m_supports.flfSave = true;
    m_supports.flfLoad = true;
    m_supports.asmExport = false;
    m_currentChar = 0;
    m_metaParams.append(new MetaParameter("width","Width",320,2,2000));
    m_metaParams.append(new MetaParameter("height","Height",200,2,2000));
}

void LImageGeneric::LoadBin(QFile &file)
{

    ushort width;
    ushort height;
    file.read((char*)&width, 2);
    file.read((char*)&height, 2);
    Initialize(width,height);
    LImageQImage::LoadBin(file);

}

void LImageGeneric::SaveBin(QFile &file)
{
    ushort width = m_width;
    ushort height = m_height;
    file.write((char*)&width, 2);
    file.write((char*)&height, 2);
    LImageQImage::SaveBin(file);

}

QString LImageGeneric::getMetaInfo()
{
    m_width = getMetaParameter("width")->value;
    m_height = getMetaParameter("height")->value;

    return "Custom NxM indexed image type.";
}


