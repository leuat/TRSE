#include "abstractsystem.h"
#include <QProcess>
#include "source/LeLib/util/lz4/lz4hc.h"
#include "source/LeLib/util/util.h"
#include "source/OrgAsm/orgasm.h"
#include "source/OrgAsm/zorgasm.h"
#include "source/OrgAsm/morgasm.h"
#include "source/OrgAsm/orgasm68k.h"
#include <QMessageBox>
#include "source/LeLib/data.h"

extern "C" {
    #include "source/LeLib/util/zx0/zx0.h"
}
QHash<SystemLabel::Type, QColor> AbstractSystem::m_labelColors;
QHash<QString, QString> AbstractSystem::m_systemParams;

AbstractSystem::AbstractSystem(AbstractSystem *a) {
}


QStringList AbstractSystem::ApplyDefaultMameParams()
{
    QStringList params;
    params <<  "-resolution0" << "640x480@60" <<"-window";
    //    params <<"-nothrottle";
    params << "-skip_gameinfo";
    return params;
}

QString AbstractSystem::CompressLZ4(QString fileName, QString outFileName) {
    QString old = fileName;
    QByteArray in = Util::loadBinaryFile(fileName);
    fileName = outFileName;

    QByteArray out;
    out.resize(65536);
    //LZ4LIB_API int LZ4_compress_default(const char* src, char* dst, int srcSize, int dstCapacity);
    //    int size = LZ4_compress_default(in.data(),out.data(), in.length(),65535);
    int size = LZ4_compress_HC(in.data(),out.data(), in.length(),65535,12);
    out.resize(size);
    QByteArray header;
    header.append(0x04); // version
    header.append(0x22);
    header.append(0x4d);
    header.append(0x18); // magic number
    //    header.append((0x64 | in.length()) & 0xFF); // lo size
    header.append((0x64)); // lo size
    //    header.append((in.length()>>8)&0xFF);
    header.append(0x40);

    //        header<<0x04<<0x22<<0x4d<<0x18<<0x64<<0x40<<0xa7<<0x4d<<0x15<<0x00<<0x00;
    header.append(0xa7); // ?? some checksum I guess
    header.append((out.length()) & 0xFF); // lo size
    header.append((out.length()>>8)&0xFF);
    header.append((char)0x00); // ??
    header.append((char)0x00); // ??

    header.append(out);
    Util::SaveByteArray(header,fileName);
    //      if (Util::fileChanged(filename) || !QFile(old).exists())
    //        fc.Encode(old,fileName);
    return fileName;




}

QString AbstractSystem::CompressZX0(QString fileName, QString outFileName)
{
    QStringList params;
    if (QFile::exists(outFileName))
        QFile::remove(outFileName);
    params<<"zx0"<<fileName<<outFileName;
    zx0_compress(3,Util::StringListToChar(params));
    return outFileName;
}


void AbstractSystem::StartProcess(QString file, QStringList params, QString& output, bool standardOutput, QString currentDir) {
    // qDebug() << params;
    QProcess process;
    if (currentDir != "")
        process.setWorkingDirectory(currentDir);
    process.start(file, params);
    process.waitForFinished();
   //        qDebug() << process.readAllStandardOutput();;
   //       qDebug() << process.readAllStandardError();
    //        output+= process.readAllStandardOutput();
    if (standardOutput)
        output+= process.readAllStandardOutput();
    output+= process.readAllStandardError();

}

void AbstractSystem::InitLabelColors() {
    m_labelColors[SystemLabel::IO] = QColor(255,128,50);
    m_labelColors[SystemLabel::SID] = QColor(255,255,128);
    m_labelColors[SystemLabel::VIC] = QColor(64,128,255);
    m_labelColors[SystemLabel::FREE] = QColor(128,192,128);
    m_labelColors[SystemLabel::ROM] = QColor(64,192,200);
    m_labelColors[SystemLabel::BASIC] = QColor(64,64,255);
    m_labelColors[SystemLabel::STACK] = QColor(255,128,64);
    m_labelColors[SystemLabel::COLOUR] = QColor(255,64,255);
    m_labelColors[SystemLabel::KERNAL] = QColor(64,64,64);
    m_labelColors[SystemLabel::SCREEN] = QColor(64,255,255);
    m_labelColors[SystemLabel::ZEROPAGE] = QColor(64,255,128);


}

QString AbstractSystem::StringFromProcessor(Processor s) {
    if (s == MOS6502) return "MOS6502";
    if (s == M68000) return "M68000";
    if (s == PX86) return "PX86";
    if (s == GBZ80) return "GBZ80";
    if (s == Z80) return "Z80";
    if (s == Z180) return "Z180";
    if (s == ARM) return "ARM";
    if (s == WDC65C816) return "WDC65C816";
    if (s == WDC65C02) return "WDC65C02";
    if (s == PJDH8) return "JDH8";
    if (s == S1C88) return "S1C88";
    if (s == M6809) return "M6809";
    if (s == PCHIP8) return "PCHIP8";
    if (s == PDP11) return "PPDP11";
    qDebug() << "SYSTEM CPU NOT FOUND for system "<<s;
    return "";
}

AbstractSystem::Processor AbstractSystem::ProcessorFromString(QString s) {
    if (s == "MOS6502" || s=="6502") return MOS6502;
    if (s == "M68000") return M68000;
    if (s == "PX86" || s=="X86") return PX86;
    if (s == "GBZ80") return GBZ80;
    if (s == "Z80") return Z80;
    if (s == "ARM") return ARM;
    if (s == "WDC65C816" || s=="65C816") return WDC65C816;
    if (s == "WDC65C02" || s =="65C02") return WDC65C02;
    if (s == "PJDH8") return PJDH8;
    if (s == "S1C88") return S1C88;
    if (s == "Z180") return Z180;
    if (s == "M6809") return M6809;
    if (s == "PCHIP8") return PCHIP8;
    if (s == "PPDP11") return PDP11;
    qDebug() << "SYSTEM CPU NOT FOUND for system "<<s;
    return MOS6502;
}

QString AbstractSystem::StringFromProcessor(QString s) {
    if (s == "CUSTOM") return "CUSTOM";//StringFromProcessor("CUSTOM");
    if (s == "PET" || s == "C64" || s == "ATARI2600" ||s == "VIC20" || s == "NES" || s == "OK64" || s == "C128" || s == "PLUS4" || s == "X16" || s == "BBCM" || s=="ATARI800"|| s=="APPLEII" || s=="ORIC"|| s=="ACORN") return "MOS6502";
    if (s == "AMIGA" || s == "ATARI520ST") return "M68000";
    if (s == "X86" || s=="WONDERSWAN") return "PX86";
    if (s == "M1ARM") return "ARM";
    if (s == "GAMEBOY") return "GBZ80";
    if (s == "SNES") return "WDC65C816";
    if (s == "MEGA65" || s=="FOENIX") return "WDC65C02";
    if (s == "JDH8") return "PJDH8";
    if (s == "TRS80COCO" || s=="VECTREX" || s=="THOMSON" || s=="DRAGON") return "M6809";
    if (s == "POKEMONMINI") return "S1C88";
    if (s == "AMSTRADCPC" || s == "TIKI100" || s=="VZ200" || s == "SPECTRUM" || s =="COLECO" || s == "MSX" || s=="TRS80" || s=="TIM" || s=="TVC" || s=="PCW" || s=="PRIMO") return "Z80";
    if (s == "CHIP8") return "PCHIP8";
    if (s == "BK0010") return "PPDP11";
    if (s == "AGON") return "EZ80";
    qDebug() << "SYSTEM STRING NOT FOUND for system "<<s ;
    return "";
}


AbstractSystem::System AbstractSystem::SystemFromString(QString s) {

    if (s.toLower()=="c64")
        return C64;
    if (s.toLower()=="c128")
        return C128;
    if (s.toLower()=="pet")
        return PET;
    if (s.toLower()=="vic20")
        return VIC20;
    if (s.toLower()=="nes")
        return NES;
    if (s.toLower()=="bbcm")
        return BBCM;
    if (s.toLower()=="amiga")
        return AMIGA;
    if (s.toLower()=="plus4")
        return PLUS4;
    if (s.toLower()=="ok64")
        return OK64;
    if (s.toLower()=="x16")
        return X16;
    if (s.toLower()=="x86")
        return X86;
    if (s.toLower()=="gameboy")
        return GAMEBOY;
    if (s.toLower()=="spectrum")
        return SPECTRUM;
    if (s.toLower()=="tiki100")
        return TIKI100;
    if (s.toLower()=="atari2600")
        return ATARI2600;
    if (s.toLower()=="atari520st")
        return ATARI520ST;
    if (s.toLower()=="amstradcpc" || s.toLower()=="amstradcpc464")
        return AMSTRADCPC;
    if (s.toLower()=="coleco")
        return COLECO;
    if (s.toLower()=="mega65")
        return MEGA65;
    if (s.toLower()=="foenix")
        return FOENIX;
    if (s.toLower()=="atari800")
        return ATARI800;
    if (s.toLower()=="msx")
        return MSX;
    if (s.toLower()=="appleii")
        return APPLEII;
    if (s.toLower()=="m1arm")
        return M1ARM;
    if (s.toLower()=="oric")
        return ORIC;
    if (s.toLower()=="snes")
        return SNES;
    if (s.toLower()=="custom")
        return CUSTOM;
    if (s.toLower()=="vz200")
        return VZ200;
    if (s.toLower()=="acorn")
        return ACORN;
    if (s.toLower()=="jdh8")
        return JDH8;
    if (s.toLower()=="pokemonmini")
        return POKEMONMINI;
    if (s.toLower()=="trs80")
        return TRS80;
    if (s.toLower()=="trs80coco")
        return TRS80COCO;
    if (s.toLower()=="wonderswan")
        return WONDERSWAN;
    if (s.toLower()=="tim")
        return TIM;
    if (s.toLower()=="tvc")
        return TVC;
    if (s.toLower()=="vectrex")
        return VECTREX;
    if (s.toLower()=="thomson")
        return THOMSON;
    if (s.toLower()=="chip8")
        return CHIP8;
    if (s.toLower()=="pcw")
        return PCW;
    if (s.toLower()=="bk0010")
        return BK0010;
    if (s.toLower()=="dragon")
        return DRAGON;
    if (s.toLower()=="agon")
        return AGON;
    if (s.toLower()=="primo")
        return PRIMO;

    qDebug() << "AbstractSystem::SystemFromString error could not identify :"+s;
    return C64;
}

QString AbstractSystem::StringFromSystem(AbstractSystem::System s) {
    if (s == C64) return "C64";
    if (s == PET) return "PET";
    if (s == VIC20) return "VIC20";
    if (s == NES) return "NES";
    if (s == C128) return "C128";
    if (s == BBCM) return "BBCM";
    if (s == AMIGA) return "AMIGA";
    if (s == PLUS4) return "PLUS4";
    if (s == OK64) return "OK64";
    if (s == X16) return "X16";
    if (s == X86) return "X86";
    if (s == GAMEBOY) return "GAMEBOY";
    if (s == SPECTRUM) return "SPECTRUM";
    if (s == TIKI100) return "TIKI100";
    if (s == ATARI2600) return "ATARI2600";
    if (s == ATARI520ST) return "ATARI520ST";
    if (s == AMSTRADCPC) return "AMSTRADCPC";
    if (s == COLECO) return "COLECO";
    if (s == MEGA65) return "MEGA65";
    if (s == MSX) return "MSX";
    if (s == ATARI800) return "ATARI800";
    if (s == APPLEII) return "APPLEII";
    if (s == M1ARM) return "M1ARM";
    if (s == ORIC) return "ORIC";
    if (s == SNES) return "SNES";
    if (s == CUSTOM) return "CUSTOM";
    if (s == VZ200) return "VZ200";
    if (s == ACORN) return "ACORN";
    if (s == JDH8) return "JDH8";
    if (s == POKEMONMINI) return "POKEMONMINI";
    if (s == TRS80) return "TRS80";
    if (s == TRS80COCO) return "TRS80COCO";
    if (s == WONDERSWAN) return "WONDERSWAN";
    if (s == TIM) return "TIM";
    if (s == TVC) return "TVC";
    if (s == VECTREX) return "VECTREX";
    if (s == THOMSON) return "THOMSON";
    if (s == CHIP8) return "CHIP8";
    if (s == PCW) return "PCW";
    if (s == BK0010) return "BK0010";
    if (s == DRAGON) return "DRAGON";
    if (s == FOENIX) return "FOENIX";
    if (s == AGON) return "AGON";
    if (s == PRIMO) return "PRIMO";
    return "";
}

bool AbstractSystem::isWDC65() {
    return m_processor==WDC65C816;
}

bool AbstractSystem::iseZ80()
{
    return m_system == AGON;

}

bool AbstractSystem::systemIsOfType(QString val)
{
    val = val.trimmed().toLower();
    if (val==StringFromSystem(m_system).toLower())
        return true;
    // Check whether CPU is the same:
    if (val == StringFromProcessor(m_processor).toLower())
        return true;
    if (val=="all")
        return true;
    if (val=="mos6502" && (m_processor==WDC65C02 || m_processor==WDC65C816))
        return true;
    if (val=="z80" && (m_processor==GBZ80))
        return true;
    if (val=="z180" && (m_processor==Z180))
        return true;
    if (val=="px86" && (m_processor==PX86))
        return true;
    if (val=="m6809" && (m_processor==M6809))
        return true;
    if (val=="chip8")
        return true;
    if (val=="ppdp11")
        return true;

    return false;
}

bool AbstractSystem::systemIsOfType(QStringList val)
{
    for (QString&s : val) {
        if (systemIsOfType(s))
            return true;
    }
    return false;
}

void AbstractSystem::AssembleOrgasm(QString& output,QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab) {
    m_orgAsm = QSharedPointer<Orgasm>(new Orgasm());
    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::MEGA65)
        m_orgAsm->m_cpuFlavor = Orgasm::CPUFLAVOR_GS4510;



    emit EmitTick("<br></font><font color=\"yellow\">Assembling with OrgAsm ");
    connect(m_orgAsm.get(), SIGNAL(EmitTick(QString)), this, SLOT( AcceptDispatcherTick(QString)));
    if (symTab!=nullptr) {
        m_orgAsm->SetupConstants(symTab);
        m_orgAsm->m_extraSymbols = symTab->m_extraAtSymbols;
        m_orgAsm->m_extraMonCommands = symTab->m_extraMonCommands;
    }
    m_orgAsm->Assemble(filename+".asm", filename+".prg");

    disconnect(m_orgAsm.get(), SIGNAL(EmitTick(QString)), this, SLOT( AcceptDispatcherTick(QString)));
    output = m_orgAsm->m_output;
    if (m_orgAsm->m_hasOverlappingError) {
        output = m_orgAsm->error.msg;
        output +="<br><br>Line "+QString::number(m_orgAsm->error.oline.m_lineNumber+1)+ " in " +"<font color=\"orange\">"+filename+" :</font> "" : " +m_orgAsm->error.oline.m_orgLine;
        m_orgOutput = output;
        text="<font color=\"#FF6040\">Fatal error during OrgAsm assembly!</font><br>";
        text+=output;
        m_buildSuccess = false;
        return;

    }

    m_addresses = m_orgAsm->m_lineAddress;
    if (m_projectIni->getdouble("output_debug_symbols")==1.0)
        m_orgAsm->SaveSymbolsList(filename+".sym");

}

void AbstractSystem::AssembleZOrgasm(QString& output, QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab, int orgType)
{
    if (orgType==0)
        m_orgAsm = QSharedPointer<ZOrgasm>(new ZOrgasm());
    if (orgType==1)
        m_orgAsm = QSharedPointer<MOrgasm>(new MOrgasm());
    if (orgType==2) {
        m_orgAsm = QSharedPointer<MOrgasm>(new MOrgasm());
        m_orgAsm->m_header = Orgasm::HEADER_DECB;
    }
    if (orgType==4)
        m_orgAsm = QSharedPointer<OrgAsm68k>(new OrgAsm68k());

    if (m_orgAsm == nullptr)
        return;
    m_orgAsm->m_cpuFlavor = getCPUFlavorint();

    emit EmitTick("<br></font><font color=\"yellow\">Assembling with OrgAsm ");
    connect(m_orgAsm.get(), SIGNAL(EmitTick(QString)), this, SLOT( AcceptDispatcherTick(QString)));
    /*    if (symTab!=nullptr)
            m_orgAsm->SetupConstants(symTab);
    */
    if (symTab!=nullptr)
        m_orgAsm->m_extraSymbols = symTab->m_extraAtSymbols;

    m_orgAsm->Assemble(filename+".asm", filename+".bin");

    disconnect(m_orgAsm.get(), SIGNAL(EmitTick(QString)), this, SLOT( AcceptDispatcherTick(QString)));
    output = m_orgAsm->m_output;
    if (m_orgAsm->m_hasOverlappingError) {
        output = m_orgAsm->error.msg;
        output +="<br><br>Line "+QString::number(m_orgAsm->error.oline.m_lineNumber+1)+ " in " +"<font color=\"orange\">"+filename+" :</font> "" : " +m_orgAsm->error.oline.m_orgLine;
        m_orgOutput = output;
        text="<font color=\"#FF6040\">Fatal error during OrgAsm assembly!</font><br>";
        text+=output;
        m_buildSuccess = false;
        return;

    }

    m_addresses = m_orgAsm->m_lineAddress;
    if (m_projectIni->getdouble("output_debug_symbols")==1.0)
        m_orgAsm->SaveSymbolsList(filename+".sym");

    text+=output;
    m_buildSuccess = m_orgAsm->m_success;

}

void AbstractSystem::AssembleCL65(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab, QString ending, QStringList params)
{
    QString output;
    int time = timer.elapsed();
    int codeEnd = 0;
    QString assembler = m_settingsIni->getString("cl65");

    if (!QFile::exists(assembler)) {
        text  += "<br><font color=\"#FF6040\">Please set up a link to the CL65 assembler in the TRSE settings panel.</font>";
        m_buildSuccess = false;
        return;
    }

    //qDebug() << m_settingsIni->getString("assembler");
    emit EmitTick("<br>Assembling with CL65 ...");

    QProcess process;
    //    QStringList params = QStringList() <<"-C" <<smc <<("-o"+filename+".smc") <<(filename +".asm");
    process.start(assembler, params);
    process.waitForFinished();
    //process;
    output = process.readAllStandardOutput() + process.readAllStandardError();
    //    qDebug() << output;
    // codeEnd=FindEndSymbol(output);

    int assembleTime = timer.elapsed()- time;
    time = timer.elapsed();

    //        qDebug() << "Code end: " << Util::numToHex(codeEnd) << codeEnd;
    int orgFileSize = QFile(filename+"."+ending).size();

    //    qDebug() << "MOS " << Syntax::s.m_stripPrg << Syntax::s.m_ignoreSys;




    int size = QFile(filename+"."+ending).size();
    int crunchTime = timer.elapsed()- time;



    m_buildSuccess = true;
    m_orgOutput = "";
    if (output.toLower().contains("error")) {
        m_orgOutput = output;
        text="<font color=\"#FF6040\">Fatal error during assembly!</font><br>";
        m_buildSuccess = false;
    }

    if (output.trimmed().contains("error")) {
        m_buildSuccess = false;
    }

    if (m_buildSuccess) {

        output ="Assembled file size: <b>" + QString::number(size) + "</b> bytes";
        if (m_projectIni->getdouble("exomizer_toggle")==1) {
            output=output+" (<font color=\"#70FF40\"> " + QString::number((int)(100.0*(float)size/(float)orgFileSize))+  " % </font> of original size ) <br>";
            output=output+"Original file size: " + QString::number(orgFileSize) + " bytes";
        }
        output = output + "\nAssemble time: <font color=\"#70FF40\">"+ (Util::MilisecondToString(assembleTime)) + "</font>";
        //       output = output + "Crunch time: <font color=\"#70FF40\">"+ (Util::MilisecondToString(crunchTime)) + "</font>";

    }


    text+=output;


}

QStringList AbstractSystem::AnalyseForPotentialVariables(QString asmCode)
{
    QStringList lst = asmCode.split("\n");
    QStringList results;
    for (auto s : lst) {
        s = s.trimmed().simplified();
        if (s.startsWith(";"))
            continue;
        QStringList p = s.split(" ");
        // variables only show up in "lda myvar etc"
        for (int i=1;i<p.count();i++) {
            QStringList split = p[i].split(","); // Might be a comma!
            for (auto sp : split) {
                QStringList v = Wash(sp);
                for (auto item:v) {
                    if (Syntax::s.m_currentSystem->m_registers.contains(item.toLower()))
                        continue;
                    results.append(item);
                }
            }
        }
    }
    results.removeAll("");
//    qDebug() << results;
    return results;
}

QStringList AbstractSystem::Wash(QString s)
{
    s = s.replace("[","");
    s = s.replace("]","");
    s = s.replace("(","");
    s = s.replace(")","");
    s = s.replace("#","");
    s = s.replace("<","");
    s = s.replace(">","");

    s = s.trimmed().simplified();
    QStringList allHoly;
    allHoly<< s.split("+");
    allHoly<< s.split("-");
    allHoly<< s.split("*");
    allHoly<< s.split("/");
    allHoly.removeDuplicates();

    return allHoly;
}

void AbstractSystem::Sparkle(QString &text, QString filename, QString currentDir)
{
    // First, create sparkle text file
//    QString s;
//    s+="DirIndex:\t00\n";
 /*   s+="Tracks:\t35\n";

    s+="File:\t" + filename+".prg\n";



    CIniFile paw;
    paw.Load(currentDir + "/"+m_projectIni->getString("disk1_paw"));
    QStringList data = paw.getStringList("data");
    //QStringList data_tc = paw.getStringList("data_tinycrunch");

    int count = data.count()/3;
//    qDebug() << data;
    for (int i=0;i<count;i++) {
        QString orgFileName = data[3*i+1];
        QString name = data[3*i];

        int address = Util::NumberFromStringHex( data[3*i+2]);
        QString fn = currentDir+orgFileName;
        if (!QFile::exists(fn)) {
            text=text + "<br><font color=\"#FF6040\">Error: Could not append disk file '"+fn+"' because it does not exist</font><br>";
            m_buildSuccess = false;
            return;
        }
        QString oname = name;

        s+="\nDirIndex:\t" +QString::number((i+1),16)+"\n";
        s+="Align\n";
        s+="File:\t" + currentDir+"/"+orgFileName +"\t" +QString::number(address,16)+"\n";

//        QString of = outFolder+"/"+orgFileName.split("/").last();
        //Util::ConvertFileWithLoadAddress(fn,of,address);
//        d64Params << "-f" <<oname << "-w" <<of;

    }

    Util::SaveTextFile(filename+".sls",s);



*/

    Util::SaveTextFile(filename+".sls",Data::data.sparkle);

    auto disk = filename+".d64";
    if (QFile::exists(disk))
        QFile::remove(disk);
    QString sparkle = m_settingsIni->getString("sparkle");
    if (!QFile::exists(sparkle)) {
        text+="<br><br><font color=\"#FF2020\">Error assembling disk: </font>Sparkle 3 not found. Please download sparkle 3 from https://csdb.dk/release/?id=236718 and set up a link in the TRSE settings panel.";
        m_buildSuccess = false;
        return;
    }
    QString sout;
    StartProcess(sparkle,QStringList() << filename+".sls",sout,true,currentDir);

    qDebug() << sout;


    if (!QFile::exists(disk)) {
        text+="<br><br><font color=\"#FF2020\">Error assembling disk using sparkle: </font><br><br>";
        text+=sout;
        m_buildSuccess = false;
        return;

    }

}

bool AbstractSystem::GenericAssemble(QString assembler, QStringList params, QString error, QString &text, QString workingDir)
{
    QString output;
    int time = timer.elapsed();
    int codeEnd = 0;

    if (!QFile::exists(assembler)) {
        text  += "<br><font color=\"#FF6040\">"+error+"</font>";
        m_buildSuccess = false;
        return false;
    }

    //qDebug() << m_settingsIni->getString("assembler");
    QProcess process;
    //    QStringList params;
    //  params << filename+".asm";
    //    qDebug() << "Assembling with "<<assembler;
    if (workingDir!="")
        process.setWorkingDirectory(workingDir);

    process.start(assembler, params);
    process.waitForFinished();
    output = process.readAllStandardOutput();
    output+= process.readAllStandardError();
    if (output.contains("error"))
        m_buildSuccess = false;

    qDebug().noquote() << output;

    int assembleTime = timer.elapsed()- time;
    time = timer.elapsed();

    text+=output;
    return m_buildSuccess;

}

void AbstractSystem::AcceptDispatcherTick(QString val)
{
    emit EmitTick(val);
}


void AbstractSystem::AssembleTripe(QString& text, QString file, QString currentDir, QSharedPointer<SymbolTable>  symTab) {
    if (m_projectIni->getdouble("use_tripe")==0)
        return;


    QString error="";
    Util::CopyFile(file+".asm",file+".ll");
    QFile::remove(file+".asm");
    /*
    QStringList params  = QStringList() << "-arch" <<"trasm2tripe" << "-i"<< file+"_tripe.asm" << "-o"<<file+".trp";
    GenericAssemble(m_settingsIni->getString("tripe_location"),params,error,text);

    //    qDebug() << m_settingsIni->getString("tripe_location");
    //  qDebug() << params;

    //qDebug() << error<<text;

    params  = QStringList() << "-arch" <<"mos6502" << "-i"<< file+".trp" <<"-o"<<file+".asm";
    GenericAssemble(m_settingsIni->getString("tripe_location"),params,error,text);
    if (error.contains("error"))
        m_buildSuccess = false;
*/
    //    qDebug() << error;

    //    Util::CopyFile()

}
/*
bool AbstractSystem::isSupported(QString currentSystem, QString list) {
    QStringList lst  = list.toLower().trimmed().simplified().split(",");
    for (QString s : lst)
        if (s == currentSystem.toLower() || s=="all")
            return true;

    return false;
}
*/
