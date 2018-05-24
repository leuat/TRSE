
#ifndef LIMAGETIFF_H
#define LIMAGETIFF_H

#include "source/LeLib/limage/limageqimage.h"
//#include "source/ltiff/ltiff.h"

class LImageTiff : public LImageQImage {
public:
    LImageTiff();
    LImageTiff(LColorList::Type t);
    ~LImageTiff();
#ifdef USE_LIBTIFF
    QVector<LTiff*> m_tifs;
#endif
    void Initialize(int width, int height) override;

//    void setPixel(int x, int y, unsigned int color) override;
  //  unsigned int getPixel(int x, int y) override;

    bool LoadTiff(QString filename);

    void ToQImage(LColorList& lst, QImage* img, float zoom, QPointF center) override;


};
#endif // LIMAGETIFF_H
