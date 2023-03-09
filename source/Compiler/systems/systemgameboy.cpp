#include "systemgameboy.h"


SystemGameboy::SystemGameboy(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : SystemZ80(settings, proj) {
    m_processor = GBZ80;
    m_system = GAMEBOY;

    m_labels.append(SystemLabel(SystemLabel::SCREEN,"Screen",0x8000,0x8FFF));
    m_labels.append(SystemLabel(SystemLabel::SCREEN,"Video ram",0x9000,0x9FFF));
    m_labels.append(SystemLabel(SystemLabel::ROM,"BANK 0 ROM",0x0,0x3FFF));
    m_labels.append(SystemLabel(SystemLabel::ROM,"BANK 1-128 ROM",0x4000,0x7FFF));
    m_labels.append(SystemLabel(SystemLabel::FREE,"Work RAM Bank 0 (sprites etc)",0xC000,0xCFFF));
    m_labels.append(SystemLabel(SystemLabel::FREE,"Work RAM Bank 1",0xD000,0xDFFF));
    m_labels.append(SystemLabel(SystemLabel::KERNAL,"Sprite attribute table",0xFE00, 0xFE9F));
    m_labels.append(SystemLabel(SystemLabel::KERNAL,"IO",0xFF00, 0xFF7f));
    m_labels.append(SystemLabel(SystemLabel::FREE,"HRAM",0xFF80, 0xFFFF));


    m_allowedGlobalTypeFlags << "pure"<<"pure_variable" <<"pure_number" << "signed" <<"no_term" <<"wram" <<"hram" <<"sprram" << "bank";
    m_allowedProcedureTypeFlags << "pure"<<"pure_variable" <<"pure_number" << "signed" <<"no_term" <<"global" << "wram" <<"global";
    m_renameVariables.clear();

}

void SystemGameboy::Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab)
{
    QString output;
    int time = timer.elapsed();
    int codeEnd = 0;


    QString dir = m_settingsIni->getString("gameboy_rgbasm_dir");
    if (!dir.endsWith(QDir::separator()))
        dir+=QDir::separator();
    QString assembler = dir+"rgbasm";
    QString link = dir+"rgblink";
    QString fix = dir+"rgbfix";
#ifdef _WIN32
    assembler+=".exe";
    link+=".exe";
    fix+=".exe";
#endif

//    output+="<br>";
    if (!QFile::exists(assembler) || !QFile::exists(link) || !QFile::exists(fix)) {
        text  += "<br><font color=\"#FF6040\">Please set up a link to the RGBASM assembler directory in the TRSE settings panel.</font>";
        m_buildSuccess = false;
        return;
    }
//    Util::CopyFile(":resources/bin/gbt-player/gbt_player.o",currentDir+"/gbt_player.o");
//    Util::CopyFile(":resources/bin/gbt-player/gbt_player_bank1.o",currentDir+"/gbt_player_bank1.o");
    //qDebug() << m_settingsIni->getString("assembler");
        QProcess process;
        QStringList params = QStringList() <<"-H"<<"-l";
       // -kick1hunks  -o example$1 -nosym source$1.asm
  //      params << "-kick1hunks";
    //    params << "-Fhunkexe";
        QFile::remove(filename+".o");
        StartProcess(assembler, QStringList() <<"-o" << filename + ".o"<<filename+".asm" << params, output);

        // Assemble the player:
        Util::CopyFile(":resources/code/gameboy/gbt_player.asm",currentDir+"/gbt_player.asm");
        Util::CopyFile(":resources/code/gameboy/gbt_player.inc",currentDir+"/gbt_player.inc");
        Util::CopyFile(":resources/code/gameboy/gbt_player_bank1.asm",currentDir+"/gbt_player_bank1.asm");
        Util::CopyFile(":resources/code/gameboy/hardware.inc",currentDir+"/hardware.inc");
        StartProcess(assembler, QStringList() <<"-o" << currentDir+"/gbt_player.o"<<currentDir+"/gbt_player.asm" <<"-i" <<currentDir+"/"<<params, output);
        StartProcess(assembler, QStringList() <<"-o" << currentDir+"/gbt_player_bank1.o"<<currentDir+"/gbt_player_bank1.asm"<<"-i" <<currentDir<<params, output);



        if (!QFile::exists(filename+".o")) {
            text  += "<br><font color=\"#FFFF00\">Error during assembly : please check source assembly for errors.</font>";
            text+=output;
            m_buildSuccess = false;
            return;
        }
//        qDebug() << "-d"<< "-o" << filename + ".gb" << filename+".o";
        QFile::remove(filename+".gb");

//        gbt_player.o gbt_player_bank1.o
        StartProcess(link, QStringList() <<"-d"<< "-o" << filename + ".gb" << filename+".o" <<currentDir+"/gbt_player.o" <<currentDir+"/gbt_player_bank1.o" << "-n" << filename+".sym", output);
        if (!QFile::exists(filename+".gb")) {
            text  += "<br><font color=\"#FFFF00\">Error during assembly : please check source assembly for errors.</font>";
            text+=output;
            m_buildSuccess = false;
            return;
        }

//        StartProcess(fix, QStringList() <<"-p" <<"0" <<"-r" <<"0" <<"-t" <<"TRSE GB" << "-v" << filename + ".gb", output);
        StartProcess(fix, QStringList() << "-m"<< "3"<<  "-p" <<"0" <<"-t" <<"TRSE GB" << "-v" << filename + ".gb", output);
        /* Cleanup */
        QStringList dels = QStringList() << "gbt_player.asm" <<"gbt_player.o" <<"gbt_player_bank1.asm" <<"gbt_player_bank1.o" <<"hardware.inc" << "gbt_player.inc";
        for (QString s : dels)
            if (QFile::exists(currentDir+s))
                QFile::remove(currentDir+s);

        output+="<br>";



/*        process.start(link, params);
        process.waitForFinished();
        //output+= process.readAllStandardOutput();
        output+= process.readAllStandardError();

        qDebug() <<process.readAllStandardError();*/
 /*       params.clear();
        params <<"-p" <<"0" <<"-r" <<"0" <<"-t" <<"TRSE GB" << "-v" << filename + ".gb";

        process.start(fix, params);
        process.waitForFinished();
        //output+= process.readAllStandardOutput();
        output+= process.readAllStandardError();
*/


    int assembleTime = timer.elapsed()- time;
    time = timer.elapsed();

//    qDebug() << "*********" << output;

    text+=output;

}

void SystemGameboy::PostProcess(QString &text, QString file, QString currentDir)
{

}
