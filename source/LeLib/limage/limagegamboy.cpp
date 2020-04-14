#include "limagegamboy.h"

LImageGamboy::LImageGamboy(LColorList::Type t) : LImageNES(t)
{
    m_type = LImage::GAMEBOY;
}

void LImageGamboy::ExportBin(QFile &file)
{
    QByteArray data;
    int j=0;
//    qDebug() << m_charWidth;
    for (int i=0;i<(m_charWidth/2)*m_charHeight;i++) {

        for (int k=0;k<8;k++) {
            data.append( PixelChar::reverse(m_data[i*2].p[k]));
            data.append( PixelChar::reverse(m_data[i*2+1].p[k]));
        }
        //        data.append( m_data[i].p[k]);
    }
    file.write(data);


/*    QString f = file.fileName();

    QString of = f.split(".")[0] + ".pal";
    if (QFile::exists(of))
        QFile::remove(of);
    QFile wf(of);
    wf.open(QFile::WriteOnly);
    QByteArray ba = m_colorList.m_nesPPU;

    for (int i=0;i<8;i++) {
        ba[i*4+0] = m_colorList.m_nesPPU[0];
        ba[i*4+1] = m_colorList.m_nesPPU[i*4+2];
        ba[i*4+2] = m_colorList.m_nesPPU[i*4+1];
        ba[i*4+3] = m_colorList.m_nesPPU[i*4+3];
    }

    wf.write(ba);
    wf.close();

    QByteArray d2;
    d2.append(data);
    d2.append(ba);
    Util::SaveByteArray(d2,f.split(".")[0] + ".chr");
   */
}
