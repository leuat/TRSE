#include "abstractsystem.h"
#include <QProcess>
#include "source/LeLib/util/lz4/lz4hc.h"
#include "source/LeLib/util/util.h"

QMap<SystemLabel::Type, QColor> AbstractSystem::m_labelColors;

AbstractSystem::AbstractSystem(AbstractSystem *a) {
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


void AbstractSystem::StartProcess(QString file, QStringList params, QString& output, bool standardOutput, QString currentDir) {
    // qDebug() << params;
    QProcess process;
    if (currentDir != "")
        process.setWorkingDirectory(currentDir);
    process.start(file, params);
    process.waitForFinished();
//        qDebug() << process.readAllStandardOutput();;
  //      qDebug() << process.readAllStandardError();
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
    if (s.toLower()=="atari800")
        return ATARI800;
    if (s.toLower()=="msx")
        return MSX;
    if (s.toLower()=="appleii")
        return APPLEII;
    if (s.toLower()=="m1arm")
        return M1ARM;

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
    return "";
}

bool AbstractSystem::GenericAssemble(QString assembler, QStringList params, QString error, QString &text)
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
    process.start(assembler, params);
    process.waitForFinished();
    output = process.readAllStandardOutput();
    output+= process.readAllStandardError();
    if (output.contains("error"))
        m_buildSuccess = false;
    int assembleTime = timer.elapsed()- time;
    time = timer.elapsed();

    text+=output;
    return m_buildSuccess;

}

void AbstractSystem::AcceptDispatcherTick(QString val)
{
    emit EmitTick(val);
}
