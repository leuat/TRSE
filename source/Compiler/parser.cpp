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

QStringList Parser::s_usedTRUs;
QStringList Parser::s_usedTRUNames;
QVector<QSharedPointer<Parser>> Parser::m_tpus;


QString Parser::WashVariableName(QString v)
{
  //  QString p =m_symTab->m_gPrefix;
//    p = p.replace("_","::");
    v = v.replace(m_symTab->m_gPrefix,"");
    return v;

}

QStringList Parser::getFlags() {
    QStringList flags;


    bool done = false;

    while (!done)  {
        done = true;
        if (m_currentToken.m_type==TokenType::CHIPMEM) {
            Eat(TokenType::CHIPMEM);
            flags<<"chipmem";
            done = false;
        }

        if (m_currentToken.m_type==TokenType::WRAM) {
            Eat(TokenType::WRAM);
            flags<<"wram";
            done = false;
        }

        if (m_currentToken.m_type==TokenType::SPRRAM) {
            Eat(TokenType::SPRRAM);
            flags<<"sprram";
            done = false;
        }
        if (m_currentToken.m_type==TokenType::HRAM) {
            Eat(TokenType::HRAM);
            done = false;
            flags<<"hram";
        }

        if (m_currentToken.m_type==TokenType::ALIGNED) {
            Eat(TokenType::ALIGNED);
            done = false;
            flags<<"aligned";
        }
        if (m_currentToken.m_type==TokenType::BANK) {
            Eat(TokenType::BANK);
            done = false;
            Eat(TokenType::LPAREN);
            flags<<"bank"<<Util::numToHex(GetParsedInt(TokenType::INTEGER_CONST));
            Eat(TokenType::RPAREN);

        }
    }
    return flags;

}

Parser::Parser()
{
}

void Parser::Delete()
{
    for (QString val : m_procedures.keys()) {
        QSharedPointer<Node> s = m_procedures[val];
        // if (s!=nullptr) {
        //s->Delete();
        //            delete s;
        //        }
    }
    m_procedures.clear();
    //delete m_symTab;
    //m_symTab = nullptr;
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
    if (m_tick++%500==99)
        emit EmitTick(".");
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

        // must now call manually to set up for screen mode
        //InitBuiltinFunction(QStringList()<< "vbmscreenshiftleft(", "initVbmScreenShiftLeft");
        //InitBuiltinFunction(QStringList()<< "vbmscreenshiftright(", "initVbmScreenShiftRight");

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
        InitBuiltinFunction(QStringList()<< "vbmdrawsmallbcd(", "initVbmDrawSmallBCD");
        InitBuiltinFunction(QStringList()<< "vbmdrawsmallbcdo(", "initVbmDrawSmallBCDO");

        InitBuiltinFunction(QStringList()<< "vbmcopytobuffer(", "initVbmCopyToBuffer");
        InitBuiltinFunction(QStringList()<< "vbmcopyfrombuffer(", "initVbmCopyFromBuffer");


    }
    Node::m_staticBlockInfo.m_blockID = -1;
    Node::m_staticBlockInfo.m_blockPos = "";
    Node::m_staticBlockInfo.m_blockName = "";
}

void Parser::VerifyInlineSymbols6502(QString s)
{
    QStringList lst = s.split("\n");
    for (QString l : lst) {
        QStringList l2 = l.trimmed().simplified().split(" ");
        if (l2.count()>1) {
            QString s2 = l2[1];
            s2 = s2.remove("(").remove(")").remove(",x").remove(",y");
            if (m_symTab->m_symbols.contains(s2))
                m_symTab->Lookup(s2,m_currentToken.m_lineNumber);
            if (m_procedures.contains(s2))
                m_procedures[s2]->m_isUsed = true;
        }
    }
}

void Parser::InitBuiltinFunction(QStringList methodName, QString builtinFunctionName, QString initJump )
{
    if (m_ignoreMethods.contains(builtinFunctionName.toLower())) {
            return;
    }
    QString txt = m_lexer->m_text.toLower();
    for (QString s: methodName)
     if (txt.contains(s)) {
         //if (m_procedures.contains(builtinFunctionName))
             //m_ignoreBuiltinFunctionTPU.append(builtinFunctionName);
         //qDebug() << "ALRADY COMTAINS " << builtinFunctionName;
//         Node::m_staticBlockInfo.m_blockID=-1;
//         qDebug() <<m_ignoreBuiltinFunctionTPU;
         /*if (!m_ignoreBuiltinFunctionTPU.contains(builtinFunctionName))
            m_procedures[builtinFunctionName] = QSharedPointer<NodeProcedureDecl>(new NodeProcedureDecl(Token(TokenType::PROCEDURE, builtinFunctionName), builtinFunctionName));
         else {
             m_procedures.remove(builtinFunctionName);
            qDebug() << "IGNORING CREATING NEW " <<builtinFunctionName;
         }*/
         m_procedures[builtinFunctionName] = QSharedPointer<NodeProcedureDecl>(new NodeProcedureDecl(Token(TokenType::PROCEDURE, builtinFunctionName), builtinFunctionName));
         m_ignoreBuiltinFunctionTPU.append(builtinFunctionName);
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

void Parser::ApplyTPUBefore()
{
    for (QSharedPointer<Parser> p: m_tpus) {
//        QSharedPointer<NodeProgram> np = qSharedPointerDynamicCast<NodeProgram>(p->m_tree);
        for (QString k : p->m_procedures.keys()) {
            m_procedures[k] = p->m_procedures[k];
//            m_symTab->m_globalList.append(k);
//            qDebug() << "Adding TPU : " << k;
        }
//        for (QString k : p->m_procedures.keys())
  //          m_procedures[k] = p->m_procedures[k];

    }
}

void Parser::ApplyTPUAfter(QVector<QSharedPointer<Node>>& declBlock, QVector<QSharedPointer<Node>>& procs)
{
    for (QSharedPointer<Parser> p: m_tpus) {
        if (p->m_hasBeenApplied) {
            continue;
        }

        p->m_hasBeenApplied = true;

        QVector<QSharedPointer<Node>> orgProcs = procs;
        procs.clear();

        QSharedPointer<NodeProgram> np = qSharedPointerDynamicCast<NodeProgram>(p->m_tree);
        QVector<QSharedPointer<Node>> decls;

        for (QSharedPointer<Node> on : np->m_NodeBlock->m_decl) {
            QSharedPointer<NodeProcedureDecl> procDecl =
            qSharedPointerDynamicCast<NodeProcedureDecl>(on);
            if (procDecl!=nullptr) {
                if (!m_ignoreBuiltinFunctionTPU.contains(procDecl->m_procName)) {
                    procs.append(procDecl);
//                    qDebug() << "Appending : " << procDecl->m_procName << procDecl->m_isUsed;
                    m_mergedProcedures.append(procDecl);
                }

            }
            else decls.append(on);


        }

        // Important: add newest procedures *last*
        procs.append(orgProcs);

        QVector<QSharedPointer<Node>> copy;
        for (auto p: declBlock)
            copy.append(p);

        declBlock.clear();
        // Make sure that TRU variables are declared FIRST. Ordering is important for the GB
        declBlock.append(decls);
        declBlock.append(copy);
    }
}

int Parser::getParsedNumberOrConstant() {

    if (m_currentToken.m_value=="") {
        return m_currentToken.m_intVal;
    }
    QSharedPointer<Symbol> s = m_symTab->LookupConstants(m_currentToken.m_value.toUpper());
    if (s==nullptr)
        ErrorHandler::e.Error("Value required to be a number or a constant.",m_currentToken.m_lineNumber);

    return s->m_value->m_fVal;
}

int Parser::GetParsedInt(TokenType::Type forceType) {


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
            QSharedPointer<Symbol> s = m_symTab->LookupConstants(m_currentToken.m_value.toUpper());
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
//   qDebug() << str << ret.toInt();
    int r = ret.toInt();
    if (forceType==TokenType::ADDRESS && Syntax::s.m_currentSystem->m_system == AbstractSystem::OK64)
        return r;
    if (forceType==TokenType::BYTE)
        return r&0xFF;
    if (forceType==TokenType::INTEGER || forceType==TokenType::INTEGER_CONST || forceType==TokenType::ADDRESS)
        return r&0xFFFF;

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
        QSharedPointer<Symbol> s = m_symTab->Lookup(t.m_value,t.m_lineNumber);
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

void Parser::VerifyTypeSpec(Token t)
{
    try {
        bool ok = false;
        if (m_symTab->m_records.contains(t.m_value))
            ok=true;
        //return;
        if (!ok)
            m_symTab->Lookup(t.m_value,t.m_lineNumber);

    } catch (FatalErrorException fe) {
        fe.message = "Unknown type specification : " +t.m_value;
        throw fe;
    }
}

void Parser::RemoveUnusedProcedures()
{
    QString removeProcedures = "Removing unused procedures: ";
    bool outputUnusedWarning = false;
    QVector<QSharedPointer<Node>> procs;

/*    m_proceduresOnly.append(m_mergedProcedures);
    for (QSharedPointer<Node> n:m_proceduresOnly) {
        qDebug() << qSharedPointerDynamicCast<NodeProcedureDecl>(n)->m_procName;
    }*/
    for (QSharedPointer<Node> n: m_proceduresOnly) {
        QSharedPointer<NodeProcedureDecl> np = qSharedPointerDynamicCast<NodeProcedureDecl>(n);

        if ((np->m_isUsed==true) || m_doNotRemoveMethods.contains(np->m_procName))
            procs.append(n);
        else {
            outputUnusedWarning = true;
            removeProcedures+=np->m_procName + ",";
//            if (m_procedures.contains(np->m_procName))
  //              m_procedures.remove(np->m_procName);
        }
    }

    m_proceduresOnly = procs;

    if (outputUnusedWarning) {
        removeProcedures.remove(removeProcedures.count()-1,1);
        ErrorHandler::e.Warning(removeProcedures);
    }


}

void Parser::RemoveUnusedSymbols(QSharedPointer<NodeProgram> root)
{
    QVector<QSharedPointer<Node>> newDecl;
    QStringList removedSymbols;
    for (QSharedPointer<Node> n: root->m_NodeBlock->m_decl) {
        bool add = true;

        QSharedPointer<NodeVarDecl> nv = qSharedPointerDynamicCast<NodeVarDecl>(n);
        if (nv!=nullptr) {
            QString val = qSharedPointerDynamicCast<NodeVar>(nv->m_varNode)->value;

            if (m_symTab->m_symbols.contains(val) && !m_doNotRemoveMethods.contains(val)) {

                QSharedPointer<Symbol> s = m_symTab->m_symbols[val];//m_symTab->Lookup(,0);
//                qDebug() << s->m_type;
                if (!s->isUsed && !(s->m_type=="INCBIN" || s->m_type=="INCSID")) {
                    removedSymbols.append(val);
                    m_symTab->m_symbols.remove(val);
//                    qDebug() << s->m_name << s->isUsed;
                    add = false;
                }

            }
            else add=true;
        }
        if (add)
            newDecl.append(n);
    }
    root->m_NodeBlock->m_decl = newDecl;
    if (removedSymbols.count()!=0) {
        QString s = "Removed the following unused symbols: ";
        for (int i=0;i<removedSymbols.count();i++) {
            s = s + removedSymbols[i];
            if (i!=removedSymbols.count()-1)
                s=s+", ";
        }
        ErrorHandler::e.Warning(s);
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
    if (m_currentToken.m_value=="deletefile") {
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
    if (m_currentToken.m_value=="exportrgb8palette") {
        Eat();
        Eat();
        Eat();
        return;
    }
    if (m_currentToken.m_value=="exportprg2bin") {
        Eat();
        Eat();
        Eat();
        Eat();
        Eat();
        return;
    }
    if (m_currentToken.m_value=="execute") {
        Eat();
        Eat();
        return;
    }
    if (m_currentToken.m_value=="spritepacker") {
        Eat();
        Eat(TokenType::STRING);
        Eat(TokenType::STRING);
        Eat(TokenType::STRING);
        Eat(TokenType::STRING);
        Eat();
        Eat();
        Eat();
        Eat();
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
    if (m_currentToken.m_value=="vbmexportchunk") {
        Eat();
        Eat(TokenType::STRING);
        Eat(TokenType::STRING);
        Eat(TokenType::INTEGER_CONST);
        Eat(TokenType::INTEGER_CONST);
        Eat(TokenType::INTEGER_CONST);
        Eat(TokenType::INTEGER_CONST);
        return;
    }
    if (m_currentToken.m_value=="ignoresystemheaders") {
        Eat();
    }

    if (m_currentToken.m_value=="exportframe") {
        Eat();
        Eat(TokenType::STRING);
        Eat(TokenType::STRING);
        Eat(TokenType::INTEGER_CONST);
        Eat(TokenType::INTEGER_CONST);
        Eat(TokenType::INTEGER_CONST);
        Eat(TokenType::INTEGER_CONST);
        Eat(TokenType::INTEGER_CONST);
        Eat(TokenType::INTEGER_CONST);
        Eat(TokenType::INTEGER_CONST);
        Eat(TokenType::INTEGER_CONST);
        return;
    }


    if (m_currentToken.m_value=="donotremove") {
        Eat();
        m_doNotRemoveMethods.append(m_symTab->m_gPrefix+m_currentToken.m_value);
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
    if (m_currentToken.m_value=="projectsettings") {
        Eat();
        Eat();
        Eat();
        return;
    }
    if (m_currentToken.m_value=="buildpaw") {
        Eat();
        Eat();
        return;
    }
    if (m_currentToken.m_value=="startassembler") {
        Eat();
        Eat();
        return;
    }
    if (m_currentToken.m_value=="use") {
        Eat();
        Eat();
//        Eat();
//        Eat();
        return;
    }


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
    m_lexer->m_text = m_lexer->m_text.replace("sta(", "sta (");
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



QSharedPointer<Node> Parser::Variable()
{
    QSharedPointer<Node> n = nullptr;


    bool isConstant = false;
    if (m_symTab->m_constants.contains(m_currentToken.m_value.toUpper())) {
        QSharedPointer<Symbol> s = m_symTab->m_constants[m_currentToken.m_value.toUpper()];
        isConstant=true;

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
        QSharedPointer<Node> expr = nullptr;
        if (m_currentToken.m_type==TokenType::LBRACKET) {
            Eat(TokenType::LBRACKET);
            expr = Expr();
            Eat(TokenType::RBRACKET);
         }

        QSharedPointer<Node> subVar = nullptr;
        if (m_currentToken.m_type==TokenType::DOT) {
            Eat();
            subVar = Variable();
        }

        if (t.m_type==TokenType::ADDRESS && expr!=nullptr) {
            t.m_value = "$"+QString::number( (int)s->m_value->m_fVal,16);
            QSharedPointer<NodeVar> nv = QSharedPointer<NodeVar>(new NodeVar(t,expr));
            nv->m_subNode = subVar;
            //nv->m_expr = expr;
            n=nv;
        }
        else {
            n = QSharedPointer<NodeNumber>(new NodeNumber(t, s->m_value->m_fVal));

            //qDebug()  << s->m_value->m_fVal;
        }


    }
    else {

        Token t = m_currentToken;
        if (m_currentToken.m_type==TokenType::STRING || m_currentToken.m_type==TokenType::CSTRING) {
           n = String();
           return n;

        }
        // Make sure that prefixes are added
//        qDebug() << "PREFIX " << m_symTab->m_globalList.contains(t.m_value) << t.m_value <<  m_symTab->m_gPrefix+t.m_value ;
  //     qDebug() << m_symTab->m_globalList;

        if (!m_symTab->m_globalList.contains(t.m_value))
            t.m_value = m_symTab->m_gPrefix+t.m_value;

//        qDebug() << t.m_value;
        Eat(m_currentToken.m_type);

        if (m_currentToken.m_type!=TokenType::LBRACKET) {
            if (t.m_value.endsWith("^")) {
                t.m_value.replace("^","");
  //              qDebug() << t.m_value;
//                exit(1);
                t.m_type = TokenType::ADDRESS;
            }
            QSharedPointer<Node> subVar = nullptr;
            if (m_currentToken.m_type==TokenType::DOT) {
                Eat();
                subVar = Variable();
            }

            n = QSharedPointer<NodeVar>(new NodeVar(t));
            qSharedPointerDynamicCast<NodeVar>(n)->m_subNode = subVar;
        }
        else
            {
                Eat(TokenType::LBRACKET);
                QSharedPointer<Node> expr = Expr();
                Eat(TokenType::RBRACKET);
                QSharedPointer<Node> subVar = nullptr;
                if (m_currentToken.m_type==TokenType::DOT) {
                    Eat();
                    subVar = Variable();
                }


                n = QSharedPointer<NodeVar>(new NodeVar(t, expr));
                qSharedPointerDynamicCast<NodeVar>(n)->m_subNode = subVar;


        }

    }
    if (n==nullptr) {
        ErrorHandler::e.Error("Could not assign variable!");
    }
/*    QSharedPointer<NodeVar> nv = qSharedPointerDynamicCast<NodeVar>n;
    if (nv->m_subNode!=nullptr && (qSharedPointerDynamicCast<NodeVar>nv->m_subNode)->m_expr!=nullptr)
        nv->m_expr = (qSharedPointerDynamicCast<NodeVar>nv->m_subNode)->m_expr;
*/
    // Verify that we're not trying to screw with the variable

    QSharedPointer<NodeVar> nv = qSharedPointerDynamicCast<NodeVar>(n);
    if (nv!=nullptr) {
        QSharedPointer<Symbol> s = m_symTab->Lookup(nv->value,m_currentToken.m_lineNumber);
        // If variable doesn't exist
//        qDebug
        if (s==nullptr) {
            ErrorHandler::e.Error("Could not find variable : " +nv->value);
        }
//        qDebug() << nv->value<<s->m_type;
        if (!(s->m_type.toUpper()=="ARRAY" || s->m_type.toUpper()=="POINTER" || s->m_type.toUpper()=="STRING"  ||s->m_type.toUpper()=="CSTRING" ||s->m_type.toUpper()=="INCBIN" || s->m_type.toUpper()=="ADDRESS" || isConstant) && nv->m_expr!=nullptr)
            ErrorHandler::e.Error("Variable '<b>" +nv->value + "</b>' is neither a pointer nor an array.",nv->m_op.m_lineNumber);

    }

    return n;
}

QSharedPointer<Node> Parser::Empty()
{
    return QSharedPointer<Node>(new NoOp());
}

void Parser::Record(QString name)
{
    Eat(TokenType::RECORD);
//    SymbolTable
    QSharedPointer<SymbolTable>  record = QSharedPointer<SymbolTable> (new SymbolTable());
    record->m_symbols.clear();
    m_symTab->m_records[name] = record;
    //m_symTab->Define(new Symbol(name,"record"));
    record->setName(name);

    while (m_currentToken.m_type!=TokenType::END) {
        QVector<QSharedPointer<Node>> vars = VariableDeclarations("");
        for (QSharedPointer<Node> n : vars) {
            QSharedPointer<NodeVarDecl> nv = qSharedPointerDynamicCast<NodeVarDecl>(n);
            QSharedPointer<NodeVarType> typ = qSharedPointerDynamicCast<NodeVarType>(nv->m_typeNode);
            QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(nv->m_varNode);
            QSharedPointer<Symbol> s = QSharedPointer<Symbol>(new Symbol(var->value, typ->value));
            s->m_arrayType = typ->m_arrayVarType.m_type;
            s->m_arrayTypeText = typ->m_arrayVarTypeText;
            record->Define(s);
        }
        Eat(TokenType::SEMI);

    }
    Eat(TokenType::END);
//    Eat(TokenType::SEMI);
}


QSharedPointer<Node> Parser::AssignStatement()
{
    QSharedPointer<Node> arrayIndex = nullptr;
    Token t = m_currentToken;
    QSharedPointer<Node> left = Variable();
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
    QSharedPointer<Node> right = Expr();
    QSharedPointer<NodeAssign>na = QSharedPointer<NodeAssign>(new NodeAssign(left, t, right));
    na->m_right->ApplyFlags();// make sure integer:=byte*byte works
    return na;


}

QSharedPointer<Node> Parser::Statement()
{
    QSharedPointer<Node> node = nullptr;
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

QSharedPointer<Node> Parser::Case()
{
    QSharedPointer<NodeCase> n = QSharedPointer<NodeCase>(new NodeCase(m_currentToken));
    Eat(); // Eat "case"
    n->m_variable = qSharedPointerDynamicCast<NodeVar>(Variable());
    Eat(TokenType::OF);
    while (m_currentToken.m_type != TokenType::END && m_currentToken.m_type != TokenType::ELSE) {
        QSharedPointer<Node> expr = Expr();
        Eat(TokenType::COLON);
        QSharedPointer<Node> block = Block(false);
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

QSharedPointer<Node> Parser::BinaryClause()
{
    if (m_currentToken.m_type == TokenType::LPAREN) {
        // Logical clause AND OR
        Eat(TokenType::LPAREN);
        QSharedPointer<Node> a = BinaryClause();
        if (m_currentToken.m_type==TokenType::RPAREN) {
            Eat();
            return a;
        }
        Token logical = m_currentToken;
        Eat();
        QSharedPointer<Node> b = BinaryClause();
        Eat(TokenType::RPAREN);
        return QSharedPointer<NodeBinaryClause>(new NodeBinaryClause(logical, a, b));
    }

    QSharedPointer<Node> a = Expr();
    Token comparetoken;
    QSharedPointer<Node> b;
    // Nothing : the null test. Check if NOT EQUALS ZERO
    if (m_currentToken.m_type==TokenType::RPAREN || m_currentToken.m_type==TokenType::THEN) {
        Token t;
        t.m_type = TokenType::BYTE;
        t.m_intVal = 0;
        b = QSharedPointer<NodeNumber>(new NodeNumber(t,0));
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
    return QSharedPointer<NodeBinaryClause>(new NodeBinaryClause(comparetoken, a, b));
}


void Parser::AppendComment(QSharedPointer<Node> n)
{
    if (m_lexer->m_currentComment=="")
        return;
    if (n==nullptr)
        return;
    n->m_comment = m_lexer->m_currentComment;
    m_lexer->m_currentComment="";
}



QSharedPointer<Node> Parser::Conditional(bool isWhileLoop)
{

    QVector<QSharedPointer<Node>> left, right;
    QVector<Token> compareTokens, conditionals;

    // Start
    Token t = m_currentToken;
    bool done=false;
    int linenum = m_currentToken.m_lineNumber;

    QSharedPointer<Node> clause = BinaryClause();

    int forcePage = findPage();

    if (m_currentToken.m_type==TokenType::THEN || m_currentToken.m_type==TokenType::DO)
        Eat(m_currentToken.m_type);
    else {
        ErrorHandler::e.Error("Expected THEN or DO after conditional", linenum);
    }

    QSharedPointer<Node> block = Block(false);

    QSharedPointer<Node> nodeElse = nullptr;
    if (m_currentToken.m_type==TokenType::ELSE) {
        Eat(TokenType::ELSE);
        nodeElse = Block(false);
    }

    return QSharedPointer<NodeConditional>(new NodeConditional(t, forcePage, clause, block, isWhileLoop, nodeElse));
}


QVector<QSharedPointer<Node>> Parser::StatementList()
{

    QSharedPointer<Node> node = Statement();
    QVector<QSharedPointer<Node>> results;
    results.append(node);
    while (m_currentToken.m_type == TokenType::SEMI) {
        Eat(TokenType::SEMI);
        QSharedPointer<Node> n = Statement();

        results.append(n);

    }
    if (m_currentToken.m_type==TokenType::ID)
//        ErrorHandler::e.Error("Parser::Statementlist SYNTAX ERROR : Token should not be ID", m_currentToken.m_lineNumber);
    ErrorHandler::e.Error("Did you forget a semicolon? (Token should not be ID in Parser)", m_currentToken.m_lineNumber);


    return results;

}

QSharedPointer<Node> Parser::CompoundStatement()
{
    if (m_currentToken.m_type!=TokenType::BEGIN) {
        // Single statement
        QSharedPointer<Node> n =  Statement();
  //      Eat(TokenType::SEMI);
    //    qDebug() << m_currentToken.getType();
      //  qDebug() << m_currentToken.m_value;

        return n;
    }
    Token t = m_currentToken;
    Eat(TokenType::BEGIN);
    QVector<QSharedPointer<Node>> nodes = StatementList();
    Eat(TokenType::END);
    QSharedPointer<NodeCompound> root = QSharedPointer<NodeCompound>(new NodeCompound(t));
    for (QSharedPointer<Node> n: nodes)
        root->children.append(n);

//    qDebug() << "from begin block : " +m_currentToken.getType();
    return root;

}

QSharedPointer<Node> Parser::Program(QString param)
{
//    QSharedPointer<Node> n = CompoundStatement();
    if (!m_isTRU)
        Eat(TokenType::PROGRAM);
    else
        Eat(TokenType::UNIT);

//    QSharedPointer<NodeVar> varNode = qSharedPointerDynamicCast<NodeVar>Variable();
    QString progName = m_currentToken.m_value;// varNode->value;
    // Prefix all TRUs with the name

    Eat();
    m_symTab->Define(QSharedPointer<Symbol>(new Symbol(progName,"STRING")));
    Eat(TokenType::SEMI);
    QSharedPointer<NodeBlock> block;

    if (m_isTRU) {
        m_symTab->m_gPrefix = progName+"_";
        if (s_usedTRUNames.contains(progName))
            ErrorHandler::e.Error("TRU '"+progName+"' is already defined! ",m_currentToken.m_lineNumber);
        s_usedTRUNames.append(progName);
    }
    else
        m_symTab->m_gPrefix = "";


    if (!m_isTRU)
     block = qSharedPointerDynamicCast<NodeBlock>(Block(true));
    else
        block = qSharedPointerDynamicCast<NodeBlock>(BlockNoCompound(true));

    QSharedPointer<NodeProgram> program = QSharedPointer<NodeProgram>(new NodeProgram(progName,  param, block));

    ApplyTPUAfter(block->m_decl,m_proceduresOnly);

    if (!m_isTRU) {
        Eat(TokenType::DOT);
    }
    else {
        Eat(TokenType::END);
        Eat(TokenType::DOT);

    }

    return program;

}


QSharedPointer<Node> Parser::Factor()
{
    if (m_currentToken.m_type == TokenType::LENGTH) {
        Eat();
        Eat(TokenType::LPAREN);
        QString varName = m_currentToken.m_value;
        QSharedPointer<Symbol> s = m_symTab->Lookup(varName,m_currentToken.m_lineNumber);
        if (s==nullptr) {
            ErrorHandler::e.Error("Internal function 'Length' reqruires a variable");
        }
        Token t = m_currentToken;
        t.m_intVal = s->getLength();
        t.m_type  = TokenType::INTEGER_CONST;
        Eat();
        Eat(TokenType::RPAREN);
        return QSharedPointer<NodeNumber>(new NodeNumber(t,s->getLength()));

    }


    Token t = m_currentToken;

    if (t.m_type==TokenType::TEOF)
        ErrorHandler::e.Error("Syntax error", m_currentToken.m_lineNumber);


    if (t.m_type == TokenType::INTEGER_CONST || t.m_type ==TokenType::REAL_CONST
            || t.m_type ==TokenType::ADDRESS) {
        Eat(t.m_type);
//        qDebug() << "parser: " <<t.m_value << t.m_intVal;
        return QSharedPointer<NodeNumber>(new NodeNumber(t, t.m_intVal));
    }

    if (t.m_type == TokenType::PLUS || t.m_type==TokenType::MINUS ){
        Eat(t.m_type);
        return QSharedPointer<NodeUnaryOp>(new NodeUnaryOp(t, Factor()));
    }




    if (t.m_type == TokenType::LPAREN) {
        Eat(TokenType::LPAREN);
        QSharedPointer<Node> node = Expr();
        Eat(TokenType::RPAREN);
        return node;

    }
    if (t.m_type == TokenType::ID) {
//        qDebug() << "FINDING PROCEDURE IN TERM: " << t.m_value;
        QSharedPointer<Node> node = FindProcedure();
        if (node!=nullptr)
            return node;
        node = BuiltinFunction();
        if (node!=nullptr)
            return node;

    }
    return Variable();
}

QSharedPointer<Node> Parser::RepeatUntil()
{
    Token t = m_currentToken;
    Eat(TokenType::REPEAT);
    QVector<QSharedPointer<Node>> nodes = StatementList();
    QSharedPointer<NodeCompound> root = QSharedPointer<NodeCompound>(new NodeCompound(t));
    Eat(TokenType::UNTIL);
    QSharedPointer<Node> cond = BinaryClause();
    QVector<QSharedPointer<Node>> decl;

    for (QSharedPointer<Node> n: nodes)
        root->children.append(n);

    QSharedPointer<NodeBlock> block = QSharedPointer<NodeBlock>(new NodeBlock(t,decl,root));

//    qDebug() << "from begin block : " +m_currentToken.getType();
//    NodeRepeatUntil(Token op, int forcePage, QSharedPointer<Node> cond, QSharedPointer<Node> block);

    return QSharedPointer<Node>(new NodeRepeatUntil(t,false,qSharedPointerDynamicCast<NodeBinaryClause>(cond),block));

}

QSharedPointer<Node> Parser::Term()
{
    QSharedPointer<Node> node = Factor();
    while (m_currentToken.m_type == TokenType::Type::MUL || m_currentToken.m_type == TokenType::Type::DIV
    || m_currentToken.m_type == TokenType::Type::BITAND || m_currentToken.m_type == TokenType::Type::BITOR
     || m_currentToken.m_type == TokenType::Type::SHR || m_currentToken.m_type == TokenType::Type::SHL
           || m_currentToken.m_type == TokenType::Type::XOR

           ){
        Token t = m_currentToken;
        Eat(m_currentToken.m_type);

        node = QSharedPointer<NodeBinOP>(new NodeBinOP(node, t, Factor()));

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
            if (m_currentToken.m_value.toLower() =="deletefile") {
                Eat(TokenType::PREPROCESSOR);
                QString file = m_currentToken.m_value;
                Eat();
                if (QFile::exists(m_currentDir+file))
                    QFile::remove(m_currentDir+file);

            }
            else
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
            else if (m_currentToken.m_value.toLower() =="ignoresystemheaders") {
                Eat(TokenType::PREPROCESSOR);
                Syntax::s.m_currentSystem->m_systemParams["ignoresystemheaders"]="1";
            }

            else if (m_currentToken.m_value.toLower() =="userdata") {
                Eat(TokenType::PREPROCESSOR);
                QString from = m_currentToken.getNumAsHexString();
                Eat();
                QString to = m_currentToken.getNumAsHexString();
                Eat();
                QString name = m_currentToken.m_value;
                bool ok;
                m_userBlocks.append(QSharedPointer<MemoryBlock>(new MemoryBlock(Util::NumberFromStringHex(from), Util::NumberFromStringHex(to),
                                                MemoryBlock::USER, name)));

            }
            else if (m_currentToken.m_value.toLower() =="projectsettings") {
                Eat(TokenType::PREPROCESSOR);
                HandleProjectSettingsPreprocessors();

            }
            else if (m_currentToken.m_value.toLower() =="buildpaw") {
                Eat(TokenType::PREPROCESSOR);
                HandleBuildPaw();
            }
            else if (m_currentToken.m_value.toLower() =="spritepacker") {
                Eat(TokenType::PREPROCESSOR);
                HandleSpritePacker();
            }

            else if (m_currentToken.m_value.toLower() =="ignoremethod") {
                Eat(TokenType::PREPROCESSOR);
                m_ignoreMethods.append(m_currentToken.m_value);
            }
            else if (m_currentToken.m_value.toLower() =="export") {
                Eat(TokenType::PREPROCESSOR);
                HandleExport();
            }
            else if (m_currentToken.m_value.toLower() =="exportrgb8palette") {
                Eat(TokenType::PREPROCESSOR);
                HandleExportPalette();
            }
            else if (m_currentToken.m_value.toLower() =="exportprg2bin") {
                Eat(TokenType::PREPROCESSOR);
                HandleExportPrg2Bin();
            }
            else if (m_currentToken.m_value.toLower() =="vbmexport") {
                Eat(TokenType::PREPROCESSOR);
                HandleVBMExport();
            }
            else if (m_currentToken.m_value.toLower() =="vbmexportchunk") {
                Eat(TokenType::PREPROCESSOR);
                HandleVBMExportChunk();
            }
            else if (m_currentToken.m_value.toLower() =="exportframe") {
                Eat(TokenType::PREPROCESSOR);
                HandleExportFrame();
            }
            else if (m_currentToken.m_value.toLower() =="spritecompiler") {
                Eat(TokenType::PREPROCESSOR);
                HandleSpriteCompiler();
            }

            else if (m_currentToken.m_value.toLower() =="importchar") {
                Eat(TokenType::PREPROCESSOR);
                HandleImportChar();
            }

            else if (m_currentToken.m_value.toLower() =="execute") {
                Eat(TokenType::PREPROCESSOR);
                HandleExecute();
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
                else {
//                    if (m_pass==0)
                        HandleUseTPU(type);
                }
  /*              if (!ok) {
                    ErrorHandler::e.Error("Uknown @use parameter : "+type, m_currentToken.m_lineNumber);
                }*/
            }
            else {
                Eat();
            }

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

QSharedPointer<Node> Parser::Parse(bool removeUnusedDecls, QString param, QString globalDefines, bool useLocals)
{
    // Call preprocessor for include files etc
    m_lexer->m_orgText = m_lexer->m_orgText + "\n" + globalDefines+"\n";
    m_lexer->m_text = m_lexer->m_orgText;
    m_removeUnusedDecls = removeUnusedDecls;

    if (!m_isTRU || m_symTab==nullptr)
        m_symTab = QSharedPointer<SymbolTable>(new SymbolTable());

    m_symTab->m_currentFilename = m_currentFileShort;
    emit EmitTick("<font color=\"grey\">Parsing pass #1 ");
    m_pass = 0;
  //  RemoveComments();
    InitObsolete();
    if (Syntax::s.m_currentSystem->m_processor!=AbstractSystem::M68000)
        StripWhiteSpaceBeforeParenthesis(); // TODO: make better fix for this

    InitSystemPreprocessors();
    bool done = false;
    //while (!done)
    done = PreprocessIncludeFiles();
//    m_symTab->m_constants.clear();
    Preprocess();
    ApplyTPUBefore();
//    PreprocessConstants();
    m_pass = 1;

//    if (!m_isTRU)
  //      m_tpus.clear();

    m_lexer->m_currentComment = "";
    m_parserBlocks.clear();
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
    emit EmitTick("<br>Parsing pass #2 ");
    m_symTab->Initialize();
    Node::m_staticBlockInfo.m_blockID=-1;
    QSharedPointer<NodeProgram> root = qSharedPointerDynamicCast<NodeProgram>(Program(param));


    // First add builtin functions
    if (removeUnusedDecls && !m_isTRU) {
        RemoveUnusedProcedures();
        RemoveUnusedSymbols(root);
    }


    InitBuiltinFunctions();


    for (QString s: m_procedures.keys())
        if (qSharedPointerDynamicCast<NodeProcedureDecl>(m_procedures[s])->m_block==nullptr) {
            root->m_NodeBlock->m_decl.append(m_procedures[s]);
        }


    // Then add regular ones ORDERED BY DEFINITION
    //for (QString s: m_procedures.keys())
     //   if (((QSharedPointer<NodeProcedureDecl>)m_procedures[s])->m_block!=nullptr)
        for ( QSharedPointer<Node> n: m_proceduresOnly )
            root->m_NodeBlock->m_decl.append(n);


    if (m_currentToken.m_type!=TokenType::TEOF)
        ErrorHandler::e.Error("End of file error");


//    qDebug() << m_symTab->m_symbols.keys();

    return root;
}


QSharedPointer<Node> Parser::FindProcedure()
{
    Token procToken = m_currentToken;
    bool dual = false;
    // TRUs can also search for other TRU names without prefix
    if (m_isTRU)
        dual = m_procedures.contains(m_currentToken.m_value);

//    qDebug() << "Searching for procedure : " <<m_currentToken.m_value;// << m_procedures.keys();
  //  qDebug() << "Searching for procedure : " <<m_currentToken.m_value;
    if (m_procedures.contains(m_symTab->m_gPrefix+m_currentToken.m_value) || dual) {
    //    qDebug() << "FOUND";
        QString procName = m_symTab->m_gPrefix+m_currentToken.m_value;
        if (dual) procName = m_currentToken.m_value;;
        Token t = m_currentToken;
        Eat(TokenType::ID);
        Eat(TokenType::LPAREN);
        QVector<QSharedPointer<Node>> paramList;
        while (m_currentToken.m_type!=TokenType::RPAREN && !m_lexer->m_finished) {
            paramList.append(Expr());

            if (m_currentToken.m_type==TokenType::COMMA)
                Eat(TokenType::COMMA);
            //if (m_currentToken.m_type==TokenType::SEMI)
            //    ErrorHandler::e.Error("Syntax errror", m_currentToken.m_lineNumber);
        }
//        qDebug() << "Searching for :" << procName << " in " << m_procedures.keys();
        if (!m_procedures.contains(procName))
            ErrorHandler::e.Error("Could not find procedure :" + procName, m_currentToken.m_lineNumber);

        QSharedPointer<NodeProcedureDecl> p = qSharedPointerDynamicCast<NodeProcedureDecl>(m_procedures[procName]);
        Eat(TokenType::RPAREN);

        //p->SetParameters(paramList);
        p->m_isUsed = true;
//        if (p->m_procName==BGMUpdateSpriteLoc)

        return QSharedPointer<NodeProcedure>(new NodeProcedure(p, paramList, t));
    }

    //qDebug() << m_currentToken.getType() << " with value " << m_currentToken.m_value;
    return nullptr;
}


QSharedPointer<Node> Parser::Block(bool useOwnSymTab, QString blockName)
{

/*    if (m_currentToken.m_type!=TokenType::VAR  && m_currentToken.m_type!=TokenType::BEGIN)
        return nullptr;
*/

    if (m_currentToken.m_type==TokenType::PROCEDURE || m_currentToken.m_type==TokenType::INTERRUPT || m_currentToken.m_type==TokenType::WEDGE)
        return nullptr;

    QVector<QSharedPointer<Node>> decl =  Declarations(useOwnSymTab, blockName);

    int pos = m_currentToken.m_lineNumber;
    QSharedPointer<Node> statements =  CompoundStatement();
    QSharedPointer<NodeBlock> bl =  QSharedPointer<NodeBlock>(new NodeBlock(m_currentToken,decl, statements, useOwnSymTab));
    bl->m_op.m_lineNumber = pos;
    return bl;
}

QSharedPointer<Node> Parser::BlockNoCompound(bool useOwnSymTab, QString blockName)
{

    if (m_currentToken.m_type==TokenType::PROCEDURE || m_currentToken.m_type==TokenType::INTERRUPT || m_currentToken.m_type==TokenType::WEDGE)
        return nullptr;


    QVector<QSharedPointer<Node>> decl =  Declarations(useOwnSymTab, blockName, false);
    int pos = m_currentToken.m_lineNumber;
    QSharedPointer<NodeBlock> bl =  QSharedPointer<NodeBlock>(new NodeBlock(m_currentToken,decl, nullptr, useOwnSymTab));
    bl->m_op.m_lineNumber = pos;
    return bl;

}


QVector<QSharedPointer<Node> > Parser::Parameters(QString blockName)
{
    QVector<QSharedPointer<Node>> decl;
    if (m_currentToken.m_type==TokenType::LPAREN) {
        Eat(TokenType::LPAREN);
        while (m_currentToken.m_type==TokenType::ID) {
            QVector<QSharedPointer<Node>> ns = VariableDeclarations(blockName);

            for (QSharedPointer<Node> n: ns)
                decl.append(n);
            Eat(m_currentToken.m_type);
        }
    }
    //Eat(TokenType::RPAREN);
    return decl;
}

QSharedPointer<Node> Parser::ForLoop(bool inclusive)
{
    int ln = m_currentToken.m_lineNumber;
    if (inclusive)
        Eat(TokenType::FORI);
    else
        Eat(TokenType::FOR);
    QSharedPointer<Node> a = AssignStatement();
    Eat(TokenType::TO);
    QSharedPointer<Node> b = Expr();
    bool unroll = false;
    QSharedPointer<Node> step = nullptr;

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
    QSharedPointer<Node> block = Block(false);

//    qDebug() << m_currentToken.getType();
  //  exit(1);
    return QSharedPointer<NodeForLoop>(new NodeForLoop(a,b,block, step, unroll, forcePage, loopType, inclusive));

}

QSharedPointer<Node> Parser::String()
{

    if (m_currentToken.m_type==TokenType::STRING || m_currentToken.m_type==TokenType::CSTRING) {
        QSharedPointer<NodeString> node = QSharedPointer<NodeString>(new NodeString(m_currentToken,QStringList()<<m_currentToken.m_value,m_currentToken.m_type==TokenType::CSTRING));
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

    QSharedPointer<NodeString> node = QSharedPointer<NodeString>(new NodeString(token, lst, token.m_type==TokenType::CSTRING));
    return node;
}


QVector<QSharedPointer<Node>> Parser::Declarations(bool isMain, QString blockName, bool hasBlock )
{
    QVector<QSharedPointer<Node>> decl;

//    qDebug() << "PARSER " <<isMain <<m_currentToken.getType();
  /*  if (isMain)
        if (m_currentToken.m_type!=TokenType::VAR)
            ErrorHandler::e.Error("Main program must start with 'var' keyword",m_currentToken.m_lineNumber);

*/
    if (m_currentToken.m_type==TokenType::VAR) {
        Eat(TokenType::VAR);
  //      qDebug() << "PARSER INSIDE " <<isMain  <<m_currentToken.getType();
        while (m_currentToken.m_type==TokenType::ID || m_currentToken.m_type == TokenType::CONSTANT) {

            if (m_currentToken.m_type == TokenType::CONSTANT) {
                ConstDeclaration();
            }
            else {

                QVector<QSharedPointer<Node>> ns = VariableDeclarations(blockName);

                for (QSharedPointer<Node> n: ns)
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
            QVector<QSharedPointer<Node>> ns = VariableDeclarations();
            for (QSharedPointer<Node> n: ns)
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
        bool isInline = false;
        QString procName =m_symTab->m_gPrefix+ m_currentToken.m_value;
        //qDebug() << tok.m_value  << " : " << procName;
        Eat(TokenType::ID);
        //exit(1);
        QVector<QSharedPointer<Node>> paramDecl;
        if (m_currentToken.m_type==TokenType::LPAREN)
            paramDecl = Parameters(procName);
        //qDebug()<< "current : " << m_currentToken.getType();
        // If no parameters
        if (m_currentToken.m_type==TokenType::RPAREN)
            Eat(TokenType::RPAREN);

        if (m_currentToken.m_type == TokenType::INLINE) {
            isInline = true;
            Eat(TokenType::INLINE);
        }


        Eat(TokenType::SEMI);
        QSharedPointer<Node> block = nullptr;
        QSharedPointer<NodeProcedureDecl> procDecl = QSharedPointer<NodeProcedureDecl>(new NodeProcedureDecl(tok, procName, paramDecl, block, type));
        procDecl->m_fileName = m_currentFileShort;
        procDecl->m_isInline = isInline;
        AppendComment(procDecl);

        if (m_procedures[procName]!=nullptr) {
            procDecl->m_isUsed = m_procedures[procName]->m_isUsed;
            // Make sure that the correct number of parameters + types etc are identical for the forward declared procedure
            QSharedPointer<NodeProcedureDecl> existing = qSharedPointerDynamicCast<NodeProcedureDecl>(m_procedures[procName]);
            if (existing->m_paramDecl.count()!=procDecl->m_paramDecl.count())
                ErrorHandler::e.Error("Forward declared procedure '"+ WashVariableName(procName) +"' has incorrect number of parameters. ", tok.m_lineNumber);

            for (int i=0;i<existing->m_paramDecl.count();i++) {
                QSharedPointer<NodeVarDecl> a = qSharedPointerDynamicCast<NodeVarDecl>(existing->m_paramDecl[i]);
                QSharedPointer<NodeVarDecl> b = qSharedPointerDynamicCast<NodeVarDecl>(procDecl->m_paramDecl[i]);
                if ( qSharedPointerDynamicCast<NodeVar>(a->m_varNode)->value!=
                     qSharedPointerDynamicCast<NodeVar>(b->m_varNode)->value)
                    ErrorHandler::e.Error("Forward declared procedure '"+ WashVariableName(procName) +"' has incorrect or missing declared parameter '"+WashVariableName(qSharedPointerDynamicCast<NodeVar>(a->m_varNode)->value)+"'", tok.m_lineNumber);

                if ( qSharedPointerDynamicCast<NodeVarType>(a->m_typeNode)->value!=
                     qSharedPointerDynamicCast<NodeVarType>(b->m_typeNode)->value)
                    ErrorHandler::e.Error("Forward declared procedure '"+ WashVariableName(procName) +
                                          "' has incorrect declared parameter type for parameter '"+WashVariableName(qSharedPointerDynamicCast<NodeVar>(a->m_varNode)->value)+"', should be "
                                          +WashVariableName(qSharedPointerDynamicCast<NodeVarType>(a->m_typeNode)->value), tok.m_lineNumber);


            }
//            qDebug() << procName << procDecl->m_paramDecl.count() << existing->m_paramDecl.count();

        }
        m_procedures[procName] = procDecl;
//        qDebug() << "Adding to GLOBAL list: " <<procName;
        m_symTab->m_globalList.append(procName);

        // For recursive procedures, it is vital that we forward declare the current procedure
        block = Block(false, procName);
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
            for (QSharedPointer<Node> n: m_proceduresOnly) {
                QSharedPointer<NodeProcedureDecl> proc =qSharedPointerDynamicCast<NodeProcedureDecl>(n);
                if (proc->m_procName==procName) {
                    ok = false;
                    // Verify that the parameters are identical:
                }


            }
            if (!ok)
                ErrorHandler::e.Error("Procedure '"+ procName +"' already defined", tok.m_lineNumber);

            m_proceduresOnly.append(procDecl);
        }

    }
//    }
   // qDebug() << "Finally:" << m_currentToken.getType();
    if (m_currentToken.m_type!=TokenType::BEGIN && isMain && hasBlock)
        ErrorHandler::e.Error("After declarations, BEGIN is expected", m_currentToken.m_lineNumber);

    return decl;
}

QVector<QSharedPointer<Node>> Parser::ConstDeclaration()
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
    TokenType::Type dType = m_currentToken.m_type;
    if (type=="") {
        ErrorHandler::e.Error("Unknown or illegal type when defining constant of type: '"+m_currentToken.m_value+"' ("+m_currentToken.getType()+")<br>Allowed types are : <b>address, byte, integer.</b> ",m_currentToken.m_lineNumber);
    }
    Eat();
    Eat(TokenType::EQUALS);
    int value = GetParsedInt(dType);

    m_symTab->m_constants[name.toUpper()] = QSharedPointer<Symbol>(new Symbol(name.toUpper(),type.toUpper(),value));
    return QVector<QSharedPointer<Node>>();
}



QVector<QSharedPointer<Node> > Parser::VariableDeclarations(QString blockName)
{
    if (blockName!="")
        m_symTab->SetCurrentProcedure(blockName+"_");
    else
        m_symTab->SetCurrentProcedure("");



    // All the vars that will be declared
    QVector<QSharedPointer<Node>> vars;
    // Make sure that prefix is added
    m_currentToken.m_value = m_symTab->m_gPrefix+m_currentToken.m_value;
    vars.append(QSharedPointer<NodeVar>(new NodeVar(m_currentToken)));
    QString varName = m_currentToken.m_value;
//    qDebug() << "CURRENT VARNAME "<< varName;
    QVector<QSharedPointer<Symbol>> syms;
    syms.append(QSharedPointer<Symbol>(new Symbol(m_currentToken.m_value,"")));
    Eat(TokenType::ID);

    // Declare a record?
    if (m_currentToken.m_type==TokenType::EQUALS) {
        Eat(TokenType::EQUALS);
        if (m_currentToken.m_type==TokenType::RECORD) {
            Record(varName);
            return QVector<QSharedPointer<Node> >();
        }
        ErrorHandler::e.Error("Unknown declaration : " +m_currentToken.m_value);
    }

    // Make sure that ALL are defined!

    while (m_currentToken.m_type == TokenType::COMMA) {
        Eat(TokenType::COMMA);
        // Prefix
        m_currentToken.m_value = m_symTab->m_gPrefix+m_currentToken.m_value;
        vars.append(QSharedPointer<NodeVar>(new NodeVar(m_currentToken)));
        AppendComment(vars[vars.count()-1]);
        syms.append(QSharedPointer<Symbol>(new Symbol(m_currentToken.m_value,"")));

        Eat(TokenType::ID);
    }
    Eat(TokenType::COLON);
    bool isGlobal = false;
    if (m_currentToken.m_type==TokenType::GLOBAL) {
        isGlobal = true;
        Eat();
    }

    // NOW do the syms define
    if (!isGlobal)
    for (QSharedPointer<Symbol> s: syms) {
        if (Syntax::s.m_illegaVariableNames.contains(s->m_name))
            ErrorHandler::e.Error("Illegal variable name '" + s->m_name +"' on the "+AbstractSystem::StringFromSystem(Syntax::s.m_currentSystem->m_system)+" (name already used in the assembler etc) ",m_currentToken.m_lineNumber);

        m_symTab->Define(s ,false);
        s->m_lineNumber = m_currentToken.m_lineNumber;
    }

//    qDebug() << "CURVAL " <<m_currentToken.m_value;
    QSharedPointer<NodeVarType> typeNode = qSharedPointerDynamicCast<NodeVarType>(TypeSpec());
    typeNode->m_flags.append(getFlags());
    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::GAMEBOY) {
        //if (typeNode->m_op.m_type==TokenType::POINTER)
        if (typeNode->m_data.count()<=1 && typeNode->m_op.m_type!=TokenType::INCBIN && typeNode->m_op.m_type!=TokenType::STRING)
        {
//            qDebug() << "HERE" << vars;
            typeNode->m_flags.append("wram"); // Always declare pointers in WRAM on GB
        }
    }
    // Set all types

    for (QSharedPointer<Symbol> s: syms) {
       s->m_type = typeNode->m_op.m_value;
       s->m_flags = typeNode->m_flags;
       s->m_bank = typeNode->m_bank;
       if (typeNode->m_data.count()!=0)
           s->m_size = typeNode->m_data.count();
       else {
           s->m_size = 1;
           if (s->m_type=="integer")
               s->m_size =2;
           if (s->m_type=="long")
               s->m_size =4;
       }
      // qDebug() << "A";
       s->m_size = typeNode->m_op.m_intVal;
       if (s->m_size==0)
           s->m_size = typeNode->m_data.count();
//       qDebug() << "B";
       s->m_arrayType = typeNode->m_arrayVarType.m_type;
  //     qDebug() << typeNode->m_arrayVarType.m_type;
       s->m_arrayTypeText = TokenType::getType(typeNode->m_arrayVarType.m_type);
    //   qDebug() << "C";
       if (typeNode->m_arrayVarType.m_type==TokenType::RECORD) {
  //         qDebug() << typeNode->m_op.m_value << TokenType::getType(typeNode->m_op.m_type)  << TokenType::getType(typeNode->m_arrayVarType.m_type);
//           ;
           s->m_arrayTypeText = typeNode->m_arrayVarType.m_value;
       }
//       qDebug() << "D";
    }


/*    for (QSharedPointer<Node> v: vars) {
        Syntax::s.globals[((Var*)v)->value] = 0;
    }*/

    QVector<QSharedPointer<Node>> var_decleratons;
    if (isGlobal) { // Typecheck that they exist
        for (QSharedPointer<Node> n : vars) {
            QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(n);
            try {
               m_symTab->Lookup(v->value,v->m_op.m_lineNumber);
            } catch (FatalErrorException fe) {
                fe.message = fe.message + "When using the <font color=\"yellow\">global</font> keyword, the variable must in question must be declared in the global variable scope. ";
                throw fe;
            }

        }
    }
   for (QSharedPointer<Node> n : vars) {
        QSharedPointer<NodeVarDecl> decl = QSharedPointer<NodeVarDecl>(new NodeVarDecl(n, typeNode));
        QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(n);
        v->m_isGlobal = isGlobal;
        var_decleratons.append(decl);
//        qDebug() <<  typeNode->m_op.getType() << typeNode->m_op.m_value << (qSharedPointerDynamicCast<NodeVar>n)->value;;
        if (typeNode->m_op.m_type == TokenType::INCSID) {
            //            decl->m_isUsed = true;

            int sidloc = 0;
            if (m_preprocessorDefines.contains("SIDEmulatorLocation")) {
                sidloc = Util::NumberFromStringHex(m_preprocessorDefines["SIDEmulatorLocation"]);
                //qDebug() << sidloc;
            }
            //            exit(1);
            decl->InitSid(m_symTab,m_currentDir, sidloc, "sid");
        }
        if (typeNode->m_op.m_type == TokenType::INCNSF) {
            int sidloc = 0;
            //           exit(1);
            decl->InitSid(m_symTab, m_currentDir, sidloc, "nsf");
        }
   }

//    return vars;
    return var_decleratons;
}



QSharedPointer<Node> Parser::TypeSpec()
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
            position = Util::numToHex(GetParsedInt(TokenType::ADDRESS));
        }
        Eat(TokenType::RPAREN);


        QStringList flags = getFlags();

        QSharedPointer<NodeVarType> nt =  QSharedPointer<NodeVarType>(new NodeVarType(t,binFile, position));
        nt->m_flags = flags;

        return nt;


    }

    // Array [] of blah = (1,2,3);
    if (m_currentToken.m_type == TokenType::ARRAY) {
        Eat(TokenType::ARRAY);
        Eat(TokenType::LBRACKET);
        float count = 0;
        if (m_currentToken.m_type != TokenType::RBRACKET)
            count = GetParsedInt(TokenType::NADA);

        Eat(TokenType::RBRACKET);
        Eat(TokenType::OF);
        Token arrayType = m_currentToken;
        VerifyTypeSpec(arrayType);
        Eat(m_currentToken.m_type);
        TokenType::Type dataType = m_currentToken.m_type;
        QStringList data;
        // Contains constant init?
        if (m_currentToken.m_type==TokenType::EQUALS) {
            Eat();
            if (m_currentToken.m_type==TokenType::BUILDTABLE) {
                data = BuildTable(count);
            }
            else {
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
                        data << "$0"+QString::number(GetParsedInt(dataType),16);//QString::number(m_currentToken.m_intVal);
                    //data << "$0"+QString::number(GetParsedInt(),16);//QString::number(m_currentToken.m_intVal);
                    if (m_currentToken.m_type!=TokenType::RPAREN) {

                        Eat();
                        if (m_currentToken.m_type==TokenType::COMMA)
                            Eat();
                    }
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

        } // end of array

        QString position = "";
        if (m_currentToken.m_type==TokenType::AT || m_currentToken.m_type==TokenType::ABSOLUT) {
            Eat();
            //position = m_currentToken.getNumAsHexString();
            position = Util::numToHex(GetParsedInt(TokenType::ADDRESS));

           // Eat(m_currentToken.m_type);
        }
        QStringList flags = getFlags();

        t.m_intVal = count;
//        qDebug() << "Type: " << t.m_value;
  //      t.m_type = arrayType.m_type;
//        qDebug()<< "PARSE "<< arrayType.getType() <<arrayType.m_value;
        if (m_symTab->m_records.contains(arrayType.m_value)) {
            arrayType.m_type = TokenType::RECORD;
            if (m_symTab->m_records[arrayType.m_value]->ContainsArrays())
                ErrorHandler::e.Error("You cannot declare an array of records that contain sub-arrays due to 6502 limitations. <br>Please remove the sub-array from the record type in question : '"+arrayType.m_value+"'.",arrayType.m_lineNumber);
        }

        QSharedPointer<NodeVarType> nt =  QSharedPointer<NodeVarType>(new NodeVarType(t,position, arrayType,data));
        nt->m_flags = flags;
        return nt;


    }

    if (m_currentToken.m_type == TokenType::STRING || m_currentToken.m_type==TokenType::CSTRING) {
        Eat();
        QStringList initData;
        if (m_currentToken.m_type == TokenType::EQUALS) {
            Eat();
            QSharedPointer<NodeString> str = qSharedPointerDynamicCast<NodeString>(String());
            initData = str->m_val;
        }
        return QSharedPointer<NodeVarType>(new NodeVarType(t,initData));
    }

    if (m_currentToken.m_type == TokenType::POINTER) {
        Eat();
        QString type;
        QSharedPointer<NodeVarType> nvt = QSharedPointer<NodeVarType>(new NodeVarType(t,""));
        nvt->m_arrayVarType.m_type = TokenType::BYTE;

        if (m_currentToken.m_type == TokenType::OF) {
            Eat();
            TokenType::Type typ = m_currentToken.m_type;

//            QSharedPointer<NodeString> str = (QSharedPointer<NodeString>)String();
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
        QSharedPointer<NodeVarType> nt = QSharedPointer<NodeVarType>(new NodeVarType(t, position ));
//        qDebug() << "IN TYPESPEC : " << t.m_value << TokenType::getType(t.m_type);
        nt->m_position = position;
        // Only declare as CONST if NOT a record
        nt->m_flag = 1;
//        VerifyTypeSpec(t);

        return nt;
    }


    QString initVal = "";
    if (m_currentToken.m_type == TokenType::EQUALS) {
        Eat();
//        data << "$0"+QString::number(getIntVal(m_currentToken),16);//QString::number(m_currentToken.m_intVal);

        initVal = Util::numToHex(GetParsedInt(t.m_type));
        if (Syntax::s.m_currentSystem->m_system == AbstractSystem::NES || Syntax::s.m_currentSystem->m_system == AbstractSystem::GAMEBOY)
            ErrorHandler::e.Error("You cannot initialize variables stored in work ram (WRAM) on the Gameboy/NES, as these needs to be initialized manually. " ,m_currentToken.m_lineNumber-1);



    }
    VerifyTypeSpec(t);

    return QSharedPointer<NodeVarType>(new NodeVarType(t,initVal));

}

QSharedPointer<Node> Parser::BuiltinFunction()
{
    if (Syntax::s.builtInFunctions.contains(m_currentToken.m_value.toLower())) {
        QString procName = m_currentToken.m_value.toLower();
        int noParams = Syntax::s.builtInFunctions[procName].m_params.count();
        Eat(TokenType::ID);
        Eat(TokenType::LPAREN);
        QVector<QSharedPointer<Node>> paramList;
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

        return QSharedPointer<NodeBuiltinMethod>(new NodeBuiltinMethod(procName,paramList,&Syntax::s.builtInFunctions[procName]));
        //p->SetParameters(paramList);


    }
    return nullptr;

}

QSharedPointer<Node> Parser::Constant()
{
    QString id = m_currentToken.m_value;
    if (m_symTab->m_constants.contains(id)) {
        Eat(m_currentToken.m_type);
        QSharedPointer<Symbol> s = m_symTab->m_constants[id];
        QSharedPointer<Node> n =  QSharedPointer<NodeNumber>(new NodeNumber(Token(s->getTokenType(), s->m_value->m_fVal), s->m_value->m_fVal));
        return n;
    }
    return nullptr;
}

QSharedPointer<Node> Parser::InlineAssembler()
{
    Eat(TokenType::ASM);
    Eat(TokenType::LPAREN);
    if (m_currentToken.m_type!=TokenType::STRING)
        ErrorHandler::e.Error("Inline assembler must be enclosed as a string");

    if (Syntax::s.m_currentSystem->m_processor==AbstractSystem::MOS6502) {
        VerifyInlineSymbols6502(m_currentToken.m_value);
    }

    QSharedPointer<Node> n = QSharedPointer<NodeAsm>(new NodeAsm(m_currentToken));
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

QStringList Parser::BuildTable(int cnt)
{
    Eat(TokenType::BUILDTABLE);
    if (cnt==0)
        ErrorHandler::e.Error("BuildTable must have at least 1 element in array.",m_currentToken.m_lineNumber);


    Eat(TokenType::LPAREN);
    QString sentence = m_currentToken.m_value;
    Eat(TokenType::STRING);
    QStringList data;
    QJSEngine myEngine;
    for (int i=0;i<cnt;i++) {
        QString str = sentence;
//        str = str.replace("i",QString::number(i));
//        QJSValue ret = myEngine.evaluate(str);

        QJSValue fun = myEngine.evaluate("(function(i) { return "+str+"; })");
        if (fun.isError())
            ErrorHandler::e.Error("Error evaluation javascript expression : " + fun.toString() + " <br><br>", m_currentToken.m_lineNumber);

        QJSValueList args;
        args << i;
        QJSValue ret = fun.call(args);

        if (ret.isError())
            ErrorHandler::e.Error("Error evaluation javascript expression : " + ret.toString() + " <br><br>", m_currentToken.m_lineNumber);

//        data << Util::numToHex(ret.toInt()&0xFF);
 //       if ()
//        data << Util::numToHex(ret.toInt()&0xFF);
        data << Util::numToHex(ret.toInt()&0xFFFF);
    }

    return data;
}

void Parser::HandleExportPalette()
{
    int ln = m_currentToken.m_lineNumber;
    QString inFile = m_currentDir+"/"+ m_currentToken.m_value;
    Eat(TokenType::STRING);
    QString outFile =m_currentDir+"/"+ m_currentToken.m_value;
    Eat(TokenType::STRING);

    if (!QFile::exists(inFile)) {
        ErrorHandler::e.Error("File not found : "+inFile,ln);
    }
    LImage* img = LImageIO::Load(inFile);
    if (QFile::exists(outFile))
        QFile::remove(outFile);


    img->ExportRGB8Palette(outFile);

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
    if (dynamic_cast<LImageNES*>(img)!=nullptr) {
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

void Parser::HandleBuildPaw()
{
    int ln = m_currentToken.m_lineNumber;
    QString inFile = m_currentDir+"/"+ m_currentToken.m_value; //    QString inFile = m_currentToken.m_value;form
    Eat(TokenType::STRING);
    FormPaw fp;
    fp.InitDocument(nullptr,m_settingsIni,m_projectIni);
    if (!QFile::exists(inFile))
        ErrorHandler::e.Error("Could not locate paw file for building: "+inFile, ln);
//    fp.Load(inFile);
    fp.m_pawData->Load(inFile);

    fp.BuildSingle();
}

void Parser::HandleExportPrg2Bin()
{
    int ln = m_currentToken.m_lineNumber;
    QString inFile = m_currentDir+"/"+ m_currentToken.m_value;
    Eat(TokenType::STRING);
    QString outFile =m_currentDir+"/"+ m_currentToken.m_value;
    Eat(TokenType::STRING);
    int from = m_currentToken.m_intVal;
    Eat(TokenType::INTEGER_CONST);
    int to = m_currentToken.m_intVal;
    Eat(TokenType::INTEGER_CONST);

    QByteArray in  = Util::loadBinaryFile(inFile);
    QByteArray out;
    if (outFile.toLower().contains(".prg")) {
        out.append((char)((from)&0xFF)); // lo byte
        out.append((char)((from>>8)&0xFF)); // hi byte
    }
    int start = (in[0] | ((int)(in[1])<<8))&0xFFFF;
//    qDebug() << "PRG2BIN START "<<Util::numToHex(start);
    in = in.remove(0,2);
  //  qDebug() << "PRG2BIN "<<Util::numToHex(from) << Util::numToHex(to);
    for (int i=from;i<to;i++) {
        int j = i-start;
        if (in.count()<j)
            out.append((char)0);
        else
  //          ErrorHandler::e.Error("ExportPrg2Bin error: .prg file does not contain specified binary range.", m_currentToken.m_lineNumber);
            out.append(in[j]);
    }
    Util::SaveByteArray(out,outFile);
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

void Parser::HandleVBMExportChunk()
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

    img->VBMExportChunk(file,param1,param2,param3,param4);

    file.close();

}

void Parser::HandleExportFrame()
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
    Eat(TokenType::INTEGER_CONST);
    int param5 = m_currentToken.m_intVal;
    Eat(TokenType::INTEGER_CONST);
    int param6 = m_currentToken.m_intVal;
    Eat(TokenType::INTEGER_CONST);
    int param7 = m_currentToken.m_intVal;
    Eat(TokenType::INTEGER_CONST);
    int param8 = m_currentToken.m_intVal;

    if (!QFile::exists(inFile)) {
        ErrorHandler::e.Error("File not found : "+inFile,ln);
    }
    LImage* img = LImageIO::Load(inFile);
    if (QFile::exists(outFile))
        QFile::remove(outFile);

    QFile file(outFile);

    file.open(QFile::WriteOnly);
    img->m_silentExport = true;

    img->ExportFrame(file,param1,param2,param3,param4,param5,param6,param7,param8);

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

void Parser::HandleSpritePacker()
{
    QString inFile = m_currentDir+ m_currentToken.m_value;
    Eat(TokenType::STRING); // Filename

    QString outChrFileName = m_currentDir+m_currentToken.m_value;
    Eat(TokenType::STRING); // Filename

    QString outSpriteFileName = m_currentDir+m_currentToken.m_value;
    Eat(TokenType::STRING); // Filename

    QString type = m_currentToken.m_value.toLower();
    Eat(TokenType::STRING); // Filename

    int x = m_currentToken.m_intVal;
    Eat(TokenType::INTEGER_CONST); // X
    int y = m_currentToken.m_intVal;
    Eat(TokenType::INTEGER_CONST); // Y
    int w = m_currentToken.m_intVal;
    Eat(TokenType::INTEGER_CONST); // W
    int h = m_currentToken.m_intVal;
    Eat(TokenType::INTEGER_CONST); // H
    int comp = m_currentToken.m_intVal;
    Eat(TokenType::INTEGER_CONST); // H

    LImage* imgChrOut = nullptr;
    LImage* imgSrc = nullptr;

    if (QFile::exists(outChrFileName)) {
        imgChrOut = LImageIO::Load(outChrFileName);
    }
    else {
        if (type=="gameboy")
            imgChrOut = LImageFactory::Create(LImage::GAMEBOY, LColorList::NES);
    }
    if (imgChrOut == nullptr)
        ErrorHandler::e.Error("Unknown image type : "+type+". For now, only 'gameboy' is supported.", m_currentToken.m_lineNumber);

    if (inFile.toLower().endsWith(".bin") || inFile.toLower().endsWith(".chr")) {
        imgSrc = LImageFactory::Create(imgChrOut);
        QFile f(inFile);
        f.open(QFile::ReadOnly);
        imgSrc->ImportBin(f);
        f.close();
    }
    if (inFile.toLower().endsWith(".flf"))
      imgSrc = LImageIO::Load(inFile);

    if (imgSrc == nullptr) {
        ErrorHandler::e.Error("Importing char error : unknown filetype for input binary '"+inFile +"'");
    }

    QByteArray spriteData;
    if (QFile::exists(outSpriteFileName))
        spriteData = Util::loadBinaryFile(outSpriteFileName);

    int curPos = spriteData.count();

    imgChrOut->SpritePacker(imgSrc, spriteData, x,y,w,h,comp);

    Util::SaveByteArray(spriteData, outSpriteFileName);
    LImageIO::Save(outChrFileName,imgChrOut);
    ErrorHandler::e.Warning("Added new sprite data from '"+inFile+"' : sprite from "+QString::number(curPos) + "  to " + QString::number(spriteData.count()),m_currentToken.m_lineNumber);
//    qDebug() << "SPRDATA " <<spriteData;

}

void Parser::HandleProjectSettingsPreprocessors()
{
    QString cmd = m_currentToken.m_value.toLower();
    Eat(TokenType::STRING); // Filename
    QString val = m_currentToken.m_value;
    if (val=="")
       val = QString::number(m_currentToken.m_intVal);
    if (cmd == "startaddress") {
        m_projectIni->setFloat("override_target_settings", 1);
        m_projectIni->setString("override_target_settings_org", val);
    }
    if (cmd == "basicsysaddress") {
        m_projectIni->setFloat("override_target_settings", 1);
        m_projectIni->setString("override_target_settings_basic", val);
    }
    if (cmd == "ignorebasicsysstart") {
        m_projectIni->setFloat("override_target_settings", 1);
        m_projectIni->setFloat("override_target_settings_sys", val.toInt());
    }

    if (cmd == "stripprg") {
        m_projectIni->setFloat("override_target_settings", 1);
        m_projectIni->setFloat("override_target_settings_prg", val.toInt());
    }
    if (cmd == "ignorejmp") {
        m_projectIni->setFloat("override_target_settings", 1);
        m_projectIni->setFloat("ignore_initial_jump", val.toInt());
    }

    Eat(); // H
}

void Parser::HandleExecute()
{
    QStringList ls = m_currentToken.m_value.split(" ");
    Eat();
    QString f = ls.first();
    ls.removeAll(f);
    QString out;
    Syntax::s.m_currentSystem->StartProcess(f,ls,out);
}

void Parser::HandleUseTPU(QString fileName)
{
    //QString fname = m_currentDir + QDir::separator() + fileName+".tru";

//    if (m_isTRU)
  //      ErrorHandler::e.Error("TRSE Unit files cannot include other unit files")

    if (s_usedTRUs.contains(fileName)) {
        return;
    }

    QStringList dirs;
    dirs << m_currentDir + QDir::separator();
    dirs << Util::GetSystemPrefix()+ "tutorials/"+Syntax::s.m_systemString+ QDir::separator()+ "tru"+QDir::separator();
    QString fname = Util::findFileInDirectories(fileName + ".tru", dirs);


    if (fname == "" || !QFile::exists(fname)) {
        ErrorHandler::e.Error("Could not find TRU file for inclusion : "+fileName,m_currentToken.m_lineNumber);
    }
    s_usedTRUs.append(fileName);
//    qDebug() << "ADDING" << fileName;

    QSharedPointer<Parser> p = QSharedPointer<Parser>(new Parser);
    QSharedPointer<Lexer> l = QSharedPointer<Lexer>(new Lexer);
    p->m_lexer = l;
    p->m_currentDir= m_currentDir;
    l->m_orgText = l->m_text;
    p->m_isTRU = true;
    p->m_projectIni = m_projectIni;
    p->m_settingsIni = m_settingsIni;
 //   p->m_tpus = m_tpus;
    p->m_symTab = QSharedPointer<SymbolTable>(new SymbolTable);
    p->m_symTab->m_globalList = m_symTab->m_globalList;


    p->m_symTab->m_symbols = m_symTab->m_symbols;

//    qDebug() << "Copying over: " << m_procedures.keys();
    p->m_procedures = m_procedures;


    l->m_text = Util::loadTextFile(fname);
    l->m_text = l->m_text.replace("\r\n","\n");
    l->m_orgText = l->m_text;
    p->m_currentFileShort = fname;

    try {

        p->m_tree = p->Parse( false
                             ,m_projectIni->getString("vic_memory_config"),
                             Util::fromStringList(m_projectIni->getStringList("global_defines")),
                             m_projectIni->getdouble("pascal_settings_use_local_variables")==1.0);
    }catch (FatalErrorException e)
    {
        e.message = "<font color=\"#FFB030\">Error during compiling the Turbo Rascal Unit file : '</font><font color=\"yellow\">" +fileName + "</font>'<br>Please make sure that this TRU files is working correctly, and compiles stand-alone! <br><br>Original error message: <font color=\"grey\">"+e.message+"</font>";
        e.linenr = m_currentToken.m_lineNumber;
       throw e;
    }


    m_symTab->Merge(p->m_symTab.get(),true);

    m_doNotRemoveMethods.append(p->m_doNotRemoveMethods);
    m_ignoreBuiltinFunctionTPU.append(p->m_ignoreBuiltinFunctionTPU);
//    m_userBlocks.append(p->m_userBlocks);


    for (QString s : p->m_symTab->m_symbols.keys()) {
        m_symTab->m_globalList.append(s);
    }
   // qDebug() << "LIST : " <<m_symTab->m_symbols.keys();
    if (!m_tpus.contains(p))
        m_tpus.append(p);

//    qDebug() << m_currentToken.m_value;
//    Eat();
//    qDebug() << m_currentToken.m_value;
}

QSharedPointer<Node> Parser::Expr()
{
    QSharedPointer<Node> node = Term();




    while (m_currentToken.m_type == TokenType::Type::PLUS || m_currentToken.m_type == TokenType::Type::MINUS
            || m_currentToken.m_type == TokenType::Type::BITAND || m_currentToken.m_type == TokenType::Type::BITOR) {
        Token t = m_currentToken;

        Eat(m_currentToken.m_type);

        node = QSharedPointer<NodeBinOP>(new NodeBinOP(node, t, Term()));

    }

    if (node->isPureNumeric() && qSharedPointerDynamicCast<NodeNumber>(node)==nullptr) {
        // Calculate and COLLAPSE. Easier on the dispatcher.
        int val = node->getValueAsInt(nullptr);
        Token t;
        t.m_type = node->VerifyAndGetNumericType();
        t.m_lineNumber = node->m_lineNumber;
//        qDebug() << "Collapsing node to " << Util::numToHex(val) << t.getType() << t.m_lineNumber << node->m_op.getType();

        return QSharedPointer<NodeNumber>(new NodeNumber(t,val));

//        qDebug() << node->m_op.getType();
//        t.m_type = TokenType::CON
    }

    return node;
}

