#ifndef POSTOPTIMIZER6809_H
#define POSTOPTIMIZER6809_H

#include "postoptimiser.h"

class PostOptimiser6809 : public PostOptimiser
{
public:
    PostOptimiser6809();
    QStringList m_axModifiers;
    QMap<QString, QString> m_lastMemoryStoredValue;
    void InitRegisters();

    QString prevCmd="";
    QStringList prevPar;
    SourceLine* prevLine = nullptr;

    void Analyze(SourceLine& line);
    void ChangeReg(SourceLine& line, QString reg, QString val);
};

#endif // POSTOPTIMIZERX86_H
