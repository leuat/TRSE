#ifndef MOVIECONVERTER_H
#define MOVIECONVERTER_H

#include <QString>
#include <QImage>
class MovieConverter
{
public:
    MovieConverter();


    QByteArray ConvertImage(QImage& img, int w, int h, float zoom);
    void Convert(QString dir, QString baseName,  QString fileExtension,QString outFile,int frameSkip, int maxFrames, int width, int height, float zoom);


};

#endif // MOVIECONVERTER_H
