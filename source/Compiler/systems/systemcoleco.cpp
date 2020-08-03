#include "systemcoleco.h"

SystemColeco::SystemColeco(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj): SystemZ80(settings, proj) {
    m_processor = Z80;
    m_system = COLECO;

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
    m_startAddress = 0x8000;
    m_programStartAddress = 0x8000;

}

void SystemColeco::Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab)
{
    QString output;
    int time = timer.elapsed();
    int codeEnd = 0;

    output+="<br>";
    QString assembler = m_settingsIni->getString("pasmo");
    if (!QFile::exists(assembler)) {
        text  += "<br><font color=\"#FF6040\">Please set up a link to the PASMO assembler directory in the TRSE settings panel.</font>";
        return;
    }

    if (QFile::exists(filename+".bin"))
        QFile::remove(filename+".bin");

    QProcess process;
    QStringList params;
    StartProcess(assembler, QStringList() << filename+".asm" <<filename+".bin", output);

    if (!QFile::exists(filename+".bin")) {
        text  += "<br><font color=\"#FFFF00\">Error during assembly : please check source assembly for errors.</font>";
        text+=output;
        m_buildSuccess = false;
        return;
    }
    output+="<br>";

    QByteArray data = Util::loadBinaryFile(filename+".bin");
    data.remove(0,0x1000);
    Util::SaveByteArray(data,filename+".bin");


    time = timer.elapsed();

    //    qDebug() << "*********" << output;

    text+=output;

}

void SystemColeco::PostProcess(QString &text, QString file, QString currentDir)
{

}
