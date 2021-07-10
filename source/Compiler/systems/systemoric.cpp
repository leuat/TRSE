#include "systemoric.h"


SystemOric::SystemOric(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : SystemMOS6502(settings, proj) {
    m_processor = MOS6502;
    m_system = ORIC;
    DefaultValues();

    m_systemColor = QColor(80,80,80);

}

void SystemOric::DefaultValues()
{
    m_startAddress = getDefaultBasicAddress();
    m_programStartAddress = 0x0501;
    m_ignoreSys = true;
    m_stripPrg = true;
}

void SystemOric::Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab)
{
    // Overriding assembling
    SystemMOS6502::Assemble(text,filename,currentDir,symTab);
    QByteArray ba = Util::loadBinaryFile(filename+".prg");
    QByteArray header;
    // Append TAPE header https://wiki.defence-force.org/doku.php?id=oric:hardware:tape_encoding
    // Sync bytes
    header.append(0x16);
    header.append(0x16);
    header.append(0x16);
    header.append(0x24); // End of stream

    header.append((char)0x49); // reserved bytes??
    header.append((char)0x90);

    header.append((char)0x80); // Machine code
    header.append((char)0xC7); // Run as machine code

    int start = m_programStartAddress;
    int end = m_programStartAddress+ba.length()-1;

    header.append((end>>8)&0xff); // end BIG endian
    header.append((end)&0xff); // end BIG endian
    header.append((start>>8)&0xff); // end BIG endian
    header.append((start)&0xff); // end BIG endian

    header.append((char)0xE8);
    header.append((char)0x41);

    header.append((char)0x00);
    header.append((char)0x00);

    header.append(ba);
    Util::SaveByteArray(header,filename+".prg");

}

