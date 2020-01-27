#include "asmx86.h"

AsmX86::AsmX86()
{
    m_hash = "";
}

void AsmX86::Program(QString name, QString vicParam)
{
    m_source+=m_startInsertAssembler;
    Asm("[ORG "+Util::numToHex(Syntax::s.m_currentSystem->m_programStartAddress) + "]");
    m_hash = "";

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
    QString t = "";

    if (type.toLower()=="const") {
        Write(name + " = " + initval,0);
        return;
    }


    if (type.toLower()=="integer")
        t = word;
    if (type.toLower()=="byte") {
        t = byte;
    }

    if (t=="")
        ErrorHandler::e.Error("Cannot declare variable of type: " + type);

    Write(name+":" +"\t" + t + "\t"+initval,0);

}

void AsmX86::BinOP(TokenType::Type t)
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

QString AsmX86::String(QStringList lst)
{
    return "";
}

void AsmX86::Label(QString s)
{
    Write(s+":",0);
}
