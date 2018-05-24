#ifndef LMESSAGE_H
#define LMESSAGE_H

#include <QString>
#include <QVector>

class LMessage
{
public:

    int m_max = 15;

    QVector<QString> m_messages;

    static LMessage lMessage;

    LMessage(int max);
    LMessage();
    void Maintain();
    void Message(QString s);
    void Error(QString s);
    QString Build();
};

#endif // LMESSAGE_H
