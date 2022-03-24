#include "methods6502.h"

Methods6502::Methods6502()
{

}



bool Methods6502::Command(QString name)
{
    return m_node->m_procName.toLower() == name.toLower();
}

void Methods6502::PointToVera(Assembler* as, int address, bool initLow)
{
    as->Asm("lda #"+Util::numToHex((address>>16)&0xff));
    as->Asm("sta $9F22");
    as->Asm("lda #"+Util::numToHex((address>>8)&0xff));
    as->Asm("sta $9F21");
//        as->Asm("lda #0");
    if (initLow) {
        as->Asm("lda #"+Util::numToHex((address)&0xff));
        as->Asm("sta $9F20");
    }

}




void Methods6502::Assemble(Assembler *as, AbstractCodeGen* dispatcher) {


    m_codeGen = dispatcher;
/*    if (Command("Writeln")) {
        as->Writeln();

        m_node->m_params[0]->Accept(dispatcher);
        as->EndWriteln();
    }
*/

    if (Command("EnableAllRam")) {
            as->Comment("Enable all ram visible");
/*            as->Asm("lda $01");
            as->Asm("and #%11111100");
  //          as->Asm("and #%00");
//            as->Asm("ora #%100");
            as->Asm("sta $01");*/

            as->Asm("lda $01");
            as->Asm("and #%00");
            as->Asm("ora #%100");
            as->Asm("sta $01");


    }



    if (Command("strtolower"))
        StrToLower(as,true);
    else
    if (Command("strtoupper"))
        StrToLower(as,false);
    else
    if (Command("strcmp"))
        StrCmp(as);
    else
    if (Command("strsplit"))
        StrSplit(as);
    else
    if (Command("strgetfromindex"))
        StrGetFromIndex(as);
    else

    if (Command("ProcedureToPointer")) {
        ErrorHandler::e.Error("'ProcedureToPointer' has been deprecated. Please simply use the address of the procedure ('pointer := #myProcedure;') instead.",m_node->m_op.m_lineNumber);
/*       QSharedPointer<NodeProcedure> addr = qSharedPointerDynamicCast<NodeProcedure>(m_node->m_params[0]);
        if (addr==nullptr)
            ErrorHandler::e.Error("ProcedureToPointer parameter must be a procedure.", m_node->m_op.m_lineNumber);

        QString name = addr->m_procedure->m_procName;
        as->Asm("lda #<"+name);
        as->Asm("ldy #>"+name);
*/
    }
    else
    if (Command("min"))
        MinMax(as,false);
    else
    if (Command("max"))
        MinMax(as,true);


    /*
     *
     *  OK64 methods
     *
     *
     *
     *
     * */


    if (Command("CreateInteger"))
        CreateInteger(as,"y");

    if (Command("AddBreakpoint")) {
        as->Label("trse_breakpoint_"+QString::number(as->m_currentBreakpoint++));
    }
    if (Command("disassemble")) {
        as->Label("trse_disassemble_"+QString::number(as->m_currentBreakpoint++));
    }


    if (Command("CreatePointer"))
        CreateInteger(as,"x");

    if (Command("LoadPalette"))
        LoadPalette(as);

    if (Command("ReadInput"))
        ReadInput(as);

    if (Command("PlaySong")) {
        if (as->m_symTab->m_constants.contains("SIDFILE_1_PLAY"))
            as->Asm("jsr " + (Util::numToHex(as->m_symTab->m_constants["SIDFILE_1_PLAY"]->m_value->m_fVal)));
    }

    if (Command("ToggleNMI"))
        ToggleRegisterBit(as,"$2000",7);

    if (Command("ToggleVRAM32Inc"))
        ToggleRegisterBit(as,"$2000",2);

    if (Command("SetVideoMode"))
        SetVideoMode(as);

    if (Command("enablesprites")) {
        PointToVera(as,0x0F4000,true);
        LoadVar(as,0);
        as->Asm("sta $9F23");

    }

    if (Command("inputIRQ")) {

        QSharedPointer<NodeProcedure> addr = qSharedPointerDynamicCast<NodeProcedure>(m_node->m_params[0]);
        if (addr==nullptr)
            ErrorHandler::e.Error("First parameter must be interrupt procedure!", m_node->m_op.m_lineNumber);

        QString name = addr->m_procedure->m_procName;

//        as->Asm("sei");
        as->Asm("lda     #<"+name);
        as->Asm("sta     $FFF8");
        as->Asm("lda     #>"+name);
        as->Asm("sta     $FFF9");

    }

    if (Command("SetVeraTileMode")) {

        PointToVera(as,0x0F2001,true);
        as->Asm("lda #%00110000");
        as->Asm("sta $9F23");
        as->Asm("lda #4");
        as->Asm("sta $9F20");
        as->Asm("lda #0");
        as->Asm("sta $9F23");
        as->Asm("inc $9F20");
        as->Asm("sta $9F23");


/*        verapoke(0,1,%00110000);
        verapoke(0,4,0);
        verapoke(0,5,0);
  */
    }

    if (Command("InitPalette")) {
        PointToVera(as,0x1F1000,false);
//        as->Asm("lda #0");
        LoadVar(as,0);
        as->Asm("sta $9F20");
    }

    if (Command("ReturnValue")) {
        LoadVar(as,0);
        as->Asm(m_codeGen->getReturn());
    }

    if (Command("ToggleSpriteTableAddress"))
        ToggleRegisterBit(as,"$2000",3);

    if (Command("ToggleBackgroundTableAddress"))
        ToggleRegisterBit(as,"$2000",4);

    if (Command("ToggleBorderBackground"))
        ToggleRegisterBit(as,"$2001",1);

    if (Command("ToggleBorderSprites"))
        ToggleRegisterBit(as,"$2001",2);


    if (Command("ToggleBackground"))
        ToggleRegisterBit(as,"$2001",3);

    if (Command("ToggleSprites"))
        ToggleRegisterBit(as,"$2001",4);

    if (Command("ToggleSpriteSize"))
        ToggleRegisterBit(as,"$2000",5);

    if (Command("ToggleGreyscale"))
        ToggleRegisterBit(as,"$2001",0);


    if (Command("SetNameTable")) {
        //LoadVar(as,0);
        //QString tn = as->StoreInTempVar("tempName");
        as->Asm("lda $2000");
        as->Asm("and #%11111100");
        as->Term("ora ");
        m_node->m_params[0]->Accept(m_codeGen);
        as->Term();
        as->Asm("sta $2000");
    }
    if (Command("Lo")) {
        LoHi(as,0);
    }
    if (Command("Hi")) {
        LoHi(as,1);
    }
    if (Command("bankbyte")) {
        LoHi(as,2);
    }
    if (Command("ToPointer"))
        ToPointer(as);
    if (Command("int2ptr")) {
        LoadVar(as, 0);

    }

    if (Command("SetSpriteLocation")) {
        as->Asm("lda #0");
        as->Asm("sta $2003");
        LoadVar(as,0);
        as->Asm("sta $4014");
    }
    if (Command("Split")) {
        QString lbl1 = as->NewLabel("l1");
        QString lbl2 = as->NewLabel("l2");

        as->Label(lbl1);
        as->Asm("bit $2002");
        as->Asm("bvs " +lbl1);
        as->Label(lbl2);
        as->Asm("bit $2002");
        as->Asm("bvc " +lbl2);

        as->PopLabel("l1");
        as->PopLabel("l2");
    }
    if (Command("Scroll")) {
        LoadVar(as,0);
        as->Asm("sta $2005;keep");
        LoadVar(as,1);
        as->Asm("sta $2005;keep");
    }

    if (Command("PPUBackgroundDump"))
       PPUDump(as,0x20,00,240,4);

    if (Command("PPUAttributeDump"))
        PPUDump(as,0x23,0xC0,64,1);

    if (Command("PPUDrawColumn"))
        PPUDrawColumn(as);

    if (Command("PPUSingle"))
        PPUSingle(as,0);

    if (Command("PPUPoint"))
        PPUSingle(as,1);

    if (Command("PPUPointI"))
        PPUSingle(as,3);


    if (Command("PPUWrite"))
        PPUSingle(as,2);

    if (Command("PPURead"))
        PPURead(as);

//    m; SetVeraBank;X16; b
  //  m; SetVeraAddition;X16; b


    if (Command("SetVeraBank")) {
        LoadVar(as,0);
        QString zp = as->m_internalZP[0];
        as->Asm("sta "+zp);
        as->Asm("lda $9F22");
        as->Asm("and #%11110000");
        as->Asm("ora "+zp);
        as->Asm("sta $9F22");

    }
    if (Command("SetVeraIncrement")) {
        LoadVar(as,0);
        QString zp = as->m_internalZP[0];
        as->Asm("asl");
        as->Asm("asl");
        as->Asm("asl");
        as->Asm("asl");
        as->Asm("sta "+zp);
        as->Asm("lda $9F22");
        as->Asm("and #%00001111");
        as->Asm("ora "+zp);
        as->Asm("sta $9F22");

    }



    if (Command("KrillLoad")) {
        KrillLoad(as,false);
    }
    if (Command("KrillLoadCompressed")) {
        KrillLoad(as,true);
    }
    if (Command("InitKrill")) {
        InitKrill(as);
    }
    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::C128
            || Syntax::s.m_currentSystem->m_system==AbstractSystem::C64) {

        if (Command("initGetKey")) {
            as->IncludeFile(":resources/code/c64_keyboard_input.asm");
        }
        if (Command("getKey")) {
            as->Asm("jsr c64_getKey");
        }
    } else if (Syntax::s.m_currentSystem->m_system==AbstractSystem::VIC20) {
        if (Command("getKey")) {
            as->Asm("lda 197");
        }
    }


    if (Command("Go80Columns")) {
        as->Comment("Go 80 columns");
        as->Asm("lda $d7");

        //;bmi jess
        as->Asm("jsr $FF5F");

    }

    if (Command("LoadSong"))
        LoadSong(as);

    if (Command("RightBitShift"))
        RightBitShift(as, true);

    if (Command("LeftBitShift"))
        RightBitShift(as, false);



    if (Command("VeraPoke"))
        VeraPoke(as,false);

    if (Command("VeraPokeDual"))
        VeraPoke(as,true);


    if (Command("init_wedge")) {
        as->IncludeFile(":resources/code/init_wedge_regular.ras");
        as->IncludeFile(":resources/code/init_wedge_bad.ras");

    }

    if (Command("Nop"))
        Nop(as);
    if (Command("KernalInterrupt")) {
        if (Syntax::s.m_currentSystem->m_system==AbstractSystem::PLUS4)
            as->Asm("jmp $fcc3        ; return to kernal interrupt routine");
        else
            as->Asm("jmp $ea81        ; return to kernal interrupt routine");
    }

    if (Command("Loop"))
        as->Asm("jmp * ; loop like (�/%");

    if (Command("Call")) {
        Call(as);
    }

    if (Command("Abs")) {
        Abs(as);
    }
    if (Command("mod"))
        Modulo(as);
    if (Command("mod16"))
        Modulo16(as);

    if (Command("IsOverlapping")) {
        IsOverlapping(as);
    }
    if (Command("IsOverlappingWH")) {
        IsOverlappingWH(as);
    }

    if (Command("initsqrt16")) {
        InitSqrt16(as);
    }
    if (Command("initatan2")) {
        InitAtan2(as);
    }
    if (Command("include_modplayer"))
        IncludeModPlayer(as);

    if (Command("initmodplayer"))
        InitModPlayer(as);

    if (Command("playmod")) {

        as->Asm("ldx mod_save_x");
        as->Asm("lda mod_save_a");
        as->Asm("jsr music_cont");
        as->Asm("stx mod_save_x");
        as->Asm("sta mod_save_a");

    }

    if (Command("SetScreenLocation"))
        SetScreenLocation(as);

    if (Command("VDCWrite")) {
        VDCWrite(as);
    }
    if (Command("VDCInit")) {
        VDCInit(as);
    }

    if (Command("StartRasterChain")) {
        as->Asm("sei");
        DisableInterrupts(as);
        RasterIRQ(as);
        EnableRasterIRQ(as);
        if (Syntax::s.m_currentSystem->m_system==AbstractSystem::PLUS4)
            as->Asm("asl $ff09");
        else
            as->Asm("asl $d019");
        as->Asm("cli");
    }


    if (Command("Sqrt")) {
        Sqrt(as);
    }
    if (Command("Atan2")) {
        Atan2(as);
    }
    if (Command("NmiIRQ")) {
        DisableNMI(as);
    }
    if (Command("initlog2"))
        as->IncludeFile(":resources/code/log2_table.asm");


    if (Command("SetMemoryConfig")) {
        SetMemoryConfig(as);
    }
/*    if (Command("Return")) {
        as->Asm("rts");
    }*/
    if (Command("ReturnInterrupt")) {
        as->Asm("rti");
    }

    if (Command("EnableRasterIRQ")) {
        EnableRasterIRQ(as);
    }
    if (Command("BlockMemCpy")) {
        BlockMemCpy(as);
    }

    if (Command("Keypressed")) {
        KeyPressed(as);
    }

    if (Command("DisableVIC20IRQ")) {
       as->Asm("lda #$7f");
       as->Asm("sta $912e ; disable and acknowledge interrupts");
       as->Asm("sta $912d");

    }

    if (Command("clearsound")) {
        Clearsound(as);
    }

    if (Command("copybytesshift"))
        CopyBytesShift(as,true);

    if (Command("copycharsetfromrom")) {
        CopyCharsetFromRom(as);
    }
    if (Command("setcharsetlocation")) {
        SetCharsetLocation(as);
    }
    if (Command("SetCharsetAndScreenLocation")) {
        SetCharsetAndScreenLocation(as);
    }

    if (Command("setspriteloc"))
        SetSpriteLoc(as);



    if (Command("copydatatovera"))
        CopyDataToVera(as);


    if (Command("initsid")) {
        InitSid(as);
    }
/*    if (Command("initzeropage")) {
        InitZeroPage(as);
    }
*/
    if (Command("screenoff")) {
        if (Syntax::s.m_currentSystem->m_system==AbstractSystem::PLUS4) {
            as->Asm("lda $ff06");
            as->Asm("and #%11101111");
            as->Asm("sta $ff06");
        }
        else {
            as->Asm("lda $D011");
            as->Asm("and #%01101111");
            as->Asm("sta $D011");
        }
    }

    if (Command("screenon")) {
        if (Syntax::s.m_currentSystem->m_system==AbstractSystem::PLUS4) {
            as->Asm("lda $ff06");
            as->Asm("ora #%00010000");
            as->Asm("sta $ff06");
        }
        else {
            as->Asm("lda $D011");
            as->Asm("ora #%00010000");
            as->Asm("and #%01111111");
            as->Asm("sta $D011");
        }
    }

    if (Command("decrunch"))
        Decrunch(as);

    if (Command("decrunchfromindex"))
        DecrunchFromIndex(as);


    if (Command("init_decrunch")) {
        InitDecrunch(as);
    }


    if (Command("startirq")) {
        StartIRQ(as);
    }
    if (Command("startirqwedge"))
        StartIRQWedge(as);

    if (Command("closeirq"))
        CloseIRQ(as,false);


    if (Command("closeirqwedge"))
        CloseIRQ(as,true);

    if (Command("poke"))
        Poke(as);

    if (Command("createaddresstable"))
        CreateAddressTable(as);

    if (Command("addresstable"))
        AddressTable(as);


/*    if (Command("copyzpdata")
        CopyZPdata(as);
*/
    if (Command("swap"))
        Swap(as);

    if (Command("clearbitmap"))
        ClearBitmap(as);

    if (Command("clearscreen"))
        ClearScreen(as);

    if (Command("drawtextbox"))
        DrawTextBox(as);

    if (Command("drawcolortextbox"))
        DrawColorTextBox(as);

    if (Command("initdrawtextbox"))
        InitDrawTextBox(as);

    if (Command("peek"))
        Peek(as);

    if (Command("print80"))
        Print80(as);

    if (Command("fill80"))
        Fill80(as);

    if (Command("copyimagecolordata"))
        CopyImageColorData(as);

    if (Command("waitforraster"))
        WaitForRaster(as);

    if (Command("setmulticolormode")) {
        as->Comment("Multicolor mode");
        if (Syntax::s.m_currentSystem->m_system==AbstractSystem::PLUS4) {
            as->Asm("lda $ff07");
            as->Asm("ora #%00010000");
            as->Asm("sta $ff07");
        }
        else {
            as->Asm("lda #16"); // 0000 0000
            as->Asm("ora $d016");
            as->Asm("sta $d016");
        }
    }
    if (Command("setregularcolormode")) {
        as->Comment("Regularcolor mode");
        if (Syntax::s.m_currentSystem->m_system==AbstractSystem::PLUS4) {
            as->Asm("lda $ff07");
            as->Asm("and #%11101111");
            as->Asm("sta $ff07");
        }
        else {
            as->Asm("lda $d016");
            as->Asm("and #%11101111");
            as->Asm("sta $d016");
        }
    }

    if (Command("hideborderx")) {
        if (Syntax::s.m_currentSystem->m_system==AbstractSystem::PLUS4)
            ToggleRegisterBit(as,"$ff07",3,false);
        else
            ToggleRegisterBit(as,"$D016",3,false);
/*        as->Comment("Hide y border");
        as->Asm("lda $d011");
        as->Asm("and #$77  ; change bit 4");
        as->Asm("sta $d011");*/

    }
    if (Command("hidebordery")) {
        if (Syntax::s.m_currentSystem->m_system==AbstractSystem::PLUS4)
            ToggleRegisterBit(as,"$ff06",3,false);
        else
            ToggleRegisterBit(as,"$D011",3,false);

/*        as->Comment("Hide x border");
        as->Asm("lda $D016");
        as->Asm("and #$f7  ; change bit 4");
        as->Asm("sta $D016");
*/
    }
    if (Command("settextmode")) {
        as->Comment("Regular text mode ");
        if (Syntax::s.m_currentSystem->m_system==AbstractSystem::PLUS4) {
            as->Asm("lda $ff06");
            as->Asm("and #%11011111");
            as->Asm("sta $ff06");
        }
        else {
            as->Asm("lda $D011");
            as->Asm("and #%01011111");
            as->Asm("sta $D011");
        }
    }
    if (Command("setbank")) {
        SetBank(as);
    }

    if (Command("togglebit")) {
        ToggleBit(as);
    }
    if (Command("getbit")) {
        GetBit(as);
    }

    if (Command("copyhalfscreen"))
        CopyHalfScreen(as);

    if (Command("copyfullscreen")) {
        CopyFullScreen(as);
    }

    if (Command("copyfullscreenuunrolled"))
        CopyFullScreenUnrolled(as);

    if (Command("setbitmapmode")) {
        as->Comment("Bitmap mode ");
        as->Asm("lda #$3b");
        as->Asm("sta $d011");
    }


    if (Command("waitnoraster"))
        WaitNoRasterLines(as);

    if (Command("inc"))
        IncDec(as, "inc");

    if (Command("incrange"))
        IncMax(as, "inc");

    if (Command("decrange"))
        IncMax(as, "dec");


    if (Command("printdecimal"))
        PrintDecimal(as);

    if (Command("wait"))
        Wait(as);

    if (Command("dec"))
        IncDec(as, "dec");


    if (Command("memcpy"))
        MemCpy(as,false);

    if (Command("memcpyfast"))
        MemCpy(as,true);

    if (Command("memcpyunroll"))
        MemCpyUnroll(as,false);

    if (Command("memcpyunrollreverse"))
        MemCpyUnroll(as,true);


    if (Command("memcpylarge"))
        MemCpyLarge(as);


    if (Command("and"))
        BitOp(as,0);

    if (Command("or"))
        BitOp(as,1);

    if (Command("rand"))
        Rand(as);

    if (Command("random"))
        Random(as);


    if (Command("scrollx"))
       ScrollX(as);

    if (Command("scrolly"))
       ScrollY(as);

/*    if (Command("incscreenx")
            IncScreenX(as);
*/
    if (Command("inczp"))
            IncZp(as);

    if (Command("deczp"))
            DecZp(as);



    if (Command("spritepos"))
            SetSpritePos(as);

    if (Command("initeightbitmul"))
            InitEightBitMul(as);

    if (Command("init8x8mulNes"))
        Init8x8MulNes(as);

    if (Command("initprintdecimal"))
            InitPrintDecimal(as);

    if (Command("init16x8mul"))
            InitMul16x8(as);

    if (Command("init8x8muls")) {
        as->Asm("S_T1 = "+as->m_internalZP[0]);
        as->Asm("S_T2 = "+as->m_internalZP[1]);
        as->Asm("S_PRODUCT = "+as->m_internalZP[2]);
        as->IncludeFile(":resources/code/6502/mul8bit_signed.asm");
    }

    if (Command("init16x8div"))
            InitDiv16x8(as);

    if (Command("preventirq")) {
        as->Asm("sei");
    }
    if (Command("Set256CharMode")) {
        as->Asm("lda $ff07");
        as->Asm("ora #%10000000");
        as->Asm("sta $ff07");

    }

    if (Command("enableirq")) {
        as->Asm("asl $d019");
        as->Asm("cli");
    }

    if (Command("init8x8div"))
            InitDiv8x8(as);

/*    if (Command("init16x8mul")
            InitMul16x8(as);
  */
    if (Command("fill"))
        Fill(as);
    if (Command("fillfast"))
        FillFast(as);

    if (Command("initrandom"))
        InitRandom(as);

    if (Command("initrandom256"))
        InitRandom256(as);

    if (Command("transformcolors"))
        TransformColors(as);

//    if (Command("enableinterrupts")
  //      EnableInterrupts(as);

    if (Command("definescreen")) {
        DefineScreen(as);
    }
    if (Command("setcolormemoryaddress")) {
        SetColorMemoryAddress(as);
    }
    if (Command("initmoveto")) {
        InitMoveto(as);
    }
    if (Command("initmoveto80")) {
        InitMoveto80(as);
    }

    if (Command("initbcd")) {
        InitBcd(as);
    }
    if (Command("bcdadd")) {
        BcdAdd(as);
    }
    if (Command("bcdsub")) {
        BcdSub(as);
    }
    if (Command("bcdcompare")) {
        BcdCompare(as);
    }
    if (Command("bcdisequal")) {
        BcdIsEqual(as);
    }
    if (Command("bcdprint")) {
        BcdPrint(as);
    }
    if (Command("initjoystick")) {
        InitJoystick(as);
    }
    if (Command("joystick")) {
        Joystick(as);
    }
//    if (Command("initjoy")) {
//        InitJoy(as);
//    }
    if (Command("initjoy1")) {
        InitJoy1(as);
    }
    if (Command("readjoy1")) {
        ReadJoy1(as);
    }
    if (Command("initjoy2")) {
        InitJoy2(as);
    }
    if (Command("readjoy2")) {
        ReadJoy2(as);
    }
    if (Command("playsound")) {
        PlaySound(as);
    }
    if (Command("setfrequency")) {
        SetFrequency(as);
    }

    if (Command("initprintstring")) {
        InitPrintString(as);
    }
    if (Command("printnumber")) {
        PrintNumber(as);
    }

    if (Command("printstring")) {
        PrintString(as);
    }

    /*    if (Command("definesinetable")
            InitSinusTable(as);
    */

    if (Command("initsinetable"))
        InitSinusTable(as);

    if (Command("moveto"))
        MoveTo(as);

    if (Command("moveto80")) {
        MoveTo80(as);
    }

    if (Command("tile"))
        Tile(as);

    if (Command("pokescreen")) {
        PokeScreen(as, 0);
    }
    if (Command("pokescreencolor")) {
        PokeScreenColor(as, 0);
    }
    if (Command("disableciainterrupts"))
        DisableInterrupts(as);

    if (Command("rasterirq"))
        RasterIRQ(as);

    if (Command("rasterirqwedge"))
        RasterIRQWedge(as);


    if (Command("Set2MhzMode")) {
        as->Asm("lda $D030");
        as->Asm("ora #1");
        as->Asm("sta $D030");

    }
    if (Command("Set1MhzMode")) {
        as->Asm("lda $D030");
        as->Asm("and #%11111110");
        as->Asm("sta $D030");

    }

//    as->PopCounter(m_node->m_op.m_lineNumber-1);
}







void Methods6502::AddMemoryBlock(Assembler *as, int param)
{

    QSharedPointer<NodeNumber> num = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[param]);
    if (!m_node->m_params[param]->isPureNumericOrAddress())
        return;
    int val = m_node->m_params[param]->getValueAsInt(as);
    MemoryBlock b(val,val+1,MemoryBlock::USER,"Memory write");
    b.m_lineNumber = m_node->m_op.m_lineNumber;
    as->m_userWrittenBlocks.append(b);
}

void Methods6502::Poke(Assembler* as)
{
    // Optimization : if parameter 1 is zero, drop the ldx / tax
    as->Comment("Poke");
    //m_node->RequireAddress(m_node->m_params[0],"Poke", m_node->m_op.m_lineNumber);
    AddMemoryBlock(as,0);
    QSharedPointer<NodeNumber> num = (QSharedPointer<NodeNumber>)qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[1]);

    // Parameter is pointer
    if (m_node->m_params[0]->isPointer(as)) {
        // change to zp[b] := c;
        if (!m_node->m_params[0]->isPure())
            ErrorHandler::e.Error("Pointer must be pure variable",m_node->m_op.m_lineNumber);
        QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
        var->m_expr = m_node->m_params[1];
        auto assign = NodeFactory::CreateAssign(m_node->m_op,var,m_node->m_params[2]);
        assign->Accept(m_codeGen);
        return;
    }


    if (num!=nullptr && num->m_val==0) {
        as->Comment("Optimization: shift is zero");
        LoadVar(as,2);
        SaveVar(as,0);
        return;
    }
    // Optimization #2 : if parameter is num AND parameter 2 is num, just add
    QSharedPointer<NodeNumber> num2 = (QSharedPointer<NodeNumber>)qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[0]);
    if (num2!=nullptr && num!=nullptr) {
        as->Comment("Optimization: both storage and shift are constant");
        LoadVar(as,2);
        //SaveVar(as,0);
        as->Asm("sta $" + QString::number((int)(num2->m_val + num->m_val),16));
        return;
    }



    if (m_node->m_params[2]->isPure()) {
        LoadVar(as,1);
        as->Asm("tax");
        LoadVar(as,2);
        SaveVar(as,0,"x");
        return;
    }

    // Generic case
    LoadVar(as,1);
    QString zp = as->m_internalZP[2];
    as->Asm("sta "+zp);
    LoadVar(as,2);
    as->Asm("ldx "+zp);
    SaveVar(as,0,"x");

}

void Methods6502::Peek(Assembler* as)
{
    as->Comment("Peek");
    //m_node->RequireAddress(m_node->m_params[0],"Peek", m_node->m_op.m_lineNumber);

    // If pointer
    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Term("ldy ");
        m_node->m_params[1]->Accept(m_codeGen);
        as->Term();
        as->Term("lda (");
        m_node->m_params[0]->Accept(m_codeGen);
        as->Term("),y", true);
        return;
    }

    // Optimize if numeric
    QSharedPointer<NodeNumber> num = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[1]);
    if (num!=nullptr && m_node->m_params[0]->isPure()) {
        QString add = m_node->m_params[1]->getValue(as);
        QString org = m_node->m_params[0]->getValue(as);
        as->Asm("lda "+org + " + "+add.remove("#") + ";keep");
/*        as->ClearTerm();
        as->Term("lda ");
        m_node->m_params[0]->Accept(m_codeGen);
        as->Term(" + " + num->HexValue());
        as->Term();*/
        return;

    }
    LoadVar(as, 1);
    as->Asm("tax");
    LoadVar(as,0,"x");
    //SaveVar(as,2);

}

void Methods6502::Modulo(Assembler *as)
{
    as->Comment("Modulo");

/*
    if (m_node->m_params[0]->isWord(as)) {
//        InitDiv16x8()
        //as->m_internalZP[0]
        Node::flags["div16"] = true;
        LoadVar(as,1);
        as->Asm("sta "+as->m_internalZP[0]);
        as->Asm("sty "+as->m_internalZP[0]+"+1");
        LoadVar(as,0);
        as->Asm("sta "+as->m_internalZP[1]);
        if (m_node->m_params[1]->isWord(as)) {
            as->Asm("sty "+as->m_internalZP[1]+"+1");

        }
        else {
            as->Asm("ldy #0 ; force 16-bit");
            as->Asm("sty "+as->m_internalZP[1]+"+1");
        }
        as->Asm("jsr divide16x8");
        as->Asm("lda "+as->m_internalZP[2]);
        as->Asm("ldy "+as->m_internalZP[2]+"+1");
        return;
    }
    */
    LoadVar(as,1);
    QString val = as->StoreInTempVar("val");

    LoadVar(as,0);
//    QString mod = as->StoreInTempVar("modulo");
    as->Asm("sec");
    QString lbl = as->NewLabel("modulo");
    as->Label(lbl);
    as->Asm("sbc "+val);
    as->Asm("bcs "+lbl);
    as->Asm("adc "+val);


    as->PopLabel("modulo");

    as->PopTempVar();

}

void Methods6502::Modulo16(Assembler *as)
{
    as->Comment("Modulo16");


//        InitDiv16x8()
        //as->m_internalZP[0]
        LoadVar(as,1);
        as->Asm("sta "+as->m_internalZP[0]);
        as->Asm("sty "+as->m_internalZP[0]+"+1");
        LoadVar(as,0);
        as->Asm("sta "+as->m_internalZP[1]);
        if (m_node->m_params[1]->isWord(as)) {
            as->Asm("sty "+as->m_internalZP[1]+"+1");

        }
        else {
            as->Asm("ldy #0 ; force 16-bit");
            as->Asm("sty "+as->m_internalZP[1]+"+1");
        }
        as->Asm(m_codeGen->getCallSubroutine()+" divide16x8");
        as->Asm("lda "+as->m_internalZP[2]);
        as->Asm("ldy "+as->m_internalZP[2]+"+1");
        return;

}

void Methods6502::MemCpy(Assembler* as, bool isFast)
{
    //as->ClearTerm();
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    QSharedPointer<NodeNumber> num = (QSharedPointer<NodeNumber>)qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumericOrAddress()) {
        ErrorHandler::e.Error("First parameter must be variable or number", m_node->m_op.m_lineNumber);
    }
    AddMemoryBlock(as,2);

    QString addr = "";
    if (m_node->m_params[0]->isPureNumericOrAddress())
        addr = m_node->m_params[0]->HexValue();

    if (var!=nullptr)
        addr = var->getValue(as);

//    QSharedPointer<NodeNumber> num2 = (QSharedPointer<NodeNumber>)qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[1]);

    if (!m_node->m_params[1]->isPureNumericOrAddress()) {
        ErrorHandler::e.Error("Second parameter must be pure numeric", m_node->m_op.m_lineNumber);
    }
    int num2 = m_node->m_params[1]->getValueAsInt(as);
    m_node->RequireAddress(m_node->m_params[0], "MemCpy", m_node->m_op.m_lineNumber);
    m_node->RequireAddress(m_node->m_params[2], "MemCpy", m_node->m_op.m_lineNumber);

    QString ap1 = "";
    QString ap2 = "";
    QString bp1 = "";
    QString bp2 = "";

    QString x = "x";
    if (m_node->m_params[2]->getType(as)==TokenType::POINTER) {
        bp1="(";
        bp2=")";
        x="y";
    }

    if (m_node->m_params[0]->getType(as)==TokenType::POINTER)
        x="y";
    if (!isFast)
        as->Comment("memcpy");
    else
        as->Comment("memcpyfast");

    QString lbl = as->NewLabel("memcpy");
    if (!isFast)
        as->Asm("ld"+x+" #0");
    else {
        if (m_node->m_params[3]->isPureNumericOrAddress()) {
            as->Asm("ld"+x+ " #"+ QString::number((unsigned char)(Util::NumberFromStringHex(m_node->m_params[3]->getValue(as).remove("#"))-1)) );
        }
        else {
            as->Term("ld"+x+" ");
            m_node->m_params[3]->Accept(m_codeGen);
            as->Term();
            as->Asm("de"+x);
        }
    }


    as->Label(lbl);
    //LoadVar(as, 0, "x");

    if (m_node->m_params[0]->getType(as)==TokenType::POINTER)
        as->Asm("lda ("+ addr +"),"+x);
    else {
        QString v = Util::numToHex(num2);
        if (v=="$0") v=""; else v="+ "+v;

        as->Asm("lda " +addr + v + ","+x);
    }
    as->ClearTerm();


    as->Term("sta " + bp1);
    m_node->m_params[2]->Accept(m_codeGen);
    as->Term(bp2 + ","+x, true);


    if (isFast) {
        as->Asm("de"+x+"");
        as->Asm("bpl " + lbl);
    }
    else {
        as->Asm("in"+x+"");
        as->Term("cp"+x+" ");
        // BUG HERE INTEGER
        if (m_node->m_params[3]->isWord(as) && is6502()) {
            ErrorHandler::e.Error("Error: memcpy cannot take an integer as count, must be byte. ",m_node->m_op.m_lineNumber);
//            m_node->m_params[3]->setForceType(TokenType::BYTE);
        }
        if (!is6502()&&m_node->m_params[3]->isPureNumericOrAddress()) {
            as->Term("#"+Util::numToHex(m_node->m_params[3]->getValueAsInt(as)),true);
        }
        else
        m_node->m_params[3]->Accept(m_codeGen);
        as->Term();

        as->Asm("bne " + lbl);

    }

    as->PopLabel("memcpy");

}

void Methods6502::MemCpyUnroll(Assembler* as, bool isReverse)
{
    //as->ClearTerm();
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    QSharedPointer<NodeNumber> num = (QSharedPointer<NodeNumber>)qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumericOrAddress())
    {
        ErrorHandler::e.Error("First parameter must be variable or number", m_node->m_op.m_lineNumber);
    }
    QString addr = "";
    if (m_node->m_params[0]->isPureNumericOrAddress())
        addr = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr = var->getValue(as);

//    int num2 = (QSharedPointer<NodeNumber>)qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[1]);
    if (!m_node->m_params[1]->isPureNumericOrAddress()) {
        ErrorHandler::e.Error("Second parameter must be pure numeric", m_node->m_op.m_lineNumber);
    }
    if (!m_node->m_params[3]->isPureNumericOrAddress())
        ErrorHandler::e.Error("Third parameter must be pure numeric", m_node->m_op.m_lineNumber);

    int counter = m_node->m_params[3]->getValueAsInt(as);
    QString ap1 = "";
    QString ap2 = "";
    QString bp1 = "";
    QString bp2 = "";
    AddMemoryBlock(as,2);

    if (m_node->m_params[2]->getType(as)==TokenType::POINTER) {
        bp1="(";
        bp2=")";
    }

    int start = 0;
    int end = counter;
    int inc = 1;
    if (isReverse) {
        start = counter-1;
        end = 0;
        inc = -1;
        as->Asm("ldy #" +QString::number(start));
    }
    as->Comment("memcpy unrolled");
    for (int i=start;i!=end;i+=inc) {
        if (m_node->m_params[0]->getType(as)==TokenType::POINTER || m_node->m_params[2]->getType(as)==TokenType::POINTER)
        {
            if (isReverse) {
                as->Asm("dey");

            }
            else {
            if (i==0)
                as->Asm("ldy #" +QString::number(i));
            else
                as->Asm("iny");
            }
        }

        if (m_node->m_params[0]->getType(as)==TokenType::POINTER)
            as->Asm("lda ("+ addr +"),y");
        else
//            as->Asm("lda " +addr +" +  #" + m_node->m_params[1]->getValue(as) + ",y");
          as->Asm("lda " +addr +" +  " + m_node->m_params[1]->getValue(as) + " + " +Util::numToHex(i));

        as->ClearTerm();


        if (m_node->m_params[2]->isPureNumericOrAddress() || m_node->m_params[2]->isReference()) {
            as->Asm("sta " +m_node->m_params[2]->getValue(as) +" +  " +Util::numToHex(i));

        }
        else {
            as->Term("sta " + bp1);
            m_node->m_params[2]->Accept(m_codeGen);
            as->Term(bp2 + ",y", true);
        }
    }

}


void Methods6502::Rand(Assembler* as)
{
    VerifyInitialized("rand","InitRandom");
    LoadVar(as, 0);
    as->Asm("sta lowerRandom");
    LoadVar(as, 1);
    as->Asm("sta upperRandom");
    as->Asm("jsr callRandom");
    SaveVar(as,2);

}

void Methods6502::Random(Assembler* as)
{
    VerifyInitialized("random","InitRandom256");
    as->Asm(m_codeGen->getCallSubroutine()+" Random");
    //SaveVar(as,2);
}


void Methods6502::InitRandom256(Assembler *as)
{
    if (m_node->m_isInitialized["random"])
        return;
    m_node->m_isInitialized["random"] = true;

    QString lblRandomSkip = as->NewLabel("RandomSkip");

    as->Asm ("; init random256");
    as->Label("Random");

    as->Asm("lda #$01");
    as->Asm("asl");
    as->Asm("bcc "+ lblRandomSkip);
    as->Asm("eor #$4d");

    as->Label(lblRandomSkip);

    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::C128 || Syntax::s.m_currentSystem->m_system==AbstractSystem::C64)
        as->Asm("eor $dc04");
    else if (Syntax::s.m_currentSystem->m_system==AbstractSystem::VIC20)
        as->Asm("eor $9124");
    else if (Syntax::s.m_currentSystem->m_system==AbstractSystem::PLUS4)
        as->Asm("eor $ff00");
    else if (Syntax::s.m_currentSystem->m_system==AbstractSystem::X16)
        as->Asm("eor $ff00");

    as->Asm("sta Random+1");

    //as->Asm("rts");

    as->PopLabel("RandomSkip");

}

// Ensure ScreenMemory constant is present
void Methods6502::DefineScreen(Assembler *as)
{
    if (m_node->m_isInitialized["initscreen"])
        return;

    as->Label("screenmemory =  "+as->m_zeropageScreenMemory);
    as->Label("colormemory =  "+as->m_zeropageColorMemory);

    m_node->m_isInitialized["initscreen"]=true;
}

// Set the colormemory pointer to the same location on screen as the screenmemory pointer
// but in the colour space
// TODO:  This is currently only implemented for the Vic20. It could be converted for C64/128
void Methods6502::SetColorMemoryAddress(Assembler *as)
{

    as->Comment("Set ColorMemory pointer to same position on screen as ScreenMemory pointer");
    as->Comment("For VIC20 take high byte AND with $03, then add $94");
    as->Asm("lda screenmemory+1	; address of screen (high byte)");
    as->Asm("and #$03");
    as->Asm("clc");
    as->Asm("adc #$94		; bring up to colour space");
    as->Asm("sta colormemory+1");
    as->Asm("lda screenmemory");
    as->Asm("sta colormemory");

}

void Methods6502::InitMoveto(Assembler *as)
{
    if (m_node->m_isInitialized["moveto"])
        return;

    QString lbl = as->NewLabel("moveto");
    as->Asm("jmp " + lbl);
    //as->Label("screenmemory =  "+as->m_zeropageScreenMemory);
    DefineScreen(as);
//    as->Label("screen_x .byte 0 ");
//    as->Label("screen_y .byte 0 ");
    as->Label("screen_x = "+as->m_internalZP[0]);
    as->Label("screen_y = "+as->m_internalZP[1]);
    as->Label("SetScreenPosition");
    //as->Asm("lda #4");
    as->Asm("sta screenmemory+1");
    as->Asm("lda #0");
    as->Asm("sta screenmemory");
    as->Asm("ldy screen_y");
  //  as->Asm("cpy #0");
    as->Asm("beq sydone");
    as->Label("syloop");
    as->Asm("clc");
    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::C128 || Syntax::s.m_currentSystem->m_system==AbstractSystem::C64 || Syntax::s.m_currentSystem->m_system==AbstractSystem::PLUS4)
        as->Asm("adc #40");
    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::MEGA65)
        as->Asm("adc #80");
    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::VIC20)
        as->Asm("adc #22");
    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::NES)
        as->Asm("adc #32");
    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::PET)
        as->Asm("adc #80");
    as->Asm("bcc sskip");
    as->Asm("inc screenmemory+1");
    as->Label("sskip");
    as->Asm("dey");
    // Do we really need the next one?
//    as->Asm("cpy #$00");
    as->Asm("bne syloop");
    as->Label("sydone");
    as->Asm("ldx screen_x");
    //as->Asm("cpx #0");
    as->Asm("beq sxdone");
    as->Asm("clc");
    as->Asm("adc screen_x");
    as->Asm("bcc sxdone");
    as->Asm("inc screenmemory+1");
    as->Label("sxdone");
    as->Asm("sta screenmemory");
    as->Asm("rts");
    as->Label(lbl);

    as->PopLabel("moveto");
    m_node->m_isInitialized["moveto"]=true;

}

void Methods6502::InitMoveto80(Assembler *as)
{
    if (m_node->m_isInitialized["moveto80"])
        return;

    QString lbl = as->NewLabel("moveto80");
    as->Asm("jmp " + lbl);
    as->Label("screenmemory80 =  "+as->m_zeropageScreenMemory);
    as->Label("screen_x_80 .byte 0 ");
    as->Label("screen_y_80 .byte 0 ");

    as->Label("SetScreenPosition80");

    //as->Asm("lda #4");
    as->Asm("sta screenmemory80+1");
    as->Asm("lda #0");
    as->Asm("sta screenmemory80");
    as->Asm("ldy screen_y_80");
    as->Asm("cpy #0");
    as->Asm("beq sydone80");
    as->Label("syloop80");
    as->Asm("clc");
    as->Asm("adc #80");
    as->Asm("bcc sskip80");
    as->Asm("inc screenmemory80+1");
    as->Label("sskip80");
    as->Asm("dey");
    as->Asm("cpy #$00");
    as->Asm("bne syloop80");
    as->Label("sydone80");
    as->Asm("ldx screen_x_80");
    as->Asm("cpx #0");
    as->Asm("beq sxdone80");
    as->Asm("clc");
    as->Asm("adc screen_x_80");
    as->Asm("bcc sxdone80");
    as->Asm("inc screenmemory80+1");
    as->Label("sxdone80");
    as->Asm("sta screenmemory80");
    as->Label(lbl);

    as->PopLabel("moveto80");


    QString lbl1 = as->NewLabel("a_moveto80");
    QString lbl2 = as->NewLabel("b_moveto80");

    as->Asm("lda #18");
    as->Asm("sta $D600");
    as->Label(lbl1);
    as->Asm("bit $D600");
    as->Asm("bpl "+lbl1);
    as->Asm("lda screenmemory80+1");
    as->Asm("sta $D601");

    as->Asm("lda #19");
    as->Asm("sta $D600");
    as->Label(lbl2);
    as->Asm("bit $D600");
    as->Asm("bpl "+lbl2);
    as->Asm("lda screenmemory80");
    as->Asm("sta $D601");

    as->Asm("rts");


    as->PopLabel("a_moveto80");
    as->PopLabel("b_moveto80");




    m_node->m_isInitialized["moveto80"]=true;

}

void Methods6502::InitEightBitMul(Assembler *as)
{
    if (m_node->m_isInitialized["eightbitmul"])
        return;

    m_node->m_isInitialized["eightbitmul"] = true;

    QString l = as->NewLabel("multiply_eightbit");
//    as->Asm("jmp " + l);
  //  as->Label("multiplier .byte 0");
    m_codeGen->Disable16bit();
    as->Label("multiplier = " + as->m_internalZP[0]);
    as->Label("multiplier_a = " + as->m_internalZP[1]);
    as->Label("multiply_eightbit");
    as->Asm(m_codeGen->getInitProcedure());
    as->Asm("cpx #$00");
    as->Asm("beq mul_end");
    as->Asm("dex");
    as->Asm("stx "+as->m_internalZP[1]);
    as->Asm("lsr");
    as->Asm("sta multiplier");
    as->Asm("lda #$00");
//    as->Asm("sta multiplier_a");
    as->Asm("ldx #$08");
    as->Label("mul_loop");
    as->Asm("bcc mul_skip");
    as->Label("mul_mod");
    as->Asm("adc multiplier_a");
    as->Label("mul_skip");
    as->Asm("ror");
    as->Asm("ror multiplier");
    as->Asm("dex");
    as->Asm("bne mul_loop");
    as->Asm("ldx multiplier");
    as->Asm(m_codeGen->getReturn());
    as->Label("mul_end");
    as->Asm("txa");
//    as->Asm("ldy #0");
    m_codeGen->Enable16bit();
    as->Asm(m_codeGen->getReturn());




    as->Label(l);
    as->PopLabel("multiply_eightbit");


/*    as->Label("multiplier = " + as->m_internalZP[0]);
    as->Label("multiplier_a = " + as->m_internalZP[1]);
    as->Label("multiply_eightbit");
    as->Asm("cpx #$00");
    as->Asm("beq mul_end");
    as->Asm("dex");
    as->Asm("stx mul_mod+1");
    as->Asm("lsr");
    as->Asm("sta multiplier");
    as->Asm("lda #$00");
    as->Asm("ldx #$08");
    as->Label("mul_loop");
    as->Asm("bcc mul_skip");
    as->Label("mul_mod");
    as->Asm("adc #$00");
    as->Label("mul_skip");
    as->Asm("ror");
    as->Asm("ror multiplier");
    as->Asm("dex");
    as->Asm("bne mul_loop");
    as->Asm("ldx multiplier");
    as->Asm("rts");
    as->Label("mul_end");
    as->Asm("txa");
    as->Asm("rts");
    as->Label(l);
    as->PopLabel("multiply_eightbit");
*/
}

void Methods6502::Init8x8MulNes(Assembler *as)
{
    m_node->m_isInitialized["eightbitmul"] = true;

    as->Label("multiply_eightbit");    as->Label("prodlo = " + as->m_internalZP[0]);
    as->Label("factor2 = " + as->m_internalZP[1]);
    as->Label("initmul8x8_a = " + as->m_internalZP[2]);
    as->Asm("sta initmul8x8_a");
    as->Asm("lsr initmul8x8_a");
    as->Asm("sta prodlo");
    as->Asm("stx factor2");
    as->Asm("lda #0");
    as->Asm("ldy #8");
    as->Label("initmul8x8_loop");
    as->Asm("bcc initmul8x8_noadd");
    as->Asm("clc");
    as->Asm("adc factor2");
    as->Label("initmul8x8_noadd");
    as->Asm("ror initmul8x8_a");
    as->Asm("ror prodlo");
    as->Asm("dey");
    as->Asm("bne initmul8x8_loop");
    as->Asm("rts");

 /*   .proc mul8
    prodlo  = $0000
    factor2 = $0001

      ; Factor 1 is stored in the lower bits of prodlo; the low byte of
      ; the product is stored in the upper bits.
      lsr a  ; prime the carry bit for the loop
      sta prodlo
      sty factor2
      lda #0
      ldy #8
    loop:
      ; At the start of the loop, one bit of prodlo has already been
      ; shifted out into the carry.
      bcc noadd
      clc
      adc factor2
    noadd:
      ror a
      ror prodlo  ; pull another bit out for the next iteration
      dey         ; inc/dec don't modify carry; only shifts and adds do
      bne loop
      rts
    .endproc
   */
}

void Methods6502::InitPrintString(Assembler *as)
{
    if (m_node->m_isInitialized["initprintstring"])
        return;
    m_node->m_isInitialized["initprintstring"] = true;

    as->ClearTerm();
    as->Label("print_text = "+as->m_internalZP[0]);
    if (Syntax::s.m_currentSystem->CL65Syntax())
        as->Write("print_number_text: .asciiz \"    \"",0);
    else
        as->Label("print_number_text: .dc \"    \",0");
    as->Label("printstring");
    as->Asm("ldy #0");
    as->Label("printstringloop");
    as->Asm("lda (print_text),y");
    as->Asm("cmp #0 ;keep");
    as->Asm("beq printstring_done");
    as->Asm("cmp #64");
    as->Asm("bcc printstring_skip");

    as->Asm("sec");
    as->Asm("sbc #64");
//    as->Asm("cmp #28");
//    as->Asm("bcs printstring_skip");
    as->Label("printstring_skip");
    as->Asm("sta (screenmemory),y");
    as->Asm("iny");
    as->Asm("dex");
    as->Asm("cpx #0");
    as->Asm("beq printstring_done");

    as->Asm("jmp printstringloop");
    as->Label("printstring_done");



}

void Methods6502::InitDecrunch(Assembler *as)
{
    as->Label("init_decrunch");
    as->Asm("jmp end_init_decrunch");
//    as->IncludeFile(Util::path+"includes/init_decompress.asm");
    as->IncludeFile(":resources/code/init_decompress.asm");
    as->Label("end_init_decrunch");

}



void Methods6502::InitPrintDecimal(Assembler *as)
{
//    as->Label("ipd_div_lo "+as->byte+" 0");
//    as->Label("ipd_div_hi "+as->byte+" 0");

    as->DeclareInternalVariable("ipd_div_hi");
    as->DeclareInternalVariable("ipd_div_lo");
    as->Label("init_printdecimal_div10");
//div10:
    as->Asm("ldx #$11");
    as->Asm("lda #$00");
    as->Asm("clc");
    as->Label("init_printdecimal_loop");
    as->Asm("rol");
    as->Asm("cmp #$0A");
    as->Asm("bcc init_printdecimal_skip");
    as->Asm("sbc #$0A");
    as->Label("init_printdecimal_skip");
    as->Asm("rol ipd_div_lo");
    as->Asm("rol ipd_div_hi");
    as->Asm("dex");
    as->Asm("bne init_printdecimal_loop");
    //as->Asm("rts");
}

void Methods6502::PrintDecimal(Assembler *as)
{
    QString lbl= as->NewLabel("printdecimal");

    as->Asm("ldy #0");
    m_node->m_params[0]->Accept(m_codeGen);
    as->Term();
  //  as->Asm("")

    as->Asm("sta ipd_div_lo");
    as->Asm("sty ipd_div_hi");

    m_node->m_params[1]->Accept(m_codeGen);
    as->Term();
    as->Asm("tay");
    as->Label(lbl);
    as->Asm("jsr init_printdecimal_div10 ");
    as->Asm("ora #$30");
    as->Asm("sta (screenmemory),y");
    as->Asm("dey");
    as->Asm("bpl "+lbl);
//    as->Asm("rts");

 //   as->P
}




void Methods6502::PrintNumber(Assembler *as)
{

    QString lbl= as->NewLabel("printnumber_call");
    QString lbl1= as->NewLabel("printnumber_l1");
    QString lbl2= as->NewLabel("printnumber_l2");
    as->Asm("ldx #0");

    as->ClearTerm();
    m_node->m_params[0]->Accept(m_codeGen);
    as->Term();

    as->Asm("tay ;keep");
    as->Asm("and #$F0");
    as->Asm("lsr");
    as->Asm("lsr ");
    as->Asm("lsr ");
    as->Asm("lsr ");


    as->Asm("cmp #$0A");
    as->Asm("bcc " + lbl1);
    as->Asm("sec");
    as->Asm("sbc #$39");
    as->Label(lbl1);
    as->Asm("adc #$30 + #64");
    as->Asm("sta print_number_text,x");
    as->Asm("inx");
    as->Asm("tya");
    as->Asm("and #$0F");

    as->Asm("cmp #$0A");
    as->Asm("bcc " + lbl2);
    as->Asm("sec");
    as->Asm("sbc #$39");
    as->Label(lbl2);

    as->Asm("adc #$30 + #64");
    as->Asm("sta print_number_text,x");
    as->Asm("inx");
    as->Asm("lda #0");
    as->Asm("sta print_number_text,x");

    as->Asm("ldx #0");

    as->Asm("lda #<print_number_text");
    as->Asm("ldy #>print_number_text");
    as->Asm("sta print_text+0");
    as->Asm("sty print_text+1");


    as->Asm("jsr printstring");

    as->PopLabel("printnumber_call");
    as->PopLabel("printnumber_l1");
    as->PopLabel("printnumber_l2");
}

void Methods6502::PrintString(Assembler *as)
{
    QString lbl= as->NewLabel("printstring_call");
    QString lbl2= as->NewLabel("printstring_text");
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    QSharedPointer<NodeString> str = qSharedPointerDynamicCast<NodeString>(m_node->m_params[0]);

//    if (str==nullptr && var==nullptr)
  //      ErrorHandler::e.Error("PrintString must take variable or string constant");

    QString varName = "";
    if (var!=nullptr)
        varName = var->getValue(as);


    if (str!=nullptr) {
//        as->Asm("jmp " + lbl);
        varName = lbl2;
        as->m_tempVars<<as->getLabelEnding(varName) + as->String(str->m_val,true);
//        as->Label(varName + as->String(str->m_val));
        as->m_term="";
    }

    if (str!=nullptr)
        as->Label(lbl);



/*    if (var!=nullptr && var->isPointer(as)) {
        as->Asm("lda "+var->getValue(as));
        as->Asm("ldy "+var->getValue(as)+"+1");
    }
    else {




        as->Asm("clc");
        as->Asm("lda #<" +varName);
        as->Term("adc ");
        m_node->m_params[1]->Accept(m_codeGen);
        as->Term();
        as->Asm("ldy #>" +varName);
    }
*/
    if (str!=nullptr) {
        as->Asm("clc");
        as->Asm("lda #<" +varName);
        as->Term("adc ");
        m_node->m_params[1]->Accept(m_codeGen);
        as->Term();
        as->Asm("ldy #>" +varName);

    }
    else {
        as->ClearTerm();
        m_node->m_params[0]->Accept(m_codeGen);
        as->Term();
    }
    as->Asm("sta print_text+0");
    as->Asm("sty print_text+1");


//    LoadVar(as, 1);
 //   as->Asm("tay");
    LoadVar(as, 2);
    as->Asm("tax");
    as->Asm("jsr printstring");

    as->PopLabel("printstring_call");
    as->PopLabel("printstring_text");

}



// Tile a,a,a,a,b,n
// a1 = Top Left tile, a2 = Top right, a3 = bottom left, a4 = bottom right
// b = tile number
// n = screen width (eg: vic 20 has variable width screen depending on screen width register)
void Methods6502::Tile(Assembler *as) {

    //VerifyInitialized("initscreen", "InitScreen");
    DefineScreen(as);
    if (!m_node->m_params[4]->isPure())
    {
        ErrorHandler::e.Error("Tile number can only be a variable or number.");
    }
    QString nextline = Util::numToHex( m_node->m_params[5]->getValueAsInt(as)  );
    QString nextline2 = Util::numToHex( m_node->m_params[5]->getValueAsInt(as)+1  );

    as->Comment("Tile tl,tr,bl,br, tileno, screen_width");

    as->Asm("ldx " + m_node->m_params[4]->getValue(as)); // tile number

    as->Asm("lda "+ m_node->m_params[0]->getValue(as) + ",x");
    as->Asm("ldy #0");
    as->Asm("sta (screenmemory),y");

    as->Asm("lda "+ m_node->m_params[1]->getValue(as) + ",x");
    as->Asm("ldy #1");
    as->Asm("sta (screenmemory),y");

    as->Asm("lda "+ m_node->m_params[2]->getValue(as) + ",x");
    as->Asm("ldy #" + nextline);
    as->Asm("sta (screenmemory),y");

    as->Asm("lda "+ m_node->m_params[3]->getValue(as) + ",x");
    as->Asm("ldy #" + nextline2);
    as->Asm("sta (screenmemory),y");

}


void Methods6502::MoveTo(Assembler *as)
{
    //AddMemoryBlock(as,1);

    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::X16) {
//        LoadVar(as, 2);
/*        as->Asm("and #$0F");
        as->Asm("ora #$1F");
        as->Asm("sta $9F22");*/
        LoadVar(as, 1);
        as->Asm("sta $9F21");
        LoadVar(as, 0);
        as->Asm("sta $9F20");
        return;
    }


    if (m_node->m_params[0]->isPureNumericOrAddress() && m_node->m_params[1]->isPureNumericOrAddress() && as->m_symTab->m_constants.contains("SCREEN_WIDTH")) {
        // Calculate value directly, much much faster
        as->Comment("MoveTo optimization");
        int width = as->m_symTab->m_constants["SCREEN_WIDTH"]->m_value->m_fVal;
        int shift = m_node->m_params[0]->getValueAsInt(as) + m_node->m_params[1]->getValueAsInt(as)*width;
        if (m_node->m_params[2]->isPureNumericOrAddress()) {
            // Super optimized!
            as->Asm("lda #" +Util::numToHex(shift&0xff));
            as->Asm("sta screenmemory");
            as->Asm("lda #" +Util::numToHex(m_node->m_params[2]->getValueAsInt(as)+ (shift>>8)));
            as->Asm("sta screenmemory+1");
            return;
        }
        else {
            as->Asm("lda #" +Util::numToHex(shift&0xff));
            as->Asm("sta screenmemory");
            LoadVar(as, 2);
            as->Asm("clc");
            as->Asm("adc #" +Util::numToHex((shift>>8)));
            as->Asm("sta screenmemory+1");
            return;

        }
    }


    VerifyInitialized("moveto", "InitMoveto");
    LoadVar(as, 0);
    as->Asm("sta screen_x");
    LoadVar(as, 1);
    as->Asm("sta screen_y");
    LoadVar(as, 2);
    as->Asm("jsr SetScreenPosition");
}




void Methods6502::MoveTo80(Assembler *as)
{
    //AddMemoryBlock(as,1);

    VerifyInitialized("moveto80", "InitMoveto80");
    LoadVar(as, 0);
    as->Asm("sta screen_x_80");
    LoadVar(as, 1);
    as->Asm("sta screen_y_80");
    LoadVar(as, 2);
    as->Asm("jsr SetScreenPosition80");



}

void Methods6502::CopyBytesShift(Assembler *as, bool isLeft)
{

    QString lblOuter = as->NewLabel("copybytesshift_outer");
    QString lblInner = as->NewLabel("copybytesshift_inner");
    QString lblSkip = as->NewLabel("copybytesshift_skip");
    QString lblSkipInside = as->NewLabel("copybytesshift_skip2");

    QSharedPointer<NodeNumber> num = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[4]);

    QString cmd = "asl";
    if (num->m_val==1) cmd ="lsr";
    if (num->m_val==2) cmd ="rol";
    if (num->m_val==3) cmd ="ror";

    as->Asm("; CopyBytesShift");

//    LoadVar(as,2);
    as->Asm("ldy #0");
    as->Label(lblOuter);
    LoadVar(as,3);
    as->Asm("tax");

    as->Term("lda ");
    m_node->m_params[0]->Accept(m_codeGen);
    as->Term(",y",true);

    as->Asm("cpx #0");
    as->Asm("beq "+lblSkip);
    if (cmd=="ror" || cmd=="rol") {

        as->Asm("clc");
//        as->Asm("bit #128");

//        as->Asm("cmp #128");

//        as->Asm("bcc "+lblSkip);
  //      as->Asm("ora #%10000001");
    }

    as->Label(lblInner);

    as->Asm(cmd);

    if (cmd=="rol") {
        as->Asm("and #%11111110");
        as->Asm("bcc "+lblSkipInside);
        as->Asm("ora #%00000001");
        as->Label(lblSkipInside);
    }


//    as->Asm("cpx #0");
    as->Asm("dex");
    as->Asm("bne "+lblInner);

    if (cmd=="ror" || cmd=="rol") {
//        as->Asm("clc");
//        as->Asm("bcc "+lblSkip);
  //      as->Asm("ora #%10000001");
    }


    as->Label(lblSkip);
    as->Term("sta ");
    m_node->m_params[1]->Accept(m_codeGen);
    as->Term(",y",true);
    as->Asm("iny");
    as->Term("cpy ");
    m_node->m_params[2]->Accept(m_codeGen);
    as->Term();
    as->Asm("bne "+lblOuter);



    as->PopLabel("copybytesshift_outer");
    as->PopLabel("copybytesshift_inner");
    as->PopLabel("copybytesshift_skip");
    as->PopLabel("copybytesshift_skip2");

}

void Methods6502::Print80(Assembler *as)
{
    as->Comment("Print80");

    QString lbl1= as->NewLabel("print80_text");
    QString lbl2= as->NewLabel("print80_loop");

    as->Asm("ldx #0");
    as->Asm("ldy #31");
    as->Asm("sty $D600");
    as->Label(lbl1);
    as->Asm("bit $D600");
    as->Asm("bpl "+lbl1);
    as->Term("lda ");
    m_node->m_params[0]->Accept(m_codeGen);
    as->Term(",x",true);
    as->Asm("sta $D601");
    as->Asm("inx");
    as->Label(lbl2);

    as->Term("lda ");
    m_node->m_params[0]->Accept(m_codeGen);
    as->Term(",x",true);
    as->Asm("sta $D601");
    as->Asm("inx");
    as->Term("cpx ");
    m_node->m_params[1]->Accept(m_codeGen);
    as->Term();
    as->Asm("bne "+lbl2);

    as->PopLabel("print80_text");
    as->PopLabel("print80_loop");


}

void Methods6502::Fill80(Assembler *as)
{
    as->Comment("Fill80");

    QString lbl1= as->NewLabel("fill80_text");
    QString lbl2= as->NewLabel("fill80_loop");

    LoadVar(as,0);
    as->Asm("tay");
    LoadVar(as,1);
    as->Asm("tax");
    as->Asm("lda #31");
    as->Asm("sta $D600");
    as->Label(lbl1);
    as->Asm("bit $D600");
    as->Asm("bpl "+lbl1);
    as->Asm("sty $D601");
    as->Asm("dex");
    as->Label(lbl2);

    as->Asm("sty $D601");
    as->Asm("dex");
    as->Asm("bne "+lbl2);

    as->PopLabel("fill80_text");
    as->PopLabel("fill80_loop");

}

void Methods6502::KrillLoad(Assembler *as, bool isCompressed)
{
    QSharedPointer<NodeVar> varName = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
//    QSharedPointer<NodeVar> varJump = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[1]);
    if (varName==nullptr) {
        ErrorHandler::e.Error("When loading a file, first parameter must point to a zero-terminated string", m_node->m_op.m_lineNumber);
    }
/*    QString jmp = "";
    if (varJump==nullptr) {
        jmp = varJump->
    }
    QString filename = ->;*/
    if (varName->getType(as)==TokenType::POINTER) {
        as->Asm("ldx "+varName->getValue(as));
        as->Asm("ldy "+varName->getValue(as) +"+1");

    }
    else {
        as->Asm("ldx #<"+varName->getValue(as));
        as->Asm("ldy #>"+varName->getValue(as));
    }
    if (!isCompressed)
        as->Asm("jsr "+as->m_defines["_LoadrawKrill"]);
    else
        as->Asm("jsr "+Util::numToHex(Util::NumberFromStringHex(as->m_defines["_LoadrawKrill"])+0xb));
//    m_node->m_params[1]->Accept(m_codeGen);
//    as->Term();
    QString lbl = as->NewLabel("loadkrill");
    QString lblcrash = as->NewLabel("loadkrillcrash");
    as->PopLabel("loadkrill");
    as->Asm("bcc "+lbl);
    as->Label(lblcrash);
    as->Asm("inc $D021");

    as->Asm("jmp "+lblcrash);
    as->Label(lbl);

    as->PopLabel("loadkrillcrash");

/*    ldx #<filename
    ldy #>filename
    jsr	@loadraw
  */
}

void Methods6502::InitKrill(Assembler *as)
{
//    qDebug() << as->m_defines;
    as->Comment("Init Krill");
    DisableInterrupts(as);
    as->Asm("jsr " + as->m_defines["_InstallKrill"]);
    as->Asm("sei");

    QString src = as->m_defines["_ResidentLoaderSource"];
    QString dst = as->m_defines["_ResidentLoaderDestination"];

    if (src==dst)
        return;
    as->Asm("ldx #0");
    QString lbl = as->NewLabel("copylabel");
    as->Label(lbl);
    as->Asm("lda "+src+",x");
    as->Asm("sta "+dst+",x");
    as->Asm("lda "+src+"+$100"+",x");
    as->Asm("sta "+dst+"+$100"+",x");
/*    as->Asm("lda "+src+"+$200"+",x");
    as->Asm("sta "+dst+"+$200"+",x");
    as->Asm("lda "+src+"+$300"+",x");
    as->Asm("sta "+dst+"+$300"+",x");*/
    as->Asm("inx");
    as->Asm("bne " + lbl);
//    blockmemcpy(^@_ResidentLoaderSource,^@_ResidentLoaderDestination,2);


}

void Methods6502::InitRandom(Assembler *as)
{
    if (m_node->m_isInitialized["rand"])
        return;
    m_node->m_isInitialized["rand"] = true;
    as->Asm ("; init random");
    as->Label("init_random_call");
  if (Syntax::s.m_currentSystem->m_system==AbstractSystem::PLUS4) {
        // Initializing on Plus/4 is unnecessary
    }
    else {
    as->Asm("LDA #$FF");
    as->Asm("STA $D40E");
    as->Asm("STA $D40F");
    as->Asm("LDA #$80");
    as->Asm("STA $D412");
    }
    as->Asm("rts");

    as->DeclareVariable("upperRandom", "byte","0","");
    as->DeclareVariable("lowerRandom", "byte","0","");
    as->Label("callRandom");
    as->Asm("lda upperRandom");
    as->Asm("sbc lowerRandom");
    as->Asm("sta upperRandom");
    as->Label("RandomLoop");
    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::PLUS4) {
        as->Asm("LDA $ff00 ; get random value");
        as->Asm("EOR $ff1e");
    }
    else {
        as->Asm("LDA $D41B ; get random value");
    }
    as->Asm("CMP upperRandom  ; compare to");
    as->Asm("BCS RandomLoop   ; branch if value >");
    as->Asm("ADC lowerRandom");
    as->Asm("RTS");
}


void Methods6502::PokeScreen(Assembler *as, int shift)
{
    LoadVar(as, 0);
    as->Term("ldy ");
    m_node->m_params[1]->Accept(m_codeGen);
    as->Term();
    as->Asm("sta (screenmemory),y");
}

void Methods6502::PokeScreenColor(Assembler *as, int hiAddress)
{
    QSharedPointer<NodeNumber> num = (QSharedPointer<NodeNumber>)qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[3]);
    if (num==nullptr)
        ErrorHandler::e.Error("PokeScreenColor: last parameter required to be pure constant number");



    LoadVar(as, 0);
    as->Term("ldy ");
    m_node->m_params[2]->Accept(m_codeGen);
    as->Term();
    as->Asm("sta (screenmemory),y");
    for (int i=0;i<num->m_val-1;i++) {
        as->Asm("iny");
        as->Asm("sta (screenmemory),y");
    }

    as->Asm("lda screenmemory+1");
    as->Asm("tax");
    as->Asm("adc #$D4");
    as->Asm("sta screenmemory+1");
    //LoadVar(as, 1);
   as->Asm("lda #$F2");
    as->Asm("sta (screenmemory),y");
    for (int i=0;i<num->m_val-1;i++) {
        as->Asm("dey");
        as->Asm("sta (screenmemory),y");
    }

    as->Asm("txa");
    as->Asm("sta screenmemory+1");
}



/*(char=65, col=4, x shift 65 count 20,
                x shift, count... )
*/





void Methods6502::SetSpritePos(Assembler *as)
{

    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::X16) {
        as->Comment("Set VERA sprite position");
        as->Asm("lda #2");
        as->Asm("clc");
        as->Asm("adc $9F20"); // Point to sprite x
        as->Asm("sta $9F20"); // Point to sprite x
        LoadVar(as,0);
        as->Asm("sta $9F23");
        as->Asm("inc $9F20");
        as->Asm("sty $9F23");
        as->Asm("inc $9F20");
        LoadVar(as,1);
        as->Asm("sta $9F23");
        as->Asm("inc $9F20");
        as->Asm("sty $9F23");
        as->Asm("lda $9F20");
        as->Asm("sec");
        as->Asm("sbc #5");
        as->Asm("sta $9F20"); // Reset to zero

        return;
    }



    QString lbl = as->NewLabel("spritepos");
    QString lbl2 = as->NewLabel("spriteposcontinue");



    as->Comment("Setting sprite position");


    QSharedPointer<NodeNumber> spriteNum = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[2]);
    if (spriteNum!=nullptr) {
        as->Comment("isi-pisi: value is constant");
        uchar v = 1 << (uchar)spriteNum->m_val;
        LoadVar(as, 0);
//        as->Comment("BALLE");
        //as->Asm("tax");
        as->Asm("ldx #" +QString::number((int)spriteNum->m_val*2) );
        as->Asm("sta $D000,x");
//        qDebug() << "Spritepos : " << m_node->m_params[0]->isWord(as);
        if (m_node->m_params[0]->isWord(as)) {

//            m_node->m_params[0]->Accept(m_codeGen);
  //          as->Term("+1",true);
            as->Asm("cpy #0");
            as->Asm("beq " + lbl);
            as->Asm("lda $D010");
            as->Asm("ora #%" + QString::number(v,2) );
            as->Asm("sta $D010");
            as->Asm("jmp "+lbl2);
        }


        //m_node->m_params[3]->Build(as);
    //    as->Term();
        as->Label(lbl);

        as->Asm("lda $D010");
        as->Asm("and #%" + QString::number((uchar)(~v),2) );
      //  m_node->m_params[4]->Build(as);
      //  as->Term();
        as->Asm("sta $D010");

        as->Label(lbl2);

        as->Asm("inx");
        as->Asm("txa");
        as->Asm("tay");
        LoadVar(as, 1); // In case we load from an array
        as->Asm("sta $D000,y");

    }
    else {
        // Shift left number of blah

        LoadVar(as, 0);
        as->Asm("pha");

        m_node->m_params[2]->Accept(m_codeGen);
        as->Term();

        as->Asm("pha");
        as->Asm("tax");

        QString var = BitShiftX(as);

        as->Asm("pla"); // Get back the original st�ff

        as->Asm("asl"); // Multiply by two in the end
        as->Asm("tax"); // X is the counter
        as->Asm("pla");

        as->Asm("sta $D000,x");
        if (m_node->m_params[0]->isWord(as)) {

//            m_node->m_params[0]->Accept(m_codeGen);

//            as->Term("+1",true);
            as->Asm("cpy #0");
            as->Asm("beq " + lbl);
            as->Asm("lda $D010");
            as->Asm("ora " + var );
            //m_node->m_params[3]->Build(as);
        //    as->Term();
            as->Asm("sta $D010");
            as->Asm("jmp "+lbl2);
        }

        as->Label(lbl);

        as->Asm("lda #$FF");
        as->Asm("eor "+var);
        as->Asm("sta "+var);
        as->Asm("lda $D010");
        as->Asm("and " + var);
      //  m_node->m_params[4]->Build(as);
      //  as->Term();
        as->Asm("sta $D010");

        as->Label(lbl2);

        as->Asm("inx");
        as->Asm("txa");
        as->Asm("tay");
        LoadVar(as, 1); // In case we load from an array
        as->Asm("sta $D000,y");
        as->PopTempVar();

    }


//    as->Asm("tay");


    as->PopLabel("spritepos");
    as->PopLabel("spriteposcontinue");
}

void Methods6502::Fill(Assembler *as)
{
    QString lbl = as->NewLabel("fill");
    m_node->RequireAddress(m_node->m_params[0],"Fill",m_node->m_op.m_lineNumber);
    AddMemoryBlock(as,0);
    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        if (m_node->m_params[2]->isPure()) {
            LoadVar(as,1);
            as->Asm("ldy #0");
            as->Label(lbl);
            as->Term("sta (");
            m_node->m_params[0]->Accept(m_codeGen);
            as->Term("),y", true);
            as->Asm("iny");
            as->Term("cpy ");
            m_node->m_params[2]->Accept(m_codeGen);
            as->Term();
            as->Asm("bne "+lbl);
            as->PopLabel("fill");

        }
        else {
            LoadVar(as,2);
            QString tmp = as->StoreInTempVar("filltmp");
            LoadVar(as,1);
            as->Asm("ldy #0");
            as->Label(lbl);
            as->Term("sta (");
            m_node->m_params[0]->Accept(m_codeGen);
            as->Term("),y", true);
            as->Asm("iny");
            as->Term("cpy "+tmp);
            as->Term();
            as->Asm("bne "+lbl);
            as->PopLabel("fill");
            as->PopTempVar();

        }
        return;
    }



    if (m_node->m_params[2]->isPure()) {
        LoadVar(as,1);
        as->Asm("ldx #0");
        as->Label(lbl);
        as->Term("sta ");
        m_node->m_params[0]->Accept(m_codeGen);
        as->Term(",x", true);
        as->Asm("inx");
        as->Term("cpx ");
        m_node->m_params[2]->Accept(m_codeGen);
        as->Term();
        as->Asm("bne "+lbl);
        as->PopLabel("fill");
    }
    else {
        LoadVar(as,2);
        QString tmp = as->StoreInTempVar("filltmp");
        LoadVar(as,1);
        as->Asm("ldx #0");
        as->Label(lbl);
        as->Term("sta ");
        m_node->m_params[0]->Accept(m_codeGen);
        as->Term(",x", true);
        as->Asm("inx");
        as->Term("cpx "+tmp);
        as->Term();
        as->Asm("bne "+lbl);
        as->PopLabel("fill");
        as->PopTempVar();

    }
}

void Methods6502::FillFast(Assembler *as)
{
    QString lbl = as->NewLabel("fill");
    m_node->RequireAddress(m_node->m_params[0],"Fill",m_node->m_op.m_lineNumber);
    if (!m_node->m_params[1]->isPure())
        ErrorHandler::e.Error("FillFast parameter 2 must be pure numeric/variable!",m_node->m_op.m_lineNumber);

    AddMemoryBlock(as,0);
    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {

        LoadVar(as,2);
        as->Asm("tay");
        LoadVar(as,1);
        as->Label(lbl);
        as->Term("sta (");
        m_node->m_params[0]->Accept(m_codeGen);
        as->Term("),y", true);
        as->Asm("dey");
        as->Asm("bpl "+lbl);
        as->PopLabel("fill");

        return;
    }


    LoadVar(as,2);
    as->Asm("tax");
    LoadVar(as,1);
    as->Label(lbl);
    as->Term("sta ");
    m_node->m_params[0]->Accept(m_codeGen);
    as->Term(",x", true);
    as->Asm("dex");
    as->Asm("bpl "+lbl);
    as->PopLabel("fill");

}


void Methods6502::ScrollX(Assembler *as)
{
    if (as->m_tempZeroPointers.count()==0)
        return;
    LoadVar(as,0);
    as->Comment("ScrollX method");

    as->Asm("sta " + as->m_tempZeroPointers[2]);
    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::PLUS4) {
        as->Asm("lda $ff07");
        as->Asm("and #$F8");
        as->Asm("ora " +as->m_tempZeroPointers[2]);
        as->Asm("sta $ff07");
    }
    else {
        as->Asm("lda $d016  ");
        as->Asm("and #$F8");
        as->Asm("ora " +as->m_tempZeroPointers[2]);
        as->Asm("sta $d016");
    }
}

void Methods6502::VeraPoke(Assembler *as, bool isExtended)
{
    LoadVar(as,0);
    as->Asm("sta $9F21");
    LoadVar(as,1);
    as->Asm("sta $9F20");
    LoadVar(as,2);
    as->Asm("sta $9F23");
    if (isExtended) {
//        as->Asm("inc $9F22");
        LoadVar(as,3);
        as->Asm("sta $9F23");
    }
}



void Methods6502::ScrollY(Assembler *as)
{
    if (as->m_tempZeroPointers.count()==0)
        return;

    as->Comment("ScrollY method ");
    LoadVar(as,0);
    as->Asm("sta " + as->m_tempZeroPointers[2]);

    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::PLUS4) {
        as->Asm("lda $ff06");
        as->Asm("and #$f8");
        as->Asm("ora "+as->m_tempZeroPointers[2]);
        as->Asm("sta $ff06");
    }
    else {
        as->Asm("lda $d011  ");
        as->Asm("and #$78"); // 8 = 1000
        as->Asm("ora "+as->m_tempZeroPointers[2]);
        as->Asm("and #$7F"); // 8 = 1000
        as->Asm("sta $d011");
    }
}


void Methods6502::CopyDataToVera(Assembler *as)
{
//    m; copydatatovera; X16; a, b,b,b, b

        QString lblOuter = as->NewLabel("outer");
        QString lblInner = as->NewLabel("inner");

        as->Comment("CopyDataToVera");

        LoadVar(as,1);
        as->Term();
        as->Asm("and #%00001111");
        as->Asm("ora #%00010000");
        as->Asm("sta $9F22");
        LoadVar(as,2);
        as->Term();
        as->Asm("sta $9F21");
        LoadVar(as,3);
        as->Term();
        as->Asm("sta $9F20");

        QString zp = as->m_internalZP[0];
        LoadAndStoreInZp(m_node->m_params[0],as, zp);

//        as->Asm("sta "+zp);
  //      as->Asm("sty "+zp+"+1");

        LoadVar(as,4);
        as->Asm("tax");

        as->Label(lblOuter);

        as->Asm("ldy #0");
        as->Label(lblInner);
        as->Asm("lda ("+zp+"),y");
        as->Asm("sta $9F23");
        as->Asm("dey");
        as->Asm("cpy #0");
        as->Asm("bne "+lblInner);
        as->Asm("inc "+zp+"+1");
        as->Asm("dex");
        as->Asm("cpx #0");
        as->Asm("bne "+lblOuter);



        as->PopLabel("outer");
        as->PopLabel("inner");

//    LoadVar()

}


void Methods6502::SetVideoMode(Assembler *as)
{
    // Three bytes
//vera_addr_hi:=4;
//verapoke(0,0,%00100001);

    if (!m_node->m_params[1]->isPureNumericOrAddress())
        ErrorHandler::e.Error("Parameter 2 (layer 1/2) must be 1 or 2 ",m_node->m_op.m_lineNumber);

    int num = m_node->m_params[1]->getValueAsInt(as);

    if (!(num==1 || num==2))
        ErrorHandler::e.Error("Parameter 2 (layer 1/2) must be 1 or 2 ",m_node->m_op.m_lineNumber);

    num = (num-1)*0x10;
//    $0F2000
    as->Asm("lda #$0F");
    as->Asm("sta $9F22");
    as->Asm("lda #$20");
    as->Asm("sta $9F21");
    as->Asm("lda #"+Util::numToHex(num));
    as->Asm("sta $9F20");
    QString zp = as->m_internalZP[0];
    LoadVar(as,2);
    as->Asm("sta "+zp);
    LoadVar(as,0);
    as->Asm("asl");
    as->Asm("asl");
    as->Asm("asl");
    as->Asm("asl");
    as->Asm("asl");
    as->Asm("ora "+zp);
    as->Asm("sta $9F23");



}

QString Methods6502::checkAndInitStringParameter(Assembler *as, int n)
{
    QSharedPointer<NodeString> str = qSharedPointerDynamicCast<NodeString>(m_node->m_params[n]);

    if (str!=nullptr) {
        QString name= as->NewLabel("new_string_define");

        as->m_tempVars<< name + as->String(str->m_val,true);
    //        as->Label(varName + as->String(str->m_val));
        as->m_term="";
        as->PopLabel("new_string_define");
        return name;
    }
    return "";
}

void Methods6502::RightBitShift(Assembler *as, bool isRight)
{
    as->Comment("Right bitshift");
    QString lbl = as->NewLabel("RightBitshift");

    QString addr = m_node->m_params[0]->getAddress();


  //  qDebug() << m_node->m_params[0]->m_op.m_value;
//    exit(1);
    int num = m_node->m_params[1]->getValueAsInt(as);

    as->PopLabel("RightBitshift");


    as->Asm("ldx #0");
    as->Label(lbl);

    as->Asm("lda "+addr+",x");

    if (isRight) {

        as->Asm("lsr");
        for (int i=1;i<num;i++)
            as->Asm("ror "+addr+"+"+QString::number(i*8)+" ,x");
        as->Asm("ror "+addr+",x");
    }else {
        as->Asm("lda "+addr+",x");

        as->Asm("asl");
        for (int i=1;i<num;i++)
            as->Asm("rol "+addr+"+"+QString::number(i*8)+" ,x");
       as->Asm("rol "+addr+",x");


    }

    as->Asm("inx");
    as->Asm("cpx #8");
    as->Asm("bne "+lbl);

}

void Methods6502::Joystick(Assembler *as)
{
    QSharedPointer<NodeNumber> num = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[0]);
    if (num==nullptr || num->m_val<1 || num->m_val>2)
        ErrorHandler::e.Error("Joystick requires numeric parameter : 1 or 2 (for port 1 or 2)", m_node->m_op.m_lineNumber);
    //QString port = "$dc00";
    if (as->m_internalZP.count()==0)
        return;


    if (num->m_val==1.0f) {
        as->Asm("lda #%11111111  ; CIA#1 port A = outputs ");
        as->Asm("sta $dc02             ");

        as->Asm("lda #%00000000  ; CIA#1 port B = inputs");
        as->Asm("sta $dc03             ");
        as->Asm("lda $dc01");
    }
    else {
        as->Asm("lda #%11111111  ; CIA#1 port A = outputs ");
        as->Asm("sta $dc03             ");

        as->Asm("lda #%00000000  ; CIA#1 port B = inputs");
        as->Asm("sta $dc02             ");
        as->Asm("lda $dc00");
    }

    as->Asm("sta "+as->m_internalZP[2]);
    as->Asm("jsr callJoystick");
}

void Methods6502::BitOp(Assembler *as, int type)
{
    as->ClearTerm();
    as->Term("lda ");
    m_node->m_params[0]->Accept(m_codeGen);
    as->Term();
    if (type==0)
        as->Term("and ");
    if (type==1)
        as->Term("ora ");
    m_node->m_params[1]->Accept(m_codeGen);
    as->Term();
    as->Term("sta ");
    m_node->m_params[0]->Accept(m_codeGen);
    as->Term();
}



void Methods6502::PlaySound(Assembler *as)
{
//    LoadVar(as, 0);
    QSharedPointer<NodeNumber>num = (QSharedPointer<NodeNumber>)qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[0]);
    if (num==nullptr) {
        ErrorHandler::e.Error("First value must be constant - addresses chip");
    }
    QString add = " + " + num->HexValue();

    LoadVar(as, 1);
    as->Asm("sta 54272 +24" );
    LoadVar(as, 2);
    as->Asm("sta 54272 +1" + add);
    LoadVar(as, 3);
    as->Asm("sta 54272 +5"+add);
    LoadVar(as, 4);
    as->Asm("sta 54272 +6"+add);
    LoadVar(as, 5);
    as->Asm("sta 54272 +$4"+add);
    LoadVar(as, 6);
    as->Asm("sta 54272 +$4"+add);

    //    if (num->m_val==1) {
 //       as->
 //   }

}

void Methods6502::CreateInteger(Assembler *as, QString reg)
{

    LoadVar(as, 0);
    as->Asm("tay");
    LoadVar(as, 1);
}

void Methods6502::LoHi(Assembler *as, int type)
{
//    qDebug() << m_node->m_params.count();

     QSharedPointer<NodeProcedure> proc = qSharedPointerDynamicCast<NodeProcedure>(m_node->m_params[0]);
    if (proc!=nullptr) {
            QString name = proc->m_procedure->m_procName;
            if (type==0)
                as->Asm("lda #<"+name);
            if (type==1)
                as->Asm("lda #>"+name);
            if (type==1)
                as->Asm("lda #^"+name);
            return;

     }


    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        if (type==0)
            as->Asm("lda " + m_node->m_params[0]->getAddress());
        if (type==1)
            as->Asm("lda " + m_node->m_params[0]->getAddress()+"+1");
        if (type==2)
            as->Asm("lda " + m_node->m_params[0]->getAddress()+"+2");
        return;
    }
    if (m_node->m_params[0]->getType(as)==TokenType::ADDRESS) {
        if (type==0)
            as->Asm("lda #<" + m_node->m_params[0]->getAddress());
        if (type==1)
            as->Asm("lda #>" + m_node->m_params[0]->getAddress());
        if (type==2)
            as->Asm("lda #^" + m_node->m_params[0]->getAddress());
        return;
    }
    if (m_node->m_params[0]->getType(as)==TokenType::INTEGER) {
        if (type==0)
            as->Asm("lda " + m_node->m_params[0]->getValue(as));
        if (type==1)
            as->Asm("lda " + m_node->m_params[0]->getValue(as) +"+1");
        if (type==2)
            as->Asm("lda " + m_node->m_params[0]->getValue(as) +"+2");

/*        if (isLo)
            as->Asm("lda #" + Util::numToHex(m_node->m_params[0]->getValueAsInt(as)&0xFF));
        else
            as->Asm("lda #" + Util::numToHex((m_node->m_params[0]->getValueAsInt(as)>>8)&0xFF));*/
        return;
    }
    if (m_node->m_params[0]->getType(as)==TokenType::BYTE) {
        if (type==0)
            as->Asm("lda " + m_node->m_params[0]->getValue(as));
        if (type==1)
            as->Asm("lda #0");
        if (type==2)
            as->Asm("lda #0");

        return;
    }
//    ErrorHandler::e.Error("Lo/Hi currently only supports pure values (no expressions).", m_node->m_op.m_lineNumber);

}

void Methods6502::LoadAndStoreInZp(QSharedPointer<Node> n, Assembler *as, QString zp)
{
    as->ClearTerm();
   // qDebug() << n->getType(as);

    if (n->getType(as) == TokenType::POINTER) {
        as->Comment("Loading pointer");
        as->Term("lda ");
        n->Accept(m_codeGen);
        as->Term();
        as->Asm("sta "+zp);
        as->Term("lda ");
        n->Accept(m_codeGen);
        as->Term("+1", true);
        as->Asm("sta "+zp+"+1");
        return;
    }
    if (n->getType(as) == TokenType::VAR || (n->isVariable() && n->isReference())) {
        as->Comment("Loading variable");

        as->Asm("lda #<" + n->getAddress());
        as->Asm("sta "+zp);
        as->Asm("lda #>" + n->getAddress());
        as->Asm("sta "+zp+"+1");
        return;
    }
    if (n->getType(as) == TokenType::ADDRESS) {
        as->Comment("Loading address");

        as->Asm("lda #" + Util::numToHex(Util::NumberFromStringHex(n->getAddress())&0xFF));
        as->Asm("sta "+zp);
        as->Asm("lda #" + Util::numToHex((Util::NumberFromStringHex(n->getAddress())>>8)&0xFF));
        as->Asm("sta "+zp+"+1");
        return;
    }



}

/*
 *
 *  NES METHODS
 *
 *
*/

void Methods6502::LoadPalette(Assembler* as)
{

    QString lbl = as->NewLabel("LoadPalette");

//  as->Asm("LDA $2002");
  as->Asm("LDA #$3F");
  as->Asm("STA $2006");
  as->Asm("LDA #$00");
  as->Asm("STA $2006");
  as->Asm("LDX #$00");
  as->Label(lbl);
  as->Asm("LDA "+m_node->m_params[0]->getAddress()+",x");
  as->Asm("STA $2007");
  as->Asm("INX");
  as->Asm("CPX #$20");
  as->Asm("BNE "+lbl);

  as->PopLabel("LoadPalette");

}

void Methods6502::ReadInput(Assembler *as)
{
    as->Label("buttons = " + Util::numToHex(as->m_zbyte));
    as->m_zbyte++;
    as->IncludeFile(":resources/code/nes_readinput.asm");

    as->m_symTab->Define(QSharedPointer<Symbol>(new Symbol("buttons","byte")));
}

void Methods6502::PPUDump(Assembler *as, int hi, int lo, int x, int y)
{
//  as->Asm("lda $2002");
  //m_node->m_params[0]->Accept(m_codeGen);
  LoadVar(as,1);
//  as->Asm("lda #"+Util::numToHex(hi));
  as->Asm("sta $2006;keep");
  LoadVar(as,2);
//  m_node->m_params[1]->Accept(m_codeGen);
  QString addr = m_node->m_params[0]->getAddress();
//  as->Asm("lda #"+Util::numToHex(lo));
  as->Asm("sta $2006;keep");
  for (int i=0;i<y;i++) {
      as->Asm("ldx #0");
      QString lbl = as->NewLabel("PPUDump");
      as->Label(lbl);
      as->Asm("lda "+addr+"+" +Util::numToHex(x*i) +",x");
      as->Asm("sta $2007");
      as->Asm("inx");
      as->Asm("cpx #" + Util::numToHex(x));
      as->Asm("bne "+lbl);
      as->PopLabel("PPUDump");
  }
  as->ClearTerm();
}

void Methods6502::PPUSingle(Assembler *as, int type )
{
    if (type==0 || type==1) {
        LoadVar(as,0);
        as->Asm("sta $2006");
        LoadVar(as,1);
        as->Asm("sta $2006;keep");
    }
    if (type==0) {
        LoadVar(as,2);
        as->Asm("sta $2007");
    }
    if (type==2) {
        LoadVar(as,0);
        as->Asm("sta $2007;keep");
    }

    if (type==3) {
        if (m_node->m_params[0]->isPureNumericOrAddress()) {
            as->Comment("Is pure numeric ppupoint");
            as->Asm("ldy #"+QString::number(m_node->m_params[0]->getValueAsInt(as)>>8));
            as->Asm("lda #"+QString::number(m_node->m_params[0]->getValueAsInt(as)&0xFF));
            as->Asm("sty $2006");
            as->Asm("sta $2006;keep");
            return;
        }
        LoadVar(as,0);
        as->Asm("sty $2006");
        as->Asm("sta $2006;keep");
    }

}

void Methods6502::PPURead(Assembler *as)
{
//    return;
//    as->Asm("lda $2002");
    LoadVar(as,0);
    as->Asm("sta $2006");
    LoadVar(as,1);
    as->Asm("sta $2006;keep");
//    QString addr = m_node->m_params[0]->getAddress();
//    as->Asm("sta $2006");
    as->Asm("lda $2007");

}

void Methods6502::LoadSong(Assembler *as)
{
    LoadVar(as,0);
    as->Asm("ldx #1");

    as->Asm("jsr " + (Util::numToHex(as->m_symTab->m_constants["SIDFILE_1_INIT"]->m_value->m_fVal)));

}

void Methods6502::PPUDrawColumn(Assembler *as)
{
    as->Comment("PPU Draw column");
    as->Asm("lda #%00000100");
    as->Asm("sta $2000");
    as->Asm("lda $2002");
    LoadVar(as,1);
    as->Asm("sta $2006");
    LoadVar(as,2);
    as->Asm("sta $2006;keep");
    LoadVar(as,3);
    as->Asm("tax");
    as->Asm("ldy #0");
    QString lbl = as->NewLabel("PPUDrawColumn");
    as->Label(lbl);
    as->Asm("lda "+m_node->m_params[0]->getAddress()+",y");
    as->Asm("sta $2007");
    as->Asm("iny");
    as->Asm("dex");
    as->Asm("bne "+lbl);

    as->PopLabel("PPUDrawColumn");
}


void Methods6502::SetFrequency(Assembler *as)
{
//    LoadVar(as, 0);
    QSharedPointer<NodeNumber>num = (QSharedPointer<NodeNumber>)qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[0]);
    if (num==nullptr) {
        ErrorHandler::e.Error("First value must be constant - addresses chip");
    }
    QString add = " + " + num->HexValue();

    int SID = 54272;
//    LoadVar(as, 1);
  //  as->Asm("sta 54272 +24" );
    LoadVar(as, 1);
    as->Asm("sta 54272 +1" + add);
    /*LoadVar(as, 3);
    as->Asm("sta 54272 +5"+add);
    LoadVar(as, 4);
    as->Asm("sta 54272 +6"+add);
    LoadVar(as, 5);
    as->Asm("sta 54272 +$4"+add);
    LoadVar(as, 6);
    as->Asm("sta 54272 +$4"+add);
*/
    //    if (num->m_val==1) {
 //       as->
 //   }

}

void Methods6502::StrToLower(Assembler *as, bool toLower)
{
    as->Comment("String To Lower");
    bool isPointer= m_node->m_params[0]->isPointer(as);
    QString v = m_node->m_params[0]->getValue(as);
    QString lbl = as->NewLabel("lower");
    QString lblEnd = as->NewLabel("lowerEnd");
    QString lblSkip = as->NewLabel("lowerSkip");
    QString zp = as->m_internalZP[0];

    QString low = "#$41";
    QString high = "#$5B";
    QString add = "#$20";

    if (!toLower) {
        low = "#$61";
        high = "#$7B";
        add = "#$E0";
    }

    QString x="x";
    QString p1 = "";
    QString p2 = "";
    if (isPointer) {
        x= "y";
        p1 ="(";
        p2 =")";
    }

    as->Asm("ld"+x+" #0");
    as->Label(lbl);
    as->Asm("lda "+p1+v+p2+","+x);
    as->Asm("cmp #0");
    as->Asm("beq "+lblEnd);
    as->Asm("cmp "+low);
    as->Asm("bcc "+lblSkip);
    as->Asm("cmp "+high);
    as->Asm("bcs "+lblSkip);
    as->Asm("clc");
    as->Asm("adc "+add);
    as->Asm("sta "+p1+v+p2+","+x);
    as->Label(lblSkip);
    as->Asm("in"+x);
    as->Asm("jmp "+lbl);

    as->Label(lblEnd);

    as->PopLabel("lower");

    as->PopLabel("lowerEnd");
    as->PopLabel("lowerSkip");
}

void Methods6502::StrCmp(Assembler *as)
{
    as->Comment("Compare two strings");
    bool isPointerA= m_node->m_params[0]->isPointer(as);
    bool isPointerB= m_node->m_params[1]->isPointer(as);

    QString av = checkAndInitStringParameter(as,0);
    QString bv = checkAndInitStringParameter(as,1);

    if (av=="") av = m_node->m_params[0]->getValue(as);
    if (bv=="") bv = m_node->m_params[1]->getValue(as);



    QString lbl = as->NewLabel("strcmp");
    QString lblFalse = as->NewLabel("endTrue");
    QString lblTrue = as->NewLabel("endFalse");
    QString lblFinal = as->NewLabel("endFinal");
    QString zp = as->m_internalZP[0];

    QString ax="x";
    QString ap1 = "";
    QString ap2 = "";

    QString bx="x";
    QString bp1 = "";
    QString bp2 = "";
    if (isPointerA) {
        ax= "y";
        ap1 ="(";
        ap2 =")";
    }
    if (isPointerB) {
        bx= "y";
        bp1 ="(";
        bp2 =")";
    }

    as->Asm("ld"+ax+" #0");
    if (ax!=bx)
        as->Asm("ld"+bx+" #0");

    as->Label(lbl);
    as->Asm("lda "+ap1+av+ap2+","+ax);
    as->Asm("cmp "+bp1+bv+bp2+","+bx);
    as->Asm("bne "+lblFalse);
    as->Asm("cmp #0 ;keep");
    as->Asm("beq "+lblTrue);
    as->Asm("in"+ax);
    if (ax!=bx)
        as->Asm("in"+bx);
    as->Asm("jmp "+lbl);

    as->Label(lblFalse);
    as->Asm("lda #0");
    as->Asm("jmp "+lblFinal);
    as->Label(lblTrue);
    as->Asm("lda #1");
    as->Label(lblFinal);

    as->PopLabel("strcmp");
    as->PopLabel("endTrue");

    as->PopLabel("endFalse");
    as->PopLabel("endFinal");
}

void Methods6502::StrSplit(Assembler *as)
{
    if (!m_node->m_params[0]->isPure())
        ErrorHandler::e.Error("Parameter 0 is required to be a pure address");
    if (!m_node->m_params[1]->isPure())
        ErrorHandler::e.Error("Parameter 1 is required to be a pure address");
    if (!m_node->m_params[2]->isPure())
        ErrorHandler::e.Error("Parameter 2 is required to be a pure variable or constant");
    QString c = m_node->m_params[2]->getValue(as);

    QString s1 = m_node->m_params[0]->getValue(as);
    QString s2 = m_node->m_params[1]->getValue(as);

    as->Comment("StrSplit");
    QString lbl = as->NewLabel("loop");
    QString lblSkip = as->NewLabel("skip");
    QString lblEnd = as->NewLabel("end");
    QString lbl2 = as->NewLabel("loopi");
    as->Asm("ldx #0");
    as->Asm("ldy #0");
    as->Label(lbl);
    as->Asm("lda "+s1+",x");
    as->Asm("cmp #0; keep");
    as->Asm("beq "+lblEnd);
    as->Asm("lda "+s1+",x");
    as->Asm("cmp "+c);
    as->Asm("bne "+lblSkip);
    as->Asm("lda #0"); // String stop
    as->Label(lblSkip);
    as->Asm("sta "+s2+",y");
    as->Asm("iny");
    as->Label(lbl2);
    as->Asm("inx");
/*    as->Asm("lda "+s1+",x");
    as->Asm("cmp "+c);
    as->Asm("beq "+lbl2);
*/
    as->Asm("jmp "+lbl);
    as->Label(lblEnd);
    as->Asm("sta "+s2+",y");

    as->PopLabel("loop");
    as->PopLabel("loopi");
    as->PopLabel("skip");
    as->PopLabel("end");

}

void Methods6502::StrGetFromIndex(Assembler *as)
{
//    a, i
    QString zp = as->m_internalZP[0];
   // LoadVar(as,0);
//    m_node->m_params[0]->Accept(m_codeGen);
 //   as->Term();
    LoadAddress(as,0);
    as->Term();
    as->Asm("sta "+zp+"+0");
    as->Asm("sty "+zp+"+1");

    QString lbl = as->NewLabel("cloop");
    QString lblSkip = as->NewLabel("cskip");
    QString lblSkipZP = as->NewLabel("cskipzp");
    QString lblSkipZP2 = as->NewLabel("cskipzp2");
    QString lblEnd = as->NewLabel("cend");
    LoadVar(as,1);
    as->Asm("tax");
    as->Asm("dex");
    as->Asm("cpx #255");
    as->Asm("beq "+lblSkipZP2);
    as->Asm("ldy #0");
    as->Label(lbl);
    as->Asm("lda ("+zp+"),y");
    as->Asm("cmp #0 ;keep");
    as->Asm("bne "+lblSkip);
    as->Asm("dex");
//    as->Asm("lda #0");
//    as->Asm("inc $FFe9");
    as->Asm("cpx #255");
//    as->Asm("jmp "+lblEnd);
    as->Asm("beq "+lblEnd);

    as->Label(lblSkip);
    as->Asm("clc");
    as->Asm("inc "+zp);
    as->Asm("bcc " + lblSkipZP);
    as->Asm("inc "+ zp + " +1");
    as->Label(lblSkipZP);
    as->Asm("jmp "+lbl);
    as->Label(lblEnd);

    // point to NEXT
    as->Asm("clc");
    as->Asm("inc "+zp);
    as->Asm("bcc " + lblSkipZP2);
    as->Asm("inc "+ zp + " +1");
    as->Label(lblSkipZP2);


    as->Asm("lda "+zp);
    as->Asm("ldy "+zp+"+1");

//    qDebug() << "Methods LoadAddress ETF";
    as->PopLabel("cloop");
    as->PopLabel("cskip");
    as->PopLabel("cskipzp");
    as->PopLabel("cskipzp2");
    as->PopLabel("cend");

}


void Methods6502::Clearsound(Assembler *as)
{
    // 0, 7, 14

       as->Asm("lda #0");
       as->Asm("sta 54276" );
       as->Asm("sta 54283" );
       as->Asm("sta 54290" );

 /*   as->Asm("lda #0");
    as->Asm("sta 54272" );
    as->Asm("sta 54272+7" );
    as->Asm("sta 54272+14" );
    as->Asm("sta 54272+6" );
    as->Asm("sta 54272+7+6" );
    as->Asm("sta 54272+14+6" );
    as->Asm("sei");
    as->Asm("lda #$ea");
    as->Asm("sta $0315 ");
    as->Asm("lda #$31");
    as->Asm("sta $0314 ");
    as->Asm("lda #$81");
    as->Asm("sta $dc0d");
    as->Asm("lda #0");
    as->Asm("sta $d01a");
    as->Asm("inc $d019");
    as->Asm("lda $dc0d");
    as->Asm("cli");*/
}

void Methods6502::SetCharsetLocation(Assembler *as)
{
/*    QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (v==nullptr)
        ErrorHandler::e.Error("SetCharsetLocation parameter must be an incbin address!");
  */
    QSharedPointer<NodeNumber> v = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[0]);
    if (v==nullptr)
        ErrorHandler::e.Error("SetCharsetLocation parameter must be an address!", m_node->m_op.m_lineNumber);


    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::C128 || Syntax::s.m_currentSystem->m_system==AbstractSystem::C64) {

    int n = (unsigned int)v->m_val % 0x4000;
    bool ok=false;
    uchar b = 0;
    if (n==0x00) { b=0b0000; ok=true;}
    if (n==0x0800) { b=0b0010; ok=true;}
    if (n==0x1000) { b=0b0100; ok=true;}
    if (n==0x1800) { b=0b0110; ok=true;}
    if (n==0x2000) { b=0b1000; ok=true;}
    if (n==0x2800) { b=0b1010; ok=true;}
    if (n==0x3000) { b=0b1100; ok=true;}
    if (n==0x3800) { b=0b1110; ok=true;}
    if (!ok)
        ErrorHandler::e.Error("SetCharsetLocation parameter must be one of the following values: $0000,$0800,$1000,$1800,$2000, $2800, $3000 or $3800", m_node->m_op.m_lineNumber);

    QString addr = Util::numToHex(as->m_symTab->m_constants["VIC_DATA_LOC"]->m_value->m_fVal);
    as->Asm("lda "+addr);
    as->Asm("and #%11110001");
    as->Asm("ora #"+QString::number(b));
    as->Asm("sta "+addr);
    }
    else if (Syntax::s.m_currentSystem->m_system==AbstractSystem::PLUS4) {
        uchar n = ((unsigned int)v->m_val % 0xF800) >> 8;
        as->Asm("lda $ff12"); // Charset reading from RAM
        as->Asm("and #%11111011");
        as->Asm("sta $ff12");
        as->Asm("lda $ff13"); // Charset location
        as->Asm("and #%00000111");
        as->Asm("ora #"+QString::number(n));
        as->Asm("sta $ff13");
    }
    else if (Syntax::s.m_currentSystem->m_system==AbstractSystem::VIC20) {
        int n = (unsigned int)v->m_val;
        QString v = "";
        if (n==0x8000) v="0000";
        if (n==0x8400) v="0001";
        if (n==0x8800) v="0010";
        if (n==0x8C00) v="0011";
        if (n==0x1000) v="1100";
        if (n==0x1400) v="1101";
        if (n==0x1800) v="1110";
        if (n==0x1C00) v="1111";
        if (v=="")
            ErrorHandler::e.Error("SetCharsetLocation parameter must be one of the following values: $8000, $8400, $8800,$8C00, $1000, $1400, $1800, $1C00", m_node->m_op.m_lineNumber);


        as->Asm("lda $9005");
  //      as->Asm("and #%00001111");
//        as->Asm("ora #%"+v+"0000");
        as->Asm("and #%11110000");
        as->Asm("ora #%0000"+v);
        as->Asm("sta $9005");


    }



}

void Methods6502::SetCharsetAndScreenLocation(Assembler *as)
{
    QSharedPointer<NodeNumber> vc = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[0]);
    QSharedPointer<NodeNumber> vs = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[1]);
    if (vc==nullptr)
        ErrorHandler::e.Error("SetCharsetAndScreenLocation parameter 1 and 2 must be an address", m_node->m_op.m_lineNumber);

    if (vs==nullptr)
        ErrorHandler::e.Error("SetCharsetAndScreenLocation parameter 1 and 2 must be an address", m_node->m_op.m_lineNumber);


    int n = (unsigned int)vc->m_val % 0x4000;
    bool ok=false;
    uchar b = 0;
    if (n==0x00) { b=0b0000; ok=true;}
    if (n==0x0800) { b=0b0010; ok=true;}
    if (n==0x1000) { b=0b0100; ok=true;}
    if (n==0x1800) { b=0b0110; ok=true;}
    if (n==0x2000) { b=0b1000; ok=true;}
    if (n==0x2800) { b=0b1010; ok=true;}
    if (n==0x3000) { b=0b1100; ok=true;}
    if (n==0x3800) { b=0b1110; ok=true;}

    if (!ok)
        ErrorHandler::e.Error("SetCharsetAndScreenLocation parameter must have both parameters set to valid addresses. For the screen location, every $400 byte on the current vic bank, ie $400, $800, $1C00 etc. For the charset location, every $800 byte on the current VIC bank, i.e. $0, $800,$1000 etc.", m_node->m_op.m_lineNumber);

    n = (unsigned int)vs->m_val % 0x4000;
    ok=false;
    uchar c = 0;
    if (n==0x00) { c=0b0000; ok=true;}
    if (n==0x0400) { c=0b0001; ok=true;}
    if (n==0x0800) { c=0b0010; ok=true;}
    if (n==0x0C00) { c=0b0011; ok=true;}
    if (n==0x1000) { c=0b0100; ok=true;}
    if (n==0x1400) { c=0b0101; ok=true;}
    if (n==0x1800) { c=0b0110; ok=true;}
    if (n==0x1C00) { c=0b0111; ok=true;}
    if (n==0x2000) { c=0b1000; ok=true;}
    if (n==0x2400) { c=0b1001; ok=true;}
    if (n==0x2800) { c=0b1010; ok=true;}
    if (n==0x2C00) { c=0b1011; ok=true;}
    if (n==0x3000) { c=0b1100; ok=true;}
    if (n==0x3400) { c=0b1101; ok=true;}
    if (n==0x3800) { c=0b1110; ok=true;}
    if (n==0x3C00) { c=0b1111; ok=true;}
    if (!ok)
        ErrorHandler::e.Error("SetCharsetAndScreenLocation parameter must be one of the following values: $0000,$0400,$0800,$0C00.. repeating every $1000 bytes", m_node->m_op.m_lineNumber);



/*    as->Asm("lda "+addr);
    as->Asm("and #%00001111");
    as->Asm("ora #"+QString::number(b<<4));
    as->Asm("sta "+addr);
  */


    QString addr = Util::numToHex(as->m_symTab->m_constants["VIC_DATA_LOC"]->m_value->m_fVal);
    as->Asm("lda "+addr);
    as->Asm("and #%00000001");
    as->Asm("ora #"+QString::number(b|(c<<4)));
    as->Asm("sta "+addr);

}


void Methods6502::SetScreenLocation(Assembler *as)
{
    QSharedPointer<NodeNumber> v = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[0]);
    if (v==nullptr)
        ErrorHandler::e.Error("SetScreenLocation parameter must be an address!", m_node->m_op.m_lineNumber);


    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::C128 || Syntax::s.m_currentSystem->m_system==AbstractSystem::C64) {
 //       ErrorHandler::e.Error("SetScreenLocation not implemented for C64 yet", m_node->m_op.m_lineNumber);
        int n = (unsigned int)v->m_val % 0x4000;
        bool ok=false;
        uchar b = 0;
        if (n==0x00) { b=0b0000; ok=true;}
        if (n==0x0400) { b=0b0001; ok=true;}
        if (n==0x0800) { b=0b0010; ok=true;}
        if (n==0x0C00) { b=0b0011; ok=true;}
        if (n==0x1000) { b=0b0100; ok=true;}
        if (n==0x1400) { b=0b0101; ok=true;}
        if (n==0x1800) { b=0b0110; ok=true;}
        if (n==0x1C00) { b=0b0111; ok=true;}
        if (n==0x2000) { b=0b1000; ok=true;}
        if (n==0x2400) { b=0b1001; ok=true;}
        if (n==0x2800) { b=0b1010; ok=true;}
        if (n==0x2C00) { b=0b1011; ok=true;}
        if (n==0x3000) { b=0b1100; ok=true;}
        if (n==0x3400) { b=0b1101; ok=true;}
        if (n==0x3800) { b=0b1110; ok=true;}
        if (n==0x3C00) { b=0b1111; ok=true;}
        if (!ok)
            ErrorHandler::e.Error("SetScreenLocation parameter must be one of the following values: $0000,$0400,$0800,$0C00.. repeating every $1000 bytes", m_node->m_op.m_lineNumber);

        QString addr = Util::numToHex(as->m_symTab->m_constants["VIC_DATA_LOC"]->m_value->m_fVal);
        as->Asm("lda "+addr);
        as->Asm("and #%00001111");
        as->Asm("ora #"+QString::number(b<<4));
        as->Asm("sta "+addr);

    }
    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::VIC20) {
        int n = (unsigned int)v->m_val;
        QString v = ""; // $9005
        QString w = ""; // $9002
        if (n==0x8000) v="0000";
        if (n==0x8400) v="0001";
        if (n==0x8800) v="0010";
        if (n==0x8C00) v="0011";
        if (n==0x1000) v="1100";
        if (n==0x1400) v="1101";
        if (n==0x1800) v="1110";
        if (n==0x1C00) v="1111";
        if (n==0x1E00) { v="1111"; w="1"; }

        if (v=="")
            ErrorHandler::e.Error("SetScreenLocation parameter must be one of the following values: $8000, $8400, $8800,$8C00, $1000, $1400, $1800, $1C00", m_node->m_op.m_lineNumber);

        as->Asm("lda $9005");
        as->Asm("and #%00001111");
        as->Asm("ora #%"+v+"0000");
        as->Asm("sta $9005");

        if (w!="") {
            as->Asm("lda $9002");
            as->Asm("ora #%"+w+"0000000");
            as->Asm("sta $9002");
        }
/*
$9005: bit  7   = b15 (inverted),
$9005: bits 6-4 = b12-b10,
$9002: bit  7 = b9 of screen and colour memory

BITS  15 14 13 12  11 10 09 08  07 06 05 04  03 02 01 00
REG   !7  -  -  6   5  4  7  -   -  -  -  -   -  -  -  -
       $9005------------  $9002

Default
unexp / 3k
$9002 = $96 = 1001 0110
$9005 = $f0 = 1111 0000

8k+
$9002 = $16 = 0001 0110
$9005 = $c0 = 1100 0000
*/

    }

}

void Methods6502::InitZeroPage(Assembler* as) {
/*    as->Asm("jmp initzeropage_continue");

    as->Label("initzeropage_continue");*/
}

void Methods6502::Abs(Assembler *as)
{

    if (m_node->m_params[0]->isWord(as)) {

        as->Comment("abs(x) integer");
        as->ClearTerm();
        m_node->m_params[0]->Accept(m_codeGen);
        as->Term();
        QString l = as->NewLabel("abslabel");
        as->Asm("cpy #127");
        as->Asm("bcc " + l);
        as->Asm("pha");
        as->Asm("tya");

        as->Asm("eor #$ff"); // negate hi
        as->Asm("tay");
        as->Asm("pla");
        as->Asm("eor #$ff"); // negate lo
        as->Asm("clc");
        as->Asm("adc #$01");

        as->Label(l);

        as->PopLabel("abslabel");


        return;
    }
    as->Comment("abs(x) byte");
    as->ClearTerm();
    m_node->m_params[0]->Accept(m_codeGen);
    as->Term();
    QString l = as->NewLabel("abslabel");
    as->Asm("cmp #127"); // sets the Carry flag if -ve number
    as->Asm("bcc " + l); // branch if carry clear
    as->Asm("eor #$ff"); // negate
    //as->Asm("clc"); // can save an instruction
    as->Asm("adc #$00"); // add just the carry from above
    as->Label(l);


    as->PopLabel("abslabel");

}

void Methods6502::Sqrt(Assembler *as)
{
    as->Comment("Setup sqrt");
    m_node->m_params[0]->Accept(m_codeGen);
    if (as->m_internalZP.count()<4)
        return;
    if (m_node->m_params[0]->isWord(as)) {
        as->Asm("sta " + as->m_internalZP[0]);
        as->Asm("sty " + as->m_internalZP[1]);
    }
    else
    {
        as->Asm("sta " + as->m_internalZP[0]);
        as->Asm("ldy #0");
        as->Asm("sty " + as->m_internalZP[1]);

    }
    as->Asm(m_codeGen->getCallSubroutine()+" sqrt16_init");
    as->Asm("lda " +as->m_internalZP[0]);

}

void Methods6502::CreateAddressTable(Assembler *as) {

    as->Comment("----------");
    as->Comment("DefineAddressTable address, StartValue, IncrementValue, TableSize");

    QString lblDTLoop = as->NewLabel("dtloop");
    QString lblDTNoOverflow = as->NewLabel("dtnooverflow");


    QString startValue = Util::numToHex( m_node->m_params[1]->getValueAsInt(as)  );
    QString incrementValue = Util::numToHex( m_node->m_params[2]->getValueAsInt(as)  );
    QString tableSize = Util::numToHex( ((m_node->m_params[3]->getValueAsInt(as)) - 1) * 2 ); //  ; ( 10 * 2 ) - 2     or     10-1 = 9*2 = 18


    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    QSharedPointer<NodeNumber> num = (QSharedPointer<NodeNumber>)qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumericOrAddress()) {
        ErrorHandler::e.Error("First parameter must be variable or number", m_node->m_op.m_lineNumber);
    }

    QString addr = "";
    if (m_node->m_params[0]->isPureNumericOrAddress())
        addr = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr = var->getValue(as);

    if (!m_node->m_params[1]->isPure()) {
        ErrorHandler::e.Error("second parameter must be variable or number", m_node->m_op.m_lineNumber);
    }

    as->Asm("ldy #>" + startValue);
    as->Asm("lda #<" + startValue);

    as->Asm("ldx #0");

    as->Asm("sta " + addr +",x   ; Address of table");
    as->Asm("tya");
    as->Asm("sta " + addr +"+1,x");

    as->Label( lblDTLoop );

    as->Asm("tay");
    as->Asm("lda " + addr + ",x");
    as->Asm("inx");
    as->Asm("inx");

    as->Asm("clc");
    as->Asm("adc #" + incrementValue );
    as->Asm("bcc " + lblDTNoOverflow );
    as->Asm("iny");

    as->Label( lblDTNoOverflow );

    as->Asm("sta " + addr +",x");
    as->Asm("tya");
    as->Asm("sta " + addr +"+1,x");

    as->Asm("cpx #" + tableSize ); //  ; ( 10 * 2 ) - 2     or     10-1 = 9*2 = 18
    as->Asm("bcc " + lblDTLoop );

    as->PopLabel("dtloop");
    as->PopLabel("dtnooverflow");


}

void Methods6502::AddressTable(Assembler *as) {

    as->Comment("----------");
    as->Comment("AddressTable address, xoffset, yoffset");

    QString lblDTNoOverflow = as->NewLabel("dtnooverflow");

    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    QSharedPointer<NodeNumber> num = (QSharedPointer<NodeNumber>)qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumericOrAddress()) {
        ErrorHandler::e.Error("First parameter must be variable or number", m_node->m_op.m_lineNumber);
    }

    QString addr = "";
    if (m_node->m_params[0]->isPureNumericOrAddress())
        addr = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        addr = var->getValue(as);

    if (m_node->m_params[2]->isPureNumericOrAddress()) {
        // pure numeric
        as->Asm( "ldx #" + QString::number( m_node->m_params[2]->getValueAsInt(as) * 2 ) );
    /*} else if (m_node->m_params[2]->isPure()) {
        // pure
        as->Asm( "ldx " + m_node->m_params[2]->getValue(as) ); */
    } else {
        // complex
        as->Comment("yoffset is complex");
        LoadVar(as, 2);
        as->Asm("asl ; *2");
        as->Asm("tax");
    }

    as->Asm("lda " + addr +",x   ; Address of table lo");
    as->Asm("ldy " + addr +"+1,x   ; Address of table hi");

    if (m_node->m_params[1]->isPure()) {
        // pure
        if (m_node->m_params[1]->isPureNumericOrAddress() && m_node->m_params[1]->getValueAsInt(as) == 0 ) {} else {
            as->Asm("clc");
            as->Asm("adc " + m_node->m_params[1]->getValue(as) );
            as->Asm("bcc " + lblDTNoOverflow );
            as->Asm("iny  ; overflow into high byte");
            as->Label( lblDTNoOverflow );

        }
    } else {
        // complex
        QString zp0 = as->m_internalZP[0];

        as->Comment("xoffset is complex");
        as->Asm("sta " + zp0);
        LoadVar(as, 1);
        as->Asm("clc");
        as->Asm("adc " + zp0);
        as->Asm("bcc " + lblDTNoOverflow );
        as->Asm("iny  ; overflow into high byte");
        as->Label( lblDTNoOverflow );
    }

    as->PopLabel("dtnooverflow");

}

void Methods6502::ToPointer(Assembler *as)
{
    LoadVar(as, 1);
    as->Asm("pha");
    LoadVar(as, 0);
    as->Asm("tay");
    as->Asm("pla");
}

void Methods6502::InitDrawTextBox(Assembler* as) {
    if (m_node->m_isInitialized["drawtextbox"])
        return;
    as->Comment("----------");
    as->Comment("InitDrawTextBox");
    as->Comment("addr vars: addrtableaddr,petsciitable");
    as->Write("idtb_at_lo: "+as->byte+" 0");
    as->Write("idtb_at_hi: "+as->byte+" 0");
    as->Write("idtb_petscii_tl: "+as->byte+" 0");
    as->Write("idtb_petscii_t: "+as->byte+" 0");
    as->Write("idtb_petscii_tr: "+as->byte+" 0");
    as->Write("idtb_petscii_r: "+as->byte+" 0");
    as->Write("idtb_petscii_br: "+as->byte+" 0");
    as->Write("idtb_petscii_b: "+as->byte+" 0");
    as->Write("idtb_petscii_bl: "+as->byte+" 0");
    as->Write("idtb_petscii_l: "+as->byte+" 0");
    as->Comment("temp vars: col,row,width,height");
    as->Write("idtb_t_col: "+as->byte+" 0");
    as->Write("idtb_t_row: "+as->byte+" 0");
    as->Write("idtb_t_wid: "+as->byte+" 0");
    as->Write("idtb_t_hei: "+as->byte+" 0");
    as->Write("idtb_tmp: "+as->byte+" 0");

    QString zp = as->m_internalZP[0];

    as->Label("AddrCalcRowTextBoxDraw");
    as->Comment("Calculate screen offset for row number in A");
    as->Asm("ldy idtb_at_hi");
    as->Asm("asl");
    as->Asm("clc");
    as->Asm("adc idtb_at_lo");
    as->Asm("sta " + zp);
    as->Asm("bcc NooverflowTextBox");
    as->Asm("iny");
    as->Label("NooverflowTextBox");
    as->Asm("sty " + zp + "+1");
    as->Asm("ldy #0");
    as->Asm("lda (" + zp +"),y");
    as->Asm("tax");
    as->Asm("iny");
    as->Asm("lda (" + zp +"),y");
    as->Asm("sta "+ zp + "+1");
    as->Asm("stx "+ zp);
    as->Asm("rts");

    as->Label("PerformTextBoxDraw");
    as->Comment("Draw box top");
    as->Asm("lda idtb_t_row");
    as->Asm("jsr AddrCalcRowTextBoxDraw");
    as->Asm("lda idtb_petscii_tl");
    as->Asm("ldy idtb_t_col");
    as->Asm("sta (" + zp + "),y");
    as->Asm("lda idtb_petscii_t");
    as->Label("TopLoopTextBox");
    as->Asm("iny");
    as->Asm("sta (" + zp + "),y");
    as->Asm("cpy idtb_t_wid");
    as->Asm("bne TopLoopTextBox");
    as->Asm("iny");
    as->Asm("lda idtb_petscii_tr");
    as->Asm("sta (" + zp + "),y");

    as->Comment("Draw box center");
    as->Asm("ldx idtb_t_row");
    as->Label("CenterLoopTextBox");
    as->Asm("inx");
    as->Asm("stx idtb_tmp");
    as->Asm("txa");
    as->Asm("jsr AddrCalcRowTextBoxDraw");
    as->Asm("lda idtb_petscii_l");
    as->Asm("ldy idtb_t_col");
    as->Asm("sta (" + zp + "),y");
    as->Asm("ldy idtb_t_wid");
    as->Asm("iny");
    as->Asm("lda idtb_petscii_r");
    as->Asm("sta (" + zp + "),y");
    as->Asm("ldx idtb_tmp");
    as->Asm("cpx idtb_t_hei");
    as->Asm("bne CenterLoopTextBox");

    as->Comment("Draw box bottom");
    as->Asm("inc idtb_t_hei");
    as->Asm("lda idtb_t_hei");
    as->Asm("jsr AddrCalcRowTextBoxDraw");
    as->Asm("lda idtb_petscii_bl");
    as->Asm("ldy idtb_t_col");
    as->Asm("sta (" + zp + "),y");
    as->Asm("lda idtb_petscii_b");
    as->Label("BotLoopTextBox");
    as->Asm("iny");
    as->Asm("sta (" + zp + "),y");
    as->Asm("cpy idtb_t_wid");
    as->Asm("bne BotLoopTextBox");
    as->Asm("iny");
    as->Asm("lda idtb_petscii_br");
    as->Asm("sta (" + zp + "),y");

    m_node->m_isInitialized["drawtextbox"]=true;
}

// Draw PETSCII box using address table
void Methods6502::DrawTextBox(Assembler* as) {
    as->Comment("----------");
    as->Comment("DrawTextBox addrtable, petsciiarray, column, row, width, height");
    QString lblPetsciiCopy = as->NewLabel("PetsciiCopy");
    AddMemoryBlock(as,0);

    QSharedPointer<NodeVar> addr = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    QSharedPointer<NodeVar> petscii = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[1]);
    if (addr==nullptr) {
        ErrorHandler::e.Error("First parameter must be variable containing address table", m_node->m_op.m_lineNumber);
    }
    if (petscii==nullptr) {
        ErrorHandler::e.Error("Second parameter must be variable containing array of petscii values", m_node->m_op.m_lineNumber);
    }

    QString addrval = addr->getValue(as);
    QString petval = petscii->getValue(as);
    as->Asm("lda #<" + addrval + " ; address table lo");
    as->Asm("ldx #>"+ addrval + " ; address table hi");
    as->Asm("sta idtb_at_lo");
    as->Asm("stx idtb_at_hi");

    as->Asm("ldx #8");
    as->Label(lblPetsciiCopy);
    as->Asm("dex");
    as->Asm("lda " + petval + ",x");
    as->Asm("sta idtb_petscii_tl,x");
    as->Asm("cpx #0");
    as->Asm("bne " + lblPetsciiCopy);

    if (m_node->m_params[2]->isPureNumericOrAddress()) {
        as->Asm("lda " + m_node->m_params[2]->getValue(as) );
    } else {
        LoadVar(as, 2);
    }
    as->Asm("sta idtb_t_col");

    if (m_node->m_params[3]->isPureNumericOrAddress()) {
        as->Asm("lda " + m_node->m_params[3]->getValue(as) );
    } else {
        LoadVar(as, 3);
    }
    as->Asm("sta idtb_t_row");

    if (m_node->m_params[4]->isPureNumericOrAddress()) {
        as->Asm("lda " + m_node->m_params[4]->getValue(as) );
    } else {
        LoadVar(as, 4);
    }
    as->Asm("clc");
    as->Asm("adc idtb_t_col");
    as->Asm("sbc #1");
    as->Asm("sta idtb_t_wid");

    if (m_node->m_params[5]->isPureNumericOrAddress()) {
        as->Asm("lda " + m_node->m_params[5]->getValue(as) );
    } else {
        LoadVar(as, 5);
    }
    as->Asm("clc");
    as->Asm("adc idtb_t_row");
    as->Asm("sbc #1");
    as->Asm("sta idtb_t_hei");
    as->Asm("jsr PerformTextBoxDraw");

    as->PopLabel("PetsciiCopy");
}


// Draw PETSCII box using address table
void Methods6502::DrawColorTextBox(Assembler* as) {
    as->Comment("----------");
    as->Comment("DrawColorTextBox addrtable, coloraddrtable, petsciiarray, column, row, width, height, color");
    QString lblPetsciiCopy = as->NewLabel("PetsciiCopy");
    QString lblColorCopy = as->NewLabel("ColorCopy");
    AddMemoryBlock(as,0);

    QSharedPointer<NodeVar> addr = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    QSharedPointer<NodeVar> coloraddr = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[1]);
    QSharedPointer<NodeVar> petscii = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[2]);

    if (addr==nullptr) {
        ErrorHandler::e.Error("First parameter must be variable containing screen address table", m_node->m_op.m_lineNumber);
    }
    if (coloraddr==nullptr) {
        ErrorHandler::e.Error("Second parameter must be variable containing color address table", m_node->m_op.m_lineNumber);
    }
    if (petscii==nullptr) {
        ErrorHandler::e.Error("Third parameter must be variable containing array of petscii values", m_node->m_op.m_lineNumber);
    }

    QString addrval = addr->getValue(as);
    QString colorval = coloraddr->getValue(as);
    QString petval = petscii->getValue(as);

    as->Asm("lda #<" + addrval + " ; address table lo");
    as->Asm("ldx #>"+ addrval + " ; address table hi");
    as->Asm("sta idtb_at_lo");
    as->Asm("stx idtb_at_hi");

    as->Asm("ldx #8");
    as->Label(lblPetsciiCopy);
    as->Asm("dex");
    as->Asm("lda " + petval + ",x");
    as->Asm("sta idtb_petscii_tl,x");
    as->Asm("cpx #0");
    as->Asm("bne " + lblPetsciiCopy);

    if (m_node->m_params[3]->isPureNumericOrAddress()) {
        as->Asm("lda " + m_node->m_params[3]->getValue(as) );
    } else {
        LoadVar(as, 3);
    }
    as->Asm("sta idtb_t_col");

    if (m_node->m_params[4]->isPureNumericOrAddress()) {
        as->Asm("lda " + m_node->m_params[4]->getValue(as) );
    } else {
        LoadVar(as, 4);
    }
    as->Asm("sta idtb_t_row");

    if (m_node->m_params[5]->isPureNumericOrAddress()) {
        as->Asm("lda " + m_node->m_params[5]->getValue(as) );
    } else {
        LoadVar(as, 5);
    }
    as->Asm("clc");
    as->Asm("adc idtb_t_col");
    as->Asm("sbc #1");
    as->Asm("sta idtb_t_wid");

    if (m_node->m_params[6]->isPureNumericOrAddress()) {
        as->Asm("lda " + m_node->m_params[6]->getValue(as) );
    } else {
        LoadVar(as, 6);
    }
    as->Asm("clc");
    as->Asm("adc idtb_t_row");
    as->Asm("sbc #1");
    as->Asm("sta idtb_t_hei");
    as->Asm("jsr PerformTextBoxDraw");

    as->Comment("Draw color");
    as->Asm("lda #<" + colorval + " ; address table lo");
    as->Asm("ldx #>"+ colorval + " ; address table hi");
    as->Asm("sta idtb_at_lo");
    as->Asm("stx idtb_at_hi");

    if (m_node->m_params[6]->isPureNumericOrAddress()) {
        as->Asm("lda " + m_node->m_params[6]->getValue(as) );
    } else {
        LoadVar(as, 6);
    }
    as->Asm("clc");
    as->Asm("adc idtb_t_row");
    as->Asm("sbc #1");
    as->Asm("sta idtb_t_hei");

    if (m_node->m_params[7]->isPureNumericOrAddress()) {
        as->Asm("lda " + m_node->m_params[7]->getValue(as) );
    } else {
        LoadVar(as, 7);
    }

    as->Asm("ldx #8");
    as->Label(lblColorCopy);
    as->Asm("dex");
    as->Asm("sta idtb_petscii_tl,x");
    as->Asm("bne " + lblColorCopy);
    as->Asm("jsr PerformTextBoxDraw");

    as->PopLabel("PetsciiCopy");
    as->PopLabel("ColorCopy");
}


// Calculate if x1,y1 and x2,y2 are with hitbox distance of each other
void Methods6502::IsOverlapping(Assembler *as)
{
    as->Comment("----------");
    as->Comment("IsOverlapping collision  =  x1,y1, x2,y2, dist");

    // need a negative constant
    QString distNeg = Util::numToHex( 255 - m_node->m_params[4]->getValueAsInt(as)  );

    QString lblColXConfirmed = as->NewLabel("ColXConfirmed");
    QString lblNoCollision = as->NewLabel("NoCollision");
    QString lblCollision = as->NewLabel("Collision");
    QString lblCollisionDone = as->NewLabel("CollisionDone");

    QString zp0 = as->m_internalZP[0];  // used for x/y positions
    QString zp1 = as->m_internalZP[1];  // used for -ve distance
    QString zp2 = as->m_internalZP[2];  // used for +ve distance

    // calc -ve distance
    if (!m_node->m_params[4]->isPureNumericOrAddress()) {
        as->Comment("Distance is not a constant, store in zero page addresses");
        LoadVar(as, 4); // Loads distance
        as->Asm("sta "+zp2); // Store +ve in zp
        as->Asm("eor #$ff"); // negate, but don't perform 2's compliment as bcs later needs -ve 1 smaller
        as->Asm("sta "+zp1); // Store -ve in zp
    }

    if (m_node->m_params[2]->isPure()) {                    // x2
        m_node->m_params[0]->Accept(m_codeGen);          // x1 - this handles pure and complex
        as->Term();
        as->Asm("clc");                                     // pure
        as->Asm("sbc " + m_node->m_params[2]->getValue(as));
    } else {
        as->Comment("x2 is complex");                       // complex
        LoadVar(as, 2);                                     // Loads x2
        as->Asm("sta "+zp0);                                // Store
        m_node->m_params[0]->Accept(m_codeGen);          // x1 - this handles pure and complex
        as->Term();
        as->Asm("clc");
        as->Asm("sbc " + zp0);
    }

    if (m_node->m_params[4]->isPureNumericOrAddress())               // distance on X
        as->Asm("cmp #" + distNeg );                        // -ve pure
    else
        as->Asm("cmp " + zp1 );                             // -ve complex

    as->Asm("bcs "+ lblColXConfirmed);                      // x vector 1 within distance

    if (m_node->m_params[4]->isPureNumericOrAddress())               // distance on X
        as->Asm("cmp "+ m_node->m_params[4]->getValue(as)); // +ve pure
    else
        as->Asm("cmp " + zp2 );                             // +ve complex

    as->Asm("bcs "+ lblNoCollision);                        // x vector 2 not within distance

    as->Label(lblColXConfirmed);                            // X is within +/- distance, now check Y

    if (m_node->m_params[3]->isPure()) {                    // y2
        m_node->m_params[1]->Accept(m_codeGen);          // y1 - this handles pure and complex
        as->Term();
        as->Asm("clc");
        as->Asm("sbc " + m_node->m_params[3]->getValue(as));
    }
    else {
        as->Comment("y2 is complex");                       // complex
        LoadVar(as, 3);                                     // Loads y2
        as->Asm("sta "+zp0);                                // Store
        m_node->m_params[1]->Accept(m_codeGen);          // y1 - this handles pure and complex
        as->Term();
        as->Asm("clc");
        as->Asm("sbc " + zp0);
    }

    if (m_node->m_params[4]->isPureNumericOrAddress())               // distance on Y
        as->Asm("cmp #" + distNeg );                        // -ve pure
    else
        as->Asm("cmp " + zp1 );                             // -ve complex

    as->Asm("bcs "+ lblCollision);                          // y vector 1 within distance, no more checks needed

    if (m_node->m_params[4]->isPureNumericOrAddress())               // distance on Y
        as->Asm("cmp "+ m_node->m_params[4]->getValue(as)); // +ve pure
    else
        as->Asm("cmp " + zp2 );                             // +ve complex

    as->Asm("bcs "+ lblNoCollision);                        // y vector 2 not within distance

    as->Label(lblCollision);                                // Collision, set a = 1
    as->Asm("lda #1");
    as->Asm("jmp " + lblCollisionDone);

    as->Label(lblNoCollision);                              // no collision, set a = 0
    as->Asm("lda #0");

    as->Label(lblCollisionDone);

    as->PopLabel("ColXConfirmed");
    as->PopLabel("NoCollision");
    as->PopLabel("Collision");
    as->PopLabel("CollisionDone");

}

// Calculate if x1,y1 and x2,y2 are with rectangular hitbox distance of each other
void Methods6502::IsOverlappingWH(Assembler *as)
{
    as->Comment("----------");
    as->Comment("IsOverlapping collision  =  x1,y1, x2,y2, width, height");

    // need a negative constant
    QString distNegX = Util::numToHex( 255 - m_node->m_params[4]->getValueAsInt(as)  );
    QString distNegY = Util::numToHex( 255 - m_node->m_params[5]->getValueAsInt(as)  );

    QString lblColXConfirmed = as->NewLabel("ColXConfirmed");
    QString lblNoCollision = as->NewLabel("NoCollision");
    QString lblCollision = as->NewLabel("Collision");
    QString lblCollisionDone = as->NewLabel("CollisionDone");

    QString zp0 = as->m_internalZP[0];  // used for x/y positions
    QString zp1 = as->m_internalZP[1];  // used for -ve distance
    QString zp2 = as->m_internalZP[2];  // used for +ve distance

    // calc -ve width
    if (!m_node->m_params[4]->isPureNumericOrAddress()) {
        as->Comment("Distance is not a constant, store in zero page addresses");
        LoadVar(as, 4); // Loads distance
        as->Asm("sta "+zp2); // Store +ve in zp
        as->Asm("eor #$ff"); // negate, but don't perform 2's compliment as bcs later needs -ve 1 smaller
        as->Asm("sta "+zp1); // Store -ve in zp
    }

    if (m_node->m_params[2]->isPure()) {                    // x2
        m_node->m_params[0]->Accept(m_codeGen);          // x1 - this handles pure and complex
        as->Term();
        as->Asm("clc");                                     // pure
        as->Asm("sbc " + m_node->m_params[2]->getValue(as));
    } else {
        as->Comment("x2 is complex");                       // complex
        LoadVar(as, 2);                                     // Loads x2
        as->Asm("sta "+zp0);                                // Store
        m_node->m_params[0]->Accept(m_codeGen);          // x1 - this handles pure and complex
        as->Term();
        as->Asm("clc");
        as->Asm("sbc " + zp0);
    }

    if (m_node->m_params[4]->isPureNumericOrAddress())               // distance on X
        as->Asm("cmp #" + distNegX );                        // -ve pure
    else
        as->Asm("cmp " + zp1 );                             // -ve complex

    as->Asm("bcs "+ lblColXConfirmed);                      // x vector 1 within distance

    if (m_node->m_params[4]->isPureNumericOrAddress())               // distance on X
        as->Asm("cmp "+ m_node->m_params[4]->getValue(as)); // +ve pure
    else
        as->Asm("cmp " + zp2 );                             // +ve complex

    as->Asm("bcs "+ lblNoCollision);                        // x vector 2 not within distance

    as->Label(lblColXConfirmed);                            // X is within +/- distance, now check Y

    // calc -ve height
    if (!m_node->m_params[5]->isPureNumericOrAddress()) {
        as->Comment("Distance is not a constant, store in zero page addresses");
        LoadVar(as, 5); // Loads distance
        as->Asm("sta "+zp2); // Store +ve in zp
        as->Asm("eor #$ff"); // negate, but don't perform 2's compliment as bcs later needs -ve 1 smaller
        as->Asm("sta "+zp1); // Store -ve in zp
    }

    if (m_node->m_params[3]->isPure()) {                    // y2
        m_node->m_params[1]->Accept(m_codeGen);          // y1 - this handles pure and complex
        as->Term();
        as->Asm("clc");
        as->Asm("sbc " + m_node->m_params[3]->getValue(as));
    }
    else {
        as->Comment("y2 is complex");                       // complex
        LoadVar(as, 3);                                     // Loads y2
        as->Asm("sta "+zp0);                                // Store
        m_node->m_params[1]->Accept(m_codeGen);          // y1 - this handles pure and complex
        as->Term();
        as->Asm("clc");
        as->Asm("sbc " + zp0);
    }

    if (m_node->m_params[5]->isPureNumericOrAddress())               // distance on Y
        as->Asm("cmp #" + distNegY );                        // -ve pure
    else
        as->Asm("cmp " + zp1 );                             // -ve complex

    as->Asm("bcs "+ lblCollision);                          // y vector 1 within distance, no more checks needed

    if (m_node->m_params[5]->isPureNumericOrAddress())               // distance on Y
        as->Asm("cmp "+ m_node->m_params[5]->getValue(as)); // +ve pure
    else
        as->Asm("cmp " + zp2 );                             // +ve complex

    as->Asm("bcs "+ lblNoCollision);                        // y vector 2 not within distance

    as->Label(lblCollision);                                // Collision, set a = 1
    as->Asm("lda #1");
    as->Asm("jmp " + lblCollisionDone);

    as->Label(lblNoCollision);                              // no collision, set a = 0
    as->Asm("lda #0");

    as->Label(lblCollisionDone);

    as->PopLabel("ColXConfirmed");
    as->PopLabel("NoCollision");
    as->PopLabel("Collision");
    as->PopLabel("CollisionDone");

}

void Methods6502::Atan2(Assembler *as)
{
      as->Comment("Call atan2");

      m_node->m_params[0]->Accept(m_codeGen);
      as->Term();
      as->Asm("sta atan2_x1");

      m_node->m_params[1]->Accept(m_codeGen);
      as->Term();
      as->Asm("sta atan2_x2");

      m_node->m_params[2]->Accept(m_codeGen);
      as->Term();
      as->Asm("sta atan2_y1");

      m_node->m_params[3]->Accept(m_codeGen);
      as->Term();
      as->Asm("sta atan2_y2");


//      as->Asm("pha");
//      m_node->m_params[1]->Build(as);
  //    as->Term();
      as->Asm(m_codeGen->getCallSubroutine()+" atan2_call");
}

void Methods6502::CopyCharsetFromRom(Assembler *as)
{
    m_node->RequireAddress(m_node->m_params[0],"CopyCharsetFromRom",m_node->m_op.m_lineNumber);
    as->Comment("Copy charset from ROM");
    as->Asm("sei ");
    QString lbl = as->NewLabel("charsetcopy");
    as->Asm("lda #$33 ;from rom - rom visible at d800");
    as->Asm("sta $01");

    as->Asm("ldy #$00");
    as->Label(lbl);
    for (int i=0;i<8;i++) {
        QString mp = Util::numToHex(i*100);
    as->Asm("lda $D000 + "+mp+",y");
    if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
        as->Term("sta (");
        m_node->m_params[0]->Accept(m_codeGen);
        as->Term("),y", true);

    }
    else {
        as->Term("sta ");
        m_node->m_params[0]->Accept(m_codeGen);
        as->Term("+"+mp+",y", true);
    }
    }
    //as->Asm("sta (zeropage1),y");
    as->Asm("dey");
    as->Asm("bne "+lbl);

    as->Asm("lda #$37");
    as->Asm("sta $01");
}

void Methods6502::IncMax(Assembler *as, QString cmd)
{
    IncDec(as,cmd);
    QString lbl = as->NewLabel("incmax");
    if (cmd=="inc") {
        LoadVar(as,0);
        //as->Asm("lda "+m_node->m_params[0]->getValue(as));
        as->Asm("cmp "+m_node->m_params[2]->getValue(as)+" ; keep");
        as->Asm("bne "+lbl);
        LoadVar(as,1);
        SaveVar(as,0);
        //as->Asm("sta "+m_node->m_params[0]->getValue(as));
        as->Label(lbl);
    }

   if (cmd=="dec") {
//        as->Asm("lda "+m_node->m_params[0]->getValue(as));
       LoadVar(as,0);

        as->Asm("cmp "+m_node->m_params[1]->getValue(as) + " ; keep");
        as->Asm("bne "+lbl);
        LoadVar(as,2);
        SaveVar(as,0);

//        as->Asm("sta "+m_node->m_params[0]->getValue(as));
        as->Label(lbl);
    }
    as->PopLabel("incmax");
}



void Methods6502::SetMemoryConfig(Assembler *as)
{
    as->Comment("Set Memory Config");

    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::C64 || Syntax::s.m_currentSystem->m_system == AbstractSystem::MEGA65) {


        m_node->RequireNumber(m_node->m_params[0], "SetMemoryConfig", m_node->m_op.m_lineNumber);
        m_node->RequireNumber(m_node->m_params[1], "SetMemoryConfig", m_node->m_op.m_lineNumber);
        m_node->RequireNumber(m_node->m_params[2], "SetMemoryConfig", m_node->m_op.m_lineNumber);


        int n1 = m_node->m_params[0]->getValueAsInt(as); // Kernal
        int n2 = m_node->m_params[1]->getValueAsInt(as); // Basic
        int n3 = m_node->m_params[2]->getValueAsInt(as); // IO

        if (n1==1 && n2==0 && n3 == 0)
            n3=1; // Bit 2 must be toggled

        uchar val = n1<<2 | n2<<1 | n3 << 0;

        as->Asm("lda $01");
        as->Asm("and #%11111000");
        as->Asm("ora #%" + QString::number(val,2));
        as->Asm("sta $01");

    }

    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::C128) {
        //as->Asm("lda #0");
        uchar v = 0;
        if (qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[0])->m_val!=0)
            v |= 0b00000001; // (0=IO, 1=RAM/ROM)
        if (qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[1])->m_val!=0)
            v |= 0b00000010; // Bank 1 RAM = 1, ROM =0))
        if (qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[2])->m_val!=0)
            v |= 0b00001100; // B000 RAM = 1, ROM =0))
        if (qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[3])->m_val!=0)
            v |= 0b00110000; // C000 RAM = 1, ROM =0))
        if (qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[4])->m_val!=0)
            v |= 0b01000000; // C000 RAM = 1, ROM =0))

        as->Asm("lda #"+Util::numToHex(v));
        as->Asm("sta $FF00");
    }
    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::PLUS4) {
        m_node->RequireNumber(m_node->m_params[0], "SetMemoryConfig", m_node->m_op.m_lineNumber);

        if (m_node->m_params[0]->getValueAsInt(as)!=0)
            as->Asm("sta $ff3f");
        else
            as->Asm("sta $ff3e");
    }
}

void Methods6502::EnableRasterIRQ(Assembler* as)
{
    as->Comment("Enable raster IRQ");
    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::PLUS4) {
        as->Asm("lda $ff09");
        as->Asm("ora #$02");
        as->Asm("sta $ff09");
    }
    else {
        as->Asm("lda $d01a");
        as->Asm("ora #$01");
        as->Asm("sta $d01a");
        as->Asm("lda #$1B");
        as->Asm("sta $d011");
/*        as->Asm("lda $d01a");
        as->Asm("ora #$01");
        as->Asm("sta $d01a");
        */
    }
}

void Methods6502::StartIRQ(Assembler *as)
{
    as->Comment("StartIRQ");

    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::NES || Syntax::s.m_currentSystem->m_system == AbstractSystem::OK64 || Syntax::s.m_currentSystem->m_system == AbstractSystem::BBCM) {
        as->Asm("pha");
        as->Asm("txa");
        as->Asm("pha");
        as->Asm("tya");
        as->Asm("pha");
    }

    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::C64 || Syntax::s.m_currentSystem->m_system == AbstractSystem::MEGA65) {
        m_node->RequireNumber(m_node->m_params[0], "StartIRQ", m_node->m_op.m_lineNumber);
        QSharedPointer<NodeNumber> n = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[0]);
        if (n->m_val==1) {
            as->Asm("asl $d019");
        }
        else {
            as->Asm("pha");
            as->Asm("txa");
            as->Asm("pha");
            as->Asm("tya");
            as->Asm("pha");
            as->Asm("asl $d019");
        }
    }
    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::PLUS4) {
        m_node->RequireNumber(m_node->m_params[0], "StartIRQ", m_node->m_op.m_lineNumber);
        QSharedPointer<NodeNumber> n = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[0]);
        if (n->m_val==1) {
            as->Asm("asl $ff09");
        }
        else {
            as->Asm("pha");
            as->Asm("txa");
            as->Asm("pha");
            as->Asm("tya");
            as->Asm("pha");
            as->Asm("asl $ff09");
        }
    }
    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::C128) {
        as->Asm("lda $d019");
        as->Asm("sta $d019");
        as->Asm("lda $FF00");
        as->Asm("pha");
    }
    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::VIC20) {
        as->Asm("pha");
        as->Asm("txa");
        as->Asm("pha");
        as->Asm("tya");
        as->Asm("pha");

    }

}

void Methods6502::StartIRQWedge(Assembler *as)
{
    QString lbl1 = as->NewLabel("startirqwedge_lbl1");
    QString lbl2 = as->NewLabel("startirqwedge_lbl2");

    QSharedPointer<NodeNumber> num = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[0]);

    as->Asm("txs");
    as->Asm("ldx #"+Util::numToHex(num->m_val));
    as->Label(lbl1);
    as->Asm("dex");
    as->Asm("bne "+lbl1);
    as->Asm("bit $ea");

    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::PLUS4) {
        as->Asm("lda $ff1d");
        as->Asm("cmp $ff1d");
    }
    else {
        as->Asm("lda $d012");
        as->Asm("cmp $d012");
    }

    as->Asm("beq "+lbl2);
    as->Label(lbl2);

    as->PopLabel("startirqwedge_lbl1");
    as->PopLabel("startirqwedge_lbl2");

}

void Methods6502::CloseIRQ(Assembler *as, bool isWedge)
{
    as->Comment("CloseIRQ");
    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::NES || Syntax::s.m_currentSystem->m_system == AbstractSystem::OK64 || Syntax::s.m_currentSystem->m_system == AbstractSystem::BBCM) {


        as->Asm("pla");
        as->Asm("tay");
        as->Asm("pla");
        as->Asm("tax");
        as->Asm("pla");
        if (Syntax::s.m_currentSystem->m_system == AbstractSystem::BBCM)
            as->Asm("lda $fc");

    }
    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::C64  || Syntax::s.m_currentSystem->m_system == AbstractSystem::MEGA65) {

        if (isWedge)
            as->Asm("asl $D019");

        as->Asm("pla");
        as->Asm("tay");
        as->Asm("pla");
        as->Asm("tax");
        as->Asm("pla");
    }
    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::PLUS4) {

        if (isWedge)
            as->Asm("asl $ff09");

        as->Asm("pla");
        as->Asm("tay");
        as->Asm("pla");
        as->Asm("tax");
        as->Asm("pla");
    }
    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::C128) {
        as->Asm("pla");
        as->Asm("sta $FF00");

        as->Asm("lda #$00");
        as->Asm("sta $d030");
        as->Asm("jmp $fa65");

    }
    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::VIC20) {
        as->Asm("pla");
        as->Asm("tay");
        as->Asm("pla");
        as->Asm("tax");
        as->Asm("pla");
        as->Asm(" jmp $eabf     ; return to normal IRQ	");

    }
}

void Methods6502::DisableNMI(Assembler *as)
{
    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::C64 ) {

        as->Comment("Hook NMI");

        QSharedPointer<NodeProcedure> addr = qSharedPointerDynamicCast<NodeProcedure>(m_node->m_params[0]);
        if (addr==nullptr)
            ErrorHandler::e.Error("First parameter must be interrupt procedure!", m_node->m_op.m_lineNumber);

        QString name = addr->m_procedure->m_procName;

        as->Asm("sei");
        as->Asm("lda     #<"+name);
        as->Asm("sta     $0318");
        as->Asm("lda     #>"+name);
        as->Asm("sta     $0319");

        as->Asm("lda     #$00            ; Stop time A CIA2");
        as->Asm("sta     $dd0e");

        as->Asm("lda     #$00");
        as->Asm("sta     $dd04           ; Set timer value #1 (Timer A CIA 2)");
        as->Asm("lda     #$00");
        as->Asm("sta     $dd05           ; Set timer value #2 (Timer A CIA 2)");

        as->Asm("lda     #%10000001      ; Fill bit to 1 and enable NMI to occur from Timer A");
        as->Asm("sta     $dd0d");

        as->Asm("lda     #$01");
        as->Asm("sta     $dd0e           ; Start timer A CIA (NMI will occur immediately)(*)");

        as->Asm("cli");

    }

}

void Methods6502::KeyPressed(Assembler *as)
{
    as->Comment("Keypressed");

    QSharedPointer<NodeNumber> num = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[0]);
    if (num==nullptr)
        ErrorHandler::e.Error("KeyPressed requires key to be numeric! KEY_A etc");


    if (!Syntax::s.m_c64keys.contains(num->m_val))
        ErrorHandler::e.Error("KeyPressed: does not recognize character " + QString::number(num->m_val));

    C64Key key = Syntax::s.m_c64keys[num->m_val];

    QString lbl1 = as->NewLabel("keypressedA");
    QString lbl2 = as->NewLabel("keypressedB");

    as->PopLabel("keypressedA");
    as->PopLabel("keypressedB");


    as->Asm("lda #$FF  ; CIA#1 port A = outputs ");
    as->Asm("sta $dc02             ");

    as->Asm("lda #%00000000  ; CIA#1 port B = inputs");
    as->Asm("sta $dc03             ");

    as->Asm("lda #%" + QString::number(key.m_row,2));
    as->Asm("sta  $dc00");

    as->Asm("lda $dc01");
    as->Asm("and #%"+QString::number((uchar)key.m_column,2));
    as->Asm("cmp #%"+QString::number((uchar)key.m_column,2));
    as->Asm("bne "+lbl1);
    as->Asm("lda #0");
    as->Asm("jmp "+lbl2);
as->Label(lbl1);
    as->Asm("lda #1");
as->Label(lbl2);
    //sta key_ispressed

}

void Methods6502::Wait(Assembler *as)
{
    as->Comment("Wait");
    LoadVar(as,0);
    as->Asm("tax");

    as->Asm("dex");
    as->Asm("bne *-1");

}

QString Methods6502::BitShiftX(Assembler *as)
{
    QString lblshiftbit = as->NewLabel("shiftbit");
    QString lblshiftbitDone = as->NewLabel("shiftbitdone");
    as->Asm("lda #1");
    as->Label(lblshiftbit);
    as->Asm("cpx #0");
    as->Asm("beq " + lblshiftbitDone);
    as->Asm("asl");
    as->Asm("dex");
    as->Asm("jmp "+lblshiftbit);
    as->Label(lblshiftbitDone);

    as->PopLabel("shiftbit");
    as->PopLabel("shiftbitdone");
    return as->StoreInTempVar("bitmask");

}

void Methods6502::LoadAddress(Assembler *as, int paramNo)
{
    QSharedPointer<Node> node = m_node->m_params[paramNo];

    if (node->isPureNumericOrAddress()) {
        as->Asm("lda #" + Util::numToHex(node->getValueAsInt(as)&0xff));
        as->Asm("ldy #" + Util::numToHex((node->getValueAsInt(as)>>8)&0xff));
        return;
    }
    if (node->isPointer(as) && (!node->hasArrayIndex())) {
        as->Asm("lda "+node->getValue(as));
        as->Asm("ldy "+node->getValue(as)+"+1");
        return;
    }
    as->Asm("lda #<"+node->getValue(as));
    as->Asm("ldy #>"+node->getValue(as));
}




void Methods6502::InitSinusTable(Assembler *as)
{
    if (m_node->m_isInitialized["sinetab"])
        return;
//    as->Asm("jmp initsin_continue");
    if (Syntax::s.m_currentSystem->CL65Syntax()) {
        as->Write("sine: .res 256,0");

    }
    else {
        as->Label("sine .byte 0 ");
        as->Asm("org sine +#255");
    }
    as->Write("value: .word 0");
    as->Write("delta: .word 0");
    as->Label("initsine_calculate");


    as->Asm("ldy #$3f");
    as->Asm("ldx #$00");
    as->Label("initsin_a");
    as->Asm("lda value");
    as->Asm("clc");
    as->Asm("adc delta");
    as->Asm("sta value");
    as->Asm("lda value+1");
    as->Asm("adc delta+1");
    as->Asm("sta value+1");

    as->Asm("sta sine+$c0,x");
    as->Asm("sta sine+$80,y");
    as->Asm("eor #$ff");
    as->Asm("sta sine+$40,x");
    as->Asm("sta sine+$00,y");

    as->Asm("lda delta");
//    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::OK64)
  //  as->Asm("adc #$14   ; this value adds up to the proper amplitude");
   // else {
        as->Asm("adc #$10   ; this value adds up to the proper amplitude");

   // }
    as->Asm("sta delta");
    as->Asm("bcc initsin_b");
    as->Asm("inc delta+1");
    as->Label("initsin_b");

    as->Asm("inx");
    as->Asm("dey");
    as->Asm("bpl initsin_a");
    //as->Asm("rts");


    m_node->m_isInitialized["sinetab"]=true;


}


void Methods6502::IncZp(Assembler *as)
{
    as->m_labelStack["incscreenx"].push();
    QString lbl = as->getLabel("incscreenx");

    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr) {
        ErrorHandler::e.Error("IncZp: Left-hand parameter must be zeropage pointer");
    }
    if (var->getType(as)!=TokenType::POINTER) {
        ErrorHandler::e.Error("IncZp: Left-hand parameter must be zeropage pointer");
    }


/*    as->Term("lda ");
    m_node->m_params[1]->Build(as);
    as->Term();
    as->Asm("sta screen_x");
    as->Asm("lda "+ var->getValue(as));
    as->Asm("clc");
    as->Asm("adc screen_x");
    as->Asm("bcc " + lbl);
    as->Asm("inc "+ var->getValue(as) + " +1");
    as->Label(lbl);
    as->Asm("sta "+ var->getValue(as));
*/

    m_node->m_params[1]->Accept(m_codeGen);
    as->Term();
    as->Asm("clc");
    as->Asm("adc "+ var->getValue(as));
    as->Asm("bcc " + lbl);
    as->Asm("inc "+ var->getValue(as) + " +1");
    as->Label(lbl);
    as->Asm("sta "+ var->getValue(as));


    as->m_labelStack["incscreenx"].pop();

}


void Methods6502::DecZp(Assembler *as)
{
    as->m_labelStack["deczp"].push();
    QString lbl = as->getLabel("deczp");

    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr) {
        ErrorHandler::e.Error("DecZp: Left-hand parameter must be zeropage pointer");
    }
    if (var->getType(as)!=TokenType::POINTER) {
        ErrorHandler::e.Error("DecZp: Left-hand parameter must be zeropage pointer");
    }
    as->Comment("Decrease zeropage pointer");
    as->Asm("lda " + var->getValue(as));
    as->Asm("clc");
//    as->Asm("sec");
    as->Term("sbc ");
    m_node->m_params[1]->Accept(m_codeGen);
    as->Term();

    as->Asm("bcc " + lbl);
    as->Asm("dec "+ var->getValue(as) + " +1");
    as->Label(lbl);
    as->Asm("sta "+ var->getValue(as));



    as->m_labelStack["deczp"].pop();

}



void Methods6502::IncScreenX(Assembler *as)
{
    ErrorHandler::e.Error("incscreenx is deprecated. Please use inczp(screenmemory, val) instead", m_node->m_op.m_lineNumber);
    as->m_labelStack["incscreenx"].push();
    QString lbl = as->getLabel("incscreenx");

    as->Term("lda ");
    m_node->m_params[0]->Accept(m_codeGen);
    as->Term();
    as->Asm("sta screen_x");
    as->Asm("lda screenmemory");
    //as->Asm("cpx #0");
   // as->Asm("beq " + lbl);
    as->Asm("clc");
    as->Asm("adc screen_x");
    as->Asm("bcc " + lbl);
    as->Asm("inc screenmemory+1");
    as->Label(lbl);
    as->Asm("sta screenmemory");


    as->m_labelStack["incscreenx"].pop();

}



void Methods6502::Call(Assembler *as)
{
    //QSharedPointer<NodeNumber> num= (QSharedPointer<NodeNumber>)qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[0]);
    if (m_node->m_params[0]->isPointer(as)) {
        //QString zp0 = as->m_internalZP[0];
        //QString zp1 = as->m_internalZP[1];
        QString lblF = as->NewLabel("foo");
        QString lblB = as->NewLabel("bar");

/*        as->Asm("jsr *+3");
        as->Asm("pla");
        as->Asm("sta "+zp0+"; lo");
        as->Asm("pla");
        as->Asm("sta "+zp1+" ; hi");


        as->Asm("lda "+zp0+" ; lo");
        as->Asm("clc");
        as->Asm("adc #26 ; lo");
        as->Asm("sta "+zp0+"");
        as->Asm("bcc "+lbl);
        as->Asm("inc "+zp1+" ; inc hi");
        as->Label(lbl);
        as->Asm("lda "+zp1+" ; load hi");
        as->Asm("pha      ");
        as->Asm("lda "+zp0+" ; load lo");
        as->Asm("pha");
*/
        as->Asm("jsr "+lblF);
        as->Asm("jmp "+lblB);
        as->Label(lblF);
        as->Asm("jmp ("+m_node->m_params[0]->getValue(as)+")");
        as->Label(lblB);

        as->PopLabel("foo");
        as->PopLabel("bar");
        return;
    }


    as->Term("jsr ");
    m_node->m_params[0]->Accept(m_codeGen);
    as->Term();

  /*  if (num!=nullptr) {
        as->Term("jsr ");
        num->Build(as);
        as->Term();
        return;
    }*/
//    ErrorHandler::e.Error("Call currently only supports constant values", m_node->m_op.m_lineNumber);
/*    QSharedPointer<NodeVar> num= qSharedPointer<NodeNumber>>(m_node->m_params[0]);
    if (num!=nullptr) {
        as->Asm("jsr $" + QString::number(num->m_val,16));
        return;
    }
*/
}

void Methods6502::ToggleRegisterBit(Assembler *as, QString addr, int bit, bool regular)
{
    QSharedPointer<NodeNumber> num = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[0]);
    if (num==nullptr) {
           ErrorHandler::e.Error("Call currently only supports constant 0/1 (on/off)", m_node->m_op.m_lineNumber);

    }
    int val = num->m_val;
    uchar v=1<<bit;
    as->Asm("lda "+addr);

    int a = 1;
    int b = 0;

    if (!regular) { b=1; a=0; }
    if (val==a)
        as->Asm("ora #%"+QString::number(v,2));
    if (val==b)
        as->Asm("and #%"+QString::number((uchar)~v,2));
    as->Asm("sta "+addr);

}

void Methods6502::InitSid(Assembler *as)
{
    QSharedPointer<NodeNumber> num= (QSharedPointer<NodeNumber>)qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[0]);
//    qDebug() << "initsid:" << num->HexValue();
    if (num!=nullptr) {
        as->Comment("initsid");
        as->Asm("lda #0");
        as->Asm("tax");
        as->Asm("tay");
        as->Term("jsr ");
        num->Accept(m_codeGen);
        as->Term();

        if (Syntax::s.m_currentSystem->m_system == AbstractSystem::VIC20) {
            as->Asm("lda	$900e");
            as->Asm("ora	#$0f");
            as->Asm("sta	$900e");
//            as->Asm("rts");
        }



        return;
    }
    ErrorHandler::e.Error("InitSid currently only supports constant values", m_node->m_op.m_lineNumber);

}


/*void Methods6502::InitDiv8x8(Assembler* as) {
    as->Asm("jmp div8x8_def_end");
    as->Label("div8x8_procedure_defs");
    as->Label("div8x8_c .byte 0 ");
    as->Label("div8x8_d .byte 0 ");
    as->Label("div8x8_e .byte 0 ");
    as->Comment("Normal 8x8 bin div");
    as->Label("div8x8_procedure");
    as->Asm("ASL div8x8_d");
    as->Asm("LDA #$00");
    as->Asm("ROL");
    as->Asm("LDX #$08");
    as->Label("div8x8loop1");
    as->Asm("CMP div8x8_c");
    as->Asm("BCC *+4");
    as->Asm("SBC div8x8_c");
    as->Asm("ROL div8x8_d");
    as->Asm("ROL");
    as->Asm("DEX");
    as->Asm("BNE div8x8loop1");
    as->Asm("LDX #$08");
    as->Label("div8x8loop2");
    as->Asm("CMP div8x8_c");
    as->Asm("BCC *+4");
    as->Asm("SBC div8x8_c");
    as->Asm("ROL div8x8_e");
    as->Asm("ASL");
    as->Asm("DEX");
    as->Asm("BNE div8x8loop2");
    as->Asm("lda div8x8_d");
    as->Asm("rts");
    as->Label("div8x8_def_end");
}
*/

void Methods6502::InitDiv8x8(Assembler* as) {
//    as->Asm("jmp div8x8_def_end");
  //  as->Label("div8x8_procedure_defs");

/*    as->Label("div8x8_c .byte 0 ");
    as->Label("div8x8_d .byte 0 ");
    as->Label("div8x8_e .byte 0 ");
*/
    as->Label("div8x8_c = " + as->m_internalZP[0]);
    as->Label("div8x8_d = "+ as->m_internalZP[1]);
    as->Label("div8x8_e = "+ as->m_internalZP[2]);

    as->Comment("Normal 8x8 bin div");
    as->Label("div8x8_procedure");

    as->Asm("lda #$00");
    as->Asm("ldx #$07");
    as->Asm("clc");
   as->Label("div8x8_loop1");
   as->Asm("rol div8x8_d");
     as->Asm("rol");
     as->Asm("cmp div8x8_c");
     as->Asm("bcc div8x8_loop2");
      as->Asm("sbc div8x8_c");
   as->Label("div8x8_loop2");
   as->Asm("dex");
    as->Asm("bpl div8x8_loop1");
    as->Asm("rol div8x8_d");

    as->Asm("lda div8x8_d");
    //as->Asm("rts");


    as->Label("div8x8_def_end");

}



void Methods6502::InitDiv16x8(Assembler *as)
{
/*    qDebug() << as->m_internalZP.count();
    exit(1);
    if (as->m_internalZP.count()<4)
        return;*/
//    as->Asm("jmp div16x8_def_end");
    if (m_initDiv16x8_initialised)
        return;

    as->Label("initdiv16x8_divisor = "+as->m_internalZP[0]+"     ;$59 used for hi-byte");   //0
    as->Label("initdiv16x8_dividend = "+as->m_internalZP[1]+"	  ;$fc used for hi-byte");    //1
    as->Label("initdiv16x8_remainder = "+as->m_internalZP[2]+"	  ;$fe used for hi-byte");  // 2
    as->Label("initdiv16x8_result = "+as->m_internalZP[1]+" ;save memory by reusing divident to store the result");

    as->Label("divide16x8");
    as->Asm(m_codeGen->getInitProcedure());
    as->Asm("lda #0	        ;preset remainder to 0");
    as->Asm("sta initdiv16x8_remainder");
    as->Asm("sta initdiv16x8_remainder+1");
    as->Asm("ldx #16	        ;repeat for each bit: ...");

    as->Write("divloop16:	asl initdiv16x8_dividend	;dividend lb & hb*2, msb -> Carry");
    as->Asm("rol initdiv16x8_dividend+1");
    as->Asm("rol initdiv16x8_remainder	;remainder lb & hb * 2 + msb from carry");
    as->Asm("rol initdiv16x8_remainder+1");
    as->Asm("lda initdiv16x8_remainder");
    as->Asm("sec");
    as->Asm("sbc initdiv16x8_divisor	;substract divisor to see if it fits in");
    as->Asm("tay	        ;lb result -> Y, for we may need it later");
    as->Asm("lda initdiv16x8_remainder+1");
    as->Asm("sbc initdiv16x8_divisor+1");
    as->Asm("bcc skip16	;if carry=0 then divisor didn't fit in yet");

    as->Asm("sta initdiv16x8_remainder+1	;else save substraction result as new remainder,");
    as->Asm("sty initdiv16x8_remainder");
    as->Asm("inc initdiv16x8_result	;and INCrement result cause divisor fit in 1 times");
    as->Label("skip16");
    as->Asm("dex");
    as->Asm("bne divloop16");
//    m_initDiv16x8_initialised = true;

//    as->Label("div16x8_def_end");
}

void Methods6502::InitSqrt16(Assembler *as)
{
    if (as->m_internalZP.count()<4)
        return;
    as->Label("sqrt16_init");
    as->Asm("ldy #$01 ");


    as->Asm("sty " + as->m_internalZP[2]);
    as->Asm("dey");
    as->Asm("sty "+as->m_internalZP[3]+" ; msby of first odd number (sqrt = 0)");
    as->Label("sqrt16_again");
    as->Asm("sec");
    as->Asm("lda "+as->m_internalZP[0]+" ; save remainder in X register");
    as->Asm("tax ; subtract odd lo from integer lo");
    as->Asm("sbc "+as->m_internalZP[2]+"");
    as->Asm("sta "+as->m_internalZP[0]+"");
    as->Asm("lda "+as->m_internalZP[1]+" ; subtract odd hi from integer hi");
    as->Asm("sbc "+as->m_internalZP[3]+"");
    as->Asm("sta "+as->m_internalZP[1]+" ; is subtract result negative?");
    as->Asm("bcc sqrt16_nomore ; no. increment square root");
    as->Asm("iny");
    as->Asm("lda "+as->m_internalZP[2]+" ; calculate next odd number");
    as->Asm("adc #$01");
    as->Asm("sta "+as->m_internalZP[2]+"");
    as->Asm("bcc sqrt16_again");
    as->Asm("inc "+as->m_internalZP[3]+"");
    as->Asm("jmp sqrt16_again");
    as->Label("sqrt16_nomore");
    as->Asm("sty "+as->m_internalZP[0]+" ; all done, store square root");
    as->Asm("stx "+as->m_internalZP[1]+" ; and remainder");
    //as->Asm("rts");
}

void Methods6502::InitAtan2(Assembler *as)
{
    as->IncludeFile(":resources/code/atan2.asm");

}

void Methods6502::InitModPlayer(Assembler *as)
{

    // First set up:
    m_node->m_params[0]->Accept(m_codeGen);
    as->Term();
    as->Asm("sta reset_pattern_pointer+5");
    as->Asm("sta reset_pattern_pointer_two+5");
    m_node->m_params[1]->Accept(m_codeGen);
    as->Term();
    as->Asm("sta reset_pattern_pointer+1");
    as->Asm("sta reset_pattern_pointer_two+1");
/*
    m_node->m_params[1]->Accept(m_codeGen);
    lda #$00
    sta reset_pattern_pointer_two+1
    lda #$a0
    sta reset_pattern_pointer_two+5

*/

    as->Asm("jsr		setup_music");
    as->Asm("jsr		skip_order_wrap ; enter in the middle of the music loop");

    as->Asm("stx mod_save_x");
    as->Asm("sta mod_save_a");


}

void Methods6502::IncludeModPlayer(Assembler *as)
{
    as->IncludeFile(":resources/code/vic20_mod_music.asm");

}



void Methods6502::InitMul16x8(Assembler *as)
{
//     Multiplies "num1" by "num2" and stores result in .A (low byte, also in .X) and .Y (high byte)
//    as->Asm("jmp mul16x8_def_end");
  //  as->Label("mul16x8_procedure_defs");

/*    as->Label("mul16x8_num1Hi .byte 0 ");
    as->Label("mul16x8_num1 .byte 0 ");
    as->Label("mul16x8_num2 .byte 0 ");*/
    as->Label("mul16x8_num1Hi = " + as->m_internalZP[0]);
    as->Label("mul16x8_num1 = " + as->m_internalZP[1]);
    as->Label("mul16x8_num2 = " + as->m_internalZP[2]);

    as->Label("mul16x8_procedure");
    as->Asm(m_codeGen->getInitProcedure());;
    m_codeGen->Disable16bit();
    as->Asm("lda #$00");
    as->Asm("ldy #$00");
//    as->Asm("tay");
  //  as->Asm("sty mul16x8_num1Hi  ; remove this line for 16*8=16bit multiply");
    as->Asm("beq mul16x8_enterLoop");

    as->Label("mul16x8_doAdd");
    as->Asm("clc");
    as->Asm("adc mul16x8_num1");
    as->Asm("tax");

    as->Asm("tya");
    as->Asm("adc mul16x8_num1Hi");
    as->Asm("tay");
    as->Asm("txa");

    as->Label("mul16x8_loop");
    as->Asm("asl mul16x8_num1");
    as->Asm("rol mul16x8_num1Hi");
    as->Label("mul16x8_enterLoop");
    as->Asm("lsr mul16x8_num2");
    as->Asm("bcs mul16x8_doAdd");
    as->Asm("bne mul16x8_loop");
//    as->Asm(m_codeGen->ProcedureEndWithoutReturn());
    m_codeGen->Enable16bit();
  //  as->Asm("rts");

//    as->Label("mul16x8_def_end");
}

void Methods6502::DisableInterrupts(Assembler *as)
{
    as->Comment("Disable interrupts");
//    as->Asm("sei ");
/*    as->Asm("ldy #$7f    ; $7f = %01111111");
    as->Asm("sta $dc0d");
    as->Asm("sta $dd0d");
    as->Asm("lda #$01");
    as->Asm("sta $d01a");
*/

    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::C64 || Syntax::s.m_currentSystem->m_system == AbstractSystem::MEGA65) {
        as->Asm("ldy #$7f    ; $7f = %01111111");
        as->Asm("sty $dc0d   ; Turn off CIAs Timer interrupts");
        as->Asm("sty $dd0d   ; Turn off CIAs Timer interrupts");
        as->Asm("lda $dc0d   ; cancel all CIA-IRQs in queue/unprocessed");
        as->Asm("lda $dd0d   ; cancel all CIA-IRQs in queue/unprocessed");
    }
    else if (Syntax::s.m_currentSystem->m_system == AbstractSystem::PLUS4) {
        as->Asm("lda #$00");
        as->Asm("sta $ff09");
    }


    as->Asm("");
}

/*void Methods6502::RasterIRQ(Assembler *as)
{
    QSharedPointer<NodeProcedure> addr = (QSharedPointer<NodeProcedure>)dynamic_cast<QSharedPointer<NodeProcedure>>(m_node->m_params[0]);
    if (addr==nullptr)
        ErrorHandler::e.Error("First parameter must be interrupt procedure!", m_node->m_op.m_lineNumber);
    QString name = addr->m_procedure->m_procName;
    as->Comment("Set raster interrupt pointing to : " +name);
    as->Asm("lda #$01    ; Set Interrupt Request Mask...");
    as->Asm("sta $d01a   ; ...we want IRQ by Rasterbeam");
    as->Asm("lda #<" + name);
    as->Asm("ldx #>"+ name);
    as->Asm("sta $314    ; store in $314/$315");
    as->Asm("stx $315");
    LoadVar(as,1);
    as->Asm("sta $d012");
}
*/
void Methods6502::RasterIRQ(Assembler *as)
{
    QSharedPointer<NodeProcedure> addr = qSharedPointerDynamicCast<NodeProcedure>(m_node->m_params[0]);
//    if (addr==nullptr)
  //      ErrorHandler::e.Error("First parameter must be interrupt procedure!", m_node->m_op.m_lineNumber);




    QString name = addr->m_procedure->m_procName;

    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::BBCM) {
        as->Comment("RasterIRQ");
        as->Asm("lda #<"+name);
        as->Asm("sta $204");
        as->Asm("lda #>"+name);
        as->Asm("sta $205");
        return;
    }


    m_node->RequireNumber(m_node->m_params[2], "RasterIRQ", m_node->m_op.m_lineNumber);
    QSharedPointer<NodeNumber> num = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[2]);

    as->Comment("RasterIRQ : Hook a procedure");

    as->ClearTerm();
    m_node->m_params[1]->Accept(m_codeGen);
    as->Term();

    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::PLUS4)
        as->Asm("sta $ff0b");
    else
        as->Asm("sta $d012");

    if (num->m_val==0) {
//        as->Asm("lda #$(adresstosignalIRQ)
        as->Asm("lda #<"+name);
        as->Asm("sta $fffe");
        as->Asm("lda #>"+name);
        as->Asm("sta $ffff");
    }
    else
    {
        as->Asm("lda #<"+name);
        as->Asm("sta $0314");
        as->Asm("lda #>"+name);
        as->Asm("sta $0315");
    }
}



void Methods6502::RasterIRQWedge(Assembler *as)
{
    QSharedPointer<NodeProcedure> addr = qSharedPointerDynamicCast<NodeProcedure>(m_node->m_params[0]);
    QString name = addr->m_procedure->m_procName;

    QSharedPointer<NodeNumber> num = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[2]);

    as->Comment("RasterIRQ Wedge: Hook a wedge");

    as->ClearTerm();
    m_node->m_params[1]->Accept(m_codeGen);
    as->Term();

    if (num->m_val==0) {
//        as->Asm("lda #$(adresstosignalIRQ)
        as->Asm("sta $d012");

        QString lbl1 = as->NewLabel("rasterirqwedgelbl1");
        QString lbl2 = as->NewLabel("rasterirqwedgelbl2");

        //  0000 0011
        //  0000 1011
        as->Asm("and #%00001011");
        as->Asm("cmp #%0011");
        as->Asm("beq " + lbl1);
        as->Asm("cmp #%1011");
        as->Asm("beq " + lbl1);
        as->Asm("adc #1");
        as->Asm("cmp #%0011");
        as->Asm("beq " + lbl1);
        as->Asm("cmp #%1011");
        as->Asm("beq " + lbl1);

        as->Asm("lda #<"+name);
        as->Asm("sta IRQWedgeStartRegular+18");
        as->Asm("lda #>"+name);
        as->Asm("sta IRQWedgeStartRegular+23");

        as->Asm("lda #<IRQWedgeStartRegular");
        as->Asm("sta $fffe");
        as->Asm("lda #>IRQWedgeStartRegular");
        as->Asm("sta $ffff");
        as->Asm("jmp "+lbl2);

        as->Label(lbl1); // BAD BAD BAAAAD

        as->Asm("lda #<"+name);
        as->Asm("sta IRQWedgeStartBad+18");
        as->Asm("lda #>"+name);
        as->Asm("sta IRQWedgeStartBad+23");

        as->Asm("lda #<IRQWedgeStartBad");
        as->Asm("sta $fffe");
        as->Asm("lda #>IRQWedgeStartBad");
        as->Asm("sta $ffff");
//        as->Asm("jmp "+lbl2);


        as->Label(lbl2);

        as->PopLabel("rasterirqwedgelbl1");
        as->PopLabel("rasterirqwedgelbl2");
    }
    else
    {
            ErrorHandler::e.Error("Error! Kernal wedge not implemented. Nag the developer (leuat).", m_node->m_currentLineNumber);
/*        as->Asm("sta $d012");
        as->Asm("lda #<"+name);
        as->Asm("sta $0314");
        as->Asm("lda #>"+name);
        as->Asm("sta $0315");
*/
    }

}


void Methods6502::ClearScreen(Assembler *as)
{
    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::OK64)
        return;
    int val = m_node->m_params[1]->numValue();  // start address (offset) to fill
    if (m_node->m_params[1]->isPointer(as)) {
/*        if (Syntax::s.m_currentSystem->m_system==AbstractSystem::C64) {



            return;
        }*/
        ErrorHandler::e.Error("ClearScreen does currently not support pointers.",m_node->m_op.m_lineNumber);
    }

    if (!m_node->m_params[1]->isPure())
        ErrorHandler::e.Error("ClearScreen address must be pure numeric or a variable, and not an expression.",m_node->m_op.m_lineNumber);

    AddMemoryBlock(as,1);
    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::PLUS4 ||
            Syntax::s.m_currentSystem->m_system==AbstractSystem::C128 ||
            Syntax::s.m_currentSystem->m_system==AbstractSystem::C64 ||
            Syntax::s.m_currentSystem->m_system==AbstractSystem::APPLEII ||
            Syntax::s.m_currentSystem->m_system==AbstractSystem::NES ||
            Syntax::s.m_currentSystem->m_system==AbstractSystem::MEGA65 ||
            Syntax::s.m_currentSystem->m_system==AbstractSystem::PET) {

        QString lbl = as->NewLabel("clearloop");
        QString shift = Util::numToHex(val);
        as->Comment("Clear screen with offset");
        LoadVar(as, 0);
        as->Asm("ldx #$fa");
        as->Label(lbl);
        as->Asm("dex");
        as->Asm("sta $0000+"+shift+",x");
        as->Asm("sta $00fa+"+shift+",x");
        as->Asm("sta $01f4+"+shift+",x");
        as->Asm("sta $02ee+"+shift+",x");
        if (Syntax::s.m_currentSystem->m_system==AbstractSystem::PET || Syntax::s.m_currentSystem->m_system==AbstractSystem::MEGA65) {
            as->Asm("sta $03e8+"+shift+",x");
            as->Asm("sta $04e2+"+shift+",x");
            as->Asm("sta $05dc+"+shift+",x");
            as->Asm("sta $06d6+"+shift+",x");

        }
        as->Asm("bne "+lbl);

        as->PopLabel("clearloop");

    }
    else if (Syntax::s.m_currentSystem->m_system==AbstractSystem::VIC20) {

        QString lbl = as->NewLabel("clearloop");
        QString shift = Util::numToHex(val);
        as->Comment("Clear screen with offset");
        LoadVar(as, 0);
        as->Asm("ldx #$fe");
        as->Label(lbl);
        as->Asm("dex");
        as->Asm("sta $0000+"+shift+",x");
        as->Asm("sta $00fd+"+shift+",x");
        as->Asm("bne "+lbl);

        as->PopLabel("clearloop");

    }

}


void Methods6502::WaitNoRasterLines(Assembler *as)
{
   /* QString var = "";
    QString lbl = as->NewLabel("waitnoraster");
    QSharedPointer<NodeNumber> num = (QSharedPointer<NodeNumber>)qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[0]);
    if (num!=nullptr)
            var = "#" + num->HexValue();
    QSharedPointer<NodeVar> nvar = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (nvar!=nullptr)
            var = nvar->getValue(as);
    if (var=="")
        ErrorHandler::e.Error("WaitNoRasterLines: parameter must be either constant or variable, not expression", m_node->m_op.m_lineNumber);
*/
    as->Comment("Wait for no of raster lines");
  //  as->Label(lbl);
    LoadVar(as, 0);

    QString cmp ="$d012";
    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::VIC20)
        cmp="$9004";
    else if (Syntax::s.m_currentSystem->m_system==AbstractSystem::PLUS4)
        cmp="$ff1d";




    as->Asm("clc ");
    as->Asm("adc "+cmp);
    //as->Asm("adc " + var);
    as->Asm("cmp "+cmp);
    as->Asm("bne *-3" );


//    as->PopLabel("waitnoraster");
}




void Methods6502::WaitForRaster(Assembler *as)
{
    as->Comment("wait for raster");
//    LoadVar(as, 0,"", "ldx ");

/*    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::C64 || Syntax::s.m_currentSystem->m_system==AbstractSystem::C128 || Syntax::s.m_currentSystem->m_system==AbstractSystem::MEGA65) {
 //       m_node->m_params[0]->setForceType(TokenType::BYTE);
        if (m_node->m_params[0]->isPureNumericOrAddress()) {
            int val =  m_node->m_params[0]->getValueAsInt(as);
             if (val<=255) {
                as->Asm("bit $D011");
                as->Asm("bpl *-3");
                as->Asm("bit $D011");
                as->Asm("bmi *-3");
                as->Asm("ldx "+Util::numToHex(val));
                as->Asm("tax");
                as->Asm("cpx $d012");
                return;
            }
            else
            if (val<=255) {
                as->Asm("bit $D011");
                as->Asm("bmi *-3");
                as->Asm("bit $D011");
                as->Asm("bpl *-3");
                as->Asm("ldx "+Util::numToHex(val&0xFF));
                as->Asm("tax");
                as->Asm("cpx $d012");
                return;
            }
        }

    }
*/
    LoadVar(as,0);
    as->Asm("tax");
//    as->Asm("lda $d012 ; raster line pos");
//    as->Asm("clc ; clear carry ");
 //   as->Label("lblb");
    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::C64 || Syntax::s.m_currentSystem->m_system==AbstractSystem::C128 || Syntax::s.m_currentSystem->m_system==AbstractSystem::MEGA65)
        as->Asm("cpx $d012");
    else if (Syntax::s.m_currentSystem->m_system==AbstractSystem::VIC20)
        as->Asm("cpx $9004");
    else if (Syntax::s.m_currentSystem->m_system==AbstractSystem::PLUS4)
        as->Asm("cpx $ff1d");

    as->Asm("bne *-3");

}

void Methods6502::SetSpriteLoc(Assembler *as)
{

  /*  QSharedPointer<NodeNumber> num2 = (QSharedPointer<NodeNumber>)qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[0]);
    if (num2==nullptr)
        ErrorHandler::e.Error("SetSpriteLoc parameter 0 must be constant");
    QSharedPointer<NodeNumber> num = (QSharedPointer<NodeNumber>)qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[1]);
    if (num==nullptr)
        ErrorHandler::e.Error("SetSpriteLoc parameter 1 must be constant");
*/

    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::X16) {
        as->Comment("Set VERA sprite location");
        LoadVar(as,0);
        as->Asm("sta $9F23");
        as->Asm("inc $9F20");
        as->Asm("sty $9F23");
        as->Asm("dec $9F20");

        return;
    }


    QSharedPointer<NodeNumber> num3 = (QSharedPointer<NodeNumber>)qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[2]);
    if (num3==nullptr)
        ErrorHandler::e.Error("SetSpriteLoc parameter 2 (bank) must be constant 0-3");

    QString bank=  "$"+QString::number((int)(num3->m_val*0x4000),16);


    as->Comment("Set sprite location");

    if (m_node->m_params[1]->isPure()) {
        LoadVar(as,0);
        as->Asm("tax");
        LoadVar(as,1);
    } else {
/*        if (m_node->m_params[0]->isPureNumericOrAddress())
            m_node->m_params[0]->setForceType(TokenType::BYTE);*/
        LoadVar(as,0);
        QString zp = as->m_internalZP[2];
        as->Asm("sta "+zp);
        LoadVar(as,1);
        as->Asm("ldx "+zp);
    }

//    SaveVar(as,0,"x");
    as->Asm("sta $07f8 + "+bank+",x");

/*    int newLoc = 64*num->m_val;
    QString c = "SPRITE_LOC" +QString::number((int)num2->m_val+1);
//    qDebug() << "sprite num : " << ("$" + QString::number((int)(newLoc),16)) << " with value " << c;
    as->m_symTab->m_constants[c]->m_value->m_fVal = newLoc;
    as->m_symTab->m_constants[c]->m_value->m_strVal = "$" + QString::number((int)(newLoc),16);
*/
}

void Methods6502::ClearBitmap(Assembler *as)
{

    if (!m_node->m_params[0]->isPureNumericOrAddress() || !m_node->m_params[1]->isPureNumericOrAddress())
        ErrorHandler::e.Error("ClearBitmap: both parameters must be integer constants");

    QString lbl = as->NewLabel("clear");


    QString screen = m_node->m_params[0]->getValue(as);


    as->Asm("; Clear bitmap method");

    as->Asm("ldy #0");
    as->Asm("tya");
    as->Label(lbl);
    for (int i=0;i<m_node->m_params[1]->getValueAsInt(as);i++)
        as->Asm("sta "+screen+"+$" + QString::number(i*256,16) + " ,y");
    as->Asm("dey");
    as->Asm("bne "+lbl);

}

void Methods6502::Swap(Assembler *as)
{
    QSharedPointer<NodeVar> vars[2];
    for (int i=0;i<2;i++) {
        QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[i]);
        if (var==nullptr)
            ErrorHandler::e.Error("Swap error: both parameters must be variables");
        vars[i]=var;
    }
    as->Comment("Swap variables");
//    as->Asm("lda " + vars[0]->getValue(as));
    LoadVar(as, 0);
    as->Asm("tay ");
    LoadVar(as, 1);
//    SaveVar(as, 0);
    m_codeGen->StoreVariable(vars[0]);
    LoadVar(as, 0);
    as->Asm("tya");
    m_codeGen->StoreVariable(vars[1]);
    //as->Asm("sta " + vars[1]->getValue(as)+ ",x");


}


void Methods6502::MemCpyLarge(Assembler *as)
{

    /*as->Comment("Memory copy large > 255 bytes");
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr)
        ErrorHandler::e.Error("MemCpyLarge parameter 0 must be Variable location");
    QSharedPointer<NodeNumber> num = (QSharedPointer<NodeNumber>)qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[3]);
    if (num==nullptr)
        ErrorHandler::e.Error("MemCpyLarge parameter 4 must be constant");
    QString defs = as->NewLabel("memcpy_defs");
    QString counter = as->NewLabel("memcpy_counter");
    QString carry = as->NewLabel("memcpy_carry");
    as->Asm("lda <"+var->getValue(as));
    as->Asm("ldy >"+var->getValue(as));
    as->Asm("sta print_text+0");
    as->Asm("sty print_text+1");
    as->Asm("lda <"+var->getValue(as));
    as->Asm("ldy >"+var->getValue(as));
    as->Asm("sta print_text+0");
    as->Asm("sty print_text+1");
    as->Asm("jmp " + defs);
    as->Label(counter + "\t dc.w\t" + num->HexValue());
    as->Label(defs);
    as->
    as->PopLabel("memcpy_defs");
    as->PopLabel("memcpy_counter");
    as->PopLabel("memcpy_carry");
*/
/*    as->Comment("memcpy");
    QString lbl = as->NewLabel("memcpy");
    as->Asm("ldx #0");
    as->Label(lbl);
    //LoadVar(as, 0, "x");
    as->Asm("lda " + var->getValue(as) + " + #" + num->HexValue() + ",x");
    SaveVar(as, 2, "x");
    as->Asm("inx");
    as->Term("cpx ");
    m_node->m_params[3]->Build(as);
    as->Term();
    as->Asm("bne " + lbl);
  */
}

void Methods6502::SetBank(Assembler *as)
{
    as->Comment("Set bank");
//    as->Asm("lda $dd00");
//    as->Asm("and #$fc");
 //   as->Asm("and #%00000000");
    as->Term("lda ");
    m_node->m_params[0]->Accept(m_codeGen);
    as->Term();
    as->Asm("sta $dd00");
}

void Methods6502::Decrunch(Assembler *as)
{
    QSharedPointer<NodeNumber>num = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[0]);
    if (num!=nullptr) {
    as->Comment("; Decrunch number direct");
    as->Asm("lda #" + QString::number(((int)num->m_val)&0xFF));
    as->Asm("sta opbase+1");
    as->Asm("lda #" + QString::number(((int)num->m_val>>8)&0xFF));
    as->Asm("sta opbase+2");
    as->Asm(m_codeGen->getCallSubroutine()+" exod_decrunch");
    return;
    }


    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr)
            ErrorHandler::e.Error("Decrunch : parameter 0 must be an incbin block or address!");

    if (var->getType(as)!=TokenType::INCBIN)
        ErrorHandler::e.Error("Decrunch : parameter 0 must be a pointer to a IncBin block or address!");



    QSharedPointer<Symbol> varS = as->m_symTab->Lookup(var->getValue(as), m_node->m_op.m_lineNumber);
    int pos = varS->m_org + varS->m_size;

//    qDebug() <<" DECRUNCH:" << varS->m_org << varS->m_size;

        as->Comment("; Decrunch number direct");
        as->Asm("lda #" + QString::number(((int)pos)&0xFF));
        as->Asm("sta opbase+1");
        as->Asm("lda #" + QString::number(((int)pos>>8)&0xFF));
        as->Asm("sta opbase+2");
        as->Asm(m_codeGen->getCallSubroutine()+" exod_decrunch");
}

void Methods6502::DecrunchFromIndex(Assembler *as)
{
    m_node->m_params[1]->Accept(m_codeGen);
    as->Term();
    as->Asm("asl");
    as->Asm("tax");

    as->Comment("; Decrunch from index");
    as->Asm("lda " + m_node->m_params[0]->getAddress()+",x");
//    m_node->m_params[0]->Accept(m_codeGen);
    //as->Term(",x", true);
    as->Asm("sta opbase+1");
    as->Asm("inx");
  //  as->Term("lda ");
  //  m_node->m_params[0]->Accept(m_codeGen);
//    as->Term(",x",true);
    as->Asm("lda " + m_node->m_params[0]->getAddress()+",x");
    as->Asm("sta opbase+2");
    as->Asm(m_codeGen->getCallSubroutine()+" exod_decrunch");

}

void Methods6502::CopyImageColorData(Assembler *as)
{
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    QString varName;

    if (var==nullptr)
    {
        QSharedPointer<NodeNumber> num =qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[0]);
        if (num==nullptr)
            ErrorHandler::e.Error("CopyImageColorData : parameter 0 must be a variable or address");

        varName = num->HexValue();
    }
    else varName = var->getValue(as);

    QSharedPointer<NodeNumber> bank = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[1]);
    if (bank==nullptr)
        ErrorHandler::e.Error("CopyImageColorData : parameter 1 must be a constant number!");





    QString addBank="0";
    if (bank->m_val==1)
        addBank="$4000";
    if (bank->m_val==2)
        addBank="$8000";
    if (bank->m_val==3)
        addBank="$c000";

    QString lbl = as->NewLabel("copyimageloop");


    as->Comment("Copy image color data");
    as->Asm("lda "+varName);
    as->Asm("sta $d020 ; background");
    as->Asm("lda "+varName+"+1");
    as->Asm("sta $d021; foreground");
    as->Asm("ldx #$00");
    as->Label(lbl);
    as->Asm("lda "+varName+"+2,x");
    as->Asm("sta $0400 +"+addBank+",x");
    as->Asm("lda "+varName+" + $102,x");
    as->Asm("sta $0500+"+addBank+",x");
    as->Asm("lda "+varName+" + $202,x");
    as->Asm("sta $0600+"+addBank+",x");
    as->Asm("lda "+varName+" + $302,x");
    as->Asm("sta $0700+"+addBank+",x");

    as->Comment("color #2");
    as->Asm("lda "+varName+"+1002,x");
    as->Asm("sta $d800,x");
    as->Asm("lda "+varName+"+1002+ $100,x");
    as->Asm("sta $d900,x");
    as->Asm("lda "+varName+"+1002+ $200,x");
    as->Asm("sta $da00,x");
    as->Asm("lda "+varName+"+1002+ $300,x");
    as->Asm("sta $db00,x");

    as->Asm("inx");
    as->Asm("bne " + lbl);

    as->PopLabel("copyimageloop");

}

void Methods6502::CopyHalfScreen(Assembler *as)
{
    as->Comment("Copy half screen unrolled 500 bytes = 12.5*40");
    m_node->RequireAddress(m_node->m_params[0],"CopyHalfscreen", m_node->m_op.m_lineNumber);
    m_node->RequireAddress(m_node->m_params[1],"CopyHalfscreen", m_node->m_op.m_lineNumber);

    QString lbl = as->NewLabel("halfcopyloop");
    QString lbl2 = as->NewLabel("halfcopyloop2");

    // Copy last bit first
  /*  as->Asm("ldx #20");
    as->Label(lbl2);
    as->Term("lda ");
    m_node->m_params[0]->Build(as);
    as->Term(" + #40*#12-#1,x", true);
    as->Term("sta ");
    m_node->m_params[1]->Build(as);
    as->Term(" + #40*#12-#1,x", true);
    as->Asm("dex");
    as->Asm("bne "+lbl2);
*/
    AddMemoryBlock(as,1);

    QSharedPointer<NodeNumber> lines = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[2]);
    if (lines==nullptr)
        ErrorHandler::e.Error("CopyHalfScreen : parameter 3 must be a constant number!");

    QSharedPointer<NodeNumber>inverted = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[3]);
    if (inverted==nullptr)
        ErrorHandler::e.Error("CopyHalfScreen : parameter 4 must be a constant number!");

    QSharedPointer<NodeNumber>invertedx = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[4]);
    if (invertedx==nullptr)
        ErrorHandler::e.Error("CopyHalfScreen : parameter 5 must be a constant number!");

    if (invertedx->m_val==1)
        as->Asm("ldx #40");
    else
        as->Asm("ldx #00");
    as->Label(lbl);



    int cnt = lines->m_val;

    QString shift = "";
    for (int i=0;i<cnt;i++) {
        if (inverted->m_val==1)
            shift = "" +QString::number(((cnt-1-i)*40)) +" -1 ";
        else
            shift = "" +QString::number(((i)*40)) +" -1 ";

        as->ClearTerm();
        as->Term("lda ");
        m_node->m_params[0]->Accept(m_codeGen);
        as->Term(" + "+shift+",x", true);
        as->Term("sta ");
        m_node->m_params[1]->Accept(m_codeGen);
        as->Term(" + "+shift+",x", true);
    }
    // Afterwards, copy last 25 bytes
    if (invertedx->m_val==1)
        as->Asm("dex");
    else {
        as->Asm("inx");
        as->Asm("cpx #40");
    }
    as->Asm("bne "+lbl);

    // Param 0: from
    // Param 1: to

    as->PopLabel("halfcopyloop");
    as->PopLabel("halfcopyloop2");

}

void Methods6502::CopyFullScreenUnrolled(Assembler *as)
{
    as->Comment("Copy screen unrolled 1000 bytes");
    m_node->RequireAddress(m_node->m_params[0],"CopyFullscreen", m_node->m_op.m_lineNumber);
    m_node->RequireAddress(m_node->m_params[1],"CopyFullscreen", m_node->m_op.m_lineNumber);

    QString lbl = as->NewLabel("fullcopyloop");
    QString lbl2 = as->NewLabel("fullcopyloop2");
    QString lblDone = as->NewLabel("fullcopydone");

    as->Asm("ldx #40");
    as->Label(lbl);
    for (int i=0;i<25;i++) {
//        QString shift = "" +QString::number(((24-i)*40)) +" -1 ";
        QString shift = "" +QString::number(((24-i)*40)) +" -1 ";

        as->ClearTerm();
        as->Term("lda ");
        m_node->m_params[0]->Accept(m_codeGen);
        as->Term(" + "+shift+",x", true);
        as->Term("sta ");
        m_node->m_params[1]->Accept(m_codeGen);
        as->Term(" + "+shift+",x", true);
    }
    // Afterwards, copy last 25 bytes
    as->Asm("dex");
    as->Asm("beq "+lblDone);
    as->Asm("jmp " + lbl);
    as->Label(lblDone);

    as->PopLabel("fullcopyloop");
    as->PopLabel("fullcopyloop2");
    as->PopLabel("fullcopydone");

}

void Methods6502::CopyFullScreen(Assembler *as)
{

    as->Comment("Copy full screen");
//    m_node->RequireAddress(m_node->m_params[0],"CopyFullscreen", m_node->m_op.m_lineNumber);
  //  m_node->RequireAddress(m_node->m_params[1],"CopyFullscreen", m_node->m_op.m_lineNumber);
    AddMemoryBlock(as,1);


   if (m_node->m_params[0]->getType(as) == TokenType::POINTER || m_node->m_params[1]->getType(as) == TokenType::POINTER) {
        // Do full pointer blah

        QString lblOuter = as->NewLabel("outer");
        QString lblInner = as->NewLabel("inner");
        QString lblFinal = as->NewLabel("final");


        QString zpf = as->m_internalZP[0];
        QString zpt = as->m_internalZP[1];

        LoadAndStoreInZp(m_node->m_params[0],as,zpf);
        LoadAndStoreInZp(m_node->m_params[1],as,zpt);



        as->Comment("CopyFullscreen with pointers");

        as->Asm("ldx #3");
        as->Label(lblOuter);

        as->Asm("ldy #0");
        as->Label(lblInner);
        as->ClearTerm();
        as->Asm("lda ("+zpf+"),y");
        as->Asm("sta ("+zpt+"),y");


        as->Asm("dey");
        as->Asm("bne "+lblInner);
        as->Asm("inc "+zpf+"+1");
        as->Asm("inc "+zpt+"+1");

        as->Asm("dex");
        as->Asm("bne "+lblOuter);



        as->Asm("ldy #0");
        as->Label(lblFinal);
        as->ClearTerm();
        as->Asm("lda ("+zpf+"),y");
        as->Asm("sta ("+zpt+"),y");

        as->Asm("iny");
        as->Asm("cpy #232");
        as->Asm("bne "+lblFinal);



        as->PopLabel("outer");
        as->PopLabel("inner");
        as->PopLabel("final");


        return;
    }
    QString lbl = as->NewLabel("fullcopyloop");
    QString lbl2 = as->NewLabel("fullcopyloop2");

    as->Asm("ldx #0");
    as->Label(lbl);
    for (int i=0;i<3;i++) {
//        QString shift = "" +QString::number(((24-i)*40)) +" -1 ";
        QString shift = "$"+QString::number(0x100*i,16);
        as->ClearTerm();
        as->Term("lda ");
        m_node->m_params[0]->Accept(m_codeGen);
        as->Term(" + "+shift+",x", true);
        as->Term("sta ");
        m_node->m_params[1]->Accept(m_codeGen);
        as->Term(" + "+shift+",x", true);
    }
    as->Asm("dex");
    as->Asm("bne " + lbl);
    as->Asm("ldx #232");
    as->Label(lbl2);
    QString shift = "$"+QString::number(0x100*3-1,16);
    as->ClearTerm();
    as->Term("lda ");
    m_node->m_params[0]->Accept(m_codeGen);
    as->Term(" + "+shift+",x", true);
    as->Term("sta ");
    m_node->m_params[1]->Accept(m_codeGen);
    as->Term(" + "+shift+",x", true);

    as->Asm("dex");
    as->Asm("bne " + lbl2);

}

void Methods6502::BlockMemCpy(Assembler *as)
{

    as->Comment("Block memcpy");
//    m_node->RequireAddress(m_node->m_params[0],"BlockMemCpy", m_node->m_op.m_lineNumber);
  //  m_node->RequireAddress(m_node->m_params[1],"BlockMemCpy", m_node->m_op.m_lineNumber);


/*    QSharedPointer<NodeNumber> from = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[0]);
    if (from==nullptr)
        ErrorHandler::e.Error("BlockMemCpy parameter 1 *must* be a pure number");
*/

    AddMemoryBlock(as,1);




//    lda (zp),y


    QString lbl = as->NewLabel("Blockmemcpy_lbl1");
    QString lbl2 = as->NewLabel("Blockmemcpy_lbl2");

    as->PopLabel("Blockmemcpy_lbl1");
    as->PopLabel("Blockmemcpy_lbl2");

    if (m_node->m_params[0]->getType(as)!=TokenType::POINTER) {
        QSharedPointer<NodeNumber> num = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[2]);
        if (num==nullptr)
            ErrorHandler::e.Error("BlockMemCpy parameter 3 *must* be a pure number");

        QSharedPointer<NodeNumber> to = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[1]);
        if (to==nullptr)
            ErrorHandler::e.Error("BlockMemCpy parameter 2 *must* be a pure number");


        int v = num->m_val;

        as->Asm("ldy #0");
        as->Label(lbl);

        for (int i=0;i<v;i++) {
            if (m_node->m_params[0]->isPureNumericOrAddress())
                as->Asm("lda $"+QString::number(m_node->m_params[0]->getValueAsInt(as) + i*256,16)+",y");
            else
                as->Asm("lda #"+m_node->m_params[0]->getValue(as) +"+$"+ QString::number(i*256,16)+",y");
            as->Asm("sta $"+QString::number((int)to->m_val + i*256,16)+",y");
        }

        as->Asm("dey");
        as->Asm("bne " + lbl);

    }

    if (m_node->m_params[0]->getType(as)==TokenType::POINTER || m_node->m_params[1]->getType(as)==TokenType::POINTER) {
//        qDebug() << "HERE" << as->m_internalZP.count();
        if (as->m_internalZP.count()==0)
            return;

        QString zp = as->m_internalZP[0];
        QString zp2 = as->m_internalZP[1];
  //      qDebug() << "HERE -1";
/*        if (m_node->m_params[0]->getType(as)==TokenType::POINTER) {
            as->Asm("lda "+m_node->m_params[0]->getAddress());
    //    qDebug() << "HERE -2";
            as->Asm("sta "+zp);
            as->Asm("lda "+m_node->m_params[0]->getAddress()+"+1");
            as->Asm("sta "+zp+"+1");
        }
        else
             {
            m_node->m_params[0]->Accept(m_codeGen);
            as->Asm("sta "+zp);
            as->Asm("sty "+zp+"+1");
        }

        if (m_node->m_params[1]->getType(as)==TokenType::POINTER) {
            as->Asm("lda "+m_node->m_params[1]->getAddress());
            as->Asm("sta "+zp2);
            as->Asm("lda "+m_node->m_params[1]->getAddress()+"+1");
            as->Asm("sta "+zp2+"+1");
        }
        else
        {
       m_node->m_params[1]->Accept(m_codeGen);
       as->Asm("sta "+zp2);
       as->Asm("sty "+zp2+"+1");
   }*/

        LoadAddress(as,0);
        as->Term();
        as->Asm("sta "+zp);
        as->Asm("sty "+zp+"+1");
        LoadAddress(as,1);
        as->Term();
        as->Asm("sta "+zp2);
        as->Asm("sty "+zp2+"+1");



      //  qDebug() << "HERE 0";
        m_node->m_params[2]->Accept(m_codeGen);
        //qDebug() << "HERE 1";
        as->Term();
       // qDebug() << "HERE 2";

        as->Asm("tax");
        as->Label(lbl2);
        as->Asm("ldy #0");
        as->Label(lbl);

            as->Asm("lda ("+zp+"),y");
            as->Asm("sta ("+zp2+"),y");

        as->Asm("dey");
        as->Asm("bne " + lbl);
        as->Asm("inc "+zp+"+1");
        as->Asm("inc "+zp2+"+1");
        as->Asm("dex");
        as->Asm("bne " + lbl2);
        //qDebug() << "HERE 3";

    }

}


void Methods6502::TransformColors(Assembler *as)
{
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    if (var==nullptr)
        ErrorHandler::e.Error("Parameter 0 must be variable (array)");

    QSharedPointer<NodeNumber> num = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[1]);
    if (num==nullptr)
        ErrorHandler::e.Error("Parameter 1 must be address");

    QString tempVar = as->StoreInTempVar("tempTransform");
    for (int i=0;i<4;i++) {
        QString loopInner = as->NewLabel("transform_loop_outer");
        QString shift = " -#1 + #" + QString::number(i*250);
        as->Asm("ldx #250");
        as->Label(loopInner);
        as->Asm("lda " + num->StringValue() + shift +",x");
        as->Asm("pha");
        as->Asm("and #$F0");
        as->Asm("lsr");
        as->Asm("lsr");
        as->Asm("lsr");
        as->Asm("lsr");
        as->Asm("tay");
        as->Asm("lda " + var->getValue(as)+ ",y");

        as->Asm("asl");
        as->Asm("asl");
        as->Asm("asl");
        as->Asm("asl");
        as->Asm("sta " + tempVar);
        as->Asm("pla");
        as->Asm("and #$0F");
        as->Asm("tay");
        as->Asm("lda " + var->getValue(as)+ ",y");
        as->Asm("ora " + tempVar);
        as->Asm("sta "+num->StringValue() + shift +",x");

        as->Asm("lda $D800"+shift+",x");
        as->Asm("tay");
        as->Asm("lda " + var->getValue(as)+ ",y");
        as->Asm("sta $D800"+shift+",x");


        as->Asm("dex");
        as->Asm("bne " + loopInner);
        as->PopLabel("transform_loop_outer");
    }
    as->PopTempVar();
}

void Methods6502::ToggleBit(Assembler *as)
{
    QSharedPointer<NodeNumber> toggle = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[2]);
    if (toggle==nullptr)
        ErrorHandler::e.Error("TogglesBit (for now) needs param 3 to be a number");


    QSharedPointer<NodeNumber> spriteNum = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[1]);

    if (spriteNum!=nullptr) {

        uchar v = 1 << (uchar)spriteNum->m_val;
        as->Comment("Toggle bit with constant");
        if (toggle->m_val==0) { // turn off}
            LoadVar(as, 0);
            as->Asm("and #%"+QString::number((uchar)~v,2));
            SaveVar(as,0);
        }
        if (toggle->m_val==1) { // turn off}
            LoadVar(as, 0);
            as->Asm("ora #%"+QString::number((uchar)v,2));
            SaveVar(as,0);
        }
    }
    m_node->m_params[1]->Accept(m_codeGen);
    as->Term();
    as->Asm("tax");
    QString var = BitShiftX(as);
    if (toggle->m_val==0) { // turn off}
        as->Asm("lda #$FF");
        as->Asm("eor " + var);
        as->Asm("sta " + var);
        LoadVar(as, 0);
        as->Asm("and " + var);
        SaveVar(as,0);
    }
    if (toggle->m_val==1) { // turn off}
        LoadVar(as, 0);
        as->Asm("ora " +var);
        SaveVar(as,0);
    }
    as->PopTempVar();
    // Else, perform the full shit...
}


void Methods6502::GetBit(Assembler *as)
{
    QSharedPointer<NodeNumber> sprite = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[1]);


    if (sprite!=nullptr) {
        QString lbl = as->NewLabel("getbit_false");
        uchar v = 1 << (uchar)sprite->m_val;
        as->Comment("Get bit");
        LoadVar(as, 0);
        as->Asm("and #%"+QString::number((uchar)v,2));
        as->Asm("beq " + lbl);
        as->Asm("lda #1");
        as->Label(lbl);
//        as->Asm("eor #1");
        as->PopLabel("getbit_false");
        return;
    }

    QString lbl = as->NewLabel("getbit_false");
    QString lblD = as->NewLabel("getbit_done");
    m_node->m_params[1]->Accept(m_codeGen);
    as->Term();
    as->Asm("tax");
    QString var = BitShiftX(as);
    LoadVar(as, 0);
    as->Asm("and " + var);
    as->Asm("cmp " + var);
    as->Asm("bne " + lbl);
    as->Asm("lda #1");
    as->Asm("jmp " + lblD);
    as->Label(lbl);
    as->Asm("lda #0");
    as->Label(lblD);

    as->PopLabel("getbit_false");

    as->PopTempVar();
//    ErrorHandler::e.Error("NOT IMPLEMENTED (getbit with number)");

    // do full shit
}


void Methods6502::CopyZPdata(Assembler *as)
{
    as->Comment("Copy Zero-page data");
    QString loop = as->NewLabel("zpcopy");
    as->ClearTerm();
    as->Term("ldy ");
    m_node->m_params[1]->Accept(m_codeGen);
    as->Term();
    as->Label(loop);
    as->Asm("lda (print_text),y");
 //   as->Asm("lda #46");
    as->Term("sta ");
    m_node->m_params[0]->Accept(m_codeGen);
    as->Term(",y", true);
    as->Asm("dey");
    as->Asm("bne " + loop);

    as->PopLabel("zpcopy");


}

/*
void Methods6502::EnableInterrupts(Assembler* as) {
  //  as->Asm("lda $dc0d");
  //  as->Asm("lda $dd0d");
//    as->Asm("asl $d019");
    as->Asm("lda $d011   ; Bit#0 of $d011 is basically...");
    as->Asm("and #$7f    ; ...the 9th Bit for $d012");
    as->Asm("sta $d011   ; we need to make sure it is set to zero ");
    as->Asm("cli");
}
*/
void Methods6502::SaveVar(Assembler *as, int paramNo, QString reg, QString extra)
{

//    qDebug() << "SAVEVAR: " <<  TokenType::getType(m_node->m_params[paramNo]->getType(as));

    if (!(m_node->m_params[paramNo]->isVariable() || m_node->m_params[paramNo]->isAddress()))
        ErrorHandler::e.Error("Parameter "+QString::number(paramNo) + " must be a variable or address", m_node->m_op.m_lineNumber);

    QString vName = m_node->m_params[paramNo]->getValue(as);
    QSharedPointer<NodeVar> v = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[paramNo]);
    as->Term();

    if (v==nullptr || v->m_expr==nullptr) {
//        qDebug() << "SAVEVAR extra " << reg << extra;
        if (reg!="")
            vName+=","+reg;
        as->Asm("sta "+vName);
        return;
    }
    QString pa = "";
    QString pb = "";
    QString x = "x";
    if (v->isPointer(as)) {
        pa = "(";
        pb = ")";
        x = "y";
    }

    // Has expression
    if (v->m_expr->isPure()) {
        as->Asm("ld"+x+" "+v->m_expr->getValue(as));
        as->Asm("sta "+pa+vName+pb + ","+x);
        return;
    }
    // Not pure. Must evaluate
    as->Asm("pha");

    v->m_expr->Accept(m_codeGen);
    as->Term();
    as->Asm("ta"+x);
    as->Asm("pla");
    as->Asm("sta "+pa+vName+pb + ","+x);


/*    as->ClearTerm();
    if (extra=="")
        as->Term("sta ");
    else
        as->Term(extra);

    m_node->m_params[paramNo]->Accept(m_codeGen);

    if (reg!="")
        reg = "," + reg;
    as->Term(reg, true);
*/
}


void Methods6502::LoadVar(Assembler *as, int paramNo, QString reg, QString lda)
{

    QSharedPointer<Node> node = m_node->m_params[paramNo];

/*
        // Override with INLINE parameter
//    qDebug() << node->getValue(as) << m_codeGen->m_inlineParameters.keys();
    if (m_codeGen->m_inlineParameters.contains(node->getValue(as)))
        node = m_codeGen->m_inlineParameters[node->getValue(as)];
*/

    QSharedPointer<NodeVar> nodevar = qSharedPointerDynamicCast<NodeVar>(node);
    if (node->isPureNumericOrAddress() && node->getValueAsInt(as)>=256 && !node->isAddress()) {
        m_codeGen->Disable16bit();
        as->Asm("lda #" + Util::numToHex(node->getValueAsInt(as)&0xff));
        as->Asm("ldy #" + Util::numToHex((node->getValueAsInt(as)>>8)&0xff));
        m_codeGen->Enable16bit();
        return;
    }
    bool disable16bit = true;
    if (lda=="ldx" || lda=="ldy" || (node->getType(as)==TokenType::POINTER && nodevar->m_expr==nullptr) || reg!="")
        disable16bit = true;



    if (qSharedPointerDynamicCast<NodeVar>(node)!=nullptr ||
        qSharedPointerDynamicCast<NodeNumber>(node)!=nullptr) {

        if (disable16bit)
            m_codeGen->Disable16bit();


        as->ClearTerm();
        if (lda=="")
            as->Term("lda ");
        else
            as->Term(lda);


        m_node->m_params[paramNo]->Accept(m_codeGen);
        if (reg!="")
            reg = "," + reg;

        as->Term(reg, true);

        if (nodevar!=nullptr)
        if (node->getType(as)==TokenType::POINTER && nodevar->m_expr==nullptr) {
            as->Asm("ldy " + node->getValue(as)+"+1");
        }

        if (disable16bit)
            m_codeGen->Enable16bit();


    }
    else
        m_node->m_params[paramNo]->Accept(m_codeGen);


}

void Methods6502::LoadVar(Assembler *as, int paramNo)
{
    LoadVar(as, paramNo, "");
}

void Methods6502::SaveVar(Assembler *as, int paramNo)
{
    SaveVar(as, paramNo, "");
}

void Methods6502::VerifyInitialized(QString method, QString initmethod)
{
    if (!m_node->m_isInitialized[method])
        ErrorHandler::e.Error("Please declare "+ initmethod+"() before using " + method+"();");

}

void Methods6502::VDCWrite(Assembler *as)
{
    QString lbl = as->NewLabel("vdc_write");

    m_node->m_params[0]->Accept(m_codeGen);
    as->Term();
    as->Asm("sta $D600");
    as->Label(lbl);
    as->Asm("bit $D600");
    as->Asm("bpl "+lbl);
    m_node->m_params[1]->Accept(m_codeGen);
    as->Term();
    as->Asm("sta $D601");
    as->PopLabel("vdc_write");
}

void Methods6502::VDCInit(Assembler *as)
{
    QString lbl = as->NewLabel("vdc_init");

    m_node->m_params[0]->Accept(m_codeGen);
    as->Term();
    as->Asm("sta $D600");
    as->Label(lbl);
    as->Asm("bit $D600");
    as->Asm("bpl "+lbl);
    as->PopLabel("vdc_init");

}



// initialise the Bcd Print Digit code
void Methods6502::InitBcd(Assembler *as)
{
    if (m_node->m_isInitialized["initbcd"])
        return;
    m_node->m_isInitialized["initbcd"] = true;

    as->Comment("----------");
    as->Comment("BCD support");
    as->Comment("Commands for manipulating and displaying BCD numbers");
    as->Comment("");
    as->Comment("a = digit to display");
    as->Comment("y = screen offset");

    as->Label("bcdplotdigit");
    as->Asm("clc");
    as->Asm("adc #48 ; 0 screen code");
    as->Asm("sta (screenmemory),y");
    as->Asm("dey");
    //as->Asm("rts");

}

void Methods6502::BcdAdd(Assembler *as)
{
    as->Comment("----------");
    as->Comment("BcdAdd address, address, number");

    // BCD array address to add to
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    QSharedPointer<NodeNumber> num = (QSharedPointer<NodeNumber>)qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumericOrAddress()) {
        ErrorHandler::e.Error("First parameter must be variable or number", m_node->m_op.m_lineNumber);
    }

    QString srcaddr = "";
    if (m_node->m_params[0]->isPureNumericOrAddress())
        srcaddr = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        srcaddr = var->getValue(as);

    // BCD array address containing value to add
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[1]);
    num = (QSharedPointer<NodeNumber>)qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[1]);
    if (var==nullptr && !m_node->m_params[1]->isPureNumericOrAddress()) {
        ErrorHandler::e.Error("Second parameter must be variable or number", m_node->m_op.m_lineNumber);
    }

    QString addaddr = "";
    if (m_node->m_params[1]->isPureNumericOrAddress())
        addaddr = m_node->m_params[1]->HexValue();
    if (var!=nullptr)
        addaddr = var->getValue(as);

    QSharedPointer<NodeNumber> num3 = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[2]);
    if (num3==nullptr)
        ErrorHandler::e.Error("BCD: last parameter, number of digits, required to be pure constant number");

    int numDigits = num3->m_val;
    if (numDigits < 1 || numDigits > 254)
        ErrorHandler::e.Error("BCD: last parameter, number of digits, must be greater than 0 but less than 255");


    as->Asm("sed");
    as->Asm("clc");
    as->Asm("lda " + srcaddr);
    as->Asm("adc " + addaddr);
    as->Asm("sta " + srcaddr);
    if (numDigits > 1) {

        for (int i=1; i<numDigits; i++) {

            as->Asm("lda " + srcaddr + "+" + Util::numToHex(i));
            as->Asm("adc " + addaddr + "+" + Util::numToHex(i));
            as->Asm("sta " + srcaddr + "+" + Util::numToHex(i));

        }

    }
    as->Asm("cld");

}

void Methods6502::BcdSub(Assembler *as)
{

    as->Comment("----------");
    as->Comment("BcdSub address, address, number");

    // BCD array address to subtract from
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    QSharedPointer<NodeNumber> num = (QSharedPointer<NodeNumber>)qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumericOrAddress()) {
        ErrorHandler::e.Error("First parameter must be variable or number", m_node->m_op.m_lineNumber);
    }

    QString srcaddr = "";
    if (m_node->m_params[0]->isPureNumericOrAddress())
        srcaddr = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        srcaddr = var->getValue(as);

    // BCD array address containing value to subtract
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[1]);
    num = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[1]);
    if (var==nullptr && !m_node->m_params[1]->isPureNumericOrAddress()) {
        ErrorHandler::e.Error("Second parameter must be variable or number", m_node->m_op.m_lineNumber);
    }

    QString addaddr = "";
    if (m_node->m_params[1]->isPureNumericOrAddress())
        addaddr = m_node->m_params[1]->HexValue();
    if (var!=nullptr)
        addaddr = var->getValue(as);

    QSharedPointer<NodeNumber> num3 = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[2]);
    if (num3==nullptr)
        ErrorHandler::e.Error("BCD: last parameter, number of digits, required to be pure constant number");

    int numDigits = num3->m_val;
    if (numDigits < 1 || numDigits > 254)
        ErrorHandler::e.Error("BCD: last parameter, number of digits, must be greater than 0 but less than 255");


    as->Asm("sed");
    as->Asm("sec");
    as->Asm("lda " + srcaddr);
    as->Asm("sbc " + addaddr);
    as->Asm("sta " + srcaddr);
    if (numDigits > 1) {

        for (int i=1; i<numDigits; i++) {

            as->Asm("lda " + srcaddr + "+" + Util::numToHex(i));
            as->Asm("sbc " + addaddr + "+" + Util::numToHex(i));
            as->Asm("sta " + srcaddr + "+" + Util::numToHex(i));

        }

    }
    as->Asm("cld");

}

void Methods6502::BcdCompare(Assembler *as)
{

    as->Comment("----------");
    as->Comment("BcdCompare address, address, number");

    // BCD array address to compare
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    QSharedPointer<NodeNumber> num = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumericOrAddress()) {
        ErrorHandler::e.Error("First parameter must be variable or number", m_node->m_op.m_lineNumber);
    }

    QString srcaddr = "";
    if (m_node->m_params[0]->isPureNumericOrAddress())
        srcaddr = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        srcaddr = var->getValue(as);

    // BCD array address containing value to compare with
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[1]);
    num = (QSharedPointer<NodeNumber>)qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[1]);
    if (var==nullptr && !m_node->m_params[1]->isPureNumericOrAddress()) {
        ErrorHandler::e.Error("Second parameter must be variable or number", m_node->m_op.m_lineNumber);
    }

    QString dstaddr = "";
    if (m_node->m_params[1]->isPureNumericOrAddress())
        dstaddr = m_node->m_params[1]->HexValue();
    if (var!=nullptr)
        dstaddr = var->getValue(as);

    QSharedPointer<NodeNumber> num3 = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[2]);
    if (num3==nullptr)
        ErrorHandler::e.Error("BCD: last parameter, number of digits, required to be pure constant number");

    int numDigits = num3->m_val;
    if (numDigits < 1 || numDigits > 254)
        ErrorHandler::e.Error("BCD: last parameter, number of digits, must be greater than 0 but less than 255");
    numDigits--;

    QString lblbcdMorethanLoop = as->NewLabel("bcdMorethanLoop");
    QString lblbcdIsEqual = as->NewLabel("bcdIsEqual");
    QString lblbcdIsLess = as->NewLabel("bcdIsLess");
    QString lblbcdIsMore = as->NewLabel("bcdIsMore");
    QString lblbcdDone = as->NewLabel("bcdDone");

    as->Asm("ldx #" + Util::numToHex(numDigits));
    as->Label(lblbcdMorethanLoop);
    as->Asm("lda " + srcaddr +",x");
    as->Asm("cmp "+dstaddr+",x");
    as->Asm("bcc "+ lblbcdIsLess);
    as->Asm("beq "+ lblbcdIsEqual);
    as->Asm("bcs "+ lblbcdIsMore);
    as->Label(lblbcdIsEqual);
    as->Asm("dex");
    as->Asm("cpx #$ff");
    as->Asm("bne "+lblbcdMorethanLoop);
    as->Label(lblbcdIsLess);
    as->Asm("lda #0");
    as->Asm("beq "+ lblbcdDone + "; skip next with short branch");
    as->Label(lblbcdIsMore);
    as->Asm("lda #1");
    as->Label(lblbcdDone);

    as->PopLabel("bcdMorethanLoop");
    as->PopLabel("bcdIsEqual");
    as->PopLabel("bcdIsLess");
    as->PopLabel("bcdIsMore");
    as->PopLabel("bcdDone");
}


void Methods6502::BcdIsEqual(Assembler *as)
{
    as->Comment("----------");
    as->Comment("BcdIsEqual address, address, number");

    // BCD array address to compare
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    QSharedPointer<NodeNumber> num = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumericOrAddress()) {
        ErrorHandler::e.Error("First parameter must be variable or number", m_node->m_op.m_lineNumber);
    }

    QString srcaddr = "";
    if (m_node->m_params[0]->isPureNumericOrAddress())
        srcaddr = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        srcaddr = var->getValue(as);

    // BCD array address containing value to compare with
    var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[1]);
    num = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[1]);
    if (var==nullptr && !m_node->m_params[1]->isPureNumericOrAddress()) {
        ErrorHandler::e.Error("Second parameter must be variable or number", m_node->m_op.m_lineNumber);
    }

    QString dstaddr = "";
    if (m_node->m_params[1]->isPureNumericOrAddress())
        dstaddr = m_node->m_params[1]->HexValue();
    if (var!=nullptr)
        dstaddr = var->getValue(as);

    QSharedPointer<NodeNumber> num3 = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[2]);
    if (num3==nullptr)
        ErrorHandler::e.Error("BCD: last parameter, number of digits, required to be pure constant number");

    int numDigits = num3->m_val;
    if (numDigits < 1 || numDigits > 254)
        ErrorHandler::e.Error("BCD: last parameter, number of digits, must be greater than 0 but less than 255");
    numDigits--;

    QString lblbcdEqualLoop = as->NewLabel("bcdEqualLoop");
    QString lblbcdNotEqual = as->NewLabel("bcdNotEqual");
    QString lblbcdDone = as->NewLabel("bcdDone");

    as->Asm("ldx #" + Util::numToHex(numDigits));
    as->Label(lblbcdEqualLoop);
    as->Asm("lda " + srcaddr +",x");
    as->Asm("cmp "+dstaddr+",x");
    as->Asm("bne "+ lblbcdNotEqual);
    as->Asm("dex");
    as->Asm("cpx #$ff");
    as->Asm("bne "+lblbcdEqualLoop);
    as->Asm("lda #1 ; is equal");
    as->Asm("bne "+ lblbcdDone +" ; skip next with short branch");
    as->Label(lblbcdNotEqual);
    as->Asm("lda #0 ; is not equal");
    as->Label(lblbcdDone);

    as->PopLabel("bcdEqualLoop");
    as->PopLabel("bcdNotEqual");
    as->PopLabel("bcdDone");

}

void Methods6502::BcdPrint(Assembler *as)
{

    as->Comment("----------");
    as->Comment("BcdPrint address, number");

    // BCD array address to add to
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(m_node->m_params[0]);
    //QSharedPointer<NodeNumber> num = (QSharedPointer<NodeNumber>)qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[0]);
    if (var==nullptr && !m_node->m_params[0]->isPureNumericOrAddress()) {
        ErrorHandler::e.Error("First parameter must be variable or number", m_node->m_op.m_lineNumber);
    }

    QString srcaddr = "";
    if (m_node->m_params[0]->isPureNumericOrAddress())
        srcaddr = m_node->m_params[0]->HexValue();
    if (var!=nullptr)
        srcaddr = var->getValue(as);

    QSharedPointer<NodeNumber> num3 = qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[1]);
    if (num3==nullptr)
        ErrorHandler::e.Error("BCD: last parameter, number of digits, required to be pure constant number");

    int numDigits = num3->m_val;
    if (numDigits < 1 || numDigits > 254)
        ErrorHandler::e.Error("BCD: last parameter, number of digits, must be greater than 0 but less than 255");
    int numScreen = (numDigits * 2) - 1;

    QString lblLoop = as->NewLabel("bcdprintloop");

    as->Asm("ldy #" + Util::numToHex(numScreen) + " ; screen offset");
    as->Asm("ldx #0 ; score byte index");
    as->Label(lblLoop);
    as->Asm("lda " + srcaddr+",x");
    as->Asm("inx");
    as->Asm("pha");
    as->Asm("and #$0f");
    as->Asm("jsr bcdplotdigit");
    as->Asm("pla");
    as->Asm("lsr");
    as->Asm("lsr");
    as->Asm("lsr");
    as->Asm("lsr");
    as->Asm("jsr bcdplotdigit");
    as->Asm("bpl " + lblLoop);

    as->PopLabel("bcdprintloop");
}

void Methods6502::InitJoystick(Assembler *as)
{

    if (as->m_internalZP.count()==0)
        return;

//    as->Asm("jmp callJoystick");
    as->Write("joystickup: .byte 0");
    as->Write("joystickdown: .byte 0");
    as->Write("joystickleft: .byte 0");
    as->Write("joystickright: .byte 0");
    as->Write("joystickbutton: .byte 0");
    as->Label("callJoystick");
/*
    as->Asm("lda $dc00 ;read joystick port 2");
    as->Asm("lsr       ;get switch bits");
    as->Asm("ror joystickup   ;switch_history = switch_history/2 + 128*current_switch_state");
    as->Asm("lsr       ;update the other switches' history the same way");
    as->Asm("ror joystickdown");
    as->Asm("lsr");
    as->Asm("ror joystickleft");
    as->Asm("lsr");
    as->Asm("ror joystickright");
    as->Asm("lsr");
    as->Asm("ror joystickbutton");
//    as->Asm("rts*/

/*    as->Asm("lda joystickvalue");
    as->Asm("cmp $dc00");
    as->Asm("beq callJoystick_end");
    as->Asm("lda  $dc00      ; store new value in memory location 2.");
    as->Asm("sta joystickvalue");
*/

    as->Asm("lda #0");
    as->Asm("sta joystickup");
    as->Asm("sta joystickdown");
    as->Asm("sta joystickleft");
    as->Asm("sta joystickright");
    as->Asm("sta joystickbutton");


    //QString port = "$dc00";
    // UP
    QString addr = as->m_internalZP[2];

    as->Asm("lda #%00000001 ; mask joystick up mment");
    as->Asm("bit "+addr+"      ; bitwise AND with address 56320");
    as->Asm("bne joystick_down       ; zero flag is not set -> skip to down");
    as->Asm("lda #1");
    as->Asm("sta joystickup");

    as->Label("joystick_down");
    // DOWN

    as->Asm("lda #%00000010 ; mask joystick down movement");
    as->Asm("bit "+addr+"      ; bitwise AND with address 56320");
    as->Asm("bne joystick_left       ; zero flag is not set -> skip to down");
    as->Asm("lda #1");
    as->Asm("sta joystickdown");
    // LEFT

    as->Label("joystick_left");

    as->Asm("lda #%00000100 ; mask joystick left movement");
    as->Asm("bit "+addr+"      ; bitwise AND with address 56320");
    as->Asm("bne joystick_right       ; zero flag is not set -> skip to down");
    as->Asm("lda #1");
    as->Asm("sta joystickleft");

    // RIGHT
    as->Label("joystick_right");

    as->Asm("lda #%00001000 ; mask joystick up movement");
    as->Asm("bit "+addr+"      ; bitwise AND with address 56320");
    as->Asm("bne joystick_button       ; zero flag is not set -> skip to down");
    as->Asm("lda #1");
    as->Asm("sta joystickright");

    as->Label("joystick_button");
    // BUTTON

    as->Asm("lda #%00010000 ; mask joystick up movement");
    as->Asm("bit "+addr+"      ; bitwise AND with address 56320");
    as->Asm("bne callJoystick_end       ; zero flag is not set -> skip to down");
    as->Asm("lda #1");
    as->Asm("sta joystickbutton");


    as->Label("callJoystick_end");
    as->Asm("rts");

}


// the constants are needed for Joy1 and Joy2
void Methods6502::InitJoy(Assembler *as)
{
    if (m_node->m_isInitialized["initjoy"])
        return;
    m_node->m_isInitialized["initjoy"] = true;

    as->Comment("----------");
    as->Comment("ReadJoy1 and ReadJoy2 (on supported platforms)");
    as->Comment("populates joy1 and joy1pressed which can be tested by AND-ing with the following constants:");

    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::C64) {
        as->Label(";JOY_DOWN  = %00000010");
        as->Label(";JOY_UP    = %00000001");
        as->Label(";JOY_LEFT  = %00000100");
        as->Label(";JOY_RIGHT = %00001000");
        as->Label(";JOY_FIRE  = %00010000");
        as->Label("C64_JOY_CIAPRA = $DC00   ; joy2");
        as->Label("C64_JOY_CIAPRB = $DC01   ; joy1");
    } else if (Syntax::s.m_currentSystem->m_system==AbstractSystem::VIC20) {
        as->Label(";JOY_DOWN  = %00000100");
        as->Label(";JOY_UP    = %00000010");
        as->Label(";JOY_LEFT  = %00001000");
        as->Label(";JOY_RIGHT = %00000001");
        as->Label(";JOY_FIRE  = %00010000");
        as->Label("VIC20_PORTACASS = $911F");
        as->Label("VIC20_PORTBVIA2 = $9120  ; Port B 6522 2 value (joystick)");
        as->Label("VIC20_PORTBVIA2d = $9122 ; Port B 6522 2 direction (joystick)");
    }
}

// If using Joystick port 1 (Vic20 only has one)
void Methods6502::InitJoy1(Assembler *as)
{
    InitJoy(as);

    if (m_node->m_isInitialized["readjoy1"])
        return;
    m_node->m_isInitialized["readjoy1"] = true;


//    Create bitmasks for results for Joystick, these are read directly by TRSE code (see symboltable.cpp)
//    as->Label("joy1 .byte 0"); // current state (is held)
//    as->Label("joy1last .byte 0");  // previous state
//    as->Label("joy1pressed .byte 0"); // Just pressed state
    as->DeclareInternalVariable("joy1");
    as->DeclareInternalVariable("joy1last");
    as->DeclareInternalVariable("joy1pressed");

    as->Label("callReadJoy1");

    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::C64) {
        as->Asm("lda C64_JOY_CIAPRB");
        as->Asm("eor #255");
        as->Asm("sta joy1");
        as->Asm("eor joy1last");
        as->Asm("and joy1");
        as->Asm("sta joy1pressed");
        as->Asm("lda joy1");
        as->Asm("sta joy1last");
    } else if (Syntax::s.m_currentSystem->m_system==AbstractSystem::VIC20) {
        QString lblJoySkip = as->NewLabel("JoySkip");
        as->Asm("LDA VIC20_PORTACASS");
        as->Asm("EOR #$FF");
        as->Asm("AND #$3C");
        as->Asm("LDX #$7F");
        as->Asm("SEI");
        as->Asm("STX VIC20_PORTBVIA2d");
        as->Asm("LDY VIC20_PORTBVIA2");
        as->Asm("BMI " + lblJoySkip);
        as->Asm("ORA #$02");

        as->Label(lblJoySkip);

        as->Asm("LDX #$FF");
        as->Asm("STX VIC20_PORTBVIA2d");
        as->Asm("CLI");
        as->Asm("LSR");

        as->Asm("STA joy1");
        as->Asm("eor joy1last");
        as->Asm("and joy1");
        as->Asm("sta joy1pressed");
        as->Asm("lda joy1");
        as->Asm("sta joy1last");

        as->PopLabel("JoySkip");
    }

}
// If using Joystick port 2 (for C64 and other platforms)
// Note: separated the code so that the ASM output can be as small as possible if only using one joystick port
void Methods6502::InitJoy2(Assembler *as)
{
    InitJoy(as);

    if (m_node->m_isInitialized["readjoy2"])
        return;
    m_node->m_isInitialized["readjoy2"] = true;


//    Create bitmasks for results for Joystick, these are read directly by TRSE code (see symboltable.cpp)
//    as->Label("joy2 .byte 0"); // current state (is held)
//    as->Label("joy2last .byte 0");  // previous state
//    as->Label("joy2pressed .byte 0"); // Just pressed state
    as->DeclareInternalVariable("joy2");
    as->DeclareInternalVariable("joy2last");
    as->DeclareInternalVariable("joy2pressed");


    as->Label("callReadJoy2");

    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::C64) {
        as->Asm("lda C64_JOY_CIAPRA");
        as->Asm("eor #255");
        as->Asm("sta joy2");
        as->Asm("eor joy2last");
        as->Asm("and joy2");
        as->Asm("sta joy2pressed");
        as->Asm("lda joy2");
        as->Asm("sta joy2last");
    }
    // VIC20 does not support a second joystick port
}

void Methods6502::ReadJoy1(Assembler *as)
{
    VerifyInitialized("readjoy1","InitJoy1");

     as->Asm("jsr callReadJoy1");
}

void Methods6502::ReadJoy2(Assembler *as)
{
    VerifyInitialized("readjoy2","InitJoy2");

    as->Asm("jsr callReadJoy2");
}

void Methods6502::MinMax(Assembler *as, bool isMin)
{
    QString lbl = as->NewLabel("minmax");
    QString lblCont = as->NewLabel("minmaxc");
    LoadVar(as,0);
    if (!m_node->m_params[1]->isPure())
        ErrorHandler::e.Error("Min/max : parameter 1 must be variable or constant", m_node->m_op.m_lineNumber);
    as->Asm("cmp " +m_node->m_params[1]->getValue(as));
    if (isMin)
        as->Asm("bcs "+lbl);
    else
        as->Asm("bcc "+lbl);
    as->Asm("lda "+m_node->m_params[1]->getValue(as));
    as->Asm("jmp "+lblCont);
    as->Label(lbl);
    LoadVar(as,0);

    as->Label(lblCont);

    as->PopLabel("minmax");
    as->PopLabel("minmaxc");
}

