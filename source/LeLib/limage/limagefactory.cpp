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

#include "limagefactory.h"


LImage *LImageFactory::Create(LImage::Type t, LColorList::Type colorType) {
//    qDebug() << "Creating type: " <<LImage::TypeToString(t);

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
//        return new LImageCharsetRegular(colorType);
      return new CharsetImage(colorType);
    if (t == LImage::Type::FullScreenChar)
        return new C64FullScreenChar(colorType);
    if (t == LImage::Type::LevelEditor)
        return new ImageLevelEditor(colorType);
    if (t == LImage::Type::Sprites)
        return new LImageSprites(colorType);
    if (t == LImage::Type::CharMapMultiColorFixed)
        return new LImageCharsetFixedColor(colorType);
    if (t == LImage::Type::VIC20_MultiColorbitmap)
        return new LImageVIC20(colorType);
    if (t == LImage::Type::Sprites2)
        return new LImageSprites2(colorType);
    if (t == LImage::Type::CGA)
        return new LImageCGA(colorType);
    if (t == LImage::Type::AMIGA320x200)
        return new LImageAmiga(colorType,0);
    if (t == LImage::Type::AMIGA320x256)
        return new LImageAmiga(colorType,1);
    if (t == LImage::Type::OK64_256x256)
        return new LImageOK64(colorType);

    if (t == LImage::Type::X16_640x480)
        return new LImageX16(colorType);
    if (t == LImage::Type::NES)
        return new LImageNES(colorType);
    if (t == LImage::Type::LMetaChunk)
        return new LImageMetaChunk(colorType);
    if (t == LImage::Type::LevelEditorNES)
        return new LImageLevelNES(colorType);
    if (t == LImage::Type::SpritesNES)
        return new LImageMetaBlockSprites(colorType);
    if (t == LImage::Type::GAMEBOY)
        return new LImageGamboy(colorType);
    if (t == LImage::Type::LevelEditorGameboy)
        return new LImageLevelGameboy(colorType);
    if (t == LImage::Type::ATARI320x200)
        return new LImageAtari520ST(colorType,0);
    if (t == LImage::Type::HybridCharset)
        return new LImageHybridCharset(colorType);
    if (t == LImage::Type::AmstradCPC)
        return new LImageAmstradCPC(colorType);
    if (t == LImage::Type::AmstradCPCGeneric)
        return new LImageAmstradGeneric(colorType);
    if (t == LImage::Type::BBC)
        return new LImageBBC(colorType);
    if (t == LImage::Type::VGA)
        return new LImageVGA(colorType);
    if (t == LImage::Type::Spectrum)
        return new LImageSpectrum(colorType);
    if (t == LImage::Type::SNES)
        return new LImageSNES(colorType);
    if (t == LImage::Type::LevelEditorSNES)
        return new LImageLevelSNES(colorType);
    if (t == LImage::VZ200)
        return new LImageVZ200(colorType);
    if (t == LImage::CustomC64)
        return new LImageCustomC64(colorType);
    if (t == LImage::JDH8)
        return new LImageJDH8(colorType);
    if (t == LImage::LImageGeneric)
        return new LImageGeneric(colorType);
    if (t == LImage::GenericSprites)
        return new LImageGenericSprites(colorType);
    if (t == LImage::CGA160x100)
        return new LImageCGA160x100(colorType);
    if (t == LImage::AmstradSprites)
        return new LImageAmstradSprites(colorType);
    if (t == LImage::SNESGeneric)
        return new LImageSnesGeneric(colorType);
    if (t == LImage::TIM)
        return new LImageTIM(colorType);
    if (t == LImage::TVC)
        return new LImageTVC(colorType);
    if (t == LImage::COCO3)
        return new LImageCOCO3(colorType);
    if (t == LImage::THOMSON)
        return new LImageThomson(colorType);
    if (t == LImage::TIMG)
        return new LImageTIMGen(colorType);
    if (t == LImage::LevelEditorGeneric)
        return new LImageLevelGeneric(colorType);


    qDebug() << "ERROR: LImageFactory could not find type " << t;
    return nullptr;
}
