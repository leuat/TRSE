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

    QString topic = "tru";
    if (file.toLower().endsWith("ras"))
        topic = "prg";
    QString unitName = Util::getFileWithoutEnding(file).replace("\\","/");
    unitName = unitName.split("/").last();
//    QString doc = "<h1>Hello World!</h1><br>"+file;

    QString curDoc = "";
    bool inAddSection = false;

    QStringList lst = text.split("\n");
    for (QString s:lst) {
        if (s.trimmed().startsWith("///")) {
            s = s.remove("///");
            curDoc+=s;
        }
        if (s.trimmed().startsWith("/**")) {
            inAddSection = true;
            continue;
        }
        if (s.trimmed().startsWith("**/")) {
            inAddSection = false;
            continue;
        }
        if (inAddSection)
           curDoc +=s +"\n";

        if (curDoc!="" && !inAddSection)
        if (s.trimmed().toLower().startsWith("procedure") || s.trimmed().toLower().startsWith("function")) {
            QString name = unitName+"::"+s.trimmed().split(" ")[1].split("(").first();
            QString parameters="none";

            if (s.contains("(") && s.contains(")"))
                parameters = s.trimmed().split("(").last().split(")").first();

            if (parameters=="")
                parameters = "none.";
            curDoc = "<h1>"+name+"</h1><p>Parameters: <b>"+parameters+"</b><br></p>"+curDoc +"<br><br>";

            m_documents.append(QSharedPointer<HelpDocumentBuilderDocument>(
                                   new HelpDocumentBuilderDocument(topic, name, curDoc)));
           curDoc = "";
        }
    }

}
