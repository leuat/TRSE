#ifndef SYSTEMTIKI100_H
#define SYSTEMTIKI100_H

#include "systemz80.h"

class SystemTiki100 : public SystemZ80
{
public:
    SystemTiki100(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : SystemZ80(settings, proj) {
        m_processor = Z80;
        m_system = TIKI100;

        m_startAddress = 0x0100;
        m_programStartAddress = 0x0100;
    }



    virtual void Assemble(QString& text, QString file, QString currentDir, QSharedPointer<SymbolTable>  symTab) override;
    virtual void PostProcess(QString& text, QString file, QString currentDir) override;
    virtual QString getEmulatorName() override {
        return m_settingsIni->getString("tiki100_emulator");
    }

    void applyEmulatorParameters(QStringList& params, QString debugFile, QString filename, CIniFile* pini) override {
        QString file = QFileInfo(filename+".asm").absolutePath()+QDir::separator()+"disk.dsk";
        params << "-diska"<< file<< "-40x"<< "2"<< "-80x"<< "2";
    }

};

#endif // SYSTEMTIKI100_H
