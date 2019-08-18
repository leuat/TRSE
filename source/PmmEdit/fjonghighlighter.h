#ifndef FJONGHIGHLIGHTER_H
#define FJONGHIGHLIGHTER_H


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


#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QRegularExpression>
#include <QString>
#include "source/LeLib/util/cinifile.h"



class FjongHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    FjongHighlighter(CIniFile ini, int type, QTextDocument *parent = 0);
    CIniFile m_colors;
    void Save(QString fn);

    void HighlightText(QString &text);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;



    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat numberFormat;
    QTextCharFormat addressFormat;
    QTextCharFormat symbolsFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
    QTextCharFormat builtinFunctionFormat;
    QTextCharFormat constantsFormat;
};


#endif // FJONGHIGHLIGHTER_H
