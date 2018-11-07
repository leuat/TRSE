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

namespace Ui {
class DialogNewImage;
}

class DialogNewImage : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewImage(QWidget *parent = 0);
    void Initialize(QStringList cmbData);
    int retVal = -1;
    bool isResize = false;
    bool started = true;
    CharmapGlobalData m_meta;
    void SetResizeMeta(CharmapGlobalData gd);
    void ToMeta();
    void CreateInfo();

    void VICImageToData();

    void CharImageToData();

    int m_charWidth;
    int m_charHeight;

    ~DialogNewImage();
private slots:
    void slotOk();

    void on_comboBox_currentIndexChanged(int index);

    void on_leScreenWidth_textChanged(const QString &arg1);

    void on_leScreenHeight_textChanged(const QString &arg1);

    void on_leLevelsX_textChanged(const QString &arg1);

    void on_leLevelsY_textChanged(const QString &arg1);

    void on_leExtraDataSize_textChanged(const QString &arg1);

    void on_leStartX_textChanged(const QString &arg1);

    void on_leStartY_textChanged(const QString &arg1);

    void on_leChunkSize_textChanged(const QString &arg1);

    void on_leDataChunks_textChanged(const QString &arg1);

    void on_checkBox_clicked();

    void on_chkUseColors_stateChanged(int arg1);

    void on_leCharWidth_textChanged(const QString &arg1);

    void on_leCharHeight_textChanged(const QString &arg1);

private:
    Ui::DialogNewImage *ui;
};

#endif // DIALOGNEWIMAGE_H
