#include "methods6502ok64.h"

Methods6502OK64::Methods6502OK64()
{

}

void Methods6502OK64::Assemble(Assembler *as, AbstractASTDispatcher *dispenser)
{
    Methods6502::Assemble(as,dispenser);


    if (Command("ResetFileList"))
        CallOKVC(as,0,9);
    else

    if (Command("ReadNextFile"))
        CallOKVC(as,0,10);
    else

    if (Command("fmul"))
        CallOKVC(as,2,15);
    else

    if (Command("DrawPixel")) {
        CallOKVC(as,3,1);
    }
    else
    if (Command("SetDefaultPalette")) {
        CallOKVC(as,0,14);
    }
    else
    if (Command("DrawRect")) {
        CallOKVC(as,5,8);
    }
    else
    if (Command("drawCircleFilled")) {
        CallOKVC(as,4,4);
    }
    else
    if (Command("drawPoly")) {
        CallOKVC(as,7,12);
    }
    else

    if (Command("DrawLine")) {
        as->Comment("Draw Line");
        LoadVar(as,0);
        as->Asm("sta $FF00");
        LoadVar(as,1);
        as->Asm("sta $FF01");
        LoadVar(as,2);
        as->Asm("sta $FF04");
        LoadVar(as,3);
        as->Asm("sta $FF05");
        LoadVar(as,4);
        as->Asm("sta $FF02");
        as->Asm("lda #2");
        as->Asm("sta $FF10"); // Initialize pixel drawing
    }
    else
    if (Command("ClearScreen")) {
        if (Syntax::s.m_currentSystem->m_system==AbstractSystem::OK64) {
            as->Comment("ClearScreen");
            LoadVar(as,0);
            as->Asm("sta $FF02");
            as->Asm("lda #3");
            as->Asm("sta $FF10"); // Initialize pixel drawing
        }
    }
    else
    if (Command("WaitForVSync")) {
        as->Asm("lda #1");
        as->Asm("sta $FFEF");
    }
    else
    if (Command("toColor"))
        toColor(as);
    else
    if (Command("setPalette")) {
        CallOKVC(as,4,5);
    }
    else
    if (Command("loadfile")) {
        CallOKVC(as,0,11);
    }
    else
    if (Command("memcpyokvc")) {
        CallOKVC(as,8,13);
    }
    else
    if (Command("blit")) {
        CallOKVC(as,6,6);
    }
    else
    if (Command("printchar")) {
        CallOKVC(as,4,7);
    }

    /*
     *
     *
     *
     *
     *
     *
     *
     * */


}

void Methods6502OK64::WaitForVerticalBlank(Assembler *as)
{
    QString l1 = as->NewLabel("verticalblank1");
    QString l2 = as->NewLabel("verticalblank2");

    as->Label(l1);
    as->Asm("bit $D011");
    as->Asm("bpl "+l1);
    as->Label(l2);
    as->Asm("bit $D011");
    as->Asm("bmi "+l2);


    as->PopLabel("verticalblank1");
    as->PopLabel("verticalblank2");
}

void Methods6502OK64::CallOKVC(Assembler *as, int noParams, uchar val)
{
    for (uchar i=0;i<noParams;i++) {
        LoadVar(as,i);
        as->Asm("sta "+Util::numToHex(0xFF00+i));
    }
    as->Asm("lda #"+QString::number(val));
    as->Asm("sta $FF10"); // Start

}

void Methods6502OK64::toColor(Assembler *as)
{
    /*
        QString zp = as->m_internalZP[0];
        LoadVar(as,0);
        as->Asm("and #3");
        as->Asm("sta "+zp);
        LoadVar(as,1);
        as->Asm("and #3");
        as->Asm("asl");
        as->Asm("asl");
        as->Asm("ora "+zp);
        as->Asm("sta "+zp);
        LoadVar(as,2);
        as->Asm("and #3");
        as->Asm("asl");
        as->Asm("asl");
        as->Asm("asl");
        as->Asm("asl");
        as->Asm("ora "+zp);
        */
    QString zp = as->m_internalZP[0];
    LoadVar(as,0);
    as->Asm("and #7");
    as->Asm("sta "+zp);
    LoadVar(as,1);
    as->Asm("and #3");
    as->Asm("asl");
    as->Asm("asl");
    as->Asm("asl");
    as->Asm("ora "+zp);
    as->Asm("sta "+zp);
    LoadVar(as,2);
    as->Asm("and #7");
    as->Asm("asl");
    as->Asm("asl");
    as->Asm("asl");
    as->Asm("asl");
    as->Asm("asl");
    as->Asm("ora "+zp);

}
