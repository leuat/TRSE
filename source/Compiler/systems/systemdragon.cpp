#include "source/LeLib/util/util.h"
#include "source/LeLib/util/cinifile.h"
#include "systemdragon.h"


SystemDragon::SystemDragon(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : System6809(settings,proj)
{
    m_processor = M6809;
    m_system = DRAGON;

    m_startAddress = 0xC000;
    m_programStartAddress = 0xC000;
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


void SystemDragon::PostProcess(QString &text, QString file, QString currentDir)
{
    if (!m_buildSuccess)
        return;


    auto ba = Util::loadBinaryFile(file+".bin");

    QByteArray all;

    all.append(0x55);
    all.append(02);
    Util::appendInt16(all,m_programStartAddress);
    Util::appendInt16(all,ba.size());
    Util::appendInt16(all,m_programStartAddress);
    all.append(0xaa);

    all.append(ba);

    Util::SaveByteArray(all,file+".bin");
}


void SystemDragon::applyEmulatorParameters(QStringList &params, QString debugFile, QString filename, CIniFile *pini) {
/*    params << "dragon64";
    params << "-flop1" <<filename+".vdk";
  //  params << "-rompath" << "/Applications/xroar-1.4.2-macosx/roms"
    params <<  "-resolution0" << "640x480@60" <<"-window";
    params <<"-nothrottle";
*/
//    params <<"-load-fd0" <<filename+".vdk";
    params << "-rompath" <<"/Applications/xroar-1.4.2-macosx/Dragon/roms";
    params << "-machine" << "Dragon64";
    params << "-run" <<filename+".bin";

//    params << "-skip_gameinfo" << "-autoboot_delay"<< "1" <<"-autoboot_command"<< "loadm \"T\":exec\\n";
    //        qDebug() <<"CURRADDR" <<"0x"+QString::number(Syntax::s.m_currentSystem->m_programStartAddress,16);

    m_requireEmulatorWorkingDirectory = true;

}
