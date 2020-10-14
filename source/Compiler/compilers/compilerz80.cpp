#include "compilerz80.h"


void CompilerZ80::InitAssemblerAnddispatcher(QSharedPointer<AbstractSystem> system)
{
    m_assembler = QSharedPointer<AsmZ80>(new AsmZ80());//
    m_dispatcher = QSharedPointer<ASTdispatcherZ80>(new ASTdispatcherZ80());
    m_dispatcher->m_outputLineNumbers = false;

    if (m_projectIni->getdouble("override_target_settings")==1) {
        Syntax::s.m_currentSystem->m_programStartAddress = Util::NumberFromStringHex(m_projectIni->getString("override_target_settings_org"));
    }

    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::AMSTRADCPC464)
    if (m_projectIni->getdouble("exomizer_toggle")==1.0)
        Syntax::s.m_currentSystem->m_programStartAddress = 0x300; // Unpack address


//    m_assembler->Asm("CPU "+m_projectIni->getString("cpu_Z80_system"));

    if (Syntax::s.m_currentSystem->m_system != AbstractSystem::COLECO)
        m_assembler->Asm(" org "+Util::numToHex(Syntax::s.m_currentSystem->m_programStartAddress));

    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::TIKI100) {
        m_assembler->Asm(" jp $8000");
        m_assembler->Asm(" org $8000");

    }

    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::COLECO) {
        m_assembler->IncludeFile(":resources/code/coleco/header.asm");
//        m_assembler->Asm(" org 8000h");


    }
    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::AMSTRADCPC464) {
        m_assembler->m_symTab->m_constants = m_parser.m_symTab->m_constants;
        m_assembler->WriteConstants();
    }


    // Init default stuff



}

void CompilerZ80::Connect()
{
    m_assembler->m_source<<m_parser.m_parserAppendix;
    m_assembler->IncludeFile(":resources/code/Z80/memcpy.asm");
    m_assembler->Connect();

    m_assembler->EndMemoryBlock();


    if (Syntax::s.m_currentSystem->m_system != AbstractSystem::COLECO)
        m_assembler->Asm("end");//+Util::numToHex(Syntax::s.m_currentSystem->m_programStartAddress));





    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::COLECO) {

        m_assembler->IncludeFile(":resources/code/coleco/footer.asm");
        m_assembler->m_source <<m_assembler->m_wram->m_source;
    }

    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::AMSTRADCPC464) {
//        m_assembler->IncludeFile(":resources/code/amstrad/playerakg.asm");
    }




}

void CompilerZ80::SetupMemoryAnalyzer(QString filename)
{
 /*   Orgasm orgAsm;
    orgAsm.SetupConstants(m_parser.m_symTab);
    //orgAsm.Codes();
    orgAsm.Assemble(filename+".asm", filename+".prg");
    if (!orgAsm.m_success) {
        return;
    }
    */

    QProcess process;
    QString assembler = m_ini->getString("pasmo");
    QString output;
    Syntax::s.m_currentSystem->StartProcess(assembler, QStringList() << "-1"<< filename+".asm" <<filename+".bin", output, true);

    QVector<QSharedPointer<MemoryBlock>> nb;
    for (QSharedPointer<MemoryBlock> mb: m_assembler->blocks) {
        if (mb->m_type == MemoryBlock::USER)
            nb.append(mb);
    }

    m_assembler->blocks.clear();
    m_assembler->blocks = nb;

/*    int codeEnd=FindEndSymbol(orgAsm);
    QVector<int> ends = FindBlockEndSymbols(orgAsm);
    //    qDebug() << "B";
    ConnectBlockSymbols(ends);
        */
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
/*        if (l[0].toLower().startsWith("endblock")) {
//            qDebug() << "FOUND END AT " <<
            end = l[0].toLower().remove("endblock").toInt(&ok,16);
            codeBlocks.append(QSharedPointer<MemoryBlock>(new MemoryBlock(start, end, MemoryBlock::CODE, "code")));

        }*/

        if (l.count()>=2) {
            if (l[1].toLower()=="label") {
                if (l[2].toLower().startsWith("mainprogram_end")) {
                    end = l[0].split(":")[0].toInt(&ok,16);
                    codeBlocks.append(QSharedPointer<MemoryBlock>(new MemoryBlock(start, end, MemoryBlock::CODE, "code")));
                  //  qDebug() << "**************************** END found at "<< Util::numToHex(end);
                    continue;

                }
            }
/*            if (l[1].toLower() == "end") {
                end = l[0].split(":")[0].toInt(&ok,16);
                codeBlocks.append(QSharedPointer<MemoryBlock>(new MemoryBlock(start, end, MemoryBlock::CODE, "code")));
                qDebug() << "**************************** END found at "<< Util::numToHex(end);
                continue;
            }
            */
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

}

