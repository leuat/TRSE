#include "fjongeditor.h"

FjongEditor::FjongEditor(QWidget *parent)
{
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateCycleNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateCycleNumberArea(QRect,int)));


    updateLineNumberAreaWidth(0);
    updateCycleNumberAreaWidth(0);
    highlightCurrentLine();
    InitCompleter(nullptr, nullptr);
    connect(this, SIGNAL(textChanged()), this, SLOT(onTextChanged()));
}
