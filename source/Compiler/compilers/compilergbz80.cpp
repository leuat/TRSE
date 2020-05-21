#include "compilergbz80.h"


void CompilerGBZ80::InitAssemblerAnddispatcher(QSharedPointer<AbstractSystem> system)
{
    m_assembler = QSharedPointer<AsmZ80>(new AsmZ80());//
    m_dispatcher = QSharedPointer<ASTdispatcherZ80>(new ASTdispatcherZ80());
    //    m_assembler->Asm("CPU "+m_projectIni->getString("cpu_GBZ80_system"));

    m_assembler->IncludeFile(":resources/code/gameboy/defs.s");
    m_assembler->IncludeFile(":resources/code/gameboy/gbt_player.inc");
    m_assembler->IncludeFile(":resources/code/gameboy/init.s");



}

void CompilerGBZ80::Connect()
{
    m_assembler->m_source<<m_parser.m_parserAppendix;

    m_assembler->EndMemoryBlock();
    m_assembler->Connect();



    for (QString key : m_assembler->m_banks.keys()) {
        QSharedPointer<Appendix> app = m_assembler->m_banks[key];
        QString rom = QString::number(Util::NumberFromStringHex(key));
        app->m_source.insert(0,"		SECTION	\"ROM"+rom+"\",ROMX[$4000], BANK["+rom+"]"		);
        m_assembler->m_source << app->m_source;

    }

    m_assembler->m_sprram->m_source.insert(0,"		SECTION	\"WRAM\",WRAM0[$C000]"		);

    m_assembler->m_source <<m_assembler->m_sprram->m_source;

    m_assembler->m_source <<"nada_wram_padding : ds "+Util::numToHex(0x100-m_assembler->m_sprram->m_dataSize);


    //    m_assembler->m_wram->m_source.insert(0,"		SECTION	\"WRAM\",WRAM0[$C100]"		);

    m_assembler->m_source <<m_assembler->m_wram->m_source;

    m_assembler->m_hram->m_source.insert(0,"		SECTION	\"HRAM\",HRAM[$FF8A]"		);
    m_assembler->m_source <<m_assembler->m_hram->m_source;


}

void CompilerGBZ80::SetupMemoryAnalyzer(QString filename)
{
    QString s = Util::loadTextFile(filename+".sym");
    if (!QFile::exists(filename+".sym"))
        return;


    QStringList lst = s.split("\n");
    lst.removeFirst();
    int codeStart = 0x150;
    int varStart = 0xC000;
    bool done = false;
    int i=0;
    int maxB1  = codeStart;
    int maxV1  = varStart;
    while (!done) {
        if (lst[i].count()==0) {
            done=++i>=lst.count();
            continue;
        }
        QString d = lst[i].split(" ")[0];
        QString b = d.split(":")[1];
        int val = Util::NumberFromStringHex("$"+b);
        if (val>codeStart & val <0x4000)
            maxB1=val;
        if (val>varStart & val <0xDFFFF)
            maxV1=val;
        done=++i>=lst.count();

    }
    m_assembler->blocks.append(QSharedPointer<MemoryBlock>(new MemoryBlock(codeStart, maxB1, MemoryBlock::CODE, "code")));
    m_assembler->blocks.append(QSharedPointer<MemoryBlock>(new MemoryBlock(varStart, maxV1, MemoryBlock::DATA, "varialbes")));

}

