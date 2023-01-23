#include "factorymethods.h"

FactoryMethods::FactoryMethods()
{

}

QSharedPointer<AbstractMethods> FactoryMethods::CreateMethods(AbstractSystem::System s)
{

    if (s==AbstractSystem::VIC20)
        return QSharedPointer<Methods6502Vic20>(new Methods6502Vic20);

    if (s==AbstractSystem::C64)
        return QSharedPointer<Methods6502C64>(new Methods6502C64);

    if (s==AbstractSystem::PLUS4)
        return QSharedPointer<Methods6502>(new Methods6502);

    if (s==AbstractSystem::ORIC)
        return QSharedPointer<Methods6502>(new Methods6502);

    if (s==AbstractSystem::NES)
        return QSharedPointer<Methods6502C64>(new Methods6502C64);

    if (s==AbstractSystem::ATARI2600)
        return QSharedPointer<Methods6502>(new Methods6502);

    if (s==AbstractSystem::OK64)
        return QSharedPointer<Methods6502OK64>(new Methods6502OK64);


    if (s==AbstractSystem::TIM || s==AbstractSystem::VIDEOTON)
        return QSharedPointer<MethodsZ80>(new MethodsZ80);

    if (s==AbstractSystem::X86 ||s==AbstractSystem::WONDERSWAN )
        return QSharedPointer<MethodsX86>(new MethodsX86);

    if (s==AbstractSystem::AMIGA)
        return QSharedPointer<Methods68000Amiga>(new Methods68000Amiga);

    if (s==AbstractSystem::ATARI520ST)
        return QSharedPointer<Methods68000Atari>(new Methods68000Atari);

    if (s==AbstractSystem::GAMEBOY)
        return QSharedPointer<MethodsZ80>(new MethodsZ80);


    if (s==AbstractSystem::MSX)
        return QSharedPointer<MethodsZ80>(new MethodsZ80);

    if (s==AbstractSystem::VZ200)
        return QSharedPointer<MethodsZ80>(new MethodsZ80);

    if (s == AbstractSystem::SPECTRUM ||
            s==AbstractSystem::TIKI100 ||
            s==AbstractSystem::COLECO ||
            s==AbstractSystem::TRS80 ||
            s==AbstractSystem::AMSTRADCPC)
        return QSharedPointer<MethodsZ80>(new MethodsZ80);



    if (s==AbstractSystem::X16)
        return QSharedPointer<Methods6502>(new Methods6502);

    if (s==AbstractSystem::MEGA65)
        return QSharedPointer<Methods6502C64>(new Methods6502C64);

    // Default one
    return QSharedPointer<Methods6502>(new Methods6502);
}
