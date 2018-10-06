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

#ifndef LIMAGEIO_H
#define LIMAGEIO_H

#include "source/LeLib/limage/limagefactory.h"
#include "source/LeLib/limage/limage.h"
#include "source/LeLib/limage/lcolorlist.h"
#include "source/LeLib/data.h"

class LImageIO
{
public:
    LImageIO();

    static QString m_fileExtension;
    static QString m_ID;

    static bool Save(QString filename, LImage* img);
    static LImage* Load(QString filename);


};

#endif // LIMAGEIO_H
