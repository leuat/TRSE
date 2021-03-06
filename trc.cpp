#include "trc.h"
#include <QDebug>
#include <QTextDocument>
#include <stdio.h>

int main(int argc, char *argv[])
{
//    QCoreApplication app(argc, argv);
    ClascExec ras(argc, argv);
    return ras.Perform();
  //  return app.exec();
}

ClascExec::ClascExec(int argc, char *argv[]) {
    Out("Welcome to trc, the TRSE command-line assembler/compiler!");
    Out("");

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

void ClascExec::RequireParam(QString param)
{
    if (!m_vals.contains(param))
        throw QString("Parameter '"+param+"' is required.");
}

void ClascExec::RequireFile(QString param)
{
    RequireParam(param);
    if (!QFile::exists(m_vals[param]))
      throw QString("Could not find file: " + m_vals[param]);
}

int ClascExec::Perform()
{
    if (m_hasError)
        return 1;

    try {
        RequireFile("input_file");
        RequireParam("op");
        QString op = m_vals["op"].toLower();
        if (m_vals.contains("output_file"))
            m_outputFile = m_vals["output_file"];

        if (op=="project") {
            RequireFile("settings");
            RequireFile("project");
            m_settings.Load(m_vals["settings"]);

            m_project.Load(m_vals["project"]);
            m_failure = CompileFromProject(m_vals["input_file"]);
        }
        else
            if (op=="orgasm") {
            m_failure= Assemble(m_vals["input_file"]);
        }
        else {
            m_failure = 1;
            Out("Unknown operation: " +op);
        }

    }
    catch (QString s) {
        PrintUsage();
        Out("Fatal error:");
        Out(s);
        return 1;
    }


    return m_failure;
}

int ClascExec::CompileFromProject(QString sourceFile)
{
    QString system = m_project.getString("system");
    Out("Compiling '"+sourceFile+"' for system: " +system);
    Out("");
    Syntax::s.Init(AbstractSystem::SystemFromString(system),&m_settings, &m_project);
    QString source = Util::loadTextFile(sourceFile);
//    Out(QDir::currentPath());
    m_builder = new SourceBuilder(&m_settings, &m_project, QDir::currentPath()+"/", sourceFile);
    int failure=0;
    if (!m_builder->Build(source)) {
        failure=1;
    }
    if (m_outputFile!="")
        QFile::rename(m_builder->m_filename+".asm", m_outputFile);

    QTextDocument doc;
    doc.setHtml( m_builder->getOutput() );
    Out(doc.toPlainText());
    return failure;
}

int ClascExec::Assemble(QString file)
{
    Orgasm orgAsm;
    QString filename = file.split(".")[0] + ".prg";
    if (m_outputFile!="")
        filename = m_outputFile;
/*    for(QString k: symTab->m_constants.keys()) {
        orgAsm.m_constants[k] = Util::numToHex(symTab->m_constants[k]->m_value->m_fVal);
    }
*/
    Out("Assembling file:'"+filename);
    orgAsm.Assemble(file, filename);
    Out(orgAsm.m_output);
    if (orgAsm.m_output.contains("Complete"))
        return 0;

    return 1;
}

void ClascExec::PrintUsage()
{
    Out("Usage: ");
    Out("");
    Out("trc op=[ operation types ] input_file=[ source file ] output_file=[ optional output file ].... [ type specific operation parameters ]");
    Out("Valid operation types are: project, orgasm");
    Out("");
    Out("Examples: ");
    Out("  compile a project with a main source file: ");
    Out("     trc op=project   settings=~/.TRSE/fluff64.ini project=myDemo.trse input_file=main_demo.ras");
    Out("");
    Out("  Use OrgAsm to assemble an .asm to .prg: ");
    Out("     trc op=orgasm   input_file=main_demo.asm");
    Out("");;
}

void ClascExec::Out(QString m)
{
 //   qInfo() << m << endl;
//    printf(m.toStdString().c_str());
    std::cout << m.toStdString() << std::endl;
}
