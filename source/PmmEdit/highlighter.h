#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QRegularExpression>
#include <QString>
#include "source/LeLib/util/cinifile.h"



class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(CIniFile ini, QTextDocument *parent = 0);
    CIniFile m_colors;
    void Save(QString fn);

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

#endif // HIGHLIGHTER_H
