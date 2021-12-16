#include "limagecustomc64multicolor.h"


LImageCustomC64::LImageCustomC64(LColorList::Type t) : MultiColorImage(t) {
    m_type = CustomC64;
  //  m_type = CharMapMulticolor;

    m_metaParams.append(new MetaParameter("char_width","Char width",40,1,255));
    m_metaParams.append(new MetaParameter("char_height","Char height",25,1,255));
//    ReInitialize();
    EnsureSystemColours();
    m_noColors = 4;
}

void LImageCustomC64::SaveBin(QFile &file)
{
    unsigned char dummy = m_charWidth;
    file.write( ( char * )( &(dummy) ),  1 );

    dummy = m_charHeight;
    file.write( ( char * )( &(dummy) ), 1 );

    file.write( ( char * )( &m_data ),  m_charHeight*m_charWidth*12 );

}

void LImageCustomC64::LoadBin(QFile &file)
{
    unsigned char dummy;
    file.read( ( char * )( &dummy ), 1);
    m_width = dummy*8;
    m_charWidth = dummy;

    file.read( ( char * )( &dummy ), 1);
    m_height = dummy*8;
    m_charHeight = dummy;
//    qDebug() << m_charHeight;
    file.read( ( char * )( &m_data ),  m_charHeight*m_charWidth*12 );
}

QString LImageCustomC64::getMetaInfo()
{
    m_charWidth = getMetaParameter("char_width")->value;
    m_charHeight = getMetaParameter("char_height")->value;
    m_width = m_charWidth*8;
    m_height = m_charHeight*8;



    return "Custom C64 image type. Paint NxM 8x8 cells.";
}

void LImageCustomC64::ReInitialize()
{
}

void LImageCustomC64::ExportBin(QFile &ofile)
{

    QString f = ofile.fileName();

    QString filenameBase = Util::getFileWithoutEnding(f);

    QString fData = filenameBase + "_data.bin";
    QString fColor = filenameBase + "_color.bin";

    if (QFile::exists(fData))
        QFile::remove(fData);

    if (QFile::exists(fColor))
        QFile::remove(fColor);

    QByteArray data;
    int sx = static_cast<int>(0);
    int ex = static_cast<int>(m_charWidth);
    int sy = static_cast<int>(0);
    int ey = static_cast<int>(m_charHeight);


    for (int j=sy;j<ey;j++)
        for (int i=sx;i<ex;i++)
            data.append(m_data[i + j*m_charWidth].data());
    /*    for (int i=0;i<m_charWidth*m_charHeight;i++) {
            data.append(m_data[i].data());
        }*/
    QFile file(fData);
    file.open(QIODevice::WriteOnly);
    file.write( data );
    file.close();

    QByteArray colorData;
//    colorData.append(getBackground());
  //  colorData.append(getBackground());
    data.clear();
    int charC = 3;


    if (m_bitMask== 0b1 || m_type==LImage::Type::HiresBitmap || m_footer.get(LImageFooter::POS_DISPLAY_MULTICOLOR)==0) { // Regular color
        charC = 1;
    }

    if (charC==3)
        for (int j=sy;j<ey;j++)
            for (int i=sx;i<ex;i++) {
                //            data.append(m_data[i + j*m_charWidth].data()); {
                colorData.append((uchar)m_data[j*m_charWidth + i].colorMapToNumber(1,2));
            }

    /*  for (int i=0;i<m_charWidth*m_charHeight;i++) {
          //  qDebug () << QString::number((uchar)colorData[colorData.count()-1]);
    //        colorData.append((uchar)m_data[i].colorMapToNumber(1,2));
        }*/
    for (int j=sy;j<ey;j++)
        for (int i=sx;i<ex;i++) {

            //    for (int i=0;i<m_charWidth*m_charHeight;i++) {
            //        uchar c = (uchar)m_data[i].c[3];
            uchar c = 0;
            if (charC==3)
                c = (uchar)m_data[i+j*m_charWidth].c[charC];
            if (charC==1) {
                //            c = (uchar)m_data[i+j*m_charWidth].c[charC];
                c = ((uchar)m_data[j*m_charWidth + i].colorMapToNumber(0,1));
            }
            if (c==255)
                c=0;
            //        if (c!=0)
            //          qDebug() << c;



            //data.append((char)c);
        }
    QFile file2(fColor);
    file2.open(QIODevice::WriteOnly);
    file2.write( colorData );
    file2.write( data );
    file2.close();


    // Take care of color data!

    ofile.close();
    QFile::remove(ofile.fileName());

}
