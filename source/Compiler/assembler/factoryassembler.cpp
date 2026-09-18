#include "factoryassembler.h"
#include "asm6502.h"
#include "asm68000.h"
#include "asm6809.h"
#include "asmx86.h"
#include "asmz80.h"
#include "asmchip8.h"
#include "asmjdh8.h"
#include "asmpdp11.h"
#include "asmarm.h"
#include "source/Compiler/errorhandler.h"

FactoryAssembler::FactoryAssembler() {}
/*
enum Processor {
	S1C88,
};
*/
QSharedPointer<Assembler> FactoryAssembler::create(AbstractSystem::Processor p)
{
	if (p==AbstractSystem::MOS6502 || p==AbstractSystem::WDC65C02 || p==AbstractSystem::WDC65C816)
		return QSharedPointer<Asm6502>(new Asm6502());

	if (p==AbstractSystem::M68000)
		return QSharedPointer<Asm68000>(new Asm68000());

	if (p==AbstractSystem::PX86)
		return QSharedPointer<AsmX86>(new AsmX86());

	if (p==AbstractSystem::Z80 || p==AbstractSystem::GBZ80 || p==AbstractSystem::Z180 || p==AbstractSystem::S1C88)
		return QSharedPointer<AsmZ80>(new AsmZ80());

	if (p==AbstractSystem::ARM)
		return QSharedPointer<AsmARM>(new AsmARM());

	if (p==AbstractSystem::PJDH8)
		return QSharedPointer<AsmJDH8>(new AsmJDH8());

	if (p==AbstractSystem::M6809)
		return QSharedPointer<Asm6809>(new Asm6809());

	if (p==AbstractSystem::PDP11)
		return QSharedPointer<AsmPDP11>(new AsmPDP11());

	if (p==AbstractSystem::PCHIP8)
		return QSharedPointer<AsmChip8>(new AsmChip8());

	ErrorHandler::e.Error("::FactoryAssembler: Could not create assembler ",0);
}
