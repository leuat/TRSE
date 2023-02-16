#ifndef METHODS6809_H
#define METHODS6809_H

#include "source/Compiler/codegen/abstractcodegen.h"
#include "source/Compiler/codegen/methods/abstractmethods.h"
#include "source/Compiler/assembler/assembler.h"
#include "source/Compiler/ast/nodenumber.h"
#include "source/Compiler/ast/nodevar.h"
#include "source/Compiler/ast/nodebuiltinmethod.h"
#include "source/Compiler/ast/nodestring.h"
#include "source/Compiler/ast/nodeprocedure.h"
#include "source/Compiler/ast/nodeconditional.h"
#include "source/Compiler/ast/nodeassign.h"
#include "source/Compiler/ast/nodeconditional.h"
#include "source/Compiler/ast/nodeblock.h"
#include "source/Compiler/ast/nodebinaryclause.h"
#include "source/Compiler/ast/nodevar.h"
#include "source/Compiler/ast/nodenumber.h"


class Methods6809 : public AbstractMethods
{
public:
    Methods6809();

    void Assemble(Assembler* as, AbstractCodeGen* dispenser) override;
    bool m_initDiv16x8_initialised = false;

    bool Command(QString name) override;

    void Poke(Assembler* as);
    void Peek(Assembler* as);
    void Modulo(Assembler* as);
    void Modulo16(Assembler* as);
    void MemCpy(Assembler* as, bool isFast);
    void MemCpyUnroll(Assembler* as, bool isReverse);


    QString checkAndInitStringParameter(Assembler* as, int n);

    void ToggleRegisterBit(Assembler* as, QString addr, int bit, bool regular=true);



    // Initialize

    void InitJoystick(Assembler* as);
    void InitJoy(Assembler* as);
    void InitJoy1(Assembler* as);
    void InitJoy2(Assembler* as);
    void InitRandom(Assembler* as);
    void InitRandom256(Assembler* as);

    void InitSinusTable(Assembler* as);
    void DefineScreen(Assembler *as);
    void SetColorMemoryAddress(Assembler *as);
    void InitMoveto(Assembler *as);
    void InitMoveto80(Assembler *as);
    void InitEightBitMul(Assembler* as);
    void Init8x8MulNes(Assembler* as);
    void InitPrintString(Assembler* as);
    void InitDecrunch(Assembler* as);
    void InitPrintDecimal(Assembler* as);
    void InitPlaySidVIC20(Assembler* as);
    void InitBcd(Assembler* as);
    void LoHi(Assembler*as, bool isHi);
    void MemCpyUnroll(Assembler* as);

    void LoadAddress(Assembler* as, int paramNo);
    void LoadVar(Assembler* as, int paramNo, QString registe, QString load="");
    void LoadVar(Assembler* as, int paramNo);
    void SaveVar(Assembler* as, int paramNo, QString registe, QString load="");
    void SaveVar(Assembler* as, int paramNo);

};

#endif // METHODS6809_H
