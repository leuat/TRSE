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

