#include "asmhighlighter.h"

#include "source/Compiler/token.h"
#include "source/Compiler/symboltable.h"
#include "source/Compiler/syntax.h"

#include <QDebug>


AsmHighlighter::AsmHighlighter(CIniFile ini, int type, QTextDocument *parent)
    : TRSEHighlighter(ini,type,parent)
{
    HighlightingRule rule;
    keywordFormat.setForeground(m_colors.getColor("keywordcolor"));
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    QString filename = ":/resources/text/opcodes.txt";
    QStringList list = Util::loadTextFile(filename).split("\n");
    QStringList operands;
    for (QString s: list) {

        s = s.simplified();
        if (s.startsWith("#")) continue;
        if (s=="") continue;
        operands << s.split(",").first().simplified();
    }

    for (QString op: operands) {
        //qDebug() << QString::number(i) << TokenType::types[i].toLower();
        QString s = "\\b" + op + "\\b";
        keywordPatterns<<s;
    }

    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegularExpression(pattern,QRegularExpression::CaseInsensitiveOption);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }
/*
    if (type==0)
    {
        builtinFunctionFormat.setForeground(m_colors.getColor("builtinfunctioncolor"));
        builtinFunctionFormat.setFontWeight(QFont::Bold);
        keywordPatterns.clear();
        for (QString k: Syntax::s.builtinFunctionsFjong.keys()) {
            QString name = k;
            QString s = "\\b" + name.toLower() + "\\b";
            keywordPatterns<<s;
        }

        foreach (const QString &pattern, keywordPatterns) {
            rule.pattern = QRegularExpression(pattern,QRegularExpression::CaseInsensitiveOption);
            rule.format = builtinFunctionFormat;
            highlightingRules.append(rule);
        }


        //SymbolTable::Initialize();

        constantsFormat.setForeground(m_colors.getColor("constantscolor"));
        constantsFormat.setFontWeight(QFont::Normal);
        keywordPatterns.clear();

    }
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegularExpression(pattern,QRegularExpression::CaseInsensitiveOption);
        rule.format = constantsFormat;
        highlightingRules.append(rule);
    }

*/
    numberFormat.setFontWeight(QFont::Normal);
    numberFormat.setForeground(m_colors.getColor("numberscolor"));
//    rule.pattern = QRegularExpression("#[0-9a-f#\\\$%]+",QRegularExpression::CaseInsensitiveOption);
//    rule.pattern = QRegularExpression("(\\b\\$[0-9a-f]+)||(\\b[0-9]+)",QRegularExpression::CaseInsensitiveOption);
//    rule.pattern = QRegularExpression("(?!(\\\^))(\\\$)?\\b[0-9a-f]+\\b",QRegularExpression::CaseInsensitiveOption);
    rule.pattern = QRegularExpression(R"!(((\$)\b([0-9a-f]+)\b)|(\b([0-9]+)\b))!",QRegularExpression::CaseInsensitiveOption);
    rule.format = numberFormat;
    highlightingRules.append(rule);


    addressFormat.setFontWeight(QFont::Normal);
    addressFormat.setForeground(m_colors.getColor("addresscolor"));
//    rule.pattern = QRegularExpression("(?!#)(\\\$[0-9a-f]+)|\\b[0-9]+\\b",QRegularExpression::CaseInsensitiveOption);
//    rule.pattern = QRegularExpression("(\\b\\$[0-9a-f]+\\b)|(\\b[0-9]+\\b)",QRegularExpression::CaseInsensitiveOption);
    rule.pattern = QRegularExpression(R"!(\^(\$)?\b([0-9a-f]+)\b)!",QRegularExpression::CaseInsensitiveOption);
    rule.format = addressFormat;
    highlightingRules.append(rule);

    builtinFunctionFormat.setForeground(m_colors.getColor("builtinfunctioncolor"));
//    rule.pattern = QRegularExpression("--[^\n]*");
    rule.pattern = QRegularExpression("^[^\\t][^\n]*");
    rule.format = builtinFunctionFormat;
    highlightingRules.append(rule);


    symbolsFormat.setFontWeight(QFont::Normal);
    symbolsFormat.setForeground(m_colors.getColor("symbolscolor"));
    rule.pattern = QRegularExpression(R"!([\+\-:=\/\*\(\)\<\>\[\]])!",QRegularExpression::CaseInsensitiveOption);
    rule.format = symbolsFormat;
    highlightingRules.append(rule);



    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::green);
    rule.pattern = QRegularExpression("\\bQ[A-Za-z]+\\b",QRegularExpression::CaseInsensitiveOption);
    rule.format = classFormat;
    highlightingRules.append(rule);




    quotationFormat.setForeground(m_colors.getColor("quotationcolor"));
    rule.pattern = QRegularExpression("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);


    singleLineCommentFormat.setForeground(m_colors.getColor("commentcolor"));
//    rule.pattern = QRegularExpression("--[^\n]*");
    rule.pattern = QRegularExpression("^\\s*;[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);


    multiLineCommentFormat.setForeground(m_colors.getColor("commentcolor"));

    commentStartExpression = QRegularExpression("--\\[\\[");
//    commentEndExpression = QRegularExpression("\\*/");
    commentEndExpression = QRegularExpression("\\]\\]--");




    /*
    builtinFunctionFormat.setForeground(m_colors.getColor("builtinfunctioncolor"));
    builtinFunctionFormat.setFontWeight(QFont::Bold);
    keywordPatterns.clear();
    for (QString k: Syntax::s.builtinFunctionsFjong.keys()) {
        QString name = k;
        QString s = "\\b" + name.toLower() + "\\b";
        keywordPatterns<<s;
    }

    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegularExpression(pattern,QRegularExpression::CaseInsensitiveOption);
        rule.format = builtinFunctionFormat;
        highlightingRules.append(rule);
    }
    */

}
