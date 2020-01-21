#include "abstractimageeditor.h"

AbstractImageEditor::AbstractImageEditor()
{

}

bool AbstractImageEditor::AIE_eventFilter(QObject *object, QEvent *event, QWidget* p) {
/*    if (m_cancel)
        return true;
    if(object==this) {*/
  //      qDebug() << event;
        AIE_mouseMoveEvent((QMouseEvent*)event,p);
        if (event->type()==QEvent::Enter) {
            Data::data.imageEvent = 1;
        }
        if (event->type()==QEvent::Leave) Data::data.imageEvent = 0;
        if (m_buttonDown)
            m_updateThread->m_currentButton=m_keepButton;

        return false;
  //  }
//    return true;
}

void AbstractImageEditor::AIE_mouseReleaseEvent(QMouseEvent *e)
{
    if (m_cancel)
        return;
    if (m_updateThread==nullptr)
        return;
    m_prevButton = m_updateThread->m_currentButton;
    if (m_updateThread->m_currentButton==1) {
        m_updateThread->m_currentButton = -1;
    }
    else
        m_updateThread->m_currentButton = 0;

    m_buttonDown = false;

//    emit EmitMouseMove();
//    emit EmitMouseRelease();

}

/*void AbstractImageEditor::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Space)  {
        emit EmitSwapDisplayMode();
    }
}
*/
void AbstractImageEditor::AIE_mousePressEvent(QMouseEvent *e)
{
    if (m_cancel)
        return;
    if (m_updateThread==nullptr)
        return;
    m_updateThread->m_work->m_currentImage->AddUndo();



    if(e->buttons() == Qt::RightButton) {
        m_imageChanged = true;
        m_updateThread->m_currentButton = 2;
        m_keepButton = 2;
        if (e->modifiers() & Qt::ShiftModifier) {
            m_updateThread->m_prevPos = m_updateThread->m_currentPos;
            m_updateThread->m_currentButton = 4;
            m_keepButton = 4;

        }
        m_buttonDown = true;
    }

    if(e->buttons() == Qt::LeftButton) {
        m_updateThread->m_currentButton = 1;
        m_keepButton = 1;
        m_buttonDown = true;
        m_imageChanged = true;
    }
/*    if(e->buttons() == Qt::RightButton) {
        m_updateThread->m_currentButton = 2;
        m_buttonDown = true;
        m_imageChanged = true;
    }
*/
  //  emit EmitMouseMove();

}

bool AbstractImageEditor::AIE_mouseMoveEvent(QMouseEvent *e, QWidget* p)
{
    if (m_cancel)
        return false;
    if (m_work==nullptr)
        return false;
    if (m_work->m_currentImage==nullptr)
        return false;
    if (m_work->m_currentImage->m_image==nullptr)
        return false;
    if (m_updateThread==nullptr)
        return false;


    m_active=true;

    QPointF pos = QCursor::pos() -p->mapToGlobal(QPoint(0,0));
    pos.setX(pos.x()*(float)m_work->m_currentImage->m_image->m_width/p->width());
    pos.setY(pos.y()*(float)m_work->m_currentImage->m_image->m_height/p->height());

    m_updateThread->m_prevPos = m_updateThread->m_currentPos;
    m_updateThread->m_currentPos = QPointF(pos.x(), pos.y());

    if ((m_updateThread->m_prevPos-m_updateThread->m_currentPos).manhattanLength()>0.0)
        return true;

    return false;
    //    emit EmitMouseMove();

}
