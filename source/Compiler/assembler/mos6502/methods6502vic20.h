#ifndef METHODS6502VIC20_H
#define METHODS6502VIC20_H

#include "methods6502.h"

class Methods6502Vic20 : public Methods6502
{
public:
    Methods6502Vic20();

    void Assemble(Assembler* as, AbstractASTDispatcher* dispenser) override;

    void PlayVIC20Sid(Assembler *as);

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

    void vbmInitScreenShiftLeft(Assembler *as);
    void vbmInitScreenShiftRight(Assembler *as);
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

    void initVbmDrawSprite2(Assembler *as);
    void vbmDrawSprite2(Assembler *as);
    void initVbmDrawSprite2E(Assembler *as);
    void vbmDrawSprite2E(Assembler *as);
    void initVbmClearSprite2(Assembler *as);
    void vbmClearSprite2(Assembler *as);

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
    void initVbmDrawSmallBCD(Assembler *as);
    void vbmDrawSmallBCD(Assembler *as);
    void initVbmDrawSmallBCDO(Assembler *as);
    void vbmDrawSmallBCDO(Assembler *as);

    void initVbmCopyToBuffer(Assembler *as);
    void vbmCopyToBuffer(Assembler *as);
    void initVbmCopyFromBuffer(Assembler *as);
    void vbmCopyFromBuffer(Assembler *as);



    void VIAIRQ(Assembler *as);

    void VIARasterIRQ(Assembler *as);


    void InitVIAIRQ(Assembler *as);


};

#endif // METHODS6502VIC20_H
