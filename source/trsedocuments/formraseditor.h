#ifndef FORMRASEDITOR_H
#define FORMRASEDITOR_H

#include <QWidget>
#include "trsedocument.h"
#include "source/PmmEdit/highlighter.h"
#include "source/PmmEdit/codeeditor.h"
#include "source/messages.h"
#include "source/Compiler/lexer.h"
#include "source/Compiler/parser.h"
#include "source/Compiler/interpreter.h"
#include <QElapsedTimer>
#include "source/dialoghelp.h"
#include "source/dialogmemoryanalyze.h"

namespace Ui {
    class FormRasEditor;
}


class FormRasEditor : public TRSEDocument
{
    Q_OBJECT

public:
    explicit FormRasEditor(QWidget *parent = 0);
    ~FormRasEditor();
    Highlighter* highlighter = nullptr;
    QFont m_font;

    Interpreter interpreter;
    Parser parser;
    Lexer lexer;
    QString filename;
    QElapsedTimer timer;
    bool isInitialized=false;
    int m_searchFromPos = 0;
    int m_currentFromPos = 0;
    bool m_buildSuccess = false;
    void LoadRasFile(QString fileName);
    static void ExecutePrg(QString fileName, QString emulator);
    void InitDocument(WorkerThread *t, CIniFile *ini, CIniFile* iniProject) override;
    void setupEditor();
    void Compress();
    void Build() override;
    void Setup();
    void Run() override;
    void SetLights();
    void SetText(QString s);
    void SetupHighlighter();
    void SearchInSource();
    void UpdateColors() override;
    void UpdateFromIni() override;
    void AutoFormat() override;
    bool BuildStep();
    void FillFromIni();
    void FillToIni();

    void MemoryAnalyze() override;


    void Reload();

    void Save(QString filename) override;
    void Load(QString filename) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent* e) override;

    void Destroy() override {}

    void GotoLine(int ln) override;

    Ui::FormRasEditor* UI(){return ui;}

    void SetOutputText(QString txt);


private:

    static QString path;
    Ui::FormRasEditor *ui;

signals:
    void OpenOtherFile(QString filename, int ln);


private slots:
    void on_leSearch_textChanged();

    void on_leSearch_returnPressed();

    void on_leSearch_textChanged(const QString &arg1);

    void on_btnReplace_clicked();
    void on_chkExomize_stateChanged(int arg1);
    void on_chkPostOpt_stateChanged(int arg1);
};

#endif // FORMRASEDITOR_H
