#include "asmz80.h"

AsmZ80::AsmZ80()
{
    m_hash = "";
    m_hram = QSharedPointer<Appendix>(new Appendix);
    m_wram = QSharedPointer<Appendix>(new Appendix);
    m_sprram = QSharedPointer<Appendix>(new Appendix);

    m_wram->Append("i_input_current:	DS	1",0);
    m_wram->Append("i_input_previous:	DS	1",0);
    m_wram->Append("cmpvar:	DS	1",0);


}

void AsmZ80::Connect() {
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

void AsmZ80::Program(QString name, QString vicParam)
{
    m_source+=m_startInsertAssembler;
//    Asm("[ORG "+Util::numToHex(Syntax::s.m_currentSystem->m_programStartAddress) + "]");
    m_hash = "";

}

void AsmZ80::EndProgram()
{

}

void AsmZ80::Write(QString str, int level)
{
//    QString s = str.replace("$","0x");
    Assembler::Write(str,level);
}

void AsmZ80::DeclareArray(QString name, QString type, int count, QStringList data, QString pos)
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
         Write(name + " = " + pos);
         return;
     }


    if (data.count()==0 && pos=="") {

/*        QString t = "resb";
        if (type.toLower()=="integer")
            t = "resw";

        Write(name+":" +"\t array " + t + "\t "+QString::number(count),0);
  */

        //Write(name+":" +"\t times "+QString::number(count) +" "+t+" 0",0);
        if (m_currentBlock==m_hram || m_currentBlock==m_wram || m_currentBlock==m_sprram) {
            Write(name+":" +"\t ds "+QString::number(count),0);
            m_currentBlock->m_dataSize+=count;

        }
        else

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

void AsmZ80::DeclareVariable(QString name, QString type, QString initval, QString position)
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


    if (m_currentBlock==m_hram || m_currentBlock==m_wram || m_currentBlock==m_sprram) {
        t = "ds";
        if (type.toLower()=="byte") {
            initval = "1";
            m_currentBlock->m_dataSize+=1;
        }
        if (type.toLower()=="integer") {
            initval = "2";
            m_currentBlock->m_dataSize+=2;
        }


    }

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

void AsmZ80::DeclareString(QString name, QStringList initVal) {
//    qDebug() << "HERE" << name << initVal;
    Write(name +"\t" + String(initVal),0);
}

void AsmZ80::BinOP(TokenType::Type t)
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

int AsmZ80::CodeSizeEstimator(QStringList &lines) {
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
            QStringList plst = sp1[1].toLower().trimmed().split(","); // a, hl, [hl], [var] etc
            // ld [var], a
            QString p = plst[0].trimmed();
            // 1 param
            if (plst.count()==1) {
                if (p=="a" || p=="b" || p=="c" || p=="d" || p=="e") {
                    add = 1; // add b
                }
                else
                {
                    // Test if number
                    int i;
                    bool ok;
                    i = p.remove("$").toInt(&ok,16);
                    if (ok)
                        add=2; // sub 10
                    else add=3;   // call someting

                }
            }
            //            else
            if (p.startsWith("[")) {
                if (p!="[hl]" && p!="[de]" && p!="[bc]")
                    add=3;
                else add=1;
            }
            else
            if (plst.count()>=2) {
                QString p2 = plst[1].toLower().trimmed(); // a, hl, [hl], [var] etc
//                qDebug() << "H";


                if (p=="a" || p=="b" || p=="c" || p=="d" || p=="e") {
                    if (p2=="a" || p2=="b" || p2=="c" || p2=="d" || p2=="e")
                        add=1; // ld a,b
                    else
                        if (p2.startsWith("[")) {

                            if (p2=="[hl]" || p2=="[de]" || p2=="[bc]" )
                                add=1; // ld a, [hl]
                            else
                                add=3; // ld a, [var]

                        }
                        else
                            add=2; // ld a,10
                }
                else add=3; // ld hl, var
            }

            // ld a,b

/*            QStringList ps = p.split(",");
            p = ps[0]; // pick first one, ignore ",x" etc
            if (p.startsWith("")) { add=2; }
            else add=3;*/
        }
        size+=add;
//        qDebug() << t << add;

    }
  //  qDebug() << "********************* SIZE "<<size;
    return size;



}

void AsmZ80::Label(QString s)
{
    Write(s+":",0);
}

QString AsmZ80::GetOrg(int pos)
{
    return "[org " + Util::numToHex(pos).replace("$","0x") + "]";
}

QString AsmZ80::String(QStringList lst)
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
    res=res + "\t"+mark+"\t0";
    m_term +=res;
    return res;
}

