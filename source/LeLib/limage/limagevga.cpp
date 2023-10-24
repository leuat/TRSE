#include "limagevga.h"

LImageVGA::LImageVGA(LColorList::Type t)  : LImageQImage(t)
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

    m_updateCharsetPosition = true;
    m_colorList.m_isCharset = true;
    m_colorList.m_supportsFooterPen = true;
    m_savePalette = true;

    m_supports.displayCharOperations = true;

    m_GUIParams[btnEditFullCharset] = "Full charset";

    m_GUIParams[tabCharset] = "1";

}

void LImageVGA::ExportBin(QFile &file)
{
    unsigned char *ddata = new unsigned char[m_width*m_height];
    for (int i=0;i<m_qImage->width();i++)
        for (int j=0;j<m_qImage->height();j++) {
            unsigned char val = getPixel(i,j);
            ddata[(i+j*m_qImage->width())] = val;
        }
    file.write((char*)ddata, m_width*m_height);
    delete[] ddata;

//    file.write(m_data);
    QString n = file.fileName().remove(".bin");
    QByteArray data;
    m_colorList.toArray(data);
    data.remove(0,1);
    Util::SaveByteArray(data,n+".pal");

}

