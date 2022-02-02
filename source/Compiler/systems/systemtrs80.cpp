#include "systemtrs80.h"

SystemTRS80::SystemTRS80(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj): SystemZ80(settings, proj) {
    m_processor = Z80;
    m_system = TRS80;

    m_startAddress = 0x4000;
    m_programStartAddress = 0x4000;
    m_supportsExomizer = true;


 //   m_labels.append(SystemLabel(SystemLabel::ZEROPAGE,"System values",0,0x00FF));
 /* m_labels.append(SystemLabel(SystemLabel::STACK,"Stack",0xBF00,0xBFFF));
    m_labels.append(SystemLabel(SystemLabel::BASIC,"Basic",0x0200,0x3FFF));
    m_labels.append(SystemLabel(SystemLabel::FREE,"Free",0x4000,0xAFFF));
    m_labels.append(SystemLabel(SystemLabel::VIC,"Video memory",0xC000,0xFFFF));
    m_labels.append(SystemLabel(SystemLabel::SID,"Firmware",0xB000,0xBEFF));
*/
    m_systemColor = QColor(40,90,40);
    m_requireEmulatorWorkingDirectory = true;

}

void SystemTRS80::Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab)
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

    output+="<br>";

    time = timer.elapsed();


    text+=output;

}

void SystemTRS80::PostProcess(QString &text, QString file, QString currentDir)
{
    if (!m_buildSuccess)
        return;

    auto ba = Util::loadBinaryFile(file+".bin");

    QByteArray all;

    int left = ba.size();
    int pos = m_programStartAddress;
    int apos = 0;
    // Split into 256-byte blocks
    while (left!=0) {
        QByteArray h;
        h.append(01); // READ
        int size = left;
        if (size>253) size=253;
        h.append(size+2);
//    Util::appendInt16(h,ba.count());
        Util::appendInt16(h,pos);
        for (int i=0;i<size;i++)
            h.append(ba[i+apos]);

        all.append(h);
        pos+=size;
        left-=size;
        apos+=size;
    }

    // Add entry point
    all.append(02);
    all.append(02);
    Util::appendInt16(all,m_programStartAddress);





    Util::SaveByteArray(all,file+".cmd");

}

void SystemTRS80::applyEmulatorParameters(QStringList &params, QString debugFile, QString filename, CIniFile *pini) {
     params << filename+".cmd";
    //        qDebug() <<"CURRADDR" <<"0x"+QString::number(Syntax::s.m_currentSystem->m_programStartAddress,16);

    m_requireEmulatorWorkingDirectory = true;
}
