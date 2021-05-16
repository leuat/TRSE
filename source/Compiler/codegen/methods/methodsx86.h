#ifndef METHODSX86_H
#define METHODSX86_H

//#include "CodeGen68k.h"
#include "source/Compiler/ast/nodebuiltinmethod.h"
#include "source/Compiler/ast/nodenumber.h"
#include "source/Compiler/ast/nodeprocedure.h"
#include "source/Compiler/codegen/methods/abstractmethods.h"
#include "source/Compiler/codegen/abstractcodegen.h"
#include "source/Compiler/codegen/codegen_x86.h"

class MethodsX86 : public AbstractMethods
{
public:
    MethodsX86();


    void Assemble(Assembler* as, AbstractCodeGen* dispenser) override;

    void PushPopAll(Assembler* as, bool isPush);


    bool Command(QString name) override;

    void LoadVar(Assembler *as, int paramNo);
    void LoadAddress(Assembler *as, int paramNo, bool isSource);

    void ClearScreen(Assembler* as, int bits);

    void WaitForVerticalBlank(Assembler* as);

    void ScrollX(Assembler* as);

    void AddInitMethod(Assembler* as, QString name, QString file);

    void MemCpy(Assembler* as, QString type);

    void InitPlaynote(Assembler* as);

//    void Poke(Assembler* as);

};

#endif // METHODSX86_H
