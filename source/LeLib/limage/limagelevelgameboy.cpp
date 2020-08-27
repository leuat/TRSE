#include "limagelevelgameboy.h"

void LImageLevelGameboy::ExportBin(QFile &file)
{


    file.write(m_meta.toHeader());
    if (m_levels.count()==0)
        return;

    for (CharmapLevel* l : m_levels) {
        file.write( l->m_CharData);

        if (l->m_ExtraData.count()!=0)
            file.write( l->m_ExtraData);

    }

}

void LImageLevelGameboy::SetPalette(int pal)
{
    LImageLevelNES::SetPalette(0);
    //    m_colorList.setPen(2-1, m_colorList.)
    //    if (pal==m_oldPal)
    //      return;
    /*
        if (m_charset!=nullptr) {
            m_charset->SetPalette(pal);
            return;
        }

         m_colorList.setPen(2-1,m_colorList.m_nesPPU[pal*4 +1 +0]);
         m_colorList.setPen(2-0,m_colorList.m_nesPPU[pal*4 +1 +1]);
         m_colorList.setPen(2-2,m_colorList.m_nesPPU[pal*4 +1 +2]);
    //     m_colorList.setPen(3,m_colorList.m_nesPPU[pal*4 +1 +3]);
         m_colorList.setPen(3,m_colorList.m_nesPPU[0]);

    */
}

void LImageLevelGameboy::InitPens()
{
    m_colorList.DefaultPen(LPen::FixedSingleNumbers,1);
    m_colorList.setPen(0,0);
    Data::data.currentColor = 0;
}
