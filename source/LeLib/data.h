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

#ifndef DATA_H
#define DATA_H

#include <QString>


class Data
{
public:
    Data();
    int flfVersion = 2;
    QString version = "0.09.internal";

    bool redrawOutput=false;
    bool redrawInput=false;
    bool forceRedraw = false;
    bool redrawFileList = false;
    bool abort = false;
    int imageEvent = 0;
    bool requestSaveAs = false;
    bool requestCloseWindow = false;
    bool blink = false;
    QString currentPath;
    float percent = 0;
    unsigned char currentColor = 0;
    bool currentIsColor = false;
    static Data data;
    void Redraw();
};

#endif // DATA_H
