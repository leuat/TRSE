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

QLabelLImage::QLabelLImage(QWidget *parent) : QLabel(parent) {
    setMouseTracking(true);
    installEventFilter(this);
}

void QLabelLImage::mouseMoveEvent(QMouseEvent *e)
{
    if (m_updateThread==nullptr)
        return;
    if (AIE_mouseMoveEvent(e,this))
//    if ((m_updateThread->m_prevPos-m_updateThread->m_currentPos).manhattanLength()>0.0)

       emit EmitMouseMove(e);

}

void QLabelLImage::wheelEvent(QWheelEvent *e)
{
    emit EmitMouseMove(e);
}

bool QLabelLImage::eventFilter(QObject *object, QEvent *event){
    if (m_cancel)
        return true;
    if(object==this) {
        if (AIE_eventFilter(object,event,this)) {
            emit EmitMouseMove(event);
        }
        return false;
    }
    return true;
}

void QLabelLImage::mouseReleaseEvent(QMouseEvent *e)
{
    AIE_mouseReleaseEvent(e);
    emit EmitMouseMove(e);

}

void QLabelLImage::mousePressEvent(QMouseEvent *e)
{
    AIE_mousePressEvent(e);
    emit EmitMouseMove(e);

}

void QLabelLImage::resizeEvent(QResizeEvent *event)
{
    if (event!=nullptr)
        event->accept();

    float scale = 1;
    if (m_aspectType==2)
        scale = 1.5;



    if (m_aspectType!=0) {
        // Fixed 1:1

        int m = std::min((float)event->size().width(),event->size().height()*scale);

        QWidget::resize(m,m/scale);

    }
    QLabel::resizeEvent(event);
}



