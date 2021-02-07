#ifndef HELPDOCUMENTBUILDER_H
#define HELPDOCUMENTBUILDER_H

#include <QString>
#include <QSharedPointer>
#include <QVector>

class HelpDocumentBuilderDocument;

class HelpDocumentBuilderDocument {
    public:
    QString m_topic;
    QString m_name;
    QString m_document;
    HelpDocumentBuilderDocument() {

    }
    HelpDocumentBuilderDocument(QString topic, QString name, QString doc) {
        m_topic = topic;
        m_name = name;
        m_document = doc;
    }
};

class HelpDocumentBuilder
{
public:
    HelpDocumentBuilder();
    QVector<QSharedPointer<HelpDocumentBuilderDocument>> m_documents;

    void ProcessSourceFile(QString file);






};

#endif // HELPDOCUMENTBUILDER_H
