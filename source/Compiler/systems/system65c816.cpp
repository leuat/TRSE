#include "system65c816.h"

System65C816::System65C816(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : AbstractSystem(settings, proj) {
    m_allowedGlobalTypeFlags << "compressed"<<"pure"<<"pure_variable" <<"pure_number" << "signed" <<"no_term" <<"invert"<<"bank";
    m_allowedProcedureTypeFlags << "pure"<<"pure_variable" <<"pure_number" << "signed" <<"no_term" <<"invert" <<"global" <<"stack";
    m_supportsExomizer = true;
    m_registers << "_a"<<"_x" <<"_y" <<"_ax" <<"_ay" <<"_xy";
    m_canRunAsmFiles = true;
    m_allowClasses = true; // EXPERIMENTAL
    m_processor = WDC65C816;

    m_renameVariables << "a" <<"x"<<"y"<<"pc"<<"sp";

}

void System65C816::Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab)
{
    if (Syntax::s.m_currentSystem->m_system==SNES) {
        QString smc = currentDir + QDir::separator() + "smc.cfg";
        if (!QFile::exists(smc))
            Util::CopyFile(":resources/code/snes/smc.cfg",smc);
        QStringList params = QStringList() <<"-C" <<smc <<("-o"+filename+".smc") <<(filename +".asm");

        QString music = currentDir + QDir::separator() + "music.asm";
        if (!QFile::exists(music))
            Util::CopyFile(":resources/code/snes/music.asm",music);


        AssembleCL65(text,filename,currentDir,symTab,"smc",params);
    }
    if (Syntax::s.m_currentSystem->m_system==MEGA65) {
        QString smc = currentDir + QDir::separator() + "config.cfg";
//        if (!QFile::exists(smc))
            Util::CopyFile(":resources/code/mega65/config.cfg",smc);
        QStringList params = QStringList() <<"-C" <<smc <<"--start-addr"<< "$2020"<<("-o"+filename+".prg") <<(filename +".asm") ;
//        QStringList params = QStringList() <<"--cpu"<<"4510"<<(filename +".asm") <<("-o"+filename+".prg");
        AssembleCL65(text,filename,currentDir,symTab,"prg",params);

        QString f= filename+".prg";
        QByteArray da = Util::loadBinaryFile(f);
        da.insert(0,(uchar)01);
        da.insert(1,(uchar)0x20);
        Util::SaveByteArray(da,f);



    }


}

void System65C816::AssembleCL65(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab, QString ending, QStringList params)
{
    QString output;
    int time = timer.elapsed();
    int codeEnd = 0;
    QString assembler = m_settingsIni->getString("cl65");

    if (!QFile::exists(assembler)) {
        text  += "<br><font color=\"#FF6040\">Please set up a link to the CL65 assembler in the TRSE settings panel.</font>";
        m_buildSuccess = false;
        return;
    }

    //qDebug() << m_settingsIni->getString("assembler");
    emit EmitTick("<br>Assembling with CL65 ...");

    QProcess process;
//    QStringList params = QStringList() <<"-C" <<smc <<("-o"+filename+".smc") <<(filename +".asm");
    process.start(assembler, params);
    process.waitForFinished();
    //process;
    output = process.readAllStandardOutput() + process.readAllStandardError();

    // codeEnd=FindEndSymbol(output);

    int assembleTime = timer.elapsed()- time;
    time = timer.elapsed();

    //        qDebug() << "Code end: " << Util::numToHex(codeEnd) << codeEnd;
    int orgFileSize = QFile(filename+"."+ending).size();

    //    qDebug() << "MOS " << Syntax::s.m_stripPrg << Syntax::s.m_ignoreSys;




    int size = QFile(filename+"."+ending).size();
    int crunchTime = timer.elapsed()- time;



    m_buildSuccess = true;
    m_orgOutput = "";
    if (output.toLower().contains("error")) {
        m_orgOutput = output;
        text="<font color=\"#FF6040\">Fatal error during assembly!</font><br>";
        m_buildSuccess = false;
    }

    if (output.trimmed()!="") {
        m_buildSuccess = false;
    }

    if (m_buildSuccess) {

        output ="Assembled file size: <b>" + QString::number(size) + "</b> bytes";
        if (m_projectIni->getdouble("exomizer_toggle")==1) {
            output=output+" (<font color=\"#70FF40\"> " + QString::number((int)(100.0*(float)size/(float)orgFileSize))+  " % </font> of original size ) <br>";
            output=output+"Original file size: " + QString::number(orgFileSize) + " bytes";
        }
        output = output + "\nAssemble time: <font color=\"#70FF40\">"+ (Util::MilisecondToString(assembleTime)) + "</font>";
        output = output + "Crunch time: <font color=\"#70FF40\">"+ (Util::MilisecondToString(crunchTime)) + "</font>";

    }


    text+=output;


}

void System65C816::PostProcess(QString &text, QString file, QString currentDir)
{

}

void System65C816::DefaultValues()
{
    m_ignoreSys = false;
}
