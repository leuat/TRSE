#include "systemcustom.h"
#include <QProcess>
#include "source/Compiler/syntax.h"
#include "source/LeLib/util/util.h"
#include "source/Compiler/assembler/assembler.h"
SystemCustom::SystemCustom(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : AbstractSystem(settings, proj) {
    m_allowedGlobalTypeFlags << "compressed"<<"pure"<<"pure_variable" <<"pure_number" << "signed" <<"no_term" <<"invert"<<"bank"<<"lpointer";
    m_allowedProcedureTypeFlags << "pure"<<"pure_variable" <<"pure_number" << "signed" <<"no_term" <<"invert" <<"global" <<"stack"<<"bank"<<"lpointer";
    m_supportsExomizer = false;
    //        m_registers << "_a"<<"_x" <<"_y" <<"_ax" <<"_ay" <<"_xy";
    m_canRunAsmFiles = false;
    m_allowClasses = true; // EXPERIMENTAL
    m_processor = ProcessorFromString(proj->getString("custom_system_cpu"));
//    qDebug() << "SYSTEMCUSTOM" <<(m_processor==WDC65C816);
    m_alternateMethods = proj->getString("custom_system_methods");
    m_requireEmulatorWorkingDirectory = true;
    m_allowedBaseTypes<<"LONG";

}

void SystemCustom::Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab)
{
    QString output="";
    QString ext = m_projectIni->getString("custom_system_ending");
    if (ext=="") {
        text+="<br><font color=\"yellow\"><b>No default extension. </b>Assuming '.bin' as default - please set up in the project settings!'</font><br>";
        ext=".bin";
    }
    if (!ext.startsWith("."))
        ext = "."+ext;

    int time = timer.elapsed();
    bool useInternal = m_projectIni->getString("custom_system_assembler").toLower()=="from system";
    QStringList params = m_projectIni->getString("custom_system_assembler_parameters").trimmed().simplified().split(" ");

    for (QString& s:params)
        s = s.replace("@prg",filename);

    QString assembler = m_projectIni->getString("custom_system_assembler_custom");

    if (useInternal) {
        if (m_processor==MOS6502) {
            AssembleOrgasm(output,text,filename,currentDir,symTab);

            if (Syntax::s.m_currentSystem->m_stripPrg)
                Util::ConvertFileWithLoadAddress(filename+".prg", filename+".prg");
            if (ext!=".prg") {
                Util::CopyFile(filename+".prg",filename+ext);
                QFile::remove(filename+".prg");
            }
        }
        if (m_processor==Z80) {
            AssembleZOrgasm(output,text,filename,currentDir,symTab);

            if (ext!=".bin") {
                Util::CopyFile(filename+".bin",filename+ext);
                QFile::remove(filename+".bin");
            }
        }
        if (m_processor==WDC65C02 || m_processor==WDC65C816) {
            //  AssembleZOrgasm(text,filename,currentDir,symTab);
            ext = ext.remove(".");
            AssembleCL65(text,filename,currentDir,symTab,ext,params);
            return;
        }
        if (m_processor==M68000) {
            //  AssembleZOrgasm(text,filename,currentDir,symTab);
            assembler = m_settingsIni->getString("vasmm");
            useInternal = false;

        }
    }

    if (!useInternal) {
        QProcess process;
        //    QStringList params = QStringList() <<"-C" <<smc <<("-o"+filename+".smc") <<(filename +".asm");
        process.start(assembler, params);
        process.waitForFinished();
        //process;
        output = process.readAllStandardOutput() + process.readAllStandardError();
    }


    int assembleTime = timer.elapsed()- time;
    time = timer.elapsed();

    //        qDebug() << "Code end: " << Util::numToHex(codeEnd) << codeEnd;
    int orgFileSize = QFile(filename+ext).size();

    //    qDebug() << "MOS " << Syntax::s.m_stripPrg << Syntax::s.m_ignoreSys;

    int size = QFile(filename+ext).size();



    m_buildSuccess = true;
    m_orgOutput = "";
    if (output.toLower().contains("error")) {
        m_orgOutput = output;
        text="<font color=\"#FF6040\">Fatal error during assembly!</font><br>";
        m_buildSuccess = false;
    }

    if (!QFile::exists(filename+ext)) {
        m_buildSuccess = false;

        text+="<br>Error during assembly: <br><br>"+output;
    }

    if (m_buildSuccess) {

        output ="Assembled file size: <b>" + QString::number(size) + "</b> bytes<br>";
        text+=output;



    }
}

void SystemCustom::PostProcess(QString &text, QString file, QString currentDir)
{

}

bool SystemCustom::CL65Syntax() {
    return (m_processor==WDC65C02 || m_processor==WDC65C816);
}

int SystemCustom::addressBusBits() {
    if ((m_processor==WDC65C02 || m_processor==WDC65C816))
        return 24;

    return 16;
}

QString SystemCustom::getCPUAssemblerString() {
    if (m_processor==WDC65C02) {
     return ".p4510   ; 65816 processor";
    }
    if (m_processor==WDC65C816) {
        return ".p816   ; 65816 processor";
    }
    return "";
}

void SystemCustom::PrepareInitialAssembler(Assembler* as) {
    as->Write(as->GetOrg(Syntax::s.m_currentSystem->m_programStartAddress));

}
