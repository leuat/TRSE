#ifndef METHODSTRIPE_H
#define METHODSTRIPE_H

#include "abstractmethods.h"

class MethodsTripe : public AbstractMethods {
  public:
	MethodsTripe();

	void Assemble(Assembler *as, AbstractCodeGen *dispatcher) override;

	bool Command(QString name) override;

};

#endif // METHODSTRIPE_H
