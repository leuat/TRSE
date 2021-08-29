#include "limagevz200.h"


void LImageVZ200::ExportBin(QFile &file)
{
    QByteArray data;

    for (int y=0;y<m_height;y++) {
        for (int x=0;x<m_width;x+=4) {
            uchar c = 0;
            for (int i=0;i<4;i++) {
                uchar v = getPixel(x+i,y);
                c=c|(v<<(6-2*i));
            }
            data.append(c);
        }
    }
    file.write(data);




}
