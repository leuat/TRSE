#include "limagecga160x100.h"


LImageCGA160x100::LImageCGA160x100(LColorList::Type t)  : LImageQImage(t)
{
    m_scale = 1;
    m_type = LImage::Type::CGA160x100;
    m_supports.asmExport = false;
    m_supports.binaryLoad = false;
    m_supports.binarySave = false;
    m_supports.flfSave = true;
    m_supports.flfLoad = true;
    m_supports.asmExport = false;
    m_currentChar = 0;
    m_metaParams.append(new MetaParameter("screen_width","Screen width",160,2,2048));
    m_metaParams.append(new MetaParameter("screen_height","Screen height",100,2,2048));
    //    Initialize(160,100);

    m_supports.displayCharOperations = true;
    m_updateCharsetPosition = true;
    m_colorList.m_isCharset = true;
    //    Initialize(160,100);
    m_currentChar = 0;

    m_GUIParams[tabCharset] = "Charset";
    m_GUIParams[tabData] = "";
    m_GUIParams[tabLevels] = "";
    m_GUIParams[tabEffects] = "";

    m_GUIParams[btnEditFullCharset] = "Char";
    m_supports.displayCharOperations = true;

    m_GUIParams[tabCharset] = "1";
    m_updateCharsetPosition = true;
    m_colorList.m_isCharset = true;

    m_charWidth=64;
    m_charHeight=32;
    m_updateCharsetPosition = true;

    m_supports.displayCharOperations = true;

}

void LImageCGA160x100::ExportBin(QFile &file)
{
    QByteArray d;
    int type = m_exportParams["export1"];
    if (type==0)
        for (int y=0;y<m_height;y++) {
            for (int x=0;x<m_width/2;x++) {
                int d1 = getPixel(x*2,y);
                int d2 = getPixel(x*2+1,y);
                d.append(d2|(d1<<4));
            }
        }
    if (type==1)
        for (int y=0;y<m_height;y++) {
            for (int x=0;x<m_width;x++) {
                int d1 = getPixel(x,y);
                d.append(d1);
            }
        }


    file.write(d);
}

void LImageCGA160x100::LoadBin(QFile &file)
{
    file.read((char*)&m_width,sizeof(int));
    file.read((char*)&m_height,sizeof(int));

    Initialize(m_width,m_height);
    m_charWidth=m_width/8;
    m_charHeight=m_height/8;

    LImageQImage::LoadBin(file);

}

void LImageCGA160x100::SaveBin(QFile &file)
{
    file.write((char*)&m_width,sizeof(int));
    file.write((char*)&m_height,sizeof(int));

    LImageQImage::SaveBin(file);
}

void LImageCGA160x100::ReInitialize()
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
