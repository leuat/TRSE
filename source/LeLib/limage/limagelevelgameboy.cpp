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
