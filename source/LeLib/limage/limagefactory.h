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

#include "source/LeLib/limage/limage.h"
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
#include "source/LeLib/limage/limagevic20.h"
#include "source/LeLib/limage/limagesprites2.h"
#include "source/LeLib/limage/limagecga.h"
#include "source/LeLib/limage/limageamiga4.h"
#include "source/LeLib/limage/limageok64.h"
#include "source/LeLib/limage/limagex16.h"
#include "source/LeLib/limage/limagenes.h"
#include "source/LeLib/limage/limagegamboy.h"
#include "source/LeLib/limage/limagemetachunk.h"
#include "source/LeLib/limage/limagelevelnes.h"
#include "source/LeLib/limage/limagelevelgameboy.h"
#include "source/LeLib/limage/limagegamboy.h"
#include "source/LeLib/limage/limageatari520st.h"
#include "source/LeLib/limage/limagemetablocksprites.h"
#include "source/LeLib/limage/limagehybridcharset.h"
#include "source/LeLib/limage/limageamstradcpc.h"
#include "source/LeLib/limage/limageamstradgeneric.h"
#include "source/LeLib/limage/limagebbc.h"
#include "source/LeLib/limage/limagevga.h"
#include "source/LeLib/limage/limagespectrum.h"
#include "source/LeLib/limage/limagesnes.h"
#include "source/LeLib/limage/limagelevelsnes.h"
#include "source/LeLib/limage/limagevz200.h"
#include "source/LeLib/limage/limagecustomc64multicolor.h"
#include "source/LeLib/limage/limagejdh8.h"
#include "source/LeLib/limage/limagegeneric.h"
#include "source/LeLib/limage/limagegenericsprites.h"
#include "source/LeLib/limage/limagecga160x100.h"
#include "source/LeLib/limage/limageamstradsprites.h"
#include "source/LeLib/limage/limagesnesgeneric.h"
#include "source/LeLib/limage/limagetim.h"
#include "source/LeLib/limage/limagetvc.h"
#include "source/LeLib/limage/limagecoco3.h"
#include "source/LeLib/limage/limagethomson.h"
#include "source/LeLib/limage/limagetimgen.h"
#include "source/LeLib/limage/limagelevelgeneric.h"

class LImageFactory {
public:
    static LImage* Create(LImage* img) {
        return Create(img->m_type, img->m_colorList.m_type);
    }

    static LImage* Create(LImage::Type t, LColorList::Type colorType);



};




#endif // LIMAGEFACTORY_H
