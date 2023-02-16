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

#include "lexer.h"
#include <QDebug>

#include <iostream>
#include <QFile>


int Lexer::getPositionInPercent()
{
    if (m_text.length()==0)
        return 100;

    int val = (100*m_pos)/m_text.length();
//    int val = (100*Pmm::Data::d.lineNumber)/m_lines.length();
    return val;
}

int Lexer::getTotalNumberOfLines()
{
    QStringList lst = m_text.split("\n");
    lst.removeAll("");
    return lst.count() + m_currentLineCount;
}

int Lexer::getLineNumber(QString find)
{
    QStringList  l= m_text.split("\n");
    for (int i=0;i<l.count();i++)
        if (l[i].contains(find))
            return i;//-m_shiftLines;

    return -1;
}

void Lexer::FindLineNumberAndFile(int inLe, QString &file, int &outle)
{
    file="";
    outle = inLe;
    if (m_includeFiles.count()==0) {
        return;
    }

    int cur = inLe;

    //    qDebug() << "input line number: " << inLe;
    //  qDebug() << "Start line: " << m_parser.m_lexer->m_includeFiles[0].m_startLine;
    if (m_includeFiles.count()>0) {
        if (cur<=m_includeFiles[0].m_startLineAcc) {
            return;
        }
    }


    for (FilePart fp: m_includeFiles) {
        if (inLe >= fp.m_startLineAcc && inLe<fp.m_endLineAcc) {
            file = fp.m_name;
            outle = inLe - fp.m_startLineAcc;
            return;
        }
        //        qDebug() << "Include file size : " << (fp.m_endLine-fp.m_startLine);
        cur=cur - (fp.m_endLineAcc-fp.m_startLineAcc)+1;
    }
    outle = cur;
}

Lexer::Lexer()
{

}

Lexer::Lexer(QString text, QStringList lines, QString path) {
    m_orgText = text;
    m_pos = 0;
    m_path = path;
    if (m_text.length()>0)
        m_currentChar = m_text[m_pos];
    m_lines = lines;
    m_ignorePreprocessor = true;
    Pmm::Data::d.Init();
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

void Lexer::PushState()
{
    m_stack.append(m_prevPos);
    m_stack.append(m_pos);
    m_stack.append(m_localPos);
    m_stack.append(Pmm::Data::d.lineNumber);
}

void Lexer::PopState()
{
    if (m_stack.count()==0)
        return;
    Pmm::Data::d.lineNumber= m_stack.last();m_stack.removeLast();
    m_localPos = m_stack.last();m_stack.removeLast();
    m_pos = m_stack.last();m_stack.removeLast();
    m_prevPos = m_stack.last();m_stack.removeLast();
    m_currentChar = m_text[m_pos];
}


void Lexer::Advance()
{
    m_prevPos = m_pos;
    m_pos++;
    m_localPos++;
    if (m_pos>m_text.length()-1) {
        m_finished = true;
    }
    else {
        m_currentChar = m_text[m_pos];
        if (m_currentChar=="\n") {
            Pmm::Data::d.lineNumber ++;
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
    while (!(m_currentChar== "*" && peek()=="/") && !m_finished) {
        m_currentComment+=m_currentChar;
        Advance();
    }

    Advance();
    Advance();
    m_isCurrentlyInABlockComment = false;

}

void Lexer::SkipUntilNewLine()
{
    while (!(m_currentChar== "\n") && !m_finished) {
        m_currentComment+=m_currentChar;

        Advance();
    }

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

Token Lexer::Number(bool& isOk)
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
//        qDebug() << "LEXER " << res<<res.toFloat();

        return Token(TokenType::REAL_CONST, res.toFloat());
    }
//    qDebug() <<"LEXER NUMBER " <<res;
    org = res;
    bool ok;
    long val = 0;
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
    val = res.toLong(&isOk, base);

//    qDebug() << val;
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
        // Replace "::" with "_"
        if (m_currentChar==":" && peek()==":") {
            Advance();
            m_currentChar = "_";
        }
/*        if (m_currentChar=="Æ" || m_currentChar=="?" || m_currentChar =="•") {
//            Advance();
            m_currentChar = "_";
        }
        */

    }
    bool isRef =  m_nextIsReference;
    m_nextIsReference = false;
//    if (isRef && Syntax::s.isDigit(QString(result[0])))
  //      ErrorHandler::e.Error("Constant numbers cannot be referenced by #",Pmm::Data::d.lineNumber);
//    qDebug() << result << isRef;
    return Syntax::s.GetID(result,isRef);

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
//        qDebug() <<m_currentChar << result;
        result +=m_currentChar;
        Advance();
        if (m_currentChar=="\\" && peek()=="\"") {
            Advance();
            Advance();
            result[result.length()-1]='\"';
        }
    }
    Advance();
//    ErrorHandler::e.DebugLow("Calling Lexer::String with string: " + result);

    return Token(TokenType::STRING, result);

}

Token Lexer::InlineAsm()
{
    QString result="";
    bool finished = false;
    while (!finished) {
        result +=m_currentChar;
        QString next3  =peek(1) + peek(2) + peek(3)+peek(4);
        if (next3.toLower()=="end;")
            finished = true;
        Advance();
    }
    return Token(TokenType::STRING, result);

}

Token Lexer::Macro()
{
    QString result="";
    bool finished = false;
    while (!finished) {
        result +=m_currentChar;
        QString next3  =peek(1) + peek(2) + peek(3)+peek(4)+peek(5)+peek(6)+peek(7)+peek(8);
//        qDebug() <<next3;
        if (next3=="")
            finished = true;
        if (next3.toLower().trimmed()=="endmacro")
            finished = true;

        Advance();
    }
    return Token(TokenType::STRING, result);

}


QString Lexer::peek()
{
    int k=m_pos+1;
    if (k>=m_text.length())
        return "";
    return QString(m_text[k]);
}

QString Lexer::peek(int n)
{
    int k=m_pos+n;
    if (k>=m_text.length())
        return "";
    return QString(m_text[k]);
}

void Lexer::Initialize()
{
    Pmm::Data::d.lineNumber = 0;
    m_finished = false;
    if (m_text.length()>0)
        m_currentChar = m_text[0];
    m_pos = 0;
    m_localPos = 0;
}
/*
 *
 * GetNextToken is the main tokenizen that transforms the stream
 * of characters into streams of tokens.
*/

Token Lexer::GetNextToken()
{
    while (!m_finished) {
        // Ignore whitespace
        if (m_currentChar==" " || m_currentChar=="\n" || m_currentChar=="\t") {
            SkipWhiteSpace();
            continue;
        }
        // Ignore tab
        if (m_currentChar == "\t" ) {
            Advance();
            continue;
        }

        // Ignore comments
        if (m_currentChar=="/") {
            if (peek()=="*") {
                m_isCurrentlyInABlockComment = true;
                Advance();
                Advance();
                SkipComment();
                continue;
            }

        }

        // end comments
        if (m_currentChar=="/") {
            if (peek()=="/") {
                SkipUntilNewLine();
                continue;
            }

        }
        // Convert ascii to number
        if (m_currentChar=="'") {
            Advance();
            int c = m_currentChar.toLatin1()[0];
//            qDebug() << c;
            Advance();
            if (m_currentChar!="'")
                ErrorHandler::e.Error( "Chars chan only contain one symbol",Pmm::Data::d.lineNumber );

            Advance();

            return Token(TokenType::INTEGER_CONST, c);


        }

        if (m_currentChar=="\"") {
            Advance();
            return String();
        }

        // Generate preprocessor
        if (m_currentChar=="@") {
            Advance();
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
            return Token(TokenType::COMMA, ",");
        }

        if (Syntax::s.isDigit(m_currentChar)) {
            QString keep = m_currentChar;
            uint pos = m_pos;
            uint ppos = m_prevPos;
            bool ok=true;
            Token number = Number(ok);

            if (ok)
                return number;
            else {
                m_currentChar = keep;
                m_pos = pos;
                m_prevPos = ppos;
            }
        }

        if (Syntax::s.isAlpha(m_currentChar) || m_currentChar=="$" || (m_currentChar =="^" && Syntax::s.isAlpha(peek()))) {
            Token id =_Id();

            if (id.m_value.endsWith("^")) {
                // Replaces p^ with p[0]
                id.m_value.remove(id.m_value.length()-1,1);
                m_text.remove(m_pos-1,1);
                m_text.insert(m_pos-1,"[0]"); m_currentChar = "[";

            }
            //qDebug() << id.m_value;
            return id;
        }
        // Assign statement a:=b;
        if (m_currentChar==":" && peek()=="=") {
            Advance();
            Advance();
            return Token(TokenType::ASSIGN,":=");
        }

        if (m_currentChar==":") {
            Advance();
            return Token(TokenType::COLON, ":");
        }
        // Assign next to be a reference
        if (m_currentChar=="#") {
            Advance();
            m_nextIsReference = true;
            return _Id();
        }

        if (m_currentChar=="=") {
            Advance();
            return Token(TokenType::EQUALS, "=");
        }
        if (m_currentChar==">") {
            if (peek()=="=") {
                Advance();
                Advance();
                return Token(TokenType::GREATEREQUAL, ">=");
            }
            if (peek()==">") {
                if (peek(2)=="=") {
                    Advance();
                    Advance();
                    Advance();
                    return Token(TokenType::ASSIGNOP, ">>");

                }

                Advance();
                Advance();
                return Token(TokenType::SHR, ">>");
            }
            Advance();
            return Token(TokenType::GREATER, ">");
        }
        if (m_currentChar=="<") {
            QString c = m_currentChar;
            if (peek()=="=") {
                Advance();
                Advance();
                return Token(TokenType::LESSEQUAL, "<=");
            }
            if (peek()=="<") {
                if (peek(2)=="=") {
                    Advance();
                    Advance();
                    Advance();
                    return Token(TokenType::ASSIGNOP, "<<");

                }

                Advance();
                Advance();
                return Token(TokenType::SHL, "<<");
            }

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
            QString c = m_currentChar;
            if (peek()=="=") {
                Advance();
                Advance();
                return Token(TokenType::ASSIGNOP, c);

            }
            Advance();
            return Token(TokenType::PLUS, "+");
        }
        if (m_currentChar=="-") {
            QString c = m_currentChar;
            if (peek()=="=") {
                Advance();
                Advance();
                return Token(TokenType::ASSIGNOP, c);

            }
            Advance();
            return Token(TokenType::MINUS, "-");
        }
        if (m_currentChar=="&") {
            QString c = m_currentChar;
            if (peek()=="=") {
                Advance();
                Advance();
                return Token(TokenType::ASSIGNOP, c);

            }
            Advance();
            return Token(TokenType::BITAND, "&");
        }
        if (m_currentChar=="|") {
            QString c = m_currentChar;
            if (peek()=="=") {
                Advance();
                Advance();
                return Token(TokenType::ASSIGNOP, c);

            }
            Advance();
            return Token(TokenType::BITOR, "|");
        }
        if (m_currentChar=="^") {
            QString c = m_currentChar;
            if (peek()=="=") {
                Advance();
                Advance();
                return Token(TokenType::ASSIGNOP, c);

            }
            Advance();
            return Token(TokenType::XOR, "^");
        }
        if (m_currentChar=="~") {
            QString c = m_currentChar;
            if (peek()=="=") {
                Advance();
                Advance();
                return Token(TokenType::ASSIGNOP, c);

            }
            Advance();
            return Token(TokenType::BITNOT, "~");
        }

        if (m_currentChar=="*") {
            QString c = m_currentChar;
            if (peek()=="=") {
                Advance();
                Advance();
                return Token(TokenType::ASSIGNOP, c);

            }
            Advance();
            return Token(TokenType::MUL, "*");
        }
        if (m_currentChar=="/") {
            QString c = m_currentChar;
            if (peek()=="=") {
                Advance();
                Advance();
                return Token(TokenType::ASSIGNOP, c);

            }
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
//            Advance();
            //m_text = m_text.replace("\n", "");
            return Token(TokenType::DOT, ".");
        }
        // Picked up a stray ^
        if (m_currentChar=="^") {
            Advance();
            return Token(TokenType::NADA, "NADA");
        }

        ErrorHandler::e.Error( "Error parsing: " + m_currentChar,Pmm::Data::d.lineNumber );



    }
    return Token(TokenType::TEOF, "EOF");
}
