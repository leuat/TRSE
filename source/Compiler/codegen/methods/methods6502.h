#ifndef METHODS6502_H
#define METHODS6502_H


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

class Methods6502 : public AbstractMethods
{
public:
    Methods6502();

    void Assemble(Assembler* as, AbstractCodeGen* dispenser) override;
    bool m_initDiv16x8_initialised = false;

    bool Command(QString name) override;

    void PointToVera(Assembler* as, int address, bool);
    void toColor(Assembler* as);
    void AddMemoryBlock(Assembler* as, int param);
    void Poke(Assembler* as);
    void Peek(Assembler* as);
    void Modulo(Assembler* as);
    void Modulo16(Assembler* as);
    void MemCpy(Assembler* as, bool isFast);
    void MemCpyUnroll(Assembler* as, bool isReverse);
    void Rand(Assembler* as);
    void Random(Assembler* as);

    bool is6502() {
        return Syntax::s.m_currentSystem->m_processor == AbstractSystem::MOS6502;
    }

    void MoveTo(Assembler* as);
    void MoveTo80(Assembler* as);
    void SetFrequency(Assembler *as);
    void StrToLower(Assembler* as, bool);
    void StrCmp(Assembler* as);
    void StrSplit(Assembler* as);
    void StrGetFromIndex(Assembler* as);
    void CopyBytesShift(Assembler* as, bool isLeft);
    void PokeScreen(Assembler* as, int hiAddress);
    void PokeScreenColor(Assembler* as, int hiAddress);
    void SetSpritePos(Assembler* as);
    void Fill(Assembler* as);
    void FillFast(Assembler* as);
    void PrintNumber(Assembler* as);
    void PrintString(Assembler* as);
    void ScrollX(Assembler* as);
    void VeraPoke(Assembler* as, bool isExtended);
    void ScrollY(Assembler* as);
    void CopyDataToVera(Assembler* as);
    void SetVideoMode(Assembler* as);

    QString checkAndInitStringParameter(Assembler* as, int n);

    void RightBitShift(Assembler* as, bool isRight);
    void Joystick(Assembler* as);
    void ReadJoy1(Assembler* as);
    void ReadJoy2(Assembler* as);
    void BitOp(Assembler* as, int type);
    void PlaySound(Assembler* as);
    void CreateInteger(Assembler* as, QString reg);
    void Tile(Assembler *as);
    void CreateAddressTable(Assembler *as);
    void AddressTable(Assembler *as);
    void ToPointer(Assembler* as);

    void LoHi(Assembler* as, int type);

    void LoadAndStoreInZp(QSharedPointer<Node> n, Assembler* as, QString zp);

    void BcdAdd(Assembler* as);
    void BcdSub(Assembler* as);
    void BcdCompare(Assembler* as);
    void BcdIsEqual(Assembler* as);
    void BcdPrint(Assembler* as);

    void MinMax(Assembler*as, bool isMin);


    /*
     * NES
     * */
    void LoadPalette(Assembler* as);
    void ReadInput(Assembler* as);
    void PPUDump(Assembler* as, int hi, int lo,int x, int y);
    void PPUSingle(Assembler* as, int type);
    void PPURead(Assembler* as);
    void LoadSong(Assembler *as);
    void PPUDrawColumn(Assembler* as);
    void Clearsound(Assembler* as);
    void SetBitmapLocation(Assembler *as);

    void SetCharsetLocation(Assembler* as);
    void SetCharsetAndScreenLocation(Assembler* as);
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
    void RasterIRQWedge(Assembler* as);
    void ClearScreen(Assembler* as);
    void WaitForRaster(Assembler* as);
    void WaitNoRasterLines(Assembler* as);
    void WaitForVerticalBlank(Assembler *as);


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
    void IncMax(Assembler* as, QString cmd);
    void SetMemoryConfig(Assembler* as);
    void EnableRasterIRQ(Assembler* as);
    void StartIRQ(Assembler* as);
    void StartIRQWedge(Assembler* as);
    void CloseIRQ(Assembler* as, bool isWedge);
    void DisableNMI(Assembler* as);
    void KeyPressed(Assembler* as);
    void Wait(Assembler* as);
    void InitDrawTextBox(Assembler* as);
    void DrawTextBox(Assembler* as);
    void DrawColorTextBox(Assembler* as);


    QString BitShiftX(Assembler* as);

    void LoadAddress(Assembler* as, int paramNo);
    void LoadVar(Assembler* as, int paramNo, QString registe, QString load="");
    void LoadVar(Assembler* as, int paramNo);
    void SaveVar(Assembler* as, int paramNo, QString registe, QString load="");
    void SaveVar(Assembler* as, int paramNo);

    void VerifyInitialized(QString method, QString initmethod);

    void VDCWrite(Assembler* as);
    void VDCInit(Assembler* as);
    void Muls8x8(Assembler* as);






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

};

#endif // METHODS6502_H