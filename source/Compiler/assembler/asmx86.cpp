#include "asmx86.h"

AsmX86::AsmX86()
{
    m_hash = "";
}

void AsmX86::Connect() {
    // Connect with temp vars
    QStringList newSource;
    for (int i=0;i<m_varDeclEndsLineNumber;i++) {
        newSource<<m_source[i];
    }
    newSource << " ; Temp vars section";
    newSource<< m_tempVars;
    newSource << " ; Temp vars section ends";
    for (int i=m_varDeclEndsLineNumber;i<m_source.count(); i++) {
        newSource<<m_source[i];
    }
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

void AsmX86::Program(QString name, QString vicParam)
{
    m_source+=m_startInsertAssembler;
    Asm("[ORG "+Util::numToHex(Syntax::s.m_currentSystem->m_programStartAddress) + "]");
    m_hash = "";
/*    Asm("jmp save_ds_register");
    Write("ds_register_saved: dw 0",0);
    Write("save_ds_register:",0);
    Asm("mov ax,ds");
    Asm("mov [ds_register_saved],ax");
*/
}

void AsmX86::EndProgram()
{

}

void AsmX86::Write(QString str, int level)
{
    QString s = str.replace("$","0x");
    Assembler::Write(s,level);
}

void AsmX86::DeclareArray(QString name, QString type, int count, QStringList data, QString pos)
{
    QString t = byte;
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

void AsmX86::DeclareVariable(QString name, QString type, QString initval, QString position)
{
    QString t = "";

    if (initval=="")
        initval = "0";
    if (type.toLower()=="const") {
        Write(name + " equ " + initval,0);
        return;
    }


    if (type.toLower()=="integer")
        t = word;
    if (type.toLower()=="byte") {
        t = byte;
    }

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

void AsmX86::BinOP(TokenType::Type t, bool clearFlag)
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
        m_term = "div ";
    }


}

void AsmX86::DeclareString(QString name, QStringList initVal, QStringList flags) {
    Write(name +"\t" + String(initVal,!flags.contains("no_term")),0);
}

QString AsmX86::String(QStringList lst, bool term)
{
    QString res;
    QString mark = "db";

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

void AsmX86::Label(QString s)
{
    Write(s+":",0);
}

QString AsmX86::GetOrg(int pos)
{
    return "[org " + Util::numToHex(pos).replace("$","0x") + "]";
}
