#pragma once
#include <QLabel>
#include "source/workerthread.h"

class QLabelLImage : public QLabel
{
public:
    QLabelLImage();
    QLabelLImage(QWidget* parent) : QLabel(parent) {
        setMouseTracking(true);
        installEventFilter(this);
    }
    int m_time=0;
    WorkerThread* m_updateThread = nullptr;
    ImageWorker* m_work = nullptr;
    bool m_active=false;

    void mouseMoveEvent(QMouseEvent *e) override;
//    void mouseLeaveEvent(QMouseEvent* e) override;

    bool eventFilter(QObject *object, QEvent *event) override;

    void mouseReleaseEvent(QMouseEvent*  e) override;
    void mousePressEvent(QMouseEvent* e) override;
};

