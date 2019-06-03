#include "projectbuilder.h"

ProjectBuilder::ProjectBuilder()
{

}

ProjectBuilder::ProjectBuilder(CIniFile *ini, CIniFile *project, QString curDir)
{
    m_iniFile = *ini; // Make copies for those changs
    m_projectIniFile = *project;
    m_curDir = curDir;

}

void ProjectBuilder::Message(QString s)
{
    m_output +=s+"\n";
    emitTextUpdate();

}

void ProjectBuilder::Build(QString file)
{
    SourceBuilder* sb = new SourceBuilder(&m_iniFile, &m_projectIniFile,m_curDir,m_curDir+"/"+file);
    QString f = m_curDir + "/" + file;
    if (!QFile::exists(f))
        throw QString("Could not find source file: " + f);

    QString source = Util::loadTextFile(f);
    if (sb->Build(source)) {
//        Message("Assembling...");
        sb->Assemble();
        emitTextUpdate();
    }
    else throw QString("Error compiling : " + f+ "\n" + sb->getOutput());

    delete sb;
}

void ProjectBuilder::run()
{
    m_output = "";
    for (QString& s: m_list) {
        QStringList cmdList = s.trimmed().simplified().split(" ");
        if (cmdList.count()==0)
            continue;

        QString cmd = cmdList[0].toLower();
        try {
            if (cmd== "define") {
                if (cmdList.count()!=3)
                    throw QString("Build command 'define' requires two parameters: name and value");

                m_projectIniFile.addStringList("global_defines","@define "+cmdList[1] + " " + cmdList[2],false);
                Message("Adding define: " + cmdList[1] + " " + cmdList[2]);

            }
            if (cmd== "b") {
                if (cmdList.count()!=2)
                    throw QString("Build command 'b' requires an input file");
                Message("Compiling:  " + cmdList[1]);;

                Build(cmdList[1]);
                Message("OK.");
            }
            if (cmd == "setvalue") {
                if (cmdList.count()!=3)
                    throw QString("Error: setvalue requires 2 parameters: a name and a float");
                QString id = cmdList[1];
                float value = cmdList[2].toFloat();
                if (m_iniFile.contains(id))
                    m_iniFile.setFloat(id, value);
                if (m_projectIniFile.contains(id))
                    m_projectIniFile.setFloat(id, value);
                Message("Setting compiler value:  "+id + " to " + QString::number(value));
            }

        }
        catch (QString s) {
            m_output += "Error: " +s + "\n";
            emitTextUpdate();
            return;
        }
    }
}

void ProjectBuilder::LoadBuildList(QStringList buildList)
{
    m_list = buildList;
}
