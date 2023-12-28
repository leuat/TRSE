#include "source/LeLib/util/util.h"
#include "source/LeLib/util/cinifile.h"
#include "systemvectrex.h"
#include <QMessageBox>

SystemVectrex::SystemVectrex(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : System6809(settings,proj)
{
    m_processor = M6809;
    m_system = VECTREX;

    m_startAddress = 0x0000;
    m_programStartAddress = 0x0000;
    m_supportsExomizer = false;


    m_ramAddress = 0xC800;
    m_usesRom = true;
    m_allowedGlobalTypeFlags <<"ram";
    m_allowedProcedureTypeFlags<<"ram";

 //   m_labels.append(SystemLabel(SystemLabel::ZEROPAGE,"System values",0,0x00FF));
 /* m_labels.append(SystemLabel(SystemLabel::STACK,"Stack",0xBF00,0xBFFF));
    m_labels.append(SystemLabel(SystemLabel::BASIC,"Basic",0x0200,0x3FFF));
    m_labels.append(SystemLabel(SystemLabel::FREE,"Free",0x4000,0xAFFF));
    m_labels.append(SystemLabel(SystemLabel::VIC,"Video memory",0xC000,0xFFFF));
    m_labels.append(SystemLabel(SystemLabel::SID,"Firmware",0xB000,0xBEFF));
*/
    m_systemColor = QColor(40,40,40);
    m_requireEmulatorWorkingDirectory = true;

}

void SystemVectrex::PostProcess(QString &text, QString file, QString currentDir)
{
    QByteArray b = Util::loadBinaryFile(file+".bin");
//    b.remove(0,5);
    Util::SaveByteArray(b,file+".vec");
    if (!m_buildSuccess)
        return;

}

void SystemVectrex::applyEmulatorParameters(QStringList &params, QString debugFile, QString filename, CIniFile *pini) {
#ifdef __APPLE__
    QString rArch_core = QDir::homePath() +"/Library/Application\ Support/RetroArch/cores/vecx_libretro.dylib";
    if (!QFile::exists(rArch_core)) {
        QMessageBox msgBox;
        msgBox.setText("You need to install the GCE Vectrex core in RetroArch before being able to run this program");
        msgBox.exec();
        params.clear();
        return;

    }
    params << "-L"<<rArch_core;
#endif
    params << filename+".vec";
    //        qDebug() <<"CURRADDR" <<"0x"+QString::number(Syntax::s.m_currentSystem->m_programStartAddress,16);
//    params <<"-L"<<
    m_requireEmulatorWorkingDirectory = false;

}
