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

#ifndef DIALOGIMPORT_H
#define DIALOGIMPORT_H


#include "imageworker.h"
#include <QDialog>
#include <QFontDatabase>
#include "source/LeLib/limage/bitmapfont.h"

namespace Ui {
class DialogImport;
}

class DialogImport : public QDialog
{
    Q_OBJECT

public:
    explicit DialogImport(QWidget *parent = 0);
    ~DialogImport();
    void Initialize(LImage::Type imageType, LColorList::Type colorType, LImage* img);


    BitmapFont m_bf;
    float m_contrast = 1;
    float m_shift = 0;
    float m_hsv = 0.5;
    float m_saturation = 0.5;
    float m_blur = 0;
    int matrixSizeX = 2;
    int matrixSizeY = 1;
    QPointF m_scale = QPointF(1,1);

    bool isPetscii = false;
    LImageQImage m_input;
    LImageQImage m_work;
    LImageQImage m_output;
    LImage* m_image = nullptr;


    LImage::Type m_imageType = LImage::Type::QImageBitmap;

    void Convert();
    void Blur();


    bool m_ok = false;


private slots:
    void on_hsContrast_sliderMoved(int position);

    void on_hsShift_sliderMoved(int position);

    void on_hsHsv_sliderMoved(int position);

    void on_hsSat_sliderMoved(int position);

    void on_hsBlur_sliderMoved(int position);

    void on_cmbForeground_activated(int index);

    void on_cmbBackground_activated(int index);

    void SetColors();


private slots:
    void UpdateOutput();

    void on_btnImport_clicked();

    void slotOk();



    void on_cmbMC1_activated(int index);

    void on_cmbMC2_activated(int index);

    void on_chkDither_stateChanged(int arg1);

    void on_btnFromFont_clicked();

    void on_btnImport_2_clicked();

    void on_hsDither_sliderMoved(int position);

    void on_hsScaleX_sliderMoved(int position);
    void on_hsScaleY_sliderMoved(int position);

    void on_cmbDither_currentIndexChanged(int index);

private:
    Ui::DialogImport *ui;
};

#endif // DIALOGIMPORT_H
