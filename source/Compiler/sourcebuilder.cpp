#include "sourcebuilder.h"

SourceBuilder::SourceBuilder()
{

}

SourceBuilder::SourceBuilder(QSharedPointer<CIniFile> ini, QSharedPointer<CIniFile> project, QString curDir, QString curSourceFile)
{
    m_iniFile = QSharedPointer<CIniFile>(new CIniFile);
    m_projectIniFile = QSharedPointer<CIniFile>(new CIniFile);
    *m_iniFile = *ini; // Make copies for those changs
    *m_projectIniFile = *project;
    m_curDir = curDir;
    m_currentSourceFile = curSourceFile;
    m_isTRU = m_currentSourceFile.toLower().endsWith(".tru");
}

SourceBuilder::~SourceBuilder() {
    //    qDebug() << "~SourceBuilder destroying : "<<compiler;
}

bool SourceBuilder::Build(QString source)
{
    m_system = QSharedPointer<AbstractSystem>(FactorySystem::Create(AbstractSystem::SystemFromString(
                                                                        m_projectIniFile->getString("system")),
                                                                    m_iniFile,
                                                                    m_projectIniFile));

    compiler = QSharedPointer<Compiler>(FactoryCompiler::CreateCompiler(m_iniFile, m_projectIniFile));
    connect(compiler.get(), SIGNAL(emitRequestSystemChange(QString)), this, SLOT( AcceptRequestSystemChange(QString)));


    compiler->ApplyOptions(m_options);

    if (m_currentSourceFile.toLower().endsWith(".asm")) {
        m_buildSuccess=true;
        m_assembleSuccess=false;

        m_filename = Util::getFileWithoutEnding(m_currentSourceFile);
        /*       m_system = FactorySystem::Create(AbstractSystem::SystemFromString(
                                            m_projectIniFile.getString("system")),
                                           &m_iniFile, &m_projectIniFile);
*/
        m_useSyntaxSystem = true;
        return true;
    }

    m_useSyntaxSystem = false;
    m_buildSuccess = false;
    m_assembleSuccess = false;
    m_output = "";
    if (!(m_currentSourceFile.toLower().endsWith(".ras") || m_currentSourceFile.toLower().endsWith(".tru"))) {
        m_output = "Turbo Rascal SE can only compile .ras files";
        return false;
        //ErrorHandler::e.CatchError(fe, e + msg);

        //      return false;
        //        ErrorHandler::e.Error("BALLE");
    }
    m_source = source;
    //    QString text = ui->txtEditor->toPlainText();
    ErrorHandler::e.m_level = ErrorHandler::e.ERROR_ONLY;
    ErrorHandler::e.m_teOut = "";
    ErrorHandler::e.exitOnError = false;
    source = source.replace("\r","");
    QStringList lst = source.split("\n");


    //    qDebug() << "CREATED COMPILER " <<compiler;
    if (!m_isShadow)
        connect(&compiler->m_parser, SIGNAL(EmitTick(QString)), this, SLOT( AcceptParserTick(QString)));

    compiler->m_parser.m_diskFiles = getFileList();
    compiler->m_parser.m_currentDir = m_curDir;

    //    qDebug() << lst;
    compiler->m_isTRU = m_isTRU;
    /*
     *   P A R S I N G
     *
     *
    */
    m_system->timer.start();
    m_system->m_buildSuccess = true;
    int start = m_system->timer.elapsed();
    compiler->Parse(source,lst, m_currentSourceFile);
    int end = m_system->timer.elapsed();
    m_output+="Parse time: <font color=\"#30FF30\">"+Util::MilisecondToString(end-start)+"</font>. ";



    //    if (compiler->m_tree==nullptr)
    //      return true;

    QString path = m_curDir+"/";//m_projectIniFile.getString("project_path") + "/";
    if (m_currentSourceFile.toLower().endsWith(".ras"))
        m_filename = m_currentSourceFile.split(".ras")[0];
    if (m_currentSourceFile.toLower().endsWith(".tru"))
        m_filename = m_currentSourceFile.split(".tru")[0];



    /*    m_system->m_buildSuccess = false;
    return false;
*/
    if (!m_isShadow)
        connect(compiler.get(), SIGNAL(EmitTick(QString)), this, SLOT( AcceptParserTick(QString)));

    // Main BUILD of the assembler (dispatcher)
    m_buildSuccess = compiler->Build(m_system, path);
    //    m_buildString+="<br>Post";
    //    emit EmitBuildString();

    if (!m_isShadow)
        disconnect(&compiler->m_parser, SIGNAL(EmitTick(QString)), this, SLOT( AcceptParserTick(QString)));
    if (!m_isShadow)
        disconnect(compiler.get(), SIGNAL(EmitTick(QString)), this, SLOT( AcceptParserTick(QString)));

    disconnect(compiler.get(), SIGNAL(emitRequestSystemChange(QString)), this, SLOT( AcceptRequestSystemChange(QString)));



    /*    if (m_buildSuccess)
        BuildSuccesString();
*/
    if (!m_isShadow)
        compiler->SaveBuild(m_filename + ".asm");
    //     qDebug() << "Saving to "+m_filename + ".asm";


    return m_buildSuccess;
}

void SourceBuilder::Destroy()
{
    /*    if (compiler!=nullptr) {
        qDebug() << "~SourceBuilder TRYING TO DESTROY COMPILER " <<compiler;
        compiler->Destroy();
        delete compiler;
        compiler = nullptr;
    }*/
    /*    if (m_system!=nullptr && !m_useSyntaxSystem) {
        delete m_system;
    }
    m_system = nullptr;
*/
}


bool SourceBuilder::Assemble()
{
    //    qDebug() << m_filename << m_curDir;
    //    qDebug() << m_system;
    if (compiler->m_parser.m_isTRU) {
        BuildSuccesString();

        return true;
    }
    connect(m_system.get(), SIGNAL(EmitTick(QString)), this, SLOT( AcceptParserTick(QString)));
    Data::data.compilerState = Data::ASSEMBLER;
    m_system->Assemble(m_output,m_filename, m_curDir,compiler->m_parser.m_symTab);
    if (compiler->m_assembler!=nullptr && m_system!=nullptr) {
        compiler->m_assembler->m_addresses = m_system->m_addresses;
        compiler->CleanupCycleLinenumbers("",compiler->m_assembler->m_addresses,compiler->m_assembler->m_addressesOut,false);
    }

    if (m_system->m_buildSuccess)
        m_system->PostProcess(m_output, m_filename, m_curDir);

    //  qDebug() << "AA2";
    m_assembleSuccess=m_system->m_buildSuccess;
    if (m_buildSuccess && m_assembleSuccess)
        BuildSuccesString();

    disconnect(m_system.get(), SIGNAL(EmitTick(QString)), this, SLOT( AcceptParserTick(QString)));

    return true;
}

QString SourceBuilder::getOutput() {
    return ErrorHandler::e.m_teOut +m_output;
}

QStringList SourceBuilder::getFileList()
{
    int disk=1;
    QStringList ret;
    while (m_projectIniFile->contains("disk"+QString::number(disk)+"_paw")) {
        QString d = "disk"+QString::number(disk);
        QString pawFile = m_projectIniFile->getString(d+"_paw");
//        if (pawFile=="none") return QStringList();
        CIniFile paw;
        paw.Load(m_curDir + "/"+pawFile);
        QStringList data = paw.getStringList("data");
        int count = data.length()/3;
        for (int i=0;i<count;i++) {
            ret<< data[3*i];
        }
        disk+=1;
    }



    return ret;

}

void SourceBuilder::BuildSuccesString()
{
    QString text ="<br>Build <b><font color=\"#90FF90\">Successful</font>!</b> ( "+  (Util::MilisecondToString(m_system->timer.elapsed())) +")<br>";
    //    text+="Assembler file saved to : <b>" + m_filename+".asm</b><br>";
    if (compiler!=nullptr && compiler->m_parser.m_lexer!=nullptr)
        text+="Compiled <b>" + QString::number(compiler->m_parser.m_lexer->getTotalNumberOfLines()) +"</b> lines of Turbo Rascal to <b>";
    if (compiler!=nullptr && compiler->m_assembler!=nullptr) {
        int prevCnt = Assembler::m_prevCycles;
        int cnt = compiler->m_assembler->getLineCount();
        Assembler::m_prevCycles = cnt;
        QString extraText="";
        if (abs(prevCnt-cnt)/(float)cnt<0.3) {
            int add = cnt-prevCnt;
            if (add<0)
                extraText+="<font color=\"green\">("+QString::number(add)+" lines)</font>";
            else
                if (add>0)
                    extraText+="<font color=\"yellow\">(+"+QString::number(add)+" lines)</font>";
        }

        text+=QString::number(cnt) + "</b> lines of assembler instructions (and variables/labels) " + extraText + "<br>";
    }
    if (m_iniFile->getdouble("post_optimize")==1 && compiler!=nullptr && compiler->m_assembler!=nullptr) {

        text+="Post-optimized away <b>" + QString::number(compiler->m_assembler->m_totalOptimizedLines) +"</b> lines of assembler instructions ";
        text=text+"(<font color=\"#70FF40\"> " + QString::number((int)(100.0*(float)compiler->m_assembler->m_totalOptimizedLines/(float)compiler->m_assembler->getLineCount()))+  " % </font> of total ) <br>";

    }
    else
        text+="<font color=\"#FFA090\">Warning:</font>Post-optimizer disabled. Enable for faster results (unless post-optimizer breaks something).<br>";

    m_output+=text;
}

void SourceBuilder::AcceptParserTick(QString val)
{
    if (val.startsWith("&")) {
        val = val.remove("&");
        int pos = m_buildString.length()-1;
        while (m_buildString[pos]!='[' && pos>0) {
            m_buildString.remove(pos--,1);
        }
        m_buildString+=" " +val + "% ]";

    }
    else
        m_buildString +=val;

    emit EmitBuildString();
}

void SourceBuilder::AcceptRequestSystemChange(QString val)
{
    emit emitRequestSystemChange(val);
}
