#ifndef METHODS68000_H
#define METHODS68000_H

#include "source/Compiler/codegen/abstractcodegen.h"
#include "abstractmethods.h"
//#include "CodeGen68k.h"
#include "source/Compiler/ast/nodebuiltinmethod.h"
#include "source/Compiler/ast/nodenumber.h"
#include "source/Compiler/ast/nodeprocedure.h"
#include "source/Compiler/codegen/codegen_m68k.h"

class Methods68000 : public AbstractMethods
{
public:
    Methods68000();

    void Assemble(Assembler* as, AbstractCodeGen* dispenser) override;

    bool Command(QString name) override;
    void Asm(Assembler* as, QString cmd, QString a, QString b, QSharedPointer<Node> n) {
        as->Asm(cmd + (static_cast<CodeGen68k*>(m_codeGen))->getEndType(as,n) + " " + a +","+b);
    }
    void Asm(Assembler* as, QString cmd, QString a, QString b) {
        as->Asm(cmd + " " + a +","+b);
    }

    void LoadVariable(Assembler* as, QString cmd, QSharedPointer<Node> n, QString d0);
//    void LoadAddress(Assembler* as, QSharedPointer<Node> n, QString d0);

    void DrawLine(Assembler*as);
    void Fill(Assembler*as);
    void Poke(Assembler* as, QString bb);
    void SetCopperList32(Assembler* as);
    void Memcpy(Assembler *as);
    void MemcpyUnroll(Assembler *as);
    void Setpalette(Assembler *as);
    void ABlit(Assembler* as, bool isFiller);
    void AddCopperCommand(Assembler* as);
    void SkipCopperCommands(Assembler* as);
    void MatMul(Assembler* as);
    void SetRotation(Assembler* as, QString mat);
    void MatMulVec(Assembler* as);
    void MatMulVecNormalZ(Assembler* as, bool isZonly);
    void ProjectToScreen(Assembler* as);
    void EnableInterrupt(Assembler* as);
    void ProjectAllVertices(Assembler* as);
    void Swap(Assembler* as);
    void Mod(Assembler* as);



};

#endif // METHODS68000_H
