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

#include "data.h"

// Extra level of indirection to allow the preprocessor to expand the macros
// before they are converted to strings
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define MAJOR 0
#define FEATURE 13
#define NIGHTLY 8
#define VERSION STR(MAJOR) "." STR(FEATURE) "." STR(NIGHTLY)

Data Data::data;

Data::Data()
{
    version = VERSION;
}

void Data::Redraw()
{
    redrawOutput = true;
}

void Data::UpdatePens()
{
//    redrawOutput = true;
    emit EmitPenChanged();

}
