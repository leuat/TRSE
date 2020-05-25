#ifndef SYSTEMATARI520ST_H
#define SYSTEMATARI520ST_H

#include "systemm6800.h"

class SystemAtari520ST : public SystemM68000
{
public:
    SystemAtari520ST(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : SystemM68000(settings, proj) {
        m_processor = M68000;
        m_system = ATARI520ST;
    }

    void Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable>  symTab) override;

    void PostProcess(QString &text, QString file, QString currentDir);

};

#endif // SYSTEMATARI520ST_H
