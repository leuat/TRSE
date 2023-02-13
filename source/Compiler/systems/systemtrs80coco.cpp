#include "source/LeLib/util/util.h"
#include "source/LeLib/util/cinifile.h"
#include "systemtrs80coco.h"


SystemTRS80CoCo::SystemTRS80CoCo(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : System6809(settings,proj)
{
    m_processor = M6809;
    m_system = TRS80COCO;

    m_startAddress = 0x2000;
    m_programStartAddress = 0x2000;
    m_supportsExomizer = false;


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


void SystemTRS80CoCo::PostProcess(QString &text, QString file, QString currentDir)
{
    if (!m_buildSuccess)
        return;

    QString tool = getEmulatorName();
    QString imgtool = QFileInfo(tool).absoluteDir().absolutePath()+"/imgtool";
    qDebug() << imgtool;
#ifdef _WIN32
    imgtool+=".exe";
#endif
    if (!QFile::exists(imgtool)) {
        text+="Could not find 'imgtool' in mame directory. please verify!";
        m_buildSuccess = false;

    }
    QString output;


/*    auto ba = Util::loadBinaryFile(file+".bin");

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
  */
    StartProcess(imgtool,QStringList() <<"create"<<"coco_jvc_rsdos"<<file+".dsk", output);
 //   qDebug() << output;
    StartProcess(imgtool,QStringList() <<"put"<<"coco_jvc_rsdos"<<file+".dsk"<<file+".bin"<<"t.bin", output);
   // qDebug() << output;

}

void SystemTRS80CoCo::applyEmulatorParameters(QStringList &params, QString debugFile, QString filename, CIniFile *pini) {
    params << "coco3";
    params << "-flop1" <<filename+".dsk";
    params <<  "-resolution0" << "640x480@60" <<"-window";
    params <<"-nothrottle";
    params << "-skip_gameinfo" << "-autoboot_delay"<< "1" <<"-autoboot_command"<< "loadm \"T\":exec\\n";
    //        qDebug() <<"CURRADDR" <<"0x"+QString::number(Syntax::s.m_currentSystem->m_programStartAddress,16);

    m_requireEmulatorWorkingDirectory = true;

}
