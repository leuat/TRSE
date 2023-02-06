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

#include "asm6809.h"
#include "source/Compiler/syntax.h"


QString Asm6809::m_defaultZeroPointers = "$02, $04, $08, $16, $0B,$0D, $10, $12, $22,$24, $68";
QString Asm6809::m_defaultViaZeroPointers = "$5B, $5C, $5D, $5E";
QString Asm6809::m_defaultTempZeroPointers = "$54, $56, $58, $5A";


Asm6809::Asm6809() :Assembler()
{
//    m_stack["for"] = new Stack();
    m_countCycles = false;
}

Asm6809::~Asm6809() {
}


void Asm6809::Program(QString programName, QString vicConfig)
{


    Nl();

    QString org = Util::numToHex(Syntax::s.m_currentSystem->m_startAddress);
    StartMemoryBlock(org);
    Write(Syntax::s.m_currentSystem->getCPUAssemblerString(),0);

    Syntax::s.m_currentSystem->PrepareInitialAssembler(this);

    m_currentBlock->m_isMainBlock = true;
    m_mainBlock = m_currentBlock;

    Label(programName);
}

void Asm6809::EndProgram()
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


void Asm6809::DeclareArray(QString name, QString type, int count, QStringList data, QString pos)
{
    QString t = byte;

    if (DeclareRecord(name,type,count,data,pos))
        return;
    if (DeclareClass(name,type,count,data,pos))
        return;





    if (type.toLower()=="integer" || type.toLower()=="pointer")
        t = word;
    if (type.toLower()=="byte")
        t = byte;

    if (type.toLower()=="string")
        t = byte;

//    qDebug() << "Declaring array "+name+" of type " + type + " " +t;

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

        if (Syntax::s.m_currentSystem->CL65Syntax()) {
           Write(getLabelEnding(name) +"\t.res\t" + QString::number(count*scale) + ",0");
            return;
        }


        Write(getLabelEnding(name) +"\t" + "fill" + "\t 0,"+QString::number(count));
//        Asm("org "+name+"+" +QString::number(count*scale));

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
            else if (i!=data.count()-1) s=s+",";

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

/*
void Asm6809::VarDeclHeader()
{
    m_labelStack["block"].push();
    Asm("jmp "+ getLabel("block"));
}
*/
void Asm6809::DeclareVariable(QString name, QString type, QString initval, QString position)
{
    QString n = name;
    n=n.toLower();
    if (n=="_a" || n=="_x" || n=="_y" || n=="_ax" || n=="_ay" || n=="_xy")
        return;
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
    if (type.toLower()=="long") {
        t = llong;

    }
    if (Syntax::s.m_currentSystem->useZByte) {
        Write(name +"\t=\t"+Util::numToHex(m_zbyte));
        m_zbyte++;
        if (t==word)
            m_zbyte++;


        return;
    }

/*    if (m_symTab->m_records.contains(type)) {
//        ErrorHandler::e.Error("Record types not implemented yet: " + type);
        QSharedPointer<SymbolTable>  st = m_symTab->m_records[type];
        for (QSharedPointer<Symbol> s : st->m_symbols) {
            //qDebug() << "WTF " <<s->m_name <<s->m_type;
            // Build the name
            QString n = name + "_" + st->m_name+"_"+s->m_name;
            if (s->m_type.toLower()=="byte")
                n = n+ "\t"+byte + "\t0";
            if (s->m_type.toLower()=="integer")
                ErrorHandler::e.Error("Record types does not support integer yet for record : " + type);
            Write(n);

        }
        return;
    }
*/
    if (t=="")
        ErrorHandler::e.Error("Cannot declare variable of type: " + type);

    Write(getLabelEnding(name) +"\t" + t + "\t"+initval);

}

void Asm6809::DeclareString(QString name, QStringList initval, QStringList flags)
{
//    qDebug() << "Asm6809 FLAGS " <<flags <<!flags.contains("no_term");

/*    QSharedPointer<Appendix> app = QSharedPointer<Appendix>(new Appendix(pos));
    app->Append("org " + pos,1);
    for (int i=0;i<lst.count();i++)
        app->Append(lst[i],0);

    int p = Util::NumberFromStringHex(pos);

    blocks.append(QSharedPointer<MemoryBlock>(new MemoryBlock(p,p+count, MemoryBlock::ARRAY, name)));

    m_appendix.append(app);
*/

    Write(getLabelEnding(name) +"\t" + String(initval,!flags.contains("no_term")));
    m_term="";
}

void Asm6809::DeclareCString(QString name, QStringList initVal, QStringList flags)
{
    Write(getLabelEnding(name) + "\t");
    bool done=false;
    int curIdx=0;
    int curLin = 0;
    int curOut = 0;
    if (initVal.count()==0) {
        Write("\t"+byte+"\t");
        return;
    }
//    qDebug() << initVal.count();
    QString curStr = initVal[curLin];
    QString curOutData = "";
    while (!done) {
        // First check if current is a pure number
        if (curOutData == "")
            curOutData = "\t"+byte+"\t";

        bool isNumber = false;
        if (curStr.startsWith("*&NUM")) {
            curStr = curStr.remove("*&NUM"); // Remove the number tag
            int val = curStr.toInt(&isNumber);
            if (flags.contains("invert")) val |= +128;
            if (isNumber) {
                //Write(" dc.b " + curStr);
                //qDebug() << "IS NUMBER : " << isNumber;
                curOutData += curStr;
                curIdx = curStr.length();
  //              continue;
            }
        }

        if (!isNumber)

            if (curIdx<curStr.length()) {

                QString c = curStr[curIdx].toUpper();
                //qDebug() << c;
//                qDebug() << curStr << m_cstr.keys();

            }

        if (curOut++<8)
            curOutData+=", ";
        else {
            curOut=0;
            Write(curOutData);
            curOutData = "";
        }

        curIdx++;
        if (curIdx>=curStr.length()) {
            curIdx=0;
            curLin++;
//            qDebug() << curLin;
            if (curLin<initVal.count()) {
                curStr=initVal[curLin];
  //              qDebug() << curStr;
            }
            else done=true;
        }


    }



    if (curOut!=0) {
        if (!flags.contains("no_term"))
            Write(curOutData + "0"); // add 0
        else {
            curOutData.remove(curOutData.length()-2,1);// Remove final ","
            Write(curOutData);
        }
    }

    else
    if (!flags.contains("no_term"))
        Write("\tdc.b\t0 "); // add 0

}


void Asm6809::BeginBlock()
{
    Nl();
}


void Asm6809::EndBlock()
{
    Nl();
    m_labelStack["block"].pop();
}

void Asm6809::ApplyTerm()
{
    //Asm("sta " + m_currentVar);

    m_currentVar = "";
}

void Asm6809::Number(QString n)
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

void Asm6809::Comment(QString s)
{
    Asm("; "+ s) ;
}

QString Asm6809::String(QStringList lst, bool term)
{

    QString res;
    QString mark = "dc.b";
    if (Syntax::s.m_currentSystem->is6809())
        mark = "fcc";
    if (Syntax::s.m_currentSystem->CL65Syntax()) {
        mark = ".asciiz";
        term = false;
    }

    for (QString s:lst) {
        bool ok=false;
        uchar val = s.toInt(&ok);
        if (!ok)
            res=res+"\t"+mark+"\t" +"\"" + s + "\"\n";

        else res=res + "\t"+"fcb"+"\t"+QString::number(val) + "\n";

/*        if (s!=lst.last())
            res=res + "\n";
*/

    }
    if (term)
        res=res + "\t"+byte+"\t0";

    m_term +=res;
    return res;
}

/*void Asm6809::AssignVariable(QString v)
{
    m_currentVar = v;
    m_term="lda ";
}

void Asm6809::EndAssignVariable(QString variable)
{
    Asm(m_term);
    Asm("sta " + variable);
}
*/
void Asm6809::BinOP(TokenType::Type t,  bool clearFlag)
{
    if (t == TokenType::PLUS) {
        if (clearFlag)
        m_term = "adda ";
    }

    if (t == TokenType::MINUS) {
        if (clearFlag)
        m_term = "suba ";
    }

    if (t == TokenType::BITAND) {
        m_term = "anda ";
    }

    if (t == TokenType::BITOR) {
        m_term = "ora ";
    }
    if (t == TokenType::XOR) {
        m_term = "eora ";
    }

}

void Asm6809::BinOP16(TokenType::Type t, bool clearFlag)
{
    if (t == TokenType::PLUS) {
        if (clearFlag)
        m_term = "addd ";
    }

    if (t == TokenType::MINUS) {
        if (clearFlag)
        m_term = "subd ";
    }

    if (t == TokenType::BITAND) {
        m_term = "andd ";
    }

    if (t == TokenType::BITOR) {
        m_term = "ord ";
    }
    if (t == TokenType::XOR) {
        m_term = "eord ";
    }

}




/*


void Asm6809::Poke(bool start)
{
    //if (start)
     //   m_term = "lda ";
    if (!start)
       m_term = "sta ";
}

void Asm6809::Peek(bool start)
{
    if (start)
        m_term = "lda ";
   else m_term = "sta ";

}
*/
QString Asm6809::PushZeroPointer()
{
    QString zp = m_zeroPointers[m_curZeroPointer];
    m_curZeroPointer++;
    return zp;
}

void Asm6809::PopZeroPointer()
{
    if (m_curZeroPointer<=0)
        ErrorHandler::e.Error("Zero pointer cannot be pushed below zero");

    m_curZeroPointer--;
}

bool Asm6809::CheckZPAvailability()
{
    if (m_curZeroPointer<m_zeroPointers.count())
        return true;
    return false;
}

QString Asm6809::GetOrg(int pos)
{
    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::SNES)
        return "";
    if (Syntax::s.m_currentSystem->CL65Syntax())
//        return ".res " + Util::numToHex(pos)+"-*";
      return ".org " + Util::numToHex(pos);
    return "org " + Util::numToHex(pos);
}

QString Asm6809::GetOrg() {
    if (Syntax::s.m_currentSystem->CL65Syntax())
        return ".org ";
    return "org ";
}

void Asm6809::DeclareInternalVariable(QString name)
{
    if (m_varZeroPointers.count()!=0) {
        QString addr = m_varZeroPointers[0];
        m_varZeroPointers.removeFirst();
//        m_tempVars << (name + " = " + addr);
        Write(name + " = " + addr);
        return;
    }
    Write(name + ": "+byte+" 0");
//    m_tempVars << (name + " dc.b 0");
}

int Asm6809::CodeSizeEstimator(QStringList &lines) {
    int size = 0;
 //   qDebug() << "EST START";
    for (auto&s : lines) {
        if (!s.startsWith("\t")) // IS label
            continue;
        QString t = s.trimmed().simplified().split(";").first();
        int add = 0;
        if (t=="")
            continue;
        QStringList sp1 = t.split(" ");
        if (sp1.count()==1) {
            add=1;
        }
        if (sp1.count()>=2) {
            QString p = sp1[1];
            QStringList ps = p.split(",");
            p = ps[0]; // pick first one, ignore ",x" etc
            if (p.startsWith("#")) { add=2; }
            else add=3;
//            else size+=3;
/*            p = p.remove("$");
            bool conversionOk = false;
            int value = p.toInt(&conversionOk, 16);
            if (conversionOk)*/

        }
//        qDebug() << t << add;
        size+=add;

    }
//    if (size>100)
//        qDebug() << "********************** EST SIZE "<< size;
    return size;
}

QString Asm6809::StoreInTempVar(QString name, QString type, bool actuallyStore)
{
   // qDebug() << "Using reglar variables: " << m_zpStack.count();
    QString tmpVar = NewLabel(name+"_var");
    QString labelVar = getLabelEnding(tmpVar) + "\t."+type+"\t0 ";
    m_tempVars << labelVar;
    if (actuallyStore) {

        if (type=="word")
            Asm("sty " + tmpVar);
        else
            Asm("sta " + tmpVar);

    }
    PopLabel(name+ "_var");
    return tmpVar;
}

void Asm6809::PopTempVar()
{
    if (m_zpStack.count()>0) {
        m_zpStack.removeLast();
    }
//    else
  //      ErrorHandler::e.Error("COMPILER ERROR: Assembler::PopTempVar Trying to pop tempvar from zero");
}

/*
void Asm6809::Writeln()
{
    m_labelStack["writeln"].push();
    QString l = "L" + getLabel("writeln");
    Asm("LDY #$00");
    Label(l+"0");
    Asm("LDA "+l+"1,Y");
    Asm("CMP #0");
    Asm("BEQ "+l+"2");
    Asm("JSR $FFD2");
    Asm("INY");
    Asm("JMP "+l+"0");
    Label(l+"1");
    m_term = "";

}

void Asm6809::EndWriteln()
{
    Asm(m_term + ",0");
    Label("L"+getLabel("writeln")+"2");
    m_labelStack["writeln"].pop();

}
*/
void Asm6809::LoadVariable(QString var)
{
    Asm("lda "+ var);
}

void Asm6809::Variable(QString v, bool isByte)
{
    if (isByte) {
        if (m_term=="")
            m_term = "lda ";
        m_term+=v;
    }
    else {
        Comment("integer assignment NodeVar");
        Term("ldy "+v);

    }
}



/*
void Asm6809::StartForLoop(QString var, QString startVal)
{
}

void Asm6809::EndForLoop(QString endVal)
{
    m_stack["for"].pop();
    m_labelStack["forLoopFix"].push();
    m_labelStack["forLoopDone"].push();
    QString label2 = getLabel("forLoopFix");
    QString labelDone = getLabel("forLoopDone");

    Asm("inc " + m_stack["for"].current());
    Asm("lda "+m_stack["for"].current());

//    if (Syntax::s.isNumeric(endVal))
//        endVal = "#" + endVal;
    Asm("cmp " + endVal);
    Asm("bne "+label2);
    Asm("jmp "+labelDone);
    Label(label2);
    Asm("jmp " + getLabel("for"));

    Label(labelDone);
    m_labelStack["for"].pop();
    m_labelStack["forLoopFix"].pop();
    m_labelStack["forLoopDone"].pop();

 //   qDebug() << label2;
  //  qDebug() << labelDone;



 //   qDebug() << "loop: " << m_stack["for"].current();

}

*/
// 8790 vs 8717
void Asm6809::Optimise(CIniFile& ini)
{

    if (ini.getdouble("post_optimizer_passldatax")==1)
        OptimisePassLdaTax("x");


    if (ini.getdouble("post_optimizer_passldatax")==1)
        OptimisePassLdaTax("y");


    if (ini.getdouble("post_optimizer_passstalda")==1) {
        OptimisePassStaLda();
        if (Syntax::s.m_currentSystem->m_processor==AbstractSystem::M6809)
            OptimisePassStaLda2();
    }

    if (ini.getdouble("post_optimizer_passldx")==1)
        OptimisePassLdx("x");

    if (ini.getdouble("post_optimizer_passldx")==1)
        OptimisePassLdx("y");



    if (ini.getdouble("post_optimizer_passlda")==1)

        OptimisePassLdx("a");

    if (ini.getdouble("post_optimizer_passlda")==1)

        OptimisePassLdx("y");

    if (ini.getdouble("post_optimizer_passjmp")==1)
        OptimiseJumps();

//    if (ini.getdouble("post_optimizer_passcmp")==1)
    if (ini.getdouble("post_optimizer_passcmp")==1)
        OptimiseCmp("cmp");
            // 6157
    if (ini.getdouble("post_optimizer_passphapla")==1)
        OptimisePhaPla1();

    OptimisePhaLdxPla();
    OptimiseLdLd();

    OptimisePassStaLdx("x");
    OptimisePassStaLdx("y");

    OptimisePassLdyLdy("y");
    OptimisePassLdyLdy("x");


//    if (ini.getdouble("post_optimizer_passldatax")==1)
  //      RemoveUnusedLabels();


//        OptimisePhaPla2();
  //      OptimiseCmp("cpy");
  //      OptimiseCmp("cpx");

}


void Asm6809::OptimisePassStaLda()
{
    m_removeLines.clear();
    int j;
    for (int i=0;i<m_source.count()-1;i++) {
        QString l0 = getLine(i);
        if (l0.contains("sta ")) {
            QString l1 = getNextLine(i,j);
            if (l0==l1 && !
                    l1.contains("keep")) {

//                qDebug() << "Removing: " << l0 << l1 << " on line " << j;

                m_removeLines.append(j);
                continue;
            }
            QString var = getToken(l0,1);
            if (getToken(l1,1)==var && getToken(l1,0)=="lda") {

//                qDebug() << "Removing: " << l0 << l1<< " on line " << j;
                m_removeLines.append(j);
                i++;
                continue;
            }
        }
    }
    RemoveLines();
}

void Asm6809::OptimisePassStaLdx(QString x)
{
    m_removeLines.clear();
    int j;
    for (int i=0;i<m_source.count()-1;i++) {
        QString l0 = getLine(i);
        if (l0.contains("sta")) {
            QString l1 = getNextLine(i,j);
/*            if (l0==l1) {
                //qDebug() << "Removing " <<l0;
                m_removeLines.append(j);
                continue;
            }*/
            QString var = getToken(l0,1);
            if (getToken(l1,1)==var && getToken(l1,0)=="ld"+x) {

              //  qDebug() << "Removing: " << l1 << " on line " << j;
//                m_removeLines.append(j);
                m_source[j] = "\tta"+x+" ; optimized "+x+", look out for bugs L22 ORG " + m_source[j];

                i++;
                continue;
            }
        }
    }
    RemoveLines();
}



void Asm6809::OptimisePassLdx(QString x)
{

    bool allDone = false;
    while (!allDone) {

        m_removeLines.clear();
        int j;
        for (int i=0;i<m_source.count()-1;i++) {
            QString l0 = getLine(i);
            QString value = getToken(l0,1);
            if (l0.contains("ld"+x)) {
                bool done = false;
//                qDebug() << l0;
                int k=i;
                int curCnt = 0;
                int ll=i;
                while (!done) {
                    QString l1 = getNextLine(k,j);
                    k=j;
                    QString op2 = getToken(l1,1);
                    QString op = getToken(l1,0);
  //                  qDebug() << op2 << op << l0 << l1<<value;
                    if (l0==l1 && !op2.startsWith("(") && !op2.contains(",") && !op2.startsWith("$")) {
//                      if (l0==l1 && !op2.startsWith("(") && !op2.contains(",")) {
//                        if (x=="x")
//                           qDebug () << "Removing because equal: " << l0 << ", " << l1 << "  on line " <<i;
                        if (!l1.contains(";keep"))
                            m_removeLines.append(j);
                        curCnt++;
                        continue;
                    }
                    //qDebug() << "curop:" << op;
                    // Changex in x
                    if (x=="x" || x=="y")
                    if (op==("ld"+x) || op==("ta"+x) || op=="jmp" || op=="rts" || op=="jsr" ||
                            op==("in" +x) || op==("de"+x)|| op.length()!=3)
//                        op==("in" +x) || op==("de"+x)|| op==("sta")|| op.length()!=3 ) {
//                        qDebug() << "Done because: " << l1;
                        done=true;
                    // Stuff like
                    // ldx i
                    // inc i
                    // ldx i

                    if (x=="a") {
                        if (l0.contains(",y"))
                            if (op == "iny" || op == "ldy")
                                done = true;
                        if (l0.contains(",x"))
                            if (op == "inx" || op == "ldx")
                                done = true;
                    }

                    if ((x=="x" || x=="y" )) {
                        if (op=="inc" || op=="dec" || op=="sta") {
                            if (op2==value)
                                done = true;
                        }
                    }


                    ll++;
                     if (x=="a") {
                        if (op==("lda") || op==("txa") || op==("tya") || op=="jmp" || op=="rts" || op=="jsr" ||
                                op==("ina") || op==("dea")|| op.length()!=3  || op==("adc") || op==("sbc") || op==("cmp")
                                || op=="clc" || op=="sec" || op=="asl" || op=="rol" || op=="ror" || op=="lsr"
                                || op=="bcc" || op=="bcs" || op=="pla" || op=="beq" || op=="bpl" || op=="bne"
                                || op=="and" || op=="ora" || op=="eor" || op=="bit"
                                )

//                          if (!(op[2] == "x" || op[2] == "y" || op[2]=="sta"))

                        done = true;
/*                        if (curCnt!=0 && done==true) {
                            for (int kk=i;kk<ll;kk++) {
                                QString ss =QString::number(kk) +("   " + l0)+ " : " +getLine(kk);// << "  :  " << l1;
                                if (kk-i==curCnt-1)
                                    ss+="  << HER";
                                qDebug() << ss;
                            }

                            qDebug() << "**** DONE";
                     }*/

                     }


                }
                if (m_removeLines.count()!=0)
                    break;
            }

        }
        if (m_removeLines.count()==0)
            allDone = true;

        RemoveLines();
    }
}

void Asm6809::OptimisePassLdaTax(QString x)
{
    m_removeLines.clear();
    int j;
    for (int i=0;i<m_source.count()-1;i++) {
        QString l0 = getLine(i);
        if (l0.contains("lda") && !l0.contains(",")) {
            int k=i;
            int n;
                QString l1 = getNextLine(k,j);
                QString l2 = getNextLine(j,n);
                k=j;
                QString op = getToken(l1,0);
                if (op==("ta"+x) && !l1.contains(";keep")) {
                    bool perform=true;
                    QString tst = getToken(l2,0);
                    if (tst.startsWith("ta") || tst=="sta")
                        perform=false;
//                    qDebug() << "TEST " << perform << tst ;
                    if (perform) {
                        m_removeLines.append(j);
                        QString org = m_source[i];
                        m_source[i] = m_source[i].replace("lda", "ld"+x)+" ; optimized, look out for bugs";
  //                      qDebug() << "Changed : "<< org << " to " <<m_source[i] << " with op " <<op;
                    }
                    //m_removeLines.append(i);



            }
//            if (m_removeLines.count()!=0)
  //              break;
        }
    }

    RemoveLines();

}


void Asm6809::OptimiseJumps()
{
    m_removeLines.clear();
    int j;
    for (int i=0;i<m_source.count()-1;i++) {
        QString l0 = getLine(i);
        if (l0.contains("jmp ")) {
            QString l1 = getNextLine(i,j);
            QString lbl0 = getToken(l0, 1);
            //qDebug() << lbl0 << " vs " << l1 ;
            if (l1.toLower().contains(lbl0.toLower()) && !l1.toLower().contains("keep"))
            {
//                qDebug() << "Removing:";
 //               qDebug() << " " + l0;
  //              qDebug() << " " + l1;
                m_removeLines.append(i);
                //m_removeLines.append(j);
                i++;
                continue;
            }
        }
    }
    RemoveLines();

}

void Asm6809::OptimiseCmp(QString op)
{
    m_removeLines.clear();
    for (int i=0;i<m_source.count()-1;i++) {
        QString l0 = getLine(i).toLower();
        if (l0.contains(op+ " ") && !l0.contains(";keep")) {



            QString lbl0 = getToken(l0, 1);
            if (lbl0.startsWith("#")) {
                lbl0 = lbl0.remove(0,1);
                int num = Util::NumberFromStringHex(lbl0);
                if (num==0) {
                    m_removeLines.append(i);
                   // qDebug() << "REmoving: " << l0;
                }
            }
        }
    }
    RemoveLines();

}

void Asm6809::OptimisePhaPla1()
{

    /*
     * Removes :
     *   pha
     *   tax
     *   pla
     *
     * */

    m_removeLines.clear();
    int j,k;
    for (int i=0;i<m_source.count()-1;i++) {
        QString l0 = getLine(i).toLower().trimmed();
        if (l0 == "pha") {
            QString l1 = getNextLine(i,j);
            if (l1.toLower().trimmed()=="tax") {
                l1= getNextLine(j,k);
                if (l1.toLower().trimmed()=="pla") {
                    m_removeLines.append(i);
                    m_removeLines.append(k);
                }
            }
        }
    }
    RemoveLines();

}

/*
 * Removes:
 *
 * pha
 *
 * ..
 *
 * pla
 * lda ...
 *
 * */
void Asm6809::OptimisePhaPla2()
{
    m_removeLines.clear();
    int j,k;
    for (int i=0;i<m_source.count()-1;i++) {
        QString l0 = getLine(i).toLower().trimmed();
        if (l0 == "pha") {
            QString l1="";
            k=i;
         //   qDebug() << " START ************************";
            while (!(l1=="pla" || l1=="pha")) {
                j=k;
                l1 = getNextLine(j,k).toLower().trimmed();
                //qDebug() << l1;
            }
           // if (l1=="pha") {i=j;continue;}

            j=k;
            if (getNextLine(j,k).toLower().trimmed().startsWith("lda ")) {
               //     m_removeLines.append(two);
                 //   m_removeLines.append(i);
                    //qDebug() << "Removing: "<< m_source[two];
                    //qDebug() << "Removing: "<< m_source[i];
                    //qDebug() << getNextLine(j,k).toLower().trimmed();
            }

            i=k;
        }
    }
    RemoveLines();

}

void Asm6809::OptimisePhaLdxPla()
{

    m_removeLines.clear();
    int j,k;
    for (int i=0;i<m_source.count()-1;i++) {
        QString l0 = getLine(i).toLower().trimmed();
        if (l0 == "pha") {
            QString l1 = getNextLine(i,j);
            if (l1.toLower().trimmed().startsWith("ldx") || l1.toLower().trimmed().startsWith("ldy")) {
                l1= getNextLine(j,k);
                if (l1=="pla") {
                    m_removeLines.append(i);
                    m_removeLines.append(k);
                }
            }
            else
            if (l1=="pla") {
               m_removeLines.append(i);
               m_removeLines.append(j);
//               qDebug() << l0 << l1;
            }
        }
    }
    RemoveLines();
}


void Asm6809::OptimiseLdLd()
{
    m_removeLines.clear();
    int j,k;
    for (int i=0;i<m_source.count()-1;i++) {
        QString l0 = getLine(i).toLower().trimmed();
        if (l0.startsWith("ld")) {
            QString a = QString(l0[2]);
            QString l1 = getNextLine(i,j);
            if (l1.toLower().trimmed().startsWith("ld"+a)) {
               m_removeLines.append(i);
            }
        }
    }
    RemoveLines();

}


bool Asm6809::ContainsAChangingOpcodes(QString l1) {
    l1 = l1.trimmed().toLower();
    return    l1.startsWith("txa") || l1.startsWith("tya")
            || l1.startsWith("lda") || l1.startsWith("pla")
            || l1.startsWith("ina") || l1.startsWith("dea")
            || l1.startsWith("adc") || l1.startsWith("sbc")
            || l1.startsWith("asl") || l1.startsWith("lsr")
            || l1.startsWith("and")
            || l1.startsWith("ror") || l1.startsWith("rol") ||
            l1.startsWith("jsr") || l1.startsWith("rts");

}

bool Asm6809::ContainsYUsingOpcodes(QString l1,QString y)
{
    l1 = l1.trimmed().toLower();
    return  l1.startsWith("t"+y+"a") || l1.startsWith("st"+y) || l1.contains(","+y) || l1.contains(", "+y)  || ContainsBranches(l1);;

}

bool Asm6809::ContainsYChangingOpcodes(QString l1,QString y)
{
    l1 = l1.trimmed().toLower();
    return  l1.startsWith("ta"+y) || l1.startsWith("ld"+y) || ContainsBranches(l1);

}

bool Asm6809::ContainsBranches(QString l1)
{
    l1 = l1.trimmed().toLower();
    return
            l1.startsWith("jsr") || l1.startsWith("rts")
            || l1.startsWith("bcc") || l1.startsWith("bcs")
            || l1.startsWith("bpl") || l1.startsWith("bmi")
            || l1.startsWith("bvs") || l1.startsWith("bne")
            || l1.startsWith("jmp") || l1.startsWith("bra")
            || l1.startsWith("beq") || l1.startsWith("bvc");

}

int Asm6809::CountInstructionCycle(QStringList s)
{
    MOSOperation op = GetOperand(s);
    return CalculateCycles(op);

}

MOSOperation Asm6809::GetOperand(QStringList s)
{
    MOSOperation op;
    op.operand = s[0].toLower();
    if (s.count()==1)
        return op;
    QString data = s[1].toLower();
    if (data.contains("(")) {
        op.isZeroPage = true;
        data = data.remove("(");
        data = data.remove(")");
    }

    QStringList p2 = data.split(",");
    op.param1 = p2[0];
    if (p2.count()==2)
        op.param2 = p2[1];


    if (op.param1.startsWith("#"))
        op.isNumeric = true;

/*    op.param1 = op.param1.remove("#");
    op.param1 = op.param1.remove("$");
    op.param1 = op.param1.remove("%");

    int i = op.param1.toInt(&ok);
    if (ok) {
        op.isNumeric = true;
        if (i>255)
            op.is16bit = true;
    }
*/
    return op;

}

int Asm6809::CalculateCycles(MOSOperation op)
{
    if (op.operand.contains("\t"))
        return 0;
    return 0;
//    if (!m_opCycles.contains(op.operand)) {
        //qDebug() << "Error: could not count operands for type:" << op.operand;
    //    return 0;
  //  }
/*    MOSOperandCycle oc = m_opCycles[op.operand];
    // is immediate
    if (op.param1=="")
        return oc.m_implied;

    if (op.param2!="") {
        if (op.isZeroPage)
            return oc.m_zeropageWithParam;
        return oc.m_absoluteWithParam;
    }
    if (op.isZeroPage)
        return oc.m_zeropage;

    if (op.isNumeric)
        return oc.m_immediate;

    return oc.m_absolute;
    */

}



void Asm6809::InitZeroPointers(QStringList lst, QStringList tmpList, QStringList varList)
{
    m_zeroPointers.clear();
    for (QString s: lst) {
        QString test=s;
        test = test.replace("$", "");
        bool ok = true;
//        int testVal = test.toInt(&ok);
  //      if (!ok)
    //        qDebug() << "NOT OK for " << s << " " << test;
        if (s!="" && ok)
            m_zeroPointers.append(s);
    }
    m_tempZeroPointers.clear();

    for (QString zp : tmpList) {
        if (zp!="") {
            m_tempZeroPointers.append(zp);
        }
    }
    for (QString zp : varList)
        if (zp!="") {
            m_varZeroPointers.append(zp);
        }
//       qDebug() << "Asm6809 initzero " <<m_tempZeroPointers;


}

// Optimises: "sta p1   lda p1"
void Asm6809::OptimisePassStaLda2()
{
    m_removeLines.clear();
    int j;
    QString curA ="";
    for (int i=0;i<m_source.count()-1;i++) {
        QString l0 = getLine(i);
        if (l0.contains("sta ") && !l0.contains(",") && !nextLineIsLabel(i)) {
            curA = l0.split(" ")[1];
            QString l1 = getNextLine(i,j);
        //    qDebug() << "FOUND sta "<<l0;
            bool done = false;
            int old_j = -1;
            while (!done) {

/*                if (op==("lda") || op==("txa") || op==("tya") || op=="jmp" || op=="rts" || op=="jsr" ||
                        op==("ina") || op==("dea")|| op.length()!=3  || op==("adc") || op==("sbc") || op==("cmp")
                        || op=="clc" || op=="sec" || op=="asl" || op=="rol" || op=="ror" || op=="lsr"
                        || op=="bcc" || op=="bcs" || op=="pla" || op=="beq" || op=="bpl" || op=="bne"
                        || op=="and" || op=="ora" || op=="eor" || op=="bit"
  */
                bool abort = ContainsAChangingOpcodes(l1) & !nextLineIsLabel(j);
;
//                qDebug() << "NEXT IS LABEL " <<nextLineIsLabel(j);
                old_j=j;
                while ((done==false && !l1.startsWith("lda")) && !abort && !nextLineIsLabel(j)) {
                    l1 = getNextLine(j,j);
//                    qDebug() << l1;
                    if (!l1.startsWith("lda")) {
                        abort = ContainsAChangingOpcodes(l1);
                        if (!abort) {
                            abort = nextLineIsLabel(j);
                   //         if (abort)
                     //           qDebug() << "ABORTING NEXT LINE IS LABEL "<<l1;

                        }
                       /* if (abort) {
                            qDebug() << "ABORTING because "<<l1;
                        }*/
                    }
//                    qDebug() << j <<old_j<<m_source.count();
                    if (j>=m_source.count()-2 || old_j==j)
                        done = true;
                    if (old_j==j)
                        abort = true;
                    old_j = j;
                }
                done = true;
                if (!abort) {
                    QStringList lst = l1.split(" ");
                    if (lst.count()>=2)
                        if (lst[0] == "lda" && lst[1]==curA &&!l1.contains("keep")) {
                      //      qDebug() << "Removing: " << l1 << " on line " << j << lst[0] << lst[1] << curA;
                            m_removeLines.append(j);
                            //                    done = false;


                        }
                }
            }
        }
    }
    RemoveLines();
}

// Optimises: "sta p1   lda p1"
void Asm6809::OptimisePassLdyLdy(QString y)
{
    m_removeLines.clear();
    int j;
    QString curA ="";
    QString lda = "ld"+y;
    for (int i=0;i<m_source.count()-1;i++) {
        QString l0 = getLine(i);
        if (l0.contains(lda+" ") && !l0.contains(",") && !l0.contains(";keep")) {
            curA = l0.split(" ")[1];
            bool done = false;
            bool deleteme = true;
            j=i;
            while (!done) {
                QString l1 = getNextLine(j,j);

                if (ContainsYUsingOpcodes(l1,y) || nextLineIsLabel(j) || j>=m_source.count()) {
                        done = true;
                        deleteme = false;
                   }
                if (ContainsYChangingOpcodes(l1,y)) {
                    done = true;
                }
            }
            if (deleteme)
                m_removeLines.append(i);

        }
    }
    RemoveLines();
}
