#ifndef ASMZ80_H
#define ASMZ80_H



#include "source/Compiler/assembler/assembler.h"

class AsmZ80 : public Assembler
{
public:
    AsmZ80();
    ~AsmZ80() {}



    void Connect() override;

    void Program(QString name, QString vicParam) override;
    void EndProgram() override;
    void Write(QString str, int level=1) override;


    void DeclareArray(QString name, QString type, int count, QStringList data, QString pos) override;
    void DeclareVariable(QString name, QString type, QString initval, QString position) override;
    void DeclareString(QString name, QStringList initVal, QStringList flags) override;

    void BinOP(TokenType::Type t, bool clearFlag=true) override;

    int CodeSizeEstimator(QStringList& lines) override;

    void Label(QString s) override;

    QString GetOrg(int pos) override;

    QString String(QStringList lst, bool term) override;


    void StartMemoryBlock(QString pos) override;

    void EndMemoryBlock()  override;



};

#endif // ASMX86_H
