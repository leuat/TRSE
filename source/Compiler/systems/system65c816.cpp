#include "system65c816.h"

System65C816::System65C816(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : AbstractSystem(settings, proj) {
    m_allowedGlobalTypeFlags << "compressed"<<"pure"<<"pure_variable" <<"pure_number" << "signed" <<"no_term" <<"invert"<<"bank"<<"lpointer";
    m_allowedProcedureTypeFlags << "pure"<<"pure_variable" <<"pure_number" << "signed" <<"no_term" <<"invert" <<"global" <<"stack"<<"lpointer";
    m_supportsExomizer = true;
    m_registers << "_a"<<"_x" <<"_y" <<"_ax" <<"_ay" <<"_xy";
    m_canRunAsmFiles = true;
    m_allowClasses = true; // EXPERIMENTAL
    m_processor = WDC65C816;

    m_renameVariables << "a" <<"x"<<"y"<<"pc"<<"sp";

}

void System65C816::Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab)
{
    if (Syntax::s.m_currentSystem->m_system==SNES) {
        QString smc = currentDir + QDir::separator() + "smc.cfg";
        if (!QFile::exists(smc))
            Util::CopyFile(":resources/code/snes/smc.cfg",smc);

        QStringList params = QStringList() <<"-C" <<smc <<("-o"+filename+".smc") <<(filename +".asm");

/*        QString music = currentDir + QDir::separator() + "music.asm";
        if (!QFile::exists(music))
            Util::CopyFile(":resources/code/snes/music.asm",music);
*/

        AssembleCL65(text,filename,currentDir,symTab,"smc",params);
    }
    if (Syntax::s.m_currentSystem->m_system==MEGA65) {
        QString smc = currentDir + QDir::separator() + "config.cfg";
//        if (!QFile::exists(smc))
            Util::CopyFile(":resources/code/mega65/config.cfg",smc);
        QStringList params = QStringList() <<"-C" <<smc <<"--start-addr"<< "$2020"<<("-o"+filename+".prg") <<(filename +".asm") ;
//        QStringList params = QStringList() <<"--cpu"<<"4510"<<(filename +".asm") <<("-o"+filename+".prg");
        AssembleCL65(text,filename,currentDir,symTab,"prg",params);

        QString f= filename+".prg";
        QByteArray da = Util::loadBinaryFile(f);
        da.insert(0,(uchar)01);
        da.insert(1,(uchar)0x20);
        Util::SaveByteArray(da,f);



    }


}


void System65C816::PostProcess(QString &text, QString file, QString currentDir)
{

}

void System65C816::DefaultValues()
{
    m_ignoreSys = false;
    m_startAddress=0x7FF;
}
