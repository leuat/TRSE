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

#ifndef DIALOGNEWIMAGE_H
#define DIALOGNEWIMAGE_H

#include <QDialog>
#include <QDebug>
#include "source/LeLib/limage/imageleveleditor.h"
#include "imageworker.h"
#include "source/LeLib/limage/limagefactory.h"

namespace Ui {
class DialogNewImage;
}

class DialogNewImage : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewImage(QWidget *parent = 0);
    //void Initialize(QStringList cmbData);
    void Initialize(QVector<ImageType>);
    int retVal = -1;
    bool isResize = false;
    bool started = true;
    LImage* m_metaImage = nullptr;

    QVector<ImageType> m_types;

    CharmapGlobalData m_meta;
    void SetResizeMeta(CharmapGlobalData gd);
    void ToMeta();
    void FromMeta();



//    int m_charWidth=40;
  //  int m_charHeight=25;

    ~DialogNewImage();
private slots:
    void slotOk();

    void on_comboBox_currentIndexChanged(int index);
private:
    Ui::DialogNewImage *ui;
};

#endif // DIALOGNEWIMAGE_H
