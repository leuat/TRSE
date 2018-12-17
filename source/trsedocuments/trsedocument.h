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

#ifndef TRSEDOCUMENT_H
#define TRSEDOCUMENT_H

#include "source/workerthread.h"
#include "source/LeLib/util/cinifile.h"
#include <QWidget>
#include "source/LeLib/data.h"
#include <QApplication>
#include <QMessageBox>
#include <QProcess>

class TRSEDocument : public QWidget
{
    Q_OBJECT
public:
    TRSEDocument(QWidget* parent);


    bool m_documentIsChanged = false;
    QString m_currentSourceFile;
    QString m_currentFileShort;
    QString m_currentDir;

    QString m_fileExtension;
    static QPalette m_defaultPalette;
    QString m_outputText;

//    WorkerThread* m_updateThread;
    CIniFile* m_iniFile;
    CIniFile* m_projectIniFile;

    virtual void Save(QString filename) {}
    virtual void Load(QString filename) {}
    virtual void AutoFormat() {}
    bool SaveChanges();
    void SaveCurrent() {
        if (m_currentSourceFile=="") {
            emit requestSaveAs();
            return;
        }
        Save(m_currentSourceFile);
        Data::data.blink = true;
        emit updatePaletteSignal();
    }
    virtual void GotoLine(int ln) {}
    virtual void Build() {}
    virtual void Run() {}
    virtual void PrepareClose() {}

    virtual void MemoryAnalyze() {}

    virtual void UpdateFromIni() {}
    virtual void Init() {}
    virtual void UpdateColors() { }
    virtual void InitDocument(WorkerThread* t, CIniFile* ini, CIniFile* iniProject) {
      //  m_updateThread = t;
        m_iniFile = ini;
        m_projectIniFile = iniProject;

    }

    virtual void keyPressEvent(QKeyEvent *e);


    void UserDefined();

    virtual void Reload() {}


    virtual void Destroy() = 0;

signals:
    void emitFindFile();
    void requestCloseWindow();
    void updatePaletteSignal();
    void requestBuild();
    void requestBuildMain();
    void requestRunMain();
    void requestSaveAs();

};


#endif // TRSEDOCUMENT_H
