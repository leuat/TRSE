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



#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QTextEdit>
#include <QObject>
#include <QHash>
#include <QCompleter>
#include <QStringListModel>
#include "source/Compiler/symboltable.h"
#include "source/Compiler/syntax.h"
#include "source/Compiler/parser.h"
#include "source/LeLib/util/cinifile.h"

class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;



class LineNumberArea;



class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    bool m_textChanged = false;
    CodeEditor(QWidget *parent = 0);
    QColor currentLineColor, lineNumberBackgroundColor, cyclesBackgroundColor, cyclesColor, blockCyclesColor, lineNumbersColor;
    void setCompleter(QCompleter *c);

    void InitColors(CIniFile cols) {
        currentLineColor = cols.getColor("currentline");
        lineNumberBackgroundColor = cols.getColor("linenumbersbackground");
        cyclesBackgroundColor = cols.getColor("cyclesbackground");
        blockCyclesColor = QColor(255,255,80);
        cyclesColor = cols.getColor("cycles");
        lineNumbersColor = cols.getColor("linenumbers");

    }
    QCompleter *completer() const;
    QAbstractItemModel *modelFromFile(const QString& fileName);
    QAbstractItemModel *modelFromTRSE(QSharedPointer<SymbolTable>  symtab, Parser* parser);

    QFont m_font;
    QFontMetrics* m_metrics;
    bool m_displayCycles = true;
    bool m_displayAddresses = true;
    float m_fontScale = 0.8;
    enum FileType {RAS, TRU, INC, ASM, FJO};
    FileType m_fileType = RAS;
    bool m_autoComplete = true;
    bool m_autoIndent = true;


    // Handle indenting automatically if enabled in settings
    void SetIndent(bool shift);
    // Tab/Backtab handling in editor
    void TabBackTab(QKeyEvent* e, bool back);
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
    void cycleNumberAreaPaintEvent(QPaintEvent *event);
    int cycleNumberAreaWidth();
    int AddressAreaWidth();
    void addressAreaPaintEvent(QPaintEvent *event);
    void ToggleComments();
    void InitCompleter(QSharedPointer<SymbolTable>  m_symTab, Parser* parser);

    QHash<int,int> m_cycles, m_blockCycles;
    QHash<int,int> m_addresses;


    void mousePressEvent(QMouseEvent *e) override;

    void RepaintCycles() {
        cycleNumberArea->repaint();
        addressArea->repaint();
    }
    void update();

protected slots:
    void onTextChanged();

signals:
    void emitLookupWord();


protected:
    void resizeEvent(QResizeEvent *event) override;
//    void cycleresizeEvent(QResizeEvent *event) override;

protected slots:
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);
    void updateLineNumberAreaWidth(int newBlockCount);


    void updateCycleNumberArea(const QRect &, int);
    void updateCycleNumberAreaWidth(int newBlockCount);


    void updateAddressArea(const QRect &, int);
    void updateAddressAreaWidth(int newBlockCount);

protected:
    QWidget *lineNumberArea;
    QWidget *cycleNumberArea;
    QWidget *addressArea;


 // Completer stuff

protected:
    void keyPressEvent(QKeyEvent *e) override;
    void focusInEvent(QFocusEvent *e) override;

protected slots:
    void insertCompletion(const QString &completion);

protected:
    QString textUnderCursor() const;

protected:
    QCompleter *c = nullptr;


    bool eventFilter(QObject *obj, QEvent *event) override;

};

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditor *editor) : QWidget(editor) {
        codeEditor = editor;
    }

    QSize sizeHint() const override {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

protected:
    CodeEditor *codeEditor;
};


class CycleNumberArea : public QWidget
{
public:
    CycleNumberArea(CodeEditor *editor) : QWidget(editor) {
        codeEditor = editor;
    }

    QSize sizeHint() const override {
        return QSize(codeEditor->cycleNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        codeEditor->cycleNumberAreaPaintEvent(event);
    }

protected:
    CodeEditor *codeEditor;
};

class AddressArea : public QWidget
{
public:
    AddressArea(CodeEditor *editor) : QWidget(editor) {
        codeEditor = editor;
    }

    QSize sizeHint() const override {
        return QSize(codeEditor->AddressAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        codeEditor->addressAreaPaintEvent(event);
    }

protected:
    CodeEditor *codeEditor;
};



#endif
