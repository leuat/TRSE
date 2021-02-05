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

#ifndef LEXER_H
#define LEXER_H

#include <QString>
#include "syntax.h"
#include "token.h"
#include "data_pmm.h"

#include "errorhandler.h"

class FilePart {
public:
    QString m_name;
    int m_startLine, m_endLine, m_count, m_startLineAcc, m_endLineAcc;
    FilePart() {}
    FilePart(QString name, int s, int e,int sa, int ea, int cnt) {
        m_startLine = s;
        m_endLine = e;
        m_startLineAcc = sa;
        m_endLineAcc = ea;
        m_name = name;
        m_count = cnt;
    }
};


class Lexer
{

public:
    QString m_currentChar;
    uint m_pos =0, m_prevPos = 0;
    bool m_finished = false;
    QString m_orgText = "";
    QString m_text = "";
    uint m_localPos = 0;
    QStringList m_lines;
    QString m_path;
    QVector<FilePart> m_includeFiles;
    bool m_ignorePreprocessor, m_nextIsReference = false;
    bool m_isCurrentlyInABlockComment = false;
    int getPositionInPercent();
    int getTotalNumberOfLines();

    QString m_currentComment="";


    int getLineNumber(QString find);

    void FindLineNumberAndFile(int inLe, QString& file, int& outle);

    QVector<uint> m_stack;


    void PushState();
    void PopState();

    Lexer();
    Lexer(QString text, QStringList lines, QString path);

    void Error(QString text);
    void Advance();
    void SkipWhiteSpace();
    void SkipComment();
    void SkipUntilNewLine();
    QString loadTextFile(QString filename);
    Token Number(bool& isOk);
    Token _Id();
    Token Preprocessor();
    Token String();
    QString peek();
    void Initialize();

    Token GetNextToken();

};

#endif // LEXER_H
