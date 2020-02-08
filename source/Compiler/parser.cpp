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

#include "parser.h"
#include "source/Compiler/data_pmm.h"

Parser::Parser()
{

}

void Parser::Delete()
{
    for (QString val : m_procedures.keys()) {
        Node* s = m_procedures[val];
           // if (s!=nullptr) {
            //s->Delete();
//            delete s;
//        }
    }
    m_procedures.clear();

}

void Parser::InitObsolete()
{
    m_obsoleteWarnings.clear();
    m_obsoleteWarnings.append(QStringList() << "rand"<<"Funtion 'Rand()' is scheduled to be deprecated from 0.09. Please use 'Random()' instead. ");
    m_obsoleteWarnings.append(QStringList() << "inczp"<<"Funtion 'IncZP()' is scheduled to be deprecated from 0.09. Please use 'zp:=zp+value;' to increase pointers instead. ");
    m_obsoleteWarnings.append(QStringList() << "writeln"<<"Funtion 'Writeln()' is scheduled to be deprecated from 0.09. Please use 'PrintString()' instead. ");
//    m_obsoleteWarnings.append(QStringList() << "copycharsetfromrom"<<"Funtion 'CopyCharsetFromROM()' is scheduled to be deprecated from 0.09. ");
}

void Parser::Eat(TokenType::Type t)
{
 //   qDebug() << m_currentToken.m_value << m_currentToken.m_intVal;
    if (m_currentToken.m_type == t) {
        m_currentToken = m_lexer->GetNextToken();
//        if (m_pass==1)
  //          qDebug() << "Token : " <<m_currentToken.m_value <<(m_currentToken.m_type==TokenType::PREPROCESSOR) << m_pass;
        int cnt =0;
        while (m_currentToken.m_type==TokenType::PREPROCESSOR && m_pass==1 && cnt++<32 ) {
            HandlePreprocessorInParsing();
          //  qDebug() << "Inside handle: " << m_currentToken.m_value;

//            qDebug() <<cnt++ <<m_currentToken.m_value;
        }

    }
    else {
        QString warning = "\nDid you forget a semicolon (;) ?";
        ErrorHandler::e.Error("Expected '" + TokenType::getType(t) + "' but found '" +m_currentToken.m_value+"'" + warning,m_currentToken.m_lineNumber);
    }
/*    if (m_currentToken.m_type==TokenType::TEOF) {
        qDebug() << m_currentToken.getType();
        ErrorHandler::e.Error("Syntax errror", m_currentToken.m_lineNumber);
    }
*/
}

void Parser::Eat()
{
    Eat(m_currentToken.m_type);
}


int Parser::findSymbolLineNumber(QString symbol)
{
    int i=1;
    for (QString& s: m_lexer->m_lines) {
        i++;
        if (s.contains(symbol))
            return i;
    }
    return 1;
}

void Parser::InitBuiltinFunctions()
{
    m_initJumps.clear();
    if (m_preprocessorDefines.contains("BuiltinMethodsLocation")) {

        ParserBlock pb;
        pb.m_blockID = m_parserBlocks.count();
        pb.pos = m_preprocessorDefines["BuiltinMethodsLocation"];
        pb.pos = Util::numToHex(pb.pos.toInt());
        m_parserBlocks.append(pb);
        Node::m_staticBlockInfo.m_blockID = pb.m_blockID;
        Node::m_staticBlockInfo.m_blockPos = pb.pos;
        Node::m_staticBlockInfo.m_blockName = "BuiltinMethods";
    }

/*    if (Syntax::s.m_currentSystem->m_system == Syntax::NES)
        InitBuiltinFunction(QStringList()<< "*", "init8x8mulNes");
    else*/
    //m; InitP61Player; Amiga;
    //m; PlayP61Mod; Amiga; a

   if (Syntax::s.m_currentSystem->m_system == AbstractSystem::AMIGA) {

       InitBuiltinFunction(QStringList()<< "playp61module"<<"initp61module" , "initp61playerinternal");
    }

   if (Syntax::s.m_currentSystem->m_system!=AbstractSystem::X86) {
       InitBuiltinFunction(QStringList()<< "SetPixelCGA", "init_cga_scanlines");
   }


    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::C64 ||
            Syntax::s.m_currentSystem->m_system == AbstractSystem::C128 ||
            Syntax::s.m_currentSystem->m_system == AbstractSystem::PLUS4 ||
            Syntax::s.m_currentSystem->m_system == AbstractSystem::NES ||
            Syntax::s.m_currentSystem->m_system == AbstractSystem::PET ||
            Syntax::s.m_currentSystem->m_system == AbstractSystem::OK64 ||
            Syntax::s.m_currentSystem->m_system == AbstractSystem::X16 ||
            Syntax::s.m_currentSystem->m_system == AbstractSystem::VIC20 ||
            Syntax::s.m_currentSystem->m_system == AbstractSystem::BBCM  ) {
/*        InitBuiltinFunction(QStringList()<< "*", "initeightbitmul");

        InitBuiltinFunction(QStringList()<< "*", "init16x8mul");
        InitBuiltinFunction(QStringList()<< "/", "init8x8div");
        InitBuiltinFunction(QStringList()<< "/", "init16x8div");
        */
//          qDebug() << Node::flags.keys();
        if (Node::flags.contains("mul8"))
            InitBuiltinFunction(QStringList()<< "", "initeightbitmul");
        if (Node::flags.contains("mul16"))
            InitBuiltinFunction(QStringList()<< "", "init16x8mul");
        if (Node::flags.contains("div16"))
            InitBuiltinFunction(QStringList()<< "", "init16x8div");
        if (Node::flags.contains("div8"))
            InitBuiltinFunction(QStringList()<< "", "init8x8div");

        if (Syntax::s.m_currentSystem->m_system == AbstractSystem::C64 ||
                Syntax::s.m_currentSystem->m_system == AbstractSystem::C128) {
            InitBuiltinFunction(QStringList()<< "getkey(", "initgetkey");
        }
        InitBuiltinFunction(QStringList()<< "rand(", "initrandom","init_random_call");
        InitBuiltinFunction(QStringList()<< "random(", "initrandom256");

        InitBuiltinFunction(QStringList()<< "rasterirqwedge(" , "init_wedge");
        InitBuiltinFunction(QStringList()<< "playvic20sid(" , "init_vic20_sidplay");
        InitBuiltinFunction(QStringList()<< "viairq(" , "init_viairq");
        InitBuiltinFunction(QStringList()<< "viarasterirq(" , "init_viairq");
        InitBuiltinFunction(QStringList()<< "initmodplayer(" , "include_modplayer");
        InitBuiltinFunction(QStringList()<< "decrunch("<<"decrunchfromindex(", "init_decrunch");

        if (Syntax::s.m_currentSystem->m_system!=AbstractSystem::NES)
            InitBuiltinFunction(QStringList()<< "sine[", "initsinetable", "initsine_calculate");

        InitBuiltinFunction(QStringList()<< "log2_table[" << "atan2(", "initlog2");

        InitBuiltinFunction(QStringList()<< "atan2(", "initatan2");


        InitBuiltinFunction(QStringList()<< "sqrt(", "initsqrt16");
        InitBuiltinFunction(QStringList()<< "printdecimal(", "initprintdecimal");
        InitBuiltinFunction(QStringList()<< "moveto80(", "initmoveto80");
        InitBuiltinFunction(QStringList()<< "moveto(" << "printstring(" << "tile(", "initmoveto");
        InitBuiltinFunction(QStringList()<< "printstring(" << "printnumber(", "initprintstring");

        InitBuiltinFunction(QStringList()<< "drawtextbox(" << "drawcolortextbox(", "initdrawtextbox");

        InitBuiltinFunction(QStringList()<< "joystick(" , "initjoystick");
        InitBuiltinFunction(QStringList()<< "readjoy1(" , "initjoy1");
        InitBuiltinFunction(QStringList()<< "readjoy2(" , "initjoy2");

        InitBuiltinFunction(QStringList()<< "bcdprint(", "initbcd");

        InitBuiltinFunction(QStringList()<< "vbmtesttilepixel(", "initVbmTestTilePixel");
        InitBuiltinFunction(QStringList()<< "vbmtesttilepixel2(", "initVbmTestTilePixel2");

        InitBuiltinFunction(QStringList()<< "vbmsetdisplaymode(", "initVbm");
        InitBuiltinFunction(QStringList()<< "vbmsetposition1(" << "vbmsetposition2(" << "vbmsetposition4(", "initVbmSetPosition");
        InitBuiltinFunction(QStringList()<< "vbmclear(", "initVbmClear");
        InitBuiltinFunction(QStringList()<< "vbmnextcolumn(", "initVbmNextColumn");
        InitBuiltinFunction(QStringList()<< "vbmdrawdot(" << "vbmcleardot(" << "vbmdrawdote(" << "vbmtestpixel(", "initVbmDot");
        InitBuiltinFunction(QStringList()<< "vbmdrawblot(" << "vbmclearblot(" << "vbmdrawblote(" << "vbmtestpixel2(", "initVbmBlot");
        InitBuiltinFunction(QStringList()<< "vbmscrollleft(", "initVbmScrollLeft");
        InitBuiltinFunction(QStringList()<< "vbmscrollright(", "initVbmScrollRight");
        InitBuiltinFunction(QStringList()<< "vbmscrollfixtop(", "initVbmScrollFixTop");
        InitBuiltinFunction(QStringList()<< "vbmscrollfixbottom(", "initVbmScrollFixBottom");

        InitBuiltinFunction(QStringList()<< "vbmdrawtilemap(", "initVbmDrawTileMap");
        InitBuiltinFunction(QStringList()<< "vbmdrawtilemapo(", "initVbmDrawTileMapO");
        InitBuiltinFunction(QStringList()<< "vbmdrawtilemape(", "initVbmDrawTileMapE");
        InitBuiltinFunction(QStringList()<< "vbmcleartilemap(", "initVbmClearTileMap");

        InitBuiltinFunction(QStringList()<< "vbmscreenshiftleft(", "initVbmScreenShiftLeft");
        InitBuiltinFunction(QStringList()<< "vbmscreenshiftright(", "initVbmScreenShiftRight");

        InitBuiltinFunction(QStringList()<< "vbmspritestitch(", "initVbmSpriteStitch");
        InitBuiltinFunction(QStringList()<< "vbmspriteshiftr(", "initVbmSpriteShiftR");
        InitBuiltinFunction(QStringList()<< "vbmspriteshiftl(", "initVbmSpriteShiftL");
        InitBuiltinFunction(QStringList()<< "vbmspriteshiftsr(", "initVbmSpriteShiftSR");
        InitBuiltinFunction(QStringList()<< "vbmspriteshiftsl(", "initVbmSpriteShiftSL");

        InitBuiltinFunction(QStringList()<< "vbmdrawsprite8(", "initVbmDrawSprite8");
        InitBuiltinFunction(QStringList()<< "vbmdrawsprite8e(", "initVbmDrawSprite8E");
        InitBuiltinFunction(QStringList()<< "vbmclearsprite8(", "initVbmClearSprite8");

        InitBuiltinFunction(QStringList()<< "vbmdrawsprite(", "initVbmDrawSprite");
        InitBuiltinFunction(QStringList()<< "vbmdrawspritee(", "initVbmDrawSpriteE");
        InitBuiltinFunction(QStringList()<< "vbmclearsprite(", "initVbmClearSprite");

        InitBuiltinFunction(QStringList()<< "vbmdrawsprite2(", "initVbmDrawSprite2");
        InitBuiltinFunction(QStringList()<< "vbmdrawsprite2e(", "initVbmDrawSprite2E");
        InitBuiltinFunction(QStringList()<< "vbmclearsprite2(", "initVbmClearSprite2");

        InitBuiltinFunction(QStringList()<< "vbmdrawsprite16(", "initVbmDrawSprite16");
        InitBuiltinFunction(QStringList()<< "vbmdrawsprite16e(", "initVbmDrawSprite16E");
        InitBuiltinFunction(QStringList()<< "vbmclearsprite16(", "initVbmClearSprite16");

        InitBuiltinFunction(QStringList()<< "vbmdrawspriteslice(", "initVbmDrawSpriteSlice");
        InitBuiltinFunction(QStringList()<< "vbmdrawspriteslicee(", "initVbmDrawSpriteSliceE");
        InitBuiltinFunction(QStringList()<< "vbmclearspriteslice(", "initVbmClearSpriteSlice");

        InitBuiltinFunction(QStringList()<< "vbmdrawtext(", "initVbmDrawText");
        InitBuiltinFunction(QStringList()<< "vbmdrawtexto(", "initVbmDrawTextO");
        InitBuiltinFunction(QStringList()<< "vbmdrawtexte(", "initVbmDrawTextE");
        InitBuiltinFunction(QStringList()<< "vbmcleartext(", "initVbmClearText");

        InitBuiltinFunction(QStringList()<< "vbmdrawsmalltexto(", "initVbmDrawSmallTextO");
        InitBuiltinFunction(QStringList()<< "vbmdrawsmalltexte(", "initVbmDrawSmallTextE");
        InitBuiltinFunction(QStringList()<< "vbmclearsmalltext(", "initVbmClearSmallText");

        InitBuiltinFunction(QStringList()<< "vbmdrawbcd(", "initVbmDrawBCD");

        InitBuiltinFunction(QStringList()<< "vbmcopytobuffer(", "initVbmCopyToBuffer");
        InitBuiltinFunction(QStringList()<< "vbmcopyfrombuffer(", "initVbmCopyFromBuffer");


    }
    Node::m_staticBlockInfo.m_blockID = -1;
    Node::m_staticBlockInfo.m_blockPos = "";
    Node::m_staticBlockInfo.m_blockName = "";
//    EndMemoryBlock();
 }

void Parser::InitBuiltinFunction(QStringList methodName, QString builtinFunctionName, QString initJump )
{
    if (m_ignoreMethods.contains(builtinFunctionName.toLower()))
            return;
    QString txt = m_lexer->m_text.toLower();
    for (QString s: methodName)
     if (txt.contains(s)) {
//         Node::m_staticBlockInfo.m_blockID=-1;
        m_procedures[builtinFunctionName] = new NodeProcedureDecl(Token(TokenType::PROCEDURE, builtinFunctionName), builtinFunctionName);
        if (initJump!="")
            m_initJumps << "\tjsr "+ initJump;
        return;
     }

}

void Parser::VerifyToken(Token t)
{
    //if (Syntax::s.globals.contains(t.m_value))
    //    return;

    // ErrorHandler::e.Error("Does not recognize '"+t.m_value + "'");
}

void Parser::InitSystemPreprocessors()
{
    m_preprocessorDefines[AbstractSystem::StringFromSystem(Syntax::s.m_currentSystem->m_system)] = "1";

}

void Parser::PreprocessIfDefs(bool ifdef)
{

    //Eat();
     Eat();
    QString key = m_currentToken.m_value;
//    Eat();

    if (ifdef && m_preprocessorDefines.contains(key)) {
        Eat();
        return; // K
    }

    if (!ifdef && !m_preprocessorDefines.contains(key)) {
        Eat();
        return;
    }

    // Remove everything!
    m_ignoreAll = true;
    while (!m_lexer->m_finished) {
        if (m_currentToken.m_type==TokenType::PREPROCESSOR) {
            m_pass=1;
            Eat();
        }
        else {
            m_pass = 0;
            Eat(); // OM NOM NOM
            m_pass = 1;
        }
        if (m_currentToken.m_type==TokenType::PREPROCESSOR) {
            if (m_currentToken.m_value=="endif") {
                Eat();
                m_ignoreAll = false;

                return; // Finish
            }
        }
    }


}

void Parser::PreprocessConstants()
{
    QString txt;
    //qDebug() << "Here";
    for (QString s: m_lexer->m_text.split("\n")) {
//        qDebug() << s;
        QVector<QString> numbers;

        QString cur = "";
        for (int i=0;i<s.length();i++) {
            if (s[i]==' ')
                continue;
            if (Syntax::s.binop.contains(QString(s[i].toLower()))) {
                cur+=s[i];
            }
            else if (cur!="") {
                //int i = Util::NumberFromStringHex(cur);
                if (cur.contains('*') || cur.contains('+') || cur.contains('-') || cur.contains('/'))
                numbers.append(cur);
                cur ="";

            }
        }
//        if (numbers.count()!=0)
  //          qDebug() << numbers;

        txt += s +"\n";
    }

    m_lexer->m_text = txt;
}

int Parser::getParsedNumberOrConstant() {

    if (m_currentToken.m_value=="") {
        return m_currentToken.m_intVal;
    }
    Symbol* s = m_symTab->LookupConstants(m_currentToken.m_value.toUpper());
    if (s==nullptr)
        ErrorHandler::e.Error("Value required to be a number or a constant.",m_currentToken.m_lineNumber);

    return s->m_value->m_fVal;
}

int Parser::GetParsedInt() {


    bool done = false;
    QString str = "";
    int p = 0;



    while (!done) {
//        qDebug() << "ival:"  << QString::number(m_currentToken.m_intVal);
//        qDebug() << m_currentToken.getType();
        if (m_currentToken.m_type==TokenType::LPAREN) {
            str = str+ "(";
            p++;
            Eat();
            continue;
        }
        if (m_currentToken.m_type==TokenType::RPAREN) {
            if (p==0) {
//                Eat();
                // OOps! Hit a ")" that is not part of an equation!
                done=true;
                continue;

            }
            str = str+ ")";
            p--;
            Eat();
            continue;
        }

        if (m_currentToken.m_type==TokenType::PLUS) {
            str = str+ "+";
            Eat();
            continue;
        }
        if (m_currentToken.m_type==TokenType::MINUS) {
            str = str+ "-";
            Eat();
            continue;
        }
        if (m_currentToken.m_type==TokenType::MUL) {
            str = str+ "*";
            Eat();
            continue;
        }
        if (m_currentToken.m_type==TokenType::DIV) {
            str = str+ "/";
            Eat();
            continue;
        }

        if (m_currentToken.m_value=="") {
            str+=QString::number(m_currentToken.m_intVal);
            Eat();
        }
        else {
            Symbol* s = m_symTab->LookupConstants(m_currentToken.m_value.toUpper());
              if (s==nullptr)
                  done=true;
              else {
               str+=QString::number(s->m_value->m_fVal);
               Eat();
              }

     //             ErrorHandler::e.Error("Value required to be a number or a constant.",m_currentToken.m_lineNumber);

        }
    }
//    if (p!=0)
    for (int i=0;i<p;i++)
        str+=")";
    str = str.remove("()");

    QJSEngine myEngine;
    QJSValue ret = myEngine.evaluate(str);
//    qDebug() << str << ret.toInt();
    return ret.toInt();
}


int Parser::GetParsedIntOld()
{
    bool done = false;
    int val = 0;
    QString op = "plus";
/*    if (m_currentToken.m_value!="") {
        val= getIntVal(m_currentToken);
        Eat();
        return val;
    }
*/
    while (!done) {
//        qDebug() << "ival:"  << QString::number(m_currentToken.m_intVal);
        if (op == "plus")
            val = val + getParsedNumberOrConstant();
        if (op == "mul")
            val = val * getParsedNumberOrConstant();
        if (op == "div")
            val = val / getParsedNumberOrConstant();
        if (op == "minus")
            val = val - getParsedNumberOrConstant();


        Eat();
        done = true;
        if (m_currentToken.m_type == TokenType::MUL) {
            done = false;
            op = "mul";
        }
        if (m_currentToken.m_type == TokenType::DIV) {
            done = false;
            op = "div";
        }
        if (m_currentToken.m_type == TokenType::MINUS) {
            done = false;
            op = "minus";
        }
        if (m_currentToken.m_type == TokenType::PLUS) {
            done = false;
            op = "plus";
        }
        if (!done)
            Eat();

    }
//    qDebug() << QString::number(val);
//    qDebug() << "End " << Util::numToHex(val);

    return val;
}

int Parser::getIntVal(Token t)
{
    int val = t.m_intVal;
    if (t.m_value!="") {
        Symbol* s = m_symTab->Lookup(t.m_value,t.m_lineNumber);
        if (s!=nullptr)
            return s->m_value->m_fVal;
    }

    return val;
}

int Parser::findPage()
{
    int forcePage = 0;

    if (m_currentToken.m_type==TokenType::OFFPAGE) {
        forcePage = 1;
        Eat();
    }
    else
    if (m_currentToken.m_type==TokenType::ONPAGE) {
        forcePage = 2;
        Eat();
    }

    return forcePage ;
}

void Parser::RemoveUnusedProcedures()
{
    QString removeProcedures = "Removing unused procedures: ";
    bool outputUnusedWarning = false;
    QVector<Node*> procs;
    for (Node* n: m_proceduresOnly) {
        NodeProcedureDecl* np = (NodeProcedureDecl*)n;
        if ((np->m_isUsed==true) || m_doNotRemoveMethods.contains(np->m_procName))
            procs.append(n);
        else {
            outputUnusedWarning = true;
            removeProcedures+=np->m_procName + ",";
            //                qDebug() << "Removing procedure: " << np->m_procName;
            //            m_proceduresOnly.removeOne(m_procedures[s]);
        }
    }
    m_proceduresOnly = procs;
    if (outputUnusedWarning) {
        removeProcedures.remove(removeProcedures.count()-1,1);
        ErrorHandler::e.Warning(removeProcedures);
    }


}


void Parser::HandlePreprocessorInParsing()
{

    if (m_currentToken.m_value=="define") {
        Eat();
        Eat();
        Eat();
        return;
    }
    if (m_currentToken.m_value=="export") {
        Eat();
        Eat();
        Eat();
        Eat();
        if (m_currentToken.m_type==TokenType::INTEGER_CONST)
            Eat();
        return;
    }
    if (m_currentToken.m_value=="importchar") {
        Eat();
        Eat();
        Eat();
        Eat();
        Eat();
        return;
    }
    if (m_currentToken.m_value=="spritecompiler") {
        Eat(); // Preprocessor
        Eat(); // Filename
        Eat(); // Name
        Eat(); // X
        Eat(); // Y
        Eat(); // W
        Eat(); // H
    }

    if (m_currentToken.m_value=="vbmexport") {
        Eat();
        Eat(TokenType::STRING);
        Eat(TokenType::STRING);
        Eat(TokenType::INTEGER_CONST);
        Eat(TokenType::INTEGER_CONST);
        Eat(TokenType::INTEGER_CONST);
        Eat(TokenType::INTEGER_CONST);
        return;
    }


    if (m_currentToken.m_value=="donotremove") {
        Eat();
        m_doNotRemoveMethods.append(m_currentToken.m_value);
        Eat();
        return;
    }
    if (m_currentToken.m_value=="userdata") {
        Eat();
        Eat();
        Eat();
        Eat();
        return;
    }
    if (m_currentToken.m_value=="requirefile") {
        Eat();
        Eat();
        Eat();
        return;
    }
    if (m_currentToken.m_value=="raisewarning") {
        Eat();
        Eat();
        return;
    }

    if (m_currentToken.m_value=="raiseerror") {
        Eat();
        Eat();
        return;
    }

    if (m_currentToken.m_value=="ignoremethod") {
        Eat();
        Eat();
        return;
    }
    if (m_currentToken.m_value=="startassembler") {
        Eat();
        Eat();
        return;
    }
/*    if (m_currentToken.m_value=="use") {
        Eat();
        Eat();
        Eat();
        Eat();
        return;
    }
*/

    if (m_currentToken.m_value=="include") {
        Eat();
        Eat();
        return;
    }
    if (m_currentToken.m_value=="endif") {
        Eat();
        return;
    }
    if (m_currentToken.m_value=="ifdef") {
        PreprocessIfDefs(true);
        return;
    }

    if (m_currentToken.m_value=="ifndef") {
        PreprocessIfDefs(false);
        return;
    }
    if (m_currentToken.m_value=="error") {
            Eat();

            if (!m_ignoreAll)
              ErrorHandler::e.Error(m_currentToken.m_value, m_currentToken.m_lineNumber);

            return;
    }



    if (m_currentToken.m_value=="endblock") {
        int i = m_pass;
        m_pass = 2;
        Eat();
        m_pass=i;
//        qDebug() << "HandleCurrent Endblock " << Node::m_staticBlockInfo.m_blockPos;
        Node::m_staticBlockInfo.m_blockID = -1;
        Node::m_staticBlockInfo.m_blockPos = "";
        Node::m_staticBlockInfo.m_blockName="";
        m_pass = i;
        return;
    }
    if (m_currentToken.m_value=="startblock") {
        int i = m_pass;
        m_pass = 2;
        Eat();
        QString startPos = m_currentToken.getNumAsHexString();
        Eat();
        QString name = m_currentToken.m_value;
        Eat();
        ParserBlock pb;
        pb.m_blockID = m_parserBlocks.count();
        pb.pos = startPos;
        m_parserBlocks.append(pb);
        Node::m_staticBlockInfo.m_blockID = pb.m_blockID;
        Node::m_staticBlockInfo.m_blockPos = pb.pos;
        Node::m_staticBlockInfo.m_blockName = name;
//        qDebug() << "HandleCurrent StartBlock " << Node::m_staticBlockInfo.m_blockPos;
        m_pass = i;

        return;
    }


}

void Parser::StripWhiteSpaceBeforeParenthesis()
{
    m_lexer->m_text = m_lexer->m_text.replace(QRegularExpression("\\s*(\\()"),"\\1");
}

void Parser::RemoveComments()
{
    QRegularExpression rg = QRegularExpression("/\\*([^*]|[\\r\\n]|(\\*+([^*/]|[\\r\\n])))*\\*+/");

    //qDebug() << rg;
    m_lexer->m_text = m_lexer->m_text.replace(rg, "");

    QRegularExpression rg2 = QRegularExpression("//.*?\\n");
    //qDebug() << rg;
    m_lexer->m_text = m_lexer->m_text.replace(rg2, "");

    //qDebug() << m_lexer->m_text;

}

bool Parser::PreprocessIncludeFiles()
{
    m_lexer->Initialize();
    m_lexer->m_ignorePreprocessor = false;
    m_acc = 0;
    bool done = true;
    m_currentToken = m_lexer->GetNextToken();
    //m_preprocessorDefines.clear();
    while (m_currentToken.m_type!=TokenType::TEOF) {
        if (m_currentToken.m_type == TokenType::PREPROCESSOR) {
            if (m_currentToken.m_value.toLower()=="include") {

//                QString str = m_currentToken.m_value;
                Eat(TokenType::PREPROCESSOR);
                QString name = m_currentToken.m_value;
                QString filename =(m_currentDir +"/"+ m_currentToken.m_value);
                filename = filename.replace("//","/");
                QString text = m_lexer->loadTextFile(filename);
                int ln=m_lexer->getLineNumber(m_currentToken.m_value)+m_acc;
                m_lexer->m_text.insert(m_lexer->m_pos, text);
                int count = text.split("\n").count();
                m_lexer->m_includeFiles.append(
                            FilePart(name,ln, ln+ count, ln-m_acc,ln+count-m_acc,count));
                m_acc-=count-1;
                done = false;
                Eat(TokenType::STRING);
            }
        }
        Eat();
//        qDebug() << m_currentToken.m_value;

    }
    return done;
}



Node *Parser::Variable()
{
    Node* n = nullptr;

    if (SymbolTable::m_constants.contains(m_currentToken.m_value.toUpper())) {
        Symbol* s = SymbolTable::m_constants[m_currentToken.m_value.toUpper()];

//        qDebug() << "looking for " << m_currentToken.m_value << " , found symbol: " << s->m_name << " with value " << s->m_value->m_fVal << " of type " << s->m_type;

//        qDebug() << m_currentToken.m_value;




        if (s->m_type=="ADDRESS") m_currentToken.m_type=TokenType::ADDRESS;
        if (s->m_type=="LONG") m_currentToken.m_type=TokenType::LONG;
        if (s->m_type=="INTEGER") m_currentToken.m_type=TokenType::INTEGER;
        if (s->m_type=="WORD") m_currentToken.m_type=TokenType::INTEGER;
        if (s->m_type=="BYTE") m_currentToken.m_type=TokenType::BYTE;
        if (s->m_type=="STRING") m_currentToken.m_type=TokenType::STRING;
        if (s->m_type=="CSTRING") m_currentToken.m_type=TokenType::CSTRING;

        Token t = m_currentToken;
        Eat(m_currentToken.m_type);
        Node *expr = nullptr;
        if (m_currentToken.m_type==TokenType::LBRACKET) {
            Eat(TokenType::LBRACKET);
            expr = Expr();
            Eat(TokenType::RBRACKET);
         }

        if (t.m_type==TokenType::ADDRESS && expr!=nullptr) {
            t.m_value = "$"+QString::number( (int)s->m_value->m_fVal,16);
            n = new NodeVar(t,expr);
        }
        else {
            n = new NodeNumber(t, s->m_value->m_fVal);
            //qDebug()  << s->m_value->m_fVal;
        }

    }
    else {
        Token t = m_currentToken;
        if (m_currentToken.m_type==TokenType::STRING || m_currentToken.m_type==TokenType::CSTRING) {
           n = String();
           return n;

        }

        Eat(m_currentToken.m_type);
        if (m_currentToken.m_type!=TokenType::LBRACKET) {
            if (t.m_value.endsWith("^")) {
                t.m_value.replace("^","");
  //              qDebug() << t.m_value;
//                exit(1);
                t.m_type = TokenType::ADDRESS;
            }
            n = new NodeVar(t);
        }
        else
            {
                Eat(TokenType::LBRACKET);
                Node* expr = Expr();
                Eat(TokenType::RBRACKET);
                n = new NodeVar(t, expr);


        }

    }
    if (n==nullptr) {
        ErrorHandler::e.Error("Could not assign variable!");
    }
    return n;
}

Node *Parser::Empty()
{
    return new NoOp();
}


Node *Parser::AssignStatement()
{
    Node* arrayIndex = nullptr;
    Token t = m_currentToken;
    Node* left = Variable();
    Token token = m_currentToken;


    if (m_currentToken.m_type!=TokenType::ASSIGN) {
//        qDebug() << m_currentToken;
        // First, check if similar procedure exists
        QString val = t.m_value;
        QString em = "Could not find procedure '<font color=\"#FF8000\">" + val + "</font>'<br>";
//        QString similarSymbol = m_symTab->findSimilarSymbol(val,70,2,m_procedures.keys());
        QString similarSymbol = "";
          for (QString s:m_procedures.keys()) {
                if (Util::QStringIsSimilar(val,s,65,2,Qt::CaseInsensitive)) {
                    similarSymbol = s;
                    break;
                }
            }

        if (similarSymbol!="") {
            em+="Did you mean '<font color=\"#A080FF\">"+similarSymbol+"</font>'?<br>";
            ErrorHandler::e.Error(em , token.m_lineNumber);
        }



        ErrorHandler::e.Error("Error assigning variable <b>'" + t.m_value+  "'</b>, did you forget a colon or mistype? Syntax should be: <b>'a := b;'</b>." , token.m_lineNumber);
    }
    Eat(TokenType::ASSIGN);
    Node* right = Expr();
    NodeAssign* na = dynamic_cast<NodeAssign*>(new NodeAssign(left, t, right));
    na->m_right->ApplyFlags();// make sure integer:=byte*byte works
    return na;


}

Node *Parser::Statement()
{
    Node *node = nullptr;
    if (m_currentToken.m_type == TokenType::BEGIN) {
        node = CompoundStatement();
    }
    else if (m_currentToken.m_type == TokenType::ID) {

        node = FindProcedure();
        if (node==nullptr)
            node = BuiltinFunction();
//        if (node==nullptr)
//            node = Constant();
        if (node==nullptr)
            node = AssignStatement();

    }
    else if (m_currentToken.m_type == TokenType::ADDRESS) {
        if (node==nullptr)
            node = AssignStatement();

    }
    else if (m_currentToken.m_type == TokenType::IF) {
        Eat(TokenType::IF);
        node = Conditional();
    }
    else if (m_currentToken.m_type == TokenType::FORI) {
        node = ForLoop(true);
    }
    else if (m_currentToken.m_type == TokenType::FOR) {
        node = ForLoop(false);
    }
    else if (m_currentToken.m_type == TokenType::WHILE) {
        Eat(TokenType::WHILE);
        node = Conditional(true);
    }
    else if (m_currentToken.m_type == TokenType::REPEAT) {
        node = RepeatUntil();
    }
    else if (m_currentToken.m_type == TokenType::ASM) {
        return InlineAssembler();

    } else if (m_currentToken.m_type == TokenType::CASE) {
        return Case();
    }

    else {
        //ErrorHandler::e.Error("Unknown method " + m_currentToken.getType());
        return Empty();
    }

    if (node==nullptr)
        ErrorHandler::e.Error("Node is nullpointer. Should not happen. Contact leuat@irio.co.uk and slap him.",0);

    AppendComment(node);

    return node;


}

Node *Parser::Case()
{
    NodeCase* n = new NodeCase(m_currentToken);
    Eat(); // Eat "case"
    n->m_variable = dynamic_cast<NodeVar*>(Variable());
    Eat(TokenType::OF);
    while (m_currentToken.m_type != TokenType::END && m_currentToken.m_type != TokenType::ELSE) {
        Node* expr = Expr();
        Eat(TokenType::COLON);
        Node* block = Block(false);
        n->Append(expr, block);
        Eat(); // Eat the semicolon

    }
    if (m_currentToken.m_type == TokenType::ELSE) {
        Eat();
        n->m_elseBlock = Block(false);
    }
    else
    Eat(); // Eat final END
    return n;

}

Node *Parser::BinaryClause()
{
    if (m_currentToken.m_type == TokenType::LPAREN) {
        // Logical clause AND OR
        Eat(TokenType::LPAREN);
        Node* a = BinaryClause();
        if (m_currentToken.m_type==TokenType::RPAREN) {
            Eat();
            return a;
        }
        Token logical = m_currentToken;
        Eat();
        Node* b = BinaryClause();
        Eat(TokenType::RPAREN);
        return new NodeBinaryClause(logical, a, b);
    }

    Node* a = Expr();
    Token comparetoken;
    Node* b;
    // Nothing : the null test. Check if NOT EQUALS ZERO
    if (m_currentToken.m_type==TokenType::RPAREN || m_currentToken.m_type==TokenType::THEN) {
        Token t;
        t.m_type = TokenType::BYTE;
        t.m_intVal = 0;
        b = new NodeNumber(t,0);
        comparetoken.m_type = TokenType::NOTEQUALS;
    }
    else
    {
        comparetoken = m_currentToken;

        if (!(comparetoken.m_type==TokenType::EQUALS || comparetoken.m_type==TokenType::NOTEQUALS ||
            comparetoken.m_type==TokenType::GREATER || comparetoken.m_type==TokenType::LESS ||
            comparetoken.m_type==TokenType::GREATEREQUAL || comparetoken.m_type==TokenType::LESSEQUAL))
        {
            ErrorHandler::e.Error("Unknown compare type : '" + comparetoken.m_value+"'. Did you mean '=' or '>' etc?",comparetoken.m_lineNumber);
        }



         Eat();
        b = Expr();

    }
    return new NodeBinaryClause(comparetoken, a, b);
}


void Parser::AppendComment(Node *n)
{
    if (m_lexer->m_currentComment=="")
        return;
    if (n==nullptr)
        return;
    n->m_comment = m_lexer->m_currentComment;
    m_lexer->m_currentComment="";
}



Node *Parser::Conditional(bool isWhileLoop)
{

    QVector<Node*> left, right;
    QVector<Token> compareTokens, conditionals;

    // Start
    Token t = m_currentToken;
    bool done=false;
    int linenum = m_currentToken.m_lineNumber;

    Node* clause = BinaryClause();

    int forcePage = findPage();

    if (m_currentToken.m_type==TokenType::THEN || m_currentToken.m_type==TokenType::DO)
        Eat(m_currentToken.m_type);
    else {
        ErrorHandler::e.Error("Expected THEN or DO after conditional", linenum);
    }

    Node* block = Block(false);

    Node* nodeElse = nullptr;
    if (m_currentToken.m_type==TokenType::ELSE) {
        Eat(TokenType::ELSE);
        nodeElse = Block(false);
    }

    return new NodeConditional(t, forcePage, clause, block, isWhileLoop, nodeElse);
}


QVector<Node*> Parser::StatementList()
{

    Node* node = Statement();
    QVector<Node*> results;
    results.append(node);
    while (m_currentToken.m_type == TokenType::SEMI) {
        Eat(TokenType::SEMI);
        Node* n = Statement();

        results.append(n);

    }
    if (m_currentToken.m_type==TokenType::ID)
//        ErrorHandler::e.Error("Parser::Statementlist SYNTAX ERROR : Token should not be ID", m_currentToken.m_lineNumber);
    ErrorHandler::e.Error("Did you forget a semicolon? (Token should not be ID in Parser)", m_currentToken.m_lineNumber);


    return results;

}

Node *Parser::CompoundStatement()
{
    if (m_currentToken.m_type!=TokenType::BEGIN) {
        // Single statement
        Node* n =  Statement();
  //      Eat(TokenType::SEMI);
    //    qDebug() << m_currentToken.getType();
      //  qDebug() << m_currentToken.m_value;

        return n;
    }
    Token t = m_currentToken;
    Eat(TokenType::BEGIN);
    QVector<Node*> nodes = StatementList();
    Eat(TokenType::END);
    NodeCompound* root = new NodeCompound(t);
    for (Node* n: nodes)
        root->children.append(n);

//    qDebug() << "from begin block : " +m_currentToken.getType();
    return root;

}

Node *Parser::Program(QString param)
{
//    Node* n = CompoundStatement();
    Eat(TokenType::PROGRAM);
    NodeVar* varNode = (NodeVar*)Variable();
    QString progName = varNode->value;

    Eat(TokenType::SEMI);
    NodeBlock* block = (NodeBlock*)Block(true);
    NodeProgram* program = new NodeProgram(progName,  param, block);
    Eat(TokenType::DOT);

    return program;
}


Node* Parser::Factor()
{
    if (m_currentToken.m_type == TokenType::LENGTH) {
        Eat();
        Eat(TokenType::LPAREN);
        QString varName = m_currentToken.m_value;
        Symbol* s = m_symTab->Lookup(varName,m_currentToken.m_lineNumber);
        if (s==nullptr) {
            ErrorHandler::e.Error("Internal function 'Length' reqruires a variable");
        }
        Token t = m_currentToken;
        t.m_intVal = s->getLength();
        t.m_type  = TokenType::INTEGER_CONST;
        Eat();
        Eat(TokenType::RPAREN);
        return new NodeNumber(t,s->getLength());

    }


    Token t = m_currentToken;

    if (t.m_type==TokenType::TEOF)
        ErrorHandler::e.Error("Syntax error", m_currentToken.m_lineNumber);


    if (t.m_type == TokenType::INTEGER_CONST || t.m_type ==TokenType::REAL_CONST
            || t.m_type ==TokenType::ADDRESS) {
        Eat(t.m_type);
        //qDebug() << "parser: " <<t.m_value << t.m_intVal;
        return new NodeNumber(t, t.m_intVal);
    }

    if (t.m_type == TokenType::PLUS || t.m_type==TokenType::MINUS ){
        Eat(t.m_type);
        return new NodeUnaryOp(t, Factor());
    }




    if (t.m_type == TokenType::LPAREN) {
        Eat(TokenType::LPAREN);
        Node* node = Expr();
        Eat(TokenType::RPAREN);
        return node;

    }
    if (t.m_type == TokenType::ID) {
//        qDebug() << "FINDING PROCEDURE IN TERM: " << t.m_value;
        Node* node = FindProcedure();
        if (node!=nullptr)
            return node;
        node = BuiltinFunction();
        if (node!=nullptr)
            return node;

    }
    return Variable();
}

Node *Parser::RepeatUntil()
{
    Token t = m_currentToken;
    Eat(TokenType::REPEAT);
    QVector<Node*> nodes = StatementList();
    NodeCompound* root = new NodeCompound(t);
    Eat(TokenType::UNTIL);
    Node* cond = BinaryClause();
    QVector<Node*> decl;

    for (Node* n: nodes)
        root->children.append(n);

    NodeBlock* block = new NodeBlock(t,decl,root);

//    qDebug() << "from begin block : " +m_currentToken.getType();
//    NodeRepeatUntil(Token op, int forcePage, Node* cond, Node* block);

    return new NodeRepeatUntil(t,false,dynamic_cast<NodeBinaryClause*>(cond),block);

}

Node* Parser::Term()
{
    Node* node = Factor();
    while (m_currentToken.m_type == TokenType::Type::MUL || m_currentToken.m_type == TokenType::Type::DIV
    || m_currentToken.m_type == TokenType::Type::BITAND || m_currentToken.m_type == TokenType::Type::BITOR
     || m_currentToken.m_type == TokenType::Type::SHR || m_currentToken.m_type == TokenType::Type::SHL
           || m_currentToken.m_type == TokenType::Type::XOR

           ){
        Token t = m_currentToken;
        Eat(m_currentToken.m_type);

        node = new NodeBinOP(node, t, Factor());

    }
    return node;
}


void Parser::Preprocess()
{
    m_lexer->Initialize();
    m_lexer->m_ignorePreprocessor = false;
    m_acc = 0;
    m_currentToken = m_lexer->GetNextToken();
    //m_preprocessorDefines.clear();
    while (m_currentToken.m_type!=TokenType::TEOF) {
//        qDebug() << m_currentToken.getType() << m_currentToken.m_value;



        if (m_currentToken.m_type == TokenType::PREPROCESSOR) {
  //          qDebug() << "***PRE" << m_currentToken.m_value;


/*            if (m_currentToken.m_value.toLower()=="include") {

//                QString str = m_currentToken.m_value;
                Eat(TokenType::PREPROCESSOR);
                QString name = m_currentToken.m_value;
                QString filename =(m_currentDir +"/"+ m_currentToken.m_value);
                filename = filename.replace("//","/");
                QString text = m_lexer->loadTextFile(filename);
                int ln=m_lexer->getLineNumber(m_currentToken.m_value)+m_acc;
                m_lexer->m_text.insert(m_lexer->m_pos, text);
                int count = text.split("\n").count();
                m_lexer->m_includeFiles.append(
                            FilePart(name,ln, ln+ count, ln-m_acc,ln+count-m_acc,count));
                m_acc-=count-1;

                Eat(TokenType::STRING);
            }*/
            if (m_currentToken.m_value.toLower() =="define") {
                Eat(TokenType::PREPROCESSOR);
                QString key = m_currentToken.m_value;
                Eat();
//                qDebug() << m_currentToken.m_value;
    //            int i = getIntVal(m_currentToken);
  //              qDebug() << "After: " << Util::numToHex(i);
                QString val = m_currentToken.m_value;
                if (val=="")
                    val = QString::number(m_currentToken.m_intVal);

                m_preprocessorDefines[key] = val;

//                qDebug() << "Defined: " << key << val;

            }
            else if (m_currentToken.m_value.toLower() =="userdata") {
                Eat(TokenType::PREPROCESSOR);
                QString from = m_currentToken.getNumAsHexString();
                Eat();
                QString to = m_currentToken.getNumAsHexString();
                Eat();
                QString name = m_currentToken.m_value;
                bool ok;
                m_userBlocks.append(new MemoryBlock(Util::NumberFromStringHex(from), Util::NumberFromStringHex(to),
                                                MemoryBlock::USER, name));

            }
            else if (m_currentToken.m_value.toLower() =="ignoremethod") {
                Eat(TokenType::PREPROCESSOR);
                m_ignoreMethods.append(m_currentToken.m_value);
            }
            else if (m_currentToken.m_value.toLower() =="export") {
                Eat(TokenType::PREPROCESSOR);
                HandleExport();
            }
            else if (m_currentToken.m_value.toLower() =="vbmexport") {
                Eat(TokenType::PREPROCESSOR);
                HandleVBMExport();
            }
            else if (m_currentToken.m_value.toLower() =="spritecompiler") {
                Eat(TokenType::PREPROCESSOR);
                HandleSpriteCompiler();
            }

            else if (m_currentToken.m_value.toLower() =="importchar") {
                Eat(TokenType::PREPROCESSOR);
                HandleImportChar();
            }

            else if (m_currentToken.m_value.toLower() =="startassembler") {
                Eat(TokenType::PREPROCESSOR);
                m_initAssembler = m_currentToken.m_value;
                //m_ignoreMethods.append(m_currentToken.m_value);
            }
            else if (m_currentToken.m_value.toLower() =="requirefile") {
                Eat();
                QString requiredFile = m_currentToken.m_value;
                Eat();
                QString message = m_currentToken.m_value;
                if (!QFile::exists(m_currentDir+"/"+requiredFile))
                    ErrorHandler::e.Error("The following file is required for compilation: <font color=\"#FF80A0\">'" + requiredFile + "'</font>.<br><font color=\"#FFB060\">" +message+"</font>", m_currentToken.m_lineNumber);

            }
            else if (m_currentToken.m_value.toLower() =="raisewarning") {
                Eat();
                ErrorHandler::e.Warning(m_currentToken.m_value, m_currentToken.m_lineNumber);
            }
            else if (m_currentToken.m_value.toLower() =="raiseerror") {
                Eat();
                ErrorHandler::e.Error(m_currentToken.m_value, m_currentToken.m_lineNumber);
            }
            else if (m_currentToken.m_value.toLower() =="use") {
                Eat();
                QString type = m_currentToken.m_value;
                bool ok=false;
                if (type.toLower()=="krillsloader") {
                    ok=true;
                    int ln = Pmm::Data::d.lineNumber;

                    //m_lexer->m_lines.removeAt(ln);
                    //m_lexer->m_orgText.replace(orgL,"\n");
                    Eat();
                    int loaderPos = m_currentToken.m_intVal;
                    Eat();
                    int loaderOrgPos = m_currentToken.m_intVal;
                    Eat();
                    int installerPos = m_currentToken.m_intVal;


                    m_preprocessorDefines["_InstallKrill"] = Util::numToHex(installerPos + 0x1390);
                    m_preprocessorDefines["_LoadrawKrill"] = Util::numToHex(loaderPos);
//                    m_preprocessorDefines["_LoadrawKrill"] = Util::numToHex(loaderPos);
//                    qDebug() << m_preprocessorDefines["_LoadrawKrill"];
                    m_preprocessorDefines["_ResidentLoaderSource"] = Util::numToHex(loaderOrgPos);
                    m_preprocessorDefines["_ResidentLoaderDestination"] = Util::numToHex(loaderPos);


                    QString pos = QString::number(loaderPos,16);
                    if (pos=="200") pos = "0200";
                    QString loaderFile =":resources/bin/krill/loader_PAL_NTSC_"+pos.toUpper()+"-c64.prg";
                    QString installerFile =":resources/bin/krill/install_PAL_NTSC_"+QString::number(installerPos,16).toUpper()+"-c64.prg";

                    if (!QFile::exists(loaderFile))
                        ErrorHandler::e.Error("When using krills loader, the loader location must be either 0200, 1000,2000 etc");

                    if (!QFile::exists(installerFile))
                        ErrorHandler::e.Error("When using krills loader, the installer location must be either 1000, 2000, 3000 etc");


                    QString outFolder = m_currentDir+"/auto_bin/";
                    QString outFolderShort = "auto_bin/";

                    if (!QDir().exists(outFolder))
                            QDir().mkdir(outFolder);

                    QString outFile = outFolder+"krill_loader.bin";
                    if (QFile::exists(outFile)) {
                        QFile f(outFile);
                        f.remove();
                    }
//                    QFile::copy(loaderFile, outFile);
                    Util::ConvertFileWithLoadAddress(loaderFile,outFile);

                    outFile = outFolder+"krill_installer.bin";
                    if (QFile::exists(outFile)) {
                        QFile f(outFile);
                        f.remove();
                    }
                    Util::ConvertFileWithLoadAddress(installerFile,outFile);
//                    QFile in(installerFile);
  //                  QByteArray data =

                    //QFile::copy(installerFile, outFile);

                    outFile = outFolderShort+"krill_loader.bin";
                    QString replaceLine = "_ResidentLoader_Binary: 	incbin (\""+outFile+ "\",$"+QString::number(loaderOrgPos,16)+");";
                    outFile = outFolderShort+"krill_installer.bin";
                    replaceLine += "\n_Installer_Binary: 	incbin (\""+outFile+ "\",$"+QString::number(installerPos,16)+");";

                    for (QString s: m_diskFiles) {
                        QString var = s;
                        for (int i=0;i<256;i++) {
                            QString r = "#P"+QString::number(i)+";";
                            var = var.replace(r,"");
//                            s = s.replace(r,QChar(i));
 //                           s = s.replace(r,"\""  +QString::number(i)  + "\"");
                        }

                        replaceLine+= var + ": string=(\""+s.toUpper()+"\");";

                    }
//                    qDebug() << replaceLine;

                    // Now load all disk files
//                    CIniFile paw;
  //                  paw.Load()


//_Installer_Binary: 		incbin ("bin/install-c64.bin",$5000);
  //                  qDebug() << replaceLine;
//                    qDebug() << Util::numToHex(loaderPos);
                    QString orgL =  m_lexer->m_lines[ln];



                    m_lexer->m_text.replace(orgL,replaceLine+"\n\t");
                    m_lexer->m_pos-=orgL.count();


                    //Eat();
                }
                if (!ok) {
                    ErrorHandler::e.Error("Uknown @use parameter : "+type, m_currentToken.m_lineNumber);
                }
            }
            else Eat();


          /*  else if (m_currentToken.m_value.toLower() =="error") {
                Eat(TokenType::PREPROCESSOR);
                ErrorHandler::e.Error("Error from preprocessor3: " +m_currentToken.m_value);

            }*/
        }
        else Eat();
//        if (m_currentToken.m_type!=TokenType::PREPROCESSOR)
  //          Eat();
        //qDebug() << "VAL: " << m_currentToken.m_value << m_currentToken.getType();

    }

    // Afterwards, replace all preprocessor defines
//    PreprocessIfDefs();
    PreprocessReplace();
}

void Parser::PreprocessReplace()
{

    for (QString k: m_preprocessorDefines.keys()) {
        QString val = m_preprocessorDefines[k];
//        qDebug() << "Replacing: @" + k << "  with " << val;
        QRegularExpression rg = QRegularExpression("@\\b"+k+"\\b");
        //qDebug() << rg;

//        m_lexer->m_text = m_lexer->m_text.replace("@" +k, val);
        m_lexer->m_text = m_lexer->m_text.replace(rg, val);

    }

  //  qDebug() << m_lexer->m_text;
//    exit(1);

//    qDebug() << m_preprocessorDefines.keys();
}

Node* Parser::Parse(bool removeUnusedDecls, QString param, QString globalDefines, bool useLocals)
{
    // Call preprocessor for include files etc
    m_lexer->m_orgText = m_lexer->m_orgText + "\n" + globalDefines+"\n";
    m_lexer->m_text = m_lexer->m_orgText;
    m_pass = 0;
  //  RemoveComments();
    InitObsolete();
    StripWhiteSpaceBeforeParenthesis(); // TODO: make better fix for this
    InitSystemPreprocessors();
    bool done = false;
    //while (!done)
        done = PreprocessIncludeFiles();

    Preprocess();
//    PreprocessConstants();
    m_pass = 1;
    m_lexer->m_currentComment = "";
    m_parserBlocks.clear();
    SymbolTable::m_constants.clear();
    m_symTab = new SymbolTable();
    m_symTab->m_useLocals = useLocals;


    Node::parserSymTab = m_symTab; // Clear all node flags
    Node::flags.clear();

    m_lexer->Initialize();
//    qDebug() << m_lexer->m_text;

    m_lexer->m_ignorePreprocessor = true;
    m_currentToken = m_lexer->GetNextToken();
   /* qDebug() << m_lexer->m_pos;
    qDebug() << m_lexer->m_currentChar;
    qDebug() << m_currentToken.getType();
*/
//    qDebug() << m_ignoreMethods;
    //qDebug() <<m_lexer->m_text[0];
    SymbolTable::Initialize();
    Node::m_staticBlockInfo.m_blockID=-1;
    NodeProgram* root = (NodeProgram*)Program(param);
    // First add builtin functions
    if (removeUnusedDecls)
        RemoveUnusedProcedures();

    InitBuiltinFunctions();

    for (QString s: m_procedures.keys())
        if (((NodeProcedureDecl*)m_procedures[s])->m_block==nullptr)
            root->m_NodeBlock->m_decl.append(m_procedures[s]);


    // Then add regular ones ORDERED BY DEFINITION
    //for (QString s: m_procedures.keys())
     //   if (((NodeProcedureDecl*)m_procedures[s])->m_block!=nullptr)
        for ( Node* n: m_proceduresOnly )
            root->m_NodeBlock->m_decl.append(n);


    if (m_currentToken.m_type!=TokenType::TEOF)
        ErrorHandler::e.Error("End of file error");


    return root;
}

Node *Parser::FindProcedure()
{
    Token procToken = m_currentToken;
    if (m_procedures.contains(m_currentToken.m_value)) {
        QString procName = m_currentToken.m_value;
        Token t = m_currentToken;
        Eat(TokenType::ID);
        Eat(TokenType::LPAREN);
        QVector<Node*> paramList;
        while (m_currentToken.m_type!=TokenType::RPAREN && !m_lexer->m_finished) {
            paramList.append(Expr());

            if (m_currentToken.m_type==TokenType::COMMA)
                Eat(TokenType::COMMA);
            //if (m_currentToken.m_type==TokenType::SEMI)
            //    ErrorHandler::e.Error("Syntax errror", m_currentToken.m_lineNumber);
        }
        if (!m_procedures.contains(procName))
            ErrorHandler::e.Error("Could not find procedure :" + procName, m_currentToken.m_lineNumber);

        NodeProcedureDecl* p = (NodeProcedureDecl*)m_procedures[procName];
        Eat(TokenType::RPAREN);

        //p->SetParameters(paramList);
        p->m_isUsed = true;
//        if (p->m_procName==BGMUpdateSpriteLoc)

        return new NodeProcedure(p, paramList, t);
    }

    //qDebug() << m_currentToken.getType() << " with value " << m_currentToken.m_value;
    return nullptr;
}


Node *Parser::Block(bool useOwnSymTab, QString blockName)
{

/*    if (m_currentToken.m_type!=TokenType::VAR  && m_currentToken.m_type!=TokenType::BEGIN)
        return nullptr;
*/

    if (m_currentToken.m_type==TokenType::PROCEDURE || m_currentToken.m_type==TokenType::INTERRUPT || m_currentToken.m_type==TokenType::WEDGE)
        return nullptr;
    QVector<Node*> decl =  Declarations(useOwnSymTab, blockName);
    int pos = m_currentToken.m_lineNumber;
    Node* vars =  CompoundStatement();
    NodeBlock* bl =  new NodeBlock(m_currentToken,decl, vars, useOwnSymTab);
    bl->m_op.m_lineNumber = pos;
    return bl;
}

QVector<Node *> Parser::Parameters(QString blockName)
{
    QVector<Node*> decl;
    if (m_currentToken.m_type==TokenType::LPAREN) {
        Eat(TokenType::LPAREN);
        while (m_currentToken.m_type==TokenType::ID) {
            QVector<Node*> ns = VariableDeclarations(blockName);

            for (Node* n: ns)
                decl.append(n);
            Eat(m_currentToken.m_type);
        }
    }
    //Eat(TokenType::RPAREN);
    return decl;
}

Node *Parser::ForLoop(bool inclusive)
{
    int ln = m_currentToken.m_lineNumber;
    if (inclusive)
        Eat(TokenType::FORI);
    else
        Eat(TokenType::FOR);
    Node* a = AssignStatement();
    Eat(TokenType::TO);
    Node* b = Expr();
    bool unroll = false;
    Node* step = nullptr;

    int forcePage = 0;
    int loopType = 0; // use var
    int curCnt=0;
    while (m_currentToken.m_type!=TokenType::DO) {
        if (m_currentToken.m_type==TokenType::ONPAGE || m_currentToken.m_type==TokenType::OFFPAGE)
            forcePage = findPage();
        if (m_currentToken.m_type==TokenType::STEP) {
            Eat();
            step = Expr();
            //qDebug() << TokenType::getType(step->m_op.m_type);
        }
        if (m_currentToken.m_type==TokenType::LOOPX) {
            Eat();
            loopType = 1;
        }
        if (m_currentToken.m_type==TokenType::LOOPY) {
            Eat();
            loopType = 2;
        }
        if (m_currentToken.m_type==TokenType::UNROLL) {
            Eat();
            unroll = true;
        }
        if (curCnt++>15) {
            ErrorHandler::e.Error("For loop needs a 'DO' keyword", ln);
        }
    }


    Eat(m_currentToken.m_type);
//    qDebug() << "Current: " << m_currentToken.getType();
//    Eat(TokenType::DO);
    Node* block = Block(false);

//    qDebug() << m_currentToken.getType();
  //  exit(1);
    return new NodeForLoop(a,b,block, step, unroll, forcePage, loopType, inclusive);

}

Node *Parser::String()
{

    if (m_currentToken.m_type==TokenType::STRING || m_currentToken.m_type==TokenType::CSTRING) {
        NodeString* node = new NodeString(m_currentToken,QStringList()<<m_currentToken.m_value,m_currentToken.m_type==TokenType::CSTRING);
        Eat();
        return node;
    }
    if (m_currentToken.m_type!=TokenType::LPAREN)
        ErrorHandler::e.Error("String declaration must be single string or paranthesis with multi values.", m_currentToken.m_lineNumber);

    Eat();
    Token token(TokenType::STRING, m_currentToken.m_value);
    //m_currentToken.m_type = TokenType::STRING;
    QStringList lst;
    bool done = false;
//    lst<<m_currentToken.m_value;
    int max=0;
    while (m_currentToken.m_type!=TokenType::RPAREN) {
        if (m_currentToken.m_value=="")
            m_currentToken.m_value = QString::number(m_currentToken.m_intVal);

        if (m_currentToken.m_value!="")
            lst<<m_currentToken.m_value;
        Eat();
        if (m_currentToken.m_type==TokenType::COMMA)
            Eat();
        if (max++>10000)
            ErrorHandler::e.Error("String error!", token.m_lineNumber);
    }
    Eat(); // RParen
//    qDebug() <<m_currentToken.getType();

    NodeString* node = new NodeString(token, lst, token.m_type==TokenType::CSTRING);
    return node;
}


QVector<Node*> Parser::Declarations(bool isMain, QString blockName)
{
    QVector<Node*> decl;
    if (m_currentToken.m_type==TokenType::VAR) {
        Eat(TokenType::VAR);
        while (m_currentToken.m_type==TokenType::ID || m_currentToken.m_type == TokenType::CONSTANT) {

            if (m_currentToken.m_type == TokenType::CONSTANT) {
                ConstDeclaration();
            }
            else {

                QVector<Node*> ns = VariableDeclarations(blockName);

                for (Node* n: ns)
                    decl.append(n);
            }
            Eat(TokenType::SEMI);
        }
    }




/*
    while (m_currentToken.m_type==TokenType::ID || m_currentToken.m_type==TokenType::PROCEDURE ||
           m_currentToken.m_type==TokenType::INTERRUPT || m_currentToken.m_type == TokenType::VAR) {


    if (m_currentToken.m_type==TokenType::VAR) {
        Eat(TokenType::VAR);
        continue;
    }

    if (m_currentToken.m_type==TokenType::ID) {
            QVector<Node*> ns = VariableDeclarations();
            for (Node* n: ns)
                decl.append(n);
            Eat(TokenType::SEMI);
            continue;
     }


*/
    while (m_currentToken.m_type==TokenType::PROCEDURE || m_currentToken.m_type==TokenType::INTERRUPT || m_currentToken.m_type == TokenType::WEDGE) {
        int type=0;
        if (m_currentToken.m_type == TokenType::INTERRUPT) type=1;
        if (m_currentToken.m_type == TokenType::WEDGE) type=2;
     //   bool isInterrupt= (m_currentToken.m_type==TokenType::PROCEDURE)?false:true;
        Token tok = m_currentToken;
        Eat(m_currentToken.m_type);
        QString procName = m_currentToken.m_value;
        //qDebug() << tok.m_value  << " : " << procName;
        Eat(TokenType::ID);
        //exit(1);
        QVector<Node*> paramDecl;
        if (m_currentToken.m_type==TokenType::LPAREN)
            paramDecl = Parameters(procName);
        //qDebug()<< "current : " << m_currentToken.getType();
        // If no parameters
        if (m_currentToken.m_type==TokenType::RPAREN)
            Eat(TokenType::RPAREN);

        Eat(TokenType::SEMI);
        Node* block = nullptr;
        NodeProcedureDecl* procDecl = new NodeProcedureDecl(tok, procName, paramDecl, block, type);
        AppendComment(procDecl);

        if (m_procedures[procName]!=nullptr)
            procDecl->m_isUsed = m_procedures[procName]->m_isUsed;
        m_procedures[procName] = procDecl;

        // For recursive procedures, it is vital that we forward declare the current procedure
        block = Block(true, procName);
//        if (block==nullptr)
  //          qDebug() << "Procedure decl: " << procName;
        //decl.append(procDecl);
        if (block!=nullptr)
            Eat(TokenType::SEMI);
        else {
            // Forward declared variables are used
            procDecl->m_isUsed = true;

        }

        procDecl->AppendBlock(block);
        //qDebug() <<procName;

        if (block!=nullptr) {
            bool ok = true;
             // Check if procedure already declared
            for (Node* n: m_proceduresOnly) {
                NodeProcedureDecl* proc =(NodeProcedureDecl*)n;
                if (proc->m_procName==procName) ok = false;

            }
            if (!ok)
                ErrorHandler::e.Error("Procedure '"+ procName +"' already defined", tok.m_lineNumber);

            m_proceduresOnly.append(procDecl);
        }

    }
//    }
   // qDebug() << "Finally:" << m_currentToken.getType();
    if (m_currentToken.m_type!=TokenType::BEGIN && isMain)
        ErrorHandler::e.Error("After declarations, BEGIN is expected", m_currentToken.m_lineNumber);

    return decl;
}

QVector<Node*> Parser::ConstDeclaration()
{
    Eat(TokenType::CONSTANT);
    QString name = m_currentToken.m_value;
    Eat();
    Eat(TokenType::COLON);
    QString type = "";
    if (m_currentToken.m_type == TokenType::ADDRESS)
        type="address";
    if (m_currentToken.m_type == TokenType::BYTE)
        type="byte";
    if (m_currentToken.m_type == TokenType::INTEGER)
        type="integer";
    if (type=="") {
        ErrorHandler::e.Error("Unknown or illegal type when defining constant of type: '"+m_currentToken.m_value+"' ("+m_currentToken.getType()+")<br>Allowed types are : <b>address, byte, integer.</b> ",m_currentToken.m_lineNumber);
    }
    Eat();
    Eat(TokenType::EQUALS);
    int value = GetParsedInt();

    m_symTab->m_constants[name.toUpper()] = new Symbol(name.toUpper(),type.toUpper(),value);
    return QVector<Node*>();
}



QVector<Node *> Parser::VariableDeclarations(QString blockName)
{
    if (blockName!="")
        m_symTab->SetCurrentProcedure(blockName+"_");
    else
        m_symTab->SetCurrentProcedure("");




    QVector<Node*> vars;
    vars.append(new NodeVar(m_currentToken));
    QString varName = m_currentToken.m_value;
    QVector<Symbol*> syms;
    syms.append(new Symbol(m_currentToken.m_value,""));
    m_symTab->Define(syms.last() ,false);
    Eat(TokenType::ID);



    // Make sure that ALL are defined!
    while (m_currentToken.m_type == TokenType::COMMA) {
        Eat(TokenType::COMMA);
        vars.append(new NodeVar(m_currentToken));
        AppendComment(vars[vars.count()-1]);

        syms.append(new Symbol(m_currentToken.m_value,""));
        m_symTab->Define(syms.last() ,false);

        Eat(TokenType::ID);
    }
    Eat(TokenType::COLON);

    NodeVarType* typeNode = dynamic_cast<NodeVarType*>(TypeSpec());
    // Set all types
    for (Symbol* s: syms) {
       s->m_type = typeNode->m_op.m_value;
       if (typeNode->m_data.count()!=0)
           s->m_size = typeNode->m_data.count();
       else {
           s->m_size = 1;
           if (s->m_type=="integer")
               s->m_size =2;
           if (s->m_type=="long")
               s->m_size =4;
       }
    }



/*    for (Node* v: vars) {
        Syntax::s.globals[((Var*)v)->value] = 0;
    }*/

    QVector<Node*> var_decleratons;

    for (Node* n : vars) {

        NodeVarDecl* decl = new NodeVarDecl(n, typeNode);
        var_decleratons.append(decl);
//        qDebug() <<  typeNode->m_op.getType() << typeNode->m_op.m_value;;
        if (typeNode->m_op.m_type == TokenType::INCSID) {
//            decl->m_isUsed = true;

            int sidloc = 0;
            if (m_preprocessorDefines.contains("SIDEmulatorLocation")) {
                sidloc = Util::NumberFromStringHex(m_preprocessorDefines["SIDEmulatorLocation"]);
                //qDebug() << sidloc;
            }
//            exit(1);
            decl->InitSid(m_currentDir, sidloc, "sid");
        }
        if (typeNode->m_op.m_type == TokenType::INCNSF) {
            int sidloc = 0;
 //           exit(1);
            decl->InitSid(m_currentDir, sidloc, "nsf");
        }
    }
//    return vars;
    return var_decleratons;
}



Node *Parser::TypeSpec()
{
    Token t = m_currentToken;

    if (m_currentToken.m_type == TokenType::INCBIN || m_currentToken.m_type == TokenType::INCSID || m_currentToken.m_type == TokenType::INCNSF) {
        Eat();
//        qDebug() << Node::m_staticBlockInfo.m_blockPos;
        Eat(TokenType::LPAREN);
        QString binFile = m_currentToken.m_value;
        Eat();
        QString position ="";
        if (m_currentToken.m_type==TokenType::COMMA) {
            Eat();
            position = Util::numToHex(GetParsedInt());
        }
        Eat(TokenType::RPAREN);


        QStringList flags;
        if (m_currentToken.m_type==TokenType::CHIPMEM) {
            Eat(TokenType::CHIPMEM);
            flags<<"chipmem";
        }

        NodeVarType *nt =  new NodeVarType(t,binFile, position);
        nt->m_flags = flags;
        return nt;


    }

    // Array [] of blah = (1,2,3);
    if (m_currentToken.m_type == TokenType::ARRAY) {
        Eat(TokenType::ARRAY);
        Eat(TokenType::LBRACKET);
        float count = 0;
        if (m_currentToken.m_type != TokenType::RBRACKET)
            count = GetParsedInt();

        Eat(TokenType::RBRACKET);
        Eat(TokenType::OF);
        Token arrayType = m_currentToken;
        Eat(m_currentToken.m_type);
        QStringList data;
        // Contains constant init?
        if (m_currentToken.m_type==TokenType::EQUALS) {
            Eat();
            Eat(TokenType::LPAREN);
            while (m_currentToken.m_type!=TokenType::RPAREN) {
                bool found = false;
                // First check if symbol exists:
                if (m_currentToken.m_value!="") {
                    if (m_symTab->m_symbols.contains(m_currentToken.m_value) && (m_symTab->LookupConstants(m_currentToken.m_value.toUpper())==nullptr)) {
                        data<<m_currentToken.m_value;
                        found = true;
                    }
//                    qDebug() << "ADDRESS " << m_currentToken.m_value <<m_symTab->LookupConstants(m_currentToken.m_value.toUpper());
                 }
                if (!found)
                    data << "$0"+QString::number(GetParsedInt(),16);//QString::number(m_currentToken.m_intVal);
                //data << "$0"+QString::number(GetParsedInt(),16);//QString::number(m_currentToken.m_intVal);
                if (m_currentToken.m_type!=TokenType::RPAREN) {

                    Eat();
                    if (m_currentToken.m_type==TokenType::COMMA)
                        Eat();
                }
            }
            Eat(TokenType::RPAREN);
            if (count!=data.count() && count!=0) {
                if (count>data.count()) {
                    ErrorHandler::e.Warning("Declared array count ("+QString::number((int)count)+") does not match with data ("+QString::number(data.count())+"). Padding with zeros. ", m_currentToken.m_lineNumber);
                    for (int i=data.count();i<count;i++)
                        data.append(QString("0"));
                }
                else
                    ErrorHandler::e.Warning("Declared array count ("+QString::number((int)count)+") does not match with data ("+QString::number(data.count())+"). Adjusting array size to fit data. ", m_currentToken.m_lineNumber);

            }

        }

        QString position = "";
        if (m_currentToken.m_type==TokenType::AT || m_currentToken.m_type==TokenType::ABSOLUT) {
            Eat();
            //position = m_currentToken.getNumAsHexString();
            position = Util::numToHex(GetParsedInt());
           // Eat(m_currentToken.m_type);
        }
        QStringList flags;
        if (m_currentToken.m_type==TokenType::CHIPMEM) {
            Eat(TokenType::CHIPMEM);
            flags<<"chipmem";
        }

        t.m_intVal = count;
//        qDebug() << "Type: " << t.m_value;
  //      t.m_type = arrayType.m_type;

        NodeVarType *nt =  new NodeVarType(t,position, arrayType,data);
        nt->m_flags = flags;
        return nt;

    }

    if (m_currentToken.m_type == TokenType::STRING || m_currentToken.m_type==TokenType::CSTRING) {
        Eat();
        QStringList initData;
        if (m_currentToken.m_type == TokenType::EQUALS) {
            Eat();
            NodeString* str = (NodeString*)String();
            initData = str->m_val;
        }
        return new NodeVarType(t,initData);
    }

    if (m_currentToken.m_type == TokenType::POINTER) {
        Eat();
        QString type;
        NodeVarType* nvt = new NodeVarType(t,"");
        nvt->m_arrayVarType.m_type = TokenType::BYTE;

        if (m_currentToken.m_type == TokenType::OF) {
            Eat();
            TokenType::Type typ = m_currentToken.m_type;

//            NodeString* str = (NodeString*)String();
  //          initData = str->m_val;
            Eat();
            nvt->m_arrayVarType.m_type = typ;
        }

//        qDebug() <<"Parser typespec pointer: "  << nvt->m_arrayVarType.getType();

        return nvt;
    }


    Eat();
    // Is regular single byte / pointer

    QString position = "";
    if (m_currentToken.m_type==TokenType::AT || m_currentToken.m_type==TokenType::ABSOLUT) {
        Eat();
        position = m_currentToken.getNumAsHexString();

        Eat(m_currentToken.m_type);
        NodeVarType* nt = new NodeVarType(t,position);
        nt->m_position = position;
        nt->m_flag = 1;
        return nt;
    }


    QString initVal = "";
    if (m_currentToken.m_type == TokenType::EQUALS) {
        Eat();
//        data << "$0"+QString::number(getIntVal(m_currentToken),16);//QString::number(m_currentToken.m_intVal);

        initVal = Util::numToHex(GetParsedInt());



    }

    return new NodeVarType(t,initVal);

}

Node *Parser::BuiltinFunction()
{
    if (Syntax::s.builtInFunctions.contains(m_currentToken.m_value.toLower())) {
        QString procName = m_currentToken.m_value.toLower();
        int noParams = Syntax::s.builtInFunctions[procName].m_params.count();
        Eat(TokenType::ID);
        Eat(TokenType::LPAREN);
        QVector<Node*> paramList;
        QString prev;

        while (m_currentToken.m_type!=TokenType::RPAREN) {
            if (m_currentToken.m_type==TokenType::STRING) {
                paramList.append( String() );
            }
            else {
                QString pname = m_currentToken.m_value;
                try {
                    paramList.append(Expr());
                } catch (FatalErrorException fe) {
                    QString em = "Could not find symbol '<font color=\"#FF8000\">" + prev + "</font>'<br>";
                    QString similarSymbol = m_symTab->findSimilarSymbol(prev,65,2,m_procedures.keys());
                    if (similarSymbol!="") {
                        em+="Did you mean '<font color=\"#A080FF\">"+similarSymbol+"</font>'?<br>";
                    }
                    fe.message = em + fe.message;

                    throw fe;
                }
                prev = pname;
            }

            if (m_currentToken.m_type==TokenType::COMMA)
                Eat(TokenType::COMMA);
        }

        Eat(TokenType::RPAREN);

        //qDebug() << "Params count for " << procName << " :" << paramList.count();
        if (noParams!=paramList.count()) {
            QString s = "Error using builtin function " + procName + " \n";
            s += "Requires " + QString::number(noParams) + " parameters but has " + QString::number(paramList.count());

            ErrorHandler::e.Error(s, m_currentToken.m_lineNumber);
        }

        // Give obsolete warnings
        for (QStringList& lst: m_obsoleteWarnings) {
                if(lst[0].toLower() == procName.toLower()) {
                    if (!m_warningsGiven.contains(procName)) {
                      m_warningsGiven.append(procName);
                      ErrorHandler::e.Warning(lst[1], m_currentToken.m_lineNumber);
                     }
                 }
            }

        return new NodeBuiltinMethod(procName,paramList,&Syntax::s.builtInFunctions[procName]);
        //p->SetParameters(paramList);


    }
    return nullptr;

}

Node *Parser::Constant()
{
    QString id = m_currentToken.m_value;
    if (SymbolTable::m_constants.contains(id)) {
        Eat(m_currentToken.m_type);
        Symbol* s = SymbolTable::m_constants[id];
        Node* n =  new NodeNumber(Token(s->getTokenType(), s->m_value->m_fVal), s->m_value->m_fVal);
        return n;
    }
    return nullptr;
}

Node *Parser::InlineAssembler()
{
    Eat(TokenType::ASM);
    Eat(TokenType::LPAREN);
    if (m_currentToken.m_type!=TokenType::STRING)
        ErrorHandler::e.Error("Inline assembler must be enclosed as a string");
    Node* n = new NodeAsm(m_currentToken);
    Eat(TokenType::STRING);
    Eat(TokenType::RPAREN);
    return n;
}

void Parser::HandleImportChar()
{
    int ln = m_currentToken.m_lineNumber;
    QString inFile = m_currentDir+"/"+ m_currentToken.m_value;
    Eat(TokenType::STRING);
    QString outFile =m_currentDir+"/"+ m_currentToken.m_value;
    Eat(TokenType::STRING);
    int param1 = m_currentToken.m_intVal;
    Eat(TokenType::INTEGER_CONST);
    int param2 = m_currentToken.m_intVal;
    Eat(TokenType::INTEGER_CONST);

    LImage* imgB = LImageIO::Load(outFile);

    LImage* imgA = nullptr;
    if (inFile.toLower().endsWith(".bin") || inFile.toLower().endsWith(".chr")) {
        imgA = LImageFactory::Create(imgB);
        QFile f(inFile);
        f.open(QFile::ReadOnly);
        imgA->ImportBin(f);
        f.close();
    }
    if (inFile.toLower().endsWith(".flf"))
      imgA = LImageIO::Load(inFile);

    if (imgA == nullptr) {
        ErrorHandler::e.Error("Importing char error : unknown filetype for '"+inFile +"'");
    }

//    qDebug() << "HERE" << param1 << param2;
    imgB->CopySingleChar(imgA, param1, param2);

    LImageIO::Save(outFile,imgB);


}

void Parser::HandleExport()
{
    int ln = m_currentToken.m_lineNumber;
    QString inFile = m_currentDir+"/"+ m_currentToken.m_value;
    Eat(TokenType::STRING);
    QString outFile =m_currentDir+"/"+ m_currentToken.m_value;
    Eat(TokenType::STRING);



    int param1 = m_currentToken.m_intVal;
    Eat(TokenType::INTEGER_CONST);
    int param2 = 0;
    if (m_currentToken.m_type==TokenType::INTEGER_CONST) {
        param2 = m_currentToken.m_intVal;
        Eat(TokenType::INTEGER_CONST);

    }
    else {
        param2 = param1;
        param1 = 0;
    }


    if (!QFile::exists(inFile)) {
        ErrorHandler::e.Error("File not found : "+inFile,ln);
    }
    LImage* img = LImageIO::Load(inFile);
    if (dynamic_cast<CharsetImage*>(img)!=nullptr) {
        img->m_exportParams["Start"] = param1;
        img->m_exportParams["End"] = param2;
    }
    if (QFile::exists(outFile))
        QFile::remove(outFile);



    QFile file(outFile);

    file.open(QFile::WriteOnly);
    img->m_silentExport = true;
    if (dynamic_cast<C64FullScreenChar*>(img)!=nullptr) {
        C64FullScreenChar* c = dynamic_cast<C64FullScreenChar*>(img);
        c->ExportMovie(file);
    }
    else
        img->ExportBin(file);




    file.close();

}

void Parser::HandleVBMExport()
{
    int ln = m_currentToken.m_lineNumber;
    QString inFile = m_currentDir+"/"+ m_currentToken.m_value;
    Eat(TokenType::STRING);
    QString outFile =m_currentDir+"/"+ m_currentToken.m_value;
    Eat(TokenType::STRING);
    int param1 = m_currentToken.m_intVal;
    Eat(TokenType::INTEGER_CONST);
    int param2 = m_currentToken.m_intVal;
    Eat(TokenType::INTEGER_CONST);
    int param3 = m_currentToken.m_intVal;
    Eat(TokenType::INTEGER_CONST);
    int param4 = m_currentToken.m_intVal;

    if (!QFile::exists(inFile)) {
        ErrorHandler::e.Error("File not found : "+inFile,ln);
    }
    LImage* img = LImageIO::Load(inFile);
    if (QFile::exists(outFile))
        QFile::remove(outFile);

    QFile file(outFile);

    file.open(QFile::WriteOnly);
    img->m_silentExport = true;

    img->VBMExport(file,param1,param2,param3,param4);




    file.close();

}

void Parser::HandleSpriteCompiler()
{

    QString filename = m_currentToken.m_value;
    Eat(TokenType::STRING); // Filename
    QString name = m_currentToken.m_value;
    Eat(TokenType::STRING); // Name
    int x = m_currentToken.m_intVal;
    Eat(TokenType::INTEGER_CONST); // X
    int y = m_currentToken.m_intVal;
    Eat(TokenType::INTEGER_CONST); // Y
    int w = m_currentToken.m_intVal;
    Eat(TokenType::INTEGER_CONST); // W
    int h = m_currentToken.m_intVal;
    Eat(TokenType::INTEGER_CONST); // H

    LImage* img = LImageIO::Load(m_currentDir +"/"+filename);
    m_parserAppendix << img->SpriteCompiler(name,"","",x,y,w,h);



    QString id = "drawsprite_cga_"+name;
    QList<BuiltInFunction::Type> paramList;
    paramList<<BuiltInFunction::ADDRESS;
    paramList<<BuiltInFunction::ADDRESS;
    paramList<<BuiltInFunction::INTEGER;
    paramList<<BuiltInFunction::INTEGER;

    Syntax::s.builtInFunctions[id] = BuiltInFunction(id, paramList);


    delete img;
}

Node* Parser::Expr()
{
    Node* node = Term();




    while (m_currentToken.m_type == TokenType::Type::PLUS || m_currentToken.m_type == TokenType::Type::MINUS
            || m_currentToken.m_type == TokenType::Type::BITAND || m_currentToken.m_type == TokenType::Type::BITOR) {
        Token t = m_currentToken;

        Eat(m_currentToken.m_type);

        node = new NodeBinOP(node, t, Term());

    }
    return node;
}

