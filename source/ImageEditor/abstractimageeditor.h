#ifndef ABSTRACTIMAGEEDITOR_H
#define ABSTRACTIMAGEEDITOR_H

#include <QObject>
#include "source/workerthread.h"
#include <QGLWidget>

class AbstractImageEditor// : public QObject
{
//    Q_OBJECT
public:
    AbstractImageEditor();

    bool m_imageChanged = false;
    int m_time=0;
    WorkerThread* m_updateThread = nullptr;
    ImageWorker* m_work = nullptr;
    bool m_active=false;
    bool m_cancel = false;
    bool m_buttonDown = false;
    int m_prevButton = 0;
    int m_keepButton = 1;


    bool AIE_mouseMoveEvent(QMouseEvent *e, QWidget* p);

    void CancelAll() {m_cancel=true; m_active=false;}

    bool AIE_eventFilter(QObject *object, QEvent *event,QWidget* p);

    void AIE_mouseReleaseEvent(QMouseEvent *e);
//    void keyPressEvent(QKeyEvent *e) override;

    void AIE_mousePressEvent(QMouseEvent *e);




};

#endif // ABSTRACTIMAGEEDITOR_H
