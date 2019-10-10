#include "clasc.h"
#include <QDebug>
#include <QTextDocument>

int main(int argc, char *argv[])
{
    std::cout << "Welcome to clasc, the TRSE command-line assembler/compiler!"<<endl<<endl;
    ClascExec ras(argc, argv);
    return ras.Perform();
}

ClascExec::ClascExec(int argc, char *argv[]) {
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
        throw QString("Parameter '"+param+"' is required to be defined.");
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
        else if (op=="orgasm") {
            m_failure= Assemble(m_vals["input_file"]);
        }

    }
    catch (QString s) {
        PrintUsage();
        out() << "Fatal error:" << endl;
        out() << s <<endl;
        return 1;
    }


    return m_failure;
}

int ClascExec::CompileFromProject(QString sourceFile)
{
    QString system = m_project.getString("system");
    out() << "Compiling '"<<sourceFile<<"' for system: " <<system <<endl;
    Syntax::s.Init(AbstractSystem::SystemFromString(system),&m_settings, &m_project);
    QString source = Util::loadTextFile(sourceFile);
    m_builder = new SourceBuilder(&m_settings, &m_project, QDir::currentPath(), sourceFile);
    if (!m_builder->Build(source)) {
        return 1;
    }
    if (m_outputFile!="")
        QFile::rename(m_builder->m_filename+".asm", m_outputFile);

    QTextDocument doc;
    doc.setHtml( m_builder->getOutput() );
    out() << doc.toPlainText() << endl;
    return 0;
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
    out()<< "Assembling file:'"<< filename<<endl;
    orgAsm.Assemble(file, filename);
    out() << orgAsm.m_output << endl;
    if (orgAsm.m_output.contains("Complete"))
        return 0;

    return 1;
}

void ClascExec::PrintUsage()
{
    out() << endl;
    out() << "Usage: " << endl;
    out() << "clasc op=[ operation types ] input_file=[ source file ] output_file=[ optional output file ].... [ type specific operation parameters ]"<<endl;
    out() << "Valid operation types are: project, orgasm"<<endl<<endl;
    out() << "Examples: " <<endl;
    out() << "  compile a project with a main source file: " <<endl;
    out() << "     clasc op=project   settings=~/.TRSE/fluff64.ini project=myDemo.trse input_file=main_demo.ras" <<endl <<endl;
    out() << "  Use OrgAsm to assemble an .asm to .prg: " <<endl;
    out() << "     clasc op=orgasm   input_file=main_demo.asm" <<endl;
    out() << endl;
}
