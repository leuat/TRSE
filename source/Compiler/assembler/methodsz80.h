#ifndef METHODSZ80_H
#define METHODSZ80_H

#include "abstractastdispatcher.h"
//#include "dispatcherz80.h"
//#include "astdispatcher68000.h"
#include "source/Compiler/ast/nodebuiltinmethod.h"
#include "source/Compiler/ast/nodenumber.h"
#include "source/Compiler/ast/nodeprocedure.h"
#include "source/Compiler/assembler/abstractmethods.h"

class MethodsZ80 : public AbstractMethods
{
public:
    MethodsZ80();


    void Assemble(Assembler* as, AbstractASTDispatcher* dispenser) override;




    bool Command(QString name) override;

    void LoadVar(Assembler *as, int paramNo, QString s="");
    void LoadAddress(Assembler *as, int paramNo, QString reg = "hl");

    void Fill(Assembler* as);
    void MemCpy(Assembler* as, bool isCont);
    void Poke(Assembler* as);
    void SetSprite(Assembler* as, int type);
    void InitSpriteFromData(Assembler *as, int type);
    void LoadSong(Assembler* as);
    void InitDMA(Assembler* as);
    void InitTimer(Assembler* as);
    void MemCpyOnHBLank(Assembler* as,QString lbl, int div);
    void WaitForHBLank(Assembler* as);
    void HiLo(Assembler* as, bool isHi);
};

#endif // METHODSX86_H
