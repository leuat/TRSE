/*
 * Turbo Rascal Syntax error, “;” expected but “BEGIN” (TRSE, Turbo Rascal SE)
 * 8 bit software development IDE for the Commodore 64
 * Copyright (C) 2018  Nicolaas Ervik Groeneboom (nicolaas.groeneboom@gmail.com)
 *
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program (LICENSE.txt).
 *   If not, see <https://www.gnu.org/licenses/>.
*/

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
#include "source/LeLib/limage/limagecharsetfixedcolor.h"

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
        if (t == LImage::Type::CharMapMultiColorFixed)
            return new LImageCharsetFixedColor(colorType);

        qDebug() << "ERROR: LImageFactory could not find type " << t;
        qDebug() << "Charmapfactory: " << LImage::Type::CharmapRegular;
        return nullptr;
    }



};




#endif // LIMAGEFACTORY_H
