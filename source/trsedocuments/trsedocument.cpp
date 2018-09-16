#include "trsedocument.h"

TRSEDocument::TRSEDocument(QWidget* parent) : QWidget(parent)
{

}

void TRSEDocument::keyPressEvent(QKeyEvent *e) {

    if (e->key() == Qt::Key_S &&  (QApplication::keyboardModifiers() & Qt::ControlModifier)) {
        SaveCurrent();
    }
    if (e->key() == Qt::Key_B &&  (QApplication::keyboardModifiers() & Qt::ControlModifier)) {
        Build();
    }

}
