/*#ifndef ASMPASCAL_H
#define ASMPASCAL_H

#include "source/Compiler/assembler/assembler.h"

class AsmPascal : public Assembler
{
public:
    AsmPascal();
    int m_currentLvl = 0;
    void Program(QString name) override;
    void EndProgram() override;
    void VarDeclHeader( ) override;
    void DeclareVariable(QString name, QString type, QString InitVal) override;
    void DeclareString(QString name, QStringList InitVal) override;
    void BeginBlock() override;
    void EndBlock() override;
    void AssignVariable(QString var) override;
    void ApplyTerm() override;
    void Number(QString n) override;
    QString String(QStringList s) override;
    void BinOP(TokenType::Type t) override;
    void Poke(bool b) override  { }
    void Writeln() override;
    void EndWriteln() override;


};

#endif // ASMPASCAL_H
*/
