#ifndef METHODS6502_H
#define METHODS6502_H

#include "abstractastdispatcher.h"
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

class Methods6502
{
public:
    Methods6502();
    AbstractASTDispatcher* m_dispatcher;
    NodeBuiltinMethod* m_node = nullptr;

    void Assemble(Assembler* as, AbstractASTDispatcher* dispenser);


    bool Command(QString name);

    void PointToVera(Assembler* as, int address, bool);
    void toColor(Assembler* as);
    void AddMemoryBlock(Assembler* as, int param);
    void Poke(Assembler* as);
    void Peek(Assembler* as);
    void Modulo(Assembler* as);
    void MemCpy(Assembler* as, bool isFast);
    void MemCpyUnroll(Assembler* as);
    void Rand(Assembler* as);
    void Random(Assembler* as);

    void CallOKVC(Assembler* as, int noParams, uchar val);
    void WaitForVerticalBlank(Assembler* as);
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
    void PlayVIC20Sid(Assembler* as);
    void PrintNumber(Assembler* as);
    void PrintString(Assembler* as);
    void ScrollX(Assembler* as);
    void VeraPoke(Assembler* as, bool isExtended);
    void ScrollY(Assembler* as);
    void InitVeraSprite(Assembler* as);
    void CopyDataToVera(Assembler* as);
    void SetColor(Assembler* as);
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

    void LoHi(Assembler* as, bool isLo);

    void LoadAndStoreInZp(Node* n, Assembler* as, QString zp);

    void BcdAdd(Assembler* as);
    void BcdSub(Assembler* as);
    void BcdCompare(Assembler* as);
    void BcdIsEqual(Assembler* as);
    void BcdPrint(Assembler* as);



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
    void VIARasterIRQ(Assembler* as);
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

    void Jammer(Assembler *as);
    void FLD(Assembler* as);


    // Vic20 Bitmap Mode
    void initVbm(Assembler* as);
    void vbmSetDisplayMode(Assembler* as);
    void vbmResetDisplayMode(Assembler* as);
    void vbmDebug(Assembler* as);
    void vbmSetColumn(Assembler *as);
    void initVbmNextColumn(Assembler *as);
    void vbmNextColumn(Assembler *as);
    void initvbmSetPosition(Assembler *as);
    void vbmSetPosition1(Assembler *as);
    void vbmSetPosition2(Assembler *as);
    void vbmSetPosition4(Assembler *as);
    void initVbmClear(Assembler* as);
    void vbmClear(Assembler* as);
    void vbmClearColor(Assembler* as);

    void vbmDrawTile(Assembler *as);
    void vbmDrawTileO(Assembler *as);
    void vbmDrawTileE(Assembler *as);
    void vbmClearTile(Assembler *as);
    void vbmClearBlock(Assembler *as);

    void initVbmDrawTileMap(Assembler *as);
    void vbmDrawTileMap(Assembler *as);
    void initVbmDrawTileMapO(Assembler *as);
    void vbmDrawTileMapO(Assembler *as);
    void initVbmDrawTileMapE(Assembler *as);
    void vbmDrawTileMapE(Assembler *as);
    void initVbmClearTileMap(Assembler *as);
    void vbmClearTileMap(Assembler *as);

    void initVbmTestTilePixel(Assembler *as);
    void initVbmTestTilePixel2(Assembler *as);
    void vbmTestTilePixel(Assembler *as);
    void vbmTestTilePixel2(Assembler *as);

    void initVbmDot(Assembler* as);
    void initVbmBlot(Assembler* as);
    void vbmTestPixel(Assembler *as);
    void vbmTestPixel2(Assembler *as);
    void vbmDrawDot(Assembler *as);
    void vbmClearDot(Assembler *as);
    void vbmDrawDotE(Assembler *as);
    void vbmDrawBlot(Assembler *as);
    void vbmClearBlot(Assembler *as);
    void vbmDrawBlotE(Assembler *as);

    void initVbmScrollLeft(Assembler *as);
    void initVbmScrollRight(Assembler *as);
    void initVbmScrollFixTop(Assembler *as);
    void initVbmScrollFixBottom(Assembler *as);
    void vbmScrollLeft(Assembler *as);
    void vbmScrollRight(Assembler *as);
    void vbmScrollFixTop(Assembler *as);
    void vbmScrollFixBottom(Assembler *as);

    void initVbmScreenShiftLeft(Assembler *as);
    void initVbmScreenShiftRight(Assembler *as);
    void vbmScreenShiftLeft(Assembler *as);
    void vbmScreenShiftRight(Assembler *as);

    void initVbmSpriteStitch(Assembler* as);
    void vbmSpriteStitch(Assembler* as);
    void initVbmSpriteShiftR(Assembler* as);
    void vbmSpriteShiftR(Assembler* as);
    void initVbmSpriteShiftL(Assembler* as);
    void vbmSpriteShiftL(Assembler* as);

    void initVbmSpriteShiftSR(Assembler* as);
    void vbmSpriteShiftSR(Assembler* as);
    void initVbmSpriteShiftSL(Assembler* as);
    void vbmSpriteShiftSL(Assembler* as);

    void initVbmDrawSprite(Assembler *as);
    void vbmDrawSprite(Assembler *as);
    void initVbmDrawSpriteE(Assembler *as);
    void vbmDrawSpriteE(Assembler *as);
    void initVbmClearSprite(Assembler *as);
    void vbmClearSprite(Assembler *as);

    void initVbmDrawSprite8(Assembler *as);
    void vbmDrawSprite8(Assembler *as);
    void initVbmDrawSprite8E(Assembler *as);
    void vbmDrawSprite8E(Assembler *as);
    void initVbmClearSprite8(Assembler *as);
    void vbmClearSprite8(Assembler *as);

    void initVbmDrawSprite16(Assembler *as);
    void vbmDrawSprite16(Assembler *as);
    void initVbmDrawSprite16E(Assembler *as);
    void vbmDrawSprite16E(Assembler *as);
    void initVbmClearSprite16(Assembler *as);
    void vbmClearSprite16(Assembler *as);

    void initVbmDrawSpriteSlice(Assembler *as);
    void vbmDrawSpriteSlice(Assembler *as);
    void initVbmDrawSpriteSliceE(Assembler *as);
    void vbmDrawSpriteSliceE(Assembler *as);
    void initVbmClearSpriteSlice(Assembler *as);
    void vbmClearSpriteSlice(Assembler *as);

    void initVbmDrawText(Assembler *as);
    void vbmDrawText(Assembler *as);
    void initVbmDrawTextO(Assembler *as);
    void vbmDrawTextO(Assembler *as);
    void initVbmDrawTextE(Assembler *as);
    void vbmDrawTextE(Assembler *as);
    void initVbmClearText(Assembler *as);
    void vbmClearText(Assembler *as);

    void initVbmDrawSmallTextO(Assembler *as);
    void vbmDrawSmallTextO(Assembler *as);
    void initVbmDrawSmallTextE(Assembler *as);
    void vbmDrawSmallTextE(Assembler *as);
    void initVbmClearSmallText(Assembler *as);
    void vbmClearSmallText(Assembler *as);

    void initVbmDrawBCD(Assembler *as);
    void vbmDrawBCD(Assembler *as);

    void initVbmCopyToBuffer(Assembler *as);
    void vbmCopyToBuffer(Assembler *as);
    void initVbmCopyFromBuffer(Assembler *as);
    void vbmCopyFromBuffer(Assembler *as);




    // Initialize

    void InitJoystick(Assembler* as);
    void InitJoy(Assembler* as);
    void InitJoy1(Assembler* as);
    void InitJoy2(Assembler* as);
    void InitRandom(Assembler* as);
    void InitRandom256(Assembler* as);

    void InitSinusTable(Assembler* as);
    void DefineScreen(Assembler *as);
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
