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

#include "workerthread.h"
#include "source/LeLib/util/util.h"
#include <QPainter>

void WorkerThread::UpdateDrawing()
{
    if (m_work==nullptr)
        return;

    if (m_isPanning)
        return;





    if (Data::data.currentIsColor) {
        Data::data.currentIsColor=false;
        m_work->m_currentImage->m_image->SetCurrentType(LImage::Color);
    }





    if (Data::data.imageEvent == 0 && !m_hasLeft) {
        m_hasLeft = true;
        isPreview=false;

        Data::data.Redraw();
        //qDebug() << "HASLEFT " << m_time << m_currentPos;
        return;

    }

//    qDebug() << "Before" << m_time;
    // Check if leave event triggerede

    if (m_work==nullptr) {
        qDebug() << "M_WORK is null in updatedrawinbg";
        return;
    }

    if (m_work->m_currentImage==nullptr){
        qDebug() << "M_WORK->m_currentImage is null in updatedrawinbg";
        return;
    }

    if (m_work->m_currentImage->m_image==nullptr){
        qDebug() << "M_WORK->m_currentImage->m_image is null in updatedrawinbg";
        return;
    }

    QPointF pos = (m_currentPos - QPointF(0.5, 0.5) -m_zoomCenter)*m_zoom + m_zoomCenter ;
    bool isInWindow  =(pos.x()>=0 && pos.x()<m_work->m_currentImage->m_image->m_width &&
                       pos.y()>=0 && pos.y()<m_work->m_currentImage->m_image->m_height);



//    isInWindow = true;
//    qDebug() << (m_prevPos-m_currentPos);




    //if (!Data::data.forceRedraw && m_currentButton == 0)

//    if ((abs(m_prevPos.x()-m_currentPos.x())<1) && (abs(m_prevPos.y()-m_currentPos.y()))<1)
  //      return;

//    if (Data::data.imageEvent != 2)
  //      return;

//    qDebug() << isInWindow;


    if ( isInWindow || Data::data.forceRedraw )
    {
         if (m_work==nullptr) return;

        if (m_work->m_currentImage==nullptr) return;
        if (m_work->m_currentImage->m_image==nullptr) return;




        m_work->m_currentImage->m_temp->CopyFrom(m_work->m_currentImage->m_image);
        Data::data.forceRedraw = false;
        m_hasLeft = false;
        LImage* img = (LImage*)m_work->m_currentImage->m_image;
        isPreview = false;

        //qDebug() << m_currentButton;
        if (m_currentButton == 0) {
            isPreview = true;
        }
        m_toolBox->m_current->IsPreview(m_currentButton, isPreview);
        unsigned int col = Data::data.currentColor;
        if (m_currentButton == 2) {// draw background
            col = img->getBackground();
            img->BeforeRightButton();
        }


        // Check if left click + ctrl
        if (m_currentButton == 1  && (QApplication::keyboardModifiers() & Qt::ControlModifier)) {
            img->CtrlLeftShift(pos.x(), pos.y());
            return;
        }


        if (isPreview)
            img = (LImage*)m_work->m_currentImage->m_temp;

        bool perform = true;

//        pos.setX(pos.x()-0.25f);
//        pos.setY(pos.y()-0.25f);
        if (!(QApplication::keyboardModifiers() & Qt::ControlModifier) && perform) {
            m_toolBox->m_current->Perform(pos.x(), pos.y(), col, img, isPreview, m_currentButton);
            //if (dynamic_cast<Filler*>(m_toolBox->m_current)!=nullptr) {
            //    msleep(10);
           // }
        }
        if (m_currentButton == 2)
            img->AfterRightButton();

        m_currentPosInImage = img->GetCurrentPosInImage(pos.x(), pos.y());
        Data::data.Redraw();
    }
    else {
//        qDebug() << "OUTSIDE" << m_time;

        //Data::data.Redraw();
        m_currentButton = 0;
    }
    if (m_currentButton == -1)
        m_currentButton = 0;

}

void WorkerThread::UpdateMousePosition()
{

}

void WorkerThread::UpdatePanning()
{
    if (!m_work)
        return;
    m_isPanning = false;
//    if (m_currentButton == 2 && (QApplication::keyboardModifiers() & Qt::ShiftModifier)) {
        if (m_currentButton == 4) {
            //QPointF delta = (m_prevPos - m_currentPos);
   /*         QPointF delta = (QPoint(m_work->m_currentImage->m_image->m_width,
                                    m_work->m_currentImage->m_image->m_height)/2
                                     - (m_currentPos-m_prevPos));*/
          //  QPointF delta = (m_currentPos-m_prevPos);
            m_zoomCenter-=(QPointF)m_delta*m_zoom*m_panningScale;
            //        qDebug() << delta;
            m_isPanning = true;
            Data::data.Redraw();
            Data::data.forceRedraw = true;
            m_prevPos = m_currentPos;
        }



}




void WorkerThread::UpdateImage(LImage * mc)
{
    QElapsedTimer ep;
    ep.start();
   if (m_work == nullptr)
       return;
   if (mc==nullptr)
       return;
    if (m_tmpImage == nullptr) {
        m_tmpImage = new QImage(mc->m_width,mc->m_height,QImage::Format_ARGB32);
    }

    if (m_tmpImage->width()!=mc->m_width || m_tmpImage->height()!=mc->m_height) {
        delete m_tmpImage;
        m_tmpImage = new QImage(mc->m_width,mc->m_height,QImage::Format_ARGB32);

    }

    if (m_isPanning)
        CreateGrid();

    if (mc==nullptr)
        return;
    if (m_work->m_currentImage==nullptr)
        return;
    if (m_work->m_currentImage->m_image==nullptr)
        return;

        mc->ToQImage(m_work->m_currentImage->m_image->m_colorList, *m_tmpImage, m_zoom, m_zoomCenter);
//        *m_tmpImage = m_tmpImage->scaled(320,200,Qt::KeepAspectRatio);
   // m_tmpImage->fill(QColor(255,0,0));
    m_pixMapImage.convertFromImage(*m_tmpImage);

    if (m_grid!=nullptr && m_drawGrid) {

        QPixmap grid;
        grid.convertFromImage(*m_grid->m_qImage);
        QPixmap p = m_pixMapImage.scaled(QSize(grid.width(),grid.height()),  Qt::IgnoreAspectRatio, Qt::FastTransformation);
        QPainter painter(&p);
        painter.drawPixmap(0,0,grid);
        m_pixMapImage = p;
    }
    //ui->lblImageName->setText(m_work->m_currentImage->m_name  + "(" + m_work->m_currentImage->m_imageType->name + ")");


//    qDebug() << "Emit " << rand()%100;
    emit updateImageSignal();
//    qDebug() << "UpdateImage : " <<ep.elapsed();
    //ui->lblImage->setPixmap(m_pixMapImage);
}

void WorkerThread::UpdateMessages()
{

    if (Data::data.blink) {
        Data::data.blink = false;
        m_blinkTimer = 15;
        m_pal = m_orgPal;
        m_pal.setColor(QPalette::Window, QColor(90,90,70));
        emit updatePaletteSignal();
    }
    if (m_blinkTimer>0) {
        m_blinkTimer--;
        if (m_blinkTimer==0) {
            m_pal = m_orgPal;
            emit updatePaletteSignal();

        }

    }

    if (Data::data.requestSaveAs) {
        Data::data.requestSaveAs = false;
        emit requestSaveAs();
    }
    if (Data::data.requestCloseWindow) {
        Data::data.requestCloseWindow = false;
        emit requestCloseWindowSignal();
    }

}

void WorkerThread::OnQuit()
{
    m_quit = true;
}

void WorkerThread::Park()
{
    m_park = true;
    msleep(35);
}

void WorkerThread::Continue()
{
    m_park = false;
}

void WorkerThread::CreateGrid()
{
    if (m_drawGrid)
        m_grid->CreateGrid(m_work->m_currentImage->m_image->getGridWidth(),m_work->m_currentImage->m_image->getGridHeight(),m_gridColor,2, m_zoom, QPointF(m_zoomCenter.x(), m_zoomCenter.y())*m_gridScale,m_work->m_currentImage->m_image->m_scaleX);
    else
        m_grid->m_qImage->fill(0);
}

void WorkerThread::RunContents()
{
    UpdateMessages();
    // Mouse position now being handled by QLabelLImage
    //UpdateMousePosition();
    UpdatePanning();
    UpdateDrawing();
    m_time++;

//    qDebug() << rand()%100;

//    if (Data::data.redrawOutput && m_work!=nullptr)
        if (m_work!=nullptr)
    {

        //            qDebug() << "Redraw" << rand()%100;
        LImage* img = m_work->m_currentImage->m_image;
        if (isPreview) {
            img = m_work->m_currentImage->m_temp;
        }
        //qDebug() << "Updating image" << m_time;
        UpdateImage(img);
        Data::data.redrawOutput = false;

    }


}




void WorkerThread::run()
{
    while (!m_quit) {
        if (!m_park) {
            RunContents();
        }

        QThread::msleep(10);
    }

}
