#include "utilclasses.h"
#include <QDebug>
#include "source/Compiler/errorhandler.h"

QString Stack::pop() {
    if (m_vars.count()==0) {
        qDebug() << "Trying to POP stack from 0 CONTACT leuat@irio.co.uk this shouldn't happen EVER";
        return "NADA empty popped stack BUG contact leuat@.irio.co.uk";
//        exit(1);
    }
    m_current=m_vars[m_vars.count()-1];
    //m_vars.removeLast();
    m_vars.remove(m_vars.count()-1);
    //        m_current = m_vars.last();
    return m_current;
}

QString Stack::current() const
{
    return m_current;
}


QString RegisterStack::Get() {
    if (m_free.count()!=0) {
        QString reg = m_free[0];
        m_free.removeFirst();
        m_latest.append(reg);
        return reg;
    }
    //    qDebug() << "NO FREE REGISTERS :  RegisterStack::Get()";
    //  exit(1);
    ErrorHandler::e.Error("NO FREE Registers (shouldn't happen)",0);
    return QString();
}

void RegisterStack::Pop(QString reg) {
    m_free.insert(0,reg);
    m_free.removeDuplicates();
    //m_latest.removeAll(reg);

}

/*QString RegisterStack::getLatest() {
    if (m_latest.count()!=0) {
        QString l = m_latest.last();
        m_latest.removeLast();
        return l;
    }

    qDebug() << "NO LATEST :  RegisterStack::getLatest()";
    exit(1);

}

QString RegisterStack::peekLatest() {
    if (m_latest.count()!=0) {
        return m_latest.last();
    }

    qDebug() << "NO LATEST :  RegisterStack::peekLatest()";
    exit(1);

}
*/
