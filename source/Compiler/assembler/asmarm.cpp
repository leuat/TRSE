#include "asmarm.h"
#include "source/Compiler/optimiser/postoptimizerx86.h"
#include <QRegularExpression>


AsmARM::AsmARM()
{
    m_hash = "";
    byte=".byte";
    word=".word";
    llong =".word";
    ppointer =".word";
    //m_optimiser = QSharedPointer<PostOptimiser>(new PostOptimiserX86());
    m_ignoreInitialJump = true;


}

void AsmARM::Connect() {
    // Connect with temp vars
    QStringList newSource;
    newSource<<(".global _start");
    newSource<<(".align 4");
    newSource<<("_start:");
    newSource<<("       b block1");


    for (int i=m_varDeclEndsLineNumber;i<m_source.count(); i++) {
        newSource<<m_source[i];
    }
    for (int i=0;i<m_varDeclEndsLineNumber;i++) {
        newSource<<m_source[i];
    }
    newSource << " ; Temp vars section";
    newSource<< m_tempVars;
    newSource << " ; Temp vars section ends";
    m_source = newSource;
    //m_source<<m_appendix;
    //    m_appendix.append(m_ extraBlocks);
    SortAppendix();

    //  qDebug() << m_appendix[0].m_source;
    QStringList pre;
    for (int i=0;i<m_appendix.count();i++) {
//                qDebug() << (m_appendix[i].m_pos);
        if (Util::NumberFromStringHex(m_appendix[i]->m_pos)<Syntax::s.m_currentSystem->m_programStartAddress)
            pre <<m_appendix[i]->m_source;
        else m_source << m_appendix[i]->m_source;

    }

    m_source.removeAll("");
    m_appendix.clear();


}

void AsmARM::Program(QString name, QString vicParam)
{
    m_source+=m_startInsertAssembler;
//    if (Syntax::s.m_currentSystem->m_programStartAddress!=0)
  //      Asm("[ORG "+Util::numToHex(Syntax::s.m_currentSystem->m_programStartAddress) + "]");
    m_hash = "";
/*    Asm("jmp save_ds_register");
    Write("ds_register_saved: dw 0",0);
    Write("save_ds_register:",0);
    Asm("mov ax,ds");
    Asm("mov [ds_register_saved],ax");
*/
}

void AsmARM::EndProgram()
{

}

void AsmARM::Write(QString str, int level)
{
    QRegularExpression regexp("\\$\\b[0-9a-fA-F]+\\b");
    if (str.contains(regexp))
        str = str.replace("$","0x");
//    QString s = str.replace(regexp,"0x");
  //  if (s.contains("$"))
//        qDebug() << s;
//if (s!=str)     qDebug() <<s;
    //QString s = str.replace("$","0x");
    Assembler::Write(str,level);
}

void AsmARM::DeclareArray(QString name, QString type, int count, QStringList data, QString pos)
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
// array  resb  251*256  ;251 ROWS X 256 COLUMNS.

     if (data.count()==0 && pos!="") {
         Write(name + " equ " + pos);
         return;
     }


    if (data.count()==0 && pos=="") {

/*        QString t = "resb";
        if (type.toLower()=="integer")
            t = "resw";

        Write(name+":" +"\t array " + t + "\t "+QString::number(count),0);
  */
        Write(name+":" +"\t times "+QString::number(count) +" "+t+" 0",0);

    }
    else {
        QString s="";
        s=s+name+":" + "\t"+t+" ";

        for (int i=0;i<data.count();i++) {
            s=s+data[i];
            if (i%8==7) {
                s=s+"\n";
                s=s + "\t" +t + " ";
            }
            else if (i!=data.count()-1) s=s+", ";

        }
        QStringList lst = s.split("\n");
        if (pos=="") {
            for (int i=0;i<lst.count();i++)
                Write(lst[i],0);
        }
        else {
            QSharedPointer<Appendix> app = QSharedPointer<Appendix>(new Appendix(pos));
            app->Append("org [" + pos +"]",1);
            for (int i=0;i<lst.count();i++)
                app->Append(lst[i],0);

            int p = Util::NumberFromStringHex(pos);

            blocks.append(QSharedPointer<MemoryBlock>(new MemoryBlock(p,p+count, MemoryBlock::ARRAY, name)));

            m_appendix.append(app);
        }

    }

}

void AsmARM::DeclareVariable(QString name, QString type, QString initval, QString position)
{
    QString t = "";

    if (initval=="")
        initval = "0";
    Write(".align 4");
    if (type.toLower()=="const") {
        Write(name + " equ " + initval,0);
        return;
    }


    if (type.toLower()=="integer")
        t = word;
    if (type.toLower()=="byte") {
        t = byte;
    }

    if (DeclareClass(name,type,1,QStringList(),position))
         return;

    if (t=="")
        ErrorHandler::e.Error("Cannot declare variable of type: " + type);


    if (position=="")
        Write(name+":" +"\t" + t + "\t"+initval,0);
    else
    {
        Write(name +"\t equ \t"+position,0);
/*        Appendix* app = new Appendix(position);
        app->Append(GetOrg(Util::NumberFromStringHex(position)),1);

        app->Append(name+":" +"\t" + t + "\t"+initval,0);

        int p = Util::NumberFromStringHex(position);

        blocks.append(new MemoryBlock(p,p+2, MemoryBlock::ARRAY, name));

        m_appendix.append(app);*/
    }
}

void AsmARM::BinOP(TokenType::Type t, bool clearFlag)
{
    if (t == TokenType::PLUS) {
        m_term = "add ";
    }

    if (t == TokenType::MINUS) {
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
        m_term = "mul ";
    }
    if (t == TokenType::DIV) {
//        m_term = "xor dx,dx \n\tidiv ";
        m_term = "div ";
    }
    if (t == TokenType::SHR) {
        m_term = "sar ";
    }
    if (t == TokenType::SHL) {
        m_term = "sal ";
    }


}

void AsmARM::DeclareString(QString name, QStringList initVal, QStringList flags) {
    Write(".align 4");
    Write(name +":\t" + String(initVal,!flags.contains("no_term")),0);
}

QString AsmARM::String(QStringList lst, bool term)
{
    QString res;
    QString mark = ".ascii";

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
        res=res + "\t.byte\t0";
    m_term +=res;
    return res;
}

void AsmARM::Label(QString s)
{
    Write(s+":",0);
}

QString AsmARM::GetOrg(int pos)
{
    return "[org " + Util::numToHex(pos).replace("$","0x") + "]";
}

bool AsmARM::DeclareClass(QString name, QString type, int count, QStringList data, QString pos)
{
    if (m_symTab->m_records.contains(type)) {
        if (!m_symTab->m_records[type]->m_isClass)
            return false;

        QSharedPointer<SymbolTable>  st = m_symTab->m_records[type];
        Write(name + ": times " +QString::number(st->getSize()*count)+" db 0");
//        Asm("[org "+name+"+" +QString::number(st->getSize()*count)+"]");

        return true;
    }
    return false;

}

QString AsmARM::NewLabel(QString s) {
    if (s.toLower().contains("block"))
        return Assembler::NewLabel(s);
    m_curLabel++;
    return QString::number(m_curLabel);
}

void AsmARM::PopLabel(QString s) {
    if (s.contains("block")) {
        Assembler::PopLabel(s);
        return;
    }

    m_curLabel--;
}
