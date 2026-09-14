#include "factorymethods.h"

#include "methods6502.h"
#include "methods6502c64.h"
#include "methods6502ok64.h"
#include "methods6502vic20.h"
#include "methods68000.h"
#include "methods68000atari.h"
#include "methods6800amiga.h"
#include "methods6809.h"
#include "methodsx86.h"
#include "methodsz80.h"
#include "methodstripe.h"

bool FactoryMethods::s_useTripe = false;

FactoryMethods::FactoryMethods()
{

}

QSharedPointer<AbstractMethods> FactoryMethods::CreateMethods(AbstractSystem::System s)
{

//	if (s_useTripe)
	//	return QSharedPointer<MethodsTripe>(new MethodsTripe);

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


    if (s==AbstractSystem::TIM || s==AbstractSystem::TVC || s==AbstractSystem::PRIMO)
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
            s==AbstractSystem::PCW ||
            s==AbstractSystem::AMSTRADCPC ||
            s==AbstractSystem::AGON)
        return QSharedPointer<MethodsZ80>(new MethodsZ80);



    if (s==AbstractSystem::X16)
        return QSharedPointer<Methods6502>(new Methods6502);

    if (s==AbstractSystem::MEGA65)
        return QSharedPointer<Methods6502C64>(new Methods6502C64);

    if (s==AbstractSystem::TRS80COCO || s==AbstractSystem::THOMSON || s==AbstractSystem::VECTREX || s==AbstractSystem::DRAGON)
        return QSharedPointer<Methods6809>(new Methods6809);

    // Default one
    return QSharedPointer<Methods6502>(new Methods6502);
}
