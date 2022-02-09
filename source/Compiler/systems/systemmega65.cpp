#include "systemmega65.h"


void SystemMega65::PrepareInitialAssembler(Assembler *as)
{
    as->Asm(" .org $2001");
    if (!Syntax::s.m_ignoreSys){// && (Syntax::s.m_currentSystem->m_programStartAddress!=Syntax::s.m_currentSystem->m_startAddress)) {
        as->Asm(" .byte $09,$20 ;End of command marker (first byte after the 00 terminator)");
        as->Asm(" .byte $0a,$00 ;10");
        as->Asm(" .byte $fe,$02,$30,$00 ;BANK 0");
        //            Asm(" .byte <endd_s, >endd_s ");
        as->Asm(" .byte $13, $20 ");
        as->Asm(" .byte $14,$00 ;20");
        as->Asm(" .byte $9e ;SYS");
        //            Asm(intToHexString(Syntax::s.m_currentSystem->m_programStartAddress));
        //          QString s = QString::number(Syntax::s.m_currentSystem->m_programStartAddress);
        as->Asm(" .byte $38,$32,$32,$34");
        //
        //QString extra = "";
        //if (s.count()<5)
        //                extra=", $00";
        as->Asm("  .byte $00");
        as->Label("endd_s:");
        as->Asm("  .byte $00,$00    ;End of basic terminators");
        as->Asm("  .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF    ;extra");
        as->EndMemoryBlock();
        //        Comment("End of SYS memory block, starting new");
        as->StartMemoryBlock(Util::numToHex(Syntax::s.m_currentSystem->m_programStartAddress));
        as->m_insertEndBlock = "EndBlock"+Util::numToHex(Syntax::s.m_currentSystem->m_programStartAddress).remove("$");

    }

}

void SystemMega65::Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab)
{
    QString smc = currentDir + QDir::separator() + "config.cfg";
    if (!QFile::exists(smc))
        Util::CopyFile(":resources/code/mega65/config.cfg",smc);
    //        QStringList params = QStringList() << "-t none" << "-C" <<smc <<"--start-addr"<< "$2020"<<("-o"+filename+".prg") <<(filename +".asm") ;
    QStringList params = QStringList() << "-t"<<"none" << "-C" <<smc <<"--start-addr"<< "$2020"<<("-o"+filename+".prg") <<(filename +".asm") ;
    //        QStringList params = QStringList() <<"--cpu"<<"4510"<<(filename +".asm") <<("-o"+filename+".prg");
    AssembleCL65(text,filename,currentDir,symTab,"prg",params);
    QString f= filename+".prg";
    QByteArray da = Util::loadBinaryFile(f);
    da.insert(0,(uchar)01);
    da.insert(1,(uchar)0x20);
    Util::SaveByteArray(da,f);

    System65C816::Assemble(text,filename,currentDir,symTab);

}
