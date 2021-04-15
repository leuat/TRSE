#ifndef POSTOPTIMIZERM68K_H
#define POSTOPTIMIZERM68K_H

#include "postoptimiser.h"

class PostOptimiserM68K : public PostOptimiser
{
public:
    PostOptimiserM68K();
    QStringList m_axModifiers;
    void InitRegisters();

    void Analyze(SourceLine& line);
    void ChangeReg(SourceLine& line, QString reg, QString val);
};

#endif // POSTOPTIMIZERX86_H
