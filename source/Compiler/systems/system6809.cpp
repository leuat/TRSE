#include "system6809.h"

#include "source/LeLib/util/util.h"
#include "source/LeLib/util/cinifile.h"

//#include "source/LeLib/util/lz4/lz4.h"

System6809::System6809(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : AbstractSystem(settings, proj) {
    m_allowedGlobalTypeFlags << "compressed" <<"pure"<<"pure_variable" <<"pure_number" << "signed" <<"no_term";
    m_allowedProcedureTypeFlags << "pure"<<"pure_variable" <<"pure_number" << "signed" <<"no_term" <<"global";
    m_exomizerName ="Compress executable";
    m_renameVariables << "a" <<"b"<<"x"<<"y";

    //        m_registers <<"_a" <<"_b"<<"_c"<<"_d"<<"_af"<<"_bc"<<"_de"<<"_hl"<<"_ix"<<"_iy"<<"_h"<<"_l";
}

void System6809::Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab)
{
    QString output;
    int time = timer.elapsed();

    output+="<br>";

    PerformAssembling(filename,text,currentDir,symTab);

    if (!QFile::exists(filename+".bin")) {
        text  += "<br><font color=\"#FFFF00\">Error during assembly : please check source assembly for errors.</font>";
        text+=output;
        m_buildSuccess = false;
        return;
    }




    if (m_buildSuccess) {
        text +="<br>Assembled file size: <b>" + QString::number(QFileInfo(filename+".bin").size()) + "</b> bytes";
    }

    //    output+="<br>";

    time = timer.elapsed();


    text+=output;


}

void System6809::PostProcess(QString &text, QString file, QString currentDir)
{
}


void System6809::PerformAssembling(QString filename, QString &text,QString currentDir, QSharedPointer<SymbolTable> symTab)
{
    bool useOrgasm = false;
    //    if (m_settingsIni->contains("assembler_TRS80CoCo"))
    //          useOrgasm = m_settingsIni->getString("assembler_TRS80CoCo")!="Pasmo";


    QString assembler = m_settingsIni->getString("lwasm");
    bool useMorgasm = (m_settingsIni->getString("assembler_6809")=="orgasm");
    if (QFile::exists(filename+".bin"))
        QFile::remove(filename+".bin");


    if (!useMorgasm && !QFile::exists(assembler)) {
        text  += "<br><font color=\"#FF6040\">Please set up a link to the LWASM assembler in the TRSE settings panel, or use Morgasm (which isn't done yet.. so use lwasm).</font>";
        m_buildSuccess = false;
        return;

    }

    QString output = "";
    QString format = "-r";
    if (m_system==TRS80COCO || m_system==THOMSON )
        format ="-decb";
//    qDebug() << format;
    //    StartProcess(assembler, QStringList() << "-9bl" <<"-p" <<"cd"<<filename+".asm" <<"-o"+filename+".bin", text);
    if (useMorgasm) {
        AssembleZOrgasm(output,text,filename,currentDir, symTab,1);
    }
    else
        StartProcess(assembler, QStringList() << format<<"--6809"  <<filename+".asm" <<"-o"+filename+".bin" << "--symbol-dump="+filename+".sym", text);


}

QString System6809::CompressFile(QString fileName)
{
    return CompressZX0(fileName);
}
