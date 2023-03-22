#include "systempcw.h"
#include <QThread>

SystemPCW::SystemPCW(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj): SystemTIM(settings, proj)
{
    m_processor = Z80;
    m_system = PCW;

    m_startAddress = 0x0100;
    m_programStartAddress = 0x0100;
    m_supportsExomizer = false;

    m_systemColor = QColor(50,90,20);
    m_requireEmulatorWorkingDirectory = true;

}


void SystemPCW::ExtraEmulatorCommands()
{
/*    QString t = "
    "sleep 1"
    "ID=$(xdotool search --name TIM-011)"
    "sendKey space space Scroll_Lock F11 F10"
    "sleep 2"
    "sendKey F10 r Return F10"
    "sleep 0.2"
    "sendKey F10 F11 Scroll_Lock"
            ""


*/
    //sendkeys -a "tim011" -c "<c:return> hello"
#ifdef __APPLE__
//    QThread::sleep(3);
  //  SendKeyCommand("<c:space:space>");

 #endif
}

void SystemPCW::applyEmulatorParameters(QStringList &params, QString debugFile, QString filename, CIniFile *pini) {
    //    $MAME tim011 -window -v -r 720x512 -switchres -flop1 $FLOPPY.img 1>/dev/null &

    params <<"pcw" <<"-window" <<"-v"<<"-r"<<"720x512"<<"-switchres" <<"-flop1"  <<filename+".img";
    params << "-skip_gameinfo" << "-autoboot_delay"<< "3" <<"-autoboot_command"<< "r\\n";

//    params <<"tim011" <<"-window" <<"-v"<<"-r"<<"720x512"<<"-switchres" <<"-flop1"  <<filename+".img";

    m_requireEmulatorWorkingDirectory = true;
}
