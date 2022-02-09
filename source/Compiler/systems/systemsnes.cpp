#include "systemsnes.h"


void SystemSNES::DefaultValues()
{
    useZByte = true;
    m_hasVariableColorPalette = true;

}

void SystemSNES::Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab)
{
    QString smc = currentDir + QDir::separator() + "smc.cfg";
    if (!QFile::exists(smc))
        Util::CopyFile(":resources/code/snes/smc.cfg",smc);

    QStringList params = QStringList() <<"-C" <<smc <<("-o"+filename+".smc") <<(filename +".asm");

    /*        QString music = currentDir + QDir::separator() + "music.asm";
        if (!QFile::exists(music))
            Util::CopyFile(":resources/code/snes/music.asm",music);
*/

    AssembleCL65(text,filename,currentDir,symTab,"smc",params);

    System65C816::Assemble(text,filename,currentDir,symTab);

}
