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
    int m_startLine, m_endLine, m_count;
    FilePart() {}
    FilePart(QString name, int s, int e, int cnt) {
        m_startLine = s;
        m_endLine = e;
        m_name = name;
        m_count = cnt;
    }
};


class Lexer
{

public:
    QString m_currentChar;
    uint m_pos = 0;
    bool m_finished = false;
    QString m_orgText = "";
    QString m_text = "";
    uint m_localPos = 0;
    QStringList m_lines;
    QString m_path;
    QVector<FilePart> m_includeFiles;
    bool m_ignorePreprocessor;



    int getLineNumber(QString find);

    Lexer();
    Lexer(QString text, QStringList lines, QString path) {
        m_orgText = text;
        m_pos = 0;
        m_path = path;
        m_currentChar = m_text[m_pos];
        m_lines = lines;
        m_ignorePreprocessor = true;
        Pmm::Data::d.Init();
    }

    void Error(QString text);
    void Advance();
    void SkipWhiteSpace();
    void SkipComment();
    void SkipUntilNewLine();
    QString loadTextFile(QString filename);
    Token Number();
    Token _Id();
    Token Preprocessor();
    Token String();
    QString peek();
    void Initialize();

    Token GetNextToken();

};

#endif // LEXER_H
