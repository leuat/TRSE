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

#ifndef IMAGEWORKER_H
#define IMAGEWORKER_H

#include <QVector>

#include "source/LeLib/limage/limageqimage.h"
#include "source/LeLib/limage/lcolorlist.h"
#include "source/LeLib/limage/multicolorimage.h"
#include "source/LeLib/limage/limagefactory.h"
#include "imageeditor.h"
#include "source/LeLib/limage/limagefactory.h"
#include <QListView>


class ImageWorker
{
public:
    ImageWorker();
    ~ImageWorker();
    //LImageFactory::Type imageType = LImageFactory::MultiColorBitmap;

    //ImageEdit m_editor = ImageEdit(imageType);

    //LColorList m_colorList;

    ImageEdit* m_currentImage = nullptr;// = ImageEdit(LImageFactory::HiresBitmap);
    QVector<ImageEdit*> m_images;
    QVector<ImageType> m_types;

    ImageType* findType(LImage::Type imageType, LColorList::Type colType);

    void UpdateListView(QListView* lst);

    QStringList getImageTypes();

//    void New(int type, CharmapGlobalData gd);
    void New(LImage* img, int image);
    void New(LImage* image, QString name);
    void SetImage(int cur);

//    void Load(QString filename);

    void Blur();
    void Convert();


};

#endif // IMAGEWORKER_H
