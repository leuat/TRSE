#include "limageio.h"


LImageIO::LImageIO()
{

}

QString LImageIO::m_fileExtension = "flf";
QString LImageIO::m_ID = "FLUFF64";

bool LImageIO::Save(QString filename, LImage* img)
{
    QString headerID = LImageIO::m_ID;// + m_version;
    QByteArray array = headerID.toLocal8Bit();
    char* header = array.data();
    unsigned char version[sizeof(float)];
    memcpy(version, &Data::data.version, sizeof(float));

    QFile file( filename);
    if( !file.open( QFile::WriteOnly ) )
        return false;

    unsigned char imageType = LImage::TypeToChar(img->m_type);
    unsigned char colorType = LColorList::TypeToChar(img->m_colorList.m_type);

    // 7 + 4 + 1 + 1 = 13

    file.write( ( char * )( header ), array.count() );
    file.write( ( char * )( version ),sizeof(float) );
    file.write( ( char * )( &imageType ),1 );
    file.write( ( char * )( &colorType ),1 );
    img->SaveBin(file);
    file.close();
}

LImage* LImageIO::Load(QString filename)
{

//    return LImageFactory::Create(LImage::HiresBitmap,LColorList::C64);

    QString headerID = "fluff64";// + m_version;
    QByteArray array = headerID.toLocal8Bit();
    char* header = array.data();
    //memcpy(Data::data.version, &f, sizeof(f));

    QFile file( filename);
    if( !file.open( QFile::ReadOnly ) )
        return false;

    float version;
    unsigned char imageType;
    unsigned char paletteType;

    file.read( ( char * )( header ), array.count() );
    file.read( ( char * )( &version ),sizeof(float) );
    file.read( ( char * )( &imageType ),1);
    file.read( ( char * )( &paletteType ),1);

    if (version>Data::data.version) {
        qDebug() << "File version higher than current version (" << version << " vs current " << Data::data.version << ")";
        file.close();
        return false;
    }

    LImage* img = LImageFactory::Create(LImage::CharToType(imageType), LColorList::CharToType(paletteType));
    img->LoadBin(file);
    file.close();
    return img;
}

