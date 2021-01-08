#include "helpdocumentbuilder.h"
#include "source/LeLib/util/util.h"
HelpDocumentBuilder::HelpDocumentBuilder()
{

}

void HelpDocumentBuilder::ProcessSourceFile(QString file)
{
    if (!QFile::exists(file))
        return;

    QString text = Util::loadTextFile(file);

    QString topic = "Units";
    QString name = "test1";
    QString doc = "<h1>Hello World!</h1>";


    m_documents.append(QSharedPointer<HelpDocumentBuilderDocument>(
                           new HelpDocumentBuilderDocument(topic, name, doc)));
}
