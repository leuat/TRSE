#include "customqtablewidget.h"

CustomQTableWidget::CustomQTableWidget(QWidget* parent): QTableWidget(parent)
{

}
void CustomQTableWidget::InitDoc(TRSEDocument *doc)
{
    m_doc = doc;

}

void CustomQTableWidget::keyPressEvent(QKeyEvent *e)
{
//    qDebug() << "KEYPRESS";
 /*   if (m_doc!=nullptr) {
        m_doc->keyPressEvent(e);

    }
        QTableWidget::keyPressEvent(e);*/
}


