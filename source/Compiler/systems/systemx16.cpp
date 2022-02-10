#include "systemx16.h"


SystemX16::SystemX16(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : System65C816(settings, proj) {
    m_processor = WDC65C02;
    m_system = X16;
    m_systemColor = QColor(100,60,40);
    DefaultValues();
}

void SystemX16::DefaultValues()
{
    m_ignoreSys = false;
    m_startAddress=0x801;
    m_programStartAddress = 0x810;
    m_stripPrg = false;

}

void SystemX16::PrepareInitialAssembler(Assembler *as)
{
    //Asm(".byte $00 ; fill $xxx0");
    as->Asm(".byte $0b,$08,$e3,$07,$9E");
    // write PETSCII / ASCII representation of address to call
    as->Asm(Util::IntToHexString(Syntax::s.m_currentSystem->m_programStartAddress));
    as->Asm(".byte $00, $00, $00,0,0,0 ; end of program");

    /* // old method
        Asm(".byte    $0, $0E, $08, $0A, $00, $9E, $20");
        Asm(intToHexString(Syntax::s.m_currentSystem->m_programStartAddress));
        Asm(".byte     $00");   // 6, 4, )
        */
    as->Nl();

    as->EndMemoryBlock();
//        Comment("End of SYS memory block, starting new");
    as->StartMemoryBlock(Util::numToHex(Syntax::s.m_currentSystem->m_programStartAddress));
    as->Write(as->GetOrg(Syntax::s.m_currentSystem->m_programStartAddress));
    as->m_insertEndBlock = "EndBlock"+Util::numToHex(Syntax::s.m_currentSystem->m_programStartAddress).remove("$");




}

void SystemX16::Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab)
{

    QString smc = currentDir + QDir::separator() + "config.cfg";
    if (!QFile::exists(smc))
        Util::CopyFile(":resources/code/mega65/config.cfg",smc);
    //        QStringList params = QStringList() << "-t none" << "-C" <<smc <<"--start-addr"<< "$2020"<<("-o"+filename+".prg") <<(filename +".asm") ;
//    QStringList params = QStringList() << "-t"<<"none" << "-C" <<smc <<"--start-addr"<< "$801"<<("-o"+filename+".prg") <<(filename +".asm") ;
    QStringList params = QStringList() << "-t"<<"none" << "-C" <<smc <<("-o"+filename+".prg") <<(filename +".asm") ;
    //        QStringList params = QStringList() <<"--cpu"<<"4510"<<(filename +".asm") <<("-o"+filename+".prg");
    AssembleCL65(text,filename,currentDir,symTab,"prg",params);
    QString f= filename+".prg";
    QByteArray da = Util::loadBinaryFile(f);
    da.insert(0,(uchar)01);
    da.insert(1,(uchar)0x08);
    Util::SaveByteArray(da,f);

    System65C816::Assemble(text,filename,currentDir,symTab);
}

