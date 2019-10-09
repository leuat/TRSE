#include "cras.h"
#include <QDebug>
#include <QTextDocument>

int main(int argc, char *argv[])
{
    CRasExec ras(argc, argv);
    return ras.Perform();
}

CRasExec::CRasExec(int argc, char *argv[]) {
    for (int i=1;i<argc;i++) {
        m_args.append(QString(argv[i]));
        QStringList l = QString(argv[i]).split("=");
        if (l.count()!=2) {
            qInfo() << "Error : parameters must be of the format  'p1=someting'";
            m_hasError = true;
            break;
        }
        else
        m_vals[l[0].toLower().trimmed()] = l[1].trimmed();
    }
}

void CRasExec::RequireParam(QString param)
{
    if (!m_vals.contains(param))
        throw QString("Parameter '"+param+"' is required to be defined.");
}

void CRasExec::RequireFile(QString param)
{
    RequireParam(param);
    if (!QFile::exists(m_vals[param]))
      throw QString("Could not find file: " + m_vals[param]);
}

int CRasExec::Perform()
{
    if (m_hasError)
        return 1;

    try {
        RequireFile("settings");
        RequireFile("project");
        if (!QFile::exists(m_vals["settings"])) {
            throw QString("Could not open TRSE settings file: " + m_vals["settings"]);
        }
        RequireFile("input_file");

        m_settings.Load(m_vals["settings"]);

        if (m_vals["type"].toLower()=="project") {
            m_project.Load(m_vals["project"]);
            CompileFromProject(m_vals["input_file"]);

        }

    }
    catch (QString s) {
        out() << "Fatal error:" << endl;
        out() << s <<endl;
        return 1;
    }
    return 0;
}

int CRasExec::CompileFromProject(QString sourceFile)
{
    QString system = m_project.getString("system");
    Syntax::s.Init(AbstractSystem::SystemFromString(system),&m_settings, &m_project);
    QString source = Util::loadTextFile(sourceFile);
//    qDebug() << source;
    m_builder = new SourceBuilder(&m_settings, &m_project, QDir::currentPath(), sourceFile);
    if (m_builder->Build(source)) {
        QTextDocument doc;
        doc.setHtml( m_builder->getOutput() );

        out() << doc.toPlainText();
    }
}
