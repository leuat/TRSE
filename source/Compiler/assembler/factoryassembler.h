#ifndef FACTORYASSEMBLER_H
#define FACTORYASSEMBLER_H

#include "assembler.h"
#include "source/Compiler/systems/abstractsystem.h"

class FactoryAssembler {
  public:
	FactoryAssembler();


	static QSharedPointer<Assembler> create(AbstractSystem::Processor p);
};

#endif // FACTORYASSEMBLER_H
