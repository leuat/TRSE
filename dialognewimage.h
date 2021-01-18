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
    QVector<ImageType> m_currentTypes;
    QStringList m_categories;

    CharmapGlobalData m_meta;
    void CreateCategories();
    void SetResizeMeta(CharmapGlobalData gd);
    void ToMeta();
    void FromMeta();




    ~DialogNewImage();
private slots:
    void slotOk();

    void on_cmbImageType_currentIndexChanged(int index);

    void on_cmbSystem_currentTextChanged(const QString &arg1);

private:
    Ui::DialogNewImage *ui;
};

#endif // DIALOGNEWIMAGE_H
