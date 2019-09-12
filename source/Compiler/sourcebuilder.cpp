#include "sourcebuilder.h"

SourceBuilder::SourceBuilder()
{

}

SourceBuilder::SourceBuilder(CIniFile *ini, CIniFile *project, QString curDir, QString curSourceFile)
{
    m_iniFile = *ini; // Make copies for those changs
    m_projectIniFile = *project;
    m_curDir = curDir;
    m_currentSourceFile = curSourceFile;
}

bool SourceBuilder::Build(QString source)
{
    if (m_currentSourceFile.toLower().endsWith(".asm")) {
        m_buildSuccess=true;
       m_assembleSuccess=false;
       m_filename = m_currentSourceFile.split(".")[0];
       if (m_system != nullptr)
           delete m_system;
       m_system = FactorySystem::Create(AbstractSystem::SystemFromString(
                                            m_projectIniFile.getString("system")),
                                           &m_iniFile, &m_projectIniFile);

       compiler = Compiler(&m_iniFile, &m_projectIniFile);
       return true;
    }

    m_buildSuccess = false;
    m_assembleSuccess = false;
    m_output = "";
    if (!m_currentSourceFile.toLower().endsWith(".ras")) {
       m_output = "Turbo Rascal SE can only compile .ras files";
        return false;
    }
    m_source = source;
//    QString text = ui->txtEditor->toPlainText();
    ErrorHandler::e.m_level = ErrorHandler::e.ERROR_ONLY;
    ErrorHandler::e.m_teOut = "";
    ErrorHandler::e.exitOnError = false;
    QStringList lst = source.split("\n");


    compiler = Compiler(&m_iniFile, &m_projectIniFile);
    compiler.m_parser.m_diskFiles = getFileList();
    compiler.m_parser.m_currentDir = m_curDir;

    compiler.Parse(source,lst);

    QString path = m_projectIniFile.getString("project_path") + "/";
    m_filename = m_currentSourceFile.split(".ras")[0];
//    m_filename = m_curDir+"/"+ m_currentSourceFile.split(".")[0];

    if (m_system != nullptr)
        delete m_system;


    m_system = FactorySystem::Create(AbstractSystem::SystemFromString(
                                         m_projectIniFile.getString("system")),
                                        &m_iniFile, &m_projectIniFile);

    m_system->timer.start();
    m_system->m_buildSuccess = true;
    m_buildSuccess = compiler.Build(m_system, path);
    if (m_buildSuccess)
        BuildSuccesString();

     compiler.SaveBuild(m_filename + ".asm");
//     qDebug() << "Saving to "+m_filename + ".asm";

     return m_buildSuccess;
}


bool SourceBuilder::Assemble()
{
//    qDebug() << m_filename << m_curDir;
//    qDebug() << m_system;
    m_system->Assemble(m_output,m_filename, m_curDir,compiler.m_parser.m_symTab);
    if (m_system->m_buildSuccess)
        m_system->PostProcess(m_output, m_filename, m_curDir);

  //  qDebug() << "AA2";
    m_assembleSuccess=m_system->m_buildSuccess;
    return true;
}

QString SourceBuilder::getOutput() {
    return ErrorHandler::e.m_teOut +m_output;
}

QStringList SourceBuilder::getFileList()
{
    QString pawFile = m_projectIniFile.getString("d64_paw_file");
    if (pawFile=="none") return QStringList();
    CIniFile paw;
    paw.Load(m_curDir + "/"+pawFile);
    QStringList data = paw.getStringList("data");
    QStringList ret;
    int count = data.count()/3;
    for (int i=0;i<count;i++) {
        ret<< data[3*i];
    }

    QString pawFile2 = m_projectIniFile.getString("d64_paw_file_disk2");
    if (pawFile2=="none") return ret;
    CIniFile paw2;
    paw2.Load(m_curDir + "/"+pawFile2);
    QStringList data2 = paw2.getStringList("data");
    int count2 = data2.count()/3;
    for (int i=0;i<count2;i++) {
        ret<< data2[3*i];
    }


    return ret;

}

void SourceBuilder::BuildSuccesString()
{
    QString text ="Build <b><font color=\"#90FF90\">Successful</font>!</b> ( "+  (Util::MilisecondToString(m_system->timer.elapsed())) +")<br>";
    text+="Assembler file saved to : <b>" + m_filename+".asm</b><br>";
    text+="Compiled <b>" + QString::number(compiler.m_parser.m_lexer->m_lines.count()) +"</b> lines of Turbo Rascal to <b>";
    text+=QString::number(compiler.m_assembler->getLineCount()) + "</b> lines of assembler instructions (and variables/labels)<br>";
    if (m_iniFile.getdouble("post_optimize")==1) {
        text+="Post-optimized away <b>" + QString::number(compiler.m_assembler->m_totalOptimizedLines) +"</b> lines of assembler instructions ";
        text=text+"(<font color=\"#70FF40\"> " + QString::number((int)(100.0*(float)compiler.m_assembler->m_totalOptimizedLines/(float)compiler.m_assembler->getLineCount()))+  " % </font> of total ) <br>";

    }
    else
        text+="<font color=\"#FFA090\">Warning:</font>Post-optimizer disabled. Enable for faster results (unless post-optimizer breaks something).<br>";

    m_output+=text;
}
