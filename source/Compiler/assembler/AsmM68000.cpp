#include "AsmM68000.h"

AsmM68000::AsmM68000()
{
    m_regAcc = RegisterStack(QStringList()<<"d0"<<"d1"<<"d2"<<"d3"<<"d4"<<"d5"<<"d6" );
    m_regMem = RegisterStack(QStringList()<<"a0"<<"a1"<<"a2"<<"a3"<<"a4"<<"a5"<<"a6" );

}

void AsmM68000::Program(QString name, QString vicParam)
{
    IncludeFile(":resources/code/amiga/init.s");
}

void AsmM68000::EndProgram()
{
    IncludeFile(":resources/code/amiga/end.s");

}

void AsmM68000::DeclareArray(QString name, QString type, int count, QStringList data, QString pos)
{
    QString t = byte;



    if (data.count()==0 && pos=="") {
        if (type.toLower()=="integer")
            t = "blk.w";
        if (type.toLower()=="byte")
            t = "blk.b";

        if (type.toLower()=="string")
            t = "blk.b";

        if (type.toLower()=="long")
            t = "blk.l";

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
            else s=s+", ";

        }
        QStringList lst = s.split("\n");
            for (int i=0;i<lst.count();i++)
                Write(lst[i]);

    }
}

void AsmM68000::DeclareVariable(QString name, QString type, QString initval)
{
    QString t = "";
    if (type.toLower()=="integer")
        t = word;
    if (type.toLower()=="byte")
        t = byte;
    if (type.toLower()=="long")
        t = llong;


    if (t=="")
        ErrorHandler::e.Error("Cannot declare variable of type: " + type);
    if (initval=="")
        initval="0";
    Write(name +"\t" + t + "\t"+initval);

}

void AsmM68000::BinOP(TokenType::Type t)
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

    m_varStack.push(v);
}
