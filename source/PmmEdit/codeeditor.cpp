
#include <QtWidgets>

#include "codeeditor.h"


CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);
    cycleNumberArea = new CycleNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateCycleNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateCycleNumberArea(QRect,int)));

    updateLineNumberAreaWidth(0);
    updateCycleNumberAreaWidth(0);
    highlightCurrentLine();
    InitCompleter(nullptr, nullptr);
}

void CodeEditor::setCompleter(QCompleter *c)
{
    if (c)
        QObject::disconnect(c, 0, this, 0);

    c = completer();

    if (!c)
        return;

    c->setWidget(this);
    c->setCompletionMode(QCompleter::PopupCompletion);
    c->setCaseSensitivity(Qt::CaseSensitive);

    QObject::connect(c, SIGNAL(activated(QString)),
                     this, SLOT(insertCompletion(QString)));

}

QCompleter *CodeEditor::completer() const
{
    return c;
}

QAbstractItemModel *CodeEditor::modelFromFile(const QString &fileName)
{

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        qDebug() << "Cannot open autocomplete file: " << fileName;
        return new QStringListModel(completer());
    }

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif
    QStringList words;

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        if (!line.isEmpty()) {
            words << line.trimmed();
            qDebug() << words;
        }
    }

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    return new QStringListModel(words, completer());
}

QAbstractItemModel *CodeEditor::modelFromTRSE(SymbolTable *symtab, Parser* parser)
{
    QStringList words;
    for (QString s: Syntax::s.builtInFunctions.keys()) {
        BuiltInFunction& bf = Syntax::s.builtInFunctions[s];
        QString params = "(";
        for (int i=0;i<bf.m_params.count()-1;i++)
            params +=",";
        params+=");";
        words.append(bf.m_name + params );
    }

    if (symtab!=nullptr) {
        for (QString s: symtab->m_constants.keys()) {
            words.append(s );
        }

        for (Symbol* s: symtab->m_symbols)
            words.append(s->m_name );
    }
    if (parser!=nullptr) {
        for (QString s: parser->m_procedures.keys()) {
            NodeProcedureDecl* n = (NodeProcedureDecl*)parser->m_procedures[s];
            int cnt = n->m_paramDecl.count();
            QString params = "(";
            for (int i=0;i<cnt-1;i++)
                params +=",";
            params+=");";

            words.append(s+params);
        }
    }

    return new QStringListModel(words, completer());

}



int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}

int CodeEditor::cycleNumberAreaWidth()
{

    return 80;
}

void CodeEditor::FixBackTab(QKeyEvent *e)
{

    QTextCursor cur = textCursor();
    int pos = cur.position();
    int anchor = cur.anchor();
    cur.setPosition(pos);

    cur.setPosition(pos-1,QTextCursor::KeepAnchor);
    setTextCursor(cur);
    cur = textCursor();


    if (cur.selectedText()=="\t") {
        cur.removeSelectedText();
        cur.setPosition(anchor-1);
        cur.setPosition(pos-1,QTextCursor::KeepAnchor);
    }
    else {
        cur.setPosition(anchor) ;
        cur.setPosition(anchor-1,QTextCursor::KeepAnchor);
        if (cur.selectedText() == "\t") {
            cur.removeSelectedText();
            cur.setPosition(anchor-1);
            cur.setPosition(pos-1,QTextCursor::KeepAnchor);
        }
        else {

// Its not a tab, so reset the selection to what it was
            cur.setPosition(anchor);
            cur.setPosition(pos,QTextCursor::KeepAnchor);
        }
    }
    setTextCursor(cur);

}
void CodeEditor::InitCompleter(SymbolTable* symTab, Parser* parser)
{

    QCompleter* completer;

    completer = new QCompleter(this);
//    completer->setModel(modelFromFile(":/resources/autocompletion.txt"));
    completer->setModel(modelFromTRSE(symTab, parser));
//    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseSensitive);
    completer->setWrapAround(false);

    c=completer;
    setCompleter(completer);

}



void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(),0,cycleNumberAreaWidth(),0);
}


void CodeEditor::updateCycleNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(),0,cycleNumberAreaWidth(),0);
}

void CodeEditor::keyPressEvent(QKeyEvent *e)
{

//    if (Qt::Key_Tab && (e->modifiers() & Qt::ShiftModifier))
    if (e->key() == Qt::Key_Backtab) {
        FixBackTab(e);
//        QPlainTextEdit::keyPressEvent(e);
//        return;
//        e->ignore();
      //  return;
    }


    if (c && c->popup()->isVisible()) {
            // The following keys are forwarded by the completer to the widget
           switch (e->key()) {
           case Qt::Key_Enter:
           case Qt::Key_Return:
           case Qt::Key_Escape:
           case Qt::Key_Tab:

           case Qt::Key_Backtab:
                e->ignore();
                return; // let the completer do default behavior
           default:
               break;
           }
        }
    if ((e->modifiers() & Qt::ControlModifier)) {
        c->popup()->hide();
        QPlainTextEdit::keyPressEvent(e);
        return;
    }


    if (!(e->modifiers() & Qt::ControlModifier))
    if (e->key()==Qt::Key_Tab || e->key()==Qt::Key_Backtab || e->key()==Qt::Key_Space || e->key()==Qt::Key_Backspace || e->key()==Qt::Key_Escape) {
        QPlainTextEdit::keyPressEvent(e);
        c->popup()->hide();
        return;
    }
        bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_Space); // CTRL+E
        if (!c || !isShortcut) // do not process the shortcut when we have a completer
            QPlainTextEdit::keyPressEvent(e);
//    We also handle other modifiers and shortcuts for which we do not want the completer to respond to.

        const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
        if (!c || (ctrlOrShift && e->text().isEmpty()))
            return;

        static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
        bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
        QString completionPrefix = textUnderCursor();
//        completionPrefix= completionPrefix.remove("\t");
  //      completionPrefix= completionPrefix.remove(" ");
       // qDebug() << completionPrefix;
        if (!isShortcut && (hasModifier || e->text().isEmpty()|| completionPrefix.length() < 3
                          || eow.contains(e->text().right(1)))) {
            c->popup()->hide();
            return;
        }

        if (completionPrefix != c->completionPrefix()) {
            c->setCompletionPrefix(completionPrefix);
            c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
        }

        QRect cr = cursorRect();
        cr.setWidth(c->popup()->sizeHintForColumn(0)
                    + c->popup()->verticalScrollBar()->sizeHint().width());


        c->complete(cr); // popup it up!
}

void CodeEditor::focusInEvent(QFocusEvent *e)
{
    if (c)
          c->setWidget(this);
      QPlainTextEdit::focusInEvent(e);

}

void CodeEditor::insertCompletion(const QString &completion)
{
    if (c->widget() != this)
        return;
    QTextCursor tc = textCursor();
    int extra = completion.length() - c->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    setTextCursor(tc);
}

QString CodeEditor::textUnderCursor() const
{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}


void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}


void CodeEditor::updateCycleNumberArea(const QRect &rect, int dy)
{

    if (dy)
        cycleNumberArea->scroll(0, dy);
    else
//        cycleNumberArea->update(rect.x()-cycleNumberArea->width()-1, rect.y(), cycleNumberArea->width(), rect.height());
      cycleNumberArea->update(0, rect.y(), cycleNumberArea->width(), rect.height());

//    if (rect.contains(viewport()->rect()))
  //      updateCycleNumberAreaWidth(0);
}




void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
    cycleNumberArea->setGeometry(QRect(cr.right()-cycleNumberAreaWidth(), cr.top(), cycleNumberAreaWidth(), cr.height()));
}



void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(QColor(10,40,50));

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}



void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), QColor(50,60,70));


    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::lightGray);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}


void CodeEditor::cycleNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(cycleNumberArea);

    painter.fillRect(event->rect(), QColor(50,60,70));


    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    painter.setPen(Qt::yellow);
    painter.drawText(0, top, cycleNumberArea->width(), fontMetrics().height(),
                 Qt::AlignLeft, "#Cycles");

    top+=30;
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            int ln = blockNumber+1;
           // qDebug() << m_cycles.count();
            if (m_cycles.contains(ln))
            {

                QString number = QString::number(m_cycles[ln]);
                painter.setPen(Qt::yellow);
                painter.drawText(0, top, cycleNumberArea->width(), fontMetrics().height(),
                             Qt::AlignLeft, number);
            }
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}
