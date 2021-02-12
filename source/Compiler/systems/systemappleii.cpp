#include "systemappleii.h"
#include <QProcess>

SystemAppleII::SystemAppleII(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : SystemMOS6502(settings, proj) {
    m_processor = MOS6502;
    m_system = APPLEII;
    DefaultValues();

    m_systemColor = QColor(80,110,80);

}

void SystemAppleII::DefaultValues()
{
    m_startAddress = getDefaultBasicAddress();
    m_programStartAddress = 0x0810;
    m_ignoreSys = true;
    m_stripPrg = true;
//    m_projectIni->setFloat("ignore_initial_jump",0);
}
void SystemAppleII::PostProcess(QString &text, QString file, QString currentDir)
{
    /*

0000000 00 05 16 00 00 02 00 00 00 00 00 00 00 00 00 00
0000010 00 00 00 00 00 00 00 00  00 02  00 00 00 01 00 00
0000020 00 3a  00 00 0b 18   00 00 00 0b  00 00 00 32  00 00
0000030 00 08   00 c3 00 06  00 00 08 03    a2 ff 9a 20 9b 12
0000040 20 c4 10 20 30 0c a2 1c a9 08 20 53 0c 2c 82 c0
0000050 20 47 0c a2 02 bd 8c 12 9d f2 03 ca 10 f7 a2 19
0000060 bd 72 12 95 80 ca 10 f8 a2 ff 9a 4c 22 12 2c 82

*/
    QByteArray d = Util::loadBinaryFile(file+".prg");
    QByteArray h;
    uchar  header[]={0x00, 0x05,0x16,0x00, // magic num
                     0,2,0,0,//version
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // filler
                     00, 02, // num files
                     00,00,00,01, // Start of block
                     00,00,00,0x3a, // Length of data ?

                    };
    int start = m_programStartAddress;
    int end = m_programStartAddress+d.size()-1;


    for (auto v : header)
        h.append(v);
//    Util::WriteInt32(h,d.size());
    Util::WriteInt32(h,d.size());
    Util::WriteInt32(h,0xB); // Prodos file next
    Util::WriteInt32(h,0x32); // Offset
    Util::WriteInt32(h,0x08); // Length?

    Util::WriteInt32(h, 0x00c30006); // No idea
    Util::WriteInt32(h, 0x803); // start address?

    h.append(d);

/*    QByteArray disk = Util::loadBinaryFile(":resources/bin/apple2/disk.do");
    for (int i=0;i<d.size();i++) {
        disk[0x7100 + i] = d[i];
    }
    disk[0xb8D] = (d.size()>>8)&0xff;
    disk[0xb8E] = (d.size())&0xff;

    Util::SaveByteArray(disk, file+".do");*/
    Util::SaveByteArray(h, file+".bin");
    Util::CopyFile(":resources/bin/apple2/ac.jar","ac.jar");
    Util::CopyFileBytes(":resources/bin/apple2/stripped.do",file+".do");
    QString tout;
    QStringList params = QStringList() << "-jar" << "ac.jar" <<"-as" <<file+".do" <<"HIWORLD" <<"BIN";

//    "-as" << file+".do"<<"HIWORLD" <<"BIN ";
    QString p = " " +file+".do HIWORLD BIN ";
//    qDebug() << params;
   /* StartProcess("java",
                 params, tout);;*/

    QProcess process;
    process.setStandardInputFile("f1");
 //   process.setReadChannel(SeparateChannels);
    QByteArray inp;
    //inp.append(p.toUtf8());
    inp.append(Util::loadBinaryFile(file+".bin"));
    Util::SaveByteArray(inp,"f1");

    process.start("java", params);


    process.waitForFinished();
 //   qDebug() << process.readAllStandardError();
   // qDebug() << process.readAllStandardOutput();
    QFile::remove("ac.jar");
    QFile::remove("f1");




}

