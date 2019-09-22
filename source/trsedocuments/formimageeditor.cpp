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

#include "formimageeditor.h"
#include "ui_formimageeditor.h"
#include <algorithm>
#include <cmath>
#include <QPushButton>
#include "source/LeLib/limage/charsetimage.h"
#include "source/messages.h"


FormImageEditor::FormImageEditor(QWidget *parent) :
    TRSEDocument(parent),
    ui(new Ui::Formimageeditor)
{
    ui->setupUi(this);
    m_toolBox.Initialize(ui->lyToolbox_3);;

    UpdatePalette();
    m_grid.Initialize(320,200);
    m_fileExtension = "flf";
    Data::data.currentColor=1;
    m_grid.ApplyToLabel(ui->lblGrid);
    updateCharSet();
    ui->lblGrid->setVisible(ui->chkGrid->isChecked());
/*    ui->lblImage->setMouseTracking(true);
    ui->lblGrid->setMouseTracking(true);
    setMouseTracking(true);
*/
    //ui->lblImage->visibleRegion()

    //ui->lblGrid->m_work = &m_work;
    //ui->lblGrid->setMouseTracking(true);


    ui->lblImage->m_work = &m_work;
    ui->lblImage->setMouseTracking(true);
    ui->lblImage->m_updateThread = &m_updateThread;
//    void EmitMouseMove();

    connect(ui->lblImage, SIGNAL(EmitMouseMove()), this, SLOT(onImageMouseEvent()));

    m_updateThread.SetCurrentImage(&m_work, &m_toolBox, getLabelImage());




}

void FormImageEditor::InitDocument(WorkerThread *t, CIniFile *ini, CIniFile *iniProject) {
//    m_updateThread = t;
    m_iniFile = ini;
    m_projectIniFile = iniProject;
//    ui->lblImage->m_updateThread = t;
    //ui->lblGrid->m_updateThread = t;
    m_updateThread.m_grid = &m_grid;
//    m_grid.CreateGrid(40,25,m_updateThread.m_gridColor,4, 1, QPoint(0,0));
    UpdateGrid();


}


void FormImageEditor::onImageMouseEvent()
{
//    emit EmitMouseEvent();
    m_updateThread.RunContents();
    UpdateImage();
    if (dynamic_cast<LImageSprites2*>(m_work.m_currentImage->m_image)!=nullptr)
        UpdateSpriteImages();
    if (dynamic_cast<C64FullScreenChar*>(m_work.m_currentImage->m_image)!=nullptr)
        UpdateSpriteImages();
//    if (dynamic_cast<LImageCharsetRegular*>(m_work.m_currentImage->m_image)!=nullptr)
  //      updateCharSet();

    //    updateCharSet();
    updateSingleCharSet();
}

FormImageEditor::~FormImageEditor()
{
    delete ui;
}

void FormImageEditor::mousePressEvent(QMouseEvent *e)
{
/*    if(e->buttons() == Qt::LeftButton) {

        m_work.m_currentImage->AddUndo();
    }
    if(e->buttons() == Qt::RightButton)
        m_updateThread.m_currentButton = 2;
    if(e->buttons() == Qt::LeftButton) {
        m_updateThread.m_currentButton = 1;
    }
*/
}

void FormImageEditor::mouseReleaseEvent(QMouseEvent *e)
{
/*    if (m_updateThread.m_currentButton==2)
        m_updateThread.m_currentButton = 0;
    else
        m_updateThread.m_currentButton = -1;
*/
    updateCharSet();
}

void FormImageEditor::wheelEvent(QWheelEvent *event)
{
    float f = event->delta()/100.0f;

    if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
        m_updateThread.m_zoom -=f*0.05;
        m_updateThread.m_zoom = std::min(m_updateThread.m_zoom, 1.0f);
        m_updateThread.m_zoom = std::max(m_updateThread.m_zoom, 0.1f);
        float t = 0.0f;
        m_updateThread.m_zoomCenter = (m_updateThread.m_zoomCenter*t + (1-t)*m_updateThread.m_currentPos);//*(2-2*m_zoom);
        Data::data.redrawOutput = true;

        m_grid.CreateGrid(40,25,m_updateThread.m_gridColor,4, m_updateThread.m_zoom, QPoint(m_updateThread.m_zoomCenter.x(), m_updateThread.m_zoomCenter.y()));

    }
    else {
        m_toolBox.m_current->m_size +=f;
        m_toolBox.m_current->m_size = std::max(m_toolBox.m_current->m_size,1.0f);
        m_toolBox.m_current->m_size = std::min(m_toolBox.m_current->m_size,50.0f);
    }
//    if (m_updateThread.m_isPanning)

    Data::data.redrawOutput = true;
    Data::data.forceRedraw = true;
}

void FormImageEditor::keyPressEvent(QKeyEvent *e)
{
    TRSEDocument::keyPressEvent(e);
    //    qDebug() << (QApplication::keyboardModifiers() & Qt::ShiftModifier) << " + "  << rand()%100;
        if (e->key()==Qt::Key_Shift) {
            m_toolBox.m_current->m_type = 1;
            Data::data.redrawOutput = true;
            Data::data.forceRedraw = true;
        }

        if (!(QApplication::keyboardModifiers() & Qt::ControlModifier)) {
            if (e->key()==Qt::Key_D) {
                m_updateThread.m_zoomCenter.setX(m_updateThread.m_zoomCenter.x() + 1);
                emit onImageMouseEvent();
                Data::data.forceRedraw = true;
                Data::data.Redraw();
            }
            if (e->key()==Qt::Key_A) {
                m_updateThread.m_zoomCenter.setX(m_updateThread.m_zoomCenter.x() - 1);
                emit onImageMouseEvent();
                Data::data.forceRedraw = true;
                Data::data.Redraw();
            }
            if (e->key()==Qt::Key_W) {
                m_updateThread.m_zoomCenter.setY(m_updateThread.m_zoomCenter.y() - 1);
                emit onImageMouseEvent();
                Data::data.forceRedraw = true;
                Data::data.Redraw();
            }
            if (e->key()==Qt::Key_S) {
                m_updateThread.m_zoomCenter.setY(m_updateThread.m_zoomCenter.y() + 1);
                emit onImageMouseEvent();
                Data::data.forceRedraw = true;
                Data::data.Redraw();
            }
        }
        if (e->key()==Qt::Key_T) {
            MultiColorImage* mc = dynamic_cast<MultiColorImage*>(m_work.m_currentImage->m_image);
            if (mc!=nullptr) {
                mc->ForceBackgroundColor(0,3);
            }
        }

        if (!ui->tblData->hasFocus() && !(QApplication::keyboardModifiers() & Qt::ControlModifier)) {
            m_work.m_currentImage->m_image->StoreData(ui->tblData);
            m_work.m_currentImage->m_image->KeyPress(e);
            QStringList lst = m_projectIniFile->getStringList("data_header_"+m_currentFileShort);

            m_work.m_currentImage->m_image->BuildData(ui->tblData, lst);

        }


        FillCMBColors();

        updateCharSet();

        if (e->key()==Qt::Key_W && (QApplication::keyboardModifiers() & Qt::ControlModifier))
//            Data::data.requestCloseWindow = true;
            emit requestCloseWindow();

        if (e->key()==Qt::Key_Z && QApplication::keyboardModifiers() & Qt::ControlModifier) {
            m_work.m_currentImage->Undo();

        }
        if (e->key()==Qt::Key_M && !(QApplication::keyboardModifiers() & Qt::ControlModifier)) {
            bool is = !m_work.m_currentImage->m_image->isMultiColor();
            m_work.m_currentImage->m_image->setMultiColor(is);
            ui->chkDisplayMulticolor->setChecked(is);

            Data::data.Redraw();
        }

        if (e->key() == Qt::Key_Z  && !(QApplication::keyboardModifiers() & Qt::ControlModifier)) {
                ui->chkGrid->setChecked(!ui->chkGrid->isChecked());
                //ui->lblGrid->setVisible(ui->chkGrid->isChecked());
                m_updateThread.m_drawGrid=!m_updateThread.m_drawGrid;

        }
        if (e->key() == Qt::Key_X) {
            m_work.m_currentImage->m_image->renderPathGrid =!m_work.m_currentImage->m_image->renderPathGrid;
            ui->chkBackgroundArea->setChecked(m_work.m_currentImage->m_image->renderPathGrid);
        }

        emit onImageMouseEvent();
        Data::data.forceRedraw = true;
        Data::data.Redraw();

}

void FormImageEditor::keyReleaseEvent(QKeyEvent *e)
{
    if (e->key()==Qt::Key_Shift) {
        m_toolBox.m_current->m_type = 0;
        Data::data.redrawOutput = true;
        Data::data.forceRedraw = true;
    }

}

void FormImageEditor::UpdateImage()
{
//    return;
//    m_updateThread.m_
   // m_updateThread.m_pixMapImage.fill(QColor(0,255,0));


    ui->lblImage->setVisible(true);
    ui->lblImage->setScaledContents(true);
//        ui->lblImage->setPixmap(m_updateThread.m_pixMapImage.scaled(320, 320, Qt::IgnoreAspectRatio, Qt::FastTransformation));
    ui->lblImage->setPixmap(m_updateThread.m_pixMapImage.scaled(ui->lblImage->size().width(), m_work.m_currentImage->m_image->m_height, Qt::IgnoreAspectRatio, Qt::FastTransformation));
//    ui->lblImage->setPixmap(m_updateThread.m_pixMapImage.scaled(16, 16, Qt::IgnoreAspectRatio, Qt::FastTransformation));
  //  ui->lblImage->setPixmap(m_updateThread.m_pixMapImage);
    ui->lblImage->setMaximumHeight(ui->lblImage->size().width()/(320/200.0));


    m_documentIsChanged = ui->lblImage->m_imageChanged;

    if (!ui->tblData->hasFocus())
        ui->lblImage->setFocus();



    QString currentChar = m_work.m_currentImage->m_image->GetCurrentDataString();
    ui->lblPosition->setText("Position: " +
                             QString::number(m_updateThread.m_currentPosInImage.x()) + ", " +
                             QString::number(m_updateThread.m_currentPosInImage.y()) + currentChar);


    UpdateGrid();

    if (Data::data.redrawFileList) {
        //m_work.UpdateListView(ui->lstImages);
        Data::data.redrawFileList = false;
    }


}

void FormImageEditor::UpdateGrid()
{
    m_grid.CreateGrid(40,25,m_updateThread.m_gridColor,4, m_updateThread.m_zoom, QPoint(m_updateThread.m_zoomCenter.x(), m_updateThread.m_zoomCenter.y()));
    m_grid.ApplyToLabel(ui->lblGrid);

}

void FormImageEditor::Load(QString filename)
{
/*    QString f = "Image Files (*." + LImageIO::m_fileExtension + ")";
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Open Image"), m_iniFile->getString("project_path"), f);
    if (filename=="")
        return;
*/

    LImage* img = LImageIO::Load(filename);
    if (img==nullptr)
        return;
    m_work.New(img, filename);


    PrepareImageTypeGUI();
    if (dynamic_cast<ImageLevelEditor*>(img)!=nullptr || dynamic_cast<C64FullScreenChar*>(img)!=nullptr)
        img->LoadCharset(m_projectIniFile->getString("charset_"+m_currentFileShort),0);
    updateCharSet();

    Data::data.redrawFileList = true;
    Data::data.Redraw();
    UpdatePalette();
    FillCMBColors();

    m_work.m_currentImage->m_image->BuildData(ui->tblData, m_projectIniFile->getStringList("data_header"));

    m_imageEffects.Init(m_work.m_currentImage->m_image);

    //ui->lblImageName->setText(m_currentFileShort);

    ui->cmbEffect->clear();
    ui->cmbEffect->addItems(m_imageEffects.getStringList());

    ui->chkDisplayMulticolor->setChecked(m_work.m_currentImage->m_image->isMultiColor());

    QString s = "";
    QStringList lst = m_projectIniFile->getStringList("data_header_"+m_currentFileShort);

    for (QString q:lst)
        s+= q +",";
    s.remove(s.count()-1,1);
    ui->leHeaders->setText(s);

    m_work.m_currentImage->m_image->BuildData(ui->tblData,lst);


    if (dynamic_cast<LImageSprites*>(m_work.m_currentImage->m_image)!=nullptr) {
        Messages::messages.DisplayMessage(Messages::messages.OLD_SPRITE_FILE);
    }


    onImageMouseEvent();
    updateCharSet();


}

void FormImageEditor::Save(QString filename)
{
    m_work.m_currentImage->m_image->StoreData(ui->tblData);

    LImageIO::Save(filename,m_work.m_currentImage->m_image);
    QStringList lst;
    for (int i=0;i<ui->tblData->rowCount();i++) {
        if (ui->tblData->verticalHeaderItem(i)!=nullptr)
            lst<<ui->tblData->verticalHeaderItem(i)->text();
    }

    //QStringList lst = m_projectIniFile->getStringList("data_header_"+m_currentFileShort);

    m_projectIniFile->setStringList("data_header_"+m_currentFileShort,lst);
    m_projectIniFile->Save(m_projectIniFile->filename);
}

void FormImageEditor::FillImageEffect()
{
    if (m_currentImageEffect==nullptr)
        return;
    int row=0;
    //for (QWidget* qw : ui->grdEffectParams->items)
        QLayoutItem *item = nullptr;
        while ((item = ui->grdEffectParams->takeAt(0)) != nullptr) {
        delete item->widget();
        }
    //ui->grdEffectParams->removeWidget()
    for (QString key: m_currentImageEffect->m_params.keys()) {
        QLabel* lab= new QLabel(key);
        QLineEdit* le=new QLineEdit(QString::number(m_currentImageEffect->m_params[key]));

        m_imageEffectsLineEdits[key] = le;
        ui->grdEffectParams->addWidget(lab,row,0);
        ui->grdEffectParams->addWidget(le,row,1);


        row++;

    }

}

void FormImageEditor::FillToImageParams()
{
    for (QString key: m_imageEffectsLineEdits.keys()) {
//        qDebug() << key << m_imageEffectsLineEdits[key]->text();
        if (m_currentImageEffect->m_params.contains(key))
            m_currentImageEffect->m_params[key] = m_imageEffectsLineEdits[key]->text().toFloat();
    }
}

void FormImageEditor::SelectCharacter(int idx)
{
    m_work.m_currentImage->m_image->SetCurrentType(LImage::WriteType::Character);
//    Data::data.currentColor = idx;
    m_work.m_currentImage->m_image->setCurrentChar(idx);

}

void FormImageEditor::on_cmbEffect_currentIndexChanged(int index)
{
    m_currentImageEffect = m_imageEffects.m_effects[index];
    FillImageEffect();
}

Ui::Formimageeditor *FormImageEditor::getUi() const
{
    return ui;
}

QLabel* FormImageEditor::getLabelImage()
{
    return ui->lblImage;
}

void FormImageEditor::UpdateCurrentMode()
{
    ui->lblMode->setText(m_work.m_currentImage->m_image->GetCurrentModeString());
}
void FormImageEditor::UpdatePalette()
{
    if (m_work.m_currentImage==nullptr)
        return;
    LColorList* l = &m_work.m_currentImage->m_image->m_colorList;
    //if (m_currentColorList!=l)
    //{
        l->CreateUI(ui->layoutColorsEdit_3,1);
        l->FillComboBox(ui->cmbBackgroundMain_3);
        l->FillComboBox(ui->cmbBorderMain_3);
        l->FillComboBox(ui->cmbMC1);
        l->FillComboBox(ui->cmbMC2);
        m_currentColorList = l;
    //}

    if (m_work.m_currentImage==nullptr)
        return;

    if (m_work.m_currentImage->m_image==nullptr)
        return;

    ui->cmbMC1->setCurrentIndex(m_work.m_currentImage->m_image->m_extraCols[1]);
    ui->cmbMC2->setCurrentIndex(m_work.m_currentImage->m_image->m_extraCols[2]);

    ui->btnExportCompressed->setVisible(m_work.m_currentImage->m_image->m_supports.compressedExport);
    ui->btnExportBin->setVisible(m_work.m_currentImage->m_image->m_supports.binarySave);
    ui->btnImportBin->setVisible(m_work.m_currentImage->m_image->m_supports.binaryLoad);

    ui->btnExportC->setVisible(m_work.m_currentImage->m_image->m_supports.exportc);
    ui->btnImportC->setVisible(m_work.m_currentImage->m_image->m_supports.importc);


    ui->btnExportKoala->setVisible(m_work.m_currentImage->m_image->m_supports.koalaExport);
    ui->btnImportKoala->setVisible(m_work.m_currentImage->m_image->m_supports.koalaImport);

    ui->btnExportMovie->setVisible(m_work.m_currentImage->m_image->m_supports.movieExport);

    ui->cmbMC1->setVisible(m_work.m_currentImage->m_image->m_supports.displayMC1);
    ui->cmbMC2->setVisible(m_work.m_currentImage->m_image->m_supports.displayMC2);
    ui->cmbBorderMain_3->setVisible(m_work.m_currentImage->m_image->m_supports.displayForeground);
    ui->layoutColorsEdit_3->setEnabled(m_work.m_currentImage->m_image->m_supports.displayColors);

    ui->lblTimeStamp->setVisible(m_work.m_currentImage->m_image->m_supports.displayTimestamp);
    ui->leTimeStamp->setVisible(m_work.m_currentImage->m_image->m_supports.displayTimestamp);

    m_work.m_currentImage->m_image->ApplyColor();


//    ui->btnLoad->setVisible(m_work.m_currentImage->m_image->m_supports.flfLoad);
//    ui->btnSave->setVisible(m_work.m_currentImage->m_image->m_supports.flfSave);
//    ui->btnExportAsm->setVisible(m_work.m_currentImage->m_image->m_supports.asmExport);

}

void FormImageEditor::FillCMBColors()
{
    ui->cmbBackgroundMain_3->setCurrentIndex(m_work.m_currentImage->m_image->m_background);
    ui->cmbMC1->setCurrentIndex(m_work.m_currentImage->m_image->m_extraCols[1]);
    ui->cmbMC2->setCurrentIndex(m_work.m_currentImage->m_image->m_extraCols[2]);

}

void FormImageEditor::resizeEvent(QResizeEvent *event)
{
    ui->lblImage->setVisible(true);
    ui->lblGrid->setGeometry(ui->lblImage->geometry());
    ui->lblGrid->repaint();
   // qDebug() <<
//    qDebug() << ui->lblImage->geometry();
    //  ui->lblImage->setVisible(false);
}

void FormImageEditor::mouseMoveEvent(QMouseEvent *e)
{
    /*  if (m_work==nullptr)
              return;
          if (m_imgLabel==nullptr)
              return;
          if (m_work==nullptr)
              return;
          if (m_quit)
              return;
          if (m_work->m_currentImage==nullptr)
              return;
      */



}


void FormImageEditor::on_btnExportAsm_clicked()
{

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Export Multicolor Assembler image"), m_projectIniFile->getString("project_path"),
                                                    tr("Bin (*.bin);"));


//    m_work.m_currentImage->m_image->ExportAsm(fileName);
    MultiColorImage* mi = (MultiColorImage*)dynamic_cast<MultiColorImage*>(m_work.m_currentImage->m_image);

    if (mi==nullptr)
        return;

    //fileName.remove(".bin");

    mi->ExportAsm(fileName);
    //    mi->ExportRasBin(fileName, "");
}

void FormImageEditor::on_btnGenerate_clicked()
{
    FillToImageParams();
    m_imageEffects.Render(ui->cmbEffect->currentText());
    Data::data.Redraw();
    Data::data.forceRedraw = true;


}


void FormImageEditor::on_btnFlipVert_clicked()
{
    m_work.m_currentImage->m_image->FlipVertical();

    Data::data.forceRedraw = true;
    Data::data.Redraw();
    onImageMouseEvent();

}

void FormImageEditor::on_btnFlipHorisontal_clicked()
{
    m_work.m_currentImage->m_image->FlipHorizontal();

    Data::data.forceRedraw = true;
    Data::data.Redraw();
    onImageMouseEvent();

}



/*void FormImageEditor::on_btnSave_clicked()
{
    QString filename = m_work.m_currentImage->m_fileName;

    if (filename=="")
        filename = QFileDialog::getSaveFileName(this,
                                                tr("Save Image"), m_iniFile->getString("project_path"), ("Image Files (*." + LImageIO::m_fileExtension + ")"));
    if (filename=="")
        return;

    LImageIO::Save(filename,m_work.m_currentImage->m_image);



}
*/
void FormImageEditor::on_chkGrid_clicked(bool checked)
{
//    if (checked)
//    ui->lblGrid->setVisible(checked);
    m_updateThread.m_drawGrid=!m_updateThread.m_drawGrid;
    Data::data.Redraw();
    onImageMouseEvent();

}

void FormImageEditor::on_chkBackgroundArea_clicked(bool checked)
{
    m_work.m_currentImage->m_image->renderPathGrid = checked;
    Data::data.forceRedraw = true;
}




void FormImageEditor::on_btnNew_clicked()
{
    m_work.m_currentImage->m_image->Clear();
    Data::data.Redraw();
    Data::data.forceRedraw = true;
}

void FormImageEditor::on_btnExportImage_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Image"), m_iniFile->getString("project_path"), tr("Image Files (*.png *.jpg *.bmp )"));

    if (fileName == "")
        return;



    m_updateThread.m_tmpImage->save(fileName);


}


void FormImageEditor::on_lstImages_clicked(const QModelIndex &index)
{
    m_work.SetImage(index.row());
    Data::data.redrawFileList = true;
    UpdatePalette();

    onImageMouseEvent();

}

void FormImageEditor::on_btnImport_clicked()
{
    DialogImport* di = new DialogImport(this);
    di->Initialize(m_work.m_currentImage->m_imageType->type, m_work.m_currentImage->m_image->m_colorList.m_type, m_work.m_currentImage->m_image);
    di->exec();
    if (di->m_ok) {
        m_work.m_currentImage->m_image->CopyFrom(di->m_image);
        m_work.m_currentImage->m_image->m_colorList.m_list = di->m_image->m_colorList.m_list;
        UpdatePalette();
        FillCMBColors();
        Data::data.redrawOutput = true;
    }
    onImageMouseEvent();

}

void FormImageEditor::on_btnCharsetFull_clicked()
{
    CharsetImage* ci = dynamic_cast<CharsetImage*>(m_work.m_currentImage->m_image);
    if (ci==nullptr)
        return;

    ci->m_currentMode = CharsetImage::Mode::FULL_IMAGE;
    Data::data.forceRedraw = true;
    UpdateCurrentMode();
    onImageMouseEvent();
}

void FormImageEditor::on_btnCharset1x1_clicked()
{
    CharsetImage* ci = dynamic_cast<CharsetImage*>(m_work.m_currentImage->m_image);
    if (ci==nullptr)
        return;

    m_updateThread.m_zoom = 1.0;
    ci->m_currentMode = CharsetImage::Mode::CHARSET1x1;
    Data::data.forceRedraw = true;
    onImageMouseEvent();

}

void FormImageEditor::on_btnCharset2x2_clicked()
{
    CharsetImage* ci = dynamic_cast<CharsetImage*>(m_work.m_currentImage->m_image);
    if (ci==nullptr)
        return;

    m_updateThread.m_zoom = 1.0;
    ci->m_currentMode = CharsetImage::Mode::CHARSET2x2;
    Data::data.forceRedraw = true;
    UpdateCurrentMode();
    onImageMouseEvent();
}

void FormImageEditor::on_btnCharset2x2Repeat_clicked()
{
    CharsetImage* ci = dynamic_cast<CharsetImage*>(m_work.m_currentImage->m_image);
    if (ci==nullptr)
        return;
    m_updateThread.m_zoom = 1.0;

    ci->m_currentMode = CharsetImage::Mode::CHARSET2x2_REPEAT;
    UpdateCurrentMode();
    Data::data.forceRedraw = true;
    onImageMouseEvent();

}

void FormImageEditor::on_btnCharsetCopy_clicked()
{
    m_work.m_currentImage->m_image->CopyChar();
}

void FormImageEditor::on_btnCharsetPaste_clicked()
{
    m_work.m_currentImage->AddUndo();
    m_work.m_currentImage->m_image->PasteChar();
    Data::data.forceRedraw = true;
    onImageMouseEvent();
    updateCharSet();
    Data::data.Redraw();

}

/*void FormImageEditor::on_btnTiff_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Tiled Tiff"), m_projectIniFile->getString("project_path"), tr("Image Files (*.tif *.tiff )"));

    if (fileName == "")
        return;


    LImageTiff *tif = (LImageTiff*)LImageFactory::Create(LImage::Tiff, LColorList::TIFF);
    tif->Initialize(640,480);
    tif->LoadTiff(fileName);
//    tif->m_type = LImage::Tiff;
    m_work.New(tif, fileName);
    Data::data.redrawFileList = true;
    Data::data.Redraw();
    UpdatePalette();

}
*/

/*
void FormImageEditor::on_btnSaveAs_clicked()
{

    QString filename = QFileDialog::getSaveFileName(this,
                                                tr("Save Image"), m_iniFile->getString("project_path"), ("Image Files (*." + LImageIO::m_fileExtension + ")"));
    if (filename=="")
        return;
    m_work.m_currentImage->m_fileName  =filename;

    LImageIO::Save(filename,m_work.m_currentImage->m_image);

}

*/



void FormImageEditor::updateCharSet()
{
    UpdateCurrentMode();
    CharsetImage* charmap = m_work.m_currentImage->m_image->getCharset();
    ImageLevelEditor* le = dynamic_cast<ImageLevelEditor*>(m_work.m_currentImage->m_image);
    if (le!=nullptr && charmap==nullptr) {
        Messages::messages.DisplayMessage(Messages::messages.CHARSET_WARNING);
        return;
    }
    if (charmap == nullptr)
        return;

    QVector<QPixmap> maps;
    charmap->ToQPixMaps(maps);



  /*  Util::clearLayout(ui->grdChars);
    int i=0;
    int j=0;
    for (int cnt=0;cnt<maps.count();cnt++) {
        QPushButton* item = new QPushButton(this);
        item->setAutoFillBackground(true);
        item->setText("");
        item->setIcon(maps[cnt]);
        QObject::connect( item, &QPushButton::clicked,  [=](){ SelectCharacter(cnt);} );

        ui->grdChars->addWidget(item, j,i);

        i++;
        if (i>=8) {
            i=0;
            j++;
        }
    }
*/
//   ui->lstCharMap->setViewMode(QListView::IconMode);
   ui->lstCharMap->setColumnCount(40);
   ui->lstCharMap->setRowCount(1+maps.count()/40);
    int cnt=0;
    int j=0;
    int i=0;
    int size=32;
    ui->lstCharMap->setIconSize(QSize(size,size));
    for (int i=0;i<40;i++) {
        ui->lstCharMap->setColumnWidth(i,size);
        //ui->lstCharMap->setCol
    }
    QStringList lst;
    for (int i=0;i<40;i++)
         lst<<"";

    ui->lstCharMap->setHorizontalHeaderLabels(lst);

    int kk= 0;
    for (int k=0;k<maps.count();k++) {

        QPixmap q = maps[k];

        QTableWidgetItem *itm = ui->lstCharMap->item(j,i);
        if (itm == nullptr) {
            itm = new QTableWidgetItem(q,nullptr,Qt::DisplayRole);
            ui->lstCharMap->setItem(j,i,itm);

        }
        //qDebug() << kk;
        itm->setIcon(q);
        itm->setData(Qt::UserRole, kk);
        cnt++;
        i++;
        kk++;
        if (i>=40) {
            i=0;
            j++;
           // kk+=40-8;

        }

       // kk=kk%255;

    }
//    ui->lstCharMap->
//    int size = (ui->lstCharMap->rect().width()-ui->lstCharMap->spacing())/8;
  //  ui->lstCharMap->setIconSize(QSize(size,size));

    onImageMouseEvent();
}

void FormImageEditor::updateSingleCharSet()
{
    UpdateCurrentMode();
    CharsetImage* charmap = m_work.m_currentImage->m_image->getCharset();
    ImageLevelEditor* le = dynamic_cast<ImageLevelEditor*>(m_work.m_currentImage->m_image);
    if (le!=nullptr && charmap==nullptr) {
        Messages::messages.DisplayMessage(Messages::messages.CHARSET_WARNING);
        return;
    }
    if (charmap == nullptr)
        return;
    if (charmap->m_currencChar<0)
        return;
    QPixmap pmap = charmap->ToQPixMap(charmap->m_currencChar);

    int kk= 0;
    int i = charmap->m_currencChar/(int)40;
    int j = charmap->m_currencChar%40;
    QTableWidgetItem *itm = ui->lstCharMap->item(i,j);
    if (itm!=nullptr)
        itm->setIcon(pmap);


}





void FormImageEditor::PrepareImageTypeGUI()
{
    // Only display "load charset" for levels
    SetButton(ui->btnLoadCharmap,LImage::GUIType::btnLoadCharset);
    SetButton(ui->btnCharset1x1,LImage::GUIType::btn1x1);
    SetButton(ui->btnCharset2x2,LImage::GUIType::btn2x2);
    SetButton(ui->btnCharset2x2Repeat,LImage::GUIType::btn2x2repeat);
    SetButton(ui->btnCharsetCopy,LImage::GUIType::btnCopy);
    SetButton(ui->btnCharsetPaste,LImage::GUIType::btnPaste);
    SetButton(ui->btnFlipHorisontal,LImage::GUIType::btnFlipH);
    SetButton(ui->btnFlipVert,LImage::GUIType::btnFlipV);
    SetButton(ui->btnCharsetFull,LImage::GUIType::btnEditFullCharset);

    int idx=0;
//    qDebug() << m_work.m_currentImage->m_image->m_GUIParams;
    if (m_work.m_currentImage->m_image->m_GUIParams[LImage::GUIType::tabCharset]=="") {
        ui->tabMain->removeTab(1);
        idx++;
    }
    if (m_work.m_currentImage->m_image->m_GUIParams[LImage::GUIType::tabSprites]=="") {
        ui->tabMain->removeTab(2-idx);
        idx++;
    }
    else {
        ui->tabMain->setTabText(2-idx,m_work.m_currentImage->m_image->m_GUIParams[LImage::GUIType::tabSprites]);

    }
    if (m_work.m_currentImage->m_image->m_GUIParams[LImage::GUIType::tabLevels]=="") {
        ui->tabMain->removeTab(3-idx);
        idx++;
    }



    if (m_work.m_currentImage->m_image->m_GUIParams[LImage::GUIType::tabData]==""){
            ui->tabMain->removeTab(4-idx);
            idx++;
        }
    if (m_work.m_currentImage->m_image->m_GUIParams[LImage::GUIType::tabEffects]==""){
            ui->tabMain->removeTab(5-idx);
            idx++;
        }



}

void FormImageEditor::SetButton(QPushButton *btn, LImage::GUIType type)
{
    if (m_work.m_currentImage->m_image->m_GUIParams[type]=="")
        btn->setVisible(false);
    else
        btn->setText(m_work.m_currentImage->m_image->m_GUIParams[type]);

}

void FormImageEditor::PrepareClose()
{
    ui->lblImage->CancelAll();
}

void FormImageEditor::SetMCColors()
{
    int a = ui->cmbMC1->currentIndex();
    int b = ui->cmbMC2->currentIndex();
    int back = ui->cmbBackgroundMain_3->currentIndex();

    m_work.m_currentImage->m_image->SetColor(back, 0);
    m_work.m_currentImage->m_image->SetColor(a, 1);
    m_work.m_currentImage->m_image->SetColor(b, 2);

    updateCharSet();
    emit onImageMouseEvent();

    Data::data.Redraw();
    Data::data.forceRedraw = true;

}

void FormImageEditor::UpdateLevels()
{
//    qDebug() << "ICONS!";
    ImageLevelEditor* le = dynamic_cast<ImageLevelEditor*>(m_work.m_currentImage->m_image);
    if (le==nullptr)
        return;

    QVector<QPixmap> icons = le->CreateIcons();


    Util::clearLayout(ui->gridLevels);
//    ui->gridLevels->set
    for (int i=0;i<le->m_meta.m_sizex;i++)
        for (int j=0;j<le->m_meta.m_sizey;j++) {
            QPushButton* l = new QPushButton();
            l->setText("");
            QIcon icon(icons[j + i*le->m_meta.m_sizey]);
            l->setIcon(icon);
            //l->setScaledContents(true);
            l->setIconSize(QSize(64,64));
            ui->gridLevels->addWidget(l, j,i);

            QObject::connect( l, &QPushButton::clicked,  [=](){
                m_work.m_currentImage->m_image->StoreData(ui->tblData);
                le->SetLevel(QPoint(i,j));
                m_work.m_currentImage->m_image->BuildData(ui->tblData,m_projectIniFile->getStringList("data_header_"+m_currentFileShort));
                Data::data.Redraw();
                Data::data.forceRedraw = true;
                onImageMouseEvent();

            }

            );

        }



}

void FormImageEditor::GenericExportImage(QString type, QString ext)
{
    if (m_work.m_currentImage->m_image->m_exportParams.keys().count()!=0) {
        DialogExport* de = new DialogExport();
        de->Init(m_work.m_currentImage->m_image);
        de->exec();

        if (!de->isOk)
            return;

    }
    QString ttr  = "Export " + type.toLower() + " file";
    QString f = type+" ( *."+ext+" )";

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    ttr.toStdString().c_str(), m_projectPath,
                                                    f);


//    m_work.m_currentImage->m_image->ExportAsm(fileName);
//    MultiColorImage* mi = (MultiColorImage*)dynamic_cast<MultiColorImage*>(m_work.m_currentImage->m_image);

    if (fileName=="")
        return;

    if (QFile::exists(fileName))
        QFile::remove(fileName);
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    if (ext=="bin")
        m_work.m_currentImage->m_image->ExportBin(file);
    if (ext=="koa")
        m_work.m_currentImage->m_image->ExportKoa(file);
    if (ext=="trm")
        m_work.m_currentImage->m_image->ExportMovie(file);
    if (file.isOpen())
        file.close();


}

void FormImageEditor::GenericImportImage(QString type, QString ext)
{

    QString f = type+" ( *."+ext+" )";
    QString ttr  = "Import " + type.toLower() + " file";
    QString filename = QFileDialog::getOpenFileName(this,
        ttr.toStdString().c_str(), m_projectPath, f);
    if (filename=="")
        return;


    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    if (ext=="bin")
        m_work.m_currentImage->m_image->ImportBin(file);
    if (ext=="koa")
        m_work.m_currentImage->m_image->ImportKoa(file);
    if (ext=="c")
        m_work.m_currentImage->m_image->ImportC(file);

    file.close();

//    SetMCColors();

    Data::data.redrawFileList = true;
    Data::data.Redraw();
    UpdatePalette();

}

void FormImageEditor::UpdateSpriteImages()
{
/*    LImageSprites2* img = dynamic_cast<LImageSprites2*>(m_work.m_currentImage->m_image);
    if (img==nullptr)
        return;
*/
//    ui->lblSprite1->setPixmap(m_updateThread.m_pixMapImage.scaled(320, 320, Qt::IgnoreAspectRatio, Qt::FastTransformation));
//    QImage m_tmpImage = QImage(img->m_width,img->m_height,QImage::Format_ARGB32);
    QImage m_tmpImage = QImage(m_work.m_currentImage->m_image->m_width,m_work.m_currentImage->m_image->m_height,QImage::Format_ARGB32);

    if (m_keepSpriteChar.count()==0)
        m_keepSpriteChar.resize(3);

    QImage empty = QImage(1,1,QImage::Format_ARGB32);
    empty.fill(QColor(0,0,0,0));
    QPixmap pixmapEmpty;
    pixmapEmpty.convertFromImage(empty);
    QIcon emptyIcon(pixmapEmpty);

    LImageContainer* cont = dynamic_cast<LImageContainer*>(m_work.m_currentImage->m_image);

    ui->leTimeStamp->setText(QString::number(cont->getExtraData(0)));

    if (cont==nullptr)
        return;


    int keep = cont->m_current;
    int sx = 40*2.5;
    int sy = 32*2.5;
    for (int i=0;i<3;i++) {
        int k = i-1+cont->m_current;
        QPushButton* l = nullptr;
            if (i==0)
                l = ui->lblSprite1;
            if (i==2)
                l = ui->lblSprite3;

        if (k<0) {
            l->setIcon(emptyIcon);
            m_keepSpriteChar[i] = -1;
        }


        if (k == cont->m_current) {
            QIcon butt(m_updateThread.m_pixMapImage.scaled(sx, sy, Qt::IgnoreAspectRatio, Qt::FastTransformation));
            ui->lblSprite2->setIcon(butt);
            ui->lblSprite2->setIconSize(QSize( sx,sy));
        }
        else

            if (m_keepSpriteChar[i]!=k) {
                if ((k>=0) && (k<m_work.m_currentImage->m_image->getContainerCount())) {
                    m_keepSpriteChar[i] = k;
                    cont->m_current = k;
                    m_work.m_currentImage->m_image->ToQImage(m_work.m_currentImage->m_image->m_colorList, m_tmpImage, 1, QPoint(0.5, 0.5));
                    cont->m_current = keep;
                    QPixmap pixmap = QPixmap();
                    pixmap.convertFromImage(m_tmpImage);
                    pixmap = pixmap.scaled(sx, sy, Qt::IgnoreAspectRatio, Qt::FastTransformation);
                    QIcon ButtonIcon(pixmap);
                        if (l!=nullptr)
//                            l->setPixmap(pixmap.scaled(40, 32, Qt::IgnoreAspectRatio, Qt::FastTransformation));
                        l->setIcon(ButtonIcon);
                        l->setIconSize(QSize(sx,sy));

                }
                else {
//                    l->setPixmap(pixmapEmpty.scaled(40, 32, Qt::IgnoreAspectRatio, Qt::FastTransformation));
                    l->setIcon(emptyIcon);
                    m_keepSpriteChar[i] = -1;
                }

            }


    }

//    img->m_currencChar = keep;
//    ui->lblSprite2->setPixmap(m_updateThread.m_pixMapImage.scaled(40, 32, Qt::IgnoreAspectRatio, Qt::FastTransformation));



}


void FormImageEditor::on_btnExportKoala_clicked()
{
    GenericExportImage("Koala", "koa");

}

void FormImageEditor::on_btnImportKoala_clicked()
{
    GenericImportImage("Koala", "koa");

}



void FormImageEditor::on_btnImportBin_clicked()
{
    GenericImportImage("Binary", "bin");
}

void FormImageEditor::on_btnExportBin_clicked()
{

    GenericExportImage("Binary", "bin");

}

void FormImageEditor::on_tabMain_currentChanged(int index)
{
    if (index==1)
        m_work.m_currentImage->m_image->SetCurrentType(LImage::WriteType::Color);
    if (index==2)
        UpdateLevels();
//    if (index==2)
//        m_work.m_currentImage->m_image->SetCurrentType(LImage::WriteType::Character);
}

void FormImageEditor::on_btnLoadCharmap_clicked()
{


    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Character map"), m_projectIniFile->getString("project_path"), tr("Binary Files (*.bin, *.flf )"));

    if (fileName == "")
        return;


    m_work.m_currentImage->m_image->LoadCharset(fileName,0);
    m_projectIniFile->setString("charset_"+m_currentFileShort, fileName);
    m_projectIniFile->Save();
    updateCharSet();

}

void FormImageEditor::on_lstCharMap_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous)
{

    if (current==nullptr)
        return;
    int idx = current->data(Qt::UserRole).toInt();
    m_work.m_currentImage->m_image->SetCurrentType(LImage::WriteType::Character);
   // Data::data.currentColor = idx;
//    qDebug() << idx << Util::numToHex(idx);
    m_work.m_currentImage->m_image->setCurrentChar(idx);

    Data::data.Redraw();
    Data::data.forceRedraw = true;
    onImageMouseEvent();


}

void FormImageEditor::Destroy()
{
    //delete m_work.m_currentImage;

    //m_updateThread.m_imgLabel = nullptr;
//    m_updateThread.
    //m_updateThread.m_work = nullptr;
//    m_updateThread.m_ = nullptr;

}


void FormImageEditor::on_cmbMC1_activated(int index)
{
    SetMCColors();
    emit onImageMouseEvent();

}

void FormImageEditor::on_cmbMC2_activated(int index)
{
    SetMCColors();
    emit onImageMouseEvent();

}

void FormImageEditor::on_cmbBackgroundMain_3_activated(int index)
{
    m_work.m_currentImage->m_image->setBackground(index);
    SetMCColors();
    emit onImageMouseEvent();
    Data::data.redrawOutput = true;

}

void FormImageEditor::on_btnResizeData_clicked()
{
    ImageLevelEditor* img = dynamic_cast<ImageLevelEditor*>(m_work.m_currentImage->m_image);
    if (img==nullptr)
        return;


    DialogNewImage* dResize = new DialogNewImage(this);
    dResize->Initialize(m_work.getImageTypes());
    dResize->setModal(true);
    dResize->SetResizeMeta(img->m_meta);

    dResize->exec();
    if (dResize->retVal!=-1) {
        img->Resize(dResize->m_meta);
        QStringList lst = m_projectIniFile->getStringList("data_header_"+m_currentFileShort);
        m_work.m_currentImage->m_image->BuildData(ui->tblData,lst);
    }



    UpdatePalette();

    delete dResize;


}

void FormImageEditor::on_chkDisplayMulticolor_stateChanged(int arg1)
{
    m_work.m_currentImage->m_image->setMultiColor(ui->chkDisplayMulticolor->isChecked());
    Data::data.Redraw();
    onImageMouseEvent();

}

void FormImageEditor::on_leHeaders_editingFinished()
{
    QStringList lst = ui->leHeaders->text().remove(" ").split(",");

    m_work.m_currentImage->m_image->BuildData(ui->tblData, lst);
    m_projectIniFile->setStringList("data_header_"+m_currentFileShort,lst);
    m_projectIniFile->Save(m_projectIniFile->filename);


}

void FormImageEditor::on_tblData_cellChanged(int row, int column)
{
    //m_work.m_currentImage->m_image->StoreData(ui->tblData);

}

void FormImageEditor::on_btnHelpImage_clicked()
{
    DialogImageHelp* dih = new DialogImageHelp(this);
    dih->show();

}


void FormImageEditor::on_btnNewSprite_clicked()
{
    m_work.m_currentImage->AddUndo();

    LImageContainer* cont = dynamic_cast<LImageContainer*>(m_work.m_currentImage->m_image);

    if (cont==nullptr)
        return;

    cont->AddNew(ui->cmbSpriteX->currentText().toInt(),ui->cmbSpriteY->currentText().toInt());
    onImageMouseEvent();

}

void FormImageEditor::on_btnDeleteSprite_clicked()
{
    LImageContainer* cont = dynamic_cast<LImageContainer*>(m_work.m_currentImage->m_image);
    if (cont==nullptr)
        return;
    m_work.m_currentImage->AddUndo();

    cont->Delete();

    onImageMouseEvent();
    for (int i=0;i<3;i++)
        m_keepSpriteChar[i]=-1;

    UpdateSpriteImages();

}

void FormImageEditor::on_btnCopySprite_clicked()
{
    m_work.m_currentImage->m_image->CopyChar();
    onImageMouseEvent();
}

void FormImageEditor::on_btnPasteSprite_clicked()
{
    m_work.m_currentImage->AddUndo();
    m_work.m_currentImage->m_image->PasteChar();
    onImageMouseEvent();

}

void FormImageEditor::on_btnFlipXSprite_clicked()
{
    m_work.m_currentImage->AddUndo();
    m_work.m_currentImage->m_image->FlipVertical();
    onImageMouseEvent();

}

void FormImageEditor::on_btnFlipYSprite_clicked()
{
    m_work.m_currentImage->AddUndo();
    m_work.m_currentImage->m_image->FlipHorizontal();
    onImageMouseEvent();
}

void FormImageEditor::on_sliderX_actionTriggered(int action)
{
/*    int v = ui->sliderX->value();
    if (v>60) {
        m_work.m_currentImage->m_image->Transform(1,0);
//        ui->sliderX->setValue(50);

    }
    if (v>40) {
        m_work.m_currentImage->m_image->Transform(-1,0);
  //      ui->sliderX->setValue(50);

    }
    onImageMouseEvent();
*/
}

void FormImageEditor::on_btnPanLeft_clicked()
{
    m_work.m_currentImage->AddUndo();
    m_work.m_currentImage->m_image->Transform(-1,0);
    onImageMouseEvent();
}

void FormImageEditor::on_btnPanRight_clicked()
{
    m_work.m_currentImage->AddUndo();
    m_work.m_currentImage->m_image->Transform(1,0);
    onImageMouseEvent();

}

void FormImageEditor::on_btnPanUp_clicked()
{
    m_work.m_currentImage->AddUndo();
    m_work.m_currentImage->m_image->Transform(0,-1);
    onImageMouseEvent();

}

void FormImageEditor::on_btnPanDown_clicked()
{
    m_work.m_currentImage->AddUndo();
    m_work.m_currentImage->m_image->Transform(0,1);
    onImageMouseEvent();

}

void FormImageEditor::on_lblSprite1_clicked()
{
    LImageContainer* cont = dynamic_cast<LImageContainer*>(m_work.m_currentImage->m_image);

    if (cont==nullptr)
        return;

    cont->Prev();
    onImageMouseEvent();

}

void FormImageEditor::on_lblSprite3_clicked()
{
    LImageContainer* cont = dynamic_cast<LImageContainer*>(m_work.m_currentImage->m_image);

    if (cont==nullptr)
        return;

    cont->Next();
    onImageMouseEvent();
}

void FormImageEditor::on_btnMoveSpriteLeft_clicked()
{
    LImageContainer* img = dynamic_cast<LImageContainer*>(m_work.m_currentImage->m_image);
    if (img==nullptr)
        return;

    img->Shift(-1);

    for (int i=0;i<3;i++)
        m_keepSpriteChar[i]=-1;



    onImageMouseEvent();
}

void FormImageEditor::on_btnMoveSpriteRight_clicked()
{
    LImageContainer* img = dynamic_cast<LImageContainer*>(m_work.m_currentImage->m_image);
    if (img==nullptr)
        return;
    img->Shift(1);

    for (int i=0;i<3;i++)
        m_keepSpriteChar[i]=-1;

    onImageMouseEvent();
}

void FormImageEditor::on_btnSpriteMulticolor_clicked()
{
    LImageSprites2* img = dynamic_cast<LImageSprites2*>(m_work.m_currentImage->m_image);
    if (img==nullptr)
        return;

    img->ToggleSpriteMulticolor();
    onImageMouseEvent();

}

void FormImageEditor::on_btnImportRom_clicked()
{

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Import ROM charset", "This will import the standard C64 ROM charset and overwrite your current data.\n\nAre you sure?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No)
        return;

    QString fileName = ":resources/character.rom";
    m_work.m_currentImage->m_image->LoadCharset(fileName,0);

    m_projectIniFile->setString("charset_"+m_currentFileShort, fileName);
    m_projectIniFile->Save();
    updateCharSet();
    m_work.m_currentImage->m_image->setMultiColor(false);
    ui->chkDisplayMulticolor->setChecked(false);
    Data::data.Redraw();

    SetMCColors();



}

void FormImageEditor::on_btnExportMovie_clicked()
{
    GenericExportImage("TRSE Movie","trm");
}

void FormImageEditor::on_leTimeStamp_textChanged(const QString &arg1)
{
    char c = ui->leTimeStamp->text().toInt();
    LImageContainer* le = dynamic_cast<LImageContainer*>(m_work.m_currentImage->m_image);
    if (le==nullptr)
        return;
    le->setExtraData(0,c);
}

void FormImageEditor::on_btnImportC_clicked()
{
    GenericImportImage("c", "c");

}

void FormImageEditor::on_cmbZoomLevel_activated(const QString &arg1)
{
    QString s = ui->cmbZoomLevel->currentText();
    s = s.replace("x","");
    m_updateThread.m_zoom = 1.0f/s.toFloat();
    onImageMouseEvent();

}

void FormImageEditor::on_btnExportCompressed_clicked()
{
    if (m_work.m_currentImage->m_image->m_exportParams.keys().count()!=0) {
        DialogExport* de = new DialogExport();
        de->Init(m_work.m_currentImage->m_image);
        de->exec();

        if (!de->isOk)
            return;

    }
    QString ext = "bin";
    QString ttr  = "Export compressed image";
    QString f = "Binary( *."+ext+" )";

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    ttr.toStdString().c_str(), m_projectPath,
                                                    f);


//    m_work.m_currentImage->m_image->ExportAsm(fileName);
//    MultiColorImage* mi = (MultiColorImage*)dynamic_cast<MultiColorImage*>(m_work.m_currentImage->m_image);

    fileName = fileName.remove(".bin");

    if (fileName=="")
        return;

    QString f1 = fileName + "_screen.bin";
    QString f2 = fileName + "_charset.bin";

    if (QFile::exists(f1))
        QFile::remove(f1);

    if (QFile::exists(f2))
        QFile::remove(f2);


    m_work.m_currentImage->m_image->ExportCompressed(f1,f2);


}

void FormImageEditor::on_btnPalette_clicked()
{
    DialogColors* dc = new DialogColors();
    dc->Initialize(&m_work.m_currentImage->m_image->m_colorList, m_projectPath);
    dc->exec();
    UpdatePalette();
}
