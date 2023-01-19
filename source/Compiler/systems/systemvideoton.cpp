#include "systemvideoton.h"

SystemVideoton::SystemVideoton(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj): SystemZ80(settings, proj)
{
    m_processor = Z80;
    m_system = AbstractSystem::VIDEOTON;

    m_startAddress = 0x1A01;
    m_programStartAddress = 0x1A01;
    m_supportsExomizer = false;


    //   m_labels.append(SystemLabel(SystemLabel::ZEROPAGE,"System values",0,0x00FF));
    /* m_labels.append(SystemLabel(SystemLabel::STACK,"Stack",0xBF00,0xBFFF));
    m_labels.append(SystemLabel(SystemLabel::BASIC,"Basic",0x0200,0x3FFF));
    m_labels.append(SystemLabel(SystemLabel::FREE,"Free",0x4000,0xAFFF));
    m_labels.append(SystemLabel(SystemLabel::VIC,"Video memory",0xC000,0xFFFF));
    m_labels.append(SystemLabel(SystemLabel::SID,"Firmware",0xB000,0xBEFF));
*/
    m_systemColor = QColor(50,10,60);
    m_requireEmulatorWorkingDirectory = true;

}

void SystemVideoton::Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab)
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

    //output+="<br>";

    // Create header
    QByteArray d = Util::loadBinaryFile(filename+".bin");
    int len = d.size();

    QByteArray h;
    h.resize(128);
    h.fill(0);
    int bsize = len>>7;
    h[0] = 0x11;
    h[2] = (bsize)&0xff;
    h[3] = (bsize>>8)&0xff;
    h[4] = len&0x7F;

    h.append(d);

    Util::SaveByteArray(h,filename+".bin");
    time = timer.elapsed();

    text+=output;

}


void SystemVideoton::applyEmulatorParameters(QStringList &params, QString debugFile, QString filename, CIniFile *pini) {
    //    $MAME tim011 -window -v -r 720x512 -switchres -flop1 $FLOPPY.img 1>/dev/null &

    params <<filename+".bin";
  //  params <<"tim011" <<"-window" <<"-v"<<"-r"<<"720x512"<<"-switchres" <<"-flop1"  <<filename+".img";

    m_requireEmulatorWorkingDirectory = true;
}
