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

    if (s==AbstractSystem::OK64)
        return QSharedPointer<Methods6502OK64>(new Methods6502OK64);


    if (s==AbstractSystem::X86)
        return QSharedPointer<MethodsX86>(new MethodsX86);

    if (s==AbstractSystem::AMIGA)
        return QSharedPointer<Methods68000>(new Methods68000);

    if (s==AbstractSystem::GAMEBOY)
        return QSharedPointer<MethodsZ80>(new MethodsZ80);

    // Default one
    return QSharedPointer<Methods6502>(new Methods6502);
}
