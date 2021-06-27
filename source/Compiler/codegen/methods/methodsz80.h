#ifndef METHODSZ80_H
#define METHODSZ80_H

#include "source/Compiler/codegen/abstractcodegen.h"
#include "source/Compiler/codegen/methods/abstractmethods.h"
//#include "dispatcherz80.h"
//#include "CodeGen68k.h"
#include "source/Compiler/ast/nodebuiltinmethod.h"
#include "source/Compiler/ast/nodenumber.h"
#include "source/Compiler/ast/nodeprocedure.h"

class MethodsZ80 : public AbstractMethods
{
public:
    MethodsZ80();


    void Assemble(Assembler* as, AbstractCodeGen* dispenser) override;




    bool Command(QString name) override;

    void LoadVar(Assembler *as, int paramNo, QString s="");
    void LoadAddress(Assembler *as, int paramNo, QString reg = "hl");

    void Fill(Assembler* as);
    void Mod(Assembler* as);
    void MemCpy(Assembler* as, bool isCont);
    void Poke(Assembler* as);
    void Call(Assembler* as);
    void SetSprite(Assembler* as, int type);
    void InitSpriteFromData(Assembler *as, int type);
    void LoadSong(Assembler* as);
    void InitDMA(Assembler* as);
    void InitTimer(Assembler* as);
    void MemCpyOnHBLank(Assembler* as,QString lbl, int div);
    void WaitForHBLank(Assembler* as);
    void HiLo(Assembler* as, bool isHi);
    void Joypad(Assembler* as);
};

#endif // METHODSX86_H
