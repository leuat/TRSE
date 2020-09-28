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
    return CompressLZ4(fileName);
}
