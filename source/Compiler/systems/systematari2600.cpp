#include "systematari2600.h"


SystemAtari2600::SystemAtari2600(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj):SystemMOS6502(settings, proj)
{
    m_processor = MOS6502;
    m_system = ATARI2600;
    DefaultValues();

}

void SystemAtari2600::DefaultValues()
{
    m_startAddress = 0xF000;
    m_programStartAddress = 0xF000;
    m_ignoreSys = true;
    m_stripPrg = true;

}

void SystemAtari2600::applyEmulatorParameters(QStringList &params, QString debugFile, QString filename, CIniFile *pini) {
    params<< filename+".bin";
    //        qDebug() << filename;

}
void SystemAtari2600::PostProcess(QString &text, QString filename, QString currentDir)
{
    QByteArray ba = Util::loadBinaryFile(filename+".prg");
    for (int i=ba.size();i<4096-4;i++)
        ba.append((uchar)0);

    Util::WriteInt16LH(ba,m_startAddress);
    Util::WriteInt16LH(ba,m_startAddress);

    Util::SaveByteArray(ba,filename+".bin");
}
