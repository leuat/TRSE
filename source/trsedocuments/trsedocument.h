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
    enum Type {FJONG, RAS, PAW };

    Type m_type;
    bool m_run = false;

    bool m_hasFocus = true;

    bool m_documentIsChanged = false;
    QString m_currentSourceFile;
    QString m_currentFileShort;
    QString m_currentDir;
    QString m_programEndingType = "prg";

    QString m_fileExtension;
    static QPalette m_defaultPalette;
    QString m_outputText;

//    WorkerThread* m_updateThread;
    QSharedPointer<CIniFile> m_iniFile;
    QSharedPointer<CIniFile> m_projectIniFile;


    virtual bool isBuilding() {return false;}
    virtual void Save(QString filename) {}
    virtual bool Load(QString filename) { return false;}
    virtual void AutoFormat() {}

    virtual void ToggleComment() {}

    bool SaveChanges();
    void SaveCurrent() {
        if (m_currentSourceFile=="") {
            emit requestSaveAs();
            return;
        }
        if (m_iniFile->getdouble("auto_backup")==1.0)
            Backup();
        Save(m_currentSourceFile);
        Data::data.blink = true;
        emit updatePaletteSignal();
    }
    virtual void GotoLine(int ln) {}
    virtual void Build(bool isShadow = false) {}
    virtual void Run() {}
    virtual void PrepareClose() {}




    void Backup();

    virtual void MemoryAnalyze(bool isHidden) {}
    virtual void LookupSymbolUnderCursor() {}
    virtual void LookupAssemblerUnderCursor() {}

    virtual void UpdateFromIni() {}
    virtual void Init() {}
    virtual void UpdateColors() { }
    virtual void InitDocument(WorkerThread* t, QSharedPointer<CIniFile> ini, QSharedPointer<CIniFile> iniProject) {
      //  m_updateThread = t;
        m_iniFile = ini;
        m_projectIniFile = iniProject;

    }

    virtual void Focus() {}
    virtual void keyPressEvent(QKeyEvent *e);


    void UserDefined();

    virtual void Reload() {}

    virtual void setOutputText(QString text) {}
    virtual void SearchInSource(QString text) {}


    virtual void Destroy() = 0;

signals:
    void emitFindFile();
    void emitNewRas();
    void emitNewImage();
    void requestCloseWindow();
    void updatePaletteSignal();
    void requestBuild();
    void requestBuildMain(bool isRun);
    void requestRunMain();
    void requestSaveAs();
    void emitSuccess();
    void emitGotoSymbol(QString sym);
    void emitGotoAssemblerLine(QString rasSrc, int lineNuber);

};


#endif // TRSEDOCUMENT_H
