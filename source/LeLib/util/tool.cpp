#include "tool.h"
#include <QProcess>

Tool::Tool()
{

}

bool Tool::AKGCompiler(QString filename, int Address, SymbolTable *symTab)
{
    QString path = QFileInfo(filename).path() + QDir::separator();

    if ((!QFile::exists(filename+".asm")) && (!QFile::exists(filename+".inc")))
        return false;

    if ((QFile::exists(filename+".asm"))) {
        Util::CopyFile(filename+".asm",filename+".inc");
        Util::CopyFile(filename+"_playerconfig.asm",filename+"_playerconfig.inc");

    }

//    qDebug() << path << QFile::exists(":resources/bin/rasm.exe");
    QString player = path+"playerakg.asm";
    QString snd = path+"playerakg_soundeffects.asm";
    QString asmFile = path+"temp.asm";

#ifdef __linux__
    QString rasm = path+"rasm";
    Util::CopyFile(":resources/bin/rasm",rasm);
    QFile::setPermissions(rasm,QFile::ExeUser);
#endif
#ifdef _WIN32
    QString rasm = path+"rasm.exe";
    Util::CopyFile(":resources/bin/rasm.exe",rasm);
#endif



    Util::CopyFile(":resources/code/amstrad/playerakg.asm",player);
    Util::CopyFile(":resources/code/amstrad/playerakg_soundeffects.asm",snd);

    QFile file(asmFile);
    file.open(QIODevice::WriteOnly);
    QTextStream qout(&file);
    qout<< "org " + Util::numToHex(Address)<< "\n";
    qout<<"include \""+filename+".inc\" \n";
    qout<<"include \""+filename + "_playerconfig.inc\" \n";


    qout<<"PLY_AKG_HARDWARE_CPC = 1\n";
    qout<<"include \""+player+"\" \n";

    file.close();

    QProcess process;
    process.start(rasm,QStringList() << asmFile << "-o" <<filename << "-s"<<"-sl"<<"-sq");
    process.waitForFinished();

    // Ok. We have the binary file - now we need to read the symbol file.

    QStringList symFile = Util::loadTextFile(filename+".sym").split("\n");
    for (QString s: symFile) {
        QStringList d = s.trimmed().simplified().split(" ");
        if (d.count()>1)
            d[1] = d[1].remove("#");
        bool ok;
        if (d[0]=="PLY_AKG_INIT") {
            symTab->m_constants["INIT_MUSIC"]->m_value->m_fVal = d[1].toInt(&ok,16);
        }
        if (d[0]=="PLY_AKG_PLAY") {
            symTab->m_constants["PLAY_MUSIC"]->m_value->m_fVal = d[1].toInt(&ok,16);
        }
        if (d[0]=="PLY_AKG_STOP") {
            symTab->m_constants["STOP_MUSIC"]->m_value->m_fVal = d[1].toInt(&ok,16);
        }

    }
    //qDebug() << Util::numToHex(symTab->m_constants["PLAY_MUSIC"]->m_value->m_fVal);

  //  qDebug() << process.readAllStandardError() << process.readAllStandardOutput();
    // Compile time

    QFile::remove(rasm);
    QFile::remove(filename+".sym");
    QFile::remove(player);
    QFile::remove(snd);
    QFile::remove(asmFile);
    return true;
}
