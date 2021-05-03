#include "limagespectrum.h"

LImageSpectrum::LImageSpectrum(LColorList::Type t) : LImageQImage(t)
{
    m_width = 256;
    m_height = 192;
    m_scaleX = 1.0f;
    m_noColors = 8;
    m_scale = 1;
    m_type = LImage::Type::Spectrum;
    m_colorList.m_supportsFooterPen = true;

    m_supports.koalaExport = false;
    m_supports.koalaImport = false;
    m_supports.asmExport = false;
    m_supports.binaryLoad = true;
    m_supports.binarySave = true;
    m_supports.flfSave = true;
    m_supports.flfLoad = true;
    m_supports.asmExport = false;
    m_colorList.m_selectClosestFromPen = false;
/*    m_GUIParams[col2] = "";
    m_GUIParams[col3] = "Multicolor 1";
    m_GUIParams[col4] = "Multicolor 2";
  */
    m_currentChar=0;
    //m_currentMode=Mode::FULL_IMAGE;
    m_exportParams.clear();
/*    m_exportParams["Start"] = 0;
    m_exportParams["End"] = 256;
    m_exportParams["IncludeColors"] = 0;
    m_exportParams["VIC20mode"] = 0;
  */
    m_supports.displayCharOperations = true;

    m_colorList.InitSPECTRUM();
    //    InitPens();

}

void LImageSpectrum::setPixel(int x, int y, unsigned int color)
{
    if (m_qImage==nullptr)
        return;

    if (x>=0 && x<m_qImage->width() && y>=0 && y<m_qImage->height()) {
        m_qImage->setPixel(x,y,QRgb(color));
        MaintainSpectrumAttribute(x,y);
    }


}

void LImageSpectrum::InitPens()
{
    m_colorList.setPen(1,Data::data.currentColor);
    m_colorList.InitSPECTRUM();
}

void LImageSpectrum::MaintainSpectrumAttribute(int x, int y)
{
    int ix = x-(x&7);
    int iy = y-(y&7);
    int bg = m_colorList.getPen(0);
    int fg = m_colorList.getPen(1);
    bg &= 0b11110111;
    bg |=((fg) & 0b1000);
 //   if (rand()%100>98)
   //     qDebug() << bg << fg;
    for (int j=0;j<8;j++)
        for (int i=0;i<8;i++) {
            int c = m_qImage->pixel(i+ix,j+iy);

            if (!(c==bg || c==fg))
                m_qImage->setPixel(ix+i,iy+j,QRgb(bg));
        }
}
