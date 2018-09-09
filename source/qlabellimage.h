#pragma once
#include <QLabel>
#include "source/workerthread.h"

class QLabelLImage : public QLabel
{
public:
    QLabelLImage();
    QLabelLImage(QWidget* parent) : QLabel(parent) {
        setMouseTracking(true);
    }
    WorkerThread* m_updateThread = nullptr;
    ImageWorker* m_work = nullptr;

    void mouseMoveEvent(QMouseEvent *e) override;

};

