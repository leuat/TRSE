#include "asmx86.h"

AsmX86::AsmX86()
{

}

void AsmX86::Program(QString name, QString vicParam)
{
    m_source+=m_startInsertAssembler;
    Asm("[ORG "+Util::numToHex(Syntax::s.m_currentSystem->m_programStartAddress) + "]");

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

}

void AsmX86::DeclareVariable(QString name, QString type, QString initval)
{

}

void AsmX86::BinOP(TokenType::Type t)
{

}

QString AsmX86::String(QStringList lst)
{

}

void AsmX86::Label(QString s)
{
    Write(s+":",0);
}
