#include "systempokemonmini.h"


SystemPokemonMini::SystemPokemonMini(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : SystemZ80(settings,proj)
{
    m_processor = S1C88;
    m_system = POKEMONMINI;

    m_startAddress = 0x0000;
    m_programStartAddress = 0x0000;
    m_supportsExomizer = true;


    m_systemColor = QColor(70,40,80);
    m_requireEmulatorWorkingDirectory = true;
    m_allowedGlobalTypeFlags << "pure"<<"pure_variable" <<"pure_number" << "signed" <<"no_term" <<"wram" <<"hram" <<"sprram" << "bank";
    m_allowedProcedureTypeFlags << "pure"<<"pure_variable" <<"pure_number" << "signed" <<"no_term" <<"global" << "wram" <<"global";

}

void SystemPokemonMini::Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab)
{
    QString output,error;
    int time = timer.elapsed();

    output+="<br>";
    if (QFile::exists(filename+".min"))
        QFile::remove(filename+".min");
    auto pmas = m_settingsIni->getString("pmas");
    if (!QFile::exists(pmas)) {
        text  += "<br><font color=\"#FF6040\">Please set up a link to the PMAS assembler in the TRSE settings panel.</font>";
        m_buildSuccess = false;
        return;
    }
    QString pms = currentDir+ "cpu/pm.s";
    if (!QFile::exists(pms)) {
        QDir().mkdir(currentDir+"cpu");
        Util::CopyFile(":resources/code/pokemini/pm.s",pms);
    }

    GenericAssemble(pmas,QStringList() <<filename+".asm"<<filename+".min",error,output,currentDir);
    if (!output.toLower().contains("error"))
        output ="<br>ok.";
    else output="<br>error.";
//    PerformAssembling(filename,text,currentDir,symTab);

    if (!QFile::exists(filename+".min")) {
        text  += "<br><font color=\"#FFFF00\">Error during assembly : please check source assembly for errors.</font>";
        text+=output;
        m_buildSuccess = false;
        return;
    }

    if (m_buildSuccess) {

/*        auto h = Util::loadBinaryFile("/Users/leuat/code/TRSE/Publish/tutorials/POKEMONMINI/test/org.min");
//        h = h.remove(0x11B,h.size()-0x11B);
  //      qDebug() << h.size();
        auto b = Util::loadBinaryFile(filename+".bin");
        b.remove(0,2);
//        b = h +b;
        Util::SaveByteArray(b,filename+".min");*/
        text +="<br>Assembled file size: <b>" + QString::number(QFileInfo(filename+".min").size()) + "</b> bytes";
    }

    output+="<br>";

    time = timer.elapsed();


    text+=output;

}

void SystemPokemonMini::PostProcess(QString &text, QString file, QString currentDir)
{

}

void SystemPokemonMini::applyEmulatorParameters(QStringList &params, QString debugFile, QString filename, CIniFile *pini) {
    params << "pokemini";
    params << "-cart" <<filename+".min";
    params <<  "-resolution0" << "640x480@60" <<"-window";

}
