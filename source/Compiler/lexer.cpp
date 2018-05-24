#include "lexer.h"
#include <QDebug>

#include <iostream>
#include <QFile>

using namespace std;

int Lexer::getLineNumber(QString find)
{
    QStringList  l= m_text.split("\n");
    for (int i=0;i<l.count();i++)
        if (l[i].contains(find))
            return i;

    return -1;
}

Lexer::Lexer()
{

}


/*void Lexer::IncludeFiles()
{
    QRegExp rx("\\{\\s*\\$include.*\\}", Qt::CaseInsensitive);
    QStringList list;
    int pos = 0;
    QString newText = m_orgText;
    m_text = m_orgText;
   // return;
    while ((pos = rx.indexIn(m_orgText, pos)) != -1) {
        QString filename= rx.cap(0).toLower().remove("{").remove("}").remove("$include").trimmed();
        QString include = loadTextFile(m_path +"/" +filename);
        int ln = getLineNumber(rx.cap(0));
        m_includeFiles.append(FilePart(m_path +"/" +filename,ln, ln+ include.split("\n").count()));
        newText.replace(rx.cap(0), include);
        pos += rx.matchedLength();
    }
    m_text = newText;
}*/

void Lexer::Advance()
{
    m_pos++;
    m_localPos++;
    if (m_pos>m_text.length()-1) {
        m_finished = true;
    }
    else {

        m_currentChar = m_text[m_pos];
        //if (Pmm::Data::d.lineNumber<m_lines.count())
        if (m_currentChar=="\n") {
            //qDebug() << "Increase linenumber to " << Pmm::Data::d.lineNumber;

          /*  if (!m_ignorePreprocessor)
            for (FilePart& fp: m_includeFiles)
                if (fp.m_startLine==Pmm::Data::d.lineNumber) {
                    qDebug() << "Found " << fp.m_name << ", skipping " << fp.m_count;
                    Pmm::Data::d.lineNumber+=fp.m_count;
                }
*/
            Pmm::Data::d.lineNumber ++;

            //if (Pmm::Data::d.lineNumber<m_lines.count())
            //Pmm::Data::d.currentLineText = m_lines[Pmm::Data::d.lineNumber];
            m_localPos = 0;




        }


    }

}

void Lexer::SkipWhiteSpace()
{
    while (!m_finished && (m_currentChar==" "|| m_currentChar=="\n" || m_currentChar=="\t"))
        Advance();
}

void Lexer::SkipComment()
{
    while (!(m_currentChar== "*" && peek()=="/") && !m_finished)

        Advance();

    Advance();
    Advance();

}

void Lexer::SkipUntilNewLine()
{
    while (!(m_currentChar== "\n") && !m_finished)

        Advance();

}

QString Lexer::loadTextFile(QString filename)
{
    QFile file(filename);

    if(!file.open( QIODevice::ReadOnly|QIODevice::Text ) ) {
        ErrorHandler::e.Error("Could not open file for inclusion: " + filename);
    }
    QTextStream in(&file);
    QString text ="";
    while(!in.atEnd()) {
        text+= in.readLine() + "\n";

    }
//    text = text.remove("\t");
//    qDebug() << text;
    return text;
}

Token Lexer::Number()
{
    QString res="";
    QString org;
    while (!m_finished && Syntax::s.digitAll.contains(m_currentChar)) {
        res+=m_currentChar;
        Advance();
    }
    if (m_currentChar==".") {
        res+=m_currentChar;
        Advance();
        while (!m_finished && Syntax::s.digitAll.contains(m_currentChar)) {
            res+=m_currentChar;
            Advance();
        }


        return Token(TokenType::REAL_CONST, res.toFloat());
    }
    org = res;
    bool ok;
    float val = 0;
    // Memory address
    bool isConstant = true;
    int base = 10;

    if (res.contains("%")) {
        res.remove("%");
        base=2;
    }

    if (res.contains("$")) {
        res.remove("$");
        base=16;
    }
    if (res.contains("^")) {
        res.remove("^");
        isConstant = false;
    }
    val = res.toInt(&ok, base);


    if (isConstant)
        return Token(TokenType::INTEGER_CONST, val);
    else {
        Token t = Token(TokenType::ADDRESS, val);
        t.m_value = res;
        return t;
    }

}

Token Lexer::_Id()
{
    QString result="";
    //qDebug() << "ID: " << m_currentChar;
    while (!m_finished && Syntax::s.isAlnum(m_currentChar)) {
        result +=m_currentChar;
        Advance();
    }
    return Syntax::s.GetID(result);

}

Token Lexer::Preprocessor()
{
    QString result="";
    while (!m_finished && Syntax::s.isAlnum(m_currentChar)) {
        result +=m_currentChar;
        Advance();

    }
    return Token(TokenType::PREPROCESSOR, result.toLower());

}

Token Lexer::String()
{
    QString result="";
    while (!m_finished && m_currentChar!="\"") {
        result +=m_currentChar;
        Advance();
    }
    Advance();
    ErrorHandler::e.DebugLow("Calling Lexer::String with string: " + result);

    return Token(TokenType::STRING, result);

}

QString Lexer::peek()
{
    if (m_pos+1>=m_text.length())
        return "";
    return QString(m_text[m_pos+1]);
}

void Lexer::Initialize()
{
    Pmm::Data::d.lineNumber = 0;
    m_finished = false;
    m_currentChar = m_text[0];
    m_pos = 0;
    m_localPos = 0;
}

Token Lexer::GetNextToken()
{
    while (!m_finished) {
        if (m_currentChar==" " || m_currentChar=="\n" || m_currentChar=="\t") {
            SkipWhiteSpace();
            continue;
        }
        if (m_currentChar == "\t" ) {
            Advance();
            continue;
        }

        if (m_currentChar=="/") {
            if (peek()=="*") {
                Advance();
                Advance();
                SkipComment();
                continue;
            }

        }

        if (m_currentChar=="/") {
            if (peek()=="/") {
                SkipUntilNewLine();
                continue;
            }

        }
        if (m_currentChar=="\"") {
            Advance();
            return String();
        }

        if (m_currentChar=="@") {
            Advance();
/*            if (m_ignorePreprocessor) {
                SkipUntilNewLine();
                continue;
            }
            else*/
                return Preprocessor();
        }

        if (m_currentChar=="[") {
            Advance();
            return Token(TokenType::LBRACKET,"[");
        }
        if (m_currentChar=="]") {
            Advance();
            return Token(TokenType::RBRACKET,"]");
        }

        if (m_currentChar==",") {
            Advance();
            return Token(TokenType::COMMA, ":");
        }

        if (Syntax::s.isDigit(m_currentChar)) {
            return Number();
        }

        if (Syntax::s.isAlpha(m_currentChar) || m_currentChar=="$") {

            //qDebug() << m_currentChar << " is Alpha";

            return _Id();
        }


        if (m_currentChar==":" && peek()=="=") {
            Advance();
            Advance();
            return Token(TokenType::ASSIGN,":=");
        }
        if (m_currentChar==":") {
            Advance();
            return Token(TokenType::COLON, ":");
        }

        if (m_currentChar=="=") {
            Advance();
            return Token(TokenType::EQUALS, "=");
        }
        if (m_currentChar==">") {
            Advance();
            return Token(TokenType::GREATER, ">");
        }
        if (m_currentChar=="<") {
            Advance();
            if (m_currentChar==">") {
                Advance();
                return Token(TokenType::NOTEQUALS, "<>");
            }
            return Token(TokenType::LESS, "<");
        }

        if (m_currentChar==";") {
            Advance();
            return Token(TokenType::SEMI,";");
        }

        if (m_currentChar=="+") {
            Advance();
            return Token(TokenType::PLUS, "+");
        }
        if (m_currentChar=="-") {
            Advance();
            return Token(TokenType::MINUS, "-");
        }
        if (m_currentChar=="&") {
            Advance();
            return Token(TokenType::BITAND, "&");
        }
        if (m_currentChar=="|") {
            Advance();
            return Token(TokenType::BITOR, "|");
        }

        if (m_currentChar=="*") {
            Advance();
            return Token(TokenType::MUL, "*");
        }
        if (m_currentChar=="/") {
            Advance();
            return Token(TokenType::DIV, "/");
        }
        if (m_currentChar=="(") {
            Advance();
            return Token(TokenType::LPAREN, "(");
        }
        if (m_currentChar==")") {
            Advance();
            return Token(TokenType::RPAREN, ")");
        }
        if (m_currentChar==".") {
            Advance();
            Advance();
            //m_text = m_text.replace("\n", "");
            return Token(TokenType::DOT, ".");
        }
        ErrorHandler::e.Error( "Error parsing: " + m_currentChar );



    }
    return Token(TokenType::TEOF, "EOF");
}
