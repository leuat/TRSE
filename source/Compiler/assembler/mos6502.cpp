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

#include "mos6502.h"
#include "source/Compiler/syntax.h"

QString AsmMOS6502::m_defaultZeroPointers = "$02, $04, $08, $16, $0B,$0D, $10, $12, $22,$24, $68";
QString AsmMOS6502::m_defaultTempZeroPointers = "$54, $56, $58, $5A";


AsmMOS6502::AsmMOS6502() :Assembler()
{
//    m_stack["for"] = new Stack();
    InitMosOpCycles();
    InitCStrings();
}

void AsmMOS6502::InitCStrings()
{
    m_cstr.clear();
    m_cstr[" "] = CStringItem("32", 32,32);
    m_cstr["!"] = CStringItem("!", 33,33);
    m_cstr[""""] = CStringItem("""", 34,34);
    m_cstr["#"] = CStringItem("#", 35,35);
    m_cstr["$"] = CStringItem("$", 36,36);
    m_cstr["%"] = CStringItem("%", 37,37);
    m_cstr["&"] = CStringItem("&", 38,38);
    m_cstr["'"] = CStringItem("'",39 ,39);
    m_cstr["("] = CStringItem("(",40 ,40);
    m_cstr[")"] = CStringItem(")",41 ,41);
    m_cstr["*"] = CStringItem("*", 42,42);
    m_cstr["+"] = CStringItem("+", 43,43);
    m_cstr[","] = CStringItem(",", 44,44);
    m_cstr["-"] = CStringItem("-", 45,45);
    m_cstr["."] = CStringItem(".",46 ,46);
    m_cstr["/"] = CStringItem("/", 47,47);
    m_cstr["0"] = CStringItem("0", 48,48);
    m_cstr["1"] = CStringItem("1", 49,49);
    m_cstr["2"] = CStringItem("2", 50,50);
    m_cstr["3"] = CStringItem("3", 51,51);
    m_cstr["4"] = CStringItem("4", 52,52);
    m_cstr["5"] = CStringItem("5", 53,53);
    m_cstr["6"] = CStringItem("6", 54,54);
    m_cstr["7"] = CStringItem("7", 55,55);
    m_cstr["8"] = CStringItem("8", 56,56);
    m_cstr["9"] = CStringItem("9", 57,57);
    m_cstr[":"] = CStringItem(":", 58,58);
    m_cstr[";"] = CStringItem(";", 59,59);
    m_cstr["<"] = CStringItem("<", 60,60);
    m_cstr["="] = CStringItem("=", 61,61);
    m_cstr[">"] = CStringItem(">", 62,62);
    m_cstr["?"] = CStringItem("?", 63,63);
    m_cstr["@"] = CStringItem("@", 64,64 -64);
    m_cstr["A"] = CStringItem("A", 65,65-64);
    m_cstr["B"] = CStringItem("B", 66,66-64);
    m_cstr["C"] = CStringItem("C", 67,67-64);
    m_cstr["D"] = CStringItem("D", 68,68-64);
    m_cstr["E"] = CStringItem("E", 69,69-64);
    m_cstr["F"] = CStringItem("F", 70,70-64);
    m_cstr["G"] = CStringItem("G", 71,71-64);
    m_cstr["H"] = CStringItem("H", 72,72-64);
    m_cstr["I"] = CStringItem("I", 73,73-64);
    m_cstr["J"] = CStringItem("J", 74,74-64);
    m_cstr["K"] = CStringItem("K", 75,75-64);
    m_cstr["L"] = CStringItem("L", 76,76-64);
    m_cstr["M"] = CStringItem("M", 77,77-64);
    m_cstr["N"] = CStringItem("N", 78,78-64);
    m_cstr["O"] = CStringItem("O", 79,79-64);
    m_cstr["P"] = CStringItem("P", 80,80-64);
    m_cstr["Q"] = CStringItem("Q", 81,81-64);
    m_cstr["R"] = CStringItem("R", 82,82-64);
    m_cstr["S"] = CStringItem("S", 83,83-64);
    m_cstr["T"] = CStringItem("T", 84,84-64);
    m_cstr["U"] = CStringItem("U", 85,85-64);
    m_cstr["V"] = CStringItem("V", 86,86-64);
    m_cstr["W"] = CStringItem("W", 87,87-64);
    m_cstr["X"] = CStringItem("X", 88,88-64);
    m_cstr["Y"] = CStringItem("Y", 89,89-64);
    m_cstr["Z"] = CStringItem("Z", 90,90-64);
    m_cstr["["] = CStringItem("",91, 91-64);
    m_cstr["£"] = CStringItem("",92,92-64);
    m_cstr["]"] = CStringItem("",93,93-64);
//    m_cstr[""] = CStringItem("", ,);

}

void AsmMOS6502::Program(QString programName)
{
    Asm("processor 6502");
    Nl();
    if (Syntax::s.m_currentSystem==Syntax::C64) {
        Asm("ORG $0801");
        // 2064
        Asm(".byte    $0E, $08, $0A, $00, $9E, $20, $28, $32, $30"); //  ( 2,0
        Asm(".byte    $36, $34, $29, $00, $00, $00");   // 6, 4, )*/
        Nl();
        Asm("ORG $0810");
        Nl();
    }
    if (Syntax::s.m_currentSystem==Syntax::VIC20) {
        Asm("  ORG $1001");
        // 4112
        Asm(".byte    $0E, $08, $0A, $00, $9E, $20, $28, $34, $31"); //  ( 2,0
        Asm(".byte    $31, $32, $29, $00, $00, $00");   // 6, 4, )*/

        Nl();
        Asm("ORG $1010");
        Nl();

/*        Asm("   ORG $1001");
        Asm("   dc.w basicEnd    ; Next Line link, here end of Basic program");
        Asm("            dc.w 2013        ; The line number for the SYS statement");
        Asm("            dc.b  $9e         ; SYS token");
        Asm("            dc.b  \" \"");
        Asm("            dc.b  \"4110\"");
        Asm("            dc.b  0  ");
        Asm("basicEnd    dc.w  0         ");
*/
    }
    Label(programName);
}

void AsmMOS6502::EndProgram()
{
    Asm("rts");
}

void AsmMOS6502::DeclareArray(QString name, QString type, int count, QStringList data, QString pos)
{
    QString t = byte;
    if (type.toLower()=="integer")
        t = word;
    if (type.toLower()=="byte")
        t = byte;

    if (type.toLower()=="string")
        t = byte;


    if (data.count()==0 && pos=="") {
        Write(name +"\t" + t + "\t ");
        Asm("org "+name+"+" +QString::number(count));

    }
    else {
        QString s="";
        s=s+name + "\t"+t+" ";

        for (int i=0;i<data.count();i++) {
            s=s+data[i];
            if (i%8==7) {
                s=s+"\n";
                s=s + "\t" +t + " ";
            }
            else s=s+", ";

        }
        QStringList lst = s.split("\n");
        if (pos=="") {
            for (int i=0;i<lst.count();i++)
                Write(lst[i]);
        }
        else {
            Appendix app(pos);
            app.Append("org " + pos,1);
            for (int i=0;i<lst.count();i++)
                app.Append(lst[i],0);

            int p = Util::NumberFromStringHex(pos);

            blocks.append(new MemoryBlock(p,p+count, MemoryBlock::ARRAY, name));

            m_appendix.append(app);
        }

    }
}


void AsmMOS6502::VarDeclHeader()
{
    m_labelStack["block"].push();
    Asm("jmp "+ getLabel("block"));
}

void AsmMOS6502::DeclareVariable(QString name, QString type, QString initval)
{
    QString t = "";
    if (type.toLower()=="integer")
        t = word;
    if (type.toLower()=="byte")
        t = byte;

    if (t=="")
        ErrorHandler::e.Error("Cannot declare variable of type: " + type);

    Write(name +"\t" + t + "\t"+initval);

}

void AsmMOS6502::DeclareString(QString name, QStringList initval)
{
    Write(name +"\t" + String(initval));
    m_term="";
}

void AsmMOS6502::DeclareCString(QString name, QStringList initVal)
{
    Write(name + "\t");
    bool done=false;
    int curIdx=0;
    int curLin = 0;
    int curOut = 0;
    QString s ="\tdc.b\t";
    if (initVal.count()==0) {
        Write(s);
        return;
    }
//    qDebug() << initVal.count();
    QString curStr = initVal[curLin];
    while (!done) {

        // First check if current is a pure number

/*        if (curIdx==0) {
            QString cc = curStr[curIdx];
            if (cc.count()<=3 && Syntax::s.isDigitHex(cc)) {
                s+=cc + ", ";
                curIdx = cc.length();
            }

        }*/
        if (curIdx<curStr.length()) {
            QString c = curStr[curIdx].toUpper();
            //qDebug() << c;
            if (m_cstr.contains(c)) {
                uchar sc = m_cstr[c].m_screenCode;
                s+="$"+QString::number(sc,16);
                if (curOut++<8)
                    s+=", ";
                else {
                    curOut=0;
                    Write(s);
                    s="\tdc.b\t";
                }
            }
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
    if (curOut!=0)
        Write(s + "0");
    else Write("\tdc.b\t0 ");
}


void AsmMOS6502::BeginBlock()
{
    Nl();
}


void AsmMOS6502::EndBlock()
{
    Nl();
    m_labelStack["block"].pop();
}

void AsmMOS6502::ApplyTerm()
{
    //Asm("sta " + m_currentVar);

    m_currentVar = "";
}

void AsmMOS6502::Number(QString n)
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

void AsmMOS6502::Comment(QString s)
{
    Asm("; "+ s) ;
}

QString AsmMOS6502::String(QStringList lst)
{

    QString res;
    QString mark = "dc.b";

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
    res=res + "\t"+mark+"\t0";
    m_term +=res;
    return res;
}

void AsmMOS6502::AssignVariable(QString v)
{
    m_currentVar = v;
    m_term="lda ";
}

void AsmMOS6502::EndAssignVariable(QString variable)
{
    Asm(m_term);
    Asm("sta " + variable);
}

void AsmMOS6502::BinOP(TokenType::Type t)
{
    if (t == TokenType::PLUS) {
        Asm("clc");
        m_term = "adc ";
    }

    if (t == TokenType::MINUS) {
        Asm("sec");
        m_term = "sbc ";
    }

    if (t == TokenType::BITAND) {
        m_term = "and ";
    }

    if (t == TokenType::BITOR) {
        m_term = "ora ";
    }

}


void AsmMOS6502::Poke(bool start)
{
    //if (start)
     //   m_term = "lda ";
    if (!start)
       m_term = "sta ";
}

void AsmMOS6502::Peek(bool start)
{
    if (start)
        m_term = "lda ";
   else m_term = "sta ";

}

QString AsmMOS6502::PushZeroPointer()
{
    QString zp = m_zeroPointers[m_curZeroPointer];
    m_curZeroPointer++;
    return zp;
}

void AsmMOS6502::PopZeroPointer()
{
    if (m_curZeroPointer<=0)
        ErrorHandler::e.Error("Zero pointer cannot be pushed below zero");

    m_curZeroPointer--;
}

bool AsmMOS6502::CheckZPAvailability()
{
    if (m_curZeroPointer<m_zeroPointers.count())
        return true;
    return false;
}

QString AsmMOS6502::StoreInTempVar(QString name, QString type)
{
    if (m_zpStack.count()<m_tempZeroPointers.count()) {
       // qDebug() << "B" << m_tempZeroPointers.count();

        int i = m_zpStack.count();
        m_zpStack.append(m_tempZeroPointers[i]);
//        qDebug() << name << " INCREASE TO " << m_zpStack.count();

        QString tmpVar = NewLabel(name+"_var");
        QString labelVar = tmpVar + " = " + m_zpStack.last();
        Label(labelVar);
//        m_tempVars << labelVar;
        Asm("sta " + tmpVar);
        PopLabel(name+ "_var");
        return tmpVar;

    }

   // qDebug() << "Using reglar variables: " << m_zpStack.count();
    QString tmpVar = NewLabel(name+"_var");
    QString labelVar = tmpVar + "\t."+type+"\t0 ";
    m_tempVars << labelVar;
    Asm("sta " + tmpVar);
    PopLabel(name+ "_var");
    return tmpVar;
}

void AsmMOS6502::PopTempVar()
{
    if (m_zpStack.count()>0) {
        m_zpStack.removeLast();
    }
//    else
  //      ErrorHandler::e.Error("COMPILER ERROR: Assembler::PopTempVar Trying to pop tempvar from zero");
}


void AsmMOS6502::Writeln()
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

void AsmMOS6502::EndWriteln()
{
    Asm(m_term + ",0");
    Label("L"+getLabel("writeln")+"2");
    m_labelStack["writeln"].pop();

}

void AsmMOS6502::LoadVariable(QString var)
{
    Asm("lda "+ var);
}

void AsmMOS6502::Variable(QString v, bool isByte)
{
    if (isByte) {
        if (m_term=="")
            m_term = "lda ";
        m_term+=v;
    }
    else {
        Comment("integer assignment NodeVar");
        if (m_term=="")
            m_term = "lda ";
        m_term+=v + "+1 ; Next one";
        Term();
        Asm("tay");
        Term("lda "+v);

    }
}

void AsmMOS6502::WriteBuiltinWriteln()
{

}

void AsmMOS6502::StartPrint()
{
    /*
    LDY #$00
 L0
     LDA L1,Y
     CMP #0
     BEQ L2
     JSR $FFD2
     INY
     JMP L0
 L1
     .byte "hello world",0
 L2
 */
}

void AsmMOS6502::IncludeFile(QString pfile)
{
    QFile file(pfile);
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Could not open core include file: " +pfile;
        return;
    }

    QTextStream in(&file);

    while(!in.atEnd()) {
        QString line = in.readLine();
        for (QString key: m_replaceValues.keys())
            line = line.replace(key, m_replaceValues[key]); // Replace stuff like zps

/*        if (line.startsWith(" ") | line.startsWith("\t"))
            Asm(line);
            else
            Label(line);*/
        m_source << line;
  //      QStringList fields = line.split(",");
    }

    file.close();
}


void AsmMOS6502::StartForLoop(QString var, QString startVal)
{
}

void AsmMOS6502::EndForLoop(QString endVal)
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

void AsmMOS6502::Optimise(CIniFile& ini)
{
    m_totalOptimizedLines = 0;
    if (ini.getdouble("post_optimizer_passstalda")==1)
        OptimisePassStaLda();
    if (ini.getdouble("post_optimizer_passldx")==1)
        OptimisePassLdx("x");
    if (ini.getdouble("post_optimizer_passldx")==1)
        OptimisePassLdx("y");

    if (ini.getdouble("post_optimizer_passldatax")==1)

        OptimisePassLdaTax("x");
    if (ini.getdouble("post_optimizer_passldatax")==1)
        OptimisePassLdaTax("y");

    if (ini.getdouble("post_optimizer_passlda")==1)

        OptimisePassLdx("a");

    if (ini.getdouble("post_optimizer_passjmp")==1)
        OptimiseJumps();
}


void AsmMOS6502::OptimisePassStaLda()
{
    m_removeLines.clear();
    int j;
    for (int i=0;i<m_source.count()-1;i++) {
        QString l0 = getLine(i);
        if (l0.contains("sta")) {
            QString l1 = getNextLine(i,j);
            if (l0==l1) {
                m_removeLines.append(j);
                continue;
            }
            QString var = getToken(l0,1);
            if (getToken(l1,1)==var && getToken(l1,0)=="lda") {
              //  qDebug() << "Removing: " << l1 << " on line " << j;
                m_removeLines.append(j);
                i++;
                continue;
            }
        }
    }
    RemoveLines();
}

void AsmMOS6502::OptimisePassLdx(QString x)
{

    bool allDone = false;

    while (!allDone) {

        m_removeLines.clear();
        int j;
        int shift=0;
        for (int i=0;i<m_source.count()-1;i++) {
            QString l0 = getLine(i);
            if (l0.contains("ld"+x)) {
                bool done = false;
                //qDebug() << l0;
                int k=i;
                int cnt = 0;
                int curCnt = 0;
                int ll=i;
                while (!done) {
                    QString l1 = getNextLine(k,j);
                    k=j;
                    QString op2 = getToken(l1,1);
                    QString op = getToken(l1,0);

                    if (l0==l1 && !op2.startsWith("(") && !op2.contains(",")) {
                        if (x=="a")
//                        qDebug () << "Removing because equal: " << l0 << ", " << l1;
                        m_removeLines.append(j);
                        curCnt++;
                        //qDebug() << "Removing: " << l1 << " on line " << j;
                        continue;
                    }
                    //qDebug() << "curop:" << op;
                    // Changex in x
                    if (x=="x" || x=="y")
                    if (op==("ld"+x) || op==("ta"+x) || op=="jmp" || op=="rts" || op=="jsr" ||
                            op==("in" +x) || op==("de"+x)|| op.length()!=3 )
//                        op==("in" +x) || op==("de"+x)|| op==("sta")|| op.length()!=3 ) {
                        //qDebug() << "Done because: " << l1;
                        done=true;

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

void AsmMOS6502::OptimisePassLdaTax(QString x)
{
    m_removeLines.clear();
    int j;
    for (int i=0;i<m_source.count()-1;i++) {
        QString l0 = getLine(i);
        if (l0.contains("lda") && !l0.contains(",")) {
            //qDebug() << l0;
            int k=i;
            int n;
                QString l1 = getNextLine(k,j);
                QString l2 = getNextLine(j,n);
                k=j;
                QString op = getToken(l1,0);
                if (op==("ta"+x)) {
                    bool perform=true;
                    QString tst = getToken(l2,0);
                    if (tst.startsWith("ta") || tst=="sta")
                        perform=false;

                    if (perform) {
                        m_removeLines.append(j);
                        QString org = m_source[i];
                        m_source[i] = m_source[i].toLower().replace("lda", "ld"+x)+" ; optimized, look out for bugs";
                        //qDebug() << "Changed : "<< org << " to " <<m_source[i] << " with op " <<op;
                    }
                    //m_removeLines.append(i);



            }
//            if (m_removeLines.count()!=0)
  //              break;
        }
    }

    RemoveLines();

}


void AsmMOS6502::OptimiseJumps()
{
    m_removeLines.clear();
    int j;
    for (int i=0;i<m_source.count()-1;i++) {
        QString l0 = getLine(i);
        if (l0.contains("jmp ")) {
            QString l1 = getNextLine(i,j);
            QString lbl0 = getToken(l0, 1);
            //qDebug() << lbl0 << " vs " << l1 ;
            if (l1.toLower().contains(lbl0.toLower()))
            {
                //qDebug() << "Removing:";
                //qDebug() << " " + l0;
                //qDebug() << " " + l1;
                m_removeLines.append(i);
                m_removeLines.append(j);
                i++;
                continue;
            }
        }
    }
    RemoveLines();

}

QString AsmMOS6502::getLine(int i)
{
    QString s = m_source[i].trimmed().toLower().simplified().remove("\n");
    return s;
}

QString AsmMOS6502::getNextLine(int i, int &j)
{
    bool ok = false;
    i=i+1;
    QString line ="";

    while (i<m_source.count() && (
           getLine(i).remove(" ")=="" ||
           getLine(i).remove(" ")=="\t" ||
           getLine(i).remove(" ").remove("\t").startsWith(";")==true)) {
        //if (getLine(i).contains(";"))
    //            qDebug() << getLine(i);
        i++;
    }
    j=i;
//    qDebug() << "RET: " << getLine(i);
    return getLine(i);

}

QString AsmMOS6502::getToken(QString s, int t)
{
    QStringList lst = s.split(" ");
    if (t>=lst.count())
        return "";
    return lst[t];
}

void AsmMOS6502::RemoveLines()
{
    int k=0;
    for (int i: m_removeLines) {
        //qDebug() << "Removing line " << (i) << " : " << getLine(i-k);
        m_source.removeAt(i-k);
        k++;
        m_totalOptimizedLines++;
    }
    m_removeLines.clear();
}

int AsmMOS6502::getLineCount()
{
    int lc = 0;
    for (QString s: m_source) {
        s=s.remove("\n");
        s=s.remove("\t");
        s=s.trimmed();
        if (s=="")
            continue;
        if (s.startsWith(";"))
            continue;
        lc++;
    }
    return lc;
}

int AsmMOS6502::CountInstructionCycle(QStringList s)
{
    MOSOperation op = GetOperand(s);
    return CalculateCycles(op);

}

MOSOperation AsmMOS6502::GetOperand(QStringList s)
{
    MOSOperation op;
    bool ok= false;
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

int AsmMOS6502::CalculateCycles(MOSOperation op)
{
    if (op.operand.contains("\t"))
        return 0;
    if (!m_opCycles.contains(op.operand)) {
        qDebug() << "Error: could not count operands for type:" << op.operand;
        return 0;
    }
    MOSOperandCycle oc = m_opCycles[op.operand];
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


}

void AsmMOS6502::InitMosOpCycles()
{
//   int implied, int immediate, int absolute, int abswp, int zp, int zpwp) {

    m_opCycles.clear();
    m_opCycles["lda"] = MOSOperandCycle("lda",0,2,4,4,3,5);
    m_opCycles["sta"] = MOSOperandCycle("sta",0,0,4,5,4,6);
    m_opCycles["sty"] = MOSOperandCycle("sty",0,0,4,0,3,4);
    m_opCycles["stx"] = MOSOperandCycle("stx",0,0,4,0,3,4);

    m_opCycles["tax"] = MOSOperandCycle("stx",2,0,0,0,0,0);
    m_opCycles["tay"] = MOSOperandCycle("tay",2,0,0,0,0,0);
    m_opCycles["tya"] = MOSOperandCycle("tya",2,0,0,0,0,0);
    m_opCycles["txa"] = MOSOperandCycle("txa",2,0,0,0,0,0);

    m_opCycles["pha"] = MOSOperandCycle("pha",3,0,0,0,0,0);
    m_opCycles["pla"] = MOSOperandCycle("pla",4,0,0,0,0,0);

    m_opCycles["dex"] = MOSOperandCycle("dex",2,0,0,0,0,0);
    m_opCycles["dey"] = MOSOperandCycle("dey",2,0,0,0,0,0);
    m_opCycles["inx"] = MOSOperandCycle("inx",2,0,0,0,0,0);
    m_opCycles["iny"] = MOSOperandCycle("iny",2,0,0,0,0,0);

    m_opCycles["and"] = MOSOperandCycle("and",0,2,4,4,3,5);
    m_opCycles["ora"] = MOSOperandCycle("ora",0,2,4,4,3,5);
    m_opCycles["eor"] = MOSOperandCycle("eor",0,2,4,4,3,5);
    m_opCycles["bit"] = MOSOperandCycle("bit",0,0,4,0,3,3);

    m_opCycles["sec"] = MOSOperandCycle("sec",2,0,0,0,0,0);
    m_opCycles["cli"] = MOSOperandCycle("cli",2,0,0,0,0,0);
    m_opCycles["sei"] = MOSOperandCycle("sei",2,0,0,0,0,0);
    m_opCycles["clc"] = MOSOperandCycle("clc",2,0,0,0,0,0);

    m_opCycles["lsr"] = MOSOperandCycle("lsr",2,2,6,7,5,6);
    m_opCycles["asl"] = MOSOperandCycle("asl",2,2,6,7,5,6);
    m_opCycles["ror"] = MOSOperandCycle("ror",2,2,6,7,5,6);
    m_opCycles["rol"] = MOSOperandCycle("rol",2,2,6,7,5,6);

    m_opCycles["org"] = MOSOperandCycle("org",0,0,0,0,0,0);
    m_opCycles["nop"] = MOSOperandCycle("nop",2,0,0,0,0,0);

    m_opCycles["sbc"] = MOSOperandCycle("sbc",0,2,4,4,3,5);
    m_opCycles["adc"] = MOSOperandCycle("adc",0,2,4,4,3,5);


    m_opCycles["ldy"] = MOSOperandCycle("ldy",0,2,4,4,3,0);
    m_opCycles["ldx"] = MOSOperandCycle("ldx",0,2,4,4,3,0);
    m_opCycles["cpy"] = MOSOperandCycle("cpy",0,2,4,4,3,0);
    m_opCycles["cpx"] = MOSOperandCycle("cpx",0,2,4,4,3,0);

    m_opCycles["cmp"] = MOSOperandCycle("cmp",0,2,4,4,3,5);
    m_opCycles["jmp"] = MOSOperandCycle("jmp",0,0,3,3,0,5);
    m_opCycles["jsr"] = MOSOperandCycle("jsr",0,0,6,6,0,0);
    m_opCycles["rts"] = MOSOperandCycle("rts",6,0,0,6,0,0);
    m_opCycles["rti"] = MOSOperandCycle("rti",6,0,0,6,0,0);
    m_opCycles["beq"] = MOSOperandCycle("beq",0,0,2,0,0,0);
    m_opCycles["bcs"] = MOSOperandCycle("bcs",0,0,2,0,0,0);
    m_opCycles["bcc"] = MOSOperandCycle("bcc",0,0,2,0,0,0);
    m_opCycles["bne"] = MOSOperandCycle("bne",0,0,2,0,0,0);
    m_opCycles["bpl"] = MOSOperandCycle("bpl",0,0,2,0,0,0);

    m_opCycles["inc"] = MOSOperandCycle("inc",0,0,6,7,5,6);
    m_opCycles["dec"] = MOSOperandCycle("dec",0,0,6,7,5,6);




}



void AsmMOS6502::InitZeroPointers(QStringList lst, QStringList tmpList)
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


}
