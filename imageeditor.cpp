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

#include "imageeditor.h"


ImageEdit::ImageEdit(ImageType* t, QString name)
{
    //m_imageType = t;
    m_fileName = name;
    setName();
    Initialize();
}

ImageEdit::ImageEdit(LImage* image, ImageType* it, QString name)
{
//    m_imageType = it;
    m_fileName = name;
    setName();
    m_image = image;
    m_temp = LImageFactory::Create(m_image->m_type,m_image->m_colorList.m_type);
    m_temp->Initialize(m_image->m_width, m_image->m_height);

}

void ImageEdit::Initialize()
{
    m_image = LImageFactory::Create(m_image->m_type,m_image->m_colorList.m_type);
    m_temp = LImageFactory::Create(m_image->m_type,m_image->m_colorList.m_type);
}

void ImageEdit::Initialize(LImage* img)
{
    m_image = img;

    m_temp = LImageFactory::Create(m_image->m_type,m_image->m_colorList.m_type);
    img->ReInitialize();
}

void ImageEdit::Undo()
{
    if (m_undo.count()<1)
        return;
    m_image->CopyFrom(m_undo.last());
    m_undo.removeLast();
    Data::data.redrawOutput = true;
}

void ImageEdit::AddUndo()
{
    m_undo.append(LImageFactory::Create(m_image->m_type, m_image->m_colorList.m_type));
    m_undo.last()->CopyFrom(m_image);
    if (m_undo.count()>m_undoMax) {
        m_undo.removeFirst();
    }

}

void ImageEdit::setName()
{
    QStringList fs = m_fileName.split("/");
    m_name = Util::getFileWithoutEnding((fs[fs.count()-1]));
}
