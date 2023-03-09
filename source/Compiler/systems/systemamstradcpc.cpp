#include "systemamstradcpc.h"
#include "source/LeLib/util/lz4/lz4.h"
#include "source/Compiler/errorhandler.h"

SystemAmstradCPC::SystemAmstradCPC(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj): SystemZ80(settings, proj) {
    m_processor = Z80;
    m_system = AMSTRADCPC;

    m_startAddress = 0x4000;
    m_programStartAddress = 0x4000;
    m_supportsExomizer = true;


    m_labels.append(SystemLabel(SystemLabel::ZEROPAGE,"System values",0,0x00FF));
    m_labels.append(SystemLabel(SystemLabel::STACK,"Stack",0xBF00,0xBFFF));
    m_labels.append(SystemLabel(SystemLabel::BASIC,"Basic",0x0200,0x3FFF));
    m_labels.append(SystemLabel(SystemLabel::FREE,"Free",0x4000,0xAFFF));
    m_labels.append(SystemLabel(SystemLabel::VIC,"Video memory",0xC000,0xFFFF));
    m_labels.append(SystemLabel(SystemLabel::SID,"Firmware",0xB000,0xBEFF));

    m_systemColor = QColor(40,90,40);
    m_requireEmulatorWorkingDirectory = true;
    m_hasVariableColorPalette = true;
}

void SystemAmstradCPC::Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab)
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



    if (m_projectIni->getdouble("exomizer_toggle")==1)
        PackProgram(filename,0x4000, 0xB780, text, currentDir, symTab,":resources/code/amstrad/unpack.asm");

    if (m_buildSuccess) {
        text +="<br>Assembled file size: <b>" + QString::number(QFileInfo(filename+".bin").size()) + "</b> bytes";
    }

    //output+="<br>";

    time = timer.elapsed();


    text+=output;

}

void SystemAmstradCPC::PostProcess(QString &text, QString file, QString currentDir)
{

}
