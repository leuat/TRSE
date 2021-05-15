#include "AsmM68000.h"
#include "source/Compiler/optimiser/postoptimizerm68k.h"
AsmM68000::AsmM68000()
{
    m_regAcc = RegisterStack(QStringList()<<"d0"<<"d1"<<"d2"<<"d3"<<"d4"<<"d5"<<"d6" );
    m_regMem = RegisterStack(QStringList()<<"a0"<<"a1"<<"a2"<<"a3"<<"a4"<<"a5"<<"a6" );
    m_chipMem = QSharedPointer<Appendix>(new Appendix);

    m_optimiser = QSharedPointer<PostOptimiser>(new PostOptimiserM68K());
}

void AsmM68000::Connect() {
    // Connect with temp vars
    QStringList newSource;
    for (int i=0;i<m_varDeclEndsLineNumber;i++) {
        newSource<<m_source[i];
    }
    newSource << " ; Temp vars section";
    Asm(" 	CNOP 0,4");
    newSource<< m_tempVars;
    Asm(" 	CNOP 0,4");
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

        if (Util::NumberFromStringHex(m_appendix[i]->m_pos)<Syntax::s.m_currentSystem->m_programStartAddress)
            pre <<m_appendix[i]->m_source;
        else m_source << m_appendix[i]->m_source;

    }

    m_source.removeAll("");
    m_appendix.clear();

    m_source = QStringList() <<pre << m_source;




}

void AsmM68000::Program(QString name, QString vicParam)
{
    /*  if (!Syntax::s.m_currentSystem->m_systemParams.contains("ignoresystemheaders"))
        IncludeFile(":resources/code/amiga/init.s");
    else
        IncludeFile(":resources/code/amiga/init_stripped.s");
        */
}

bool AsmM68000::DeclareClass(QString name, QString type, int count, QStringList data, QString pos)
{
    if (m_symTab->m_records.contains(type)) {
        if (!m_symTab->m_records[type]->m_isClass)
            return false;

        QSharedPointer<SymbolTable>  st = m_symTab->m_records[type];

        Asm(" 	CNOP 0,4");
        Label(name + "\tdcb.b\t"+QString::number(st->getSize()*count ));

        return true;
    }
    return false;

}

/*void AsmM68000::EndProgram()
{
    if (!Syntax::s.m_currentSystem->m_systemParams.contains("ignoresystemheaders"))
        IncludeFile(":resources/code/amiga/end.s");
    else
        IncludeFile(":resources/code/amiga/end_stripped.s");

}
*/
void AsmM68000::DeclareArray(QString name, QString type, int count, QStringList data, QString pos)
{
    QString t = byte;


    if (DeclareRecord(name,type,count,data,pos))
        return;

    if (DeclareClass(name,type,count,data,pos))
        return;


    if (data.count()==0 && pos=="") {
        if (Syntax::s.m_currentSystem->m_system==AbstractSystem::AMIGA) {
            if (type.toLower()=="integer")
                t = "blk.w";
            if (type.toLower()=="byte")
                t = "blk.b";

            if (type.toLower()=="string")
                t = "blk.b";

        if (type.toLower()=="long")
            t = "blk.l";
        }
        if (Syntax::s.m_currentSystem->m_system==AbstractSystem::ATARI520ST) {
        if (type.toLower()=="integer")
            t = "dcb.w";
        if (type.toLower()=="byte")
            t = "dcb.b";

        if (type.toLower()=="string")
            t = "dcb.b";

        if (type.toLower()=="long")
            t = "dcb.l";
        }


        Write(name +"\t" + t + "\t " + QString::number(count));

    }
    else {

        if (type.toLower()=="integer")
            t = word;
        if (type.toLower()=="byte")
            t = byte;

        if (type.toLower()=="string")
            t = byte;

        if (type.toLower()=="long")
            t = llong;


        QString s="";
        s=s+name + "\t"+t+" ";

        for (int i=0;i<data.count();i++) {
            s=s+data[i];
            if (i%8==7) {
                s=s+"\n";
                if (i!=data.count()-1)
                    s=s + "\t" +t + " ";
            }
            else if (i!=data.count()-1) s=s+", ";

        }
        s = s.replace("$0","$");
        QStringList lst = s.split("\n");
            for (int i=0;i<lst.count();i++)
                Write(lst[i]);

    }
}

void AsmM68000::DeclareVariable(QString name, QString type, QString initval, QString position)
{
    QString t = "";
    if (type.toLower()=="integer")
        t = word;
    if (type.toLower()=="byte") {
        t = byte;
    }
    if (type.toLower()=="long")
        t = llong;

    if (DeclareClass(name,type,1,QStringList() <<initval,position))
        return;



    if (t=="")
        ErrorHandler::e.Error("Cannot declare variable of type: " + type);
    if (initval=="")
        initval="0";



    Write(name +"\t" + t + "\t"+initval);
    if (t==byte)
        Asm(" 	CNOP 0,4");


}

void AsmM68000::BinOP(TokenType::Type t, bool clearFlag)
{
    QString v="";
    if (t == TokenType::PLUS)
        v = "add";

    if (t == TokenType::MINUS)
        v = "sub";


    if (t == TokenType::BITAND)
        v = "and";


    if (t == TokenType::BITOR)
        v = "or";

    if (t == TokenType::MUL)
        v = "mulu";

    if (t == TokenType::DIV)
        v = "divu";

    if (t == TokenType::SHR)
        v = "lsr";

    if (t == TokenType::SHL)
        v = "lsl";

    m_varStack.push(v);
}

void AsmM68000::DeclareString(QString name, QStringList initval, QStringList flags)
{

    Asm(" 	CNOP 0,4");
    Write(name +"\t" + String(initval,!flags.contains("no_term")));
    m_term="";
    Asm(" 	CNOP 0,4");

}

QString AsmM68000::String(QStringList lst, bool term)
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
    if (term)
        res=res + "\t"+mark+"\t0";
    m_term +=res;
    return res;
}

QString AsmM68000::GetOrg(int pos)
{
    return "org " + Util::numToHex(pos);
}
