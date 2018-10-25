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
    m_grid.CreateGrid(40,25,m_updateThread.m_gridColor,4, 1, QPoint(0,0));
}


void FormImageEditor::onImageMouseEvent()
{
//    emit EmitMouseEvent();
    m_updateThread.RunContents();
    UpdateImage();

//    updateCharSet();

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

//    m_updateThread.m_


   // m_updateThread.m_pixMapImage.fill(QColor(0,255,0));
    ui->lblImage->setVisible(true);

    ui->lblImage->setPixmap(m_updateThread.m_pixMapImage.scaled(320, 320, Qt::IgnoreAspectRatio, Qt::FastTransformation));


    m_documentIsChanged = ui->lblImage->m_imageChanged;

    if (!ui->tblData->hasFocus())
        ui->lblImage->setFocus();


    QString currentChar = "   Current char: $" + QString::number(m_work.m_currentImage->m_image->m_currencChar,16);
    currentChar+=" (" + QString::number(m_work.m_currentImage->m_image->m_currencChar) + ")";
    ui->lblPosition->setText("Position: " +
                             QString::number(m_updateThread.m_currentPosInImage.x()) + ", " +
                             QString::number(m_updateThread.m_currentPosInImage.y()) + currentChar);


    m_grid.ApplyToLabel(ui->lblGrid);
    if (Data::data.redrawFileList) {
        //m_work.UpdateListView(ui->lstImages);
        Data::data.redrawFileList = false;
    }

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
    img->LoadCharset(m_projectIniFile->getString("charset_"+m_currentFileShort));
    updateCharSet();

    Data::data.redrawFileList = true;
    Data::data.Redraw();
    UpdatePalette();
    FillCMBColors();

    m_work.m_currentImage->m_image->BuildData(ui->tblData, m_projectIniFile->getStringList("data_header"));

    m_imageEffects.Init(m_work.m_currentImage->m_image);

    ui->lblImageName->setText(m_currentFileShort);

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

    ui->btnExportBin->setVisible(m_work.m_currentImage->m_image->m_supports.binarySave);
    ui->btnImportBin->setVisible(m_work.m_currentImage->m_image->m_supports.binaryLoad);

    ui->btnExportKoala->setVisible(m_work.m_currentImage->m_image->m_supports.koalaExport);
    ui->btnImportKoala->setVisible(m_work.m_currentImage->m_image->m_supports.koalaImport);

    ui->cmbMC1->setVisible(m_work.m_currentImage->m_image->m_supports.displayMC1);
    ui->cmbMC2->setVisible(m_work.m_currentImage->m_image->m_supports.displayMC2);
    ui->cmbBorderMain_3->setVisible(m_work.m_currentImage->m_image->m_supports.displayForeground);
    ui->layoutColorsEdit_3->setEnabled(m_work.m_currentImage->m_image->m_supports.displayColors);



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
    di->Initialize(m_work.m_currentImage->m_imageType->type, m_work.m_currentImage->m_image->m_colorList.m_type);
    di->exec();
    if (di->m_ok) {
        m_work.m_currentImage->m_image->CopyFrom(di->m_image);
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
    m_work.m_currentImage->m_image->PasteChar();
    Data::data.forceRedraw = true;
    onImageMouseEvent();

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
   ui->lstCharMap->setRowCount(maps.count()/40);
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

    emit EmitMouseEvent();
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
    if (m_work.m_currentImage->m_image->m_GUIParams[LImage::GUIType::tabCharset]=="") {
        ui->tabMain->removeTab(3);
        idx++;
    }
    if (m_work.m_currentImage->m_image->m_GUIParams[LImage::GUIType::tabLevels]=="") {
        ui->tabMain->removeTab(4-idx);
        idx++;
    }
    if (m_work.m_currentImage->m_image->m_GUIParams[LImage::GUIType::tabData]==""){
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

    if (QFile::exists(fileName))
        QFile::remove(fileName);
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    if (ext=="bin")
        m_work.m_currentImage->m_image->ExportBin(file);
    if (ext=="koa")
        m_work.m_currentImage->m_image->ExportKoa(file);
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
    file.close();

    Data::data.redrawFileList = true;
    Data::data.Redraw();
    UpdatePalette();

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
    qDebug() << index;
    if (index==1)
        m_work.m_currentImage->m_image->SetCurrentType(LImage::WriteType::Color);
    if (index==3)
        UpdateLevels();
//    if (index==2)
//        m_work.m_currentImage->m_image->SetCurrentType(LImage::WriteType::Character);
}

void FormImageEditor::on_btnLoadCharmap_clicked()
{

    MultiColorImage* mci = dynamic_cast<MultiColorImage*>(m_work.m_currentImage->m_image);

    if (mci==nullptr)
        return;


    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Character map"), m_projectIniFile->getString("project_path"), tr("Binary Files (*.bin, *.flf )"));

    if (fileName == "")
        return;


    mci->LoadCharset(fileName);
    m_projectIniFile->setString("charset_"+m_currentFileShort, fileName);
    m_projectIniFile->Save();
    updateCharSet();

}

void FormImageEditor::on_lstCharMap_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous)
{

    int idx = current->data(Qt::UserRole).toInt();
    m_work.m_currentImage->m_image->SetCurrentType(LImage::WriteType::Character);
   // Data::data.currentColor = idx;
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

}

void FormImageEditor::on_cmbMC2_activated(int index)
{
    SetMCColors();

}

void FormImageEditor::on_cmbBackgroundMain_3_activated(int index)
{
    m_work.m_currentImage->m_image->setBackground(index);
    SetMCColors();
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

