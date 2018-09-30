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

  //  qDebug() << "WHOO" << pos;
/*    if (pos.x()>=m_work->m_currentImage->m_image->m_width || pos.x()<0) {
        Data::data.Redraw();
        //qDebug() << "OUTSIDE";
        m_active=false;
    }
*/
    //m_prevPos = m_currentPos;
    // m_currentPos = QPoint(pos.x(), pos.y());
    if (m_time%256==0)
    m_updateThread->m_prevPos = m_updateThread->m_currentPos;
    m_updateThread->m_currentPos = QPointF(pos.x(), pos.y());

/*    qDebug() << this;
    Data::data.Redraw();
*/
//    qDebug() << "WT MV:" << m_updateThread->m_currentButton;

   // qDebug() << "pp:" <<m_updateThread->m_prevPos;
   // qDebug() << "cp:" <<m_updateThread->m_currentPos;
    m_time++;
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


}

void QLabelLImage::mousePressEvent(QMouseEvent *e)
{
    if (m_cancel)
        return;
    if (m_updateThread==nullptr)
        return;
    m_updateThread->m_work->m_currentImage->AddUndo();

        if(e->buttons() == Qt::RightButton) {
            m_updateThread->m_currentButton = 2;
            if (e->modifiers() & Qt::ShiftModifier) {
                m_updateThread->m_prevPos = m_updateThread->m_currentPos;
                m_updateThread->m_currentButton = 4;
            }
        }

        if(e->buttons() == Qt::LeftButton) {
            m_updateThread->m_currentButton = 1;
        }
  //  qDebug() << m_updateThread->m_currentButton;
}



