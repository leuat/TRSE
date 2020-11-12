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
    bool ignore = false;
    bool ignoreText = false;
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
    LImageQImage m_intermediate;
    LImageQImage m_work;
    LImageQImage m_output;
    LImage* m_image = nullptr;


    LImage::Type m_imageType = LImage::Type::QImageBitmap;

    void Convert();
    void Blur();
    void UpdateSliders();
    void UpdateText();

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

    void on_chkTreatCharset_stateChanged(int arg1);

    void on_leCharWidth_textChanged(const QString &arg1);

    void on_leCharHeight_textChanged(const QString &arg1);

    void on_leBlockWidth_textChanged(const QString &arg1);

    void on_leBlockHeight_textChanged(const QString &arg1);

    void on_leOutCharWidth_textChanged(const QString &arg1);

    void on_leOutCharHeight_textChanged(const QString &arg1);

    void on_leAllowance_textChanged(const QString &arg1);

    void on_leDither_textChanged(const QString &arg1);

    void on_leGamma_textChanged(const QString &arg1);

    void on_leShift_textChanged(const QString &arg1);

    void on_leHsv_textChanged(const QString &arg1);

    void on_leSat_textChanged(const QString &arg1);

    void on_leBlur_textChanged(const QString &arg1);

    void on_leScaleX_textChanged(const QString &arg1);

    void on_leScaleY_textChanged(const QString &arg1);

private:
    Ui::DialogImport *ui;
};

#endif // DIALOGIMPORT_H
