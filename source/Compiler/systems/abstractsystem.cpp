#include "abstractsystem.h"
#include <QProcess>
#include "source/LeLib/util/lz4/lz4.h"
#include "source/LeLib/util/util.h"

QMap<SystemLabel::Type, QColor> AbstractSystem::m_labelColors;

AbstractSystem::AbstractSystem(AbstractSystem *a) {
}

QString AbstractSystem::CompressLZ4(QString fileName) {
    QString old = fileName;
    QByteArray in = Util::loadBinaryFile(fileName);
    fileName = fileName+ "_c";
    QByteArray out;
    out.resize(65536);
    //LZ4LIB_API int LZ4_compress_default(const char* src, char* dst, int srcSize, int dstCapacity);
    int size = LZ4_compress_default(in.data(),out.data(), in.length(),65535);
    out.resize(size);
    QByteArray header;
    header.append(0x04); // version
    header.append(0x22);
    header.append(0x4d);
    header.append(0x18); // magic number
    header.append((0x64+in.length()) & 0xFF); // lo size
    header.append((in.length()>>8)&0xFF);

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


void AbstractSystem::StartProcess(QString file, QStringList params, QString& output, bool standardOutput ) {
    // qDebug() << params;
    QProcess process;
    process.start(file, params);
    process.waitForFinished();
    //    qDebug() << process.readAllStandardOutput();;
    //    qDebug() << process.readAllStandardError();
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

void AbstractSystem::AcceptDispatcherTick(QString val)
{
    emit EmitTick(val);
}
