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

#ifndef IMAGEEDITOR_H
#define IMAGEEDITOR_H

#include <QString>
#include <QVector>
#include "source/LeLib/limage/limagefactory.h"
#include "source/LeLib/limage/lcolorlist.h"

class ImageType {
public:
    QString name;
    QString category="";
    LImage::Type type;
    LColorList::Type colorType;
    ImageType() {

    }

    ImageType(QString n,QString cat,LImage::Type t, LColorList::Type colType) {
        type = t;
        name = n;
        category = cat;
        colorType = colType;
    }
};



class ImageEdit {
public:
    ImageEdit(ImageType* t, QString name);
    ImageEdit(LImage* image, ImageType* it,QString name);


    //ImageType* m_imageType;
    QString m_name;
    QString m_fileName;

    //LColorList* m_colorList = nullptr;
    void Initialize();
    void Initialize(LImage* img);

    LImage* m_image;
    LImage* m_temp;
    QVector<LImage*> m_undo;
    void Undo();
    void AddUndo();
    void setName();
    const int m_undoMax = 10;


};


#endif // IMAGEEDITOR_H
