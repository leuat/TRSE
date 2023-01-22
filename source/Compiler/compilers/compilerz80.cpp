#include "compilerz80.h"


void CompilerZ80::InitAssemblerAnddispatcher(QSharedPointer<AbstractSystem> system)
{
    m_assembler = QSharedPointer<AsmZ80>(new AsmZ80());//
    m_codeGen = QSharedPointer<CodeGenZ80>(new CodeGenZ80());
    m_codeGen->m_outputLineNumbers = false;

    if (m_projectIni->getdouble("override_target_settings")==1) {
        Syntax::s.m_currentSystem->m_programStartAddress = Util::NumberFromStringHex(m_projectIni->getString("override_target_settings_org"));
    }

    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::AMSTRADCPC)
    if (m_projectIni->getdouble("exomizer_toggle")==1.0)
        Syntax::s.m_currentSystem->m_programStartAddress = 0x300; // Unpack address

    auto sys = m_projectIni->getString("cpu_Z80_system");
    if (sys=="") sys ="z80";
    m_assembler->Asm("CPU "+sys);

    if (Syntax::s.m_currentSystem->m_system != AbstractSystem::COLECO)
        m_assembler->Asm(" org "+Util::numToHex(Syntax::s.m_currentSystem->m_programStartAddress));

    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::MSX) {
        /*        m_assembler->Asm("db $FE     ; magic number");
        m_assembler->Asm("dw msx_prg_begin ; begin address   ");
        m_assembler->Asm("dw msx_prg_end - 1 ; end   ");
        m_assembler->Asm("dw msx_prg_begin ; execute   ");
        m_assembler->Label("msx_prg_begin");*/

        m_assembler->Asm("db \"AB\"		; ID for auto-executable ROM");
        m_assembler->Asm("dw msx_prg_start		; Main program execution address.");
        m_assembler->Asm("dw 0		; STATEMENT");
        m_assembler->Asm("dw 0		; DEVICE");
        m_assembler->Asm("dw 0		; TEXT");
        m_assembler->Asm("dw 0,0,0	; Reserved    ");
        m_assembler->Label("msx_prg_start");
    }


    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::TIKI100) {
//        m_assembler->Asm(" jp $8000");
        m_assembler->m_source.clear();
        m_assembler->Asm(" org $8000");

    }

    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::COLECO) {
        m_assembler->IncludeFile(":resources/code/coleco/header.asm");
//        m_assembler->Asm(" org 8000h");

    }
    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::VIDEOTON)
        m_assembler->IncludeFile(":resources/code/videoton.asm");

    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::AMSTRADCPC ||
        Syntax::s.m_currentSystem->m_system == AbstractSystem::SPECTRUM
      ||  Syntax::s.m_currentSystem->m_system == AbstractSystem::MSX
            ||  Syntax::s.m_currentSystem->m_system == AbstractSystem::TIM
            ||  Syntax::s.m_currentSystem->m_system == AbstractSystem::VIDEOTON
        ) {
        m_assembler->m_symTab->m_constants = m_parser.m_symTab->m_constants;
        m_assembler->WriteConstants();
    }

    // Init default stuff



}

void CompilerZ80::Connect()
{
    m_assembler->m_source<<m_parser.m_parserAppendix;
//    if (Syntax::s.m_currentSystem->m_system!=AbstractSystem::TRS80)
  //  {
        m_assembler->IncludeFile(":resources/code/Z80/memcpy.asm");
        m_assembler->IncludeFile(":resources/code/Z80/init.s");
    //    }
    m_assembler->Connect();

    m_assembler->EndMemoryBlock();



    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::MSX) {
        m_assembler->m_ram->m_source.insert(0," org $e000 ; RAM ");
        m_assembler->m_source <<m_assembler->m_ram->m_source;


    }

    if (Syntax::s.m_currentSystem->m_system != AbstractSystem::COLECO)
        m_assembler->Asm("end");//+Util::numToHex(Syntax::s.m_currentSystem->m_programStartAddress));


    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::COLECO) {

        m_assembler->IncludeFile(":resources/code/coleco/footer.asm");
        m_assembler->m_source <<m_assembler->m_wram->m_source;
    }

    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::AMSTRADCPC) {
//        m_assembler->IncludeFile(":resources/code/amstrad/playerakg.asm");
    }




}

/*bool CompilerZ80::SetupMemoryAnalyzer(QString filename, Orgasm* orgAsm)
{
    QString output;
    QString assembler = m_ini->getString("pasmo");
    Syntax::s.m_currentSystem->StartProcess(assembler, QStringList() << "-1"<< filename+".asm" <<filename+".bin", output, true);
    if (m_assembler==nullptr)
        return true;

//    qDebug().noquote() << output;

  //  output = Util::loadTextFile(filename+".sym");

    QVector<QSharedPointer<MemoryBlock>> nb;
    for (QSharedPointer<MemoryBlock> mb: m_assembler->blocks) {
        if (mb->m_type == MemoryBlock::USER)
            nb.append(mb);
    }

    m_assembler->blocks.clear();
    m_assembler->blocks = nb;

    int start = 0;
    int end = 0;
    QStringList lst = output.split("\n");
    int cnt = 0;
    QVector<QSharedPointer<MemoryBlock>> codeBlocks;
    bool ignore = false;
    for (QString s : lst) {

        if (ignore)
            continue;
        s = s.replace("\t"," ").trimmed().simplified();
        QStringList l = s.split(" ");
        if (l.count()==0) continue;
        bool ok = false;
        if (l[0].toLower() == "org") {
            start = l[1].toInt(&ok,16);
//            qDebug() << "ORG " << Util::numToHex(start);
        }



        if (l[0].toLower()=="incbin") {
            QString file = l[1];
            int binStart = lst[cnt-1].split(":")[0].toInt(&ok,16);
            if (binStart==0) binStart = start;
            int binEnd = binStart + QFileInfo(file).size();
            m_assembler->blocks.append(QSharedPointer<MemoryBlock>(new MemoryBlock(binStart, binEnd, MemoryBlock::DATA, file)));
//            qDebug() << "Found FILE " << file << Util::numToHex(binStart) << Util::numToHex(binEnd);

        }

        if (l.count()>=2) {
            if (l[1].toLower()=="label") {
                if (l[2].toLower().startsWith("mainprogram_end")) {
                    end = l[0].split(":")[0].toInt(&ok,16);
                    codeBlocks.append(QSharedPointer<MemoryBlock>(new MemoryBlock(start, end, MemoryBlock::CODE, "code")));
                  //  qDebug() << "**************************** END found at "<< Util::numToHex(end);
                    continue;

                }
            }
            if (l[1].toLower() == "end") {
//                qDebug() <<" Breaking at END";
                ignore = true;
                break;

            }

        }
        cnt++;

    }
  //  qDebug().noquote() << output;
    //qDebug() << Util::numToHex(start) << Util::numToHex(end);
    //int start = Syntax::s.m_currentSystem->m_startAddress;
    //if (Syntax::s.m_currentSystem->m_startAddress!=Syntax::s.m_currentSystem->m_programStartAddress)
    //    start = Syntax::s.m_currentSystem->m_programStartAddress;
    m_assembler->blocks = codeBlocks <<m_assembler->blocks;
    return true;
}
*/
