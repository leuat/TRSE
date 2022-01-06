#include "systemjdh8.h"
#include <QDir>

SystemJDH8::SystemJDH8(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : AbstractSystem(settings, proj) {
    m_processor = PJDH8;
    m_system = JDH8;
    DefaultValues();

    m_labels.append(SystemLabel(SystemLabel::ROM,"Os",0,0x1000));
    m_labels.append(SystemLabel(SystemLabel::FREE,"RAM",0xC000,0xFFF0));
    m_systemColor = QColor(110,80,50);
    m_allowedGlobalTypeFlags << "compressed" <<"pure"<<"pure_variable" <<"pure_number" << "signed" <<"no_term";
    m_allowedProcedureTypeFlags << "pure"<<"pure_variable" <<"pure_number" << "signed" <<"no_term" <<"global";
    m_exomizerName ="Compress executable";
    m_renameVariables << "a" <<"b"<<"c"<<"d"<<"h"<<"l"<<"f"<<"z";

}

void SystemJDH8::DefaultValues() {

}

void SystemJDH8::Assemble(QString &text, QString file, QString currentDir, QSharedPointer<SymbolTable> symTab)
{
    QString dir = m_settingsIni->getString("jdh8_directory");
    if (!QDir(dir).exists() || dir=="") {
        text+="<br><font color=\"#FF0000\">Error: JDH-8 project path not set up in the project settings</font>";
        m_buildSuccess = false;
        return;
    }

    QString assembler = dir + "/bin/asm";
    QString f = file;
    QStringList params = QStringList() <<f+".asm"<<"-o"<<f+".bin";
    text+="<br>";
    GenericAssemble(assembler,params,"Error?",text,currentDir);
//    qDebug() << text;
    if (text.toLower().contains("error"))
        m_buildSuccess = false;





}

void SystemJDH8::PostProcess(QString &text, QString file, QString currentDir)
{

}

void SystemJDH8::applyEmulatorParameters(QStringList &params, QString debugFile, QString filename, CIniFile *pini)
{
    QString dir = m_settingsIni->getString("jdh8_directory");
    params<<"-m"<<"screen";
    params<<"-m"<<"kb";

    // Load OS
    params<<"--load"<< dir+"/os.bin" << "0x0000";
    params<<"--load"<< filename+".bin" << "0xc000";
    params<<"-r"<< "10000000";

    qDebug() << params;
//    exit(1);

  //  m_requireEmulatorWorkingDirectory = true;

}
