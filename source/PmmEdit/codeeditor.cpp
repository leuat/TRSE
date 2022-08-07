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


#include <QtWidgets>
#include <QRegularExpression>
#include "codeeditor.h"


CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);
    cycleNumberArea = new CycleNumberArea(this);
    addressArea = new AddressArea(this);
    m_metrics = new QFontMetrics(font());
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateCycleNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateCycleNumberArea(QRect,int)));

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateAddressAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateAddressArea(QRect,int)));

    connect(this, SIGNAL(textChanged()), this, SLOT(onTextChanged()));

    updateLineNumberAreaWidth(0);
    updateCycleNumberAreaWidth(0);
    updateAddressAreaWidth(0);
//    this->installEventFilter(this);

    highlightCurrentLine();
    InitCompleter(nullptr, nullptr);

}

void CodeEditor::setCompleter(QCompleter *c)
{
    if (c)
        QObject::disconnect(c, nullptr, this, nullptr);

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

QAbstractItemModel *CodeEditor::modelFromTRSE(QSharedPointer<SymbolTable> symtab, Parser* parser)
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
    for (Token& s: Syntax::s.reservedWords) {
//        qDebug() << s.m_value.toLower();
        words.append(s.m_value.toLower());
    }


    if (symtab!=nullptr) {
        for (QString s: symtab->m_constants.keys()) {
            words.append(s );
        }

        for (QSharedPointer<Symbol> s: symtab->m_symbols)
            words.append(s->m_name );
    }
    if (parser!=nullptr) {
        for (QString s: parser->m_procedures.keys()) {
            QSharedPointer<NodeProcedureDecl> n = qSharedPointerDynamicCast<NodeProcedureDecl>(parser->m_procedures[s]);
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

// This method handles both the automatic indent, handling
// of automatic "end" addition or just keeping the current
// indent.
void CodeEditor::SetIndent(bool shift)
{
    QTextCursor cursor = textCursor();
    QString line = textCursor().block().text();
    QString space, lastWord, firstWord;
    qsizetype txtPos;

    // Find out where contents of line begin, after
    // the indent space
    txtPos = line.indexOf(QRegularExpression("\\S"), 0);
    if (txtPos == -1)
    {
        txtPos = line.length();
    }
    space = line.left(txtPos);

    // Automatic indent. First sanity check if we're at the end of line
    if(!cursor.atBlockEnd()) {
        insertPlainText("\n");
        return;
    }

    // The autocomplete & indent is enabled only for .RAS and .TRU files and
    // when either enabled in settings or shift-entered
    bool enable = (m_autoIndent | shift) & (m_fileType == RAS || m_fileType == TRU || m_fileType == INC);

    // Only keep the current indent if not enabled
    if (!enable)
    {
        cursor.beginEditBlock();
        insertPlainText("\n");
        insertPlainText(space);
        cursor.endEditBlock();
        return;
    }

    // First split line into words,
    // then store the first and last word if there is any.
    QStringList splitLine = line.trimmed().split(" ");
    if (!splitLine.isEmpty()) {
        firstWord = splitLine.first().toLower();
        lastWord = splitLine.last().toLower();
    }

    // Indent and automatically insert "end" for certain
    // words. If there is begin and document contains no
    // "end." then that is inserted instead semicolon-version.
    if ((lastWord == "begin")
            || lastWord == "record"
            || lastWord == "class"
            || lastWord == "asm"
            || (firstWord == "case" && lastWord == "of"))
    {
        cursor.beginEditBlock();
        insertPlainText("\n\t");
        insertPlainText(space);

        // Find if we already have end on right place
        bool hasEnd = false;
        qsizetype loc = 0;
        QString endTxt = cursor.document()->toPlainText().mid(cursor.position());
        QStringList lines = endTxt.split("\n");
        for (int i=0; i < lines.size(); i++)
        {
            loc = lines[i].indexOf(QRegularExpression("\\S"), 0);
            if (loc == -1) loc = lines[i].length();
            if (lines[i].trimmed().isEmpty()) continue;
            if (loc < txtPos) break;
            if (lines[i].trimmed().contains("begin", Qt::CaseInsensitive) && loc == txtPos) break;
            if (lines[i].trimmed().contains("end", Qt::CaseInsensitive)
                    && !lines[i].trimmed().contains("endif", Qt::CaseInsensitive)
                    && loc == txtPos) hasEnd = true;
        }

        // Only add "end" if we don't have it already
        if (!hasEnd) {
            insertPlainText("\n");
            insertPlainText(space);
            insertPlainText("end");
            // Special handling for "." after "begin"
            if (lastWord == "begin"
                    && !(cursor.document()->toPlainText().contains("end.",Qt::CaseInsensitive))
                    && (cursor.document()->toPlainText().trimmed().startsWith("program", Qt::CaseInsensitive)))
            {
                insertPlainText(".");
            }
            else
            {
                insertPlainText(";");
            }
            // Move cursor to the end of the previous line to make typing
            // a breeze.
            cursor.movePosition(QTextCursor::Up, QTextCursor::MoveAnchor, 1);
            cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::MoveAnchor, 1);
            setTextCursor(cursor);
        }
        cursor.endEditBlock();
    }
    // Don't insert the "end" after var, just indent
    else if (lastWord == "var")
    {
        cursor.beginEditBlock();
        insertPlainText("\n\t");
        insertPlainText(space);
        cursor.endEditBlock();
    }
    else
    {
        // If nothing else, just keep the old indent
        cursor.beginEditBlock();
        insertPlainText("\n");
        insertPlainText(space);
        cursor.endEditBlock();
    }
}

// Handle tab and backtab in code editor.
// Supports both single-line and selection.
void CodeEditor::TabBackTab(QKeyEvent* e, bool back)
{
    QTextCursor cursor = textCursor();

    // Check if we actually have selection or just tab/backtab
    // press in single line and handle them separately.
    if (cursor.hasSelection())
        // We have selection, handle multiple line indent/unindent
    {
        // Find out the selection start and end, expand
        // to beginning and end of line if needed
        int start = cursor.selectionStart();
        int end = cursor.selectionEnd();
        cursor.setPosition(start, QTextCursor::MoveAnchor);
        cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
        cursor.setPosition(end - 1, QTextCursor::KeepAnchor);
        cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
        setTextCursor(cursor);

        // Get selected text and split into lines
        QString str = cursor.selection().toPlainText();
        QStringList lines = str.split("\n");

        if (back)
            // Selection backtab handling
        {
            for (int i=0; i < lines.size(); i++)
            {
                if (lines[i].startsWith("\t") || lines[i].startsWith(" "))
                {
                    lines[i].remove(0, 1);
                    end--;
                }
            }
        }
        else
            // Selection forward tab handling
        {
            for (int i=0; i < lines.size(); i++)
            {
                lines[i].insert(0, "\t");
            }
        }

        // Move cursors back to selection and insert
        // new content
        str = lines.join("\n");
        cursor.beginEditBlock();
        cursor.removeSelectedText();
        cursor.insertText(str);
        cursor.endEditBlock();

        // Move cursor to make new selection of inserted text
        cursor.setPosition(start, QTextCursor::MoveAnchor);
        cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
        cursor.setPosition(end - 1, QTextCursor::KeepAnchor);
        cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
        setTextCursor(cursor);
    }
    else
        // Single line tab handling
    {
        if (back)
            // Backtab handling for single line
        {
            QString line = textCursor().block().text();
            if (line.startsWith("\t") || line.startsWith(" "))
            {
                int pos = cursor.position();
                cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor, 1);
                cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 1);
                cursor.removeSelectedText();
                cursor.setPosition(pos - 1);
            }
        }
        else
            // Forward single tab is simple
        {
            insertPlainText("\t");
        }
    }
    return;
}

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }
    int space = 3 + m_metrics->maxWidth() * (digits)*m_fontScale;

    return space;
}

int CodeEditor::cycleNumberAreaWidth()
{
    if (!m_displayCycles)
        return 0;
    return m_metrics->maxWidth()*7*m_fontScale;
}

int CodeEditor::AddressAreaWidth()
{
    if (!m_displayAddresses)
        return 0;
    if (m_addresses.keys().count()==0)
        return 0;

    return m_metrics->maxWidth() * 6*m_fontScale;

}

void CodeEditor::InitCompleter(QSharedPointer<SymbolTable>  symTab, Parser* parser)
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

void CodeEditor::mousePressEvent(QMouseEvent *e)
{
    QPlainTextEdit::mousePressEvent(e);
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);

    if (QApplication::keyboardModifiers() & Qt::ControlModifier)
        //        emit emitLookupWord(tc.selectedText());
        emit emitLookupWord();

}


void CodeEditor::onTextChanged() {
    m_textChanged = true;
}



void CodeEditor::update()
{
    setViewportMargins(AddressAreaWidth()+lineNumberAreaWidth(),0,cycleNumberAreaWidth(),0);

}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    update();
}

void CodeEditor::updateAddressAreaWidth(int newBlockCount)
{
    update();

}


void CodeEditor::updateCycleNumberAreaWidth(int /* newBlockCount */)
{
    update();
}


void CodeEditor::keyPressEvent(QKeyEvent *e)
{
    // Indent handling
    if (!(c && c->popup()->isVisible()) && e->key()==Qt::Key_Return) {
        SetIndent(e->modifiers() & Qt::ShiftModifier);
        return;
    }

/*
    if (e->key() == Qt::Key_U &&  (QApplication::keyboardModifiers() & Qt::ControlModifier)) {
        auto keep = m_textChanged;
        QPlainTextEdit::keyPressEvent(e);
        m_textChanged = false;
        return;
    }
*/



    // Handle completer popup
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

    // Tab/Backtab Handling
    if (e->key()==Qt::Key_Tab || e->key()==Qt::Key_Backtab) {
        TabBackTab(e, e->key()==Qt::Key_Backtab);
        return;
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

    if (m_autoComplete)
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

bool CodeEditor::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        qDebug("Ate key press %d", keyEvent->key());

        if (keyEvent->key() == Qt::Key_Control) {
            QTextCursor tc = textCursor();
            tc.select(QTextCursor::WordUnderCursor);

            QTextCharFormat defcharfmt = currentCharFormat();
            QTextCharFormat newcharfmt = defcharfmt;
            newcharfmt.setFontUnderline( true );
            setCurrentCharFormat( newcharfmt );
            setTextCursor( tc ); // added
            setCurrentCharFormat( defcharfmt );
//            qDebug() << "HERE";
            //        emit emitLookupWord(tc.selectedText());
        }



        return true;
    } else {
        // standard event processing
        return QObject::eventFilter(obj, event);

    }
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
void CodeEditor::updateAddressArea(const QRect &rect, int dy)
{
    if (dy)
        addressArea->scroll(0, dy);
    else
        //        cycleNumberArea->update(rect.x()-cycleNumberArea->width()-1, rect.y(), cycleNumberArea->width(), rect.height());
        addressArea->update(0, rect.y(), addressArea->width(), rect.height());

    //    updateAddressAreaWidth(0);
    //    if (rect.contains(viewport()->rect()))
    //      updateCycleNumberAreaWidth(0);

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
    lineNumberArea->setGeometry(QRect(cr.left()+AddressAreaWidth(), cr.top(), lineNumberAreaWidth(), cr.height()));
    cycleNumberArea->setGeometry(QRect(cr.right()-cycleNumberAreaWidth(), cr.top(), cycleNumberAreaWidth(), cr.height()));
    addressArea->setGeometry(QRect(cr.left(), cr.top(), AddressAreaWidth(), cr.height()));
}



void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = currentLineColor;//  QColor(QColor(10,40,50));

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}



void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event) {
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), lineNumberBackgroundColor);


    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(lineNumbersColor);
            painter.setFont(m_font);
            painter.drawText(0, top, lineNumberArea->width(), m_metrics->height(),
                             Qt::AlignRight | Qt::AlignVCenter, number);

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
    painter.setFont(m_font);

    painter.fillRect(event->rect(), cyclesBackgroundColor);


    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    painter.setPen(cyclesColor);
    painter.setFont(m_font);

    painter.drawText(0, top, cycleNumberArea->width(), m_metrics->height(),
                     Qt::AlignLeft  | Qt::AlignVCenter, "Cycles");

    //    top+=20;
    top+=m_metrics->height();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            int ln = blockNumber+1;
            // qDebug() << m_cycles.count();
            if (m_cycles.contains(ln))
            {

                QString number = QString::number(m_cycles[ln]);
                painter.setPen(cyclesColor);
                painter.drawText(0, top, cycleNumberArea->width(), m_metrics->height(),
                                 Qt::AlignLeft | Qt::AlignVCenter, number);
            }
            if (m_blockCycles.contains(ln))
            {

                int val = 0;
                if (m_cycles.contains(ln))
                    val+=m_cycles[ln];
                QString number = QString::number(m_blockCycles[ln]+val);
                painter.setPen(blockCyclesColor);
                painter.drawText(0, top, cycleNumberArea->width(), m_metrics->height(),
                                 Qt::AlignLeft  | Qt::AlignVCenter, number);
            }
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

void CodeEditor::addressAreaPaintEvent(QPaintEvent *event) {
    QPainter painter(addressArea);
    painter.setFont(m_font);

    painter.fillRect(event->rect(), cyclesBackgroundColor);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    painter.setPen(cyclesColor);
    painter.drawText(0, top, addressArea->width(), m_metrics->height(),
                     Qt::AlignLeft | Qt::AlignVCenter, "");

    //    top+=20;
    top+=m_metrics->height();
    int bottom = top + (int) blockBoundingRect(block).height();
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            int ln = blockNumber+2;
            // qDebug() << m_cycles.count();
            if (m_addresses.contains(ln))
            {

                QString number = Util::numToHex(m_addresses[ln]).toUpper();
                for (int i=0;i<5-number.length();i++)
                    number.insert(1,"0");
                painter.setPen(cyclesColor);
                painter.drawText(10, top, addressArea->width(), m_metrics->height(),
                                 Qt::AlignLeft | Qt::AlignVCenter, number);
            }
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

void CodeEditor::ToggleComments()
{
    QString s = textCursor().selection().toPlainText();

    QStringList SL = s.split("\n");
    for(int i=0; i<SL.length(); i++){
        if(SL.at(i).trimmed().startsWith("//")){
            s = SL.at(i);
            s.remove(0,2);//.replace("//","");
        }else{
            if (SL.at(i).trimmed()!="")
                s = "//" + SL.at(i);
            else s = SL.at(i);
        }
        if(i<SL.length()-1) s.append("\n");
        //        qDebug() << s;
        textCursor().insertText(s);
    }

}
