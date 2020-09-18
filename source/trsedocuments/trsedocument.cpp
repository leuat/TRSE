/*
 * Turbo Rascal Syntax error, “;” expected but “BEGIN” (TRSE, Turbo Rascal SE)
 * 8 bit software development IDE for the Commodore 64
 * Copyright (C) 2018  Nicolaas Ervik Groeneboom (nicolaas.groeneboom@gmail.com)
 *
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program (LICENSE.txt).
 *   If not, see <https://www.gnu.org/licenses/>.
*/

#include "trsedocument.h"
#include <QDateTime>

QPalette TRSEDocument::m_defaultPalette;

TRSEDocument::TRSEDocument(QWidget* parent) : QWidget(parent)
{

}

bool TRSEDocument::SaveChanges()
{
//    qDebug() << "Document changed: " << m_documentIsChanged;
    if (!m_documentIsChanged)
        return true;
    QMessageBox msgBox;
    msgBox.setText("The document has been modified.");
    msgBox.setInformativeText("Do you want to save your changes in '"+m_currentFileShort+"'?");
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

void TRSEDocument::Backup()
{
    QString op = m_currentDir + ".backup";
    if (!QDir().exists(op))
        QDir().mkdir(op);


    QString washedFile = m_currentFileShort;
    washedFile = washedFile.replace(QDir::separator(),"_");

    QString fileDir = op + QDir::separator() + washedFile;

    if (!QDir().exists(fileDir))
        QDir().mkdir(fileDir);

    QString fileName = fileDir +QDir::separator()+ "backup_"+ QDateTime::currentDateTime().toString().replace(":","_") +".bak";

    Save(fileName);

    QDir folder (fileDir);
    QStringList files = folder.entryList(QDir::Files, QDir::Time);
    // delete the oldest files
    int keepCount = m_iniFile->getdouble("backup_files_count");
    int i=0;
    for (QString f : files) {
        if (i++>=keepCount) {
            QString name =fileDir+QDir::separator()+f;
//            qDebug() << "Deleting file "<<name;
            QFile::remove(name);
        }
    }


}

void TRSEDocument::keyPressEvent(QKeyEvent *e) {

//    qDebug() << (e->key() == 16777249) << e->key() << Qt::Key_S <<(e->modifiers() & Qt::ControlModifier);
    if (((e->key() == Qt::Key_S) &&  ((QApplication::keyboardModifiers() & Qt::ControlModifier)))) {
        SaveCurrent();
    }
    if (e->key() == Qt::Key_B &&  (QApplication::keyboardModifiers() & Qt::ControlModifier)) {
        Build();
    }

    if (e->key() == Qt::Key_T &&  (QApplication::keyboardModifiers() & Qt::ControlModifier)) {
        UserDefined();
    }

    if (e->key() == Qt::Key_L &&  (QApplication::keyboardModifiers() & Qt::ControlModifier)) {
        emit emitFindFile();
    }

    if (e->key() == Qt::Key_N &&  (QApplication::keyboardModifiers() & Qt::ControlModifier))
        emit emitNewRas();

    if (e->key() == Qt::Key_I &&  (QApplication::keyboardModifiers() & Qt::ControlModifier))
        emit emitNewImage();


    if (e->key()==Qt::Key_W && (QApplication::keyboardModifiers() & Qt::ControlModifier))
        emit requestCloseWindow();

}

void TRSEDocument::UserDefined()
{
    QProcess p;

    QString s = m_iniFile->getString("user_defined_command").trimmed();
    if (s=="")
        return;

    QString src = m_currentSourceFile;

    if (m_projectIniFile->getString("main_ras_file")!="none")
        if (m_projectIniFile->getString("main_ras_file")!=m_currentFileShort)
            src = m_currentDir+ m_projectIniFile->getString("main_ras_file");



    s=s.replace("@prg", src.replace(".ras", "."+m_programEndingType));
    QStringList params = s.split(" ");
    QString cmd = params[0];
    params.removeFirst();
    p.startDetached(cmd,params);
    p.waitForFinished();

}
