#include "trsehighlighter.h"


TRSEHighlighter::TRSEHighlighter(CIniFile ini, int type, QTextDocument *parent) : QSyntaxHighlighter(parent)
{
    m_colors = ini;

}



void TRSEHighlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);


    while (startIndex >= 0) {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                    + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}


void TRSEHighlighter::Save(QString fn)
{
    m_colors.setVec("keywordcolor", QVector3D(128,200,255));
    m_colors.setVec("builtinfunctioncolor",QVector3D(255,200,50));
    m_colors.setVec("constantscolor", QVector3D(50,200,100));
    m_colors.setVec("addresscolor", QVector3D(255,150,170));
    m_colors.setVec("numberscolor", QVector3D(180,140,255));
    m_colors.setVec("symbolscolor", QVector3D(120,140,160));
    m_colors.setVec("quotationcolor", QVector3D(255,128,40));
    m_colors.setVec("commentcolor", QVector3D(128,128,128));
    m_colors.Save(fn);
}

void TRSEHighlighter::HighlightText(QString &text)
{
    QString otext = text;
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
//            qDebug() << match.captured();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
            //setFormat(match.capturedStart(), match.capturedLength(), QColor(255,0,0,1));
 //             qDebug() << match.captured()
/*            if (match.capturedLength()>5) {
                text.insert(match.capturedStart(),"<font color=\"#FF0000\">");
                text.insert(match.capturedEnd(),"</font>");
            }*/
        }

    }
//    text = otext;
}


void TRSEHighlighter::AppendSymboltable(QList<QString> procs)
{

    if (m_isUpdated)
        while (highlightingRules.count()>m_cur)
            highlightingRules.removeLast();
    else
    {
    // Procedures
        m_isUpdated = true;
        m_cur = highlightingRules.count();
    }
    QStringList keywordPatterns;
    if (m_colors.contains("functioncolor"))
        procedureFormat.setForeground(m_colors.getColor("functioncolor"));
    else
        procedureFormat.setForeground(m_colors.getColor("builtinfunctioncolor"));
//    procedureFormat.setFontWeight(QFont::Bold);
    keywordPatterns.clear();

    for (QString k: procs) {
        //qDebug() << QString::number(i) << TokenType::types[i].toLower();
        QString name = k;
        QString s = "\\b" + name.toLower() + "\\b";
        keywordPatterns<<s;
        name = name.replace("_","::");
        s = "\\b" + name.toLower() + "\\b";
        keywordPatterns<<s;

    }
    HighlightingRule rule;

    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegularExpression(pattern,QRegularExpression::CaseInsensitiveOption);
        rule.format = procedureFormat;
        highlightingRules.append(rule);
    }

}

void TRSEHighlighter::ApplyCustomKeywordList()
{
    HighlightingRule rule;
    QStringList keywordPatterns;
    if (m_customBold)
        keywordFormat.setFontWeight(QFont::Bold);
    else
        keywordFormat.setFontWeight(QFont::Thin);


    keywordFormat.setFontItalic(m_customItalic);
    keywordFormat.setForeground(m_customColour);

    for (QString t:  m_customList) {

        QString s = "\\b" + t.toLower() + "\\b";
         keywordPatterns<<s;
    }


    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegularExpression(pattern,QRegularExpression::CaseInsensitiveOption);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

}
