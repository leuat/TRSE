#include "methodsx86.h"

MethodsX86::MethodsX86()
{

}

void MethodsX86::Assemble(Assembler *as, AbstractASTDispatcher *dispatcher)
{
    m_dispatcher = dispatcher;
/*    if (Command("Writeln")) {
        as->Writeln();

        m_node->m_params[0]->Accept(dispatcher);
        as->EndWriteln();
    }
*/
    if (Command("waitforverticalblank"))
        WaitForVerticalBlank(as);

    if (Command("setscreenmode")) {
        LoadVar(as,0);
        as->Asm("mov ah,0");
        as->Asm("int $10");
    }
    if (Command("VGAclearScreen16")) {
        ClearScreen(as,16);
    }
    if (Command("VGAclearScreen32")) {
        ClearScreen(as,32);
    }
    if (Command("scrollx")) {
        ScrollX(as);
    }
    if (Command("setpalette")) {
        as->Asm("mov dx,3c8h");
        LoadVar(as,0);
        as->Asm("out dx,al");
        as->Asm("inc dx");
        LoadVar(as,1);
        as->Asm("out dx,al");
        LoadVar(as,2);
        as->Asm("out dx,al");
        LoadVar(as,3);
        as->Asm("out dx,al");
        as->Asm("Sti");

    }

}

bool MethodsX86::Command(QString name)
{
    return m_node->m_procName.toLower() == name.toLower();

}

void MethodsX86::LoadVar(Assembler *as, int paramNo)
{
    m_node->m_params[paramNo]->Accept(m_dispatcher);

}

void MethodsX86::ClearScreen(Assembler *as, int bits)
{
    if (bits==16) {
        as->Asm("mov ax, $A000");
        as->Asm("mov es, ax");
        LoadVar(as,0);

        as->Asm("mov bh,al");
        as->Asm("mov bl,al");
        as->Asm("mov ax, bx");
        as->Asm("xor di, di");
        as->Asm("mov cx, 32000");
        as->Asm("rep stosw");

    }
    if (bits==32) {
        as->Asm("mov ax, $A000");
        as->Asm("mov es, ax");
        LoadVar(as,0);

        as->Asm("mov bh,al");
        as->Asm("mov bl,al");
        as->Asm("shl ebx,16");
        as->Asm("mov bh,al");
        as->Asm("mov bl,al");
        as->Asm("mov eax, ebx");
        as->Asm("xor di, di");
        as->Asm("mov cx, 16000");
        as->Asm("rep stosd");
    }

}

void MethodsX86::WaitForVerticalBlank(Assembler *as)
{
    QString l1 = as->NewLabel("vblank1");
    QString l2 = as->NewLabel("vblank2");

    as->Asm("mov dx, $3da");
    as->Label(l1);
    as->Asm("in al, dx");
    as->Asm("test al, 8");
    as->Asm("je "+l1);
    as->Label(l2);
    as->Asm("in al, dx");
    as->Asm("test al, 8");
    as->Asm("jne "+l2);

    as->PopLabel("vblank2");
    as->PopLabel("vblank1");
}

void MethodsX86::ScrollX(Assembler *as)
{
    LoadVar(as,0);
    as->Asm("push ax");
    as->Asm("mov dx,$3d4");
    as->Asm("mov al,$0d");
    as->Asm("out dx,al");
    as->Asm("inc dx");
    as->Asm("pop ax");
    as->Asm("out dx,al");
    as->Asm("dec dx");
    as->Asm("mov al,$00");
    as->Asm("out dx,al");
    as->Asm("inc dx");
    as->Asm("mov al,ah");
    as->Asm("out dx,al");

}
