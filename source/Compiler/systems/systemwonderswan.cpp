#include "systemwonderswan.h"

QString SystemWonderswan::getEmulatorName() {
    return m_settingsIni->getString("wonderswan_emulator");
}

void SystemWonderswan::applyEmulatorParameters(QStringList &params, QString debugFile, QString filename, CIniFile *pini) {
    params << filename+".wsc";
    //    qDebug() << params;
}

void SystemWonderswan::PostProcess(QString &text, QString file, QString currentDir)
{
    SystemX86::PostProcess(text,file,currentDir);
    if (m_buildSuccess) {
        if (QFile::exists(file+".wsc"))
            QFile::remove(file+".wsc");
        QFile::rename(file+".com",file+".wsc");
    }
}


SystemWonderswan::SystemWonderswan(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : SystemX86(settings, proj) {
    m_processor = PX86;
    m_system = WONDERSWAN;
    m_startAddress = 0x0;

    m_cpu = "186";//m_projectIni->getString("cpu_x86_system");

}
