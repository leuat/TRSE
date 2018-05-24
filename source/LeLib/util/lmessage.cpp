#include "lmessage.h"
#include <QDateTime>

LMessage LMessage::lMessage;

LMessage::LMessage(int max)
{
    m_max = max;
}

LMessage::LMessage() {

    m_max = 15;
}

void LMessage::Maintain()
{
    if (m_messages.length()>m_max)
        m_messages.removeFirst();
}

void LMessage::Message(QString s)
{
    m_messages.append(QDateTime::currentDateTime().toString() + " : " +  s);
    Maintain();
}

void LMessage::Error(QString s)
{
    m_messages.append(QDateTime::currentDateTime().toString() + " : <font color=\"red\">" +  s + "</font>");
    Maintain();

}



QString LMessage::Build()
{
    QString m = "";
    for (QString s : m_messages) {
        m = m + s + "<br>";
    }
    return m;
}
