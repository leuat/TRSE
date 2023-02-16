
#include "factorysystem.h"

FactorySystem::FactorySystem()
{

}

AbstractSystem *FactorySystem::Create(AbstractSystem::System type, QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) {
    AbstractSystem* s = nullptr;

    if (type==AbstractSystem::C64)
        return new SystemC64(settings, proj);
    if (type==AbstractSystem::C128)
        return new SystemC128(settings, proj);
    if (type==AbstractSystem::PET)
        return new SystemPET(settings, proj);
    if (type==AbstractSystem::VIC20)
        return new SystemVIC20(settings, proj);
    if (type==AbstractSystem::NES)
        return new SystemNES(settings, proj);
    if (type==AbstractSystem::AMIGA)
        return new SystemAmiga(settings, proj);
    if (type==AbstractSystem::PLUS4)
        return new SystemPlus4(settings, proj);
    if (type==AbstractSystem::OK64)
        return new SystemOK64(settings, proj);
    if (type==AbstractSystem::X16)
        return new SystemX16(settings, proj);
    if (type==AbstractSystem::X86)
        return new SystemX86(settings, proj);
    if (type==AbstractSystem::GAMEBOY)
        return new SystemGameboy(settings, proj);
    if (type==AbstractSystem::SPECTRUM)
        return new SystemSpectrum(settings, proj);
    if (type==AbstractSystem::TIKI100)
        return new SystemTiki100(settings, proj);
    if (type==AbstractSystem::ATARI2600)
        return new SystemAtari2600(settings, proj);
    if (type==AbstractSystem::ATARI520ST)
        return new SystemAtari520ST(settings, proj);
    if (type==AbstractSystem::AMSTRADCPC)
        return new SystemAmstradCPC(settings, proj);
    if (type==AbstractSystem::COLECO)
        return new SystemColeco(settings, proj);
    if (type==AbstractSystem::MEGA65)
        return new SystemMega65(settings, proj);
    if (type==AbstractSystem::BBCM)
        return new SystemBBCM(settings, proj);
    if (type==AbstractSystem::ATARI800)
        return new SystemAtari800(settings, proj);
    if (type==AbstractSystem::MSX)
        return new SystemMSX(settings, proj);
    if (type==AbstractSystem::APPLEII)
        return new SystemAppleII(settings, proj);
    if (type==AbstractSystem::M1ARM)
        return new SystemM1ARM(settings, proj);
    if (type==AbstractSystem::ORIC)
        return new SystemOric(settings, proj);
    if (type==AbstractSystem::SNES)
        return new SystemSNES(settings, proj);
    if (type==AbstractSystem::CUSTOM)
        return new SystemCustom(settings, proj);
    if (type==AbstractSystem::VZ200)
        return new SystemVZ200(settings, proj);
    if (type==AbstractSystem::ACORN)
        return new SystemAcorn(settings, proj);
    if (type==AbstractSystem::JDH8)
        return new SystemJDH8(settings, proj);
    if (type==AbstractSystem::POKEMONMINI)
        return new SystemPokemonMini(settings, proj);
    if (type==AbstractSystem::TRS80)
        return new SystemTRS80(settings, proj);
    if (type==AbstractSystem::TRS80COCO)
        return new SystemTRS80CoCo(settings, proj);
    if (type==AbstractSystem::WONDERSWAN)
        return new SystemWonderswan(settings, proj);
    if (type==AbstractSystem::TIM)
        return new SystemTIM(settings, proj);
    if (type==AbstractSystem::TVC)
        return new SystemTVC(settings, proj);
    if (type==AbstractSystem::VECTREX)
        return new SystemVectrex(settings, proj);
    if (type==AbstractSystem::THOMSON)
        return new SystemThomson(settings, proj);
    if (type==AbstractSystem::CHIP8)
        return new SystemChip8(settings, proj);


    return s;
}
