#include "systemChip8.h"
#include "source/OrgAsm/orgasm.h"
#include <QElapsedTimer>
#include <QProcess>
#include "source/LeLib/limage/c64fullscreenchar.h"
#include "source/LeLib/limage/limageio.h"
#include "source/LeLib/util/cc1541.h"
#include "source/LeLib/util/dirartd64.h"


SystemChip8::SystemChip8(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : AbstractSystem(settings, proj) {
    m_allowedGlobalTypeFlags << "pure"<<"pure_variable" <<"pure_number" << "signed" <<"no_term" <<"invert";
    m_allowedProcedureTypeFlags << "pure"<<"pure_variable" <<"pure_number" << "signed" <<"no_term" <<"invert" <<"global";
    m_supportsExomizer = true;
    //   m_registers "_y" <<"_ax" <<"_ay" <<"_xy";
    m_canRunAsmFiles = false;
    m_allowClasses = false; // EXPERIMENTAL

    m_processor = PCHIP8;
    m_system = CHIP8;


}

void SystemChip8::Assemble(QString& text, QString filename, QString currentDir, QSharedPointer<SymbolTable>  symTab)
{


    QString output;
    int time = timer.elapsed();
    int codeEnd = 0;

    m_buildSuccess = true;
    AssembleTripe(text,filename,currentDir,symTab);
    if (!m_buildSuccess)
        return;

    //qDebug() << m_settingsIni->getString("assembler");
    emit EmitTick("<br>Assembling with c8asm ...");
    QProcess process;
    if (!QFile::exists(m_settingsIni->getString("c8asm"))) {
        text  += "<br><font color=\"#FF6040\">Please set up a link to the c8asm assembler in the TRSE settings panel, or use c8gasm (which isn't done yet.. so use lwasm).</font>";
        m_buildSuccess = false;
        return;

    }
    process.start(m_settingsIni->getString("c8asm"), QStringList()<< ("-o"+filename+".ch8") << (filename +".asm"));//) << "-v3");
    process.waitForFinished();
    //process;
    qDebug() <<process.readAllStandardOutput();
    output = process.readAllStandardOutput();

    // codeEnd=FindEndSymbol(output);
    int assembleTime = timer.elapsed()- time;
    time = timer.elapsed();

    //        qDebug() << "Code end: " << Util::numToHex(codeEnd) << codeEnd;
    int orgFileSize = QFile(filename+".ch8").size();

    //    qDebug() << "MOS " << Syntax::s.m_stripPrg << Syntax::s.m_ignoreSys;

    int size = QFile(filename+".ch8").size();



    m_buildSuccess = true;
    m_orgOutput = "";
    if (output.toLower().contains("error")) {
        m_orgOutput = output;
        text="<font color=\"#FF6040\">Fatal error during assembly!</font><br>";
        m_buildSuccess = false;
    }

    if (!output.toLower().contains("complete.")) {
        m_buildSuccess = false;
    }

    if (m_buildSuccess) {
        output = output.remove("Complete.");
        output = output + "Assemble time: <font color=\"#70FF40\">"+ (Util::MilisecondToString(assembleTime)) + "</font>. ";

        output+="Assembled file size: <b>" + QString::number(size) + "</b> bytes. ";

    }


    text+=output;


}

void SystemChip8::PostProcess(QString &text, QString filename, QString currentDir)
{
}


void SystemChip8::DefaultValues()
{
    m_startAddress = 0x512;
    m_programStartAddress = 0x512;
    m_ignoreSys = true;
    m_stripPrg = false;

}

void SystemChip8::applyEmulatorParameters(QStringList &params, QString debugFile, QString baseFile, CIniFile *pini)
{
    params << baseFile+".ch8";
}

void SystemChip8::PrepareInitialAssembler(Assembler *as) {

}

