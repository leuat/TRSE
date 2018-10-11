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

#include "qlabellimage.h"

QLabelLImage::QLabelLImage()
{
       setMouseTracking(true);
       installEventFilter(this);
}

void QLabelLImage::mouseMoveEvent(QMouseEvent *e)
{
    if (m_cancel)
        return;
    if (m_work==nullptr)
        return;
    if (m_work->m_currentImage==nullptr)
        return;
    if (m_work->m_currentImage->m_image==nullptr)
        return;
    if (m_updateThread==nullptr)
        return;


    m_active=true;

    QPointF pos = QCursor::pos() -mapToGlobal(QPoint(0,0));
    pos.setX(pos.x()*(float)m_work->m_currentImage->m_image->m_width/width());
    pos.setY(pos.y()*(float)m_work->m_currentImage->m_image->m_height/height());

    m_updateThread->m_prevPos = m_updateThread->m_currentPos;
    m_updateThread->m_currentPos = QPointF(pos.x(), pos.y());

    if ((m_updateThread->m_prevPos-m_updateThread->m_currentPos).manhattanLength()>0.0)

       emit EmitMouseMove();

}

void QLabelLImage::wheelEvent(QWheelEvent *e)
{
    emit EmitMouseMove();
}

bool QLabelLImage::eventFilter(QObject *object, QEvent *event){
/*    if ( !dynamic_cast<QInputEvent*>( event ) )
           return false;
*/
    if (m_cancel)
        return true;
    if(object==this) {
//        if  (event->type()==QEvent::Move)
    //    qDebug() << "Move event";
        mouseMoveEvent((QMouseEvent*)event);
/*        mousePressEvent((QMouseEvent*)event);
        mouseReleaseEvent((QMouseEvent*)event);*/
        //mousePressEvent((QMouseEvent*)event);
        //mouseReleaseEvent((QMouseEvent*)event);
        //m_updateThread->
        if (event->type()==QEvent::Enter) Data::data.imageEvent = 1;
        if (event->type()==QEvent::Leave) Data::data.imageEvent = 0;

//        Data::data.Redraw();

//        qDebug() << this;

        return false;
    }
    return true;
}

void QLabelLImage::mouseReleaseEvent(QMouseEvent *e)
{
    if (m_cancel)
        return;
    if (m_updateThread==nullptr)
        return;
    if (m_updateThread->m_currentButton==1) {
        m_updateThread->m_currentButton = -1;
    }
    else
        m_updateThread->m_currentButton = 0;



    emit EmitMouseMove();

}

void QLabelLImage::mousePressEvent(QMouseEvent *e)
{
    if (m_cancel)
        return;
    if (m_updateThread==nullptr)
        return;
    m_updateThread->m_work->m_currentImage->AddUndo();



        if(e->buttons() == Qt::RightButton) {
            m_imageChanged = true;
            m_updateThread->m_currentButton = 2;
            if (e->modifiers() & Qt::ShiftModifier) {
                m_updateThread->m_prevPos = m_updateThread->m_currentPos;
                m_updateThread->m_currentButton = 4;
            }
        }

        if(e->buttons() == Qt::LeftButton) {
            m_updateThread->m_currentButton = 1;
            m_imageChanged = true;
        }
        emit EmitMouseMove();

}



