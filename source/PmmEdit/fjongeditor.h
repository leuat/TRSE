#ifndef FJONGEDITOR_H
#define FJONGEDITOR_H

#include <QPlainTextEdit>
#include "codeeditor.h"

class FjongEditor : public CodeEditor
{

    Q_OBJECT

public:
    FjongEditor(QWidget *parent = nullptr);


};

#endif // FJONGEDITOR_H
