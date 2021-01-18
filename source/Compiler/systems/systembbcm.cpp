#include "systembbcm.h"
#include "source/LeLib/bbc/globaldata.h"
#include "source/LeLib/bbc/discimage.h"

SystemBBCM::SystemBBCM(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : SystemMOS6502(settings, proj)
{
    m_processor = MOS6502;
    m_system = BBCM;
    DefaultValues();

    m_labels.append(SystemLabel(SystemLabel::ZEROPAGE,"Zero pages",0,0x00FF));
    m_labels.append(SystemLabel(SystemLabel::STACK,"Stack",0x0100,0x01FF));

    m_labels.append(SystemLabel(SystemLabel::SCREEN,"Screen bank 0",0x4000,0x7FFF));

    m_systemColor = QColor(120,100,80);

}

void SystemBBCM::PostProcess(QString &text, QString filename, QString currentDir)
{
//    qDebug() << "POST processing file";
 //   GlobalData::Instance().Create();
 //   GlobalData::Instance().GetDiscImage().
    GlobalData::Create();
    QString diskFileName = filename + ".ssd";
    GlobalData::Instance().SetUseDiscImage( true );
    GlobalData::Instance().SetBootFile("CODE");
    DiscImage* di = new DiscImage(diskFileName.toStdString().c_str(),nullptr);
    QByteArray data = Util::loadBinaryFile(filename+".prg");
    unsigned char d[0x10000];
    for (int i=0;i<data.count();i++)
        d[i] = data[i];
    di->AddFile("CODE",
                d,
                0x1100,0x1100,data.count()
                );

    delete di;
    // Second pass
/*    di = nullptr;
    di = new DiscImage(diskFileName.toStdString().c_str(),diskFileName.toStdString().c_str());

*/
  //  delete di;
    GlobalData::Destroy();

}

void SystemBBCM::DefaultValues()
{
    m_startAddress = 0x1100;
    m_programStartAddress = 0x1100;
    m_ignoreSys = true;
    m_stripPrg = true;

}
