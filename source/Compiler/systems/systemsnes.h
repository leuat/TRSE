#ifndef SYSTEMSNES_H
#define SYSTEMSNES_H

#include "system65c816.h"

class SystemSNES : public System65C816
{
public:
    SystemSNES(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : System65C816(settings, proj)   {
        m_system = AbstractSystem::SNES;
        DefaultValues();
    }
    void DefaultValues() override;

/*    int getDefaultBasicAddress() override {
        return 0x801;
    };
    */
    bool isCommodoreSystem() override  {return false;}


};


#endif // SYSTEMSNES_H
