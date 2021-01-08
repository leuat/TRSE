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
    QString unitName = Util::getFileWithoutEnding(file).split(QDir::separator()).last();
//    QString doc = "<h1>Hello World!</h1><br>"+file;

    QString curDoc = "";
    QStringList lst = text.split("\n");
    for (QString s:lst) {
        if (s.trimmed().startsWith("///")) {
            s = s.remove("///");
            curDoc+=s;
        }
        if (curDoc!="")
        if (s.trimmed().toLower().startsWith("procedure") || s.trimmed().toLower().startsWith("function")) {
            QString name = unitName+"::"+s.trimmed().split(" ")[1].split("(").first();
            QString parameters="none";
            if (s.contains("(") && s.contains(")"))
                parameters = s.trimmed().split("(").last().split(")").first();



            curDoc = "<h1>"+name+"</h1><p>Parameters: <b>"+parameters+"</b><br>"+curDoc;
            m_documents.append(QSharedPointer<HelpDocumentBuilderDocument>(
                                   new HelpDocumentBuilderDocument(topic, name, curDoc)));
           curDoc = "";
        }
    }

}
