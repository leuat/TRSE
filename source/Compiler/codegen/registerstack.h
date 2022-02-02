#ifndef REGISTERSTACK_H
#define REGISTERSTACK_H

#include <QStringList>
#include "source/Compiler/errorhandler.h"

class GenRegisterStack
{
public:
    QStringList m_regs;
    int m_lvl = 0;
    GenRegisterStack();
    GenRegisterStack(QStringList regs) { m_regs = regs; }

    QString Push();

    void Pop();

    QString Get(int dd=0);

    QString operator[] (int i) {return Get(i);}

};

#endif // REGISTERSTACK_H
