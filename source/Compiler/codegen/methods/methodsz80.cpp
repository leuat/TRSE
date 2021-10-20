#include "methodsz80.h"

MethodsZ80::MethodsZ80()
{

}

void MethodsZ80::Assemble(Assembler *as, AbstractCodeGen *dispatcher)
{
    m_codeGen = dispatcher;
/*    if (Command("Writeln")) {
        as->Writeln();

        m_node->m_params[0]->Accept(dispatcher);
        as->EndWriteln();
    }
*/
  //   CodeGenZ80* disp = dynamic_cast<CodeGenZ80*>(m_codeGen);
    as->ClearTerm();
    if (Command("mod"))
        Mod(as);
    if (Command("fill"))
        Fill(as);
    else
    if (Command("joypad"))
        Joypad(as);
        else
    if (Command("poke")) {
        Poke(as);
    }
    if (Command("Nop")) {
        if (!m_node->m_params[0]->isPureNumeric())
            ErrorHandler::e.Error("Nop() requires a pure numeric value.",m_node->m_op.m_lineNumber);

        int val = Util::NumberFromStringHex(m_node->m_params[0]->getValue(as).remove("#"));
        for (int i=0;i<val;i++)
            as->Asm("nop");
    }
    if (Command("call")) {
        Call(as);
    }
    else
    if (Command("memcpy"))
        MemCpy(as,false);

    if (Command("Nop"))
        Nop(as);
    if (Command("returnvalue")) {
        m_node->m_params[0]->Accept(m_codeGen);
    }

    if (Command("inc"))
        IncDec(as, "inc");

    if (Command("dec"))
        IncDec(as, "dec");

    if (Command("AddBreakpoint")) {
        as->Label("trse_breakpoint_"+QString::number(as->m_currentBreakpoint++));
    }
    if (Command("disassemble")) {
        as->Label("trse_disassemble_"+QString::number(as->m_currentBreakpoint++));
    }

    if (Command("memcpycont"))
        MemCpy(as,true);
    else
        if (Command("waitforverticalblank")) {
            QString lbl = as->NewLabel("vblank");
            as->Label(lbl);
            as->Asm("ld	a,[rLY]");
            as->Asm("cp	$90");
            as->Asm("jr	nz,"+lbl);
            as->PopLabel("vblank");

        }
    else
    if (Command("waitforraster")) {
        QString lbl = as->NewLabel("vblank");
        as->Label(lbl);
        as->Asm("ld	a,[rLY]");
        if (m_node->m_params[0]->isPureVariable()) {

            as->Asm("ld c,a");
            as->Asm("ld a,["+m_node->m_params[0]->getValue(as)+"]");
            as->Asm("cp	c");
        }
        else
            as->Asm("cp	" +m_node->m_params[0]->getValue(as));
        as->Asm("jr	nz,"+lbl);
        as->PopLabel("vblank");

    }
    else
    if (Command("memcpyonhblank"))
        MemCpyOnHBLank(as,"internal_copy_hblank8",8);
    if (Command("memcpyonhblank4"))
        MemCpyOnHBLank(as,"internal_copy_hblank4",4);
    else
        if (Command("memcpyonhblankexp"))
            MemCpyOnHBLank(as,"internal_copy_hblank_exp",12);
        else
    if (Command("waitnoraster")) {
        QString lbl = as->NewLabel("vblank");
        as->Asm("ld	a,[rLY]");
        as->Asm("add " +m_node->m_params[0]->getValue(as));
        as->Asm("ld	b,a");
        as->Label(lbl);
        as->Asm("ld	a,[rLY]");
        as->Asm("cp	b");

        as->Asm("jr	nz,"+lbl);
        as->PopLabel("vblank");

    }
    else if (Command("rasterirq")) {
        if (Syntax::s.m_currentSystem->m_system == AbstractSystem::AMSTRADCPC) {
            QSharedPointer<NodeProcedure> proc = qSharedPointerDynamicCast<NodeProcedure>(m_node->m_params[0]);

            as->Asm("di");
            as->Asm("ld a,&c3	;; Z80 JP instruction");
            as->Asm("ld hl,"+proc->m_procedure->m_procName);
            as->Asm("ld ($0038),a	;; write JP instruction");
            as->Asm("ld ($0039),hl	;; write address");
            as->Asm("ei");
        }
    }
    else if (Command("enablevblank")) {
        as->Asm("ld	a,[rIE]");
        as->Asm("or	a,IEF_VBLANK");
        as->Asm("ld	[rIE],a");
        as->Asm("ei");

    }
    else if (Command("enabletimer")) {
        as->Asm("ld	a,[rIE]");
        as->Asm("or	a,IEF_TIMER");
        as->Asm("ld	[rIE],a");
        as->Asm("ei");

    }
    else if (Command("loop")) {
        QString lbl = as->NewLabel("end");
        as->Label(lbl);
        if (Syntax::s.m_currentSystem->m_system==AbstractSystem::GAMEBOY)
            as->Asm("halt");
        as->Asm(";nop");
        as->Asm("jr "+lbl);
        as->PopLabel("end");
    }
    else if (Command("halt")) {
        as->Asm("halt");
    }
    else if (Command("setsprite"))
      SetSprite(as,0);
    else if (Command("initsprite"))
      SetSprite(as,1);
    else if (Command("initspritefromdata"))
      InitSpriteFromData(as,0);
    else if (Command("updatemusic"))
        as->Asm("call gbt_update ");
    else if (Command("loadmusic"))
        LoadSong(as);
//    else if (Command("return"))
  //      as->Asm("ret");
    else if(Command("initvblank"))
        InitDMA(as);
    else if(Command("inittimer"))
        InitTimer(as);
    else if (Command("push")) {
        as->Asm("push af");
        as->Asm("push bc");
        as->Asm("push de");
        as->Asm("push hl");

    }
    else if (Command("pop")) {
        as->Asm("pop hl");
        as->Asm("pop de");
        as->Asm("pop bc");
        as->Asm("pop af");

    }
    else if (Command("wait")) {
        as->Comment("Wait");
        QString lbl = as->NewLabel("wait");
        LoadVar(as,0);
        as->Label(lbl);
        as->Asm("sub 1");
        as->Asm("jr nz,"+lbl);
        as->PopLabel("wait");

    }
    else
    if (Command("hi")) {
        HiLo(as, true);
    }
    else
    if (Command("lo")) {
        HiLo(as, false);
    }
    else if (Command("waitforhblank"))
        WaitForHBLank(as);

/*   if (Command("togglebit")) {
       ToggleBit(as);
   }*/
}


bool MethodsZ80::Command(QString name)
{
    return m_node->m_procName.toLower() == name.toLower();
}

void MethodsZ80::LoadVar(Assembler *as, int paramNo, QString s)
{
    //    qDebug() << "LOADVAR FORCE WORD  TYPE "<<paramNo <<m_node->m_params[paramNo]->m_builtInFunctionParameterType  <<m_node->m_params[paramNo]->isWord(as);
        if (m_node->m_params[paramNo]->m_builtInFunctionParameterType==BuiltInFunction::INTEGER
                && !m_node->m_params[paramNo]->isWord(as)) {
            m_node->m_params[paramNo]->setForceType(TokenType::INTEGER);
      //      qDebug() << "LOADVAR FORCE WORD "<<paramNo ;
        }

//   qDebug() << "ISN " << qSharedPointerDynamicCast<NodeNumber>(m_node->m_params[paramNo]);
   m_codeGen->m_useNext = s;
   m_node->m_params[paramNo]->Accept(m_codeGen);


}

void MethodsZ80::LoadAddress(Assembler *as, int paramNo, QString reg)
{
    m_codeGen->m_useNext = reg;
    m_codeGen->LoadAddress(m_node->m_params[paramNo]);


/*    if (m_node->m_params[paramNo]->isPure()) {
        QString v= m_node->m_params[paramNo]->getValue(as);
        if (m_node->m_params[paramNo]->isPointer(as)) {
            as->Asm("ld a,["+v+"]");
            as->Asm("ld "+reg[0]+",a");
            as->Asm("ld a,["+v+"+1]");
            as->Asm("ld "+reg[1]+",a");
            return;
        }


        as->Asm("ld "+reg+", "+v);
        return;
    }
*/
}

void MethodsZ80::Fill(Assembler *as)
{
    as->Comment("Loading param 2 ");
    LoadVar(as,2,"bc");
    if (!m_node->m_params[1]->isPure()) {
        as->Asm("push bc");
    }
    as->Comment("Loading param 0 ");
    LoadAddress(as, 0);

    as->Comment("Loading param 1 ");
    LoadVar(as,1);
    if (!m_node->m_params[1]->isPure()) {
        as->Asm("pop bc");
    }

    QString lblFill = "."+as->NewLabel("fill");
    QString lblSkip = "."+as->NewLabel("skip");

    as->Asm("inc b");
    as->Asm("inc c");
    as->Asm("jr "+lblSkip);
    as->Label(lblFill);
    if (Syntax::s.m_currentSystem->m_processor==AbstractSystem::GBZ80)
        as->Asm("ld [hl+],a");
    else {
        as->Asm("ld [hl],a");
        as->Asm("inc hl");
    }
    as->Label(lblSkip);
    as->Asm("dec c");
    as->Asm("jr nz,"+lblFill);
    as->Asm("dec b");
    as->Asm("jr nz,"+lblFill);

    as->PopLabel("fill");
    as->PopLabel("skip");
//    ld	hl,_HRAM			; clear hram
//	ld	c,$80				; a = 0, b = 0 here, so let's save a byte and 4 cycles
   // (ld c,$80 - 2/8 vs ld bc,$80 - 3/12)

/*finit_fill:
;-------------------------------------------------------------------------------
; a - byte to fill with
; hl - destination address
; bc - size of area to fill

    inc	b
    inc	c
    jr	.skip
.fill
    ld	[hl+],a
.skip
    dec	c
    jr	nz,.fill
    dec	b
    jr	nz,.fill
    ret
  */
}

void MethodsZ80::Mod(Assembler *as)
{
    //LoadVar(as,0,"bc");
//    m_node->m_params[0]->setForceType(TokenType::INTEGER);
    m_node->m_params[0]->Accept(m_codeGen);
    if (m_node->m_params[0]->isWord(as)) {
        as->Asm("ld b,h");
        as->Asm("ld c,l");
    }
    else {
        as->Asm("ld b,0");
        as->Asm("ld c,a");
    }

    if (!m_node->m_params[1]->isPure()) {
        as->Asm("push bc");
    }
    m_node->m_params[1]->setForceType(TokenType::INTEGER);
    LoadVar(as,1,"de");
//    m_codeGen->ExDeHl();

    if (!m_node->m_params[1]->isPure()) {
        as->Asm("pop bc");
    }

    as->Asm("call _Mod_16");
    as->Asm("ld a,l");

}

void MethodsZ80::MemCpy(Assembler *as, bool isCont)
{
/*    ; hl - source address
    ; de - destination
    ; bc - size*/

    if (Syntax::s.m_currentSystem->m_system!=AbstractSystem::GAMEBOY) {

        LoadAddress(as,1,"de");
        as->Term();
        LoadAddress(as,0,"hl");
        as->Term();
        LoadVar(as,2,"bc");
        as->Asm("call z80_copy_mem");
        return;
    }


    QString lblCopy = "."+as->NewLabel("copy");
    QString lblSkip = "."+as->NewLabel("skip");
    as->ClearTerm();
    int ii=0;
    if (!isCont) {
        LoadAddress(as,1,"de");
        as->Term();
        LoadAddress(as,0,"hl");
        as->Term();
        ii=2;
    }


    LoadVar(as,ii,"bc");
    as->Asm("inc b");
    as->Asm("inc c");
    as->Asm("jr "+lblSkip);
    as->Label(lblCopy);
    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::GAMEBOY)
        as->Asm("ld	a,[hl+]");
    else {
        as->Asm("ld	a,[hl]");
        as->Asm("inc hl");

    }
    as->Asm("ld	[de],a");
    as->Asm("inc de");
    as->Label(lblSkip);
    as->Asm("dec c");
    as->Asm("jr	nz,"+lblCopy);
    as->Asm("dec b");
    as->Asm("jr	nz,"+lblCopy);
}

void MethodsZ80::Poke(Assembler *as)
{
    if (m_node->m_params[0]->isPureNumeric() &&
            m_node->m_params[1]->isPureNumeric() &&
            m_node->m_params[1]->getValueAsInt(as)==0) {

        LoadVar(as,2);
        as->Asm("ld ["+m_node->m_params[0]->getValue(as)+"],a");
        return;
    }


    LoadAddress(as,0);
//        as->Asm("ld a,"+m_node->m_params[1]->getValue(as));
    as->Term();
    if (m_node->m_params[1]->isPureNumeric() && m_node->m_params[1]->getValueAsInt(as)==0) {
        LoadVar(as,2);
        as->Asm("ld [hl],a");
        return;
    }
    LoadVar(as,1);
    as->Term();
    as->Asm("ld e,a");
    as->Asm("ld d,0");
    as->Asm("add hl,de");
    LoadVar(as,2);
    as->Asm("ld [hl],a");

}

void MethodsZ80::Call(Assembler *as)
{
 //   LoadVar(as,0);
    if (m_node->m_params[0]->isPointer(as)) {
        if (!m_node->m_params[0]->isPure())
            ErrorHandler::e.Error("Parameter 1 must be a pure pointer or number",m_node->m_op.m_lineNumber);

        QString lblCall = "."+as->NewLabel("call");
        QString lblContinue = "."+as->NewLabel("continue");
        as->Asm("call "+lblCall);
        as->Asm("jp "+lblContinue);
        as->Label(lblCall);
        as->Asm("ld hl,["+m_node->m_params[0]->getValue(as)+"]");
        as->Asm("jp [hl]");
        as->Label(lblContinue);
        return;
    }
    as->Asm("call "+m_node->m_params[0]->getValue(as));

}


void MethodsZ80::SetSprite(Assembler *as, int type)
{
//    LoadAddress(as,0);
    if (!m_node->m_params[2]->isPure())
        ErrorHandler::e.Error("Parameter 1 (x) must be pure constant or variable");
    if (!m_node->m_params[1]->isPure())
        ErrorHandler::e.Error("Parameter 2 (y) must be pure constant or variable");

    if (!m_node->m_params[3]->isPureNumeric())
        ErrorHandler::e.Error("Parameter 3 (width) must be constant value!");
    if (!m_node->m_params[4]->isPureNumeric())
        ErrorHandler::e.Error("Parameter 4 (height) must be constant value");


    int x = m_node->m_params[3]->getValueAsInt(as);
    int y = m_node->m_params[4]->getValueAsInt(as);




    QString addr = m_node->m_params[0]->getValue(as);

    as->Comment("SetSprite");
    if (m_node->m_params[0]->isPointer(as)) {
        // Completely different shit
        as->Comment("SetSprite is a pointer");

        type +=2;
        as->Asm("ld a,["+addr+"]");
        as->Asm("ld h,a");
        as->Asm("ld a,["+addr+"+1]");
        as->Asm("ld l,a");
        as->Asm("ld de,2");
        if (type==3)
        {
            as->Asm("add hl,de");
//            as->Asm("ld de,"+Util::num())
            as->Asm("ld de,4");
            if (m_node->m_params[1]->isPureVariable()) {
                as->Asm("ld a, ["+m_node->m_params[1]->getValue(as)+"]");
                as->Asm("ld b,a");
            }

        }
//        return;
    }
    int mcur = 0;
    for (int j=0;j<y;j++) {
        for (int i=0;i<x;i++) {
            int cnt = 4* (x*j+i);
            if (type==0) { // Set sprite
                LoadVar(as,2);
                if (j!=0)
                    as->Asm("add "+QString::number(j*8));
                as->Asm("ld ["+addr + "+"+QString::number(cnt)+" +0 ],a");
                LoadVar(as,1);
                if (i!=0)
                    as->Asm("add "+QString::number(i*8));
                as->Asm("ld ["+addr + "+"+QString::number(cnt)+" +1 ],a");
            }

            if (type==2) { // Set sprite from POINTEr
                LoadVar(as,2);
                if (j!=0)
                    as->Asm("add "+QString::number(j*8));
                as->Asm("ld [hl+],a");
                LoadVar(as,1);
                if (i!=0)
                    as->Asm("add "+QString::number(i*8));
                as->Asm("ld [hl+],a");
                as->Asm("add hl,de");
            }



            if (type==1) { // Init
                if (m_node->m_params[1]->isPureNumeric()) {
                    int dx = m_node->m_params[1]->getValueAsInt(as);
                    int dy = m_node->m_params[2]->getValueAsInt(as);
                    int num = (dx+i)+ (dy+j)*16;
                    as->Asm("ld a,"+QString::number(num));
                }
                as->Asm("ld ["+addr + "+"+QString::number(cnt)+" +2 ],a");
            }
            if (type==3) { // Init from POINTER
                if (m_node->m_params[1]->isPureNumeric()) {
                    int dx = m_node->m_params[1]->getValueAsInt(as);
                    int dy = m_node->m_params[2]->getValueAsInt(as);
                    int num = (dx+i)+ (dy+j)*16;
                    as->Asm("ld a,"+QString::number(num));
                }
                else {
                    as->Asm("ld a,"+QString::number(mcur));
                    as->Asm("add a,b");

                }
                as->Asm("ld [hl] ,a");
                as->Asm("add hl,de");
            }

            mcur++;
        }


    }
}

void MethodsZ80::InitSpriteFromData(Assembler *as, int type)
{
//    LoadAddress(as,0);
    if (!m_node->m_params[2]->isPure())
        ErrorHandler::e.Error("Parameter 2 (start) must be constant/variable", m_node->m_op.m_lineNumber);
    if (!m_node->m_params[3]->isPureNumeric())
        ErrorHandler::e.Error("Parameter 3 (length) must be pure constant", m_node->m_op.m_lineNumber);

    //    LoadAddress(as,0);
    if (!m_node->m_params[4]->isPureNumeric())
        ErrorHandler::e.Error("Parameter 4 (width) must be pure constant", m_node->m_op.m_lineNumber);
    if (!m_node->m_params[5]->isPureNumeric())
        ErrorHandler::e.Error("Parameter 5 (flip x) must be pure constant", m_node->m_op.m_lineNumber);

//    int start = m_node->m_params[2]->getValueAsInt(as);
    int length = m_node->m_params[3]->getValueAsInt(as);

    int width = m_node->m_params[4]->getValueAsInt(as);

    int attr = m_node->m_params[5]->getValueAsInt(as);




    QString addr = m_node->m_params[0]->getValue(as);
    as->Comment("InitSpriteFromData");
    LoadAddress(as,1);
    as->Asm("ld d,0");
    m_node->m_params[2]->Accept(m_codeGen);
//    as->Asm("ld a,["+m_node->m_params[2]->getValue(as)+"]");
    as->Asm("ld e,a");
    as->Asm("add hl,de");
    int x = 0;
    int y =0;
    bool flipx = (attr & 0b00100000)==0b00100000;
    bool flipy = (attr & 0b01000000)==0b01000000;
    for (int i=0;i<length;i++) {
//        int cnt = 4* (i-start);
        int dx = x;
        int dy = y;
        if (flipx) dx = width-1-dx;
        if (flipy) dy = (length)/width-1-dy;
        int cnt = 4* (dx+dy*width);

        as->Asm("ld a,[hl+]");
        as->Asm("ld ["+addr + "+"+QString::number(cnt)+" +2 ],a");
        as->Asm("ld a,"+Util::numToHex(attr));
        as->Asm("ld ["+addr + "+"+QString::number(cnt)+" +3 ],a");
        x++;
        if (x==width) {
            x=0;
            y++;
        }
    }
}

void MethodsZ80::LoadSong(Assembler *as)
{
    QString song = m_node->m_params[0]->getValue(as);
    as->Asm("ld de," +song +"_data");
    as->Asm("ld bc,BANK(" +song +"_data)");
    LoadVar(as,1);
    as->Asm("call    gbt_play ; Play song");

    auto app = QSharedPointer<Appendix>(new Appendix);


    QString fn1 = as->m_projectDir + song+".inc";
    QString fn2 = as->m_projectDir+ song+".asm";
    QString fn="";
    if (QFile::exists(fn1))
        fn=fn1;
    if (QFile::exists(fn2))
        fn=fn2;
    if (fn=="")
        ErrorHandler::e.Error("Required mod2gbt song assember file: '"+fn+"' not found", m_node->m_op.m_lineNumber);

    app->m_source<<	"INCLUDE \""+fn+"\"";
    app->m_pos = "$10000";
    as->m_appendix.append(app);
}

void MethodsZ80::InitDMA(Assembler *as)
{
    QSharedPointer<NodeProcedure> addr = qSharedPointerDynamicCast<NodeProcedure>(m_node->m_params[0]);
    if (addr==nullptr)
        ErrorHandler::e.Error("ProcedureToPointer parameter must be a procedure.", m_node->m_op.m_lineNumber);

    QString lblCopy = "."+as->NewLabel("copy");

    as->Asm("ld de, $FF80");
    as->Asm("ld hl, default_DMAData");
    as->Asm("ld c,13");
    as->Label(lblCopy);
    as->Asm("ld	a,[hl+]");
    as->Asm("ld	[de],a");
    as->Asm("inc de");
    as->Asm("dec c");
    as->Asm("jr	nz,"+lblCopy);
    as->PopLabel("copy");

    as->Asm("ld hl,"+addr->m_procedure->m_procName);
    as->Asm("ld a,l");
    as->Asm("ld [$FF80+$A],a");
    as->Asm("ld a,h");
    as->Asm("ld [$FF80+$B],a");
}

void MethodsZ80::InitTimer(Assembler *as)
{
    QSharedPointer<NodeProcedure> addr = qSharedPointerDynamicCast<NodeProcedure>(m_node->m_params[0]);
    if (addr==nullptr)
        ErrorHandler::e.Error("ProcedureToPointer parameter must be a procedure.", m_node->m_op.m_lineNumber);

    QString lblCopy = "."+as->NewLabel("copy");

    as->Asm("ld de, $FF90");
    as->Asm("ld hl, default_DMATimer");
    as->Asm("ld c,4");
    as->Label(lblCopy);
    as->Asm("ld	a,[hl+]");
    as->Asm("ld	[de],a");
    as->Asm("inc de");
    as->Asm("dec c");
    as->Asm("jr	nz,"+lblCopy);
    as->PopLabel("copy");

    as->Asm("ld hl,"+addr->m_procedure->m_procName);
    as->Asm("ld a,l");
    as->Asm("ld [$FF90+$1],a");
    as->Asm("ld a,h");
    as->Asm("ld [$FF90+$2],a");
}

void MethodsZ80::MemCpyOnHBLank(Assembler *as, QString jlbl, int div)
{
    LoadAddress(as,0,"de");
    LoadAddress(as,1,"hl");
    //if (!m_node->m_params[2]->isPureNumeric())
    //    ErrorHandler::e.Error("Parameter 3 must be pure constant and a divisible by 8", m_node->m_op.m_lineNumber);
    as->Comment("HBLANK 8-byte copy per scanline");
    int cnt = m_node->m_params[2]->getValueAsInt(as);
    QString lbl = as->NewLabel("memcpyhblank");
    if (m_node->m_params[2]->isPureNumeric())
        as->Asm("ld a, "+Util::numToHex(ceil(cnt/(float)div)));
    else {
        as->Comment("hblank non-const");
        m_node->m_params[2]->Accept(m_codeGen);
//        LoadVar(as,2);
        as->Asm("rrca");
        as->Asm("rrca");
        as->Asm("rrca");

    }
    as->Label(lbl);
    as->Asm("push af");
    as->Asm("call "+jlbl);
    as->Asm("pop af");
    as->Asm("dec a");
    as->Asm("cp 0");
    as->Asm("jr nz, "+lbl);

    as->PopLabel("memcpyhblank");
//    for j:=0 to ($80/8) do

}




void MethodsZ80::WaitForHBLank(Assembler *as)
{
    as->Comment("Wait for HBLank");
    QString lbl = as->NewLabel("hblank");
    as->Label(lbl);
    as->Asm("ld a,[$FF41]");
    as->Asm("and 3");
    as->Asm("cp 0");
    as->Asm("jr nz, "+lbl);
    as->PopLabel("hblank");
}

void MethodsZ80::HiLo(Assembler *as, bool isHi)
{
    if (m_node->m_params[0]->isPureVariable())
    {
        if (m_node->m_params[0]->isWord(as)) {
            if (isHi)
                as->Asm("ld a,["+m_node->m_params[0]->getValue(as)+"]");
            else
                as->Asm("ld a,["+m_node->m_params[0]->getValue(as)+"+1]");
            return;
        }
    }
    ErrorHandler::e.Error("Hi / lo uknown parameter type", m_node->m_op.m_lineNumber);

}

void MethodsZ80::Joypad(Assembler *as)
{
    if (!m_node->m_params[0]->isPure() || !m_node->m_params[1]->isPure())
        ErrorHandler::e.Error("Parameter 1 and 2 must be addresses", m_node->m_op.m_lineNumber);

    as->Asm("call read_keys");
    as->Asm("ld a,b");
    as->Asm("ld ["+m_node->m_params[0]->getValue(as)+"],a");
    as->Asm("ld a,c");
    as->Asm("ld ["+m_node->m_params[1]->getValue(as)+"],a");

}

void MethodsZ80::ToggleBit(Assembler *as)
{
     if (!m_node->m_params[2]->isPureNumeric())
         ErrorHandler::e.Error("Parameter 3 must be pure numeric", m_node->m_op.m_lineNumber);
}
