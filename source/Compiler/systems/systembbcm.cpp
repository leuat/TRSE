#include "systembbcm.h"
#include "source/LeLib/bbc/asmexception.h"
SystemBBCM::SystemBBCM(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : SystemMOS6502(settings, proj)
{
    m_processor = MOS6502;
    m_system = BBCM;
    DefaultValues();

    m_labels.append(SystemLabel(SystemLabel::ZEROPAGE,"Zero pages",0,0x00FF));
    m_labels.append(SystemLabel(SystemLabel::STACK,"Stack",0x0100,0x01FF));

    m_labels.append(SystemLabel(SystemLabel::SCREEN,"Screen Mode 5",0x5800,0x7FFF));

    m_labels.append(SystemLabel(SystemLabel::KERNAL,"System",0x200,0x1100));

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
    AddFileToDisk(di, filename+".prg", "CODE", m_programStartAddress);
    if (m_projectIni->getString("d64_paw_file")!="none") {
        BuildDiskFiles(di,currentDir,"d64_paw_file",text);
    }

    delete di;
    GlobalData::Destroy();
}

bool SystemBBCM::BuildDiskFiles(DiscImage* di, QString currentDir, QString iniData, QString& text)
{

    QString pawFile = m_projectIni->getString(iniData);
    CIniFile paw;
    paw.Load(currentDir + QDir::separator()+pawFile);
    QStringList data = paw.getStringList("data");
    int count = data.count()/3;

    for (int i=0;i<count;i++) {
        QString orgFileName = data[3*i+1];

        QString name = data[3*i];

        int address = Util::NumberFromStringHex( data[3*i+2]);
        QString fn = currentDir+QDir::separator()+orgFileName;
        if (!QFile::exists(fn)) {
            m_buildSuccess = false;
            text+="<br>Error: Could not find file to include '"+fn+"' when processing paw file "+pawFile+"<br>";
            return false;
        }
        try {
            AddFileToDisk(di,fn,name,address);
        } catch (AsmException_FileError e) {
            qDebug() << "ERROR when adding file '"+name+"' to disk : " <<e.Message();
        }
    }

}


void SystemBBCM::DefaultValues()
{
    m_startAddress = 0x1100;
    m_programStartAddress = 0x1100;
    m_ignoreSys = true;
    m_stripPrg = true;
    m_memorySize = 65536/2;

}

void SystemBBCM::AddFileToDisk(DiscImage *di, QString filename, QString name, int address)
{
    QByteArray data = Util::loadBinaryFile(filename);
    unsigned char d[0x10000];
    for (int i=0;i<data.count();i++)
        d[i] = data[i];

    di->AddFile(name.toStdString().c_str(),
                d,
                address,address,data.count()
                );


}
