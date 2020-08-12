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

#include "standardcolorimage.h"
#include <QFile>
#include <QTextStream>

StandardColorImage::StandardColorImage(LColorList::Type t) : MultiColorImage(t)
{
    m_width = 320;
    m_height = 200;
    //m_scaleX = 1.2f;
    m_bitMask = 0b1;
    m_noColors = 2;
    m_scale = 1;
    m_minCol = 0;
    Clear();
    m_type = LImage::Type::HiresBitmap;


    m_supports.asmExport = false;
    m_supports.binaryLoad = false;
    m_supports.binarySave = true;
    m_supports.flfSave = true;
    m_supports.flfLoad = true;
    m_supports.asmExport = true;

}

