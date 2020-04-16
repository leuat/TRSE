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

    int start = m_exportParams["Start"];
    int end = m_exportParams["End"];
    int size = end-start;


    for (int i=0;i<min((m_charWidth/2)*m_charHeight,size);i++) {
//        for (int i=0;i<(m_charWidth/2)*m_charHeight;i++) {

        for (int k=0;k<8;k++) {
            data.append( PixelChar::reverse(m_data[i*2].p[k]));
            data.append( PixelChar::reverse(m_data[i*2+1].p[k]));
        }
        //        data.append( m_data[i].p[k]);
    }
    file.write(data);


}

void LImageGamboy::SpritePacker(LImage *in, QByteArray &sprData, int x, int y, int w, int h, int compression) {

    int cur = 1;
    while (!(m_data[cur*2].isEmpty() && m_data[cur*2+1].isEmpty())) { //Find first non-empty sprite
        cur++;
    }
//    qDebug() << "FIRST START "<<cur;
  //  qDebug()<< m_data[cur*2].isEmpty() <<m_data[cur*2+2].isEmpty();
    LImageGamboy* lga = (LImageGamboy*)in;
    for (int j=0;j<h;j++)
        for (int i=0;i<w;i++) {
            int ii =(x+i+(j+y)*16);
            PixelChar& orgA = lga->m_data[ii*2];
            PixelChar& orgB = lga->m_data[ii*2+1];
            int idx = SearchForIdenticalPixelChar(orgA, orgB,compression);
//            qDebug() << "Testing if equal : "<<i<<j;
            if (idx==-1) {
                idx = ii;
                PixelChar& orgA = lga->m_data[idx*2];
                PixelChar& orgB = lga->m_data[idx*2+1];
                m_data[cur*2] = orgA;
                m_data[cur*2+1] = orgB;
                sprData.append(cur);
            //    qDebug() << "WRITING TO " << cur << orgA.isEmpty() << orgB.isEmpty();
                cur++;

  //              qDebug() << "FOUND NEW TILE";
            }
            else {
              //  qDebug() << "FOUND EQUAL TILE" <<idx;
                sprData.append(idx);
            }
        }

}

int LImageGamboy::SearchForIdenticalPixelChar(PixelChar o1, PixelChar o2, int compare) {
    for (int i=0;i<m_charWidth*m_charHeight;i++) {
        int j=i*2;
        int c1 = m_data[j].CompareLength(o1);
        int c2 = m_data[j+1].CompareLength(o2);
//        if (m_data[j].isEqualBytes(o1) && m_data[j+1].isEqualBytes(o2))
        if (c1+c2<=compare)
            return i;
    }

    return -1;
}

void LImageGamboy::ImportBin(QFile &file)
{
    QByteArray data = file.readAll();
    int j=0;
    for (int i=0;i<m_charWidth*m_charHeight/2;i++) {
        for (int k=0;k<8;k++) {
            m_data[i*2].p[k] = PixelChar::reverse(data[j]);
            j++;
            m_data[i*2+1].p[k] = PixelChar::reverse(data[j]);
            j++;
        }
    }
    ForceColor();
}
