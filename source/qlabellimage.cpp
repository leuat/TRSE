#include "qlabellimage.h"

QLabelLImage::QLabelLImage()
{
       setMouseTracking(true);
}

void QLabelLImage::mouseMoveEvent(QMouseEvent *e)
{
    if (m_work==nullptr)
        return;
    if (m_work->m_currentImage==nullptr)
        return;
    if (m_updateThread==nullptr)
        return;



    QPointF pos = QCursor::pos() -mapToGlobal(QPoint(0,0));
    pos.setX(pos.x()*(float)m_work->m_currentImage->m_image->m_width/width());
    pos.setY(pos.y()*(float)m_work->m_currentImage->m_image->m_height/height());
    //m_prevPos = m_currentPos;
    // m_currentPos = QPoint(pos.x(), pos.y());
    m_updateThread->m_prevPos = m_updateThread->m_currentPos;
    m_updateThread->m_currentPos = QPoint(pos.x(), pos.y());
   // qDebug() << "pp:" <<m_updateThread->m_prevPos;
   // qDebug() << "cp:" <<m_updateThread->m_currentPos;

}



