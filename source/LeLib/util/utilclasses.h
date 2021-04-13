#ifndef UTILCLASSES_H
#define UTILCLASSES_H

#include <QString>
#include <QVector>
#include <QStringList>

class Stack {
public:
    QVector<QString> m_vars;

    QString m_current;
    void push(QString s) {
        m_vars.push_back(s);
        m_current = s;
    }
    QString pop();
  public:
        QString current() const;
};



class RegisterStack {
public:
//    QStringList m_registers;
    QStringList m_free;
    QStringList m_occupied;
//    QVector<QString> m_latest;
    QString m_latest;
    RegisterStack() {}
    RegisterStack(QStringList vals) {
        m_free = vals;
    }
    int m_current = 0;
    QString Get();
    QString Get(int i) {
        if (i<m_free.count())
            return m_free[i];
        throw QString("RegisterStack::Get internal error: index out of bounds when retrieving stack");
    }
    void Pop(QString reg);

    int count() {
        return m_free.count();
    }

    QString operator[] (int i) {return Get(i);}

};

#endif // UTILCLASSES_H
