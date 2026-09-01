#ifndef POSTOPTIMISER6502_H
#define POSTOPTIMISER6502_H

#include "postoptimiser.h"

class PostOptimiser6502 : public PostOptimiser
{
public:
    PostOptimiser6502() ;
//    QStringList m_axModifiers;
    QHash<QString, QString> m_lastMemoryStoredValue;
    void InitRegisters();

    QString prevCmd="";
    QStringList prevPar;
    SourceLine* prevLine = nullptr;

    void Analyze(SourceLine& line);
    void ChangeReg(SourceLine& line, QString reg, QString val);

};

#endif // POSTOPTIMISER6502_H
