#ifndef MOVIECONVERTER_H
#define MOVIECONVERTER_H

#include <QString>
#include <QImage>
#include <QDir>
class MovieConverter
{
public:
    MovieConverter();
    uchar SKIP = 255;
    uchar END = 254;

    QByteArray ConvertImage(QImage& img, int w, int h, float zoom);
    void ConvertRaw(QString dir, QString baseName,  QString fileExtension,QString outFile,int frameSkip, int maxFrames, int width, int height, float zoom);
    void ConvertPacked(QString dir, QString baseName,  QString fileExtension,QString outFile,int frameSkip, int maxFrames, int width, int height, float zoom);
    QByteArray CompressImage(QByteArray prevFrame, QByteArray newFrame, int w, int h, float& compr);

};

#endif // MOVIECONVERTER_H
