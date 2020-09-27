#include "systemz80.h"
#include "source/LeLib/util/lz4/lz4.h"

void SystemZ80::Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab)
{

}

void SystemZ80::PostProcess(QString &text, QString file, QString currentDir)
{

}

QString SystemZ80::CompressFile(QString fileName)
{
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
