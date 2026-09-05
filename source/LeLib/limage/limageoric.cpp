#include "limageoric.h"
/*
QVector<int> LImageOric::table160 =  {0b00000000, 0b00000001,0b00000100,0b00000101,
                          0b00010000, 0b00010001,0b00010100,0b00010101,
                          0b01000000, 0b01000001,0b01000100,0b01000101,
                          0b01010000, 0b01010001,0b01010100,0b01010101};
*/
LImageOric::LImageOric(LColorList::Type t)  : LImageQImage(t)
{
    m_type = LImage::Type::ORIC;
    m_supports.asmExport = false;
    m_supports.binaryLoad = false   ;
    m_supports.binarySave = false;
    m_supports.flfSave = true;
    m_supports.flfLoad = true;
    m_colorList.m_supportsFooterPen = true;
    usePens = true;

/*    m_metaParams.append(new MetaParameter("mode","Mode",0,3,3));
    m_metaParams.append(new MetaParameter("screen_width","Screen width",160,2,1000));
    m_metaParams.append(new MetaParameter("screen_height","Screen height",200,2,1000));*/
    SetMode();

}

void LImageOric::SetMode()
{
    Initialize(240,200);
    InitPens();
}

void LImageOric::InitPens()
{
}

void LImageOric::ToQImage(LColorList &lst, QImage &img, double zoom, QPointF center)
{
    ToQImageUsingPens(lst, img, zoom, center);
}

void LImageOric::ExportBin(QFile &ofile)
{

    QString f = ofile.fileName();

    QString filenameBase = Util::getFileWithoutEnding(f);

    QString fColor = filenameBase + "_palette.bin";

    if (QFile::exists(fColor))
        QFile::remove(fColor);

    QByteArray palette,data;

    QVector<int> lst = m_colorList.getPenList();
    for (auto i : lst)
        palette.append(((unsigned char)i));
//    qDebug() << lst;

    Util::SaveByteArray(palette,fColor);

    int y = 0;
    int dy = 0;
    int xw;
/*    if (m_width==320)  xw=80;
    if (m_width==160)  xw=80;
    if (m_width==256)  xw=64;
*/
    // mono
    if (m_colorList.m_type==LColorList::ORIC)
    for (int y=0;y<m_height;y++) {
        char c = 0;
        int curBit = 0;
        for (int x=0;x<m_width;x++) {
            //int pixel = ((dy+y)/10)&15;
            int pixel = getPixel(x,y);
//            if (rand()%100>98) qDebug() << pixel;
            c |= (pixel<<(5-curBit));
            curBit+=1;
            if (curBit==6) {
                data.append(c+64);
                curBit=0;
                c=0;
            }
        }

    }

    if (m_colorList.m_type==LColorList::ORIC8)
        for (int y=0;y<m_height;y++) {
            char c = 0;
            int curBit = 0;
            int curColor = 7;
            for (int x=0;x<m_width;x+=6) {
                //int pixel = ((dy+y)/10)&15;
                int dom = curColor;
                QVector<int> w;
                w.resize(8);
                w.fill(0);
                c=0;
                // get next step
                if (x+6<m_width)
                for (int i=0;i<6;i++) {
                    int pixel = getPixel(x+i+6,y);
                    w[pixel]++;

                }
                int cnt =0;
                for (int i=0;i<6;i++) {
                    int pixel = getPixel(x+i,y);
                    if (pixel!=0) {
                        c|= 1<<(5-i);
                        cnt++;
                    }

                }
                int maxIndex = -1;
                int maxx = -1;
                for (int i=1;i<8;i++)
                    if (w[i]>=maxx) {
                        maxx=w[i];
                        maxIndex = i;
                    }

                if (cnt<m_exportParams["export1"] && curColor!=0 && curColor!=maxIndex) {
                    int add = 0;
                    curColor = maxIndex;
                    c = curColor + add;

                }
                else c+=64;
               data.append(c);
            //            if (rand()%100>98) qDebug() << pixel;
//                c |= (pixel<<(5-curBit));
            }

        }



    ofile.write(data);
}

void LImageOric::LoadBin(QFile &file)
{
    m_qImage = new QImage(m_width, m_height, QImage::Format_ARGB32);
    unsigned char *temp_data = new unsigned char[m_width*m_height];
    file.read((char*)temp_data, m_width*m_height);
    // Perform actual copy of data)
    for (int i=0;i<m_width;i++)
        for (int j=0;j<m_height;j++) {
            setPixel(i,j, temp_data[i+j*m_width]);
        }
    delete[] temp_data;
}

