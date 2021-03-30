#ifndef POSTOPTIMIZERX86_H
#define POSTOPTIMIZERX86_H

#include "postoptimiser.h"

class PostOptimiserX86 : public PostOptimiser
{
public:
    PostOptimiserX86();
    QStringList m_axModifiers;
    void InitRegisters();

    void Analyze(SourceLine& line);
    void ChangeReg(SourceLine& line, QString reg, QString val);
};

#endif // POSTOPTIMIZERX86_H
