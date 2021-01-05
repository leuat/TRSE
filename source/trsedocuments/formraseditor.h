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

#ifndef FORMRASEDITOR_H
#define FORMRASEDITOR_H

#include <QWidget>
#include "trsedocument.h"
#include "source/PmmEdit/highlighter.h"
#include "source/PmmEdit/codeeditor.h"
#include "source/messages.h"
#include "source/Compiler/lexer.h"
#include "source/Compiler/parser.h"
#include "source/Compiler/compilers/compiler.h"
#include <QElapsedTimer>
#include "source/dialoghelp.h"
#include "source/dialogmemoryanalyze.h"
#include "source/Compiler/misc/machinecodeanalyzer.h"
#include "source/OrgAsm/orgasm.h"
#include "source/OrgAsm/orgasmlexer.h"

#include "source/Compiler/systems/abstractsystem.h"
#include "source/Compiler/systems/factorysystem.h"

#include "source/Compiler/sourcebuilder.h"

#include <QThread>
#include <QElapsedTimer>
#include <QSharedPointer>
#include <QTimer>
namespace Ui {
    class FormRasEditor;
}


/*class Builder : public QThread  {
public:




};
*/

class BuilderThread : public QThread {
    Q_OBJECT
public:
    QSharedPointer<SourceBuilder> m_builder;
    QString m_source, m_filename;
    bool m_isRunning = false;
    void run() override;

signals:
    void emitText();
    void emitSuccess();
    void emitError();

};


class FormRasEditor : public TRSEDocument
{
    Q_OBJECT

public:
    explicit FormRasEditor(QWidget *parent = 0);
    ~FormRasEditor();
    static bool m_broadcast;
    Highlighter* highlighter = nullptr;
    QFont m_font;
    QSharedPointer<DialogHelp> m_help = nullptr;
    bool m_isTRU = false;

    BuilderThread m_builderThread;
    QElapsedTimer m_timer;
    int m_lastBuild= 0;
    QVector3D m_curCol = QVector3D(180,160,50);;
    QVector3D m_startCol = QVector3D(64,32,0);
    QVector3D m_endCol = QVector3D(255,200,100);

    static QString m_globalOutput;

    MachineCodeAnalyzer m_mca;
    QString filename;
    bool isInitialized=false;
    int m_searchFromPos = 0;
    int m_currentFromPos = 0;

    void FocusOnOutput();

    void setOutputText(QString text) override;

    void LoadRasFile(QString fileName);
    void ExecutePrg(QString fileName,  QString system);
    void InitDocument(WorkerThread *t, QSharedPointer<CIniFile> ini, QSharedPointer<CIniFile> iniProject) override;
    void setupEditor();
    void Compress();
    void Build(bool isShadow=false) override;
    bool BuildDiskFiles(QStringList& d64Params, QString data);
    void Setup();

    void Focus() override;

    bool isBuilding() override;

    void Run() override;
    void SetLights();
    void SetText(QString s);
    void SetupHighlighter();
    void SearchInSource(QString text) override;
    void UpdateColors() override;
    void UpdateFromIni() override;
    void AutoFormat() override;
  //  bool BuildStep();
    void Assemble6502(QString& text);
    void FillFromIni();
    void FillToIni();

    void MemoryAnalyze(bool isHidden) override;


    void Reload() override;

    void Save(QString filename) override;
    bool Load(QString filename) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent* e) override;

    void Destroy() override {}
    void TestForCodeOverwrite(int codeEnd, QString& output);
    void GotoLine(int ln) override;

    Ui::FormRasEditor* UI(){return ui;}

    void SetOutputText(QString txt);

    void BuildNes(QString prg);
    void LookupSymbolUnderCursor() override;
    void LookupAssemblerUnderCursor() override;
    void ToggleComment() override;

public slots:
    void HandleBuildComplete();

private:

//    bool VerifyMachineCodeZP(QString fname);

    static QString path;
    Ui::FormRasEditor *ui;
    void Help(QString word);



signals:
    void emitSearchSymbols();
    void OpenOtherFile(QString filename, int ln);
    void NotifyOtherSourceFiles(QSharedPointer<SourceBuilder> builder);
    void emitFailure();
private slots:
    void ShadowBuild();

    void on_leSearch_textChanged();

    void AcceptBuildString();
    void on_leSearch_returnPressed();

    void on_leSearch_textChanged(const QString &arg1);
    void on_btnReplace_clicked();
    void on_chkExomize_stateChanged(int arg1);
    void on_chkRemoveUnusedSymbols_stateChanged(int arg1);
    void on_chkPostOpt_stateChanged(int arg1);

    void HandleUpdateBuildText();
    void HandleBuildError();
    void HandleErrorDialogs(QString& output);
//    void HandleRun();
    void on_chkWarnings_stateChanged(int arg1);
    void on_btnHelpExomize_clicked();
    void on_btnHelpRemoveUnusedSymbols_clicked();
    void on_btnHelpPostOptimize_clicked();
    void on_chkDisplayAddresses_stateChanged(int arg1);
    void on_chkDisplayCycles_stateChanged(int arg1);
    void on_btnViewHelp_clicked();
};

#endif // FORMRASEDITOR_H
