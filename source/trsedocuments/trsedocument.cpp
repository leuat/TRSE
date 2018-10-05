#include "trsedocument.h"

TRSEDocument::TRSEDocument(QWidget* parent) : QWidget(parent)
{

}

bool TRSEDocument::SaveChanges()
{
    if (!m_documentIsChanged)
        return true;
    QMessageBox msgBox;
    msgBox.setText("The document has been modified.");
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    msgBox.setWindowTitle("Save document");
    int ret = msgBox.exec();
    if (ret==QMessageBox::Save)
        SaveCurrent();
    if (ret==QMessageBox::Cancel)
        return false;

    return true;

}

void TRSEDocument::keyPressEvent(QKeyEvent *e) {

    if (e->key() == Qt::Key_S &&  (QApplication::keyboardModifiers() & Qt::ControlModifier)) {
        SaveCurrent();
    }
    if (e->key() == Qt::Key_B &&  (QApplication::keyboardModifiers() & Qt::ControlModifier)) {
        Build();
    }

}
