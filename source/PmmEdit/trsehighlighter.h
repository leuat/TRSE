#ifndef TRSEHIGHLIGHTER_H
#define TRSEHIGHLIGHTER_H
#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QRegularExpression>
#include <QString>
#include "source/LeLib/util/cinifile.h"
#include "source/Compiler/symboltable.h"


class TRSEHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    TRSEHighlighter(CIniFile ini, int type, QTextDocument *parent = 0);

    CIniFile m_colors;
    void Save(QString fn);

    virtual void HighlightText(QString &text);


    bool m_isUpdated = false;
    int m_cur = -1;
    void AppendSymboltable(QList<QString> procs);
    void ApplyCustomKeywordList();

    QSharedPointer<SymbolTable> m_symTab = nullptr;


    QStringList m_customList;
    QColor m_customColour;
    bool m_customBold = false;
    bool m_customItalic = false;


protected:
    void highlightBlock(const QString &text) override;

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
    QTextCharFormat constantAddressFormat;
    QTextCharFormat procedureFormat;

};

#endif // TRSEHIGHLIGHTER_H
