#ifndef SYSTEMX86_H
#define SYSTEMX86_H


#include "abstractsystem.h"
#include <QProcess>
#include "source/LeLib/util/util.h"

class SystemX86 : public AbstractSystem
{
public:
    SystemX86(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : AbstractSystem(settings, proj) {
        m_processor = PX86;
        m_system = X86;
        m_startAddress = 0x100;
        m_programStartAddress = m_startAddress;
    }

    virtual void Assemble(QString& text, QString file, QString currentDir, QSharedPointer<SymbolTable>  symTab);
    virtual void PostProcess(QString& text, QString file, QString currentDir);
};


#endif // SYSTEMX86_H
