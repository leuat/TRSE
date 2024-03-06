#include "limageprimo.h"

LImagePrimo::LImagePrimo(LColorList::Type t) : LImageQImage(t)
{
    m_width = 256;
    m_height = 192;
    m_scaleX = 1.0f;
    m_noColors = 2;
    m_scale = 1;
    m_type = LImage::Type::PRIMO;
    m_colorList.m_supportsFooterPen = true;
    Initialize(m_width,m_height);

    m_supports.koalaExport = false;
    m_supports.koalaImport = false;
    m_supports.asmExport = false;
    m_supports.binaryLoad = true;
    m_supports.binarySave = true;
    m_supports.flfSave = true;
    m_supports.flfLoad = true;
    m_supports.asmExport = false;
    m_colorList.m_selectClosestFromPen = false;
    m_currentChar=0;
    //m_currentMode=Mode::FULL_IMAGE;
    m_exportParams.clear();
    m_supports.displayCharOperations = true;
    m_GUIParams[tabCharset] = "Charset";
    m_GUIParams[tabData] = "";
    m_GUIParams[tabLevels] = "";
    m_GUIParams[tabEffects] = "";

    m_GUIParams[btnEditFullCharset] = "Char";

    m_metaParams.append(new MetaParameter("width","Width",256,2,2000));
    m_metaParams.append(new MetaParameter("height","Height",192,2,2000));
    m_colorList.InitMono();
    //    InitPens();

}
void LImagePrimo::InitPens()
{
    m_colorList.InitMono();
}


void LImagePrimo::ExportBin(QFile &ofile)
{
    QByteArray data;
    data.resize(32*192);
    QByteArray cols;


    for (int y=0;y<m_height;y++) {
        int pos = y*32;
        for (int x=0;x<m_width;x+=8) {

            uchar b = 0;
            for (int dx=0;dx<8;dx++) {
                uchar c = getPixel(x+dx,y);
                if (c==1) b|=1<<(7-dx);

            }
            data[pos + x/8]=b;

        }
    }

    ofile.write(data);
    // Take care of color data!

    ofile.close();

}

QString LImagePrimo::getMetaInfo()
{
    m_width = ((int)(getMetaParameter("width")->value)) &0b1111111111111000;
    m_height = getMetaParameter("height")->value;

    return "Custom NxM PRIMO image type. Width will be adjusted to nearest 8 pixles.";
}


void LImagePrimo::LoadBin(QFile &file)
{

    ushort width;
    ushort height;
    file.read((char*)&width, 2);
    file.read((char*)&height, 2);
    Initialize(width,height);
    LImageQImage::LoadBin(file);

}

void LImagePrimo::SaveBin(QFile &file)
{
    ushort width = m_width;
    ushort height = m_height;
    file.write((char*)&width, 2);
    file.write((char*)&height, 2);
    LImageQImage::SaveBin(file);

}
