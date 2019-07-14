#ifndef METHODS6502_H
#define METHODS6502_H

#include "abstractastdispatcher.h"
#include "source/Compiler/assembler/assembler.h"
#include "source/Compiler/ast/nodenumber.h"
#include "source/Compiler/ast/nodevar.h"
#include "source/Compiler/ast/nodebuiltinmethod.h"
#include "source/Compiler/ast/nodestring.h"
#include "source/Compiler/ast/nodeprocedure.h"

class Methods6502
{
public:
    Methods6502();
    AbstractASTDispatcher* m_dispatcher;
    NodeBuiltinMethod* m_node = nullptr;

    void Assemble(Assembler* as, AbstractASTDispatcher* dispenser);


    bool Command(QString name);

    void AddMemoryBlock(Assembler* as, int param);
    void Poke(Assembler* as);
    void Peek(Assembler* as);
    void MemCpy(Assembler* as, bool isFast);
    void MemCpyUnroll(Assembler* as);
    void Rand(Assembler* as);
    void MoveTo(Assembler* as);
    void MoveTo80(Assembler* as);
    void SetFrequency(Assembler *as);


    void CopyBytesShift(Assembler* as, bool isLeft);
    void PokeScreen(Assembler* as, int hiAddress);
    void PokeScreenColor(Assembler* as, int hiAddress);
    void SetSpritePos(Assembler* as);
    void Fill(Assembler* as);
    void PlayVIC20Sid(Assembler* as);
    void PrintNumber(Assembler* as);
    void PrintString(Assembler* as);
    void ScrollX(Assembler* as);
    void ScrollY(Assembler* as);
    void RightBitShift(Assembler* as, bool isRight);
    void Joystick(Assembler* as);
    void BitOp(Assembler* as, int type);
    void PlaySound(Assembler* as);
    void CreateInteger(Assembler* as, QString reg);
    void Tile(Assembler *as);

    void LoHi(Assembler* as, bool isLo);

    void LoadAndStoreInZp(Node* n, Assembler* as, QString zp);

    /*
     * NES
     * */
    void LoadPalette(Assembler* as);
    void ReadInput(Assembler* as);
    void PPUDump(Assembler* as, int hi, int lo,int x, int y);
    void PPUSingle(Assembler* as);
    void LoadSong(Assembler *as);
    void PPUDrawColumn(Assembler* as);
    void Clearsound(Assembler* as);
    void SetCharsetLocation(Assembler* as);
    void SetScreenLocation(Assembler* as);
    void IncScreenX(Assembler* as);
    void Print80(Assembler* as);
    void Fill80(Assembler* as);
    void KrillLoad(Assembler* as, bool isCompressed);
    void InitKrill(Assembler* as);
    void IncZp(Assembler* as);
    void DecZp(Assembler* as);
    void Call(Assembler* as);
    void ToggleRegisterBit(Assembler* as, QString addr, int bit, bool regular=true);
    void InitSid(Assembler* as);
    void InitMul16x8(Assembler* as);
    void InitDiv8x8(Assembler* as);
    void InitDiv16x8(Assembler* as);
    void InitSqrt16(Assembler* as);
    void InitAtan2(Assembler* as);
    void InitModPlayer(Assembler* as);
    void IncludeModPlayer(Assembler* as);
    void DisableInterrupts(Assembler* as);
//    void EnableInterrupts(Assembler* as);
    void RasterIRQ(Assembler* as);
    void VIAIRQ(Assembler* as);
    void InitVIAIRQ(Assembler* as);
    void RasterIRQWedge(Assembler* as);
    void ClearScreen(Assembler* as);
    void WaitForRaster(Assembler* as);
    void WaitNoRasterLines(Assembler* as);
    void SetSpriteLoc(Assembler* as);
    void ClearBitmap(Assembler* as);
    void Swap(Assembler* as);
    void PrintDecimal(Assembler* as);
    void MemCpyLarge(Assembler* as);
    void SetBank(Assembler* as);
    void Decrunch(Assembler* as);
    void DecrunchFromIndex(Assembler* as);
    void CopyImageColorData(Assembler* as);
    void CopyHalfScreen(Assembler* as);
    void BlockMemCpy(Assembler* as);
    void CopyFullScreenUnrolled(Assembler* as);
    void CopyFullScreen(Assembler* as);
    void TransformColors(Assembler* as);
    void ToggleBit(Assembler* as);
    void GetBit(Assembler* as);
    void CopyZPdata(Assembler* as);
    void InitZeroPage(Assembler* as);
    void Abs(Assembler* as);
    void Sqrt(Assembler* as);
    void Atan2(Assembler* as);
    void IsOverlapping(Assembler* as);
    void IsOverlappingWH(Assembler* as);
    void CopyCharsetFromRom(Assembler* as);
    void IncDec(Assembler* as, QString cmd);
    void SetMemoryConfig(Assembler* as);
    void EnableRasterIRQ(Assembler* as);
    void StartIRQ(Assembler* as);
    void StartIRQWedge(Assembler* as);
    void CloseIRQ(Assembler* as, bool isWedge);
    void DisableNMI(Assembler* as);
    void KeyPressed(Assembler* as);
    void Wait(Assembler* as);



    QString BitShiftX(Assembler* as);

    void LoadVar(Assembler* as, int paramNo, QString registe, QString load="");
    void LoadVar(Assembler* as, int paramNo);
    void SaveVar(Assembler* as, int paramNo, QString registe, QString load="");
    void SaveVar(Assembler* as, int paramNo);

    void VerifyInitialized(QString method, QString initmethod);

    void VDCWrite(Assembler* as);
    void VDCInit(Assembler* as);

    void Jammer(Assembler *as);
    void FLD(Assembler* as);

    // Initialize

    void InitJoystick(Assembler* as);
    void InitRandom(Assembler* as);
    void InitSinusTable(Assembler* as);
    void InitMoveto(Assembler *as);
    void InitMoveto80(Assembler *as);
    void InitEightBitMul(Assembler* as);
    void Init8x8MulNes(Assembler* as);
    void InitPrintString(Assembler* as);
    void InitDecrunch(Assembler* as);
    void InitPrintDecimal(Assembler* as);
    void InitPlaySidVIC20(Assembler* as);
};

#endif // METHODS6502_H
