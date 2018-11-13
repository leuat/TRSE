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

#ifndef FormImageEditor_H
#define FormImageEditor_H

#include <QWidget>
#include "trsedocument.h"
#include "source/toolbox.h"
#include "source/LeLib/limage/limage.h"
#include "source/LeLib/limage/limageqimage.h"
#include "imageworker.h"
#include <QListWidget>
#include <QAbstractItemView>
#include <QListWidgetItem>
#include "source/LeLib/limage/limageio.h"
#include "dialognewimage.h"
#include "dialogimport.h"
#include "source/dialogexport.h"
#include "source/LeLib/util/util.h"
#include "source/LeLib/limage/limageeffects.h"
#include "source/dialogimagehelp.h"
#include "source/qlabellimage.h"
#include "source/LeLib/limage/limagefactory.h"
#include "source/LeLib/limage/limagecontainer.h"

#include <QLineEdit>
namespace Ui {
class Formimageeditor;
}



class FormImageEditor :  public TRSEDocument
{
    Q_OBJECT

public:
    explicit FormImageEditor(QWidget *parent = 0);
    ~FormImageEditor();
    ImageWorker m_work;

    WorkerThread m_updateThread;

    LImageQImage m_grid;
    Toolbox m_toolBox;
    LColorList* m_currentColorList = nullptr;
    LImageEffects m_imageEffects;
    QMap<QString, QLineEdit*> m_imageEffectsLineEdits;
    LImageEffect* m_currentImageEffect = nullptr;
    QString m_projectPath;
    QVector<int> m_keepSpriteChar;

    void UpdatePalette();
    void updateCharSet();

    void PrepareImageTypeGUI();

    void SetButton(QPushButton* btn, LImage::GUIType type);

    void PrepareClose() override;

    void SetMCColors();
    void UpdateLevels();
    void FillCMBColors();

    void resizeEvent(QResizeEvent *event) override;

    void mouseMoveEvent(QMouseEvent* e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent* e) override;
    void keyReleaseEvent(QKeyEvent *e) override;
    void UpdateImage();

    void Load(QString filename) override;
    void Save(QString filename) override;

    void FillImageEffect();
    void FillToImageParams();

    Ui::Formimageeditor *getUi() const;

    QLabel* getLabelImage();

    void UpdateCurrentMode();

    void InitDocument(WorkerThread* t, CIniFile* ini, CIniFile* iniProject) override;


    void GenericExportImage(QString type, QString ext);
    void GenericImportImage(QString type, QString ext);

    void UpdateSpriteImages();

/*signals:
    void EmitMouseEvent();
*/
private:
    Ui::Formimageeditor *ui;

private slots:

    void onImageMouseEvent();

    void on_btnExportAsm_clicked();

    void on_btnGenerate_clicked();

    void on_btnFlipVert_clicked();
    void on_btnFlipHorisontal_clicked();

    void SelectCharacter(int j);

    void on_cmbEffect_currentIndexChanged(int index);




//    void on_btnLoad_clicked();

 //   void on_btnSave_clicked();

    void on_chkGrid_clicked(bool checked);
    void on_chkBackgroundArea_clicked(bool checked);

    // Actually clear4
    void on_btnNew_clicked();

    void on_btnExportImage_clicked();

    void on_lstImages_clicked(const QModelIndex &index);


    void on_btnImport_clicked();

    void on_btnCharsetFull_clicked();
    void on_btnCharset1x1_clicked();
    void on_btnCharset2x2_clicked();
    void on_btnCharset2x2Repeat_clicked();
    void on_btnCharsetCopy_clicked();
    void on_btnCharsetPaste_clicked();







    void on_btnImportBin_clicked();

    void on_btnExportBin_clicked();

    void on_tabMain_currentChanged(int index);

    void on_btnLoadCharmap_clicked();

    void on_lstCharMap_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous);

    void Destroy();

    void on_cmbMC1_activated(int index);

    void on_cmbMC2_activated(int index);

    void on_cmbBackgroundMain_3_activated(int index);



    void on_btnResizeData_clicked();

    void on_chkDisplayMulticolor_stateChanged(int arg1);
    void on_leHeaders_editingFinished();
    void on_tblData_cellChanged(int row, int column);

    void on_btnHelpImage_clicked();

    void on_btnExportKoala_clicked();
    void on_btnImportKoala_clicked();
    void on_btnNewSprite_clicked();
    void on_btnDeleteSprite_clicked();
    void on_btnCopySprite_clicked();
    void on_btnPasteSprite_clicked();
    void on_btnFlipXSprite_clicked();
    void on_btnFlipYSprite_clicked();
    void on_sliderX_actionTriggered(int action);
    void on_btnPanLeft_clicked();
    void on_btnPanRight_clicked();
    void on_btnPanUp_clicked();
    void on_btnPanDown_clicked();
    void on_lblSprite1_clicked();
    void on_lblSprite3_clicked();
    void on_btnMoveSpriteLeft_clicked();
    void on_btnMoveSpriteRight_clicked();
    void on_btnSpriteMulticolor_clicked();
    void on_btnImportRom_clicked();
    void on_btnExportMovie_clicked();
    void on_leTimeStamp_textChanged(const QString &arg1);
    void on_btnImportC_clicked();
};


#endif // FormImageEditor_H
