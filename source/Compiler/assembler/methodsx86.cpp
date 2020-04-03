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
     ASTdispatcherX86* disp = dynamic_cast<ASTdispatcherX86*>(m_dispatcher);

    if (Command("waitforverticalblank"))
        WaitForVerticalBlank(as);

    if (Command("init_cga_scanlines"))
        AddInitMethod(as,"init_cga_scanlines","init_cga_scanlines.asm");

    if (Command("init_keyboard_irq"))
        AddInitMethod(as,"init_keyboard_irq","init_keyboard_irq.asm");

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
    if (Command("setpixel")) {
/*        Procedure Putpixel (X,Y : Integer; Col : Byte; P :Pointer);
        BEGIN
          If (X>0) and (X<=319) and (Y>0) and (Y<=200) then
          Asm*/
            as->Asm("Cli");
//            as->Asm("mov    ax,$a000");
            LoadVar(as,0);

            as->Asm("mov    es,ax");
            as->Asm("mov    di,0");
//            as->Asm("Les     Di,$a000");
            LoadVar(as,1);
            as->Asm("mov     bx,ax");
            LoadVar(as,2);
            as->Asm("mov     dx,ax");
            as->Asm("Add     di,bx");
            as->Asm("mov     bx, dx ");
            as->Asm("shl     dx, 8");
            as->Asm("shl     bx, 6");
            as->Asm("add     dx, bx");
            as->Asm("add     di, dx");
            as->Asm("Dec     Di");
 //           as->Asm("mov     al, [Col]");
            LoadVar(as,3);
            as->Asm("stosb");
            as->Asm("Sti ");
//                    End;
//        END;

    }
    if (m_node->m_procName.toLower().startsWith("drawsprite_cga_")) {
        LoadAddress(as,0,true);
        LoadAddress(as,1,false);
        LoadVar(as,2);
        as->Asm("mov [ds_xx],ax");
        LoadVar(as,3);
        as->Asm("mov [ds_yy],ax");

        as->Asm("call "+m_node->m_procName);

    }

    if (Command("setpixelcga")) {
        LoadVar(as,3);
//        as->Asm("mov al,ah");
        as->Asm("mov [table_cga_scanlines_x],al");
//        as->Asm("push ah");
        LoadVar(as,0);
//        as->Asm("mov ax,0");
//        as->Asm("sta ds,ax");
        as->Asm("mov  es, ax");
//        as->Asm("mov  ax, x");
        LoadVar(as,1); // Load x into ax

        as->Asm("mov  di, ax");
        as->Asm("shr  di, 1");
        as->Asm("shr  di, 1");
//        as->Asm("mov  bx, y");
        disp->PushX();
        LoadVar(as,2); // Load x into bx
        disp->PopX();
        as->Asm("shl  bx, 1");
        as->Asm("add  di, [bx + table_cga_scanlines]");
/*        as->Asm("push ax");
        as->Asm("mov ax, 80");
        as->Asm("mul bx");
        as->Asm("add  di, ax");
        as->Asm("pop ax");
*/
        as->Asm("and  ax, 3");
        as->Asm("mov  bx, table_cga_masks");
        as->Asm("xlat");
//        as->Asm("mov  ah, 1"); // Fix colour later
//        as->Asm("pop ah"); // Get colour back
        as->Asm("mov ah,[table_cga_scanlines_x]");
//        as->Asm("mov ah,$FF");

        as->Asm("and  ah, al");
        as->Asm("not  al");
        as->Asm("and  al, [es:di]");
        as->Asm("or   al, ah");
        as->Asm("stosb");

    }

    if (Command("memcpyb"))
        MemCpy(as, "b");

    if (Command("memcpyw"))
        MemCpy(as,"w");

    if (Command("setcgapalette")) {
        as->Asm("mov ah, 0Bh");
        disp->PushX();
        LoadVar(as,0);
        disp->PopX();
        as->Asm("int 10h");
    }

    if (Command("topointer")) {
        LoadVar(as,0);
        disp->PushX();
        LoadVar(as,1);
        disp->PopX();
    }

    if (Command("hi")) {
        LoadAddress(as,0,true);
        as->Asm("mov "+disp->getAx(m_node)+",ds");
    }
    if (Command("lo")) {
        LoadAddress(as,0,true);
        as->Asm("mov "+disp->getAx(m_node)+",si");
    }
    if (Command("nosound")) {
        as->Comment("No sound");
        as->Asm("in al,61h");
        as->Asm("and al,0xfc");
        as->Asm("out 61h, al");
    }
    if (Command("init_playnote"))
        AddInitMethod(as,"init_playnote","init_playnote.asm");

    if (Command("init_drawline_cga"))
        AddInitMethod(as,"init_drawline_cga","init_drawline_cga.asm");

    if (Command("drawlinecga")) {
        LoadAddress(as,0,false);
        LoadVar(as,1);
        as->Asm("shl ax,1");
        as->Asm("shl ax,1");
        as->Asm("mov [pointX1], ax");
        LoadVar(as,2);
        as->Asm("shl ax,1");
        as->Asm("shl ax,1");
        as->Asm("mov [pointY1], ax");
        LoadVar(as,3);
        as->Asm("shl ax,1");
        as->Asm("shl ax,1");
        as->Asm("mov [pointX2], ax");
        LoadVar(as,4);
        as->Asm("shl ax,1");
        as->Asm("shl ax,1");
        as->Asm("mov [pointY2], ax");
        LoadVar(as,5);
        as->Asm("mov [drawline_color], al");
        as->Asm("call CGA_DrawLine");
    }
    if (Command("return")) {
        as->Asm("ret");
    }

    if (Command("playnote")) {
        LoadVar(as,0);
        as->Asm("call init_playnote_call");
    }
    if (Command("fillw")) {
       LoadAddress(as,0,false);
       disp->PushX();
       disp->PushX();
       LoadVar(as,2);
       disp->PopX();
       disp->PopX();
       LoadVar(as,1);
       as->Asm(" rep stosw");
    }
    if (Command("ReadKey")) {
        as->Asm(" mov ah, 0h");
        as->Asm(" int 0x16");
    }
    if (Command("KeyPressed")) {
        QString lbl1 = as->NewLabel("nochar");
        QString lbl2 = as->NewLabel("cont");
        as->Asm(" Mov Dx,1");
        as->Asm(" Mov Ah,01h");
        as->Asm(" Int 16h");
        as->Asm(" Jz "+lbl1);
        as->Asm(" Jmp "+lbl2);
        as->Label(lbl1);
        as->Asm(" Mov Dx,0");
        as->Label(lbl2);
        as->Asm(" Mov Al,Dl");
    }
    if (Command("installKeyboardIRQ")) {

        as->Asm("cli");
//        QSharedPointer<NodeProcedure> addr = (QSharedPointer<NodeProcedure>)dynamic_cast<QSharedPointer<NodeProcedure>>(m_node->m_params[0]);
  //      QString name = addr->m_procedure->m_procName;

        as->Comment("Install new ISR");
        as->Asm("mov al,9h");
        as->Asm("mov ah,25h");
//        as->Asm("lea dx, ["+name+"]");
        as->Asm("lea dx, [init_keyboard_irq]");
        as->Asm("int 21h");

        as->Asm("sti");

    }

}

bool MethodsX86::Command(QString name)
{
    return m_node->m_procName.toLower() == name.toLower();

}

void MethodsX86::LoadVar(Assembler *as, int paramNo)
{
//    qDebug() << "LOADVAR FORCE WORD  TYPE "<<paramNo <<m_node->m_params[paramNo]->m_builtInFunctionParameterType  <<m_node->m_params[paramNo]->isWord(as);
    if (m_node->m_params[paramNo]->m_builtInFunctionParameterType==BuiltInFunction::INTEGER
            && !m_node->m_params[paramNo]->isWord(as)) {
        m_node->m_params[paramNo]->setForceType(TokenType::INTEGER);
  //      qDebug() << "LOADVAR FORCE WORD "<<paramNo ;
    }

    m_node->m_params[paramNo]->Accept(m_dispatcher);

}

void MethodsX86::LoadAddress(Assembler *as, int paramNo, bool isSource)
{


    QString es ="es";
    QString di = "di";
    if (isSource) {
        es = "ds";
        di = "si";
   }


        if (m_node->m_params[paramNo]->isPureVariable()) {
            if (m_node->m_params[paramNo]->isPointer(as)) {
                as->Asm("push ax");
                as->Asm("mov ax,["+m_node->m_params[paramNo]->getValue(as)+"]");
                as->Asm("mov "+es+",ax");
                as->Asm("pop ax");
                as->Asm("mov "+di+",["+m_node->m_params[paramNo]->getValue(as)+"+2]");
                return;
            }

        as->Asm("mov ax,ds");
        as->Asm("mov es,ax");

        as->Asm("lea "+di+",["+m_node->m_params[paramNo]->getValue(as)+"]");
        return;
    }
    if (m_node->m_params[paramNo]->isPureNumeric()) {
        as->Asm("mov ax, "+m_node->m_params[paramNo]->getValue(as));
        as->Asm("mov "+es+",ax");
        as->Asm("xor "+di+","+di);
        return;
    }
    ErrorHandler::e.Error("Parameter "+QString::number(paramNo)+" must be address (variable, number or pointer)");
  //  m_node->m_params[paramNo]->Accept(m_dispatcher);

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

void MethodsX86::AddInitMethod(Assembler *as, QString name, QString file)
{
    if (m_node->m_isInitialized[name])
        return;

    file = ":resources/code/x86/"+file;

    as->IncludeFile(file);
    m_node->m_isInitialized[name] = true;


}

void MethodsX86::MemCpy(Assembler *as, QString type)
{
    as->Comment("Memcpy");
    LoadVar(as,2);
    as->Asm("push ax");
    LoadAddress(as, 0,true);
    LoadAddress(as, 1,false);
    as->Asm("pop cx");

    as->Asm("rep movs"+type);

}

