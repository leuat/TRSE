#ifndef CHIP8EMU_H
#define CHIP8EMU_H

#include <QByteArray>
#include <QPixmap>
class Chip8Emu
{
public:
    Chip8Emu();

    void loadROM(QString file);

    void step();

    void keydown(unsigned char k);
    void keyup(unsigned char k);
    void init();
    void unknownOpcode();
    bool needsDisplay();
    void setNeedsDisplay(bool needsDisplay);
    QImage m_screen;
    QPixmap getScreen(int w, int h);

    uchar gfx[64][32]; // we expose the graphics buffer so the Chip8View can read from it to render to the screen.



    // Used to store the current opcode (2 bytes)
    unsigned short opcode;

    // Sytem Memory (4 KB)
    unsigned char memory[4096];

    // Registers
    unsigned char	V[16];	// 8-bit registers
    #define VF		V[0xF]	// VF (register 16) is a special register reserved for carry operations. So we define a convience accessor here.

    // Delay Timer register
    unsigned char delay_timer;

    // Sound Timer register
    unsigned char sound_timer;

    // Index Register (sometimes called Address register) is 2 bytes wide and is used with several opcodes that involve memory operations.
    unsigned short I;

    // Program Coutner (PC) - 2 bytes wide, so we can point at any valid memory location (0-4095) in the 4KB of memory. Technically we only need 12-bits for this.
    unsigned short pc;

    // The Stack, and Stack Pointer (sp)
    unsigned short stack[16];
    unsigned short sp;

    // Keypad - used as input from a HEX keypad
    unsigned char key[16];

    unsigned char fontset[80] = {

        0b11110000, // 0
        0b10010000,
        0b10010000,
        0b10010000,
        0b11110000,

        0b00100000, // 1
        0b01100000,
        0b00100000,
        0b00100000,
        0b01110000,

        0b11110000, // 2
        0b00010000,
        0b11110000,
        0b10000000,
        0b11110000,

        0b11110000, // 3
        0b00010000,
        0b11110000,
        0b00010000,
        0b11110000,

        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80, // F
    };


    // Flag used to let the renderer (Chip8View) know when we have updated the graphics memory.
    bool _needsDisplay = false;



};

#endif // CHIP8EMU_H
