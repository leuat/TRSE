#include "nodebuiltinmethod.h"
#include "nodenumber.h"
#include "nodevar.h"
#include "nodestring.h"
#include "nodeprocedure.h"

QMap<QString, bool> NodeBuiltinMethod::m_isInitialized;

QString NodeBuiltinMethod::Build(Assembler *as) {
    Node::Build(as);

    as->PushCounter();

    if (m_procName.toLower()=="writeln") {
        as->Writeln();

        m_params[0]->Build(as);
        as->EndWriteln();
    }

    if (m_procName.toLower()=="kernalinterrupt")
        as->Asm("jmp $ea81        ; return to kernal interrupt routine");

    if (m_procName.toLower()=="loop")
        as->Asm("jmp * ; loop like (¤/%");

    if (m_procName.toLower()=="call") {
        Call(as);
    }
    if (m_procName.toLower()=="fld")
        FLD(as);

    if (m_procName.toLower()=="abs") {
        Abs(as);
    }
    if (m_procName.toLower()=="nmiirq") {
        DisableNMI(as);
    }

    if (m_procName.toLower()=="setmemoryconfig") {
        SetMemoryConfig(as);
    }
    if (m_procName.toLower()=="return") {
        as->Asm("rts");
    }
    if (m_procName.toLower()=="returninterrupt") {
        as->Asm("rti");
    }

    if (m_procName.toLower()=="enablerasterirq") {
        EnableRasterIRQ(as);
    }
    if (m_procName.toLower()=="keypressed") {
        KeyPressed(as);
    }

    if (m_procName.toLower()=="clearsound") {
        Clearsound(as);
    }

    if (m_procName.toLower()=="copycharsetfromrom") {
        CopyCharsetFromRom(as);
    }

    if (m_procName.toLower() == "setspriteloc")
        SetSpriteLoc(as);

    if (m_procName.toLower() == "jammer")
        Jammer(as);

    if (m_procName.toLower()=="initsid") {
        InitSid(as);
    }
    if (m_procName.toLower()=="initzeropage") {
        InitZeroPage(as);
    }

    if (m_procName.toLower() == "decrunch")
        Decrunch(as);

    if (m_procName.toLower()=="init_decrunch") {
        InitDecrunch(as);
    }


    if (m_procName.toLower()=="startirq") {
        StartIRQ(as);
    }
    if (m_procName.toLower()=="closeirq") {
        CloseIRQ(as);
    }

    if (m_procName.toLower()=="poke")
        Poke(as);


    if (m_procName.toLower()=="copyzpdata")
        CopyZPdata(as);

    if (m_procName.toLower()=="swap")
        Swap(as);

    if (m_procName.toLower()=="clearbitmap")
        ClearBitmap(as);
    if (m_procName.toLower()=="clearscreen")
        ClearScreen(as);

    if (m_procName.toLower()=="peek")
        Peek(as);


    if (m_procName.toLower() == "copyimagecolordata")
        CopyImageColorData(as);

    if (m_procName.toLower()=="waitforraster")
        WaitForRaster(as);

    if (m_procName.toLower() =="setmulticolormode") {
        as->Comment("Multicolor mode");
        as->Asm("lda #16"); // 0000 0000
        as->Asm("ora $d016");
        as->Asm("sta $d016");
    }
    if (m_procName.toLower() =="setregularcolormode") {
        as->Comment("Regularcolor mode");
        as->Asm("lda $d016");
        as->Asm("and #%11101111");
        as->Asm("sta $d016");

    }

    if (m_procName.toLower()=="hidebordery") {
        as->Comment("Hide y border");
        as->Asm("lda $d011");
        as->Asm("and #$f7  ; change bit 4");
        as->Asm("sta $d011");

    }
    if (m_procName.toLower()=="hideborderx") {
        as->Comment("Hide x border");
        as->Asm("lda $D016");
        as->Asm("and #$f7  ; change bit 4");
        as->Asm("sta $D016");

    }
    if (m_procName.toLower() =="settextmode") {
        as->Comment("Regular text mode ");
        as->Asm("lda $D011");
        as->Asm("and #%11011111");
        as->Asm("sta $D011");
    }
    if (m_procName.toLower() =="setbank") {
        SetBank(as);
    }

    if (m_procName.toLower() =="togglebit") {
        ToggleBit(as);
    }
    if (m_procName.toLower() =="getbit") {
        GetBit(as);
    }

    if (m_procName.toLower() == "copyhalfscreen")
        CopyHalfScreen(as);

    if (m_procName.toLower() == "copyfullscreen")
        CopyFullScreen(as);

    if (m_procName.toLower() == "copyfullscreenuunrolled")
        CopyFullScreenUnrolled(as);

    if (m_procName.toLower() =="setbitmapmode") {
        as->Comment("Bitmap mode ");
        as->Asm("lda #$3b");
        as->Asm("sta $d011");
    }


    if (m_procName.toLower()=="waitnoraster")
        WaitNoRasterLines(as);

    if (m_procName.toLower()=="inc")
        IncDec(as, "inc");

    if (m_procName.toLower()=="wait")
        Wait(as);

    if (m_procName.toLower()=="dec")
        IncDec(as, "dec");


    if (m_procName.toLower()=="memcpy")
        MemCpy(as);

    if (m_procName.toLower()=="unrolledmemcpy")
        MemCpyUnroll(as);

    if (m_procName.toLower()=="memcpylarge")
        MemCpyLarge(as);


    if (m_procName.toLower()=="and")
        BitOp(as,0);

    if (m_procName.toLower()=="or")
        BitOp(as,1);

    if (m_procName.toLower()=="rand")
        Rand(as);
    if (m_procName.toLower() == "scrollx")
       ScrollX(as);

    if (m_procName.toLower() == "scrolly")
       ScrollY(as);

    if (m_procName.toLower() == "incscreenx")
            IncScreenX(as);

    if (m_procName.toLower() == "inczp")
            IncZp(as);

    if (m_procName.toLower() == "deczp")
            DecZp(as);



    if (m_procName.toLower() == "spritepos")
            SetSpritePos(as);

    if (m_procName.toLower() == "initeightbitmul")
            InitEightBitMul(as);

    if (m_procName.toLower() == "init16x8mul")
            InitMul16x8(as);

    if (m_procName.toLower() == "init16x8div")
            InitDiv16x8(as);

    if (m_procName.toLower() == "preventirq") {
        as->Asm("sei");
    }

    if (m_procName.toLower() == "enableirq") {
        as->Asm("asl $d019");
        as->Asm("cli");
    }

    if (m_procName.toLower() == "init8x8div")
            InitDiv8x8(as);

/*    if (m_procName.toLower() == "init16x8mul")
            InitMul16x8(as);
  */
    if (m_procName.toLower()=="fill")
        Fill(as);

    if (m_procName.toLower()=="initrandom")
        InitRandom(as);

    if (m_procName.toLower()=="transformcolors")
        TransformColors(as);

//    if (m_procName.toLower()=="enableinterrupts")
  //      EnableInterrupts(as);

    if (m_procName.toLower()=="initmoveto") {
        InitMoveto(as);
    }
    if (m_procName.toLower()=="initjoystick") {
        InitJoystick(as);
    }
    if (m_procName.toLower()=="joystick") {
        Joystick(as);
    }
    if (m_procName.toLower()=="playsound") {
        PlaySound(as);
    }

    if (m_procName.toLower()=="initprintstring") {
        InitPrintString(as);
    }
    if (m_procName.toLower()=="printnumber") {
        PrintNumber(as);
    }

    if (m_procName.toLower()=="printstring") {
        PrintString(as);
    }

    /*    if (m_procName.toLower()=="definesinetable")
            InitSinusTable(as);
    */

    if (m_procName.toLower()=="initsinetable")
        InitSinusTable(as);

    if (m_procName.toLower()=="moveto")
        MoveTo(as);

    if (m_procName.toLower()=="pokescreen") {
        PokeScreen(as, 0);
    }
    if (m_procName.toLower()=="pokescreencolor") {
        PokeScreenColor(as, 0);
    }
    if (m_procName.toLower()=="disableciainterrupts")
        DisableInterrupts(as);

    if (m_procName.toLower()=="rasterirq")
        RasterIRQ(as);

    if (m_procName.toLower()=="printchar") {
        as->ClearTerm();
        m_params[1]->Build(as);
        int x = as->m_term.toInt();
        as->ClearTerm();
        m_params[2]->Build(as);
        int y = as->m_term.toInt();
        as->ClearTerm();
        int pos = y*40 +x;



    }

    if (m_procName.toLower()=="print") {
        QString s = "";
        if (m_params.count()!=4)
            ErrorHandler::e.Error("Print requires 4 parameters");

        as->StartPrint();
        m_params[0]->Build(as);
        m_params[0]->Build(as);
//        if (m_params[1]!=nullptr)
  //          s+=m_params[1]->Execute(symTab, level).toString();

    }


    as->PopCounter(m_op.m_lineNumber-1);
    return "";
}

void NodeBuiltinMethod::Poke(Assembler* as)
{
    // Optimization : if parameter 1 is zero, drop the ldx / tax
    as->Comment("Poke");
    RequireAddress(m_params[0],"Poke", m_op.m_lineNumber);
    NodeNumber* num = (NodeNumber*)dynamic_cast<NodeNumber*>(m_params[1]);
    if (num!=nullptr!=0 && num->m_val==0) {
        as->Comment("Optimization: shift is zero");
        LoadVar(as,2);
        SaveVar(as,0);
        return;
    }
    // Optimization #2 : if parameter is num AND parameter 2 is num, just add
    NodeNumber* num2 = (NodeNumber*)dynamic_cast<NodeNumber*>(m_params[0]);
    if (num2!=nullptr!=0 && num!=nullptr) {
        as->Comment("Optimization: both storage and shift are constant");
        LoadVar(as,2);
        //SaveVar(as,0);
        as->Asm("sta $" + QString::number((int)(num2->m_val + num->m_val),16));
        return;
    }


    LoadVar(as,2);

    if (dynamic_cast<NodeVar*>(m_params[1])!=nullptr ||
            dynamic_cast<NodeNumber*>(m_params[1])!=nullptr)
        LoadVar(as,1, "", "ldx ");
    else {
      as->Asm("pha");
      LoadVar(as,1); // Load expression through a

      as->Asm("tax");
      as->Asm("pla");
    }


    SaveVar(as,0,"x");

}

void NodeBuiltinMethod::Peek(Assembler* as)
{
    as->Comment("Peek");
    RequireAddress(m_params[0],"Peek", m_op.m_lineNumber);

    // If pointer

    if (m_params[0]->getType(as)==TokenType::POINTER) {
        as->Term("ldy ");
        m_params[1]->Build(as);
        as->Term();
        as->Term("lda (");
        m_params[0]->Build(as);
        as->Term("),y", true);
        return;
    }

    // Optimize if numeric
    NodeNumber* num = dynamic_cast<NodeNumber*>(m_params[1]);
    if (num!=nullptr) {
        as->ClearTerm();
        as->Term("lda ");
        m_params[0]->Build(as);
        as->Term(" + " + num->HexValue());
        as->Term();
        return;

    }

    LoadVar(as, 1);
    as->Asm("tax");
    LoadVar(as,0,"x");
    //SaveVar(as,2);

}

void NodeBuiltinMethod::MemCpy(Assembler* as)
{
    //as->ClearTerm();
    NodeVar* var = (NodeVar*)dynamic_cast<NodeVar*>(m_params[0]);
    NodeNumber* num = (NodeNumber*)dynamic_cast<NodeNumber*>(m_params[0]);
    if (var==nullptr && !m_params[0]->isPureNumeric()) {
        ErrorHandler::e.Error("First parameter must be variable or number", m_op.m_lineNumber);
    }
    QString addr = "";
    if (m_params[0]->isPureNumeric())
        addr = m_params[0]->HexValue();
    if (var!=nullptr)
        addr = var->value;

    NodeNumber* num2 = (NodeNumber*)dynamic_cast<NodeNumber*>(m_params[1]);
    if (!m_params[1]->isPureNumeric()) {
        ErrorHandler::e.Error("Second parameter must be pure numeric", m_op.m_lineNumber);
    }

    RequireAddress(m_params[0], "MemCpy", m_op.m_lineNumber);
    RequireAddress(m_params[2], "MemCpy", m_op.m_lineNumber);

    QString ap1 = "";
    QString ap2 = "";
    QString bp1 = "";
    QString bp2 = "";

    if (m_params[2]->getType(as)==TokenType::POINTER) {
        bp1="(";
        bp2=")";
    }


    as->Comment("memcpy");
    QString lbl = as->NewLabel("memcpy");
    as->Asm("ldy #0");
    as->Label(lbl);
    //LoadVar(as, 0, "x");

    if (m_params[0]->getType(as)==TokenType::POINTER)
        as->Asm("lda ("+ addr +"),y");
    else
        as->Asm("lda " +addr +" +  " + num2->HexValue() + ",y");
    as->ClearTerm();


    as->Term("sta " + bp1);
    m_params[2]->Build(as);
    as->Term(bp2 + ",y", true);

    as->Asm("iny");
    as->Term("cpy ");
    m_params[3]->Build(as);
    as->Term();
    as->Asm("bne " + lbl);

    as->PopLabel("memcpy");

}

void NodeBuiltinMethod::MemCpyUnroll(Assembler* as)
{
    //as->ClearTerm();
    NodeVar* var = (NodeVar*)dynamic_cast<NodeVar*>(m_params[0]);
    NodeNumber* num = (NodeNumber*)dynamic_cast<NodeNumber*>(m_params[0]);
    if (var==nullptr && num==nullptr) {
        ErrorHandler::e.Error("First parameter must be variable or number", m_op.m_lineNumber);
    }
    QString addr = "";
    if (num!=nullptr)
        addr = num->HexValue();
    if (var!=nullptr)
        addr = var->value;

    NodeNumber* num2 = (NodeNumber*)dynamic_cast<NodeNumber*>(m_params[1]);
    if (num2==nullptr) {
        ErrorHandler::e.Error("Second parameter must be pure numeric", m_op.m_lineNumber);
    }
    NodeNumber* counter = (NodeNumber*)dynamic_cast<NodeNumber*>(m_params[3]);
    if (counter==nullptr) {
        ErrorHandler::e.Error("Third parameter must be pure numeric", m_op.m_lineNumber);
    }



    QString ap1 = "";
    QString ap2 = "";
    QString bp1 = "";
    QString bp2 = "";

    if (m_params[2]->getType(as)==TokenType::POINTER) {
        bp1="(";
        bp2=")";
    }


    as->Comment("memcpy unrolled");
    for (int i=0;i<counter->m_val;i++) {
        as->Asm("ldy #" +QString::number(i));

        if (m_params[0]->getType(as)==TokenType::POINTER)
            as->Asm("lda ("+ addr +"),y");
        else
            as->Asm("lda " +addr +" +  #" + num2->HexValue() + ",y");
        as->ClearTerm();


        as->Term("sta " + bp1);
        m_params[2]->Build(as);
        as->Term(bp2 + ",y", true);
    }

}


void NodeBuiltinMethod::Rand(Assembler* as)
{
    VerifyInitialized("rand","InitRandom");
    LoadVar(as, 0);
    as->Asm("sta lowerRandom");
    LoadVar(as, 1);
    as->Asm("sta upperRandom");
    as->Asm("jsr callRandom");
    SaveVar(as,2);

}

void NodeBuiltinMethod::InitMoveto(Assembler *as)
{
    if (m_isInitialized["moveto"])
        return;

    QString lbl = as->NewLabel("moveto");
    as->Asm("jmp " + lbl);
    as->Label("screenmemory = $fb ");
    as->Label("screen_x .byte 0 ");
    as->Label("screen_y .byte 0 ");

    as->Label("SetScreenPosition");
    //as->Asm("lda #4");
    as->Asm("sta screenmemory+1");
    as->Asm("lda #0");
    as->Asm("sta screenmemory");
    as->Asm("ldy screen_y");
    as->Asm("cpy #0");
    as->Asm("beq sydone");
    as->Label("syloop");
    as->Asm("clc");
    as->Asm("adc #40");
    as->Asm("bcc sskip");
    as->Asm("inc screenmemory+1");
    as->Label("sskip");
    as->Asm("dey");
    as->Asm("cpy #$00");
    as->Asm("bne syloop");
    as->Label("sydone");
    as->Asm("ldx screen_x");
    as->Asm("cpx #0");
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
    m_isInitialized["moveto"]=true;

}

void NodeBuiltinMethod::InitEightBitMul(Assembler *as)
{
    if (m_isInitialized["eightbitmul"])
        return;

    m_isInitialized["eightbitmul"] = true;

    QString l = as->NewLabel("multiply_eightbit");
    as->Asm("jmp " + l);
    as->Label("multiplier .byte 0");
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
}

void NodeBuiltinMethod::InitPrintString(Assembler *as)
{
    if (m_isInitialized["initprintstring"])
        return;
    m_isInitialized["initprintstring"] = true;

    as->ClearTerm();
    as->Label("print_text = $fd");
    as->Label("print_number_text .dc \"    \",0");
    as->Label("printstring");
    as->Asm("ldy #0");
    as->Label("printstringloop");
    as->Asm("lda (print_text),y");
    as->Asm("cmp #0");
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

void NodeBuiltinMethod::InitDecrunch(Assembler *as)
{
    as->Label("init_decrunch");
    as->Asm("jmp end_init_decrunch");
    as->IncludeFile(Util::path+"includes/init_decompress.asm");
    as->Label("end_init_decrunch");

}
void NodeBuiltinMethod::PrintNumber(Assembler *as)
{

    QString lbl= as->NewLabel("printnumber_call");
    QString lbl1= as->NewLabel("printnumber_l1");
    QString lbl2= as->NewLabel("printnumber_l2");
    as->Asm("ldx #0");

    as->ClearTerm();
    m_params[0]->Build(as);
    as->Term();

    as->Asm("tay");
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

void NodeBuiltinMethod::PrintString(Assembler *as)
{
    QString lbl= as->NewLabel("printstring_call");
    QString lbl2= as->NewLabel("printstring_text");
    NodeVar* var = (NodeVar*)dynamic_cast<NodeVar*>(m_params[0]);
    NodeString* str = (NodeString*)dynamic_cast<NodeString*>(m_params[0]);

    if (str==nullptr && var==nullptr)
        ErrorHandler::e.Error("PrintString must take variable or string constant");

    QString varName = "";
    if (var!=nullptr)
        varName = var->value;


    if (str!=nullptr) {
        as->Asm("jmp " + lbl);
        varName = lbl2;
        as->Label(varName + as->String(str->m_val));
        as->m_term="";
    }

    if (str!=nullptr)
        as->Label(lbl);
    as->Asm("clc");
    as->Comment("TEST");
    as->Asm("lda #<" +varName);
    as->Term("adc ");
    m_params[1]->Build(as);
    as->Term();
    as->Asm("ldy #>" +varName);
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


void NodeBuiltinMethod::MoveTo(Assembler *as)
{
    VerifyInitialized("moveto", "InitMoveto");
    LoadVar(as, 0);
    as->Asm("sta screen_x");
    LoadVar(as, 1);
    as->Asm("sta screen_y");
    LoadVar(as, 2);
    as->Asm("jsr SetScreenPosition");
}

void NodeBuiltinMethod::InitRandom(Assembler *as)
{
    if (m_isInitialized["rand"])
        return;
    m_isInitialized["rand"] = true;
    as->Asm ("; init random");
    as->Asm("LDA #$FF");
    as->Asm("STA $D40E");
    as->Asm("STA $D40F");
    as->Asm("LDA #$80");
    as->Asm("STA $D412");
    as->Asm("jmp continueRandom");
    as->DeclareVariable("upperRandom", "byte","0");
    as->DeclareVariable("lowerRandom", "byte","0");
    as->Label("callRandom");
    as->Asm("lda upperRandom");
    as->Asm("sbc lowerRandom");
    as->Asm("sta upperRandom");
    as->Label("RandomLoop");
    as->Asm("LDA $D41B ; get random value");
    as->Asm("CMP upperRandom  ; compare to");
    as->Asm("BCS RandomLoop   ; branch if value >");
    as->Asm("ADC lowerRandom");
    as->Asm("RTS");
    as->Label("continueRandom");
}

void NodeBuiltinMethod::PokeScreen(Assembler *as, int shift)
{
    LoadVar(as, 0);
    as->Term("ldy ");
    m_params[1]->Build(as);
    as->Term();
    as->Asm("sta (screenmemory),y");
}

void NodeBuiltinMethod::PokeScreenColor(Assembler *as, int hiAddress)
{
    NodeNumber* num = (NodeNumber*)dynamic_cast<NodeNumber*>(m_params[3]);
    if (num==nullptr)
        ErrorHandler::e.Error("PokeScreenColor: last parameter required to be pure constant number");



    LoadVar(as, 0);
    as->Term("ldy ");
    m_params[2]->Build(as);
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





void NodeBuiltinMethod::SetSpritePos(Assembler *as)
{

    QString lbl = as->NewLabel("spritepos");
    QString lbl2 = as->NewLabel("spriteposcontinue");



    as->Comment("Setting sprite position");


    NodeNumber* spriteNum = dynamic_cast<NodeNumber*>(m_params[2]);
    if (spriteNum!=nullptr) {
        as->Comment("isi-pisi: value is constant");
        uchar v = 1 << (uchar)spriteNum->m_val;
        as->Asm("ldx #" +QString::number((int)spriteNum->m_val*2) );
        LoadVar(as, 0);
        //as->Asm("tax");
        as->Asm("sta $D000,x");
        m_params[0]->Build(as);
        as->Term("+1",true);
        as->Asm("cmp #0");
        as->Asm("beq " + lbl);


        as->Asm("lda $D010");
        as->Asm("ora #%" + QString::number(v,2) );
        //m_params[3]->Build(as);
    //    as->Term();
        as->Asm("sta $D010");
        as->Asm("jmp "+lbl2);
        as->Label(lbl);

        as->Asm("lda $D010");
        as->Asm("and #%" + QString::number((uchar)(~v),2) );
      //  m_params[4]->Build(as);
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
        m_params[2]->Build(as);
        as->Term();
        // Shift left number of blah
        as->Asm("pha");
        as->Asm("tax");

        QString var = BitShiftX(as);

        as->Asm("pla"); // Get back the original støff

        as->Asm("asl"); // Multiply by two in the end
        as->Asm("tax"); // X is the counter

        LoadVar(as, 0);
        as->Asm("sta $D000,x");
        m_params[0]->Build(as);
        as->Term("+1",true);
        as->Asm("cmp #0");
        as->Asm("beq " + lbl);

        as->Asm("lda $D010");
        as->Asm("ora " + var );
        //m_params[3]->Build(as);
    //    as->Term();
        as->Asm("sta $D010");
        as->Asm("jmp "+lbl2);
        as->Label(lbl);

        as->Asm("lda #$FF");
        as->Asm("eor "+var);
        as->Asm("sta "+var);
        as->Asm("lda $D010");
        as->Asm("and " + var);
      //  m_params[4]->Build(as);
      //  as->Term();
        as->Asm("sta $D010");

        as->Label(lbl2);

        as->Asm("inx");
        as->Asm("txa");
        as->Asm("tay");
        LoadVar(as, 1); // In case we load from an array
        as->Asm("sta $D000,y");


    }


//    as->Asm("tay");


    as->PopLabel("spritepos");
    as->PopLabel("spriteposcontinue");
}

void NodeBuiltinMethod::Fill(Assembler *as)
{
    QString lbl = as->NewLabel("fill");
    RequireAddress(m_params[0],"Fill",m_op.m_lineNumber);
    if (m_params[0]->getType(as)==TokenType::POINTER) {

        LoadVar(as,1);
        as->Asm("ldy #0");
        as->Label(lbl);
        as->Term("sta (");
        m_params[0]->Build(as);
        as->Term("),y", true);
        as->Asm("iny");
        as->Term("cpy ");
        m_params[2]->Build(as);
        as->Term();
        as->Asm("bne "+lbl);
        as->PopLabel("fill");

        return;
    }


    LoadVar(as,1);
    as->Asm("ldx #0");
    as->Label(lbl);
    as->Term("sta ");
    m_params[0]->Build(as);
    as->Term(",x", true);
    as->Asm("inx");
    as->Term("cpx ");
    m_params[2]->Build(as);
    as->Term();
    as->Asm("bne "+lbl);
    as->PopLabel("fill");

}


void NodeBuiltinMethod::ScrollX(Assembler *as)
{
//    LoadVar(as, 0);
   // as->Asm("dec $d019");
    as->Asm("lda $d016  ");
    as->Asm("and #$F8");
    //as->Asm("clc");
    as->Term("ora ");
    m_params[0]->Build(as);
    as->Term();
    as->Asm("sta $d016");

}

void NodeBuiltinMethod::ScrollY(Assembler *as)
{
//    LoadVar(as, 0);
   // as->Asm("dec $d019");
    as->Asm("lda $d011  ");
    as->Asm("and #$F8"); // 8 = 1000
    //as->Asm("clc");
    as->Term("ora ");
    m_params[0]->Build(as);
    as->Term();
    as->Asm("sta $d011");

}

void NodeBuiltinMethod::Joystick(Assembler *as)
{
    as->Asm("jsr callJoystick");
}

void NodeBuiltinMethod::BitOp(Assembler *as, int type)
{
    as->ClearTerm();
    as->Term("lda ");
    m_params[0]->Build(as);
    as->Term();
    if (type==0)
        as->Term("and ");
    if (type==1)
        as->Term("ora ");
    m_params[1]->Build(as);
    as->Term();
    as->Term("sta ");
    m_params[0]->Build(as);
    as->Term();
}


PVar NodeBuiltinMethod::Execute(SymbolTable *symTab, uint lvl) {
    ErrorHandler::e.DebugLow("Calling Builtin",level);
    level = lvl+1;

    if (m_procName.toLower()=="writeln") {
        QString s = "";
        if (m_params[0]!=nullptr)
            s+=m_params[0]->Execute(symTab, level).toString();
        if (m_params[1]!=nullptr)
            s+=m_params[1]->Execute(symTab, level).toString();

        ErrorHandler::e.OutputText(s);
    }

    return PVar();

}

void NodeBuiltinMethod::PlaySound(Assembler *as)
{
//    LoadVar(as, 0);
    NodeNumber *num = (NodeNumber*)dynamic_cast<NodeNumber*>(m_params[0]);
    if (num==nullptr) {
        ErrorHandler::e.Error("First value must be constant - addresses chip");
    }
    QString add = " + " + num->HexValue();

    int SID = 54272;
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

void NodeBuiltinMethod::Clearsound(Assembler *as)
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
/*    as->Asm("sei");
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

void NodeBuiltinMethod::InitZeroPage(Assembler* as) {
    as->Asm("jmp initzeropage_continue");
    as->Label("zeropage1 = $02");
    as->Label("zeropage2 = $04");
    as->Label("zeropage3 = $08");
    as->Label("zeropage4 = $16");
    as->Label("zeropage5 = $22");
    as->Label("zeropage6 = $24");
    as->Label("zeropage7 = $68");

    as->Label("initzeropage_continue");
}

void NodeBuiltinMethod::Abs(Assembler *as)
{


    as->Comment("abs(x)");
    as->ClearTerm();
    m_params[0]->Build(as);
    as->Term();
    QString l = as->NewLabel("abslabel");
    as->Asm("cmp #127");
    as->Asm("bcc " + l);
    as->Asm("eor #$ff"); // negate
    as->Asm("adc #$01");
    as->Label(l);


    as->PopLabel("abslabel");

}

void NodeBuiltinMethod::CopyCharsetFromRom(Assembler *as)
{
    RequireAddress(m_params[0],"CopyCharsetFromRom",m_op.m_lineNumber);
    as->Comment("Copy charset from ROM");
    as->Asm("sei ;copy charset");
    QString lbl = as->NewLabel("charsetcopy");
    as->Asm("lda #$33 ;from rom - rom visible at d800");
    as->Asm("sta $01");

    as->Asm("ldy #$00");
    as->Label(lbl);
    as->Asm("lda $D800,y");
    if (m_params[0]->getType(as)==TokenType::POINTER) {
        as->Term("sta (");
        m_params[0]->Build(as);
        as->Term("),y", true);

    }
    else {
        as->Term("sta ");
        m_params[0]->Build(as);
        as->Term(",y", true);
    }
    //as->Asm("sta (zeropage1),y");
    as->Asm("dey");
    as->Asm("bne "+lbl);

    as->Asm("lda #$37");
    as->Asm("sta $01");
}

void NodeBuiltinMethod::IncDec(Assembler *as, QString cmd)
{
//    RequireAddress(m_params[0], "Inc/Dec", m_op.m_lineNumber);
    NodeNumber* n = dynamic_cast<NodeNumber*>(m_params[0]);
    NodeVar* v = dynamic_cast<NodeVar*>(m_params[0]);
    if (v!=nullptr && v->m_expr!=nullptr) {
        as->ClearTerm();
        v->m_expr->Build(as);
        as->Term();
        as->Asm("tax");
        as->Asm(cmd +" " + v->value + ",x");
        return;

    }
    if (n==nullptr && v==nullptr)
        ErrorHandler::e.Error("Inc / Dec requires an address / variable", m_op.m_lineNumber);

    if (n!=nullptr && n->getType(as)!=TokenType::ADDRESS)
        ErrorHandler::e.Error("Inc / Dec requires an address / variable", m_op.m_lineNumber);

    as->ClearTerm();
    as->Term(cmd + " ");
    m_params[0]->Build(as);
    as->Term();


}

void NodeBuiltinMethod::SetMemoryConfig(Assembler *as)
{
    as->Comment("Set Memory Config");
    RequireNumber(m_params[0], "SetMemoryConfig", m_op.m_lineNumber);
    RequireNumber(m_params[1], "SetMemoryConfig", m_op.m_lineNumber);
    RequireNumber(m_params[2], "SetMemoryConfig", m_op.m_lineNumber);

    NodeNumber* num1 = dynamic_cast<NodeNumber*>(m_params[0]);
    NodeNumber* num2 = dynamic_cast<NodeNumber*>(m_params[1]);
    NodeNumber* num3 = dynamic_cast<NodeNumber*>(m_params[2]);

    int n1 = num1->m_val; // Kernal
    int n2 = num2->m_val; // Basic
    int n3 = num3->m_val; // IO

    if (n1==1 && n2==0 && n3 == 0)
        n3=1; // Bit 2 must be toggled

    uchar val = n1<<2 | n2<<1 | n3 << 0;

    as->Asm("lda $01");
    as->Asm("and #%11111000");
    as->Asm("ora #%" + QString::number(val,2));
    as->Asm("sta $01");

}

void NodeBuiltinMethod::EnableRasterIRQ(Assembler* as)
{
    as->Comment("Enable raster IRQ");
    as->Asm("lda $d01a");
    as->Asm("ora #$01");
    as->Asm("sta $d01a");
    as->Asm("lda #$1B");
    as->Asm("sta $d011");
}

void NodeBuiltinMethod::StartIRQ(Assembler *as)
{
    as->Comment("StartIRQ");
    RequireNumber(m_params[0], "StartIRQ", m_op.m_lineNumber);
    NodeNumber* n = dynamic_cast<NodeNumber*>(m_params[0]);
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

void NodeBuiltinMethod::CloseIRQ(Assembler *as)
{
    as->Comment("CloseIRQ");
    as->Asm("pla");
    as->Asm("tay");
    as->Asm("pla");
    as->Asm("tax");
    as->Asm("pla");
   // as->Asm("rti");
}

void NodeBuiltinMethod::DisableNMI(Assembler *as)
{
    as->Comment("Hook NMI");

    NodeProcedure* addr = (NodeProcedure*)dynamic_cast<NodeProcedure*>(m_params[0]);
    if (addr==nullptr)
        ErrorHandler::e.Error("First parameter must be interrupt procedure!", m_op.m_lineNumber);

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

void NodeBuiltinMethod::KeyPressed(Assembler *as)
{
    as->Comment("Keypressed test");

    NodeNumber* num = dynamic_cast<NodeNumber*>(m_params[0]);
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

void NodeBuiltinMethod::Wait(Assembler *as)
{
    as->Comment("Wait");
    LoadVar(as,0,"","ldx ");

    as->Asm("dex");
    as->Asm("bne *-1");

}

QString NodeBuiltinMethod::BitShiftX(Assembler *as)
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


void NodeBuiltinMethod::InitSinusTable(Assembler *as)
{
    if (m_isInitialized["sinetab"])
        return;
    as->Asm("jmp initsin_continue");
    as->Label("sine .byte 0 ");
    as->Asm("org sine +#255");

    as->Label("value .word 0");
    as->Label("delta .word 0");
    as->Label("initsin_continue");


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
    as->Asm("adc #$10   ; this value adds up to the proper amplitude");

    as->Asm("sta delta");
    as->Asm("bcc initsin_b");
    as->Asm("inc delta+1");
    as->Label("initsin_b");

    as->Asm("inx");
    as->Asm("dey");
    as->Asm("bpl initsin_a");


    m_isInitialized["sinetab"]=true;


}


void NodeBuiltinMethod::IncZp(Assembler *as)
{
    as->m_labelStack["incscreenx"].push();
    QString lbl = as->getLabel("incscreenx");

    NodeVar* var = dynamic_cast<NodeVar*>(m_params[0]);
    if (var==nullptr) {
        ErrorHandler::e.Error("IncZp: Left-hand parameter must be zeropage pointer");
    }
    if (var->getType(as)!=TokenType::POINTER) {
        ErrorHandler::e.Error("IncZp: Left-hand parameter must be zeropage pointer");
    }


/*    as->Term("lda ");
    m_params[1]->Build(as);
    as->Term();
    as->Asm("sta screen_x");
    as->Asm("lda "+ var->value);
    as->Asm("clc");
    as->Asm("adc screen_x");
    as->Asm("bcc " + lbl);
    as->Asm("inc "+ var->value + " +1");
    as->Label(lbl);
    as->Asm("sta "+ var->value);
*/

    m_params[1]->Build(as);
    as->Term();
    as->Asm("clc");
    as->Asm("adc "+ var->value);
    as->Asm("bcc " + lbl);
    as->Asm("inc "+ var->value + " +1");
    as->Label(lbl);
    as->Asm("sta "+ var->value);


    as->m_labelStack["incscreenx"].pop();

}


void NodeBuiltinMethod::DecZp(Assembler *as)
{
    as->m_labelStack["deczp"].push();
    QString lbl = as->getLabel("deczp");

    NodeVar* var = dynamic_cast<NodeVar*>(m_params[0]);
    if (var==nullptr) {
        ErrorHandler::e.Error("DecZp: Left-hand parameter must be zeropage pointer");
    }
    if (var->getType(as)!=TokenType::POINTER) {
        ErrorHandler::e.Error("DecZp: Left-hand parameter must be zeropage pointer");
    }
    as->Comment("Decrease zeropage pointer");
    as->Asm("lda " + var->value);
    as->Asm("clc");
//    as->Asm("sec");
    as->Term("sbc ");
    m_params[1]->Build(as);
    as->Term();

    as->Asm("bcc " + lbl);
    as->Asm("dec "+ var->value + " +1");
    as->Label(lbl);
    as->Asm("sta "+ var->value);



    as->m_labelStack["deczp"].pop();

}



void NodeBuiltinMethod::IncScreenX(Assembler *as)
{
    ErrorHandler::e.Error("incscreenx is deprecated. Please use inczp(screenmemory, val) instead", m_op.m_lineNumber);
    as->m_labelStack["incscreenx"].push();
    QString lbl = as->getLabel("incscreenx");

    as->Term("lda ");
    m_params[0]->Build(as);
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


void NodeBuiltinMethod::Call(Assembler *as)
{
    NodeNumber* num= (NodeNumber*)dynamic_cast<NodeNumber*>(m_params[0]);
    if (num!=nullptr) {
        as->Term("jsr ");
        num->Build(as);
        as->Term();
        return;
    }
    ErrorHandler::e.Error("Call currently only supports constant values", m_op.m_lineNumber);
/*    NodeVar* num= (NodeVar*)dynamic_cast<NodeNumber*>(m_params[0]);
    if (num!=nullptr) {
        as->Asm("jsr $" + QString::number(num->m_val,16));
        return;
    }
*/
}

void NodeBuiltinMethod::InitSid(Assembler *as)
{
    NodeNumber* num= (NodeNumber*)dynamic_cast<NodeNumber*>(m_params[0]);
//    qDebug() << "initsid:" << num->HexValue();
    if (num!=nullptr) {
        as->Comment("initsid");
        as->Asm("lda #0");
        as->Asm("tax");
        as->Asm("tay");
        as->Term("jsr ");
        num->Build(as);
        as->Term();

        return;
    }
    ErrorHandler::e.Error("InitSid currently only supports constant values", m_op.m_lineNumber);

}


/*void NodeBuiltinMethod::InitDiv8x8(Assembler* as) {
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

void NodeBuiltinMethod::InitDiv8x8(Assembler* as) {
    as->Asm("jmp div8x8_def_end");
    as->Label("div8x8_procedure_defs");

    as->Label("div8x8_c .byte 0 ");
    as->Label("div8x8_d .byte 0 ");
    as->Label("div8x8_e .byte 0 ");

    as->Comment("Normal 8x8 bin div");
    as->Label("div8x8_procedure");

    as->Asm("lda #$00");
    as->Asm("ldx #$07");
    as->Asm("clc");
   as->Label("div8x8_loop1 rol div8x8_d");
     as->Asm("rol");
     as->Asm("cmp div8x8_c");
     as->Asm("bcc div8x8_loop2");
      as->Asm("sbc div8x8_c");
   as->Label("div8x8_loop2 dex");
    as->Asm("bpl div8x8_loop1");
    as->Asm("rol div8x8_d");

    as->Asm("lda div8x8_d");
    as->Asm("rts");


    as->Label("div8x8_def_end");

}

void NodeBuiltinMethod::InitDiv16x8(Assembler *as)
{
    as->Asm("jmp div16x8_def_end");
    as->Label("divisor = $58     ;$59 used for hi-byte");
    as->Label("dividend = $fb	  ;$fc used for hi-byte");
    as->Label("remainder = $fd	  ;$fe used for hi-byte");
    as->Label("result = dividend ;save memory by reusing divident to store the result");

    as->Label("divide16x8	lda #0	        ;preset remainder to 0");
    as->Asm("sta remainder");
    as->Asm("sta remainder+1");
    as->Asm("ldx #16	        ;repeat for each bit: ...");

    as->Label("divloop16	asl dividend	;dividend lb & hb*2, msb -> Carry");
    as->Asm("rol dividend+1");
    as->Asm("rol remainder	;remainder lb & hb * 2 + msb from carry");
    as->Asm("rol remainder+1");
    as->Asm("lda remainder");
    as->Asm("sec");
    as->Asm("sbc divisor	;substract divisor to see if it fits in");
    as->Asm("tay	        ;lb result -> Y, for we may need it later");
    as->Asm("lda remainder+1");
    as->Asm("sbc divisor+1");
    as->Asm("bcc skip16	;if carry=0 then divisor didn't fit in yet");

    as->Asm("sta remainder+1	;else save substraction result as new remainder,");
    as->Asm("sty remainder");
    as->Asm("inc result	;and INCrement result cause divisor fit in 1 times");
    as->Label("skip16	dex");
    as->Asm("bne divloop16");
    as->Asm("rts");

    as->Label("div16x8_def_end");
}



void NodeBuiltinMethod::InitMul16x8(Assembler *as)
{

//     Multiplies "num1" by "num2" and stores result in .A (low byte, also in .X) and .Y (high byte)
    as->Asm("jmp mul16x8_def_end");
    as->Label("mul16x8_procedure_defs");

    as->Label("mul16x8_num1Hi .byte 0 ");
    as->Label("mul16x8_num1 .byte 0 ");
    as->Label("mul16x8_num2 .byte 0 ");

    as->Label("mul16x8_procedure");

    as->Asm("lda #$00");
    as->Asm("tay");
    as->Asm("sty mul16x8_num1Hi  ; remove this line for 16*8=16bit multiply");
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
    as->Label("mul16x8_enterLoop  ; accumulating multiply entry point (enter with .A=lo, .Y=hi)");
    as->Asm("lsr mul16x8_num2");
    as->Asm("bcs mul16x8_doAdd");
    as->Asm("bne mul16x8_loop");
    as->Asm("rts");

    as->Label("mul16x8_def_end");
}

void NodeBuiltinMethod::DisableInterrupts(Assembler *as)
{
    as->Comment("Disable interrupts");
//    as->Asm("sei ");
/*    as->Asm("ldy #$7f    ; $7f = %01111111");
    as->Asm("sta $dc0d");
    as->Asm("sta $dd0d");
    as->Asm("lda #$01");
    as->Asm("sta $d01a");
*/

    as->Asm("ldy #$7f    ; $7f = %01111111");
    as->Asm("sty $dc0d   ; Turn off CIAs Timer interrupts");
    as->Asm("sty $dd0d   ; Turn off CIAs Timer interrupts");
    as->Asm("lda $dc0d   ; cancel all CIA-IRQs in queue/unprocessed");
    as->Asm("lda $dd0d   ; cancel all CIA-IRQs in queue/unprocessed");

    as->Asm("");
}

/*void NodeBuiltinMethod::RasterIRQ(Assembler *as)
{
    NodeProcedure* addr = (NodeProcedure*)dynamic_cast<NodeProcedure*>(m_params[0]);
    if (addr==nullptr)
        ErrorHandler::e.Error("First parameter must be interrupt procedure!", m_op.m_lineNumber);
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
void NodeBuiltinMethod::RasterIRQ(Assembler *as)
{
    NodeProcedure* addr = (NodeProcedure*)dynamic_cast<NodeProcedure*>(m_params[0]);
    if (addr==nullptr)
        ErrorHandler::e.Error("First parameter must be interrupt procedure!", m_op.m_lineNumber);

    QString name = addr->m_procedure->m_procName;

    RequireNumber(m_params[2], "RasterIRQ", m_op.m_lineNumber);
    NodeNumber* num = dynamic_cast<NodeNumber*>(m_params[2]);

    as->Comment("RasterIRQ : Hook a procedure");

    as->ClearTerm();
    m_params[1]->Build(as);
    as->Term();

    if (num->m_val==0) {
//        as->Asm("lda #$(adresstosignalIRQ)
        as->Asm("sta $d012");
        as->Asm("lda #<"+name);
        as->Asm("sta $fffe");
        as->Asm("lda #>"+name);
        as->Asm("sta $ffff");
    }
    else
    {
        as->Asm("sta $d012");
        as->Asm("lda #<"+name);
        as->Asm("sta $0314");
        as->Asm("lda #>"+name);
        as->Asm("sta $0315");

    }
/*    as->Comment("Set raster interrupt pointing to : " +name);
    as->Asm("lda #$01    ; Set Interrupt Request Mask...");
    as->Asm("sta $d01a   ; ...we want IRQ by Rasterbeam");
    as->Asm("lda #<" + name);
    as->Asm("ldx #>"+ name);
    as->Asm("sta $314    ; store in $314/$315");
    as->Asm("stx $315");
    LoadVar(as,1);
    as->Asm("sta $d012");
*/

}

void NodeBuiltinMethod::ClearScreen(Assembler *as)
{
    NodeNumber* num = (NodeNumber*)dynamic_cast<NodeNumber*>(m_params[1]);
    if (num==nullptr)
        ErrorHandler::e.Error("ClearScreen: second parameter must be constant number", m_op.m_lineNumber);


    QString lbl = as->NewLabel("clearloop");
  //  QString lbl2 = as->NewLabel("clearloop2");
    QString shift = "$" + QString::number((int)num->m_val, 16);
    as->Comment("Clear screen with offset");
    as->Asm("ldx #$00");
    LoadVar(as, 0);
    as->Label(lbl);
    as->Asm("sta $0000+"+shift+",x");
    as->Asm("sta $0100+"+shift+",x");
    as->Asm("sta $0200+"+shift+",x");
    as->Asm("sta $02e8+"+shift+",x");
//    as->Asm("sta $0300+"+shift+",x");
    as->Asm("dex");
    as->Asm("bne "+lbl);
   /* as->Asm("ldx #232");
    as->Label(lbl2);
    as->Asm("sta $02FF+"+shift+",x");
    as->Asm("dex");
    as->Asm("bne "+lbl2);
*/
    as->PopLabel("clearloop");
//    as->PopLabel("clearloop2");

}

void NodeBuiltinMethod::WaitNoRasterLines(Assembler *as)
{
   /* QString var = "";
    QString lbl = as->NewLabel("waitnoraster");
    NodeNumber* num = (NodeNumber*)dynamic_cast<NodeNumber*>(m_params[0]);
    if (num!=nullptr)
            var = "#" + num->HexValue();
    NodeVar* nvar = (NodeVar*)dynamic_cast<NodeVar*>(m_params[0]);
    if (nvar!=nullptr)
            var = nvar->value;
    if (var=="")
        ErrorHandler::e.Error("WaitNoRasterLines: parameter must be either constant or variable, not expression", m_op.m_lineNumber);
*/
    as->Comment("Wait for no of raster lines");
  //  as->Label(lbl);
    LoadVar(as, 0);
    as->Asm("clc ");
    as->Asm("adc $d012");
    //as->Asm("adc " + var);
    as->Asm("cmp $d012");
    as->Asm("bne *-3" );


//    as->PopLabel("waitnoraster");
}




void NodeBuiltinMethod::WaitForRaster(Assembler *as)
{
    as->Comment("wait for raster");
    LoadVar(as, 0,"", "ldx ");
//    as->Asm("lda $d012 ; raster line pos");
//    as->Asm("clc ; clear carry ");
 //   as->Label("lblTest");
    as->Asm("cpx $d012");
    as->Asm("bne *-3");

}

void NodeBuiltinMethod::SetSpriteLoc(Assembler *as)
{

  /*  NodeNumber* num2 = (NodeNumber*)dynamic_cast<NodeNumber*>(m_params[0]);
    if (num2==nullptr)
        ErrorHandler::e.Error("SetSpriteLoc parameter 0 must be constant");
    NodeNumber* num = (NodeNumber*)dynamic_cast<NodeNumber*>(m_params[1]);
    if (num==nullptr)
        ErrorHandler::e.Error("SetSpriteLoc parameter 1 must be constant");
*/
    NodeNumber* num3 = (NodeNumber*)dynamic_cast<NodeNumber*>(m_params[2]);
    if (num3==nullptr)
        ErrorHandler::e.Error("SetSpriteLoc parameter 2 (bank) must be constant 0-3");

    QString bank=  "$"+QString::number((int)(num3->m_val*0x4000),16);


    as->Comment("Set sprite location");

    LoadVar(as,1);
    LoadVar(as,0,"","ldx ");


//    SaveVar(as,0,"x");
    as->Asm("sta $07f8 + "+bank+",x");

/*    int newLoc = 64*num->m_val;
    QString c = "SPRITE_LOC" +QString::number((int)num2->m_val+1);
//    qDebug() << "sprite num : " << ("$" + QString::number((int)(newLoc),16)) << " with value " << c;
    as->m_symTab->m_constants[c]->m_value->m_fVal = newLoc;
    as->m_symTab->m_constants[c]->m_value->m_strVal = "$" + QString::number((int)(newLoc),16);
*/
}

void NodeBuiltinMethod::ClearBitmap(Assembler *as)
{
    NodeNumber* addr = dynamic_cast<NodeNumber*>(m_params[0]);
    NodeNumber* cnt = dynamic_cast<NodeNumber*>(m_params[1]);

    if (addr==nullptr || cnt == nullptr)
        ErrorHandler::e.Error("ClearBitmap: both parameters must be integer constants");

    QString lbl = as->NewLabel("clear");


    QString screen = addr->HexValue();


    as->Asm("; Clear bitmap method");

    as->Asm("ldy #0");
    as->Asm("tya");
    as->Label(lbl);
    for (int i=0;i<cnt->m_val;i++)
        as->Asm("sta "+screen+"+$" + QString::number(i*256,16) + " ,y");
    as->Asm("dey");
    as->Asm("bne "+lbl);

}

void NodeBuiltinMethod::Swap(Assembler *as)
{
    NodeVar* vars[2];
    for (int i=0;i<2;i++) {
        NodeVar* var = (NodeVar*)dynamic_cast<NodeVar*>(m_params[i]);
        if (var==nullptr)
            ErrorHandler::e.Error("Swap error: both parameters must be variables");
        vars[i]=var;
    }
    as->Comment("Swap variables");
//    as->Asm("lda " + vars[0]->value);
    LoadVar(as, 0);
    as->Asm("tay ");
    LoadVar(as, 1);
//    SaveVar(as, 0);
    vars[0]->StoreVariable(as);
    LoadVar(as, 0);
    as->Asm("tya");
    vars[1]->StoreVariable(as);
    //as->Asm("sta " + vars[1]->value+ ",x");


}

void NodeBuiltinMethod::MemCpyLarge(Assembler *as)
{
    /*as->Comment("Memory copy large > 255 bytes");
    NodeVar* var = (NodeVar*)dynamic_cast<NodeVar*>(m_params[0]);
    if (var==nullptr)
        ErrorHandler::e.Error("MemCpyLarge parameter 0 must be Variable location");
    NodeNumber* num = (NodeNumber*)dynamic_cast<NodeNumber*>(m_params[3]);
    if (num==nullptr)
        ErrorHandler::e.Error("MemCpyLarge parameter 4 must be constant");
    QString defs = as->NewLabel("memcpy_defs");
    QString counter = as->NewLabel("memcpy_counter");
    QString carry = as->NewLabel("memcpy_carry");
    as->Asm("lda <"+var->value);
    as->Asm("ldy >"+var->value);
    as->Asm("sta print_text+0");
    as->Asm("sty print_text+1");
    as->Asm("lda <"+var->value);
    as->Asm("ldy >"+var->value);
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
    as->Asm("lda " + var->value + " + #" + num->HexValue() + ",x");
    SaveVar(as, 2, "x");
    as->Asm("inx");
    as->Term("cpx ");
    m_params[3]->Build(as);
    as->Term();
    as->Asm("bne " + lbl);
  */
}

void NodeBuiltinMethod::SetBank(Assembler *as)
{
    as->Comment("Set bank");
    as->Asm("lda $dd00");
    as->Asm("and #$fc");
    as->Term("ora ");
    m_params[0]->Build(as);
    as->Term();
    as->Asm("sta $dd00");
}

void NodeBuiltinMethod::Decrunch(Assembler *as)
{
    NodeNumber *num = dynamic_cast<NodeNumber*>(m_params[0]);
    if (num!=nullptr) {
    as->Comment("; Decrunch number direct");
    as->Asm("lda #" + QString::number(((int)num->m_val)&0xFF));
    as->Asm("sta opbase+1");
    as->Asm("lda #" + QString::number(((int)num->m_val>>8)&0xFF));
    as->Asm("sta opbase+2");
    as->Asm("jsr exod_decrunch");
    return;
    }


    NodeVar *var = dynamic_cast<NodeVar*>(m_params[0]);
    if (var==nullptr)
            ErrorHandler::e.Error("Decrunch : parameter 0 must be an incbin block or address!");

    if (var->getType(as)!=TokenType::INCBIN)
        ErrorHandler::e.Error("Decrunch : parameter 0 must be a pointer to a IncBin block or address!");



    Symbol* varS = as->m_symTab->Lookup(var->value, m_op.m_lineNumber);
    int pos = varS->m_org + varS->m_size;

//    qDebug() <<" DECRUNCH:" << varS->m_org << varS->m_size;

        as->Comment("; Decrunch number direct");
        as->Asm("lda #" + QString::number(((int)pos)&0xFF));
        as->Asm("sta opbase+1");
        as->Asm("lda #" + QString::number(((int)pos>>8)&0xFF));
        as->Asm("sta opbase+2");
        as->Asm("jsr exod_decrunch");
}

void NodeBuiltinMethod::CopyImageColorData(Assembler *as)
{
    NodeVar* var = dynamic_cast<NodeVar*>(m_params[0]);
    if (var==nullptr)
        ErrorHandler::e.Error("CopyImageColorData : parameter 0 must be a variable!");

    NodeNumber * bank = dynamic_cast<NodeNumber*>(m_params[1]);
    if (bank==nullptr)
        ErrorHandler::e.Error("CopyImageColorData : parameter 1 must be a constant number!");


    QString addBank="0";
    if (bank->m_val==1)
        addBank="$4000";
    if (bank->m_val==2)
        addBank="$8000";
    if (bank->m_val==3)
        addBank="$c000";

    QString varName = var->value;
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

void NodeBuiltinMethod::CopyHalfScreen(Assembler *as)
{
    as->Comment("Copy half screen unrolled 500 bytes = 12.5*40");
    RequireAddress(m_params[0],"CopyHalfscreen", m_op.m_lineNumber);
    RequireAddress(m_params[1],"CopyHalfscreen", m_op.m_lineNumber);

    QString lbl = as->NewLabel("halfcopyloop");
    QString lbl2 = as->NewLabel("halfcopyloop2");

    // Copy last bit first
  /*  as->Asm("ldx #20");
    as->Label(lbl2);
    as->Term("lda ");
    m_params[0]->Build(as);
    as->Term(" + #40*#12-#1,x", true);
    as->Term("sta ");
    m_params[1]->Build(as);
    as->Term(" + #40*#12-#1,x", true);
    as->Asm("dex");
    as->Asm("bne "+lbl2);
*/
    NodeNumber * lines = dynamic_cast<NodeNumber*>(m_params[2]);
    if (lines==nullptr)
        ErrorHandler::e.Error("CopyImageColorData : parameter 3 must be a constant number!");

    NodeNumber *inverted = dynamic_cast<NodeNumber*>(m_params[3]);
    if (inverted==nullptr)
        ErrorHandler::e.Error("CopyImageColorData : parameter 4 must be a constant number!");

    NodeNumber *invertedx = dynamic_cast<NodeNumber*>(m_params[4]);
    if (invertedx==nullptr)
        ErrorHandler::e.Error("CopyImageColorData : parameter 5 must be a constant number!");

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
        m_params[0]->Build(as);
        as->Term(" + "+shift+",x", true);
        as->Term("sta ");
        m_params[1]->Build(as);
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

void NodeBuiltinMethod::CopyFullScreenUnrolled(Assembler *as)
{
    as->Comment("Copy screen unrolled 1000 bytes");
    RequireAddress(m_params[0],"CopyFullscreen", m_op.m_lineNumber);
    RequireAddress(m_params[1],"CopyFullscreen", m_op.m_lineNumber);

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
        m_params[0]->Build(as);
        as->Term(" + "+shift+",x", true);
        as->Term("sta ");
        m_params[1]->Build(as);
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

void NodeBuiltinMethod::CopyFullScreen(Assembler *as)
{

    as->Comment("Copy full screen");
    RequireAddress(m_params[0],"CopyFullscreen", m_op.m_lineNumber);
    RequireAddress(m_params[1],"CopyFullscreen", m_op.m_lineNumber);

    QString lbl = as->NewLabel("fullcopyloop");
    QString lbl2 = as->NewLabel("fullcopyloop2");

    as->Asm("ldx #0");
    as->Label(lbl);
    for (int i=0;i<3;i++) {
//        QString shift = "" +QString::number(((24-i)*40)) +" -1 ";
        QString shift = "$"+QString::number(0x100*i,16);
        as->ClearTerm();
        as->Term("lda ");
        m_params[0]->Build(as);
        as->Term(" + "+shift+",x", true);
        as->Term("sta ");
        m_params[1]->Build(as);
        as->Term(" + "+shift+",x", true);
    }
    as->Asm("dex");
    as->Asm("bne " + lbl);
    as->Asm("ldx #232");
    as->Label(lbl2);
    QString shift = "$"+QString::number(0x100*3-1,16);
    as->ClearTerm();
    as->Term("lda ");
    m_params[0]->Build(as);
    as->Term(" + "+shift+",x", true);
    as->Term("sta ");
    m_params[1]->Build(as);
    as->Term(" + "+shift+",x", true);

    as->Asm("dex");
    as->Asm("bne " + lbl2);

}

void NodeBuiltinMethod::TransformColors(Assembler *as)
{
    NodeVar* var = dynamic_cast<NodeVar*>(m_params[0]);
    if (var==nullptr)
        ErrorHandler::e.Error("Parameter 0 must be variable (array)");

    NodeNumber* num = dynamic_cast<NodeNumber*>(m_params[1]);
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
        as->Asm("lda " + var->value+ ",y");

        as->Asm("asl");
        as->Asm("asl");
        as->Asm("asl");
        as->Asm("asl");
        as->Asm("sta " + tempVar);
        as->Asm("pla");
        as->Asm("and #$0F");
        as->Asm("tay");
        as->Asm("lda " + var->value+ ",y");
        as->Asm("ora " + tempVar);
        as->Asm("sta "+num->StringValue() + shift +",x");

        as->Asm("dex");
        as->Asm("bne " + loopInner);
        as->PopLabel("transform_loop_outer");
    }
}

void NodeBuiltinMethod::ToggleBit(Assembler *as)
{
    NodeNumber* toggle = dynamic_cast<NodeNumber*>(m_params[2]);
    if (toggle==nullptr)
        ErrorHandler::e.Error("TogglesBit (for now) needs param 3 to be a number");


    NodeNumber* spriteNum = dynamic_cast<NodeNumber*>(m_params[1]);

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
    m_params[1]->Build(as);
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

    // Else, perform the full shit...
}


void NodeBuiltinMethod::GetBit(Assembler *as)
{
    NodeNumber* sprite = dynamic_cast<NodeNumber*>(m_params[1]);


    if (sprite!=nullptr) {
        QString lbl = as->NewLabel("getbit_false");
        uchar v = 1 << (uchar)sprite->m_val;
        as->Comment("Get bit");
        LoadVar(as, 0);
        as->Asm("and #%"+QString::number((uchar)v,2));
        as->Asm("beq " + lbl);
        as->Asm("lda #1");
        as->Label(lbl);
        as->Asm("eor #1");
        as->PopLabel("getbit_false");
        return;
    }

    QString lbl = as->NewLabel("getbit_false");
    QString lblD = as->NewLabel("getbit_done");
    m_params[1]->Build(as);
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


//    ErrorHandler::e.Error("NOT IMPLEMENTED (getbit with number)");

    // do full shit
}


void NodeBuiltinMethod::CopyZPdata(Assembler *as)
{
    as->Comment("Copy Zero-page data");
    QString loop = as->NewLabel("zpcopy");
    as->ClearTerm();
    as->Term("ldy ");
    m_params[1]->Build(as);
    as->Term();
    as->Label(loop);
    as->Asm("lda (print_text),y");
 //   as->Asm("lda #46");
    as->Term("sta ");
    m_params[0]->Build(as);
    as->Term(",y", true);
    as->Asm("dey");
    as->Asm("bne " + loop);

    as->PopLabel("zpcopy");


}

/*
void NodeBuiltinMethod::EnableInterrupts(Assembler* as) {
  //  as->Asm("lda $dc0d");
  //  as->Asm("lda $dd0d");
//    as->Asm("asl $d019");
    as->Asm("lda $d011   ; Bit#0 of $d011 is basically...");
    as->Asm("and #$7f    ; ...the 9th Bit for $d012");
    as->Asm("sta $d011   ; we need to make sure it is set to zero ");
    as->Asm("cli");
}
*/
void NodeBuiltinMethod::SaveVar(Assembler *as, int paramNo, QString reg, QString extra)
{
    as->ClearTerm();
    if (extra=="")
        as->Term("sta ");
    else
        as->Term(extra);
    m_params[paramNo]->Build(as);
    if (reg!="")
        reg = "," + reg;
    as->Term(reg, true);

}


void NodeBuiltinMethod::LoadVar(Assembler *as, int paramNo, QString reg, QString lda)
{
    as->ClearTerm();
    if (lda=="")
        as->Term("lda ");
    else
        as->Term(lda);
    m_params[paramNo]->Build(as);
    if (reg!="")
        reg = "," + reg;
    as->Term(reg, true);

}

void NodeBuiltinMethod::LoadVar(Assembler *as, int paramNo)
{
    LoadVar(as, paramNo, "");
}

void NodeBuiltinMethod::SaveVar(Assembler *as, int paramNo)
{
    SaveVar(as, paramNo, "");
}

void NodeBuiltinMethod::VerifyInitialized(QString method, QString initmethod)
{
    if (!m_isInitialized[method])
        ErrorHandler::e.Error("Please declare "+ initmethod+"() before using " + method+"();");

}

void NodeBuiltinMethod::Jammer(Assembler *as)
{
    as->Comment("Jammer");
    QString lbl = as->NewLabel("jammer");
    m_params[0]->Build(as);
    as->Term();
    //;sta     $7000
    as->Asm("cmp $d012");

    as->Asm("bcs "+lbl);
    as->Asm("lda #$02");
    as->Asm("sta     $0400");
    m_params[1]->Build(as);
    as->Term();
    as->Asm("sta     $d020");
    as->Asm("sta     $d021");

    as->Asm("jmp     *");
    as->Label(lbl);
    as->PopLabel("jammer");
}

void NodeBuiltinMethod::FLD(Assembler *as)
{

    NodeNumber* num = dynamic_cast<NodeNumber*>(m_params[1]);
    if (num==nullptr)
        ErrorHandler::e.Error("FLD: last parameter required to be pure constant number (0 or 1)");

    int val = num->m_val;

    QString lbl = as->NewLabel("fld");
    as->Comment("FLD effect");
    m_params[0]->Build(as);
    as->Term();
    as->Asm("tax");

    as->Label(lbl);
    as->Asm("lda $d012 ; Wait for beginning of next line");
    as->Asm("cmp $d012");
    as->Asm("beq *-3");

    as->Asm("clc ; Do one line of FLD");
    as->Asm("lda $d011");
    as->Asm("adc #1");
    as->Asm("and #7");
    if (val==0)
    as->Asm("ora #$18");
        else
    as->Asm("ora #$38");
    as->Asm("sta $d011");

    as->Asm("dex ; Decrease counter");
    as->Asm("bne " + lbl);

    as->PopLabel("fld");
}

void NodeBuiltinMethod::InitJoystick(Assembler *as)
{

    as->Asm("jmp callJoystick");
    as->Label("joystickup .byte 0");
    as->Label("joystickdown .byte 0");
    as->Label("joystickleft .byte 0");
    as->Label("joystickright .byte 0");
    as->Label("joystickbutton .byte 0");
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

    as->Asm("lda #%11111111  ; CIA#1 port A = outputs ");
    as->Asm("sta $dc03             ");

    as->Asm("lda #%00000000  ; CIA#1 port B = inputs");
    as->Asm("sta $dc02             ");


    QString port = "$dc00";

    // UP
    as->Asm("lda #%00000001 ; mask joystick up movement");
    as->Asm("bit "+port+"      ; bitwise AND with address 56320");
    as->Asm("bne joystick_down       ; zero flag is not set -> skip to down");
    as->Asm("lda #1");
    as->Asm("sta joystickup");

    as->Label("joystick_down");
    // DOWN

    as->Asm("lda #%00000010 ; mask joystick down movement");
    as->Asm("bit "+port+"      ; bitwise AND with address 56320");
    as->Asm("bne joystick_left       ; zero flag is not set -> skip to down");
    as->Asm("lda #1");
    as->Asm("sta joystickdown");
    // LEFT

    as->Label("joystick_left");

    as->Asm("lda #%00000100 ; mask joystick left movement");
    as->Asm("bit "+port+"      ; bitwise AND with address 56320");
    as->Asm("bne joystick_right       ; zero flag is not set -> skip to down");
    as->Asm("lda #1");
    as->Asm("sta joystickleft");

    // RIGHT
    as->Label("joystick_right");

    as->Asm("lda #%00001000 ; mask joystick up movement");
    as->Asm("bit "+port+"      ; bitwise AND with address 56320");
    as->Asm("bne joystick_button       ; zero flag is not set -> skip to down");
    as->Asm("lda #1");
    as->Asm("sta joystickright");

    as->Label("joystick_button");
    // BUTTON

    as->Asm("lda #%00010000 ; mask joystick up movement");
    as->Asm("bit "+port+"      ; bitwise AND with address 56320");
    as->Asm("bne callJoystick_end       ; zero flag is not set -> skip to down");
    as->Asm("lda #1");
    as->Asm("sta joystickbutton");


    as->Label("callJoystick_end");
    as->Asm("rts");

}
