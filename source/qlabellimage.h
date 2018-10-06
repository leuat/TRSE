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

#pragma once
#include <QLabel>
#include "source/workerthread.h"

class QLabelLImage : public QLabel
{
public:
    bool m_imageChanged = false;
    QLabelLImage();
    QLabelLImage(QWidget* parent) : QLabel(parent) {
        setMouseTracking(true);
        installEventFilter(this);
    }
    int m_time=0;
    WorkerThread* m_updateThread = nullptr;
    ImageWorker* m_work = nullptr;
    bool m_active=false;
    bool m_cancel = false;

    void mouseMoveEvent(QMouseEvent *e) override;
//    void mouseLeaveEvent(QMouseEvent* e) override;
    void CancelAll() {m_cancel=true; m_active=false;}
    bool eventFilter(QObject *object, QEvent *event) override;

    void mouseReleaseEvent(QMouseEvent*  e) override;
    void mousePressEvent(QMouseEvent* e) override;
};

