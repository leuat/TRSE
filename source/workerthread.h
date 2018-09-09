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

    WorkerThread() {

    }


    void SetCurrentImage(ImageWorker* work, Toolbox* tb, QLabel* lbl) {
        m_work = work;
        m_toolBox = tb;
        //m_imgLabel = lbl;
    }

//    QPointF pos;

    QPointF m_currentPos, m_prevPos;
    int m_currentButton = 0;
    float m_zoom = 1;
    bool m_isPanning = false;
    QPointF m_zoomCenter = QPoint(00,00);
    QPoint m_currentPosInImage;

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
