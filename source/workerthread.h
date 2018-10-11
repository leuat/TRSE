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

#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QMainWindow>
#include "imageworker.h"
#include <QString>
#include <QPixmap>
#include <QFileDialog>
#include "source/LeLib/data.h"
#include <QThread>
#include <thread>
#include <QLayout>
#include <QPointF>
#include <QLabel>
#include <source/toolbox.h>



class WorkerThread : public QThread {
    Q_OBJECT

public:
    int m_time = 0;
    bool m_quit = false;
    ImageWorker* m_work = nullptr;
    Toolbox* m_toolBox = nullptr;
  //  QLabel* m_imgLabel = nullptr;
    int m_blinkTimer = 0;
    QPalette m_pal, m_orgPal;

    bool m_hasLeft = false;
    bool m_park = false;

    WorkerThread() {

    }


    void Park();
    void Continue();

    void SetCurrentImage(ImageWorker* work, Toolbox* tb, QLabel* lbl) {
        m_work = work;
        m_toolBox = tb;
        //m_imgLabel = lbl;
    }

//    QPointF pos;

    LImageQImage* m_grid=nullptr;

    QPointF m_currentPos, m_prevPos;
    int m_currentButton = 0;
    float m_zoom = 1;
    bool m_drawGrid = false;
    bool m_isPanning = false;
    QPointF m_zoomCenter = QPoint(00,00);
    QPoint m_currentPosInImage;
    QColor m_gridColor = QColor(64,128,128,128);


    void RunContents();
    void run() override;
    QImage* m_tmpImage = nullptr;
    QPixmap m_pixMapImage;
    bool isPreview;

    void UpdateDrawing();
    void UpdateMousePosition();
    void UpdatePanning();
    void UpdateImage(LImage* mc);

    void UpdateMessages();

signals:
    void updateImageSignal();
    void updatePaletteSignal();
    void requestSaveAs();
    void requestCloseWindowSignal();
public slots:
    void OnQuit();
};


#endif // WORKERTHREAD_H
