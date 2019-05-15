/*#include "asmpascal.h"

AsmPascal::AsmPascal()
{

}

void AsmPascal::Program(QString name)
{
    Write("program "+name);
}

void AsmPascal::EndProgram()
{
    Write("end.");
}

void AsmPascal::VarDeclHeader()
{
    Write("var",m_currentLvl);
}

void AsmPascal::DeclareVariable(QString name, QString type, QString initval)
{
    Write(name + " : " + type +";",1 + m_currentLvl);
}

void AsmPascal::DeclareString(QString name, QStringList InitVal)
{

}

void AsmPascal::BeginBlock()
{

    Write("begin", m_currentLvl);
    m_currentLvl++;
}

void AsmPascal::EndBlock()
{
    m_currentLvl--;
    Write("end",m_currentLvl);
}

void AsmPascal::AssignVariable(QString var)
{
    m_term += var + ":=";
}

void AsmPascal::ApplyTerm()
{
    Write(m_term +";", m_currentLvl);
    ClearTerm();
}

void AsmPascal::Number(QString s)
{
    m_term+=s;
}

QString AsmPascal::String(QStringList s)
{
    return "";
//    m_term+="\"" + s + "\"";
}

void AsmPascal::BinOP(TokenType::Type t)
{
    if (t==TokenType::PLUS)
        m_term+=" + ";
    if (t==TokenType::MINUS)
        m_term+=" - ";
    if (t==TokenType::DIV)
        m_term+=" / ";
    if (t==TokenType::MUL)
        m_term+=" * ";


}

void AsmPascal::Writeln()
{
    ClearTerm();
    m_term +="Writeln(";
}

void AsmPascal::EndWriteln()
{
    Write(m_term +",0)");
    ClearTerm();
}


*/
