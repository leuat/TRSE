#ifndef NODEBUILTINMETHOD_H
#define NODEBUILTINMETHOD_H


#include "source/Compiler/token.h"
#include "source/Compiler/pvar.h"
#include "source/Compiler/symboltable.h"
#include "source/Compiler/errorhandler.h"
#include "source/Compiler/ast/node.h"

class NodeBuiltinMethod : public Node {
public:
    QString m_procName;
    QVector<Node*> m_params;
    static QMap<QString, bool> m_isInitialized;

    NodeBuiltinMethod(QString m, QVector<Node*> params):Node() {
        m_procName = m;
        m_params = params;
        m_op.m_type = TokenType::BYTE;
    }

    void Delete() override {
        for (Node* n: m_params) {
            n->Delete();
            delete n;
        }
    }

    QString Build(Assembler *as) override;
    void Poke(Assembler* as);
    void Peek(Assembler* as);
    void MemCpy(Assembler* as);
    void MemCpyUnroll(Assembler* as);
    void Rand(Assembler* as);
    void MoveTo(Assembler* as);
    void PokeScreen(Assembler* as, int hiAddress);
    void PokeScreenColor(Assembler* as, int hiAddress);
    void SetSpritePos(Assembler* as);
    void Fill(Assembler* as);
    void PrintNumber(Assembler* as);
    void PrintString(Assembler* as);
    void ScrollX(Assembler* as);
    void ScrollY(Assembler* as);
    void Joystick(Assembler* as);
    void BitOp(Assembler* as, int type);
    PVar Execute(SymbolTable* symTab, uint lvl) override;
    void PlaySound(Assembler* as);
    void Clearsound(Assembler* as);
    void SetCharsetLocation(Assembler* as);
    void IncScreenX(Assembler* as);
    void IncZp(Assembler* as);
    void DecZp(Assembler* as);
    void Call(Assembler* as);
    void InitSid(Assembler* as);
    void InitMul16x8(Assembler* as);
    void InitDiv8x8(Assembler* as);
    void InitDiv16x8(Assembler* as);
    void DisableInterrupts(Assembler* as);
//    void EnableInterrupts(Assembler* as);
    void RasterIRQ(Assembler* as);
    void ClearScreen(Assembler* as);
    void WaitForRaster(Assembler* as);
    void WaitNoRasterLines(Assembler* as);
    void SetSpriteLoc(Assembler* as);
    void ClearBitmap(Assembler* as);
    void Swap(Assembler* as);
    void MemCpyLarge(Assembler* as);
    void SetBank(Assembler* as);
    void Decrunch(Assembler* as);
    void CopyImageColorData(Assembler* as);
    void CopyHalfScreen(Assembler* as);
    void CopyFullScreenUnrolled(Assembler* as);
    void CopyFullScreen(Assembler* as);
    void TransformColors(Assembler* as);
    void ToggleBit(Assembler* as);
    void GetBit(Assembler* as);
    void CopyZPdata(Assembler* as);
    void InitZeroPage(Assembler* as);
    void Abs(Assembler* as);
    void CopyCharsetFromRom(Assembler* as);
    void IncDec(Assembler* as, QString cmd);
    void SetMemoryConfig(Assembler* as);
    void EnableRasterIRQ(Assembler* as);
    void StartIRQ(Assembler* as);
    void CloseIRQ(Assembler* as);
    void DisableNMI(Assembler* as);
    void KeyPressed(Assembler* as);
    void Wait(Assembler* as);



    QString BitShiftX(Assembler* as);

    void LoadVar(Assembler* as, int paramNo, QString register, QString load="");
    void LoadVar(Assembler* as, int paramNo);
    void SaveVar(Assembler* as, int paramNo, QString register, QString load="");
    void SaveVar(Assembler* as, int paramNo);

    void VerifyInitialized(QString method, QString initmethod);



    void Jammer(Assembler *as);
    void FLD(Assembler* as);

    // Initialize

    void InitJoystick(Assembler* as);
    void InitRandom(Assembler* as);
    void InitSinusTable(Assembler* as);
    void InitMoveto(Assembler *as);
    void InitEightBitMul(Assembler* as);
    void InitPrintString(Assembler* as);
    void InitDecrunch(Assembler* as);



    void ExecuteSym(SymbolTable* symTab) override {

    }
};


#endif // NODEBUILTINMETHOD_H
