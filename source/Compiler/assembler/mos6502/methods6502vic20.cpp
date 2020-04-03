#include "methods6502vic20.h"

Methods6502Vic20::Methods6502Vic20()
{

}

void Methods6502Vic20::Assemble(Assembler *as, AbstractASTDispatcher *dispenser)
{
    Methods6502::Assemble(as,dispenser);


    if (Command("init_viairq"))
        InitVIAIRQ(as);
    else
    if (Command("VIAIRQ"))
        VIAIRQ(as);
    else
    if (Command("VIARasterIRQ"))
        VIARasterIRQ(as);
    else

    if (Command("init_vic20_sidplay"))
        as->IncludeFile(":resources/code/vic20_playsid_init.asm");
    else
    if (Command("PlayVIC20Sid"))
        PlayVIC20Sid(as);

    else

    if (Command("initVbm"))
        initVbm(as);
    else
    if (Command("vbmSetDisplayMode"))
        vbmSetDisplayMode(as);
    // Restore regular mode
    else
    if (Command("vbmResetDisplayMode"))
        vbmResetDisplayMode(as);

    // Enable or disable debug mode - switches off characterset
    else
    if (Command("vbmDebug"))
        vbmDebug(as);

    // Set the screenmemory pointer to start of a bitmap column.  There are 20 columns
    else
    if (Command("vbmSetColumn"))
        vbmSetColumn(as);

    // Set the screenmemory pointer to next bitmap column.  ie: add 192
    else
    if (Command("initVbmNextColumn"))
        initVbmNextColumn(as);
    else
    if (Command("vbmNextColumn"))
        vbmNextColumn(as);

    else
    if (Command("max"))
        MinMax(as,false);
    else
    if (Command("min"))
        MinMax(as,true);
    // Set the screenmemory pointer to an exact address and load vbmX with the 0-7 x offset
    else
    if (Command("initvbmSetPosition"))
        initvbmSetPosition(as);
    else
    if (Command("vbmSetPosition1"))
        vbmSetPosition1(as);
    else
    if (Command("vbmSetPosition2"))
        vbmSetPosition2(as);
    else
    if (Command("vbmSetPosition4"))
        vbmSetPosition4(as);

    // clear the bitmap area that starts at $1100
    else
    if (Command("initVbmClear"))
        initVbmClear(as);
    else
    if (Command("vbmClear"))
        vbmClear(as);

    // clear the colour memory
    else
    if (Command("vbmClearColor"))
        vbmClearColor(as);

    // Draw a tile at screenmemory position
    else
    if (Command("vbmDrawTile"))
        vbmDrawTile(as);
    // Draw a tile with OR operation at screenmemory position
    else
    if (Command("vbmDrawTileO"))
        vbmDrawTileO(as);
    // Draw a tile with EOR operation at screenmemory position
    else
    if (Command("vbmDrawTileE"))
        vbmDrawTileE(as);
    // Clear a tile at screenmemory position
    else
    if (Command("vbmClearTile"))
        vbmClearTile(as);
    // Clear an 8x8 block at screenmemory position
    else
    if (Command("vbmClearBlock"))
        vbmClearBlock(as);

    // TileMap Replace
    else
    if (Command("initVbmDrawTileMap"))
        initVbmDrawTileMap(as);
    // Draw TileMap
    else
    if (Command("vbmDrawTileMap"))
        vbmDrawTileMap(as);
    // TileMap OR
    else
    if (Command("initVbmDrawTileMapO"))
        initVbmDrawTileMapO(as);
    // Draw TileMap
    else
    if (Command("vbmDrawTileMapO"))
        vbmDrawTileMapO(as);
    // TileMap EOR
    else
    if (Command("initVbmDrawTileMapE"))
        initVbmDrawTileMapE(as);
    // Draw TileMap
    else
    if (Command("vbmDrawTileMapE"))
        vbmDrawTileMapE(as);
    // TileMap AND
    else
    if (Command("initVbmClearTileMap"))
        initVbmClearTileMap(as);
    // Draw TileMap
    else
    if (Command("vbmClearTileMap"))
        vbmClearTileMap(as);


    // Test pixel commands
    else
    if (Command("vbmTestPixel"))
        vbmTestPixel(as);
    else
    if (Command("vbmTestPixel2"))
        vbmTestPixel2(as);

    else
    if (Command("initVbmTestTilePixel"))
        initVbmTestTilePixel(as);
    else
    if (Command("initVbmTestTilePixel2"))
        initVbmTestTilePixel2(as);
    else
    if (Command("vbmTestTilePixel"))
        vbmTestTilePixel(as);
    else
    if (Command("vbmTestTilePixel2"))
        vbmTestTilePixel2(as);

    // Dot commands
    else
    if (Command("initVbmDot"))
        initVbmDot(as);
    // Draw dot
    else
    if (Command("vbmDrawDot"))
        vbmDrawDot(as);
    // Clear dot
    else
    if (Command("vbmClearDot"))
        vbmClearDot(as);
    // Draw dot with Eor
    else
    if (Command("vbmDrawDotE"))
        vbmDrawDotE(as);

    // Blot commands
    else
    if (Command("initVbmBlot"))
        initVbmBlot(as);
    // Draw Blot
    else
    if (Command("vbmDrawBlot"))
        vbmDrawBlot(as);
    // Clear Blot
    else
    if (Command("vbmClearBlot"))
        vbmClearBlot(as);
    // Draw Blot with Eor
    else
    if (Command("vbmDrawBlotE"))
        vbmDrawBlotE(as);

    // Horizontal Scrolling
    else
    if (Command("initVbmScrollLeft"))
        initVbmScrollLeft(as);
    else
    if (Command("initVbmScrollRight"))
        initVbmScrollRight(as);
    else
    if (Command("initVbmScrollFixTop"))
        initVbmScrollFixTop(as);
    else
    if (Command("initVbmScrollFixBottom"))
        initVbmScrollFixBottom(as);

    else
    if (Command("vbmScrollLeft"))
        vbmScrollLeft(as);
    else
    if (Command("vbmScrollRight"))
        vbmScrollRight(as);
    else
    if (Command("vbmScrollFixTop"))
        vbmScrollFixTop(as);
    else
    if (Command("vbmScrollFixBottom"))
        vbmScrollFixBottom(as);

    else
    if (Command("vbmInitScreenShiftLeft"))
        vbmInitScreenShiftLeft(as);
    else
    if (Command("vbmInitScreenShiftRight"))
        vbmInitScreenShiftRight(as);
    else
    if (Command("vbmScreenShiftLeft"))
        vbmScreenShiftLeft(as);
    else
    if (Command("vbmScreenShiftRight"))
        vbmScreenShiftRight(as);

    // Sprites
    else
    if (Command("initVbmSpriteStitch"))
        initVbmSpriteStitch(as);
    else
    if (Command("vbmSpriteStitch"))
        vbmSpriteStitch(as);
    else
    if (Command("initVbmSpriteShiftR"))
        initVbmSpriteShiftR(as);
    else
    if (Command("vbmSpriteShiftR"))
        vbmSpriteShiftR(as);
    else
    if (Command("initVbmSpriteShiftL"))
        initVbmSpriteShiftL(as);
    else
    if (Command("vbmSpriteShiftL"))
        vbmSpriteShiftL(as);
    else
    if (Command("initVbmSpriteShiftSR"))
        initVbmSpriteShiftSR(as);
    else
    if (Command("vbmSpriteShiftSR"))
        vbmSpriteShiftSR(as);
    else
    if (Command("initVbmSpriteShiftSL"))
        initVbmSpriteShiftSL(as);
    else
    if (Command("vbmSpriteShiftSL"))
        vbmSpriteShiftSL(as);

    else
    if (Command("initVbmDrawSprite"))
        initVbmDrawSprite(as);
    else
    if (Command("vbmDrawSprite"))
        vbmDrawSprite(as);
    else
    if (Command("initVbmDrawSpriteE"))
        initVbmDrawSpriteE(as);
    else
    if (Command("vbmDrawSpriteE"))
        vbmDrawSpriteE(as);
    else
    if (Command("initVbmClearSprite"))
        initVbmClearSprite(as);
    else
    if (Command("vbmClearSprite"))
        vbmClearSprite(as);

    else
    if (Command("initVbmDrawSprite2"))
        initVbmDrawSprite2(as);
    else
    if (Command("vbmDrawSprite2"))
        vbmDrawSprite2(as);
    else
    if (Command("initVbmDrawSprite2E"))
        initVbmDrawSprite2E(as);
    else
    if (Command("vbmDrawSprite2E"))
        vbmDrawSprite2E(as);
    else
    if (Command("initVbmClearSprite2"))
        initVbmClearSprite2(as);
    else
    if (Command("vbmClearSprite2"))
        vbmClearSprite2(as);
    else

    if (Command("initVbmDrawSprite8"))
        initVbmDrawSprite8(as);
    else
    if (Command("vbmDrawSprite8"))
        vbmDrawSprite8(as);
    else
    if (Command("initVbmDrawSprite8E"))
        initVbmDrawSprite8E(as);
    else
    if (Command("vbmDrawSprite8E"))
        vbmDrawSprite8E(as);
    else
    if (Command("initVbmClearSprite8"))
        initVbmClearSprite8(as);
    else
    if (Command("vbmClearSprite8"))
        vbmClearSprite8(as);

    else
    if (Command("initVbmDrawSprite16"))
        initVbmDrawSprite16(as);
    else
    if (Command("vbmDrawSprite16"))
        vbmDrawSprite16(as);
    else
    if (Command("initVbmDrawSprite16E"))
        initVbmDrawSprite16E(as);
    else
    if (Command("vbmDrawSprite16E"))
        vbmDrawSprite16E(as);
    else
    if (Command("initVbmClearSprite16"))
        initVbmClearSprite16(as);
    else
    if (Command("vbmClearSprite16"))
        vbmClearSprite16(as);

    else
    if (Command("initVbmDrawSpriteSlice"))
        initVbmDrawSpriteSlice(as);
    else
    if (Command("vbmDrawSpriteSlice"))
        vbmDrawSpriteSlice(as);
    else
    if (Command("initVbmDrawSpriteSliceE"))
        initVbmDrawSpriteSliceE(as);
    else
    if (Command("vbmDrawSpriteSliceE"))
        vbmDrawSpriteSliceE(as);
    else
    if (Command("initVbmClearSpriteSlice"))
        initVbmClearSpriteSlice(as);
    else
    if (Command("vbmClearSpriteSlice"))
        vbmClearSpriteSlice(as);

    // 8x8 text commands
    else
    if (Command("initVbmDrawText"))
        initVbmDrawText(as);
    else
    if (Command("vbmDrawText"))
        vbmDrawText(as);
    else
    if (Command("initVbmDrawTextO"))
        initVbmDrawTextO(as);
    else
    if (Command("vbmDrawTextO"))
        vbmDrawTextO(as);
    else
    if (Command("initVbmDrawTextE"))
        initVbmDrawTextE(as);
    else
    if (Command("vbmDrawTextE"))
        vbmDrawTextE(as);
    else
    if (Command("initVbmClearText"))
        initVbmClearText(as);
    else
    if (Command("vbmClearText"))
        vbmClearText(as);

    // 4x8 text commands
    else
    if (Command("initVbmDrawSmallTextO"))
        initVbmDrawSmallTextO(as);
    else
    if (Command("vbmDrawSmallTextO"))
        vbmDrawSmallTextO(as);
    else
    if (Command("initVbmDrawSmallTextE"))
        initVbmDrawSmallTextE(as);
    else
    if (Command("vbmDrawSmallTextE"))
        vbmDrawSmallTextE(as);
    else
    if (Command("initVbmClearSmallText"))
        initVbmClearSmallText(as);
    else
    if (Command("vbmClearSmallText"))
        vbmClearSmallText(as);

    // 8x8 Draw BCD numbers
    else
    if (Command("initVbmDrawBCD"))
        initVbmDrawBCD(as);
    else
    if (Command("vbmDrawBCD"))
        vbmDrawBCD(as);

    // 4x8 Draw BCD numbers
    else
    if (Command("initVbmDrawSmallBCD"))
        initVbmDrawSmallBCD(as);
    else
    if (Command("vbmDrawSmallBCD"))
        vbmDrawSmallBCD(as);
    else
    if (Command("initVbmDrawSmallBCDO"))
        initVbmDrawSmallBCDO(as);
    else
    if (Command("vbmDrawSmallBCDO"))
        vbmDrawSmallBCDO(as);


    // bitmap buffers
    else
    if (Command("initVbmCopyToBuffer"))
        initVbmCopyToBuffer(as);
    else
    if (Command("vbmCopyToBuffer"))
        vbmCopyToBuffer(as);

    else
    if (Command("initVbmCopyFromBuffer"))
        initVbmCopyFromBuffer(as);
    else
    if (Command("vbmCopyFromBuffer"))
        vbmCopyFromBuffer(as);

}

void Methods6502Vic20::PlayVIC20Sid(Assembler *as)
{
    as->Term("jsr ");
    m_node->m_params[0]->Accept(m_dispatcher);
    as->Term();
    as->Asm("jsr vic20_playsid_init");

}


/*
 * vbm
 */

void Methods6502Vic20::initVbm(Assembler* as)
{
    if (m_node->m_isInitialized["vbm"])
        return;

    m_node->m_isInitialized["vbm"] = true;

    as->Comment("Initialise the core VBM (Vic20 Bitmap Mode) library");
    as->Comment("Created by Andy H - Hewco.uk for use in Turbo Rascal");
    as->Comment("See help to get started, all commmands begin with 'vbm'");
    as->IncludeFile(":resources/code/vbm/init_vbm.asm");

}

void Methods6502Vic20::vbmSetDisplayMode(Assembler* as)
{
    DefineScreen(as);

    int mode = 0;

    VerifyInitialized("vbm","InitVbm");

    as->Comment("Set special display mode for VBM bitmap graphics");

    if ( m_node->m_params[0]->isPureNumeric() ) {

        // pure numeric
        mode = m_node->m_params[0]->getValueAsInt(as);

        if (mode <0 || mode > 15)
            ErrorHandler::e.Error("vbmSetDisplayMode - Please pass in a constant: 0 - 15", m_node->m_op.m_lineNumber);

    } else {

        // complex not supported
        ErrorHandler::e.Error("vbmSetDisplayMode - Please pass in a constant: 0 - 15", m_node->m_op.m_lineNumber);

    }

    if (mode == 0) {    // 20 x 24 (12 dbl) - 192 pixel high
        as->Asm("lda #20 ; screen width in chars");
        as->Asm("sta vbmNumColumns");
        as->Asm("lda #192 ; screen height in pixels");
        as->Asm("sta vbmScrHeight");
        as->Asm("lda #12 ; screen height in chars");
        as->Asm("sta vbmJ");
    } else if (mode == 1) {    // 19 x 24 (12 dbl) - 192 pixel high
        as->Asm("lda #19 ; screen width in chars");
        as->Asm("sta vbmNumColumns");
        as->Asm("lda #192 ; screen height in pixels");
        as->Asm("sta vbmScrHeight");
        as->Asm("lda #12 ; screen height in chars");
        as->Asm("sta vbmJ");

    } else if (mode == 2) {    // 20 x 22 (11 dbl) - 176 pixel high
        as->Asm("lda #20 ; screen width in chars");
        as->Asm("sta vbmNumColumns");
        as->Asm("lda #176 ; screen height in pixels");
        as->Asm("sta vbmScrHeight");
        as->Asm("lda #11 ; screen height in chars");
        as->Asm("sta vbmJ");
    } else if (mode == 3) {    // 19 x 22 (11 dbl) - 176 pixel high
        as->Asm("lda #19 ; screen width in chars");
        as->Asm("sta vbmNumColumns");
        as->Asm("lda #176 ; screen height in pixels");
        as->Asm("sta vbmScrHeight");
        as->Asm("lda #11 ; screen height in chars");
        as->Asm("sta vbmJ");

    } else if (mode == 4) {    // 20 x 20 (10 dbl) - 160 pixel high
        as->Asm("lda #20 ; screen width in chars");
        as->Asm("sta vbmNumColumns");
        as->Asm("lda #160 ; screen height in pixels");
        as->Asm("sta vbmScrHeight");
        as->Asm("lda #10 ; screen height in chars");
        as->Asm("sta vbmJ");
    } else if (mode == 5) {    // 19 x 20 (10 dbl) - 160 pixel high
        as->Asm("lda #19 ; screen width in chars");
        as->Asm("sta vbmNumColumns");
        as->Asm("lda #160 ; screen height in pixels");
        as->Asm("sta vbmScrHeight");
        as->Asm("lda #10 ; screen height in chars");
        as->Asm("sta vbmJ");

    } else if (mode == 6) {    // 20 x 18 (9 dbl) - 144 pixel high
        as->Asm("lda #20 ; screen width in chars");
        as->Asm("sta vbmNumColumns");
        as->Asm("lda #144 ; screen height in pixels");
        as->Asm("sta vbmScrHeight");
        as->Asm("lda #9 ; screen height in chars");
        as->Asm("sta vbmJ");
    } else if (mode == 7) {    // 19 x 18 (9 dbl) - 144 pixel high
        as->Asm("lda #19 ; screen width in chars");
        as->Asm("sta vbmNumColumns");
        as->Asm("lda #144 ; screen height in pixels");
        as->Asm("sta vbmScrHeight");
        as->Asm("lda #9 ; screen height in chars");
        as->Asm("sta vbmJ");



    } else if (mode == 8) {    // 18 x 24 (12 dbl) - 192 pixel high
        as->Asm("lda #18 ; screen width in chars");
        as->Asm("sta vbmNumColumns");
        as->Asm("lda #192 ; screen height in pixels");
        as->Asm("sta vbmScrHeight");
        as->Asm("lda #12 ; screen height in chars");
        as->Asm("sta vbmJ");
    } else if (mode == 9) {    // 17 x 24 (12 dbl) - 192 pixel high
        as->Asm("lda #17 ; screen width in chars");
        as->Asm("sta vbmNumColumns");
        as->Asm("lda #192 ; screen height in pixels");
        as->Asm("sta vbmScrHeight");
        as->Asm("lda #12 ; screen height in chars");
        as->Asm("sta vbmJ");

    } else if (mode == 10) {    // 18 x 22 (11 dbl) - 160 pixel high
        as->Asm("lda #18 ; screen width in chars");
        as->Asm("sta vbmNumColumns");
        as->Asm("lda #176 ; screen height in pixels");
        as->Asm("sta vbmScrHeight");
        as->Asm("lda #11 ; screen height in chars");
        as->Asm("sta vbmJ");
    } else if (mode == 11) {    // 17 x 22 (11 dbl) - 160 pixel high
        as->Asm("lda #17 ; screen width in chars");
        as->Asm("sta vbmNumColumns");
        as->Asm("lda #176 ; screen height in pixels");
        as->Asm("sta vbmScrHeight");
        as->Asm("lda #11 ; screen height in chars");
        as->Asm("sta vbmJ");

    } else if (mode == 12) {    // 18 x 20 (10 dbl) - 160 pixel high
        as->Asm("lda #18 ; screen width in chars");
        as->Asm("sta vbmNumColumns");
        as->Asm("lda #160 ; screen height in pixels");
        as->Asm("sta vbmScrHeight");
        as->Asm("lda #10 ; screen height in chars");
        as->Asm("sta vbmJ");
    } else if (mode == 13) {    // 17 x 20 (10 dbl) - 160 pixel high
        as->Asm("lda #17 ; screen width in chars");
        as->Asm("sta vbmNumColumns");
        as->Asm("lda #160 ; screen height in pixels");
        as->Asm("sta vbmScrHeight");
        as->Asm("lda #10 ; screen height in chars");
        as->Asm("sta vbmJ");

    } else if (mode == 14) {    // 18 x 18 (9 dbl) - 144 pixel high
        as->Asm("lda #18 ; screen width in chars");
        as->Asm("sta vbmNumColumns");
        as->Asm("lda #144 ; screen height in pixels");
        as->Asm("sta vbmScrHeight");
        as->Asm("lda #9 ; screen height in chars");
        as->Asm("sta vbmJ");
    } else if (mode == 15) {    // 17 x 18 (9 dbl) - 144 pixel high
        as->Asm("lda #17 ; screen width in chars");
        as->Asm("sta vbmNumColumns");
        as->Asm("lda #144 ; screen height in pixels");
        as->Asm("sta vbmScrHeight");
        as->Asm("lda #9 ; screen height in chars");
        as->Asm("sta vbmJ");

    }
    else
    {   // something went wrong
        ErrorHandler::e.Error("vbmSetDisplayMode - Please pass in a constant: 0 - 5", m_node->m_op.m_lineNumber);
    }

    as->Asm("jsr vbmSetDisplayMode");

}
void Methods6502Vic20::vbmResetDisplayMode(Assembler* as)
{
    VerifyInitialized("vbm","InitVbm");

    as->Comment("Restore graphics mode");

    as->Asm("lda vbm9005;keep");
    as->Asm("sta $9005");
    as->Asm("lda vbm9001;keep");
    as->Asm("sta $9001");
    as->Asm("lda vbm9000;keep");
    as->Asm("sta $9000");
    as->Asm("lda #22;keep");
    as->Asm("sta $9002");
    as->Asm("lda #46;keep");
    as->Asm("sta $9003");

}

void Methods6502Vic20::vbmDebug(Assembler* as)
{
    int mode = 0;

    VerifyInitialized("vbm","InitVbm");

    as->Comment("Set debug mode on / off to check character layout");

    if ( m_node->m_params[0]->isPureNumeric() ) {

        // pure numeric
        mode = m_node->m_params[0]->getValueAsInt(as);

        if (mode !=0 && mode != 1)
            ErrorHandler::e.Error("vbmDebug - Please pass in a constant: 0 = off, 1 = on", m_node->m_op.m_lineNumber);

    } else {

        // complex not supported
        ErrorHandler::e.Error("vbmDebug - Please pass in a constant: 0 = off, 1 = on", m_node->m_op.m_lineNumber);

    }

    if (mode == 0) {

        as->Asm("lda #%11001100	; 204 (CC) - set screen and character to $1000");
        as->Asm("sta $9005");

    } else {

        as->Asm("lda #%11000000	; 192 (C0) - set screen and character to $1000");
        as->Asm("sta $9005");

    }

}

void Methods6502Vic20::initVbmClear(Assembler* as)
{
    if (m_node->m_isInitialized["vbmClear"])
        return;

    m_node->m_isInitialized["vbmClear"] = true;

    as->Comment("VBM Clear bitmap routine");
    as->IncludeFile(":resources/code/vbm/vbmClear.asm");

}
void Methods6502Vic20::vbmClear(Assembler* as)
{

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmClear","InitVbmClear");

    as->Comment("Clear VBM bitmap");

    if (m_node->m_params[0]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[0]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("column is complex");
        LoadVar(as, 0);
    }

    as->Asm("sta vbmI ; byte to clear bitmap with");

    as->Asm("jsr vbmClear");

}

void Methods6502Vic20::vbmSetColumn(Assembler *as) {

    VerifyInitialized("vbm","InitVbm");

    as->Comment("----------");
    as->Comment("vbmSetColumn in ScreenMemory ZP - column offset");

//    QString lblDTNoOverflow = as->NewLabel("dtnooverflow");

    // ypos
    if (m_node->m_params[0]->isPureNumeric()) {
        // pure numeric
        as->Asm( "ldx #" + QString::number( m_node->m_params[0]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("column is complex");
        LoadVar(as, 0);
        as->Asm("tax");
    }

    as->Asm("lda vbmScrL,x   ; Address of table lo");
    as->Asm("ldy vbmScrH,x   ; Address of table hi");

    as->Asm("sta screenmemory   ; Set sceenmemory to start of column lo");
    as->Asm("sty screenmemory+1 ; Set sceenmemory to start of column hi");

}

void Methods6502Vic20::initvbmSetPosition(Assembler *as) {

    VerifyInitialized("vbm","InitVbm");
//    VerifyInitialized("vbmSetPos","InitVbmSetPosition");

    if (m_node->m_isInitialized["vbmSetPos"])
        return;

    m_node->m_isInitialized["vbmSetPos"] = true;

    as->Comment("----------");
    as->Comment("init vbmSetPosition");
    as->IncludeFile(":resources/code/vbm/vbmSetPosition.asm");

}

void Methods6502Vic20::vbmSetPosition1(Assembler *as) {

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmSetPos","InitVbmSetPosition");

    as->Comment("----------");
    as->Comment("vbmSetPosition1 x, y");

    // ypos
    if (m_node->m_params[1]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[1]->getValueAsInt(as) ) );
    } else {
        // complex
        as->Comment("y is complex");
        LoadVar(as, 1);
    }
    as->Asm("sta vbmY");
    // xpos
    if (m_node->m_params[0]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[0]->getValueAsInt(as) ) );
    } else {
        // complex
        as->Comment("x is complex");
        LoadVar(as, 0);
    }
    as->Asm("sta vbmX");

    as->Asm("jsr vbmSetPosition1");

}
void Methods6502Vic20::vbmSetPosition2(Assembler *as) {

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmSetPos","InitVbmSetPosition");

    as->Comment("----------");
    as->Comment("vbmSetPosition2 x, y");

    // ypos
    if (m_node->m_params[1]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[1]->getValueAsInt(as) ) );
    } else {
        // complex
        as->Comment("y is complex");
        LoadVar(as, 1);
    }
    as->Asm("sta vbmY");
    // xpos
    if (m_node->m_params[0]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[0]->getValueAsInt(as) ) );
    } else {
        // complex
        as->Comment("x is complex");
        LoadVar(as, 0);
    }
    as->Asm("sta vbmX");

    as->Asm("jsr vbmSetPosition2");

}
void Methods6502Vic20::vbmSetPosition4(Assembler *as) {

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmSetPos","InitVbmSetPosition");

    as->Comment("----------");
    as->Comment("vbmSetPosition4 x, y");

    // ypos
    if (m_node->m_params[1]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[1]->getValueAsInt(as) ) );
    } else {
        // complex
        as->Comment("y is complex");
        LoadVar(as, 1);
    }
    as->Asm("sta vbmY");
    // xpos
    if (m_node->m_params[0]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[0]->getValueAsInt(as) ) );
    } else {
        // complex
        as->Comment("x is complex");
        LoadVar(as, 0);
    }
    as->Asm("sta vbmX");

    as->Asm("jsr vbmSetPosition4");

}

void Methods6502Vic20::initVbmNextColumn(Assembler *as)
{
    if (m_node->m_isInitialized["vbmNextColumn"])
        return;

    m_node->m_isInitialized["vbmNextColumn"] = true;

    as->Label("vbmNextColumn");
    as->Comment("move screenmemory to next column");
    as->Comment("WARNING: This is not safe to use with Screen Scrolling as the character map position is unknown");
    as->Asm("lda screenmemory");
    as->Asm("clc");
    as->Asm("adc vbmScrHeight ; #192 ; next column");
    as->Asm("bcc vbmCSS_overflow");
    as->Asm("inc screenmemory+1");
    as->Label("vbmCSS_overflow");
    as->Asm("sta screenmemory");

}
void Methods6502Vic20::vbmNextColumn(Assembler* as)
{

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmNextColumn","InitVbmNextColumn");

    if (m_node->m_isInitialized["vbmScrollLeft"] || m_node->m_isInitialized["vbmScrollRight"])
        ErrorHandler::e.Error("vbmscrollright and vbmscrollleft are not compatible with vbmNextColumn. Use vbmSetPosition? or vbmSetColumn instead.", m_node->m_op.m_lineNumber);

    as->Comment("WARNING: Do not use if using character screen scrolling commands");
    as->Asm("jsr vbmNextColumn");
}

void Methods6502Vic20::vbmClearColor(Assembler* as)
{

    VerifyInitialized("vbm","InitVbm");

    QString lbl = as->NewLabel("vbmCC_loop");
        as->Asm("lda #$93");
        as->Asm("sta screenmemory+1");
        as->Asm("lda #$ff");
        as->Asm("sta screenmemory");

        // load A with colour value to clear with
        if (m_node->m_params[0]->isPureNumeric()) {
            // pure numeric
            as->Asm( "lda #" + QString::number( m_node->m_params[0]->getValueAsInt(as)  ) );
        } else {
            // complex
            as->Comment("colour is complex");
            LoadVar(as, 0);
        }

        as->Asm("ldy #241 ; colour mem to clear (stops at zero so +1)");
    as->Label(lbl);
        as->Asm("sta (screenmemory),y");
        as->Asm("dey");
        as->Asm("bne " + lbl);

    as->PopLabel("vbmCC_loop");
}

void Methods6502Vic20::vbmDrawTile(Assembler *as) {

    VerifyInitialized("vbm","InitVbm");

    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    //QSharedPointer<NodeNumber> num = (QSharedPointer<NodeNumber>)qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be variable or number", m_node->m_op.m_lineNumber);
    }

    QString addr = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr = var->getValue(as);

    as->Asm("ldy #$0");

    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Asm("lda (" + addr +"),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + addr +"),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + addr +"),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + addr +"),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + addr +"),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + addr +"),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + addr +"),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + addr +"),y");
        as->Asm("sta (screenmemory),y");
    } else {
        as->Asm("lda " + addr +",y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda " + addr +",y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda " + addr +",y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda " + addr +",y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda " + addr +",y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda " + addr +",y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda " + addr +",y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda " + addr +",y");
        as->Asm("sta (screenmemory),y");
    }

}

void Methods6502Vic20::vbmDrawTileO(Assembler *as) {

    VerifyInitialized("vbm","InitVbm");

    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    //QSharedPointer<NodeNumber> num = (QSharedPointer<NodeNumber>)qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be variable or number", m_node->m_op.m_lineNumber);
    }

    QString addr = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr = var->getValue(as);

    as->Asm("ldy #$0");

    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Asm("lda (" + addr +"),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + addr +"),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + addr +"),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + addr +"),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + addr +"),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + addr +"),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + addr +"),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + addr +"),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
    } else {
        as->Asm("lda " + addr +",y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda " + addr +",y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda " + addr +",y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda " + addr +",y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda " + addr +",y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda " + addr +",y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda " + addr +",y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda " + addr +",y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
    }

}

void Methods6502Vic20::vbmDrawTileE(Assembler *as) {

    VerifyInitialized("vbm","InitVbm");

    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    //QSharedPointer<NodeNumber> num = (QSharedPointer<NodeNumber>)qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be variable or number", m_node->m_op.m_lineNumber);
    }

    QString addr = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr = var->getValue(as);

    as->Asm("ldy #$0");

    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Asm("lda (" + addr +"),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + addr +"),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + addr +"),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + addr +"),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + addr +"),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + addr +"),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + addr +"),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + addr +"),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
    } else {
        as->Asm("lda " + addr +",y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda " + addr +",y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda " + addr +",y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda " + addr +",y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda " + addr +",y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda " + addr +",y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda " + addr +",y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda " + addr +",y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
    }

}

void Methods6502Vic20::vbmClearTile(Assembler *as) {

    VerifyInitialized("vbm","InitVbm");

    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    //QSharedPointer<NodeNumber> num = (QSharedPointer<NodeNumber>)qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be variable or number", m_node->m_op.m_lineNumber);
    }

    QString addr = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr = var->getValue(as);

    as->Asm("ldy #$0");

    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Asm("lda (" + addr +"),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + addr +"),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + addr +"),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + addr +"),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + addr +"),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + addr +"),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + addr +"),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + addr +"),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
    } else {
        as->Asm("lda " + addr +",y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda " + addr +",y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda " + addr +",y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda " + addr +",y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda " + addr +",y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda " + addr +",y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda " + addr +",y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda " + addr +",y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
    }

}

void Methods6502Vic20::vbmClearBlock(Assembler *as) {

    VerifyInitialized("vbm","InitVbm");

    as->Asm("ldy #$0");
    as->Asm("tya");

    as->Asm("sta (screenmemory),y");
    as->Asm("iny");
    as->Asm("sta (screenmemory),y");
    as->Asm("iny");
    as->Asm("sta (screenmemory),y");
    as->Asm("iny");
    as->Asm("sta (screenmemory),y");
    as->Asm("iny");
    as->Asm("sta (screenmemory),y");
    as->Asm("iny");
    as->Asm("sta (screenmemory),y");
    as->Asm("iny");
    as->Asm("sta (screenmemory),y");
    as->Asm("iny");
    as->Asm("sta (screenmemory),y");

}

void Methods6502Vic20::initVbmDrawTileMap(Assembler* as)
{
    if (m_node->m_isInitialized["vbmTileMap"])
        return;

    m_node->m_isInitialized["vbmTileMap"] = true;

    as->Comment("Draw tile characters to the bitmap using a tilemap array (columns,rows)");
    as->Comment("Tilemap    = " + as->m_internalZP[0]);
    as->Comment("Tile chars = " + as->m_internalZP[1]);
    as->Comment("Temp addr  = " + as->m_internalZP[2] + " - used to calculate tile address");
    as->Label("vbmDrawTileMap");

    as->Label("vbmDTM_Xloop");

        as->Comment("calculate next screen memory position");
        as->Asm("ldx vbmX");
        as->Asm("lda vbmScrL,x   ; Address of table lo");
        as->Asm("ldy vbmScrH,x   ; Address of table hi");
        as->Asm("clc");
        as->Asm("adc vbmY		; Add Y offset");
        as->Asm("bcc vbmDTM_NSP_NoOverflow");
        as->Asm("iny");

    as->Label("vbmDTM_NSP_NoOverflow");
        as->Asm("sta screenmemory");
        as->Asm("sty screenmemory+1");

    as->Label("vbmDTM_GetTileNum");
        as->Comment("convert tile number (0-255) * 8 = memory offset");
// 54 = tilemap, 56 = tiles, 58 = selected tile
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[0] +"),y		; get tile from current position on tilemap");
        as->Asm("sta " + as->m_internalZP[2]);
        as->Asm("sty " + as->m_internalZP[2] + "+1");

        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x2");
        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x4");
        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x8");

        as->Asm("lda " + as->m_internalZP[2] );
        as->Asm("clc");
        as->Asm("adc "+ as->m_internalZP[1] + "  ; add tile low address");
        as->Asm("sta " + as->m_internalZP[2] );
        as->Asm("lda " + as->m_internalZP[2] + "+1");
        as->Asm("adc " + as->m_internalZP[1] + "+1 ; add tile high address");
        as->Asm("sta " + as->m_internalZP[2] + "+1" );

    as->Label("vbmDTM_DrawTile");
        as->Comment("y reg is ZERO from ldy #0 in GetTileNum");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("sta (screenmemory),y");

// 54 = tilemap, 56 = tiles, 58 = selected tile
    as->Label("vbmDTM_NextTileMap");
        as->Asm("clc");
        as->Asm("inc " + as->m_internalZP[0] + "  ; low byte");
        as->Asm("bne vbmDTM_NTM_NoOverflow");
        as->Asm("inc " + as->m_internalZP[0] + "+1  ; high byte");
    as->Label("vbmDTM_NTM_NoOverflow");
        as->Comment("next x pos on screen");
        as->Asm("inc vbmX");
        as->Asm("lda vbmI   ; tilemap end x pos");
        as->Asm("cmp vbmX  ; have we reached this?");
        as->Asm("bne vbmDTM_Xloop  ; no, draw next column");

        as->Comment("yes, set x back to start x and inc vbmY by 8 rows (pixels)");
        as->Asm("lda vbmT");
        as->Asm("sta vbmX");

        as->Comment("check if y pos at end and loop if not");
        as->Asm("lda vbmY");
        as->Asm("clc");
        as->Asm("adc #8");
        as->Asm("sta vbmY");

        as->Asm("lda vbmJ   ; tilemap end Y pos in pixels");
        as->Asm("cmp vbmY  ; have we reached this?");
        as->Asm("bcc vbmDTM_Done");
        as->Asm("jmp vbmDTM_Xloop");

    as->Label("vbmDTM_Done");

}
void Methods6502Vic20::vbmDrawTileMap(Assembler* as)
{
    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmTileMap","InitVbmDrawTileMap");

    if (as->m_internalZP.count()==0)
        return;
    if (as->m_internalZP.count() < 3) {
        ErrorHandler::e.Error("This TRSE command needs at least 3 temporary ZP pointers but has less. Check the TRSE settings for temporary pointers.", m_node->m_op.m_lineNumber);
    }

    // address 1 - tilemap
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr1 = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr1 = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr1 = var->getValue(as);

    // address 2 - tiles
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[1]);
    if (var==nullptr && !m_node->m_params[1]->isPureNumeric()) {
        ErrorHandler::e.Error("Second parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr2= "";
    if (m_node->m_params[1]->isPureNumeric())
        addr2 = m_node->m_params[1]->HexValue();
    if (var!=nullptr)
        addr2 = var->getValue(as);

    as->Comment("Using a tilemap, draw a screenfull of tiles to the bitmap");

    as->Comment("Tilemap to use:");
    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr1 );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda " + addr1 +"+1" );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    } else {
        as->Asm("lda #<" + addr1 );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda #>" + addr1 );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    }
    as->Comment("Tile characters to use:");
    if (m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr2 );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda " + addr2 +"+1" );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    } else {
        as->Asm("lda #<" + addr2 );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda #>" + addr2 );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    }

    // start X
    if (m_node->m_params[2]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[2]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("x start is complex");
        LoadVar(as, 2);
    }
    as->Asm("sta vbmX ; x start position");
    as->Asm("sta vbmT ; x start position reset");

    // start Y
    if (m_node->m_params[3]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[3]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("y start is complex");
        LoadVar(as, 3);
    }
    as->Asm("sta vbmY ; y start position in pixels");

    // end X
    if (m_node->m_params[4]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[4]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("x end is complex");
        LoadVar(as, 4);
    }
    as->Asm("sta vbmI ; x end position");

    // end Y
    if (m_node->m_params[5]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[5]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("y end is complex");
        LoadVar(as, 5);
    }
    as->Asm("sta vbmJ ; y end position in pixels");

    as->Asm("jsr vbmDrawTileMap");

}

void Methods6502Vic20::initVbmDrawTileMapO(Assembler* as)
{
    if (m_node->m_isInitialized["vbmTileMapO"])
        return;

    m_node->m_isInitialized["vbmTileMapO"] = true;

    as->Comment("Draw tile characters to the bitmap using a tilemap array (columns,rows) with OR draw operation");
    as->Comment("Tilemap    = " + as->m_internalZP[0]);
    as->Comment("Tile chars = " + as->m_internalZP[1]);
    as->Comment("Temp addr  = " + as->m_internalZP[2] + " - used to calculate tile address");
    as->Label("vbmDrawTileMapO");

    as->Label("vbmDTMO_Xloop");

        as->Comment("calculate next screen memory position");
        as->Asm("ldx vbmX");
        as->Asm("lda vbmScrL,x   ; Address of table lo");
        as->Asm("ldy vbmScrH,x   ; Address of table hi");
        as->Asm("clc");
        as->Asm("adc vbmY		; Add Y offset");
        as->Asm("bcc vbmDTMO_NSP_NoOverflow");
        as->Asm("iny");

    as->Label("vbmDTMO_NSP_NoOverflow");
        as->Asm("sta screenmemory");
        as->Asm("sty screenmemory+1");

    as->Label("vbmDTMO_GetTileNum");
        as->Comment("convert tile number (0-255) * 8 = memory offset");
// 54 = tilemap, 56 = tiles, 58 = selected tile
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[0] +"),y		; get tile from current position on tilemap");
        as->Asm("sta " + as->m_internalZP[2]);
        as->Asm("sty " + as->m_internalZP[2] + "+1");

        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x2");
        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x4");
        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x8");

        as->Asm("lda " + as->m_internalZP[2] );
        as->Asm("clc");
        as->Asm("adc " + as->m_internalZP[1] + "  ; add tile low address");
        as->Asm("sta " + as->m_internalZP[2] );
        as->Asm("lda " + as->m_internalZP[2] + "+1"); ////////
        as->Asm("adc " + as->m_internalZP[1] + "+1 ; add tile high address");
        as->Asm("sta " + as->m_internalZP[2] + "+1" );

    as->Label("vbmDTMO_DrawTile");
        as->Comment("y reg is ZERO from ldy #0 in GetTileNum");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");

// 54 = tilemap, 56 = tiles, 58 = selected tile
    as->Label("vbmDTMO_NextTileMap");
        as->Asm("clc");
        as->Asm("inc " + as->m_internalZP[0] + "  ; low byte");
        as->Asm("bne vbmDTMO_NTM_NoOverflow");
        as->Asm("inc " + as->m_internalZP[0] + "+1  ; high byte");
    as->Label("vbmDTMO_NTM_NoOverflow");
        as->Comment("next x pos on screen");
        as->Asm("inc vbmX");
        as->Asm("lda vbmI   ; tilemap end x pos");
        as->Asm("cmp vbmX  ; have we reached this?");
        as->Asm("bne vbmDTMO_Xloop  ; no, draw next column");

        as->Comment("yes, set x back to start x and inc vbmY by 8 rows (pixels)");
        as->Asm("lda vbmT");
        as->Asm("sta vbmX");

        as->Comment("check if y pos at end and loop if not");
        as->Asm("lda vbmY");
        as->Asm("clc");
        as->Asm("adc #8");
        as->Asm("sta vbmY");

        as->Asm("lda vbmJ   ; tilemap end Y pos in pixels");
        as->Asm("cmp vbmY  ; have we reached this?");
        as->Asm("bcc vbmDTMO_Done");
        as->Asm("jmp vbmDTMO_Xloop");

    as->Label("vbmDTMO_Done");

}
void Methods6502Vic20::vbmDrawTileMapO(Assembler* as)
{
    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmTileMapO","InitVbmDrawTileMapO");

    if (as->m_internalZP.count()==0)
        return;
    if (as->m_internalZP.count() < 3) {
        ErrorHandler::e.Error("This TRSE command needs at least 3 temporary ZP pointers but has less. Check the TRSE settings for temporary pointers.", m_node->m_op.m_lineNumber);
    }

    // address 1 - tilemap
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr1 = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr1 = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr1 = var->getValue(as);

    // address 2 - tiles
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[1]);
    if (var==nullptr && !m_node->m_params[1]->isPureNumeric()) {
        ErrorHandler::e.Error("Second parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr2= "";
    if (m_node->m_params[1]->isPureNumeric())
        addr2 = m_node->m_params[1]->HexValue();
    if (var!=nullptr)
        addr2 = var->getValue(as);

    as->Comment("Using a tilemap, draw a screenfull of tiles to the bitmap with an OR operation");

    as->Comment("Tilemap to use:");
    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr1 );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda " + addr1 +"+1" );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    } else {
        as->Asm("lda #<" + addr1 );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda #>" + addr1 );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    }
    as->Comment("Tile characters to use:");
    if (m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr2 );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda " + addr2 +"+1" );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    } else {
        as->Asm("lda #<" + addr2 );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda #>" + addr2 );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    }

    // start X
    if (m_node->m_params[2]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[2]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("x start is complex");
        LoadVar(as, 2);
    }
    as->Asm("sta vbmX ; x start position");
    as->Asm("sta vbmT ; x start position reset");

    // start Y
    if (m_node->m_params[3]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[3]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("y start is complex");
        LoadVar(as, 3);
    }
    as->Asm("sta vbmY ; y start position in pixels");

    // end X
    if (m_node->m_params[4]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[4]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("x end is complex");
        LoadVar(as, 4);
    }
    as->Asm("sta vbmI ; x end position");

    // end Y
    if (m_node->m_params[5]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[5]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("y end is complex");
        LoadVar(as, 5);
    }
    as->Asm("sta vbmJ ; y end position in pixels");

    as->Asm("jsr vbmDrawTileMapO");

}

void Methods6502Vic20::initVbmDrawTileMapE(Assembler* as)
{
    if (m_node->m_isInitialized["vbmTileMapE"])
        return;

    m_node->m_isInitialized["vbmTileMapE"] = true;

    as->Comment("Draw tile characters to the bitmap using a tilemap array (columns,rows) with EOR draw operation");
    as->Comment("Tilemap    = " + as->m_internalZP[0]);
    as->Comment("Tile chars = " + as->m_internalZP[1]);
    as->Comment("Temp addr  = " + as->m_internalZP[2] + " - used to calculate tile address");
    as->Label("vbmDrawTileMapE");

    as->Label("vbmDTME_Xloop");

        as->Comment("calculate next screen memory position");
        as->Asm("ldx vbmX");
        as->Asm("lda vbmScrL,x   ; Address of table lo");
        as->Asm("ldy vbmScrH,x   ; Address of table hi");
        as->Asm("clc");
        as->Asm("adc vbmY		; Add Y offset");
        as->Asm("bcc vbmDTME_NSP_NoOverflow");
        as->Asm("iny");

    as->Label("vbmDTME_NSP_NoOverflow");
        as->Asm("sta screenmemory");
        as->Asm("sty screenmemory+1");

    as->Label("vbmDTME_GetTileNum");
        as->Comment("convert tile number (0-255) * 8 = memory offset");
// 54 = tilemap, 56 = tiles, 58 = selected tile
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[0] +"),y		; get tile from current position on tilemap");
        as->Asm("sta " + as->m_internalZP[2]);
        as->Asm("sty " + as->m_internalZP[2] + "+1");

        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x2");
        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x4");
        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x8");

        as->Asm("lda " + as->m_internalZP[2] );
        as->Asm("clc");
        as->Asm("adc " + as->m_internalZP[1] + "  ; add tile low address");
        as->Asm("sta " + as->m_internalZP[2] );
        as->Asm("lda " + as->m_internalZP[2] + "+1"); //////////////
        as->Asm("adc " + as->m_internalZP[1] + "+1 ; add tile high address");
        as->Asm("sta " + as->m_internalZP[2] + "+1" );

    as->Label("vbmDTME_DrawTile");
        as->Comment("y reg is ZERO from ldy #0 in GetTileNum");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");

// 54 = tilemap, 56 = tiles, 58 = selected tile
    as->Label("vbmDTME_NextTileMap");
        as->Asm("clc");
        as->Asm("inc " + as->m_internalZP[0] + "  ; low byte");
        as->Asm("bne vbmDTME_NTM_NoOverflow");
        as->Asm("inc " + as->m_internalZP[0] + "+1  ; high byte");
    as->Label("vbmDTME_NTM_NoOverflow");
        as->Comment("next x pos on screen");
        as->Asm("inc vbmX");
        as->Asm("lda vbmI   ; tilemap end x pos");
        as->Asm("cmp vbmX  ; have we reached this?");
        as->Asm("bne vbmDTME_Xloop  ; no, draw next column");

        as->Comment("yes, set x back to start x and inc vbmY by 8 rows (pixels)");
        as->Asm("lda vbmT");
        as->Asm("sta vbmX");

        as->Comment("check if y pos at end and loop if not");
        as->Asm("lda vbmY");
        as->Asm("clc");
        as->Asm("adc #8");
        as->Asm("sta vbmY");

        as->Asm("lda vbmJ   ; tilemap end Y pos in pixels");
        as->Asm("cmp vbmY  ; have we reached this?");
        as->Asm("bcc vbmDTME_Done");
        as->Asm("jmp vbmDTME_Xloop");

    as->Label("vbmDTME_Done");

}
void Methods6502Vic20::vbmDrawTileMapE(Assembler* as)
{
    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmTileMapE","InitVbmDrawTileMapE");

    if (as->m_internalZP.count()==0)
        return;
    if (as->m_internalZP.count() < 3) {
        ErrorHandler::e.Error("This TRSE command needs at least 3 temporary ZP pointers but has less. Check the TRSE settings for temporary pointers.", m_node->m_op.m_lineNumber);
    }

    // address 1 - tilemap
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr1 = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr1 = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr1 = var->getValue(as);

    // address 2 - tiles
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[1]);
    if (var==nullptr && !m_node->m_params[1]->isPureNumeric()) {
        ErrorHandler::e.Error("Second parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr2= "";
    if (m_node->m_params[1]->isPureNumeric())
        addr2 = m_node->m_params[1]->HexValue();
    if (var!=nullptr)
        addr2 = var->getValue(as);

    as->Comment("Using a tilemap, draw a screenfull of tiles to the bitmap with an EOR operation");

    as->Comment("Tilemap to use:");
    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr1 );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda " + addr1 +"+1" );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    } else {
        as->Asm("lda #<" + addr1 );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda #>" + addr1 );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    }
    as->Comment("Tile characters to use:");
    if (m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr2 );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda " + addr2 +"+1" );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    } else {
        as->Asm("lda #<" + addr2 );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda #>" + addr2 );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    }

    // start X
    if (m_node->m_params[2]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[2]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("x start is complex");
        LoadVar(as, 2);
    }
    as->Asm("sta vbmX ; x start position");
    as->Asm("sta vbmT ; x start position reset");

    // start Y
    if (m_node->m_params[3]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[3]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("y start is complex");
        LoadVar(as, 3);
    }
    as->Asm("sta vbmY ; y start position in pixels");

    // end X
    if (m_node->m_params[4]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[4]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("x end is complex");
        LoadVar(as, 4);
    }
    as->Asm("sta vbmI ; x end position");

    // end Y
    if (m_node->m_params[5]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[5]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("y end is complex");
        LoadVar(as, 5);
    }
    as->Asm("sta vbmJ ; y end position in pixels");

    as->Asm("jsr vbmDrawTileMapE");

}

void Methods6502Vic20::initVbmClearTileMap(Assembler* as)
{
    if (m_node->m_isInitialized["vbmClearTileMap"])
        return;

    m_node->m_isInitialized["vbmClearTileMap"] = true;

    as->Comment("Clear tile characters to the bitmap using a tilemap array (columns,rows) - AND operation");
    as->Comment("Tilemap    = " + as->m_internalZP[0]);
    as->Comment("Tile chars = " + as->m_internalZP[1]);
    as->Comment("Temp addr  = " + as->m_internalZP[2] + " - used to calculate tile address");
    as->Label("vbmClearTileMap");

    as->Label("vbmCTM_Xloop");

        as->Comment("calculate next screen memory position");
        as->Asm("ldx vbmX");
        as->Asm("lda vbmScrL,x   ; Address of table lo");
        as->Asm("ldy vbmScrH,x   ; Address of table hi");
        as->Asm("clc");
        as->Asm("adc vbmY		; Add Y offset");
        as->Asm("bcc vbmCTM_NSP_NoOverflow");
        as->Asm("iny");

    as->Label("vbmCTM_NSP_NoOverflow");
        as->Asm("sta screenmemory");
        as->Asm("sty screenmemory+1");

    as->Label("vbmCTM_GetTileNum");
        as->Comment("convert tile number (0-255) * 8 = memory offset");
// 54 = tilemap, 56 = tiles, 58 = selected tile
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[0] +"),y		; get tile from current position on tilemap");
        as->Asm("sta " + as->m_internalZP[2]);
        as->Asm("sty " + as->m_internalZP[2] + "+1");

        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x2");
        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x4");
        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x8");

        as->Asm("lda " + as->m_internalZP[2] );
        as->Asm("clc");
        as->Asm("adc "+ as->m_internalZP[1] + "  ; add tile low address");
        as->Asm("sta " + as->m_internalZP[2] );
        as->Asm("lda " + as->m_internalZP[2] + "+1");
        as->Asm("adc " + as->m_internalZP[1] + "+1 ; add tile high address");
        as->Asm("sta " + as->m_internalZP[2] + "+1" );

    as->Label("vbmCTM_DrawTile");
        as->Comment("y reg is ZERO from ldy #0 in GetTileNum");
        as->Asm("jsr vbmCTM_Clear");

// 54 = tilemap, 56 = tiles, 58 = selected tile
    as->Label("vbmCTM_NextTileMap");
        as->Asm("clc");
        as->Asm("inc " + as->m_internalZP[0] + "  ; low byte");
        as->Asm("bne vbmCTM_NTM_NoOverflow");
        as->Asm("inc " + as->m_internalZP[0] + "+1  ; high byte");
    as->Label("vbmCTM_NTM_NoOverflow");
        as->Comment("next x pos on screen");
        as->Asm("inc vbmX");
        as->Asm("lda vbmI   ; tilemap end x pos");
        as->Asm("cmp vbmX   ; have we reached this?");
        as->Asm("bne vbmCTM_Xloop  ; no, draw next column");

        as->Comment("yes, set x back to start x and inc vbmY by 8 rows (pixels)");
        as->Asm("lda vbmT");
        as->Asm("sta vbmX");

        as->Comment("check if y pos at end and loop if not");
        as->Asm("lda vbmY");
        as->Asm("clc");
        as->Asm("adc #8");
        as->Asm("sta vbmY");

        as->Asm("lda vbmJ   ; tilemap end Y pos in pixels");
        as->Asm("cmp vbmY   ; have we reached this?");
        as->Asm("bcc vbmCTM_Done");
        as->Asm("jmp vbmCTM_Xloop");

    as->Label("vbmCTM_Done");
        as->Asm("rts");

    as->Label("vbmCTM_Clear");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");

}
void Methods6502Vic20::vbmClearTileMap(Assembler* as)
{
    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmClearTileMap","InitVbmClearTileMap");

    if (as->m_internalZP.count()==0)
        return;
    if (as->m_internalZP.count() < 3) {
        ErrorHandler::e.Error("This TRSE command needs at least 3 temporary ZP pointers but has less. Check the TRSE settings for temporary pointers.", m_node->m_op.m_lineNumber);
    }

    // address 1 - tilemap
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr1 = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr1 = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr1 = var->getValue(as);

    // address 2 - tiles
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[1]);
    if (var==nullptr && !m_node->m_params[1]->isPureNumeric()) {
        ErrorHandler::e.Error("Second parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr2= "";
    if (m_node->m_params[1]->isPureNumeric())
        addr2 = m_node->m_params[1]->HexValue();
    if (var!=nullptr)
        addr2 = var->getValue(as);

    as->Comment("Using a tilemap, clear a screenfull of tiles to the bitmap (AND drawing operation)");

    as->Comment("Tilemap to use:");
    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr1 );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda " + addr1 +"+1" );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    } else {
        as->Asm("lda #<" + addr1 );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda #>" + addr1 );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    }
    as->Comment("Tile characters to use:");
    if (m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr2 );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda " + addr2 +"+1" );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    } else {
        as->Asm("lda #<" + addr2 );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda #>" + addr2 );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    }

    // start X
    if (m_node->m_params[2]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[2]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("x start is complex");
        LoadVar(as, 2);
    }
    as->Asm("sta vbmX ; x start position");
    as->Asm("sta vbmT ; x start position reset");

    // start Y
    if (m_node->m_params[3]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[3]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("y start is complex");
        LoadVar(as, 3);
    }
    as->Asm("sta vbmY ; y start position in pixels");

    // end X
    if (m_node->m_params[4]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[4]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("x end is complex");
        LoadVar(as, 4);
    }
    as->Asm("sta vbmI ; x end position");

    // end Y
    if (m_node->m_params[5]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[5]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("y end is complex");
        LoadVar(as, 5);
    }
    as->Asm("sta vbmJ ; y end position in pixels");

    as->Asm("jsr vbmClearTileMap");

}

void Methods6502Vic20::initVbmTestTilePixel(Assembler* as)
{
    if (m_node->m_isInitialized["vbmTestTilePixel"])
        return;

    m_node->m_isInitialized["vbmTestTilePixel"] = true;

    as->Comment("VBM pixel mask");
    as->Label("vbmTestTilePixelBit    dc.b $80, $40, $20, $10, $08, $04, $02, $01");

    as->Label("vbmTestTilePixel");
    as->Comment("Accumulator contains X position and y reg required (offset in tile a / y)");
    as->Asm("and #7   ; find offset for dot");
    as->Asm("tax");
    as->Asm("lda vbmTestTilePixelBit,x   ; get dot pattern");
    as->Asm("and ("+as->m_internalZP[0]+"),y ; AND with screenmemory to get pixel value");
}
void Methods6502Vic20::initVbmTestTilePixel2(Assembler* as)
{
    if (m_node->m_isInitialized["vbmTestTilePixel2"])
        return;

    m_node->m_isInitialized["vbmTestTilePixel2"] = true;

    as->Comment("VBM 2 pixel mask");
    as->Label("vbmTestTilePixel2Bit    dc.b $c0, $30, $0c, $03");

    as->Label("vbmTestTilePixel2");
    as->Comment("Accumulator contains X position and y reg required (offset in tile a / y)");
    as->Asm("and #6   ; find offset for dot");
    as->Asm("lsr");
    as->Asm("tax");
    as->Asm("lda vbmTestTilePixel2Bit,x   ; get blot pattern (double pixel, even aligned)");
    as->Asm("and ("+as->m_internalZP[0]+"),y");

}
void Methods6502Vic20::vbmTestTilePixel(Assembler* as)
{
    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmTestTilePixel","initVbmTestTilePixel");

    if (as->m_internalZP.count()==0)
        return;
    if (as->m_internalZP.count() < 1) {
        ErrorHandler::e.Error("This TRSE command needs at least 1 temporary ZP pointers but has less. Check the TRSE settings for temporary pointers.", m_node->m_op.m_lineNumber);
    }

    // address 1 - tile
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr1 = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr1 = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr1 = var->getValue(as);

    as->Comment("test pixel in an 8x8 tile");

    as->Comment("Tile to use:");
    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr1 );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda " + addr1 +"+1" );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    } else {
        as->Asm("lda #<" + addr1 );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda #>" + addr1 );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    }

    //  Y
    if (m_node->m_params[2]->isPureNumeric()) {
        // pure numeric
        as->Asm( "ldy #" + QString::number( m_node->m_params[2]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("y is complex");
        LoadVar(as, 2);
        as->Asm("tay");
    }

    //  X
    if (m_node->m_params[1]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[1]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("x is complex");
        LoadVar(as, 1);
    }

    as->Asm("jsr vbmTestTilePixel");

}
void Methods6502Vic20::vbmTestTilePixel2(Assembler* as)
{
    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmTestTilePixel2","initVbmTestTilePixel2");

    if (as->m_internalZP.count()==0)
        return;
    if (as->m_internalZP.count() < 1) {
        ErrorHandler::e.Error("This TRSE command needs at least 1 temporary ZP pointers but has less. Check the TRSE settings for temporary pointers.", m_node->m_op.m_lineNumber);
    }

    // address 1 - tile
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr1 = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr1 = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr1 = var->getValue(as);

    as->Comment("test pixel in an 8x8 tile");

    as->Comment("Tile to use:");
    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr1 );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda " + addr1 +"+1" );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    } else {
        as->Asm("lda #<" + addr1 );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda #>" + addr1 );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    }

    //  Y
    if (m_node->m_params[2]->isPureNumeric()) {
        // pure numeric
        as->Asm( "ldy #" + QString::number( m_node->m_params[2]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("y is complex");
        LoadVar(as, 2);
        as->Asm("tay");
    }

    //  X
    if (m_node->m_params[1]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[1]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("x is complex");
        LoadVar(as, 1);
    }

    as->Asm("jsr vbmTestTilePixel2");

}

void Methods6502Vic20::initVbmDot(Assembler* as)
{
    if (m_node->m_isInitialized["vbmDot"])
        return;

    m_node->m_isInitialized["vbmDot"] = true;

    as->Comment("VBM Dot mask");
    as->Label("vbmDotBit    dc.b $80, $40, $20, $10, $08, $04, $02, $01");

    as->Label("vbmDrawDot");
    as->Comment("Accumulator contains X position");
    as->Asm("lsr   ; divide by 8 to find column number");
    as->Asm("lsr");
    as->Asm("lsr");
    as->Asm("tax");
    as->Asm("lda vbmScrL,x   ; Address of table lo");
    as->Asm("ldy vbmScrH,x   ; Address of table hi");
    as->Asm("sta screenmemory   ; Set sceenmemory to start of column lo");
    as->Asm("sty screenmemory+1 ; Set sceenmemory to start of column hi");
    as->Asm("lda vbmX");
    as->Asm("and #7   ; find offset for dot");
    as->Asm("tax");
    as->Asm("lda vbmDotBit,x   ; get dot pattern");
    as->Asm("ldy vbmY  ; draw dot in row");
    as->Asm("ora (screenmemory),y");
    as->Asm("sta (screenmemory),y");
    as->Asm("rts");

    as->Label("vbmClearDot");
    as->Comment("Accumulator contains X position");
    as->Asm("lsr   ; divide by 8 to find column number");
    as->Asm("lsr");
    as->Asm("lsr");
    as->Asm("tax");
    as->Asm("lda vbmScrL,x   ; Address of table lo");
    as->Asm("ldy vbmScrH,x   ; Address of table hi");
    as->Asm("sta screenmemory   ; Set sceenmemory to start of column lo");
    as->Asm("sty screenmemory+1 ; Set sceenmemory to start of column hi");
    as->Asm("lda vbmX");
    as->Asm("and #7   ; find offset for dot");
    as->Asm("tax");
    as->Asm("lda vbmDotBit,x   ; get dot pattern");
    as->Asm("eor #$ff  ; invert it");
    as->Asm("ldy vbmY  ; clear dot in row");
    as->Asm("and (screenmemory),y");
    as->Asm("sta (screenmemory),y");
    as->Asm("rts");

    as->Label("vbmDrawDotE");
    as->Comment("Accumulator contains X position");
    as->Asm("lsr   ; divide by 8 to find column number");
    as->Asm("lsr");
    as->Asm("lsr");
    as->Asm("tax");
    as->Asm("lda vbmScrL,x   ; Address of table lo");
    as->Asm("ldy vbmScrH,x   ; Address of table hi");
    as->Asm("sta screenmemory   ; Set sceenmemory to start of column lo");
    as->Asm("sty screenmemory+1 ; Set sceenmemory to start of column hi");
    as->Asm("lda vbmX");
    as->Asm("and #7   ; find offset for dot");
    as->Asm("tax");
    as->Asm("lda vbmDotBit,x   ; get dot pattern");
    as->Asm("ldy vbmY  ; draw dot in row");
    as->Asm("eor (screenmemory),y");
    as->Asm("sta (screenmemory),y");
    as->Asm("rts");

    as->Label("vbmTestPixel");
    as->Comment("Accumulator contains X position");
    as->Asm("lsr   ; divide by 8 to find column number");
    as->Asm("lsr");
    as->Asm("lsr");
    as->Asm("tax");
    as->Asm("lda vbmScrL,x   ; Address of table lo");
    as->Asm("ldy vbmScrH,x   ; Address of table hi");
    as->Asm("sta screenmemory   ; Set sceenmemory to start of column lo");
    as->Asm("sty screenmemory+1 ; Set sceenmemory to start of column hi");
    as->Asm("lda vbmX");
    as->Asm("and #7   ; find offset for dot");
    as->Asm("tax");
    as->Asm("lda vbmDotBit,x   ; get dot pattern");
    as->Asm("ldy vbmY  ; get row");
    as->Asm("and (screenmemory),y ; AND with screenmemory to get pixel value");
}
void Methods6502Vic20::initVbmBlot(Assembler* as)
{
    if (m_node->m_isInitialized["vbmBlot"])
        return;

    m_node->m_isInitialized["vbmBlot"] = true;

    as->Comment("VBM Blot mask");
    as->Label("vbmBlotBit    dc.b $c0, $30, $0c, $03");

    as->Label("vbmDrawBlot");
    as->Comment("Accumulator contains X position");
    as->Asm("lsr   ; divide by 8 to find column number");
    as->Asm("lsr");
    as->Asm("lsr");
    as->Asm("tax");
    as->Asm("lda vbmScrL,x   ; Address of table lo");
    as->Asm("ldy vbmScrH,x   ; Address of table hi");
    as->Asm("sta screenmemory   ; Set sceenmemory to start of column lo");
    as->Asm("sty screenmemory+1 ; Set sceenmemory to start of column hi");
    as->Asm("lda vbmX");
    as->Asm("and #6   ; find offset for dot");
    as->Asm("lsr");
    as->Asm("tax");
    as->Asm("lda vbmBlotBit,x   ; get blot pattern");
    as->Asm("ldy vbmY  ; draw dot in row");
    as->Asm("ora (screenmemory),y");
    as->Asm("sta (screenmemory),y");
    as->Asm("iny");
    as->Asm("lda vbmBlotBit,x   ; get blot pattern for second row");
    as->Asm("ora (screenmemory),y");
    as->Asm("sta (screenmemory),y");
    as->Asm("rts");

    as->Label("vbmClearBlot");
    as->Comment("Accumulator contains X position");
    as->Asm("lsr   ; divide by 8 to find column number");
    as->Asm("lsr");
    as->Asm("lsr");
    as->Asm("tax");
    as->Asm("lda vbmScrL,x   ; Address of table lo");
    as->Asm("ldy vbmScrH,x   ; Address of table hi");
    as->Asm("sta screenmemory   ; Set sceenmemory to start of column lo");
    as->Asm("sty screenmemory+1 ; Set sceenmemory to start of column hi");
    as->Asm("lda vbmX");
    as->Asm("and #6   ; find offset for dot");
    as->Asm("lsr");
    as->Asm("tax");
    as->Asm("lda vbmBlotBit,x   ; get blot pattern");
    as->Asm("eor #$ff  ; invert");
    as->Asm("ldy vbmY  ; draw dot in row");
    as->Asm("and (screenmemory),y");
    as->Asm("sta (screenmemory),y");
    as->Asm("iny");
    as->Asm("lda vbmBlotBit,x   ; get blot pattern for second row");
    as->Asm("eor #$ff  ; invert");
    as->Asm("and (screenmemory),y");
    as->Asm("sta (screenmemory),y");
    as->Asm("rts");

    as->Label("vbmDrawBlotE");
    as->Comment("Accumulator contains X position");
    as->Asm("lsr   ; divide by 8 to find column number");
    as->Asm("lsr");
    as->Asm("lsr");
    as->Asm("tax");
    as->Asm("lda vbmScrL,x   ; Address of table lo");
    as->Asm("ldy vbmScrH,x   ; Address of table hi");
    as->Asm("sta screenmemory   ; Set sceenmemory to start of column lo");
    as->Asm("sty screenmemory+1 ; Set sceenmemory to start of column hi");
    as->Asm("lda vbmX");
    as->Asm("and #6   ; find offset for dot");
    as->Asm("lsr");
    as->Asm("tax");
    as->Asm("lda vbmBlotBit,x   ; get blot pattern");
    as->Asm("ldy vbmY  ; draw dot in row");
    as->Asm("eor (screenmemory),y");
    as->Asm("sta (screenmemory),y");
    as->Asm("iny");
    as->Asm("lda vbmBlotBit,x   ; get blot pattern for second row");
    as->Asm("eor (screenmemory),y");
    as->Asm("sta (screenmemory),y");
    as->Asm("rts");

    as->Label("vbmTestPixel2");
    as->Comment("Accumulator contains X position");
    as->Asm("lsr   ; divide by 8 to find column number");
    as->Asm("lsr");
    as->Asm("lsr");
    as->Asm("tax");
    as->Asm("lda vbmScrL,x   ; Address of table lo");
    as->Asm("ldy vbmScrH,x   ; Address of table hi");
    as->Asm("sta screenmemory   ; Set sceenmemory to start of column lo");
    as->Asm("sty screenmemory+1 ; Set sceenmemory to start of column hi");
    as->Asm("lda vbmX");
    as->Asm("and #6   ; find offset for dot");
    as->Asm("lsr");
    as->Asm("tax");
    as->Asm("lda vbmBlotBit,x   ; get blot pattern (double pixel, even aligned)");
    as->Asm("ldy vbmY  ; draw dot in row");
    as->Asm("and (screenmemory),y");

}

void Methods6502Vic20::vbmTestPixel(Assembler *as) {

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmDot","InitVbmDot");

    as->Comment("----------");
    as->Comment("vbmTestPixel x, y");

    // ypos
    if (m_node->m_params[1]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[1]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("y is complex");
        LoadVar(as, 1);
    }
    as->Asm("sta vbmY");
    // xpos
    if (m_node->m_params[0]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[0]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("x is complex");
        LoadVar(as, 0);
    }
    as->Asm("sta vbmX");
    as->Asm("jsr vbmTestPixel ; returns A = 1 or 0");

}
void Methods6502Vic20::vbmTestPixel2(Assembler *as) {

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmBlot","InitVbmBlot");

    as->Comment("----------");
    as->Comment("vbmTestPixel2 x, y  - can be used to test for multi-color mode pixels");

    // ypos
    if (m_node->m_params[1]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[1]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("y is complex");
        LoadVar(as, 1);
    }
    as->Asm("sta vbmY");
    // xpos
    if (m_node->m_params[0]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[0]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("x is complex");
        LoadVar(as, 0);
    }
    as->Asm("sta vbmX");
    as->Asm("jsr vbmTestPixel2");

}

void Methods6502Vic20::vbmDrawDot(Assembler *as) {

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmDot","InitVbmDot");

    as->Comment("----------");
    as->Comment("vbmDrawDot x, y");

    // ypos
    if (m_node->m_params[1]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[1]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("y is complex");
        LoadVar(as, 1);
    }
    as->Asm("sta vbmY");
    // xpos
    if (m_node->m_params[0]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[0]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("x is complex");
        LoadVar(as, 0);
    }
    as->Asm("sta vbmX");
    as->Asm("jsr vbmDrawDot");

}
void Methods6502Vic20::vbmClearDot(Assembler *as) {

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmDot","InitVbmDot");

    as->Comment("----------");
    as->Comment("vbmClearDot x, y");

    // ypos
    if (m_node->m_params[1]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[1]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("y is complex");
        LoadVar(as, 1);
    }
    as->Asm("sta vbmY");
    // xpos
    if (m_node->m_params[0]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[0]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("x is complex");
        LoadVar(as, 0);
    }
    as->Asm("sta vbmX");
    as->Asm("jsr vbmClearDot");

}
void Methods6502Vic20::vbmDrawDotE(Assembler *as) {

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmDot","InitVbmDot");

    as->Comment("----------");
    as->Comment("vbmDrawDotE x, y  - draw with Eor");

    // ypos
    if (m_node->m_params[1]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[1]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("y is complex");
        LoadVar(as, 1);
    }
    as->Asm("sta vbmY");
    // xpos
    if (m_node->m_params[0]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[0]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("x is complex");
        LoadVar(as, 0);
    }
    as->Asm("sta vbmX");
    as->Asm("jsr vbmDrawDotE");

}
void Methods6502Vic20::vbmDrawBlot(Assembler *as) {

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmBlot","InitVbmBlot");

    as->Comment("----------");
    as->Comment("vbmDrawBlot x, y");

    // ypos
    if (m_node->m_params[1]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[1]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("y is complex");
        LoadVar(as, 1);
    }
    as->Asm("sta vbmY");
    // xpos
    if (m_node->m_params[0]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[0]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("x is complex");
        LoadVar(as, 0);
    }
    as->Asm("sta vbmX");
    as->Asm("jsr vbmDrawBlot");

}
void Methods6502Vic20::vbmClearBlot(Assembler *as) {

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmBlot","InitVbmBlot");

    as->Comment("----------");
    as->Comment("vbmClearBlot x, y");

    // ypos
    if (m_node->m_params[1]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[1]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("y is complex");
        LoadVar(as, 1);
    }
    as->Asm("sta vbmY");
    // xpos
    if (m_node->m_params[0]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[0]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("x is complex");
        LoadVar(as, 0);
    }
    as->Asm("sta vbmX");
    as->Asm("jsr vbmClearBlot");

}
void Methods6502Vic20::vbmDrawBlotE(Assembler *as) {

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmBlot","InitVbmBlot");

    as->Comment("----------");
    as->Comment("vbmDrawBlotE x, y - Eor");

    // ypos
    if (m_node->m_params[1]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[1]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("y is complex");
        LoadVar(as, 1);
    }
    as->Asm("sta vbmY");
    // xpos
    if (m_node->m_params[0]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[0]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("x is complex");
        LoadVar(as, 0);
    }
    as->Asm("sta vbmX");
    as->Asm("jsr vbmDrawBlotE");

}

// vbm Scrolling

void Methods6502Vic20::initVbmScrollLeft(Assembler* as)
{
    if (m_node->m_isInitialized["vbmScrollLeft"])
        return;

    m_node->m_isInitialized["vbmScrollLeft"] = true;

    as->Comment("VBM Scroll Left routine");
    as->IncludeFile(":resources/code/vbm/vbmScrollLeft.asm");

}
void Methods6502Vic20::vbmScrollLeft(Assembler* as)
{

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmScrollLeft","InitVbmScrollLeft");

    as->Comment("Scroll VBM bitmap to the left");
    as->Asm("jsr vbmScrollLeft");

}

void Methods6502Vic20::initVbmScrollRight(Assembler* as)
{
    if (m_node->m_isInitialized["vbmScrollRight"])
        return;

    m_node->m_isInitialized["vbmScrollRight"] = true;

    as->Comment("VBM Scroll Right routine");
    as->IncludeFile(":resources/code/vbm/vbmScrollRight.asm");

}
void Methods6502Vic20::vbmScrollRight(Assembler* as)
{

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmScrollRight","InitVbmScrollRight");

    as->Comment("Scroll VBM bitmap to the Right");
    as->Asm("jsr vbmScrollRight");

}

void Methods6502Vic20::initVbmScrollFixTop(Assembler* as)
{
    if (m_node->m_isInitialized["vbmScrollFixTop"])
        return;

    m_node->m_isInitialized["vbmScrollFixTop"] = true;

    as->Comment("VBM redraw top line so is not scrolling");
    as->IncludeFile(":resources/code/vbm/vbmScrollFixTop.asm");

}
void Methods6502Vic20::vbmScrollFixTop(Assembler* as)
{

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmScrollFixTop","InitVbmScrollFixTop");

    as->Comment("Redraw top line characters so not scrolled");
    as->Asm("jsr vbmScrollFixTop");

}

void Methods6502Vic20::initVbmScrollFixBottom(Assembler* as)
{
    if (m_node->m_isInitialized["vbmScrollFixBottom"])
        return;

    m_node->m_isInitialized["vbmScrollFixBottom"] = true;

    as->Comment("VBM redraw Bottom line so is not scrolling");
    as->IncludeFile(":resources/code/vbm/vbmScrollFixBottom.asm");

}
void Methods6502Vic20::vbmScrollFixBottom(Assembler* as)
{

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmScrollFixBottom","InitVbmScrollFixBottom");

    as->Comment("Redraw Bottom line characters so not scrolled");
    as->Asm("jsr vbmScrollFixBottom");

}

void Methods6502Vic20::vbmInitScreenShiftLeft(Assembler* as)
{
    if (m_node->m_isInitialized["vbmScreenShiftLeft"])
        return;

    m_node->m_isInitialized["vbmScreenShiftLeft"] = true;

    if (as->m_internalZP.count()==0)
        return;

//    QStringList sl;

    int mode = 0;

    if ( m_node->m_params[0]->isPureNumeric() ) {

        // pure numeric
        mode = m_node->m_params[0]->getValueAsInt(as);

        if (mode <0 || mode > 15)
            ErrorHandler::e.Error("vbmInitScreenShiftLeft - Please pass in a constant: 0 - 15", m_node->m_op.m_lineNumber);

    } else {

        // complex not supported
        ErrorHandler::e.Error("vbmInitScreenShiftLeft - Please pass in a constant: 0 - 15", m_node->m_op.m_lineNumber);

    }

    as->Comment("VBM - Shift the screen to the left");
    as->Comment("x reg = start line, vbmY = end line");
    //as->IncludeFile(":resources/code/vbm/vbmScreenShiftLeft.asm");
    /*
    vbmScreenShiftLeft
        ;ldx vbmX	; line to start at
    vbmLSP_loop
        lda $1100,x ; to get bit from left
        asl
        rol $1100+$E40,x
        rol $1100+$D80,x
        rol $1100+$CC0,x
        rol $1100+$C00,x
        rol $1100+$B40,x
        rol $1100+$A80,x
        rol $1100+$9C0,x
        rol $1100+$900,x
        rol $1100+$840,x
        rol $1100+$780,x
        rol $1100+$6C0,x
        rol $1100+$600,x
        rol $1100+$540,x
        rol $1100+$480,x
        rol $1100+$3C0,x
        rol $1100+$300,x
        rol $1100+$240,x
        rol $1100+$180,x
        rol $1100+$C0,x
        rol $1100,x

        inx
        cpx vbmY	; line to end at
        bne vbmLSP_loop
*/
    int inc = 0, cnt = 0;
    if (mode == 0 || mode == 1) { inc = 192; cnt = 20; }
    if (mode == 2 || mode == 3) { inc = 176; cnt = 20; }
    if (mode == 4 || mode == 5) { inc = 160; cnt = 20; }
    if (mode == 6 || mode == 7) { inc = 144; cnt = 20; }
    if (mode == 8 || mode == 9) { inc = 192; cnt = 18; }
    if (mode == 10 || mode == 11) { inc = 176; cnt = 18; }
    if (mode == 12 || mode == 13) { inc = 160; cnt = 18; }
    if (mode == 14 || mode == 15) { inc = 144; cnt = 18; }

    as->Label( "vbmScreenShiftLeft" );
    as->Asm( "lda $1100,x" );
    as->Asm( "asl ; to get bit from left" );

    for (int i = cnt-1; i >= 0; i--) {
        as->Asm( "rol $1100+"+ QString::number( i * inc ) +",x" );
    }

    as->Asm( "inx ; next row" );
    as->Asm( "cpx vbmY ; line to end at" );
    as->Asm( "bne vbmScreenShiftLeft" );
//    sl << " rts";

//    as->m_startInsertAssembler << sl;
    //qDebug( "Here" );
}
void Methods6502Vic20::vbmInitScreenShiftRight(Assembler* as)
{
    if (m_node->m_isInitialized["vbmScreenShiftRight"])
        return;

    m_node->m_isInitialized["vbmScreenShiftRight"] = true;

    if (as->m_internalZP.count()==0)
        return;

    as->Comment("VBM - Shift the screen to the left");
    as->Comment("x reg = start line, vbmY = end line");
    //as->IncludeFile(":resources/code/vbm/vbmScreenShiftRight.asm");
    int mode = 0;

    if ( m_node->m_params[0]->isPureNumeric() ) {

        // pure numeric
        mode = m_node->m_params[0]->getValueAsInt(as);

        if (mode <0 || mode > 15)
            ErrorHandler::e.Error("vbmInitScreenShiftLeft - Please pass in a constant: 0 - 15", m_node->m_op.m_lineNumber);

    } else {

        // complex not supported
        ErrorHandler::e.Error("vbmInitScreenShiftLeft - Please pass in a constant: 0 - 15", m_node->m_op.m_lineNumber);

    }

    as->Comment("VBM - Shift the screen to the left");
    as->Comment("x reg = start line, vbmY = end line");
    //as->IncludeFile(":resources/code/vbm/vbmScreenShiftLeft.asm");

    int inc = 0, cnt = 0;
    if (mode == 0 || mode == 1) { inc = 192; cnt = 20; }
    if (mode == 2 || mode == 3) { inc = 176; cnt = 20; }
    if (mode == 4 || mode == 5) { inc = 160; cnt = 20; }
    if (mode == 6 || mode == 7) { inc = 144; cnt = 20; }
    if (mode == 8 || mode == 9) { inc = 192; cnt = 18; }
    if (mode == 10 || mode == 11) { inc = 176; cnt = 18; }
    if (mode == 12 || mode == 13) { inc = 160; cnt = 18; }
    if (mode == 14 || mode == 15) { inc = 144; cnt = 18; }

    as->Label( "vbmScreenShiftRight" );
    as->Asm( "lda $1100+"+ QString::number( (cnt-1) * inc )+",x" ); //lda $1100+$E40,x
    as->Asm( "lsr ; to get bit from right" );

    for (int i = 0; i < cnt; i++) {
        as->Asm( "ror $1100+"+ QString::number( i * inc ) +",x" );
    }


    as->Asm( "inx ; next row" );
    as->Asm( "cpx vbmY ; line to end at" );
    as->Asm( "bne vbmScreenShiftRight" );

}
void Methods6502Vic20::vbmScreenShiftLeft(Assembler* as)
{

    VerifyInitialized("vbm","InitVbm");
    if (!m_node->m_isInitialized["vbmScreenShiftLeft"])
        ErrorHandler::e.Error("Must call vbmInitScreenShiftLeft first", m_node->m_op.m_lineNumber);


    as->Comment("Screen Shift Left");

    // end line
    if (m_node->m_params[1]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[1]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("end line is complex");
        LoadVar(as, 1);
    }
    as->Asm("sta vbmY");

    // start line
    if (m_node->m_params[0]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[0]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("start line is complex");
        LoadVar(as, 0);
    }
    as->Asm("tax");

    as->Asm("jsr vbmScreenShiftLeft");

}
void Methods6502Vic20::vbmScreenShiftRight(Assembler* as)
{

    VerifyInitialized("vbm","InitVbm");
    if (!m_node->m_isInitialized["vbmScreenShiftRight"])
        ErrorHandler::e.Error("Must call vbmInitScreenShiftRight first", m_node->m_op.m_lineNumber);

    as->Comment("Screen Shift Left");

    // end line
    if (m_node->m_params[1]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[1]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("end line is complex");
        LoadVar(as, 1);
    }
    as->Asm("sta vbmY");

    // start line
    if (m_node->m_params[0]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[0]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("start line is complex");
        LoadVar(as, 0);
    }
    as->Asm("tax");

    as->Asm("jsr vbmScreenShiftRight");

}

void Methods6502Vic20::initVbmSpriteStitch(Assembler* as)
{
    if (m_node->m_isInitialized["vbmSpriteStitch"])
        return;

    m_node->m_isInitialized["vbmSpriteStitch"] = true;

    if (as->m_internalZP.count()==0)
        return;

    as->Comment("VBM - stitch two sprites together");

    as->Label("vbmSpriteStitch");

    as->Asm("ldy #0");

    as->Label("vbmSSt_loop");

    as->Asm("lda (" + as->m_internalZP[0] + "),y");
    as->Asm("ora (" + as->m_internalZP[1] + "),y");
    as->Asm("sta (" + as->m_internalZP[1] + "),y");
    as->Asm("iny");
    as->Asm("cpy vbmY");
    as->Asm("bne vbmSSt_loop");

}
void Methods6502Vic20::vbmSpriteStitch(Assembler* as)
{

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmSpriteStitch","InitVbmSpriteStitch");

    if (as->m_internalZP.count()==0)
        return;
    if (as->m_internalZP.count() < 2) {
        ErrorHandler::e.Error("This TRSE command needs at least 2 temporary ZP pointers but has less. Check the TRSE settings for temporary pointers.", m_node->m_op.m_lineNumber);
    }

    // address 1
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr1 = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr1 = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr1 = var->getValue(as);

    // address 2
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[1]);
    if (var==nullptr && !m_node->m_params[1]->isPureNumeric()) {
        ErrorHandler::e.Error("Second parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr2= "";
    if (m_node->m_params[1]->isPureNumeric())
        addr2 = m_node->m_params[1]->HexValue();
    if (var!=nullptr)
        addr2 = var->getValue(as);

    as->Comment("Stitch two sprites together");
    // load addr 1, addr 2 and height

    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr1 );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda " + addr1 +"+1" );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    } else {
        as->Asm("lda #<" + addr1 );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda #>" + addr1 );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    }

    if (m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr2 );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda " + addr2 +"+1" );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    } else {
        as->Asm("lda #<" + addr2 );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda #>" + addr2 );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    }
    // height
    if (m_node->m_params[2]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[2]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("height is complex");
        LoadVar(as, 2);
    }
    as->Asm("sta vbmY");
    as->Asm("jsr vbmSpriteStitch");

}

void Methods6502Vic20::initVbmSpriteShiftL(Assembler *as)
{
    if (m_node->m_isInitialized["vbmSpriteShiftL"])
        return;

    m_node->m_isInitialized["vbmSpriteShiftL"] = true;

    if (as->m_internalZP.count()==0)
        return;
    if (as->m_internalZP.count() < 3) {
        ErrorHandler::e.Error("This TRSE command needs at least 3 temporary ZP pointers but has less. Check the TRSE settings for temporary pointers.", m_node->m_op.m_lineNumber);
    }

    // p1 (src) p2 (dest)
    // vbmX and vbmY
    as->Comment("VBM - create left shifted positions for a sprite");
    as->Comment("Source = "+ as->m_internalZP[0]);
    as->Comment("Destination = "+ as->m_internalZP[1]);
    as->Comment("vbmX = shift increment value 1, 2 or 4 for 8, 4 or 2 shifted sprites respectively");
    as->Comment("vbmY = height of sprite to shift");


    as->Label("vbmSpriteShiftL");

    as->Asm("lda #0");
    as->Asm("sta vbmI	; current shift iteration");

    as->Comment("copy char");
    as->Label("vbmSSLCopyChar");
    as->Asm("ldy #0");

    as->Label("vbmSSLCopyCharLoop");
    as->Asm("lda (" + as->m_internalZP[0] + "),y	; read");
    as->Asm("sta (" + as->m_internalZP[1] + "),y	; copy to 1st column");
    as->Asm("iny");
    as->Asm("cpy vbmY");
    as->Asm("bne vbmSSLCopyCharLoop");

    as->Comment("shift char");
    as->Asm("lda vbmI	; skip first shift");
    as->Asm("cmp #9");
    as->Asm("beq vbmSSLShiftDone");

    as->Asm("ldy #0");
    as->Label("vbmSSLShiftLoop");
    as->Asm("clc");
    as->Asm("lda #8");
    as->Asm("sbc vbmI");
    as->Asm("sta vbmT	; number of times to shift");

    as->Asm("lda (" + as->m_internalZP[1] + "),y ; read");

    as->Label("vbmSSLShiftLoop2");
    as->Asm("asl");
    as->Asm("dec vbmT");
    as->Asm("bpl vbmSSLShiftLoop2");

    as->Asm("sta (" + as->m_internalZP[1] + "),y ; store");

    as->Asm("iny");
    as->Asm("cpy vbmY");
    as->Asm("bne vbmSSLShiftLoop ; repeat rotating all rows");

    // completed a single shifted sprite
    as->Label("vbmSSLShiftDone");
    as->Comment("store address of pre-shifted sprite in look up table");
    // copy temp pointer 1 (destination) into temp pointer 2 (lookup table)
    as->Asm("ldy #0");
    as->Asm("lda " + as->m_internalZP[1]);
    as->Asm("sta (" + as->m_internalZP[2] + "),y");
    as->Asm("iny");
    as->Asm("lda " + as->m_internalZP[1] + "+1");
    as->Asm("sta (" + as->m_internalZP[2] + "),y");
    // add 2 (as we are storing low/high in serial list for simplicity)
    as->Asm("lda " + as->m_internalZP[2]);
    as->Asm("clc");
    as->Asm("adc #2");
    as->Asm("bcc vbmSSL_p3overflow");
    as->Asm("inc " + as->m_internalZP[2] + "+1");
    as->Label("vbmSSL_p3overflow");
    as->Asm("sta " + as->m_internalZP[2]);

    as->Comment("add y to vbmP2");
    as->Asm("lda " + as->m_internalZP[1]);
    as->Asm("clc");
    as->Asm("adc vbmY");
    as->Asm("bcc vbmSSL_p2overflow");
    as->Asm("inc " + as->m_internalZP[1] + "+1");
    as->Label("vbmSSL_p2overflow");
    as->Asm("sta " + as->m_internalZP[1]);

    // do next shifted sprite
    as->Comment("increment iteration");
    as->Asm("lda vbmI");
    as->Asm("clc");
    as->Asm("adc vbmX");
    as->Asm("sta vbmI");
    as->Asm("cmp #8"); //9
    as->Asm("bcc vbmSSLCopyChar ; do next char iteration");
    //; done
}
void Methods6502Vic20::initVbmSpriteShiftR(Assembler *as)
{
    if (m_node->m_isInitialized["vbmSpriteShiftR"])
        return;

    m_node->m_isInitialized["vbmSpriteShiftR"] = true;

    if (as->m_internalZP.count()==0)
        return;
    if (as->m_internalZP.count() < 3) {
        ErrorHandler::e.Error("This TRSE command needs at least 3 temporary ZP pointers but has less. Check the TRSE settings for temporary pointers.", m_node->m_op.m_lineNumber);
    }

    // p1 (src) p2 (dest)
    // vbmX and vbmY
    as->Comment("VBM - create right shifted positions for a sprite");
    as->Comment("Source = "+ as->m_internalZP[0]);
    as->Comment("Destination = "+ as->m_internalZP[1]);
    as->Comment("vbmX = shift increment value 1, 2 or 4 for 8, 4 or 2 shifted sprites respectively");
    as->Comment("vbmY = height of sprite to shift");


    as->Label("vbmSpriteShiftR");

    as->Asm("lda #0");
    as->Asm("sta vbmI	; current shift iteration");

    as->Comment("copy char");
    as->Label("vbmSSRCopyChar");
    as->Asm("ldy #0");

    as->Label("vbmSSRCopyCharLoop");
    as->Asm("lda (" + as->m_internalZP[0] + "),y	; read");
    as->Asm("sta (" + as->m_internalZP[1] + "),y	; copy to 1st column");
    as->Asm("iny");
    as->Asm("cpy vbmY");
    as->Asm("bne vbmSSRCopyCharLoop");

    as->Comment("shift char");
    as->Asm("lda vbmI	; skip first shift");
    as->Asm("beq vbmSSRShiftDone");

    as->Asm("ldy #0");
    as->Label("vbmSSRShiftLoop");
    as->Asm("lda vbmI");
    as->Asm("sta vbmT	; number of times to shift");

    as->Asm("lda (" + as->m_internalZP[1] + "),y ; read");

    as->Label("vbmSSRShiftLoop2");
    as->Asm("lsr");
    as->Asm("dec vbmT");
    as->Asm("bne vbmSSRShiftLoop2");

    as->Asm("sta (" + as->m_internalZP[1] + "),y ; store");

    as->Asm("iny");
    as->Asm("cpy vbmY");
    as->Asm("bne vbmSSRShiftLoop ; repeat rotating all rows");

    // completed a single shifted sprite
    as->Label("vbmSSRShiftDone");
    as->Comment("store address of pre-shifted sprite in look up table");
    // copy temp pointer 1 (destination) into temp pointer 2 (lookup table)
    as->Asm("ldy #0");
    as->Asm("lda " + as->m_internalZP[1]);
    as->Asm("sta (" + as->m_internalZP[2] + "),y");
    as->Asm("iny");
    as->Asm("lda " + as->m_internalZP[1] + "+1");
    as->Asm("sta (" + as->m_internalZP[2] + "),y");
    // add 2 (as we are storing low/high in serial list for simplicity)
    as->Asm("lda " + as->m_internalZP[2]);
    as->Asm("clc");
    as->Asm("adc #2");
    as->Asm("bcc vbmSSR_p3overflow");
    as->Asm("inc " + as->m_internalZP[2] + "+1");
    as->Label("vbmSSR_p3overflow");
    as->Asm("sta " + as->m_internalZP[2]);

    as->Comment("add y to vbmP2");
    as->Asm("lda " + as->m_internalZP[1]);
    as->Asm("clc");
    as->Asm("adc vbmY");
    as->Asm("bcc vbmSSR_p2overflow");
    as->Asm("inc " + as->m_internalZP[1] + "+1");
    as->Label("vbmSSR_p2overflow");
    as->Asm("sta " + as->m_internalZP[1]);

    // do next shifted sprite
    as->Comment("increment iteration");
    as->Asm("lda vbmI");
    as->Asm("clc");
    as->Asm("adc vbmX");
    as->Asm("sta vbmI");
    as->Asm("cmp #8"); //9
    as->Asm("bcc vbmSSRCopyChar ; do next char iteration");
    //; done
}
void Methods6502Vic20::vbmSpriteShiftL(Assembler* as)
{

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmSpriteShiftL","InitVbmSpriteShiftL");

    if (as->m_internalZP.count()<3)
        return;

    // address 1
    as->Comment("Read address 1");
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr1 = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr1 = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr1 = var->getValue(as);

    // address 2
    as->Comment("Read address 2");
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[1]);
    if (var==nullptr && !m_node->m_params[1]->isPureNumeric()) {
        ErrorHandler::e.Error("Second parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr2= "";
    if (m_node->m_params[1]->isPureNumeric())
        addr2 = m_node->m_params[1]->HexValue();
    if (var!=nullptr)
        addr2 = var->getValue(as);

    // address 3
    as->Comment("Read address 3");
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[4]);
    if (var==nullptr && !m_node->m_params[4]->isPureNumeric()) {
        ErrorHandler::e.Error("fifth parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr3= "";
    if (m_node->m_params[4]->isPureNumeric())
        addr3 = m_node->m_params[4]->HexValue();
    if (var!=nullptr)
        addr3 = var->getValue(as);

    as->Comment("Sprite Shift Left");
    // load addr 1, addr 2 and height

    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr1 );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda " + addr1 +"+1" );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    } else {
        as->Asm("lda #<" + addr1 );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda #>" + addr1 );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    }

    if (m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr2 );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda " + addr2 +"+1" );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    } else {
        as->Asm("lda #<" + addr2 );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda #>" + addr2 );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    }

    // Shift increments
    if (m_node->m_params[2]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[2]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("shift increment is complex");
        LoadVar(as, 2);
    }
    as->Asm("sta vbmX");
    // Height of sprite
    if (m_node->m_params[3]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[3]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("sprite height is complex");
        LoadVar(as, 3);
    }
    as->Asm("sta vbmY");

    // address where to store lookups
    if (m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr3 );
        as->Asm("sta " + as->m_internalZP[2] );
        as->Asm("lda " + addr3 +"+1" );
        as->Asm("sta " + as->m_internalZP[2] + "+1" );
    } else {
        as->Asm("lda #<" + addr3 );
        as->Asm("sta " + as->m_internalZP[2] );
        as->Asm("lda #>" + addr3 );
        as->Asm("sta " + as->m_internalZP[2] + "+1" );
    }

    as->Asm("jsr vbmSpriteShiftL");

}
void Methods6502Vic20::vbmSpriteShiftR(Assembler* as)
{

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmSpriteShiftR","InitVbmSpriteShiftR");

    if (as->m_internalZP.count()<3)
        return;

    // address 1
    as->Comment("Read address 1");
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr1 = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr1 = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr1 = var->getValue(as);

    // address 2
    as->Comment("Read address 2");
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[1]);
    if (var==nullptr && !m_node->m_params[1]->isPureNumeric()) {
        ErrorHandler::e.Error("Second parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr2= "";
    if (m_node->m_params[1]->isPureNumeric())
        addr2 = m_node->m_params[1]->HexValue();
    if (var!=nullptr)
        addr2 = var->getValue(as);

    // address 3
    as->Comment("Read address 3");
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[4]);
    if (var==nullptr && !m_node->m_params[4]->isPureNumeric()) {
        ErrorHandler::e.Error("fifth parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr3= "";
    if (m_node->m_params[4]->isPureNumeric())
        addr3 = m_node->m_params[4]->HexValue();
    if (var!=nullptr)
        addr3 = var->getValue(as);

    as->Comment("Sprite Shift Right");
    // load addr 1, addr 2 and height

    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr1 );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda " + addr1 +"+1" );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    } else {
        as->Asm("lda #<" + addr1 );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda #>" + addr1 );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    }

    if (m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr2 );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda " + addr2 +"+1" );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    } else {
        as->Asm("lda #<" + addr2 );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda #>" + addr2 );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    }
    // Shift increments
    if (m_node->m_params[2]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[2]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("shift increment is complex");
        LoadVar(as, 2);
    }
    as->Asm("sta vbmX");
    // Height of sprite
    if (m_node->m_params[3]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[3]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("sprite height is complex");
        LoadVar(as, 3);
    }
    as->Asm("sta vbmY");

    // address where to store lookups
    if (m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr3 );
        as->Asm("sta " + as->m_internalZP[2] );
        as->Asm("lda " + addr3 +"+1" );
        as->Asm("sta " + as->m_internalZP[2] + "+1" );
    } else {
        as->Asm("lda #<" + addr3 );
        as->Asm("sta " + as->m_internalZP[2] );
        as->Asm("lda #>" + addr3 );
        as->Asm("sta " + as->m_internalZP[2] + "+1" );
    }

    as->Asm("jsr vbmSpriteShiftR");

}

void Methods6502Vic20::initVbmSpriteShiftSL(Assembler *as)
{
    if (m_node->m_isInitialized["vbmSpriteShiftSL"])
        return;

    m_node->m_isInitialized["vbmSpriteShiftSL"] = true;

    if (as->m_internalZP.count()==0)
        return;
    if (as->m_internalZP.count() < 3) {
        ErrorHandler::e.Error("This TRSE command needs at least 3 temporary ZP pointers but has less. Check the TRSE settings for temporary pointers.", m_node->m_op.m_lineNumber);
    }

    // p1 (src) p2 (dest)
    // vbmX and vbmY
    as->Comment("VBM - create a single left shifted position for a sprite");
    as->Comment("Source = "+ as->m_internalZP[0]);
    as->Comment("Destination = "+ as->m_internalZP[1]);
    as->Comment("vbmX = shift increment value 0-7");
    as->Comment("vbmY = height of sprite to shift");


    as->Label("vbmSpriteShiftSL");

    as->Asm("lda vbmX");
    as->Asm("sta vbmI	; current shift iteration");

    as->Comment("copy char");
    as->Label("vbmSSSLCopyChar");
    as->Asm("ldy #0");

    as->Label("vbmSSSLCopyCharLoop");
    as->Asm("lda (" + as->m_internalZP[0] + "),y	; read");
    as->Asm("sta (" + as->m_internalZP[1] + "),y	; copy to 1st column");
    as->Asm("iny");
    as->Asm("cpy vbmY");
    as->Asm("bne vbmSSSLCopyCharLoop");

    as->Comment("shift char");
    as->Asm("lda vbmI	; skip first shift");
    as->Asm("cmp #9");
    as->Asm("beq vbmSSSLShiftDone");

    as->Asm("ldy #0");
    as->Label("vbmSSSLShiftLoop");
    as->Asm("clc");
    as->Asm("lda #8");
    as->Asm("sbc vbmI");
    as->Asm("sta vbmT	; number of times to shift");

    as->Asm("lda (" + as->m_internalZP[1] + "),y ; read");

    as->Label("vbmSSSLShiftLoop2");
    as->Asm("asl");
    as->Asm("dec vbmT");
    as->Asm("bpl vbmSSSLShiftLoop2");

    as->Asm("sta (" + as->m_internalZP[1] + "),y ; store");

    as->Asm("iny");
    as->Asm("cpy vbmY");
    as->Asm("bne vbmSSSLShiftLoop ; repeat rotating all rows");

    // completed a single shifted sprite
    as->Label("vbmSSSLShiftDone");
    as->Comment("store address of pre-shifted sprite in look up table");
    // copy temp pointer 1 (destination) into temp pointer 2 (lookup table)
    as->Asm("ldy #0");
    as->Asm("lda " + as->m_internalZP[1]);
    as->Asm("sta (" + as->m_internalZP[2] + "),y");
    as->Asm("iny");
    as->Asm("lda " + as->m_internalZP[1] + "+1");
    as->Asm("sta (" + as->m_internalZP[2] + "),y");
    // add 2 (as we are storing low/high in serial list for simplicity)
    as->Asm("lda " + as->m_internalZP[2]);
    as->Asm("clc");
    as->Asm("adc #2");
    as->Asm("bcc vbmSSSL_p3overflow");
    as->Asm("inc " + as->m_internalZP[2] + "+1");
    as->Label("vbmSSSL_p3overflow");
    as->Asm("sta " + as->m_internalZP[2]);

    //; done
}
void Methods6502Vic20::initVbmSpriteShiftSR(Assembler *as)
{
    if (m_node->m_isInitialized["vbmSpriteShiftSR"])
        return;

    m_node->m_isInitialized["vbmSpriteShiftSR"] = true;

    if (as->m_internalZP.count()==0)
        return;
    if (as->m_internalZP.count() < 3) {
        ErrorHandler::e.Error("This TRSE command needs at least 3 temporary ZP pointers but has less. Check the TRSE settings for temporary pointers.", m_node->m_op.m_lineNumber);
    }

    // p1 (src) p2 (dest)
    // vbmX and vbmY
    as->Comment("VBM - create a single right shifted position for a sprite");
    as->Comment("Source = "+ as->m_internalZP[0]);
    as->Comment("Destination = "+ as->m_internalZP[1]);
    as->Comment("vbmX = shift increment value 0-7");
    as->Comment("vbmY = height of sprite to shift");


    as->Label("vbmSpriteShiftSR");

    as->Asm("lda vbmX");
    as->Asm("sta vbmI	; current shift iteration");

    as->Comment("copy char");
    as->Label("vbmSSSRCopyChar");
    as->Asm("ldy #0");

    as->Label("vbmSSSRCopyCharLoop");
    as->Asm("lda (" + as->m_internalZP[0] + "),y	; read");
    as->Asm("sta (" + as->m_internalZP[1] + "),y	; copy to 1st column");
    as->Asm("iny");
    as->Asm("cpy vbmY");
    as->Asm("bne vbmSSSRCopyCharLoop");

    as->Comment("shift char");
    as->Asm("lda vbmI	; skip first shift");
    as->Asm("beq vbmSSSRShiftDone");

    as->Asm("ldy #0");
    as->Label("vbmSSSRShiftLoop");
    as->Asm("lda vbmI");
    as->Asm("sta vbmT	; number of times to shift");

    as->Asm("lda (" + as->m_internalZP[1] + "),y ; read");

    as->Label("vbmSSSRShiftLoop2");
    as->Asm("lsr");
    as->Asm("dec vbmT");
    as->Asm("bne vbmSSSRShiftLoop2");

    as->Asm("sta (" + as->m_internalZP[1] + "),y ; store");

    as->Asm("iny");
    as->Asm("cpy vbmY");
    as->Asm("bne vbmSSSRShiftLoop ; repeat rotating all rows");

    // completed a single shifted sprite
    as->Label("vbmSSSRShiftDone");
    as->Comment("store address of pre-shifted sprite in look up table");
    // copy temp pointer 1 (destination) into temp pointer 2 (lookup table)
    as->Asm("ldy #0");
    as->Asm("lda " + as->m_internalZP[1]);
    as->Asm("sta (" + as->m_internalZP[2] + "),y");
    as->Asm("iny");
    as->Asm("lda " + as->m_internalZP[1] + "+1");
    as->Asm("sta (" + as->m_internalZP[2] + "),y");
    // add 2 (as we are storing low/high in serial list for simplicity)
    as->Asm("lda " + as->m_internalZP[2]);
    as->Asm("clc");
    as->Asm("adc #2");
    as->Asm("bcc vbmSSSR_p3overflow");
    as->Asm("inc " + as->m_internalZP[2] + "+1");
    as->Label("vbmSSSR_p3overflow");
    as->Asm("sta " + as->m_internalZP[2]);

    //; done
}
void Methods6502Vic20::vbmSpriteShiftSL(Assembler* as)
{

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmSpriteShiftSL","InitVbmSpriteShiftSL");

    if (as->m_internalZP.count()<3)
        return;

    // address 1
    as->Comment("Read address 1");
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr1 = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr1 = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr1 = var->getValue(as);

    // address 2
    as->Comment("Read address 2");
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[1]);
    if (var==nullptr && !m_node->m_params[1]->isPureNumeric()) {
        ErrorHandler::e.Error("Second parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr2= "";
    if (m_node->m_params[1]->isPureNumeric())
        addr2 = m_node->m_params[1]->HexValue();
    if (var!=nullptr)
        addr2 = var->getValue(as);

    // address 3
    as->Comment("Read address 3");
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[4]);
    if (var==nullptr && !m_node->m_params[4]->isPureNumeric()) {
        ErrorHandler::e.Error("fifth parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr3= "";
    if (m_node->m_params[4]->isPureNumeric())
        addr3 = m_node->m_params[4]->HexValue();
    if (var!=nullptr)
        addr3 = var->getValue(as);

    as->Comment("Single Sprite Shift Left");
    // load addr 1, addr 2 and height

    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr1 );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda " + addr1 +"+1" );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    } else {
        as->Asm("lda #<" + addr1 );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda #>" + addr1 );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    }

    if (m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr2 );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda " + addr2 +"+1" );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    } else {
        as->Asm("lda #<" + addr2 );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda #>" + addr2 );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    }

    // Shift position
    if (m_node->m_params[2]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[2]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("shift position is complex");
        LoadVar(as, 2);
    }
    as->Asm("sta vbmX");
    // Height of sprite
    if (m_node->m_params[3]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[3]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("sprite height is complex");
        LoadVar(as, 3);
    }
    as->Asm("sta vbmY");

    // address where to store lookups
    if (m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr3 );
        as->Asm("sta " + as->m_internalZP[2] );
        as->Asm("lda " + addr3 +"+1" );
        as->Asm("sta " + as->m_internalZP[2] + "+1" );
    } else {
        as->Asm("lda #<" + addr3 );
        as->Asm("sta " + as->m_internalZP[2] );
        as->Asm("lda #>" + addr3 );
        as->Asm("sta " + as->m_internalZP[2] + "+1" );
    }

    as->Asm("jsr vbmSpriteShiftSL");

}
void Methods6502Vic20::vbmSpriteShiftSR(Assembler* as)
{

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmSpriteShiftSR","InitVbmSpriteShiftSR");

    if (as->m_internalZP.count()<3)
        return;

    // address 1
    as->Comment("Read address 1");
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr1 = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr1 = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr1 = var->getValue(as);

    // address 2
    as->Comment("Read address 2");
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[1]);
    if (var==nullptr && !m_node->m_params[1]->isPureNumeric()) {
        ErrorHandler::e.Error("Second parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr2= "";
    if (m_node->m_params[1]->isPureNumeric())
        addr2 = m_node->m_params[1]->HexValue();
    if (var!=nullptr)
        addr2 = var->getValue(as);

    // address 3
    as->Comment("Read address 3");
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[4]);
    if (var==nullptr && !m_node->m_params[4]->isPureNumeric()) {
        ErrorHandler::e.Error("fifth parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr3= "";
    if (m_node->m_params[4]->isPureNumeric())
        addr3 = m_node->m_params[4]->HexValue();
    if (var!=nullptr)
        addr3 = var->getValue(as);

    as->Comment("Single Sprite Shift Right");
    // load addr 1, addr 2 and height

    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr1 );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda " + addr1 +"+1" );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    } else {
        as->Asm("lda #<" + addr1 );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda #>" + addr1 );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    }

    if (m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr2 );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda " + addr2 +"+1" );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    } else {
        as->Asm("lda #<" + addr2 );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda #>" + addr2 );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    }
    // Shift Position
    if (m_node->m_params[2]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[2]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("shift position is complex");
        LoadVar(as, 2);
    }
    as->Asm("sta vbmX");
    // Height of sprite
    if (m_node->m_params[3]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[3]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("sprite height is complex");
        LoadVar(as, 3);
    }
    as->Asm("sta vbmY");

    // address where to store lookups
    if (m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr3 );
        as->Asm("sta " + as->m_internalZP[2] );
        as->Asm("lda " + addr3 +"+1" );
        as->Asm("sta " + as->m_internalZP[2] + "+1" );
    } else {
        as->Asm("lda #<" + addr3 );
        as->Asm("sta " + as->m_internalZP[2] );
        as->Asm("lda #>" + addr3 );
        as->Asm("sta " + as->m_internalZP[2] + "+1" );
    }

    as->Asm("jsr vbmSpriteShiftSR");

}

void Methods6502Vic20::initVbmDrawSprite8(Assembler *as)
{
    if (m_node->m_isInitialized["vbmDrawSprite8"])
        return;

    m_node->m_isInitialized["vbmDrawSprite8"] = true;

    if (as->m_internalZP.count()==0)
        return;
    if (as->m_internalZP.count() < 2) {
        ErrorHandler::e.Error("This TRSE command needs at least 2 temporary ZP pointers but has less. Check the TRSE settings for temporary pointers.", m_node->m_op.m_lineNumber);
    }

    // p1 (src) p2 (dest)
    // vbmX and vbmY
    as->Comment("VBM - draw an 8x8 sprite - use vbmSetPosition first");
    as->Comment("Left Side = "+ as->m_internalZP[0]);
    as->Comment("Right side = "+ as->m_internalZP[1]);

    as->Label("vbmDrawSprite8");

        as->Comment("draw left side");
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");

        as->Comment("move screenmemory to next column");
        as->Asm("lda screenmemory");
        as->Asm("clc");
        as->Asm("adc vbmScrHeight ; #192 ; next column");
        as->Asm("bcc vbmDS8_overflow");
        as->Asm("inc screenmemory+1");
    as->Label("vbmDS8_overflow");
        as->Asm("sta screenmemory");


        // may not need to draw right side of 8x8 sprite
        as->Asm("lda vbmX");
        as->Asm("bne vbmDS8_Right");
        as->Asm("rts ; in position 0 there is no right side to draw");

    as->Label("vbmDS8_Right");

        as->Comment("draw right side");
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");

    //; done
}
void Methods6502Vic20::vbmDrawSprite8(Assembler* as)
{

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmDrawSprite8","InitVbmDrawSprite8");

    if (m_node->m_isInitialized["vbmScrollLeft"] || m_node->m_isInitialized["vbmScrollRight"])
        ErrorHandler::e.Error("vbmscrollright and vbmscrollleft are not compatible with this sprite command. Use Sprite Slices instead.", m_node->m_op.m_lineNumber);

    if (as->m_internalZP.count()==0)
        return;

    // address 1
    as->Comment("Read address 1");
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr1 = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr1 = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr1 = var->getValue(as);

    // address 2
    as->Comment("Read address 2");
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[1]);
    if (var==nullptr && !m_node->m_params[1]->isPureNumeric()) {
        ErrorHandler::e.Error("Second parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr2= "";
    if (m_node->m_params[1]->isPureNumeric())
        addr2 = m_node->m_params[1]->HexValue();
    if (var!=nullptr)
        addr2 = var->getValue(as);


    as->Asm("lda vbmX ; x offset 0-7");
    as->Asm("asl ; for simplicty, storing lo, hi in one array");
    if (m_node->m_params[0]->getType(as)==TokenType::POINTER
            || m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("tay");
    }
    if (m_node->m_params[0]->getType(as)!=TokenType::POINTER
            || m_node->m_params[1]->getType(as)!=TokenType::POINTER) {
        as->Asm("tax");
    }


    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Asm("lda (" + addr1 +"),y" );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("iny");
        as->Asm("lda (" + addr1 +"),y" );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
        if (m_node->m_params[1]->getType(as)==TokenType::POINTER)
        { as->Asm("dey"); }
    } else {
        as->Asm("lda " + addr1 +",x" ); //#<
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda " + addr1 +"+1,x" ); //#>
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    }

    if (m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("lda (" + addr2 +"),y" );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("iny");
        as->Asm("lda (" + addr2 +"),y" );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    } else {
        as->Asm("lda " + addr2 +",x" ); //#<
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda " + addr2 +"+1,x" ); //#>
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    }

    as->Asm("jsr vbmDrawSprite8");

}

void Methods6502Vic20::initVbmDrawSprite8E(Assembler *as)
{
    if (m_node->m_isInitialized["vbmDrawSprite8E"])
        return;

    m_node->m_isInitialized["vbmDrawSprite8E"] = true;

    if (as->m_internalZP.count()==0)
        return;
    if (as->m_internalZP.count() < 2) {
        ErrorHandler::e.Error("This TRSE command needs at least 2 temporary ZP pointers but has less. Check the TRSE settings for temporary pointers.", m_node->m_op.m_lineNumber);
    }

    // p1 (src) p2 (dest)
    // vbmX and vbmY
    as->Comment("VBM - draw an 8x8 sprite with EOR - use vbmSetPosition first");
    as->Comment("Left Side = "+ as->m_internalZP[0]);
    as->Comment("Right side = "+ as->m_internalZP[1]);

    as->Label("vbmDrawSprite8E");

        as->Comment("draw left side");
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");

        as->Comment("move screenmemory to next column");
        as->Asm("lda screenmemory");
        as->Asm("clc");
        as->Asm("adc vbmScrHeight ; #192 ; next column");
        as->Asm("bcc vbmDS8E_overflow");
        as->Asm("inc screenmemory+1");
    as->Label("vbmDS8E_overflow");
        as->Asm("sta screenmemory");

        // may not need to draw right side of 8x8 sprite
        as->Asm("lda vbmX");
        as->Asm("bne vbmDS8E_Right");
        as->Asm("rts ; in position 0 there is no right side to draw");

    as->Label("vbmDS8E_Right");

        as->Comment("draw right side");
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");

    //; done
}
void Methods6502Vic20::vbmDrawSprite8E(Assembler* as)
{

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmDrawSprite8E","InitVbmDrawSprite8E");

    if (m_node->m_isInitialized["vbmScrollLeft"] || m_node->m_isInitialized["vbmScrollRight"])
        ErrorHandler::e.Error("vbmscrollright and vbmscrollleft are not compatible with this sprite command. Use Sprite Slices instead.", m_node->m_op.m_lineNumber);

    if (as->m_internalZP.count()==0)
        return;

    // address 1
    as->Comment("Read address 1");
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr1 = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr1 = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr1 = var->getValue(as);

    // address 2
    as->Comment("Read address 2");
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[1]);
    if (var==nullptr && !m_node->m_params[1]->isPureNumeric()) {
        ErrorHandler::e.Error("Second parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr2= "";
    if (m_node->m_params[1]->isPureNumeric())
        addr2 = m_node->m_params[1]->HexValue();
    if (var!=nullptr)
        addr2 = var->getValue(as);


    as->Asm("lda vbmX ; x offset 0-7");
    as->Asm("asl ; for simplicty, storing lo, hi in one array");
    if (m_node->m_params[0]->getType(as)==TokenType::POINTER
            || m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("tay");
    }
    if (m_node->m_params[0]->getType(as)!=TokenType::POINTER
            || m_node->m_params[1]->getType(as)!=TokenType::POINTER) {
        as->Asm("tax");
    }


    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Asm("lda (" + addr1 +"),y" );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("iny");
        as->Asm("lda (" + addr1 +"),y" );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
        if (m_node->m_params[1]->getType(as)==TokenType::POINTER)
        { as->Asm("dey"); }
    } else {
        as->Asm("lda " + addr1 +",x" ); //#<
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda " + addr1 +"+1,x" ); //#>
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    }

    if (m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("lda (" + addr2 +"),y" );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("iny");
        as->Asm("lda (" + addr2 +"),y" );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    } else {
        as->Asm("lda " + addr2 +",x" ); //#<
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda " + addr2 +"+1,x" ); //#>
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    }
    as->Asm("jsr vbmDrawSprite8E");

}

void Methods6502Vic20::initVbmClearSprite8(Assembler *as)
{
    if (m_node->m_isInitialized["vbmClearSprite8"])
        return;

    m_node->m_isInitialized["vbmClearSprite8"] = true;

    if (as->m_internalZP.count()==0)
        return;
    if (as->m_internalZP.count() < 2) {
        ErrorHandler::e.Error("This TRSE command needs at least 2 temporary ZP pointers but has less. Check the TRSE settings for temporary pointers.", m_node->m_op.m_lineNumber);
    }

    // p1 (src) p2 (dest)
    // vbmX and vbmY
    as->Comment("VBM - clear (cut out) an 8x8 sprite - use vbmSetPosition first");
    as->Comment("Left Side = "+ as->m_internalZP[0]);
    as->Comment("Right side = "+ as->m_internalZP[1]);

    as->Label("vbmClearSprite8");

        as->Comment("draw left side");
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");

        as->Comment("move screenmemory to next column");
        as->Asm("lda screenmemory");
        as->Asm("clc");
        as->Asm("adc vbmScrHeight ; #192 ; next column");
        as->Asm("bcc vbmCS8_overflow");
        as->Asm("inc screenmemory+1");
    as->Label("vbmCS8_overflow");
        as->Asm("sta screenmemory");

        // may not need to draw right side of 8x8 sprite
        as->Asm("lda vbmX");
        as->Asm("bne vbmCS8_Right");
        as->Asm("rts ; in position 0 there is no right side to draw");

    as->Label("vbmCS8_Right");

        as->Comment("draw right side");
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");

    //; done
}
void Methods6502Vic20::vbmClearSprite8(Assembler* as)
{

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmClearSprite8","InitVbmClearSprite8");

    if (m_node->m_isInitialized["vbmScrollLeft"] || m_node->m_isInitialized["vbmScrollRight"])
        ErrorHandler::e.Error("vbmscrollright and vbmscrollleft are not compatible with this sprite command. Use Sprite Slices instead.", m_node->m_op.m_lineNumber);

    if (as->m_internalZP.count()==0)
        return;

    // address 1
    as->Comment("Read address 1");
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr1 = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr1 = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr1 = var->getValue(as);

    // address 2
    as->Comment("Read address 2");
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[1]);
    if (var==nullptr && !m_node->m_params[1]->isPureNumeric()) {
        ErrorHandler::e.Error("Second parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr2= "";
    if (m_node->m_params[1]->isPureNumeric())
        addr2 = m_node->m_params[1]->HexValue();
    if (var!=nullptr)
        addr2 = var->getValue(as);


    as->Asm("lda vbmX ; x offset 0-7");
    as->Asm("asl ; for simplicty, storing lo, hi in one array");
    if (m_node->m_params[0]->getType(as)==TokenType::POINTER
            || m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("tay");
    }
    if (m_node->m_params[0]->getType(as)!=TokenType::POINTER
            || m_node->m_params[1]->getType(as)!=TokenType::POINTER) {
        as->Asm("tax");
    }


    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Asm("lda (" + addr1 +"),y" );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("iny");
        as->Asm("lda (" + addr1 +"),y" );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
        if (m_node->m_params[1]->getType(as)==TokenType::POINTER)
        { as->Asm("dey"); }
    } else {
        as->Asm("lda " + addr1 +",x" ); //#<
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda " + addr1 +"+1,x" ); //#>
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    }

    if (m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("lda (" + addr2 +"),y" );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("iny");
        as->Asm("lda (" + addr2 +"),y" );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    } else {
        as->Asm("lda " + addr2 +",x" ); //#<
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda " + addr2 +"+1,x" ); //#>
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    }

    as->Asm("jsr vbmClearSprite8");

}


void Methods6502Vic20::initVbmDrawSprite(Assembler *as)
{
    if (m_node->m_isInitialized["vbmDrawSprite"])
        return;

    m_node->m_isInitialized["vbmDrawSprite"] = true;

    if (as->m_internalZP.count()==0)
        return;
    if (as->m_internalZP.count() < 2) {
        ErrorHandler::e.Error("This TRSE command needs at least 2 temporary ZP pointers but has less. Check the TRSE settings for temporary pointers.", m_node->m_op.m_lineNumber);
    }

    // p1 (src) p2 (dest)
    // vbmX and vbmY
    as->Comment("VBM - draw an 8x16 sprite - use vbmSetPosition first");
    as->Comment("Left Side = "+ as->m_internalZP[0]);
    as->Comment("Right side = "+ as->m_internalZP[1]);

    as->Label("vbmDrawSprite");

        as->Comment("draw left side");
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y"); // --
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");

        as->Comment("move screenmemory to next column");
        as->Asm("lda screenmemory");
        as->Asm("clc");
        as->Asm("adc vbmScrHeight ; #192 ; next column");
        as->Asm("bcc vbmDS_overflow");
        as->Asm("inc screenmemory+1");
    as->Label("vbmDS_overflow");
        as->Asm("sta screenmemory");


        // may not need to draw right side of 8x8 sprite
        as->Asm("lda vbmX");
        as->Asm("bne vbmDS_Right");
        as->Asm("rts ; in position 0 there is no right side to draw");

    as->Label("vbmDS_Right");

        as->Comment("draw right side");
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y"); // --
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");

    //; done
}
void Methods6502Vic20::vbmDrawSprite(Assembler* as)
{

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmDrawSprite","InitVbmDrawSprite");

    if (m_node->m_isInitialized["vbmScrollLeft"] || m_node->m_isInitialized["vbmScrollRight"])
        ErrorHandler::e.Error("vbmscrollright and vbmscrollleft are not compatible with this sprite command. Use Sprite Slices instead.", m_node->m_op.m_lineNumber);

    if (as->m_internalZP.count()==0)
        return;

    // address 1
    as->Comment("Read address 1");
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr1 = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr1 = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr1 = var->getValue(as);

    // address 2
    as->Comment("Read address 2");
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[1]);
    if (var==nullptr && !m_node->m_params[1]->isPureNumeric()) {
        ErrorHandler::e.Error("Second parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr2= "";
    if (m_node->m_params[1]->isPureNumeric())
        addr2 = m_node->m_params[1]->HexValue();
    if (var!=nullptr)
        addr2 = var->getValue(as);


    as->Asm("lda vbmX ; x offset 0-7");
    as->Asm("asl ; for simplicty, storing lo, hi in one array");
    if (m_node->m_params[0]->getType(as)==TokenType::POINTER
            || m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("tay");
    }
    if (m_node->m_params[0]->getType(as)!=TokenType::POINTER
            || m_node->m_params[1]->getType(as)!=TokenType::POINTER) {
        as->Asm("tax");
    }


    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Asm("lda (" + addr1 +"),y" );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("iny");
        as->Asm("lda (" + addr1 +"),y" );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
        if (m_node->m_params[1]->getType(as)==TokenType::POINTER)
        { as->Asm("dey"); }
    } else {
        as->Asm("lda " + addr1 +",x" ); //#<
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda " + addr1 +"+1,x" ); //#>
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    }

    if (m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("lda (" + addr2 +"),y" );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("iny");
        as->Asm("lda (" + addr2 +"),y" );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    } else {
        as->Asm("lda " + addr2 +",x" ); //#<
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda " + addr2 +"+1,x" ); //#>
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    }

    as->Asm("jsr vbmDrawSprite");

}

void Methods6502Vic20::initVbmDrawSpriteE(Assembler *as)
{
    if (m_node->m_isInitialized["vbmDrawSpriteE"])
        return;

    m_node->m_isInitialized["vbmDrawSpriteE"] = true;

    if (as->m_internalZP.count()==0)
        return;
    if (as->m_internalZP.count() < 2) {
        ErrorHandler::e.Error("This TRSE command needs at least 2 temporary ZP pointers but has less. Check the TRSE settings for temporary pointers.", m_node->m_op.m_lineNumber);
    }

    // p1 (src) p2 (dest)
    // vbmX and vbmY
    as->Comment("VBM - draw an 8x16 sprite with EOR - use vbmSetPosition first");
    as->Comment("Left Side = "+ as->m_internalZP[0]);
    as->Comment("Right side = "+ as->m_internalZP[1]);

    as->Label("vbmDrawSpriteE");

        as->Comment("draw left side");
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y"); // --
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");

        as->Comment("move screenmemory to next column");
        as->Asm("lda screenmemory");
        as->Asm("clc");
        as->Asm("adc vbmScrHeight ; #192 ; next column");
        as->Asm("bcc vbmDSE_overflow");
        as->Asm("inc screenmemory+1");
    as->Label("vbmDSE_overflow");
        as->Asm("sta screenmemory");

        // may not need to draw right side of 8x8 sprite
        as->Asm("lda vbmX");
        as->Asm("bne vbmDSE_Right");
        as->Asm("rts ; in position 0 there is no right side to draw");

    as->Label("vbmDSE_Right");

        as->Comment("draw right side");
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y"); // --
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");

    //; done
}
void Methods6502Vic20::vbmDrawSpriteE(Assembler* as)
{

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmDrawSpriteE","InitVbmDrawSpriteE");

    if (m_node->m_isInitialized["vbmScrollLeft"] || m_node->m_isInitialized["vbmScrollRight"])
        ErrorHandler::e.Error("vbmscrollright and vbmscrollleft are not compatible with this sprite command. Use Sprite Slices instead.", m_node->m_op.m_lineNumber);

    if (as->m_internalZP.count()==0)
        return;

    // address 1
    as->Comment("Read address 1");
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr1 = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr1 = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr1 = var->getValue(as);

    // address 2
    as->Comment("Read address 2");
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[1]);
    if (var==nullptr && !m_node->m_params[1]->isPureNumeric()) {
        ErrorHandler::e.Error("Second parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr2= "";
    if (m_node->m_params[1]->isPureNumeric())
        addr2 = m_node->m_params[1]->HexValue();
    if (var!=nullptr)
        addr2 = var->getValue(as);


    as->Asm("lda vbmX ; x offset 0-7");
    as->Asm("asl ; for simplicty, storing lo, hi in one array");
    if (m_node->m_params[0]->getType(as)==TokenType::POINTER
            || m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("tay");
    }
    if (m_node->m_params[0]->getType(as)!=TokenType::POINTER
            || m_node->m_params[1]->getType(as)!=TokenType::POINTER) {
        as->Asm("tax");
    }


    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Asm("lda (" + addr1 +"),y" );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("iny");
        as->Asm("lda (" + addr1 +"),y" );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
        if (m_node->m_params[1]->getType(as)==TokenType::POINTER)
        { as->Asm("dey"); }
    } else {
        as->Asm("lda " + addr1 +",x" ); //#<
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda " + addr1 +"+1,x" ); //#>
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    }

    if (m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("lda (" + addr2 +"),y" );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("iny");
        as->Asm("lda (" + addr2 +"),y" );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    } else {
        as->Asm("lda " + addr2 +",x" ); //#<
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda " + addr2 +"+1,x" ); //#>
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    }
    as->Asm("jsr vbmDrawSpriteE");

}

void Methods6502Vic20::initVbmClearSprite(Assembler *as)
{
    if (m_node->m_isInitialized["vbmClearSprite"])
        return;

    m_node->m_isInitialized["vbmClearSprite"] = true;

    if (as->m_internalZP.count()==0)
        return;
    if (as->m_internalZP.count() < 2) {
        ErrorHandler::e.Error("This TRSE command needs at least 2 temporary ZP pointers but has less. Check the TRSE settings for temporary pointers.", m_node->m_op.m_lineNumber);
    }

    // p1 (src) p2 (dest)
    // vbmX and vbmY
    as->Comment("VBM - clear (cut out) an 8x16 sprite - use vbmSetPosition first");
    as->Comment("Left Side = "+ as->m_internalZP[0]);
    as->Comment("Right side = "+ as->m_internalZP[1]);

    as->Label("vbmClearSprite");

        as->Comment("draw left side");
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y"); // --
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");

        as->Comment("move screenmemory to next column");
        as->Asm("lda screenmemory");
        as->Asm("clc");
        as->Asm("adc vbmScrHeight ; #192 ; next column");
        as->Asm("bcc vbmCS_overflow");
        as->Asm("inc screenmemory+1");
    as->Label("vbmCS_overflow");
        as->Asm("sta screenmemory");

        // may not need to draw right side of 8x8 sprite
        as->Asm("lda vbmX");
        as->Asm("bne vbmCS_Right");
        as->Asm("rts ; in position 0 there is no right side to draw");

    as->Label("vbmCS_Right");

        as->Comment("draw right side");
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y"); // --
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");


    //; done
}
void Methods6502Vic20::vbmClearSprite(Assembler* as)
{

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmClearSprite","InitVbmClearSprite");

    if (m_node->m_isInitialized["vbmScrollLeft"] || m_node->m_isInitialized["vbmScrollRight"])
        ErrorHandler::e.Error("vbmscrollright and vbmscrollleft are not compatible with this sprite command. Use Sprite Slices instead.", m_node->m_op.m_lineNumber);

    if (as->m_internalZP.count()==0)
        return;

    // address 1
    as->Comment("Read address 1");
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr1 = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr1 = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr1 = var->getValue(as);

    // address 2
    as->Comment("Read address 2");
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[1]);
    if (var==nullptr && !m_node->m_params[1]->isPureNumeric()) {
        ErrorHandler::e.Error("Second parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr2= "";
    if (m_node->m_params[1]->isPureNumeric())
        addr2 = m_node->m_params[1]->HexValue();
    if (var!=nullptr)
        addr2 = var->getValue(as);


    as->Asm("lda vbmX ; x offset 0-7");
    as->Asm("asl ; for simplicty, storing lo, hi in one array");
    if (m_node->m_params[0]->getType(as)==TokenType::POINTER
            || m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("tay");
    }
    if (m_node->m_params[0]->getType(as)!=TokenType::POINTER
            || m_node->m_params[1]->getType(as)!=TokenType::POINTER) {
        as->Asm("tax");
    }


    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Asm("lda (" + addr1 +"),y" );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("iny");
        as->Asm("lda (" + addr1 +"),y" );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
        if (m_node->m_params[1]->getType(as)==TokenType::POINTER)
        { as->Asm("dey"); }
    } else {
        as->Asm("lda " + addr1 +",x" ); //#<
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda " + addr1 +"+1,x" ); //#>
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    }

    if (m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("lda (" + addr2 +"),y" );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("iny");
        as->Asm("lda (" + addr2 +"),y" );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    } else {
        as->Asm("lda " + addr2 +",x" ); //#<
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda " + addr2 +"+1,x" ); //#>
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    }

    as->Asm("jsr vbmClearSprite");

}


void Methods6502Vic20::initVbmDrawSprite16(Assembler *as)
{
    if (m_node->m_isInitialized["vbmDrawSprite16"])
        return;

    m_node->m_isInitialized["vbmDrawSprite16"] = true;

    if (as->m_internalZP.count()==0)
        return;
    if (as->m_internalZP.count() < 3) {
        ErrorHandler::e.Error("This TRSE command needs at least 3 temporary ZP pointers but has less. Check the TRSE settings for temporary pointers.", m_node->m_op.m_lineNumber);
    }

    // p1 (src) p2 (dest)
    // vbmX and vbmY
    as->Comment("VBM - draw a 16x16 sprite with OR - use vbmSetPosition first");
    as->Comment("Left Side = "+ as->m_internalZP[0]);
    as->Comment("middle = "+ as->m_internalZP[1]);
    as->Comment("Right side = "+ as->m_internalZP[2]);

    as->Label("vbmDrawSprite16");

        as->Comment("draw left side");
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");

        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");


        as->Comment("move screenmemory to next column");
        as->Asm("lda screenmemory");
        as->Asm("clc");
        as->Asm("adc vbmScrHeight ; #192 ; next column");
        as->Asm("bcc vbmDS16_overflow");
        as->Asm("inc screenmemory+1");
    as->Label("vbmDS16_overflow");
        as->Asm("sta screenmemory");

    as->Label("vbmDS16_Middle");

        as->Comment("draw middle");
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");

        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");

        as->Comment("move screenmemory to next column");
        as->Asm("lda screenmemory");
        as->Asm("clc");
        as->Asm("adc vbmScrHeight ; #192 ; next column");
        as->Asm("bcc vbmDS16_overflow2");
        as->Asm("inc screenmemory+1");
    as->Label("vbmDS16_overflow2");
        as->Asm("sta screenmemory");

        // may not need to draw right side of 16x16 sprite
        as->Asm("lda vbmX");
        as->Asm("bne vbmDS16_Right");
        as->Asm("rts ; in position 0 there is no right side to draw");

    as->Label("vbmDS16_Right");

        as->Comment("draw right side");
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");

        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");

    //; done
}
void Methods6502Vic20::vbmDrawSprite16(Assembler* as)
{

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmDrawSprite16","InitVbmDrawSprite16");

    if (m_node->m_isInitialized["vbmScrollLeft"] || m_node->m_isInitialized["vbmScrollRight"])
        ErrorHandler::e.Error("vbmscrollright and vbmscrollleft are not compatible with this sprite command. Use Sprite Slices instead.", m_node->m_op.m_lineNumber);

    if (as->m_internalZP.count()==0)
        return;

    // address 1
    as->Comment("Read address 1");
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr1 = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr1 = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr1 = var->getValue(as);

    // address 2
    as->Comment("Read address 2");
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[1]);
    if (var==nullptr && !m_node->m_params[1]->isPureNumeric()) {
        ErrorHandler::e.Error("Second parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr2= "";
    if (m_node->m_params[1]->isPureNumeric())
        addr2 = m_node->m_params[1]->HexValue();
    if (var!=nullptr)
        addr2 = var->getValue(as);

    // address 3
    as->Comment("Read address 3");
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[2]);
    if (var==nullptr && !m_node->m_params[2]->isPureNumeric()) {
        ErrorHandler::e.Error("third parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr3= "";
    if (m_node->m_params[2]->isPureNumeric())
        addr3 = m_node->m_params[2]->HexValue();
    if (var!=nullptr)
        addr3 = var->getValue(as);



    as->Asm("lda vbmX ; x offset 0-7");
    as->Asm("asl ; for simplicty, storing lo, hi in one array");
    if (m_node->m_params[0]->getType(as)==TokenType::POINTER
            || m_node->m_params[1]->getType(as)==TokenType::POINTER
            || m_node->m_params[2]->getType(as)==TokenType::POINTER) {
        as->Asm("tay");
    }
    if (m_node->m_params[0]->getType(as)!=TokenType::POINTER
            || m_node->m_params[1]->getType(as)!=TokenType::POINTER
            || m_node->m_params[2]->getType(as)!=TokenType::POINTER) {
        as->Asm("tax");
    }


    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Asm("lda (" + addr1 +"),y" );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("iny");
        as->Asm("lda (" + addr1 +"),y" );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
        if (m_node->m_params[1]->getType(as)==TokenType::POINTER
                || m_node->m_params[2]->getType(as)==TokenType::POINTER)
        { as->Asm("dey"); }
    } else {
        as->Asm("lda " + addr1 +",x" ); //#<
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda " + addr1 +"+1,x" ); //#>
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    }

    if (m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("lda (" + addr2 +"),y" );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("iny");
        as->Asm("lda (" + addr2 +"),y" );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
        if (m_node->m_params[2]->getType(as)==TokenType::POINTER)
        { as->Asm("dey"); }
    } else {
        as->Asm("lda " + addr2 +",x" ); //#<
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda " + addr2 +"+1,x" ); //#>
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    }

    if (m_node->m_params[2]->getType(as)==TokenType::POINTER) {
        as->Asm("lda (" + addr3 +"),y" );
        as->Asm("sta " + as->m_internalZP[2] );
        as->Asm("iny");
        as->Asm("lda (" + addr3 +"),y" );
        as->Asm("sta " + as->m_internalZP[2] + "+1" );
    } else {
        as->Asm("lda " + addr3 +",x" ); //#<
        as->Asm("sta " + as->m_internalZP[2] );
        as->Asm("lda " + addr3 +"+1,x" ); //#>
        as->Asm("sta " + as->m_internalZP[2] + "+1" );
    }
    as->Asm("jsr vbmDrawSprite16");

}

void Methods6502Vic20::initVbmDrawSprite16E(Assembler *as)
{
    if (m_node->m_isInitialized["vbmDrawSprite16E"])
        return;

    m_node->m_isInitialized["vbmDrawSprite16E"] = true;

    if (as->m_internalZP.count()==0)
        return;
    if (as->m_internalZP.count() < 3) {
        ErrorHandler::e.Error("This TRSE command needs at least 3 temporary ZP pointers but has less. Check the TRSE settings for temporary pointers.", m_node->m_op.m_lineNumber);
    }

    // p1 (src) p2 (dest)
    // vbmX and vbmY
    as->Comment("VBM - draw a 16x16 sprite with EOR - use vbmSetPosition first");
    as->Comment("Left Side = "+ as->m_internalZP[0]);
    as->Comment("middle = "+ as->m_internalZP[1]);
    as->Comment("Right side = "+ as->m_internalZP[2]);

    as->Label("vbmDrawSprite16E");

        as->Comment("draw left side");
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");

        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");


        as->Comment("move screenmemory to next column");
        as->Asm("lda screenmemory");
        as->Asm("clc");
        as->Asm("adc vbmScrHeight ; #192 ; next column");
        as->Asm("bcc vbmDS16E_overflow");
        as->Asm("inc screenmemory+1");
    as->Label("vbmDS16E_overflow");
        as->Asm("sta screenmemory");

    as->Label("vbmDS16E_Middle");

        as->Comment("draw middle");
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");

        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");

        as->Comment("move screenmemory to next column");
        as->Asm("lda screenmemory");
        as->Asm("clc");
        as->Asm("adc vbmScrHeight ; #192 ; next column");
        as->Asm("bcc vbmDS16E_overflow2");
        as->Asm("inc screenmemory+1");
    as->Label("vbmDS16E_overflow2");
        as->Asm("sta screenmemory");

        // may not need to draw right side of 16x16 sprite
        as->Asm("lda vbmX");
        as->Asm("bne vbmDS16E_Right");
        as->Asm("rts ; in position 0 there is no right side to draw");

    as->Label("vbmDS16E_Right");

        as->Comment("draw right side");
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");

        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");

    //; done
}
void Methods6502Vic20::vbmDrawSprite16E(Assembler* as)
{

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmDrawSprite16E","InitVbmDrawSprite16E");

    if (m_node->m_isInitialized["vbmScrollLeft"] || m_node->m_isInitialized["vbmScrollRight"])
        ErrorHandler::e.Error("vbmscrollright and vbmscrollleft are not compatible with this sprite command. Use Sprite Slices instead.", m_node->m_op.m_lineNumber);

    if (as->m_internalZP.count()==0)
        return;

    // address 1
    as->Comment("Read address 1");
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr1 = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr1 = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr1 = var->getValue(as);

    // address 2
    as->Comment("Read address 2");
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[1]);
    if (var==nullptr && !m_node->m_params[1]->isPureNumeric()) {
        ErrorHandler::e.Error("Second parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr2= "";
    if (m_node->m_params[1]->isPureNumeric())
        addr2 = m_node->m_params[1]->HexValue();
    if (var!=nullptr)
        addr2 = var->getValue(as);

    // address 3
    as->Comment("Read address 3");
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[2]);
    if (var==nullptr && !m_node->m_params[2]->isPureNumeric()) {
        ErrorHandler::e.Error("third parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr3= "";
    if (m_node->m_params[2]->isPureNumeric())
        addr3 = m_node->m_params[2]->HexValue();
    if (var!=nullptr)
        addr3 = var->getValue(as);



    as->Asm("lda vbmX ; x offset 0-7");
    as->Asm("asl ; for simplicty, storing lo, hi in one array");
    if (m_node->m_params[0]->getType(as)==TokenType::POINTER
            || m_node->m_params[1]->getType(as)==TokenType::POINTER
            || m_node->m_params[2]->getType(as)==TokenType::POINTER) {
        as->Asm("tay");
    }
    if (m_node->m_params[0]->getType(as)!=TokenType::POINTER
            || m_node->m_params[1]->getType(as)!=TokenType::POINTER
            || m_node->m_params[2]->getType(as)!=TokenType::POINTER) {
        as->Asm("tax");
    }


    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Asm("lda (" + addr1 +"),y" );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("iny");
        as->Asm("lda (" + addr1 +"),y" );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
        if (m_node->m_params[1]->getType(as)==TokenType::POINTER
                || m_node->m_params[2]->getType(as)==TokenType::POINTER)
        { as->Asm("dey"); }
    } else {
        as->Asm("lda " + addr1 +",x" ); //#<
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda " + addr1 +"+1,x" ); //#>
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    }

    if (m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("lda (" + addr2 +"),y" );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("iny");
        as->Asm("lda (" + addr2 +"),y" );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
        if (m_node->m_params[2]->getType(as)==TokenType::POINTER)
        { as->Asm("dey"); }
    } else {
        as->Asm("lda " + addr2 +",x" ); //#<
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda " + addr2 +"+1,x" ); //#>
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    }

    if (m_node->m_params[2]->getType(as)==TokenType::POINTER) {
        as->Asm("lda (" + addr3 +"),y" );
        as->Asm("sta " + as->m_internalZP[2] );
        as->Asm("iny");
        as->Asm("lda (" + addr3 +"),y" );
        as->Asm("sta " + as->m_internalZP[2] + "+1" );
    } else {
        as->Asm("lda " + addr3 +",x" ); //#<
        as->Asm("sta " + as->m_internalZP[2] );
        as->Asm("lda " + addr3 +"+1,x" ); //#>
        as->Asm("sta " + as->m_internalZP[2] + "+1" );
    }
    as->Asm("jsr vbmDrawSprite16E");

}

void Methods6502Vic20::initVbmClearSprite16(Assembler *as)
{
    if (m_node->m_isInitialized["vbmClearSprite16"])
        return;

    m_node->m_isInitialized["vbmClearSprite16"] = true;

    if (as->m_internalZP.count()==0)
        return;
    if (as->m_internalZP.count() < 3) {
        ErrorHandler::e.Error("This TRSE command needs at least 3 temporary ZP pointers but has less. Check the TRSE settings for temporary pointers.", m_node->m_op.m_lineNumber);
    }

    // p1 (src) p2 (dest)
    // vbmX and vbmY
    as->Comment("VBM - Clear a 16x16 sprite with AND - use vbmSetPosition first");
    as->Comment("Left Side = "+ as->m_internalZP[0]);
    as->Comment("middle = "+ as->m_internalZP[1]);
    as->Comment("Right side = "+ as->m_internalZP[2]);

    as->Label("vbmClearSprite16");

        as->Comment("draw left side");
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");

        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");


        as->Comment("move screenmemory to next column");
        as->Asm("lda screenmemory");
        as->Asm("clc");
        as->Asm("adc vbmScrHeight ; #192 ; next column");
        as->Asm("bcc vbmCS16_overflow");
        as->Asm("inc screenmemory+1");
    as->Label("vbmCS16_overflow");
        as->Asm("sta screenmemory");

    as->Label("vbmCS16_Middle");

        as->Comment("draw middle");
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");

        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");

        as->Comment("move screenmemory to next column");
        as->Asm("lda screenmemory");
        as->Asm("clc");
        as->Asm("adc vbmScrHeight ; #192 ; next column");
        as->Asm("bcc vbmCS16_overflow2");
        as->Asm("inc screenmemory+1");
    as->Label("vbmCS16_overflow2");
        as->Asm("sta screenmemory");

        // may not need to draw right side of 16x16 sprite
        as->Asm("lda vbmX");
        as->Asm("bne vbmCS16_Right");
        as->Asm("rts ; in position 0 there is no right side to draw");

    as->Label("vbmCS16_Right");

        as->Comment("draw right side");
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");

        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");

    //; done
}
void Methods6502Vic20::vbmClearSprite16(Assembler* as)
{

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmClearSprite16","InitVbmClearSprite16");

    if (m_node->m_isInitialized["vbmScrollLeft"] || m_node->m_isInitialized["vbmScrollRight"])
        ErrorHandler::e.Error("vbmscrollright and vbmscrollleft are not compatible with this sprite command. Use Sprite Slices instead.", m_node->m_op.m_lineNumber);

    if (as->m_internalZP.count()==0)
        return;

    // address 1
    as->Comment("Read address 1");
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr1 = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr1 = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr1 = var->getValue(as);

    // address 2
    as->Comment("Read address 2");
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[1]);
    if (var==nullptr && !m_node->m_params[1]->isPureNumeric()) {
        ErrorHandler::e.Error("Second parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr2= "";
    if (m_node->m_params[1]->isPureNumeric())
        addr2 = m_node->m_params[1]->HexValue();
    if (var!=nullptr)
        addr2 = var->getValue(as);

    // address 3
    as->Comment("Read address 3");
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[2]);
    if (var==nullptr && !m_node->m_params[2]->isPureNumeric()) {
        ErrorHandler::e.Error("third parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr3= "";
    if (m_node->m_params[2]->isPureNumeric())
        addr3 = m_node->m_params[2]->HexValue();
    if (var!=nullptr)
        addr3 = var->getValue(as);



    as->Asm("lda vbmX ; x offset 0-7");
    as->Asm("asl ; for simplicty, storing lo, hi in one array");
    if (m_node->m_params[0]->getType(as)==TokenType::POINTER
            || m_node->m_params[1]->getType(as)==TokenType::POINTER
            || m_node->m_params[2]->getType(as)==TokenType::POINTER) {
        as->Asm("tay");
    }
    if (m_node->m_params[0]->getType(as)!=TokenType::POINTER
            || m_node->m_params[1]->getType(as)!=TokenType::POINTER
            || m_node->m_params[2]->getType(as)!=TokenType::POINTER) {
        as->Asm("tax");
    }


    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Asm("lda (" + addr1 +"),y" );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("iny");
        as->Asm("lda (" + addr1 +"),y" );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
        if (m_node->m_params[1]->getType(as)==TokenType::POINTER
                || m_node->m_params[2]->getType(as)==TokenType::POINTER)
        { as->Asm("dey"); }
    } else {
        as->Asm("lda " + addr1 +",x" ); //#<
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda " + addr1 +"+1,x" ); //#>
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    }

    if (m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("lda (" + addr2 +"),y" );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("iny");
        as->Asm("lda (" + addr2 +"),y" );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
        if (m_node->m_params[2]->getType(as)==TokenType::POINTER)
        { as->Asm("dey"); }
    } else {
        as->Asm("lda " + addr2 +",x" ); //#<
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda " + addr2 +"+1,x" ); //#>
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    }

    if (m_node->m_params[2]->getType(as)==TokenType::POINTER) {
        as->Asm("lda (" + addr3 +"),y" );
        as->Asm("sta " + as->m_internalZP[2] );
        as->Asm("iny");
        as->Asm("lda (" + addr3 +"),y" );
        as->Asm("sta " + as->m_internalZP[2] + "+1" );
    } else {
        as->Asm("lda " + addr3 +",x" ); //#<
        as->Asm("sta " + as->m_internalZP[2] );
        as->Asm("lda " + addr3 +"+1,x" ); //#>
        as->Asm("sta " + as->m_internalZP[2] + "+1" );
    }
    as->Asm("jsr vbmClearSprite16");

}


void Methods6502Vic20::initVbmDrawSprite2(Assembler *as)
{
    if (m_node->m_isInitialized["vbmDrawSprite2"])
        return;

    m_node->m_isInitialized["vbmDrawSprite2"] = true;

    if (as->m_internalZP.count()==0)
        return;
    if (as->m_internalZP.count() < 3) {
        ErrorHandler::e.Error("This TRSE command needs at least 3 temporary ZP pointers but has less. Check the TRSE settings for temporary pointers.", m_node->m_op.m_lineNumber);
    }

    // p1 (src) p2 (dest)
    // vbmX and vbmY
    as->Comment("VBM - draw a 16x8 sprite with OR - use vbmSetPosition first");
    as->Comment("Left Side = "+ as->m_internalZP[0]);
    as->Comment("middle = "+ as->m_internalZP[1]);
    as->Comment("Right side = "+ as->m_internalZP[2]);

    as->Label("vbmDrawSprite2");

        as->Comment("draw left side");
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");

        as->Comment("move screenmemory to next column");
        as->Asm("lda screenmemory");
        as->Asm("clc");
        as->Asm("adc vbmScrHeight ; #192 ; next column");
        as->Asm("bcc vbmDS2_overflow");
        as->Asm("inc screenmemory+1");
    as->Label("vbmDS2_overflow");
        as->Asm("sta screenmemory");

    as->Label("vbmDS2_Middle");

        as->Comment("draw middle");
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");

        as->Comment("move screenmemory to next column");
        as->Asm("lda screenmemory");
        as->Asm("clc");
        as->Asm("adc vbmScrHeight ; #192 ; next column");
        as->Asm("bcc vbmDS2_overflow2");
        as->Asm("inc screenmemory+1");
    as->Label("vbmDS2_overflow2");
        as->Asm("sta screenmemory");

        // may not need to draw right side of 16x8 sprite
        as->Asm("lda vbmX");
        as->Asm("bne vbmDS2_Right");
        as->Asm("rts ; in position 0 there is no right side to draw");

    as->Label("vbmDS2_Right");

        as->Comment("draw right side");
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");

    //; done
}
void Methods6502Vic20::vbmDrawSprite2(Assembler* as)
{

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmDrawSprite2","InitVbmDrawSprite2");

    if (m_node->m_isInitialized["vbmScrollLeft"] || m_node->m_isInitialized["vbmScrollRight"])
        ErrorHandler::e.Error("vbmscrollright and vbmscrollleft are not compatible with this sprite command. Use Sprite Slices instead.", m_node->m_op.m_lineNumber);

    if (as->m_internalZP.count()==0)
        return;

    // address 1
    as->Comment("Read address 1");
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr1 = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr1 = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr1 = var->getValue(as);

    // address 2
    as->Comment("Read address 2");
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[1]);
    if (var==nullptr && !m_node->m_params[1]->isPureNumeric()) {
        ErrorHandler::e.Error("Second parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr2= "";
    if (m_node->m_params[1]->isPureNumeric())
        addr2 = m_node->m_params[1]->HexValue();
    if (var!=nullptr)
        addr2 = var->getValue(as);

    // address 3
    as->Comment("Read address 3");
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[2]);
    if (var==nullptr && !m_node->m_params[2]->isPureNumeric()) {
        ErrorHandler::e.Error("third parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr3= "";
    if (m_node->m_params[2]->isPureNumeric())
        addr3 = m_node->m_params[2]->HexValue();
    if (var!=nullptr)
        addr3 = var->getValue(as);



    as->Asm("lda vbmX ; x offset 0-7");
    as->Asm("asl ; for simplicty, storing lo, hi in one array");
    if (m_node->m_params[0]->getType(as)==TokenType::POINTER
            || m_node->m_params[1]->getType(as)==TokenType::POINTER
            || m_node->m_params[2]->getType(as)==TokenType::POINTER) {
        as->Asm("tay");
    }
    if (m_node->m_params[0]->getType(as)!=TokenType::POINTER
            || m_node->m_params[1]->getType(as)!=TokenType::POINTER
            || m_node->m_params[2]->getType(as)!=TokenType::POINTER) {
        as->Asm("tax");
    }


    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Asm("lda (" + addr1 +"),y" );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("iny");
        as->Asm("lda (" + addr1 +"),y" );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
        if (m_node->m_params[1]->getType(as)==TokenType::POINTER
                || m_node->m_params[2]->getType(as)==TokenType::POINTER)
        { as->Asm("dey"); }
    } else {
        as->Asm("lda " + addr1 +",x" ); //#<
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda " + addr1 +"+1,x" ); //#>
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    }

    if (m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("lda (" + addr2 +"),y" );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("iny");
        as->Asm("lda (" + addr2 +"),y" );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
        if (m_node->m_params[2]->getType(as)==TokenType::POINTER)
        { as->Asm("dey"); }
    } else {
        as->Asm("lda " + addr2 +",x" ); //#<
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda " + addr2 +"+1,x" ); //#>
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    }

    if (m_node->m_params[2]->getType(as)==TokenType::POINTER) {
        as->Asm("lda (" + addr3 +"),y" );
        as->Asm("sta " + as->m_internalZP[2] );
        as->Asm("iny");
        as->Asm("lda (" + addr3 +"),y" );
        as->Asm("sta " + as->m_internalZP[2] + "+1" );
    } else {
        as->Asm("lda " + addr3 +",x" ); //#<
        as->Asm("sta " + as->m_internalZP[2] );
        as->Asm("lda " + addr3 +"+1,x" ); //#>
        as->Asm("sta " + as->m_internalZP[2] + "+1" );
    }
    as->Asm("jsr vbmDrawSprite2");

}

void Methods6502Vic20::initVbmDrawSprite2E(Assembler *as)
{
    if (m_node->m_isInitialized["vbmDrawSprite2E"])
        return;

    m_node->m_isInitialized["vbmDrawSprite2E"] = true;

    if (as->m_internalZP.count()==0)
        return;
    if (as->m_internalZP.count() < 3) {
        ErrorHandler::e.Error("This TRSE command needs at least 3 temporary ZP pointers but has less. Check the TRSE settings for temporary pointers.", m_node->m_op.m_lineNumber);
    }

    // p1 (src) p2 (dest)
    // vbmX and vbmY
    as->Comment("VBM - draw a 16x8 sprite with EOR - use vbmSetPosition first");
    as->Comment("Left Side = "+ as->m_internalZP[0]);
    as->Comment("middle = "+ as->m_internalZP[1]);
    as->Comment("Right side = "+ as->m_internalZP[2]);

    as->Label("vbmDrawSprite2E");

        as->Comment("draw left side");
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");

        as->Comment("move screenmemory to next column");
        as->Asm("lda screenmemory");
        as->Asm("clc");
        as->Asm("adc vbmScrHeight ; #192 ; next column");
        as->Asm("bcc vbmDS2E_overflow");
        as->Asm("inc screenmemory+1");
    as->Label("vbmDS2E_overflow");
        as->Asm("sta screenmemory");

    as->Label("vbmDS2E_Middle");

        as->Comment("draw middle");
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");

        as->Comment("move screenmemory to next column");
        as->Asm("lda screenmemory");
        as->Asm("clc");
        as->Asm("adc vbmScrHeight ; #192 ; next column");
        as->Asm("bcc vbmDS2E_overflow2");
        as->Asm("inc screenmemory+1");
    as->Label("vbmDS2E_overflow2");
        as->Asm("sta screenmemory");

        // may not need to draw right side of 16x16 sprite
        as->Asm("lda vbmX");
        as->Asm("bne vbmDS2E_Right");
        as->Asm("rts ; in position 0 there is no right side to draw");

    as->Label("vbmDS2E_Right");

        as->Comment("draw right side");
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");

    //; done
}
void Methods6502Vic20::vbmDrawSprite2E(Assembler* as)
{

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmDrawSprite2E","InitVbmDrawSprite2E");

    if (m_node->m_isInitialized["vbmScrollLeft"] || m_node->m_isInitialized["vbmScrollRight"])
        ErrorHandler::e.Error("vbmscrollright and vbmscrollleft are not compatible with this sprite command. Use Sprite Slices instead.", m_node->m_op.m_lineNumber);

    if (as->m_internalZP.count()==0)
        return;

    // address 1
    as->Comment("Read address 1");
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr1 = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr1 = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr1 = var->getValue(as);

    // address 2
    as->Comment("Read address 2");
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[1]);
    if (var==nullptr && !m_node->m_params[1]->isPureNumeric()) {
        ErrorHandler::e.Error("Second parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr2= "";
    if (m_node->m_params[1]->isPureNumeric())
        addr2 = m_node->m_params[1]->HexValue();
    if (var!=nullptr)
        addr2 = var->getValue(as);

    // address 3
    as->Comment("Read address 3");
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[2]);
    if (var==nullptr && !m_node->m_params[2]->isPureNumeric()) {
        ErrorHandler::e.Error("third parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr3= "";
    if (m_node->m_params[2]->isPureNumeric())
        addr3 = m_node->m_params[2]->HexValue();
    if (var!=nullptr)
        addr3 = var->getValue(as);



    as->Asm("lda vbmX ; x offset 0-7");
    as->Asm("asl ; for simplicty, storing lo, hi in one array");
    if (m_node->m_params[0]->getType(as)==TokenType::POINTER
            || m_node->m_params[1]->getType(as)==TokenType::POINTER
            || m_node->m_params[2]->getType(as)==TokenType::POINTER) {
        as->Asm("tay");
    }
    if (m_node->m_params[0]->getType(as)!=TokenType::POINTER
            || m_node->m_params[1]->getType(as)!=TokenType::POINTER
            || m_node->m_params[2]->getType(as)!=TokenType::POINTER) {
        as->Asm("tax");
    }


    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Asm("lda (" + addr1 +"),y" );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("iny");
        as->Asm("lda (" + addr1 +"),y" );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
        if (m_node->m_params[1]->getType(as)==TokenType::POINTER
                || m_node->m_params[2]->getType(as)==TokenType::POINTER)
        { as->Asm("dey"); }
    } else {
        as->Asm("lda " + addr1 +",x" ); //#<
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda " + addr1 +"+1,x" ); //#>
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    }

    if (m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("lda (" + addr2 +"),y" );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("iny");
        as->Asm("lda (" + addr2 +"),y" );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
        if (m_node->m_params[2]->getType(as)==TokenType::POINTER)
        { as->Asm("dey"); }
    } else {
        as->Asm("lda " + addr2 +",x" ); //#<
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda " + addr2 +"+1,x" ); //#>
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    }

    if (m_node->m_params[2]->getType(as)==TokenType::POINTER) {
        as->Asm("lda (" + addr3 +"),y" );
        as->Asm("sta " + as->m_internalZP[2] );
        as->Asm("iny");
        as->Asm("lda (" + addr3 +"),y" );
        as->Asm("sta " + as->m_internalZP[2] + "+1" );
    } else {
        as->Asm("lda " + addr3 +",x" ); //#<
        as->Asm("sta " + as->m_internalZP[2] );
        as->Asm("lda " + addr3 +"+1,x" ); //#>
        as->Asm("sta " + as->m_internalZP[2] + "+1" );
    }
    as->Asm("jsr vbmDrawSprite2E");

}

void Methods6502Vic20::initVbmClearSprite2(Assembler *as)
{
    if (m_node->m_isInitialized["vbmClearSprite2"])
        return;

    m_node->m_isInitialized["vbmClearSprite2"] = true;

    if (as->m_internalZP.count()==0)
        return;
    if (as->m_internalZP.count() < 3) {
        ErrorHandler::e.Error("This TRSE command needs at least 3 temporary ZP pointers but has less. Check the TRSE settings for temporary pointers.", m_node->m_op.m_lineNumber);
    }

    // p1 (src) p2 (dest)
    // vbmX and vbmY
    as->Comment("VBM - Clear a 16x8 sprite with AND - use vbmSetPosition first");
    as->Comment("Left Side = "+ as->m_internalZP[0]);
    as->Comment("middle = "+ as->m_internalZP[1]);
    as->Comment("Right side = "+ as->m_internalZP[2]);

    as->Label("vbmClearSprite2");

        as->Comment("draw left side");
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");

        as->Comment("move screenmemory to next column");
        as->Asm("lda screenmemory");
        as->Asm("clc");
        as->Asm("adc vbmScrHeight ; #192 ; next column");
        as->Asm("bcc vbmCS2_overflow");
        as->Asm("inc screenmemory+1");
    as->Label("vbmCS2_overflow");
        as->Asm("sta screenmemory");

    as->Label("vbmCS2_Middle");

        as->Comment("draw middle");
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[1] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");

        as->Comment("move screenmemory to next column");
        as->Asm("lda screenmemory");
        as->Asm("clc");
        as->Asm("adc vbmScrHeight ; #192 ; next column");
        as->Asm("bcc vbmCS2_overflow2");
        as->Asm("inc screenmemory+1");
    as->Label("vbmCS2_overflow2");
        as->Asm("sta screenmemory");

        // may not need to draw right side of 16x16 sprite
        as->Asm("lda vbmX");
        as->Asm("bne vbmCS2_Right");
        as->Asm("rts ; in position 0 there is no right side to draw");

    as->Label("vbmCS2_Right");

        as->Comment("draw right side");
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");

    //; done
}
void Methods6502Vic20::vbmClearSprite2(Assembler* as)
{

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmClearSprite2","InitVbmClearSprite2");

    if (m_node->m_isInitialized["vbmScrollLeft"] || m_node->m_isInitialized["vbmScrollRight"])
        ErrorHandler::e.Error("vbmscrollright and vbmscrollleft are not compatible with this sprite command. Use Sprite Slices instead.", m_node->m_op.m_lineNumber);

    if (as->m_internalZP.count()==0)
        return;

    // address 1
    as->Comment("Read address 1");
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr1 = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr1 = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr1 = var->getValue(as);

    // address 2
    as->Comment("Read address 2");
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[1]);
    if (var==nullptr && !m_node->m_params[1]->isPureNumeric()) {
        ErrorHandler::e.Error("Second parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr2= "";
    if (m_node->m_params[1]->isPureNumeric())
        addr2 = m_node->m_params[1]->HexValue();
    if (var!=nullptr)
        addr2 = var->getValue(as);

    // address 3
    as->Comment("Read address 3");
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[2]);
    if (var==nullptr && !m_node->m_params[2]->isPureNumeric()) {
        ErrorHandler::e.Error("third parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr3= "";
    if (m_node->m_params[2]->isPureNumeric())
        addr3 = m_node->m_params[2]->HexValue();
    if (var!=nullptr)
        addr3 = var->getValue(as);



    as->Asm("lda vbmX ; x offset 0-7");
    as->Asm("asl ; for simplicty, storing lo, hi in one array");
    if (m_node->m_params[0]->getType(as)==TokenType::POINTER
            || m_node->m_params[1]->getType(as)==TokenType::POINTER
            || m_node->m_params[2]->getType(as)==TokenType::POINTER) {
        as->Asm("tay");
    }
    if (m_node->m_params[0]->getType(as)!=TokenType::POINTER
            || m_node->m_params[1]->getType(as)!=TokenType::POINTER
            || m_node->m_params[2]->getType(as)!=TokenType::POINTER) {
        as->Asm("tax");
    }


    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Asm("lda (" + addr1 +"),y" );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("iny");
        as->Asm("lda (" + addr1 +"),y" );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
        if (m_node->m_params[1]->getType(as)==TokenType::POINTER
                || m_node->m_params[2]->getType(as)==TokenType::POINTER)
        { as->Asm("dey"); }
    } else {
        as->Asm("lda " + addr1 +",x" ); //#<
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda " + addr1 +"+1,x" ); //#>
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    }

    if (m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("lda (" + addr2 +"),y" );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("iny");
        as->Asm("lda (" + addr2 +"),y" );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
        if (m_node->m_params[2]->getType(as)==TokenType::POINTER)
        { as->Asm("dey"); }
    } else {
        as->Asm("lda " + addr2 +",x" ); //#<
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda " + addr2 +"+1,x" ); //#>
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    }

    if (m_node->m_params[2]->getType(as)==TokenType::POINTER) {
        as->Asm("lda (" + addr3 +"),y" );
        as->Asm("sta " + as->m_internalZP[2] );
        as->Asm("iny");
        as->Asm("lda (" + addr3 +"),y" );
        as->Asm("sta " + as->m_internalZP[2] + "+1" );
    } else {
        as->Asm("lda " + addr3 +",x" ); //#<
        as->Asm("sta " + as->m_internalZP[2] );
        as->Asm("lda " + addr3 +"+1,x" ); //#>
        as->Asm("sta " + as->m_internalZP[2] + "+1" );
    }
    as->Asm("jsr vbmClearSprite2");

}



void Methods6502Vic20::initVbmDrawSpriteSlice(Assembler *as)
{
    if (m_node->m_isInitialized["vbmDrawSpriteSlice"])
        return;

    m_node->m_isInitialized["vbmDrawSpriteSlice"] = true;

    if (as->m_internalZP.count()==0)
        return;
    if (as->m_internalZP.count() < 1) {
        ErrorHandler::e.Error("This TRSE command needs at least 1 temporary ZP pointers but has less. Check the TRSE settings for temporary pointers.", m_node->m_op.m_lineNumber);
    }

    as->Comment("VBM - draw an 8 x h sprite with OR - use vbmSetPosition first");
    as->Comment("Shifted sprite address = "+ as->m_internalZP[0]);

    as->Label("vbmDrawSpriteSlice");

        as->Asm("ldy vbmI	; line in sprite to start drawing from");

    as->Label("vbmDSS_draw");

        as->Comment("draw one slice");
        as->Asm("lda (" + as->m_internalZP[0] +"),y");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("cpy vbmT	; line is sprite to draw to (height)");
        as->Asm("bne vbmDSS_draw");

    //; done
}
void Methods6502Vic20::vbmDrawSpriteSlice(Assembler* as)
{

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmDrawSpriteSlice","InitVbmDrawSpriteSlice");

    if (as->m_internalZP.count()==0)
        return;

    // address 1
    as->Comment("Read address 1");
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr1 = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr1 = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr1 = var->getValue(as);

    as->Comment("read out the address");
    as->Asm("lda vbmX	; x offset");
    as->Asm("asl");

    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Asm("tay");
        as->Asm("lda (" + addr1 + "),y	; look up shifted sprite");
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("iny");
        as->Asm("lda (" + addr1 +"),y ;keep");
        as->Asm("sta " + as->m_internalZP[0] +"+1");
    } else {
        as->Asm("tax");
        as->Asm("lda " + addr1 +",x" ); //#<
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda " + addr1 +"+1,x" ); //#>
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    }

    // draw from line (start)
    if (m_node->m_params[1]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[1]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("start line is complex");
        LoadVar(as, 1);
    }
    as->Asm("sta vbmI");

    // draw to line (end)
    if (m_node->m_params[2]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[2]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("end line is complex");
        LoadVar(as, 2);
    }
    as->Asm("sta vbmT");

    as->Asm("jsr vbmDrawSpriteSlice");

}

void Methods6502Vic20::initVbmDrawSpriteSliceE(Assembler *as)
{
    if (m_node->m_isInitialized["vbmDrawSpriteSliceE"])
        return;

    m_node->m_isInitialized["vbmDrawSpriteSliceE"] = true;

    if (as->m_internalZP.count()==0)
        return;
    if (as->m_internalZP.count() < 1) {
        ErrorHandler::e.Error("This TRSE command needs at least 1 temporary ZP pointers but has less. Check the TRSE settings for temporary pointers.", m_node->m_op.m_lineNumber);
    }

    as->Comment("VBM - draw an 8 x h sprite with EOR - use vbmSetPosition first");
    as->Comment("Shifted sprite address = "+ as->m_internalZP[0]);

    as->Label("vbmDrawSpriteSliceE");

        as->Asm("ldy vbmI	; line in sprite to start drawing from");

    as->Label("vbmDSSE_draw");

        as->Comment("draw one slice");
        as->Asm("lda (" + as->m_internalZP[0] +"),y");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("cpy vbmT	; line is sprite to draw to (height)");
        as->Asm("bne vbmDSSE_draw");

    //; done
}
void Methods6502Vic20::vbmDrawSpriteSliceE(Assembler* as)
{

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmDrawSpriteSliceE","InitVbmDrawSpriteSliceE");

    if (as->m_internalZP.count()==0)
        return;

    // address 1
    as->Comment("Read address 1");
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr1 = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr1 = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr1 = var->getValue(as);

    as->Comment("read out the address");
    as->Asm("lda vbmX	; x offset");
    as->Asm("asl");

    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Asm("tay");
        as->Asm("lda (" + addr1 + "),y	; look up shifted sprite");
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("iny");
        as->Asm("lda (" + addr1 +"),y ;keep");
        as->Asm("sta " + as->m_internalZP[0] +"+1");
    } else {
        as->Asm("tax");
        as->Asm("lda " + addr1 +",x" ); //#<
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda " + addr1 +"+1,x" ); //#>
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    }

    // draw from line (start)
    if (m_node->m_params[1]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[1]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("start line is complex");
        LoadVar(as, 1);
    }
    as->Asm("sta vbmI");

    // draw to line (end)
    if (m_node->m_params[2]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[2]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("end line is complex");
        LoadVar(as, 2);
    }
    as->Asm("sta vbmT");

    as->Asm("jsr vbmDrawSpriteSliceE");

}

void Methods6502Vic20::initVbmClearSpriteSlice(Assembler *as)
{
    if (m_node->m_isInitialized["vbmClearSpriteSlice"])
        return;

    m_node->m_isInitialized["vbmClearSpriteSlice"] = true;

    if (as->m_internalZP.count()==0)
        return;
    if (as->m_internalZP.count() < 1) {
        ErrorHandler::e.Error("This TRSE command needs at least 1 temporary ZP pointers but has less. Check the TRSE settings for temporary pointers.", m_node->m_op.m_lineNumber);
    }

    as->Comment("VBM - Clear an 8 x h sprite with AND - use vbmSetPosition first");
    as->Comment("Shifted sprite address = "+ as->m_internalZP[0]);

    as->Label("vbmClearSpriteSlice");

        as->Asm("ldy vbmI	; line in sprite to start drawing from");

    as->Label("vbmCSS_draw");

        as->Comment("draw one slice");
        as->Asm("lda (" + as->m_internalZP[0] +"),y");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("cpy vbmT	; line is sprite to draw to (height)");
        as->Asm("bne vbmCSS_draw");

    //; done
}
void Methods6502Vic20::vbmClearSpriteSlice(Assembler* as)
{

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmClearSpriteSlice","InitVbmClearSpriteSlice");

    if (as->m_internalZP.count()==0)
        return;

    // address 1
    as->Comment("Read address 1");
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr1 = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr1 = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr1 = var->getValue(as);

    as->Comment("read out the address");
    as->Asm("lda vbmX	; x offset");
    as->Asm("asl");

    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Asm("tay");
        as->Asm("lda (" + addr1 + "),y	; look up shifted sprite");
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("iny");
        as->Asm("lda (" + addr1 +"),y ;keep");
        as->Asm("sta " + as->m_internalZP[0] +"+1");
    } else {
        as->Asm("tax");
        as->Asm("lda " + addr1 +",x" ); //#<
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda " + addr1 +"+1,x" ); //#>
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    }

    // draw from line (start)
    if (m_node->m_params[1]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[1]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("start line is complex");
        LoadVar(as, 1);
    }
    as->Asm("sta vbmI");

    // draw to line (end)
    if (m_node->m_params[2]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[2]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("end line is complex");
        LoadVar(as, 2);
    }
    as->Asm("sta vbmT");

    as->Asm("jsr vbmClearSpriteSlice");

}

void Methods6502Vic20::initVbmDrawText(Assembler* as)
{
    if (m_node->m_isInitialized["vbmDrawText"])
        return;

    m_node->m_isInitialized["vbmDrawText"] = true;

    as->Comment("Draw text characters to the bitmap using a zero terminated CSTRING");
    as->Comment("CSTRING    = " + as->m_internalZP[0]);
    as->Comment("Font chars = " + as->m_internalZP[1]);
    as->Comment("Temp addr  = " + as->m_internalZP[2] + " - used to calculate char address");
    as->Label("vbmDrawText");

    as->Label("vbmDTX_Xloop");

        as->Comment("calculate next screen memory position");
        as->Asm("ldx vbmX");
        as->Asm("lda vbmScrL,x   ; Address of table lo");
        as->Asm("ldy vbmScrH,x   ; Address of table hi");
        as->Asm("clc");
        as->Asm("adc vbmY		; Add Y offset");
        as->Asm("bcc vbmDTX_NSP_NoOverflow");
        as->Asm("iny");

    as->Label("vbmDTX_NSP_NoOverflow");
        as->Asm("sta screenmemory");
        as->Asm("sty screenmemory+1");

    as->Label("vbmDTX_GetCharNum");
        as->Comment("convert text number (0-255) * 8 = memory offset");
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[0] +"),y		; get char from current position in CSTRING");
        as->Asm("bne vbmDTX_NotEnd");
        as->Asm("rts ; if =0, we are end of the cstring");

    as->Label("vbmDTX_NotEnd");

        as->Asm("sta " + as->m_internalZP[2]);
        as->Asm("sty " + as->m_internalZP[2] + "+1");

        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x2");
        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x4");
        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x8");

        as->Asm("lda " + as->m_internalZP[2] );
        as->Asm("clc");
        as->Asm("adc "+ as->m_internalZP[1] + "  ; add tile low address");
        as->Asm("sta " + as->m_internalZP[2] );
        as->Asm("lda " + as->m_internalZP[2] + "+1");
        as->Asm("adc " + as->m_internalZP[1] + "+1 ; add tile high address");
        as->Asm("sta " + as->m_internalZP[2] + "+1" );

    as->Label("vbmDTX_DrawChar");
        as->Comment("y reg is ZERO from ldy #0 in GetTileNum");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("sta (screenmemory),y");

    as->Label("vbmDTX_NextChar");
        as->Asm("clc");
        as->Asm("inc " + as->m_internalZP[0] + "  ; low byte");
        as->Asm("bne vbmDTX_NTM_NoOverflow");
        as->Asm("inc " + as->m_internalZP[0] + "+1  ; high byte");
    as->Label("vbmDTX_NTM_NoOverflow");
        as->Comment("next x pos on screen");
        as->Asm("inc vbmX");
        as->Asm("lda vbmNumColumns   ; 0-19 columns, 20 means exceeded right of screen");
        as->Asm("cmp vbmX  ; has x pos exceeded?");
        as->Asm("bne vbmDTX_Xloop  ; no, draw next char");

        as->Comment("yes, set x back to 0 and inc vbmY by line height (pixels)");
        as->Asm("lda #0");
        as->Asm("sta vbmX");

        as->Asm("lda vbmY");
        as->Asm("clc");
        as->Asm("adc vbmI");
        as->Asm("sta vbmY");

        as->Asm("jmp vbmDTX_Xloop");

}
void Methods6502Vic20::vbmDrawText(Assembler* as)
{
    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmDrawText","InitVbmDrawText");

    if (as->m_internalZP.count()==0)
        return;
    if (as->m_internalZP.count() < 3) {
        ErrorHandler::e.Error("This TRSE command needs at least 3 temporary ZP pointers but has less. Check the TRSE settings for temporary pointers.", m_node->m_op.m_lineNumber);
    }

    // address 1 - text
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr1 = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr1 = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr1 = var->getValue(as);

    // address 2 - chars (font)
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[1]);
    if (var==nullptr && !m_node->m_params[1]->isPureNumeric()) {
        ErrorHandler::e.Error("Second parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr2= "";
    if (m_node->m_params[1]->isPureNumeric())
        addr2 = m_node->m_params[1]->HexValue();
    if (var!=nullptr)
        addr2 = var->getValue(as);

    as->Comment("Draw 8x8 text to the bitmap");

    as->Comment("Text to use:");
    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr1 );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda " + addr1 +"+1" );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    } else {
        as->Asm("lda #<" + addr1 );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda #>" + addr1 );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    }
    as->Comment("Font characters to use:");
    if (m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr2 );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda " + addr2 +"+1" );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    } else {
        as->Asm("lda #<" + addr2 );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda #>" + addr2 );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    }

    //  X
    if (m_node->m_params[2]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[2]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("x is complex");
        LoadVar(as, 2);
    }
    as->Asm("sta vbmX ; x position");

    //  Y
    if (m_node->m_params[3]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[3]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("y is complex");
        LoadVar(as, 3);
    }
    as->Asm("sta vbmY ; y position in pixels");

    // Line height
    if (m_node->m_params[4]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[4]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("Line height is complex");
        LoadVar(as, 4);
    }
    as->Asm("sta vbmI ; line height in pixels");

    as->Asm("jsr vbmDrawText");

}

void Methods6502Vic20::initVbmDrawTextO(Assembler* as)
{
    if (m_node->m_isInitialized["vbmDrawTextO"])
        return;

    m_node->m_isInitialized["vbmDrawTextO"] = true;

    as->Comment("Draw text characters to the bitmap using a zero terminated CSTRING with OR operation");
    as->Comment("CSTRING    = " + as->m_internalZP[0]);
    as->Comment("Font chars = " + as->m_internalZP[1]);
    as->Comment("Temp addr  = " + as->m_internalZP[2] + " - used to calculate char address");
    as->Label("vbmDrawTextO");

    as->Label("vbmDTXO_Xloop");

        as->Comment("calculate next screen memory position");
        as->Asm("ldx vbmX");
        as->Asm("lda vbmScrL,x   ; Address of table lo");
        as->Asm("ldy vbmScrH,x   ; Address of table hi");
        as->Asm("clc");
        as->Asm("adc vbmY		; Add Y offset");
        as->Asm("bcc vbmDTXO_NSP_NoOverflow");
        as->Asm("iny");

    as->Label("vbmDTXO_NSP_NoOverflow");
        as->Asm("sta screenmemory");
        as->Asm("sty screenmemory+1");

    as->Label("vbmDTXO_GetCharNum");
        as->Comment("convert text number (0-255) * 8 = memory offset");
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[0] +"),y		; get char from current position in CSTRING");
        as->Asm("bne vbmDTXO_NotEnd");
        as->Asm("rts ; if =0, we are end of the cstring");

    as->Label("vbmDTXO_NotEnd");

        as->Asm("sta " + as->m_internalZP[2]);
        as->Asm("sty " + as->m_internalZP[2] + "+1");

        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x2");
        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x4");
        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x8");

        as->Asm("lda " + as->m_internalZP[2] );
        as->Asm("clc");
        as->Asm("adc "+ as->m_internalZP[1] + "  ; add tile low address");
        as->Asm("sta " + as->m_internalZP[2] );
        as->Asm("lda " + as->m_internalZP[2] + "+1");
        as->Asm("adc " + as->m_internalZP[1] + "+1 ; add tile high address");
        as->Asm("sta " + as->m_internalZP[2] + "+1" );

    as->Label("vbmDTXO_DrawChar");
        as->Comment("y reg is ZERO from ldy #0 in GetTileNum");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");

    as->Label("vbmDTXO_NextChar");
        as->Asm("clc");
        as->Asm("inc " + as->m_internalZP[0] + "  ; low byte");
        as->Asm("bne vbmDTXO_NTM_NoOverflow");
        as->Asm("inc " + as->m_internalZP[0] + "+1  ; high byte");
    as->Label("vbmDTXO_NTM_NoOverflow");
        as->Comment("next x pos on screen");
        as->Asm("inc vbmX");
        as->Asm("lda vbmNumColumns   ; 0-19 columns, 20 means exceeded right of screen");
        as->Asm("cmp vbmX  ; has x pos exceeded?");
        as->Asm("beq vbmDTXO_NextLine  ;");
        as->Asm("jmp vbmDTXO_Xloop  ; no, draw next char");

        as->Label("vbmDTXO_NextLine");
        as->Comment("yes, set x back to 0 and inc vbmY by line height (pixels)");
        as->Asm("lda #0");
        as->Asm("sta vbmX");

        as->Asm("lda vbmY");
        as->Asm("clc");
        as->Asm("adc vbmI");
        as->Asm("sta vbmY");

        as->Asm("jmp vbmDTXO_Xloop");

}
void Methods6502Vic20::vbmDrawTextO(Assembler* as)
{
    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmDrawTextO","InitVbmDrawTextO");

    if (as->m_internalZP.count()==0)
        return;
    if (as->m_internalZP.count() < 3) {
        ErrorHandler::e.Error("This TRSE command needs at least 3 temporary ZP pointers but has less. Check the TRSE settings for temporary pointers.", m_node->m_op.m_lineNumber);
    }

    // address 1 - text
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr1 = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr1 = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr1 = var->getValue(as);

    // address 2 - chars (font)
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[1]);
    if (var==nullptr && !m_node->m_params[1]->isPureNumeric()) {
        ErrorHandler::e.Error("Second parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr2= "";
    if (m_node->m_params[1]->isPureNumeric())
        addr2 = m_node->m_params[1]->HexValue();
    if (var!=nullptr)
        addr2 = var->getValue(as);

    as->Comment("Draw 8x8 text to the bitmap with OR operation");

    as->Comment("Text to use:");
    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr1 );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda " + addr1 +"+1" );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    } else {
        as->Asm("lda #<" + addr1 );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda #>" + addr1 );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    }
    as->Comment("Font characters to use:");
    if (m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr2 );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda " + addr2 +"+1" );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    } else {
        as->Asm("lda #<" + addr2 );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda #>" + addr2 );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    }

    //  X
    if (m_node->m_params[2]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[2]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("x is complex");
        LoadVar(as, 2);
    }
    as->Asm("sta vbmX ; x position");

    //  Y
    if (m_node->m_params[3]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[3]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("y is complex");
        LoadVar(as, 3);
    }
    as->Asm("sta vbmY ; y position in pixels");

    // Line height
    if (m_node->m_params[4]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[4]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("Line height is complex");
        LoadVar(as, 4);
    }
    as->Asm("sta vbmI ; line height in pixels");

    as->Asm("jsr vbmDrawTextO");

}

void Methods6502Vic20::initVbmDrawTextE(Assembler* as)
{
    if (m_node->m_isInitialized["vbmDrawTextE"])
        return;

    m_node->m_isInitialized["vbmDrawTextE"] = true;

    as->Comment("Draw text characters to the bitmap using a zero terminated CSTRING with EOR operation");
    as->Comment("CSTRING    = " + as->m_internalZP[0]);
    as->Comment("Font chars = " + as->m_internalZP[1]);
    as->Comment("Temp addr  = " + as->m_internalZP[2] + " - used to calculate char address");
    as->Label("vbmDrawTextE");

    as->Label("vbmDTXE_Xloop");

        as->Comment("calculate next screen memory position");
        as->Asm("ldx vbmX");
        as->Asm("lda vbmScrL,x   ; Address of table lo");
        as->Asm("ldy vbmScrH,x   ; Address of table hi");
        as->Asm("clc");
        as->Asm("adc vbmY		; Add Y offset");
        as->Asm("bcc vbmDTXE_NSP_NoOverflow");
        as->Asm("iny");

    as->Label("vbmDTXE_NSP_NoOverflow");
        as->Asm("sta screenmemory");
        as->Asm("sty screenmemory+1");

    as->Label("vbmDTXE_GetCharNum");
        as->Comment("convert text number (0-255) * 8 = memory offset");
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[0] +"),y		; get char from current position in CSTRING");
        as->Asm("bne vbmDTXE_NotEnd");
        as->Asm("rts ; if =0, we are end of the cstring");

    as->Label("vbmDTXE_NotEnd");

        as->Asm("sta " + as->m_internalZP[2]);
        as->Asm("sty " + as->m_internalZP[2] + "+1");

        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x2");
        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x4");
        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x8");

        as->Asm("lda " + as->m_internalZP[2] );
        as->Asm("clc");
        as->Asm("adc "+ as->m_internalZP[1] + "  ; add tile low address");
        as->Asm("sta " + as->m_internalZP[2] );
        as->Asm("lda " + as->m_internalZP[2] + "+1");
        as->Asm("adc " + as->m_internalZP[1] + "+1 ; add tile high address");
        as->Asm("sta " + as->m_internalZP[2] + "+1" );

    as->Label("vbmDTXE_DrawChar");
        as->Comment("y reg is ZERO from ldy #0 in GetTileNum");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");

    as->Label("vbmDTXE_NextChar");
        as->Asm("clc");
        as->Asm("inc " + as->m_internalZP[0] + "  ; low byte");
        as->Asm("bne vbmDTXE_NTM_NoOverflow");
        as->Asm("inc " + as->m_internalZP[0] + "+1  ; high byte");
    as->Label("vbmDTXE_NTM_NoOverflow");
        as->Comment("next x pos on screen");
        as->Asm("inc vbmX");
        as->Asm("lda vbmNumColumns   ; 0-19 columns, 20 means exceeded right of screen");
        as->Asm("cmp vbmX  ; has x pos exceeded?");
        as->Asm("beq vbmDTXE_NextLine");
        as->Asm("jmp vbmDTXE_Xloop  ; no, draw next char");

        as->Label("vbmDTXE_NextLine");
        as->Comment("yes, set x back to 0 and inc vbmY by line height (pixels)");
        as->Asm("lda #0");
        as->Asm("sta vbmX");

        as->Asm("lda vbmY");
        as->Asm("clc");
        as->Asm("adc vbmI");
        as->Asm("sta vbmY");

        as->Asm("jmp vbmDTXE_Xloop");

}
void Methods6502Vic20::vbmDrawTextE(Assembler* as)
{
    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmDrawTextE","InitVbmDrawTextE");

    if (as->m_internalZP.count()==0)
        return;
    if (as->m_internalZP.count() < 3) {
        ErrorHandler::e.Error("This TRSE command needs at least 3 temporary ZP pointers but has less. Check the TRSE settings for temporary pointers.", m_node->m_op.m_lineNumber);
    }

    // address 1 - text
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr1 = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr1 = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr1 = var->getValue(as);

    // address 2 - chars (font)
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[1]);
    if (var==nullptr && !m_node->m_params[1]->isPureNumeric()) {
        ErrorHandler::e.Error("Second parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr2= "";
    if (m_node->m_params[1]->isPureNumeric())
        addr2 = m_node->m_params[1]->HexValue();
    if (var!=nullptr)
        addr2 = var->getValue(as);

    as->Comment("Draw 8x8 text to the bitmap with EOR operation");

    as->Comment("Text to use:");
    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr1 );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda " + addr1 +"+1" );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    } else {
        as->Asm("lda #<" + addr1 );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda #>" + addr1 );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    }
    as->Comment("Font characters to use:");
    if (m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr2 );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda " + addr2 +"+1" );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    } else {
        as->Asm("lda #<" + addr2 );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda #>" + addr2 );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    }

    //  X
    if (m_node->m_params[2]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[2]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("x is complex");
        LoadVar(as, 2);
    }
    as->Asm("sta vbmX ; x position");

    //  Y
    if (m_node->m_params[3]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[3]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("y is complex");
        LoadVar(as, 3);
    }
    as->Asm("sta vbmY ; y position in pixels");

    // Line height
    if (m_node->m_params[4]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[4]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("Line height is complex");
        LoadVar(as, 4);
    }
    as->Asm("sta vbmI ; line height in pixels");

    as->Asm("jsr vbmDrawTextE");

}

void Methods6502Vic20::initVbmClearText(Assembler* as)
{
    if (m_node->m_isInitialized["vbmClearText"])
        return;

    m_node->m_isInitialized["vbmClearText"] = true;

    as->Comment("Clear text characters to the bitmap using a zero terminated CSTRING");
    as->Comment("CSTRING    = " + as->m_internalZP[0]);
    as->Comment("Font chars = " + as->m_internalZP[1]);
    as->Comment("Temp addr  = " + as->m_internalZP[2] + " - used to calculate char address");
    as->Label("vbmClearText");

    as->Label("vbmCTX_Xloop");

        as->Comment("calculate next screen memory position");
        as->Asm("ldx vbmX");
        as->Asm("lda vbmScrL,x   ; Address of table lo");
        as->Asm("ldy vbmScrH,x   ; Address of table hi");
        as->Asm("clc");
        as->Asm("adc vbmY		; Add Y offset");
        as->Asm("bcc vbmCTX_NSP_NoOverflow");
        as->Asm("iny");

    as->Label("vbmCTX_NSP_NoOverflow");
        as->Asm("sta screenmemory");
        as->Asm("sty screenmemory+1");

    as->Label("vbmCTX_GetCharNum");
        as->Comment("convert text number (0-255) * 8 = memory offset");
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[0] +"),y		; get char from current position in CSTRING");
        as->Asm("bne vbmCTX_NotEnd");
        as->Asm("rts ; if =0, we are end of the cstring");

    as->Label("vbmCTX_NotEnd");

        as->Asm("sta " + as->m_internalZP[2]);
        as->Asm("sty " + as->m_internalZP[2] + "+1");

        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x2");
        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x4");
        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x8");

        as->Asm("lda " + as->m_internalZP[2] );
        as->Asm("clc");
        as->Asm("adc "+ as->m_internalZP[1] + "  ; add tile low address");
        as->Asm("sta " + as->m_internalZP[2] );
        as->Asm("lda " + as->m_internalZP[2] + "+1");
        as->Asm("adc " + as->m_internalZP[1] + "+1 ; add tile high address");
        as->Asm("sta " + as->m_internalZP[2] + "+1" );

    as->Label("vbmCTX_DrawChar");
        as->Comment("y reg is ZERO from ldy #0 in GetTileNum");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");

    as->Label("vbmCTX_NextChar");
        as->Asm("clc");
        as->Asm("inc " + as->m_internalZP[0] + "  ; low byte");
        as->Asm("bne vbmCTX_NTM_NoOverflow");
        as->Asm("inc " + as->m_internalZP[0] + "+1  ; high byte");
    as->Label("vbmCTX_NTM_NoOverflow");
        as->Comment("next x pos on screen");
        as->Asm("inc vbmX");
        as->Asm("lda vbmNumColumns   ; 0-19 columns, 20 means exceeded right of screen");
        as->Asm("cmp vbmX  ; has x pos exceeded?");
        as->Asm("beq vbmCTX_NextLine");
        as->Asm("jmp vbmCTX_Xloop  ; no, draw next char");

        as->Label("vbmCTX_NextLine");
        as->Comment("yes, set x back to 0 and inc vbmY by line height (pixels)");
        as->Asm("lda #0");
        as->Asm("sta vbmX");

        as->Asm("lda vbmY");
        as->Asm("clc");
        as->Asm("adc vbmI");
        as->Asm("sta vbmY");

        as->Asm("jmp vbmCTX_Xloop");

}
void Methods6502Vic20::vbmClearText(Assembler* as)
{
    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmClearText","InitVbmClearText");

    if (as->m_internalZP.count()==0)
        return;
    if (as->m_internalZP.count() < 3) {
        ErrorHandler::e.Error("This TRSE command needs at least 3 temporary ZP pointers but has less. Check the TRSE settings for temporary pointers.", m_node->m_op.m_lineNumber);
    }

    // address 1 - text
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr1 = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr1 = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr1 = var->getValue(as);

    // address 2 - chars (font)
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[1]);
    if (var==nullptr && !m_node->m_params[1]->isPureNumeric()) {
        ErrorHandler::e.Error("Second parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr2= "";
    if (m_node->m_params[1]->isPureNumeric())
        addr2 = m_node->m_params[1]->HexValue();
    if (var!=nullptr)
        addr2 = var->getValue(as);

    as->Comment("Clear 8x8 text to the bitmap");

    as->Comment("Text to use:");
    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr1 );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda " + addr1 +"+1" );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    } else {
        as->Asm("lda #<" + addr1 );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda #>" + addr1 );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    }
    as->Comment("Font characters to use:");
    if (m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr2 );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda " + addr2 +"+1" );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    } else {
        as->Asm("lda #<" + addr2 );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda #>" + addr2 );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    }

    //  X
    if (m_node->m_params[2]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[2]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("x is complex");
        LoadVar(as, 2);
    }
    as->Asm("sta vbmX ; x position");

    //  Y
    if (m_node->m_params[3]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[3]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("y is complex");
        LoadVar(as, 3);
    }
    as->Asm("sta vbmY ; y position in pixels");

    // Line height
    if (m_node->m_params[4]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[4]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("Line height is complex");
        LoadVar(as, 4);
    }
    as->Asm("sta vbmI ; line height in pixels");

    as->Asm("jsr vbmClearText");

}

void Methods6502Vic20::initVbmDrawSmallTextO(Assembler* as)
{
    if (m_node->m_isInitialized["vbmDrawSmallTextO"])
        return;

    m_node->m_isInitialized["vbmDrawSmallTextO"] = true;

    as->Comment("Draw text characters to the bitmap using a zero terminated CSTRING with OR operation");
    as->Comment("CSTRING    = " + as->m_internalZP[0]);
    as->Comment("Font chars = " + as->m_internalZP[1]);
    as->Comment("Temp addr  = " + as->m_internalZP[2] + " - used to calculate char address");
    as->Label("vbmDrawSmallTextO");

    as->Label("vbmDSTXO_Xloop");

        as->Comment("calculate next screen memory position");
        as->Asm("lda vbmX");
        as->Asm("lsr ; divde x by 2 (2 chars per character cell)");
        as->Asm("tax");
        as->Comment("Work out from LSR if odd or even pattern");
        as->Asm("bcs vbmDSTXO_Odd");
        as->Asm("lda #$f0 ; even, use left side of font");
        as->Asm("bcc vbmDSTXO_Even ; we know carry will be clear");
    as->Label("vbmDSTXO_Odd");
        as->Asm("lda #$0f ; odd, use right side of font");
    as->Label("vbmDSTXO_Even");
        as->Asm("sta vbmT ; store mask to use for later");
        as->Asm("lda vbmScrL,x   ; Address of table lo");
        as->Asm("ldy vbmScrH,x   ; Address of table hi");
        as->Asm("clc");
        as->Asm("adc vbmY		; Add Y offset");
        as->Asm("bcc vbmDSTXO_NSP_NoOverflow");
        as->Asm("iny");

    as->Label("vbmDSTXO_NSP_NoOverflow");
        as->Asm("sta screenmemory");
        as->Asm("sty screenmemory+1");

    as->Label("vbmDSTXO_GetCharNum");
        as->Comment("convert text number (0-255) * 8 = memory offset");
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[0] +"),y		; get char from current position in CSTRING");
        as->Asm("bne vbmDSTXO_NotEnd");
        as->Asm("rts ; if =0, we are end of the cstring");

    as->Label("vbmDSTXO_NotEnd");

        as->Asm("sta " + as->m_internalZP[2]);
        as->Asm("sty " + as->m_internalZP[2] + "+1");

        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x2");
        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x4");
        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x8");

        as->Asm("lda " + as->m_internalZP[2] );
        as->Asm("clc");
        as->Asm("adc "+ as->m_internalZP[1] + "  ; add tile low address");
        as->Asm("sta " + as->m_internalZP[2] );
        as->Asm("lda " + as->m_internalZP[2] + "+1");
        as->Asm("adc " + as->m_internalZP[1] + "+1 ; add tile high address");
        as->Asm("sta " + as->m_internalZP[2] + "+1" );

    as->Label("vbmDSTXO_DrawChar");
        as->Comment("y reg is ZERO from ldy #0 in GetTileNum");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("and vbmT");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("and vbmT");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("and vbmT");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("and vbmT");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("and vbmT");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("and vbmT");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("and vbmT");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("and vbmT");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");

    as->Label("vbmDSTXO_NextChar");
        as->Asm("clc");
        as->Asm("inc " + as->m_internalZP[0] + "  ; low byte");
        as->Asm("bne vbmDSTXO_NTM_NoOverflow");
        as->Asm("inc " + as->m_internalZP[0] + "+1  ; high byte");
    as->Label("vbmDSTXO_NTM_NoOverflow");
        as->Comment("next x pos on screen");
        as->Asm("inc vbmX");
        as->Asm("lda vbmNumColumns   ; 0-39 columns, 40 means exceeded right of screen");
        as->Asm("asl ; x2");
        as->Asm("cmp vbmX  ; has x pos exceeded?");
        as->Asm("beq vbmDSTXO_NextLine  ;");
        as->Asm("jmp vbmDSTXO_Xloop  ; no, draw next char");

    as->Label("vbmDSTXO_NextLine");
        as->Comment("yes, set x back to 0 and inc vbmY by line height (pixels)");
        as->Asm("lda #0");
        as->Asm("sta vbmX");

        as->Asm("lda vbmY");
        as->Asm("clc");
        as->Asm("adc vbmI");
        as->Asm("sta vbmY");

        as->Asm("jmp vbmDSTXO_Xloop");

}
void Methods6502Vic20::vbmDrawSmallTextO(Assembler* as)
{
    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmDrawSmallTextO","InitVbmDrawSmallTextO");

    if (as->m_internalZP.count()==0)
        return;
    if (as->m_internalZP.count() < 3) {
        ErrorHandler::e.Error("This TRSE command needs at least 3 temporary ZP pointers but has less. Check the TRSE settings for temporary pointers.", m_node->m_op.m_lineNumber);
    }

    // address 1 - text
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr1 = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr1 = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr1 = var->getValue(as);

    // address 2 - chars (font)
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[1]);
    if (var==nullptr && !m_node->m_params[1]->isPureNumeric()) {
        ErrorHandler::e.Error("Second parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr2= "";
    if (m_node->m_params[1]->isPureNumeric())
        addr2 = m_node->m_params[1]->HexValue();
    if (var!=nullptr)
        addr2 = var->getValue(as);

    as->Comment("Draw 4x8 text to the bitmap with OR operation");

    as->Comment("Text to use:");
    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr1 );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda " + addr1 +"+1" );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    } else {
        as->Asm("lda #<" + addr1 );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda #>" + addr1 );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    }
    as->Comment("Font characters to use:");
    if (m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr2 );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda " + addr2 +"+1" );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    } else {
        as->Asm("lda #<" + addr2 );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda #>" + addr2 );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    }

    //  X
    if (m_node->m_params[2]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[2]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("x is complex");
        LoadVar(as, 2);
    }
    as->Asm("sta vbmX ; x position");

    //  Y
    if (m_node->m_params[3]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[3]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("y is complex");
        LoadVar(as, 3);
    }
    as->Asm("sta vbmY ; y position in pixels");

    // Line height
    if (m_node->m_params[4]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[4]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("Line height is complex");
        LoadVar(as, 4);
    }
    as->Asm("sta vbmI ; line height in pixels");

    as->Asm("jsr vbmDrawSmallTextO");

}
void Methods6502Vic20::initVbmDrawSmallTextE(Assembler* as)
{
    if (m_node->m_isInitialized["vbmDrawSmallTextE"])
        return;

    m_node->m_isInitialized["vbmDrawSmallTextE"] = true;

    as->Comment("Draw text characters to the bitmap using a zero terminated CSTRING with EOR operation");
    as->Comment("CSTRING    = " + as->m_internalZP[0]);
    as->Comment("Font chars = " + as->m_internalZP[1]);
    as->Comment("Temp addr  = " + as->m_internalZP[2] + " - used to calculate char address");

    as->Label("vbmDrawSmallTextE");

    as->Label("vbmDSTXE_Xloop");

        as->Comment("calculate next screen memory position");
        as->Asm("lda vbmX");
        as->Asm("lsr ; divde x by 2 (2 chars per character cell)");
        as->Asm("tax");
        as->Comment("Work out from LSR if odd or even pattern");
        as->Asm("bcs vbmDSTXE_Odd");
        as->Asm("lda #$f0 ; even, use left side of font");
        as->Asm("bcc vbmDSTXE_Even ; we know carry will be clear");
    as->Label("vbmDSTXE_Odd");
        as->Asm("lda #$0f ; odd, use right side of font");
    as->Label("vbmDSTXE_Even");
        as->Asm("sta vbmT ; store mask to use for later");
        as->Asm("lda vbmScrL,x   ; Address of table lo");
        as->Asm("ldy vbmScrH,x   ; Address of table hi");
        as->Asm("clc");
        as->Asm("adc vbmY		; Add Y offset");
        as->Asm("bcc vbmDSTXE_NSP_NoOverflow");
        as->Asm("iny");

    as->Label("vbmDSTXE_NSP_NoOverflow");
        as->Asm("sta screenmemory");
        as->Asm("sty screenmemory+1");

    as->Label("vbmDSTXE_GetCharNum");
        as->Comment("convert text number (0-255) * 8 = memory offset");
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[0] +"),y		; get char from current position in CSTRING");
        as->Asm("bne vbmDSTXE_NotEnd");
        as->Asm("rts ; if =0, we are end of the cstring");

    as->Label("vbmDSTXE_NotEnd");

        as->Asm("sta " + as->m_internalZP[2]);
        as->Asm("sty " + as->m_internalZP[2] + "+1");

        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x2");
        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x4");
        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x8");

        as->Asm("lda " + as->m_internalZP[2] );
        as->Asm("clc");
        as->Asm("adc "+ as->m_internalZP[1] + "  ; add tile low address");
        as->Asm("sta " + as->m_internalZP[2] );
        as->Asm("lda " + as->m_internalZP[2] + "+1");
        as->Asm("adc " + as->m_internalZP[1] + "+1 ; add tile high address");
        as->Asm("sta " + as->m_internalZP[2] + "+1" );

    as->Label("vbmDSTXE_DrawChar");
        as->Comment("y reg is ZERO from ldy #0 in GetTileNum");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("and vbmT");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("and vbmT");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("and vbmT");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("and vbmT");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("and vbmT");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("and vbmT");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("and vbmT");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("and vbmT");
        as->Asm("eor (screenmemory),y");
        as->Asm("sta (screenmemory),y");

    as->Label("vbmDSTXE_NextChar");
        as->Asm("clc");
        as->Asm("inc " + as->m_internalZP[0] + "  ; low byte");
        as->Asm("bne vbmDSTXE_NTM_NoOverflow");
        as->Asm("inc " + as->m_internalZP[0] + "+1  ; high byte");
    as->Label("vbmDSTXE_NTM_NoOverflow");
        as->Comment("next x pos on screen");
        as->Asm("inc vbmX");
        as->Asm("lda vbmNumColumns   ; 0-39 columns, 40 means exceeded right of screen");
        as->Asm("asl ; x2");
        as->Asm("cmp vbmX  ; has x pos exceeded?");
        as->Asm("beq vbmDSTXE_NextLine  ;");
        as->Asm("jmp vbmDSTXE_Xloop  ; no, draw next char");

    as->Label("vbmDSTXE_NextLine");
        as->Comment("yes, set x back to 0 and inc vbmY by line height (pixels)");
        as->Asm("lda #0");
        as->Asm("sta vbmX");

        as->Asm("lda vbmY");
        as->Asm("clc");
        as->Asm("adc vbmI");
        as->Asm("sta vbmY");

        as->Asm("jmp vbmDSTXE_Xloop");

}
void Methods6502Vic20::vbmDrawSmallTextE(Assembler* as)
{
    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmDrawSmallTextE","InitVbmDrawSmallTextE");

    if (as->m_internalZP.count()==0)
        return;
    if (as->m_internalZP.count() < 3) {
        ErrorHandler::e.Error("This TRSE command needs at least 3 temporary ZP pointers but has less. Check the TRSE settings for temporary pointers.", m_node->m_op.m_lineNumber);
    }

    // address 1 - text
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr1 = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr1 = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr1 = var->getValue(as);

    // address 2 - chars (font)
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[1]);
    if (var==nullptr && !m_node->m_params[1]->isPureNumeric()) {
        ErrorHandler::e.Error("Second parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr2= "";
    if (m_node->m_params[1]->isPureNumeric())
        addr2 = m_node->m_params[1]->HexValue();
    if (var!=nullptr)
        addr2 = var->getValue(as);

    as->Comment("Draw 4x8 text to the bitmap with EOR operation");

    as->Comment("Text to use:");
    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr1 );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda " + addr1 +"+1" );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    } else {
        as->Asm("lda #<" + addr1 );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda #>" + addr1 );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    }
    as->Comment("Font characters to use:");
    if (m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr2 );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda " + addr2 +"+1" );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    } else {
        as->Asm("lda #<" + addr2 );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda #>" + addr2 );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    }

    //  X
    if (m_node->m_params[2]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[2]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("x is complex");
        LoadVar(as, 2);
    }
    as->Asm("sta vbmX ; x position");

    //  Y
    if (m_node->m_params[3]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[3]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("y is complex");
        LoadVar(as, 3);
    }
    as->Asm("sta vbmY ; y position in pixels");

    // Line height
    if (m_node->m_params[4]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[4]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("Line height is complex");
        LoadVar(as, 4);
    }
    as->Asm("sta vbmI ; line height in pixels");

    as->Asm("jsr vbmDrawSmallTextE");

}
void Methods6502Vic20::initVbmClearSmallText(Assembler* as)
{
    if (m_node->m_isInitialized["vbmClearSmallText"])
        return;

    m_node->m_isInitialized["vbmClearSmallText"] = true;

    as->Comment("Draw text characters to the bitmap using a zero terminated CSTRING with AND operation");
    as->Comment("CSTRING    = " + as->m_internalZP[0]);
    as->Comment("Font chars = " + as->m_internalZP[1]);
    as->Comment("Temp addr  = " + as->m_internalZP[2] + " - used to calculate char address");

    as->Label("vbmClearSmallText");

    as->Label("vbmCSTX_Xloop");

        as->Comment("calculate next screen memory position");
        as->Asm("lda vbmX");
        as->Asm("lsr ; divde x by 2 (2 chars per character cell)");
        as->Asm("tax");
        as->Comment("Work out from LSR if odd or even pattern");
        as->Asm("bcs vbmCSTX_Odd");
        as->Asm("lda #$f0 ; even, use left side of font");
        as->Asm("bcc vbmCSTX_Even ; we know carry will be clear");
    as->Label("vbmCSTX_Odd");
        as->Asm("lda #$0f ; odd, use right side of font");
    as->Label("vbmCSTX_Even");
        as->Asm("sta vbmT ; store mask to use for later");
        as->Asm("lda vbmScrL,x   ; Address of table lo");
        as->Asm("ldy vbmScrH,x   ; Address of table hi");
        as->Asm("clc");
        as->Asm("adc vbmY		; Add Y offset");
        as->Asm("bcc vbmCSTX_NSP_NoOverflow");
        as->Asm("iny");

    as->Label("vbmCSTX_NSP_NoOverflow");
        as->Asm("sta screenmemory");
        as->Asm("sty screenmemory+1");

    as->Label("vbmCSTX_GetCharNum");
        as->Comment("convert text number (0-255) * 8 = memory offset");
        as->Asm("ldy #0");
        as->Asm("lda (" + as->m_internalZP[0] +"),y		; get char from current position in CSTRING");
        as->Asm("bne vbmCSTX_NotEnd");
        as->Asm("rts ; if =0, we are end of the cstring");

    as->Label("vbmCSTX_NotEnd");

        as->Asm("sta " + as->m_internalZP[2]);
        as->Asm("sty " + as->m_internalZP[2] + "+1");

        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x2");
        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x4");
        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x8");

        as->Asm("lda " + as->m_internalZP[2] );
        as->Asm("clc");
        as->Asm("adc "+ as->m_internalZP[1] + "  ; add tile low address");
        as->Asm("sta " + as->m_internalZP[2] );
        as->Asm("lda " + as->m_internalZP[2] + "+1");
        as->Asm("adc " + as->m_internalZP[1] + "+1 ; add tile high address");
        as->Asm("sta " + as->m_internalZP[2] + "+1" );

    as->Label("vbmCSTX_DrawChar");
        as->Comment("y reg is ZERO from ldy #0 in GetTileNum");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("and vbmT ; mask out one side");
        as->Asm("eor #$ff ; invert");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("and vbmT");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("and vbmT");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("and vbmT");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("and vbmT");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("and vbmT");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("and vbmT");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("and vbmT");
        as->Asm("eor #$ff");
        as->Asm("and (screenmemory),y");
        as->Asm("sta (screenmemory),y");

    as->Label("vbmCSTX_NextChar");
        as->Asm("clc");
        as->Asm("inc " + as->m_internalZP[0] + "  ; low byte");
        as->Asm("bne vbmCSTX_NTM_NoOverflow");
        as->Asm("inc " + as->m_internalZP[0] + "+1  ; high byte");
    as->Label("vbmCSTX_NTM_NoOverflow");
        as->Comment("next x pos on screen");
        as->Asm("inc vbmX");
        as->Asm("lda vbmNumColumns   ; 0-39 columns, 40 means exceeded right of screen");
        as->Asm("asl ; x2");
        as->Asm("cmp vbmX  ; has x pos exceeded?");
        as->Asm("beq vbmCSTX_NextLine  ;");
        as->Asm("jmp vbmCSTX_Xloop  ; no, draw next char");

    as->Label("vbmCSTX_NextLine");
        as->Comment("yes, set x back to 0 and inc vbmY by line height (pixels)");
        as->Asm("lda #0");
        as->Asm("sta vbmX");

        as->Asm("lda vbmY");
        as->Asm("clc");
        as->Asm("adc vbmI");
        as->Asm("sta vbmY");

        as->Asm("jmp vbmCSTX_Xloop");

}
void Methods6502Vic20::vbmClearSmallText(Assembler* as)
{
    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmClearSmallText","InitVbmClearSmallText");

    if (as->m_internalZP.count()==0)
        return;
    if (as->m_internalZP.count() < 3) {
        ErrorHandler::e.Error("This TRSE command needs at least 3 temporary ZP pointers but has less. Check the TRSE settings for temporary pointers.", m_node->m_op.m_lineNumber);
    }

    // address 1 - text
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr1 = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr1 = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr1 = var->getValue(as);

    // address 2 - chars (font)
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[1]);
    if (var==nullptr && !m_node->m_params[1]->isPureNumeric()) {
        ErrorHandler::e.Error("Second parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr2= "";
    if (m_node->m_params[1]->isPureNumeric())
        addr2 = m_node->m_params[1]->HexValue();
    if (var!=nullptr)
        addr2 = var->getValue(as);

    as->Comment("Clear 4x8 text to the bitmap with AND operation");

    as->Comment("Text to use:");
    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr1 );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda " + addr1 +"+1" );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    } else {
        as->Asm("lda #<" + addr1 );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda #>" + addr1 );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    }
    as->Comment("Font characters to use:");
    if (m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr2 );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda " + addr2 +"+1" );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    } else {
        as->Asm("lda #<" + addr2 );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda #>" + addr2 );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    }

    //  X
    if (m_node->m_params[2]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[2]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("x is complex");
        LoadVar(as, 2);
    }
    as->Asm("sta vbmX ; x position");

    //  Y
    if (m_node->m_params[3]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[3]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("y is complex");
        LoadVar(as, 3);
    }
    as->Asm("sta vbmY ; y position in pixels");

    // Line height
    if (m_node->m_params[4]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[4]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("Line height is complex");
        LoadVar(as, 4);
    }
    as->Asm("sta vbmI ; line height in pixels");

    as->Asm("jsr vbmClearSmallText");

}

void Methods6502Vic20::initVbmDrawBCD(Assembler* as)
{
    if (m_node->m_isInitialized["vbmDrawBCD"])
        return;

    m_node->m_isInitialized["vbmDrawBCD"] = true;

    as->Comment("Draw text characters to the bitmap using a zero terminated CSTRING");
    as->Comment("Font chars = " + as->m_internalZP[1]);
    as->Comment("Temp addr  = " + as->m_internalZP[2] + " - used to calculate char address");
    as->Label("vbmDrawBCDDigit");

        as->Comment("calculate next screen memory position");
        as->Asm("ldx vbmX");
        as->Asm("lda vbmScrL,x   ; Address of table lo");
        as->Asm("ldy vbmScrH,x   ; Address of table hi");
        as->Asm("clc");
        as->Asm("adc vbmY		; Add Y offset");
        as->Asm("bcc vbmDBCD_NSP_NoOverflow");
        as->Asm("iny");

    as->Label("vbmDBCD_NSP_NoOverflow");
        as->Asm("sta screenmemory");
        as->Asm("sty screenmemory+1");

    as->Label("vbmDBCD_GetCharNum");
        as->Comment("convert text number (0-255) * 8 = memory offset");
        as->Asm("ldy #0");
        as->Asm("lda vbmI ; get digit to display");

        as->Asm("sta " + as->m_internalZP[2]);
        as->Asm("sty " + as->m_internalZP[2] + "+1");

        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x2");
        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x4");
        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x8");

        as->Asm("lda " + as->m_internalZP[2] );
        as->Asm("clc");
        as->Asm("adc "+ as->m_internalZP[1] + "  ; add char low address");
        as->Asm("sta " + as->m_internalZP[2] );
        as->Asm("lda " + as->m_internalZP[2] + "+1");
        as->Asm("adc " + as->m_internalZP[1] + "+1 ; add char high address");
        as->Asm("sta " + as->m_internalZP[2] + "+1" );

    as->Label("vbmDBCD_DrawDigit");
        as->Comment("y reg is ZERO from ldy #0 in GetTileNum");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("sta (screenmemory),y");

    as->Label("vbmDBCD_Done");
        as->Asm("inc vbmX");

}
void Methods6502Vic20::vbmDrawBCD(Assembler *as)
{

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmDrawBCD","InitVbmDrawBCD");

    if (as->m_internalZP.count()==0)
        return;
    if (as->m_internalZP.count() < 3) {
        ErrorHandler::e.Error("This TRSE command needs at least 3 temporary ZP pointers but has less. Check the TRSE settings for temporary pointers.", m_node->m_op.m_lineNumber);
    }

    as->Comment("----------");
    as->Comment("VBM DrawBCD BCD array, Font, X, Y, number of BCD bytes");

    // address 1 - BCD array
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr1 = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr1 = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr1 = var->getValue(as);

    // address 2 - chars (font)
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[1]);
    if (var==nullptr && !m_node->m_params[1]->isPureNumeric()) {
        ErrorHandler::e.Error("Second parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr2= "";
    if (m_node->m_params[1]->isPureNumeric())
        addr2 = m_node->m_params[1]->HexValue();
    if (var!=nullptr)
        addr2 = var->getValue(as);

    as->Comment("Font characters to use:");
    if (m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr2 );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda " + addr2 +"+1" );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    } else {
        as->Asm("lda #<" + addr2 );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda #>" + addr2 );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    }

    //  X
    if (m_node->m_params[2]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[2]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("x is complex");
        LoadVar(as, 2);
    }
    as->Asm("sta vbmX ; x position");

    //  Y
    if (m_node->m_params[3]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[3]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("y is complex");
        LoadVar(as, 3);
    }
    as->Asm("sta vbmY ; y position in pixels");

    // Number of digits in BCD number
    QSharedPointer<NodeNumber> numBytes = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[4]);
    if (numBytes==nullptr)
        ErrorHandler::e.Error("BCD: last parameter, number of digits, required to be pure constant number");

    int numDigits = numBytes->m_val;
    if (numDigits < 1 || numDigits > 254)
        ErrorHandler::e.Error("BCD: last parameter, number of BCD bytes, must be greater than 0 but less than 255");
    numDigits--;

    QString lblLoop = as->NewLabel("vbmDrawBCDloop");

    as->Asm("lda #" + Util::numToHex(numDigits) + " ; BCD array - highest byte (in reverse order)");
    as->Asm("sta vbmT");
    as->Label(lblLoop);
    as->Asm("ldx vbmT");
    as->Asm("lda " + addr1+",x");
    as->Asm("pha");
    as->Asm("lsr ; get high nibble"); // get high nibble
    as->Asm("lsr");
    as->Asm("lsr");
    as->Asm("lsr");
    as->Asm("sta vbmI ; digit to display");
    as->Asm("jsr vbmDrawBCDDigit");
    as->Asm("pla");
    as->Asm("and #$0f ; get low nibble"); // get low nibble
    as->Asm("sta vbmI ; digit to display");
    as->Asm("jsr vbmDrawBCDDigit");
    as->Asm("dec vbmT");
    as->Asm("bpl " + lblLoop +" ; loop until all bytes displayed");

    as->PopLabel("vbmDrawBCDloop");
}

void Methods6502Vic20::initVbmDrawSmallBCD(Assembler* as)
{
    if (m_node->m_isInitialized["vbmDrawSmallBCD"])
        return;

    m_node->m_isInitialized["vbmDrawSmallBCD"] = true;

    as->Comment("Draw small text characters to the bitmap using a zero terminated CSTRING");
    as->Comment("Font chars = " + as->m_internalZP[1]);
    as->Comment("Temp addr  = " + as->m_internalZP[2] + " - used to calculate char address");
    as->Label("vbmDrawSmallBCDDigit");

        as->Comment("calculate next screen memory position");
        as->Asm("lda vbmX");

        as->Asm("lsr ; divde x by 2 (2 chars per character cell)");
        as->Asm("tax");

        as->Comment("Work out from LSR if odd or even pattern");
        as->Asm("bcs vbmDSBCD_Odd");
        as->Asm("lda #$f0 ; even, use left side of font");
        as->Asm("bcc vbmDSBCD_Even ; we know carry will be clear");
as->Label("vbmDSBCD_Odd");
        as->Asm("lda #$0f ; odd, use right side of font");
as->Label("vbmDSBCD_Even");
        as->Asm("sta vbmJ ; store mask to use for later");

        as->Asm("lda vbmScrL,x   ; Address of table lo");
        as->Asm("ldy vbmScrH,x   ; Address of table hi");
        as->Asm("clc");
        as->Asm("adc vbmY		; Add Y offset");
        as->Asm("bcc vbmDSBCD_NSP_NoOverflow");
        as->Asm("iny");

    as->Label("vbmDSBCD_NSP_NoOverflow");
        as->Asm("sta screenmemory");
        as->Asm("sty screenmemory+1");

    as->Label("vbmDSBCD_GetCharNum");
        as->Comment("convert text number (0-255) * 8 = memory offset");
        as->Asm("ldy #0");
        as->Asm("lda vbmI ; get digit to display");

        as->Asm("sta " + as->m_internalZP[2]);
        as->Asm("sty " + as->m_internalZP[2] + "+1");

        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x2");
        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x4");
        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x8");

        as->Asm("lda " + as->m_internalZP[2] );
        as->Asm("clc");
        as->Asm("adc "+ as->m_internalZP[1] + "  ; add char low address");
        as->Asm("sta " + as->m_internalZP[2] );
        as->Asm("lda " + as->m_internalZP[2] + "+1");
        as->Asm("adc " + as->m_internalZP[1] + "+1 ; add char high address");
        as->Asm("sta " + as->m_internalZP[2] + "+1" );

        // need to make a mask to blank out 4x8 space where I am drawing to
        as->Asm("lda vbmT");
        as->Asm("pha ; store vbmT on stack for a minute");

        as->Asm("lda vbmJ ; take mask");
        as->Asm("eor #$ff ; and invert");
        as->Asm("sta vbmT ; to use to blank out new char pos");

    as->Label("vbmDSBCD_DrawDigit");
        as->Comment("y reg is ZERO from ldy #0 in GetTileNum");
        as->Asm("lda (screenmemory),y");
        as->Asm("and vbmT");
        as->Asm("sta (screenmemory),y");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("and vbmJ");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");

        as->Asm("lda (screenmemory),y");
        as->Asm("and vbmT");
        as->Asm("sta (screenmemory),y");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("and vbmJ");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");

        as->Asm("lda (screenmemory),y");
        as->Asm("and vbmT");
        as->Asm("sta (screenmemory),y");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("and vbmJ");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");

        as->Asm("lda (screenmemory),y");
        as->Asm("and vbmT");
        as->Asm("sta (screenmemory),y");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("and vbmJ");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");

        as->Asm("lda (screenmemory),y");
        as->Asm("and vbmT");
        as->Asm("sta (screenmemory),y");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("and vbmJ");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");

        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("beq vbmDSBCD_Done // special case - on 6th line if empty skip rest");

        as->Asm("lda (screenmemory),y");
        as->Asm("and vbmT");
        as->Asm("sta (screenmemory),y");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("and vbmJ");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");

        as->Asm("lda (screenmemory),y");
        as->Asm("and vbmT");
        as->Asm("sta (screenmemory),y");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("and vbmJ");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");

        as->Asm("lda (screenmemory),y");
        as->Asm("and vbmT");
        as->Asm("sta (screenmemory),y");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("and vbmJ");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");

    as->Label("vbmDSBCD_Done");
        as->Asm("pla ; store vbmT on stack for a minute");
        as->Asm("sta vbmT");

        as->Asm("inc vbmX");

}
void Methods6502Vic20::vbmDrawSmallBCD(Assembler *as)
{

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmDrawSmallBCD","InitVbmDrawSmallBCD");

    if (as->m_internalZP.count()==0)
        return;
    if (as->m_internalZP.count() < 3) {
        ErrorHandler::e.Error("This TRSE command needs at least 3 temporary ZP pointers but has less. Check the TRSE settings for temporary pointers.", m_node->m_op.m_lineNumber);
    }

    as->Comment("----------");
    as->Comment("VBM DrawSBCD BCD array, Font, X, Y, number of BCD bytes");

    // address 1 - BCD array
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr1 = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr1 = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr1 = var->getValue(as);

    // address 2 - chars (font)
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[1]);
    if (var==nullptr && !m_node->m_params[1]->isPureNumeric()) {
        ErrorHandler::e.Error("Second parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr2= "";
    if (m_node->m_params[1]->isPureNumeric())
        addr2 = m_node->m_params[1]->HexValue();
    if (var!=nullptr)
        addr2 = var->getValue(as);

    as->Comment("Font characters to use:");
    if (m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr2 );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda " + addr2 +"+1" );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    } else {
        as->Asm("lda #<" + addr2 );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda #>" + addr2 );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    }

    //  X
    if (m_node->m_params[2]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[2]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("x is complex");
        LoadVar(as, 2);
    }
    as->Asm("sta vbmX ; x position");

    //  Y
    if (m_node->m_params[3]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[3]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("y is complex");
        LoadVar(as, 3);
    }
    as->Asm("sta vbmY ; y position in pixels");

    // Number of digits in BCD number
    QSharedPointer<NodeNumber> numBytes = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[4]);
    if (numBytes==nullptr)
        ErrorHandler::e.Error("BCD: last parameter, number of digits, required to be pure constant number");

    int numDigits = numBytes->m_val;
    if (numDigits < 1 || numDigits > 254)
        ErrorHandler::e.Error("BCD: last parameter, number of BCD bytes, must be greater than 0 but less than 255");
    numDigits--;

    QString lblLoop = as->NewLabel("vbmDrawSBCDloop");

    as->Asm("lda #" + Util::numToHex(numDigits) + " ; BCD array - highest byte (in reverse order)");
    as->Asm("sta vbmT");
    as->Label(lblLoop);
    as->Asm("ldx vbmT");
    as->Asm("lda " + addr1+",x");
    as->Asm("pha");
    as->Asm("lsr ; get high nibble"); // get high nibble
    as->Asm("lsr");
    as->Asm("lsr");
    as->Asm("lsr");
    as->Asm("sta vbmI ; digit to display");
    as->Asm("jsr vbmDrawSmallBCDDigit");
    as->Asm("pla");
    as->Asm("and #$0f ; get low nibble"); // get low nibble
    as->Asm("sta vbmI ; digit to display");
    as->Asm("jsr vbmDrawSmallBCDDigit");
    as->Asm("dec vbmT");
    as->Asm("bpl " + lblLoop +" ; loop until all bytes displayed");

    as->PopLabel("vbmDrawSBCDloop");
}
void Methods6502Vic20::initVbmDrawSmallBCDO(Assembler* as)
{
    if (m_node->m_isInitialized["vbmDrawSmallBCDO"])
        return;

    m_node->m_isInitialized["vbmDrawSmallBCDO"] = true;

    as->Comment("Draw small text characters to the bitmap using a zero terminated CSTRING with OR operation");
    as->Comment("Font chars = " + as->m_internalZP[1]);
    as->Comment("Temp addr  = " + as->m_internalZP[2] + " - used to calculate char address");
    as->Label("vbmDrawSmallBCDODigit");

        as->Comment("calculate next screen memory position");
        as->Asm("lda vbmX");

        as->Asm("lsr ; divde x by 2 (2 chars per character cell)");
        as->Asm("tax");

        as->Comment("Work out from LSR if odd or even pattern");
        as->Asm("bcs vbmDSBCDO_Odd");
        as->Asm("lda #$f0 ; even, use left side of font");
        as->Asm("bcc vbmDSBCDO_Even ; we know carry will be clear");
as->Label("vbmDSBCDO_Odd");
        as->Asm("lda #$0f ; odd, use right side of font");
as->Label("vbmDSBCDO_Even");
        as->Asm("sta vbmJ ; store mask to use for later");

        as->Asm("lda vbmScrL,x   ; Address of table lo");
        as->Asm("ldy vbmScrH,x   ; Address of table hi");
        as->Asm("clc");
        as->Asm("adc vbmY		; Add Y offset");
        as->Asm("bcc vbmDSBCDO_NSP_NoOverflow");
        as->Asm("iny");

    as->Label("vbmDSBCDO_NSP_NoOverflow");
        as->Asm("sta screenmemory");
        as->Asm("sty screenmemory+1");

    as->Label("vbmDSBCDO_GetCharNum");
        as->Comment("convert text number (0-255) * 8 = memory offset");
        as->Asm("ldy #0");
        as->Asm("lda vbmI ; get digit to display");

        as->Asm("sta " + as->m_internalZP[2]);
        as->Asm("sty " + as->m_internalZP[2] + "+1");

        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x2");
        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x4");
        as->Asm("asl " + as->m_internalZP[2]);
        as->Asm("rol " + as->m_internalZP[2] + "+1 ;x8");

        as->Asm("lda " + as->m_internalZP[2] );
        as->Asm("clc");
        as->Asm("adc "+ as->m_internalZP[1] + "  ; add char low address");
        as->Asm("sta " + as->m_internalZP[2] );
        as->Asm("lda " + as->m_internalZP[2] + "+1");
        as->Asm("adc " + as->m_internalZP[1] + "+1 ; add char high address");
        as->Asm("sta " + as->m_internalZP[2] + "+1" );

    as->Label("vbmDSBCDO_DrawDigit");
        as->Comment("y reg is ZERO from ldy #0 in GetTileNum");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("and vbmJ");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("and vbmJ");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("and vbmJ");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("and vbmJ");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("and vbmJ");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("and vbmJ");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("and vbmJ");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("iny");
        as->Asm("lda (" + as->m_internalZP[2] + "),y" );
        as->Asm("and vbmJ");
        as->Asm("ora (screenmemory),y");
        as->Asm("sta (screenmemory),y");

    as->Label("vbmDSBCDO_Done");
        as->Asm("inc vbmX");

}
void Methods6502Vic20::vbmDrawSmallBCDO(Assembler *as)
{

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmDrawSmallBCDO","InitVbmDrawSmallBCDO");

    if (as->m_internalZP.count()==0)
        return;
    if (as->m_internalZP.count() < 3) {
        ErrorHandler::e.Error("This TRSE command needs at least 3 temporary ZP pointers but has less. Check the TRSE settings for temporary pointers.", m_node->m_op.m_lineNumber);
    }

    as->Comment("----------");
    as->Comment("VBM DrawSBCDO BCD array, Font, X, Y, number of BCD bytes");

    // address 1 - BCD array
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr1 = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr1 = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr1 = var->getValue(as);

    // address 2 - chars (font)
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[1]);
    if (var==nullptr && !m_node->m_params[1]->isPureNumeric()) {
        ErrorHandler::e.Error("Second parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr2= "";
    if (m_node->m_params[1]->isPureNumeric())
        addr2 = m_node->m_params[1]->HexValue();
    if (var!=nullptr)
        addr2 = var->getValue(as);

    as->Comment("Font characters to use:");
    if (m_node->m_params[1]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr2 );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda " + addr2 +"+1" );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    } else {
        as->Asm("lda #<" + addr2 );
        as->Asm("sta " + as->m_internalZP[1] );
        as->Asm("lda #>" + addr2 );
        as->Asm("sta " + as->m_internalZP[1] + "+1" );
    }

    //  X
    if (m_node->m_params[2]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[2]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("x is complex");
        LoadVar(as, 2);
    }
    as->Asm("sta vbmX ; x position");

    //  Y
    if (m_node->m_params[3]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[3]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("y is complex");
        LoadVar(as, 3);
    }
    as->Asm("sta vbmY ; y position in pixels");

    // Number of digits in BCD number
    QSharedPointer<NodeNumber> numBytes = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[4]);
    if (numBytes==nullptr)
        ErrorHandler::e.Error("BCD: last parameter, number of digits, required to be pure constant number");

    int numDigits = numBytes->m_val;
    if (numDigits < 1 || numDigits > 254)
        ErrorHandler::e.Error("BCD: last parameter, number of BCD bytes, must be greater than 0 but less than 255");
    numDigits--;

    QString lblLoop = as->NewLabel("vbmDrawSBCDOloop");

    as->Asm("lda #" + Util::numToHex(numDigits) + " ; BCD array - highest byte (in reverse order)");
    as->Asm("sta vbmT");
    as->Label(lblLoop);
    as->Asm("ldx vbmT");
    as->Asm("lda " + addr1+",x");
    as->Asm("pha");
    as->Asm("lsr ; get high nibble"); // get high nibble
    as->Asm("lsr");
    as->Asm("lsr");
    as->Asm("lsr");
    as->Asm("sta vbmI ; digit to display");
    as->Asm("jsr vbmDrawSmallBCDODigit");
    as->Asm("pla");
    as->Asm("and #$0f ; get low nibble"); // get low nibble
    as->Asm("sta vbmI ; digit to display");
    as->Asm("jsr vbmDrawSmallBCDODigit");
    as->Asm("dec vbmT");
    as->Asm("bpl " + lblLoop +" ; loop until all bytes displayed");

    as->PopLabel("vbmDrawSBCDOloop");
}

void Methods6502Vic20::initVbmCopyToBuffer(Assembler* as)
{
    if (m_node->m_isInitialized["vbmCopyToBuffer"])
        return;

    m_node->m_isInitialized["vbmCopyToBuffer"] = true;

    as->Comment("Copy bitmap data to a buffer in memory");
    as->Comment("buffer address = " + as->m_internalZP[0]);

    as->Label("vbmCopyToBuffer");

        as->Comment("X and Y regs to be loaded with width / height");
        as->Asm("sty vbmI");
        as->Asm("dey ; need one less on Y as will go from 0 - number");
        as->Asm("sty vbmY");


    as->Label("vbmCTB_Loop");

        as->Asm("lda (screenmemory),y");
        as->Asm("sta (" + as->m_internalZP[0] + "),y");
        as->Asm("dey");

        as->Asm("bpl vbmCTB_Loop");

        as->Asm("lda screenmemory");
        as->Asm("clc");
        as->Asm("adc vbmScrHeight ; #192 ; note - not compatible with screen scroll");
        as->Asm("sta screenmemory");
        as->Asm("bcc vbmCTB_NoOverflow");
        as->Asm("inc screenmemory+1");
    as->Label("vbmCTB_NoOverflow");

        as->Asm("lda " + as->m_internalZP[0]);
        as->Asm("clc");
        as->Asm("adc vbmI ; height of buffer");
        as->Asm("sta " + as->m_internalZP[0]);
        as->Asm("bcc vbmCTB_NoOverflow2");
        as->Asm("inc " + as->m_internalZP[0] +"+1");
    as->Label("vbmCTB_NoOverflow2");

        as->Asm("ldy vbmY");

        as->Asm("dex");
        as->Asm("bne vbmCTB_Loop");

}
void Methods6502Vic20::vbmCopyToBuffer(Assembler *as)
{

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmCopyToBuffer","InitVbmCopyToBuffer");

    if (as->m_internalZP.count()==0)
        return;
    if (as->m_internalZP.count() < 1) {
        ErrorHandler::e.Error("This TRSE command needs at least 1 temporary ZP pointers but has less. Check the TRSE settings for temporary pointers.", m_node->m_op.m_lineNumber);
    }

    as->Comment("----------");
    as->Comment("VBM CopyToBuffer address, width-in-chars, height-in-pixels");

    // address 1 - Buffer address
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr1 = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr1 = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr1 = var->getValue(as);

    as->Comment("Buffer to use:");
    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr1 );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda " + addr1 +"+1" );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    } else {
        as->Asm("lda #<" + addr1 );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda #>" + addr1 );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    }

    //  Width - chars
    if (m_node->m_params[1]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[1]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("width is complex");
        LoadVar(as, 1);
    }
    as->Asm("tax ; x width in chars");

    //  Height - pixels
    if (m_node->m_params[2]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[2]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("height is complex");
        LoadVar(as, 2);
    }
    as->Asm("tay ; y height in pixels");

    as->Asm("jsr vbmCopyToBuffer");

}

void Methods6502Vic20::initVbmCopyFromBuffer(Assembler* as)
{
    if (m_node->m_isInitialized["vbmCopyFromBuffer"])
        return;

    m_node->m_isInitialized["vbmCopyFromBuffer"] = true;

    as->Comment("Copy buffer data in memory to the bitmap");
    as->Comment("buffer address = " + as->m_internalZP[0]);

    as->Label("vbmCopyFromBuffer");

        as->Comment("X and Y regs to be loaded with width / height");
        as->Asm("sty vbmI");
        as->Asm("dey ; need one less on Y as will go from 0 - number");
        as->Asm("sty vbmY");

    as->Label("vbmCFB_Loop");

        as->Asm("lda (" + as->m_internalZP[0] + "),y");
        as->Asm("sta (screenmemory),y");
        as->Asm("dey");

        as->Asm("bpl vbmCFB_Loop");

        as->Asm("lda screenmemory");
        as->Asm("clc");
        as->Asm("adc vbmScrHeight ; #192 ; note - not compatible with screen scroll");
        as->Asm("sta screenmemory");
        as->Asm("bcc vbmCFB_NoOverflow");
        as->Asm("inc screenmemory+1");
    as->Label("vbmCFB_NoOverflow");

        as->Asm("lda " + as->m_internalZP[0]);
        as->Asm("clc");
        as->Asm("adc vbmI ; height of buffer");
        as->Asm("sta " + as->m_internalZP[0]);
        as->Asm("bcc vbmCFB_NoOverflow2");
        as->Asm("inc " + as->m_internalZP[0] +"+1");
    as->Label("vbmCFB_NoOverflow2");

        as->Asm("ldy vbmY");

        as->Asm("dex");
        as->Asm("bne vbmCFB_Loop");

}
void Methods6502Vic20::vbmCopyFromBuffer(Assembler *as)
{

    VerifyInitialized("vbm","InitVbm");
    VerifyInitialized("vbmCopyFromBuffer","InitVbmCopyFromBuffer");

    if (as->m_internalZP.count()==0)
        return;
    if (as->m_internalZP.count() < 1) {
        ErrorHandler::e.Error("This TRSE command needs at least 1 temporary ZP pointers but has less. Check the TRSE settings for temporary pointers.", m_node->m_op.m_lineNumber);
    }

    as->Comment("----------");
    as->Comment("VBM CopyFromBuffer address, width-in-chars, height-in-pixels");

    // address 1 - Buffer address
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be pointer or address", m_node->m_op.m_lineNumber);
    }
    QString addr1 = "";
    if (m_node->m_params[0]->isPureNumeric())
        addr1 = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr1 = var->getValue(as);

    as->Comment("Buffer to use:");
    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Asm("lda " + addr1 );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda " + addr1 +"+1" );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    } else {
        as->Asm("lda #<" + addr1 );
        as->Asm("sta " + as->m_internalZP[0] );
        as->Asm("lda #>" + addr1 );
        as->Asm("sta " + as->m_internalZP[0] + "+1" );
    }

    //  Width - chars
    if (m_node->m_params[1]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[1]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("width is complex");
        LoadVar(as, 1);
    }
    as->Asm("tax ; x width in chars");

    //  Height - pixels
    if (m_node->m_params[2]->isPureNumeric()) {
        // pure numeric
        as->Asm( "lda #" + QString::number( m_node->m_params[2]->getValueAsInt(as)  ) );
    } else {
        // complex
        as->Comment("height is complex");
        LoadVar(as, 2);
    }
    as->Asm("tay ; y height in pixels");

    as->Asm("jsr vbmCopyFromBuffer");

}

void Methods6502Vic20::VIAIRQ(Assembler *as)
{
    QSharedPointer<NodeProcedure> addr = qSharedPointerDynamicCast<NodeProcedure>(m_node->m_params[0]);
    QString name = addr->m_procedure->m_procName;
    m_node->RequireNumber(m_node->m_params[2], "RasterIRQ", m_node->m_op.m_lineNumber);
    //    QSharedPointer<NodeNumber> num = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[2]);


    as->Asm("lda #<"+name);
    //    as->Asm("sta pointers_vic_raster+1");
    as->Asm("sta "+as->m_replaceValues["@VIA_ZP3"]);
    as->Asm("lda #>"+name);
    //    as->Asm("sta pointers_vic_raster+6");
    as->Asm("sta "+as->m_replaceValues["@VIA_ZP4"]);

    m_node->m_params[1]->Accept(m_dispatcher);
    as->Term();
    //    as->Asm("sta timers_vic_raster+1");
    as->Asm("sta "+as->m_replaceValues["@VIA_ZP1"]);
    m_node->m_params[2]->Accept(m_dispatcher);
    as->Term();
    //    as->Asm("sta timers_vic_raster+3");
    as->Asm("sta "+as->m_replaceValues["@VIA_ZP2"]);

    as->Asm("ldx #0"); // Start timer from raster line 0
    as->Asm("jsr init_via_irq");
}

void Methods6502Vic20::VIARasterIRQ(Assembler *as)
{
    QSharedPointer<NodeProcedure> addr = qSharedPointerDynamicCast<NodeProcedure>(m_node->m_params[0]);
    QString name = addr->m_procedure->m_procName;
    m_node->RequireNumber(m_node->m_params[2], "RasterIRQ", m_node->m_op.m_lineNumber);

    as->Asm("lda #<"+name);
    as->Asm("sta "+as->m_replaceValues["@VIA_ZP3"]);
    //  as->Asm("sta pointers_vic_raster+1");
    as->Asm("lda #>"+name);
    //    as->Asm("sta pointers_vic_raster+6");
    as->Asm("sta "+as->m_replaceValues["@VIA_ZP4"]);

    LoadVar(as,1);
    as->Asm("tax");

    QString lbl1 = as->NewLabel("viarasterirq_ntsc_timing");
    QString lbl2 = as->NewLabel("viarasterirq_end");

    LoadVar(as,2);
    as->Asm("cmp #0");
    as->Asm("bne " + lbl1);
    as->Asm("lda #$86");
    //    as->Asm("sta timers_vic_raster+1");
    as->Asm("sta "+as->m_replaceValues["@VIA_ZP1"]);

    as->Asm("lda #$56");
    //   as->Asm("sta timers_vic_raster+3");
    as->Asm("sta "+as->m_replaceValues["@VIA_ZP2"]);
    as->Asm("jsr A0_vic_raster");
    as->Asm("jmp " + lbl2);

    as->Label(lbl1);
    as->Asm("lda #$43");
    //    as->Asm("sta timers_vic_raster+1");
    as->Asm("sta "+as->m_replaceValues["@VIA_ZP1"]);

    as->Asm("lda #$42");
    //    as->Asm("sta timers_vic_raster+3");
    as->Asm("sta "+as->m_replaceValues["@VIA_ZP2"]);

    as->Asm("jsr A0_vic_raster");

    as->Label(lbl2);
}

void Methods6502Vic20::InitVIAIRQ(Assembler *as)
{
    as->IncludeFile(":resources/code/vic20_irq.asm");
}






/*
 *
 */


