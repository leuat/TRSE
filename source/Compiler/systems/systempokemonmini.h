#ifndef SYSTEMPOKEMONMINI_H
#define SYSTEMPOKEMONMINI_H

#include "systemz80.h"

class SystemPokemonMini : public SystemZ80
{
public:
    SystemPokemonMini(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);





    virtual void Assemble(QString& text, QString file, QString currentDir, QSharedPointer<SymbolTable>  symTab) override;
    virtual void PostProcess(QString& text, QString file, QString currentDir) override;

    virtual QString getEmulatorName() override {
        return m_settingsIni->getString("pokemonmini_emulator");
    }
    void applyEmulatorParameters(QStringList& params, QString debugFile, QString filename, CIniFile* pini) override;
    virtual int getCPUFlavorint() override {
        return 4; // s1c88
    }

};
#endif // SYSTEMPOKEMONMINI_H
