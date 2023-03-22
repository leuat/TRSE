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
#include "source/dialogselectcharset.h"
#include <QTimer>

FormImageEditor::FormImageEditor(QWidget *parent) :
    TRSEDocument(parent),
    ui(new Ui::Formimageeditor)
{
    ui->setupUi(this);
//    m_toolBox.Initialize(ui->lyToolbox_3,width());
    m_toolBox.Initialize(ui->ToolboxLayout,height()*1.8);

    UpdatePalette();
    m_grid.Initialize(320,200);
    m_fileExtension = "flf";
    Data::data.currentColor=1;
    //m_grid.ApplyToLabel(ui->lblGrid);
    updateCharSet();


    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    m_windowSize = screenGeometry.size();
    //ui->lblGrid->setVisible(ui->chkGrid->isChecked());


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

    ui->lblImageQt->m_work = &m_work;
    ui->lblImageQt->setMouseTracking(true);
    ui->lblImageQt->m_updateThread = &m_updateThread;

    ui->lstCharMap->InitDoc(this);

//    void EmitMouseMove();


    m_updateThread.SetCurrentImage(&m_work, &m_toolBox);


    installEventFilter(this);
//    setEnabled(true);
    ui->tblData->setItemDelegate(new ByteDelegate());
  //  ui->splitter->setStretchFactor(1, 0);

  ui->splitter->setSizes(QList<int>() << 1000<<500);
  m_lastSizes = ui->splitter->sizes();
//  ui->splitter->s
//  ui->splitter->setCollapsible(0, false);
 // ui->splitter->setCollapsible(1, false);
}

void FormImageEditor::InitDocument(WorkerThread *t, QSharedPointer<CIniFile> ini, QSharedPointer<CIniFile> iniProject) {
    m_iniFile = ini;
    m_projectIniFile = iniProject;
    m_updateThread.m_grid = &m_grid;

    m_painterType = m_iniFile->getdouble("image_painter")==0?OpenGL:QtPaint;

    if (m_work.m_currentImage!=nullptr) {
        bool is = m_work.m_currentImage->m_image->isMultiColor();
        ui->chkDisplayMulticolor->setChecked(is);
        ui->lblName->setText(LImage::TypeToString(m_work.m_currentImage->m_image->m_type));

    }
//    qDebug() <<((C64FullScreenChar*)m_work.m_currentImage->m_image)->m_charWidth;
//    UpdateGrid();
    ui->chkBackgroundArea->setVisible(false);

    if (m_painterType==QtPaint) {
        delete ui->lblImage;
        connect(ui->lblImageQt, SIGNAL(EmitMouseMove(QEvent*)), this, SLOT(onImageMouseEvent(QEvent*)));
        connect(ui->lblImageQt, SIGNAL(EmitMouseRelease()), this, SLOT(onImageMouseReleaseEvent()));
        connect(ui->lblImageQt, SIGNAL(EmitSwapDisplayMode()), this, SLOT(onSwapDisplayMode()));
    }
    else {
        delete ui->lblImageQt;

        connect(ui->lblImage, SIGNAL(EmitMouseMove(QEvent*)), this, SLOT(onImageMouseEvent(QEvent*)));
        connect(ui->lblImage, SIGNAL(EmitMouseRelease()), this, SLOT(onImageMouseReleaseEvent()));
        connect(ui->lblImage, SIGNAL(EmitSwapDisplayMode()), this, SLOT(onSwapDisplayMode()));
    }

//    QObject::connect(ui->splitter, SIGNAL(splitterMoved(int,int)), this, SLOT(UpdateAspect()));
//    QObject::connect(&Data::data, SIGNAL(EmitPenChanged()), this,SLOT(onImageMouseEvent()));
//    QObject::disconnect(&Data::data, SIGNAL(EmitPenChanged()), this,SLOT(onPenChanged()));


    if (m_work.m_currentImage!=nullptr) {

        QObject::connect(m_work.m_currentImage->m_image, SIGNAL(emitImportRom()), this, SLOT(ImportROM()));
        m_work.m_currentImage->m_image->InitPens();
        m_work.m_currentImage->m_image->ReInitialize();


    }

}


void FormImageEditor::onImageMouseEvent(QEvent* e = nullptr)
{
//    emit EmitMouseEvent();
    m_updateThread.RunContents();
    ui->splitter->setCollapsible(0,true);
 //   ui->ImageLayout->set

    if (e!=nullptr && e->type()==QEvent::Wheel)
        wheelEvent((QWheelEvent*)e);
    UpdateImage();
    if (dynamic_cast<LImageContainer*>(m_work.m_currentImage->m_image)!=nullptr)
        UpdateSpriteImages();
    if (dynamic_cast<C64FullScreenChar*>(m_work.m_currentImage->m_image)!=nullptr)
        UpdateSpriteImages();
    if (dynamic_cast<LImageMetaChunk*>(m_work.m_currentImage->m_image)!=nullptr)
        UpdateSpriteImages();

    updateSingleCharSet();


    ui->lblSpriteInfo->setText(m_work.m_currentImage->m_image->getSpriteInfo());
    //showDetailCharButtons(m_prefMode!=CharsetImage::Mode::FULL_IMAGE);

    // This will update the current cell
//    if (dynamic_cast<LImageMetaChunk*>(m_work.m_currentImage->m_image)==nullptr)
  //      if (dynamic_cast<ImageLevelEditor*>(m_work.m_currentImage->m_image)==nullptr)


//    if (dynamic_cast<CharsetImage*>(m_work.m_currentImage->m_image)!=nullptr)
    if (m_work.m_currentImage->m_image->m_updateCharsetPosition)
    if (ui->lstCharMap->currentItem()!=nullptr) {
        int i = ui->lstCharMap->currentItem()->data(Qt::UserRole).toInt();
        LImage* charmap = m_work.m_currentImage->m_image->getCharset();

        if (charmap->m_currentChar!=i) {
/*            i = charmap->m_currentChar;
            int x = i%charmap->m_charWidthDisplay;
            int y = (i)/charmap->m_charWidthDisplay;
            ui->lstCharMap->setCurrentCell(y,x);*/
            UpdateCurrentCell();
        }

    }
    showDetailCharButtons();



    //    qDebug() << m_work.m_currentImage->m_image->m_footer.isFullscreen();
}

void FormImageEditor::onPenChanged()
{

//    qDebug() << "PEN CHANGED A "  << Data::data.currentColor;
   // qDebug() << LImage::TypeToString(m_work.m_currentImage->m_image->m_type);
    m_work.m_currentImage->m_image->InitPens();
  //  qDebug() << "PEN CHANGED B "  << Data::data.currentColor;
    m_work.m_currentImage->m_image->m_colorList.CreateUI(ui->layoutColorsEdit_3,1,m_windowSize);
  //  qDebug() << "PEN CHANGED C " << Data::data.currentColor;
//    if (m_work.m_currentImage->m_image->Copy)
//    qDebug() << "INITING PENS";

    onImageMouseEvent();

    updateCharSet();
    Data::data.Redraw();
//    onImageMouseEvent();

}

void FormImageEditor::onImageMouseReleaseEvent()
{
    AbstractImageEditor* a = dynamic_cast<AbstractImageEditor*>(getCurrentPainter());

    updateCharSet();
    if (a->m_prevButton==2  && (QApplication::keyboardModifiers() & Qt::ControlModifier) && GetFooterData(LImageFooter::POS_DISPLAY_CHAR)==0) {
        SelectFromLeftClick();
    }

}

void FormImageEditor::SelectFromLeftClick()
{
    m_prefMode = m_keepMode;
    m_work.m_currentImage->m_image->m_currentChar =
            m_work.m_currentImage->m_image->getCharAtPos(
                (QPoint(m_updateThread.m_currentPos.x(),m_updateThread.m_currentPos.y())),
                m_updateThread.m_zoom,m_updateThread.m_zoomCenter);
    //showDetailCharButtons(true);
    SetSingleCharsetEdit();
    Data::data.forceRedraw = true;
    onImageMouseEvent();
}


FormImageEditor::~FormImageEditor()
{
    delete ui;
}

void FormImageEditor::wheelEvent(QWheelEvent *event)
{
    float f = event->angleDelta().y()/100.0f;
    if ((QApplication::keyboardModifiers() & Qt::ShiftModifier)  && (event->buttons() != Qt::RightButton)) {
        m_updateThread.m_zoom -=f*0.05;
        m_updateThread.m_zoom = std::min(m_updateThread.m_zoom, 1.0f);
        m_updateThread.m_zoom = std::max(m_updateThread.m_zoom, 0.1f);
        float t = 0.2f;
        QPointF pos = (m_updateThread.m_currentPosInImage);// + m_updateThread.m_zoomCenter ;
        m_updateThread.m_zoomCenter = t*pos + (1-t)*m_updateThread.m_zoomCenter;//*(2-2*m_zoom);
        UpdateGrid();
        emit onImageMouseEvent();
        Data::data.forceRedraw = true;
        Data::data.Redraw();

//        m_grid.CreateGrid(40,25,m_updateThread.m_gridColor,4, m_updateThread.m_zoom, QPoint(m_updateThread.m_zoomCenter.x(), m_updateThread.m_zoomCenter.y()));
//        m_grid.CreateGrid(m_work.m_currentImage->m_image->m_charWidthDisplay,m_work.m_currentImage->m_image->m_charHeightDisplay,m_updateThread.m_gridColor,4, m_updateThread.m_zoom, QPoint(m_updateThread.m_zoomCenter.x(), m_updateThread.m_zoomCenter.y()));

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

        // select a tab with CTRL + 1 ... 5
        if (QApplication::keyboardModifiers() & Qt::ControlModifier)
        {
            if (e->key()==Qt::Key_1) { ui->tabMain->setCurrentIndex(0); }
            if (e->key()==Qt::Key_2) { ui->tabMain->setCurrentIndex(1); }
            if (e->key()==Qt::Key_3) { ui->tabMain->setCurrentIndex(2); }
            if (e->key()==Qt::Key_4) { ui->tabMain->setCurrentIndex(3); }
            if (e->key()==Qt::Key_5) { ui->tabMain->setCurrentIndex(4); }
        }

        if (e->key()==Qt::Key_Question) {
            QPointF pos = (  m_updateThread.m_currentPos - QPointF(0.5, 0.5) -m_updateThread.m_zoomCenter)*m_updateThread.m_zoom + m_updateThread.m_zoomCenter ;
            auto colorIndex = m_work.m_currentImage->m_image->getPixel(pos.x(),pos.y());
            Data::data.currentColor = colorIndex;
            //            m_work.m_currentImage->m_image->m_colorList.m_

//            m_work.m_currentImage->m_image->m_colorList.CreateUI(ui->layoutColorsEdit_3,1,m_windowSize);
        }

        // toggle toolbar panels on and off
        if (e->key()==Qt::Key_F6) {
            ui->tabMain->setVisible(!ui->tabMain->isVisible());
            FixSplitting(ui->tabMain, ui->tabMain->isVisible() || ui->Tools_2->isVisible());
        }

        if (e->key()==Qt::Key_F5) {
            ui->Tools_2->setVisible(!ui->Tools_2->isVisible());
            FixSplitting(ui->Tools_2, ui->tabMain->isVisible() || ui->Tools_2->isVisible());
        }
        // toggle
        if (e->key()==Qt::Key_Backslash || e->key()==Qt::Key_F7) {

            if ( ( !(QApplication::keyboardModifiers() & Qt::ControlModifier) ) )
            {

                // no Ctrl key, toggle from one to the other
                if (ui->Tools_2->isVisible() ) {
                    ui->Tools_2->setVisible(false);
                    ui->tabMain->setVisible(true);
                }
                else {
                    ui->Tools_2->setVisible(true);
                    ui->tabMain->setVisible(false);
                }

            }
            else
            {

                // Ctrl key, toggle both on and both off
                if (!ui->Tools_2->isVisible() ) {
                    ui->Tools_2->setVisible(true);
                    ui->tabMain->setVisible(true);
                }
                else {
                    ui->Tools_2->setVisible(false);
                    ui->tabMain->setVisible(false);
                }

            }

            FixSplitting(ui->Tools_2, ui->tabMain->isVisible() || ui->Tools_2->isVisible());
            FixSplitting(ui->tabMain, ui->tabMain->isVisible() || ui->Tools_2->isVisible());
        }

        if ((QApplication::keyboardModifiers() & Qt::ControlModifier)) {
            if (e->key()==Qt::Key_C)
                on_btnCharsetCopy_clicked();
            if (e->key()==Qt::Key_V)
                on_btnCharsetPaste_clicked();
        }

        if (!(QApplication::keyboardModifiers() & Qt::ControlModifier)) {
            int j = 0;
            if (QApplication::keyboardModifiers() & Qt::ShiftModifier)
                j = 4;
            float scale = m_updateThread.m_zoom*(1+j)*4;
            if (e->key()==Qt::Key_C) {
                OpenSelectCharset();
                return;
            }

            if (e->key()==Qt::Key_P) {
                ui->chkPaintSeparately->click();
                emit onImageMouseEvent();
                return;
            }

            // navigation
            if (e->key()==Qt::Key_D) {
                m_updateThread.m_zoomCenter.setX(m_updateThread.m_zoomCenter.x() + 1*scale);
                emit onImageMouseEvent();
                Data::data.forceRedraw = true;
                Data::data.Redraw();
            }
            if (e->key()==Qt::Key_A) {
                m_updateThread.m_zoomCenter.setX(m_updateThread.m_zoomCenter.x() - 1*scale);
                emit onImageMouseEvent();
                Data::data.forceRedraw = true;
                Data::data.Redraw();
            }
            if (e->key()==Qt::Key_W) {
                m_updateThread.m_zoomCenter.setY(m_updateThread.m_zoomCenter.y() - 1*scale);
                emit onImageMouseEvent();
                Data::data.forceRedraw = true;
                Data::data.Redraw();
            }
            if (e->key()==Qt::Key_S) {
                m_updateThread.m_zoomCenter.setY(m_updateThread.m_zoomCenter.y() + 1*scale);
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
        bool pressed = false;

        if (!ui->tblData->hasFocus() && !(QApplication::keyboardModifiers() & Qt::ControlModifier)) {

            m_work.m_currentImage->m_image->StoreData(ui->tblData);
            m_work.m_currentImage->m_image->KeyPress(e);
            QStringList lst = m_projectIniFile->getStringList("data_header_"+m_currentFileShort);
            pressed = true;
            m_work.m_currentImage->m_image->BuildData(ui->tblData, lst);
            UpdateLevels();

        }
        if (!pressed)
        if (!((QApplication::keyboardModifiers() & Qt::ControlModifier)))
            m_work.m_currentImage->m_image->KeyPress(e);


//        FillCMBColors();

        updateCharSet();

/*        if (e->key()==Qt::Key_W && (QApplication::keyboardModifiers() & Qt::ControlModifier))
//            Data::data.requestCloseWindow = true;
            emit requestCloseWindow();
*/
        if (e->key()==Qt::Key_Z && QApplication::keyboardModifiers() & Qt::ControlModifier) {
            m_work.m_currentImage->Undo();

        }
        if (e->key()==Qt::Key_M && !(QApplication::keyboardModifiers() & Qt::ControlModifier)) {
            bool is = !m_work.m_currentImage->m_image->isMultiColor();
            m_work.m_currentImage->m_image->setMultiColor(is);
            ui->chkDisplayMulticolor->setChecked(is);

            Data::data.Redraw();
            onImageMouseEvent();
        }

        if (e->key() == Qt::Key_Z  && !(QApplication::keyboardModifiers() & Qt::ControlModifier)) {
                ui->chkGrid->setChecked(!ui->chkGrid->isChecked());
                //ui->lblGrid->setVisible(ui->chkGrid->isChecked());
                m_updateThread.m_drawGrid=!m_updateThread.m_drawGrid;
                SetFooterData(LImageFooter::POS_DISPLAY_GRID,m_updateThread.m_drawGrid);

                Data::data.Redraw();
                emit onImageMouseEvent();

        }
        if (e->key() == Qt::Key_X) {
            m_work.m_currentImage->m_image->renderPathGrid =!m_work.m_currentImage->m_image->renderPathGrid;
            ui->chkBackgroundArea->setChecked(m_work.m_currentImage->m_image->renderPathGrid);
        }


        UpdateCurrentCell();
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

    QElapsedTimer et;
    et.start();

    if (m_painterType==OpenGL) {
        QImage txt = m_updateThread.m_pixMapImage.toImage();

//        ui->lblImage->setVisible(true);
        ui->lblImage->setTexture(txt,*m_updateThread.m_grid->m_qImage);

        m_documentIsChanged = ui->lblImage->m_imageChanged;

        if (!ui->tblData->hasFocus())
            ui->lblImage->setFocus();

    } else
    {

//        ui->lblImageQt->setVisible(true);
        //ui->lblImage->setTexture(txt,*m_updateThread.m_grid->m_qImage);
//        ui->lblImageQt->setPixmap(QPixmap::fromImage(txt));
        m_documentIsChanged = ui->lblImageQt->m_imageChanged;


        ui->lblImageQt->setScaledContents(true);
        ui->lblImageQt->setPixmap(m_updateThread.m_pixMapImage.scaled(ui->lblImageQt->size().width()-16, ui->lblImageQt->size().height()-16, Qt::IgnoreAspectRatio, Qt::FastTransformation));
//        ui->lblImage->setMaximumHeight(ui->lblImage->size().width()/(320/200.0));



        if (!ui->tblData->hasFocus())
            ui->lblImageQt->setFocus();

    }



    QString currentChar = m_work.m_currentImage->m_image->GetCurrentDataString();
    ui->lblPosition->setText("Position: " +
                             QString::number(m_updateThread.m_currentPosInImage.x()) + ", " +
                             QString::number(m_updateThread.m_currentPosInImage.y()) + currentChar);


    UpdateGrid();


    if (Data::data.redrawFileList) {
        Data::data.redrawFileList = false;
    }

}

void FormImageEditor::UpdateButtonIcons()
{
    if (GetFooterData(LImageFooter::POS_DISPLAY_CHAR)==0) // fullscreen
        ui->btnCharsetFull->setIcon(QPixmap(":resources/images/charset_icons/btn-1-FullView.png"));
    else
        ui->btnCharsetFull->setIcon(QPixmap(":resources/images/charset_icons/btn-2-CharView.png"));

    if (GetFooterData(LImageFooter::POS_CURRENT_DISPLAY_REPEAT)==0) // fullscreen
        ui->btnRepeating->setIcon(QPixmap(":resources/images/charset_icons/btn-3-RepeatTile.png"));
    else
        ui->btnRepeating->setIcon(QPixmap(":resources/images/charset_icons/btn-4-NoRepeatTile.png"));

    ui->btnCharsetFull->setIconSize(QSize(32,32));
    ui->btnRepeating->setIconSize(QSize(32,32));
}

void FormImageEditor::UpdateGrid()
{
    if (m_work.m_currentImage==nullptr)
        return;
/*    if (!m_updateThread.m_drawGrid)
        return;
*/
 //   qDebug() << xs << ys;
    m_grid.Initialize(m_updateThread.m_gridScale *m_work.m_currentImage->m_image->GetWidth(),
                      m_updateThread.m_gridScale*m_work.m_currentImage->m_image->GetHeight());
//    qDebug() << m_work.m_currentImage->m_image->m_scaleX;
    m_updateThread.CreateGrid();
//    m_grid.ApplyToLabel(ui->lblGrid);

}

void FormImageEditor::Initialize()
{
    //m_prefMode = CharsetImage::Mode::FULL_IMAGE;
    PrepareImageTypeGUI();
/*    if (QFile::exists(m_projectIniFile->getString("charset_"+m_currentFileShort))) {
        if (dynamic_cast<ImageLevelEditor*>(img)!=nullptr || dynamic_cast<C64FullScreenChar*>(img)!=nullptr ||dynamic_cast<LImageMetaChunk*>(img)!=nullptr)
            img->LoadCharset(m_projectIniFile->getString("charset_"+m_currentFileShort),0);

        updateCharSet();
    }
*/

    if (GetFooterData(LImageFooter::POS_CHARSET_WIDTH)!=0) {
        if (m_work.m_currentImage->m_image->getCharset()!=nullptr)
            m_work.m_currentImage->m_image->getCharset()->m_charWidthDisplay = GetFooterData(LImageFooter::POS_CHARSET_WIDTH);
//        SetFooterData(LImageFooter::POS_CHARSET_WIDTH, ui->cmbCharWidth->currentText().toInt());
        ui->cmbCharWidth->setCurrentText(QString::number(GetFooterData(LImageFooter::POS_CHARSET_WIDTH)));
    }


    Data::data.redrawFileList = true;
    Data::data.Redraw();


    int i = GetFooterData(LImageFooter::POS_D800_FIXED);
    if (i!=0) {
        ui->cmbFixedfD800->setCurrentIndex(i);
        m_work.m_currentImage->m_image->SetForceD800Color(i-1);
    }

    UpdatePalette();
    updateCharSet();
    FillCMBColors();

    ui->cmbNesPalette->clear();
    ui->cmbNesPalette->addItems(m_work.m_currentImage->m_image->getPaletteNames());
    if (ui->cmbBank->count()!=0)
        ui->cmbBank->clear();
    ui->cmbBank->addItems(m_work.m_currentImage->m_image->getBankNames());


    m_work.m_currentImage->m_image->BuildData(ui->tblData, m_projectIniFile->getStringList("data_header"));

    m_imageEffects.Init(m_work.m_currentImage->m_image);

    //ui->lblImageName->setText(m_currentFileShort);

    ui->cmbEffect->clear();
    ui->cmbEffect->addItems(m_imageEffects.getStringList());

//    ui->chkDisplayMulticolor->setChecked(m_work.m_currentImage->m_image->isMultiColor());

    QString s = "";
    QStringList lst = m_projectIniFile->getStringList("data_header_"+m_currentFileShort);

    for (QString q:lst)
        s+= q +",";
    s.remove(s.length()-1,1);
    ui->leHeaders->setText(s);

    ui->lblName->setText(LImage::TypeToString(m_work.m_currentImage->m_image->m_type));
    m_work.m_currentImage->m_image->BuildData(ui->tblData,lst);


    ui->chkGrid->setChecked(GetFooterData(LImageFooter::POS_DISPLAY_GRID));
    on_chkGrid_clicked(GetFooterData(LImageFooter::POS_DISPLAY_GRID));

    if (m_work.m_currentImage->m_image->m_type==LImage::Type::MultiColorBitmap ||
       m_work.m_currentImage->m_image->m_type==LImage::Type::HiresBitmap ||
//            m_work.m_currentImage->m_image->m_type==LImage::Type::LevelEditor ||
            m_work.m_currentImage->m_image->m_type==LImage::Type::CharMapMulticolor
            ) {

        ui->chkHybrid->setChecked(GetFooterData(LImageFooter::POS_DISPLAY_HYBRID));
        on_chkHybrid_clicked(GetFooterData(LImageFooter::POS_DISPLAY_HYBRID));
    }
    else ui->chkHybrid->setVisible(false);


    ui->chkPaintSeparately->setChecked(GetFooterData(LImageFooter::POS_DOUBLE_PAINT));
    on_chkPaintSeparately_stateChanged(GetFooterData(LImageFooter::POS_DOUBLE_PAINT));

//    m_prefMode = (CharsetImage::Mode)GetFooterData(LImageFooter::POS_CURRENT_MODE);
  //  m_keepMode = (CharsetImage::Mode)GetFooterData(LImageFooter::POS_KEEP_MODE);

    ui->cmbNesPalette->setCurrentIndex(GetFooterData(LImageFooter::POS_CURRENT_PALETTE));
    on_cmbNesPalette_currentIndexChanged(GetFooterData(LImageFooter::POS_CURRENT_PALETTE));
    on_cmbNesPalette_currentIndexChanged(GetFooterData(LImageFooter::POS_CURRENT_PALETTE));

    ui->cmbCharX->setCurrentIndex(GetFooterData(LImageFooter::POS_CURRENT_DISPLAY_X)-1);
    ui->cmbCharY->setCurrentIndex(GetFooterData(LImageFooter::POS_CURRENT_DISPLAY_Y)-1);

    ui->cmbTileStampX->setCurrentIndex(fmax(GetFooterData(LImageFooter::POS_CURRENT_STAMP_X),1.0f)-1);
    ui->cmbTileStampY->setCurrentIndex(fmax(GetFooterData(LImageFooter::POS_CURRENT_STAMP_Y),1.0f)-1);

//    qDebug() << GetFooterData(LImageFooter::POS_DISPLAY_MULTICOLOR);
    ui->chkDisplayMulticolor->setChecked(GetFooterData(LImageFooter::POS_DISPLAY_MULTICOLOR));
    on_chkDisplayMulticolor_stateChanged(GetFooterData(LImageFooter::POS_DISPLAY_MULTICOLOR));
//    if (!dynamic_cast<ImageLevelEditor*>(m_work.m_currentImage->m_image))
  //      m_work.m_currentImage->m_image->setMultiColor(ui->chkDisplayMulticolor->isChecked());



    int bank = GetFooterData(LImageFooter::POS_CURRENT_BANK);
//    ui->cmbBank->setCurrentIndex(0);
//    ui->cmbBank->setCurrentIndex(1);
//    ui->cmbBank->setCurrentIndex(0);
    ui->cmbBank->setCurrentIndex(bank);
//    m_work.m_currentImage->m_image->SetBank(bank);
//    on_cmbBank_currentIndexChanged(bank);
    //m_work.m_currentImage->m_image->m_currentBank = bank;

    //showDetailCharButtons(m_prefMode!=CharsetImage::Mode::FULL_IMAGE);
//    SetSingleCharsetEdit();

    QString gridSize = m_work.m_currentImage->m_image->getGridSize();
    if (gridSize!="0x0")
        ui->cbmGridSize->setCurrentText(gridSize);

    updateCharSet();

//    onImageMouseEvent();

    emit onImageMouseEvent();
    m_isInitialized = true;

    UpdateAspect();

    QTimer::singleShot(50, this, SLOT(InitAspect()));

    //for (int i=0;i<100;i++)
    //    Data::data.UpdatePens();

    QTimer::singleShot(50, this, SLOT(onPenChanged()));

//    m_work.m_currentImage->m_image->m_colorList.m_pens[1].

}



bool FormImageEditor::Load(QString filename)
{
/*    QString f = "Image Files (*." + LImageIO::m_fileExtension + ")";
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Open Image"), m_iniFile->getString("project_path"), f);
    if (filename=="")
        return;
*/
    if (!QFile::exists(filename))
        return false;
    LImage* img = LImageIO::Load(filename);
    if (img==nullptr)
        return false;

    if (!m_isInitialized)
       m_work.New(img, filename);
    else m_work.m_currentImage->m_image = img;
    m_currentFilename = filename;


//    qDebug() << "EXTRACOLS " << img->m_extraCols[0] <<img->m_extraCols[1] << img->m_extraCols[2];

    Initialize();
    return true;
}


void FormImageEditor::InitAspect()
{
//    if (m_projectIniFile->contains("aspect_ratio"))
  //      ui->cmbAspect->setCurrentIndex(m_projectIniFile->getdouble("aspect_ratio"));
//    qDebug() <<GetFooterData(LImageFooter::POS_ASPECT);
    ui->cmbAspect->setCurrentIndex(GetFooterData(LImageFooter::POS_ASPECT));
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
    ui->lblImageQt->m_imageChanged = false;
    ui->lblImage->m_imageChanged = false;
    m_documentIsChanged = false;

//    qDebug() << "SAVE";
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
    if (index==-1)
        return;
    m_currentImageEffect = m_imageEffects.m_effects[index];
    FillImageEffect();
}

Ui::Formimageeditor *FormImageEditor::getUi() const
{
    return ui;
}

/*GLWidget* FormImageEditor::getLabelImage()
{
    return ui->lblImage;
}
*/
void FormImageEditor::UpdateCurrentMode()
{
    if (m_work.m_currentImage==nullptr)
        return;
    ui->lblMode->setText(m_work.m_currentImage->m_image->GetCurrentModeString());
}
void FormImageEditor::UpdatePalette()
{
    if (m_work.m_currentImage==nullptr)
        return;
    if (m_work.m_currentImage->m_image == nullptr)
        return;

    LColorList* l = &m_work.m_currentImage->m_image->m_colorList;
    //if (m_currentColorList!=l)
    //{

    if (m_work.m_currentImage->m_image->m_supports.displayColors)
        l->CreateUI(ui->layoutColorsEdit_3,1, m_windowSize);

/*    l->FillComboBox(ui->cmbBackgroundMain_3);
    l->FillComboBox(ui->cmbBorderMain_3);
    l->FillComboBox(ui->cmbMC1);
    if (m_work.m_currentImage->m_image->m_colorList.m_type==LColorList::VIC20)
    l->FillComboBoxRestricted(ui->cmbMC2,0,8);
    else
        l->FillComboBox(ui->cmbMC2);

        */

    m_currentColorList = l;
    //}

    if (m_work.m_currentImage==nullptr)
        return;

    if (m_work.m_currentImage->m_image==nullptr)
        return;

//    ui->cmbMC1->setCurrentIndex(m_work.m_currentImage->m_image->m_extraCols[1]);
//    ui->cmbMC2->setCurrentIndex(m_work.m_currentImage->m_image->m_extraCols[2]);

    ui->btnExportCompressed->setVisible(m_work.m_currentImage->m_image->m_supports.compressedExport);
    ui->btnExportBin->setVisible(m_work.m_currentImage->m_image->m_supports.binarySave);
    ui->btnImportBin->setVisible(m_work.m_currentImage->m_image->m_supports.binaryLoad);



    ui->btnExportC->setVisible(m_work.m_currentImage->m_image->m_supports.exportc);
    ui->btnImportC->setVisible(m_work.m_currentImage->m_image->m_supports.importc);
    ui->lblFixedD800->setVisible(m_work.m_currentImage->m_image->m_supports.d800_limit);
    ui->cmbFixedfD800->setVisible(m_work.m_currentImage->m_image->m_supports.d800_limit);

    ui->btnExportKoala->setVisible(m_work.m_currentImage->m_image->m_supports.koalaExport);
    ui->btnImportKoala->setVisible(m_work.m_currentImage->m_image->m_supports.koalaImport);

    ui->btnExportMovie->setVisible(m_work.m_currentImage->m_image->m_supports.movieExport);

    ui->btnSelectDefaultClearItm->setVisible(m_work.m_currentImage->m_image->m_supports.displayDefaultClearButton);


    ui->lblTileStamp->setVisible(m_work.m_currentImage->m_image->m_supports.tilestamp);
    ui->cmbTileStampX->setVisible(m_work.m_currentImage->m_image->m_supports.tilestamp);
    ui->cmbTileStampY->setVisible(m_work.m_currentImage->m_image->m_supports.tilestamp);


/*    ui->cmbMC1->setVisible(m_work.m_currentImage->m_image->m_supports.displayMC1);
    ui->cmbMC2->setVisible(m_work.m_currentImage->m_image->m_supports.displayMC2);
    ui->cmbBorderMain_3->setVisible(m_work.m_currentImage->m_image->m_supports.displayForeground);
    */
    ui->layoutColorsEdit_3->setEnabled(m_work.m_currentImage->m_image->m_supports.displayColors);
    ui->cmbBank->setVisible(m_work.m_currentImage->m_image->m_supports.displayBank);
    if (!m_work.m_currentImage->m_image->m_supports.displayBank) {
        // NES stuff: turn off tiles, bank
        ui->lblBank->setVisible(false);
        ui->lblPalette->setVisible(false);
        ui->cmbNesPalette->setVisible(false);
    }


    ui->cmbNesPalette->setEnabled(m_work.m_currentImage->m_image->m_supports.nesPalette);

    ui->lblTimeStamp->setVisible(m_work.m_currentImage->m_image->m_supports.displayTimestamp);
    ui->leTimeStamp->setVisible(m_work.m_currentImage->m_image->m_supports.displayTimestamp);
/*    if (!m_work.m_currentImage->m_image->m_supports.displayCmbColors) {
        ui->cmbMC1->setVisible(false);
        ui->cmbMC2->setVisible(false);
        ui->cmbBorderMain_3->setVisible(false);
        ui->cmbBackgroundMain_3->setVisible(false);
    }
    */

    //m_work.m_currentImage->m_image->ApplyColor();


//    ui->btnLoad->setVisible(m_work.m_currentImage->m_image->m_supports.flfLoad);
//    ui->btnSave->setVisible(m_work.m_currentImage->m_image->m_supports.flfSave);
//    ui->btnExportAsm->setVisible(m_work.m_currentImage->m_image->m_supports.asmExport);

}

void FormImageEditor::FillCMBColors()
{
/*    ui->cmbBackgroundMain_3->setCurrentIndex(m_work.m_currentImage->m_image->m_background);
    ui->cmbMC1->setCurrentIndex(m_work.m_currentImage->m_image->m_extraCols[1]);
    ui->cmbMC2->setCurrentIndex(m_work.m_currentImage->m_image->m_extraCols[2]);
*/
}

void FormImageEditor::focusInEvent(QFocusEvent *)
{
}

void FormImageEditor::OpenSelectCharset()
{
    if (m_work.m_currentImage->m_image->getCharset()==nullptr)
        return;
    DialogSelectCharset* ds = new DialogSelectCharset(m_work.m_currentImage->m_image->getCharset());
    ds->exec();
    if (ds->result() == QDialog::Rejected) {
        return;
    }

    SelectCharacter(ds->m_char);
    SetSingleCharsetEdit();

    Data::data.Redraw();
    Data::data.forceRedraw = true;
    onImageMouseEvent();


}



void FormImageEditor::Reload()
{
    m_work.m_currentImage->m_image->onFocus();

}

bool FormImageEditor::eventFilter(QObject *ob, QEvent *e)
{
    /*if (e->type() == QEvent::Wheel) {
        qDebug() << "KEY EVENT "<<rand()%100 <<e;
        wheelEvent((QWheelEvent*)e);
//        return false;
    }*/
  //  return true;
    if(/*e->type() == QEvent::KeyPress || */e->type()==QEvent::ShortcutOverride) {
        const QKeyEvent *ke = static_cast<QKeyEvent *>(e);



        if (((ke->key() == Qt::Key_S) &&  ((QApplication::keyboardModifiers() & Qt::ControlModifier)))) {
            SaveCurrent();
        }



        if (!(QApplication::keyboardModifiers() & Qt::ShiftModifier)) {

            if(ke->key()== Qt::Key_Space) {
                onSwapDisplayMode();
                return true;
            }
/*            if ((ke->key() == Qt::Key_F)) {
                LImage* img = m_work.m_currentImage->m_image;
                for (int y=1;y<img->m_height;y+=2)
                    for (int x=0;x<img->m_width;x++) {
                        img->setPixel(x,y,0);
                    }
            }
*/
            if (ke->key() == Qt::Key_F1) {
                ui->cmbZoomLevel->setCurrentIndex(0);
                on_cmbZoomLevel_activated("1x");
                return true;
            }
            if (ke->key() == Qt::Key_F2) {
                ui->cmbZoomLevel->setCurrentIndex(1);
                on_cmbZoomLevel_activated("2x");
                return true;
            }
            if (ke->key() == Qt::Key_F3) {
                ui->cmbZoomLevel->setCurrentIndex(2);
                on_cmbZoomLevel_activated("4x");
                return true;
            }
            if (ke->key() == Qt::Key_F4) {
                ui->cmbZoomLevel->setCurrentIndex(3);
                on_cmbZoomLevel_activated("8x");
                return true;
            }
        }
        else
        {
            if (ke->key() == Qt::Key_F1) {
                ui->tabMain->setCurrentIndex(0);
            }
            if (ke->key() == Qt::Key_F2) {
                ui->tabMain->setCurrentIndex(1);
            }
            if (ke->key() == Qt::Key_F3) {
                ui->tabMain->setCurrentIndex(2);
            }
            if (ke->key() == Qt::Key_F4) {
                ui->tabMain->setCurrentIndex(3);
            }
            if (ke->key() == Qt::Key_F5) {
                ui->tabMain->setCurrentIndex(4);
            }
        }


        return false;
    }
    return QWidget::eventFilter(ob, e);
}

void FormImageEditor::resizeEvent(QResizeEvent *event)
{

    QWidget* w = getCurrentPainter();
//    w->setSizePolicy(QSizePolicy ::MinimumExpanding,QSizePolicy ::Expanding);

    w->setVisible(true);
    m_oldWidth = w->width();
//    UpdateAspect();
    SetMCColors();
    if (m_painterType == OpenGL)
        m_updateThread.m_displayImageWidth = ui->lblImage->width();
    else
        m_updateThread.m_displayImageWidth = ui->lblImageQt->width();
    //ui->lblGrid->setGeometry(ui->lblImage->geometry());
    //ui->lblGrid->repaint();
    // qDebug() <<
    //    qDebug() << ui->lblImage->geometry();
    //  ui->lblImage->setVisible(false);
}



/*void FormImageEditor::on_btnExportAsm_clicked()
{

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Export Multicolor Assembler image"), m_projectIniFile->getString("project_path"),
                                                    tr("Bin (*.bin);"));


    MultiColorImage* mi = (MultiColorImage*)dynamic_cast<MultiColorImage*>(m_work.m_currentImage->m_image);

    if (mi==nullptr)
        return;


    mi->ExportAsm(fileName);
}
*/
void FormImageEditor::on_btnGenerate_clicked()
{
    FillToImageParams();
    m_imageEffects.Render(ui->cmbEffect->currentText());
    Data::data.Redraw();
    Data::data.forceRedraw = true;


}

void FormImageEditor::showDetailCharButtons()
{
/*    if (m_work.m_currentImage!=nullptr)
    if (dynamic_cast<ImageLevelEditor*>(m_work.m_currentImage->m_image)!=nullptr)
        return;
*/
    bool doShow = (GetFooterData(LImageFooter::POS_DISPLAY_CHAR))==1 && m_work.m_currentImage->m_image->m_supports.displayCharOperations;



    ui->btnCharsetCopy->setVisible(doShow);
    ui->btnCharsetPaste->setVisible(doShow);
    ui->btnFlipVert->setVisible(doShow);
    ui->btnFlipHorisontal->setVisible(doShow);
    ui->btnRepeating->setVisible(doShow);


    if (dynamic_cast<ImageLevelEditor*>(m_work.m_currentImage->m_image)!=nullptr) {
        doShow = true;
    }
    ui->btnShiftUp->setVisible(doShow);
    ui->btnShiftDown->setVisible(doShow);
    ui->btnShiftLeft->setVisible(doShow);
    ui->btnShiftRight->setVisible(doShow);
    ui->cmbCharX->setVisible(doShow);
    ui->cmbCharY->setVisible(doShow);
    ui->lblTileSize->setVisible(doShow);

/*    if (doShow)
        ui->btnCharsetFull->setText("Full");
    else
        ui->btnCharsetFull->setText("Char");
        */
}


void FormImageEditor::on_btnFlipVert_clicked()
{
    m_work.m_currentImage->AddUndo();
    m_work.m_currentImage->m_image->FlipHorizontal();

    Data::data.forceRedraw = true;
    Data::data.Redraw();
    onImageMouseEvent();

}

void FormImageEditor::on_btnFlipHorisontal_clicked()
{
    m_work.m_currentImage->AddUndo();
    m_work.m_currentImage->m_image->FlipVertical();

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
    m_updateThread.m_drawGrid=checked;
    SetFooterData(LImageFooter::POS_DISPLAY_GRID,m_updateThread.m_drawGrid);

    UpdateGrid();
    Data::data.Redraw();
    Data::data.forceRedraw = true;
    emit onImageMouseEvent();
//    resizeEvent(nullptr);

}

void FormImageEditor::on_chkBackgroundArea_clicked(bool checked)
{
    m_work.m_currentImage->m_image->renderPathGrid = checked;
    Data::data.forceRedraw = true;
}




void FormImageEditor::on_btnNew_clicked()
{
    m_work.m_currentImage->m_image->Clear(0);
    Data::data.Redraw();
    Data::data.forceRedraw = true;
}

void FormImageEditor::on_btnExportImage_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Image"), m_iniFile->getString("project_path"), tr("Image Files (*.png *.jpg *.bmp )"));

    if (fileName == "")
        return;


    QImage img = *m_updateThread.m_tmpImage;
    QPoint size = m_work.m_currentImage->m_image->getActualPixelWidth();
/*    int wy = m_updateThread.m_tmpImage->width()*2
    if (img.width()==160) {

    }*/
    img = m_updateThread.m_tmpImage->scaled(size.x(), size.y(),
                                            Qt::IgnoreAspectRatio,Qt::FastTransformation);
    img.save(fileName);


}



void FormImageEditor::on_btnImport_clicked()
{
    DialogImport* di = new DialogImport(this);
    di->Initialize(m_work.m_currentImage->m_image->m_type, m_work.m_currentImage->m_image->m_colorList.m_type, m_work.m_currentImage->m_image);
    di->exec();
    if (di->m_ok) {

        m_work.m_currentImage->m_image->CopyFrom(di->m_image);
        m_work.m_currentImage->m_image->m_colorList.CopyFrom(&di->m_image->m_colorList);
        m_work.m_currentImage->m_image->SavePalette();

        UpdatePalette();
        FillCMBColors();
        m_work.m_currentImage->m_image->m_colorList.CreateUI(ui->layoutColorsEdit_3,1, m_windowSize);

        Data::data.redrawOutput = true;

    }
    onImageMouseEvent();

}

void FormImageEditor::on_btnCharsetFull_clicked()
{
    UpdateButtonIcons();
    m_work.m_currentImage->m_image->m_footer.toggle(LImageFooter::POS_DISPLAY_CHAR);

    getCurrentPainter()->setFocus();
    ui->lstCharMap->setCurrentItem(nullptr);

/*    CharsetImage* ci = dynamic_cast<CharsetImage*>(m_work.m_currentImage->m_image);
    if (ci==nullptr)
        return;


    ci->m_currentMode = CharsetImage::Mode::FULL_IMAGE;
    */
    Data::data.forceRedraw = true;
    UpdateCurrentMode();
    UpdateGrid();
    emit onImageMouseEvent();

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
    if (m_work.m_currentImage==nullptr)
        return;

//    return;

    UpdateCurrentMode();
    LImage* charmap = m_work.m_currentImage->m_image->getCharset();


    bool isLevelEditor = dynamic_cast<ImageLevelEditor*>(m_work.m_currentImage->m_image)!=nullptr;


//    ImageLevelEditor* le = dynamic_cast<ImageLevelEditor*>(m_work.m_currentImage->m_image);
/*    if (le!=nullptr && charmap==nullptr) {
        Messages::messages.DisplayMessage(Messages::messages.CHARSET_WARNING);
        return;
    }*/
    if (charmap == nullptr)
        return;

    ui->cmbCharWidth->setCurrentText(QString::number(charmap->m_charWidthDisplay));

    QVector<QPixmap> maps;
    charmap->ToQPixMaps(maps);
//    qDebug() << maps.count();
    ui->lstCharMap->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->lstCharMap->clearContents();


    int width = charmap->m_charWidthDisplay;
    int height = charmap->m_charHeightDisplay;
//   ui->lstCharMap->setViewMode(QListView::IconMode);
   ui->lstCharMap->setColumnCount(width);
//   ui->lstCharMap->setRowCount(1+maps.count()/width);
    int cnt=0;
    int j=0;
    int i=0;
    int size=32;
    ui->lstCharMap->setIconSize(QSize(size,size));
    for (int i=0;i<width;i++) {
        ui->lstCharMap->setColumnWidth(i,size);
        //ui->lstCharMap->setCol
    }
    QStringList lst;
    for (int i=0;i<width;i++)
         lst<<"";

    ui->lstCharMap->setHorizontalHeaderLabels(lst);

    int kk= 0;
    for (int k=0;k<maps.count();k++) {

        QPixmap q = maps[k];
  /*      if (k==0)
          q.save("test.png");
*/
        QTableWidgetItem *itm = ui->lstCharMap->item(j,i);
        if (itm == nullptr) {
//            qDebug() << k;
            itm = new QTableWidgetItem(q,nullptr,Qt::DisplayRole);
            ui->lstCharMap->setItem(j,i,itm);

        }
//        qDebug() << kk;
        itm->setIcon(q);
        itm->setData(Qt::UserRole, kk);
        cnt++;
        if (cnt>=256 && isLevelEditor) break;
        i++;
        kk++;
        if (i>=width) {
            i=0;
            j++;
           // kk+=40-8;

        }

       // kk=kk%255;

    }
//    ui->lstCharMap->
//    int size = (ui->lstCharMap->rect().width()-ui->lstCharMap->spacing())/8;
  //  ui->lstCharMap->setIconSize(QSize(size,size));
    ui->lstCharMap->setRowCount(j);

    ui->lstCharMap->horizontalHeader()->setMinimumSectionSize(1);
    ui->lstCharMap->verticalHeader()->setMinimumSectionSize(1);
    for (int i=0;i<ui->lstCharMap->rowCount();i++) {
         ui->lstCharMap->setRowHeight(i,size);
        //ui->lstCharMap->setCol
    }
    for (int i=0;i<width;i++) {
        ui->lstCharMap->setColumnWidth(i,size);
        //ui->lstCharMap->setCol

    }
    ui->lstCharMap->setShowGrid(false);
    ui->lstCharMap->verticalHeader()->setVisible(false);

    onImageMouseEvent();
}

void FormImageEditor::updateSingleCharSet()
{
    UpdateCurrentMode();
    LImage* charmap = m_work.m_currentImage->m_image->getCharset();
//    ImageLevelEditor* le = dynamic_cast<ImageLevelEditor*>(m_work.m_currentImage->m_image);
/*    if (le!=nullptr && charmap==nullptr) {
        Messages::messages.DisplayMessage(Messages::messages.CHARSET_WARNING);
        return;
    }*/
    if (charmap == nullptr)
        return;
    if (charmap->m_currentChar<0)
        return;
    int c = charmap->m_currentChar;

    for (int i=0;i<GetFooterData(LImageFooter::POS_CURRENT_DISPLAY_Y);i++) {
        for (int j=0;j<GetFooterData(LImageFooter::POS_CURRENT_DISPLAY_X);j++) {
            int k = c+j;

            QPixmap pmap = charmap->ToQPixMap(k);

            int y = k/(int)charmap->m_charWidthDisplay;
            int x = k%(int)charmap->m_charWidthDisplay;
            QTableWidgetItem *itm = ui->lstCharMap->item(y,x);
            if (itm!=nullptr)
                itm->setIcon(pmap);

        }
        c = c + charmap->m_charWidthDisplay;
    }
}

void FormImageEditor::InitQtPainter()
{
//    ui->lblImage->setVisible(false);
//    delete ui->lblImageQt;
}

void FormImageEditor::FixSplitting(QWidget *w, bool sideVisible) {
    if (not w->isVisible()) {
        m_lastSizes = ui->splitter->sizes();
        if (not sideVisible)
            ui->splitter->setSizes(QList<int> {this->width(), 1});
    } else {
        ui->splitter->setSizes(m_lastSizes);
    }
}





void FormImageEditor::PrepareImageTypeGUI()
{
    // Only display "load charset" for levels
    if (m_work.m_currentImage == nullptr)
        return;
    if (m_work.m_currentImage->m_image==nullptr)
        return;
    SetButton(ui->btnLoadCharmap,LImage::GUIType::btnLoadCharset);
    //SetButton(ui->btnCharset1x1,LImage::GUIType::btn1x1);
    //SetButton(ui->btnCharset2x2,LImage::GUIType::btn2x2);
    //SetButton(ui->btnCharset2x2Repeat,LImage::GUIType::btn2x2repeat);
//    SetButton(ui->btnCharsetCopy,LImage::GUIType::btnCopy);
//    SetButton(ui->btnCharsetPaste,LImage::GUIType::btnPaste);
//    SetButton(ui->btnFlipHorisontal,LImage::GUIType::btnFlipH);
//    SetButton(ui->btnFlipVert,LImage::GUIType::btnFlipV);
//    SetButton(ui->btnCharsetFull,LImage::GUIType::btnEditFullCharset);

    /*SetLabel(ui->lblBackground, LImage::GUIType::col1);
    SetLabel(ui->lblForeground, LImage::GUIType::col2);
    SetLabel(ui->lblMC1, LImage::GUIType::col3);
    SetLabel(ui->lblMC2, LImage::GUIType::col4);
*/

    if (m_work.m_currentImage->m_image->isNes())
        ui->btnImportRom->setVisible(false);


    int idx=0;
//    qDebug() << m_work.m_currentImage->m_image->m_GUIParams;
    if (m_work.m_currentImage->m_image->m_GUIParams[LImage::GUIType::tabCharset]=="") {
        ui->tabMain->removeTab(0);
        idx++;
    }
    if (m_work.m_currentImage->m_image->m_GUIParams[LImage::GUIType::tabSprites]=="") {
        ui->tabMain->removeTab(2-idx);
        idx++;
    }
    else {
        //ui->tabMain->setTabText(2-idx,m_work.m_currentImage->m_image->m_GUIParams[LImage::GUIType::tabSprites]);

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

void FormImageEditor::SetSingleCharsetEdit()
{
    UpdateGrid();
    SetFooterData(LImageFooter::POS_DISPLAY_CHAR,1);
    m_updateThread.m_zoom = 1.0;
    UpdateCurrentCell();
    emit onImageMouseEvent();

}

void FormImageEditor::SetFooterData(int pos, uchar val)
{
    if (m_work.m_currentImage==nullptr)
        return;
    if (m_work.m_currentImage->m_image==nullptr)
        return;
    m_work.m_currentImage->m_image->m_footer.set(pos,val);
}

uchar FormImageEditor::GetFooterData(int pos)
{
    if (m_work.m_currentImage==nullptr)
        return 0;
    if (m_work.m_currentImage->m_image==nullptr)
        return 0;
    return m_work.m_currentImage->m_image->m_footer.get(pos);

}

void FormImageEditor::UpdateCurrentCell()
{
    int c = m_work.m_currentImage->m_image->m_currentChar;
    int w = m_work.m_currentImage->m_image->m_charWidthDisplay;
//    qDebug() << "UpdateCurrentCell " << c << w << c/w << c%w;
    ui->lstCharMap->setCurrentCell(c/w, c%w);
  //  qDebug() << "Current cell set to " << ui->lstCharMap->currentRow() << " , " << ui->lstCharMap->currentColumn();

  /*              int i = m_work.m_currentImage->m_image->m_currentChar;
                int x = i%m_work.m_currentImage->m_image->m_charWidthDisplay;
                int y = (i-x)/m_work.m_currentImage->m_image->m_charWidthDisplay;
                qDebug() << "Setting current cell "<<y<<x;
                ui->lstCharMap->setCurrentCell(y,x);
*/


}

void FormImageEditor::SetButton(QPushButton *btn, LImage::GUIType type)
{
    //if (m_work.m_currentImage->m_image->m_GUIParams.contains(type))
      //  return;

    if (m_work.m_currentImage->m_image->m_GUIParams[type]=="")
        btn->setVisible(false);
    else
        btn->setText(m_work.m_currentImage->m_image->m_GUIParams[type]);

}

void FormImageEditor::SetLabel(QLabel *btn, LImage::GUIType type)
{
    if (m_work.m_currentImage->m_image->m_GUIParams[type]=="")
        btn->setVisible(false);
    else
        btn->setText(m_work.m_currentImage->m_image->m_GUIParams[type]);

}

void FormImageEditor::PrepareClose()
{
    AbstractImageEditor*a = dynamic_cast<AbstractImageEditor*>(getCurrentPainter());
    a->CancelAll();
}

void FormImageEditor::SetMCColors()
{

/*

    int a = ui->cmbMC1->currentIndex();
    int b = ui->cmbMC2->currentIndex();
    int c = ui->cmbBorderMain_3->currentIndex();
    int back = ui->cmbBackgroundMain_3->currentIndex();


//    qDebug() << a << b << c;

    if (!m_ignoreMC) {
        m_work.m_currentImage->m_image->SetColor(back, 0);
        m_work.m_currentImage->m_image->SetColor(a, 1);
        m_work.m_currentImage->m_image->SetColor(b, 2);
        if (m_work.m_currentImage->m_image->isNes())
          m_work.m_currentImage->m_image->SetColor(c, 3);

        QVector<int> lst;
        lst.append(a);
        lst.append(b);
        lst.append(c);
        lst.append(back);
        m_work.m_currentImage->m_image->ConstrainColours(lst);
        if (m_work.m_currentImage->m_image->m_supports.displayColors)
            m_work.m_currentImage->m_image->m_colorList.CreateUI(ui->layoutColorsEdit_3,1, this->size());
    }

    updateCharSet();
    UpdateMulticolorImageSettings();
    emit onImageMouseEvent();

    Data::data.Redraw();
    Data::data.forceRedraw = true;
*/
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
                UpdateLevels();;
                onImageMouseEvent();

            }

            );

        }



}

void FormImageEditor::GenericExportImage(QString type, QString ext)
{
    if (m_work.m_currentImage->m_image->m_exportParams.keys().count()!=0 && !m_work.m_currentImage->m_image->m_silentExport) {
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

void FormImageEditor::GenericImportImage(QString type, QStringList ext)
{
    QString f = type+" (";
    for (auto e: ext)
       f+="*."+e+" ";
    f+=")";
//    qDebug() << f;
    QString ttr  = "Import " + type.toLower() + " file";
    QString filename = QFileDialog::getOpenFileName(this,
        ttr.toStdString().c_str(), m_projectPath, f);
    if (filename=="")
        return;



    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    if (ext.contains("bin"))
        m_work.m_currentImage->m_image->ImportBin(file);
    if (ext.contains("koa") || ext.contains("kla"))
        m_work.m_currentImage->m_image->ImportKoa(file);
    if (ext.contains("c"))
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
    int sx = 30*2.5;
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

//    img->m_currentChar = keep;
//    ui->lblSprite2->setPixmap(m_updateThread.m_pixMapImage.scaled(40, 32, Qt::IgnoreAspectRatio, Qt::FastTransformation));



}

void FormImageEditor::AspectDone() {
    QWidget* w = getCurrentPainter();
    w->setVisible(true);
    ui->lblName->setVisible(true);

}

void FormImageEditor::Aspect1()
{
    QWidget* w = getCurrentPainter();

    int size;
    if (w->width()<=w->height()*m_scaley)
        size = w->width();
//
    else
        size = w->height()-10;//min((double)m_oldWidth,height()/1.25);
//    qDebug() << size << this->height();
    int sy = size/m_scaley;
    //  w->setMinimumWidth(size);
      w->setMaximumHeight(sy);
      w->setMaximumWidth(size);
      w->setMinimumHeight(sy);
      w->setMinimumWidth(size);
//
//    ui->vImageSpacer->changeSize(0,this->height()-sy-20,QSizePolicy::Expanding,QSizePolicy::Expanding);
    QTimer::singleShot(10, this, SLOT(AspectDone()));

//    w->setVisible(true);
  //  ui->lblName->setVisible(true);

}


void FormImageEditor::UpdateAspect()
{
    QWidget* w = getCurrentPainter();
    auto img = getCurrentPainter();

    //auto img = ui->lblImage;

    int val = ui->cmbAspect->currentIndex();
//    ((AbstractImageEditor*)img)->m_aspectType = val;
//    ui->lblImage->setVisible(false);
  //  return;




    w->setMinimumHeight(0);
    w->setMaximumHeight(100000);
    w->setMinimumWidth(0);
    w->setMaximumWidth(100000);
    //ui->vImageSpacer->changeSize(0,0);
    if (m_painterType == OpenGL) {
        ui->lblImage->m_aspectType = val;
        ui->lblImage->repaint();
        ui->lblImage->update();
        ui->lblImage->resize(ui->lblImage->width(),ui->lblImage->height());
    }
    else {
        ui->lblImageQt->m_aspectType = val;
        ui->lblImageQt->repaint();
        ui->lblImageQt->update();
        ui->lblImageQt->resize(ui->lblImageQt->width(),ui->lblImageQt->height());

    }
    Data::data.Redraw();
    onImageMouseEvent();


    this->resize(this->geometry().width()-1, this->geometry().height());
    this->resize(this->geometry().width()+1, this->geometry().height());
    repaint();
    update();
    //emit mySizeChanged(QSize(width(), height()));
 //   ui->lblImage->resizeGL(320,200);
  //  ui->splitter->update();
//    ui->splitter->repaint();
//    ui->splitter->setSizes(QList<int>()<<1<<1);
   // ui->splitter->move(1,1);
    return;

    int a = 100;
    int b = 100;
    if (val==1) {
        ui->splitter->setSizes(QList<int>()<<a*5<<b );
    }
    if (val==2) {
        ui->splitter->setSizes(QList<int>()<<a*5*1.6<<b );
    }



    return;

    if (val==0) {
        w->setVisible(true);
        return;
    }


     w->setVisible(false);
     int wait = 10;
    ui->lblName->setVisible(false);
    if (val==1) {
        m_scaley = 1.0;
        QTimer::singleShot(wait, this, SLOT(Aspect1()));
    }
    if (val==2) {
        QTimer::singleShot(wait, this, SLOT(Aspect1()));
        m_scaley = 1.6;

    }



    return;

    if (val==0) {
        w->setMinimumHeight(0);
        w->setMaximumHeight(100000);
        w->setMinimumWidth(0);
        w->setMaximumWidth(100000);
        //ui->vImageSpacer->changeSize(0,0);
//        m_oldWidth = width();
    }
    if (val==1) {

//        w->setMinimumWidth(size/2);
  //      w->setMaximumWidth(size);
    }
    if (val==2) {
        w->setMinimumWidth(0);
        w->setMaximumWidth(100000);
        w->setMinimumHeight(m_oldWidth/1.6);
        w->setMaximumHeight(m_oldWidth/1.6);
        //ui->vImageSpacer->changeSize(0,200,QSizePolicy::Expanding,QSizePolicy::Expanding);
    }
    this->resize(this->geometry().width(), this->geometry().height());
    onImageMouseEvent();

}

void FormImageEditor::Update()
{
    Data::data.forceRedraw = true;
    Data::data.Redraw();
//    updateCharSet();
    onImageMouseEvent();

}

void FormImageEditor::LoseFocus() {
    QObject::disconnect(&Data::data, SIGNAL(EmitPenChanged()), this,SLOT(onPenChanged()));

}

void FormImageEditor::SetFocus() {
    QObject::connect(&Data::data, SIGNAL(EmitPenChanged()), this,SLOT(onPenChanged()));

}

QWidget *FormImageEditor::getCurrentPainter()
{
    if (m_painterType==OpenGL)
        return ui->lblImage;

    return ui->lblImageQt;
}



void FormImageEditor::onSwapDisplayMode()
{
    m_work.m_currentImage->m_image->m_footer.toggle(LImageFooter::POS_DISPLAY_CHAR);
  //  qDebug() << "woot " << GetFooterData(LImageFooter::POS_DISPLAY_CHAR);
    getCurrentPainter()->setFocus();
    ui->lstCharMap->setCurrentItem(nullptr);
    if (GetFooterData(LImageFooter::POS_DISPLAY_CHAR)==1) {
       UpdateCurrentCell();
    }

    m_updateThread.m_zoom = 1.0;

    UpdateCurrentMode();
    UpdateGrid();
    emit onImageMouseEvent();
    Data::data.forceRedraw = true;
    Data::data.Redraw();

//    qDebug() << "woot2 " << GetFooterData(LImageFooter::POS_DISPLAY_CHAR);







}


void FormImageEditor::on_btnExportKoala_clicked()
{
    GenericExportImage("Koala", "kla");

}

void FormImageEditor::on_btnImportKoala_clicked()
{
    GenericImportImage("Koala", QStringList() <<"koa" << "kla");

}



void FormImageEditor::on_btnImportBin_clicked()
{
    GenericImportImage("Binary", QStringList() <<"bin");
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

void FormImageEditor::on_lstCharMap_itemClicked(QTableWidgetItem *item)
{
    if (item==nullptr)
        return;
    m_prefMode = m_keepMode;
    SelectCharacter(item->data(Qt::UserRole).toInt());
    SetSingleCharsetEdit();

}


void FormImageEditor::on_lstCharMap_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous)
{
    if (current==nullptr)
        return;
    m_prefMode = m_keepMode;
    SelectCharacter(current->data(Qt::UserRole).toInt());
//    SetSingleCharsetEdit();
    Data::data.Redraw();
    Data::data.forceRedraw = true;
//    qDebug() << m_work.m_currentImage->m_image->m_currentChar;
    onImageMouseEvent();
  //  qDebug() << m_work.m_currentImage->m_image->m_currentChar;


}

void FormImageEditor::Destroy()
{
    //delete m_work.m_currentImage;

    //m_updateThread.m_imgLabel = nullptr;
//    m_updateThread.
    //m_updateThread.m_work = nullptr;
//    m_updateThread.m_ = nullptr;

}


/*void FormImageEditor::on_cmbMC1_activated(int index)
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
    updateCharSet();
    emit onImageMouseEvent();
    Data::data.redrawOutput = true;

}
*/
void FormImageEditor::on_btnResizeData_clicked()
{
    ImageLevelEditor* img = dynamic_cast<ImageLevelEditor*>(m_work.m_currentImage->m_image);
    if (img==nullptr)
        return;

    DialogNewImage* dResize = new DialogNewImage(this);
    dResize->Initialize(m_work.m_types);
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

void FormImageEditor::UpdateMulticolorImageSettings()
{
    if (m_work.m_currentImage->m_image->m_supports.displayColors) {
        m_work.m_currentImage->m_image->m_colorList.SetIsMulticolor(ui->chkDisplayMulticolor->isChecked());
        m_work.m_currentImage->m_image->m_colorList.CreateUI(ui->layoutColorsEdit_3,1,m_windowSize);
    }

}

void FormImageEditor::on_chkDisplayMulticolor_stateChanged(int arg1)
{
    m_work.m_currentImage->m_image->setMultiColor(ui->chkDisplayMulticolor->isChecked());
    SetFooterData(LImageFooter::POS_DISPLAY_MULTICOLOR,ui->chkDisplayMulticolor->isChecked());
    UpdateMulticolorImageSettings();
    updateCharSet();
    Data::data.Redraw();
    onImageMouseEvent();
    onPenChanged();
}

void FormImageEditor::on_leHeaders_editingFinished()
{
    QStringList lst = ui->leHeaders->text().remove(" ").split(",");

    m_work.m_currentImage->m_image->BuildData(ui->tblData, lst);
    m_projectIniFile->setStringList("data_header_"+m_currentFileShort,lst);
    m_projectIniFile->Save(m_projectIniFile->filename);


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


void FormImageEditor::on_btnPanRight_clicked()
{
    m_work.m_currentImage->AddUndo();
    m_work.m_currentImage->m_image->Transform(1,0);
    onImageMouseEvent();

}

void FormImageEditor::on_btnPanDown_clicked()
{
    m_work.m_currentImage->AddUndo();
    m_work.m_currentImage->m_image->Transform(0,1);
    onImageMouseEvent();

}


void FormImageEditor::on_btnPanUp_clicked()
{
    m_work.m_currentImage->AddUndo();
    m_work.m_currentImage->m_image->Transform(0,-1);
    onImageMouseEvent();

}


void FormImageEditor::on_btnPanLeft_clicked()
{
    m_work.m_currentImage->AddUndo();
    m_work.m_currentImage->m_image->Transform(-1,0);
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


    ImportROM();

}

void FormImageEditor::ImportROM()
{


    QString fileName = ":resources/character.rom";

    m_work.m_currentImage->m_image->LoadCharset(fileName,0);
    ui->cmbCharX->setCurrentIndex(0);
    ui->cmbCharY->setCurrentIndex(0);
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
    GenericImportImage("c", QStringList() <<"c");

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
    QString f3 = fileName + "_color.bin";

    if (QFile::exists(f1))
        QFile::remove(f1);

    if (QFile::exists(f2))
        QFile::remove(f2);

    if (QFile::exists(f3))
        QFile::remove(f3);


    m_work.m_currentImage->m_image->ExportCompressed(f1,f2,f3);


}

void FormImageEditor::on_btnPalette_clicked()
{
    DialogColors* dc = new DialogColors();
    dc->Initialize(m_work.m_currentImage->m_image,&m_work.m_currentImage->m_image->m_colorList, m_projectPath);
    dc->exec();
    UpdatePalette();
}

/*void FormImageEditor::on_cmbBorderMain_3_activated(int index)
{
    SetMCColors();
    emit onImageMouseEvent();

}
*/
void FormImageEditor::on_cmbNesPalette_currentIndexChanged(int index)
{
    if (!m_work.m_currentImage->m_image->isNes() && !m_work.m_currentImage->m_image->isSnes() )
        return;
    if (ui->cmbNesPalette->count()==0)
        return;
//    m_ignoreMC = true;
//    qDebug() << index;
    m_work.m_currentImage->m_image->m_colorList.m_curPal = index;
    m_ignoreMC = false;
    int idx = m_work.m_currentImage->m_image->m_colorList.m_curPal;//*4+1;
/*
    ui->cmbMC2->setCurrentIndex(m_work.m_currentImage->m_image->m_colorList.m_nesPPU[idx+1]);
    ui->cmbMC1->setCurrentIndex(m_work.m_currentImage->m_image->m_colorList.m_nesPPU[idx+0]);
    ui->cmbBorderMain_3->setCurrentIndex(m_work.m_currentImage->m_image->m_colorList.m_nesPPU[idx+2]);
    ui->cmbBackgroundMain_3->setCurrentIndex(m_work.m_currentImage->m_image->m_colorList.m_nesPPU[0]);
    */
    m_ignoreMC = false;

//    qDebug() << "FormImageEditor::CurrentIndexChanged new palette index "<<idx;
    m_work.m_currentImage->m_image->SetPalette(idx);
    SetFooterData(LImageFooter::POS_CURRENT_PALETTE,index);


//    qDebug() << "on_cmbNesPalette "<< index;


//    SetMCColors();
    UpdatePalette();
    updateCharSet();
    emit onImageMouseEvent();

}

void FormImageEditor::on_cmbBank_currentIndexChanged(int index)
{
    m_work.m_currentImage->m_image->SetBank(index);
    SetFooterData(LImageFooter::POS_CURRENT_BANK,index);
//    qDebug() << "Setting :" << index;

    updateCharSet();

}

void FormImageEditor::on_cmbNesPalette_activated(int index)
{

}

void FormImageEditor::on_btnCharSelect_clicked()
{
    OpenSelectCharset();
}



void FormImageEditor::on_cmbAspect_currentIndexChanged(int index)
{
//    m_projectIniFile->setFloat("aspect_ratio",index);
    SetFooterData(LImageFooter::POS_ASPECT,index);

    UpdateAspect();

}

void FormImageEditor::on_chkPaintSeparately_stateChanged(int arg1)
{
    m_work.m_currentImage->m_image->m_forcePaintColorAndChar = !ui->chkPaintSeparately->isChecked();
    SetFooterData(LImageFooter::POS_DOUBLE_PAINT,!m_work.m_currentImage->m_image->m_forcePaintColorAndChar);

}

void FormImageEditor::on_pushButton_clicked()
{
//    Reload();
    if (m_currentFilename!="") {
//        Load(m_currentFilename);
        //m_work.m_currentImage->m_image = LImageIO::Load(m_currentFilename);
        Load(m_currentFilename);
        ui->cmbNesPalette->setCurrentIndex(GetFooterData(LImageFooter::POS_CURRENT_PALETTE));
        on_cmbNesPalette_currentIndexChanged(GetFooterData(LImageFooter::POS_CURRENT_PALETTE));
        on_cmbNesPalette_currentIndexChanged(GetFooterData(LImageFooter::POS_CURRENT_PALETTE));

        Update();

    }
}

void FormImageEditor::on_cmbCharX_currentIndexChanged(int index)
{
    SetFooterData(LImageFooter::POS_CURRENT_DISPLAY_X,ui->cmbCharX->currentText().toInt());
    SetFooterData(LImageFooter::POS_DISPLAY_CHAR,0);
    on_btnCharsetFull_clicked();

    Update();
    UpdateCurrentCell();

}

void FormImageEditor::on_cmbCharY_currentIndexChanged(int index)
{
    SetFooterData(LImageFooter::POS_CURRENT_DISPLAY_Y,ui->cmbCharY->currentText().toInt());
    SetFooterData(LImageFooter::POS_DISPLAY_CHAR,0);
    on_btnCharsetFull_clicked();
    Update();

}

void FormImageEditor::on_cmbTileStampX_currentIndexChanged(int index)
{
    SetFooterData(LImageFooter::POS_CURRENT_STAMP_X,ui->cmbTileStampX->currentText().toInt());
    SetFooterData(LImageFooter::POS_DISPLAY_CHAR,0);
    on_btnCharsetFull_clicked();

    Update();
    UpdateCurrentCell();

}

void FormImageEditor::on_cmbTileStampY_currentIndexChanged(int index)
{
    SetFooterData(LImageFooter::POS_CURRENT_STAMP_Y,ui->cmbTileStampY->currentText().toInt());
    SetFooterData(LImageFooter::POS_DISPLAY_CHAR,0);
    on_btnCharsetFull_clicked();
    Update();

}

void FormImageEditor::on_btnRepeating_clicked()
{
    m_work.m_currentImage->m_image->m_footer.toggle(LImageFooter::POS_CURRENT_DISPLAY_REPEAT);
    Update();

}

void FormImageEditor::on_btnShiftLeft_clicked()
{
    m_work.m_currentImage->AddUndo();
    m_work.m_currentImage->m_image->ShiftXY(-1,0);
    Update();

}

void FormImageEditor::on_btnShiftRight_clicked()
{
    m_work.m_currentImage->AddUndo();
    m_work.m_currentImage->m_image->ShiftXY(1,0);
    Update();

}

void FormImageEditor::on_btnShiftDown_clicked()
{
    m_work.m_currentImage->AddUndo();
    m_work.m_currentImage->m_image->ShiftXY(0,1);
    Update();

}

void FormImageEditor::on_btnShiftUp_clicked()
{
    m_work.m_currentImage->AddUndo();
    m_work.m_currentImage->m_image->ShiftXY(0,-1);
    Update();

}

void FormImageEditor::on_btnClear_clicked()
{
    m_work.m_currentImage->AddUndo();
//    m_work.m_currentImage->m_image->Clear(m_work.m_currentImage->m_image->m_currentChar);

    if (m_work.m_currentImage->m_image->m_clearWithCurrentChar)
       m_work.m_currentImage->m_image->Clear(m_work.m_currentImage->m_image->m_currentChar);
    else
        m_work.m_currentImage->m_image->Clear(0);

    updateCharSet();

    Update();
}

void FormImageEditor::on_btnSelectDefaultClearItm_clicked()
{
    if (m_work.m_currentImage->m_image->getCharset()==nullptr)
        return;
    DialogSelectCharset* ds = new DialogSelectCharset(m_work.m_currentImage->m_image->getCharset());
    ds->exec();
    if (ds->result() == QDialog::Rejected) {
        return;
    }
    SetFooterData(LImageFooter::POS_CLEAR_VALUE,ds->m_char);


}

void FormImageEditor::on_cmbCharWidth_currentTextChanged(const QString &arg1)
{
    if (m_work.m_currentImage->m_image->getCharset()==nullptr)
        return;


    m_work.m_currentImage->m_image->getCharset()->m_charWidthDisplay = arg1.toInt();
    m_work.m_currentImage->m_image->m_charWidthDisplay = arg1.toInt();
    SetFooterData(LImageFooter::POS_CHARSET_WIDTH, ui->cmbCharWidth->currentText().toInt());
    updateCharSet();
    updateCharSet();
    onImageMouseEvent();
    Update();
}


void FormImageEditor::on_btnInv_clicked()
{
    m_work.m_currentImage->m_image->Invert();
    Update();
}

void FormImageEditor::on_chkHybrid_clicked(bool checked)
{
    SetFooterData(LImageFooter::POS_DISPLAY_HYBRID,checked);
    if (checked) {
        ui->chkDisplayMulticolor->setChecked(false);
    }
    m_work.m_currentImage->m_image->SetHybridMode(checked);
    ui->chkDisplayMulticolor->setVisible(!checked);
    onPenChanged();
}

void FormImageEditor::on_cbmGridSize_currentTextChanged(const QString &arg1)
{
    m_work.m_currentImage->m_image->SetGridSize(arg1);
    UpdateGrid();
    Data::data.Redraw();
    onImageMouseEvent();
}

void FormImageEditor::on_btnImportMain_clicked()
{
    on_btnImport_clicked();
}


void FormImageEditor::on_cmbFixedfD800_activated(int index)
{
    SetFooterData(LImageFooter::POS_D800_FIXED,index);
    m_work.m_currentImage->m_image->SetForceD800Color(index-1);
    onPenChanged();
}


void FormImageEditor::on_cmbZoomLevel_activated(int index)
{
    on_cmbZoomLevel_activated("");
}


void FormImageEditor::on_cmbAspect_activated(int index)
{
    on_cmbAspect_currentIndexChanged(ui->cmbAspect->currentIndex());
}


void FormImageEditor::on_btnDuplicate_clicked()
{
    m_work.m_currentImage->AddUndo();
    m_work.m_currentImage->m_image->Duplicate();
    auto img= dynamic_cast<LImageContainer*>(m_work.m_currentImage->m_image);
    if (img!=nullptr)
        img->Next();
    onImageMouseEvent();


}

