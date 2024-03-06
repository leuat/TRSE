#include "systemprimo.h"

SystemPrimo::SystemPrimo(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj): SystemZ80(settings, proj)
{
    m_processor = Z80;
    m_system = AbstractSystem::PRIMO;

    m_startAddress = 0x4400;
    m_programStartAddress = 0x4400;
    m_supportsExomizer = false;



    //   m_labels.append(SystemLabel(SystemLabel::ZEROPAGE,"System values",0,0x00FF));
    /* m_labels.append(SystemLabel(SystemLabel::STACK,"Stack",0xBF00,0xBFFF));
    m_labels.append(SystemLabel(SystemLabel::BASIC,"Basic",0x0200,0x3FFF));
    m_labels.append(SystemLabel(SystemLabel::FREE,"Free",0x4000,0xAFFF));
    m_labels.append(SystemLabel(SystemLabel::VIC,"Video memory",0xC000,0xFFFF));
    m_labels.append(SystemLabel(SystemLabel::SID,"Firmware",0xB000,0xBEFF));
*/
    m_systemColor = QColor(20,60,30);
    m_requireEmulatorWorkingDirectory = true;

}

void SystemPrimo::Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab)
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


    QByteArray b;

    b.resize(5);
    b.fill(0);
    b[0] = 0xD9;
    b[1] = m_programStartAddress&0xff;
    b[2] = (m_programStartAddress>>8)&0xff;
    b[3] = (d.size())&0xFF;
    b[4] = (d.size()>>8)&0xFF;

//    bas[19] = 0xf5;

    b.append(d);


    b.append((uchar)0xc3);
    b.append((uchar)m_programStartAddress&0xff);
    b.append((uchar)(m_programStartAddress>>8)&0xff);

    Util::SaveByteArray(b,filename+".pri");
//    qDebug() << "HERE";
  //  Util::CopyFile("/Users/leuat/Downloads/ufo.pri", filename+".pri");
    QFile::remove(filename+".bin");
    time = timer.elapsed();

    text+=output;

}


void SystemPrimo::applyEmulatorParameters(QStringList &params, QString debugFile, QString filename, CIniFile *pini) {
#ifndef __APPLE__

    params <<filename+".pri";
#else
    QString name = qgetenv("USER");
    if (name.isEmpty())
        name = qgetenv("USERNAME");
    qDebug() << name;
    Util::CopyFile(filename+".pri","/Users/"+name+"/.wine/drive_c/trse.pri");
    params << "c:\\trse.pri";


#endif
  //  params <<"tim011" <<"-window" <<"-v"<<"-r"<<"720x512"<<"-switchres" <<"-flop1"  <<filename+".img";

    m_requireEmulatorWorkingDirectory = true;
}
