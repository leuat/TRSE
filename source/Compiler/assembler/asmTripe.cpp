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

#include "asmTripe.h"
#include "source/Compiler/syntax.h"



AsmTripe::AsmTripe() :Assembler()
{
//    m_stack["for"] = new Stack();

    byte = "uint8";
    word = "uint16";
    llong = "uint32";
    ppointer = "uint16";

}



AsmTripe::~AsmTripe() {
}

QString AsmTripe::intToHexString(int val)
{
    QString s = QString::number(val);
    QString line = ".uint8 ";
    for (int i=0;i<s.count();i++) {
        int val = QString(s[i]).toInt() + 0x30;
        line = line + Util::numToHex(val) + " ";
    }
    line = line.remove(line.count()-1,1);
    return line;

}


void AsmTripe::Program(QString programName, QString vicConfig)
{

    m_source+=m_startInsertAssembler;

    Nl();


    QString org = Util::numToHex(Syntax::s.m_currentSystem->m_startAddress);
    StartMemoryBlock(org);

    if (!Syntax::s.m_ignoreSys && (Syntax::s.m_currentSystem->m_programStartAddress!=Syntax::s.m_currentSystem->m_startAddress)) {


        // new method
        //Asm(".byte $00 ; fill $xxx0");
        Asm( "."+byte+" $" + QString::number( (Syntax::s.m_currentSystem->m_startAddress + 10) & 0x0ff, 16  ) + " ; lo byte of next line" );
        Asm( "."+byte+" $" + QString::number( ( (Syntax::s.m_currentSystem->m_startAddress + 10) & 0x0ff00 ) >> 8, 16 ) + " ; hi byte of next line" );
        Asm("."+byte+" $0a $00 ; line 10 (lo, hi)");
        Asm("."+byte+" $9e $20 ; SYS token and a space");
        // write PETSCII / ASCII representation of address to call
        Asm(intToHexString(Syntax::s.m_currentSystem->m_programStartAddress));
        Asm("."+byte+" $00 $00 $00 ; end of program");

        Nl();

        EndMemoryBlock();
//        Comment("End of SYS memory block, starting new");
        StartMemoryBlock(Util::numToHex(Syntax::s.m_currentSystem->m_programStartAddress));
        m_insertEndBlock = "EndBlock"+Util::numToHex(Syntax::s.m_currentSystem->m_programStartAddress).remove("$");
  //      Comment("Start of MAIN BLOCK");
//        qDebug() << "INSERT "+m_insertEndBlock;
    }
    m_currentBlock->m_isMainBlock = true;
    m_mainBlock = m_currentBlock;
//    m_source+=m_startInsertAssembler;
//    Asm("test");

    Label(programName);
//    if (!m_hasOpenBlock)
  //      EndMemoryBlock();
}

void AsmTripe::EndProgram()
{
//    Asm("rts");
    //if (m_hasOpenBlock)
    Comment("End of program");
/*    if (m_currentBlock==nullptr) {
        if (m_insertEndBlock!="") {
            Write(m_insertEndBlock,0);
        }
        else EndMemoryBlock();
    }
    else*/
    m_isTheRealEnd = true;
    EndMemoryBlock();
}


void AsmTripe::DeclareArray(QString name, QString type, int count, QStringList data, QString pos)
{
    QString t = byte;

    if (DeclareRecord(name,type,count,data,pos))
        return;
    if (DeclareClass(name,type,count,data,pos))
        return;




    if (type.toLower()=="integer")
        t = word;
    if (type.toLower()=="byte")
        t = byte;

    if (type.toLower()=="string")
        t = byte;

     if (data.count()==0 && pos!="") {
         Write(name + " = " + pos);
         return;
     }




    if (data.count()==0 && pos=="") {
        int scale = 1;
        if (type.toLower()=="integer")
            scale = 2;
        if (type.toLower()=="long")
            scale = 3;


        Write(getLabelEnding(name) +"\t" + t + "\t ");
        Asm("org "+name+"+" +QString::number(count*scale));

    }
    else {

        QString s="";
        s=s+getLabelEnding(name) + "\t"+t+" ";

        for (int i=0;i<data.count();i++) {
            s=s+data[i];
            if (i%8==7 && i!=data.count()-1) {
                s=s+"\n";
                s=s + "\t" +t + " ";
            }
            else if (i!=data.count()-1) s=s+" ";

        }
        QStringList lst = s.split("\n");
        if (pos=="") {
            for (int i=0;i<lst.count();i++)
                Write(lst[i]);
        }
        else {
            QSharedPointer<Appendix> app = QSharedPointer<Appendix>(new Appendix(pos));
//            app->Append("org " + pos,1);
            app->Append(GetOrg(Util::NumberFromStringHex(pos)),1);
            for (int i=0;i<lst.count();i++)
                app->Append(lst[i],0);

            int p = Util::NumberFromStringHex(pos);

            blocks.append(QSharedPointer<MemoryBlock>(new MemoryBlock(p,p+count, MemoryBlock::ARRAY, name)));

            m_appendix.append(app);
        }

    }
}


void AsmTripe::VarDeclHeader()
{
    m_labelStack["block"].push();
    Asm("jmp "+ getLabel("block"));
}

void AsmTripe::DeclareVariable(QString name, QString type, QString initval, QString position)
{
    QString n = name;
    n=n.toLower();

    QString t = "";
    if (initval=="")
        initval="0";
    if (DeclareRecord(name,type,1,QStringList(),position))
         return;
    if (DeclareClass(name,type,1,QStringList(),position))
         return;

    if (type.toLower()=="const") {
        Write(name + " = " + initval);
        return;
    }


    if (type.toLower()=="integer")  {
        t = word;
    }
    if (type.toLower()=="byte") {
        t = byte;

    }
    if (Syntax::s.m_currentSystem->useZByte) {
        Write(name +"\t=\t"+Util::numToHex0(m_zbyte));
        m_zbyte++;
        if (t==word)
            m_zbyte++;


        return;
    }

    if (t=="")
        ErrorHandler::e.Error("Cannot declare variable of type: " + type);

    Write("decl \t" + name +"\t" + t + ":"+initval);

}

void AsmTripe::DeclareString(QString name, QStringList initval, QStringList flags)
{

    Write(getLabelEnding(name) +"\t" + String(initval,!flags.contains("no_term")));
    m_term="";
}



void AsmTripe::BeginBlock()
{
    Nl();
}


void AsmTripe::EndBlock()
{
    Nl();
    m_labelStack["block"].pop();
}

void AsmTripe::ApplyTerm()
{
    //Asm("sta " + m_currentVar);

    m_currentVar = "";
}

void AsmTripe::Number(QString n)
{

    m_term +=n;
    return;

    if (m_term=="")
        m_term = "lda ";

//    if (m_term.contains("sta"))
        m_term +=n;
  //  else
    //    m_term+="\#" + n;
    if (endTerm()) {
        Asm(m_term);
        ClearTerm();
    }

    //Asm(m_term + n);

}

void AsmTripe::Comment(QString s)
{
    Asm("; "+ s) ;
}

QString AsmTripe::String(QStringList lst, bool term)
{

    QString res;
    QString mark = "dc.b";
    if (Syntax::s.m_currentSystem->CL65Syntax()) {
        mark = ".asciiz";
        term = false;
    }

    for (QString s:lst) {
        bool ok=false;
        uchar val = s.toInt(&ok);
        if (!ok)
            res=res+"\t"+mark+"\t" +"\"" + s + "\"\n";

        else res=res + "\t"+mark+"\t"+QString::number(val) + "\n";

/*        if (s!=lst.last())
            res=res + "\n";
*/

    }
    if (term)
        res=res + "\t"+mark+"\t0";
    m_term +=res;
    return res;
}

/*void AsmTripe::AssignVariable(QString v)
{
    m_currentVar = v;
    m_term="lda ";
}

void AsmTripe::EndAssignVariable(QString variable)
{
    Asm(m_term);
    Asm("sta " + variable);
}
*/
void AsmTripe::BinOP(TokenType::Type t,  bool clearFlag)
{
    if (t == TokenType::PLUS) {
     //   if (clearFlag)
        m_term = "add ";
    }

    if (t == TokenType::MINUS) {
 //       if (clearFlag)
   //     Asm("sec");
        m_term = "sub ";
    }

    if (t == TokenType::BITAND) {
        m_term = "and ";
    }

    if (t == TokenType::BITOR) {
        m_term = "or ";
    }
    if (t == TokenType::XOR) {
        m_term = "xor ";
    }
    if (t == TokenType::MUL) {
        m_term = "mulu ";
    }
    if (t == TokenType::DIV) {
        m_term = "divu ";
    }

}/*


void AsmTripe::Poke(bool start)
{
    //if (start)
     //   m_term = "lda ";
    if (!start)
       m_term = "sta ";
}

void AsmTripe::Peek(bool start)
{
    if (start)
        m_term = "lda ";
   else m_term = "sta ";

}
*/


QString AsmTripe::GetOrg(int pos)
{
    return ".data uint16:" + Util::numToHex(pos);
}

QString AsmTripe::GetOrg() {
    return ".data ";
}

void AsmTripe::Label(QString s)
{
    Write(".label "+s.remove(":"),0);
}

void AsmTripe::Connect()
{
    Assembler::Connect();
    for (QString& l: m_source)
        l.replace("StartBlock",".label StartBlock");
    for (QString& l: m_source)
        l.replace("EndBlock",".label EndBlock");
  //  for (QString& l: m_source)
    //    l.replace("processor",".processor");
    for (QString& l: m_source) {
        l.replace("processor",".processor");
        if (l.endsWith(":"))
            l = l.remove(l.length()-1,1);
    }
    // Delete appendix
    //    qDebug() << "Deleting appendices : "<<m_appendix.count() << m_blockStack.count();


}


void AsmTripe::PopTempVar()
{
    if (m_zpStack.count()>0) {
        m_zpStack.removeLast();
    }
}

