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

void CodeEditor::SetIndent()
{
    QTextCursor cursor;
      QString line=textCursor().block().text();
      QString space;
      QChar c;
      int size=line.size();

  // search for white spaces at the beginning of the line
      for(int i=0; i<size; i++)
          {
          c=line[i];

          if(! c.isSpace())
              {
              space=line.left(i);
              break;
              }
          }
      insertPlainText("\n");
      insertPlainText(space);
}



int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }
    int space = 3 + m_metrics->maxWidth() * (digits);

    return space;
}

int CodeEditor::cycleNumberAreaWidth()
{
    if (!m_displayCycles)
        return 0;
    return m_metrics->maxWidth()*7;
}

int CodeEditor::AddressAreaWidth()
{
    if (!m_displayAddresses)
        return 0;
    if (m_addresses.keys().count()==0)
        return 0;

    return m_metrics->maxWidth() * 6;

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

//    if (Qt::Key_Tab && (e->modifiers() & Qt::ShiftModifier))
    if (e->key() == Qt::Key_Backtab) {
        FixBackTab(e);
//        QPlainTextEdit::keyPressEvent(e);
//        return;
//        e->ignore();
      //  return;
    }
    if (!(c && c->popup()->isVisible()))
    if (e->key()==Qt::Key_Return ) {
        SetIndent();
        //exit(1);
        return;
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
    else {

    }


//    if (!(e->modifiers() & Qt::ControlModifier))
    // Tab selection

    if (e->key()==Qt::Key_Tab || e->key()==Qt::Key_Backtab) {
        QTextCursor cursor = textCursor();
        int lineCopy = cursor.anchor();
        if (cursor.position()<lineCopy)
            lineCopy =cursor.position();
        QString txt = cursor.selectedText();
        txt = txt.replace("\u2029","\n");
        // Damn hack.. above won't work on windows
        for (int i=0;i<txt.count();i++) {
            if (txt.at(i).unicode()==8233)
                txt[i] ='\n';
        }

        QStringList lst = txt.split('\n');

        QString str="";
        bool removeLast = false;
        for (QString& s: lst) {
            if (e->key()==Qt::Key_Tab) {
                if (s.trimmed()!="")
                    str+="\t"+s+"\n";
                else str+="\n";
                removeLast = true;
            }
            else { // Backtab!

                int i = s.indexOf("\t");
                if (s.count()>0 && i<s.count()) {
                    if (s[i]=='\t')
                        str+=s.remove(i,1)+"\n";
                    else str+=s+"\n";
                }

            }
        }
        if (removeLast)
            str.remove(str.length()-1,1);

        if(cursor.hasSelection())
        {

            cursor.beginEditBlock();
            cursor.insertText(str);
            cursor.setPosition(lineCopy, QTextCursor::KeepAnchor);
            cursor.endEditBlock();
            setTextCursor(cursor);

            return;
        }

  /*      static QChar par(0x2029); // Qt uses paragraph separators
        QTextCursor cursor = textCursor();
        QString selected = cursor.selectedText();
        qint32 amountOfLines = selected.count(par) + 1;
        static QString tab = QString(" ").repeated(4);
        QKeyEvent* event = e;

        // Adds tabs in front of the selected block(s)
        if (event->key() == Qt::Key_Tab && !textCursor().selectedText().isEmpty()) {
            // Retrieves the amount of lines within the selected text
            QTextCursor cursor = textCursor();
            QString selected = cursor.selectedText();
            qint32 amountOfLines = selected.count(par) + 1;

            // Does not do anything if only one line is selected
            if (amountOfLines == 1) {
                return;
            }

            // Selects the start of the current line and retrieves the position
            int linePos, lineCopy;
            cursor.setPosition(cursor.selectionStart());
            cursor.movePosition(QTextCursor::StartOfLine);
            linePos = lineCopy = cursor.position();
            cursor.beginEditBlock();

            // Inserts tabs for each selected line
            for (int i = 0; i < amountOfLines; ++i) {
                cursor.setPosition(linePos);
                cursor.insertText(tab);
                cursor.movePosition(QTextCursor::Down);
                cursor.movePosition(QTextCursor::StartOfLine);
                linePos = cursor.position();
            }

            // Selects all the text
            cursor.movePosition(QTextCursor::Down);
            cursor.movePosition(QTextCursor::EndOfLine);
            cursor.setPosition(lineCopy, QTextCursor::KeepAnchor);
            cursor.endEditBlock();
            setTextCursor(cursor);
            return;
        }

        // Removes tabs in front of selected block(s)
        if (event->key() == Qt::Key_Backtab && !textCursor().selectedText().isEmpty()) {
            // Retrieves the amount of lines within the selected text
            QTextCursor cursor = textCursor();
            QString selected = cursor.selectedText();
            qint32 amountOfLines = selected.count(par) + 1;

            // Does not do anything if only one line is selected
            if (amountOfLines == 1) {
                return;
            }

            // Retrieves the start of the selection
            int start = 0, line, diff, copy;
            cursor.setPosition(cursor.selectionStart());
            cursor.movePosition(QTextCursor::StartOfLine);
            copy = cursor.position();

            if (selected.at(0).isSpace()) {
                cursor.movePosition(QTextCursor::NextWord);
                start = cursor.position();
            }

            cursor.clearSelection();
            cursor.beginEditBlock();

            // Removes a tab from each line
            for (int i = 0; i < amountOfLines; ++i) {
                cursor.setPosition(start);
                cursor.movePosition(QTextCursor::StartOfLine);
                line = cursor.position();
                cursor.setPosition(start);

                if (start == line) {
                    continue;   // nothing to remove
                }

                diff = qMin(4, start - line);
                for (int i = 0; i < diff; ++i) {
                    cursor.deletePreviousChar();
                }

                // Finds position of the first word in the next line
                cursor.movePosition(QTextCursor::Down);
                cursor.movePosition(QTextCursor::StartOfLine);
                cursor.movePosition(QTextCursor::NextWord);
                cursor.movePosition(QTextCursor::StartOfWord);
                start = cursor.position();
            }

            // Selects all the text
            cursor.movePosition(QTextCursor::Down);
            cursor.movePosition(QTextCursor::EndOfLine);
            cursor.setPosition(copy, QTextCursor::KeepAnchor);
            cursor.endEditBlock();
            setTextCursor(cursor);
            return;
        }

        // Replaces a tab with four whitespaces
        if (event->key() == Qt::Key_Tab) {
            QTextCursor cursor = textCursor();
            cursor.insertText(QString(" ").repeated(4));
            setTextCursor(cursor);
            return;
        }

*/
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
    painter.setFont(m_font);

    painter.fillRect(event->rect(), cyclesBackgroundColor);


    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    painter.setPen(cyclesColor);
    painter.setFont(m_font);

    painter.drawText(0, top, cycleNumberArea->width(), m_metrics->height(),
                 Qt::AlignLeft, "Cycles");

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
                             Qt::AlignLeft, number);
            }
            if (m_blockCycles.contains(ln))
            {

                int val = 0;
                if (m_cycles.contains(ln))
                    val+=m_cycles[ln];
                QString number = QString::number(m_blockCycles[ln]+val);
                painter.setPen(blockCyclesColor);
                painter.drawText(0, top, cycleNumberArea->width(), m_metrics->height(),
                             Qt::AlignLeft, number);
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
                 Qt::AlignLeft, "");

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
                for (int i=0;i<5-number.count();i++)
                    number.insert(1,"0");
                painter.setPen(cyclesColor);
                painter.drawText(10, top, addressArea->width(), m_metrics->height(),
                             Qt::AlignLeft, number);
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
