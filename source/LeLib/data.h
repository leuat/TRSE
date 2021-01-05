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
#include <QObject>




class Data : public QObject
{
    Q_OBJECT

public:
    Data();
    int flfVersion = 2;
    QString version;
    enum CompilerState {PREPROCESSOR, PARSER, DISPATCHER, ASSEMBLER };
    CompilerState compilerState = PREPROCESSOR;
    QString unitPath = "units";
    QString cpuUnitPath = "cpu_specific";
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
    unsigned char currentColorType = 0;
    bool currentIsColor = false;
    bool strictReferences = false;
    static Data data;
    void Redraw();
    void UpdatePens();
signals:
    void EmitPenChanged();

};

class ParseData {
public:
    QString m_filename = "";
    bool m_isTRU = false;

};


#endif // DATA_H
