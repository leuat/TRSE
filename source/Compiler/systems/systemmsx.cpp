#include "systemmsx.h"
#include "source/LeLib/util/lz4/lz4.h"
#include "source/Compiler/errorhandler.h"

SystemMSX::SystemMSX(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj): SystemZ80(settings, proj) {
    m_processor = Z80;
    m_system = MSX;

/*    m_labels.append(SystemLabel(SystemLabel::SCREEN,"Screen",0x8000,0x8FFF));
    m_labels.append(SystemLabel(SystemLabel::SCREEN,"Video ram",0x9000,0x9FFF));
    m_labels.append(SystemLabel(SystemLabel::ROM,"BANK 0 ROM",0x0,0x3FFF));
    m_labels.append(SystemLabel(SystemLabel::ROM,"BANK 1-128 ROM",0x4000,0x7FFF));
    m_labels.append(SystemLabel(SystemLabel::FREE,"Work RAM Bank 0 (sprites etc)",0xC000,0xCFFF));
    m_labels.append(SystemLabel(SystemLabel::FREE,"Work RAM Bank 1",0xD000,0xDFFF));
    m_labels.append(SystemLabel(SystemLabel::KERNAL,"Sprite attribute table",0xFE00, 0xFE9F));
    m_labels.append(SystemLabel(SystemLabel::KERNAL,"IO",0xFF00, 0xFF7f));
    m_labels.append(SystemLabel(SystemLabel::FREE,"HRAM",0xFF80, 0xFFFF));
*/
    m_startAddress = 0x4000;
    m_programStartAddress = 0x4000;
    m_supportsExomizer = false;
    m_stripPrg = true;


    m_systemColor = QColor(70,40,100);

    m_allowedGlobalTypeFlags << "ram";
    m_allowedProcedureTypeFlags << "ram";

}

void SystemMSX::Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab)
{
    QString output;
    int time = timer.elapsed();
    int codeEnd = 0;

    output+="<br>";


//    AssembleOrgasm();
/*
    QString assembler = m_settingsIni->getString("pasmo");
    if (!QFile::exists(assembler)) {
        text  += "<br><font color=\"#FF6040\">Please set up a link to the PASMO assembler directory in the TRSE settings panel.</font>";
        m_buildSuccess = false;
        return;
    }
    if (QFile::exists(filename+".bin"))
        QFile::remove(filename+".bin");

    QProcess process;
    StartProcess(assembler, QStringList() << filename+".asm" <<filename+".bin", output);

    if (!QFile::exists(filename+".bin")) {
        text  += "<br><font color=\"#FFFF00\">Error during assembly : please check source assembly for errors.</font>";
        text+=output;
        m_buildSuccess = false;
        return;
    }
*/
    if (QFile::exists(filename+".bin"))
        QFile::remove(filename+".bin");

    AssembleZOrgasm(output,text,filename,currentDir,symTab);


    if (m_buildSuccess) {
        text +="<br>Assembled file size: <b>" + QString::number(QFileInfo(filename+".bin").size()) + "</b> bytes";
    }

    output+="<br>";


    time = timer.elapsed();
    if (!output.toLower().contains("complete.")) {
        m_buildSuccess = false;
    }


    text+=output;

}

void SystemMSX::PostProcess(QString &text, QString file, QString currentDir)
{
    if (!QFile::exists(file+".bin"))
        return;
    QByteArray d = Util::loadBinaryFile(file+".bin");
    QByteArray h;
    int start = m_programStartAddress;
    int end = m_programStartAddress+d.size()-1;

/*    // Add MSX header
    h.append(0xFE); // Magic num
    h.append((start>>8)&0xFF);
    h.append((start)&0xFF);
    h.append((end>>8)&0xFF);
    h.append((end)&0xFF);
    h.append((start>>8)&0xFF);
    h.append((start)&0xFF);
    h.append(d);
    */
    h.append(d);
    while (h.length()<0x4000)
        h.append(0xFF);

    Util::SaveByteArray(h, file+".rom");
}
