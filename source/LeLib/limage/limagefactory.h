#ifndef LIMAGEFACTORY_H
#define LIMAGEFACTORY_H

#include "source/LeLib/limage/limageqimage.h"
#include "source/LeLib/limage/multicolorimage.h"
#include "source/LeLib/limage/standardcolorimage.h"
//#include "source/LeLib/limage/limagetiff.h"
#include "source/LeLib/limage/charsetimage.h"
#include "source/LeLib/limage/c64fullscreenchar.h"
#include "source/LeLib/limage/imageleveleditor.h"
#include "source/LeLib/limage/limagecharsetregular.h"
#include "source/LeLib/limage/limagesprites.h"

class LImageFactory {
public:

    static LImage* Create(LImage::Type t, LColorList::Type colorType) {
        if (t == LImage::Type::QImageBitmap)
            return new LImageQImage(colorType);
        if (t == LImage::Type::MultiColorBitmap)
            return new MultiColorImage(colorType);
        if (t == LImage::Type::HiresBitmap)
            return new StandardColorImage(colorType);
  //      if (t == LImage::Type::Tiff)
    //        return new LImageTiff(colorType);
        if (t == LImage::Type::CharMapMulticolor)
            return new CharsetImage(colorType);
        if (t == LImage::Type::CharmapRegular)
            return new LImageCharsetRegular(colorType);
        if (t == LImage::Type::FullScreenChar)
            return new C64FullScreenChar(colorType);
        if (t == LImage::Type::LevelEditor)
            return new ImageLevelEditor(colorType);
        if (t == LImage::Type::Sprites)
            return new LImageSprites(colorType);

        qDebug() << "ERROR: LImageFactory could not find type " << t;
        qDebug() << "Charmapfactory: " << LImage::Type::CharmapRegular;
        return nullptr;
    }



};




#endif // LIMAGEFACTORY_H
