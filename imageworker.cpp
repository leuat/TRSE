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

#include "imageworker.h"
#include <QDebug>
#include <QStandardItemModel>

ImageWorker::ImageWorker()
{

    m_types.append(ImageType("CGA png Palette 1", "x86",LImage::Type::CGA, LColorList::Type::CGA1_HIGH));
    m_types.append(ImageType("Amiga 320x256", "amiga",LImage::Type::AMIGA320x256, LColorList::Type::C64));
    m_types.append(ImageType("Amiga 320x200", "amiga",LImage::Type::AMIGA320x200, LColorList::Type::C64));
    m_types.append(ImageType("MultiColor bitmap", "c64", LImage::Type::MultiColorBitmap,LColorList::Type::C64));
    m_types.append(ImageType("Custom (large) MultiColor bitmap", "c64", LImage::Type::CustomC64,LColorList::Type::C64));
    m_types.append(ImageType("Hires Bitmap", "c64",LImage::Type::HiresBitmap,LColorList::Type::C64));
    m_types.append(ImageType("Charmap", "c64",LImage::Type::CharMapMulticolor,LColorList::Type::C64));
//    m_types.append(ImageType("Hires Charmap", "c64",LImage::Type::CharmapRegular,LColorList::Type::C64));
    m_types.append(ImageType("Charmap", "vic20",LImage::Type::CharMapMulticolor,LColorList::Type::C64));
    m_types.append(ImageType("PBM Image", "pet",LImage::Type::CharMapMulticolor,LColorList::Type::C64));
    m_types.append(ImageType("Level Editor", "pet",LImage::Type::LevelEditor,LColorList::Type::C64));
//    m_types.append(ImageType("Hires Charmap", "vic20",LImage::Type::CharmapRegular,LColorList::Type::C64));
    m_types.append(ImageType("Screen animation", "c64",LImage::Type::FullScreenChar,LColorList::Type::C64));
    m_types.append(ImageType("Screen animation", "vic20",LImage::Type::FullScreenChar,LColorList::Type::C64));
    m_types.append(ImageType("Level Editor", "c64",LImage::Type::LevelEditor,LColorList::Type::C64));
    m_types.append(ImageType("Sprite Editor", "c64",LImage::Type::Sprites2,LColorList::Type::C64));
    m_types.append(ImageType("Multicolor bitmap", "vic20",LImage::Type::VIC20_MultiColorbitmap,LColorList::Type::VIC20));
    m_types.append(ImageType("Level Editor", "vic20",LImage::Type::LevelEditor,LColorList::Type::C64));
    m_types.append(ImageType("OK64 Image", "ok64",LImage::Type::OK64_256x256,LColorList::Type::OK64));
    m_types.append(ImageType("X16 640x480 Image", "x16",LImage::Type::X16_640x480,LColorList::Type::X16));
    m_types.append(ImageType("PICO 8", "pico-8",LImage::Type::QImageBitmap, LColorList::Type::PICO8));
    m_types.append(ImageType("NES CHR/tiles", "nes",LImage::Type::NES, LColorList::Type::NES));
    m_types.append(ImageType("Meta Tile Editor", "nes",LImage::Type::LMetaChunk, LColorList::Type::NES));
    m_types.append(ImageType("Meta Tile Editor", "c64",LImage::Type::LMetaChunk, LColorList::Type::NES));
    m_types.append(ImageType("Meta Tile Editor", "vic20",LImage::Type::LMetaChunk, LColorList::Type::NES));
    m_types.append(ImageType("NES Meta Sprite editor", "nes",LImage::Type::SpritesNES, LColorList::Type::NES));
    m_types.append(ImageType("NES Level Editor", "nes",LImage::Type::LevelEditorNES,LColorList::Type::NES));
    m_types.append(ImageType("Fake C64 png Palette 1", "c64",LImage::Type::QImageBitmap, LColorList::Type::C64));
    m_types.append(ImageType("Fake C64 png Palette 2", "c64",LImage::Type::QImageBitmap, LColorList::Type::C64_ORG));
    m_types.append(ImageType("Sprite Editor (deprecated type)","c64", LImage::Type::Sprites,LColorList::Type::C64));
    m_types.append(ImageType("Gameboy CHR/tiles", "gameboy",LImage::Type::GAMEBOY, LColorList::Type::NES));
    m_types.append(ImageType("Meta Tile Editor", "gameboy",LImage::Type::LMetaChunk, LColorList::Type::NES));
    m_types.append(ImageType("Gameboy Level Editor", "gameboy",LImage::Type::LevelEditorGameboy,LColorList::Type::NES));
    m_types.append(ImageType("320x200 16 color Atari ST", "atari520st",LImage::Type::ATARI320x200,LColorList::Type::C64));
    m_types.append(ImageType("160x200 16 color Amstrad CPC", "amstradcpc",LImage::Type::AmstradCPC,LColorList::Type::AMSTRADCPC));
    m_types.append(ImageType("Amstrad CPC sprites", "amstradcpc",LImage::Type::AmstradSprites,LColorList::Type::AMSTRADCPC));
    m_types.append(ImageType("generic NxM color Amstrad", "amstradcpc",LImage::Type::AmstradCPCGeneric,LColorList::Type::AMSTRADCPC));
    m_types.append(ImageType("BBC Image", "bbcm",LImage::Type::BBC,LColorList::Type::BBC));
    m_types.append(ImageType("VGA 320x200", "x86",LImage::Type::VGA, LColorList::Type::VGA));
    m_types.append(ImageType("ZX Spectrum 256x192", "spectrum",LImage::Type::Spectrum, LColorList::Type::SPECTRUM));
    m_types.append(ImageType("SNES tilemap/sprites", "snes",LImage::Type::SNES, LColorList::Type::SNES));
    m_types.append(ImageType("SNES Level editor", "snes",LImage::Type::LevelEditorSNES, LColorList::Type::SNES));
    m_types.append(ImageType("SNES generic image", "snes",LImage::Type::SNESGeneric, LColorList::Type::SNES));
    m_types.append(ImageType("VZ200 mode 1 (128x64)", "vz200",LImage::Type::VZ200, LColorList::Type::VZ200));
    m_types.append(ImageType("JDH8", "jdh8",LImage::Type::JDH8, LColorList::Type::CGA1_HIGH));
    m_types.append(ImageType("Generic indexed image", "all",LImage::Type::LImageGeneric, LColorList::Type::C64));
    m_types.append(ImageType("Generic 256-color sprites", "x16",LImage::Type::GenericSprites, LColorList::Type::VGA));
    m_types.append(ImageType("CGA/EGA 160x100 16 colour", "x86",LImage::Type::CGA160x100, LColorList::Type::DOS));
    m_types.append(ImageType("TIM 512x256", "TIM",LImage::Type::TIM, LColorList::Type::TIM));
    m_types.append(ImageType("Videoton generic", "VIDEOTON",LImage::Type::Videoton, LColorList::Type::VIDEOTON));

//    m_types.append(ImageType("Hybrid multicolor/hires charset", "c64",LImage::Type::HybridCharset,LColorList::Type::C64));
  //  m_types.append(ImageType("Hybrid multicolor/hires charset", "vic20",LImage::Type::HybridCharset,LColorList::Type::VIC20));
    //    m_types.append(ImageType("C64 Multicolor Charmap Fixed Colors", LImage::Type::CharMapMultiColorFixed,LColorList::Type::C64));

    /*    m_types.append(ImageType("CGA Palette 1 Lo", LImage::Type::QImageBitmap,LColorList::Type::CGA1_LOW));
    m_types.append(ImageType("CGA Palette 1 Hi", LImage::Type::QImageBitmap,LColorList::Type::CGA1_HIGH));
    m_types.append(ImageType("CGA Palette 2 Lo", LImage::Type::QImageBitmap,LColorList::Type::CGA2_LOW));
    m_types.append(ImageType("CGA Palette 2 Hi", LImage::Type::QImageBitmap,LColorList::Type::CGA2_HIGH));
    m_types.append(ImageType("TIFF", LImage::Type::Tiff,LColorList::Type::TIFF));
*/

    //New(5,);
}

ImageWorker::~ImageWorker()
{
    for (ImageEdit* ie: m_images)
        delete ie;
    m_images.clear();
}

ImageType *ImageWorker::findType(LImage::Type imageType, LColorList::Type colType)
{
    for (int i=0;i<m_types.count();i++) {
        ImageType* it=&m_types[i];
        if (it->colorType==colType && it->type == imageType)
            return it;
    }
    return nullptr;//&m_types[0];
}

void ImageWorker::UpdateListView(QListView *lst)
{
    QStandardItemModel *model = new QStandardItemModel( m_images.count(), 1, nullptr);
    for (int i=0;i<m_images.count();i++) {
        QStandardItem *item = new QStandardItem(m_images[i]->m_name);
        model->setItem(i,0, item);
    }

    lst->setModel(model);

}


QStringList ImageWorker::getImageTypes()
{
    QStringList l;
    for (ImageType it : m_types)
        l<< it.name;
    return l;
}

void ImageWorker::New(LImage *image, QString name)
{
/*    ImageType* imageType = findType(image->m_type, image->m_colorList.m_type);

    if (imageType == nullptr) {
        qDebug() << "WTF NULLPTR imagetype ";
        return;
    }*/
    m_currentImage = new ImageEdit(image, nullptr, name);
    //m_currentImage = new ImageEdit(imageType, "New Image");
    m_images.append(m_currentImage);
    Data::data.redrawFileList = true;
    Data::data.Redraw();
}

void ImageWorker::New(LImage* img, int image)
{
//    exit(1);

    m_currentImage = new ImageEdit(img, &m_types[image], "New Image");
    m_currentImage->Initialize(img);
    m_images.append(m_currentImage);
    m_currentImage->m_fileName="";

    Data::data.redrawFileList = true;
}

void ImageWorker::SetImage(int cur)
{
    if (cur>=0 && cur<m_images.count())
        m_currentImage = m_images[cur];

}



